/** Linux real-socket SOURCE contract, NOT RUN. This uses the actual server,
 * framing, credentials, worker pidfd and cleanup scopes. The adapter below is
 * explicitly a SYNTHETIC WIRE MODEL: its URMI flags/bytes are test data, never
 * observed native facts or evidence of ROS, SDK, Host or Guest acceptance.
 */
#include "../../../../../../src/ros/gateway/common/server.h"
#include <atomic>
#include <cstdlib>
#include <cstdio>
#include <thread>
namespace gw=uwvm2::ros_gateway;
namespace ipc=uwvm2::imported::ros::ipc;
namespace w=ipc::rmw_endpoint;
namespace s=ipc::rmw_subscription;
namespace m=ipc::rmw_metadata;
static void check(bool value){if(!value){std::abort();}}
struct facts
{
    std::size_t body_bytes{7};
    std::atomic<unsigned> peeks{},commits{},cancels{},failed{},clears{};
    std::atomic<std::uint32_t> primary{};
    std::atomic<bool> pending{};
    bool broken_reply{},close_peer_on_peek{},late_commit{};
    int peer{-1};std::uint64_t sequence{1},reservation{};
};
struct model final:gw::adapter
{
    facts& f;explicit model(facts& x):f{x}{}
    ipc::call_result publish(gw::ros::capability_spec const&,std::vector<std::uint8_t> const&) override{return {95,{}};}
    ipc::call_result subscribe(gw::ros::capability_spec const&,std::shared_ptr<gw::event_queue>) override{return {95,{}};}
    ipc::call_result service(gw::ros::capability_spec const&,std::vector<std::uint8_t> const&,std::uint32_t) override{return {95,{}};}
    void close(std::string const&) override{f.pending=false;}
    ipc::call_result owned_rmw_subscription_transfer(gw::ros::capability_spec const&,s::control const& request,ipc::deadline until) override
    {
        if(request.operation!=s::command::peek)
        {
            if(!f.pending||request.sequence!=f.sequence||request.reservation!=f.reservation){return {13,{}};}
            if(request.operation==s::command::commit){++f.commits;++f.sequence;}else{++f.cancels;}
            f.pending=false;
            std::array<std::uint8_t,s::header_bytes> encoded{};check(s::encode(request,true,encoded));
            if(f.late_commit){std::this_thread::sleep_until(until+std::chrono::milliseconds(1));}
            return {0,{encoded.begin(),encoded.end()}};
        }
        if(f.pending){return {17,{}};}++f.peeks;++f.reservation;f.pending=true;
        std::vector<std::uint8_t> body(f.body_bytes,'x');check(body.size()>=4);ipc::event_put(body.data(),body.size()-4,4);
        if(body.size()==7){body[4]='a';body[5]=0;body[6]='b';}
        m::header information{};information.owner=request.owner;information.incarnation=request.incarnation;
        information.sequence=f.sequence;information.payload_bytes=body.size();
        information.flags=m::source_time|m::received_time|m::publisher_gid;information.gid_size=request.gid_bytes;
        // These explicit synthetic zeros are legal byte values under presence
        // flags. They are not native-time/GID witnesses or credentials.
        std::array<std::uint8_t,m::header_bytes> raw{};check(m::encode(information,raw));
        std::vector<std::uint8_t> record(raw.begin(),raw.end());record.insert(record.end(),body.begin(),body.end());
        auto response=request;response.sequence=f.sequence;response.reservation=f.reservation;response.record_bytes=record.size();
        std::vector<std::uint8_t> packet;check(s::pack_delivery(response,record.data(),record.size(),packet)==0);
        if(f.broken_reply){packet[s::header_bytes+8]^=1;}
        if(f.close_peer_on_peek){check(::shutdown(f.peer,SHUT_RDWR)==0);}
        return {0,std::move(packet)};
    }
    std::uint32_t owned_rmw_subscription_delivery_failed(gw::ros::capability_spec const&,w::binding const&,std::uint64_t,std::uint32_t code) noexcept override
    {++f.failed;f.primary=code;if(f.pending){++f.cancels;f.pending=false;}return 0;}
    void clear_owned_rmw_subscriptions() noexcept override{++f.clears;f.pending=false;}
};
static gw::configuration configuration(std::size_t bytes,unsigned timeout=1000)
{
    gw::configuration cfg;cfg.policy.enabled=true;cfg.policy.ros_version=gw::ros::version::ros2;cfg.policy.name="synthetic-wire-model";
    cfg.worker_uid=::getuid();cfg.worker_pid=::getpid();cfg.timeout_ms=timeout;cfg.max_sessions=1;
    cfg.audit_records_per_second=100;cfg.audit_max_records=1000;cfg.audit_max_bytes=1048576;
    gw::ros::capability_spec cap;cap.alias="source";cap.operation="topic.subscribe";cap.resource="/synthetic_wire";
    cap.type="std_msgs/msg/String";cap.type_identity="explicit-synthetic-not-native-proof";
    cap.limits={{"max-message-bytes",std::to_string(bytes)},{"max-rate-hz","0.000001"},{"burst","1"},
        {"max-bytes-per-second",std::to_string(bytes)},{"audit","deny-only"},{"audit-burst","100"},
        {"audit-max-rate-hz","100"},{"audit-max-records","1000"},{"audit-max-bytes","1048576"}};
    cfg.policy.capabilities.emplace(cap.alias,std::move(cap));return cfg;
}
struct connection
{
    int client{-1};std::uint64_t session{},next{2};
    explicit connection(int fd):client{fd}
    {
        ipc::frame hello;hello.type=ipc::kind::hello;hello.backend=2;hello.request_id=1;hello.timeout_ms=1000;
        ipc::frame reply;check(ipc::write_frame(client,hello,ipc::after(1000))&&ipc::read_frame(client,reply,ipc::after(1000)));
        check(reply.type==ipc::kind::response&&reply.response_id==1&&reply.session_id&&!reply.error);session=reply.session_id;
    }
    bool request(std::string const& operation,std::vector<std::uint8_t> bytes,ipc::frame& reply,unsigned timeout=1000,std::string const& alias="source")
    {
        ipc::frame frame;frame.type=ipc::kind::request;frame.backend=2;frame.session_id=session;frame.request_id=next++;frame.timeout_ms=timeout;
        check(ipc::encode_request({operation,alias,std::move(bytes)},frame.payload));
        return ipc::write_frame(client,frame,ipc::after(timeout))&&ipc::read_frame(client,reply,ipc::after(timeout));
    }
    void bind(gw::configuration const& cfg)
    {ipc::frame reply;check(request("session.bind",gw::ros::serialize_manifest(cfg.policy),reply,1000,"target")&&!reply.error);}
};
template<class Body>static void with_server(facts& f,gw::configuration const& cfg,Body body)
{
    int pair[2];check(::socketpair(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK,0,pair)==0);
    int worker=static_cast<int>(::syscall(SYS_pidfd_open,::getpid(),0));check(worker>=0);f.peer=pair[0];
    std::exception_ptr failure;
    std::thread server;
    try{server=std::thread([&]{try{gw::serve_connection(pair[1],cfg,[&]{return std::make_unique<model>(f);},worker);}catch(...){failure=std::current_exception();}});}
    catch(...){::close(pair[0]);::close(pair[1]);::close(worker);throw;}
    try{connection client{pair[0]};client.bind(cfg);body(client);}
    catch(...){::shutdown(pair[0],SHUT_RDWR);::close(pair[0]);server.join();::close(worker);throw;}
    ::shutdown(pair[0],SHUT_RDWR);::close(pair[0]);server.join();::close(worker);if(failure){std::rethrow_exception(failure);}
}
static s::control peek(connection const& c)
{return {s::command::peek,{2,UINT64_C(0xf1234567a1234567),17,23,c.session},41,0,0,ipc::max_payload_bytes,0,16};}
static std::vector<std::uint8_t> encode(s::control const& control)
{std::array<std::uint8_t,s::header_bytes> bytes{};check(s::encode(control,false,bytes));return {bytes.begin(),bytes.end()};}
static void body_quota_and_ack()
{
    facts f;auto cfg=configuration(7);
    with_server(f,cfg,[&](connection& client){
        ipc::frame answer;auto request=peek(client);check(client.request("rmw.subscription-transfer-v1",encode(request),answer)&&!answer.error);
        s::control response{};m::header information{};check(s::associated_delivery(answer.payload.data(),answer.payload.size(),request,response,information));
        check(information.payload_bytes==7&&answer.payload.size()==s::minimum_delivery_bytes+7);
        auto ack=response;ack.operation=s::command::commit;ack.capacity=ack.record_bytes=ack.gid_bytes=0;
        check(client.request("rmw.subscription-transfer-v1",encode(ack),answer)&&!answer.error);
        check(f.commits==1&&f.failed==0); // ACK spends no message/body quota.
        check(client.request("rmw.subscription-transfer-v1",encode(request),answer)&&answer.error==11);
        check(f.failed==1&&f.primary==11&&f.cancels==1&&!f.pending);
    });check(f.clears==1);
}
static void malformed_and_write_failure()
{
    for(unsigned scenario{};scenario!=2;++scenario)
    {
        facts f;f.broken_reply=scenario==0;f.close_peer_on_peek=scenario==1;auto cfg=configuration(7);
        with_server(f,cfg,[&](connection& client){ipc::frame answer;bool received=client.request("rmw.subscription-transfer-v1",encode(peek(client)),answer);
            if(!scenario){check(received&&answer.error==13);}else{check(!received);}});
        check(f.failed==1&&f.primary==(scenario?107u:13u)&&f.cancels==1&&!f.pending&&f.clears==1);
    }
}
static void full_packet_wire_ceiling_only()
{
    facts f;f.body_bytes=s::maximum_body_bytes;
    // Direct synthetic configuration intentionally tests the wire ceiling.
    // Actual load(config) retains its smaller old -518 topic body ceiling.
    auto cfg=configuration(f.body_bytes,10000);
    with_server(f,cfg,[&](connection& client){ipc::frame answer;check(client.request("rmw.subscription-transfer-v1",encode(peek(client)),answer,10000)&&!answer.error);
        check(answer.payload.size()==ipc::max_payload_bytes);s::control response{};m::header information{};
        check(s::associated_delivery(answer.payload.data(),answer.payload.size(),peek(client),response,information)&&information.payload_bytes==f.body_bytes);
        auto ack=response;ack.operation=s::command::cancel;ack.capacity=ack.record_bytes=ack.gid_bytes=0;
        check(client.request("rmw.subscription-transfer-v1",encode(ack),answer,10000)&&!answer.error);});
    check(f.failed==0&&f.cancels==1&&f.clears==1);
}
static void late_ack_is_session_terminal()
{
    facts f;f.late_commit=true;auto cfg=configuration(7);cfg.policy.capabilities.at("source").limits["timeout-ms"]="100";
    with_server(f,cfg,[&](connection& client){ipc::frame answer;auto request=peek(client);check(client.request("rmw.subscription-transfer-v1",encode(request),answer)&&!answer.error);
        s::control response{};m::header information{};check(s::associated_delivery(answer.payload.data(),answer.payload.size(),request,response,information));
        auto ack=response;ack.operation=s::command::commit;ack.capacity=ack.record_bytes=ack.gid_bytes=0;
        // Its effect may complete before an ACK is lost. Never replay it.
        bool received=client.request("rmw.subscription-transfer-v1",encode(ack),answer,1000);check(received&&answer.error==110);
        check(!client.request("rmw.subscription-transfer-v1",encode(request),answer,1000));});
    check(f.commits==1&&f.clears==1&&!f.pending);
}
static void denied_and_exception_obligation()
{
    facts f;auto cfg=configuration(7);
    with_server(f,cfg,[&](connection& client){ipc::frame answer;
        check(client.request("rmw.subscription-transfer-v1",encode(peek(client)),answer,1000,"absent")&&answer.error==13);
        check(f.peeks==0&&f.failed==0);});
    facts exceptional;auto backend=std::shared_ptr<gw::adapter>(std::make_shared<model>(exceptional));
    auto const& cap=cfg.policy.capabilities.at("source");
    s::control request{s::command::peek,{2,UINT64_C(0xf1234567a1234567),17,23,31},41,0,0,ipc::max_payload_bytes,0,16};
    try{gw::owned_subscription_delivery obligation{backend};exceptional.pending=true;obligation.arm(cap,request);throw std::runtime_error("explicit synthetic downstream failure");}
    catch(std::runtime_error const&){}
    check(exceptional.failed==1&&exceptional.primary==5&&exceptional.cancels==1&&!exceptional.pending);
}
int main()
{
    body_quota_and_ack();malformed_and_write_failure();full_packet_wire_ceiling_only();late_ack_is_session_terminal();denied_and_exception_obligation();
    std::fputs("owned-subscription-gateway: PASS_SYNTHETIC_WIRE_AND_CLEANUP_MODELS_NOT_NATIVE\n",stdout);return 0;
}
