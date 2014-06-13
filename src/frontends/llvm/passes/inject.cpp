#include "inject.h"

#include "../types.h"

#include <llvm/IR/Constants.h>
#include <llvm/DebugInfo.h>

using namespace llvm;
InjectPass::InjectPass () {
    inject_without_location = false;
}

bool InjectPass::injectProbeBefore (Module & M, Instruction & I) const {
    MDNode * N = I.getMetadata("dbg");


    if (!inject_without_location && N == NULL)
        return false;

    Function & hit_function = getHitProbeFunction(M);
    ConstantInt * zero = ConstantInt::get(M.getContext(), APInt(sizeof(t_artifact_id) << 3, StringRef("0"), 10));
    CallInst::Create(&hit_function, zero, "", &I);

    if (N) {
        DILocation Loc(N);
        unsigned Line = Loc.getLineNumber();
        StringRef File = Loc.getFilename();
        StringRef Dir = Loc.getDirectory();
        std::string location_str = (Dir + "/" + File + ":" + std::to_string(Line)).str();
        injectMetadataBefore(M, I, "location", location_str);
    }


    return true;
}

bool InjectPass::injectMetadataBefore (Module & M,
                                       Instruction & I,
                                       std::string key,
                                       std::string value) const {
    GlobalVariable & key_var =
        getOrRegisterGlobalConstant(M, "", key);
    GlobalVariable & val_var =
        getOrRegisterGlobalConstant(M, "", value);


    ConstantInt * zero = ConstantInt::get(M.getContext(), APInt(64, StringRef("0"), 10));
    std::vector<Constant *> indices;


    indices.push_back(zero);
    indices.push_back(zero);

    std::vector<Value *> args;
    args.push_back(zero); // ID
    args.push_back(ConstantExpr::getGetElementPtr(&key_var, indices));
    args.push_back(ConstantExpr::getGetElementPtr(&val_var, indices));
    CallInst::Create(&getRegisterMetadataFunction(M), args, "", &I);
    return true;
}

bool BlockInjectPass::handleBasicBlock (Module & M, BasicBlock & B) {
    BasicBlock::iterator Bi = B.getFirstInsertionPt();


    if (Bi != B.end())
        return injectProbeBefore(M, *Bi);

    return false;
}

bool FunctionInjectPass::handleFunction (Module & M, Function & F) {
    if (F.begin() != F.end()) {
        BasicBlock::iterator Bi = F.front().getFirstInsertionPt();

        if (Bi != F.front().end())
            return injectProbeBefore(M, *Bi);
    }

    return false;
}