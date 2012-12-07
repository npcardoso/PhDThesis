#pragma once

#include "pass.h"

#include "libinterface.h"

namespace Instrument {

static const std::string lib_prefix = "_instr_";

class PrepareInstrumentionPass : public Pass {
  std::map<std::string, Value *> function_overrides;

  LibInterface * interface;

  size_t num_probes;
  size_t num_transaction_gates;
  size_t num_oracles;

  bool initFunctionOverrides(Module & M);

  bool registerArtifact(Module & M,
                        CallInst & I,
                        std::string var_prefix,
                        size_t id,
                        Function * reg_fun, Function * reg_location_fun);

  bool registerStrProp(Module & M,
                       GlobalVariable & probe_id_holder,
                       Function * registar,
                       std::string var_name,
                       std::string value);

  inline bool registerProbe(Module & M, CallInst & I) {
    return registerArtifact(M, I, "_instr_p_id_",
                            num_probes++,
                            &interface->getRegisterProbe(),
                            &interface->getRegisterProbeLocation());
  }

  inline bool registerTransactionGate(Module & M, CallInst & I) {
    return registerArtifact(M, I, "_instr_tg_id_",
                            num_transaction_gates++,
                            &interface->getRegisterTransactionGate(),
                            &interface->getRegisterTransactionGateLocation());
  }

  inline bool registerOracle(Module & M, CallInst & I) {
    return registerArtifact(M, I, "_instr_o_id_",
                            num_oracles++,
                            &interface->getRegisterOracle(),
                            &interface->getRegisterOracleLocation());
  }

  virtual bool handleFunctionCall(Module & M, CallInst & call);
public:
  PrepareInstrumentionPass();
  static char ID; // Pass identification, replacement for typeid
  virtual bool runOnModule(Module & M);
};
}
