/** @file @brief Actual sealed DDS through owned queues, without authority-held reply waits. */
#define UWVM_COMMUNICATION_CREATION_FIXTURE_ONLY
#include "communication_creation_ros2.cpp"
#include <uwvm2/imported/ros/scene/communication_jobs.h>
#include <uwvm2/imported/ros/scene/communication_mutations.h>
#include <uwvm2/imported/ros/scene/communication_queries.h>
namespace jobs=s::communication_jobs;
struct queued_peer:admitted_peer
{
    uwvm2::utils::container::std_vector<uwvm2::utils::container::std_unique_ptr<jobs::queue>> queues;
    uwvm2::utils::container::std_vector<uwvm2::utils::container::std_unique_ptr<s::communication_worker::mutations>> mutations;
    uwvm2::utils::container::std_vector<uwvm2::utils::container::std_unique_ptr<s::communication_worker::queries>> queries;
    uwvm2::utils::container::std_vector<std::shared_ptr<void const>> origins;
    std::uint64_t operations{},revalidations{};
    c::native_result last_completion;
    c::operation_family family(std::size_t selected)const
    {
        switch(definition.definitions[selected].kind)
        {
            case s::communication_configuration::family::context:return c::operation_family::context;
            case s::communication_configuration::family::node:return c::operation_family::node;
            case s::communication_configuration::family::type:return c::operation_family::type;
            case s::communication_configuration::family::topic:return c::operation_family::topic;
            case s::communication_configuration::family::service:return c::operation_family::service;
            case s::communication_configuration::family::action:return c::operation_family::action;
            case s::communication_configuration::family::graph:return c::operation_family::graph;
            default:assert(false);return {};
        }
    }
    void channel_ready()
    {
        admitted_peer::channel_ready();queues.resize(caps.size());origins.resize(caps.size());mutations.resize(caps.size());queries.resize(caps.size());
        for(std::size_t i{};i<caps.size();++i)
        {
            origins[i]=uwvm2::utils::container::make_shared<std::size_t>(i);
            if(family(i)==c::operation_family::type)continue;
            queues[i]=uwvm2::utils::container::make_unique<jobs::queue>();
            jobs::plan plan{owner(i),family(i),10000+i,i+1,definition.network.generation,4,definition.request_bytes,definition.response_bytes,
                5000000000ull,4*(definition.request_bytes+definition.response_bytes)};
            assert(queues[i]->configure(plan,admit,this)==c::error::ok);
            mutations[i]=uwvm2::utils::container::make_unique<s::communication_worker::mutations>();plan.reserved_bytes+=plan.request_bytes;
            assert(mutations[i]->configure(plan,admit,charge,this)==c::error::ok);
            queries[i]=uwvm2::utils::container::make_unique<s::communication_worker::queries>();plan.slots=1;plan.reserved_bytes=2*(plan.request_bytes+plan.reply_bytes);
            assert(queries[i]->configure(plan,admit,charge,this)==c::error::ok);
        }
    }
    c::operation_token token(std::size_t selected,c::permission operation,c::permission secondary=c::permission::unconfigured)
    {
        c::operation_token result;result.caller_instance=who.instance;result.caller_generation=who.generation;result.authority=owner(selected);
        result.capability=caps[selected];result.resource=10000+selected;result.family=family(selected);result.action=operation;result.secondary=secondary;
        result.retained_origins=origins[selected];
        auto append=[&](std::size_t position,c::permission action,std::uint64_t resource)
        {assert(result.count<result.origins.size());result.origins[result.count++]={owner(position),caps[position],resource,action};};
        append(selected,operation,10000+selected);
        if(result.family!=c::operation_family::context&&result.family!=c::operation_family::node)append(node,c::permission::node_open,10000+node);
        if(result.family!=c::operation_family::context)append(context,c::permission::context_open,10000+context);
        auto type=[&](c::profile const& p){append(index(p),c::permission::type_describe,0);};
        auto const& entry=definition.definitions[selected];
        if(result.family==c::operation_family::topic)type(std::get<c::topic_plan>(entry.policy).type);
        if(result.family==c::operation_family::service){auto const& plan=std::get<c::service_plan>(entry.policy);type(plan.request_type);type(plan.response_type);}
        if(result.family==c::operation_family::action){auto const& plan=std::get<s::communication_action_dispatch::plan>(entry.policy).policy;type(plan.goal.selected);type(plan.feedback.selected);type(plan.result.selected);}
        if(result.family==c::operation_family::graph)append(context,c::permission::context_describe,10000+context);
        if(secondary!=c::permission::unconfigured)append(selected,secondary,10000+selected);
        return result;
    }
    c::error admit_token(c::operation_token const& value,s::group_lease& held,bool charge,std::uint64_t bytes=0)
    {
        if(!value.authority.grant||value.authority.grant>caps.size())return c::error::denied;
        auto selected=static_cast<std::size_t>(value.authority.grant-1);auto expected=token(selected,value.action,value.secondary);
        if(!value.valid(value.action,family(selected),owner(selected),10000+selected,false)||value.caller_instance!=who.instance||value.caller_generation!=who.generation||
            value.capability!=caps[selected]||value.count!=expected.count||value.origins!=expected.origins||value.retained_origins!=origins[selected])return c::error::denied;
        jobs::claim_set claims;
        auto status=permissions.admit_group(who,jobs::admission_claims(value,claims,bytes),held,charge);
        if(!charge)++revalidations;
        return static_cast<c::error>(status);
    }
    static c::error admit(void* opaque,c::operation_token const& value,s::group_lease& held)noexcept
    {return static_cast<queued_peer*>(opaque)->admit_token(value,held,false);}
    static c::error charge(void* opaque,c::operation_token const& value,std::uint64_t bytes,s::group_lease& held)noexcept
    {return static_cast<queued_peer*>(opaque)->admit_token(value,held,true,bytes);}
    native_reply call(rpc::operation operation,std::string_view name,wire::bytes body={},c::handle reserved_request=0)
    {
        if(operation==rpc::operation::topic_open||operation==rpc::operation::service_open||operation==rpc::operation::action_open)
            return admitted_peer::call(operation,name,body);
        std::size_t selected{};while(selected<caps.size()&&cfg::profile_of(definition.definitions[selected]).identity!=name)++selected;assert(selected<caps.size());
        auto secondary=c::permission::unconfigured;
        if(operation==rpc::operation::action_respond)
        {wire::reader in{body};std::uint64_t ignored{},tag{};assert(in.u64(ignored)&&in.u64(ignored)&&in.u64(tag)&&tag>=1&&tag<=8);secondary=static_cast<c::permission>(static_cast<unsigned>(c::permission::action_accept)+tag-1);}
        auto identity=token(selected,c::parse_permission(action(operation)),secondary);
        bool observation=operation==rpc::operation::context_inspect||operation==rpc::operation::node_inspect||operation==rpc::operation::topic_qos||
            operation==rpc::operation::service_qos||operation==rpc::operation::service_available||operation==rpc::operation::action_describe||operation==rpc::operation::graph_capture;
        if(observation)
        {
            auto& query=*queries[selected];native_reply result;auto deadline=c::now()+4000000000ull;
            auto decode=[&](wire::bytes value){result.body.assign(value.begin(),value.end());return c::error::ok;};
            auto typed=[&]()
            {
                if(operation==rpc::operation::action_describe)
                {
                    wire::reader in{body};bool selected{};std::uint64_t goal{};assert(in.boolean(selected)&&(!selected||in.u64(goal))&&in.done());
                    return query.action_describe(identity,selected?std::optional<std::uint64_t>{goal}:std::nullopt,deadline,decode);
                }
                assert(body.empty());
                switch(operation)
                {
                    case rpc::operation::context_inspect:return query.context_describe(identity,deadline,decode);
                    case rpc::operation::node_inspect:return query.node_describe(identity,deadline,decode);
                    case rpc::operation::topic_qos:return query.topic_qos(identity,deadline,decode);
                    case rpc::operation::service_qos:return query.service_qos(identity,deadline,decode);
                    case rpc::operation::service_available:return query.service_available(identity,deadline,decode);
                    case rpc::operation::graph_capture:return query.graph_capture(identity,deadline,decode);
                    default:assert(false);return c::error::invalid;
                }
            };
            result.status=typed();assert(result.status==c::error::again&&query.occupied());
            while(result.status==c::error::again&&query.occupied())
            {
                assert(c::now()<deadline);auto status=query.pump(transport);assert(status==c::error::ok||status==c::error::again);
                result.status=typed();if(result.status==c::error::again&&query.occupied())::poll(nullptr,0,1);
            }
            assert(!query.occupied());result.observed=c::effect::none;++operations;return result;
        }
        identity.request=reserved_request?reserved_request:uwvm2::imported::ros::resources::allocate_identity();
        assert(identity.valid(identity.action,identity.family,identity.authority,identity.resource));auto& queue=*queues[selected];
        auto deadline=c::now()+4000000000ull;jobs::receipt receipt,duplicate;
        bool mutation=operation==rpc::operation::topic_publish||operation==rpc::operation::topic_liveliness||operation==rpc::operation::topic_acknowledged||
            operation==rpc::operation::service_submit||operation==rpc::operation::service_respond||operation==rpc::operation::service_abandon||
            operation==rpc::operation::action_submit||operation==rpc::operation::action_respond||operation==rpc::operation::action_cancel||operation==rpc::operation::context_shutdown;
        auto& effects=*mutations[selected];
        auto typed=[&]()
        {
            wire::reader in{body};wire::bytes payload,uuid;std::uint64_t request{},goal{},time{},change{};bool accept{};
            switch(operation)
            {
                case rpc::operation::topic_publish:assert(in.blob(payload)&&in.u64(time)&&in.done());return effects.publish(identity,payload,time);
                case rpc::operation::topic_liveliness:assert(in.done());return effects.liveliness(identity,deadline);
                case rpc::operation::topic_acknowledged:assert(in.u64(time)&&in.done());return effects.acknowledged(identity,time);
                case rpc::operation::service_submit:assert(in.u64(request)&&!request&&in.blob(payload)&&in.u64(time)&&in.done());return effects.service_submit(identity,payload,time);
                case rpc::operation::service_respond:assert(in.u64(request)&&in.blob(payload)&&in.u64(time)&&in.done());return effects.service_respond(identity,request,payload,time);
                case rpc::operation::service_abandon:assert(in.u64(request)&&in.done());return effects.service_abandon(identity,request,deadline);
                case rpc::operation::action_submit:assert(in.blob(uuid)&&in.blob(payload)&&in.u64(time)&&in.done());return effects.action_submit(identity,uuid,payload,time);
                case rpc::operation::action_respond:assert(in.u64(request)&&in.u64(goal)&&in.u64(change)&&in.blob(payload)&&in.boolean(accept)&&in.u64(time)&&in.done());return effects.action_respond(identity,request,goal,change,payload,accept,time);
                case rpc::operation::action_cancel:assert(in.u64(goal)&&in.u64(time)&&in.done());return effects.action_cancel(identity,goal,time);
                case rpc::operation::context_shutdown:assert(in.done());return effects.shutdown(identity,deadline);
                default:assert(false);return c::native_result{};
            }
        };
        if(mutation)
        {
            auto first=typed(),retry=typed();assert(first.status==c::error::ok&&first.deferred&&first.request_identity==identity.request&&
                retry.status==first.status&&retry.request_identity==first.request_identity&&retry.provider_generation==definition.network.generation);
            assert(effects.inspect(identity,receipt)==c::error::ok&&receipt.phase==jobs::state::queued);
        }
        else
        {
            assert(queue.enqueue(identity,operation,body,deadline,charge,this,receipt)==c::error::ok&&receipt.phase==jobs::state::queued);
            assert(queue.enqueue(identity,operation,body,deadline,charge,this,duplicate)==c::error::ok&&duplicate.request==receipt.request);
        }
        assert(receipt.observed==c::effect::locally_accepted);s::group_lease held;
        do
        {
            assert(c::now()<deadline);auto status=mutation?effects.pump(transport):queue.pump(transport);assert(status==c::error::ok||status==c::error::again);
            assert((mutation?effects.inspect(identity,receipt):queue.inspect(identity,identity.request,receipt))==c::error::ok);
            if(receipt.phase!=jobs::state::complete)::poll(nullptr,0,1);
        }while(receipt.phase!=jobs::state::complete);
        assert(receipt.provider_generation==definition.network.generation&&!receipt.cleanup_debt);
        native_reply answer;answer.body.resize(definition.response_bytes);assert(admit_token(identity,held,false)==c::error::ok);
        assert((mutation?effects.copy(identity,held,answer.body,receipt):queue.copy(identity,held,answer.body,receipt))==c::error::ok);held.release();
        answer.status=receipt.status;answer.observed=receipt.observed;answer.body.resize(receipt.bytes);
        c::operation_token completed;c::native_result result;
        if(mutation)
        {
            uwvm2::utils::container::std_vector<std::uint8_t> completion_body(definition.response_bytes);std::size_t extent{};
            assert(effects.completion(completed,result,completion_body,extent)==c::error::ok&&extent==answer.body.size()&&
                std::equal(answer.body.begin(),answer.body.end(),completion_body.begin())&&result.status==answer.status&&result.observed==answer.observed);
            assert(effects.release(identity)==c::error::ok);
        }
        else{assert(queue.completion(completed,result)==c::error::ok);assert(queue.release(identity,identity.request)==c::error::ok);}
        assert(completed.request==identity.request&&!result.deferred);last_completion=result;++operations;return answer;
    }
    bool graph_has(std::string_view expected)
    {
        auto result=call(rpc::operation::graph_capture,"graph");assert(result.status==c::error::ok);wire::reader in{result.bytes()};std::uint64_t rows{};assert(in.u64(rows));bool present{};
        for(std::uint64_t i{};i<rows;++i){std::uint64_t kind{};std::string_view node,endpoint,type;assert(in.u64(kind)&&in.text(node)&&in.text(endpoint)&&in.text(type));if(node==expected)present=true;}
        assert(in.done());return present;
    }
    void stop(){for(auto& query:queries)if(query)query->retire();for(auto& effects:mutations)if(effects)effects->retire();for(auto& queue:queues)if(queue)queue->retire();admitted_peer::stop();}
};
#ifndef UWVM_COMMUNICATION_JOBS_FIXTURE_ONLY
int main(int argc,char** argv)
{
    ::signal(SIGPIPE,SIG_IGN);std::array<queued_peer,2> peers;int at=1;for(auto& peer:peers)peer.parse(argc,argv,at);assert(at==argc);
    // External test owners have already provisioned their native processes.
    for(auto& peer:peers)peer.wait();
    t::private_link link;assert(link.prepare({peers[0].definition.network,peers[1].definition.network},{peers[0].child,peers[1].child},{peers[0].manifest.configuration,peers[1].manifest.configuration}));
    for(std::size_t side{};side<2;++side){peers[side].wait(true);peers[side].channel_ready();}
    assert(!peers[0].graph_has("/scene/peer")&&!peers[1].graph_has("/scene/robot")&&link.activate());auto until=c::now()+5000000000ull;bool visible{};
    while(c::now()<until&&!visible){visible=peers[0].graph_has("/scene/peer")&&peers[1].graph_has("/scene/robot");if(!visible)::poll(nullptr,0,10);}assert(visible);
    for(auto& peer:peers)
    {
        for(auto name:{"publisher","subscription","bool-publisher","bool-subscription"})assert(peer.call(rpc::operation::topic_open,name).status==c::error::ok);
        for(auto name:{"service-client","service-server"})assert(peer.call(rpc::operation::service_open,name).status==c::error::ok);
        for(auto name:{"action-client","action-server"})assert(peer.call(rpc::operation::action_open,name).status==c::error::ok);
        assert(peer.created==8);
    }
    std::array<exercise,2> tests;until=c::now()+12000000000ull;
    while(c::now()<until&&!(tests[0].complete()&&tests[1].complete())){for(std::size_t i{};i<2;++i)tests[i].tick(peers[i]);::poll(nullptr,0,2);}
    for(std::size_t i{};i<2;++i)
    {auto const& test=tests[i];std::fprintf(stderr,"queued native peer %zu flags=%d%d%d%d%d%d opens=%lu operations=%lu current-validations=%lu\n",i,test.string_received,test.bool_received,test.service_received,test.service_served,test.action_received,test.action_served,peers[i].created,peers[i].operations,peers[i].revalidations);assert(test.complete()&&peers[i].revalidations>=2*peers[i].operations);}
    for(auto& peer:peers)peer.stop();link.retire();
    std::puts("PASS actual native queued operations: 16 explicit asynchronous opens, complete per-call context/schema/action-transition origins, bounded owned queue, lease-free reply receive, private DDS String/Bool SetBool LookupTransform both directions; fixture publication handles, not VM handles");
}
#endif
