/** Real selected-SDK source witness, authored NOT RUN. No fake ROS headers.
 * It needs a separately admitted native namespace/process and original default
 * NodeOptions endpoints. Numeric source bindings below are fixture associations,
 * never reported as an actual Guest/Runtime session or authority proof.
 */
#ifdef NDEBUG
#error "Native registry witness checks must remain enabled"
#endif
#include "../../../../../../src/ros/gateway/ros2/native_rmw_subscription_registry.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>
namespace gw=uwvm2::ros_gateway;
namespace ipc=uwvm2::imported::ros::ipc;
namespace endpoint=ipc::rmw_endpoint;
namespace delivery=ipc::rmw_subscription;
static void check(bool value){if(!value){std::abort();}}
static gw::ros::capability_spec selected(std::string const& topic,bool publish)
{
    gw::ros::capability_spec c;c.alias=publish?"fixture-publisher":"fixture-subscription";
    c.operation=publish?"topic.publish":"topic.subscribe";c.type="std_msgs/msg/String";c.resource=topic;
    c.type_identity=gw::ros2_compat::builtin_catalog_identity(c.type);
    c.limits={{"max-message-bytes","4096"},{"max-queue-bytes","16384"},{"max-rate-hz","100"},{"burst","4"},
        {"max-bytes-per-second","65536"},{"overflow-policy","reject"},{"validator","none"},
        {"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.history","keep-last"},{"qos.depth","4"},
        {"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},
        {"qos.liveliness-lease-duration","infinite"},{publish?"max-publishers":"max-subscribers","1"}};
    return c;
}
static void spin(rclcpp::executors::SingleThreadedExecutor& executor)
{executor.spin_some();std::this_thread::sleep_for(std::chrono::milliseconds(1));}
static ipc::deadline until(){return std::chrono::steady_clock::now()+std::chrono::seconds(5);}
/** These are fault RESULT values, not a fake native endpoint or ROS model.
 * The production registry uses these exact helpers after its actual native
 * cancel/retire calls. Verify that three distinct failures, and successful later
 * cleanup, cannot erase the independent earlier cleanup result.
 */
static void cleanup_result_ledger_contract()
{
    namespace native=gw::native_rmw_subscription;
    native::delivery_diagnostics all_failed{};
    native::record_primary_error(all_failed,110);
    native::record_cancel_result(all_failed,17);
    native::record_retire_result(all_failed,5);
    check(all_failed.first_error==110&&all_failed.last_error==5&&all_failed.last_cleanup_error==5&&
        all_failed.cancel_attempted&&all_failed.retire_attempted&&all_failed.last_cancel_error==17&&all_failed.last_retire_error==5);
    native::delivery_diagnostics retired_ok{};
    native::record_primary_error(retired_ok,12);native::record_cancel_result(retired_ok,13);native::record_retire_result(retired_ok,0);
    check(retired_ok.first_error==12&&retired_ok.last_error==13&&retired_ok.last_cleanup_error==0&&
        retired_ok.last_cancel_error==13&&retired_ok.last_retire_error==0&&retired_ok.retire_attempted);
    native::delivery_diagnostics cancelled_ok{};
    native::record_primary_error(cancelled_ok,107);native::record_cancel_result(cancelled_ok,0);
    check(cancelled_ok.first_error==107&&cancelled_ok.last_error==107&&cancelled_ok.cancel_attempted&&
        !cancelled_ok.retire_attempted&&!cancelled_ok.last_cancel_error&&!cancelled_ok.last_retire_error);
    // Explicit cancel uses the SAME production helper, including an exception
    // translated to its actual error result, not only failed-send cleanup.
    native::delivery_diagnostics explicit_cancel{};
    native::record_primary_error(explicit_cancel,12);native::record_cancel_result(explicit_cancel,17);native::record_retire_result(explicit_cancel,5);
    check(explicit_cancel.first_error==12&&explicit_cancel.last_cancel_error==17&&explicit_cancel.last_retire_error==5&&explicit_cancel.cancel_attempted);
    native::delivery_diagnostics failed_commit{};
    native::record_commit_result(failed_commit,13);native::record_retire_result(failed_commit,5);
    check(failed_commit.commit_attempted&&failed_commit.last_commit_error==13&&!failed_commit.last_commit_completed&&failed_commit.last_retire_error==5);
    native::delivery_diagnostics late_success{};
    native::record_commit_result(late_success,0);native::record_primary_error(late_success,110);native::record_retire_result(late_success,5);
    check(late_success.commit_attempted&&!late_success.last_commit_error&&late_success.last_commit_completed&&
        late_success.first_error==110&&late_success.last_retire_error==5);
}
static std::uint32_t exercise(rclcpp::Node::SharedPtr const& node,rclcpp::executors::SingleThreadedExecutor& executor,std::string const& topic)
{
    auto subscribe=selected(topic,false),publish=selected(topic,true);
    gw::native_rmw_subscription::registry registry{node};
    endpoint::binding owner{2,UINT64_C(0xc123000180000001),7,11,13};
    endpoint::packet activation{};activation.kind=endpoint::role::subscription;activation.owner=owner;
    auto result=registry.control(subscribe,activation,until());if(result.error){return result.error;}
    endpoint::packet actual{};check(endpoint::associated_response(result.payload.data(),result.payload.size(),activation,actual)&&
        actual.profiles==1&&actual.incarnation&&!actual.gid_size);
    auto inspect=activation;inspect.operation=endpoint::command::inspect;inspect.incarnation=actual.incarnation;
    auto observed=registry.control(subscribe,inspect,until());endpoint::packet reobserved{};
    check(!observed.error&&endpoint::associated_response(observed.payload.data(),observed.payload.size(),inspect,reobserved)&&endpoint::same(actual.actual[0],reobserved.actual[0]));
    std::shared_ptr<gw::native_rmw_publisher::endpoint> publisher;endpoint::packet published{};
    endpoint::binding sender{2,UINT64_C(0xc123000280000001),17,19,23};
    auto code=gw::native_rmw_builtin::publisher<gw::native_rmw_builtin::string_codec>::create(node,publish,sender,31,publisher,published);
    if(code){registry.clear();return code;}
    auto deadline=until();
    // Discovery is an independent fixture observation, never used to grant or
    // select the registry's already-created actual retained native endpoint.
    while((node->count_publishers(topic)!=1||node->count_subscribers(topic)!=1)&&std::chrono::steady_clock::now()<deadline){spin(executor);}
    check(node->count_publishers(topic)==1&&node->count_subscribers(topic)==1);
    std_msgs::msg::String value;value.data=std::string{"same\0record",11};std::vector<std::uint8_t> canonical;
    check(gw::native_rmw_builtin::string_codec::encode(value,canonical));
    check(publisher->publish(sender,31,canonical.data(),canonical.size())==0);
    gw::native_rmw_subscription::delivery_diagnostics audit{};gw::native_rmw_queue::statistics stats{};deadline=until();
    do{spin(executor);check(!registry.observe(subscribe,owner,actual.incarnation,audit,stats));}while(!stats.records&&std::chrono::steady_clock::now()<deadline);
    check(stats.records==1&&stats.accepted==1&&stats.last_sequence==1&&stats.last_reservation==0);
    delivery::control peek{delivery::command::peek,owner,actual.incarnation,0,0,1024,0,published.gid_size};
    auto short_output=peek;short_output.capacity=delivery::minimum_delivery_bytes;
    check(registry.transfer(subscribe,short_output,until()).error==90);
    auto wrong_gid=peek;wrong_gid.gid_bytes=published.gid_size==32?31:published.gid_size+1;
    check(registry.transfer(subscribe,wrong_gid,until()).error==95);
    check(!registry.observe(subscribe,owner,actual.incarnation,audit,stats)&&stats.records==1&&!stats.reserved&&!stats.last_reservation);
    auto first=registry.transfer(subscribe,peek,until());check(!first.error);delivery::control a{};ipc::rmw_metadata::header metadata{};
    check(delivery::associated_delivery(first.payload.data(),first.payload.size(),peek,a,metadata)&&metadata.sequence==1&&metadata.gid==published.gid&&metadata.gid_size==published.gid_size);
    check(first.payload.size()==96+192+canonical.size()&&std::equal(canonical.begin(),canonical.end(),first.payload.begin()+96+192));
    std_msgs::msg::String reconstructed;check(gw::native_rmw_builtin::string_codec::decode(first.payload.data()+288,canonical.size(),reconstructed)&&reconstructed.data==value.data);
    delivery::control cancel{delivery::command::cancel,owner,actual.incarnation,a.sequence,a.reservation};
    auto cancelled=registry.transfer(subscribe,cancel,until());delivery::control acknowledgement{};
    check(!cancelled.error&&delivery::associated_ack(cancelled.payload.data(),cancelled.payload.size(),cancel,acknowledgement));
    auto second=registry.transfer(subscribe,peek,until());delivery::control b{};
    check(!second.error&&delivery::associated_delivery(second.payload.data(),second.payload.size(),peek,b,metadata)&&b.sequence==a.sequence&&b.reservation>a.reservation);
    auto obsolete=cancel;obsolete.operation=delivery::command::commit;
    check(registry.transfer(subscribe,obsolete,until()).error==13&&registry.transfer(subscribe,cancel,until()).error==13);
    // A failed send cancels only the current reservation, never its record.
    check(!registry.delivery_failed(subscribe,owner,actual.incarnation,107));
    check(!registry.observe(subscribe,owner,actual.incarnation,audit,stats)&&stats.records==1&&!stats.reserved&&audit.transport_failures==1&&audit.first_error);
    auto third=registry.transfer(subscribe,peek,until());delivery::control c{};
    check(!third.error&&delivery::associated_delivery(third.payload.data(),third.payload.size(),peek,c,metadata)&&c.sequence==b.sequence&&c.reservation>b.reservation);
    delivery::control commit{delivery::command::commit,owner,actual.incarnation,c.sequence,c.reservation};
    check(registry.transfer(subscribe,commit,std::chrono::steady_clock::now()).error==110);
    check(!registry.observe(subscribe,owner,actual.incarnation,audit,stats)&&stats.records==1&&stats.reserved);
    auto committed=registry.transfer(subscribe,commit,until());
    check(!committed.error&&delivery::associated_ack(committed.payload.data(),committed.payload.size(),commit,acknowledgement));
    check(registry.transfer(subscribe,commit,until()).error==13);
    check(!registry.observe(subscribe,owner,actual.incarnation,audit,stats)&&stats.records==0&&stats.consumed==1&&audit.committed==1&&audit.cancelled==2);
    auto retire=inspect;retire.operation=endpoint::command::retire;auto retired=registry.control(subscribe,retire,until());
    check(!retired.error&&endpoint::associated_response(retired.payload.data(),retired.payload.size(),retire,reobserved));
    check(registry.transfer(subscribe,peek,until()).error==125&&registry.control(subscribe,activation,until()).error==125);
    deadline=until();while(node->count_subscribers(topic)!=0&&std::chrono::steady_clock::now()<deadline){spin(executor);}
    check(node->count_subscribers(topic)==0);
    registry.clear();check(publisher->retire(sender,31)==0);publisher.reset();return 0;
}
int main(int argc,char** argv)
{
    if(argc!=3||std::string_view{argv[1]}!="--trusted-native-fixture"){return 13;}
    std::string topic=argv[2];if(topic.empty()||topic.front()!='/'){return 22;}
    if constexpr(!UWVM_ROS2_HAVE_COMPLETE_RMW_QOS||!gw::native_rmw_metadata::has_times<rmw_message_info_t>::value||
        !gw::native_rmw_metadata::has_entity_actual<rclcpp::Subscription<std_msgs::msg::String>>::value){return 95;}
    cleanup_result_ledger_contract();
    // Actual SDK defaults stay intact. Root must admit their true source/ROS
    // entity closure and bound the entire process, executor and native cleanup.
    rclcpp::init(argc,argv);auto node=std::make_shared<rclcpp::Node>("uwvm_subscription_registry_contract");
    rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);
    auto code=exercise(node,executor,topic);executor.remove_node(node);node.reset();rclcpp::shutdown();
    if(!code){std::fputs("real-native-subscription-registry-same-record: PASS\n",stdout);}return static_cast<int>(code);
}
