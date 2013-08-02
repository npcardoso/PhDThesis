#include "inject.h"

namespace Instrument {
class BlockInstrumentPass : public InjectPass {
protected:
    virtual bool handleBasicBlock (Module & M, BasicBlock & B);
public:
    static char ID;
    inline BlockInstrumentPass () : InjectPass(ID) {}
};
}