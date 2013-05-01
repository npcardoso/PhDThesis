#pragma once
#include <llvm/Function.h>
#include <llvm/Module.h>

#define declare_getRegister(fun_name, var_name) \
  private:\
  Function * register_ ## var_name;\
  void initRegister ## fun_name();\
  public:\
  inline Function & getRegister ## fun_name() {\
    if(!register_ ## var_name)\
    initRegister ## fun_name ();\
    return *(register_ ## var_name);\
  }

#define init_getRegister(var_name)\
  register_ ## var_name = NULL;

using namespace llvm;

class LibInterface {
  BasicBlock::iterator register_all_it;

  Module & M;
public:

  inline LibInterface(Module & M) : M(M){
    init_getRegister(all);

    init_getRegister(probe);
    init_getRegister(probe_name);
    init_getRegister(probe_location);

    init_getRegister(transaction_gate);
    init_getRegister(transaction_gate_location);
    init_getRegister(transaction_gate_name);

    init_getRegister(oracle);
    init_getRegister(oracle_location);
    init_getRegister(oracle_name);
  }

  /* Register All Related */
  declare_getRegister(All, all);

  inline BasicBlock::iterator & getRegisterAllIterator() {
    if(!register_all)
      initRegisterAll();
    return register_all_it;
  }

  /* Probe Related */
  declare_getRegister(Probe, probe);
  declare_getRegister(ProbeName, probe_name);
  declare_getRegister(ProbeLocation, probe_location);

  /* Transaction Gate Related */

  declare_getRegister(TransactionGate, transaction_gate);
  declare_getRegister(TransactionGateName, transaction_gate_name);
  declare_getRegister(TransactionGateLocation, transaction_gate_location);

  /* Transaction Oracle Related */

  declare_getRegister(Oracle, oracle);
  declare_getRegister(OracleName, oracle_name);
  declare_getRegister(OracleLocation, oracle_location);
};

