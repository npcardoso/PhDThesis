#include "inject.h"

bool Instrument::InjectPass::injectProbeBefore(Module & M, Instruction & I){
  Function * observation_register =
    cast<Function>(M.getOrInsertFunction("_instr_probe_observation_register",
                                         Type::getVoidTy(M.getContext()),
                                         Type::getInt64Ty(M.getContext()),
                                         NULL));
  Function * observation_commit =
    cast<Function>(M.getOrInsertFunction("_instr_probe_observation_commit",
                                         Type::getVoidTy(M.getContext()),
                                         NULL));


  Instruction * commit = CallInst::Create(observation_commit, "", &I);
  ConstantInt* zero = ConstantInt::get(M.getContext(), APInt(64, StringRef("0"), 10));
  CallInst::Create(observation_register, zero, "", commit);
  return true;
}
