// Actual target LLVM execution of marker arithmetic, plus effect-boundary
// preservation. The real shadow/entropy bridge has separate runtime tests.
#include <uwvm2/runtime/compiler/llvm_jit/compile_all_from_uwvm/translate/memtag_enabled_ir.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
namespace mt=uwvm2::extension::memtag;
namespace jit=uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
static void check(bool v,unsigned line){if(!v){std::fprintf(stderr,"marker IR failed %u\n",line);std::abort();}}
#define CHECK(x) check(static_cast<bool>(x),__LINE__)
std::string name(unsigned bits,unsigned op){return "mte_"+std::to_string(bits)+"_"+std::to_string(op);}
void emit(llvm::Module& m,unsigned bits,mt::instruction_descriptor const& descriptor){
 llvm::IRBuilder<> b(m.getContext());auto type=b.getIntNTy(bits);
 auto f=llvm::Function::Create(llvm::FunctionType::get(type,{type,type,type,type},false),llvm::Function::ExternalLinkage,name(bits,static_cast<unsigned>(descriptor.operation)),m);
 b.SetInsertPoint(llvm::BasicBlock::Create(m.getContext(),"entry",f));
 std::array<llvm::Value*,4> args{f->getArg(0),f->getArg(1),f->getArg(2),f->getArg(3)};unsigned calls{};
 auto r=jit::emit_enabled_memtag_ir(b,descriptor.operation,type,{args.data(),descriptor.operand_count},
  [&](mt::opcode op,std::span<llvm::Value* const> operands)noexcept->jit::enabled_memtag_ir_result{
   CHECK(op==descriptor.operation && operands.size()==descriptor.operand_count);++calls;
   return {true,llvm::ConstantInt::get(type,bits==32?0xf0001234ull:0xff00000000001234ull)};
  });
 CHECK(r.valid);
 bool const effect=descriptor.reads_allocation_tags || descriptor.writes_allocation_tags || descriptor.randomizes_tag;
 CHECK(calls==static_cast<unsigned>(effect));
 b.CreateRet(descriptor.result_count?r.value:llvm::ConstantInt::get(type,0));
}
template<class A>A expected(mt::opcode op,A a,A b,A c,A d){
 constexpr A mask=sizeof(A)==4?A{0x0fffffffu}:static_cast<A>(0x00ffffffffffffffull);
 constexpr unsigned shift=sizeof(A)==4?28:56;
 constexpr A tagmask=static_cast<A>(~mask);
 auto ins=[&](A p,A t){return static_cast<A>((p&mask)|((t<<shift)&tagmask));};
 switch(op){
 case mt::opcode::status:return 2;case mt::opcode::tagbits:return sizeof(A)==4?4:8;case mt::opcode::startbit:return shift;
 case mt::opcode::extract:return a>>shift;case mt::opcode::insert:return ins(a,b);case mt::opcode::copy:return ins(a,b>>shift);
 case mt::opcode::untag:case mt::opcode::untagstore:case mt::opcode::untagstorez:return a&mask;
 case mt::opcode::sub:return static_cast<A>((a&mask)-(b&mask));
 case mt::opcode::hint:return ins(a,static_cast<A>((b>>shift)+c));
 case mt::opcode::hintstore:case mt::opcode::hintstorez:return ins(a,static_cast<A>((c>>shift)+d));
 case mt::opcode::load:case mt::opcode::random:case mt::opcode::randomstore:case mt::opcode::randomstorez:return static_cast<A>((a&mask)|tagmask);
 case mt::opcode::store:case mt::opcode::storez:return 0;
 default:std::abort();
 }
}
template<class A>unsigned run(llvm::ExecutionEngine& e){
 unsigned count{};std::uint64_t random=0x29dd12b3a7331943ull;
 for(auto const& d:mt::instructions)if(d.operation!=mt::opcode::add){
  using fn=A(*)(A,A,A,A);auto function=reinterpret_cast<fn>(e.getFunctionAddress(name(sizeof(A)*8,static_cast<unsigned>(d.operation))));CHECK(function);
  for(unsigned i{};i!=256;++i){
   std::array<A,4> a{};for(auto& v:a){random=random*6364136223846793005ull+1442695040888963407ull;v=static_cast<A>(random);}
   CHECK(function(a[0],a[1],a[2],a[3])==expected(d.operation,a[0],a[1],a[2],a[3]));++count;
  }
 }
 return count;
}
int main(){
 CHECK(!llvm::InitializeNativeTarget()&&!llvm::InitializeNativeTargetAsmPrinter());
 llvm::LLVMContext c;auto m=std::make_unique<llvm::Module>("memtag-markers",c);
 std::unique_ptr<llvm::TargetMachine> target{llvm::EngineBuilder{}.selectTarget()};CHECK(target);m->setDataLayout(target->createDataLayout());
 for(unsigned bits:{32u,64u})for(auto const& d:mt::instructions)if(d.operation!=mt::opcode::add)emit(*m,bits,d);
 CHECK(!llvm::verifyModule(*m,&llvm::errs()));std::string error;
 std::unique_ptr<llvm::ExecutionEngine> engine{llvm::EngineBuilder(std::move(m)).setErrorStr(&error).setEngineKind(llvm::EngineKind::JIT).setOptLevel(llvm::CodeGenOptLevel::Aggressive).create()};
 if(!engine){std::fprintf(stderr,"%s\n",error.c_str());return 2;}
 engine->finalizeObject();auto checks=run<std::uint32_t>(*engine)+run<std::uint64_t>(*engine);
 std::printf("PASS native LLVM marker IR: %u cases, arithmetic/tag wrap/low address preservation\n",checks);
}
