#include "pass.h"

class InjectPass : public InstrumentationPass {
protected:
    bool injectProbeBefore (llvm::Module & M, llvm::Instruction & I);
};

class BlockInjectPass : public InjectPass {
    protected:
    virtual bool handleBasicBlock (llvm::Module & M, llvm::BasicBlock & B);
};

class FunctionInjectPass : public InjectPass {
    protected:
    virtual bool handleFunction (llvm::Module & M, llvm::Function & F);
};
