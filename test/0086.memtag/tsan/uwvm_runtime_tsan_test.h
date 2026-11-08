// TEST-ONLY: frozen snapshot qualification; never included by the live product.
#pragma once
#ifndef UWVM_TEST_TSAN_JIT
#error This header belongs only to the private TSan qualification snapshot.
#endif
#if !__has_feature(thread_sanitizer)
#error The test snapshot must itself be compiled with ThreadSanitizer.
#endif
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Transforms/Instrumentation/ThreadSanitizer.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/FileSystem.h>
#include <dlfcn.h>
#include <unwind.h>
#include <cstdint>
extern "C" _Unwind_Reason_Code __gcc_personality_v0(int, _Unwind_Action, std::uint64_t, _Unwind_Exception*, _Unwind_Context*);
#include <unistd.h>
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace uwvm_tsan_test {
inline std::atomic<unsigned long> sequence{};
inline llvm::FunctionType* expected_type(llvm::Function& fn) {
    auto& c=fn.getContext();
    auto* v=llvm::Type::getVoidTy(c);
    auto* p=llvm::PointerType::getUnqual(c);
    auto* i32=llvm::Type::getInt32Ty(c);
    auto* size=llvm::Type::getInt64Ty(c); // This test profile is native x86_64 only.
    auto name=fn.getName();
    auto type=[&](llvm::Type* result,std::initializer_list<llvm::Type*> args) {
        return llvm::FunctionType::get(result,llvm::ArrayRef<llvm::Type*>(args),false);
    };
    if(name=="__tsan_func_entry")return type(v,{p});
    if(name=="__tsan_func_exit")return type(v,{});
    if(name=="__tsan_atomic_thread_fence"||name=="__tsan_atomic_signal_fence")return type(v,{i32});
    if(name=="__tsan_memcpy"||name=="__tsan_memmove")return type(p,{p,p,size});
    if(name=="__tsan_memset")return type(p,{p,i32,size});
    for(unsigned bytes:{1u,2u,4u,8u,16u}) {
        auto suffix=std::to_string(bytes);
        for(char const* operation:{"read","write","unaligned_read","unaligned_write",
             "volatile_read","volatile_write","unaligned_volatile_read","unaligned_volatile_write",
             "read_write","unaligned_read_write"})
            if(name==("__tsan_"+std::string(operation)+suffix))return type(v,{p});
        auto* integer=llvm::IntegerType::get(c,bytes*8u);
        auto prefix="__tsan_atomic"+std::to_string(bytes*8u)+"_";
        if(name==prefix+"load")return type(integer,{p,i32});
        if(name==prefix+"store")return type(v,{p,integer,i32});
        for(char const* operation:{"exchange","fetch_add","fetch_sub","fetch_and","fetch_nand","fetch_or","fetch_xor"})
            if(name==prefix+operation)return type(integer,{p,integer,i32});
        if(name==prefix+"compare_exchange_val")return type(integer,{p,integer,integer,i32,i32});
    }
    // LLVM's sanitizer cleanup can introduce the standard C cleanup personality.
    // It resolves to existing compiler-rt + the same paired unwinder, not libgcc.
    if(name=="__gcc_personality_v0")return type(i32,{i32,i32,size,p,p});
    if(name=="_Unwind_Resume")return type(v,{p});
    return nullptr;
}
// LLVM models a personality as i32(...) even though the native unwind ABI is
// fixed. Permit that representation only in the dedicated Function operand:
// direct calls, pointer escapes, aliases, prefix/prologue data are rejected.
inline bool personality_only(llvm::Function& fn) {
    if(fn.getName()!="__gcc_personality_v0" || !fn.isDeclaration() || fn.use_empty())return false;
    auto* type=fn.getFunctionType();
    if(type!=expected_type(fn) && !(type->isVarArg() && type->getNumParams()==0 && type->getReturnType()->isIntegerTy(32)))return false;
    for(auto& use:fn.uses()) {
        auto* owner=llvm::dyn_cast<llvm::Function>(use.getUser());
        if(!owner || !owner->hasPersonalityFn() || owner->getPersonalityFn()!=&fn)return false;
        unsigned occurrences{};
        for(auto& operand:owner->operands())occurrences+=operand.get()==&fn;
        if(occurrences!=1)return false;
    }
    return true;
}
template<class Bind> bool instrument(llvm::Module& module,llvm::TargetMachine& target,Bind&& bind) {
    if(module.getModuleFlag("uwvm.test.tsan.instrumented"))return false;
    std::vector<llvm::Function*> functions;
    unsigned long before_atomics{},before_ordinary{};
    for(auto& fn:module) {
        if(fn.isDeclaration())continue;
        if(fn.hasFnAttribute(llvm::Attribute::SanitizeThread))return false;
        for(auto& block:fn)for(auto& instruction:block) {
            if(auto* call=llvm::dyn_cast<llvm::CallBase>(&instruction))
                if(auto* callee=call->getCalledFunction())
                    if(callee->getName().starts_with("__tsan_"))return false;
            if(auto* load=llvm::dyn_cast<llvm::LoadInst>(&instruction))
                (load->isAtomic()?before_atomics:before_ordinary)++;
            if(auto* store=llvm::dyn_cast<llvm::StoreInst>(&instruction))
                (store->isAtomic()?before_atomics:before_ordinary)++;
            if(llvm::isa<llvm::AtomicRMWInst>(instruction)||llvm::isa<llvm::AtomicCmpXchgInst>(instruction))before_atomics++;
        }
        fn.addFnAttr(llvm::Attribute::SanitizeThread);
        functions.push_back(&fn);
    }
    llvm::LoopAnalysisManager loops;
    llvm::FunctionAnalysisManager functions_analysis;
    llvm::CGSCCAnalysisManager graph;
    llvm::ModuleAnalysisManager modules;
    llvm::PassBuilder passes(&target);
    passes.registerLoopAnalyses(loops);passes.registerFunctionAnalyses(functions_analysis);
    passes.registerCGSCCAnalyses(graph);passes.registerModuleAnalyses(modules);
    passes.crossRegisterProxies(loops,functions_analysis,graph,modules);
    llvm::FunctionPassManager instrument_functions;
    instrument_functions.addPass(llvm::ThreadSanitizerPass{});
    // The instrumented native executable already initializes compiler-rt.
    // Do not invent a JIT constructor which this private loader does not execute.
    for(auto* fn:functions)instrument_functions.run(*fn,functions_analysis);
    module.addModuleFlag(llvm::Module::Error,"uwvm.test.tsan.instrumented",1u);
    bool has_resume{};
    unsigned long atomics{},ordinary{},entries{},exits{},unhandled_atomics{};
    for(auto const* fn:functions)for(auto const& block:*fn)for(auto const& instruction:block) {
        has_resume|=llvm::isa<llvm::ResumeInst>(instruction);
        if(auto* call=llvm::dyn_cast<llvm::CallBase>(&instruction))if(auto* callee=call->getCalledFunction()) {
            auto name=callee->getName();
            atomics+=name.starts_with("__tsan_atomic");
            ordinary+=name.starts_with("__tsan_read")||name.starts_with("__tsan_write")||
                name.starts_with("__tsan_unaligned")||name.starts_with("__tsan_volatile");
            entries+=name=="__tsan_func_entry";exits+=name=="__tsan_func_exit";
        }
        if(auto* load=llvm::dyn_cast<llvm::LoadInst>(&instruction))unhandled_atomics+=load->isAtomic();
        if(auto* store=llvm::dyn_cast<llvm::StoreInst>(&instruction))unhandled_atomics+=store->isAtomic();
        unhandled_atomics+=llvm::isa<llvm::AtomicRMWInst>(instruction)||llvm::isa<llvm::AtomicCmpXchgInst>(instruction);
    }
    if(unhandled_atomics||llvm::verifyModule(module,&llvm::errs())){std::fprintf(stderr,"TSAN_TEST rejected post-pass unhandled_atomics=%lu\n",unhandled_atomics);return false;}
    // ResumeInst becomes an exact native libunwind call during machine lowering.
    // Register that fixed ABI before the unchanged object symbol allowlist runs.
    if(has_resume && !module.getFunction("_Unwind_Resume"))
        llvm::Function::Create(llvm::FunctionType::get(llvm::Type::getVoidTy(module.getContext()),{llvm::PointerType::getUnqual(module.getContext())},false),llvm::Function::ExternalLinkage,"_Unwind_Resume",module);
    unsigned bindings{};
    for(auto& fn:module) {
        if(!fn.isDeclaration()||(fn.use_empty() && !(has_resume && fn.getName()=="_Unwind_Resume")))continue;
        auto name=fn.getName();
        if(!name.starts_with("__tsan_")&&name!="__gcc_personality_v0"&&name!="_Unwind_Resume")continue;
        auto* expected=expected_type(fn);
        if(!expected||(name=="__gcc_personality_v0" ? !personality_only(fn) : fn.getFunctionType()!=expected)||fn.getCallingConv()!=llvm::CallingConv::C){
            llvm::errs()<<"TSAN_TEST ABI rejected: "<<name<<" actual="<<*fn.getFunctionType();
            if(expected)llvm::errs()<<" expected="<<*expected;
            llvm::errs()<<"\n";return false;}
        auto spelling=name.str();
        void* pointer{};
        if(name=="__gcc_personality_v0")pointer=reinterpret_cast<void*>(&::__gcc_personality_v0);
        else if(name=="_Unwind_Resume")pointer=reinterpret_cast<void*>(&::_Unwind_Resume);
        else pointer=::dlsym(RTLD_DEFAULT,spelling.c_str());
        if(!pointer||!bind(fn,reinterpret_cast<std::uintptr_t>(pointer))) {
            std::fprintf(stderr,"TSAN_TEST binding rejected: %s\n",spelling.c_str());return false;
        }
        ++bindings;
    }
    auto* directory=std::getenv("UWVM_TSAN_IR_DIRECTORY");
    if(!directory||directory[0]!='/')return false;
    auto id=sequence.fetch_add(1,std::memory_order_relaxed);
    auto path=std::string(directory)+"/"+std::to_string(static_cast<long>(getpid()))+"-"+std::to_string(id)+".ll";
    std::error_code error;
    llvm::raw_fd_ostream output(path,error,llvm::sys::fs::OF_Text);
    if(error)return false;
    module.print(output,nullptr);output.close();
    std::fprintf(stderr,"TSAN_TEST_IR pid=%ld id=%lu functions=%zu before_atomic=%lu before_ordinary=%lu atomic_callbacks=%lu ordinary_callbacks=%lu entries=%lu exits=%lu typed_bindings=%u\n",
        static_cast<long>(getpid()),id,functions.size(),before_atomics,before_ordinary,atomics,ordinary,entries,exits,bindings);
    return true;
}
} // namespace uwvm_tsan_test
