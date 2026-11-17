/** @file @brief Real host action ledger, bounded isolated facade and nonblocking private pipes. */
#define main communication_core_fixture_entry
#include "communication_core.cpp"
#undef main
#include <uwvm2/imported/ros/scene/communication_worker_action.h>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;
namespace act=c::actions;
namespace worker=s::communication_worker;
namespace rpc=s::communication_worker_protocol;
namespace contract=uwvm2::imported::ros::communication_contract;
namespace ac=uwvm2::imported::ros::action_contract;
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
struct action_capture
{
    bool accept{true};act::delivery event;bytes body;
    static bool copy(void* data,act::delivery const& event)
    {auto& self=*static_cast<action_capture*>(data);self.event=event;self.body.assign(event.body.begin(),event.body.end());return self.accept;}
};
struct action_environment
{
    std::array<int,2> input{},output{},diagnostics{};int identity{};s::communication_worker_async::transport transport;
    s::authority permissions;int instance{};s::caller who{&instance,1};std::array<s::handle,6> caps{};
    std::shared_ptr<test_backend> backend=uwvm2::utils::container::make_shared<test_backend>();
    c::registry registry{backend,{1,1,1,1,128,64}};
    std::shared_ptr<worker::action_endpoint> endpoint=uwvm2::utils::container::make_shared<worker::action_endpoint>();
    std::shared_ptr<act::endpoint> core=uwvm2::utils::container::make_shared<act::endpoint>();
    c::owner owner{17,1,1},node_owner{17,2,1},context_owner{17,3,1};
    c::profile context{profile("ctx",1)},node{profile("node",2)};
    c::handle context_id{},node_id{},resource{};act::plan policy;
    std::shared_ptr<int> retained=uwvm2::utils::container::make_shared<int>(1);
    unsigned charges{};std::uint64_t wire_sequence{},event_sequence{};
    explicit action_environment(bool server=false,std::optional<std::uint64_t> quota={})
    {
        assert(!::pipe2(input.data(),O_CLOEXEC|O_NONBLOCK)&&!::pipe2(output.data(),O_CLOEXEC|O_NONBLOCK)&&!::pipe2(diagnostics.data(),O_CLOEXEC|O_NONBLOCK));
        identity=static_cast<int>(::syscall(SYS_pidfd_open,::getpid(),0));assert(identity>=0);
        assert(transport.prepare({input[1],output[0],diagnostics[0],identity},{128,2048,2048,32,1000000000,1000000},71)==c::error::ok);
        constexpr std::array operations{"action.submit","action.respond","action.cancel","action.take","resource.close"};
        for(auto name:operations)assert(permissions.expose(name,s::exposure::allow)==s::error::success);
        for(auto name:ac::transition_actions)assert(permissions.expose(uwvm2::utils::container::std_string{name},s::exposure::allow)==s::error::success);
        for(auto name:{"capability.open","capability.close","node.open","context.open","type.describe"})assert(permissions.expose(name,s::exposure::allow)==s::error::success);
        constexpr std::array aliases{"action","node","context","goal","feedback","result"};
        constexpr std::array kinds{s::purpose::action,s::purpose::node,s::purpose::context,s::purpose::type,s::purpose::type,s::purpose::type};
        for(std::size_t i{};i<caps.size();++i)
        {
            s::grant grant;grant.alias=aliases[i];grant.resource="action.fixture";grant.binding="private-worker";grant.kind=kinds[i];
            if(!i){for(auto name:operations)grant.actions.emplace(name);for(auto name:ac::transition_actions)grant.actions.emplace(name);
                grant.limits.calls=quota;grant.limits.uncertain=quota.has_value();grant.limits.window=std::chrono::hours{1};}
            else grant.actions.emplace(i==1?"node.open":i==2?"context.open":"type.describe");
            assert(permissions.configure(std::move(grant))==s::error::success);
        }
        assert(permissions.enroll(who)==s::error::success);for(std::size_t i{};i<caps.size();++i)assert(permissions.open(who,aliases[i],caps[i])==s::error::success);
        assert(registry.configure(c::context_plan{context_owner,context,4,{},c::transport::unauthenticated_loopback})==c::error::ok);
        assert(registry.configure(c::node_plan{node_owner,node,context,"action","/scene",1})==c::error::ok);
        assert(registry.open_context(context_owner,context,context_id)==c::error::ok&&registry.open_node(node_owner,context_owner,context_id,node,node_id)==c::error::ok);
        policy.authority=owner;policy.selected=profile("action",3);policy.resolved_name="/scene/action";policy.action_type="test/Action";policy.direction=server?act::role::server:act::role::client;
        policy.goal_quality=policy.result_quality=policy.cancel_quality=policy.feedback_quality=policy.status_quality={1,1,2,1,2,{},{},{}};
        policy.goal={profile("goal",4),"test/Goal","canonical",{},bytes_valid,{17,4,1}};
        policy.feedback={profile("feedback",5),"test/Feedback","canonical",{},bytes_valid,{17,5,1}};
        policy.result={profile("result",6),"test/Result","canonical",{},bytes_valid,{17,6,1}};
        policy.max_goals=2;policy.max_cancels_per_goal=2;policy.feedback_slots=1;policy.max_goal_bytes=policy.max_feedback_bytes=policy.max_result_bytes=1;
        policy.reserved_bytes=4096;policy.transition_mask=510;policy.max_deadline_ns=1000000000;policy.retention_ns=1000000000;
        resource=uwvm2::imported::ros::resources::allocate_identity();c::node_lease parent;assert(registry.attach_node(node_owner,node_id,node,parent)==c::error::ok);
        auto reserved=worker::action_endpoint::reservation_bytes(policy,128,2048);
        assert(endpoint->configure(policy,std::move(parent),{owner,c::operation_family::action,resource,1,71,1,128,2048,1000000000,reserved},9001,admit,charge,this)==c::error::ok);
        assert(core->configure(policy,resource,endpoint)==c::error::ok&&endpoint->attach(core)==c::error::ok);
    }
    ~action_environment(){endpoint->retire();transport.retire();for(auto fd:{input[0],input[1],output[0],output[1],diagnostics[0],diagnostics[1],identity})::close(fd);}
    c::operation_token token(c::permission operation,c::permission transition=c::permission::unconfigured)const
    {
        c::operation_token value;value.caller_instance=who.instance;value.caller_generation=who.generation;value.authority=owner;value.resource=resource;
        value.capability=caps[0];value.family=c::operation_family::action;value.action=operation;value.secondary=transition;value.count=transition==c::permission::unconfigured?6:7;value.retained_origins=retained;
        value.origins[0]={owner,caps[0],resource,operation};value.origins[1]={node_owner,caps[1],node_id,c::permission::node_open};
        value.origins[2]={context_owner,caps[2],context_id,c::permission::context_open};
        for(std::size_t i{3};i<6;++i)value.origins[i]={{17,i+1,1},caps[i],0,c::permission::type_describe};
        if(value.count==7)value.origins[6]={owner,caps[0],resource,transition};
        return value;
    }
    static c::error authorize(void* data,c::operation_token const& origin,std::uint64_t extent,s::group_lease& held,bool charge)noexcept
    {
        auto& self=*static_cast<action_environment*>(data);auto expected=self.token(origin.action,origin.secondary);
        if(!origin.valid(origin.action,c::operation_family::action,self.owner,self.resource,false)||origin.caller_instance!=self.who.instance||
            origin.caller_generation!=self.who.generation||origin.origins!=expected.origins||origin.retained_origins!=self.retained)return c::error::denied;
        s::communication_jobs::claim_set claims;auto status=self.permissions.admit_group(self.who,s::communication_jobs::admission_claims(origin,claims,extent),held,charge);
        if(status==s::error::success&&charge)++self.charges;
        return static_cast<c::error>(status);
    }
    static c::error admit(void* data,c::operation_token const& token,s::group_lease& held)noexcept{return authorize(data,token,0,held,false);}
    static c::error charge(void* data,c::operation_token const& token,std::uint64_t bytes,s::group_lease& held)noexcept{return authorize(data,token,bytes,held,true);}
    bytes send(rpc::operation expected)
    {
        assert(endpoint->pump(transport)==c::error::ok);std::array<std::uint8_t,256> bytes;auto size=::read(input[0],bytes.data(),bytes.size());rpc::request value;
        assert(size>0&&rpc::decode_request({bytes.data(),static_cast<std::size_t>(size)},71,128,value)&&value.action==expected);
        wire_sequence=value.sequence;return {value.body.begin(),value.body.end()};
    }
    void reply(c::error status,c::effect effect,s::wire::bytes body)
    {
        std::array<std::uint8_t,4096> bytes;s::wire::writer out{bytes};assert(out.u64(rpc::reply_magic)&&out.u64(71)&&out.u64(wire_sequence)&&
            out.u64(static_cast<std::uint64_t>(status))&&out.u64(static_cast<std::uint64_t>(effect))&&out.u64(body.size())&&out.encoded(body));
        assert(::write(output[1],bytes.data(),out.size())==static_cast<ssize_t>(out.size())&&endpoint->pump(transport)==c::error::ok);
    }
    c::native_result complete(c::handle native,bool malformed=false)
    {
        std::array<std::uint8_t,256> bytes;s::wire::writer out{bytes};assert(contract::encode_receipt(out,{native,malformed?72u:71u,{},c::error::ok,c::effect::locally_accepted}));
        reply(c::error::ok,c::effect::locally_accepted,out.written());c::operation_token original;c::native_result result;
        assert(endpoint->completion(original,result)==c::error::ok&&core->settle_operation(original.request,result)==c::error::ok);
        assert(endpoint->release_completion(original)==(malformed?c::error::busy:c::error::ok));return result;
    }
    c::error take(action_capture& into)
    {auto origin=token(c::permission::action_take);return core->take(owner,&into,action_capture::copy,&origin);}
    void event(act::event_kind kind,act::state state,c::handle goal,c::handle correlation,s::wire::bytes body={},std::uint64_t loss=0)
    {
        action_capture ignored;assert(take(ignored)==c::error::again);send(rpc::operation::action_take);
        act::delivery value;value.information.source=9001;value.information.generation=71;value.information.sequence=++event_sequence;value.information.loss_before=loss;
        value.kind=kind;value.observed=state;value.goal=goal;value.request=correlation;value.goal_uuid[0]=1;value.body=body;
        std::array<std::uint8_t,2048> bytes;s::wire::writer out{bytes};assert(ac::encode_delivery(out,value,policy));reply(c::error::ok,c::effect::none,out.written());
    }
    void close()
    {
        for(std::size_t i{1};i<caps.size();++i)assert(permissions.close(who,caps[i])==s::error::success);
        auto origin=token(c::permission::resource_close);assert(core->close(owner)==c::error::busy&&core->close(owner,&origin)==c::error::again);
        assert(core->close(owner,&origin)==c::error::again);send(rpc::operation::resource_close);
        std::array<std::uint8_t,8> bytes;s::wire::writer out{bytes};assert(out.u64(9001));reply(c::error::ok,c::effect::none,bytes);
        assert(core->close(owner,&origin)==c::error::ok&&endpoint->cleanup_confirmed());
    }
};
static void client_flow()
{
    action_environment f;act::uuid uuid{1};std::array<std::uint8_t,1> body{7};auto submit=f.token(c::permission::action_submit);c::receipt result;
    assert(f.core->submit(f.owner,{f.resource,1},uuid,body,c::now()+900000000,result,&submit)==c::error::ok);
    auto goal=result.request;assert(goal!=9002&&result.provider_generation==71);f.send(rpc::operation::action_submit);f.complete(9002);
    f.event(act::event_kind::acceptance,act::state::accepted,9002,9002);action_capture capture;
    assert(f.take(capture)==c::error::ok&&capture.event.goal==goal&&capture.event.request==goal);
    assert(capture.event.information.source==f.resource&&capture.event.information.generation==71);
    auto cancel=f.token(c::permission::action_cancel);
    assert(f.core->cancel(f.owner,{f.resource,2},goal,c::now()+900000000,result,&cancel)==c::error::ok);auto correlation=result.request;
    auto bytes=f.send(rpc::operation::action_cancel);s::wire::reader input{bytes};std::uint64_t native{};assert(input.u64(native)&&native==9002);f.complete(9003);
    std::array<std::uint8_t,8> accepted{};f.event(act::event_kind::cancel_response,act::state::cancelling,9002,9003,accepted);
    capture.accept=false;assert(f.take(capture)==c::error::io&&capture.event.request==correlation);auto charged=f.charges;
    capture.accept=true;assert(f.take(capture)==c::error::ok&&f.charges==charged);
    f.event(act::event_kind::result,act::state::cancelled,9002,9002,body,3);
    assert(f.take(capture)==c::error::ok&&capture.event.goal==goal&&capture.event.information.loss_before==3&&!f.endpoint->occupied());
    f.close();
}
static void server_flow()
{
    action_environment f{true};std::array<std::uint8_t,1> body{7};f.event(act::event_kind::proposal,act::state::pending,9002,9003,body);
    action_capture capture;capture.accept=false;assert(f.take(capture)==c::error::io);auto goal=capture.event.goal,proposal=capture.event.request;
    assert(goal!=proposal&&goal!=9002&&proposal!=9003);act::response response{proposal,goal,act::transition::accept,{},false};
    auto origin=f.token(c::permission::action_respond,c::permission::action_accept);c::receipt receipt;
    auto deadline=c::now()+900000000;assert(f.core->respond(f.owner,{f.resource,1},response,deadline,receipt,&origin)==c::error::denied);
    capture.accept=true;assert(f.take(capture)==c::error::ok);
    auto respond=[&](std::uint64_t sequence,act::transition transition,c::handle request=0,bool accept=false)
    {
        response={request?request:proposal,goal,transition,transition==act::transition::succeed?s::wire::bytes{body}:s::wire::bytes{},accept};
        auto secondary=static_cast<c::permission>(static_cast<unsigned>(c::permission::action_accept)+static_cast<unsigned>(transition)-1);
        auto origin=f.token(c::permission::action_respond,secondary);auto until=c::now()+900000000;
        assert(f.core->respond(f.owner,{f.resource,sequence},response,until,receipt,&origin)==c::error::ok);
        auto bytes=f.send(rpc::operation::action_respond);s::wire::reader input{bytes};std::uint64_t native_request{},native_goal{};
        assert(input.u64(native_request)&&native_request==(request?9006u:9003u)&&input.u64(native_goal)&&native_goal==9002);
        f.complete(9100+sequence);auto calls=f.charges;
        assert(f.core->respond(f.owner,{f.resource,sequence},response,until,receipt,&origin)==c::error::ok&&f.charges==calls);
    };
    respond(1,act::transition::accept);respond(2,act::transition::execute);
    f.event(act::event_kind::cancel_request,act::state::executing,9002,9006);assert(f.take(capture)==c::error::ok);
    auto cancellation=capture.event.request;assert(cancellation!=proposal&&cancellation!=goal&&cancellation!=9006);
    respond(3,act::transition::cancel_response,cancellation);respond(4,act::transition::succeed);f.close();
}
static void failures()
{
    act::uuid uuid{1};std::array<std::uint8_t,1> body{7};c::receipt result;
    action_environment zero{false,0};auto origin=zero.token(c::permission::action_submit);
    assert(zero.core->submit(zero.owner,{zero.resource,1},uuid,body,c::now()+900000000,result,&origin)==c::error::again&&!zero.charges&&!zero.endpoint->occupied());
    action_environment malformed;origin=malformed.token(c::permission::action_submit);
    assert(malformed.core->submit(malformed.owner,{malformed.resource,1},uuid,body,c::now()+900000000,result,&origin)==c::error::ok);
    malformed.send(rpc::operation::action_submit);auto unknown=malformed.complete(9002,true);
    assert(unknown.status==c::error::io&&unknown.observed==c::effect::unknown&&malformed.core->cleanup_debt()&&malformed.core->close(malformed.owner)==c::error::busy);
    auto child=::fork();assert(child>=0);if(!child){assert(malformed.endpoint->occupied()&&malformed.endpoint->pump(malformed.transport)==c::error::denied);::_exit(0);}
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
}
int main()
{client_flow();server_flow();failures();std::puts("PASS isolated typed action endpoint: distinct goal/proposal/cancel correlations, current origins, independent transitions, retained failed copies, native loss, exact completion, zero quota, unknown fencing and verified close");}
