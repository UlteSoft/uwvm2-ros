/** Link the two byte-exact upstream translation units with only their main
 * symbols renamed for this ONE host model executable. Production recipes use
 * their ordinary unmodified main and the real import backend. This model proves
 * wrapper/API/complete canonical behavior, never native ROS or hardware.
 */
#include <platform/ros2/cpp/compat/rclcpp/rclcpp/rclcpp.hpp>
#include <cassert>
int official_pub_main(int,char**);int official_sub_main(int,char**);
unsigned checks{};
void require(bool v){++checks;if(!v)std::abort();}
template<class Call>void refused(Call&& call,std::uint32_t expected){bool hit{};try{call();}catch(rclcpp::exceptions::RCLError const&e){hit=e.code==expected;}rclcpp::shutdown();require(hit);require(bridge::empty());}
int main(){char arg[]="official";char* argv[]{arg,nullptr};
 bridge::reset();require(official_pub_main(1,argv)==0);require(bridge::publishes==1&&bridge::logs==1);require(bridge::empty());
 std::string value;require(::uwvm_ros::decode_string(std::span<unsigned char const>{bridge::bytes.data(),bridge::bytes.size()},value)&&value=="Hello, world! 0");
 require(bridge::output.size()==1&&bridge::output[0]=="Publishing: 'Hello, world! 0'");
 bridge::reset();require(official_sub_main(1,argv)==0);require(bridge::polls==1&&bridge::logs==1&&bridge::times==0);require(bridge::output.size()==1&&bridge::output[0]=="I heard: 'Hello, world! 0'");require(bridge::empty());
 bridge::reset();bridge::deny=true;refused([&]{official_pub_main(1,argv);},13);require(bridge::publishes==0&&bridge::logs==0);
 bridge::reset();bridge::topics[0].keep_last_depth=11;refused([&]{official_pub_main(1,argv);},95);require(bridge::opens==0);
 bridge::reset();bridge::clock_fail=true;refused([&]{official_pub_main(1,argv);},110);require(bridge::publishes==0&&bridge::closes==2);
 bridge::reset();bridge::log_deny=true;refused([&]{official_pub_main(1,argv);},13);require(bridge::publishes==0);require(roswasm_rclcpp_cleanup_error()==0);
 bridge::reset();bridge::publish_fail=true;refused([&]{official_pub_main(1,argv);},110);require(bridge::publishes==1);
 bridge::reset();bridge::malformed=true;refused([&]{official_sub_main(1,argv);},22);require(bridge::logs==0);
 bridge::reset();rclcpp::init(1,argv);auto node=std::make_shared<rclcpp::Node>("minimal_publisher");
 auto pub=node->create_publisher<std_msgs::msg::String>("topic",10);node.reset();refused([&]{pub->publish(std_msgs::msg::String{});},9);pub.reset();
 bridge::reset();rclcpp::init(1,argv);unsigned fired{};rclcpp::TimerBase timer{500,0,[&]{++fired;}};
 require(timer.poll(1250)==0&&fired==1);require(timer.poll(1499)==11&&fired==1);require(timer.poll(1500)==0&&fired==2);timer.cancel();require(timer.poll(2000)==11);rclcpp::shutdown();
 return roswasm_rclcpp_cleanup_error()?1:0;
}
