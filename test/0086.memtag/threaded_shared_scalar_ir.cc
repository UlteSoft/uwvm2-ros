// Run the threaded child's actual shared-scalar IR against a small native
// owner and its real fair tag gate. The bridge records every cold edge.
#include <uwvm2/utils/mutex/rw_spin_lock.h>
#include <uwvm2/extension/memtag/execution_policy.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/MDBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <memory>

namespace locks=uwvm2::utils::mutex;
struct memory
{
    std::byte* base;std::atomic_size_t* length;std::atomic_size_t prepared;locks::rwlock_t* gate;
};
struct scope { void* child;unsigned char rejected;bool const* access_checks;unsigned char const* stop_byte; };
struct codegen
{
    using native_poll_state=scope;
    using memory_type=memory;
    template<typename T> static std::uintptr_t address(T p){return reinterpret_cast<std::uintptr_t>(p);}
#include <uwvm2/runtime/lib/uwvm_runtime_threaded_llvm_shared_scalar_ir.h>
};
static unsigned misses{};
static std::uint64_t bridge(std::uintptr_t,std::uint64_t,std::uint64_t,std::uint64_t){++misses;return 99;}
static void require(bool condition){if(!condition)std::abort();}

int main()
{
    require(!llvm::InitializeNativeTarget()&&!llvm::InitializeNativeTargetAsmPrinter());
    llvm::LLVMContext c;auto m=std::make_unique<llvm::Module>("shared-scalar",c);
    std::unique_ptr<llvm::TargetMachine> machine{llvm::EngineBuilder{}.selectTarget()};require(bool(machine));
    std::array<std::byte,34> data{};std::atomic_size_t length{32};locks::rwlock_t gate{};
    memory owner{data.data(),&length,32,&gate};auto* slot=&owner;
    int child{},peer{};bool checks{};unsigned char stop{};scope state{&child,0,&checks,&stop};
    codegen::shared_scalar_layout layout{&slot,0xffffu,offsetof(memory,base),offsetof(memory,length),offsetof(memory,prepared),offsetof(memory,gate)};
    llvm::IRBuilder<> b(c);auto* word=b.getIntNTy(sizeof(void*)*8u);auto* wide=b.getInt64Ty();
    auto* cold=llvm::Function::Create(llvm::FunctionType::get(wide,{word,wide,wide,wide},false),llvm::Function::ExternalLinkage,"scalar_bridge",*m);
    for(bool store:{false,true})
    {
        auto* f=llvm::Function::Create(llvm::FunctionType::get(wide,{b.getPtrTy(),wide,wide},false),
            llvm::Function::ExternalLinkage,store?"store":"load",*m);
        b.SetInsertPoint(llvm::BasicBlock::Create(c,"entry",f));
        // The production emitter creates a host pointer, then converts it to
        // the integer bridge argument before LLVM assigns the target layout.
        auto* literal=llvm::ConstantInt::get(word,codegen::address(&owner));
        auto* operand=b.CreatePtrToInt(b.CreateIntToPtr(literal,b.getPtrTy()),word);
        require(codegen::matches_shared_scalar_owner(literal,codegen::address(&owner)));
        require(codegen::matches_shared_scalar_owner(operand,codegen::address(&owner)));
        require(!codegen::matches_shared_scalar_owner(operand,codegen::address(&peer)));
        require(!codegen::matches_shared_scalar_owner(f->getArg(1),codegen::address(&owner)));
        auto* call=b.CreateCall(cold,{operand,b.getInt64(1),f->getArg(1),f->getArg(2)});
        b.CreateRet(call);
        codegen::emit_shared_scalar(*call,f->getArg(0),codegen::address(&child),layout,{64u,32u,2u,!store,store});
    }
    unsigned extent_loads{};
    for(auto const& function:*m)for(auto const& block:function)for(auto const& instruction:block)
        if(auto const* load=llvm::dyn_cast<llvm::LoadInst>(&instruction);load&&load->getType()==word&&load->getOrdering()==llvm::AtomicOrdering::Acquire)
        {
            require(load->getAlign()>=llvm::Align{alignof(std::atomic_size_t)});
            ++extent_loads;
        }
    require(extent_loads==4u);
    m->setDataLayout(machine->createDataLayout());
    require(!llvm::verifyModule(*m,&llvm::errs()));
    std::unique_ptr<llvm::ExecutionEngine> jit{llvm::EngineBuilder(std::move(m)).setEngineKind(llvm::EngineKind::JIT).create()};
    require(bool(jit));jit->addGlobalMapping("scalar_bridge",codegen::address(&bridge));jit->finalizeObject();
    using entry=std::uint64_t(*)(scope*,std::uint64_t,std::uint64_t);
    auto load=reinterpret_cast<entry>(jit->getFunctionAddress("load"));
    auto store=reinterpret_cast<entry>(jit->getFunctionAddress("store"));require(load&&store);
    auto tagged=UINT64_C(0xdcab00000000000e); // Effective address 15 straddles two granules.
    require(store(&state,tagged,0x80ab)==0 && data[15]==std::byte{0xab} && data[16]==std::byte{0x80});
    require(load(&state,tagged,0)==UINT32_C(0xffff80ab) && !misses && gate.bits.load()==0u);
    {locks::rw_fair_shared_guard_t reader{gate};require(load(&state,tagged,0)==UINT32_C(0xffff80ab)&&gate.bits.load()==locks::rwlock_reader_mask());}
    auto const saved=data;
    auto miss=[&](std::uint64_t raw){auto before=misses;require(store(&state,raw,0)==99u&&misses==before+1u&&data==saved);};
    checks=true;miss(tagged);checks=false;
    state.child=&peer;miss(tagged);state.child=&child;
    slot=nullptr;miss(tagged);slot=&owner;
    state.rejected=1;miss(tagged);state.rejected=0;stop=1;miss(tagged);stop=0;
    {locks::rw_fair_unique_guard_t writer{gate};miss(tagged);}
    gate.bits.store(locks::rwlock_pending_mask());miss(tagged);gate.bits.store(0);
    miss(30);owner.prepared.store(31);miss(tagged);owner.prepared.store(32);
    owner.prepared.store(34);length.store(34);require(store(&state,31,0x0123)==0);
    require(data[32]==std::byte{0x23}&&data[33]==std::byte{1}&&gate.bits.load()==0u);
    std::puts("PASS threaded shared-scalar authorization, range, writer exclusion and byte effects");
}
