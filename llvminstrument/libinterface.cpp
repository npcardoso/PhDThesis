#include "libinterface.h"
#include <llvm/Constants.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/Type.h>

using namespace llvm;

static void appendToGlobalArray(const char *Array,
                                Module &M, Function *F, int Priority) {
  IRBuilder<> IRB(M.getContext());
  FunctionType *FnTy = FunctionType::get(IRB.getVoidTy(), false);
  StructType *Ty =
    StructType::get(IRB.getInt32Ty(),
                    PointerType::getUnqual(FnTy),
                    NULL);

  Constant *RuntimeCtorInit =
    ConstantStruct::get(Ty,
                        IRB.getInt32(Priority),
                        F,
                        NULL);

  // Get the current set of static global constructors and add the new ctor
  // to the list.
  SmallVector<Constant *, 16> CurrentCtors;
  if (GlobalVariable * GVCtor = M.getNamedGlobal(Array)) {
    if (Constant *Init = GVCtor->getInitializer()) {
      unsigned n = Init->getNumOperands();
      CurrentCtors.reserve(n + 1);
      for (unsigned i = 0; i != n; ++i)
        CurrentCtors.push_back(cast<Constant>(Init->getOperand(i)));
    }
    GVCtor->eraseFromParent();
  }

  CurrentCtors.push_back(RuntimeCtorInit);

  // Create a new initializer.
  ArrayType *AT = ArrayType::get(RuntimeCtorInit->getType(),
                                 CurrentCtors.size());
  Constant *NewInit = ConstantArray::get(AT, CurrentCtors);

  // Create the new global variable and replace all uses of
  // the old global variable with the new one.
  (void)new GlobalVariable(M, NewInit->getType(), false,
                           GlobalValue::AppendingLinkage, NewInit, Array);
}

void LibInterface::initRegisterAll() {
  register_all = cast<Function>(M.getOrInsertFunction("_instr_register_all_" + M.getModuleIdentifier(),
                                                      Type::getVoidTy(M.getContext()),
                                                      NULL));

  appendToGlobalArray("llvm.global_ctors", M, register_all, 0);

  BasicBlock *entry = BasicBlock::Create(M.getContext(), "entry", register_all);

  IRBuilder<true> builder(entry);
  builder.CreateRetVoid();
  register_all_it = register_all->front().begin();
}

void LibInterface::initRegisterProbe() {
  register_probe =
    cast<Function>(M.getOrInsertFunction("_instr_probe_register",
                                         Type::getInt64Ty(M.getContext()),
                                         NULL));
}

void LibInterface::initRegisterProbeLocation() {
  register_probe_location =
    cast<Function>(M.getOrInsertFunction("_instr_probe_register_location",
                                         Type::getVoidTy(M.getContext()),
                                         Type::getInt64Ty(M.getContext()),
                                         Type::getInt8PtrTy(M.getContext()),
                                         NULL));
}

void LibInterface::initRegisterProbeName() {
  register_probe_name =
    cast<Function>(M.getOrInsertFunction("_instr_probe_register_name",
                                         Type::getVoidTy(M.getContext()),
                                         Type::getInt64Ty(M.getContext()),
                                         Type::getInt8PtrTy(M.getContext()),
                                         NULL));
}

void LibInterface::initRegisterTransactionGate() {
  register_transaction_gate =
    cast<Function>(M.getOrInsertFunction("_instr_transaction_gate_register",
                                         Type::getInt64Ty(M.getContext()),
                                         NULL));
}

void LibInterface::initRegisterTransactionGateLocation() {
  register_transaction_gate_location =
    cast<Function>(M.getOrInsertFunction("_instr_transaction_gate_register_location",
                                         Type::getVoidTy(M.getContext()),
                                         Type::getInt64Ty(M.getContext()),
                                         Type::getInt8PtrTy(M.getContext()),
                                         NULL));
}

void LibInterface::initRegisterTransactionGateName() {
  register_transaction_gate_name =
    cast<Function>(M.getOrInsertFunction("_instr_transaction_gate_register_name",
                                         Type::getVoidTy(M.getContext()),
                                         Type::getInt64Ty(M.getContext()),
                                         Type::getInt8PtrTy(M.getContext()),
                                         NULL));
}

void LibInterface::initRegisterOracle() {
  register_oracle =
    cast<Function>(M.getOrInsertFunction("_instr_oracle_register",
                                         Type::getInt64Ty(M.getContext()),
                                         NULL));
}

void LibInterface::initRegisterOracleLocation() {
  register_oracle_location =
    cast<Function>(M.getOrInsertFunction("_instr_oracle_register_location",
                                         Type::getVoidTy(M.getContext()),
                                         Type::getInt64Ty(M.getContext()),
                                         Type::getInt8PtrTy(M.getContext()),
                                         NULL));
}

void LibInterface::initRegisterOracleName() {
  register_oracle_name =
    cast<Function>(M.getOrInsertFunction("_instr_oracle_register_name",
                                         Type::getVoidTy(M.getContext()),
                                         Type::getInt64Ty(M.getContext()),
                                         Type::getInt8PtrTy(M.getContext()),
                                         NULL));
}
