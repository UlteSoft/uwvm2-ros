/** @file
 * @brief Exercise volatile output FP controls under a fixed throwing allocator.
 * @details Exercises the production authority, preflight, broker and private
 * cache with 48 x86 control profiles. Success, exact replay, sequence conflict,
 * malformed and denied requests, short receipts, cancellation, allocation
 * failure and failed delivery are checked for rounding/masks/flush/precision.
 * The injected allocator below throws directly. It deliberately cannot prove
 * preservation across a standard std::new_handler; the concrete host wrapper
 * must construct its native control guard before any allocation or identity
 * work. A separate real-wrapper regression covers hostile allocation handlers.
 * Only test sinks run; arbitrary runtime memory callbacks retain their separate
 * nested guard and are qualified by actual VM arithmetic tests. Sticky flags
 * are caller-saved and excluded. No WASI/ROS-v1 implementation is changed.
 */
#include <uwvm2/imported/ros/scene/broker.h>
#include "../0008.imported/wasi/wasip1/func/fp_control_probe.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <new>

namespace { thread_local int fail_allocation_after{-1}; }
[[gnu::noinline]] void* operator new(std::size_t bytes)
{
    if(fail_allocation_after>=0&&fail_allocation_after--==0)throw std::bad_alloc{};
    if(auto value=std::malloc(bytes?bytes:1))return value;
    throw std::bad_alloc{};
}
[[gnu::noinline]] void operator delete(void* value) noexcept{std::free(value);}
[[gnu::noinline]] void operator delete(void* value,std::size_t) noexcept{std::free(value);}
void* operator new[](std::size_t bytes){return ::operator new(bytes);}
void operator delete[](void* value) noexcept{::operator delete(value);}
void operator delete[](void* value,std::size_t) noexcept{::operator delete(value);}

namespace scene=uwvm2::imported::ros::scene;
namespace contract=uwvm2::imported::ros::output_contract;
namespace probe=uwvm2test::wasip1_fp_control;
using op=contract::operation;
using error=scene::error;

struct controls
{
    int rounding{std::fegetround()};
    std::uint_least64_t arch{probe::read_arch_control()};
    std::uint16_t x87{};
    controls()
    {
#if (defined(__GNUC__) || defined(__clang__)) && (defined(__i386__) || defined(__x86_64__))
        __asm__ volatile("fnstcw %0" : "=m"(x87) : : "memory");
#endif
    }
    bool operator==(controls const&) const=default;
};

static void prepare(int rounding,bool flush,bool unmask,unsigned precision)
{
    assert(std::fesetenv(FE_DFL_ENV)==0&&std::fesetround(rounding)==0);
    if(flush)probe::enable_flush_control();
#if (defined(__GNUC__) || defined(__clang__)) && (defined(__i386__) || defined(__x86_64__))
    auto state=controls{};
    auto changed=static_cast<std::uint16_t>((state.x87&~0x0300u)|(precision<<8u));
    if(unmask)changed=static_cast<std::uint16_t>(changed&~4u);
    __asm__ volatile("fnclex\n\tfldcw %0" : : "m"(changed) : "memory");
# if defined(__SSE__) || defined(__x86_64__)
    if(unmask)_mm_setcsr(_mm_getcsr()&~0x023fu);
# endif
#else
    (void)unmask;(void)precision;
#endif
}

struct fixture
{
    int identity{};scene::caller who{&identity,1};scene::broker broker;
    std::array<scene::handle,4> caps{};
    std::array<unsigned,7> successes{};
    std::array<std::uint8_t,72> output{};
    std::size_t required{};
    fixture()
    {
        assert(broker.permissions.expose("capability.open",scene::exposure::allow)==error::success);
        for(auto const& description:contract::operations)
            assert(broker.permissions.expose(std::string(description.name),scene::exposure::allow)==error::success);
        std::array names{"cache","calls","bytes","denied"};
        for(std::size_t index{};index<names.size();++index)
        {
            scene::grant grant;grant.alias=names[index];grant.resource="scratch";
            grant.binding="volatile-output";grant.kind=scene::purpose::cache;grant.max_resources=2;
            for(auto const& description:contract::operations)grant.actions.emplace(description.name);
            if(index==1){grant.limits.calls=0;grant.limits.uncertain=true;}
            if(index==2){grant.limits.bytes=0;grant.limits.uncertain=true;}
            if(index==3){grant.actions.clear();grant.actions.emplace("artifact.read");}
            assert(broker.permissions.configure(std::move(grant))==error::success);
            assert(broker.output_store.bind(names[index],"scratch",{128,2,64,64})==error::success);
        }
        assert(broker.permissions.enroll(who)==error::success);
        for(std::size_t index{};index<names.size();++index)
            assert(broker.permissions.open(who,names[index],caps[index])==error::success);
    }
    error call(op action,std::initializer_list<std::uint64_t> words,error expected=error::success,
               std::size_t capacity=72,bool sink_ok=true,bool cancel=false,int fail=-1,std::size_t owner=0)
    {
        std::array<std::uint8_t,88> input{};std::size_t index{};
        for(auto value:words)scene::put(input,index++,value);
        bool emitted{};
        auto writer=[&](scene::bytes value) noexcept
        {assert(value.size()==output.size());emitted=true;std::copy(value.begin(),value.end(),output.begin());return sink_ok;};
        scene::sink sink=std::ref(writer);std::function<bool()> cancelled=[=]{return cancel;};
        auto before=controls{};fail_allocation_after=fail;error result;
        try{result=broker.invoke_output(who,action,caps[owner],{input.data(),words.size()*8},capacity,required,sink,cancelled);}
        catch(std::bad_alloc const&){result=error::no_memory;}
        fail_allocation_after=-1;
        assert(controls{}==before&&result==expected);
        if(expected==error::success){assert(emitted);++successes[static_cast<std::size_t>(action)];}
        else if(expected!=error::invalid||sink_ok)assert(!emitted);
        return result;
    }
    void preflight(op action,error expected,std::size_t size,bool complete=true,std::size_t owner=0)
    {
        auto before=controls{};auto result=broker.preflight_output(who,action,caps[owner],size,complete);
        assert(controls{}==before&&result==expected);
    }
    std::uint64_t word(std::size_t index)const{return scene::word(output,index);}
    void unchanged_receipt(std::uint64_t key,std::array<std::uint8_t,72> const& expected)
    {call(op::receipt,{key});assert(output==expected);}
};

static void exercise()
{
    fixture f;
    // Exact action, capability, shape, short-result and budget refusals for all
    // seven operations must return without changing any FP control register.
    for(auto const& description:contract::operations)
    {
        std::array<std::uint8_t,24> input{};std::size_t required{},size=description.words<0?24:description.words*8u;
        auto writer=[](scene::bytes){assert(false);return false;};scene::sink sink=std::ref(writer);
        auto before=controls{};
        assert(f.broker.invoke_output(f.who,description.id,0,{input.data(),size},72,required,sink)==error::bad_handle);
        assert(controls{}==before);
        assert(f.broker.invoke_output(f.who,description.id,f.caps[3],{input.data(),size},72,required,sink)==error::denied);
        assert(controls{}==before);
        assert(f.broker.invoke_output(f.who,description.id,f.caps[0],{input.data(),size},71,required,sink)==error::too_large&&required==72);
        assert(controls{}==before);
        assert(f.broker.invoke_output(f.who,description.id,f.caps[0],{input.data(),size-1},72,required,sink)==error::invalid);
        assert(controls{}==before);
        for(std::size_t owner:{1u,2u})
        {
            f.preflight(description.id,error::again,size,true,owner);
            f.preflight(description.id,error::success,size,false,owner);
            assert(f.broker.invoke_output(f.who,description.id,f.caps[owner],{input.data(),size},72,required,sink)==error::again);
            assert(controls{}==before);
        }
        f.preflight(description.id,error::denied,size,true,3);
        f.preflight(description.id,error::success,size);
    }
    f.preflight(op::append,error::too_large,89);
    f.call(op::begin,{1,32},error::cancelled,72,true,true);
    f.call(op::begin,{1,32},error::no_memory,72,true,false,0); // Payload allocation.
    f.call(op::begin,{1,32},error::no_memory,72,true,false,1); // Registry node allocation.
    f.call(op::receipt,{1},error::conflict);
    f.call(op::begin,{1,32},error::invalid,72,false);auto first=f.output;auto object=f.word(2);
    assert(object&&f.word(7)==1&&f.word(3)==0);
    f.unchanged_receipt(1,first);
    f.call(op::begin,{1,32},error::success,72,true,false,0);assert(f.output==first);
    f.call(op::begin,{1,33},error::conflict);
    f.call(op::append,{2,object,0,0x0807060504030201},error::too_large,71);
    f.call(op::append,{2,object,0,0x0807060504030201},error::invalid,72,false);auto appended=f.output;
    assert(f.word(4)==8&&f.word(5)==8&&f.word(7)==1);
    f.call(op::append,{2,object,0,0x0807060504030201},error::success,72,true,false,0);assert(f.output==appended);
    f.call(op::append,{2,object,0,0x0807060504030202},error::conflict);
    f.unchanged_receipt(2,appended);
    f.call(op::commit,{3,object,9});assert(f.word(3)==static_cast<unsigned>(error::conflict)&&f.word(7)==1);
    auto failed_commit=f.output;f.call(op::commit,{3,object,9});assert(f.output==failed_commit);
    f.call(op::commit,{4,object,8},error::cancelled,72,true,true);
    f.call(op::commit,{4,object,8},error::invalid,72,false);auto published=f.output;
    assert(f.word(7)==2&&f.word(8)==1);
    f.call(op::commit,{4,object,8},error::success,72,true,false,0);assert(f.output==published);
    f.call(op::status,{object});assert(f.word(0)==4&&f.word(7)==2);
    f.unchanged_receipt(4,published);
    f.call(op::remove,{5,object},error::invalid,72,false);auto removed=f.output;
    assert(f.word(6)==0&&f.word(7)==4);
    f.call(op::remove,{5,object},error::success,72,true,false,0);assert(f.output==removed);
    f.unchanged_receipt(5,removed);
    f.call(op::status,{object},error::bad_handle);
    f.call(op::begin,{6,32});object=f.word(2);
    f.call(op::abort,{7,object},error::invalid,72,false);auto aborted=f.output;
    f.call(op::abort,{7,object},error::success,72,true,false,0);assert(f.output==aborted&&f.word(7)==3);
    f.unchanged_receipt(7,aborted);
    f.call(op::begin,{8,65});assert(f.word(3)==static_cast<unsigned>(error::again)&&f.word(2)==0);
    f.call(op::append,{9,UINT64_MAX,0});assert(f.word(3)==static_cast<unsigned>(error::bad_handle)&&f.word(2)==0);
    for(auto count:f.successes)assert(count);
    auto before=controls{};f.broker.output_store.revoke("cache");assert(controls{}==before);
    f.preflight(op::receipt,error::denied,8);
    f.call(op::receipt,{9},error::denied);
}

int main()
{
    probe::initial_state_restore restore;assert(restore.valid);unsigned profiles{};
    for(auto rounding:{FE_TONEAREST,probe::hostile_rounding_down,probe::hostile_rounding_up,FE_TOWARDZERO})
        for(auto flush:{false,true})for(auto unmask:{false,true})for(auto precision:{0u,2u,3u})
        {prepare(rounding,flush,unmask,precision);exercise();++profiles;}
    assert(std::fesetenv(FE_DFL_ENV)==0&&profiles==48);
    std::printf("PASS 48 FP control profiles; seven output operations, preflight, replay, deny, short output, cancellation, allocation failure and lost-receipt recovery\n");
}
