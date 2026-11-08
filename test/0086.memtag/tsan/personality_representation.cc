#define UWVM_TEST_TSAN_JIT 1
#include "uwvm_runtime_tsan_test.h"
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Target/TargetMachine.h>
int main(int argc,char** argv){
 llvm::InitializeNativeTarget();llvm::InitializeNativeTargetAsmPrinter();llvm::LLVMContext c;llvm::Module m("personality-control",c);std::unique_ptr<llvm::TargetMachine> target{llvm::EngineBuilder{}.selectTarget()};m.setDataLayout(target->createDataLayout());m.setTargetTriple(target->getTargetTriple());
 auto* type=llvm::FunctionType::get(llvm::Type::getVoidTy(c),false);auto* f=llvm::Function::Create(type,llvm::Function::ExternalLinkage,"control",m);auto* call=llvm::Function::Create(type,llvm::Function::ExternalLinkage,"may_throw",m);llvm::IRBuilder<> b(llvm::BasicBlock::Create(c,"entry",f));b.CreateCall(call);b.CreateRetVoid();
 if(argc>1) {
  auto* personality=llvm::Function::Create(llvm::FunctionType::get(llvm::Type::getInt32Ty(c),true),llvm::Function::ExternalLinkage,"__gcc_personality_v0",m);
  f->setPersonalityFn(personality);
  if(argv[1][0]=='c'){llvm::IRBuilder<> injection(f->getEntryBlock().getTerminator());injection.CreateCall(personality);}
  else if(argv[1][0]=='d')new llvm::GlobalVariable(m,llvm::PointerType::getUnqual(c),false,llvm::GlobalValue::ExternalLinkage,personality,"escaped_personality");
  else if(argv[1][0]=='p')f->setPrefixData(personality);
 }
 bool okay=uwvm_tsan_test::instrument(m,*target,[](llvm::Function&,std::uintptr_t p){return p!=0;});m.print(llvm::errs(),nullptr);std::fprintf(stderr,"PERSONALITY accepted=%d\n",okay);return okay?0:3;
}
