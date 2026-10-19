/** Real selected-SDK eager-publisher witness. No fake ROS headers or entity
 * mocks are used. Root must provide an unused topic and independently bound
 * process deadline/cgroup. An unsupported actual native identity/QoS/API returns
 * its nonzero errno, never successful skipped compatibility. This fixture does
 * not establish the still-required Host/Guest transport or Wasm performance. */
#include "../../gateways/ros2/native_rmw_builtin.h"
#if __has_include(<rclcpp/message_info.hpp>)
#include <rclcpp/message_info.hpp>
using native_message_info=rclcpp::MessageInfo;
static rmw_message_info_t const& original_info(native_message_info const& value){return value.get_rmw_message_info();}
#else
using native_message_info=rmw_message_info_t;
static rmw_message_info_t const& original_info(native_message_info const& value){return value;}
#endif
#include <chrono>
#include <cstdlib>
#include <cstdio>
#include <thread>
namespace gw=uwvm2::ros_gateway;
namespace ipc=uwvm2::imported::ros::ipc;
namespace wire=ipc::rmw_endpoint;
namespace metadata=ipc::rmw_metadata;
static void require(bool value){if(!value){std::abort();}}
static uwvm2::imported::ros::capability_spec grant(std::string const& topic)
{
    uwvm2::imported::ros::capability_spec c;c.alias="original-rmw-publisher";c.operation="topic.publish";c.resource=topic;
    c.type="std_msgs/msg/String";c.type_identity=gw::ros2_compat::builtin_catalog_identity(c.type);
    c.limits={{"max-message-bytes","4096"},{"validator","none"},{"qos.reliability","reliable"},{"qos.durability","volatile"},
        {"qos.history","keep-last"},{"qos.depth","4"},{"qos.deadline","infinite"},{"qos.lifespan","infinite"},
        {"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}};return c;
}
// Original-header, real-utility contracts run before creating any ROS entity.
// They do not emulate RMW duration behavior or establish ROS transport credit.
static void original_rmw_duration_contract()
{
#if UWVM_ROS2_HAVE_COMPLETE_RMW_QOS
    rmw_qos_profile_t raw=rmw_qos_profile_default;
    raw.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;raw.depth=4;
    raw.reliability=RMW_QOS_POLICY_RELIABILITY_RELIABLE;
    raw.durability=RMW_QOS_POLICY_DURABILITY_VOLATILE;
    raw.liveliness=RMW_QOS_POLICY_LIVELINESS_AUTOMATIC;
    auto alternate{raw};
    raw.deadline={1,0};alternate.deadline={0,1000000000};
    raw.lifespan={2,100};alternate.lifespan={1,1000000100};
    raw.liveliness_lease_duration=RMW_DURATION_INFINITE;
    alternate.liveliness_lease_duration={UINT64_MAX,UINT64_MAX};
    require(gw::ros2_qos::equal(raw,alternate));
    wire::qos first{},second{};
    require(gw::native_rmw_metadata::actual_profile(raw,first));
    require(gw::native_rmw_metadata::actual_profile(alternate,second));
    require(wire::same(first,second));
    alternate.avoid_ros_namespace_conventions=!raw.avoid_ros_namespace_conventions;
    require(!gw::ros2_qos::equal(raw,alternate));
    alternate.avoid_ros_namespace_conventions=raw.avoid_ros_namespace_conventions;
    alternate.deadline.nsec+=1;require(!gw::ros2_qos::equal(raw,alternate));
#endif
}
int main(int argc,char** argv)
{
    original_rmw_duration_contract();
    if constexpr(!gw::native_rmw_metadata::has_times<rmw_message_info_t>::value){return 95;}
    if(argc!=2){return 22;}std::string topic{argv[1]};if(topic.empty()||topic[0]!='/'){return 22;}
    rclcpp::init(argc,argv);
    auto node{std::make_shared<rclcpp::Node>("uwvm_actual_rmw_publisher_contract")};auto cap{grant(topic)};
    wire::binding owner{2,UINT64_C(0xf1234567a1234567),17,23,31};
    std::shared_ptr<gw::native_rmw_publisher::endpoint> endpoint;wire::packet activated{};
    auto error{gw::native_rmw_builtin::publisher<gw::native_rmw_builtin::string_codec>::create(node,cap,owner,41,endpoint,activated)};
    if(error){node.reset();rclcpp::shutdown();return static_cast<int>(error);}
    require(endpoint&&activated.profiles==1&&activated.gid_size&&activated.incarnation==41&&wire::valid_response(activated));
    unsigned received{};std::string actual_text;metadata::header actual_info{};
    auto callback=[&](std::shared_ptr<std_msgs::msg::String const> value,native_message_info const& native_info)
    {
        require(value!=nullptr);++received;actual_text=value->data;
        metadata::header h{};h.owner=owner;h.incarnation=41;h.sequence=received;h.payload_bytes=4+value->data.size();
        require(gw::native_rmw_metadata::topic(original_info(native_info),h)==0&&metadata::complete(h));actual_info=h;
    };
    auto subscriber{gw::ros2_compat::create_subscription<std_msgs::msg::String>(node,topic,gw::ros2_qos::configured(cap),callback)};
    rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);
    auto until{std::chrono::steady_clock::now()+std::chrono::seconds(5)};
    while((node->count_publishers(topic)!=1||node->count_subscribers(topic)!=1)&&std::chrono::steady_clock::now()<until)
    {executor.spin_some();std::this_thread::sleep_for(std::chrono::milliseconds(1));}
    require(node->count_publishers(topic)==1&&node->count_subscribers(topic)==1);
    for(unsigned i{};i<5;++i){executor.spin_some();std::this_thread::sleep_for(std::chrono::milliseconds(1));}
    require(received==0); // Creating/inspecting the endpoint must never publish.
    wire::packet inspected{};require(endpoint->inspect(owner,41,inspected)==0&&inspected.gid==activated.gid&&wire::same(inspected.actual[0],activated.actual[0]));
    auto foreign{owner};++foreign.generation;wire::packet sentinel{};sentinel.incarnation=73;
    require(endpoint->inspect(foreign,41,sentinel)==13&&sentinel.incarnation==73&&received==0);
    std_msgs::msg::String message;message.data.assign("original\0whole-string",21);std::vector<std::uint8_t> bytes;
    require(gw::native_rmw_builtin::string_codec::encode(message,bytes));
    require(endpoint->publish(foreign,41,bytes.data(),bytes.size())==13);
    require(endpoint->publish(owner,42,bytes.data(),bytes.size())==13);
    require(endpoint->publish(owner,41,bytes.data(),bytes.size())==0);
    until=std::chrono::steady_clock::now()+std::chrono::seconds(5);
    while(!received&&std::chrono::steady_clock::now()<until){executor.spin_some();std::this_thread::sleep_for(std::chrono::milliseconds(1));}
    require(received==1&&actual_text==message.data&&actual_info.gid_size==activated.gid_size&&actual_info.gid==activated.gid);
    require(endpoint->retire(foreign,41)==13);require(endpoint->retire(owner,41)==0);
    require(endpoint->publish(owner,41,bytes.data(),bytes.size())==125&&endpoint->inspect(owner,41,sentinel)==125&&sentinel.incarnation==73);
    require(endpoint->retire(owner,41)==125);
    until=std::chrono::steady_clock::now()+std::chrono::seconds(5);
    while(node->count_publishers(topic)!=0&&std::chrono::steady_clock::now()<until)
    {executor.spin_some();std::this_thread::sleep_for(std::chrono::milliseconds(1));}
    require(node->count_publishers(topic)==0&&received==1);
    endpoint.reset();subscriber.reset();executor.remove_node(node);node.reset();rclcpp::shutdown();
    std::fputs("actual-native-rmw-publisher: PASS\n",stdout);return 0;
}
