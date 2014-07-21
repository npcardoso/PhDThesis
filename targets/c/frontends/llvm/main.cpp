#include "passes/inject.h"
#include "passes/prepare.h"
#include "passes/overrides.h"

#include <llvm/DebugInfo.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/ValueSymbolTable.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/PassManager.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>

using namespace llvm;

cl::opt<std::string> config_file("si_config", cl::desc("Specify spectra instrumentation configuration file"), cl::value_desc("filename"));

class SpectraInstrumentation : public ModulePass {
public:
    inline SpectraInstrumentation () : ModulePass(ID) {}

    virtual bool runOnModule (Module & M) {
        BlockInjectPass inject_pass;
        MetadataInjectPass metadata_inject_pass;
        PrepareInstrumentationPass prepare_pass;
        OverridePass override_pass;


        override_pass.function_overrides["pthread_create"] = "_instr_pthread_create";

        // inject_pass.inject_without_location = true;
        bool ret = false;
        ret |= inject_pass.runOnModule(M);
        ret |= metadata_inject_pass.runOnModule(M);
        ret |= override_pass.runOnModule(M);
        ret |= prepare_pass.runOnModule(M);
        return ret;
    }

public:
    static char ID; // Pass identification, replacement for typeid
};


char SpectraInstrumentation::ID = 1;

static RegisterPass<SpectraInstrumentation> spectra_instrument("spectra_instrument", "Instrumentation for spectra collection");

static void registerMyPass (const PassManagerBuilder &,
                            PassManagerBase & PM) {
    PM.add(new SpectraInstrumentation());
}

static RegisterStandardPasses
    RegisterMyPass(PassManagerBuilder::EP_EnabledOnOptLevel0,
                   registerMyPass);