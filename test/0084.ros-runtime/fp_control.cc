/** @file
 * @brief Verify the local scheduling broker preserves floating-point controls.
 * @details This exercises the broker directly, without the host wrapper's
 * control-restoration guard. It checks rounding, exception masks and flush
 * controls on normal/refused paths. Real allocator new_handler callbacks and
 * native-wrapper restoration have a separate production-allocator fixture;
 * this injected allocator does not establish that property. Sticky exception
 * flags are caller-saved and are excluded.
 * This test varies controls, checks all sixteen operations, and injects an
 * allocator failure. It installs no native ROS, model or device callbacks.
 * Run through tools/ros_scene/in_existing_cgroup.py in the existing test gate.
 */
#include <uwvm2/imported/ros/scene/broker.h>
#include "../0008.imported/wasi/wasip1/func/fp_control_probe.h"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <new>

namespace { thread_local bool refuse_allocation{}; }
[[gnu::noinline]] void* operator new(std::size_t bytes)
{
    if(refuse_allocation)throw std::bad_alloc{};
    if(auto value=std::malloc(bytes?bytes:1))return value;
    throw std::bad_alloc{};
}
[[gnu::noinline]] void operator delete(void* pointer) noexcept{std::free(pointer);}
[[gnu::noinline]] void operator delete(void* pointer,std::size_t) noexcept{std::free(pointer);}
void* operator new[](std::size_t bytes){return ::operator new(bytes);}
void operator delete[](void* pointer) noexcept{::operator delete(pointer);}
void operator delete[](void* pointer,std::size_t) noexcept{::operator delete(pointer);}

namespace s=uwvm2::imported::ros::scene;
namespace r=uwvm2::imported::ros::resources;
namespace contract=uwvm2::imported::ros::runtime_contract;
namespace probe=uwvm2test::wasip1_fp_control;
using op=contract::operation;

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
    bool operator==(controls const&) const = default;
};

void prepare(int rounding,bool flush,bool unmask,unsigned precision)
{
    assert(std::fesetenv(FE_DFL_ENV)==0);
    assert(std::fesetround(rounding)==0);
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
    int identity{};s::caller who{&identity,1};s::broker broker;
    std::array<s::handle,4> caps{};
    std::array<unsigned,16> successes{};
    std::array<std::uint8_t,32> output{};
    std::size_t required{};
    fixture()
    {
        assert(broker.permissions.expose("capability.open",s::exposure::allow)==s::error::success);
        std::array kinds{s::purpose::guard,s::purpose::ros_clock,s::purpose::timer,s::purpose::wait};
        std::array<std::string,4> names{"guard","clock","timer","wait"};
        for(auto const& description:contract::operations)
            assert(broker.permissions.expose(std::string(description.name),s::exposure::allow)==s::error::success);
        for(std::size_t n{};n!=names.size();++n)
        {
            s::grant grant;grant.alias=names[n];grant.resource="robot";grant.binding="local";
            grant.kind=kinds[n];grant.max_resources=8;grant.max_wait_members=4;grant.max_wait_ns=1000000;
            grant.actions={"resource.observe","resource.close"};
            for(auto const& description:contract::operations)
            {
                if(description.name.starts_with(names[n]+"."))grant.actions.emplace(description.name);
            }
            assert(broker.permissions.configure(std::move(grant))==s::error::success);
        }
        assert(broker.permissions.enroll(who)==s::error::success);
        for(std::size_t n{};n!=names.size();++n)
        {
            assert(broker.permissions.open(who,names[n],caps[n])==s::error::success);
        }
    }
    s::error call(std::size_t owner,op action,std::initializer_list<std::uint64_t> arguments,
                  s::error expected=s::error::success,std::size_t capacity=32,bool sink_ok=true,
                  bool cancel=false,bool reject_new=false,s::handle override_cap=UINT64_MAX)
    {
        std::array<std::uint8_t,40> input{};std::size_t at{};
        for(auto value:arguments)s::put(input,at++,value);
        s::sink sink=[&](s::bytes bytes)
        {assert(bytes.size()<=output.size());std::copy(bytes.begin(),bytes.end(),output.begin());return sink_ok;};
        std::function<bool()> cancelled=[=]{return cancel;};
        auto before=controls{};s::error result{};bool allocation_failed{};
        refuse_allocation=reject_new;
        try{result=broker.invoke(who,action,override_cap==UINT64_MAX?caps[owner]:override_cap,
                                {input.data(),arguments.size()*8},capacity,required,sink,cancelled);}
        catch(std::bad_alloc const&){allocation_failed=true;}
        refuse_allocation=false;
        assert(controls{}==before);
        if(reject_new){assert(allocation_failed);return s::error::success;}
        assert(!allocation_failed&&result==expected);
        if(result==s::error::success)++successes[static_cast<std::size_t>(action)];
        return result;
    }
    std::uint64_t word(std::size_t index=0)const{return s::word(output,index);}
};

void exercise()
{
    fixture f;
    // Every operation must preserve controls when authority refuses it.
    for(auto const& description:contract::operations)
    {
        std::array<std::uint8_t,40> bytes{};std::size_t required{};
        s::sink sink=[](s::bytes){assert(false);return false;};
        auto before=controls{};
        assert(f.broker.invoke(f.who,description.id,0,{bytes.data(),description.words*8u},32,required,sink)==s::error::bad_handle);
        assert(controls{}==before);
    }
    f.call(0,op::guard_create,{},s::error::too_large,7);
    f.call(0,op::guard_create,{},s::error::invalid,32,false);
    f.call(0,op::guard_create,{},s::error::success,32,true,false,true);
    f.call(0,op::guard_create,{});auto guard=f.word();
    f.call(0,op::guard_trigger,{guard});
    f.call(0,op::guard_take,{guard},s::error::too_large,7);
    f.call(0,op::guard_take,{guard},s::error::invalid,32,false);
    f.call(0,op::guard_take,{guard});assert(f.word()==1);
    f.call(0,op::guard_take,{guard},s::error::again);
    f.call(1,op::clock_create,{0},s::error::denied);
    f.call(1,op::clock_create,{2});auto clock=f.word();
    f.call(1,op::clock_update,{f.caps[1],clock,100,1});
    f.call(1,op::clock_read,{clock});assert(f.word()==100);
    f.call(2,op::timer_create,{f.caps[1],clock});auto timer=f.word();
    f.call(2,op::timer_reset,{timer,105,10});
    f.call(2,op::timer_take,{timer},s::error::again);
    f.call(1,op::clock_update,{f.caps[1],clock,125,1});
    f.call(2,op::timer_take,{timer},s::error::invalid,32,false);
    f.call(2,op::timer_take,{timer});assert(f.word()==105&&f.word(2)==3);
    f.call(2,op::timer_cancel,{timer});
    f.call(3,op::wait_set_create,{});auto wait=f.word();
    f.call(3,op::wait_set_add,{wait,f.caps[1],clock,r::changed,77});
    f.call(3,op::wait_set_wait,{wait,0},s::error::timeout);
    f.call(3,op::wait_set_wait,{wait,1},s::error::timeout);
    f.call(3,op::wait_set_wait,{wait,0},s::error::cancelled,32,true,true);
    f.call(3,op::wait_set_wait,{wait,1000},s::error::cancelled,32,true,true);
    f.call(3,op::wait_set_wait,{wait,UINT64_MAX},s::error::denied);
    f.call(1,op::clock_update,{f.caps[1],clock,135,1});
    f.call(3,op::wait_set_wait,{wait,0},s::error::too_large,31);
    f.call(3,op::wait_set_wait,{wait,0},s::error::invalid,32,false);
    f.call(3,op::wait_set_wait,{wait,0});auto sequence=f.word(3);
    f.call(3,op::wait_set_wait,{wait,1000});
    f.call(3,op::wait_set_acknowledge,{wait,clock,sequence});
    f.call(3,op::wait_set_remove,{wait,clock});
    f.call(0,op::resource_close,{guard});
    f.call(0,op::resource_close,{guard},s::error::bad_handle);
    f.call(1,op::resource_close,{clock});
    f.call(2,op::resource_close,{timer});
    f.call(3,op::resource_close,{wait});
    for(auto count:f.successes)assert(count);
}

int main()
{
    probe::initial_state_restore restore;assert(restore.valid);unsigned profiles{};
    for(auto rounding:{FE_TONEAREST,probe::hostile_rounding_down,probe::hostile_rounding_up,FE_TOWARDZERO})
        for(auto flush:{false,true})for(auto unmask:{false,true})for(auto precision:{0u,2u,3u})
        {prepare(rounding,flush,unmask,precision);exercise();++profiles;}
    assert(std::fesetenv(FE_DFL_ENV)==0);
    std::cout<<"PASS: "<<profiles<<" FP control profiles, all 16 local operations, authority/capacity/sink refusal, timeout, cancellation, allocator failure\n";
}
