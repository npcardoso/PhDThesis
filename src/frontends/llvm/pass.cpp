#include "pass.h"
#include "types.h"
#include <boost/foreach.hpp>

using namespace llvm;

bool InstrumentationPass::runOnModule (llvm::Module & M) {
    bool changes = false;


    BOOST_FOREACH(Function & F, M) { // Loop through all functions within a module
        changes |= handleFunction(M, F);
    }

    return changes;
}

bool InstrumentationPass::handleFunction (llvm::Module & M, llvm::Function & F) {
    bool changes = false;


    BOOST_FOREACH(auto & B, F) { // Loop through all basic blocks within a function
        changes |= handleBasicBlock(M, B);
    }

    return changes;
}

bool InstrumentationPass::handleBasicBlock (llvm::Module & M, llvm::BasicBlock & B) {
    bool changes = false;


    BOOST_FOREACH(auto & I, B) { // Loop through all instructions within basic block
        changes |= handleInstruction(M, I);
    }
    return changes;
}

bool InstrumentationPass::handleInstruction (llvm::Module & M, llvm::Instruction & I) {
    bool changes = false;


    if (CallInst * call = dyn_cast < CallInst > (&I))
        changes |= handleFunctionCall(M, *call);

    return changes;
}

bool InstrumentationPass::handleFunctionCall (llvm::Module & M, llvm::CallInst & call) {
    return false;
}
