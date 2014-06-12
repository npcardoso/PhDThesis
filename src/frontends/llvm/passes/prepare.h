#ifndef __PREPARE_H_a020f2fceeebceb22c0ca3f031ae90a679339f87__
#define __PREPARE_H_a020f2fceeebceb22c0ca3f031ae90a679339f87__


#include "../libinterface.h"
#include "../pass.h"

#include <list>


class PrepareInstrumentionPass : public InstrumentationPass {
public:
    PrepareInstrumentionPass ();
    virtual bool runOnModule (llvm::Module & M);


private:
    bool registerConstruct (llvm::Module & M,
                            llvm::CallInst & I,
                            std::string var_prefix,
                            size_t id,
                            llvm::Function * reg_fun,
                            llvm::Function * reg_location_fun);

    bool registerStrProp (llvm::Module & M,
                          llvm::GlobalVariable & probe_id_holder,
                          llvm::Function * registar,
                          std::string var_name,
                          std::string value);

    bool metadataSetup (llvm::Module & M,
                        llvm::CallInst & I,
                        std::string id_var_name);

    inline bool registerProbe (llvm::Module & M, llvm::CallInst & I);
    inline bool registerTransactionGate (llvm::Module & M, llvm::CallInst & I);
    inline bool registerOracle (llvm::Module & M, llvm::CallInst & I);
    inline bool registerMetadata (llvm::Module & M, llvm::CallInst & I);

    virtual bool handleFunctionCall (llvm::Module & M, llvm::CallInst & call);

private:
    LibInterface * interface;

    typedef std::pair<std::string, llvm::CallInst *> metadata_call;

    std::list<metadata_call> metadata;

    size_t num_probes;
    size_t num_transactions;
    size_t num_oracles;
};

#endif