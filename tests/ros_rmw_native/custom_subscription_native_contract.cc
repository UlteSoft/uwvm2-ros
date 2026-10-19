/** Real selected-row SDK witness, authored but not run. An independent owner
 * must admit/retain this actual native process, namespace and exact input file.
 * There are no replacement ROS headers, mock Node, synthetic payload fields or
 * guessed native identities. The canonical input must come from an actual
 * independent complete native-value oracle for this exact selected catalog.
 * This native-only witness confers no Host/Guest source or transport credit.
 */
#include "../../gateways/ros2/native_rmw_subscription_factory.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <thread>
#ifndef UWVM_CUSTOM_SUBSCRIPTION_TEST_ROW
#error "Explicit actual complete-provider row ordinal required"
#endif
namespace gw=uwvm2::ros_gateway;
namespace topics=gw::ros2_custom_topics;
namespace ipc=uwvm2::imported::ros::ipc;
namespace wire=ipc::rmw_endpoint;
namespace info=ipc::rmw_metadata;
static_assert(UWVM_CUSTOM_SUBSCRIPTION_TEST_ROW<topics::detail::topic_count,
    "native witness must select one actual complete-provider row");
using row=std::tuple_element_t<UWVM_CUSTOM_SUBSCRIPTION_TEST_ROW,topics::detail::topic_rows>;
using Native=typename row::native_type;
using Codec=typename row::codec_type;
static void require(bool value){if(!value){std::abort();}}
static void spin(rclcpp::executors::SingleThreadedExecutor& executor)
{executor.spin_some();std::this_thread::sleep_for(std::chrono::milliseconds(1));}
static bool input(char const* path,std::size_t maximum,std::vector<std::uint8_t>& output)
{
    std::ifstream stream{path,std::ios::binary};if(!stream){return false;}
    std::vector<std::uint8_t> bytes;bytes.reserve(maximum);
    char buffer[4096];
    while(stream)
    {
        stream.read(buffer,sizeof buffer);auto n=stream.gcount();
        if(n<0||static_cast<std::size_t>(n)>maximum-bytes.size()){return false;}
        bytes.insert(bytes.end(),buffer,buffer+n);
    }
    if(!stream.eof()){return false;}output=std::move(bytes);return true;
}
int main(int argc,char** argv)
{
    if(argc!=4||std::string_view{argv[1]}!="--trusted-native-fixture"){return 13;}
    std::string topic{argv[2]};if(topic.empty()||topic[0]!='/'){return 22;}
    if constexpr(!UWVM_ROS2_HAVE_COMPLETE_RMW_QOS||!gw::native_rmw_metadata::has_times<rmw_message_info_t>::value){return 95;}
    auto const& declared{topics::detail::owned_publisher_rows[UWVM_CUSTOM_SUBSCRIPTION_TEST_ROW]};
    auto descriptor{ipc::find_custom_topic(2,declared.name)};
    if(!descriptor||descriptor->wire_bytes>ipc::max_payload_bytes-info::header_bytes){return 22;}
    std::vector<std::uint8_t> expected;
    if(!input(argv[3],descriptor->wire_bytes,expected)||!ipc::validate_custom_topic(*descriptor,expected.data(),expected.size())){return 22;}
    Native value;if(!Codec::decode(expected.data(),expected.size(),value)||!Codec::valid(value)){return 22;}
    std::vector<std::uint8_t> roundtrip;if(!Codec::encode(value,roundtrip)||roundtrip!=expected){return 22;}
    uwvm2::imported::ros::capability_spec cap;cap.alias="trusted-custom-native-subscription";
    cap.operation="topic.subscribe";cap.resource=topic;cap.type=declared.name;cap.type_identity=declared.native_identity;
    cap.limits={{"adapter-codec-identity",std::string{declared.codec_identity}},
        {"max-message-bytes",std::to_string(descriptor->wire_bytes)},
        {"max-queue-bytes",std::to_string((info::header_bytes+descriptor->wire_bytes)*4)},
        {"max-rate-hz","100"},{"burst","4"},{"max-bytes-per-second",std::to_string(std::max<std::size_t>(descriptor->wire_bytes,1)*100)},
        {"overflow-policy","reject"},{"validator","none"},
        {"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.history","keep-last"},{"qos.depth","4"},
        {"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}};
    // Association numbers below are explicitly fixture-local, not minted Host
    // credentials, observed Runtime facts, or an authorization bypass in Guest.
    wire::binding owner{2,UINT64_C(0xf1234567a1234567),17,23,31};
    rclcpp::init(argc,argv);auto node=std::make_shared<rclcpp::Node>("uwvm_actual_custom_subscription_contract");
    rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);
    std::shared_ptr<gw::native_rmw_subscription::endpoint> endpoint;wire::packet actual{};
    auto error{gw::native_rmw_subscription::create_selected(node,cap,owner,41,endpoint,actual)};
    if(error){executor.remove_node(node);node.reset();rclcpp::shutdown();return static_cast<int>(error);}
    require(endpoint&&wire::valid_response(actual)&&actual.kind==wire::role::subscription&&actual.profiles==1&&!actual.gid_size);
    wire::packet inspected{};require(endpoint->inspect(owner,41,inspected)==0&&wire::same(inspected.actual[0],actual.actual[0]));
    // An independent original typed Publisher sends the complete real native
    // value, rather than pushing a synthetic record into the receive queue.
    auto publisher{gw::ros2_compat::create_publisher<Native>(node,topic,gw::ros2_qos::configured(cap))};require(publisher!=nullptr);
    wire::packet pub{};pub.kind=wire::role::publisher;pub.operation=wire::command::activate;pub.owner=owner;pub.incarnation=43;
    error=gw::native_rmw_metadata::publisher(*publisher,pub);
    if(error){(void)endpoint->retire(owner,41);endpoint.reset();publisher.reset();executor.remove_node(node);node.reset();rclcpp::shutdown();return static_cast<int>(error);}
    auto until{std::chrono::steady_clock::now()+std::chrono::seconds(5)};
    while((node->count_publishers(topic)!=1||node->count_subscribers(topic)!=1)&&std::chrono::steady_clock::now()<until){spin(executor);}
    require(node->count_publishers(topic)==1&&node->count_subscribers(topic)==1);
    gw::native_rmw_queue::statistics stats{};require(endpoint->observe(owner,41,stats)==0&&stats.accepted==0);
    publisher->publish(value);until=std::chrono::steady_clock::now()+std::chrono::seconds(5);
    do{spin(executor);require(endpoint->observe(owner,41,stats)==0);}while(!stats.records&&std::chrono::steady_clock::now()<until);
    require(stats.records==1&&stats.accepted==1&&stats.last_sequence==1);
    auto foreign{owner};++foreign.generation;gw::native_rmw_queue::snapshot untouched;untouched.sequence=97;
    require(endpoint->peek(foreign,41,pub.gid_size,untouched)==13&&untouched.sequence==97);
    require(endpoint->peek(owner,41,pub.gid_size==32?31:pub.gid_size+1,untouched)==95&&untouched.sequence==97);
    require(endpoint->observe(owner,41,stats)==0&&stats.records==1&&!stats.reserved);
    gw::native_rmw_queue::snapshot first;require(endpoint->peek(owner,41,pub.gid_size,first)==0&&first.size==info::header_bytes+expected.size());
    info::header received{};require(info::decode(first.bytes.get(),info::header_bytes,expected.size(),received)&&info::complete(received));
    require(wire::same(received.owner,owner)&&received.incarnation==41&&received.sequence==first.sequence);
    require(received.gid_size==pub.gid_size&&received.gid==pub.gid);
    require(std::equal(expected.begin(),expected.end(),first.bytes.get()+info::header_bytes));
    Native decoded;require(Codec::decode(first.bytes.get()+info::header_bytes,expected.size(),decoded)&&Codec::valid(decoded));
    std::vector<std::uint8_t> encoded;require(Codec::encode(decoded,encoded)&&encoded==expected);
    require(endpoint->cancel(owner,41,first.sequence,first.reservation)==0);
    gw::native_rmw_queue::snapshot second;require(endpoint->peek(owner,41,pub.gid_size,second)==0);
    require(second.sequence==first.sequence&&second.reservation!=first.reservation&&second.size==first.size&&
        std::memcmp(second.bytes.get(),first.bytes.get(),first.size)==0);
    require(endpoint->commit(owner,41,first.sequence,first.reservation)==13);
    require(endpoint->cancel(owner,41,first.sequence,first.reservation)==13);
    require(endpoint->commit(owner,41,second.sequence,second.reservation)==0);
    require(endpoint->observe(owner,41,stats)==0&&stats.records==0&&stats.consumed==1);
    require(endpoint->retire(foreign,41)==13&&endpoint->retire(owner,41)==0);
    require(endpoint->inspect(owner,41,inspected)==125&&endpoint->peek(owner,41,pub.gid_size,untouched)==125);
    publisher->publish(value);until=std::chrono::steady_clock::now()+std::chrono::milliseconds(100);
    while(std::chrono::steady_clock::now()<until){spin(executor);}
    require(endpoint->observe(owner,41,stats)==0&&stats.retired&&stats.accepted==1&&stats.records==0);
    until=std::chrono::steady_clock::now()+std::chrono::seconds(5);
    while(node->count_subscribers(topic)!=0&&std::chrono::steady_clock::now()<until){spin(executor);}
    require(node->count_subscribers(topic)==0);
    endpoint.reset();publisher.reset();executor.remove_node(node);node.reset();rclcpp::shutdown();
    std::fputs("actual-custom-owned-subscription: PASS_SELECTED_NATIVE_VALUE_AND_RETIRE_NOT_HOST_GUEST_ACCEPTANCE\n",stdout);return 0;
}
