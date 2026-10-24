#include "../../../../../src/ros/gateway/ros2/tf.h"
#include "common.h"
#include <rclcpp/rclcpp.hpp>
#include <atomic>
#include <cstdlib>
#include <new>

namespace
{
    // Stall one allocation made by the actual cached lookup/serialization on
    // the calling thread. Native executor allocations are never stalled.
    thread_local bool delay_lookup_allocation{};
    thread_local bool delayed_lookup_allocation{};
}
void* operator new(std::size_t size)
{
    if(delay_lookup_allocation)
    {
        delay_lookup_allocation=false;delayed_lookup_allocation=true;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    if(auto memory=std::malloc(size?size:1)) { return memory; }
    throw std::bad_alloc{};
}
void operator delete(void* memory) noexcept { std::free(memory); }
void operator delete(void* memory,std::size_t) noexcept { std::free(memory); }

int main(int argc,char** argv)
{
    try
    {
        using namespace ros_probe;
        rclcpp::init(argc,argv);
        auto node{std::make_shared<rclcpp::Node>("uwvm_tf_deadline_adapter")};
        auto witness{std::make_shared<rclcpp::Node>("uwvm_tf_deadline_witness")};
        gateway::tf_adapter adapter{node};
        rclcpp::executors::MultiThreadedExecutor executor;
        executor.add_node(node);executor.add_node(witness);
        std::thread spinning{[&] { executor.spin(); }};
        struct cleanup
        {
            rclcpp::Executor& executor;std::thread& thread;
            ~cleanup() { executor.cancel();thread.join();rclcpp::shutdown(); }
        } stop{executor,spinning};
        auto cap{ros_tf_probe::capability(core::version::ros2,"lookup","tf.lookup")};
        std::vector<std::uint8_t> request(8);
        auto zero{adapter.call(cap,request,0)};
        require(zero.error==110&&zero.payload.empty(),"cold zero-budget lookup must return110 empty");
        require(node->count_subscribers("/tf")==0,"zero-budget lookup must create no native subscription");

        // A positive absent-frame call creates the real listener before native
        // data arrives; it must remain usable after that operation times out.
        require(adapter.call(cap,request,1000).error==110,"initial absent native transform must time out");
        auto publisher{witness->create_publisher<tf2_msgs::msg::TFMessage>("/tf",rclcpp::QoS{8}.reliable())};
        auto until{std::chrono::steady_clock::now()+std::chrono::seconds(4)};
        while(publisher->get_subscription_count()!=1)
        { require(std::chrono::steady_clock::now()<until,"actual TF native discovery");std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
        tf2_msgs::msg::TFMessage batch;geometry_msgs::msg::TransformStamped sample;
        sample.header.stamp.sec=123;sample.header.stamp.nanosec=456;
        sample.header.frame_id="map";sample.child_frame_id="base";
        sample.transform.translation.x=1.25;sample.transform.translation.y=2.5;sample.transform.translation.z=3.75;sample.transform.rotation.w=1.;
        batch.transforms.push_back(sample);publisher->publish(batch);
        auto warmed{adapter.call(cap,request,1500)};ros_tf_probe::assert_lookup(warmed);
        ipc::transform_data transformed;
        require(ipc::decode_transform(warmed.payload,transformed)&&transformed.seconds==123&&transformed.nanoseconds==456,"actual cached TF stamp preserved");
        zero=adapter.call(cap,request,0);
        require(zero.error==110&&zero.payload.empty(),"cached zero-budget lookup must not deliver a successful result");
        delayed_lookup_allocation=false;delay_lookup_allocation=true;
        auto late{adapter.call(cap,request,10)};
        delay_lookup_allocation=false;
        require(delayed_lookup_allocation,"actual cached lookup/serialization allocation was not exercised");
        require(late.error==110&&late.payload.empty(),"lookup result serialized after deadline must return110 empty");
        ros_tf_probe::assert_lookup(adapter.call(cap,request,1000));

        auto broadcast{ros_tf_probe::capability(core::version::ros2,"broadcast","tf.broadcast")};
        broadcast.resource="/tf_static";
        std::atomic<unsigned> observed{};
        auto subscriber{witness->create_subscription<tf2_msgs::msg::TFMessage>("/tf_static",rclcpp::QoS{8}.reliable(),[&](tf2_msgs::msg::TFMessage::ConstSharedPtr received)
        {
            for(auto const& transform:received->transforms)
            { if(transform.header.frame_id=="map"&&transform.child_frame_id=="base"&&transform.transform.translation.x==1.25) { ++observed; } }
        })};
        auto payload{ipc::encode_transform(ros_tf_probe::transform())};
        zero=adapter.call(broadcast,payload,0);
        require(zero.error==110&&zero.payload.empty(),"zero-budget broadcast must return110 empty");
        require(node->count_publishers("/tf_static")==0,"zero-budget broadcast must create no native publisher");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        require(!observed,"zero-budget broadcast caused a native publication");
        until=std::chrono::steady_clock::now()+std::chrono::seconds(4);
        while(!observed)
        {
            require(std::chrono::steady_clock::now()<until,"normal native TF broadcast was not observed");
            require(!adapter.call(broadcast,payload,1000).error,"normal TF broadcast failed");
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        adapter.close("lookup");adapter.close("broadcast");
        std::cout<<"PASS actual ROS2 TF cold/cached timeout0, no zero-budget native entities/publication, cached result serialization delayed50ms beyond10ms returns110 empty, normal cached lookup/broadcast\n";
        return 0;
    }
    catch(std::exception const& error) { delay_lookup_allocation=false;std::cerr<<error.what()<<'\n';return 1; }
}
