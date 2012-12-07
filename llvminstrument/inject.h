#include "pass.h"

namespace Instrument {
class InjectPass : public Pass {
protected:
  inline InjectPass(char ID) : Pass(ID) {}
  bool injectProbeBefore(Module & M, Instruction & I);
  bool injectTransactionStartBefore(Module & M, Instruction & I);
  bool injectTransactionEndBefore(Module & M, Instruction & I);
};
}
