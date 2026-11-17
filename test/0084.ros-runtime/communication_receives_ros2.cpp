/** @file
 * @brief Qualify host-owned reservations against two actual sealed ROS workers.
 * @details This native harness uses fixture publication handles. Native service
 * correlations stay in the harness and are never claimed to be Wasm handles.
 */
#define UWVM_COMMUNICATION_JOBS_FIXTURE_ONLY
#include "communication_jobs_ros2.cpp"
#include <uwvm2/imported/ros/scene/communication_receives.h>
#include <uwvm2/imported/ros/scene/communication_correlations.h>
#include <uwvm2/imported/ros/scene/communication_worker_receiver.h>
namespace receive=uwvm2::imported::ros::communication_receive_contract;
namespace owned=s::communication_worker;
struct receiving_peer:queued_peer
{
    struct endpoint
    {
        uwvm2::utils::container::std_unique_ptr<owned::endpoint_receiver> receiver;
        std::shared_ptr<owned::correlations> correlations;
        c::type_plan schema;owned::receive_direction direction{};
        std::uint64_t key{},max_deadline_ns{};
    };
    uwvm2::utils::container::std_vector<endpoint> receivers;
    std::uint64_t recovered{},returned{},acknowledged{};
    void channel_ready(){queued_peer::channel_ready();receivers.resize(caps.size());}
    void attach(std::size_t selected,std::uint64_t native)
    {
        auto const& entry=definition.definitions[selected];c::profile type;std::size_t maximum{};
        auto& destination=receivers[selected];
        if(entry.kind==s::communication_configuration::family::topic)
        {
            auto const& plan=std::get<c::topic_plan>(entry.policy);if(plan.direction!=c::role::subscription)return;
            destination.direction=owned::receive_direction::topic;type=plan.type;maximum=plan.max_message_bytes;destination.max_deadline_ns=plan.max_deadline_ns;
        }
        else if(entry.kind==s::communication_configuration::family::service)
        {
            auto const& plan=std::get<c::service_plan>(entry.policy);bool response=plan.direction==c::service_role::client;
            destination.direction=response?owned::receive_direction::service_response:owned::receive_direction::service_request;
            type=response?plan.response_type:plan.request_type;maximum=response?plan.max_response_bytes:plan.max_request_bytes;destination.max_deadline_ns=plan.max_deadline_ns;
            destination.correlations=uwvm2::utils::container::make_shared<owned::correlations>();
            assert(destination.correlations->configure({owner(selected),10000+selected,native,definition.network.generation,
                response?owned::correlations::role::client:owned::correlations::role::server,plan.pending})==c::error::ok);
        }
        else return;
        destination.schema=std::get<c::type_plan>(definition.definitions[index(type)].policy);
        jobs::plan plan{owner(selected),family(selected),10000+selected,selected+1,definition.network.generation,1,
            definition.request_bytes,definition.response_bytes,destination.max_deadline_ns,definition.request_bytes+3*definition.response_bytes+maximum};
        auto receiver=uwvm2::utils::container::make_unique<owned::endpoint_receiver>();
        assert(receiver->configure(plan,{native,definition.network.generation,maximum,destination.direction,destination.schema.type_identity,
            destination.schema.encoding,destination.schema.validate},admit,charge,this,destination.correlations)==c::error::ok);
        destination.receiver=std::move(receiver);
    }
    native_reply call(rpc::operation operation,std::string_view name,wire::bytes body={})
    {
        std::size_t selected{};while(selected<caps.size()&&cfg::profile_of(definition.definitions[selected]).identity!=name)++selected;assert(selected<caps.size());
        c::delivery_consumer consumer{who.instance,who.generation,caps[selected]};
        if(operation==rpc::operation::service_submit||operation==rpc::operation::service_respond)
        {
            auto& mapping=*receivers[selected].correlations;auto request=uwvm2::imported::ros::resources::allocate_identity();assert(request);
            c::handle correlation{},native{};bytes encoded_body;
            if(operation==rpc::operation::service_submit)assert(mapping.reserve_client(consumer,request)==c::error::ok);
            else
            {
                wire::reader in{body};wire::bytes payload;std::uint64_t until{};
                assert(in.u64(correlation)&&in.blob(payload)&&in.u64(until)&&in.done());
                assert(mapping.response_target(consumer,correlation,native)==c::error::ok);
                encoded_body=encoded([&](auto& out){return out.u64(native)&&out.blob(payload)&&out.u64(until);});body=encoded_body;
                assert(mapping.response_pending(consumer,correlation,request)==c::error::ok);
            }
            auto result=queued_peer::call(operation,name,body,request);assert(last_completion.request_identity==request);
            if(operation==rpc::operation::service_submit)
            {
                if(result.status==c::error::ok){wire::reader in{result.body};assert(in.u64(native)&&native);}
                assert(mapping.submitted(consumer,request,last_completion,native)==c::error::ok);
            }
            else
            {assert(mapping.responded(consumer,correlation,last_completion)==c::error::ok&&mapping.release(consumer,correlation)==c::error::ok);}
            return result;
        }
        if(operation!=rpc::operation::topic_take&&operation!=rpc::operation::service_take)
        {
            auto result=queued_peer::call(operation,name,body);
            if((operation==rpc::operation::topic_open||operation==rpc::operation::service_open)&&result.status==c::error::ok)
            {wire::reader in{result.body};std::uint64_t actual{};assert(in.u64(actual)&&actual&&in.done());attach(selected,actual);}
            return result;
        }
        assert(body.empty());auto& entry=receivers[selected];assert(entry.receiver);auto& receiver=*entry.receiver;
        assert(entry.max_deadline_ns>1000000);
        auto until=c::now()+std::min<std::uint64_t>(900000000,entry.max_deadline_ns-1000000);bool topic=operation==rpc::operation::topic_take;
        auto origin=token(selected,topic?c::permission::topic_reserve:c::permission::service_reserve);
        receive::reference reference;native_reply answer;answer.body.resize(definition.response_bytes);std::size_t extent{};bool fail_copy=true;c::handle correlation{};
        auto decode=[&](c::delivery_reference retained,std::uint64_t actual_deadline,c::service_delivery const& record)
        {
            assert(retained.source==origin.resource&&retained.generation==definition.network.generation&&actual_deadline==until);
            correlation=record.correlation;
            if(fail_copy){fail_copy=false;++recovered;throw std::bad_alloc{};}
            reference={retained.source,retained.generation,retained.sequence,retained.nonce};wire::writer out{answer.body};
            auto kind=topic?"topic.message":entry.direction==owned::receive_direction::service_request?"service.request":"service.response";
            assert(contract::encode_delivery(out,record.information,correlation,kind,
                {entry.schema.type_identity,entry.schema.encoding,record.body}));extent=out.size();return true;
        };
        auto reserve=[&]
        {
            auto status=receiver.reserve(consumer,until,origin,decode);
            while(status==c::error::no_memory||(status==c::error::again&&receiver.occupied()))
            {
                assert(c::now()<until);if(status==c::error::again)
                {auto progress=receiver.pump(transport);assert(progress==c::error::ok||progress==c::error::again);}
                status=receiver.reserve(consumer,until,origin,decode);if(status==c::error::again&&receiver.occupied())::poll(nullptr,0,1);
            }
            return status;
        };
        answer.status=reserve();answer.observed=c::effect::none;
        if(answer.status!=c::error::ok)
        {
            if(answer.status!=c::error::again)std::fprintf(stderr,"reservation %.*s status=%u occupied=%d\n",static_cast<int>(name.size()),name.data(),static_cast<unsigned>(answer.status),receiver.occupied());
            assert(answer.status==c::error::again&&!receiver.occupied());answer.body.clear();return answer;
        }
        auto original_reference=reference;auto original=answer.body;auto original_extent=extent;
        auto settle=[&](bool acknowledge)
        {
            auto current=token(selected,topic?(acknowledge?c::permission::topic_acknowledge:c::permission::topic_release):
                (acknowledge?c::permission::service_acknowledge:c::permission::service_release));
            c::request_key key{origin.resource,++entry.key};c::delivery_receipt receipt;
            c::delivery_reference selected_reference{reference.endpoint,reference.provider_generation,reference.sequence,reference.nonce};
            auto status=receiver.settle(consumer,selected_reference,key,acknowledge,receipt,current);
            while(status==c::error::again)
            {
                assert(c::now()<until);auto progress=receiver.pump(transport);assert(progress==c::error::ok||progress==c::error::again);
                status=receiver.settle(consumer,selected_reference,key,acknowledge,receipt,current);if(status==c::error::again)::poll(nullptr,0,1);
            }
            assert(status==c::error::ok&&receipt.status==c::error::ok&&receipt.observed==c::effect::locally_accepted&&!receiver.occupied());
            auto request=receipt.request;assert(receiver.settle(consumer,selected_reference,key,acknowledge,receipt,current)==c::error::ok&&receipt.request==request);
            if(acknowledge)++acknowledged;else ++returned;
        };
        auto first_correlation=correlation;
        settle(false);assert(reserve()==c::error::ok&&reference.sequence==original_reference.sequence&&reference.nonce!=original_reference.nonce);
        auto first=delivery({original.data(),original_extent}),second=delivery({answer.body.data(),extent});
        assert(first.kind==second.kind&&first.payload.identity==second.payload.identity&&std::ranges::equal(first.payload.body,second.payload.body));
        if(entry.direction==owned::receive_direction::service_request)
        {c::handle native{};assert(correlation!=first_correlation&&entry.correlations->response_target(consumer,first_correlation,native)==c::error::denied);}
        settle(true);
        answer.body.resize(extent);return answer;
    }
    void stop(){for(auto& entry:receivers){if(entry.receiver)entry.receiver->retire();if(entry.correlations)entry.correlations->retire();}queued_peer::stop();}
};
int main(int argc,char** argv)
{
    ::signal(SIGPIPE,SIG_IGN);std::array<receiving_peer,2> peers;int at=1;for(auto& peer:peers)peer.parse(argc,argv,at);assert(at==argc);
    // External test owners have already provisioned their native processes.
    for(auto& peer:peers)peer.wait();
    t::private_link link;assert(link.prepare({peers[0].definition.network,peers[1].definition.network},{peers[0].child,peers[1].child},{peers[0].manifest.configuration,peers[1].manifest.configuration}));
    for(std::size_t side{};side<2;++side)
    {peers[side].wait(true);peers[side].channel_ready();}
    assert(link.activate());auto until=c::now()+5000000000ull;bool visible{};
    while(c::now()<until&&!visible){visible=peers[0].graph_has("/scene/peer")&&peers[1].graph_has("/scene/robot");if(!visible)::poll(nullptr,0,10);}assert(visible);
    for(auto& peer:peers)
    {
        for(auto name:{"publisher","subscription","bool-publisher","bool-subscription"})assert(peer.call(rpc::operation::topic_open,name).status==c::error::ok);
        for(auto name:{"service-client","service-server"})assert(peer.call(rpc::operation::service_open,name).status==c::error::ok);
        for(auto name:{"action-client","action-server"})assert(peer.call(rpc::operation::action_open,name).status==c::error::ok);
    }
    std::array<exercise,2> tests;until=c::now()+15000000000ull;
    while(c::now()<until&&!(tests[0].complete()&&tests[1].complete())){for(std::size_t i{};i<2;++i)tests[i].tick(peers[i]);::poll(nullptr,0,2);}
    for(std::size_t i{};i<2;++i)
    {
        assert(tests[i].complete()&&peers[i].recovered>=4&&peers[i].returned==peers[i].acknowledged&&peers[i].acknowledged>=4);
        std::fprintf(stderr,"retained native peer %zu decode-recoveries=%lu releases=%lu acknowledgements=%lu\n",i,peers[i].recovered,peers[i].returned,peers[i].acknowledged);
    }
    for(auto& peer:peers)peer.stop();
    link.retire();
    std::puts("PASS actual native owned receives: current origins, verified actual endpoint/generation/schema, no reply wait under admission, allocation retry, release/reborrow original String/Bool and SetBool records, exact ACK receipts and bounded host/native service correlation translation both directions; fixture publication handles, not VM handles");
}
