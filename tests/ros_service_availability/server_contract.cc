/** Production AF_UNIX/peer-credential/manifest/dispatch contract with a MODEL
 * native adapter. Real framing and lifetime ledger are used, but no ROS context,
 * Wasm, vendor, external server availability or official-case credit is claimed.
 * Compile/run only by a separately admitted Root owner after source review. */
#include "../../gateways/common/server.h"
#include <cstdlib>
#include <exception>
namespace gw=uwvm2::ros_gateway;namespace ros=uwvm2::imported::ros;
namespace ipc=ros::ipc;namespace wire=ipc::client_availability;
static void require(bool condition){if(!condition){std::abort();}}
static std::uint64_t handle(unsigned slot,unsigned gen=1){return (std::uint64_t{gen}<<32)|slot;}
struct effects {std::atomic<unsigned> queries{},retirements{},calls{},destroyed{};};
class fake final:public gw::adapter
{
    std::shared_ptr<effects> state_;
public:
    explicit fake(std::shared_ptr<effects> state):state_{std::move(state)}{}
    ~fake(){++state_->destroyed;}
    ipc::call_result publish(ros::capability_spec const&,std::vector<std::uint8_t> const&) override{return {95,{}};}
    ipc::call_result subscribe(ros::capability_spec const&,std::shared_ptr<gw::event_queue>) override{return {95,{}};}
    ipc::call_result service(ros::capability_spec const&,std::vector<std::uint8_t> const&,std::uint32_t) override{++state_->calls;return {95,{}};}
    ipc::call_result service_availability(ros::capability_spec const& a,ros::capability_spec const& s,
        wire::packet const& request,std::uint32_t timeout) override
    {
        require(timeout&&ros::service_availability_pair(ros::version::ros1,a,s));++state_->queries;
        auto response{request};response.value=request.operation==wire::command::features?11:1;
        auto encoded{wire::encode(response)};return {0,{encoded.begin(),encoded.end()}};
    }
    void retire_service_availability(wire::context const&,wire::retirement,std::string_view,std::string_view) noexcept override{++state_->retirements;}
    void close(std::string const&) override{}
};
static ros::capability_spec source()
{
    ros::capability_spec c;c.alias="call";c.operation="service.call";c.resource="/availability_fixture";c.type="std_srvs/Trigger";
    c.type_identity=ros::builtin_type_identity(ros::version::ros1,c.type);
    c.limits={{"timeout-ms","1000"},{"max-inflight","1"},{"max-request-bytes","1"},{"max-response-bytes","128"},{"validator","none"},
        {"max-rate-hz","1000"},{"burst","100"},{"max-bytes-per-second","65536"},{"audit","deny-only"},
        {"audit-max-rate-hz","1000"},{"audit-burst","1000"},{"audit-max-records","1000"},{"audit-max-bytes","1048576"}};return c;
}
int main()
{
    gw::configuration cfg;cfg.policy.enabled=true;cfg.policy.ros_version=ros::version::ros1;
    cfg.worker_uid=::getuid();cfg.worker_pid=::getpid();cfg.timeout_ms=2000;
    cfg.audit_records_per_second=1000;cfg.audit_max_records=1000;cfg.audit_max_bytes=1048576;
    auto call_cap{source()},cap{call_cap};cap.alias="available";cap.operation="service.available";cap.limits.erase("validator");
    cap.limits["service-source-alias"]="call";cap.limits["max-wait-ns"]="1000000";
    cap.limits["max-request-bytes"]="64";cap.limits["max-response-bytes"]="64";
    cap.limits["max-native-clients"]="2";cap.limits["max-binding-history"]="3";
    // Independent Gateway parser boundaries for this NEW operation only.
    // The old operation ceilings/behavior are intentionally unchanged.
    for(auto backend:{ros::version::ros1,ros::version::ros2})
    {
        auto boundary{cap};
        if(backend==ros::version::ros2)
        {
            boundary.type="std_srvs/srv/Trigger";boundary.type_identity=ros::builtin_type_identity(backend,boundary.type);
            boundary.limits.insert({{"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.history","keep-last"},{"qos.depth","4"},
                {"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}});
        }
        boundary.limits["timeout-ms"]="60000";boundary.limits["max-wait-ns"]="59999000000";
        gw::validate(backend,boundary);
        auto rejects=[&](ros::capability_spec const& value){bool rejected{};try{gw::validate(backend,value);}catch(std::runtime_error const&){rejected=true;}require(rejected);};
        auto excessive{boundary};excessive.limits["timeout-ms"]="60001";rejects(excessive);
        excessive=boundary;excessive.limits["max-wait-ns"]="60000000000";rejects(excessive);
        excessive=boundary;excessive.limits["timeout-ms"]="59999";rejects(excessive);
    }
    auto limited_source{call_cap};limited_source.alias="limitedCall";limited_source.resource="/limited_availability_fixture";
    auto limited{cap};limited.alias="limited";limited.resource=limited_source.resource;limited.limits["service-source-alias"]=limited_source.alias;
    limited.limits["max-rate-hz"]="0.000001";limited.limits["burst"]="1";
    for(auto const& c:{call_cap,cap,limited_source,limited}){gw::validate(ros::version::ros1,c);cfg.policy.capabilities.emplace(c.alias,c);}
    gw::validate_availability_sources(cfg.policy);
    auto bad{cfg.policy};bad.capabilities["available"].limits["service-source-alias"]="missing";
    bool refused{};try{gw::validate_availability_sources(bad);}catch(std::exception const&){refused=true;}require(refused);
    int sockets[2]{-1,-1};require(!::socketpair(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC,0,sockets));
    int pidfd{static_cast<int>(::syscall(SYS_pidfd_open,::getpid(),0))};require(pidfd>=0);
    auto state{std::make_shared<effects>()};std::exception_ptr failure;
    std::thread thread{[&]{try{gw::serve_connection(sockets[1],cfg,[&]{return std::make_unique<fake>(state);},pidfd);}catch(...){failure=std::current_exception();}}};
    struct cleanup{int socket,pidfd;std::thread& thread;~cleanup(){::shutdown(socket,SHUT_RDWR);::close(socket);if(thread.joinable())thread.join();::close(pidfd);}} owned{sockets[0],pidfd,thread};
    ipc::frame frame;frame.type=ipc::kind::hello;frame.backend=1;frame.request_id=1;frame.timeout_ms=2000;ipc::frame reply;
    require(ipc::write_frame(sockets[0],frame,ipc::after(2000))&&ipc::read_frame(sockets[0],reply,ipc::after(2000))&&reply.session_id);
    frame.type=ipc::kind::request;frame.session_id=reply.session_id;
    auto rpc=[&](char const* operation,char const* alias,std::vector<std::uint8_t> data={})
    {
        ++frame.request_id;require(ipc::encode_request({operation,alias,std::move(data)},frame.payload));
        require(ipc::write_frame(sockets[0],frame,ipc::after(2000))&&ipc::read_frame(sockets[0],reply,ipc::after(2000)));
        require(reply.response_id==frame.request_id&&reply.session_id==frame.session_id);return ipc::call_result{reply.error,reply.payload};
    };
    wire::context binding{1,handle(1),handle(2),101,17};
    wire::packet query{wire::command::query,wire::retirement::none,0,binding,0};
    auto ask=[&](wire::packet const& p,char const* alias="available",bool retirement=false)
    {auto raw{wire::encode(p)};return rpc(retirement?"service.available-retire-v1":"service.available-v1",alias,{raw.begin(),raw.end()});};
    require(ask(query).error==13&&state->queries==0); // No manifest bind.
    require(!rpc("session.bind","target",ros::serialize_manifest(cfg.policy)).error);
    require(ask(query,"call").error==22&&state->queries==0); // Source alone is NOT query authority.
    auto answer{ask(query)};wire::packet decoded{};
    require(!answer.error&&wire::decode_response(answer.payload.data(),answer.payload.size(),query,decoded)&&decoded.value==1&&state->calls==0&&state->queries==1);
    auto malformed{query};malformed.binding.runtime_generation=18;require(ask(malformed).error==13&&state->queries==1);
    malformed=query;malformed.binding.source=handle(3);require(ask(malformed).error==17&&state->queries==1);
    auto raw{wire::encode(query)};raw[63]=1;require(rpc("service.available-v1","available",{raw.begin(),raw.end()}).error==22&&state->queries==1);
    auto retire{query};retire.operation=wire::command::retire;retire.scope=wire::retirement::availability_handle;
    require(!ask(retire,"available",true).error&&state->retirements==1);
    require(ask(query).error==125&&state->queries==1);
    // Historical ordinary alias close does not revoke a fresh handle generation.
    require(!rpc("capability.close","available").error);
    query.binding.availability=handle(1,2);require(!ask(query).error&&state->queries==2);
    query.binding.availability=handle(1,3);require(!ask(query).error&&state->queries==3);
    query.binding.availability=handle(1,4);require(ask(query).error==11&&state->queries==3); // Tombstones still count.
    // Independent alias has its own explicit quota, but private source retirement
    // works even after its last query token and bandwidth credit have gone.
    query.binding.availability=handle(4);auto limited_query{query};limited_query.binding.source=handle(5);require(!ask(limited_query,"limited").error&&state->queries==4);
    require(ask(limited_query,"limited").error==11&&state->queries==4);
    retire=limited_query;retire.operation=wire::command::retire;retire.scope=wire::retirement::source_handle;
    require(!ask(retire,"limited",true).error);
    auto new_query{query};new_query.binding.availability=handle(4,2);require(ask(new_query,"limited").error!=0&&state->queries==4);
    // Source-first close and alias revoke reject even same-name reopens. No
    // native Service request was ever sent by this model's query path.
    require(!rpc("capability.revoke","call").error);
    require(ask(query).error==125&&state->calls==0);
    ::shutdown(sockets[0],SHUT_RDWR);thread.join();if(failure)std::rethrow_exception(failure);require(state->destroyed==1);
}
