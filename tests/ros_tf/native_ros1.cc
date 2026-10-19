#include "common.h"
#include <ros/ros.h>
#include <tf2_msgs/TFMessage.h>
using namespace ros_probe;
int main(int argc,char** argv)
{
    try
    {
        if(argc==3&&(std::string{argv[1]}=="--publish"||std::string{argv[1]}=="--observe"||std::string{argv[1]}=="--absent"||std::string{argv[1]}=="--absent-lookup"))
        {
            std::string mode{argv[1]},resource{argv[2]};
            ros::init(argc,argv,"uwvm_ros1_tf_witness",ros::init_options::NoSigintHandler|ros::init_options::NoRosout);
            ros::NodeHandle node;ros::AsyncSpinner spinner{1};spinner.start();
            tf2_msgs::TFMessage sample;geometry_msgs::TransformStamped m;
            m.header.stamp=ros::Time{123,456};m.header.frame_id="map";m.child_frame_id="base";
            m.transform.translation.x=1.25;m.transform.translation.y=2.5;m.transform.translation.z=3.75;m.transform.rotation.w=1.;sample.transforms.push_back(m);
            std::atomic<bool> received{},wrong{};
            auto subscriber{(mode!="--publish"&&mode!="--absent-lookup")?node.subscribe<tf2_msgs::TFMessage>(resource,8,[&](tf2_msgs::TFMessage::ConstPtr const& batch)
            {for(auto const& t:batch->transforms){if(t.header.frame_id=="map"&&t.child_frame_id=="base"&&t.header.stamp.sec==123&&t.header.stamp.nsec==456&&t.transform.translation.x==1.25&&t.transform.translation.y==2.5&&t.transform.translation.z==3.75&&t.transform.rotation.x==0&&t.transform.rotation.y==0&&t.transform.rotation.z==0&&t.transform.rotation.w==1){received=true;}else{wrong=true;}}}):ros::Subscriber{}};
            auto publisher{(mode=="--publish"||mode=="--absent-lookup")?node.advertise<tf2_msgs::TFMessage>(resource,8,false):ros::Publisher{}};
            std::cout<<"READY\n"<<std::flush;
            auto end{std::chrono::steady_clock::now()+std::chrono::seconds((mode=="--absent"||mode=="--absent-lookup")?5:12)};
            while(std::chrono::steady_clock::now()<end&&!received&&!wrong){if(publisher){publisher.publish(sample);if(mode=="--absent-lookup"&&publisher.getNumSubscribers()){wrong=true;}}std::this_thread::sleep_for(std::chrono::milliseconds(20));}
            if(mode=="--observe"){require(received&&!wrong,"Wasm TF native exact frame/value/stamp witness");}
            if(mode=="--absent"){require(!received&&!wrong,"denied Wasm TF caused no native publication");}
            if(mode=="--absent-lookup"){require(!wrong,"denied Wasm TF lookup constructed no native TF subscriber/cache");}
            spinner.stop();ros::shutdown();std::cout<<"PASS actual ROS1 TF "<<mode<<'\n';return 0;
        }
        require(argc==2,"usage: TF probe gateway-executable");
        ros::init(argc,argv,"uwvm_ros1_tf_witness",ros::init_options::NoSigintHandler|ros::init_options::NoRosout);
        ros::NodeHandle node; ros::AsyncSpinner spinner{2}; spinner.start();
        fixture f{core::version::ros1,argv[1],ros_tf_probe::configure}; ipc::client client; f.connect(client);
        auto publisher{node.advertise<tf2_msgs::TFMessage>("/tf",8,false)};
        tf2_msgs::TFMessage batch;
        geometry_msgs::TransformStamped m; m.header.stamp=ros::Time{123,456}; m.header.frame_id="map"; m.child_frame_id="base";
        m.transform.translation.x=1.25; m.transform.translation.y=2.5; m.transform.translation.z=3.75; m.transform.rotation.w=1.;
        batch.transforms.push_back(m); m.header.frame_id="secret"; m.child_frame_id="private"; batch.transforms.push_back(m);
        std::thread producer{[&] { for(unsigned n{};n!=150;++n) { publisher.publish(batch); std::this_thread::sleep_for(std::chrono::milliseconds(20)); } }};
        auto lookup{client.call("tf.lookup","lookup",std::vector<std::uint8_t>(8),1500)}; producer.join(); ros_tf_probe::assert_lookup(lookup);
        require(client.call("tf.lookup","missing",std::vector<std::uint8_t>(8),1000).error==13,"unknown TF capability refused");
        require(client.call("tf.lookup","lookup",std::vector<std::uint8_t>(9),1000).error==90,"Guest cannot add requested unauthorized frames beyond canonical request size");
        std::atomic<unsigned> received{};
        std::atomic<unsigned> private_received{};
        auto subscriber{node.subscribe<tf2_msgs::TFMessage>("/tf",8,[&](tf2_msgs::TFMessage::ConstPtr const& messages)
        { for(auto const& t:messages->transforms) { if(t.header.frame_id=="map"&&t.child_frame_id=="base"&&t.transform.translation.x==1.25) { ++received; } if(t.header.frame_id=="secret"&&t.child_frame_id=="private") { ++private_received; } } })};
        auto data{ipc::encode_transform(ros_tf_probe::transform())};
        for(unsigned n{};n!=150&&!received.load();++n) { require(!client.call("tf.broadcast","broadcast",data,1000).error,"authorized TF broadcast");std::this_thread::sleep_for(std::chrono::milliseconds(20)); }
        require(received.load()>0,"native ROS1 received authorized TF"); std::this_thread::sleep_for(std::chrono::milliseconds(100)); auto before{private_received.load()};
        auto denied{ros_tf_probe::transform()}; denied.parent="secret"; denied.child="private";
        require(client.call("tf.broadcast","broadcast",ipc::encode_transform(denied),1000).error==13,"TF frame substitution refused before publish");
        std::this_thread::sleep_for(std::chrono::milliseconds(50));require(private_received.load()==before,"denied transform caused no native publication");
        denied=ros_tf_probe::transform();denied.values[0]=100.;require(client.call("tf.broadcast","broadcast",ipc::encode_transform(denied),1000).error==13,"explicit TF translation bound enforced");
        require(!client.call("capability.revoke","lookup",{},1000).error&&client.call("tf.lookup","lookup",std::vector<std::uint8_t>(8),1000).error==125,"TF revoke removes listener/cache");
        ros_tf_probe::assert_capability_deadline(core::version::ros1,argv[1]);
        std::cout<<"PASS ROS1 actual TF broadcast/lookup, mixed-message frame filtering, unauthorized substitution/no network effect, revoke\n";
        ros::shutdown(); return 0;
    }
    catch(std::exception const& e) { std::cerr<<e.what()<<'\n';return 1; }
}
