/** @file @brief Root broker routing and independent communication cleanup. */
#define UWVM_COMMUNICATION_NODE_OPEN_FIXTURE_ONLY
#include "communication_node_open.cpp"
#include <uwvm2/imported/ros/scene/broker.h>
static void hosted_node()
{
    s::broker broker;auto& authority=broker.permissions;
    s::communication_host::limits capacity{{{1,1,0,0,0,256},2,4096,4096,8192,4,10000000000},{1,1040,2}};
    assert(broker.configure_communication_dispatch(0,capacity)==s::error::invalid);
    assert(broker.configure_communication_dispatch(19,capacity)==s::error::success);
    assert(broker.configure_communication_dispatch(19,capacity)==s::error::conflict);
    auto& host=*broker.communication_store;auto dispatcher=host.dispatcher();
    auto native=uwvm2::utils::container::make_shared<backend>();auto worker=uwvm2::utils::container::make_shared<lifetime>();
    auto source=uwvm2::utils::container::make_shared<s::communication_node_open_provider>();
    int instance{};s::caller who{&instance,1};s::handle context_cap{},cap{},context{},request{},node{};
    std::size_t required{};auto delivered=[](w::bytes){return true;};
    using operation=s::communication_host::operation;using control=s::communication_host::creation_operation;
    assert(broker.invoke_communication(who,operation::node_open,0,{},48,required,delivered)==s::error::denied&&host.pump()==s::error::denied);
    assert(broker.close_capability(who,cap)==s::error::denied);
    for(auto name:{"capability.open","capability.close","context.open","node.open","resource.close","communication.open.take","communication.open.cancel","request.describe","request.release"})
        assert(authority.expose(name,s::exposure::allow)==s::error::success);
    for(bool node:{false,true})
    {
        s::grant grant;grant.alias=node?"node":"context";grant.resource=grant.alias;grant.binding="native-communication";
        grant.kind=node?s::purpose::node:s::purpose::context;grant.max_resources=1;
        grant.actions.emplace(node?"node.open":"context.open");grant.actions.emplace("resource.close");
        if(node)for(auto name:{"communication.open.take","communication.open.cancel","request.describe","request.release"})grant.actions.emplace(name);
        assert(authority.configure(std::move(grant))==s::error::success);
    }
    assert(dispatcher->bind("context",c::context_plan{{},profile("context"),0,{},c::transport::unauthenticated_loopback})==s::error::success);
    assert(dispatcher->bind("node",c::node_plan{{},profile("node"),profile("context"),"robot","/scene",2})==s::error::success);
    c::node_plan plan;assert(dispatcher->describe_plan("node",plan)==s::error::success);
    assert(source->configure("node",{plan.authority,plan.selected,opens::endpoint_kind::node,1,71,256,10000000000},dispatcher,worker)==c::error::ok);
    assert(host.bind("missing",opens::endpoint_kind::node,source)==s::error::bad_handle);
    assert(host.bind("node",opens::endpoint_kind::node,source)==s::error::success);
    assert(host.prepare(native)==s::error::success&&native->stage_calls==0);
    assert(host.bind("node",opens::endpoint_kind::node,source)==s::error::denied);
    assert(authority.enroll(who)==s::error::success&&authority.open(who,"context",context_cap)==s::error::success&&authority.open(who,"node",cap)==s::error::success);
    auto context_body=encode([](auto& out){return uwvm2::imported::ros::communication_contract::encode_profile(out,profile("context"));});
    assert(broker.invoke_communication(who,operation::context_open,context_cap,context_body,8,required,[&](w::bytes value){w::reader in{value};return in.u64(context)&&context&&in.done();})==s::error::success);
    auto body=encode([&](auto& out){return out.u64(context_cap)&&out.u64(context)&&uwvm2::imported::ros::communication_contract::encode_profile(out,plan.selected)&&out.u64(1)&&out.u64(c::now()+9000000000);});
    assert(host.preflight(who,operation::node_open,context_cap,body.size())==s::error::denied);
    assert(host.preflight(who,operation::node_open,cap,body.size())==s::error::success);
    auto open=[&](bool copied,std::size_t bytes=48){return broker.invoke_communication(who,operation::node_open,cap,body,bytes,required,[&](w::bytes value)
        {w::reader in{value};std::uint64_t key{};assert(in.u64(key)&&key==1&&in.u64(request)&&request);return copied;});};
    assert(open(true,47)==s::error::too_large&&!request&&!native->stage_calls);
    assert(open(false)==s::error::io&&open(true)==s::error::success&&request&&!native->stage_calls);
    auto request_body=encode([&](auto& out){return out.u64(request);});
    assert(broker.invoke_model(who,uwvm2::imported::ros::model_contract::operation::request_describe,cap,request_body,512,required,delivered)==s::error::success&&!native->stage_calls);
    assert(broker.invoke_communication_creation(who,control::take,cap,request_body,64,required,delivered)==s::error::again&&!native->stage_calls);
    assert(host.pump()==s::error::success&&native->node_creations==1);
    auto take=[&](bool copied){return broker.invoke_communication_creation(who,control::take,cap,request_body,64,required,[&](w::bytes value)
        {w::reader in{value};std::uint64_t field{};for(unsigned n{};n<7;++n)assert(in.u64(field));assert(field==1&&in.u64(node)&&node&&node!=request&&in.done());return copied;});};
    assert(take(false)==s::error::io&&take(true)==s::error::success);
    auto close_body=encode([&](auto& out){return out.u64(node);});
    assert(broker.invoke(who,uwvm2::imported::ros::runtime_contract::operation::resource_close,cap,close_body,0,required,[](w::bytes){return false;})==s::error::invalid);
    assert(broker.invoke(who,uwvm2::imported::ros::runtime_contract::operation::resource_close,cap,close_body,0,required,delivered)==s::error::success);
    worker->retire();assert(source->reconcile_cleanup()==c::error::ok);
    assert(broker.preflight_model_or_request(who,uwvm2::imported::ros::model_contract::operation::request_release,cap,8)==s::error::success);
    auto release=[&](bool copied){return broker.invoke_model(who,uwvm2::imported::ros::model_contract::operation::request_release,cap,request_body,8,required,[&](w::bytes){return copied;});};
    assert(release(false)==s::error::io&&release(true)==s::error::success&&release(true)==s::error::success);
    assert(authority.expose("capability.close",s::exposure::deny)==s::error::invalid);
    assert(broker.close_capability(who,cap)==s::error::success&&broker.close_capability(who,cap)==s::error::bad_handle);
    broker.retire();assert(host.pump()==s::error::denied&&host.preflight(who,operation::node_open,cap,body.size())==s::error::denied);
}
int main(){hosted_node();std::puts("PASS canonical communication broker: finite exact routing, default denial, inert opens, explicit progress, typed publication and release replay");}
