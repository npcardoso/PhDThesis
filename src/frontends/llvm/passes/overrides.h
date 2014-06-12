#ifndef __OVERRIDES_H_d8736190fd53b1d90d5de9505d6f7459a459a40e__
#define __OVERRIDES_H_d8736190fd53b1d90d5de9505d6f7459a459a40e__

#include "../pass.h"

#include <map>


class OverridePass : public InstrumentationPass {
private:
    llvm::Value * getOverride (llvm::Module & M,
                               llvm::Function & F) const;

    virtual bool handleFunctionCall (llvm::Module & M,
                                     llvm::CallInst & call);

public:
    std::map<std::string, std::string> function_overrides;
};

#endif