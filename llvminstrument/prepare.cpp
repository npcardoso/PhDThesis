#include "prepare.h"

static RegisterPass<Instrument::PrepareInstrumentionPass> prepare_instrumentation("instrument_prepare", "Prepares instrumentation instructions");

char Instrument::PrepareInstrumentionPass::ID = 1;

Instrument::PrepareInstrumentionPass::PrepareInstrumentionPass() : Pass(ID) {
  num_oracles = 0;
  num_probes = 0;
  num_transactions = 0;
  function_overrides["pthread_create"] = NULL;
}

bool Instrument::PrepareInstrumentionPass::initFunctionOverrides(Module & M){
  bool changes = false;
  ValueSymbolTable& symtbl = M.getValueSymbolTable();
  foreach(it, function_overrides){
    Value* value = symtbl.lookup(it->first);
    if(value && isa<Function>(value)){
      Function * tmp_fun = dyn_cast<Function>(value);
      StringRef instr_name = (lib_prefix + it->first);

      changes = true;
      it->second = M.getOrInsertFunction(instr_name, tmp_fun->getFunctionType());
    }
    else
      it->second = NULL;
  }
  return changes;
}

Instrument::Pass::~Pass(){

}

bool Instrument::PrepareInstrumentionPass::runOnModule(Module & M){
  bool changes = false;

  interface = new LibInterface(M);
  initFunctionOverrides(M);

  changes = Pass::runOnModule(M);

  foreach(item, metadata)
    metadataSetup(M, *item->second, item->first);
  
  metadata.clear();

  delete interface;
  return changes;
}

bool Instrument::PrepareInstrumentionPass::handleFunctionCall(Module & M, CallInst & call){

  if(Function * f = call.getCalledFunction()){
    if(Value * instr_fun = function_overrides[(f->getName()).str()]){
      call.setCalledFunction(instr_fun);
      return true;
    }

    if(f->getName() == "_instr_metadata")
      return registerMetadata(M, call);

    if(f->getName() == "_instr_probe_observation")
      return registerProbe(M, call);

    if(f->getName() == "_instr_transaction_start")
      return registerTransactionGate(M, call);

    if(f->getName() == "_instr_transaction_end")
      return registerTransactionGate(M, call);

    if(f->getName() == "_instr_oracle_observation")
      return registerOracle(M, call);
  }
  return false;
}

bool Instrument::PrepareInstrumentionPass::metadataSetup(Module & M,
                                                         CallInst & I,
                                                         std::string id_var_name) {
  GlobalVariable* id_holder = M.getGlobalVariable(id_var_name, true);

  if(!id_holder)
    return false;
  
  I.removeFromParent();
  BasicBlock::iterator & it = interface->getRegisterAllIterator();
  I.insertBefore(it);
  I.setArgOperand(0, new LoadInst(id_holder, NULL, &I));
  return true;
}


bool Instrument::PrepareInstrumentionPass::registerConstruct(Module & M,
                                                            CallInst & I,
                                                            std::string var_prefix,
                                                            size_t id,
                                                            Function * reg_fun, Function * reg_location_fun) {

  char buf[1024];
  sprintf(buf, "%lu", id);

  std::string id_var_name = var_prefix + buf;
  
  errs() << "Registering construct " << id_var_name << "\n";

  Type * operand_type = I.getArgOperand(0)->getType();
  GlobalVariable* id_holder =
    new GlobalVariable(M,
                       operand_type, //Type
                       false, //Constant
                       GlobalValue::PrivateLinkage, //Linkage
                       ConstantInt::getSigned(operand_type, 0), //Default Value
                       id_var_name);

  BasicBlock::iterator & it = interface->getRegisterAllIterator();

  /* Inject registration p_id */
  CallInst * register_instr = CallInst::Create(reg_fun, "", it);
  new StoreInst(register_instr, id_holder, false, it);

  /* Inject correct p_id */
  I.setArgOperand(0, new LoadInst(id_holder, NULL, &I));

  return true;
}

bool Instrument::PrepareInstrumentionPass::registerStrProp(Module & M,
                                                           GlobalVariable & id_holder,
                                                           Function * registar,
                                                           std::string var_name,
                                                           std::string value) {

  Constant * data_array = ConstantDataArray::getString(M.getContext(), value , true);
  GlobalVariable* variable =
    new GlobalVariable(M,
                       ArrayType::get(Type::getInt8Ty(M.getContext()),value.size()+1), //Type
                       true, //Constant
                       GlobalValue::PrivateLinkage, //Linkage
                       data_array, //Default Value
                       var_name);

  ConstantInt* zero = ConstantInt::get(M.getContext(), APInt(32, StringRef("0"), 10));
  std::vector<Constant*> indices;
  indices.push_back(zero);
  indices.push_back(zero);


  std::vector<Value*> args;
  BasicBlock::iterator & it = interface->getRegisterAllIterator();
  args.push_back(new LoadInst(&id_holder, NULL, it)); //ID
  args.push_back(ConstantExpr::getGetElementPtr(variable, indices)); //
  CallInst::Create(registar, args, "", it);

  return true;
}
