// Exercise the actual production Topic adapter, including its generated type
// identity and QoS checks, through real native DDS publication/subscriptions.
#define main uwvm_ros2_gateway_production_main
#include "../../gateways/ros2/main.cc"
#undef main
#include "probe_common.h"

int main(int argc,char** argv)
{
    try
    {
        using namespace ros_probe;
        rclcpp::init(argc,argv);
        auto node{std::make_shared<rclcpp::Node>("uwvm_image_shape_adapter")};
        auto witness{std::make_shared<rclcpp::Node>("uwvm_image_shape_witness")};
        ros2_adapter adapter{node};
        auto cap{topic(core::version::ros2,"image","topic.subscribe","/uwvm_image_shape/native","sensor_msgs/msg/Image",
            "RIHS01_d31d41a9a4c4bc8eae9be757b0beed306564f7526c88ea6a4588fb9582527d47")};
        cap.limits["burst"]="32";
        auto events{std::make_shared<gateway::event_queue>(cap)};
        require(!adapter.subscribe(cap,events).error,"actual native Image adapter subscription/type/QoS admission");
        unsigned native_received{},invalid_native{};
        auto witness_subscription{witness->create_subscription<sensor_msgs::msg::Image>(cap.resource,rclcpp::QoS{8}.reliable(),[&](sensor_msgs::msg::Image::ConstSharedPtr message)
        { ++native_received;if(message->is_bigendian>1) { ++invalid_native; } })};
        auto publisher{witness->create_publisher<sensor_msgs::msg::Image>(cap.resource,rclcpp::QoS{8}.reliable())};
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);executor.add_node(witness);
        auto pump=[&](auto condition,char const* failure)
        {
            auto until{std::chrono::steady_clock::now()+std::chrono::seconds(4)};
            while(!condition())
            { require(std::chrono::steady_clock::now()<until,failure);executor.spin_some();std::this_thread::sleep_for(std::chrono::milliseconds(5)); }
        };
        pump([&] { return publisher->get_subscription_count()==2; },"actual native Image DDS discovery");
        sensor_msgs::msg::Image image;image.header.stamp.sec=123;image.header.stamp.nanosec=456;
        image.header.frame_id="camera";image.height=1;image.width=1;image.encoding="mono8";image.step=1;image.data={17};
        for(auto invalid:{std::uint8_t{2},std::uint8_t{255}})
        {
            image.is_bigendian=invalid;auto before{native_received};publisher->publish(image);
            pump([&] { return native_received>before; },"independent native witness did not receive invalid Image endian byte");
            image.is_bigendian=invalid==2?0:1;before=native_received;publisher->publish(image);
            pump([&] { return native_received>before; },"independent native witness did not receive valid Image barrier");
            ipc::call_result delivered;
            pump([&] { delivered=events->poll();return delivered.error!=11; },"valid native Image barrier was not delivered");
            require(!delivered.error,"valid native Image queue returned an error");ipc::image decoded;
            require(ipc::decode_image(delivered.payload,2,decoded),"actual Image canonical result shape");
            require(decoded.bigendian==(image.is_bigendian==1)&&decoded.data==image.data&&decoded.frame_id==image.header.frame_id&&decoded.height==1&&decoded.width==1&&decoded.step==1&&decoded.seconds==123&&decoded.nanoseconds==456,
                "malformed native Image endian was normalized or valid barrier changed");
            auto drain_until{std::chrono::steady_clock::now()+std::chrono::milliseconds(200)};
            while(std::chrono::steady_clock::now()<drain_until)
            { executor.spin_some();require(events->poll().error==11,"malformed native Image remained queued behind valid barrier");std::this_thread::sleep_for(std::chrono::milliseconds(5)); }
        }
        require(invalid_native==2&&native_received==4,"actual native malformed/valid Image witness counts");
        events->deactivate();adapter.close("image");executor.spin_some();rclcpp::shutdown();
        std::cout<<"PASS actual ROS2 Image native uint8 endian2/255 rejected before canonical queue; native witness4/invalid2, valid0/1 exact stamp/shape/data barriers delivered\n";
        return 0;
    }
    catch(std::exception const& error) { std::cerr<<error.what()<<'\n';rclcpp::shutdown();return 1; }
}
