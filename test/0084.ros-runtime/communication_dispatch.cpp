/** @file
 * @brief Exercise canonical communication authority composition and transactional copies.
 * @details This mock factory qualifies broker decisions; native ROS interoperability
 * is qualified by the separate actual-provider cohorts, never by this fixture.
 */
#include <uwvm2/imported/ros/scene/communication_dispatch.h>
#include <uwvm2/imported/ros/scene/schema_contract.h>
#include <uwvm2/imported/ros/scene/communication_mutations.h>
#include <uwvm2/imported/ros/scene/communication_queries.h>
#include <uwvm2/imported/ros/scene/communication_worker_receiver.h>
#include <uwvm2/imported/ros/scene/call_audit.h>
#include <uwvm2/imported/ros/scene/communication_worker_closure.h>
#include <cassert>
#include <iostream>
#include <atomic>
#include <thread>
#include <barrier>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;
namespace c=s::communication;
namespace w=s::wire;
namespace contract=uwvm2::imported::ros::communication_contract;
namespace schema=s::schema_contract;
using error=s::error;
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
using text=uwvm2::utils::container::std_string;
static c::profile profile(std::string_view name)
{c::profile result;result.identity=name;result.digest.fill(17);return result;}
static bool valid_body(w::bytes body) noexcept{return body.size()<=64&&!body.empty()&&body[0]!=255;}
static c::qos quality(){return {1,1,2,1,2,{0,0},{0,0},{0,0}};}
struct context final:c::native_context
{std::atomic<bool> active{true};bool live() const noexcept override{return active.load();}c::error shutdown() noexcept override{active=false;return c::error::ok;}};
struct node final:c::native_node{};
struct topic final:c::native_topic
{
    uwvm2::utils::container::std_unique_ptr<s::communication_worker::mutations> queued;
    uwvm2::utils::container::std_unique_ptr<s::communication_worker::queries> observed;
    uwvm2::utils::container::std_unique_ptr<s::communication_worker::endpoint_receiver> receiver;
    uwvm2::utils::container::std_unique_ptr<s::communication_worker::endpoint_closure> closure;
    c::error close(c::operation_token const& token)noexcept override
    {return closure?closure->close(token,[&]noexcept{return !receiver||!receiver->occupied();}):c::error::ok;}
    bool cleanup_confirmed()const noexcept override{return !closure||closure->cleanup_confirmed();}
    bool queued_mutations()const noexcept override{return bool(queued);}
    bool queued_observations()const noexcept override{return bool(observed);}
    bool queued_receives()const noexcept override{return bool(receiver);}
    c::error reserve(c::delivery_consumer who,std::uint64_t until,void* context,reservation_sink sink,c::operation_token const& token)noexcept override
    {return receiver?receiver->reserve(who,until,token,[&](auto ref,auto deadline,c::service_delivery const& value)
        {return sink(context,ref,deadline,{value.information,value.body});}):c::error::unsupported;}
    c::error settle(c::delivery_consumer who,c::delivery_reference ref,c::request_key key,bool ack,c::delivery_receipt& output,c::operation_token const& token)noexcept override
    {return receiver?receiver->settle(who,ref,key,ack,output,token):c::error::unsupported;}
    bool occupied()noexcept override{return (receiver&&receiver->occupied())||(closure&&closure->occupied());}
    std::atomic<unsigned> publications{},assertions{},inspections{};std::atomic<bool> unknown{};
    c::endpoint_identity evidence;
    c::endpoint_identity identity()const noexcept override{return evidence;}
    std::mutex operation_mutex;c::operation_token last_operation;
    void retain(c::operation_token const& value) noexcept
    {assert(value.valid(value.action,c::operation_family::topic,value.authority,value.resource,false));std::lock_guard lock{operation_mutex};last_operation=value;}
    c::native_result publish(w::bytes body,std::uint64_t deadline,c::operation_token const& value) noexcept override
    {retain(value);return queued?queued->publish(value,body,deadline):publish(body,deadline);}
    c::error inspect(c::qos& out,std::array<std::uint8_t,32>& gid,std::size_t& size,c::operation_token const& value) noexcept override
    {
        retain(value);
        if(observed)return observed->topic_qos(value,c::now()+1000000000,[&](w::bytes bytes)
        {w::reader input{bytes};std::uint64_t depth{};if(!input.u64(depth)||!depth||!input.done())return c::error::invalid;
            out=quality();out.depth=depth;gid={};size=0;return c::error::ok;});
        return inspect(out,gid,size);
    }
    c::native_result assert_liveliness(c::operation_token const& value) noexcept override
    {retain(value);return queued?queued->liveliness(value,c::now()+1000000000):assert_liveliness();}
    c::native_result wait_acknowledged(std::uint64_t deadline,c::operation_token const& value) noexcept override
    {retain(value);return {wait_acknowledged(deadline),c::effect::observed_remote};}
    c::error poll(c::operation_token const& value) noexcept override{retain(value);return c::error::ok;}
    c::native_result publish(w::bytes,std::uint64_t) noexcept override
    {++publications;return unknown?c::native_result{c::error::io,c::effect::unknown}:c::native_result{c::error::ok,c::effect::locally_accepted};}
    c::error inspect(c::qos& out,std::array<std::uint8_t,32>& gid,std::size_t& size) noexcept override
    {++inspections;out=quality();gid.fill(2);size=16;return c::error::ok;}
    c::native_result assert_liveliness() noexcept override{++assertions;return {c::error::ok,c::effect::locally_accepted};}
    c::error wait_acknowledged(std::uint64_t) noexcept override{return c::error::ok;}
};
struct service final:c::native_service
{
    uwvm2::utils::container::std_unique_ptr<s::communication_worker::endpoint_receiver> receiver;
    std::shared_ptr<s::communication_worker::correlations> correlations;
    bool queued_receives()const noexcept override{return bool(receiver);}
    c::error reserve(c::delivery_consumer who,std::uint64_t until,void* context,c::service_reservation_sink sink,c::operation_token const& token)noexcept override
    {return receiver?receiver->reserve(who,until,token,[&](auto ref,auto deadline,c::service_delivery const& value)
        {return sink(context,ref,deadline,value);}):reserve(who,until,context,sink);}
    c::error settle(c::delivery_consumer who,c::delivery_reference ref,c::request_key key,bool ack,c::delivery_receipt& output,c::operation_token const& token)noexcept override
    {return receiver?receiver->settle(who,ref,key,ack,output,token):settle(who,ref,key,ack,output);}
    std::atomic<unsigned> submissions{},inspections{};std::atomic<bool> closed{},unknown{};
    uwvm2::utils::container::function_ref<void()> during_available;
    unsigned availability_calls{};
    std::array<std::optional<c::qos>,2> evidence;
    std::shared_ptr<c::receive_queue> records;bool response{};unsigned invalid_settlement{},settlements{};
    c::endpoint_identity endpoint_evidence;
    c::endpoint_identity identity()const noexcept override{return endpoint_evidence;}
    std::array<std::optional<c::qos>,2> channels()const noexcept override{return evidence;}
    c::native_result submit(c::handle,w::bytes,std::uint64_t) noexcept override
    {++submissions;return unknown?c::native_result{c::error::io,c::effect::unknown}:c::native_result{c::error::ok,c::effect::locally_accepted};}
    c::native_result respond(c::handle id,w::bytes body,std::uint64_t deadline) noexcept override{return submit(id,body,deadline);}
    c::error take(void*,c::service_sink) noexcept override{return c::error::again;}
    c::error reserve(c::delivery_consumer who,std::uint64_t until,void* context,c::service_reservation_sink sink)noexcept override
    {
        if(!records)return c::error::unsupported;
        return records->reserve(who,until,c::now(),[&](auto reference,auto deadline,c::delivery const& value)
        {return sink(context,reference,deadline,{value.information,53,response,value.body});});
    }
    c::error settle(c::delivery_consumer who,c::delivery_reference reference,c::request_key key,bool acknowledge,c::delivery_receipt& output)noexcept override
    {
        ++settlements;auto status=records?records->settle(who,reference,key,acknowledge,c::now(),output):c::error::unsupported;
        if(status==c::error::ok)
        {
            if(invalid_settlement==1)return c::error::io;
            if(invalid_settlement==2)++output.key.sequence;
            if(invalid_settlement==3)output.observed=c::effect::unknown;
        }
        return status;
    }
    c::error available(bool& out) noexcept override{++availability_calls;if(during_available)during_available();out=true;return c::error::ok;}
    c::error inspect(c::qos& out) noexcept override{++inspections;out=quality();return c::error::ok;}
    c::native_result abandon(c::handle) noexcept override{return {c::error::ok,c::effect::unknown};}
    bool occupied() noexcept override{return (receiver&&receiver->occupied())||(records&&records->has_reservation());}
    void retire() noexcept override{closed=true;if(receiver)receiver->request_stop();}
};
struct backend final:c::backend
{
    s::communication_dispatch* queue_dispatch{};s::communication_dispatch* receiver_dispatch{};s::communication_dispatch* query_dispatch{};
    s::communication_dispatch* closing_dispatch{};bool receive_services{};
    c::error configure_receiver(c::owner owner,c::handle id,c::type_plan const& schema,s::communication_worker::receive_direction direction,
        uwvm2::utils::container::std_unique_ptr<s::communication_worker::endpoint_receiver>& receiver,
        std::shared_ptr<s::communication_worker::correlations> mapping={})
    {
        receiver=uwvm2::utils::container::make_unique<s::communication_worker::endpoint_receiver>();
        auto validate=[](void* context,c::operation_token const& token,s::group_lease& held)noexcept
        {return static_cast<c::error>(static_cast<s::communication_dispatch*>(context)->revalidate(token,held));};
        auto charge=[](void* context,c::operation_token const& token,std::uint64_t bytes,s::group_lease& held)noexcept
        {return static_cast<c::error>(static_cast<s::communication_dispatch*>(context)->charge_operation(token,bytes,held));};
        return receiver->configure({owner,direction==s::communication_worker::receive_direction::topic?c::operation_family::topic:c::operation_family::service,
            id,1,71,1,128,512,10000000000,1728},{10001,71,64,direction,schema.type_identity,schema.encoding,schema.validate},validate,charge,receiver_dispatch,std::move(mapping));
    }
    unsigned creations{},stage_polls{},stage_launches{},stage_retirements{};bool staged{},stage_ready{};std::optional<c::node_stage_request> pending_stage;
    std::shared_ptr<topic> publisher,subscriber;std::shared_ptr<service> client;
    std::shared_ptr<c::receive_queue> queue;c::handle subscription{},generation{};
    c::error create_context(c::context_plan const&,std::shared_ptr<c::native_context>& out) noexcept override
    {try{out=uwvm2::utils::container::make_shared<context>();++creations;return c::error::ok;}catch(...){return c::error::no_memory;}}
    c::error prepare_node(c::node_plan const&,std::shared_ptr<c::native_context> const&,c::node_stage_request const& request) noexcept override
    {
        if(!staged)return c::error::ok;
        ++stage_polls;if(pending_stage&&*pending_stage!=request)return c::error::denied;
        if(!pending_stage){pending_stage=request;++stage_launches;}
        return stage_ready?c::error::ok:c::error::again;
    }
    void retire_owner(c::owner) noexcept override{if(pending_stage){pending_stage.reset();++stage_retirements;}}
    void capability_closed(c::handle cap) noexcept override
    {if(pending_stage&&(pending_stage->node_capability==cap||pending_stage->context_capability==cap)){pending_stage.reset();++stage_retirements;}}
    void context_closed(c::owner owner,c::handle resource) noexcept override
    {if(pending_stage&&pending_stage->context_authority==owner&&pending_stage->context==resource){pending_stage.reset();++stage_retirements;}}
    c::error create_node(c::node_plan const&,std::shared_ptr<c::native_context> const&,std::shared_ptr<c::native_node>& out) noexcept override
    {try{out=uwvm2::utils::container::make_shared<node>();++creations;return c::error::ok;}catch(...){return c::error::no_memory;}}
    c::error create_topic(c::topic_plan const& plan,c::type_plan const& schema,std::shared_ptr<c::native_node> const&,c::handle id,
                         std::shared_ptr<c::receive_queue> const& received,std::shared_ptr<c::native_topic>& out) noexcept override
    {
        try{auto value=uwvm2::utils::container::make_shared<topic>();
            if(queue_dispatch&&plan.direction==c::role::publisher)
            {
                value->queued=uwvm2::utils::container::make_unique<s::communication_worker::mutations>();
                s::communication_jobs::plan bound{plan.authority,c::operation_family::topic,id,1,71,2,128,256,10000000000,896};
                auto check=[](void* context,c::operation_token const& token,s::group_lease& held)noexcept
                {return static_cast<c::error>(static_cast<s::communication_dispatch*>(context)->revalidate(token,held));};
                auto charge=[](void* context,c::operation_token const& token,std::uint64_t bytes,s::group_lease& held)noexcept
                {return static_cast<c::error>(static_cast<s::communication_dispatch*>(context)->charge_operation(token,bytes,held));};
                auto status=value->queued->configure(bound,check,charge,queue_dispatch);if(status!=c::error::ok)return status;
            }
            if(receiver_dispatch&&plan.direction==c::role::subscription)
            {
                auto status=configure_receiver(plan.authority,id,schema,s::communication_worker::receive_direction::topic,value->receiver);
                if(status!=c::error::ok)return status;
                value->evidence.provider_generation=71;
            }
            if(query_dispatch&&plan.direction==c::role::publisher)
            {
                value->observed=uwvm2::utils::container::make_unique<s::communication_worker::queries>();
                auto validate=[](void* context,c::operation_token const& token,s::group_lease& held)noexcept
                {return static_cast<c::error>(static_cast<s::communication_dispatch*>(context)->revalidate(token,held));};
                auto charge=[](void* context,c::operation_token const& token,std::uint64_t bytes,s::group_lease& held)noexcept
                {return static_cast<c::error>(static_cast<s::communication_dispatch*>(context)->charge_operation(token,bytes,held));};
                auto status=value->observed->configure({plan.authority,c::operation_family::topic,id,1,71,1,128,256,10000000000,768},validate,charge,query_dispatch);
                if(status!=c::error::ok)return status;
            }
            if(closing_dispatch&&plan.direction==c::role::publisher)
            {
                value->closure=uwvm2::utils::container::make_unique<s::communication_worker::endpoint_closure>();
                auto validate=[](void* context,c::operation_token const& token,s::group_lease& held)noexcept
                {return static_cast<c::error>(static_cast<s::communication_dispatch*>(context)->revalidate(token,held));};
                auto charge=[](void* context,c::operation_token const& token,std::uint64_t bytes,s::group_lease& held)noexcept
                {return static_cast<c::error>(static_cast<s::communication_dispatch*>(context)->charge_operation(token,bytes,held));};
                auto status=value->closure->configure({plan.authority,c::operation_family::topic,id,1,71,1,1,8,10000000000,9},9001,validate,charge,closing_dispatch);
                if(status!=c::error::ok)return status;
            }
            out=value;++creations;
            if(plan.direction==c::role::publisher)publisher=value;else{subscriber=value;queue=received;subscription=id;generation=plan.authority.generation;}return c::error::ok;}
        catch(...){return c::error::no_memory;}
    }
    c::error create_service(c::service_plan const& plan,c::type_plan const& request,c::type_plan const& response,std::shared_ptr<c::native_node> const&,c::handle id,
                           std::shared_ptr<c::native_service>& out) noexcept override
    {
        try
        {
            client=uwvm2::utils::container::make_shared<service>();
            if(receiver_dispatch)
            {
                bool is_response=plan.direction==c::service_role::client;
                client->correlations=uwvm2::utils::container::make_shared<s::communication_worker::correlations>();
                auto status=client->correlations->configure({plan.authority,id,10001,71,is_response?s::communication_worker::correlations::role::client:
                    s::communication_worker::correlations::role::server,2});if(status!=c::error::ok)return status;
                status=configure_receiver(plan.authority,id,is_response?response:request,is_response?s::communication_worker::receive_direction::service_response:
                    s::communication_worker::receive_direction::service_request,client->receiver,client->correlations);if(status!=c::error::ok)return status;
                client->endpoint_evidence.provider_generation=71;
            }
            if(receive_services)
            {
                client->records=uwvm2::utils::container::make_shared<c::receive_queue>(2,64,c::overflow::reject);
                if(client->records->bind(id,plan.authority.generation)!=c::error::ok)return c::error::invalid;
                client->response=plan.direction==c::service_role::client;client->endpoint_evidence.provider_generation=71;
                c::metadata info;info.source=id;info.generation=plan.authority.generation;std::array<std::uint8_t,1> body{7};
                if(client->records->push(info,body)!=c::error::ok)return c::error::io;
            }
            out=client;++creations;return c::error::ok;
        }
        catch(...){return c::error::no_memory;}
    }
};
static bytes encode(auto&& f)
{std::array<std::uint8_t,4096> storage{};w::writer out{storage};assert(f(out));return {out.written().begin(),out.written().end()};}
[[maybe_unused]] static bytes readiness(s::handle endpoint,s::handle source=0)
{return encode([&](auto& out){return out.u64(endpoint)&&out.u64(source);});}
[[maybe_unused]] static bytes resource(s::handle id){return encode([&](auto& out){return out.u64(id);});}
static bytes open_profile(std::string_view name){return encode([&](auto& out){return contract::encode_profile(out,profile(name));});}
static s::communication_dispatch::limits limits()
{return {{1,1,2,2,1024,256,1},8,4096,4096,32768,8};}
struct fixture
{
    std::shared_ptr<s::authority> authority=uwvm2::utils::container::make_shared<s::authority>();
    s::communication_dispatch dispatch{authority,111,3,limits()};
    std::shared_ptr<backend> native=uwvm2::utils::container::make_shared<backend>();
    int instance{};s::caller who{&instance,1};
    s::handle context_cap{},node_cap{},request_cap{},response_cap{},pub_cap{},sub_cap{},service_cap{};
    s::handle context_id{},node_id{},pub_id{},sub_id{},service_id{};
    explicit fixture(bool activate=true,bool open=true,uwvm2::utils::container::function_ref<void(s::authority&)> extra={},bool zero_topic_budget=false,bool zero_node_budget=false,bool context_close_budget=false,
                     bool service_observation_only=false,c::service_role service_direction=c::service_role::client,std::optional<std::uint64_t> topic_calls={})
    {
        assert(authority->expose("capability.open",s::exposure::allow)==error::success&&authority->expose("capability.close",s::exposure::allow)==error::success&&
            authority->expose("resource.close",s::exposure::allow)==error::success);
        for(auto const& item:contract::operations)assert(authority->expose(text(item.name),s::exposure::allow)==error::success);
        for(auto const& item:uwvm2::imported::ros::communication_receive_contract::operations)
            assert(authority->expose(text(item.name),s::exposure::allow)==error::success);
        auto grant=[&](std::string_view alias,s::purpose kind,std::initializer_list<std::string_view> actions)
        {
            s::grant value;value.alias=alias;value.resource=alias;value.kind=kind;value.binding="native-communication";value.max_resources=1;
            for(auto action:actions)value.actions.emplace(action);
            value.actions.emplace("resource.close");
            if(kind==s::purpose::service&&service_observation_only)value.actions={"service.open","service.describe","resource.close"};
            if(kind==s::purpose::context||kind==s::purpose::node){value.limits.calls=kind==s::purpose::context&&context_close_budget?2:1;value.limits.uncertain=true;value.limits.window=std::chrono::hours{1};}
            if(zero_node_budget&&kind==s::purpose::node)value.limits.calls=0;
            if(zero_topic_budget&&kind==s::purpose::topic){value.limits.calls=0;value.limits.uncertain=true;value.limits.window=std::chrono::hours{1};}
            if(topic_calls&&kind==s::purpose::topic){value.limits.calls=topic_calls;value.limits.uncertain=true;value.limits.window=std::chrono::hours{1};}
            assert(authority->configure(std::move(value))==error::success);
        };
        grant("context",s::purpose::context,{"context.open","context.describe","context.shutdown","resource.observe"});
        grant("node",s::purpose::node,{"node.open","node.describe","resource.observe"});
        grant("request",s::purpose::type,{"type.describe"});grant("response",s::purpose::type,{"type.describe"});
        grant("publisher",s::purpose::topic,{"topic.open","topic.publish","topic.describe","qos.describe","qos.liveliness.assert","qos.acknowledgements.wait"});
        grant("subscriber",s::purpose::topic,{"topic.open","topic.take","topic.describe","qos.describe","resource.observe","topic.reserve","topic.acknowledge","topic.release"});
        grant("service",s::purpose::service,{"service.open","service.submit","service.take","service.available","service.abandon","service.describe","qos.describe","service.reserve","service.acknowledge","service.release"});
        assert(dispatch.bind("context",c::context_plan{{},profile("ctx"),0,{},c::transport::unauthenticated_loopback})==error::success);
        assert(dispatch.bind("node",c::node_plan{{},profile("node"),profile("ctx"),"robot","/scene",2})==error::success);
        for(auto name:{"request","response"})
        {
            c::type_plan type;type.selected=profile(name);type.type_identity=name==std::string_view{"request"}?"test/Request":"test/Response";
            type.encoding="scene-canonical";type.descriptor={1,2,3};type.validate=valid_body;
            assert(dispatch.bind(name,std::move(type))==error::success);
        }
        assert(dispatch.bind("publisher",c::topic_plan{{},profile("pub"),profile("node"),profile("request"),"/scene/data",c::role::publisher,quality(),64,0,0,c::overflow::reject,10000000000ULL})==error::success);
        assert(dispatch.bind("subscriber",c::topic_plan{{},profile("sub"),profile("node"),profile("request"),"/scene/data",c::role::subscription,quality(),64,2,128,c::overflow::reject,10000000000ULL})==error::success);
        assert(dispatch.bind("service",c::service_plan{{},profile("srv"),profile("node"),profile("request"),profile("response"),"/scene/service","test/Service",service_direction,quality(),2,64,64,128,10000000000ULL})==error::success);
        if(activate)
        {
            s::communication_dispatch::observation_clock observed{nullptr,[](void*,contract::clock_epoch& epoch,std::int64_t& time) noexcept
                {epoch={999,0};time=0;return true;}};
            assert(dispatch.prepare(native,observed)==error::success);
        }
        if(extra)extra(*authority);
        assert(authority->enroll(who)==error::success);
        for(auto [alias,handle]:{std::pair{"context",&context_cap},{"node",&node_cap},{"request",&request_cap},{"response",&response_cap},
            {"publisher",&pub_cap},{"subscriber",&sub_cap},{"service",&service_cap}})
            assert(authority->open(who,alias,*handle)==error::success);
        if(open&&activate){context_id=opened(contract::operation::context_open,context_cap,open_profile("ctx"));
            auto input=encode([&](auto& out){return out.u64(context_cap)&&out.u64(context_id)&&contract::encode_profile(out,profile("node"));});
            node_id=opened(contract::operation::node_open,node_cap,input);}
    }
    error call(contract::operation action,s::handle cap,w::bytes input,bytes& result,std::size_t capacity=4096,s::communication_dispatch::sink sink={})
    {
        std::size_t required{};auto copy=[&](w::bytes bytes){result.assign(bytes.begin(),bytes.end());return true;};
        auto status=dispatch.invoke(who,action,cap,input,capacity,required,sink?sink:s::communication_dispatch::sink(copy));
        if(status==error::success)
        {
            // This trusted core factory still returns its immediate resource.
            // Public asynchronous opens use communication_open_contract receipts
            // and remain unregistered until the creation coordinator is wired.
            if(action==contract::operation::node_open||action==contract::operation::topic_open||action==contract::operation::service_open)
            {w::reader fields{result};s::handle id{};assert(fields.u64(id)&&id&&fields.done());}
            else{auto* definition=schema::find(contract::operations[static_cast<std::size_t>(action)].name);assert(definition&&schema::validate(result,definition->result,{4096,256,64,32}));}
        }
        return status;
    }
    s::handle opened(contract::operation action,s::handle cap,w::bytes input)
    {bytes output;assert(call(action,cap,input,output)==error::success);w::reader reader{output};s::handle id{};assert(reader.u64(id)&&reader.done());return id;}
    bytes endpoint(std::string_view selected,std::string_view role,s::handle nodecap=0,s::handle schema=0)
    {
        bool service=selected=="srv";
        return encode([&](auto& out){return out.u64(nodecap?nodecap:node_cap)&&out.u64(node_id)&&out.u64(service?2:1)&&out.u64(schema?schema:request_cap)&&
            (!service||out.u64(response_cap))&&contract::encode_profile(out,profile(selected))&&out.text(service?"/scene/service":"/scene/data")&&
            out.text(service?"test/Service":"test/Request")&&out.text(role);});
    }
    bytes mutation(s::handle id,std::uint64_t sequence,std::uint64_t deadline,std::uint8_t value=1)
    {return encode([&](auto& out){return out.u64(id)&&out.u64(sequence)&&out.u64(id)&&contract::encode_message(out,{"test/Request","scene-canonical",{&value,1}})&&out.u64(deadline);});}
    std::uint64_t admissions(std::string_view alias)
    {uwvm2::utils::container::std_vector<s::authority_snapshot> rows;assert(authority->inspect(rows,10));for(auto const& row:rows)if(row.alias==alias)return row.admissions;assert(false);return 0;}
};
#ifndef UWVM_COMMUNICATION_DISPATCH_FIXTURE_ONLY
static void node_staging_identity_and_budget()
{
    auto open_context=[](fixture& f){f.context_id=f.opened(contract::operation::context_open,f.context_cap,open_profile("ctx"));};
    auto request=[](fixture const& f,s::handle cap=0){return encode([&](auto& out){return out.u64(cap?cap:f.context_cap)&&out.u64(f.context_id)&&contract::encode_profile(out,profile("node"));});};
    {
        fixture f(true,false);open_context(f);f.native->staged=true;bytes out;auto input=request(f);
        for(unsigned attempt{};attempt<4;++attempt)assert(f.call(contract::operation::node_open,f.node_cap,input,out)==error::again);
        assert(f.native->stage_launches==1&&f.native->stage_polls==4&&f.native->creations==1&&f.admissions("node")==0);
        int other_instance{};s::caller other{&other_instance,1};assert(f.authority->enroll(other)==error::success);std::size_t required{};auto copy=[](w::bytes){assert(false);return true;};
        assert(f.dispatch.invoke(other,contract::operation::node_open,f.node_cap,input,4096,required,copy)==error::denied);
        s::caller another_generation{f.who.instance,f.who.generation+1};assert(f.authority->enroll(another_generation)==error::success);
        assert(f.dispatch.invoke(another_generation,contract::operation::node_open,f.node_cap,input,4096,required,copy)==error::denied);
        s::handle other_context_cap{};assert(f.authority->open(f.who,"context",other_context_cap)==error::success);
        assert(f.call(contract::operation::node_open,f.node_cap,request(f,other_context_cap),out)==error::denied&&f.native->stage_launches==1&&f.admissions("node")==0);
        f.native->stage_ready=true;f.node_id=f.opened(contract::operation::node_open,f.node_cap,input);
        assert(f.native->creations==2&&f.admissions("node")==1&&f.admissions("context")==1);
    }
    {
        fixture zero(true,false,{},false,true);open_context(zero);zero.native->staged=true;bytes out;
        assert(zero.call(contract::operation::node_open,zero.node_cap,request(zero),out)==error::again&&!zero.native->stage_polls&&!zero.native->stage_launches);
    }
    for(unsigned ending:{0u,1u,2u,3u})
    {
        fixture f(true,false,{},false,false,ending==1);open_context(f);f.native->staged=true;bytes out;
        assert(f.call(contract::operation::node_open,f.node_cap,request(f),out)==error::again&&f.native->pending_stage);
        if(ending==0){assert(f.authority->close(f.who,f.node_cap)==error::success);f.dispatch.capability_closed(f.node_cap);}
        if(ending==1)assert(f.dispatch.close(f.who,f.context_cap,f.context_id)==error::success);
        if(ending==2)assert(f.authority->revoke("node",[&](auto alias){f.dispatch.revoke(alias);})==error::success);
        if(ending==3)f.dispatch.retire();
        assert(!f.native->pending_stage&&f.native->stage_retirements==1&&f.native->creations==1);
    }
}
static void cancellation_reentrant_origins()
{
    // Returning false must not preserve a stale recursive admission lease.
    for(bool local:{false,true})for(auto alias:{"context","node","request","publisher"})for(unsigned phase:{1u,2u,3u})
    {
        fixture f;unsigned callbacks{},copies{};std::size_t required{};
        auto cancel=[&]{if(++callbacks==phase){if(local)f.dispatch.revoke(alias);else assert(f.authority->revoke(alias,[&](auto name){f.dispatch.revoke(name);})==error::success);}return false;};
        auto copy=[&](w::bytes){++copies;return true;};auto input=f.endpoint("pub","publisher");
        auto result=f.dispatch.invoke(f.who,contract::operation::topic_open,f.pub_cap,input,4096,required,copy,cancel);
        assert(result!=error::success&&callbacks>=phase&&!copies);
        if(phase<=2)assert(f.native->creations==2);
    }
    for(auto alias:{"context","node","request","publisher"})for(unsigned phase:{2u,3u})
    {
        fixture f;f.pub_id=f.opened(contract::operation::topic_open,f.pub_cap,f.endpoint("pub","publisher"));
        unsigned callbacks{},copies{};std::size_t required{};
        auto cancel=[&]{if(++callbacks==phase)assert(f.authority->revoke(alias,[&](auto name){f.dispatch.revoke(name);})==error::success);return false;};
        auto copy=[&](w::bytes){++copies;return true;};auto input=f.mutation(f.pub_id,1,c::now()+1000000000ULL);
        assert(f.dispatch.invoke(f.who,contract::operation::topic_publish,f.pub_cap,input,4096,required,copy,cancel)!=error::success&&!copies);
        assert(f.native->publisher->publications==(phase==2?0u:1u));
    }
}
static void explicit_origins_and_capacity()
{
    fixture inert(false,false);bytes result;assert(inert.call(contract::operation::context_open,inert.context_cap,open_profile("ctx"),result)==error::denied&&!inert.native->creations);
    fixture zero(true,true,{},true);assert(zero.call(contract::operation::topic_open,zero.pub_cap,zero.endpoint("pub","publisher"),result)==error::again&&zero.native->creations==2);
    fixture cancelled;auto cancelled_input=cancelled.endpoint("pub","publisher");std::size_t required{};auto stop=[]{return true;};auto copy=[](w::bytes){return true;};
    assert(cancelled.dispatch.invoke(cancelled.who,contract::operation::topic_open,cancelled.pub_cap,cancelled_input,4096,required,copy,stop)==error::cancelled&&cancelled.native->creations==2);
    fixture f;auto before=f.native->creations;
    assert(f.call(contract::operation::topic_open,f.pub_cap,f.endpoint("pub","publisher",f.context_cap),result)==error::denied&&f.native->creations==before);
    assert(f.call(contract::operation::topic_open,f.pub_cap,f.endpoint("pub","publisher",0,f.response_cap),result)==error::denied&&f.native->creations==before);
    assert(f.call(contract::operation::topic_open,f.pub_cap,f.endpoint("pub","subscription"),result)==error::denied&&f.native->creations==before);
    auto charged=f.admissions("publisher");assert(f.call(contract::operation::topic_open,f.pub_cap,f.endpoint("pub","publisher"),result,7)==error::too_large&&f.admissions("publisher")==charged);
    f.pub_id=f.opened(contract::operation::topic_open,f.pub_cap,f.endpoint("pub","publisher"));
    f.sub_id=f.opened(contract::operation::topic_open,f.sub_cap,f.endpoint("sub","subscription"));
    f.service_id=f.opened(contract::operation::service_open,f.service_cap,f.endpoint("srv","client"));
    assert(f.admissions("node")==1&&f.admissions("context")==1&&f.admissions("request")==0&&f.admissions("response")==0);
    auto deadline=c::now()+1000000000;assert(f.call(contract::operation::topic_publish,f.pub_cap,f.mutation(f.pub_id,1,deadline),result)==error::success);
    assert(f.admissions("node")==1&&f.admissions("context")==1&&f.native->publisher->publications==1);
    assert(f.authority->close(f.who,f.context_cap)==error::success);
    assert(f.call(contract::operation::topic_publish,f.pub_cap,f.mutation(f.pub_id,1,deadline),result)==error::success&&f.native->publisher->publications==1);
    assert(f.call(contract::operation::topic_publish,f.pub_cap,f.mutation(f.pub_id,2,deadline),result)==error::bad_handle&&f.native->publisher->publications==1);
}
static void lost_open_mutation_and_take()
{
    fixture f;bytes result;auto fail=[](w::bytes){return false;};auto throwing=[](w::bytes)->bool{throw 1;};
    assert(f.call(contract::operation::topic_open,f.pub_cap,f.endpoint("pub","publisher"),result,4096,throwing)==error::io);
    s::communication_dispatch::usage usage;assert(f.dispatch.inspect("publisher",usage)==error::success&&!usage.handles&&!usage.orphaned);
    f.pub_id=f.opened(contract::operation::topic_open,f.pub_cap,f.endpoint("pub","publisher"));auto deadline=c::now()+1000000000;auto input=f.mutation(f.pub_id,1,deadline);
    assert(f.call(contract::operation::topic_publish,f.pub_cap,input,result,159)==error::too_large&&!f.native->publisher->publications);
    assert(f.call(contract::operation::topic_publish,f.pub_cap,input,result,4096,fail)==error::io&&f.native->publisher->publications==1);
    assert(f.call(contract::operation::topic_publish,f.pub_cap,input,result)==error::success&&f.native->publisher->publications==1);
    auto changed=f.mutation(f.pub_id,1,deadline,2);assert(f.call(contract::operation::topic_publish,f.pub_cap,changed,result)==error::conflict);
    f.sub_id=f.opened(contract::operation::topic_open,f.sub_cap,f.endpoint("sub","subscription"));
    std::array<std::uint8_t,3> payload{1,2,3};c::metadata meta;meta.source=f.native->subscription;meta.generation=f.native->generation;
    assert(f.native->queue->push(meta,payload)==c::error::ok);auto charges=f.admissions("subscriber");
    assert(f.call(contract::operation::topic_take,f.sub_cap,resource(f.sub_id),result,1)==error::too_large&&f.admissions("subscriber")==charges);
    assert(f.call(contract::operation::topic_take,f.sub_cap,resource(f.sub_id),result,4096,fail)==error::io);
    assert(f.call(contract::operation::topic_take,f.sub_cap,resource(f.sub_id),result)==error::success);
    assert(f.call(contract::operation::topic_take,f.sub_cap,resource(f.sub_id),result)==error::again);
}
static void node_attachment_and_reentrant_revoke()
{
    fixture f;c::node_lease attachment;assert(f.dispatch.attach_node(f.who,f.node_cap,f.node_id,profile("node"),attachment)==error::success);
    assert(attachment.live()&&attachment.authority().environment==111&&attachment.authority().generation==3);
    assert(f.dispatch.validate_node(f.who,f.node_cap,f.node_id,profile("node"),attachment.authority())==error::success);
    auto wrong=attachment.authority();++wrong.grant;assert(f.dispatch.validate_node(f.who,f.node_cap,f.node_id,profile("node"),wrong)==error::denied);
    f.sub_id=f.opened(contract::operation::topic_open,f.sub_cap,f.endpoint("sub","subscription"));bytes result;
    c::metadata meta;meta.source=f.native->subscription;meta.generation=f.native->generation;std::array<std::uint8_t,1> body{1};
    assert(f.native->queue->push(meta,body)==c::error::ok);
    auto revoke=[&](w::bytes)
    {assert(f.authority->revoke("request",[&](auto alias){f.dispatch.revoke(alias);})==error::success);return true;};
    assert(f.call(contract::operation::topic_take,f.sub_cap,resource(f.sub_id),result,4096,revoke)==error::denied);
    assert(f.call(contract::operation::topic_take,f.sub_cap,resource(f.sub_id),result)==error::denied);
    assert(f.authority->revoke("context",[&](auto alias){f.dispatch.revoke(alias);})==error::success&&!attachment.live());
    assert(f.dispatch.validate_node(f.who,f.node_cap,f.node_id,profile("node"),attachment.authority())==error::denied);
}
static void schemas_observation_and_unknown()
{
    fixture f;bytes result;auto request=encode([](auto& out){return out.text("test/Request")&&out.boolean(false)&&out.u64(1);});
    assert(f.call(contract::operation::type_describe,f.request_cap,request,result)==error::success);
    f.pub_id=f.opened(contract::operation::topic_open,f.pub_cap,f.endpoint("pub","publisher"));
    assert(f.call(contract::operation::topic_describe,f.pub_cap,resource(f.pub_id),result)==error::success);
    assert(f.call(contract::operation::qos_describe,f.pub_cap,resource(f.pub_id),result)==error::success);
    f.service_id=f.opened(contract::operation::service_open,f.service_cap,f.endpoint("srv","client"));
    assert(f.call(contract::operation::service_available,f.service_cap,readiness(f.service_id),result)==error::success);
    w::reader reader{result};bool available{};std::uint64_t clock{},epoch{};std::int64_t time{};
    assert(reader.boolean(available)&&available&&reader.u64(clock)&&clock==999&&reader.u64(epoch)&&epoch==0&&reader.i64(time)&&time==0&&reader.done());
    assert(f.call(contract::operation::qos_describe,f.service_cap,resource(f.service_id),result)==error::success);
    f.native->publisher->unknown=true;auto deadline=c::now()+1000000000;auto mutation=f.mutation(f.pub_id,1,deadline);
    assert(f.call(contract::operation::topic_publish,f.pub_cap,mutation,result)==error::success&&f.native->publisher->publications==1);
    assert(f.call(contract::operation::topic_publish,f.pub_cap,mutation,result)==error::success&&f.native->publisher->publications==1);
    assert(f.call(contract::operation::topic_publish,f.pub_cap,f.mutation(f.pub_id,2,deadline),result)==error::again);
    assert(f.dispatch.close(f.who,f.pub_cap,f.pub_id)==error::again);
}
static void fork_and_workspace()
{
    fixture f;s::communication_dispatch::workspace_lease first,second;
    assert(f.dispatch.reserve_workspace(f.who,contract::operation::topic_publish,f.pub_cap,4096,first)==error::success);
    assert(f.dispatch.reserve_workspace(f.who,contract::operation::topic_publish,f.pub_cap,4097,second)==error::too_large);
    s::communication_dispatch::usage use;assert(f.dispatch.inspect({},use)==error::success&&use.workspace_bytes==4096&&use.workspace_calls==1);
    auto pid=::fork();assert(pid>=0);if(!pid)
    {
        assert(f.dispatch.preflight(f.who,contract::operation::topic_open,f.pub_cap,1)==error::denied);
        first.reset();assert(f.dispatch.inspect({},use)==error::denied);f.dispatch.retire();::_exit(0);
    }
    int result{};assert(::waitpid(pid,&result,0)==pid&&WIFEXITED(result)&&!WEXITSTATUS(result));first.reset();
    assert(f.dispatch.inspect({},use)==error::success&&!use.workspace_bytes&&!use.workspace_calls);
}
static void current_operation_origins()
{
    fixture f;bytes result;
    f.pub_id=f.opened(contract::operation::topic_open,f.pub_cap,f.endpoint("pub","publisher"));
    s::handle current{};assert(f.authority->open(f.who,"publisher",current)==error::success&&current!=f.pub_cap);
    assert(f.call(contract::operation::topic_publish,current,f.mutation(f.pub_id,1,c::now()+1000000000),result)==error::success);
    auto token=f.native->publisher->last_operation;
    assert(token.capability==current&&token.caller_instance==f.who.instance&&token.caller_generation==f.who.generation&&
        token.request&&token.session==f.pub_id&&token.sequence==1&&token.resource==f.pub_id&&token.count==4);
    assert(token.origins[1].capability==f.node_cap&&token.origins[1].resource==f.node_id&&
        token.origins[2].capability==f.context_cap&&token.origins[2].resource==f.context_id&&
        token.origins[3].capability==f.request_cap&&!token.origins[3].resource);
    auto before=f.admissions("publisher");s::group_lease held;
    assert(f.dispatch.revalidate(token,held)==error::success);held.release();
    assert(f.admissions("publisher")==before);
    assert(f.dispatch.charge_operation(token,355,held)==error::success);
    std::array<s::admission_claim,4> costs{};
    for(std::size_t i{};i<4;++i)costs[i]={token.origins[i].capability,c::permission_name(token.origins[i].action),i?0u:355u,false,i!=0};
    assert(held.matches(f.who,costs)&&held.consume(f.who,costs)&&!held.consume(f.who,costs));held.release();
    assert(f.admissions("publisher")==before+1&&f.admissions("node")==1&&f.admissions("context")==1);
    // Shape-valid but altered origins must not authorize queue progress.
    auto forged=token;forged.origins[3].capability=f.response_cap;
    assert(f.dispatch.revalidate(forged,held)==error::denied&&!held);
    assert(f.dispatch.charge_operation(forged,355,held)==error::denied&&!held&&f.admissions("publisher")==before+1);
    forged=token;forged.origins[1].resource=f.context_id;
    assert(f.dispatch.revalidate(forged,held)==error::denied&&!held);
    forged=token;forged.caller_generation++;
    assert(f.dispatch.revalidate(forged,held)==error::denied&&!held);
    // Closing the original opener does not silently replace the actual caller.
    assert(f.authority->close(f.who,f.pub_cap)==error::success);
    assert(f.dispatch.revalidate(token,held)==error::success);held.release();
    assert(f.authority->close(f.who,current)==error::success);
    assert(f.dispatch.revalidate(token,held)!=error::success&&!held);
    s::handle replacement{};assert(f.authority->open(f.who,"publisher",replacement)==error::success);
    assert(f.call(contract::operation::topic_publish,replacement,f.mutation(f.pub_id,2,c::now()+1000000000),result)==error::success);
    token=f.native->publisher->last_operation;
    assert(token.capability==replacement&&token.capability!=current);
    assert(f.authority->revoke("request",[&](auto alias){f.dispatch.revoke(alias);})==error::success);
    assert(f.dispatch.revalidate(token,held)!=error::success&&!held);
}
static void observed_endpoint_identity()
{
    fixture f;f.pub_id=f.opened(contract::operation::topic_open,f.pub_cap,f.endpoint("pub","publisher"));bytes result;
    auto observed=[&](bool present)
    {
        auto inspections=f.native->publisher->inspections.load();
        assert(f.call(contract::operation::topic_describe,f.pub_cap,resource(f.pub_id),result)==error::success&&f.native->publisher->inspections==inspections);
        w::reader in{result};std::uint64_t node{},generation{};std::string_view name,type,role;contract::profile_view selected;
        assert(in.u64(node)&&node==f.node_id&&contract::read_profile(in,selected)&&contract::matches(selected,profile("pub"))&&
            in.text(name)&&name=="/scene/data"&&in.text(type)&&type=="test/Request"&&in.text(role)&&role=="publisher");
        bool has{};assert(in.boolean(has)&&has==present);
        if(present)assert(in.u64(generation)&&generation==777);
        assert(in.boolean(has)&&has==present);w::bytes native;
        if(present)assert(in.blob(native)&&native.size()==16&&native[0]==91);
        assert(in.done());
    };
    observed(false);
    auto& identity=f.native->publisher->evidence;identity.provider_generation=777;identity.native[0]=91;identity.size=16;observed(true);
    auto admissions=f.admissions("publisher");identity.provider_generation=0;
    assert(f.call(contract::operation::topic_describe,f.pub_cap,resource(f.pub_id),result)==error::io&&f.admissions("publisher")==admissions);
    identity.provider_generation=777;identity.size=33;
    assert(f.call(contract::operation::topic_describe,f.pub_cap,resource(f.pub_id),result)==error::io);
    identity.size=16;identity.native.fill(0);
    assert(f.call(contract::operation::topic_describe,f.pub_cap,resource(f.pub_id),result)==error::io);
    identity={};observed(false);
}
static void independent_service_description()
{
    for(bool server:{false,true})
    {
        fixture f{true,true,{},false,false,false,true,server?c::service_role::server:c::service_role::client};bytes result;
        f.service_id=f.opened(contract::operation::service_open,f.service_cap,f.endpoint("srv",server?"server":"client"));
        auto describe=[&](bool first,bool second)
        {
            auto calls=f.native->client->inspections.load();
            assert(f.call(contract::operation::service_describe,f.service_cap,resource(f.service_id),result)==error::success&&f.native->client->inspections==calls);
            w::reader in{result};std::uint64_t number{};contract::profile_view selected;std::string_view name,type,role;
            assert(in.u64(number)&&number==f.node_id&&contract::read_profile(in,selected)&&contract::matches(selected,profile("srv"))&&
                in.text(name)&&name=="/scene/service"&&in.text(type)&&type=="test/Service"&&in.text(role)&&role==(server?"server":"client"));
            bool present{};assert(in.boolean(present)&&!present&&in.boolean(present)&&!present&&in.u64(number)&&number==2);
            auto qos_depth=[](contract::message_view const& message,std::uint64_t expected)
            {
                assert(message.identity=="uwvm/qos/Profile"&&message.encoding=="scene-canonical");w::reader body{message.body};
                for(unsigned i{};i<11;++i){std::uint64_t item{};assert(body.u64(item));if(i==4)assert(item==expected);}assert(body.done());
            };
            for(unsigned leg{};leg<2;++leg)
            {
                assert(in.text(name)&&name==(leg?(server?"response_transmit":"response_receive"):(server?"request_receive":"request_transmit")));
                contract::message_view message;assert(contract::read_message(in,message));qos_depth(message,2);
                assert(in.boolean(present)&&present==(leg?second:first));
                if(present){assert(contract::read_message(in,message));qos_depth(message,leg?3:2);}
                assert(in.u64(number)&&number==(present?8u:0u));for(std::uint64_t i{};i<number;++i)assert(in.text(name)&&!name.empty());
            }
            assert(in.done());
        };
        describe(false,false);f.native->client->evidence[0]=quality();describe(true,false);
        auto response=quality();response.depth=3;f.native->client->evidence[1]=response;describe(true,true);
        auto admissions=f.admissions("service");
        assert(f.call(contract::operation::service_describe,f.service_cap,resource(f.service_id),result,1)==error::too_large&&f.admissions("service")==admissions);
        assert(f.call(contract::operation::service_available,f.service_cap,readiness(f.service_id),result)==error::denied);
        assert(f.call(contract::operation::service_take,f.service_cap,resource(f.service_id),result)==error::denied);
        assert(f.call(contract::operation::qos_describe,f.service_cap,resource(f.service_id),result)==error::denied);
        f.native->client->evidence[1]->history=999;
        assert(f.call(contract::operation::service_describe,f.service_cap,resource(f.service_id),result)==error::io&&f.admissions("service")==admissions);
        f.native->client->evidence[1]=response;
        assert(f.authority->revoke("response",[&](auto alias){f.dispatch.revoke(alias);})==error::success);
        assert(f.call(contract::operation::service_describe,f.service_cap,resource(f.service_id),result)!=error::success);
    }
}
static void observation_preflight_and_reentrant_provider()
{
    fixture f;bytes result;
    f.service_id=f.opened(contract::operation::service_open,f.service_cap,f.endpoint("srv","client"));
    assert(f.call(contract::operation::service_available,f.service_cap,readiness(f.service_id),result,31)==error::too_large&&!f.native->client->availability_calls);
    assert(f.call(contract::operation::qos_describe,f.service_cap,resource(f.service_id),result,1)==error::too_large&&!f.native->client->inspections);
    auto callback=[&]
    {
        bytes nested;
        assert(f.dispatch.close(f.who,f.service_cap,f.service_id)==error::again);
        assert(f.call(contract::operation::service_available,f.service_cap,readiness(f.service_id),nested)==error::again);
    };
    f.native->client->during_available=callback;
    assert(f.call(contract::operation::service_available,f.service_cap,readiness(f.service_id),result)==error::success&&f.native->client->availability_calls==1);
    auto revoke=[&]{assert(f.authority->revoke("response",[&](auto alias){f.dispatch.revoke(alias);})==error::success);};
    f.native->client->during_available=revoke;
    assert(f.call(contract::operation::service_available,f.service_cap,readiness(f.service_id),result)==error::cancelled&&f.native->client->closed);
    f.native->client->during_available={};
    fixture exhausted(true,true,{},false,false,false,false,c::service_role::client,1);
    exhausted.pub_id=exhausted.opened(contract::operation::topic_open,exhausted.pub_cap,exhausted.endpoint("pub","publisher"));
    assert(exhausted.call(contract::operation::qos_describe,exhausted.pub_cap,resource(exhausted.pub_id),result)==error::again&&!exhausted.native->publisher->inspections);
}
static void paired_service_readiness()
{
    auto configure=[](s::authority& grants)
    {
        for(auto name:{"observer","other"})
        {
            s::grant policy;policy.kind=s::purpose::service;policy.binding="service-observer";policy.alias=name;
            policy.resource=name==std::string_view{"observer"}?"service":"other-client";policy.actions.emplace("service.available");
            assert(grants.configure(std::move(policy))==error::success);
        }
    };
    for(bool retire_source:{false,true})
    {
        fixture f(true,true,configure);
        auto id=f.opened(contract::operation::service_open,f.service_cap,f.endpoint("srv","client"));
        s::handle observer{},other{};
        assert(f.authority->open(f.who,"observer",observer)==error::success);
        assert(f.authority->open(f.who,"other",other)==error::success);
        bytes result;
        auto call=[&](s::handle query,s::handle source){return f.call(contract::operation::service_available,query,readiness(id,source),result);};
        assert(call(observer,0)==error::denied && call(f.service_cap,f.service_cap)==error::denied);
        assert(call(observer,observer)==error::denied && call(observer,f.node_cap)==error::denied);
        assert(call(other,f.service_cap)==error::denied);
        assert(f.call(contract::operation::service_available,observer,resource(id),result)==error::invalid);
        assert(!f.native->client->availability_calls);
        auto count=[&](std::string_view name)
        {
            uwvm2::utils::container::std_vector<s::authority_snapshot> rows;
            assert(f.authority->inspect(rows,32));for(auto const& row:rows)if(row.alias==name)return row.admissions;
            assert(false);return std::uint64_t{};
        };
        auto original=count("service");
        assert(call(observer,f.service_cap)==error::success && f.native->client->availability_calls==1 && result.size()==32);
        assert(count("service")==original && count("observer")==1);
        result.clear();
        s::service_observation_audit::frame captured{true};
        auto revoke=[&]{assert(f.authority->revoke(retire_source?"service":"observer",[&](auto alias){f.dispatch.revoke(alias);})==error::success);};
        f.native->client->during_available=revoke;
        assert(call(observer,f.service_cap)!=error::success && result.empty() && f.native->client->availability_calls==2);
        f.native->client->during_available={};
        auto metadata=s::service_observation_audit::current;
        assert((std::string_view{metadata->observer.data(),metadata->observer_length}=="observer"));
        assert((std::string_view{metadata->source.data(),metadata->source_length}=="service"));
        assert(metadata->lost==(retire_source?s::service_observation_audit::withdrawal::source:
                                                s::service_observation_audit::withdrawal::observer));
        s::call_audit::journal journal;
        assert(journal.add(s::call_audit::scope::global,"*",false,{3,8192,100,3}));
        std::string entry;
        auto capture=[&](std::string_view line){entry.assign(line);return true;};
        journal.observe("robot","application",1,"service.available",13,1,capture);
        assert(entry.find("\"observer_hex\":\"6f62736572766572\"")!=entry.npos);
        assert(entry.find("\"source_hex\":\"73657276696365\"")!=entry.npos);
        assert(entry.find(retire_source?"\"authority_lost\":\"source\"":"\"authority_lost\":\"observer\"")!=entry.npos);
        {
            s::service_observation_audit::frame nested{true};entry.clear();
            assert(call(other,f.service_cap)==error::denied);
            journal.observe("robot","application",1,"service.available",13,2,capture);
            assert(!entry.empty()&&entry.find("observer_hex")==entry.npos&&entry.find("source_hex")==entry.npos);
        }
        if(!retire_source)assert(call(f.service_cap,0)==error::success && !f.native->client->closed);
    }
}
int main()
{
    node_staging_identity_and_budget();cancellation_reentrant_origins();explicit_origins_and_capacity();lost_open_mutation_and_take();node_attachment_and_reentrant_revoke();schemas_observation_and_unknown();fork_and_workspace();current_operation_origins();
    observed_endpoint_identity();
    independent_service_description();
    observation_preflight_and_reentrant_provider();
    paired_service_readiness();
    std::cout<<"passed 12 communication dispatch security groups\n";
}

#endif
