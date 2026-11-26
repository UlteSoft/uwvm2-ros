// Link with the VM's LLVM MCJIT, AsmParser and native target. This fixture
// supplies a small host callback and executes the production poll lowering.
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/compiler/llvm_jit/compile_all_from_uwvm/impl.h>
#include <uwvm2/runtime/wasm_threads/admission.h>
#include <llvm/AsmParser/Parser.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/MDBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/TargetSelect.h>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <thread>
#include <vector>

#if !defined(UWVM_IMPORT_ROS1) && !defined(UWVM_IMPORT_ROS2) && !defined(UWVM_IMPORT_ROSV2)
#error Compile this ROS poll regression with UWVM_IMPORT_ROSV2.
#endif

namespace
{
    using word=std::uintptr_t;
    using observation=uwvm2::runtime::wasm_threads::threaded_debug_observation;

    void require(bool condition, char const* message)
    {
        if(!condition){std::fprintf(stderr,"%s\n",message);std::abort();}
    }

    struct policy
    {
        observation mode{observation::none};
        bool instructions{};
        auto debug_observation() const { return mode; }
        bool instruction_control() const { return instructions; }
        policy const& admission() const { return *this; }
    };

    struct emitter
    {
        struct native_poll_state
        {
            void* child{};
            word function{};
            unsigned char* stop_byte{};
            unsigned char rejected{};
            void* authenticated_ros_child{};
            word authenticated_ros_owner{};
        };

        static thread_local native_poll_state state;
        static inline thread_local unsigned calls{};
        static constexpr char poll_name[]{"uwvm.threaded.poll.v1"};
        static constexpr char ros_poll_name[]{"uwvm.threaded.poll.ros.v1"};
        policy* world_;
        std::vector<std::pair<std::string,word>> externals_;

        template<typename T> static word address(T pointer){return reinterpret_cast<word>(pointer);}
        static native_poll_state* current_poll_state(){return &state;}
        static llvm::Function* fixed_call_target(llvm::CallInst& call){return call.getCalledFunction();}
        bool add_external(llvm::Function& f,word p){externals_.emplace_back(f.getName().str(),p);return true;}

        static word host_poll(word function,word token)
        {
            ++calls;
            state.function=function;
            return state.rejected || state.authenticated_ros_child!=state.child ||
                (token&~word{1})!=state.authenticated_ros_owner ||
                std::atomic_ref{*state.stop_byte}.load(std::memory_order_acquire);
        }
        static void reject(){state.rejected=1;}

#include <uwvm2/runtime/lib/uwvm_runtime_threaded_llvm_native_poll.h>
    };
    thread_local emitter::native_poll_state emitter::state{};

    // Both the preheader's zero edge and the back edge reach the same loop
    // check. A callback on just one arm must destroy its incoming scope.
    std::string function_ir(std::string const& name,std::string const& initial_function,
        std::string const& initial_token,std::string const& loop_token,bool callback)
    {
        std::string const type="i"+std::to_string(sizeof(word)*CHAR_BIT);
        auto poll=[&](std::string const& function,std::string const& token)
        {return "call "+type+" @uwvm.threaded.poll.ros.v1("+type+" "+function+", "+type+" "+token+")\n";};
        return "define "+type+" @"+name+"("+type+" %token,"+type+" %other,"+type+" %count,ptr %begun){\n"
            "entry:\n %initial="+poll(initial_function,initial_token)+
            "%ok=icmp eq "+type+" %initial,0\n br i1 %ok,label %begin,label %fail\n"
            "begin:\n store atomic volatile i8 1,ptr %begun release,align 1\n br label %loop\n"
            "loop:\n %i=phi "+type+" [0,%begin],[%j,%back]\n %selected=phi "+type+" [%token,%begin],[%other,%back]\n"
            "%current="+poll("7",loop_token)+"switch "+type+" %current,label %stopped ["+type+" 0,label %body]\n"
            "body:\n %j=add "+type+" %i,1\n %done=icmp eq "+type+" %j,%count\n br i1 %done,label %success,label %branch\n"
            "branch:\n %side=icmp eq "+type+" %i,1\n br i1 %side,label %sidearm,label %back\n"
            "sidearm:\n"+(callback?"call void @boundary()\n":"")+"br label %back\nback:\n br label %loop\n"
            "fail:\n ret "+type+" -1\nstopped:\n ret "+type+" %i\nsuccess:\n ret "+type+" %count\n}\n";
    }
}

int main()
{
    require(!llvm::InitializeNativeTarget() && !llvm::InitializeNativeTargetAsmPrinter(),"native target unavailable");
    llvm::LLVMContext context;
    llvm::SMDiagnostic error;
    auto const type="i"+std::to_string(sizeof(word)*CHAR_BIT);
    std::string ir="declare "+type+" @uwvm.threaded.poll.ros.v1("+type+","+type+")\ndeclare void @boundary()\n";
    ir+=function_ir("stable","7","%token","%token",false);
    ir+=function_ir("callback_join","7","%token","%token",true);
    ir+=function_ir("changed_function","3","%token","%token",false);
    ir+=function_ir("changed_owner","7","%other","%token",false);
    ir+=function_ir("varying_owner","7","%token","%selected",false);
    auto module=llvm::parseAssemblyString(ir,error,context);
    if(!module){error.print("threaded_poll_scope_ir",llvm::errs());return 1;}
    policy selection;
    emitter compiler{&selection,{}};
    emitter::native_poll_witnesses witnesses;
    bool changed{};
    auto lower=[&]{compiler.inline_cancellation_loops(*module,{600,80},witnesses,changed);};
    selection.mode=observation::readonly_metadata;lower();
    require(!changed && witnesses.empty(),"debug observation lost its host polls");
    selection.mode=observation::none;selection.instructions=true;lower();
    require(!changed && witnesses.empty(),"instruction control lost its host polls");
    selection.instructions=false;lower();
    require(changed && witnesses.size()==5 && !llvm::verifyModule(*module,&llvm::errs()),"invalid lowered IR");
    for(auto const& [load,call]:witnesses)
    {
        require(load->isVolatile() && load->getOrdering()==llvm::AtomicOrdering::Acquire,"missing live stop acquire");
        auto const full=load->getParent()->getName().starts_with("threaded.poll.acquire");
        require(full==(call->getFunction()->getName()!="stable"),"incorrect poll scope at a loop or CFG join");
    }
    std::unique_ptr<llvm::ExecutionEngine> jit{llvm::EngineBuilder(std::move(module)).setEngineKind(llvm::EngineKind::JIT).create()};
    require(bool(jit),"MCJIT creation failed");
    for(auto const& [name,target]:compiler.externals_)jit->addGlobalMapping(name,target);
    jit->addGlobalMapping(emitter::ros_poll_name,emitter::address(&emitter::host_poll));
    jit->addGlobalMapping("boundary",emitter::address(&emitter::reject));
    jit->finalizeObject();
    using entry=word(*)(word,word,word,unsigned char*);
    auto compiled=[&](char const* name){
        auto f=reinterpret_cast<entry>(jit->getFunctionAddress(name));
        require(f,"missing native entry");return f;
    };
    auto stable=compiled("stable");
    unsigned char begun{},stop{};
    emitter::state={&compiler,0,&stop,0,&compiler,2};
    require(stable(2,4,25,&begun)==25 && emitter::calls==1,"stable activation took unexpected host calls");
    emitter::calls=0;
    require(compiled("changed_function")(2,4,25,&begun)==25 && emitter::calls==2 && emitter::state.function==7,
        "a different function retained the preceding PC");
    emitter::calls=0;
    require(compiled("changed_owner")(4,2,25,&begun)==0 && emitter::calls==2,"a different owner inherited authentication");
    emitter::calls=0;
    require(compiled("varying_owner")(2,4,25,&begun)==1 && emitter::calls==2,"a loop-varying owner escaped authentication");
    emitter::calls=0;
    require(compiled("callback_join")(2,4,25,&begun)==2 && emitter::calls==2,"callback rejection escaped a control-flow join");
    emitter::state.rejected=0;emitter::calls=0;begun=0;
    unsigned payload{};
    std::thread peer([&]{
        while(!std::atomic_ref{begun}.load(std::memory_order_acquire))std::this_thread::yield();
        payload=37;
        std::atomic_ref{stop}.store(1,std::memory_order_release);
    });
    auto const stopped=stable(2,4,UINTPTR_MAX,&begun);
    require(stopped<UINTPTR_MAX && emitter::calls==2 && payload==37,"peer cancellation or its publication was lost");
    peer.join();
    std::puts("PASS threaded poll scope, callback rejection and peer cancellation");
}
#include <uwvm2/utils/macro/pop_macros.h>
