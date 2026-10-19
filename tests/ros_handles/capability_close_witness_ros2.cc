#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <fast_io.h>
#include <chrono>
#include <set>
#include <string>
#include <string_view>
#include <thread>

int main(int argc,char** argv)
{
    if(argc!=3)return 2;
    rclcpp::init(argc,argv);
    auto node=std::make_shared<rclcpp::Node>("uwvm_next_alias_witness");
    auto publisher=node->create_publisher<std_msgs::msg::String>(argv[1],rclcpp::QoS{4}.reliable());
    std::set<std::string> observed;bool initial{};
    auto end=std::chrono::steady_clock::now()+std::chrono::seconds(40);
    while(rclcpp::ok()&&std::chrono::steady_clock::now()<end)
    {
        rclcpp::spin_some(node);
        fast_io::native_file_loader input{fast_io::mnp::os_c_str(argv[2])};
        std::string phase{input.data(),input.size()};
        while(!phase.empty()&&(phase.back()=='\n'||phase.back()=='\r'))phase.pop_back();
        std_msgs::msg::String message;message.data=phase=="survived"?"after":phase=="reopened"?"again":"hello";
        publisher->publish(message);
        unsigned count{};
        for(auto const& endpoint:node->get_subscriptions_info_by_topic(argv[1]))
            if(endpoint.node_name()=="uwvm_ros2_next_alias")++count;
        if(count==1&&!initial){initial=true;fast_io::io::println("NATIVE_INITIAL actual_count=1");}
        bool match=(phase=="closed"||phase=="final"||phase=="reopenedfinal")?count==0:
                   (phase=="survived"||phase=="reopened")?count==1:false;
        if(initial&&match&&observed.insert(phase).second)
            fast_io::io::println("NATIVE_PHASE ",phase," actual_count=",count," published=",message.data);
        if(phase=="finish")
        {
            bool old_failure=observed.count("closed");
            bool fixed=observed.count("survived")&&observed.count("final")&&observed.count("reopened")&&observed.count("reopenedfinal");
            fast_io::io::println(std::string_view{initial&&(old_failure||fixed)?"PASS":"FAIL",4}," actual native sibling-close phases");
            rclcpp::shutdown();return initial&&(old_failure||fixed)?0:3;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    fast_io::io::println("FAIL native sibling-close phase deadline");rclcpp::shutdown();return 110;
}
