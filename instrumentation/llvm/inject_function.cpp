#include "inject_function.h"

char Instrument::FunctionInstrumentPass::ID = 4;

static RegisterPass<Instrument::FunctionInstrumentPass> function_instrument("instrument_function", "Injects function instrumentation instructions");

bool Instrument::FunctionInstrumentPass::handleFunction(Module & M, Function & F){
  if(F.begin() != F.end()){
    BasicBlock::iterator Bi = F.front().getFirstInsertionPt();
    if(Bi != F.front().end())
      return injectProbeBefore(M, *Bi);
  }
  return false;
}
