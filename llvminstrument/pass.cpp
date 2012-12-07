#include "pass.h"
#include "utils.h"

bool Instrument::Pass::runOnModule(Module & M){
  bool changes = false;

  foreach (F, M) { // Loop through all functions within a module
    changes |= handleFunction(M, *F);
    foreach (B, *F) { // Loop through all basic blocks within a function
      changes |= handleBasicBlock(M, *B);
      foreach (I, *B) {  // Loop through all instructions within basic block
        changes |= handleInstruction(M, *I);
        if(CallInst * call = dyn_cast<CallInst>(I))
          changes |= handleFunctionCall(M, *call);
      }
    }
  }

  return changes;
}

bool Instrument::Pass::handleFunction(Module & M, Function & F){
  return false;
}

bool Instrument::Pass::handleBasicBlock(Module & M, BasicBlock & B){
  return false;
}

bool Instrument::Pass::handleInstruction(Module & M, Instruction & I){
  return false;
}

bool Instrument::Pass::handleFunctionCall(Module & M, CallInst & call){
  return false;
}
