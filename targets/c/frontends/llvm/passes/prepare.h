#ifndef __PREPARE_H_a020f2fceeebceb22c0ca3f031ae90a679339f87__
#define __PREPARE_H_a020f2fceeebceb22c0ca3f031ae90a679339f87__

#include "../pass.h"

#include <list>


class PrepareInstrumentationPass : public InstrumentationPass {
public:
    PrepareInstrumentationPass ();

protected:
    virtual bool handleFunctionCall (llvm::Module & M,
                                     llvm::Function & F,
                                     llvm::CallInst & call);

private:
    inline bool registerProbe (llvm::Module & M,
                               llvm::CallInst & I);
    inline bool registerTransactionGate (llvm::Module & M,
                                         llvm::CallInst & I);
    inline bool registerOracle (llvm::Module & M,
                                llvm::CallInst & I);


private:
    t_count num_artifacts;
};

#endif