/** @file @brief Actual native DDS creation through nonblocking admitted RPC and exact ledger.
 * @details Uses the exact previously qualified sealed two-worker image packs.
 * Publication handles here belong to this native fixture, not a real Wasm VM.
 */
#define UWVM_COMMUNICATION_PRIVATE_FIXTURE_ONLY
#include "communication_private_ros2.cpp"
#include <uwvm2/imported/ros/scene/communication_creation.h>
#include <uwvm2/imported/ros/scene/communication_worker_openings.h>
namespace creation=s::communication_creation;
namespace asynchronous=s::communication_worker_async;
namespace opening=uwvm2::imported::ros::communication_open_contract;
struct native_reply
{
    c::error status{c::error::io};c::effect observed{};uwvm2::utils::container::std_vector<std::uint8_t> body;
    wire::bytes bytes_view()const noexcept{return body;}
    wire::bytes bytes()const noexcept{return body;}
};
struct admitted_peer:sealed_peer
{
    asynchronous::transport transport;s::authority permissions;int instance{};s::caller who{&instance,1};
    uwvm2::utils::container::std_vector<s::handle> caps;s::communication_worker::openings creations;
    std::size_t context{},node{};std::uint64_t created{};
    static std::string_view action(rpc::operation operation)
    {
        switch(operation)
        {
        case rpc::operation::context_inspect:return "context.describe";case rpc::operation::node_inspect:return "node.describe";
        case rpc::operation::topic_open:return "topic.open";case rpc::operation::topic_publish:return "topic.publish";case rpc::operation::topic_take:return "topic.take";
        case rpc::operation::topic_qos:case rpc::operation::service_qos:return "qos.describe";case rpc::operation::topic_liveliness:return "qos.liveliness.assert";
        case rpc::operation::topic_acknowledged:return "qos.acknowledgements.wait";case rpc::operation::service_open:return "service.open";
        case rpc::operation::service_submit:return "service.submit";case rpc::operation::service_respond:return "service.respond";case rpc::operation::service_take:return "service.take";
        case rpc::operation::service_available:return "service.available";case rpc::operation::service_abandon:return "service.abandon";
        case rpc::operation::action_open:return "action.open";case rpc::operation::action_submit:return "action.submit";case rpc::operation::action_respond:return "action.respond";
        case rpc::operation::action_take:return "action.take";case rpc::operation::action_cancel:return "action.cancel";case rpc::operation::action_describe:return "action.describe";
        case rpc::operation::graph_capture:return "graph.snapshot";case rpc::operation::resource_close:return "resource.close";case rpc::operation::context_shutdown:return "context.shutdown";
        case rpc::operation::topic_reserve:return "topic.reserve";case rpc::operation::topic_acknowledge:return "topic.acknowledge";case rpc::operation::topic_release:return "topic.release";
        case rpc::operation::service_reserve:return "service.reserve";case rpc::operation::service_acknowledge:return "service.acknowledge";case rpc::operation::service_release:return "service.release";
        }
        assert(false);return {};
    }
    std::size_t index(c::profile const& selected)const
    {for(std::size_t i{};i<definition.definitions.size();++i)if(cfg::profile_of(definition.definitions[i])==selected)return i;assert(false);return 0;}
    c::owner owner(std::size_t selected)const noexcept{return {81,selected+1,definition.network.generation};}
    c::error creation_admission(creation::identity const& identity,std::uint64_t bytes,s::group_lease& held,bool charged)
    {
        if(!creation::same_caller(identity.who,who)||!identity.authority.grant||identity.authority.grant>caps.size())return c::error::denied;
        auto selected=identity.authority.grant-1;auto const& entry=definition.definitions[selected];
        if(identity.authority!=owner(selected)||(entry.kind!=s::communication_configuration::family::topic&&
            entry.kind!=s::communication_configuration::family::service&&entry.kind!=s::communication_configuration::family::action))return c::error::denied;
        std::array<creation::origin,6> expected{};std::size_t count{};
        expected[count++]={creation::dependency::operation,owner(selected),caps[selected],0};
        expected[count++]={creation::dependency::node,owner(node),caps[node],101};
        expected[count++]={creation::dependency::context,owner(context),caps[context],102};
        auto schema=[&](c::profile const& profile){auto position=index(profile);expected[count++]={creation::dependency::schema,owner(position),caps[position],0};};
        auto kind=opening::endpoint_kind::topic;
        if(entry.kind==s::communication_configuration::family::topic)schema(std::get<c::topic_plan>(entry.policy).type);
        else if(entry.kind==s::communication_configuration::family::service)
        {kind=opening::endpoint_kind::service;auto const& plan=std::get<c::service_plan>(entry.policy);schema(plan.request_type);schema(plan.response_type);}
        else
        {kind=opening::endpoint_kind::action;auto const& plan=std::get<s::communication_action_dispatch::plan>(entry.policy).policy;schema(plan.goal.selected);schema(plan.feedback.selected);schema(plan.result.selected);}
        if(identity.count!=count||identity.origins!=expected)return c::error::denied;
        creation::claim_set claims;auto requested=creation::admission_claims(identity,kind,claims,bytes);
        return static_cast<c::error>(permissions.admit_group(who,requested,held,charged));
    }
    void channel_ready()
    {
        assert(transport.prepare({child->input_fd(),child->output_fd(),child->stderr_fd(),child->process_fd(),child->session_until(),child->stop_flag()},
            {definition.request_bytes,definition.response_bytes,definition.response_bytes,65536-diagnostic_bytes,5000000000ull,1000000},definition.network.generation)==c::error::ok);
        caps.resize(definition.definitions.size());
        for(auto name:{"capability.open","context.open","node.open","type.describe","communication.open.take","communication.open.cancel","request.describe","request.release","action.accept","action.reject","action.execute","action.feedback","action.succeed","action.abort","action.finish.cancelled","action.cancel.respond"})assert(permissions.expose(name,s::exposure::allow)==s::error::success);
        for(std::uint64_t op=1;op<=static_cast<std::uint64_t>(rpc::operation::service_release);++op)
        {auto status=permissions.expose(uwvm2::utils::container::std_string{action(static_cast<rpc::operation>(op))},s::exposure::allow);assert(status==s::error::success||status==s::error::conflict);}
        uwvm2::utils::container::std_vector<creation::plan> plans;
        for(std::size_t i{};i<definition.definitions.size();++i)
        {
            auto const& entry=definition.definitions[i];s::grant grant;grant.alias=cfg::profile_of(entry).identity;grant.resource="sealed.fixture";grant.binding="private-worker";
            switch(entry.kind)
            {
            case s::communication_configuration::family::context:grant.kind=s::purpose::context;context=i;break;
            case s::communication_configuration::family::node:grant.kind=s::purpose::node;node=i;break;
            case s::communication_configuration::family::type:grant.kind=s::purpose::type;break;
            case s::communication_configuration::family::topic:grant.kind=s::purpose::topic;break;
            case s::communication_configuration::family::service:grant.kind=s::purpose::service;break;
            case s::communication_configuration::family::action:grant.kind=s::purpose::action;break;
            case s::communication_configuration::family::graph:grant.kind=s::purpose::discovery;break;
            default:assert(false);
            }
            for(std::uint64_t op=1;op<=static_cast<std::uint64_t>(rpc::operation::service_release);++op)
            {auto name=action(static_cast<rpc::operation>(op));if(s::permits(grant.kind,name))grant.actions.emplace(name);}
            for(auto name:{"context.open","node.open","type.describe","communication.open.take","communication.open.cancel","request.describe","request.release","action.accept","action.reject","action.execute","action.feedback","action.succeed","action.abort","action.finish.cancelled","action.cancel.respond"})if(s::permits(grant.kind,name))grant.actions.emplace(name);
            assert(permissions.configure(std::move(grant))==s::error::success);
            auto kind=entry.kind==s::communication_configuration::family::topic?opening::endpoint_kind::topic:
                entry.kind==s::communication_configuration::family::service?opening::endpoint_kind::service:opening::endpoint_kind::action;
            if(entry.kind==s::communication_configuration::family::topic||entry.kind==s::communication_configuration::family::service||entry.kind==s::communication_configuration::family::action)
                plans.push_back({owner(i),cfg::profile_of(entry),kind,i+1,definition.network.generation,256,5000000000ull});
        }
        assert(permissions.enroll(who)==s::error::success);for(std::size_t i{};i<caps.size();++i)assert(permissions.open(who,cfg::profile_of(definition.definitions[i]).identity,caps[i])==s::error::success);
        auto revalidate=[](void* opaque,creation::identity const& identity,s::group_lease& held)noexcept
        {return static_cast<admitted_peer*>(opaque)->creation_admission(identity,0,held,false);};
        auto charge=[](void* opaque,creation::identity const& identity,std::uint64_t bytes,s::group_lease& held)noexcept
        {return static_cast<admitted_peer*>(opaque)->creation_admission(identity,bytes,held,true);};
        assert(creations.configure(plans,plans.size()*512,revalidate,charge,this)==c::error::ok);
    }
    native_reply call(rpc::operation operation,std::string_view name,wire::bytes body={})
    {
        std::size_t selected{};while(selected<definition.definitions.size()&&cfg::profile_of(definition.definitions[selected]).identity!=name)++selected;assert(selected<caps.size());
        auto const& entry=definition.definitions[selected];std::array<s::admission_claim,6> claims{};std::array<std::size_t,6> positions{};std::size_t count=1;
        positions[0]=selected;claims[0]={caps[selected],action(operation),body.size(),false,false};
        if(entry.kind!=s::communication_configuration::family::context&&entry.kind!=s::communication_configuration::family::node)
        {positions[count]=node;claims[count++]={caps[node],"node.open",0,false,true};}
        if(entry.kind!=s::communication_configuration::family::context)
        {positions[count]=context;claims[count++]={caps[context],"context.open",0,false,true};}
        auto type=[&](c::profile const& profile){auto found=index(profile);positions[count]=found;claims[count++]={caps[found],"type.describe",0,false,true};};
        if(entry.kind==s::communication_configuration::family::topic)type(std::get<c::topic_plan>(entry.policy).type);
        if(entry.kind==s::communication_configuration::family::service){auto const& plan=std::get<c::service_plan>(entry.policy);type(plan.request_type);type(plan.response_type);}
        if(entry.kind==s::communication_configuration::family::action){auto const& plan=std::get<s::communication_action_dispatch::plan>(entry.policy).policy;type(plan.goal.selected);type(plan.feedback.selected);type(plan.result.selected);}
        native_reply answer;answer.body.resize(definition.response_bytes);auto deadline=c::now()+4000000000ull;
        bool is_open=operation==rpc::operation::topic_open||operation==rpc::operation::service_open||operation==rpc::operation::action_open;
        creation::view request;creation::identity identity;
        if(is_open)
        {
            identity.who=who;identity.authority=owner(selected);identity.key=1;identity.deadline=deadline;identity.count=count;
            for(std::size_t i{};i<count;++i){auto kind=i==0?creation::dependency::operation:i==1?creation::dependency::node:i==2?creation::dependency::context:creation::dependency::schema;
                identity.origins[i]={kind,owner(positions[i]),caps[positions[i]],i==1?101u:i==2?102u:0u};}
            auto canonical=encoded([&](auto& out){return out.u64(selected+1)&&out.u64(identity.key)&&out.u64(deadline);});
            assert(body.empty()&&creations.enqueue(identity,canonical,request)==c::error::ok&&!request.reserved);
            auto duplicate=request;assert(creations.enqueue(identity,canonical,duplicate)==c::error::ok&&duplicate.receipt.request_id==request.receipt.request_id);
            while(request.receipt.status==opening::state::queued||request.receipt.status==opening::state::sending||request.receipt.status==opening::state::waiting)
            {
                auto status=creations.pump(transport);assert(status==c::error::ok||status==c::error::again);
                assert(creations.inspect(who,request.receipt.request_id,request)==c::error::ok&&c::now()<deadline);
                if(request.receipt.status!=opening::state::ready)::poll(nullptr,0,1);
            }
            creation::work origin;std::uint64_t native{};
            assert(creations.provider_result(request.receipt.request_id,origin,native)==c::error::ok&&origin.original==identity&&native);
            s::group_lease admitted;assert(creation_admission(identity,0,admitted,false)==c::error::ok);opening::published published;
            assert(creations.publish(who,request.receipt.request_id,admitted,10000+selected,published)==c::error::ok);admitted.release();++created;
            assert(creations.release(who,request.receipt.request_id)==c::error::busy);
            answer.status=request.receipt.result;answer.observed=request.receipt.observed;
            wire::writer out{answer.body};assert(out.u64(native));answer.body.resize(out.size());return answer;
        }
        asynchronous::ticket ticket;assert(transport.stage(operation,selected+1,body,deadline,ticket)==c::error::ok);
        s::group_lease admitted;assert(permissions.admit_group(who,{claims.data(),count},admitted,true)==s::error::success);
        auto send=[&]{return transport.send(ticket,admitted);};
        auto sent=send();admitted.release();while(sent==c::error::again)
        {assert(c::now()<deadline);::poll(nullptr,0,1);assert(permissions.admit_group(who,{claims.data(),count},admitted,false)==s::error::success);sent=send();admitted.release();}assert(sent==c::error::ok);
        auto received=transport.receive(ticket);while(received==c::error::again){assert(c::now()<deadline);::poll(nullptr,0,1);received=transport.receive(ticket);}assert(received==c::error::ok);
        asynchronous::result result;assert(transport.copy(ticket,answer.body,result)==c::error::ok);answer.status=result.status;answer.observed=result.observed;answer.body.resize(result.bytes);
        assert(transport.acknowledge(ticket)==c::error::ok);return answer;
    }
    bool graph_has(std::string_view expected)
    {
        auto result=call(rpc::operation::graph_capture,"graph");assert(result.status==c::error::ok);wire::reader in{result.bytes()};std::uint64_t rows{};assert(in.u64(rows));bool present{};
        for(std::uint64_t i{};i<rows;++i){std::uint64_t kind{};std::string_view node,endpoint,type;assert(in.u64(kind)&&in.text(node)&&in.text(endpoint)&&in.text(type));if(node==expected)present=true;}
        assert(in.done());return present;
    }
    void stop(){creations.retire();transport.retire();sealed_peer::stop();}
};
#ifndef UWVM_COMMUNICATION_CREATION_FIXTURE_ONLY
int main(int argc,char** argv)
{
    ::signal(SIGPIPE,SIG_IGN);std::array<admitted_peer,2> peers;int at=1;for(auto& peer:peers)peer.parse(argc,argv,at);assert(at==argc);
    // External test owners have already provisioned their native processes.
    for(auto& peer:peers)peer.wait();
    t::private_link link;
    assert(link.prepare({peers[0].definition.network,peers[1].definition.network},{peers[0].child,peers[1].child},{peers[0].manifest.configuration,peers[1].manifest.configuration}));
    for(std::size_t side{};side<2;++side){peers[side].wait(true);peers[side].channel_ready();}
    assert(!peers[0].graph_has("/scene/peer")&&!peers[1].graph_has("/scene/robot")&&link.activate());auto until=c::now()+5000000000ull;
    bool visible{};while(c::now()<until&&!visible){visible=peers[0].graph_has("/scene/peer")&&peers[1].graph_has("/scene/robot");if(!visible)::poll(nullptr,0,10);}assert(visible);
    for(auto& peer:peers)
    {
        for(auto name:{"publisher","subscription","bool-publisher","bool-subscription"})assert(peer.call(rpc::operation::topic_open,name).status==c::error::ok);
        for(auto name:{"service-client","service-server"})assert(peer.call(rpc::operation::service_open,name).status==c::error::ok);
        for(auto name:{"action-client","action-server"})assert(peer.call(rpc::operation::action_open,name).status==c::error::ok);
        assert(peer.created==8);
    }
    std::array<exercise,2> tests;until=c::now()+12000000000ull;while(c::now()<until&&!(tests[0].complete()&&tests[1].complete())){for(std::size_t i{};i<2;++i)tests[i].tick(peers[i]);::poll(nullptr,0,2);}
    for(std::size_t i{};i<2;++i){auto const& test=tests[i];std::fprintf(stderr,"async native peer %zu flags=%d%d%d%d%d%d creation=%lu\n",i,test.string_received,test.bool_received,test.service_received,test.service_served,test.action_received,test.action_served,peers[i].created);assert(test.complete());}
    for(auto& peer:peers)peer.stop();
    link.retire();
    std::puts("PASS actual native asynchronous creation: 16 exact-key admitted opens, no authority lease held across RPC wait, private DDS String/Bool SetBool LookupTransform both directions; native fixture handles, not VM handles");
}
#endif
