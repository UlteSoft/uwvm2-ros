#include <uwvm2/imported/ros/scene/broker.h>
#include <cassert>
#include <future>
#include <iostream>
#include <map>
#include <stdexcept>

namespace s=uwvm2::imported::ros::scene;
namespace r=uwvm2::imported::ros::resources;
namespace contract=uwvm2::imported::ros::runtime_contract;
using op=contract::operation;
using namespace std::chrono_literals;

std::vector<std::uint8_t> encode(std::initializer_list<std::uint64_t> words)
{
    std::vector<std::uint8_t> bytes;
    for(auto word:words)for(unsigned byte{};byte!=8;++byte)bytes.push_back(static_cast<std::uint8_t>(word>>(byte*8)));
    return bytes;
}
std::uint64_t decode(std::span<std::uint8_t const> bytes,std::size_t word=0)
{
    assert(bytes.size()>=word*8+8);std::uint64_t value{};
    for(unsigned byte{};byte!=8;++byte)value|=std::uint64_t(bytes[word*8+byte])<<(byte*8);
    return value;
}
struct reply {s::error status{};std::size_t required{};std::vector<std::uint8_t> bytes;unsigned deliveries{};};
struct fixture
{
    int identity{};s::broker broker;s::caller caller{&identity,1};std::map<std::string,s::handle> caps;
    fixture()
    {
        for(auto const& action:contract::operations)assert(broker.permissions.expose(std::string(action.name),s::exposure::allow)==s::error::success);
        assert(broker.permissions.expose("capability.open",s::exposure::allow)==s::error::success);
        assert(broker.permissions.expose("capability.close",s::exposure::allow)==s::error::success);
    }
    void add(std::string alias,s::purpose purpose,std::initializer_list<std::string> actions,
             std::string resource="robot-control",std::string binding="local",std::uint64_t maximum=2)
    {
        s::grant grant;grant.alias=alias;grant.resource=std::move(resource);grant.binding=std::move(binding);
        grant.kind=purpose;grant.actions=actions;grant.max_resources=maximum;grant.max_wait_members=4;grant.max_wait_ns=5000000000;
        assert(broker.permissions.configure(std::move(grant))==s::error::success);caps.emplace(std::move(alias),0);
    }
    void start()
    {
        assert(broker.permissions.enroll(caller)==s::error::success);
        for(auto& [alias,cap]:caps)assert(broker.permissions.open(caller,alias,cap)==s::error::success);
    }
    reply call(std::string const& alias,op operation,std::initializer_list<std::uint64_t> request,std::size_t capacity=32)
    {
        reply result;auto input=encode(request);
        result.status=broker.invoke(caller,operation,caps.at(alias),input,capacity,result.required,[&](s::bytes bytes)
        {++result.deliveries;result.bytes.assign(bytes.begin(),bytes.end());return true;});return result;
    }
};

void transactional_and_owned()
{
    fixture f;
    for(auto alias:{"signal","other"})f.add(alias,s::purpose::guard,{"guard.create","guard.trigger","guard.take","resource.close"},alias,"local",1);
    f.start();
    auto small=f.call("signal",op::guard_create,{},7);
    assert(small.status==s::error::too_large&&small.required==8&&small.deliveries==0);
    std::vector<r::snapshot> resources;assert(f.broker.runtime.inspect(resources,2)&&resources.empty());
    std::size_t required{};
    assert(f.broker.invoke(f.caller,op::guard_create,f.caps.at("signal"),{},8,required,[](s::bytes){return false;})==s::error::invalid);
    assert(f.broker.runtime.inspect(resources,2)&&resources.empty());
    bool thrown{};
    try {f.broker.invoke(f.caller,op::guard_create,f.caps.at("signal"),{},8,required,[](s::bytes)->bool{throw std::runtime_error("checked output");});}
    catch(std::runtime_error const&){thrown=true;}
    assert(thrown&&f.broker.runtime.inspect(resources,2)&&resources.empty());
    auto created=f.call("signal",op::guard_create,{},8);assert(created.status==s::error::success&&created.required==8);
    auto id=decode(created.bytes);
    assert(f.call("signal",op::guard_create,{}).status==s::error::again);
    assert(f.call("other",op::guard_trigger,{id}).status==s::error::bad_handle);
    assert(f.call("signal",op::guard_trigger,{id}).status==s::error::success);
    assert(f.call("signal",op::guard_take,{id},7).status==s::error::too_large);
    auto take=encode({id});
    assert(f.broker.invoke(f.caller,op::guard_take,f.caps.at("signal"),take,8,required,[](s::bytes){return false;})==s::error::invalid);
    auto delivered=f.call("signal",op::guard_take,{id},8);assert(delivered.status==s::error::success&&decode(delivered.bytes)==1);
    assert(f.call("signal",op::guard_take,{id},8).status==s::error::again);
    assert(f.call("signal",op::resource_close,{id},0).status==s::error::success);
    assert(f.call("signal",op::guard_trigger,{id}).status==s::error::bad_handle);
    assert(f.call("signal",op::guard_create,{}).status==s::error::success);
}

void exact_dependencies()
{
    fixture f;
    f.add("clock",s::purpose::ros_clock,{"clock.create","clock.read","resource.observe","resource.close"});
    f.add("update",s::purpose::ros_clock,{"clock.update"});
    f.add("wrong-resource",s::purpose::ros_clock,{"clock.update"},"different-resource");
    f.add("wrong-binding",s::purpose::ros_clock,{"clock.update"},"robot-control","different-provider");
    f.add("no-observation",s::purpose::guard,{"guard.create","guard.trigger"});
    f.add("wait",s::purpose::wait,{"wait.create","wait.add","wait.poll","wait.remove","wait.acknowledge"});
    f.add("timer",s::purpose::timer,{"timer.create","timer.reset","timer.take","resource.observe"});
    f.start();
    assert(f.call("clock",op::clock_create,{0}).status==s::error::denied);
    auto clock=f.call("clock",op::clock_create,{2});assert(clock.status==s::error::success);auto id=decode(clock.bytes);
    auto read=f.call("clock",op::clock_read,{id});assert(read.status==s::error::success&&decode(read.bytes,2)==0);
    assert(f.call("wrong-resource",op::clock_update,{f.caps.at("clock"),id,100,1}).status==s::error::denied);
    assert(f.call("wrong-binding",op::clock_update,{f.caps.at("clock"),id,100,1}).status==s::error::denied);
    assert(f.call("update",op::clock_update,{f.caps.at("update"),id,100,1}).status==s::error::denied);
    assert(f.call("update",op::clock_update,{f.caps.at("clock"),id,100,2}).status==s::error::denied);
    assert(f.call("update",op::clock_update,{f.caps.at("clock"),id,100,1}).status==s::error::success);
    read=f.call("clock",op::clock_read,{id});assert(decode(read.bytes)==100&&decode(read.bytes,2)==1);
    auto wait=f.call("wait",op::wait_set_create,{});assert(wait.status==s::error::success);auto set=decode(wait.bytes);
    auto guard=f.call("no-observation",op::guard_create,{});assert(guard.status==s::error::success);
    assert(f.call("wait",op::wait_set_add,{set,f.caps.at("no-observation"),decode(guard.bytes),r::readable,42}).status==s::error::denied);
    assert(f.call("wait",op::wait_set_add,{set,f.caps.at("clock"),id,r::changed|r::closed,77}).status==s::error::success);
    assert(f.call("timer",op::timer_create,{f.caps.at("no-observation"),id}).status==s::error::denied);
    auto timer=f.call("timer",op::timer_create,{f.caps.at("clock"),id});assert(timer.status==s::error::success);auto tick=decode(timer.bytes);
    assert(f.call("timer",op::timer_reset,{tick,110,10}).status==s::error::success);
    assert(f.call("update",op::clock_update,{f.caps.at("clock"),id,135,1}).status==s::error::success);
    auto observation=f.call("wait",op::wait_set_wait,{set,0});assert(observation.status==s::error::success&&decode(observation.bytes)==id&&decode(observation.bytes,1)==77);
    auto delivered=f.call("timer",op::timer_take,{tick});assert(delivered.status==s::error::success&&decode(delivered.bytes,2)==3);
    assert(f.call("wait",op::wait_set_acknowledge,{set,id,decode(observation.bytes,3)}).status==s::error::success);
    assert(f.call("wait",op::wait_set_wait,{set,0}).status==s::error::timeout);
    assert(f.call("wait",op::wait_set_wait,{set,UINT64_MAX}).status==s::error::denied);
    assert(f.broker.revoke("clock")==s::error::success);
    assert(f.call("timer",op::timer_take,{tick}).status==s::error::bad_handle);
    observation=f.call("wait",op::wait_set_wait,{set,0});assert(observation.status==s::error::success&&decode(observation.bytes,2)==r::closed);
}

void bounded_input_and_budget()
{
    fixture f;s::grant grant;grant.alias="signal";grant.resource="signal";grant.binding="local";grant.kind=s::purpose::guard;
    grant.actions={"guard.create"};grant.limits.calls=1;grant.limits.bytes=8;grant.limits.uncertain=true;grant.limits.window=1h;
    assert(f.broker.permissions.configure(grant)==s::error::success);f.caps.emplace("signal",0);f.start();
    assert(f.call("signal",op::guard_create,{1}).status==s::error::invalid);
    assert(f.call("signal",op::guard_create,{},s::transfer_limit+1).status==s::error::invalid);
    assert(f.call("signal",op::guard_create,{},7).status==s::error::too_large);
    assert(f.call("signal",op::guard_create,{},8).status==s::error::success);
    assert(f.call("signal",op::guard_create,{},8).status==s::error::again);
    assert(f.call("signal",static_cast<op>(255),{},8).status==s::error::unsupported);
    int other{};std::size_t required{};bool delivered{};
    assert(f.broker.invoke({&other,1},op::guard_create,f.caps.at("signal"),{},8,required,[&](s::bytes){delivered=true;return true;})==s::error::denied&&!delivered);
    assert(f.broker.permissions.close(f.caller,f.caps.at("signal"))==s::error::success);
    assert(f.broker.permissions.open(f.caller,"signal",f.caps.at("signal"))==s::error::success);
    assert(f.call("signal",op::guard_create,{},8).status==s::error::again);
}

void wait_revalidation()
{
    fixture f;
    f.add("signal",s::purpose::guard,{"guard.create","guard.trigger","guard.take","resource.observe"});
    f.add("wait",s::purpose::wait,{"wait.create","wait.add","wait.poll"});f.start();
    auto signal=f.call("signal",op::guard_create,{}),wait=f.call("wait",op::wait_set_create,{});
    assert(signal.status==s::error::success&&wait.status==s::error::success);
    auto source=decode(signal.bytes),set=decode(wait.bytes);
    assert(f.call("wait",op::wait_set_add,{set,f.caps.at("signal"),source,r::readable|r::closed,99}).status==s::error::success);
    assert(f.call("signal",op::guard_trigger,{source}).status==s::error::success);
    // Positive-duration waits release authority and must revalidate even when
    // immediately ready. The trusted cancellation hook simulates revocation
    // between the first observation and the final delivery.
    auto input=encode({set,1});std::size_t required{};unsigned checks{};std::vector<std::uint8_t> output;
    auto result=f.broker.invoke(f.caller,op::wait_set_wait,f.caps.at("wait"),input,32,required,[&](s::bytes bytes)
    {output.assign(bytes.begin(),bytes.end());return true;},[&]
    {
        if(++checks==2)assert(f.broker.revoke("signal")==s::error::success);
        return false;
    });
    assert(result==s::error::success&&checks==2&&decode(output,2)==r::closed); // Fresh scan excludes stale readable data.
    auto pending_input=encode({set,5000000000ULL});
    // Cancellation and source closure do not require any guest callbacks.
    result=f.broker.invoke(f.caller,op::wait_set_wait,f.caps.at("wait"),pending_input,32,required,[](s::bytes){assert(false);return true;},[]{return true;});
    assert(result==s::error::cancelled);
    assert(f.broker.revoke("wait")==s::error::success);
    assert(f.call("wait",op::wait_set_wait,{set,0}).status==s::error::denied);
}

void nonblocking_poll_holds_authority()
{
    fixture f;
    f.add("signal",s::purpose::guard,{"guard.create","guard.trigger","guard.take","resource.observe"});
    f.add("wait",s::purpose::wait,{"wait.create","wait.add","wait.poll"});f.start();
    auto signal=f.call("signal",op::guard_create,{}),wait=f.call("wait",op::wait_set_create,{});
    auto source=decode(signal.bytes),set=decode(wait.bytes);
    assert(f.call("wait",op::wait_set_add,{set,f.caps.at("signal"),source,r::readable|r::closed,99}).status==s::error::success);
    assert(f.call("signal",op::guard_trigger,{source}).status==s::error::success);
    auto input=encode({set,0});std::size_t required{};
    assert(f.broker.invoke(f.caller,op::wait_set_wait,f.caps.at("wait"),input,32,required,
        [](s::bytes){assert(false);return true;},[]{return true;})==s::error::cancelled);
    assert(f.call("wait",op::wait_set_wait,{set,0},31).status==s::error::too_large);
    std::promise<void> entered;auto started=entered.get_future();std::future<s::error> revoke;std::atomic<bool> completed{};
    assert(f.broker.invoke(f.caller,op::wait_set_wait,f.caps.at("wait"),input,32,required,[&](s::bytes bytes)
    {
        assert(decode(bytes,2)==r::readable);
        revoke=std::async(std::launch::async,[&]
        {
            entered.set_value();auto result=f.broker.revoke("signal");completed=true;return result;
        });
        started.get();assert(!completed.load());return true;
    })==s::error::success);
    assert(revoke.get()==s::error::success&&completed.load());
    auto closed=f.call("wait",op::wait_set_wait,{set,0});assert(closed.status==s::error::success&&decode(closed.bytes,2)==r::closed);
}

int main()
{
    transactional_and_owned();exact_dependencies();bounded_input_and_budget();wait_revalidation();nonblocking_poll_holds_authority();
    std::cout<<"PASS: broker transactional outputs, exact source/resource/binding dependencies, bounded input, quota accounting, cancellation and post-wait revocation revalidation\n";
}
