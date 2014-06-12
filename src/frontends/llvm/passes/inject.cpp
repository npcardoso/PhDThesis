#include "inject.h"

#include "../types.h"

#include <llvm/IR/Constants.h>

using namespace llvm;

bool InjectPass::injectProbeBefore (Module & M, Instruction & I) {
    Function * observation_register =
        cast<Function> (M.getOrInsertFunction("_instr_hit_probe_observation",
                                              Type::getVoidTy(M.getContext()),
                                              Type::getIntNTy(M.getContext(), sizeof(t_construct_id) << 3),
                                              NULL));


    ConstantInt * zero = ConstantInt::get(M.getContext(), APInt(sizeof(t_construct_id) << 3, StringRef("0"), 10));
    CallInst::Create(observation_register, zero, "", &I);


    return true;
}

bool BlockInjectPass::handleBasicBlock (Module & M, BasicBlock & B) {
    BasicBlock::iterator Bi = B.getFirstInsertionPt();


    if (Bi != B.end())
        return injectProbeBefore(M, *Bi);

    return false;
}

bool FunctionInjectPass::handleFunction (Module & M, Function & F) {
    if (F.begin() != F.end()) {
        BasicBlock::iterator Bi = F.front().getFirstInsertionPt();

        if (Bi != F.front().end())
            return injectProbeBefore(M, *Bi);
    }

    return false;
}