/** Real original-SDK witness, authored but NOT RUN. This deliberately creates
 * native fixture entities only with an explicit trusted-process command-line
 * opt-in. It is not a Guest policy, graph-derived grant or production backend.
 * The external owner must bound the process, network namespace and teardown.
 */
#ifdef NDEBUG
#error "Native witness checks must remain active"
#endif
#include "../../../gateways/ros2/native_rmw_subscription.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>
namespace gw=uwvm2::ros_gateway;
namespace ipc=uwvm2::imported::ros::ipc;
namespace w=ipc::rmw_endpoint;
namespace m=ipc::rmw_metadata;
static void check(bool value){if(!value){std::abort();}}
static uwvm2::imported::ros::capability_spec capability(std::string const& topic,std::string_view type,bool publish)
{
    uwvm2::imported::ros::capability_spec c;c.alias=publish?"native-fixture-publish":"native-fixture-subscribe";
    c.operation=publish?"topic.publish":"topic.subscribe";c.resource=topic;c.type=type;
    c.type_identity=gw::ros2_compat::builtin_catalog_identity(type);
    c.limits={{"max-message-bytes","4096"},{"max-queue-bytes","16384"},{"max-rate-hz","100"},{"burst","4"},
        {"max-bytes-per-second","65536"},{"overflow-policy","reject"},{"validator","none"},
        {"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.history","keep-last"},{"qos.depth","4"},
        {"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}};
    return c;
}
static void spin(rclcpp::executors::SingleThreadedExecutor& executor)
{executor.spin_some();std::this_thread::sleep_for(std::chrono::milliseconds(1));}
template<class Codec>static std::uint32_t exercise(rclcpp::Node::SharedPtr const& node,
    rclcpp::executors::SingleThreadedExecutor& executor,std::string const& topic,typename Codec::Native const& value,unsigned ordinal)
{
    // Fixture-local association data, never represented as an observed Host
    // source handle/session or as a credential from the production Runtime.
    w::binding publisher_owner{2,UINT64_C(0xabcd000180000001)+ordinal,7,11,13};
    w::binding subscriber_owner{2,UINT64_C(0xabcd000280000001)+ordinal,17,19,23};
    std::uint64_t publisher_incarnation{31+ordinal},subscriber_incarnation{41+ordinal};
    auto pc{capability(topic,Codec::type,true)},sc{capability(topic,Codec::type,false)};
    std::shared_ptr<gw::native_rmw_publisher::endpoint> publisher;
    std::shared_ptr<gw::native_rmw_subscription::endpoint> subscriber;
    w::packet published{},subscribed{};
    auto code{gw::native_rmw_subscription::builtin<Codec>::create(node,sc,subscriber_owner,subscriber_incarnation,subscriber,subscribed)};
    if(code){return code;}
    code=gw::native_rmw_builtin::publisher<Codec>::create(node,pc,publisher_owner,publisher_incarnation,publisher,published);
    if(code){(void)subscriber->retire(subscriber_owner,subscriber_incarnation);return code;}
    check(subscribed.kind==w::role::subscription&&subscribed.profiles==1&&!subscribed.gid_size);
    w::packet inspected{};check(subscriber->inspect(subscriber_owner,subscriber_incarnation,inspected)==0&&w::same(inspected.actual[0],subscribed.actual[0]));
    auto until{std::chrono::steady_clock::now()+std::chrono::seconds(5)};
    while((node->count_publishers(topic)!=1||node->count_subscribers(topic)!=1)&&std::chrono::steady_clock::now()<until){spin(executor);}
    check(node->count_publishers(topic)==1&&node->count_subscribers(topic)==1);
    gw::native_rmw_queue::statistics observation{};
    check(subscriber->observe(subscriber_owner,subscriber_incarnation,observation)==0&&observation.accepted==0&&observation.records==0);
    std::vector<std::uint8_t> expected;check(Codec::encode(value,expected));
    check(publisher->publish(publisher_owner,publisher_incarnation,expected.data(),expected.size())==0);
    until=std::chrono::steady_clock::now()+std::chrono::seconds(5);
    do{spin(executor);check(subscriber->observe(subscriber_owner,subscriber_incarnation,observation)==0);}
    while(!observation.records&&std::chrono::steady_clock::now()<until);
    check(observation.records==1&&observation.accepted==1&&observation.last_sequence==1);
    auto forged{subscriber_owner};++forged.generation;gw::native_rmw_queue::snapshot sentinel;sentinel.sequence=97;
    check(subscriber->peek(forged,subscriber_incarnation,published.gid_size,sentinel)==13&&sentinel.sequence==97);
    check(subscriber->peek(subscriber_owner,subscriber_incarnation,published.gid_size==32?31:published.gid_size+1,sentinel)==95&&sentinel.sequence==97);
    check(subscriber->observe(subscriber_owner,subscriber_incarnation,observation)==0&&observation.records==1&&!observation.reserved);
    gw::native_rmw_queue::snapshot actual;
    check(subscriber->peek(subscriber_owner,subscriber_incarnation,published.gid_size,actual)==0&&actual.size==192+expected.size());
    m::header header{};check(m::decode(actual.bytes.get(),192,expected.size(),header)&&m::complete(header));
    check(w::same(header.owner,subscriber_owner)&&header.incarnation==subscriber_incarnation&&header.sequence==1);
    check(header.gid_size==published.gid_size&&header.gid==published.gid);
    check(std::equal(expected.begin(),expected.end(),actual.bytes.get()+192));
    typename Codec::Native reconstructed;check(Codec::decode(actual.bytes.get()+192,expected.size(),reconstructed));
    std::vector<std::uint8_t> canonical;check(Codec::encode(reconstructed,canonical)&&canonical==expected);
    check(subscriber->cancel(subscriber_owner,subscriber_incarnation,actual.sequence,actual.reservation)==0);
    gw::native_rmw_queue::snapshot again;check(subscriber->peek(subscriber_owner,subscriber_incarnation,published.gid_size,again)==0);
    check(again.sequence==actual.sequence&&again.size==actual.size&&std::memcmp(again.bytes.get(),actual.bytes.get(),actual.size)==0);
    check(subscriber->commit(subscriber_owner,subscriber_incarnation,again.sequence,again.reservation)==0&&subscriber->commit(subscriber_owner,subscriber_incarnation,again.sequence,again.reservation)==13);
    check(subscriber->observe(subscriber_owner,subscriber_incarnation,observation)==0&&observation.consumed==1&&observation.records==0);
    check(subscriber->retire(forged,subscriber_incarnation)==13&&subscriber->retire(subscriber_owner,subscriber_incarnation)==0);
    check(subscriber->inspect(subscriber_owner,subscriber_incarnation,inspected)==125&&subscriber->peek(subscriber_owner,subscriber_incarnation,published.gid_size,sentinel)==125);
    // The publisher still exists: a post-retirement real native publication
    // cannot create business in the retired queue, even if an executor retained
    // an old callback/subscription shared reference for this scheduling turn.
    check(publisher->publish(publisher_owner,publisher_incarnation,expected.data(),expected.size())==0);
    until=std::chrono::steady_clock::now()+std::chrono::milliseconds(100);
    while(std::chrono::steady_clock::now()<until){spin(executor);}
    check(subscriber->observe(subscriber_owner,subscriber_incarnation,observation)==0&&observation.accepted==1&&observation.records==0&&observation.retired);
    until=std::chrono::steady_clock::now()+std::chrono::seconds(5);
    while(node->count_subscribers(topic)!=0&&std::chrono::steady_clock::now()<until){spin(executor);}
    check(node->count_subscribers(topic)==0);
    check(publisher->retire(publisher_owner,publisher_incarnation)==0);publisher.reset();subscriber.reset();return 0;
}
int main(int argc,char** argv)
{
    if(argc!=3||std::string_view{argv[1]}!="--trusted-native-fixture"){return 13;}
    std::string prefix{argv[2]};if(prefix.empty()||prefix.front()!='/'||prefix.back()=='/'){return 22;}
    if constexpr(!gw::native_rmw_metadata::has_times<rmw_message_info_t>::value){return 95;}
    // Native init/NodeOptions are the original SDK defaults, not quieted by
    // this witness. A real external namespace/policy owner must admit them.
    rclcpp::init(argc,argv);auto node{std::make_shared<rclcpp::Node>("uwvm_native_subscription_contract")};
    rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);std::uint32_t code{};
    std_msgs::msg::String text;text.data=std::string{"native\0whole",12};
    std_msgs::msg::Bool flag;flag.data=true;
    geometry_msgs::msg::Twist twist;twist.linear.x=-0.0;twist.linear.y=1.25;twist.linear.z=-2.5;twist.angular.x=3;twist.angular.y=-4;twist.angular.z=5;
    sensor_msgs::msg::Image image;image.header.stamp.sec=17;image.header.stamp.nanosec=23;image.header.frame_id="actual-camera";
    image.height=1;image.width=2;image.encoding="mono8";image.is_bigendian=0;image.step=2;
    std::array<std::uint8_t,2> pixels{{0,255}};gw::ros2_compat::assign_bytes(image.data,pixels.begin(),pixels.end());
    code=exercise<gw::native_rmw_builtin::string_codec>(node,executor,prefix+"/string",text,0);
    if(!code){code=exercise<gw::native_rmw_builtin::bool_codec>(node,executor,prefix+"/bool",flag,1);}
    if(!code){code=exercise<gw::native_rmw_builtin::twist_codec>(node,executor,prefix+"/twist",twist,2);}
    if(!code){code=exercise<gw::native_rmw_builtin::image_codec>(node,executor,prefix+"/image",image,3);}
    executor.remove_node(node);node.reset();rclcpp::shutdown();
    if(!code){std::fputs("actual-native-subscription-four-complete-values: PASS\n",stdout);}return static_cast<int>(code);
}
