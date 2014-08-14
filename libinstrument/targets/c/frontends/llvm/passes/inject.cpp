#include "inject.h"

#include "../types.h"

#include <boost/foreach.hpp>
#include <llvm/IR/Constants.h>
#include <llvm/DebugInfo.h>

using namespace llvm;
InjectPass::InjectPass () {
    inject_without_location = false;
}

bool InjectPass::injectProbeBefore (Module & M, Instruction & I) const {
    MDNode * N = I.getMetadata("dbg");


    if (!inject_without_location && N == NULL)
        return false;

    Function & hit_function = getHitProbeFunction(M);
    ConstantInt * zero = ConstantInt::get(M.getContext(), APInt(sizeof(t_artifact_id) << 3, StringRef("0"), 10));
    CallInst::Create(&hit_function, zero, "", &I);
    injectLocationBefore(M, *N, I);

    return true;
}

bool InjectPass::injectMetadataBefore (Module & M,
                                       Instruction & I,
                                       std::string key,
                                       std::string value) const {
    GlobalVariable & key_var =
        getOrRegisterGlobalConstant(M, "", key);
    GlobalVariable & val_var =
        getOrRegisterGlobalConstant(M, "", value);


    ConstantInt * zero = ConstantInt::get(M.getContext(), APInt(64, StringRef("0"), 10));
    std::vector<Constant *> indices;


    indices.push_back(zero);
    indices.push_back(zero);

    std::vector<Value *> args;
    args.push_back(zero); // ID
    args.push_back(ConstantExpr::getGetElementPtr(&key_var, indices));
    args.push_back(ConstantExpr::getGetElementPtr(&val_var, indices));
    CallInst::Create(&getRegisterMetadataFunction(M), args, "", &I);
    return true;
}

bool InjectPass::injectLocationBefore (Module & M,
                                       MDNode & N,
                                       Instruction & I) const {
    DILocation Loc(&N);
    auto prefix = (Loc.getFilename()[0] != '/') ? (Loc.getDirectory() + "/") : "";

    std::string location_str = (prefix +
                                Loc.getFilename() + ":" +
                                std::to_string(Loc.getLineNumber()) + ":" +
                                std::to_string(Loc.getColumnNumber())).str();


    return injectMetadataBefore(M, I, "location", location_str);
}

bool BlockInjectPass::handleBasicBlock (Module & M,
                                        BasicBlock & B) {
    BOOST_FOREACH(auto & I, B) {
        if (!isa<LandingPadInst> (&I) &&
            injectProbeBefore(M, I))
            return true;
    }
    return false;
}

bool FunctionInjectPass::handleFunction (Module & M,
                                         Function & F) {
    BOOST_FOREACH(auto & B, F) {
        BOOST_FOREACH(auto & I, B) {
            if (injectProbeBefore(M, I)) {
                injectMetadataBefore(M, I, "function", F.getName());
                return true;
            }
        }
    }
    return false;
}

bool MetadataInjectPass::handleBasicBlock (Module & M, BasicBlock & B) {
    bool changes = false;
    auto it = B.begin();


    while (it != B.end()) {
        Instruction & I = *(it++);
        MDNode * N = I.getMetadata("dbg");

        if (!N)
            continue;

        CallInst * call = dyn_cast<CallInst> (&I);

        if (!call)
            continue;

        Function * F = call->getCalledFunction();

        if (!F)
            continue;

        if (F->getName() != INSTR_PROBE_FUN &&
            F->getName() != INSTR_HIT_PROBE_FUN &&
            F->getName() != INSTR_TRANSACTION_START_FUN &&
            F->getName() != INSTR_TRANSACTION_END_FUN &&
            F->getName() != INSTR_ORACLE_FUN)
            continue;

        changes |= injectLocationBefore(M, *N, *it);
    }

    return changes;
}