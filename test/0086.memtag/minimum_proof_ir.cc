// Reject forged/mismatched compiler witnesses and preserve observable loads.
#include <uwvm2/runtime/compiler/llvm_jit/memory_minimum_proof.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <cstdio>
#include <cstdlib>
#include <string>
namespace proof=uwvm2::runtime::compiler::llvm_jit::memory_minimum_proof;
static unsigned checks{};
static void require(bool yes,unsigned line){++checks;if(!yes){std::fprintf(stderr,"minimum proof failed %u\n",line);std::abort();}}
#define CHECK(x) require(static_cast<bool>(x),__LINE__)
int main()
{
    llvm::LLVMContext context;
    for(unsigned pointer_bits:{32u,64u}) for(unsigned variant{};variant!=16u;++variant)
    {
        llvm::Module module{"minimum-witness",context};
        module.setDataLayout(pointer_bits==32u?"e-p:32:32":"e-p:64:64");
        llvm::IRBuilder<> b{context};
        auto bits{variant==13u?(pointer_bits==32u?64u:32u):pointer_bits};
        auto type{b.getIntNTy(bits)};
        auto* f{llvm::Function::Create(llvm::FunctionType::get(type,{b.getPtrTy()},false),
            llvm::Function::ExternalLinkage,"f",module)};
        b.SetInsertPoint(llvm::BasicBlock::Create(context,"entry",f));
        auto* load{b.CreateLoad(type,f->getArg(0))};load->setAlignment(llvm::Align{8});
        auto order{llvm::AtomicOrdering::Acquire};
        if(variant==3u)order=llvm::AtomicOrdering::SequentiallyConsistent;
        if(variant==4u)order=llvm::AtomicOrdering::Monotonic;
        if(variant==5u)order=llvm::AtomicOrdering::NotAtomic;
        load->setAtomic(order);
        if(variant==2u)load->setVolatile(true);
        auto md{[&](unsigned width,unsigned value){return llvm::ConstantAsMetadata::get(b.getIntN(width,value));}};
        auto minimum{variant==12u?0u:16u};
        if(variant!=6u)
            load->setMetadata(llvm::LLVMContext::MD_range,llvm::MDNode::get(context,
                {md(bits,variant==8u?32u:minimum),md(bits,variant==7u?8u:0u)}));
        if(variant!=1u && variant!=15u)
        {
            if(variant==14u)
                load->setMetadata(proof::witness_name,llvm::MDNode::get(context,{md(bits,minimum)}));
            else
                load->setMetadata(proof::witness_name,llvm::MDNode::get(context,
                    {md(variant==11u?64u:32u,variant==9u?2u:1u),
                     md(variant==10u?(bits==32u?64u:32u):bits,minimum)}));
        }
        b.CreateRet(variant==1u?static_cast<llvm::Value*>(load):static_cast<llvm::Value*>(b.getIntN(bits,0)));
        // Variant zero is the sole complete producer witness with no user.
        CHECK(proof::discard_unused_bounds_reads(module)==(variant==0u?1u:0u));
        unsigned remaining{};
        for(auto const& block:*f)for(auto const& ins:block)remaining+=llvm::isa<llvm::LoadInst>(ins);
        CHECK(remaining==(variant==0u?0u:1u));
        // Deliberately malformed metadata is rejected by our proof and may
        // separately be rejected by LLVM; only verify the well-shaped cases.
        if(variant<6u || variant==13u)CHECK(!llvm::verifyModule(module,&llvm::errs()));
    }
    // A marked result which is actually used must survive, even if valid.
    llvm::Module module{"used-size",context};module.setDataLayout("e-p:64:64");
    llvm::IRBuilder<> b{context};auto* f=llvm::Function::Create(llvm::FunctionType::get(b.getInt64Ty(),{b.getPtrTy()},false),llvm::Function::ExternalLinkage,"used",module);
    b.SetInsertPoint(llvm::BasicBlock::Create(context,"entry",f));auto* load=b.CreateLoad(b.getInt64Ty(),f->getArg(0));load->setAtomic(llvm::AtomicOrdering::Acquire);load->setAlignment(llvm::Align{8});
    auto* min=llvm::ConstantAsMetadata::get(b.getInt64(16));
    load->setMetadata(llvm::LLVMContext::MD_range,llvm::MDNode::get(context,{min,llvm::ConstantAsMetadata::get(b.getInt64(0))}));
    load->setMetadata(proof::witness_name,llvm::MDNode::get(context,{llvm::ConstantAsMetadata::get(b.getInt32(1)),min}));
    b.CreateRet(load);CHECK(proof::discard_unused_bounds_reads(module)==0u);CHECK(!llvm::verifyModule(module,&llvm::errs()));
    std::printf("PASS %u compiler minimum-witness checks\n",checks);
}
