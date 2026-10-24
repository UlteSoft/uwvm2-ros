/**
 * @file native_topics.cc
 * @brief Prepared real ROS2 fixed custom topic and rejection fixture.
 *
 * Compile and execute only inside the verified existing Linux 64 GiB cgroup.
 * Build the official interfaces package first, run the adapter generator against
 * its authoritative ROS2 description/hash artifacts, and select that exact
 * common/native provider pair for this fixture. No fabricated hash is embedded
 * here. This fixture exercises the real custom adapter against native DDS
 * publishers/subscribers; it does not claim Guest or authenticated-IPC coverage.
 */
#include "../../../../../../src/ros/gateway/ros2/custom_topics.h"
#include <uwvm_ros_custom_test_msgs/msg/fixed_sample.hpp>
#include <fast_io.h>
#include <rclcpp/executors/single_threaded_executor.hpp>
#include <chrono>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <thread>

namespace custom_ros2_test
{
    namespace gateway=uwvm2::ros_gateway;
    namespace custom=gateway::ros2_custom_topics;
    namespace ipc=uwvm2::imported::ros::ipc;
    using sample=uwvm_ros_custom_test_msgs::msg::FixedSample;
    using steady=std::chrono::steady_clock;
    unsigned checks{};

    void require(bool condition,char const* message)
    { ++checks;if(!condition) { throw std::runtime_error(message); } }

    template<typename Predicate>
    bool pump(rclcpp::executors::SingleThreadedExecutor& executor,
        std::chrono::milliseconds duration,Predicate predicate)
    {
        auto until{steady::now()+duration};
        do
        {
            executor.spin_some();if(predicate()) { return true; }
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        } while(steady::now()<until);
        return predicate();
    }

    gateway::ros::capability_spec capability(char const* alias,char const* operation,
        std::string_view name,std::string_view native_identity,std::string_view codec_identity)
    {
        gateway::ros::capability_spec cap;cap.alias=alias;cap.operation=operation;
        cap.resource="/uwvm_custom_ros2/fixed_sample";cap.type=name;cap.type_identity=native_identity;
        cap.limits={{"adapter-codec-identity",std::string{codec_identity}},
            {"max-message-bytes","45"},{"max-rate-hz","1000000"},{"burst","8"},
            {"max-bytes-per-second","1000000"},{"max-queue-bytes","360"},
            {"overflow-policy","reject"},{"validator","none"},{"audit","allow-and-deny"},
            {operation==std::string_view{"topic.publish"}?"max-publishers":"max-subscribers","1"},
            {"max-inflight","8"},{"qos.reliability","reliable"},{"qos.durability","volatile"},
            {"qos.history","keep-last"},{"qos.depth","8"},{"qos.deadline","infinite"},
            {"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"},
            {"audit-max-rate-hz","1000"},{"audit-burst","8"},{"audit-max-records","1000"},{"audit-max-bytes","65536"}};
        return cap;
    }

    sample value()
    {
        sample result;result.sequence=0x12345678;result.enabled=true;result.temperature=21.25f;
        result.acceleration={{1.5,-2.25,3.125}};result.tag={{0,1,2,127,128,253,254,255}};
        result.thresholds={{std::numeric_limits<std::int16_t>::min(),std::numeric_limits<std::int16_t>::max()}};
        return result;
    }

    template<typename Native,typename Codec>
    bool run(std::string_view name,std::string_view native_identity,std::string_view codec_identity,std::size_t wire_bytes)
    {
        if constexpr(!std::is_same_v<Native,sample>) { return false; }
        else
        {
            require(name=="uwvm_ros_custom_test_msgs/msg/FixedSample"&&wire_bytes==45,"unexpected fixture provider row");
            auto context{std::make_shared<rclcpp::Context>()};
            char const* arguments[]{"uwvm-custom-topic-fixture","--ros-args"};context->init(2,arguments);
            rclcpp::NodeOptions options;options.context(context);options.enable_rosout(false);
            options.start_parameter_services(false);options.start_parameter_event_publisher(false);
            options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
            auto gateway_node{std::make_shared<rclcpp::Node>("custom_gateway","/uwvm_custom_ros2",options)};
            auto peer{std::make_shared<rclcpp::Node>("custom_peer","/uwvm_custom_ros2",options)};
            rclcpp::ExecutorOptions executor_options;executor_options.context=context;
            rclcpp::executors::SingleThreadedExecutor executor{executor_options};executor.add_node(gateway_node);executor.add_node(peer);
            auto publish{capability("outgoing","topic.publish",name,native_identity,codec_identity)};
            auto subscribe{capability("incoming","topic.subscribe",name,native_identity,codec_identity)};
            require(custom::known(publish)&&custom::known(subscribe),"official support identity did not qualify");
            auto const* descriptor{ipc::find_custom_topic(2,name)};require(descriptor!=nullptr,"common descriptor absent");
            sample expected{value()},received;unsigned observed{},invalid_native_observed{};
            auto configured{gateway::ros2_qos::configured(publish)};
            auto native_subscription{peer->create_subscription<sample>(publish.resource,configured,
                [&](std::shared_ptr<sample const> message)
                { received=*message;++observed;if(!Codec::valid(*message)) { ++invalid_native_observed; } })};
            auto native_publisher{peer->create_publisher<sample>(publish.resource,configured)};
            std::vector<std::uint8_t> bytes(wire_bytes);
            require(Codec::valid(expected)&&Codec::encode(expected,bytes.data(),bytes.size()),"generated sample encoding failed");
            require(ipc::validate_custom_topic(*descriptor,bytes.data(),bytes.size()),"shared sample scanner refused generated bytes");
            {
                custom::adapter backend{gateway_node};
                // Initial publication registers the explicit native publisher.
                // Wait for discovery before the measured publication because
                // volatile DDS does not promise delivery before matching.
                require(!backend.publish(publish,bytes).error,"publisher registration failed");
                require(pump(executor,std::chrono::seconds(5),[&] { return native_subscription->get_publisher_count()>=2; }),"native publisher discovery failed");
                pump(executor,std::chrono::milliseconds(200),[] { return false; });
                observed=0;require(!backend.publish(publish,bytes).error,"measured custom publication failed");
                require(pump(executor,std::chrono::seconds(5),[&] { return observed!=0; }),"native peer received no custom message");
                require(received==expected,"canonical-to-native fields differ");
                pump(executor,std::chrono::milliseconds(200),[] { return false; });

                // Capture actual native observations after each invalid request.
                // Returning an error while publishing a malformed message fails.
                auto reject=[&](std::vector<std::uint8_t> const& invalid,std::uint32_t error,char const* reason)
                {
                    executor.spin_some();auto before{observed};require(backend.publish(publish,invalid).error==error,reason);
                    pump(executor,std::chrono::milliseconds(150),[] { return false; });
                    require(observed==before,"rejected request produced an actual native message");
                };
                auto truncated{bytes};truncated.pop_back();reject(truncated,22,"truncated record accepted");
                auto extra{bytes};extra.push_back(0);reject(extra,22,"trailing byte accepted");
                auto boolean{bytes};boolean[4]=2;reject(boolean,22,"noncanonical boolean accepted");
                auto infinite{bytes};ipc::custom_write_bits(infinite.data()+5,0x7f800000,4);reject(infinite,22,"infinite float accepted");
                auto nan{bytes};ipc::custom_write_bits(nan.data()+9,0x7ff8000000000001ULL,8);reject(nan,22,"NaN double accepted");
                auto changed{publish};changed.resource="/uwvm_custom_ros2/other";
                require(backend.publish(changed,bytes).error==13,"alias changed resource accepted");
                pump(executor,std::chrono::milliseconds(200),[] { return false; });
                require(!peer->get_topic_names_and_types().count(changed.resource),"rejected alias registered another native topic");
                changed=publish;changed.type_identity.back()=changed.type_identity.back()=='0'?'1':'0';
                require(backend.publish(changed,bytes).error==95,"wrong native hash accepted");
                changed=publish;changed.limits["adapter-codec-identity"].back()='z';
                require(backend.publish(changed,bytes).error==95,"wrong codec identity accepted");
                changed=publish;changed.operation="service.call";
                require(!custom::known(changed)&&backend.publish(changed,bytes).error==95,"custom service authority inferred from a topic");
                changed=publish;changed.limits["max-message-bytes"]="44";
                require(backend.publish(changed,bytes).error==90,"explicit smaller byte quota ignored");

                auto queue{std::make_shared<gateway::event_queue>(subscribe)};
                require(!backend.subscribe(subscribe,queue).error,"custom native subscription failed");
                require(pump(executor,std::chrono::seconds(5),[&] { return native_publisher->get_subscription_count()>=2; }),"native subscriber discovery failed");
                native_publisher->publish(expected);ipc::call_result event;
                require(pump(executor,std::chrono::seconds(5),[&] { event=queue->poll();return !event.error; }),"native-to-canonical event missing");
                require(event.payload==bytes,"native-to-canonical bytes differ");
                auto nonfinite{expected};nonfinite.temperature=std::numeric_limits<float>::infinity();
                auto invalid_before{invalid_native_observed};native_publisher->publish(nonfinite);
                require(pump(executor,std::chrono::seconds(5),[&] { return invalid_native_observed>invalid_before; }),"native witness did not receive the infinite float");
                pump(executor,std::chrono::milliseconds(200),[] { return false; });
                require(queue->poll().error==11,"nonfinite native message entered the Guest queue");
                nonfinite=expected;nonfinite.acceleration[1]=std::numeric_limits<double>::quiet_NaN();
                invalid_before=invalid_native_observed;native_publisher->publish(nonfinite);
                require(pump(executor,std::chrono::seconds(5),[&] { return invalid_native_observed>invalid_before; }),"native witness did not receive the NaN double");
                pump(executor,std::chrono::milliseconds(200),[] { return false; });
                require(queue->poll().error==11,"native NaN entered the Guest queue");
                backend.close(subscribe.alias);require(queue->poll().error==125,"close did not deactivate the queue");
                native_publisher->publish(expected);pump(executor,std::chrono::milliseconds(200),[] { return false; });
                require(queue->poll().error==125,"closed subscription accepted a later native message");
                backend.close(publish.alias);
            }
            executor.remove_node(gateway_node);executor.remove_node(peer);context->shutdown("custom fixture complete");
            return true;
        }
    }
}

int main(int argc,char**)
{
    try
    {
        custom_ros2_test::require(argc==1,"usage: custom ROS2 topic fixture takes no arguments");
        // Use the explicitly reserved fixture domain and disable security only
        // for this native tutorial test. Production remains policy controlled.
        if(::setenv("ROS_DOMAIN_ID","203",1)||::setenv("ROS_SECURITY_ENABLE","false",1))
        { throw std::runtime_error("cannot set explicit fixture ROS context"); }
        unsigned selected{};
#define UWVM_CUSTOM_ROS2_FIXTURE(Native,Name,NativeId,CodecId,Bytes,Codec) \
        if(custom_ros2_test::run<Native,Codec>(Name,NativeId,CodecId,Bytes)) { ++selected; }
        UWVM_ROS_CUSTOM_ROS2_TYPES(UWVM_CUSTOM_ROS2_FIXTURE)
#undef UWVM_CUSTOM_ROS2_FIXTURE
        custom_ros2_test::require(selected==1,"fixture requires exactly one official FixedSample provider row");
        fast_io::println("PASS prepared real ROS2 fixed custom topic fixture checks=",custom_ros2_test::checks);
        return 0;
    }
    catch(std::exception const& error) { fast_io::perrln("FAIL custom ROS2 fixed topic fixture: ",fast_io::mnp::os_c_str(error.what()));return 1; }
}
