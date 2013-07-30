#include "inject_block.h"

char Instrument::BlockInstrumentPass::ID = 3;

static RegisterPass < Instrument::BlockInstrumentPass >block_instrument("instrument_block", "Injects block instrumentation instructions");

bool Instrument::BlockInstrumentPass::handleBasicBlock (Module & M, BasicBlock & B) {
    BasicBlock::iterator Bi = B.getFirstInsertionPt();


    if (Bi != B.end())
        return injectProbeBefore(M, *Bi);

    return false;
}