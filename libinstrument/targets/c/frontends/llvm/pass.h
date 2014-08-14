#ifndef __PASS_H_5cdf6b8553e88a82dc1eeaec14603f46d20cc558__
#define __PASS_H_5cdf6b8553e88a82dc1eeaec14603f46d20cc558__

#include "types.h"
#include <iostream>

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>

#define INSTR_PROBE_FUN "_instr_probe_observation"
#define INSTR_HIT_PROBE_FUN "_instr_hit_probe_observation"
#define INSTR_TRANSACTION_START_FUN "_instr_transaction_start"
#define INSTR_TRANSACTION_END_FUN "_instr_transaction_end"
#define INSTR_ORACLE_FUN "_instr_oracle_observation"


#define INSTR_PROBE_REGISTER_FUN "_instr_probe_register"
#define INSTR_TRANSACTION_REGISTER_FUN "_instr_transaction_register"
#define INSTR_ORACLE_REGISTER_FUN "_instr_oracle_register"
#define INSTR_METADATA_REGISTER_FUN "_instr_metadata"

class InstrumentationPass {
protected:
    inline InstrumentationPass () {}

public:
    inline virtual ~InstrumentationPass () {}

    virtual bool runOnModule (llvm::Module & M);

protected:
    virtual bool handleFunction (llvm::Module & M,
                                 llvm::Function & F);
    virtual bool handleBasicBlock (llvm::Module & M,
                                   llvm::BasicBlock & B);
    virtual bool handleInstruction (llvm::Module & M,
                                    llvm::Instruction & I);
    virtual bool handleFunctionCall (llvm::Module & M,
                                     llvm::Function & F,
                                     llvm::CallInst & call);


    llvm::Type & getArtifactIDType (llvm::Module & M) const;

    llvm::Function & getHitProbeFunction (llvm::Module & M) const;

    llvm::Function & getRegisterProbeFunction (llvm::Module & M) const;
    llvm::Function & getRegisterTransactionFunction (llvm::Module & M) const;
    llvm::Function & getRegisterOracleFunction (llvm::Module & M) const;
    llvm::Function & getRegisterMetadataFunction (llvm::Module & M) const;
    llvm::Function & getRegisterAllFunction (llvm::Module & M) const;

    llvm::BasicBlock::iterator getRegisterAllIterator (llvm::Module & M) const;

    llvm::GlobalVariable & getOrRegisterGlobalConstant (llvm::Module & M,
                                                        std::string name,
                                                        std::string str) const;

    llvm::GlobalVariable & getOrRegisterIDHolder (llvm::Module & M,
                                                  t_artifact_id id,
                                                  t_artifact_id value) const;


    bool registerArtifact (llvm::Module & M,
                           t_artifact_id id,
                           llvm::CallInst & I,
                           llvm::Function & reg_fun) const;

    bool registerMetadata (llvm::Module & M,
                           t_artifact_id id,
                           std::string key,
                           std::string value) const;

    bool registerMetadata (llvm::Module & M,
                           t_artifact_id id,
                           llvm::CallInst & I) const;
};

#endif