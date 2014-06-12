#ifndef __PASS_H_5cdf6b8553e88a82dc1eeaec14603f46d20cc558__
#define __PASS_H_5cdf6b8553e88a82dc1eeaec14603f46d20cc558__


#include <llvm/IR/Instructions.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>


class InstrumentationPass {
protected:
    inline InstrumentationPass () {}

public:
    inline virtual ~InstrumentationPass () {}

    virtual bool runOnModule (llvm::Module & M);

protected:
    virtual bool handleFunction (llvm::Module & M, llvm::Function & F);
    virtual bool handleBasicBlock (llvm::Module & M, llvm::BasicBlock & B);
    virtual bool handleInstruction (llvm::Module & M, llvm::Instruction & I);
    virtual bool handleFunctionCall (llvm::Module & M, llvm::CallInst & call);
};

#endif