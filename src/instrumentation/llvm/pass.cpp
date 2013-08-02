#include "pass.h"
#include "utils.h"

bool Instrument::Pass::runOnModule (Module & M) {
    bool changes = false;


    foreach(F, M) // Loop through all functions within a module
    changes |= handleFunction(M, *F);

    return changes;
}

bool Instrument::Pass::handleFunction (Module & M, Function & F) {
    bool changes = false;


    foreach(B, F) // Loop through all basic blocks within a function
    changes |= handleBasicBlock(M, *B);

    return changes;
}

bool Instrument::Pass::handleBasicBlock (Module & M, BasicBlock & B) {
    bool changes = false;


    foreach(I, B) // Loop through all instructions within basic block
    changes |= handleInstruction(M, *I);

    return changes;
}

bool Instrument::Pass::handleInstruction (Module & M, Instruction & I) {
    bool changes = false;


    if (CallInst * call = dyn_cast < CallInst > (&I))
        changes |= handleFunctionCall(M, *call);

    return changes;
}

bool Instrument::Pass::handleFunctionCall (Module & M, CallInst & call) {
    return false;
}