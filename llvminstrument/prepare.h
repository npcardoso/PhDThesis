#pragma once

#include "pass.h"

#include "libinterface.h"

#include <boost/lexical_cast.hpp>
#include <list>

namespace Instrument {

static const std::string lib_prefix = "_instr_";

class PrepareInstrumentionPass : public Pass {
  std::map<std::string, Value *> function_overrides;

  LibInterface * interface;

  typedef std::pair<std::string, CallInst &> metadata_call; 

  std::list <metadata_call> metadata;

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

  bool metadataSetup(Module & M,
                     CallInst & I,
                     std::string id_var_name);

  inline bool registerProbe(Module & M, CallInst & I) {
    return registerArtifact(M, I, 
                            "_instr_p_id_",
                            num_probes++,
                            &interface->getRegisterProbe(),
                            &interface->getRegisterProbeLocation());
  }

  inline bool registerProbeMetadata(Module & M, CallInst & I) {
    std::string id_var_name = std::string("_instr_p_id_") + 
      boost::lexical_cast<std::string>(num_probes - 1);
    
    metadata.push_back(metadata_call(id_var_name, I));
    return false;
  }
  
  inline bool registerTransactionGate(Module & M, CallInst & I) {
    return registerArtifact(M, I, 
                            "_instr_tg_id_",
                            num_transaction_gates++,
                            &interface->getRegisterTransactionGate(),
                            &interface->getRegisterTransactionGateLocation());
  }

  inline bool registerTransactionGateMetadata(Module & M, CallInst & I) {
    std::string id_var_name = std::string("_instr_tg_id_") + 
      boost::lexical_cast<std::string>(num_transaction_gates - 1);
    
    metadata.push_back(metadata_call(id_var_name, I));
    return false;
  }

  inline bool registerOracle(Module & M, CallInst & I) {
    return registerArtifact(M, I,
                            "_instr_o_id_",
                            num_oracles++,
                            &interface->getRegisterOracle(),
                            &interface->getRegisterOracleLocation());
  }

  inline bool registerOracleMetadata(Module & M, CallInst & I) {
    std::string id_var_name = std::string("_instr_o_id_") + 
      boost::lexical_cast<std::string>(num_oracles - 1);
    
    errs() << "Found metadata for " << id_var_name << "\n";
    metadata.push_back(metadata_call(id_var_name, I));
    return false;
  }
  
  virtual bool handleFunctionCall(Module & M, CallInst & call);
public:
  PrepareInstrumentionPass();
  static char ID; // Pass identification, replacement for typeid
  virtual bool runOnModule(Module & M);
};
}
