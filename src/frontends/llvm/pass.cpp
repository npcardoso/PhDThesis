#include "pass.h"
#include <boost/foreach.hpp>


#define HOLDER_PREFIX "_instr_id_"
#define METADATA_KEY_PREFIX "_instr_mkey_"
#define METADATA_VAL_PREFIX "_instr_mval_"

using namespace llvm;

bool InstrumentationPass::runOnModule (Module & M) {
    bool changes = false;
    auto it = M.begin();
    Function & register_all = getRegisterAllFunction(M);


    while (it != M.end()) // Loop through all functios within a module
        // This way, handleFunction can delete stuff without breaking the loop
        if (it->getName() != register_all.getName())
            changes |= handleFunction(M, *(it++));
        else
            it++;

    return changes;
}

bool InstrumentationPass::handleFunction (Module & M, Function & F) {
    bool changes = false;
    auto it = F.begin();


    while (it != F.end()) // Loop through all basic blocks within a function
        // This way, handleBasicBlock can delete stuff without breaking the loop
        changes |= handleBasicBlock(M, *(it++));

    return changes;
}

bool InstrumentationPass::handleBasicBlock (Module & M, BasicBlock & B) {
    bool changes = false;
    auto it = B.begin();


    while (it != B.end()) // Loop through all instructions within a basic block
        // This way, handleInstruction can delete stuff without breaking the loop
        changes |= handleInstruction(M, *(it++));

    return changes;
}

bool InstrumentationPass::handleInstruction (Module & M, Instruction & I) {
    bool changes = false;


    if (CallInst * call = dyn_cast<CallInst> (&I))
        changes |= handleFunctionCall(M, *call);

    return changes;
}

bool InstrumentationPass::handleFunctionCall (Module & M, CallInst & call) {
    return false;
}

Type & InstrumentationPass::getArtifactIDType (Module & M) const {
    Type * tmp = Type::getIntNTy(M.getContext(), sizeof(t_artifact_id) << 3);


    assert(tmp != NULL);
    return *tmp;
}

Function & InstrumentationPass::getHitProbeFunction (Module & M) const {
    Function * tmp = cast<Function> (M.getOrInsertFunction("_instr_hit_probe_observation",
                                                           Type::getVoidTy(M.getContext()),
                                                           &getArtifactIDType(M),
                                                           NULL));


    assert(tmp != NULL);
    return *tmp;
}

Function & InstrumentationPass::getRegisterProbeFunction (Module & M) const {
    Function * tmp = cast<Function> (M.getOrInsertFunction("_instr_probe_register",
                                                           &getArtifactIDType(M),
                                                           NULL));


    assert(tmp != NULL);
    return *tmp;
}

Function & InstrumentationPass::getRegisterTransactionFunction (Module & M) const {
    Function * tmp = cast<Function> (M.getOrInsertFunction("_instr_transaction_register",
                                                           &getArtifactIDType(M),
                                                           NULL));


    assert(tmp != NULL);
    return *tmp;
}

Function & InstrumentationPass::getRegisterOracleFunction (Module & M) const {
    Function * tmp = cast<Function> (M.getOrInsertFunction("_instr_oracle_register",
                                                           &getArtifactIDType(M),
                                                           NULL));


    assert(tmp != NULL);
    return *tmp;
}

Function & InstrumentationPass::getRegisterMetadataFunction (Module & M) const {
    Function * tmp = cast<Function> (M.getOrInsertFunction("_instr_metadata",
                                                           Type::getVoidTy(M.getContext()),
                                                           &getArtifactIDType(M),
                                                           Type::getIntNPtrTy(M.getContext(), sizeof(char *)),
                                                           Type::getIntNPtrTy(M.getContext(), sizeof(char *)),
                                                           NULL));


    assert(tmp != NULL);
    return *tmp;
}

#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>


static void appendToGlobalArray (const char * Array,
                                 Module & M, Function * F, int Priority) {
    IRBuilder<> IRB(M.getContext());
    FunctionType * FnTy = FunctionType::get(IRB.getVoidTy(), false);
    StructType * Ty =
        StructType::get(IRB.getInt32Ty(),
                        PointerType::getUnqual(FnTy),
                        NULL);

    Constant * RuntimeCtorInit =
        ConstantStruct::get(Ty,
                            IRB.getInt32(Priority),
                            F,
                            NULL);

    // Get the current set of static global constructors and add the new ctor
    // to the list.
    SmallVector<Constant *, 16> CurrentCtors;

    if (GlobalVariable * GVCtor = M.getNamedGlobal(Array)) {
        if (Constant * Init = GVCtor->getInitializer()) {
            unsigned n = Init->getNumOperands();
            CurrentCtors.reserve(n + 1);

            for (unsigned i = 0; i != n; ++i)
                CurrentCtors.push_back(cast<Constant> (Init->getOperand(i)));
        }

        GVCtor->eraseFromParent();
    }

    CurrentCtors.push_back(RuntimeCtorInit);

    // Create a new initializer.
    ArrayType * AT = ArrayType::get(RuntimeCtorInit->getType(),
                                    CurrentCtors.size());
    Constant * NewInit = ConstantArray::get(AT, CurrentCtors);

    // Create the new global variable and replace all uses of
    // the old global variable with the new one.
    (void) new GlobalVariable(M, NewInit->getType(), false,
                              GlobalValue::AppendingLinkage, NewInit, Array);
}

Function & InstrumentationPass::getRegisterAllFunction (Module & M) const {
    Function * register_all = cast<Function> (M.getFunction("_instr_register_all_" + M.getModuleIdentifier()));


    if (register_all == NULL) {
        register_all = cast<Function> (M.getOrInsertFunction("_instr_register_all_" + M.getModuleIdentifier(),
                                                             Type::getVoidTy(M.getContext()),
                                                             NULL));

        BasicBlock * entry = BasicBlock::Create(M.getContext(), "entry", register_all);

        IRBuilder<true> builder(entry);
        builder.CreateRetVoid();

        appendToGlobalArray("llvm.global_ctors", M, register_all, 0);
    }

    assert(register_all != NULL);

    return *register_all;
}

BasicBlock::iterator InstrumentationPass::getRegisterAllIterator (Module & M) const {
    Function & register_all = getRegisterAllFunction(M);
    BasicBlock::iterator it = register_all.front().end();


    it--; // insert before return instruction
    return it;
}

GlobalVariable & InstrumentationPass::getOrRegisterGlobalConstant (Module & M,
                                                                   std::string name,
                                                                   std::string str) const {
    GlobalVariable * tmp = M.getGlobalVariable(name, true);


    if (tmp)
        return *tmp;

    Constant * data_array = ConstantDataArray::getString(M.getContext(), str, true);
    assert(data_array != NULL);

    tmp = new GlobalVariable(M,
                             ArrayType::get(Type::getInt8Ty(M.getContext()), str.size() + 1), // Type
                             true, // Constant
                             GlobalValue::PrivateLinkage, // Linkage
                             data_array, // Default Value
                             name);
    assert(tmp != NULL);
    return *tmp;
}

GlobalVariable & InstrumentationPass::getOrRegisterIDHolder (Module & M,
                                                             t_artifact_id id,
                                                             t_artifact_id value) const {
    std::string name = HOLDER_PREFIX + std::to_string(id);
    GlobalVariable * tmp = M.getGlobalVariable(name, true);


    if (tmp)
        return *tmp;

    Type * operand_type = &getArtifactIDType(M);
    tmp = new GlobalVariable(M,
                             operand_type, // Type
                             false, // Constant
                             GlobalValue::PrivateLinkage, // Linkage
                             ConstantInt::getSigned(operand_type, value), // Default Value
                             name);
    assert(tmp != NULL);
    return *tmp;
}

bool InstrumentationPass::registerArtifact (Module & M,
                                            t_artifact_id id,
                                            CallInst & I,
                                            Function & reg_fun) const {
    GlobalVariable & id_holder = getOrRegisterIDHolder(M, id, 0);
    BasicBlock::iterator it = getRegisterAllIterator(M);

    /* Inject registration id */
    CallInst * register_instr = CallInst::Create(&reg_fun, "", it);


    new StoreInst(register_instr, &id_holder, false, it);

    /* Inject correct id */
    I.setArgOperand(0, new LoadInst(&id_holder, NULL, &I));

    return true;
}

bool InstrumentationPass::registerMetadata (llvm::Module & M,
                                            t_artifact_id id,
                                            llvm::CallInst & I) const {
    GlobalVariable & id_holder = getOrRegisterIDHolder(M, id, 0);


    I.removeFromParent();

    BasicBlock::iterator it = getRegisterAllIterator(M);
    I.insertBefore(it);
    I.setArgOperand(0, new LoadInst(&id_holder, NULL, &I));
    return true;
}

bool InstrumentationPass::registerMetadata (Module & M,
                                            t_metadata_id id,
                                            std::string key,
                                            std::string value) const {
    GlobalVariable & key_var =
        getOrRegisterGlobalConstant(M, METADATA_KEY_PREFIX + std::to_string(id), key);
    GlobalVariable & val_var =
        getOrRegisterGlobalConstant(M, METADATA_VAL_PREFIX + std::to_string(id), value);


    GlobalVariable & id_holder = getOrRegisterIDHolder(M, id, 0);

    ConstantInt * zero = ConstantInt::get(M.getContext(), APInt(32, StringRef("0"), 10));
    std::vector<Constant *> indices;


    indices.push_back(zero);
    indices.push_back(zero);


    std::vector<Value *> args;
    BasicBlock::iterator it = getRegisterAllIterator(M);
    args.push_back(new LoadInst(&id_holder, NULL, it)); // ID
    args.push_back(ConstantExpr::getGetElementPtr(&key_var, indices)); //
    args.push_back(ConstantExpr::getGetElementPtr(&val_var, indices)); //
    CallInst::Create(&getRegisterMetadataFunction(M), args, "", it);
    return true;
}