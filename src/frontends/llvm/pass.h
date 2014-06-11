#pragma once


#include <llvm/IR/Instructions.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>



class InstrumentationPass {
protected:
    virtual bool handleFunction (llvm::Module & M, llvm::Function & F);
    virtual bool handleBasicBlock (llvm::Module & M, llvm::BasicBlock & B);
    virtual bool handleInstruction (llvm::Module & M, llvm::Instruction & I);
    virtual bool handleFunctionCall (llvm::Module & M, llvm::CallInst & call);

public:
    inline virtual ~InstrumentationPass () {}

    virtual bool runOnModule (llvm::Module & M);
};
