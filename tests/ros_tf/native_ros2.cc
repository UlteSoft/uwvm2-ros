#include "common.h"
#include <rclcpp/rclcpp.hpp>
#include <tf2_msgs/msg/tf_message.hpp>
using namespace ros_probe;
int main(int argc,char** argv)
{
    try
    {
        if(argc==3&&(std::string{argv[1]}=="--publish"||std::string{argv[1]}=="--observe"||std::string{argv[1]}=="--absent"||std::string{argv[1]}=="--absent-lookup"))
        {
            std::string mode{argv[1]},resource{argv[2]};rclcpp::init(argc,argv);
            auto node{std::make_shared<rclcpp::Node>("uwvm_ros2_tf_witness")};
            tf2_msgs::msg::TFMessage sample;geometry_msgs::msg::TransformStamped m;
            m.header.stamp.sec=123;m.header.stamp.nanosec=456;m.header.frame_id="map";m.child_frame_id="base";
            m.transform.translation.x=1.25;m.transform.translation.y=2.5;m.transform.translation.z=3.75;m.transform.rotation.w=1.;sample.transforms.push_back(m);
            bool received{},wrong{};rclcpp::Subscription<tf2_msgs::msg::TFMessage>::SharedPtr subscriber;
            if(mode!="--publish"&&mode!="--absent-lookup"){subscriber=node->create_subscription<tf2_msgs::msg::TFMessage>(resource,rclcpp::QoS{8}.reliable(),[&](tf2_msgs::msg::TFMessage::ConstSharedPtr batch)
            {for(auto const& t:batch->transforms){if(t.header.frame_id=="map"&&t.child_frame_id=="base"&&t.header.stamp.sec==123&&t.header.stamp.nanosec==456&&t.transform.translation.x==1.25&&t.transform.translation.y==2.5&&t.transform.translation.z==3.75&&t.transform.rotation.x==0&&t.transform.rotation.y==0&&t.transform.rotation.z==0&&t.transform.rotation.w==1){received=true;}else{wrong=true;}}});}
            rclcpp::Publisher<tf2_msgs::msg::TFMessage>::SharedPtr publisher;
            if(mode=="--publish"||mode=="--absent-lookup"){publisher=node->create_publisher<tf2_msgs::msg::TFMessage>(resource,rclcpp::QoS{8}.reliable());}
            std::cout<<"READY\n"<<std::flush;
            auto end{std::chrono::steady_clock::now()+std::chrono::seconds((mode=="--absent"||mode=="--absent-lookup")?5:12)};
            while(std::chrono::steady_clock::now()<end&&!received&&!wrong){if(publisher){publisher->publish(sample);if(mode=="--absent-lookup"&&publisher->get_subscription_count()){wrong=true;}}rclcpp::spin_some(node);std::this_thread::sleep_for(std::chrono::milliseconds(20));}
            if(mode=="--observe"){require(received&&!wrong,"Wasm TF native exact frame/value/stamp witness");}
            if(mode=="--absent"){require(!received&&!wrong,"denied Wasm TF caused no native publication");}
            if(mode=="--absent-lookup"){require(!wrong,"denied Wasm TF lookup constructed no native TF subscriber/cache");}
            rclcpp::shutdown();std::cout<<"PASS actual ROS2 TF "<<mode<<'\n';return 0;
        }
        require(argc==2,"usage: TF probe gateway-executable");
        ::setenv("ROS_DOMAIN_ID","72",1); ::setenv("RMW_IMPLEMENTATION","rmw_fastrtps_cpp",1);
        rclcpp::init(argc,argv); auto node{std::make_shared<rclcpp::Node>("uwvm_ros2_tf_witness")};
        rclcpp::executors::MultiThreadedExecutor executor; executor.add_node(node); std::thread spin{[&] { executor.spin(); }};
        // Ensure exception paths join the actual executor thread.
        struct joiner { rclcpp::Executor& executor;std::thread& thread;~joiner(){executor.cancel();thread.join();rclcpp::shutdown();} } cleanup{executor,spin};
        fixture f{core::version::ros2,argv[1],ros_tf_probe::configure}; ipc::client client; f.connect(client);
        auto publisher{node->create_publisher<tf2_msgs::msg::TFMessage>("/tf",rclcpp::QoS{8}.reliable())};
        tf2_msgs::msg::TFMessage batch;
        geometry_msgs::msg::TransformStamped m; m.header.stamp.sec=123;m.header.stamp.nanosec=456;m.header.frame_id="map";m.child_frame_id="base";
        m.transform.translation.x=1.25;m.transform.translation.y=2.5;m.transform.translation.z=3.75;m.transform.rotation.w=1.;
        batch.transforms.push_back(m);m.header.frame_id="secret";m.child_frame_id="private";batch.transforms.push_back(m);
        std::thread producer{[&]{for(unsigned n{};n!=150;++n){publisher->publish(batch);std::this_thread::sleep_for(std::chrono::milliseconds(20));}}};
        auto lookup{client.call("tf.lookup","lookup",std::vector<std::uint8_t>(8),1500)};producer.join();ros_tf_probe::assert_lookup(lookup);
        require(client.call("tf.lookup","missing",std::vector<std::uint8_t>(8),1000).error==13,"unknown TF capability refused");
        require(client.call("tf.lookup","lookup",std::vector<std::uint8_t>(9),1000).error==90,"Guest cannot add requested unauthorized frames beyond canonical request size");
        std::atomic<unsigned> received{};
        std::atomic<unsigned> private_received{};
        auto subscriber{node->create_subscription<tf2_msgs::msg::TFMessage>("/tf",rclcpp::QoS{8}.reliable(),[&](tf2_msgs::msg::TFMessage::ConstSharedPtr messages)
        {for(auto const& t:messages->transforms){if(t.header.frame_id=="map"&&t.child_frame_id=="base"&&t.transform.translation.x==1.25){++received;}if(t.header.frame_id=="secret"&&t.child_frame_id=="private"){++private_received;}}})};
        auto data{ipc::encode_transform(ros_tf_probe::transform())};
        for(unsigned n{};n!=150&&!received.load();++n){require(!client.call("tf.broadcast","broadcast",data,1000).error,"authorized TF broadcast");std::this_thread::sleep_for(std::chrono::milliseconds(20));}
        require(received.load()>0,"native ROS2 received authorized TF");std::this_thread::sleep_for(std::chrono::milliseconds(100));auto before{private_received.load()};
        auto denied{ros_tf_probe::transform()};denied.parent="secret";denied.child="private";
        require(client.call("tf.broadcast","broadcast",ipc::encode_transform(denied),1000).error==13,"TF frame substitution refused before publish");
        std::this_thread::sleep_for(std::chrono::milliseconds(50));require(private_received.load()==before,"denied transform caused no native publication");
        denied=ros_tf_probe::transform();denied.values[0]=100.;require(client.call("tf.broadcast","broadcast",ipc::encode_transform(denied),1000).error==13,"explicit TF translation bound enforced");
        require(!client.call("capability.revoke","lookup",{},1000).error&&client.call("tf.lookup","lookup",std::vector<std::uint8_t>(8),1000).error==125,"TF revoke removes listener/cache");
        ros_tf_probe::assert_capability_deadline(core::version::ros2,argv[1]);
        std::cout<<"PASS ROS2 actual TF broadcast/lookup, mixed-message frame filtering, unauthorized substitution/no network effect, revoke\n";return 0;
    }
    catch(std::exception const& e){std::cerr<<e.what()<<'\n';return 1;}
}
