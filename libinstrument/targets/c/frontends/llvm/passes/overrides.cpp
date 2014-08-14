#include "overrides.h"

using namespace llvm;


Value * OverridePass::getOverride (Module & M,
                                   Function & F) const {
    auto it = function_overrides.find(F.getName());


    if (it == function_overrides.end())
        return NULL;

    return M.getOrInsertFunction(it->second, F.getFunctionType());
}

bool OverridePass::handleFunctionCall (Module & M,
                                       Function & F,
                                       CallInst & call) {
    if (Value * override = getOverride(M, F)) {
        call.setCalledFunction(override);
        return true;
    }

    return false;
}