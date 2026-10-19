/** Industrial native metadata source; no Node, endpoint or grant. */
#include "tests/ros_custom_types/bounded-v2/native_protocol.h"
#include <array>
#include <string_view>
#include <stdexcept>
#include <csignal>
#include <cstdint>
#if UWVM_CUSTOM_NATIVE_ROS_VERSION != 2
#error "Select the exact source-reviewed ROS major"
#endif
#include <builtin_interfaces/msg/time.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/pose_with_covariance.hpp>
#include <geometry_msgs/msg/quaternion.hpp>
#include <geometry_msgs/msg/transform.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <geometry_msgs/msg/twist_with_covariance.hpp>
#include <geometry_msgs/msg/vector3.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <sensor_msgs/msg/battery_state.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <sensor_msgs/msg/magnetic_field.hpp>
#include <std_msgs/msg/header.hpp>
#include <std_srvs/srv/set_bool.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <turtlebot3_msgs/action/patrol.hpp>
#include <turtlebot3_msgs/msg/sensor_state.hpp>
#include <turtlebot3_msgs/msg/sound.hpp>
#include <turtlebot3_msgs/msg/version_info.hpp>
#include <turtlebot3_msgs/srv/dqn.hpp>
#include <turtlebot3_msgs/srv/goal.hpp>
#include <turtlebot3_msgs/srv/sound.hpp>
#include <rosidl_typesupport_cpp/message_type_support.hpp>
#include <rosidl_typesupport_cpp/service_type_support.hpp>
#include <rosidl_typesupport_cpp/action_type_support.hpp>
#include "native_constants.hpp"
namespace
{
    int output() {return STDOUT_FILENO;}
    /** Emit bounded JSON text with checked Unicode metadata supplied by ROS.
     * Escape ASCII controls and quotes; no arbitrary filename or code is loaded. */
    void quoted(std::string_view value)
    {
        if(value.size()>1048576) {throw std::length_error("native metadata ceiling");}
        size_t length{2};for(unsigned char c:value) {length+=c<32?6:c=='"'||c=='\\'?2:1;}std::string escaped;escaped.reserve(length);escaped.push_back('"');constexpr char digits[]{"0123456789abcdef"};
        for(unsigned char c:value)
        {
            if(c=='"'||c=='\\') {escaped.push_back('\\');escaped.push_back(static_cast<char>(c));}
            else if(c<32) {for(char prefix:std::string_view{"\\u00"}) {escaped.push_back(prefix);}escaped.push_back(digits[c>>4]);escaped.push_back(digits[c&15]);}
            else {escaped.push_back(static_cast<char>(c));}
        }
        escaped.push_back('"');bounded_native_io::emit(output(),escaped);
    }
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    template<class Message> void component(char const* role,bool comma)
    {
        if(comma) {bounded_native_io::print(output(),',');}quoted(role);bounded_native_io::print(output(),":{");
        bounded_native_io::print(output(),"\"type\":");quoted(::ros::message_traits::DataType<Message>::value());
        bounded_native_io::print(output(),",\"native_identity\":");quoted(::ros::message_traits::MD5Sum<Message>::value());
        bounded_native_io::print(output(),",\"definition\":");quoted(::ros::message_traits::Definition<Message>::value());bounded_native_io::print(output(),'}');
    }
    template<class Message> void message(char const* name,bool comma)
    {if(comma) {bounded_native_io::print(output(),',');}quoted(name);bounded_native_io::print(output(),":{\"native_identity\":");quoted(::ros::message_traits::MD5Sum<Message>::value());bounded_native_io::print(output(),",\"components\":{");component<Message>("message",false);bounded_native_io::print(output(),"}}");}
    template<class Service> void service(char const* name)
    {bounded_native_io::print(output(),',');quoted(name);bounded_native_io::print(output(),":{\"native_identity\":");quoted(::ros::service_traits::MD5Sum<Service>::value());bounded_native_io::print(output(),",\"official_type\":");quoted(::ros::service_traits::DataType<Service>::value());bounded_native_io::print(output(),",\"components\":{");component<typename Service::Request>("request",false);component<typename Service::Response>("response",true);bounded_native_io::print(output(),"}}");}
#else
    /** Official static borrowed description strings may report capacity equal
     * to size, excluding the separately emitted NUL. Check that actual NUL,
     * pointer and finite length; this is not the Guest string admission rule. */
    template<class Text> std::string_view text(Text const& value)
    {if(value.size>1048576||value.size>value.capacity||!value.data||value.data[value.size]!=0) {throw std::runtime_error("invalid official metadata text");}return {value.data,value.size};}
    void record(rosidl_runtime_c__type_description__IndividualTypeDescription const& value)
    {
        if(value.fields.size>4096||value.fields.size>value.fields.capacity||(value.fields.size&&!value.fields.data)) {throw std::runtime_error("native field table ceiling");}
        bounded_native_io::print(output(),"{\"type\":");quoted(text(value.type_name));bounded_native_io::print(output(),",\"fields\":[");
        for(size_t i{};i!=value.fields.size;++i)
        {auto const& f{value.fields.data[i]};if(i) {bounded_native_io::print(output(),',');}bounded_native_io::print(output(),"{\"name\":");quoted(text(f.name));bounded_native_io::print(output(),",\"type_id\":",static_cast<unsigned>(f.type.type_id),",\"capacity\":",f.type.capacity,",\"string_capacity\":",f.type.string_capacity,",\"nested_type_name\":");
            if(!f.type.nested_type_name.data&&!f.type.nested_type_name.size&&!f.type.nested_type_name.capacity) {quoted("");}else {quoted(text(f.type.nested_type_name));}
            bounded_native_io::print(output(),",\"default_value\":");if(!f.default_value.data&&!f.default_value.size&&!f.default_value.capacity) {quoted("");}else {quoted(text(f.default_value));}bounded_native_io::print(output(),'}');}
        bounded_native_io::print(output(),"]}");
    }
    template<class Support> void metadata(Support const* support)
    {
        if(!support||!support->get_type_hash_func||!support->get_type_description_func) {throw std::runtime_error("official metadata unavailable");}
        auto hash{support->get_type_hash_func(support)};auto description{support->get_type_description_func(support)};
        if(!hash||hash->version!=1||!description||description->referenced_type_descriptions.size>512||description->referenced_type_descriptions.size>description->referenced_type_descriptions.capacity||(description->referenced_type_descriptions.size&&!description->referenced_type_descriptions.data)) {throw std::runtime_error("official type description ceiling");}
        std::array<char,71> identity{};constexpr char hex[]{"0123456789abcdef"};constexpr char prefix[]{"RIHS01_"};for(unsigned i{};i!=7;++i) {identity[i]=prefix[i];}for(unsigned i{};i!=32;++i) {identity[7+2*i]=hex[hash->value[i]>>4];identity[8+2*i]=hex[hash->value[i]&15];}
        bounded_native_io::print(output(),"{\"native_identity\":");quoted({identity.data(),identity.size()});bounded_native_io::print(output(),",\"root\":");record(description->type_description);bounded_native_io::print(output(),",\"references\":[");for(size_t i{};i!=description->referenced_type_descriptions.size;++i) {if(i) {bounded_native_io::print(output(),',');}record(description->referenced_type_descriptions.data[i]);}bounded_native_io::print(output(),"]}");
    }
    template<class Message> void component(char const* name,bool comma)
    {if(comma) {bounded_native_io::print(output(),',');}quoted(name);bounded_native_io::print(output(),':');metadata(rosidl_typesupport_cpp::get_message_type_support_handle<Message>());}
    template<class Message> void message(char const* name,bool comma)
    {if(comma) {bounded_native_io::print(output(),',');}quoted(name);bounded_native_io::print(output(),":{\"official\":");metadata(rosidl_typesupport_cpp::get_message_type_support_handle<Message>());bounded_native_io::print(output(),",\"components\":{");component<Message>("message",false);bounded_native_io::print(output(),"}}");}
    template<class Service> void service(char const* name)
    {bounded_native_io::print(output(),',');quoted(name);bounded_native_io::print(output(),":{\"official\":");metadata(rosidl_typesupport_cpp::get_service_type_support_handle<Service>());bounded_native_io::print(output(),",\"components\":{");component<typename Service::Request>("request",false);component<typename Service::Response>("response",true);bounded_native_io::print(output(),"}}");}
#endif
}
namespace {template<class S> void industrial_warm(S const* s){if(!s||!s->get_type_hash_func||!s->get_type_description_func)throw std::runtime_error{"modern native metadata unavailable"};auto h=s->get_type_hash_func(s);auto d=s->get_type_description_func(s);if(!h||h->version!=1||!d)throw std::runtime_error{"native metadata warm failed"};}}

int main() {try {
if(!::uwvm_industrial_source_067124cc21aad5416a36a35360ca60f375645f164248d33a85729c018abc5c17::validate())throw std::runtime_error{"native source constants differ"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Point>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Point>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Pose>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Pose>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::PoseWithCovariance>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::PoseWithCovariance>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Quaternion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Quaternion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Transform>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Transform>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::TransformStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::TransformStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Twist>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Twist>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::TwistStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::TwistStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::TwistWithCovariance>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::TwistWithCovariance>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::nav_msgs::msg::Odometry>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::nav_msgs::msg::Odometry>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::BatteryState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::BatteryState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::Imu>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::Imu>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::JointState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::JointState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::LaserScan>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::LaserScan>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::MagneticField>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::MagneticField>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::std_srvs::srv::SetBool>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_srvs::srv::SetBool::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_srvs::srv::SetBool::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::std_srvs::srv::Trigger>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_srvs::srv::Trigger::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_srvs::srv::Trigger::Response>());
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::turtlebot3_msgs::action::Patrol>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::action::Patrol::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::action::Patrol::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::action::Patrol::Feedback>());
auto a20=rosidl_typesupport_cpp::get_action_type_support_handle<::turtlebot3_msgs::action::Patrol>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::turtlebot3_msgs::action::Patrol::Impl::SendGoalService>());if(a20->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::turtlebot3_msgs::action::Patrol::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::turtlebot3_msgs::action::Patrol::Impl::GetResultService>());if(a20->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::turtlebot3_msgs::action::Patrol::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::turtlebot3_msgs::action::Patrol::Impl::CancelGoalService>());if(a20->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::turtlebot3_msgs::action::Patrol::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::turtlebot3_msgs::action::Patrol::Impl::FeedbackMessage>());if(a20->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::turtlebot3_msgs::action::Patrol::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::turtlebot3_msgs::action::Patrol::Impl::GoalStatusMessage>());if(a20->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::turtlebot3_msgs::action::Patrol::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::msg::SensorState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::msg::SensorState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::msg::Sound>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::msg::Sound>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::msg::VersionInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::msg::VersionInfo>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::turtlebot3_msgs::srv::Dqn>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::srv::Dqn::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::srv::Dqn::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::turtlebot3_msgs::srv::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::srv::Goal::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::srv::Goal::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::turtlebot3_msgs::srv::Sound>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::srv::Sound::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::srv::Sound::Response>());
if(::raise(SIGSTOP)!=0)throw std::runtime_error{"owned mapping stop failed"};
bounded_native_io::gate('I');
bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-industrial-native-v1\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"source_profile\":\"uwvm-ros-vendor-complete-bounded-v1\",\"constant_schema_sha256\":\"067124cc21aad5416a36a35360ca60f375645f164248d33a85729c018abc5c17\",\"types\":{");
quoted("builtin_interfaces/msg/Time");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
bounded_native_io::print(output(),",\"components\":{");
component<::builtin_interfaces::msg::Time>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Point");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Point>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Point>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Pose");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Pose>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Pose>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/PoseWithCovariance");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::PoseWithCovariance>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::PoseWithCovariance>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Quaternion");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Quaternion>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Quaternion>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Transform");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Transform>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Transform>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/TransformStamped");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::TransformStamped>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::TransformStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Twist");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Twist>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Twist>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/TwistStamped");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::TwistStamped>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::TwistStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/TwistWithCovariance");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::TwistWithCovariance>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::TwistWithCovariance>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Vector3");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Vector3>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nav_msgs/msg/Odometry");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::nav_msgs::msg::Odometry>());
bounded_native_io::print(output(),",\"components\":{");
component<::nav_msgs::msg::Odometry>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("sensor_msgs/msg/BatteryState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::BatteryState>());
bounded_native_io::print(output(),",\"components\":{");
component<::sensor_msgs::msg::BatteryState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("sensor_msgs/msg/Imu");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::Imu>());
bounded_native_io::print(output(),",\"components\":{");
component<::sensor_msgs::msg::Imu>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("sensor_msgs/msg/JointState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::JointState>());
bounded_native_io::print(output(),",\"components\":{");
component<::sensor_msgs::msg::JointState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("sensor_msgs/msg/LaserScan");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::LaserScan>());
bounded_native_io::print(output(),",\"components\":{");
component<::sensor_msgs::msg::LaserScan>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("sensor_msgs/msg/MagneticField");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::MagneticField>());
bounded_native_io::print(output(),",\"components\":{");
component<::sensor_msgs::msg::MagneticField>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/msg/Header");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::msg::Header>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_srvs/srv/SetBool");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::std_srvs::srv::SetBool>());
bounded_native_io::print(output(),",\"components\":{");
component<::std_srvs::srv::SetBool::Request>("request",false);
component<::std_srvs::srv::SetBool::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_srvs/srv/Trigger");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::std_srvs::srv::Trigger>());
bounded_native_io::print(output(),",\"components\":{");
component<::std_srvs::srv::Trigger::Request>("request",false);
component<::std_srvs::srv::Trigger::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("turtlebot3_msgs/action/Patrol");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::turtlebot3_msgs::action::Patrol>());
bounded_native_io::print(output(),",\"components\":{");
component<::turtlebot3_msgs::action::Patrol::Goal>("goal",false);
component<::turtlebot3_msgs::action::Patrol::Result>("result",true);
component<::turtlebot3_msgs::action::Patrol::Feedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("turtlebot3_msgs/msg/SensorState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::msg::SensorState>());
bounded_native_io::print(output(),",\"components\":{");
component<::turtlebot3_msgs::msg::SensorState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("turtlebot3_msgs/msg/Sound");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::msg::Sound>());
bounded_native_io::print(output(),",\"components\":{");
component<::turtlebot3_msgs::msg::Sound>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("turtlebot3_msgs/msg/VersionInfo");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::turtlebot3_msgs::msg::VersionInfo>());
bounded_native_io::print(output(),",\"components\":{");
component<::turtlebot3_msgs::msg::VersionInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("turtlebot3_msgs/srv/Dqn");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::turtlebot3_msgs::srv::Dqn>());
bounded_native_io::print(output(),",\"components\":{");
component<::turtlebot3_msgs::srv::Dqn::Request>("request",false);
component<::turtlebot3_msgs::srv::Dqn::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("turtlebot3_msgs/srv/Goal");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::turtlebot3_msgs::srv::Goal>());
bounded_native_io::print(output(),",\"components\":{");
component<::turtlebot3_msgs::srv::Goal::Request>("request",false);
component<::turtlebot3_msgs::srv::Goal::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("turtlebot3_msgs/srv/Sound");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::turtlebot3_msgs::srv::Sound>());
bounded_native_io::print(output(),",\"components\":{");
component<::turtlebot3_msgs::srv::Sound::Request>("request",false);
component<::turtlebot3_msgs::srv::Sound::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),"}}\n");return 0;
}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native industrial metadata refused\n");return 1;}}
