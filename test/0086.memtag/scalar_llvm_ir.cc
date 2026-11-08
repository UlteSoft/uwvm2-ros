// Actual native LLVM execution of the scalar tag proof, with test status returns.
#include <uwvm2/runtime/compiler/llvm_jit/compile_all_from_uwvm/translate/memtag_check_ir.h>
#include <uwvm2/extension/memtag/shadow.h>
#include <llvm/IR/Verifier.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
namespace mt=uwvm2::extension::memtag;
namespace jit=uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
static void check(bool v,unsigned l){if(!v){std::fprintf(stderr,"tag IR failure %u\n",l);std::abort();}}
#define CHECK(x) check(static_cast<bool>(x),__LINE__)
std::string name(unsigned bits,unsigned bytes,bool shared)
{return "tag_"+std::to_string(bits)+"_"+std::to_string(bytes)+"_"+std::to_string(shared);}
void emit(llvm::Module& m,unsigned bits,unsigned bytes,bool shared)
{
 llvm::IRBuilder<> b(m.getContext());auto p=llvm::PointerType::getUnqual(m.getContext());
 auto f=llvm::Function::Create(llvm::FunctionType::get(b.getInt32Ty(),{p,p,b.getIntNTy(bits)},false),llvm::Function::ExternalLinkage,name(bits,bytes,shared),m);
 b.SetInsertPoint(llvm::BasicBlock::Create(m.getContext(),"entry",f));
 auto offset=b.CreateAnd(b.CreateZExtOrTrunc(f->getArg(2),b.getInt64Ty()),b.getInt64(bits==32?0x0fffffffull:0x00ffffffffffffffull));
 auto bad=llvm::BasicBlock::Create(m.getContext(),"oob",f),good=llvm::BasicBlock::Create(m.getContext(),"bounded",f);
 b.CreateCondBr(b.CreateICmpUGT(offset,b.getInt64(256u-bytes)),bad,good);
 b.SetInsertPoint(bad);b.CreateRet(b.getInt32(0xffffu));b.SetInsertPoint(good);
 CHECK(jit::emit_memtag_scalar_check(b,f->getArg(0),f->getArg(2),offset,bytes,shared,
  [&](llvm::Value*,llvm::Value* granule,llvm::Value*,llvm::Value*) noexcept
  {b.CreateRet(b.CreateAdd(b.CreateTrunc(granule,b.getInt32Ty()),b.getInt32(1u)));}));
 auto destination=b.CreateGEP(b.getInt8Ty(),f->getArg(1),offset);
 for(unsigned i{};i!=bytes;++i){auto s=b.CreateStore(b.getInt8(0xa5),b.CreateGEP(b.getInt8Ty(),destination,b.getInt64(i)));s->setAlignment(llvm::Align{1});if(shared)s->setAtomic(llvm::AtomicOrdering::Monotonic);}
 b.CreateRet(b.getInt32(0));
}
template<typename A>void exercise(llvm::ExecutionEngine& engine,unsigned bytes,bool shared,unsigned& cases)
{
 using V=mt::shadow_view<A>;auto fn=reinterpret_cast<unsigned(*)(void*,void*,A)>(engine.getFunctionAddress(name(sizeof(A)*8u,bytes,shared)));CHECK(fn);
 std::array<std::uint8_t,16> tags{};std::array<std::uint8_t,256> data{};
 for(unsigned offset{};offset<=256u-bytes;++offset)
 {
  unsigned tag=(offset*137u+bytes)&V::maximum_tag;tags.fill(tag);data.fill(0x3c);auto pointer=V::insert(offset,tag);
  CHECK(fn(tags.data(),data.data(),pointer)==0u);++cases;
  for(unsigned i{};i!=data.size();++i)CHECK(data[i]==(i>=offset&&i<offset+bytes?0xa5u:0x3cu));
  auto first=offset/16u,last=(offset+bytes-1u)/16u;
  for(auto bad:{first,last}){tags.fill(tag);tags[bad]=tag^1u;data.fill(0x3c);CHECK(fn(tags.data(),data.data(),pointer)==bad+1u);++cases;for(auto v:data)CHECK(v==0x3cu);}
 }
 data.fill(0x3c);CHECK(fn(nullptr,data.data(),V::insert(256u,1u))==0xffffu);++cases;for(auto v:data)CHECK(v==0x3cu);
}
int main()
{
 CHECK(!llvm::InitializeNativeTarget()&&!llvm::InitializeNativeTargetAsmPrinter());
 llvm::LLVMContext c;auto m=std::make_unique<llvm::Module>("memtag-scalar-proof",c);std::unique_ptr<llvm::TargetMachine> target{llvm::EngineBuilder{}.selectTarget()};CHECK(target);m->setDataLayout(target->createDataLayout());
 for(unsigned bits:{32u,64u})for(unsigned bytes=1;bytes<=16;++bytes)for(bool shared:{false,true})emit(*m,bits,bytes,shared);
 CHECK(!llvm::verifyModule(*m,&llvm::errs()));std::string error;
 std::unique_ptr<llvm::ExecutionEngine> engine{llvm::EngineBuilder(std::move(m)).setErrorStr(&error).setEngineKind(llvm::EngineKind::JIT).setOptLevel(llvm::CodeGenOptLevel::Aggressive).create()};
 if(!engine){std::fprintf(stderr,"%s\n",error.c_str());return 2;}engine->finalizeObject();unsigned cases{};
 for(unsigned bytes=1;bytes<=16;++bytes)for(bool shared:{false,true}){exercise<std::uint32_t>(*engine,bytes,shared,cases);exercise<std::uint64_t>(*engine,bytes,shared,cases);}
 std::printf("PASS native LLVM scalar tag proof: %u cases; width1..16, all alignments, no partial store\n",cases);
}
