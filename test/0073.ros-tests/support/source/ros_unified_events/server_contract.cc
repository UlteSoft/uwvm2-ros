/** @file
 * @brief Actual authenticated server/event lifecycle contract with a fake adapter.
 *
 * Build C++20 with src/ and pthread on Linux (pidfd support is mandatory).
 * This executable runs production AF_UNIX framing, peer credentials, immutable
 * manifest binding and server dispatch. It grants no Guest permissions and
 * creates no ROS context. Native adapter effects are counted independently;
 * this is not a ROS delivery or Wasm acceptance claim. Every wait is bounded.
 */
#include "../../../../../src/ros/gateway/common/server.h"
#include "../../../../../src/ros/gateway/common/bounded_feedback.h"
#include <cassert>
#include <charconv>
namespace gw=uwvm2::ros_gateway;namespace ros=uwvm2::imported::ros;namespace ipc=ros::ipc;
struct effects {
    std::mutex mutex;std::shared_ptr<gw::event_queue> legacy;
    std::atomic<unsigned> subscriptions{},source_closes{},publishes{},action_reads{},action_observes{},services{};
};
class fake final:public gw::adapter {
    std::shared_ptr<effects> state;
public:
    explicit fake(std::shared_ptr<effects> value):state{std::move(value)}{}
    ipc::call_result publish(ros::capability_spec const&,std::vector<std::uint8_t> const&) override {++state->publishes;return {5,{}};}
    ipc::call_result subscribe(ros::capability_spec const&,std::shared_ptr<gw::event_queue> q) override
    {std::lock_guard<std::mutex> guard{state->mutex};++state->subscriptions;state->legacy=std::move(q);return {};}
    ipc::call_result service(ros::capability_spec const&,std::vector<std::uint8_t> const&,std::uint32_t) override
    {++state->services;return {0,{1,0,0,0,0}};}
    ipc::call_result operation(ros::capability_spec const&,std::vector<std::uint8_t> const&,std::uint32_t) override
    {++state->action_reads;return {9,{}};}
    ipc::call_result observe_action(ros::capability_spec const&,std::uint64_t) override {++state->action_observes;return {};}
    void close(std::string const& alias) override {if(alias=="sub")++state->source_closes;}
};
static ros::capability_spec base(char const* alias,char const* operation,char const* resource,char const* type)
{
    ros::capability_spec c;c.alias=alias;c.operation=operation;c.resource=resource;c.type=type;
    c.type_identity=ros::builtin_type_identity(ros::version::ros1,c.type);
    c.limits={{"max-rate-hz","1000"},{"burst","2"},{"max-bytes-per-second","4096"},{"audit","deny-only"},
      {"audit-max-rate-hz","1000"},{"audit-burst","1000"},{"audit-max-records","10000"},{"audit-max-bytes","1048576"}};return c;
}
static ros::capability_spec observer(ros::capability_spec const& source,char const* alias,char const* kinds,unsigned record=64)
{
    auto c=base(alias,"event.observe",source.resource.c_str(),"builtin/event-v1");c.type_identity=std::string(ipc::event_type_identity);
    char buffer[32];auto formatted=std::to_chars(buffer,buffer+sizeof(buffer),record);assert(formatted.ec==std::errc{});
    c.limits.insert({{"event-source-alias",source.alias},{"event-kinds",kinds},{"max-event-bytes",std::string(buffer,formatted.ptr)},
      {"max-event-queue-depth","64"},{"max-event-queue-bytes","4096"},{"max-inflight","2"},{"overflow-policy","reject"}});c.limits["burst"]="64";return c;
}
static std::vector<std::uint8_t> number(std::uint64_t value,unsigned width=8)
{std::vector<std::uint8_t> bytes;bytes.reserve(width);ipc::put(bytes,value,width);return bytes;}
int main()
{
    // Exercise the production callback gate and bounded queue with terminal
    // callback FIRST, then feedback on the other channel. A cached terminal
    // result does not consume Goal ownership. Actual dequeue/close retirement,
    // error or deadline expiry must stop later callbacks and retain no bytes.
    struct feedback_goal {ros::capability_spec capability;std::deque<std::vector<std::uint8_t>> feedback;std::size_t feedback_bytes{};std::optional<std::vector<std::uint8_t>> feedback_pending;gw::event_queue_statistics feedback_stats;bool active{true},terminal{};std::uint32_t error{};};
    feedback_goal ordered;ordered.capability.limits={{"max-feedback-bytes","12"},{"max-event-queue-depth","1"},{"max-event-queue-bytes","12"},{"overflow-policy","reject"}};
    auto incoming_feedback=[&](bool before_deadline){return gw::feedback_callback_live(ordered.active,ordered.error,before_deadline)&&gw::feedback_admit(ordered,std::vector<std::uint8_t>(12,7));};
    ordered.terminal=true;assert(incoming_feedback(true)&&ordered.feedback_stats.enqueued==1);
    auto delayed=gw::feedback_take(ordered);assert(!delayed.error&&delayed.payload==std::vector<std::uint8_t>(12,7));
    ordered.active=false;gw::feedback_discard(ordered);assert(!incoming_feedback(true)&&gw::feedback_statistics(ordered).bytes==0&&ordered.feedback_stats.enqueued==1);
    ordered.active=true;ordered.error=110;assert(!incoming_feedback(true));ordered.error=5;assert(!incoming_feedback(true));ordered.error=0;assert(!incoming_feedback(false)&&ordered.feedback_stats.enqueued==1);
    for(bool terminal:{false,true})for(bool active:{false,true})for(std::uint32_t error:{0u,5u,110u})for(bool before:{false,true})
    {ordered.terminal=terminal;assert(gw::feedback_callback_live(active,error,before)==(active&&error==0&&before));}
    gw::configuration cfg;cfg.policy.enabled=true;cfg.policy.ros_version=ros::version::ros1;
    cfg.worker_uid=::getuid();cfg.worker_pid=::getpid();cfg.timeout_ms=2000;
    cfg.audit_records_per_second=1000;cfg.audit_max_records=10000;cfg.audit_max_bytes=1048576;
    auto sub=base("sub","topic.subscribe","/test/sub","std_msgs/String");
    sub.limits.insert({{"max-message-bytes","32"},{"max-queue-bytes","64"},{"max-subscribers","1"},{"overflow-policy","reject"},{"validator","none"},{"queue-depth","2"},{"transport","tcpros"}});
    auto pub=sub;pub.alias="pub";pub.operation="topic.publish";pub.resource="/test/pub";pub.limits.erase("max-subscribers");pub.limits.insert({{"max-publishers","1"},{"latch","false"}});
    auto action=base("result","action.receive-result","/test/action","actionlib_tutorials/Fibonacci");
    action.limits.insert({{"fibonacci.max-order","10"},{"max-active-goals","2"},{"max-actions","1"},{"timeout-ms","1000"},
      {"max-goal-bytes","4"},{"max-feedback-bytes","52"},{"max-result-bytes","53"},{"cancel-scope","not-applicable"},
      {"max-event-queue-depth","2"},{"max-event-queue-bytes","128"},{"overflow-policy","reject"},{"validator","none"}});
    action.limits["max-rate-hz"]="0.001"; // Blocked consumer probes cannot spend either of its two tokens.
    auto alternate=action;alternate.alias="resultAlt";auto feedback=action;feedback.alias="feedback";feedback.operation="action.receive-feedback";
    auto service=base("client","service.call","/test/service","std_srvs/SetBool");
    service.limits.insert({{"timeout-ms","1000"},{"max-inflight","2"},{"max-request-bytes","1"},{"max-response-bytes","5"},{"validator","none"}});
    // A deliberately smaller Source bandwidth than its response makes Native
    // response delivery refuse with11. The durable completion must remain;
    // this explicit quota is not silently enlarged by the observer grant.
    service.limits["max-bytes-per-second"]="1";
    for(auto const& c:{sub,pub,action,alternate,feedback,service,observer(sub,"subEvents","resource-error,resource-closed"),observer(pub,"pubEvents","resource-error,resource-closed"),
                         observer(action,"actionEvents","resource-error,resource-closed"),observer(service,"serviceEvents","service-response,resource-error,resource-closed",69),
                         observer(action,"resultEvents","action-result,resource-error,resource-closed",117),
                         observer(alternate,"alternateEvents","action-result,resource-error,resource-closed",117),
                         observer(feedback,"feedbackEvents","action-feedback,resource-error,resource-closed",117)})
    {gw::validate(ros::version::ros1,c);cfg.policy.capabilities.emplace(c.alias,c);}
    gw::validate_event_sources(cfg.policy);
    int sockets[2];assert(::socketpair(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC,0,sockets)==0);
    int pidfd=static_cast<int>(::syscall(SYS_pidfd_open,::getpid(),0));assert(pidfd>=0);
    auto state=std::make_shared<effects>();std::exception_ptr failure;
    std::thread server{[&]{try{gw::serve_connection(sockets[1],cfg,[state]{return std::make_unique<fake>(state);},pidfd);}catch(...){failure=std::current_exception();}}};
    struct cleanup {int fd,pidfd;std::thread& server;~cleanup(){::shutdown(fd,SHUT_RDWR);::close(fd);if(server.joinable())server.join();::close(pidfd);}} owned{sockets[0],pidfd,server};
    ipc::frame frame;frame.type=ipc::kind::hello;frame.backend=1;frame.request_id=1;frame.timeout_ms=2000;ipc::frame reply;
    assert(ipc::write_frame(sockets[0],frame,ipc::after(2000))&&ipc::read_frame(sockets[0],reply,ipc::after(2000))&&reply.session_id);
    frame.type=ipc::kind::request;frame.session_id=reply.session_id;
    auto call=[&](char const* operation,char const* alias,std::vector<std::uint8_t> data={}){
        ++frame.request_id;assert(ipc::encode_request({operation,alias,std::move(data)},frame.payload));
        assert(ipc::write_frame(sockets[0],frame,ipc::after(2000))&&ipc::read_frame(sockets[0],reply,ipc::after(2000)));
        assert(reply.response_id==frame.request_id&&reply.session_id==frame.session_id);return ipc::call_result{reply.error,reply.payload};};
    auto watch=[&](char const* alias,std::uint64_t handle,std::uint64_t correlation=0){auto data=number(handle);ipc::put(data,47,8);ipc::put(data,correlation,8);return call("event.watch",alias,std::move(data));};
    assert(!call("session.bind","target",ros::serialize_manifest(cfg.policy)).error);
    // Legacy queue exists before a metadata-only watch. Attaching/detaching the
    // latter must neither seize its consumer nor destroy its native resource.
    assert(!call("topic.subscribe","sub").error&&state->subscriptions==1);
    assert(!watch("subEvents",11).error&&state->subscriptions==1);
    std::shared_ptr<gw::event_queue> legacy;{std::lock_guard<std::mutex> guard{state->mutex};legacy=state->legacy;}
    assert(legacy&&legacy->enqueue({1,0,0,0,'x'}));auto old=call("event.poll","sub");assert(!old.error&&old.payload==std::vector<std::uint8_t>({1,0,0,0,'x'}));
    assert(call("event.peek-v1","subEvents",number(64)).error==11);
    assert(!call("capability.close","subEvents").error&&state->source_closes==0);
    assert(legacy->enqueue({1,0,0,0,'y'})&&!call("event.poll","sub").error);
    // Only an explicitly selected error record is visible. The source Native
    // operation fails once; the metadata has no copied payload or correlation.
    assert(!watch("pubEvents",12).error);assert(call("topic.publish","pub",{1,0,0,0,'z'}).error==5&&state->publishes==1);
    auto error=call("event.peek-v1","pubEvents",number(64));ipc::event_header header{};
    assert(!error.error&&ipc::decode_event(error.payload.data(),error.payload.size(),header)&&header.kind==ipc::event_kind::resource_error&&header.error==5&&header.flags==2&&header.payload_bytes==0&&header.correlation==0);
    assert(call("event.commit","pubEvents",number(header.sequence+1)).error==22);
    assert(call("event.peek-v1","pubEvents",number(64)).payload==error.payload);
    assert(!call("event.commit","pubEvents",number(header.sequence)).error);
    // A metadata-only outgoing Action watch has correlation zero, does not
    // attest/consume a goal, and does not turn a legacy result into error90.
    assert(watch("actionEvents",13,99).error==22&&state->action_observes==0);
    assert(!watch("actionEvents",13).error);assert(!call("event.ready","actionEvents").error);
    assert(call("event.peek-v1","actionEvents",number(64)).error==11&&state->action_reads==0&&state->action_observes==0);
    assert(!call("event.detach","actionEvents").error);
    // The same native goal/result queue cannot be consumed through a second
    // independently granted alias. Refusals happen before adapter dispatch AND
    // Source debit. Different kind/goal watches remain independent consumers.
    assert(!watch("resultEvents",13,99).error&&state->action_observes==1);
    assert(watch("alternateEvents",15,99).error==17&&state->action_observes==1);
    for(unsigned attempt=0;attempt<4;++attempt){assert(call("action.receive-result","result",number(99)).error==17);assert(call("action.receive-result","resultAlt",number(99)).error==17);}
    assert(state->action_reads==0);
    assert(!watch("feedbackEvents",16,99).error&&state->action_observes==2);
    assert(!watch("alternateEvents",15,100).error&&state->action_observes==3);
    assert(call("action.receive-result","resultAlt",number(101)).error==9&&state->action_reads==1);
    assert(!call("event.detach","resultEvents").error);
    assert(call("action.receive-result","result",number(99)).error==9&&state->action_reads==2);
    assert(call("action.receive-result","result",number(99)).error==9&&state->action_reads==3);
    assert(call("action.receive-result","result",number(99)).error==11&&state->action_reads==3);
    // Async completion retains the same owned ID when Source bandwidth refuses
    // it. Observer credit does not substitute for the independently denied
    // Source budget, and repeated peeks cannot repeat the Native service call.
    assert(!watch("serviceEvents",14).error);auto request=number(71);request.push_back(1);assert(!call("service.submit","serviceEvents",request).error);
    bool completed{};for(unsigned attempt=0;attempt<40&&!completed;++attempt){auto ready=call("event.ready","serviceEvents");assert(!ready.error&&ready.payload.size()==32);completed=ipc::get(ready.payload.data(),8)==1;if(!completed)std::this_thread::sleep_for(std::chrono::milliseconds(1));}
    assert(completed&&state->services==1);
    for(unsigned attempt=0;attempt<3;++attempt){assert(call("event.peek-v1","serviceEvents",number(69)).error==11);auto ready=call("event.ready","serviceEvents");assert(!ready.error&&ipc::get(ready.payload.data(),8)==1&&ipc::get(ready.payload.data()+8,8)==69);}
    assert(state->services==1);assert(!call("capability.close","client").error);
    auto closed=call("event.peek-v1","serviceEvents",number(69));assert(!closed.error&&ipc::decode_event(closed.payload.data(),closed.payload.size(),header)&&header.kind==ipc::event_kind::resource_closed&&header.error==0&&header.payload_bytes==0);
    assert(!call("event.commit","serviceEvents",number(header.sequence)).error);
    ::shutdown(sockets[0],SHUT_RDWR);server.join();if(failure)std::rethrow_exception(failure);
    // C++17 Native IO does not admit vendored concepts-based fast_io. One
    // checked bounded POSIX write exposes a fixed verdict, never arbitrary text.
    static constexpr char marker[]="PASS_UNIFIED_SERVER\n";assert(::write(1,marker,sizeof(marker)-1)==ssize_t(sizeof(marker)-1));
}
