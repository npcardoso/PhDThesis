#include "prepare.h"

#include <boost/foreach.hpp>
#include <llvm/IR/Constants.h>
#include <llvm/IR/ValueSymbolTable.h>


using namespace llvm;

PrepareInstrumentationPass::PrepareInstrumentationPass () {
    num_artifacts = 0;
}

bool PrepareInstrumentationPass::handleFunctionCall (Module & M, CallInst & call) {
    if (Function * f = call.getCalledFunction()) {
        if (f->getName() == "_instr_metadata")
            return registerMetadata(M, num_artifacts, call);

        if (f->getName() == "_instr_probe_observation" || f->getName() == "_instr_hit_probe_observation")
            return registerProbe(M, call);

        if (f->getName() == "_instr_transaction_start" ||
            f->getName() == "_instr_transaction_end")
            return registerTransactionGate(M, call);

        if (f->getName() == "_instr_oracle_observation")
            return registerOracle(M, call);
    }

    return false;
}

bool PrepareInstrumentationPass::registerProbe (llvm::Module & M, llvm::CallInst & I) {
    return registerArtifact(M, ++num_artifacts, I, getRegisterProbeFunction(M));
}

bool PrepareInstrumentationPass::registerTransactionGate (llvm::Module & M, llvm::CallInst & I) {
    return registerArtifact(M, ++num_artifacts, I, getRegisterTransactionFunction(M));
}

bool PrepareInstrumentationPass::registerOracle (llvm::Module & M, llvm::CallInst & I) {
    return registerArtifact(M, ++num_artifacts, I, getRegisterOracleFunction(M));
}