#include "inject.h"
#include "types.h"

bool Instrument::InjectPass::injectProbeBefore (Module & M, Instruction & I) {
    Function * observation_register =
        cast < Function > (M.getOrInsertFunction("_instr_hit_probe_observation",
                                                 Type::getVoidTy(M.getContext()),
                                                 Type::getIntNTy(M.getContext(), sizeof(t_construct_id) << 3),
                                                 NULL));


    ConstantInt * zero = ConstantInt::get(M.getContext(), APInt(sizeof(t_construct_id) << 3, StringRef("0"), 10));
    CallInst::Create(observation_register, zero, "", &I);


    return true;
}