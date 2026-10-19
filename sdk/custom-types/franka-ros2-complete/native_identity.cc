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
#include <franka_msgs/action/error_recovery.hpp>
#include <franka_msgs/action/grasp.hpp>
#include <franka_msgs/action/homing.hpp>
#include <franka_msgs/action/move.hpp>
#include <franka_msgs/action/ptp_motion.hpp>
#include <franka_msgs/msg/collision_indicators.hpp>
#include <franka_msgs/msg/elbow.hpp>
#include <franka_msgs/msg/errors.hpp>
#include <franka_msgs/msg/franka_robot_state.hpp>
#include <franka_msgs/msg/grasp_epsilon.hpp>
#include <franka_msgs/msg/target_status.hpp>
#include <franka_msgs/srv/set_cartesian_stiffness.hpp>
#include <franka_msgs/srv/set_force_torque_collision_behavior.hpp>
#include <franka_msgs/srv/set_full_collision_behavior.hpp>
#include <franka_msgs/srv/set_joint_stiffness.hpp>
#include <franka_msgs/srv/set_load.hpp>
#include <franka_msgs/srv/set_stiffness_frame.hpp>
#include <franka_msgs/srv/set_tcp_frame.hpp>
#include <franka_spine_msgs/action/move_absolute.hpp>
#include <franka_spine_msgs/msg/spine_parameters.hpp>
#include <franka_spine_msgs/msg/spine_position.hpp>
#include <franka_spine_msgs/msg/spine_state.hpp>
#include <franka_spine_msgs/msg/user_limits.hpp>
#include <franka_spine_msgs/srv/fault_reset.hpp>
#include <franka_spine_msgs/srv/get_parameters.hpp>
#include <franka_spine_msgs/srv/get_position.hpp>
#include <franka_spine_msgs/srv/get_spine_state.hpp>
#include <franka_spine_msgs/srv/halt.hpp>
#include <franka_spine_msgs/srv/switch_off.hpp>
#include <franka_spine_msgs/srv/switch_on.hpp>
#include <geometry_msgs/msg/accel.hpp>
#include <geometry_msgs/msg/accel_stamped.hpp>
#include <geometry_msgs/msg/inertia.hpp>
#include <geometry_msgs/msg/inertia_stamped.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/quaternion.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <geometry_msgs/msg/vector3.hpp>
#include <geometry_msgs/msg/vector3_stamped.hpp>
#include <geometry_msgs/msg/wrench.hpp>
#include <geometry_msgs/msg/wrench_stamped.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <std_msgs/msg/header.hpp>
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
if(!::uwvm_industrial_source_02d2333c0389d69b82be45096c637e8d5a76d8ecf3f59e2d09854e5718d77cc0::validate())throw std::runtime_error{"native source constants differ"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::franka_msgs::action::ErrorRecovery>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::action::ErrorRecovery::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::action::ErrorRecovery::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::action::ErrorRecovery::Feedback>());
auto a1=rosidl_typesupport_cpp::get_action_type_support_handle<::franka_msgs::action::ErrorRecovery>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::ErrorRecovery::Impl::SendGoalService>());if(a1->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::ErrorRecovery::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::ErrorRecovery::Impl::GetResultService>());if(a1->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::ErrorRecovery::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::ErrorRecovery::Impl::CancelGoalService>());if(a1->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::ErrorRecovery::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::ErrorRecovery::Impl::FeedbackMessage>());if(a1->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::ErrorRecovery::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::ErrorRecovery::Impl::GoalStatusMessage>());if(a1->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::ErrorRecovery::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::franka_msgs::action::Grasp>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::action::Grasp::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::action::Grasp::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::action::Grasp::Feedback>());
auto a2=rosidl_typesupport_cpp::get_action_type_support_handle<::franka_msgs::action::Grasp>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Grasp::Impl::SendGoalService>());if(a2->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Grasp::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Grasp::Impl::GetResultService>());if(a2->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Grasp::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Grasp::Impl::CancelGoalService>());if(a2->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Grasp::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::Grasp::Impl::FeedbackMessage>());if(a2->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::Grasp::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::Grasp::Impl::GoalStatusMessage>());if(a2->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::Grasp::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::franka_msgs::action::Homing>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::action::Homing::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::action::Homing::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::action::Homing::Feedback>());
auto a3=rosidl_typesupport_cpp::get_action_type_support_handle<::franka_msgs::action::Homing>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Homing::Impl::SendGoalService>());if(a3->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Homing::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Homing::Impl::GetResultService>());if(a3->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Homing::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Homing::Impl::CancelGoalService>());if(a3->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Homing::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::Homing::Impl::FeedbackMessage>());if(a3->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::Homing::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::Homing::Impl::GoalStatusMessage>());if(a3->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::Homing::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::franka_msgs::action::Move>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::action::Move::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::action::Move::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::action::Move::Feedback>());
auto a4=rosidl_typesupport_cpp::get_action_type_support_handle<::franka_msgs::action::Move>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Move::Impl::SendGoalService>());if(a4->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Move::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Move::Impl::GetResultService>());if(a4->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Move::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Move::Impl::CancelGoalService>());if(a4->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::Move::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::Move::Impl::FeedbackMessage>());if(a4->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::Move::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::Move::Impl::GoalStatusMessage>());if(a4->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::Move::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::franka_msgs::action::PTPMotion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::action::PTPMotion::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::action::PTPMotion::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::action::PTPMotion::Feedback>());
auto a5=rosidl_typesupport_cpp::get_action_type_support_handle<::franka_msgs::action::PTPMotion>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::PTPMotion::Impl::SendGoalService>());if(a5->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::PTPMotion::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::PTPMotion::Impl::GetResultService>());if(a5->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::PTPMotion::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::PTPMotion::Impl::CancelGoalService>());if(a5->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_msgs::action::PTPMotion::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::PTPMotion::Impl::FeedbackMessage>());if(a5->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::PTPMotion::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::PTPMotion::Impl::GoalStatusMessage>());if(a5->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_msgs::action::PTPMotion::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::CollisionIndicators>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::CollisionIndicators>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::Elbow>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::Elbow>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::Errors>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::Errors>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::FrankaRobotState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::FrankaRobotState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::GraspEpsilon>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::GraspEpsilon>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::TargetStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::TargetStatus>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_msgs::srv::SetCartesianStiffness>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::srv::SetCartesianStiffness::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::srv::SetCartesianStiffness::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_msgs::srv::SetForceTorqueCollisionBehavior>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::srv::SetForceTorqueCollisionBehavior::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::srv::SetForceTorqueCollisionBehavior::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_msgs::srv::SetFullCollisionBehavior>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::srv::SetFullCollisionBehavior::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::srv::SetFullCollisionBehavior::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_msgs::srv::SetJointStiffness>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::srv::SetJointStiffness::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::srv::SetJointStiffness::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_msgs::srv::SetLoad>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::srv::SetLoad::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::srv::SetLoad::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_msgs::srv::SetStiffnessFrame>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::srv::SetStiffnessFrame::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::srv::SetStiffnessFrame::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_msgs::srv::SetTCPFrame>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::srv::SetTCPFrame::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::srv::SetTCPFrame::Response>());
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::franka_spine_msgs::action::MoveAbsolute>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::action::MoveAbsolute::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::action::MoveAbsolute::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::action::MoveAbsolute::Feedback>());
auto a19=rosidl_typesupport_cpp::get_action_type_support_handle<::franka_spine_msgs::action::MoveAbsolute>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_spine_msgs::action::MoveAbsolute::Impl::SendGoalService>());if(a19->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_spine_msgs::action::MoveAbsolute::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_spine_msgs::action::MoveAbsolute::Impl::GetResultService>());if(a19->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_spine_msgs::action::MoveAbsolute::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_spine_msgs::action::MoveAbsolute::Impl::CancelGoalService>());if(a19->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::franka_spine_msgs::action::MoveAbsolute::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_spine_msgs::action::MoveAbsolute::Impl::FeedbackMessage>());if(a19->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_spine_msgs::action::MoveAbsolute::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_spine_msgs::action::MoveAbsolute::Impl::GoalStatusMessage>());if(a19->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::franka_spine_msgs::action::MoveAbsolute::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::msg::SpineParameters>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::msg::SpineParameters>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::msg::SpinePosition>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::msg::SpinePosition>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::msg::SpineState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::msg::SpineState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::msg::UserLimits>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::msg::UserLimits>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_spine_msgs::srv::FaultReset>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::srv::FaultReset::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::srv::FaultReset::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_spine_msgs::srv::GetParameters>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::srv::GetParameters::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::srv::GetParameters::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_spine_msgs::srv::GetPosition>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::srv::GetPosition::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::srv::GetPosition::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_spine_msgs::srv::GetSpineState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::srv::GetSpineState::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::srv::GetSpineState::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_spine_msgs::srv::Halt>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::srv::Halt::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::srv::Halt::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_spine_msgs::srv::SwitchOff>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::srv::SwitchOff::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::srv::SwitchOff::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_spine_msgs::srv::SwitchOn>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::srv::SwitchOn::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::srv::SwitchOn::Response>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Accel>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Accel>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::AccelStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::AccelStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Inertia>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Inertia>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::InertiaStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::InertiaStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Point>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Point>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Pose>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Pose>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::PoseStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::PoseStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Quaternion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Quaternion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Twist>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Twist>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::TwistStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::TwistStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3Stamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3Stamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Wrench>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Wrench>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::WrenchStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::WrenchStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::JointState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::JointState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
if(::raise(SIGSTOP)!=0)throw std::runtime_error{"owned mapping stop failed"};
bounded_native_io::gate('I');
bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-industrial-native-v1\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"source_profile\":\"uwvm-ros-vendor-complete-bounded-v1\",\"constant_schema_sha256\":\"02d2333c0389d69b82be45096c637e8d5a76d8ecf3f59e2d09854e5718d77cc0\",\"types\":{");
quoted("builtin_interfaces/msg/Time");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
bounded_native_io::print(output(),",\"components\":{");
component<::builtin_interfaces::msg::Time>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/action/ErrorRecovery");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::franka_msgs::action::ErrorRecovery>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::action::ErrorRecovery::Goal>("goal",false);
component<::franka_msgs::action::ErrorRecovery::Result>("result",true);
component<::franka_msgs::action::ErrorRecovery::Feedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/action/Grasp");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::franka_msgs::action::Grasp>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::action::Grasp::Goal>("goal",false);
component<::franka_msgs::action::Grasp::Result>("result",true);
component<::franka_msgs::action::Grasp::Feedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/action/Homing");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::franka_msgs::action::Homing>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::action::Homing::Goal>("goal",false);
component<::franka_msgs::action::Homing::Result>("result",true);
component<::franka_msgs::action::Homing::Feedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/action/Move");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::franka_msgs::action::Move>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::action::Move::Goal>("goal",false);
component<::franka_msgs::action::Move::Result>("result",true);
component<::franka_msgs::action::Move::Feedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/action/PTPMotion");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::franka_msgs::action::PTPMotion>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::action::PTPMotion::Goal>("goal",false);
component<::franka_msgs::action::PTPMotion::Result>("result",true);
component<::franka_msgs::action::PTPMotion::Feedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/msg/CollisionIndicators");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::CollisionIndicators>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::msg::CollisionIndicators>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/msg/Elbow");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::Elbow>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::msg::Elbow>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/msg/Errors");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::Errors>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::msg::Errors>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/msg/FrankaRobotState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::FrankaRobotState>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::msg::FrankaRobotState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/msg/GraspEpsilon");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::GraspEpsilon>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::msg::GraspEpsilon>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/msg/TargetStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_msgs::msg::TargetStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::msg::TargetStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/srv/SetCartesianStiffness");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_msgs::srv::SetCartesianStiffness>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::srv::SetCartesianStiffness::Request>("request",false);
component<::franka_msgs::srv::SetCartesianStiffness::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/srv/SetForceTorqueCollisionBehavior");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_msgs::srv::SetForceTorqueCollisionBehavior>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::srv::SetForceTorqueCollisionBehavior::Request>("request",false);
component<::franka_msgs::srv::SetForceTorqueCollisionBehavior::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/srv/SetFullCollisionBehavior");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_msgs::srv::SetFullCollisionBehavior>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::srv::SetFullCollisionBehavior::Request>("request",false);
component<::franka_msgs::srv::SetFullCollisionBehavior::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/srv/SetJointStiffness");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_msgs::srv::SetJointStiffness>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::srv::SetJointStiffness::Request>("request",false);
component<::franka_msgs::srv::SetJointStiffness::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/srv/SetLoad");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_msgs::srv::SetLoad>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::srv::SetLoad::Request>("request",false);
component<::franka_msgs::srv::SetLoad::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/srv/SetStiffnessFrame");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_msgs::srv::SetStiffnessFrame>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::srv::SetStiffnessFrame::Request>("request",false);
component<::franka_msgs::srv::SetStiffnessFrame::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_msgs/srv/SetTCPFrame");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_msgs::srv::SetTCPFrame>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_msgs::srv::SetTCPFrame::Request>("request",false);
component<::franka_msgs::srv::SetTCPFrame::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_spine_msgs/action/MoveAbsolute");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::franka_spine_msgs::action::MoveAbsolute>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_spine_msgs::action::MoveAbsolute::Goal>("goal",false);
component<::franka_spine_msgs::action::MoveAbsolute::Result>("result",true);
component<::franka_spine_msgs::action::MoveAbsolute::Feedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_spine_msgs/msg/SpineParameters");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::msg::SpineParameters>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_spine_msgs::msg::SpineParameters>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_spine_msgs/msg/SpinePosition");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::msg::SpinePosition>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_spine_msgs::msg::SpinePosition>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_spine_msgs/msg/SpineState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::msg::SpineState>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_spine_msgs::msg::SpineState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_spine_msgs/msg/UserLimits");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::franka_spine_msgs::msg::UserLimits>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_spine_msgs::msg::UserLimits>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_spine_msgs/srv/FaultReset");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_spine_msgs::srv::FaultReset>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_spine_msgs::srv::FaultReset::Request>("request",false);
component<::franka_spine_msgs::srv::FaultReset::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_spine_msgs/srv/GetParameters");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_spine_msgs::srv::GetParameters>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_spine_msgs::srv::GetParameters::Request>("request",false);
component<::franka_spine_msgs::srv::GetParameters::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_spine_msgs/srv/GetPosition");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_spine_msgs::srv::GetPosition>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_spine_msgs::srv::GetPosition::Request>("request",false);
component<::franka_spine_msgs::srv::GetPosition::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_spine_msgs/srv/GetSpineState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_spine_msgs::srv::GetSpineState>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_spine_msgs::srv::GetSpineState::Request>("request",false);
component<::franka_spine_msgs::srv::GetSpineState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_spine_msgs/srv/Halt");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_spine_msgs::srv::Halt>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_spine_msgs::srv::Halt::Request>("request",false);
component<::franka_spine_msgs::srv::Halt::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_spine_msgs/srv/SwitchOff");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_spine_msgs::srv::SwitchOff>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_spine_msgs::srv::SwitchOff::Request>("request",false);
component<::franka_spine_msgs::srv::SwitchOff::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("franka_spine_msgs/srv/SwitchOn");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::franka_spine_msgs::srv::SwitchOn>());
bounded_native_io::print(output(),",\"components\":{");
component<::franka_spine_msgs::srv::SwitchOn::Request>("request",false);
component<::franka_spine_msgs::srv::SwitchOn::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Accel");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Accel>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Accel>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/AccelStamped");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::AccelStamped>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::AccelStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Inertia");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Inertia>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Inertia>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/InertiaStamped");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::InertiaStamped>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::InertiaStamped>("message",false);
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
quoted("geometry_msgs/msg/PoseStamped");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::PoseStamped>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::PoseStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Quaternion");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Quaternion>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Quaternion>("message",false);
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
quoted("geometry_msgs/msg/Vector3");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Vector3>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Vector3Stamped");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3Stamped>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Vector3Stamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Wrench");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Wrench>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Wrench>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/WrenchStamped");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::WrenchStamped>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::WrenchStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("sensor_msgs/msg/JointState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::JointState>());
bounded_native_io::print(output(),",\"components\":{");
component<::sensor_msgs::msg::JointState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/msg/Header");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::msg::Header>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),"}}\n");return 0;
}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native industrial metadata refused\n");return 1;}}
