#include "inject.h"

namespace Instrument {
class FunctionInstrumentPass : public InjectPass {
protected:
  virtual bool handleFunction(Module & M, Function & F);
public:
  static char ID;
  inline FunctionInstrumentPass() : InjectPass(ID){}
};
}

