#include "../pass.h"

class InjectPass : public InstrumentationPass {
protected:
    InjectPass ();

    bool injectProbeBefore (llvm::Module & M,
                            llvm::Instruction & I) const;

    bool injectMetadataBefore (llvm::Module & M,
                               llvm::Instruction & I,
                               std::string key,
                               std::string value) const;

    bool injectLocationBefore (llvm::Module & M,
                               llvm::MDNode & N,
                               llvm::Instruction & I) const;

public:
    bool inject_without_location;
};

class BlockInjectPass : public InjectPass {
protected:
    virtual bool handleBasicBlock (llvm::Module & M,
                                   llvm::BasicBlock & B);
};

class FunctionInjectPass : public InjectPass {
protected:
    virtual bool handleFunction (llvm::Module & M,
                                 llvm::Function & F);
};

class MetadataInjectPass : public InjectPass {
protected:
    virtual bool handleBasicBlock (llvm::Module & M,
                                   llvm::BasicBlock & F);
};