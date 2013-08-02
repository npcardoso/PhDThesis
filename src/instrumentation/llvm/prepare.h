#pragma once

#include "pass.h"

#include "libinterface.h"

#include <list>
#include <cstdio>

namespace Instrument {
static const std::string lib_prefix = "_instr_";

class PrepareInstrumentionPass : public Pass {
    std::map < std::string, Value* >function_overrides;

    LibInterface * interface;

    typedef std::pair < std::string, CallInst* >metadata_call;

    std::list < metadata_call >metadata;

    size_t num_probes;
    size_t num_transactions;
    size_t num_oracles;

    bool initFunctionOverrides (Module & M);

    bool registerConstruct (Module & M,
                            CallInst & I,
                            std::string var_prefix,
                            size_t id,
                            Function * reg_fun, Function * reg_location_fun);

    bool registerStrProp (Module & M,
                          GlobalVariable & probe_id_holder,
                          Function * registar,
                          std::string var_name,
                          std::string value);

    bool metadataSetup (Module & M,
                        CallInst & I,
                        std::string id_var_name);

    inline bool registerProbe (Module & M, CallInst & I) {
        num_probes++;
        return registerConstruct(M, I,
                                 "_instr_id_",
                                 num_transactions + num_probes + num_oracles,
                                 &interface->getRegisterProbe(),
                                 &interface->getRegisterProbeLocation());
    }

    inline bool registerTransactionGate (Module & M, CallInst & I) {
        num_transactions++;
        return registerConstruct(M, I,
                                 "_instr_id_",
                                 num_transactions + num_probes + num_oracles,
                                 &interface->getRegisterTransactionGate(),
                                 &interface->getRegisterTransactionGateLocation());
    }

    inline bool registerOracle (Module & M, CallInst & I) {
        num_oracles++;
        return registerConstruct(M, I,
                                 "_instr_id_",
                                 num_transactions + num_probes + num_oracles,
                                 &interface->getRegisterOracle(),
                                 &interface->getRegisterOracleLocation());
    }

    inline bool registerMetadata (Module & M, CallInst & I) {
        char buf[1024];


        sprintf(buf, "%lu", num_oracles + num_transactions + num_probes);
        std::string id_var_name = std::string("_instr_id_") + buf;

        errs() << "Found metadata for " << id_var_name << "\n";
        metadata.push_back(metadata_call(id_var_name, &I));
        return false;
    }

    virtual bool handleFunctionCall (Module & M, CallInst & call);
public:
    PrepareInstrumentionPass ();
    static char ID; // Pass identification, replacement for typeid
    virtual bool runOnModule (Module & M);
};
}