// Real LLVM verifier and native execution regression for a returning MTE fault
// recorder. LLVM 23 getTerminator() assumes a terminated block; it is not a
// nullable query. The old implementation left memtag.mismatch unterminated.
#include <uwvm2/runtime/compiler/llvm_jit/compile_all_from_uwvm/translate/memtag_check_ir.h>
#include <uwvm2/extension/memtag/shadow.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

namespace jit=uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
namespace mt=uwvm2::extension::memtag;
static void check(bool value,unsigned line)
{ if(!value) { std::fprintf(stderr,"async LLVM proof failed at %u\n",line);std::abort(); } }
#define CHECK(...) check(static_cast<bool>((__VA_ARGS__)),__LINE__)

static std::string name(unsigned bits,unsigned bytes,bool shared,bool deferred,unsigned mask)
{ return "mte_"+std::to_string(bits)+"_"+std::to_string(bytes)+"_"+std::to_string(shared)+"_"+std::to_string(deferred)+"_"+std::to_string(mask); }

static void emit(llvm::Module& module,unsigned bits,unsigned bytes,bool shared,bool deferred,unsigned mask)
{
    llvm::IRBuilder<> b(module.getContext());
    auto* pointer=llvm::PointerType::getUnqual(module.getContext());
    auto* type=llvm::FunctionType::get(b.getInt32Ty(),{pointer,pointer,b.getIntNTy(bits),pointer},false);
    auto* function=llvm::Function::Create(type,llvm::Function::ExternalLinkage,name(bits,bytes,shared,deferred,mask),module);
    b.SetInsertPoint(llvm::BasicBlock::Create(module.getContext(),"entry",function));
    auto* offset=b.CreateAnd(b.CreateZExtOrTrunc(function->getArg(2),b.getInt64Ty()),
        b.getInt64(bits==32u?0x0fffffffull:0x00ffffffffffffffull));
    auto* outside=llvm::BasicBlock::Create(module.getContext(),"outside",function);
    auto* bounded=llvm::BasicBlock::Create(module.getContext(),"bounded",function);
    b.CreateCondBr(b.CreateICmpUGT(offset,b.getInt64(64u-bytes)),outside,bounded);
    b.SetInsertPoint(outside);b.CreateRet(b.getInt32(2u));b.SetInsertPoint(bounded);
    auto slot=[&](unsigned index) { return b.CreateGEP(b.getInt64Ty(),function->getArg(3),b.getInt64(index)); };
    CHECK(jit::emit_memtag_scalar_check(b,function->getArg(0),function->getArg(2),offset,bytes,shared,
        [&](llvm::Value* raw,llvm::Value* granule,llvm::Value* pointer_tag,llvm::Value* allocation_tag) noexcept
        {
            auto* count=b.CreateLoad(b.getInt64Ty(),slot(0u));
            auto* first=b.CreateICmpEQ(count,b.getInt64(0u));
            std::array<llvm::Value*,4u> values{raw,granule,b.CreateZExt(pointer_tag,b.getInt64Ty()),b.CreateZExt(allocation_tag,b.getInt64Ty())};
            for(unsigned index{};index!=values.size();++index)
            {
                auto* address=slot(index+1u);
                b.CreateStore(b.CreateSelect(first,values[index],b.CreateLoad(b.getInt64Ty(),address)),address);
            }
            b.CreateStore(b.CreateAdd(count,b.getInt64(1u)),slot(0u));
            if(!deferred) { b.CreateRet(b.getInt32(1u)); }
            // A deferred recorder deliberately returns without a terminator.
            // The production helper must append the branch to the access.
        },nullptr,0u,deferred,static_cast<std::uint8_t>(mask)));
    auto* data=b.CreateGEP(b.getInt8Ty(),function->getArg(1),offset);
    for(unsigned index{};index!=bytes;++index)
    {
        auto* store=b.CreateStore(b.getInt8(0xa5u),b.CreateGEP(b.getInt8Ty(),data,b.getInt64(index)));
        store->setAlignment(llvm::Align{1u});
        if(shared) { store->setAtomic(llvm::AtomicOrdering::Monotonic); }
    }
    b.CreateRet(b.getInt32(0u));
}

template<typename Address>
static void exercise(llvm::ExecutionEngine& engine,unsigned bytes,bool shared,bool deferred,unsigned mask,unsigned& cases)
{
    using view=mt::shadow_view<Address>;
    using entry=unsigned(*)(void*,void*,Address,void*);
    auto* function=reinterpret_cast<entry>(engine.getFunctionAddress(name(sizeof(Address)*8u,bytes,shared,deferred,mask)));
    CHECK(function!=nullptr);
    std::array<std::uint8_t,4u> tags{};
    std::array<std::uint8_t,64u> data{};
    std::array<std::uint64_t,5u> evidence{};
    constexpr unsigned tag=sizeof(Address)==4u?7u:0xa3u;
    for(unsigned offset:{0u,15u,16u,31u,48u,63u})
    {
        if(offset>data.size()-bytes) { continue; }
        auto pointer=view::insert(static_cast<Address>(offset),static_cast<Address>(tag));
        auto const first=offset/16u,last=(offset+bytes-1u)/16u;
        for(unsigned target:{first,last}) for(unsigned change:{0u,1u,16u})
        {
            if(sizeof(Address)==4u && change==16u) { continue; }
            tags.fill(tag);tags[target]^=change;data.fill(0x3cu);evidence.fill(0u);
            bool const mismatches=(change&mask)!=0u;
            CHECK(function(tags.data(),data.data(),pointer,evidence.data())==(mismatches&&!deferred?1u:0u));
            CHECK(evidence[0]==static_cast<unsigned>(mismatches));
            if(mismatches)
            {
                CHECK(evidence[1]==pointer);
                CHECK(evidence[2]==target*16u); // Common ABI is a BYTE offset.
                CHECK(evidence[3]==tag && evidence[4]==(tag^change));
            }
            for(unsigned index{};index!=data.size();++index)
            { CHECK(data[index]==(!mismatches||deferred ? (index>=offset && index<offset+bytes?0xa5u:0x3cu) : 0x3cu)); }
            ++cases;
        }
    }
    // Bounds failure precedes any shadow read or deferred recording, even when
    // comparison_mask==0 delegates tag checking to a physical MTE access.
    data.fill(0x3cu);evidence.fill(0u);
    CHECK(function(nullptr,data.data(),view::insert(64u,tag),evidence.data())==2u);
    CHECK(evidence[0]==0u);for(auto byte:data) { CHECK(byte==0x3cu); }++cases;
}

int main()
{
    CHECK(!llvm::InitializeNativeTarget() && !llvm::InitializeNativeTargetAsmPrinter());
    llvm::LLVMContext context;
    auto module=std::make_unique<llvm::Module>("async-tag-fault-continuations",context);
    std::unique_ptr<llvm::TargetMachine> target{llvm::EngineBuilder{}.selectTarget()};CHECK(target);
    module->setDataLayout(target->createDataLayout());
    for(unsigned bits:{32u,64u}) for(unsigned bytes:{1u,2u,4u,8u,16u}) for(bool shared:{false,true})
    {
        for(bool deferred:{false,true}) { emit(*module,bits,bytes,shared,deferred,0xffu); }
        for(bool deferred:{false,true}) { emit(*module,bits,bytes,shared,deferred,bits==32u?0u:0xf0u); }
    }
    if(llvm::verifyModule(*module,&llvm::errs())) { return 3; }
    std::string error;
    std::unique_ptr<llvm::ExecutionEngine> engine{llvm::EngineBuilder(std::move(module)).setErrorStr(&error)
        .setEngineKind(llvm::EngineKind::JIT).setOptLevel(llvm::CodeGenOptLevel::Aggressive).create()};
    if(!engine) { std::fprintf(stderr,"%s\n",error.c_str());return 2; }
    engine->finalizeObject();unsigned cases{};
    for(unsigned bytes:{1u,2u,4u,8u,16u}) for(bool shared:{false,true})
    {
        for(bool deferred:{false,true})
        { exercise<std::uint32_t>(*engine,bytes,shared,deferred,0xffu,cases);exercise<std::uint64_t>(*engine,bytes,shared,deferred,0xffu,cases); }
        for(bool deferred:{false,true})
        { exercise<std::uint32_t>(*engine,bytes,shared,deferred,0u,cases);exercise<std::uint64_t>(*engine,bytes,shared,deferred,0xf0u,cases); }
    }
    std::printf("PASS async LLVM verifier/native execution: %u cases, returning recorder, exact granule bytes, immediate bounds, full/partial tag masks\n",cases);
}
