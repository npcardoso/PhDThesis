#include "prepare.h"

#include <boost/foreach.hpp>


using namespace llvm;

PrepareInstrumentationPass::PrepareInstrumentationPass () {
    num_artifacts = 0;
}

bool PrepareInstrumentationPass::handleFunctionCall (Module & M,
                                                     Function & F,
                                                     CallInst & call) {
    if (F.getName() == INSTR_METADATA_REGISTER_FUN)
        return registerMetadata(M, num_artifacts, call);

    if (F.getName() == INSTR_PROBE_FUN ||
        F.getName() == INSTR_HIT_PROBE_FUN)
        return registerProbe(M, call);

    if (F.getName() == INSTR_TRANSACTION_START_FUN ||
        F.getName() == INSTR_TRANSACTION_END_FUN)
        return registerTransactionGate(M, call);

    if (F.getName() == INSTR_ORACLE_FUN)
        return registerOracle(M, call);

    return false;
}

bool PrepareInstrumentationPass::registerProbe (llvm::Module & M,
                                                llvm::CallInst & I) {
    return registerArtifact(M, ++num_artifacts, I, getRegisterProbeFunction(M));
}

bool PrepareInstrumentationPass::registerTransactionGate (llvm::Module & M,
                                                          llvm::CallInst & I) {
    return registerArtifact(M, ++num_artifacts, I, getRegisterTransactionFunction(M));
}

bool PrepareInstrumentationPass::registerOracle (llvm::Module & M,
                                                 llvm::CallInst & I) {
    return registerArtifact(M, ++num_artifacts, I, getRegisterOracleFunction(M));
}