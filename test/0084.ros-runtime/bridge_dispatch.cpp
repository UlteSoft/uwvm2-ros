/** @file @brief Real broker queues and canonical bridge execution with controlled native outcome faults. */
#define UWVM_COMMUNICATION_DISPATCH_FIXTURE_ONLY
#include "communication_dispatch.cpp"
#include <uwvm2/imported/ros/scene/bridge_dispatch.h>
namespace robot=s::robot_contract;
using bop=robot::operation;
static s::buffers::profile bridge_profile(std::string_view name)
{s::buffers::profile result;result.identity.assign(name);result.sha256.fill(17);return result;}
static s::bridge_dispatch::plan bridge_plan()
{
    s::bridge_dispatch::plan p;p.alias.assign("bridge");p.source_alias.assign("source");p.destination_alias.assign("destination");
    p.selected=bridge_profile("route");p.stop=bridge_profile("release-or-drain");p.source_profile=bridge_profile("source");p.destination_profile=bridge_profile("destination");
    p.codec.assign("std-bool");p.message_bytes=64;p.deadline_ns=1000000000;
    assert(s::bridge_dispatch::required_storage(p,p.storage_bytes));return p;
}
struct bridge_fixture
{
    std::shared_ptr<s::authority> permissions=uwvm2::utils::container::make_shared<s::authority>();
    s::communication_dispatch topics{permissions,111,3,limits()};std::shared_ptr<backend> native=uwvm2::utils::container::make_shared<backend>();
    s::bridge_dispatch::plan policy=bridge_plan();
    s::bridge_dispatch bridge{permissions,s::bridge_dispatch::transport::from(topics),71,{1,policy.storage_bytes,262144,1000000}};
    int instance{};s::caller who{&instance,1};s::handle cap{},context_cap{},node_cap{},type_cap{},source_cap{},destination_cap{},context_id{},node_id{},source{},destination{},id{};
    explicit bridge_fixture(std::string_view omitted={},std::optional<std::uint64_t> bridge_calls={},
                            uwvm2::utils::container::function_ref<void(s::authority&)> configure={})
    {
        for(auto name:{"capability.open","resource.close"})assert(permissions->expose(name,s::exposure::allow)==error::success);
        for(auto const& entry:contract::operations)assert(permissions->expose(text(entry.name),s::exposure::allow)==error::success);
        for(auto const& entry:uwvm2::imported::ros::communication_receive_contract::operations)assert(permissions->expose(text(entry.name),s::exposure::allow)==error::success);
        for(auto name:{"bridge.create","bridge.start","bridge.stop","bridge.describe"})assert(permissions->expose(name,s::exposure::allow)==error::success);
        auto grant=[&](std::string_view alias,s::purpose kind,std::initializer_list<char const*> actions)
        {
            s::grant g;g.alias=alias;g.resource=alias;g.binding=kind==s::purpose::bridge?"topic-forwarder":"native-communication";g.kind=kind;g.max_resources=1;
            for(auto name:actions){if(name!=omitted)g.actions.emplace(name);}g.actions.emplace("resource.close");
            if(kind==s::purpose::bridge&&bridge_calls){g.limits.calls=bridge_calls;g.limits.uncertain=true;g.limits.window=std::chrono::hours(1);}
            assert(permissions->configure(g)==error::success);
        };
        grant("context",s::purpose::context,{"context.open"});grant("node",s::purpose::node,{"node.open"});grant("type",s::purpose::type,{"type.describe"});
        grant("source",s::purpose::topic,{"topic.open","topic.describe","topic.reserve","topic.acknowledge","topic.release","data.export"});
        grant("destination",s::purpose::topic,{"topic.open","topic.describe","topic.publish"});
        grant("bridge",s::purpose::bridge,{"bridge.create","bridge.start","bridge.stop","bridge.describe"});
        if(configure)configure(*permissions);
        assert(topics.bind("context",c::context_plan{{},profile("context"),0,{},c::transport::unauthenticated_loopback})==error::success);
        assert(topics.bind("node",c::node_plan{{},profile("node"),profile("context"),"robot","/scene",2})==error::success);
        c::type_plan schema;schema.selected=profile("type");schema.type_identity="std_msgs/msg/Bool";schema.encoding="scene-canonical";schema.descriptor={1};schema.validate=c::codecs::boolean::validate;
        assert(topics.bind("type",schema)==error::success);
        assert(topics.bind("source",c::topic_plan{{},profile("source"),profile("node"),profile("type"),"/source",c::role::subscription,quality(),64,2,128,c::overflow::reject,1000000000})==error::success);
        assert(topics.bind("destination",c::topic_plan{{},profile("destination"),profile("node"),profile("type"),"/destination",c::role::publisher,quality(),64,0,0,c::overflow::reject,1000000000})==error::success);
        assert(topics.prepare(native)==error::success&&permissions->enroll(who)==error::success);
        for(auto [alias,cap]:{std::pair{"context",&context_cap},{"node",&node_cap},{"type",&type_cap},{"source",&source_cap},{"destination",&destination_cap},{"bridge",&this->cap}})
            assert(permissions->open(who,alias,*cap)==error::success);
        context_id=open(contract::operation::context_open,context_cap,encode([&](auto& out){return contract::encode_profile(out,profile("context"));}));
        node_id=open(contract::operation::node_open,node_cap,encode([&](auto& out){return out.u64(context_cap)&&out.u64(context_id)&&contract::encode_profile(out,profile("node"));}));
        for(bool target:{false,true})
        {
            auto data=encode([&](auto& out){return out.u64(node_cap)&&out.u64(node_id)&&out.u64(1)&&out.u64(type_cap)&&contract::encode_profile(out,profile(target?"destination":"source"))&&
                out.text(target?"/destination":"/source")&&out.text("std_msgs/msg/Bool")&&out.text(target?"publisher":"subscription");});
            (target?destination:source)=open(contract::operation::topic_open,target?destination_cap:source_cap,data);
        }
        native->subscriber->evidence.provider_generation=71;native->publisher->evidence.provider_generation=71;
        assert(bridge.bind(policy)==error::success&&bridge.prepare()==error::success);
    }
    s::handle open(contract::operation action,s::handle cap,w::bytes data)
    {std::size_t required{};s::handle id{};assert(topics.invoke(who,action,cap,data,4096,required,[&](auto bytes){w::reader in{bytes};return in.u64(id)&&in.done();})==error::success);return id;}
    bytes create_wire(){return encode([&](auto& out){return s::proposals::write_profile(out,policy.selected)&&out.u64(1)&&out.u64(1)&&out.u64(source_cap)&&out.u64(source)&&out.u64(destination_cap)&&out.u64(destination);});}
    bytes key(bool stop=false,std::uint64_t sequence=1)
    {return encode([&](auto& out){return out.u64(id)&&out.u64(sequence)&&out.u64(id)&&(!stop||s::proposals::write_profile(out,policy.stop))&&out.u64(c::now()+900000000);});}
    error call(bop action,w::bytes input,bytes& out,std::size_t capacity=2048,s::bridge_dispatch::sink custom={})
    {std::size_t required{};auto copy=[&](auto bytes){out.assign(bytes.begin(),bytes.end());return true;};return bridge.invoke(who,action,cap,input,capacity,required,custom?custom:s::bridge_dispatch::sink(copy));}
    void create(){bytes out;assert(call(bop::bridge_create,create_wire(),out)==error::success);w::reader in{out};assert(in.u64(id)&&id&&in.done());}
    void start(){bytes out;assert(call(bop::bridge_start,key(),out)==error::success);}
    void push(){c::metadata meta;meta.source=source;meta.generation=3;std::array<std::uint8_t,1> value{1};assert(native->queue->push(meta,value)==c::error::ok);}
    s::bridge_dispatch::inspection usage(){s::bridge_dispatch::inspection out;assert(bridge.inspect("bridge",out)==error::success);return out;}
};
static void forwarding_and_receipt_recovery()
{
    bridge_fixture f;f.create();bytes out;auto start=f.key();
    assert(f.call(bop::bridge_start,start,out,255)==error::too_large&&!f.usage().active);
    assert(f.call(bop::bridge_start,start,out,2048,[](auto){return false;})==error::io&&f.usage().active);
    assert(f.call(bop::bridge_start,start,out)==error::success);auto initial=out;
    assert(f.call(bop::bridge_start,start,out)==error::success&&out==initial);f.push();
    assert(f.bridge.pump("bridge")==error::success&&f.usage().pending==1&&!f.native->publisher->publications);
    assert(f.native->queue->has_reservation());
    assert(f.bridge.pump("bridge")==error::success&&f.native->publisher->publications==1&&f.native->queue->has_reservation());
    assert(f.bridge.pump("bridge")==error::success&&f.usage().forwarded==1&&!f.usage().pending&&!f.native->queue->has_reservation());
    auto stop=f.key(true);assert(f.call(bop::bridge_stop,stop,out,2048,[](auto){return false;})==error::io&&!f.usage().active);
    assert(f.call(bop::bridge_stop,stop,out)==error::success);
    assert(f.call(bop::bridge_describe,resource(f.id),out)==error::success);robot::response parsed;
    assert(robot::decode_response(bop::bridge_describe,out,{2048,2048,255,16},parsed)&&parsed.execution.state=="stopped"&&!parsed.execution.pending);
    assert(f.bridge.close(f.who,f.cap,f.id)==error::success);
}
static void stop_cancels_before_target_and_drains_after_target()
{
    for(bool target:{false,true})
    {bridge_fixture f;f.create();f.start();f.push();bytes out;
        assert(f.bridge.pump("bridge")==error::success);if(target)assert(f.bridge.pump("bridge")==error::success);
        auto stopped=f.key(true);assert(f.call(bop::bridge_stop,stopped,out)==error::success);
        assert(f.bridge.pump("bridge")==error::success&&!f.usage().pending&&!f.usage().active&&!f.native->queue->has_reservation());
        assert(f.native->publisher->publications==unsigned(target)&&f.usage().forwarded==unsigned(target));
        assert(f.call(bop::bridge_stop,stopped,out)==error::success);}
}
static void unknown_and_revocation_never_acknowledge()
{
    bridge_fixture f;f.create();f.start();f.push();f.native->publisher->unknown=true;
    assert(f.bridge.pump("bridge")==error::success);assert(f.bridge.pump("bridge")==error::io);
    assert(f.usage().unknown==1&&f.usage().pending==1&&!f.usage().forwarded&&f.native->queue->has_reservation());
    for(unsigned i=0;i<5;++i){(void)f.bridge.pump("bridge");}assert(f.native->publisher->publications==1);
    bytes out;assert(f.call(bop::bridge_stop,f.key(true),out)==error::success);robot::response parsed;
    assert(robot::decode_response(bop::bridge_stop,out,{2048,2048,255,4},parsed)&&parsed.outcome.state=="outcome_unknown");
    assert(f.bridge.close(f.who,f.cap,f.id)==error::again);
    bridge_fixture revoked;revoked.create();revoked.start();revoked.push();assert(revoked.bridge.pump("bridge")==error::success);
    assert(revoked.permissions->revoke("destination",[&](auto name){revoked.bridge.revoke(name);})==error::success);
    assert(revoked.usage().retired&&revoked.usage().unknown==1&&!revoked.native->publisher->publications&&revoked.native->queue->has_reservation());
}
static void independent_authority_and_reentrant_callbacks()
{
    for(auto action:{"bridge.create","topic.reserve","topic.acknowledge","topic.release","data.export","topic.publish","topic.describe"})
    {bridge_fixture f{action};bytes out;assert(f.call(bop::bridge_create,f.create_wire(),out)==error::denied&&!f.usage().handles);}
    {bridge_fixture zero{{},0};bytes out;assert(zero.call(bop::bridge_create,zero.create_wire(),out)==error::again&&!zero.usage().handles);}
    {bridge_fixture limited{{},2};limited.create();limited.start();bytes out;assert(limited.call(bop::bridge_stop,limited.key(true),out)==error::success);}
    bridge_fixture f;bytes out;assert(f.call(bop::bridge_create,f.create_wire(),out,2048,[&](auto)
        {assert(f.permissions->revoke("source",[&](auto alias){f.bridge.revoke(alias);})==error::success);return true;})==error::denied&&!f.usage().handles);
}
static void empty_receive_has_no_cleanup_debt()
{
    bridge_fixture f;f.create();f.start();
    assert(f.bridge.pump("bridge")==error::again&&!f.usage().pending);
    bool empty{};
    assert(f.topics.topic_receive_quiescent(f.who,f.source_cap,f.source,empty)==error::success&&empty);
    bytes out;assert(f.call(bop::bridge_stop,f.key(true),out)==error::success);
    robot::response decoded;
    assert(robot::decode_response(bop::bridge_stop,out,{2048,2048,255,4},decoded)&&decoded.outcome.state=="succeeded");
    assert(f.bridge.close(f.who,f.cap,f.id)==error::success);
    bridge_fixture held;held.create();held.start();held.push();assert(held.bridge.pump("bridge")==error::success);
    assert(held.topics.topic_receive_quiescent(held.who,held.source_cap,held.source,empty)==error::success&&!empty);
    assert(held.topics.topic_receive_quiescent(held.who,held.destination_cap,held.source,empty)==error::denied&&!empty);
    assert(held.permissions->revoke("source",[](auto){})==error::success);
    assert(held.topics.topic_receive_quiescent(held.who,held.source_cap,held.source,empty)==error::denied&&!empty);
}
static void actual_supervisor_and_fork_fence()
{
    bridge_fixture f;f.create();f.push();f.start();assert(f.bridge.start_supervisor()==error::success);
    auto until=c::now()+2000000000;
    while(f.native->publisher->publications.load()!=1&&c::now()<until)::usleep(1000);
    assert(f.native->publisher->publications==1);f.bridge.request_stop();f.bridge.retire();assert(f.usage().retired);
    auto child=::fork();assert(child>=0);if(!child)::_exit(f.bridge.pump("bridge")==error::denied?0:1);
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
}
int main()
{forwarding_and_receipt_recovery();stop_cancels_before_target_and_drains_after_target();unknown_and_revocation_never_acknowledge();
 independent_authority_and_reentrant_callbacks();empty_receive_has_no_cleanup_debt();actual_supervisor_and_fork_fence();std::puts("passed 6 bridge execution safety groups");return 0;}
