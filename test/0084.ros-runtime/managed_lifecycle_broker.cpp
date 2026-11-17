/** @file
 * @brief Exercise local lifecycle through real scene authority and deployment.
 * @details Covers each exact import/action gate, ownership, copy failure,
 * reentrant revocation and finite deployment configuration without native ROS.
 */
#include <uwvm2/imported/ros/scene/deployment.h>
#include <cassert>
#include <iostream>

namespace s=uwvm2::imported::ros::scene;
namespace m=s::managed_lifecycle;
namespace contract=uwvm2::imported::ros::managed_lifecycle_contract;
using op=contract::operation;
using error=s::error;
static s::grant policy(std::string alias="node")
{
    s::grant value;value.alias=std::move(alias);value.resource="logical-node";value.binding="local-lifecycle";value.kind=s::purpose::lifecycle;
    for(auto const& action:contract::operations)value.actions.emplace(action.name);
    value.actions.emplace("resource.close");return value;
}
static std::vector<std::uint8_t> encode(std::initializer_list<std::uint64_t> words)
{std::vector<std::uint8_t> bytes(words.size()*8);std::size_t index{};for(auto word:words)s::put(bytes,index++,word);return bytes;}
static std::uint64_t deadline(){return static_cast<std::uint64_t>(uwvm2::imported::ros::resources::steady_now())+1000000000;}
struct reply {error status;std::size_t required{};std::vector<std::uint8_t> bytes;};
struct fixture
{
    s::broker broker;int identity{};s::caller who{&identity,1};s::handle cap{},client{},server{};
    fixture(std::string_view omitted={},bool omit_exposure=false)
    {
        auto value=policy();if(!omitted.empty()&&!omit_exposure)value.actions.erase(std::string(omitted));
        assert(broker.permissions.expose("capability.open",s::exposure::allow)==error::success);
        assert(broker.permissions.expose("resource.close",s::exposure::allow)==error::success);
        for(auto const& action:contract::operations)
            if(action.name!=omitted||!omit_exposure)assert(broker.permissions.expose(std::string(action.name),s::exposure::allow)==error::success);
        assert(broker.permissions.configure(value)==error::success);
        assert(broker.managed_nodes.bind("node",{4,1000000000,m::all_transitions,3})==error::success);
        assert(broker.permissions.enroll(who)==error::success&&broker.permissions.open(who,"node",cap)==error::success);
        // Trusted fixture provisioning is separate from the action under test.
        assert(broker.managed_nodes.open("node",m::client,4,client)==error::success);
        assert(broker.managed_nodes.open("node",m::server,4,server)==error::success);
    }
    reply call(op action,std::vector<std::uint8_t> const& input,std::size_t capacity=56,bool copy=true)
    {
        reply value;value.status=broker.invoke_managed(who,action,cap,input,capacity,value.required,[&](s::bytes output)
        {if(!copy)return false;value.bytes.assign(output.begin(),output.end());return true;});return value;
    }
    std::vector<std::uint8_t> input(op action)
    {
        switch(action)
        {
        case op::open:return encode({m::client});
        case op::state:case op::transitions:return encode({client});
        case op::submit:return encode({client,1,1,1,deadline()});
        case op::take:return encode({server});
        case op::respond:return encode({server,1,2,97});
        }
        return {};
    }
    m::snapshot sample(){m::snapshot value;assert(broker.managed_nodes.state("node",client,value)==error::success);return value;}
};
static void every_action_gate()
{
    for(auto const& description:contract::operations)
    {
        for(bool exposure:{false,true})
        {
            fixture denied(description.name,exposure);auto request=denied.input(description.id);
            auto result=denied.call(description.id,request);assert(result.status==error::denied&&!result.required&&result.bytes.empty());
            std::vector<s::authority_snapshot> accounts;assert(denied.broker.permissions.inspect(accounts,1)&&!accounts[0].admissions);
            assert(denied.sample().state==1);
        }
        fixture allowed;auto request=allowed.input(description.id);auto before=allowed.sample();
        auto short_result=allowed.call(description.id,request,description.bytes-1);
        assert(short_result.status==error::too_large&&short_result.required==description.bytes&&short_result.bytes.empty());
        assert(allowed.sample()==before);
        std::vector<s::authority_snapshot> accounts;assert(allowed.broker.permissions.inspect(accounts,1)&&!accounts[0].admissions);
        std::size_t required=99;int foreign{};
        assert(allowed.broker.invoke_managed({&foreign,1},description.id,allowed.cap,request,56,required,[](s::bytes){assert(false);return true;})==error::denied&&!required);
        fixture peer;
        assert(peer.broker.invoke_managed(peer.who,description.id,allowed.cap,request,56,required,[](s::bytes){assert(false);return true;})==error::bad_handle);
    }
    fixture malformed;std::size_t required=88;
    assert(malformed.broker.invoke_managed(malformed.who,static_cast<op>(255),malformed.cap,{},56,required,[](s::bytes){return true;})==error::unsupported&&!required);
    assert(malformed.call(op::open,encode({1,2})).status==error::invalid);
}
static void delivery_and_replay()
{
    fixture test;auto open=encode({m::client});
    assert(test.call(op::open,open,8,false).status==error::invalid);
    std::vector<m::inspection> nodes;assert(test.broker.managed_nodes.inspect(nodes,1)&&nodes[0].handles==2);
    auto opened=test.call(op::open,open,8);assert(opened.status==error::success&&opened.bytes.size()==8);
    auto extra=s::word(opened.bytes,0);assert(extra);
    auto state=test.call(op::state,encode({test.client}),56);assert(state.status==error::success&&s::word(state.bytes,0)==1);
    auto transitions=test.call(op::transitions,encode({test.client}),8);assert(transitions.status==error::success&&s::word(transitions.bytes,0)==((1u<<1)|(1u<<5)));
    auto submit=test.input(op::submit);
    assert(test.call(op::submit,submit,40,false).status==error::invalid);
    auto replay=test.call(op::submit,submit,40);assert(replay.status==error::success&&s::word(replay.bytes,2)==2&&s::word(replay.bytes,3)==10);
    assert(test.call(op::take,encode({test.server}),40,false).status==error::invalid&&!test.sample().delivered);
    auto delivered=test.call(op::take,encode({test.server}),40);assert(delivered.status==error::success&&test.sample().delivered);
    auto response=encode({test.server,s::word(delivered.bytes,0),s::word(delivered.bytes,2),97});
    assert(test.call(op::respond,response,40,false).status==error::invalid);
    auto response_replay=test.call(op::respond,response,40);assert(response_replay.status==error::success&&s::word(response_replay.bytes,3)==2);
    assert(test.sample().state==2&&test.sample().generation==3);
    std::size_t required{};auto closed=encode({test.client});
    assert(test.broker.invoke(test.who,uwvm2::imported::ros::runtime_contract::operation::resource_close,test.cap,closed,0,required,[](s::bytes){return true;})==error::success);
    auto reopened=test.call(op::open,open,8);assert(reopened.status==error::success);test.client=s::word(reopened.bytes,0);
    assert(test.sample().state==2&&test.sample().generation==3);
    auto old=submit;s::put(old,0,test.client);
    assert(test.call(op::submit,old,40).status==error::success); // Alias history survives handle closure.
}
static void reentrant_revocation()
{
    fixture cancelled;assert(cancelled.call(op::submit,cancelled.input(op::submit),40).status==error::success);
    bool stop{};std::size_t copied{};auto pending=encode({cancelled.server});
    auto result=cancelled.broker.invoke_managed(cancelled.who,op::take,cancelled.cap,pending,40,copied,
        [&](s::bytes){stop=true;return true;},[&]{return stop;});
    assert(result!=error::success&&!cancelled.sample().delivered);
    stop=false;assert(cancelled.call(op::take,pending,40).status==error::success&&cancelled.sample().delivered);

    fixture test;assert(test.call(op::submit,test.input(op::submit),40).status==error::success);
    auto request=encode({test.server});std::size_t required{};unsigned calls{};
    assert(test.broker.invoke_managed(test.who,op::take,test.cap,request,40,required,[&](s::bytes output)
    {
        ++calls;assert(output.size()==40);
        assert(test.call(op::take,request,40).status==error::again);
        assert(test.broker.revoke("node")==error::success);return true;
    })==error::cancelled&&calls==1);
    std::vector<m::inspection> nodes;assert(test.broker.managed_nodes.inspect(nodes,1)&&nodes[0].retired&&nodes[0].current.state==4&&!nodes[0].current.delivered);
    assert(test.call(op::take,request,40).status==error::denied);
}
static void deployment_scope()
{
    s::deployment missing;assert(missing.create("robot")==error::success);
    assert(missing.bind("robot","main")==error::success&&missing.allow("robot",policy())==error::success);
    assert(missing.validate()==error::invalid);
    s::deployment deployment;assert(deployment.create("robot")==error::success&&deployment.bind("robot","main")==error::success);
    assert(deployment.managed_lifecycle_plan("robot","node",{})==error::denied);
    auto invalid=policy();invalid.kind=s::purpose::cache;assert(deployment.allow("robot",invalid)==error::invalid);
    invalid=policy();invalid.binding="native-lifecycle";assert(deployment.allow("robot",invalid)==error::unsupported);
    invalid=policy();invalid.actions={"resource.observe"};assert(deployment.allow("robot",invalid)==error::unsupported);
    assert(deployment.allow("robot",policy())==error::success);
    assert(deployment.managed_lifecycle_plan("robot","node",{1,UINT64_MAX,2,1})==error::invalid);
    assert(deployment.managed_lifecycle_plan("robot","node",{})==error::success);
    assert(deployment.managed_lifecycle_plan("robot","node",{})==error::conflict);
    assert(deployment.expose("robot","capability.open",s::exposure::allow)==error::success);
    assert(deployment.expose("robot","lifecycle.open",s::exposure::allow)==error::success);
    assert(deployment.validate()==error::success);
    int identity{};s::caller who{&identity,1};std::shared_ptr<s::broker> selected;
    assert(deployment.select("main",who,selected)==error::success);
    s::handle cap{};assert(selected->permissions.open(who,"node",cap)==error::success);
    auto request=encode({m::client});std::size_t required{};
    assert(selected->invoke_managed(who,op::open,cap,request,8,required,[](s::bytes){assert(false);return true;})==error::denied);
    assert(deployment.managed_lifecycle_plan("robot","node",{})==error::invalid);
}
int main()
{every_action_gate();delivery_and_replay();reentrant_revocation();deployment_scope();std::cout<<"passed 4 managed-lifecycle broker safety groups\n";}
