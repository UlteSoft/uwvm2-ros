/** @file @brief Real sealed worker deployment, distinct registries and independent supervisors. */
#define UWVM_COMMUNICATION_FACTORY_FIXTURE_ONLY
#include "communication_worker_provider.cpp"
#include <uwvm2/imported/ros/scene/communication_deployment_provider.h>
#include <uwvm2/imported/ros/scene/broker.h>
namespace d=s::communication_deployment_provider;
namespace contract=uwvm2::imported::ros::communication_contract;
namespace receive=uwvm2::imported::ros::communication_receive_contract;
namespace wire=s::wire;
using config=s::communication_deployment_configuration;
using text=uwvm2::utils::container::std_string;
template<class F>static bytes encode(F&& fill)
{std::array<std::uint8_t,4096> storage;wire::writer out{storage};assert(fill(out));return {out.written().begin(),out.written().end()};}
static std::uint64_t global_limit(std::string_view name)
{
    if(name=="actions"||name=="action-workspace-bytes"||name=="action-workspace-calls")
    {
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_ACTIONS)
        return name=="actions"?2:name=="action-workspace-calls"?16:8u*1024u*1024u;
#else
        return 0;
#endif
    }
    if(name=="graphs"||name=="event-bindings"||name.starts_with("graph-")||name.starts_with("event-"))
    {
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_OBSERVATIONS)
        return name=="graphs"||name=="event-bindings"?2:name.ends_with("calls")?32:8u*1024u*1024u;
#else
        return 0;
#endif
    }
    if(name=="workers"||name=="contexts"||name=="nodes")return 2;
    if(name=="pairs")return 1;
    if(name=="types"||name=="bindings"||name=="creation-bindings")return 32;
    if(name=="topics"||name=="services")return 4;
    if(name=="workspace-calls"||name=="creation-workspace-calls")return 16;
    if(name=="carrier-bytes")return 2048;
    if(name=="receipt-bytes"||name=="output-bytes")return 65536;
    if(name=="node-deadline-ns")return 15000000000ull;
    if(name=="provider-bytes")return 16ull<<30;
    if(name=="supervisor-poll-ns")return 1000000;
    return 8u*1024u*1024u;
}
struct member
{
    text context_alias,node_alias,topic_alias,service_alias;
    std::array<text,3> schemas;
    c::context_plan context_plan;c::node_plan node_plan;c::topic_plan topic;c::service_plan service;
    s::handle context_cap{},node_cap{},topic_cap{},service_cap{};
    std::array<s::handle,3> schema_caps{};
    s::handle context{},node{},topic_id{},service_id{},node_request{},topic_request{},service_request{};
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_ACTIONS)
    text action_alias;std::array<text,3> action_schema_aliases;std::array<s::handle,3> action_schema_caps{};
    s::communication_action_dispatch::plan action_plan;s::handle action_cap{},action_id{},action_request{};
    std::uint64_t action_sequence{};
#endif
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_OBSERVATIONS)
    text graph_alias,event_alias;s::communication_graph_dispatch::plan graph_plan;
    c::profile event_profile{"events",{7}};s::handle graph_cap{},event_cap{},watch{},stream{};
#endif
};
int main(int argc,char** argv)
{
    ::signal(SIGPIPE,SIG_IGN);s::broker broker;config settings;std::array<peer,2> peers;std::array<member,2> members;
    std::array<uwvm2::utils::container::std_unique_ptr<external_fixture::owner>,2> external;
    for(auto const& field:config::budget_fields)assert(settings.limit(field.name,global_limit(field.name))==s::error::success);
    assert(settings.limit("workers",2)==s::error::conflict&&settings.limit("unknown",1)==s::error::invalid);
    int at=1;using family=s::communication_configuration::family;
    for(std::size_t i{};i<2;++i)
    {
        auto& peer=peers[i];auto& member=members[i];auto start=at;peer.parse(argc,argv,at);text name="worker."+uwvm2::utils::container::to_string(i);
        std::size_t profile{};for(std::size_t n{};n<peer.manifest.files.size();++n)if(peer.manifest.files[n].destination.view()=="/worker/profile")profile=n;
        auto const& image=peer.manifest.files[profile];
        assert(settings.load_worker(name,"startup","/opt/ros/jazzy",argv[start+7+profile*4+2],image.bytes,image.sha256)==s::error::success);
        assert(settings.load_worker(name,"startup","/opt/ros/jazzy","/missing",image.bytes,image.sha256)==s::error::conflict);
        peer.manifest.configuration.identity.assign("startup");
        external[i]=uwvm2::utils::container::make_unique<external_fixture::owner>(peer.manifest);
        assert(settings.worker_setting(name,[&](auto& attachment){return attachment.channel(external[i]->attachment.endpoints());})==s::error::success);
        for(auto const& field:config::peer_fields)
        {auto value=(field.name.starts_with("action-")||field.name=="graph-query-bytes")?0ull:field.name=="rpc-deadline-ns"?5000000000ull:field.name=="rpc-poll-ns"?1000000ull:field.name=="rpc-stderr-bytes"?65536ull:4ull*1024u*1024u;
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_ACTIONS)
            if(field.name.starts_with("action-"))value=4u*1024u*1024u;
#endif
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_OBSERVATIONS)
            if(field.name=="graph-query-bytes")value=4u*1024u*1024u;
#endif
            assert(settings.worker_limit(name,field.name,value)==s::error::success);}
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_ACTIONS)
        for(auto const& item:peer.definition.definitions)if(item.kind==family::action&&cfg::profile_of(item).identity==(i?"action-server":"action-client"))
            member.action_plan=std::get<s::communication_action_dispatch::plan>(item.policy);
#endif
        for(auto const& item:peer.definition.definitions)
        {
            auto kind=item.kind;auto const& selected=cfg::profile_of(item);
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_ACTIONS)
            if(kind>family::action)continue;
            if(kind==family::action&&selected.identity!=(i?"action-server":"action-client"))continue;
#elif defined(UWVM_COMMUNICATION_DEPLOYMENT_OBSERVATIONS)
            if(kind==family::action||kind>family::graph)continue;
#else
            if(kind>family::service)continue;
#endif
            if(kind==family::topic&&selected.identity!=(i?"subscription":"publisher"))continue;
            if(kind==family::service&&selected.identity!=(i?"service-server":"service-client"))continue;
            s::grant grant;grant.alias=name+'.'+selected.identity;grant.resource=grant.alias;grant.binding="native-communication";grant.max_resources=1;
            if(kind==family::context){grant.kind=s::purpose::context;member.context_alias=grant.alias;member.context_plan=std::get<c::context_plan>(item.policy);}
            if(kind==family::node){grant.kind=s::purpose::node;member.node_alias=grant.alias;member.node_plan=std::get<c::node_plan>(item.policy);}
            if(kind==family::topic){grant.kind=s::purpose::topic;member.topic_alias=grant.alias;member.topic=std::get<c::topic_plan>(item.policy);}
            if(kind==family::service){grant.kind=s::purpose::service;member.service_alias=grant.alias;member.service=std::get<c::service_plan>(item.policy);}
            if(kind==family::type)
            {
                grant.kind=s::purpose::type;auto const& type=std::get<c::type_plan>(item.policy);
                if(type.type_identity=="std_msgs/msg/String")member.schemas[0]=grant.alias;
                else if(type.type_identity=="std_srvs/srv/SetBool_Request")member.schemas[1]=grant.alias;
                else if(type.type_identity=="std_srvs/srv/SetBool_Response")member.schemas[2]=grant.alias;
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_ACTIONS)
                std::array profiles{member.action_plan.policy.goal.selected,member.action_plan.policy.feedback.selected,member.action_plan.policy.result.selected};
                for(std::size_t n{};n<3;++n)if(type.selected==profiles[n])member.action_schema_aliases[n]=grant.alias;
#endif
            }
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_ACTIONS)
            if(kind==family::action)
            {
                grant.kind=s::purpose::action;grant.binding="native-action";member.action_alias=grant.alias;
                for(auto const& op:uwvm2::imported::ros::action_contract::operations)grant.actions.emplace(op.name);
                for(auto const op:uwvm2::imported::ros::action_contract::transition_actions)grant.actions.emplace(op);
            }
#endif
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_OBSERVATIONS)
            if(kind==family::graph)
            {grant.kind=s::purpose::discovery;grant.binding="native-graph";member.graph_alias=grant.alias;member.graph_plan=std::get<s::communication_graph_dispatch::plan>(item.policy);
                for(auto const& op:uwvm2::imported::ros::graph_contract::operations)grant.actions.emplace(op.name);}
            grant.actions.emplace("resource.observe");
#endif
            for(auto const& operation:contract::operations)if(s::permits(grant.kind,operation.name))grant.actions.emplace(operation.name);
            for(auto const& operation:receive::operations)if(s::permits(grant.kind,operation.name))grant.actions.emplace(operation.name);
            for(auto name:{"communication.open.take","communication.open.cancel","request.describe","request.release","resource.close"})if(s::permits(grant.kind,name))grant.actions.emplace(name);
            assert(settings.bind(grant,name,selected.identity)==s::error::success);
            assert(broker.permissions.configure(grant)==s::error::success);
        }
    }
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_OBSERVATIONS)
    for(std::size_t i{};i<2;++i)
    {
        auto& item=members[i];item.event_alias="worker."+uwvm2::utils::container::to_string(i)+".events";
        s::grant grant;grant.alias=grant.resource=item.event_alias;grant.kind=s::purpose::event;grant.binding="event-stream";grant.max_resources=2;
        for(auto const& op:uwvm2::imported::ros::event_contract::operations)grant.actions.emplace(op.name);
        grant.actions.emplace("resource.close");
        assert(broker.permissions.configure(grant)==s::error::success);
        assert(settings.event_plan(grant,"worker."+uwvm2::utils::container::to_string(i),item.event_profile,{2,2,8,65536,65536,1000000000,3})==s::error::success);
        assert(settings.event_plan(grant,"worker."+uwvm2::utils::container::to_string(i),item.event_profile,{2,2,8,65536,65536,1000000000,3})==s::error::conflict);
    }
    for(auto const& op:uwvm2::imported::ros::graph_contract::operations)assert(broker.permissions.expose(text(op.name),s::exposure::allow)==s::error::success);
    for(auto const& op:uwvm2::imported::ros::event_contract::operations)assert(broker.permissions.expose(text(op.name),s::exposure::allow)==s::error::success);
#endif
    assert(at==argc&&settings.pair_workers("robot","worker.0","worker.1",5000000000)==s::error::success);
    assert(settings.pair_workers("other","worker.0","worker.1",5000000000)==s::error::too_large);
    std::uint64_t reserved{};assert(settings.freeze(reserved)==s::error::success&&reserved>0&&settings.frozen());
    assert(settings.worker_limit("worker.0","threads",1)==s::error::denied);
    for(auto const& operation:contract::operations)assert(broker.permissions.expose(text(operation.name),s::exposure::allow)==s::error::success);
    for(auto const& operation:receive::operations)assert(broker.permissions.expose(text(operation.name),s::exposure::allow)==s::error::success);
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_ACTIONS)
    for(auto const& operation:uwvm2::imported::ros::action_contract::operations)assert(broker.permissions.expose(text(operation.name),s::exposure::allow)==s::error::success);
#endif
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_PARAMETERS)
    namespace pc=uwvm2::imported::ros::parameter_contract;
    for(auto const& operation:pc::operations)assert(broker.permissions.expose(text(operation.name),s::exposure::allow)==s::error::success);
    for(auto name:{"parameters","zero-parameters"})
    {
        s::grant grant;grant.alias=grant.resource=name;grant.kind=s::purpose::parameter;grant.binding="local-parameters";grant.max_resources=2;
        for(auto const& operation:pc::operations)grant.actions.emplace(operation.name);
        for(auto action:{"resource.close","request.describe","request.release"})grant.actions.emplace(action);
        if(std::string_view{name}=="zero-parameters"){grant.limits.calls=0;grant.limits.uncertain=true;grant.limits.window=std::chrono::hours{1};}
        assert(broker.permissions.configure(grant)==s::error::success);
    }
#endif
    for(auto name:{"capability.open","capability.close","communication.open.take","communication.open.cancel","request.describe","request.release","resource.close"})assert(broker.permissions.expose(name,s::exposure::allow)==s::error::success);
    assert(broker.configure_communication_dispatch(172,settings.host_limits())==s::error::success);
    auto& host=*broker.communication_store;assert(settings.bind_hosts(host)==s::error::success);
    auto left=host.dispatcher("worker.0"),right=host.dispatcher("worker.1");assert(left&&right&&left!=right&&left->shares_authority(*right));
    d::prepared prepared;assert(d::prepare(settings,host,prepared)==s::error::success&&prepared.supervisors.size()==1);
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_PARAMETERS)
    s::parameters::plan parameter_plan;parameter_plan.selected.identity.assign("parameters");parameter_plan.selected.sha256.fill(3);
    parameter_plan.node_profile.identity.assign(members[0].node_plan.selected.identity);parameter_plan.node_profile.sha256=members[0].node_plan.selected.digest;
    parameter_plan.values={4096,256,256,8,s::parameter_values::algebra::ros2,false};
    s::parameters::key_policy key;key.name.assign("gain");key.types=1ull<<2;key.integer_constraint=true;key.integers={0,100,1};parameter_plan.keys.push_back(key);
    parameter_plan.max_scopes=2;parameter_plan.max_parameters=4;parameter_plan.max_total_bytes=4096;
    parameter_plan.max_events=8;parameter_plan.max_event_bytes=65536;parameter_plan.max_cursors=2;parameter_plan.max_cursor_bytes=8192;
    parameter_plan.max_workspace=65536;parameter_plan.max_batch=4;parameter_plan.max_request_bytes=4096;
    parameter_plan.max_receipt_bytes=16384;parameter_plan.max_pending_bytes=16384;
    parameter_plan.validation_timeout_ns=parameter_plan.cursor_timeout_ns=1000000000;
    assert(broker.configure_parameter_dispatch(2)==s::error::success);
    for(auto name:{"parameters","zero-parameters"})assert(broker.parameter_store->bind(name,members[0].node_alias,parameter_plan)==s::error::success);
    assert(broker.parameter_store->prepare()==s::error::success);
    assert(broker.parameter_store->bind("late",members[0].node_alias,parameter_plan)==s::error::denied);
#endif
    assert(prepared.start(settings.supervisor_stack_bytes())==s::error::success);
    for(auto const& [name,value]:prepared.supervisors)broker.communication_workers.emplace(name,value);
    int instance{};s::caller who{&instance,1};assert(broker.permissions.enroll(who)==s::error::success);
    for(auto& member:members)
    {
        for(auto const& item:{std::pair{&member.context_alias,&member.context_cap},std::pair{&member.node_alias,&member.node_cap},std::pair{&member.topic_alias,&member.topic_cap},std::pair{&member.service_alias,&member.service_cap}})
            assert(broker.permissions.open(who,*item.first,*item.second)==s::error::success);
        for(std::size_t n{};n<3;++n)assert(broker.permissions.open(who,member.schemas[n],member.schema_caps[n])==s::error::success);
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_ACTIONS)
        assert(broker.permissions.open(who,member.action_alias,member.action_cap)==s::error::success);
        for(std::size_t n{};n<3;++n)assert(broker.permissions.open(who,member.action_schema_aliases[n],member.action_schema_caps[n])==s::error::success);
#endif
    }
    std::size_t required{};
    auto receipt=[&](contract::operation op,s::handle cap,bytes const& body,s::handle& result,std::size_t capacity=48)
    {return broker.invoke_communication(who,op,cap,body,capacity,required,[&](wire::bytes data){wire::reader in{data};std::uint64_t key{};return in.u64(key)&&key==1&&in.u64(result)&&result;});};
    for(auto& member:members)
    {
        auto body=encode([&](auto& out){return contract::encode_profile(out,member.context_plan.selected);});
        assert(broker.invoke_communication(who,contract::operation::context_open,member.context_cap,body,8,required,[&](wire::bytes data){wire::reader in{data};return in.u64(member.context)&&in.done();})==s::error::success);
        body=encode([&](auto& out){return out.u64(member.context_cap)&&out.u64(member.context)&&contract::encode_profile(out,member.node_plan.selected)&&out.u64(1)&&out.u64(c::now()+14000000000);});
        assert(receipt(contract::operation::node_open,member.node_cap,body,member.node_request)==s::error::success);
    }
    auto take=[&](s::handle cap,s::handle request,s::handle& result,std::uint64_t family=1)
    {
        auto body=encode([&](auto& out){return out.u64(request);});
        return broker.invoke_communication_creation(who,s::communication_open_dispatch::operation::take,cap,body,64,required,
            [&](wire::bytes data){wire::reader in{data};std::uint64_t field{};for(unsigned n{};n<7;++n)if(!in.u64(field))return false;return field==family&&in.u64(result)&&result&&in.done();});
    };
    auto until=[&](auto&& work){auto deadline=c::now()+20000000000ull;while(!work()){assert(c::now()<deadline);::poll(nullptr,0,1);}};
    until([&]{bool done=true;for(auto& member:members)if(!member.node){auto status=take(member.node_cap,member.node_request,member.node);assert(status==s::error::success||status==s::error::again);done&=member.node!=0;}return done;});
    for(std::size_t i{};i<2;++i)
    {
        auto& member=members[i];
        for(bool service:{false,true})
        {
            auto body=encode([&](auto& out){return out.u64(member.node_cap)&&out.u64(member.node)&&out.u64(service?2:1)&&
                out.u64(member.schema_caps[service?1:0])&&(!service||out.u64(member.schema_caps[2]))&&
                contract::encode_profile(out,service?member.service.selected:member.topic.selected)&&out.text(service?member.service.resolved_name:member.topic.resolved_name)&&
                out.text(service?"std_srvs/srv/SetBool":"std_msgs/msg/String")&&out.text(service?(i?"server":"client"):(i?"subscription":"publisher"))&&out.u64(1)&&
                out.u64(c::now()+std::min<std::uint64_t>(3000000000,service?member.service.max_deadline_ns:member.topic.max_deadline_ns));});
            auto status=receipt(service?contract::operation::service_open:contract::operation::topic_open,service?member.service_cap:member.topic_cap,body,service?member.service_request:member.topic_request);
            if(status!=s::error::success)std::fprintf(stderr,"endpoint creation side=%zu service=%d error=%u\n",i,service,static_cast<unsigned>(status));
            assert(status==s::error::success);
        }
    }
    until([&]
    {
        bool done=true;
        for(auto& member:members)for(bool service:{false,true})
        {
            auto& id=service?member.service_id:member.topic_id;if(id)continue;
            auto status=take(service?member.service_cap:member.topic_cap,service?member.service_request:member.topic_request,id,service?3:2);
            assert(status==s::error::success||status==s::error::again);done&=id!=0;
        }
        return done;
    });
    // Cross-worker capabilities do not supply a node origin despite repeated
    // schema/profile names. Shutdown drains actual workers independently of the
    // guest capability/resource handles that remain visible for inspection.
    auto foreign=encode([&](auto& out){return out.u64(members[0].node);});
    assert(broker.invoke_communication(who,contract::operation::node_describe,members[1].node_cap,foreign,4096,required,[](wire::bytes){return true;})==s::error::bad_handle);
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_PARAMETERS)
    s::handle parameter_cap{},zero_cap{},scope{};
    assert(broker.permissions.open(who,"parameters",parameter_cap)==s::error::success&&broker.permissions.open(who,"zero-parameters",zero_cap)==s::error::success);
    auto opening=[&](std::size_t side){return encode([&](auto& out){return out.u64(members[side].node_cap)&&out.u64(members[side].node)&&
        out.text(parameter_plan.selected.identity.view())&&out.blob(parameter_plan.selected.sha256);});};
    auto publish=[&](wire::bytes data){wire::reader in{data};return in.u64(scope)&&scope&&in.done();};
    assert(broker.invoke_parameter(who,pc::operation::open,parameter_cap,opening(1),8,required,publish)==s::error::denied);
    assert(broker.invoke_parameter(who,pc::operation::open,zero_cap,opening(0),8,required,publish)==s::error::again);
    assert(broker.invoke_parameter(who,pc::operation::open,parameter_cap,opening(0),7,required,publish)==s::error::too_large&&required==8);
    assert(broker.invoke_parameter(who,pc::operation::open,parameter_cap,opening(0),8,required,[](wire::bytes){return false;})==s::error::io);
    s::parameters::usage usage;assert(broker.parameter_store->try_inspect("parameters",usage)&&!usage.retained_scopes);
    assert(broker.invoke_parameter(who,pc::operation::open,parameter_cap,opening(0),8,required,publish)==s::error::success);
    auto declare=encode([&](auto& out){return out.u64(scope)&&out.u64(1)&&out.u64(scope)&&out.text("gain")&&out.text("integer")&&
        out.text("")&&out.text("")&&out.boolean(false)&&out.boolean(false)&&out.boolean(false)&&out.u64(2)&&out.i64(7);});
    bytes declared;
    auto copy=[&](wire::bytes data){declared.assign(data.begin(),data.end());return true;};
    assert(broker.invoke_parameter(who,pc::operation::declare,parameter_cap,declare,4096,required,copy)==s::error::success);
    auto read=encode([&](auto& out){return out.u64(scope)&&out.u64(1)&&out.text("gain");});
    assert(broker.invoke_parameter(who,pc::operation::get,parameter_cap,read,4096,required,[&](wire::bytes data)
        {wire::reader in{data};std::uint64_t count{},tag{},revision{};std::int64_t value{};std::string_view name;bool atomic{};
            return in.u64(count)&&count==1&&in.text(name)&&name=="gain"&&in.u64(tag)&&tag==2&&in.i64(value)&&value==7&&
                in.boolean(atomic)&&atomic&&in.u64(revision)&&revision==2&&in.done();})==s::error::success);
    auto mutation=encode([&](auto& out){return out.u64(scope)&&out.u64(2)&&out.u64(scope)&&out.boolean(false)&&out.u64(1)&&out.text("gain")&&out.u64(2)&&out.i64(9);});
    assert(broker.invoke_parameter(who,pc::operation::set_atomic,parameter_cap,mutation,4096,required,[](wire::bytes){return false;})==s::error::io);
    assert(broker.invoke_parameter(who,pc::operation::set_atomic,parameter_cap,mutation,4096,required,copy)==s::error::success);
    auto parameter_receipt=declared;assert(broker.invoke_parameter(who,pc::operation::set_atomic,parameter_cap,mutation,4096,required,copy)==s::error::success&&declared==parameter_receipt);
    wire::reader request{parameter_receipt};std::uint64_t request_id{};assert(request.u64(request_id));auto request_body=encode([&](auto& out){return out.u64(request_id);});
    assert(broker.invoke_model(who,uwvm2::imported::ros::model_contract::operation::request_describe,parameter_cap,request_body,4096,required,copy)==s::error::success);
    assert(broker.revoke(members[0].node_alias)==s::error::success);
    // Native node retirement invalidates the retained node lease before the
    // parameter authority check; the stale scope is therefore a bad handle.
    assert(broker.invoke_parameter(who,pc::operation::get,parameter_cap,read,4096,required,copy)==s::error::bad_handle);
    auto closing=encode([&](auto& out){return out.u64(scope);});
    assert(broker.invoke(who,uwvm2::imported::ros::runtime_contract::operation::resource_close,parameter_cap,closing,0,required,[](wire::bytes data){return data.empty();})==s::error::success);
    assert(broker.parameter_store->try_inspect("parameters",usage)&&!usage.retained_scopes);
#endif
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_ACTIONS)
#include "communication_worker_deployment_actions_body.h"
    // A released creation receipt must not retain a counted parent node.
    for(auto& item:members)for(bool service:{false,true})
    {
        auto cap=service?item.service_cap:item.topic_cap;auto id=service?item.service_id:item.topic_id;
        auto body=encode([&](auto& out){return out.u64(id);});
        until([&]{auto status=host.close(who,cap,id);assert(status==s::error::success||status==s::error::again);return status==s::error::success;});
        body=encode([&](auto& out){return out.u64(service?item.service_request:item.topic_request);});
        until([&]{auto status=host.invoke_creation(who,s::communication_open_dispatch::operation::release,cap,body,8,required,[](wire::bytes){return true;});
            assert(status==s::error::success||status==s::error::again);return status==s::error::success;});
    }
    for(auto& item:members)until([&]{auto status=host.close(who,item.node_cap,item.node);assert(status==s::error::success||status==s::error::again);return status==s::error::success;});
#endif
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_OBSERVATIONS)
#include "communication_worker_deployment_observations_body.h"
#endif
    broker.retire();for(auto const& [name,worker]:broker.communication_workers){(void)name;worker->retire();assert(!worker->cleanup_confirmed());}
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_OBSERVATIONS)
    std::puts("PASS actual deployed observations: sealed graph filters, exact origins, finite snapshots, host-local readiness streams, failed delivery and revoked-source cleanup");
#endif
    std::puts("PASS actual sealed deployment: finite image and authority bindings, repeated schemas in distinct worker registries, pair supervisors, canonical node/topic/service publication and positive cleanup");
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_PARAMETERS)
    std::puts("PASS actual node-bound parameters: exact node alias, zero permission budget, failed-copy rollback, atomic recovery, shared request routing and parent-independent cleanup");
#endif
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_ACTIONS)
    std::puts("PASS actual deployed actions: owned isolated supervisors, three schemas, canonical goal/feedback/cancel/result, retained delivery, independent close and release");
#endif
}
