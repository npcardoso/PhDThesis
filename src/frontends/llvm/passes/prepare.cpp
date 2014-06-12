#include "prepare.h"

#include "../types.h"

#include <boost/foreach.hpp>
#include <llvm/IR/Constants.h>
#include <llvm/IR/ValueSymbolTable.h>

using namespace llvm;

static const std::string lib_prefix = "_instr_";

PrepareInstrumentionPass::PrepareInstrumentionPass () {
    num_oracles = 0;
    num_probes = 0;
    num_transactions = 0;
}

bool PrepareInstrumentionPass::runOnModule (Module & M) {
    bool changes = false;


    interface = new LibInterface(M);

    changes = InstrumentationPass::runOnModule(M);

    BOOST_FOREACH(auto & item, metadata) {
        metadataSetup(M, *item.second, item.first);
    }
    metadata.clear();

    delete interface;
    return changes;
}

bool PrepareInstrumentionPass::handleFunctionCall (Module & M, CallInst & call) {
    if (Function * f = call.getCalledFunction()) {
        if (f->getName() == "_instr_metadata")
            return registerMetadata(M, call);

        if (f->getName() == "_instr_probe_observation" || f->getName() == "_instr_hit_probe_observation")
            return registerProbe(M, call);

        if (f->getName() == "_instr_transaction_start")
            return registerTransactionGate(M, call);

        if (f->getName() == "_instr_transaction_end")
            return registerTransactionGate(M, call);

        if (f->getName() == "_instr_oracle_observation")
            return registerOracle(M, call);
    }

    return false;
}

bool PrepareInstrumentionPass::metadataSetup (Module & M,
                                              CallInst & I,
                                              std::string id_var_name) {
    GlobalVariable * id_holder = M.getGlobalVariable(id_var_name, true);


    if (!id_holder)
        return false;

    I.removeFromParent();
    BasicBlock::iterator & it = interface->getRegisterAllIterator();
    I.insertBefore(it);
    I.setArgOperand(0, new LoadInst(id_holder, NULL, &I));
    return true;
}

bool PrepareInstrumentionPass::registerConstruct (Module & M,
                                                  CallInst & I,
                                                  std::string var_prefix,
                                                  size_t id,
                                                  Function * reg_fun,
                                                  Function * reg_location_fun) {
    char buf[1024];


    sprintf(buf, "%lu", id);

    std::string id_var_name = var_prefix + buf;


    Type * operand_type = Type::getIntNTy(M.getContext(), sizeof(t_construct_id) << 3);
    // I.getArgOperand(0)->getType();
    GlobalVariable * id_holder =
        new GlobalVariable(M,
                           operand_type, // Type
                           false, // Constant
                           GlobalValue::PrivateLinkage, // Linkage
                           ConstantInt::getSigned(operand_type, 0), // Default Value
                           id_var_name);

    // errs() << "Registering construct ";
    id_holder->dump();

    BasicBlock::iterator & it = interface->getRegisterAllIterator();

    /* Inject registration p_id */
    CallInst * register_instr = CallInst::Create(reg_fun, "", it);
    new StoreInst(register_instr, id_holder, false, it);

    /* Inject correct p_id */
    I.setArgOperand(0, new LoadInst(id_holder, NULL, &I));

    return true;
}

bool PrepareInstrumentionPass::registerStrProp (Module & M,
                                                GlobalVariable & id_holder,
                                                Function * registar,
                                                std::string var_name,
                                                std::string value) {
    Constant * data_array = ConstantDataArray::getString(M.getContext(), value, true);
    GlobalVariable * variable =
        new GlobalVariable(M,
                           ArrayType::get(Type::getInt8Ty(M.getContext()), value.size() + 1), // Type
                           true, // Constant
                           GlobalValue::PrivateLinkage, // Linkage
                           data_array, // Default Value
                           var_name);

    ConstantInt * zero = ConstantInt::get(M.getContext(), APInt(32, StringRef("0"), 10));
    std::vector<Constant *> indices;


    indices.push_back(zero);
    indices.push_back(zero);


    std::vector<Value *> args;
    BasicBlock::iterator & it = interface->getRegisterAllIterator();
    args.push_back(new LoadInst(&id_holder, NULL, it)); // ID
    args.push_back(ConstantExpr::getGetElementPtr(variable, indices)); //
    CallInst::Create(registar, args, "", it);

    return true;
}

bool PrepareInstrumentionPass::registerProbe (llvm::Module & M, llvm::CallInst & I) {
    num_probes++;
    return registerConstruct(M, I,
                             "_instr_id_",
                             num_transactions + num_probes + num_oracles,
                             &interface->getRegisterProbe(),
                             &interface->getRegisterProbeLocation());
}

bool PrepareInstrumentionPass::registerTransactionGate (llvm::Module & M, llvm::CallInst & I) {
    num_transactions++;
    return registerConstruct(M, I,
                             "_instr_id_",
                             num_transactions + num_probes + num_oracles,
                             &interface->getRegisterTransactionGate(),
                             &interface->getRegisterTransactionGateLocation());
}

bool PrepareInstrumentionPass::registerOracle (llvm::Module & M, llvm::CallInst & I) {
    num_oracles++;
    return registerConstruct(M, I,
                             "_instr_id_",
                             num_transactions + num_probes + num_oracles,
                             &interface->getRegisterOracle(),
                             &interface->getRegisterOracleLocation());
}

bool PrepareInstrumentionPass::registerMetadata (llvm::Module & M, llvm::CallInst & I) {
    char buf[1024];


    sprintf(buf, "%lu", num_oracles + num_transactions + num_probes);
    std::string id_var_name = std::string("_instr_id_") + buf;

    // errs() << "Found metadata for " << id_var_name << "\n";
    metadata.push_back(metadata_call(id_var_name, &I));
    return false;
}