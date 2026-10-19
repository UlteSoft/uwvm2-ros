/** Actual original-SDK/Gateway source witness, authored NOT RUN.
 * A separate bounded Root owner must prepare one real Gateway/config/process,
 * explicit Native network namespace/RMW/domain/security and this exact worker.
 * No mock adapter/Node/header/identity, Host import or Guest claim is supplied.
 */
#include "../../../gateways/common/config.h"
#include "../../../gateways/ros2/native_rmw_builtin.h"
#include <uwvm2/imported/ros/ipc/rmw_subscription.h>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>
namespace gw=uwvm2::ros_gateway;
namespace ipc=uwvm2::imported::ros::ipc;
namespace w=ipc::rmw_endpoint;
namespace s=ipc::rmw_subscription;
namespace m=ipc::rmw_metadata;
static void check(bool value){if(!value){std::abort();}}
static std::vector<std::uint8_t> encode(w::packet const& p)
{std::array<std::uint8_t,w::packet_bytes> b{};check(w::encode(p,false,b));return {b.begin(),b.end()};}
static std::vector<std::uint8_t> encode(s::control const& p)
{std::array<std::uint8_t,s::header_bytes> b{};check(s::encode(p,false,b));return {b.begin(),b.end()};}
int main(int argc,char** argv)
{
    if(argc!=4||std::string_view{argv[1]}!="--trusted-native-fixture"){return 13;}
    // Publish only this process identity, then accept ONE explicit stdin gate.
    // The external owner writes worker-pid/config and starts the genuine
    // Gateway BEFORE granting G. It must own a finite gate deadline too.
    std::printf("READY_OWNED_SUBSCRIPTION_NATIVE_WORKER pid=%ld uid=%ld\n",static_cast<long>(::getpid()),static_cast<long>(::getuid()));std::fflush(stdout);
    if(std::getc(stdin)!='G'){return 13;}
    auto cfg=gw::load(argv[2]);std::string alias=argv[3];
    auto at=cfg.policy.capabilities.find(alias);
    if(cfg.policy.ros_version!=gw::ros::version::ros2||cfg.worker_pid!=static_cast<std::uint32_t>(::getpid())||
        cfg.worker_uid!=static_cast<std::uint32_t>(::getuid())||at==cfg.policy.capabilities.end()){return 13;}
    auto const& cap=at->second;
    if(cap.operation!="topic.subscribe"||cap.type!="std_msgs/msg/String"||
        !gw::ros2_compat::builtin_message<std_msgs::msg::String>(cap.type,cap.type_identity)){return 13;}
    if(gw::limit(cap,"max-message-bytes")<11||gw::limit(cap,"max-queue-bytes")<m::header_bytes+11||
        gw::limit(cap,"burst")<2||gw::limit(cap,"max-bytes-per-second")<22){return 90;}
    // Same-UID explicit fixture owner only. The external controller must bind
    // the real Gateway UID/process/tree/ELF; this is not an ambient daemon.
    ipc::client client;auto error=client.connect(cfg.socket,2,cfg.worker_uid,cfg.timeout_ms);if(error){return static_cast<int>(error);}
    auto bound=client.call("session.bind","target",gw::ros::serialize_manifest(cfg.policy),cfg.timeout_ms);if(bound.error){return static_cast<int>(bound.error);}
    // These association numbers are fixture-local in the authenticated native
    // peer protocol, not observed Runtime/Guest handles or a native DDS GID.
    w::binding owner{2,UINT64_C(0xf1234567a1234567),17,23,client.session_id()};
    w::packet activate{};activate.kind=w::role::subscription;activate.owner=owner;
    std::string absent="not-granted-subscription-fixture";if(cfg.policy.capabilities.count(absent)){return 22;}
    auto denied=client.call("rmw.subscription-endpoint-v1",absent,encode(activate),cfg.timeout_ms);check(denied.error==13&&denied.payload.empty());
    auto admitted=client.call("rmw.subscription-endpoint-v1",alias,encode(activate),cfg.timeout_ms);
    if(admitted.error){return static_cast<int>(admitted.error);}
    w::packet actual{};check(w::associated_response(admitted.payload.data(),admitted.payload.size(),activate,actual));
    auto inspect=activate;inspect.operation=w::command::inspect;inspect.incarnation=actual.incarnation;
    auto inspected=client.call("rmw.subscription-endpoint-v1",alias,encode(inspect),cfg.timeout_ms);
    w::packet observed{};check(!inspected.error&&w::associated_response(inspected.payload.data(),inspected.payload.size(),inspect,observed)&&w::same(actual.actual[0],observed.actual[0]));
    // Mixing legacy receive and owned route on this same alias must not create
    // a second subscription or bypass the exact max-subscribers grant.
    auto mixed=client.call("topic.subscribe",alias,{},cfg.timeout_ms);check(mixed.error==17);
    rclcpp::init(argc,argv);auto node=std::make_shared<rclcpp::Node>("uwvm_actual_gateway_subscription_peer");
    auto publisher=gw::ros2_compat::create_publisher<std_msgs::msg::String>(node,cap.resource,gw::ros2_qos::configured(cap));check(publisher!=nullptr);
    w::packet pub{};pub.kind=w::role::publisher;pub.operation=w::command::activate;pub.owner=owner;pub.incarnation=43;
    error=gw::native_rmw_metadata::publisher(*publisher,pub);
    if(error){client.close();publisher.reset();node.reset();rclcpp::shutdown();return static_cast<int>(error);}
    auto until=std::chrono::steady_clock::now()+std::chrono::seconds(5);
    while((node->count_publishers(cap.resource)!=1||node->count_subscribers(cap.resource)!=1)&&std::chrono::steady_clock::now()<until)
    {std::this_thread::sleep_for(std::chrono::milliseconds(1));}
    check(node->count_publishers(cap.resource)==1&&node->count_subscribers(cap.resource)==1);
    s::control peek{s::command::peek,owner,actual.incarnation,0,0,ipc::max_payload_bytes,0,pub.gid_size};
    auto empty=client.call("rmw.subscription-transfer-v1",alias,encode(peek),cfg.timeout_ms);check(empty.error==11&&empty.payload.empty());
    std_msgs::msg::String value;value.data.assign("wasm\0ro",7);std::vector<std::uint8_t> expected;check(gw::native_rmw_builtin::string_codec::encode(value,expected)&&expected.size()==11);
    publisher->publish(value);
    ipc::call_result delivered;until=std::chrono::steady_clock::now()+std::chrono::seconds(5);
    do{delivered=client.call("rmw.subscription-transfer-v1",alias,encode(peek),cfg.timeout_ms);if(delivered.error==11){std::this_thread::sleep_for(std::chrono::milliseconds(1));}}
    while(delivered.error==11&&std::chrono::steady_clock::now()<until);
    if(delivered.error){client.close();publisher.reset();node.reset();rclcpp::shutdown();return static_cast<int>(delivered.error);}
    s::control first{};m::header information{};
    check(s::associated_delivery(delivered.payload.data(),delivered.payload.size(),peek,first,information));
    check(information.payload_bytes==expected.size()&&information.gid_size==pub.gid_size&&information.gid==pub.gid);
    check(std::equal(expected.begin(),expected.end(),delivered.payload.data()+s::minimum_delivery_bytes));
    std_msgs::msg::String received;check(gw::native_rmw_builtin::string_codec::decode(delivered.payload.data()+s::minimum_delivery_bytes,expected.size(),received)&&received.data==value.data);
    auto ack=first;ack.operation=s::command::cancel;ack.capacity=ack.record_bytes=ack.gid_bytes=0;
    auto cancelled=client.call("rmw.subscription-transfer-v1",alias,encode(ack),cfg.timeout_ms);s::control returned{};
    check(!cancelled.error&&s::associated_ack(cancelled.payload.data(),cancelled.payload.size(),ack,returned));
    auto again=client.call("rmw.subscription-transfer-v1",alias,encode(peek),cfg.timeout_ms);s::control second{};m::header repeated{};
    check(!again.error&&s::associated_delivery(again.payload.data(),again.payload.size(),peek,second,repeated));
    check(second.sequence==first.sequence&&second.reservation!=first.reservation&&again.payload.size()==delivered.payload.size()&&
        std::equal(again.payload.begin()+s::header_bytes,again.payload.end(),delivered.payload.begin()+s::header_bytes));
    ack=second;ack.operation=s::command::commit;ack.capacity=ack.record_bytes=ack.gid_bytes=0;
    auto committed=client.call("rmw.subscription-transfer-v1",alias,encode(ack),cfg.timeout_ms);
    check(!committed.error&&s::associated_ack(committed.payload.data(),committed.payload.size(),ack,returned));
    auto consumed=client.call("rmw.subscription-transfer-v1",alias,encode(peek),cfg.timeout_ms);check(consumed.error==11);
    auto retire=inspect;retire.operation=w::command::retire;
    auto closed=client.call("rmw.subscription-endpoint-retire-v1",alias,encode(retire),cfg.timeout_ms);
    check(!closed.error&&w::associated_response(closed.payload.data(),closed.payload.size(),retire,observed));
    until=std::chrono::steady_clock::now()+std::chrono::seconds(5);
    while(node->count_subscribers(cap.resource)!=0&&std::chrono::steady_clock::now()<until){std::this_thread::sleep_for(std::chrono::milliseconds(1));}
    check(node->count_subscribers(cap.resource)==0);
    auto retired=client.call("rmw.subscription-transfer-v1",alias,encode(peek),cfg.timeout_ms);check(retired.error==125);
    auto local=client.call("capability.close",alias,{},cfg.timeout_ms);check(!local.error);client.close();
    publisher.reset();node.reset();rclcpp::shutdown();
    std::fputs("actual-gateway-owned-subscription: PASS_FULL_NATIVE_VALUE_GID_AND_RETIRE_NOT_GUEST_ACCEPTANCE\n",stdout);return 0;
}
