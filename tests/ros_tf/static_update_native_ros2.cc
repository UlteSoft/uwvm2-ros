#include "../../gateways/ros2/tf.h"
#include "common.h"
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/executors/single_threaded_executor.hpp>
#include <atomic>
int main(int argc,char** argv)
{
    try
    {
        using namespace ros_probe;
        rclcpp::init(argc,argv);
        auto node{std::make_shared<rclcpp::Node>("uwvm_static_same_stamp_adapter")};
        auto witness{std::make_shared<rclcpp::Node>("uwvm_static_same_stamp_witness")};
        gateway::tf_adapter adapter{node};
        rclcpp::executors::SingleThreadedExecutor executor;
        executor.add_node(node);executor.add_node(witness);
        std::thread spinning{[&] { executor.spin(); }};
        struct cleanup
        {
            rclcpp::Executor& executor;std::thread& thread;
            ~cleanup() { executor.cancel();thread.join();rclcpp::shutdown(); }
        } stop{executor,spinning};
        auto cap{ros_tf_probe::capability(core::version::ros2,"static_update","tf.lookup")};
        cap.resource="/tf_static";cap.limits["qos.durability"]="transient-local";
        cap.limits["max-rate-hz"]="1000";cap.limits["burst"]="100";
        cap.limits["max-bytes-per-second"]="1048576";
        std::vector<std::uint8_t> request(8);
        require(adapter.call(cap,request,30).error==110,"cold static lookup must create listener and time out");
        auto qos{rclcpp::QoS{8}.reliable().transient_local()};
        tf2::BufferCore reference;std::mutex reference_mutex;
        std::atomic<unsigned> updates{};std::atomic<bool> rejected{};
        auto reference_subscription{witness->create_subscription<tf2_msgs::msg::TFMessage>("/tf_static",qos,
            [&](tf2_msgs::msg::TFMessage::ConstSharedPtr batch)
            {
                std::lock_guard<std::mutex> guard{reference_mutex};
                for(auto const& sample:batch->transforms)
                {
                    if(!reference.setTransform(sample,"real-dds-static-reference",true)) { rejected=true; }
                    ++updates;
                }
            })};
        auto publisher{witness->create_publisher<tf2_msgs::msg::TFMessage>("/tf_static",qos)};
        auto until{std::chrono::steady_clock::now()+std::chrono::seconds(4)};
        while(publisher->get_subscription_count()!=2)
        { require(std::chrono::steady_clock::now()<until,"actual static TF discovery needs exactly two subscriptions");std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
        tf2_msgs::msg::TFMessage batch;geometry_msgs::msg::TransformStamped sample;
        sample.header.frame_id="map";sample.child_frame_id="base";
        sample.header.stamp.sec=0;sample.header.stamp.nanosec=0;
        sample.transform.rotation.w=1.;sample.transform.translation.x=1.25;
        batch.transforms.push_back(sample);publisher->publish(batch);
        auto read=[&]
        {
            auto result{adapter.call(cap,request,1000)};ipc::transform_data transform;
            require(result.error==0&&ipc::decode_transform(result.payload,transform),"actual static gateway lookup must return canonical transform");
            return transform;
        };
        require(read().values[0]==1.25,"first actual static transform did not reach gateway");
        batch.transforms[0].transform.translation.x=2.5;publisher->publish(batch);
        until=std::chrono::steady_clock::now()+std::chrono::seconds(4);
        while(updates.load()<2)
        { require(std::chrono::steady_clock::now()<until,"reference did not receive actual DDS same-stamp update");std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
        double native_value{};
        {
            std::lock_guard<std::mutex> guard{reference_mutex};
            native_value=reference.lookupTransform("map","base",tf2::TimePointZero).transform.translation.x;
        }
        require(!rejected.load()&&native_value==2.5,"native BufferCore rejected same-stamp static update");
        // Poll the real adapter while its executor handles the same DDS update.
        // The probe never mutates or reads the adapter's private cache.
        auto actual{read()};until=std::chrono::steady_clock::now()+std::chrono::milliseconds(500);
        while(actual.values[0]!=2.5&&std::chrono::steady_clock::now()<until)
        { std::this_thread::sleep_for(std::chrono::milliseconds(10));actual=read(); }
        std::cout<<"OBSERVED actual_dds_updates="<<updates.load()<<" native_static="<<native_value
            <<" gateway_static="<<actual.values[0]<<" stamp="<<actual.seconds<<':'<<actual.nanoseconds<<'\n'<<std::flush;
        adapter.close(cap.alias);
        require(actual.seconds==0&&actual.nanoseconds==0&&actual.values[0]==2.5,
            "same-stamp static transform update was discarded by gateway");
        std::cout<<"PASS actual DDS static transform same-stamp replacement matches native BufferCore\n";
        return 0;
    }
    catch(std::exception const& error) { std::cerr<<error.what()<<'\n';return 1; }
}
