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
#include <builtin_interfaces/msg/duration.hpp>
#include <builtin_interfaces/msg/time.hpp>
#include <control_msgs/action/follow_joint_trajectory.hpp>
#include <control_msgs/msg/joint_component_tolerance.hpp>
#include <control_msgs/msg/joint_tolerance.hpp>
#include <geometry_msgs/msg/quaternion.hpp>
#include <geometry_msgs/msg/transform.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/vector3.hpp>
#include <industrial_msgs/msg/debug_level.hpp>
#include <industrial_msgs/msg/device_info.hpp>
#include <industrial_msgs/msg/robot_mode.hpp>
#include <industrial_msgs/msg/robot_status.hpp>
#include <industrial_msgs/msg/service_return_code.hpp>
#include <industrial_msgs/msg/tri_state.hpp>
#include <industrial_msgs/srv/cmd_joint_trajectory.hpp>
#include <industrial_msgs/srv/get_robot_info.hpp>
#include <industrial_msgs/srv/set_drive_power.hpp>
#include <industrial_msgs/srv/set_remote_logger_level.hpp>
#include <industrial_msgs/srv/start_motion.hpp>
#include <industrial_msgs/srv/stop_motion.hpp>
#include <motoros2_interfaces/msg/alarm_cause_remedy.hpp>
#include <motoros2_interfaces/msg/alarm_info.hpp>
#include <motoros2_interfaces/msg/error_info.hpp>
#include <motoros2_interfaces/msg/inform_job_crud_result_codes.hpp>
#include <motoros2_interfaces/msg/init_traj_enum.hpp>
#include <motoros2_interfaces/msg/io_result_codes.hpp>
#include <motoros2_interfaces/msg/motion_ready_enum.hpp>
#include <motoros2_interfaces/msg/queue_result_enum.hpp>
#include <motoros2_interfaces/msg/selection_result_codes.hpp>
#include <motoros2_interfaces/srv/get_active_alarm_info.hpp>
#include <motoros2_interfaces/srv/list_inform_jobs.hpp>
#include <motoros2_interfaces/srv/queue_traj_point.hpp>
#include <motoros2_interfaces/srv/read_group_io.hpp>
#include <motoros2_interfaces/srv/read_m_register.hpp>
#include <motoros2_interfaces/srv/read_single_io.hpp>
#include <motoros2_interfaces/srv/reset_error.hpp>
#include <motoros2_interfaces/srv/select_motion_tool.hpp>
#include <motoros2_interfaces/srv/start_point_queue_mode.hpp>
#include <motoros2_interfaces/srv/start_traj_mode.hpp>
#include <motoros2_interfaces/srv/write_group_io.hpp>
#include <motoros2_interfaces/srv/write_m_register.hpp>
#include <motoros2_interfaces/srv/write_single_io.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <std_msgs/msg/header.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <tf2_msgs/msg/tf_message.hpp>
#include <trajectory_msgs/msg/joint_trajectory.hpp>
#include <trajectory_msgs/msg/joint_trajectory_point.hpp>
#include <trajectory_msgs/msg/multi_dof_joint_trajectory.hpp>
#include <trajectory_msgs/msg/multi_dof_joint_trajectory_point.hpp>
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
if(!::uwvm_industrial_source_485a2f48a523b906ca05c20b18c53daf6b4adbcdbfa8d47a5c5fb49fc6bad037::validate())throw std::runtime_error{"native source constants differ"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Duration>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Duration>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::control_msgs::action::FollowJointTrajectory>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::action::FollowJointTrajectory::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::action::FollowJointTrajectory::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::action::FollowJointTrajectory::Feedback>());
auto a2=rosidl_typesupport_cpp::get_action_type_support_handle<::control_msgs::action::FollowJointTrajectory>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::SendGoalService>());if(a2->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::GetResultService>());if(a2->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::CancelGoalService>());if(a2->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::FeedbackMessage>());if(a2->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::GoalStatusMessage>());if(a2->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::msg::JointComponentTolerance>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::msg::JointComponentTolerance>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::msg::JointTolerance>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::msg::JointTolerance>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Quaternion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Quaternion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Transform>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Transform>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::TransformStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::TransformStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Twist>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Twist>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::DebugLevel>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::DebugLevel>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::DeviceInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::DeviceInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::RobotMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::RobotMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::RobotStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::RobotStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::ServiceReturnCode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::ServiceReturnCode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::TriState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::TriState>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::industrial_msgs::srv::CmdJointTrajectory>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::srv::CmdJointTrajectory::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::srv::CmdJointTrajectory::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::industrial_msgs::srv::GetRobotInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::srv::GetRobotInfo::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::srv::GetRobotInfo::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::industrial_msgs::srv::SetDrivePower>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::srv::SetDrivePower::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::srv::SetDrivePower::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::industrial_msgs::srv::SetRemoteLoggerLevel>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::srv::SetRemoteLoggerLevel::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::srv::SetRemoteLoggerLevel::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::industrial_msgs::srv::StartMotion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::srv::StartMotion::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::srv::StartMotion::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::industrial_msgs::srv::StopMotion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::srv::StopMotion::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::srv::StopMotion::Response>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::AlarmCauseRemedy>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::AlarmCauseRemedy>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::AlarmInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::AlarmInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::ErrorInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::ErrorInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::InformJobCrudResultCodes>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::InformJobCrudResultCodes>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::InitTrajEnum>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::InitTrajEnum>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::IoResultCodes>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::IoResultCodes>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::MotionReadyEnum>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::MotionReadyEnum>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::QueueResultEnum>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::QueueResultEnum>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::SelectionResultCodes>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::SelectionResultCodes>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::GetActiveAlarmInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::GetActiveAlarmInfo::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::GetActiveAlarmInfo::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::ListInformJobs>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::ListInformJobs::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::ListInformJobs::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::QueueTrajPoint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::QueueTrajPoint::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::QueueTrajPoint::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::ReadGroupIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::ReadGroupIO::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::ReadGroupIO::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::ReadMRegister>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::ReadMRegister::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::ReadMRegister::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::ReadSingleIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::ReadSingleIO::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::ReadSingleIO::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::ResetError>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::ResetError::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::ResetError::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::SelectMotionTool>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::SelectMotionTool::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::SelectMotionTool::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::StartPointQueueMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::StartPointQueueMode::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::StartPointQueueMode::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::StartTrajMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::StartTrajMode::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::StartTrajMode::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::WriteGroupIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::WriteGroupIO::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::WriteGroupIO::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::WriteMRegister>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::WriteMRegister::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::WriteMRegister::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::WriteSingleIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::WriteSingleIO::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::srv::WriteSingleIO::Response>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::JointState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::JointState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::std_srvs::srv::Trigger>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_srvs::srv::Trigger::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_srvs::srv::Trigger::Response>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::tf2_msgs::msg::TFMessage>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::tf2_msgs::msg::TFMessage>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::trajectory_msgs::msg::JointTrajectory>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::trajectory_msgs::msg::JointTrajectory>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::trajectory_msgs::msg::JointTrajectoryPoint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::trajectory_msgs::msg::JointTrajectoryPoint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::trajectory_msgs::msg::MultiDOFJointTrajectory>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::trajectory_msgs::msg::MultiDOFJointTrajectory>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::trajectory_msgs::msg::MultiDOFJointTrajectoryPoint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::trajectory_msgs::msg::MultiDOFJointTrajectoryPoint>());
if(::raise(SIGSTOP)!=0)throw std::runtime_error{"owned mapping stop failed"};
bounded_native_io::gate('I');
bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-industrial-native-v1\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"source_profile\":\"uwvm-ros-vendor-complete-bounded-v1\",\"constant_schema_sha256\":\"485a2f48a523b906ca05c20b18c53daf6b4adbcdbfa8d47a5c5fb49fc6bad037\",\"types\":{");
quoted("builtin_interfaces/msg/Duration");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Duration>());
bounded_native_io::print(output(),",\"components\":{");
component<::builtin_interfaces::msg::Duration>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("builtin_interfaces/msg/Time");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
bounded_native_io::print(output(),",\"components\":{");
component<::builtin_interfaces::msg::Time>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("control_msgs/action/FollowJointTrajectory");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::control_msgs::action::FollowJointTrajectory>());
bounded_native_io::print(output(),",\"components\":{");
component<::control_msgs::action::FollowJointTrajectory::Goal>("goal",false);
component<::control_msgs::action::FollowJointTrajectory::Result>("result",true);
component<::control_msgs::action::FollowJointTrajectory::Feedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("control_msgs/msg/JointComponentTolerance");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::msg::JointComponentTolerance>());
bounded_native_io::print(output(),",\"components\":{");
component<::control_msgs::msg::JointComponentTolerance>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("control_msgs/msg/JointTolerance");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::msg::JointTolerance>());
bounded_native_io::print(output(),",\"components\":{");
component<::control_msgs::msg::JointTolerance>("message",false);
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
quoted("geometry_msgs/msg/Vector3");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Vector3>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("industrial_msgs/msg/DebugLevel");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::DebugLevel>());
bounded_native_io::print(output(),",\"components\":{");
component<::industrial_msgs::msg::DebugLevel>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("industrial_msgs/msg/DeviceInfo");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::DeviceInfo>());
bounded_native_io::print(output(),",\"components\":{");
component<::industrial_msgs::msg::DeviceInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("industrial_msgs/msg/RobotMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::RobotMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::industrial_msgs::msg::RobotMode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("industrial_msgs/msg/RobotStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::RobotStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::industrial_msgs::msg::RobotStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("industrial_msgs/msg/ServiceReturnCode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::ServiceReturnCode>());
bounded_native_io::print(output(),",\"components\":{");
component<::industrial_msgs::msg::ServiceReturnCode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("industrial_msgs/msg/TriState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::industrial_msgs::msg::TriState>());
bounded_native_io::print(output(),",\"components\":{");
component<::industrial_msgs::msg::TriState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("industrial_msgs/srv/CmdJointTrajectory");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::industrial_msgs::srv::CmdJointTrajectory>());
bounded_native_io::print(output(),",\"components\":{");
component<::industrial_msgs::srv::CmdJointTrajectory::Request>("request",false);
component<::industrial_msgs::srv::CmdJointTrajectory::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("industrial_msgs/srv/GetRobotInfo");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::industrial_msgs::srv::GetRobotInfo>());
bounded_native_io::print(output(),",\"components\":{");
component<::industrial_msgs::srv::GetRobotInfo::Request>("request",false);
component<::industrial_msgs::srv::GetRobotInfo::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("industrial_msgs/srv/SetDrivePower");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::industrial_msgs::srv::SetDrivePower>());
bounded_native_io::print(output(),",\"components\":{");
component<::industrial_msgs::srv::SetDrivePower::Request>("request",false);
component<::industrial_msgs::srv::SetDrivePower::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("industrial_msgs/srv/SetRemoteLoggerLevel");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::industrial_msgs::srv::SetRemoteLoggerLevel>());
bounded_native_io::print(output(),",\"components\":{");
component<::industrial_msgs::srv::SetRemoteLoggerLevel::Request>("request",false);
component<::industrial_msgs::srv::SetRemoteLoggerLevel::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("industrial_msgs/srv/StartMotion");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::industrial_msgs::srv::StartMotion>());
bounded_native_io::print(output(),",\"components\":{");
component<::industrial_msgs::srv::StartMotion::Request>("request",false);
component<::industrial_msgs::srv::StartMotion::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("industrial_msgs/srv/StopMotion");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::industrial_msgs::srv::StopMotion>());
bounded_native_io::print(output(),",\"components\":{");
component<::industrial_msgs::srv::StopMotion::Request>("request",false);
component<::industrial_msgs::srv::StopMotion::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/msg/AlarmCauseRemedy");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::AlarmCauseRemedy>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::msg::AlarmCauseRemedy>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/msg/AlarmInfo");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::AlarmInfo>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::msg::AlarmInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/msg/ErrorInfo");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::ErrorInfo>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::msg::ErrorInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/msg/InformJobCrudResultCodes");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::InformJobCrudResultCodes>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::msg::InformJobCrudResultCodes>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/msg/InitTrajEnum");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::InitTrajEnum>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::msg::InitTrajEnum>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/msg/IoResultCodes");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::IoResultCodes>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::msg::IoResultCodes>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/msg/MotionReadyEnum");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::MotionReadyEnum>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::msg::MotionReadyEnum>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/msg/QueueResultEnum");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::QueueResultEnum>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::msg::QueueResultEnum>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/msg/SelectionResultCodes");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::motoros2_interfaces::msg::SelectionResultCodes>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::msg::SelectionResultCodes>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/srv/GetActiveAlarmInfo");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::GetActiveAlarmInfo>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::srv::GetActiveAlarmInfo::Request>("request",false);
component<::motoros2_interfaces::srv::GetActiveAlarmInfo::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/srv/ListInformJobs");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::ListInformJobs>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::srv::ListInformJobs::Request>("request",false);
component<::motoros2_interfaces::srv::ListInformJobs::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/srv/QueueTrajPoint");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::QueueTrajPoint>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::srv::QueueTrajPoint::Request>("request",false);
component<::motoros2_interfaces::srv::QueueTrajPoint::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/srv/ReadGroupIO");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::ReadGroupIO>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::srv::ReadGroupIO::Request>("request",false);
component<::motoros2_interfaces::srv::ReadGroupIO::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/srv/ReadMRegister");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::ReadMRegister>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::srv::ReadMRegister::Request>("request",false);
component<::motoros2_interfaces::srv::ReadMRegister::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/srv/ReadSingleIO");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::ReadSingleIO>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::srv::ReadSingleIO::Request>("request",false);
component<::motoros2_interfaces::srv::ReadSingleIO::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/srv/ResetError");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::ResetError>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::srv::ResetError::Request>("request",false);
component<::motoros2_interfaces::srv::ResetError::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/srv/SelectMotionTool");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::SelectMotionTool>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::srv::SelectMotionTool::Request>("request",false);
component<::motoros2_interfaces::srv::SelectMotionTool::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/srv/StartPointQueueMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::StartPointQueueMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::srv::StartPointQueueMode::Request>("request",false);
component<::motoros2_interfaces::srv::StartPointQueueMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/srv/StartTrajMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::StartTrajMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::srv::StartTrajMode::Request>("request",false);
component<::motoros2_interfaces::srv::StartTrajMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/srv/WriteGroupIO");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::WriteGroupIO>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::srv::WriteGroupIO::Request>("request",false);
component<::motoros2_interfaces::srv::WriteGroupIO::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/srv/WriteMRegister");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::WriteMRegister>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::srv::WriteMRegister::Request>("request",false);
component<::motoros2_interfaces::srv::WriteMRegister::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motoros2_interfaces/srv/WriteSingleIO");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::motoros2_interfaces::srv::WriteSingleIO>());
bounded_native_io::print(output(),",\"components\":{");
component<::motoros2_interfaces::srv::WriteSingleIO::Request>("request",false);
component<::motoros2_interfaces::srv::WriteSingleIO::Response>("response",true);
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
bounded_native_io::print(output(),',');
quoted("std_srvs/srv/Trigger");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::std_srvs::srv::Trigger>());
bounded_native_io::print(output(),",\"components\":{");
component<::std_srvs::srv::Trigger::Request>("request",false);
component<::std_srvs::srv::Trigger::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("tf2_msgs/msg/TFMessage");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::tf2_msgs::msg::TFMessage>());
bounded_native_io::print(output(),",\"components\":{");
component<::tf2_msgs::msg::TFMessage>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("trajectory_msgs/msg/JointTrajectory");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::trajectory_msgs::msg::JointTrajectory>());
bounded_native_io::print(output(),",\"components\":{");
component<::trajectory_msgs::msg::JointTrajectory>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("trajectory_msgs/msg/JointTrajectoryPoint");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::trajectory_msgs::msg::JointTrajectoryPoint>());
bounded_native_io::print(output(),",\"components\":{");
component<::trajectory_msgs::msg::JointTrajectoryPoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("trajectory_msgs/msg/MultiDOFJointTrajectory");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::trajectory_msgs::msg::MultiDOFJointTrajectory>());
bounded_native_io::print(output(),",\"components\":{");
component<::trajectory_msgs::msg::MultiDOFJointTrajectory>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("trajectory_msgs/msg/MultiDOFJointTrajectoryPoint");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::trajectory_msgs::msg::MultiDOFJointTrajectoryPoint>());
bounded_native_io::print(output(),",\"components\":{");
component<::trajectory_msgs::msg::MultiDOFJointTrajectoryPoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),"}}\n");return 0;
}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native industrial metadata refused\n");return 1;}}
