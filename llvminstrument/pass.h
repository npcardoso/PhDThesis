#pragma once

#include <llvm/Analysis/DebugInfo.h>
#include <llvm/Constants.h>
#include <llvm/Instructions.h>
#include <llvm/Function.h>
#include <llvm/Module.h>
#include <llvm/Pass.h>
#include <llvm/ValueSymbolTable.h>
#include <llvm/Type.h>
#include <llvm/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/Statistic.h>

#include <map>

#include "utils.h"

using namespace llvm;


namespace Instrument {

class Pass : public ModulePass {
protected:
  virtual bool handleFunction(Module & M, Function & F);
  virtual bool handleBasicBlock(Module & M, BasicBlock & B);
  virtual bool handleInstruction(Module & M, Instruction & I);
  virtual bool handleFunctionCall(Module & M, CallInst & call);

  inline Pass(char ID) : ModulePass(ID) {}
public:
  ~Pass();

  virtual bool runOnModule(Module & M);
};
}
