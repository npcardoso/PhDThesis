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

cl::opt<std::string> config_file("si_config", cl::desc("Specify spectrum instrumentation configuration file"), cl::value_desc("filename"));

class SpectrumInstrumentation : public ModulePass {
public:
    inline SpectrumInstrumentation () : ModulePass(ID) {}

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


char SpectrumInstrumentation::ID = 1;

static RegisterPass<SpectrumInstrumentation> spectrum_instrument("spectrum_instrument", "Instrumentation for spectrum collection");

static void registerMyPass (const PassManagerBuilder &,
                            PassManagerBase & PM) {
    PM.add(new SpectrumInstrumentation());
}

static RegisterStandardPasses
    RegisterMyPass(PassManagerBuilder::EP_EnabledOnOptLevel0,
                   registerMyPass);
