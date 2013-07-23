#pragma once

#include <llvm/DebugInfo.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/ValueSymbolTable.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Pass.h>
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
