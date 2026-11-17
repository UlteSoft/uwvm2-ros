/** @file
 * @brief Verify synchronous borrowed output delivery without callback allocation.
 * @details The production host lends a four-reference stack callback through
 * std::ref. Conversion must not allocate; fixed-output operations must execute
 * with allocation refused. Waiting may allocate bounded event records, but
 * must deliver only on its caller before returning and retain no callback for
 * later trigger/revoke/retirement. ASan use-after-scope checks those lifetimes.
 * This test does not install native providers or relax resource authority.
 */
#include <uwvm2/imported/ros/scene/broker.h>
#include <atomic>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <new>
#include <thread>

#ifndef UWVM_TEST_CALLBACK_LIFETIMES_ONLY
namespace {thread_local bool refuse_allocation{};}
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
#endif

namespace s=uwvm2::imported::ros::scene;
namespace r=uwvm2::imported::ros::resources;
namespace contract=uwvm2::imported::ros::runtime_contract;
using op=contract::operation;

struct fixture
{
    int identity{};s::caller who{&identity,1};s::broker broker;
    std::array<s::handle,4> caps{};std::array<std::uint8_t,32> output{};
    std::size_t required{};
    fixture()
    {
        assert(broker.permissions.expose("capability.open",s::exposure::allow)==s::error::success);
        for(auto const& operation:contract::operations)
            assert(broker.permissions.expose(std::string(operation.name),s::exposure::allow)==s::error::success);
        std::array kinds{s::purpose::guard,s::purpose::ros_clock,s::purpose::timer,s::purpose::wait};
        std::array<std::string,4> names{"guard","clock","timer","wait"};
        for(std::size_t n{};n!=names.size();++n)
        {
            s::grant grant;grant.alias=names[n];grant.resource="robot";grant.binding="local";grant.kind=kinds[n];
            grant.max_resources=8;grant.max_wait_members=4;grant.max_wait_ns=5000000000;
            grant.actions={"resource.observe","resource.close"};
            for(auto const& operation:contract::operations)
                if(operation.name.starts_with(names[n]+"."))grant.actions.emplace(operation.name);
            assert(broker.permissions.configure(std::move(grant))==s::error::success);
        }
        assert(broker.permissions.enroll(who)==s::error::success);
        for(std::size_t n{};n!=names.size();++n)
            assert(broker.permissions.open(who,names[n],caps[n])==s::error::success);
    }
    s::error call(std::size_t owner,op operation,std::initializer_list<std::uint64_t> words,s::sink const& sink,
                  std::function<bool()> const& cancelled={})
    {
        std::array<std::uint8_t,40> input{};std::size_t at{};for(auto value:words)s::put(input,at++,value);
        return broker.invoke(who,operation,caps[owner],{input.data(),words.size()*8},32,required,sink,cancelled);
    }
    s::error call(std::size_t owner,op operation,std::initializer_list<std::uint64_t> words)
    {
        auto copy=[&](s::bytes bytes){std::copy(bytes.begin(),bytes.end(),output.begin());return true;};
        return call(owner,operation,words,std::ref(copy));
    }
    std::uint64_t word(std::size_t index=0)const{return s::word(output,index);}
};

struct destination
{
    std::array<std::uint8_t,64> memory{};
    std::thread::id owner{std::this_thread::get_id()};
    unsigned deliveries{};bool accept{true};bool fail{};
    bool copy(std::size_t offset,s::bytes bytes)
    {
        assert(owner==std::this_thread::get_id());++deliveries;
        if(fail)throw 17;
        if(!accept)return false;
        assert(offset+bytes.size()<=memory.size());std::copy(bytes.begin(),bytes.end(),memory.begin()+offset);return true;
    }
};

#ifndef UWVM_TEST_CALLBACK_LIFETIMES_ONLY
void allocation_and_transactionality()
{
    fixture f;
    assert(f.call(0,op::guard_create,{})==s::error::success);auto guard=f.word();
    assert(f.call(1,op::clock_create,{2})==s::error::success);auto clock=f.word();
    assert(f.call(2,op::timer_create,{f.caps[1],clock})==s::error::success);auto timer=f.word();
    destination memory;std::size_t output{},length_output=40,capacity=32;
    auto deliver=[&memory,&output,&length_output,&capacity](s::bytes bytes)
    {
        if(bytes.size()>capacity)return false;
        s::put(memory.memory,length_output/8,bytes.size());return memory.copy(output,bytes);
    };
    static_assert(sizeof(deliver)==4*sizeof(void*));
    // Positive control: the original owning conversion exceeds both tested
    // standard libraries' inline storage. Do not accept a broken fault injector.
    bool owning_failed{};refuse_allocation=true;
    try{s::sink owning{deliver};(void)owning;}catch(std::bad_alloc const&){owning_failed=true;}
    assert(owning_failed);
    s::sink borrowed{std::ref(deliver)}; // Must survive with every allocation refused.
    assert(f.call(0,op::guard_trigger,{guard},std::ref(deliver))==s::error::success);
    memory.accept=false;
    assert(f.call(0,op::guard_take,{guard},borrowed)==s::error::invalid);
    memory.accept=true;memory.fail=true;bool thrown{};
    try{f.call(0,op::guard_take,{guard},borrowed);}catch(int value){assert(value==17);thrown=true;}
    assert(thrown);memory.fail=false;
    assert(f.call(0,op::guard_take,{guard},borrowed)==s::error::success);
    assert(s::word(memory.memory,0)==1);
    assert(f.call(0,op::guard_take,{guard},borrowed)==s::error::again);
    assert(f.call(1,op::clock_update,{f.caps[1],clock,100,1},borrowed)==s::error::success);
    assert(f.call(1,op::clock_read,{clock},borrowed)==s::error::success&&s::word(memory.memory,0)==100);
    assert(f.call(2,op::timer_reset,{timer,105,10},borrowed)==s::error::success);
    assert(f.call(1,op::clock_update,{f.caps[1],clock,125,1},borrowed)==s::error::success);
    memory.accept=false;
    assert(f.call(2,op::timer_take,{timer},borrowed)==s::error::invalid);
    memory.accept=true;
    assert(f.call(2,op::timer_take,{timer},borrowed)==s::error::success&&s::word(memory.memory,2)==3);
    assert(f.call(2,op::timer_cancel,{timer},borrowed)==s::error::success);
    assert(f.call(0,op::resource_close,{guard},borrowed)==s::error::success);
    refuse_allocation=false;
}
#endif

/** @brief Perform a blocking call while its output callback lives only here. */
[[gnu::noinline]] void scoped_wait(fixture& f,s::handle wait,s::error expected,bool cancel,
                                 std::atomic<bool>* entered=nullptr)
{
    destination memory;std::size_t output{},length_output=40,capacity=32;
    auto deliver=[&memory,&output,&length_output,&capacity](s::bytes bytes)
    {
        if(bytes.size()>capacity)return false;
        s::put(memory.memory,length_output/8,bytes.size());return memory.copy(output,bytes);
    };
    auto stop=[&]
    {
        if(entered){entered->store(true,std::memory_order_release);entered->notify_one();}
        return cancel;
    };
    assert(f.call(3,op::wait_set_wait,{wait,5000000000},std::ref(deliver),std::ref(stop))==expected);
    assert(memory.deliveries==(expected==s::error::success?1u:0u));
}

void wait_lifetimes()
{
    fixture f;
    assert(f.call(0,op::guard_create,{})==s::error::success);auto guard=f.word();
    assert(f.call(3,op::wait_set_create,{})==s::error::success);auto wait=f.word();
    assert(f.call(3,op::wait_set_add,{wait,f.caps[0],guard,r::readable|r::closed,77})==s::error::success);
    std::atomic<bool> entered{};
    auto trigger=std::thread{[&]
    {
        entered.wait(false,std::memory_order_acquire);
        // Separate output storage from the blocked caller's result length.
        std::array<std::uint8_t,8> input{};s::put(input,0,guard);std::size_t required{};
        assert(f.broker.invoke(f.who,op::guard_trigger,f.caps[0],input,0,required,[](s::bytes){return true;})==s::error::success);
    }};
    scoped_wait(f,wait,s::error::success,false,&entered);trigger.join();
    // The preceding callback frame has left. Further triggers and consumption
    // must never invoke it again; ASan catches a retained stack reference.
    assert(f.call(0,op::guard_take,{guard})==s::error::success);
    scoped_wait(f,wait,s::error::cancelled,true);
    assert(f.call(0,op::guard_trigger,{guard})==s::error::success);
    assert(f.call(0,op::guard_take,{guard})==s::error::success);
    entered=false;
    auto retire=std::thread{[&]
    {entered.wait(false,std::memory_order_acquire);assert(f.broker.revoke("wait")==s::error::success);}};
    scoped_wait(f,wait,s::error::cancelled,false,&entered);retire.join();
    assert(f.broker.revoke("guard")==s::error::success);f.broker.retire();
}

int main()
{
#ifndef UWVM_TEST_CALLBACK_LIFETIMES_ONLY
    allocation_and_transactionality();
    std::cout<<"PASS: borrowed sink conversion and fixed-output calls without allocation; failed/throwing delivery preserves events\n";
#endif
    wait_lifetimes();
    std::cout<<"PASS: blocking wake/cancel/revoke with stack-scoped callbacks\n";
}
