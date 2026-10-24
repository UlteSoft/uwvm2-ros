/** Industrial native metadata source; no Node, endpoint or grant. */
#include "test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/native_protocol.h"
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
#include <fanuc_msgs/msg/analog_io.hpp>
#include <fanuc_msgs/msg/analog_io_cmd.hpp>
#include <fanuc_msgs/msg/analog_io_state.hpp>
#include <fanuc_msgs/msg/bool_io.hpp>
#include <fanuc_msgs/msg/collaborative_speed_scaling.hpp>
#include <fanuc_msgs/msg/connection_status.hpp>
#include <fanuc_msgs/msg/force_sensor.hpp>
#include <fanuc_msgs/msg/frame.hpp>
#include <fanuc_msgs/msg/io_cmd.hpp>
#include <fanuc_msgs/msg/io_state.hpp>
#include <fanuc_msgs/msg/io_type.hpp>
#include <fanuc_msgs/msg/num_reg.hpp>
#include <fanuc_msgs/msg/num_reg_cmd.hpp>
#include <fanuc_msgs/msg/num_reg_state.hpp>
#include <fanuc_msgs/msg/rmi_status.hpp>
#include <fanuc_msgs/msg/robot_status.hpp>
#include <fanuc_msgs/msg/robot_status_ext.hpp>
#include <fanuc_msgs/srv/cfg_force_sensor.hpp>
#include <fanuc_msgs/srv/get_analog_io.hpp>
#include <fanuc_msgs/srv/get_bool_io.hpp>
#include <fanuc_msgs/srv/get_cartesian_position.hpp>
#include <fanuc_msgs/srv/get_group_io.hpp>
#include <fanuc_msgs/srv/get_num_reg.hpp>
#include <fanuc_msgs/srv/get_payload_comp.hpp>
#include <fanuc_msgs/srv/get_payload_id.hpp>
#include <fanuc_msgs/srv/get_payload_value.hpp>
#include <fanuc_msgs/srv/get_pos_reg.hpp>
#include <fanuc_msgs/srv/get_tcp_speed.hpp>
#include <fanuc_msgs/srv/get_u_frame_data.hpp>
#include <fanuc_msgs/srv/get_u_frame_u_tool.hpp>
#include <fanuc_msgs/srv/get_u_tool_data.hpp>
#include <fanuc_msgs/srv/read_error.hpp>
#include <fanuc_msgs/srv/reset.hpp>
#include <fanuc_msgs/srv/set_analog_io.hpp>
#include <fanuc_msgs/srv/set_bool_io.hpp>
#include <fanuc_msgs/srv/set_gen_override.hpp>
#include <fanuc_msgs/srv/set_group_io.hpp>
#include <fanuc_msgs/srv/set_num_reg.hpp>
#include <fanuc_msgs/srv/set_payload_comp.hpp>
#include <fanuc_msgs/srv/set_payload_id.hpp>
#include <fanuc_msgs/srv/set_payload_value.hpp>
#include <fanuc_msgs/srv/set_pos_reg.hpp>
#include <fanuc_msgs/srv/set_u_frame_data.hpp>
#include <fanuc_msgs/srv/set_u_frame_u_tool.hpp>
#include <fanuc_msgs/srv/set_u_tool_data.hpp>
#include <fanuc_msgs/srv/switch_control_state.hpp>
#include <geometry_msgs/msg/quaternion.hpp>
#include <geometry_msgs/msg/transform.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/vector3.hpp>
#include <rmi_msgs/msg/call_param.hpp>
#include <rmi_msgs/msg/cartesian.hpp>
#include <rmi_msgs/msg/command.hpp>
#include <rmi_msgs/msg/command_options.hpp>
#include <rmi_msgs/msg/joint.hpp>
#include <rmi_msgs/msg/motion_instruction_options.hpp>
#include <rmi_msgs/msg/motion_type.hpp>
#include <rmi_msgs/msg/representation.hpp>
#include <rmi_msgs/msg/speed_type.hpp>
#include <rmi_msgs/msg/term_type.hpp>
#include <rmi_msgs/srv/add_call.hpp>
#include <rmi_msgs/srv/add_motion_instruction.hpp>
#include <rmi_msgs/srv/add_set_payload.hpp>
#include <rmi_msgs/srv/add_set_u_frame.hpp>
#include <rmi_msgs/srv/add_set_u_tool.hpp>
#include <rmi_msgs/srv/add_wait_din.hpp>
#include <rmi_msgs/srv/add_wait_time.hpp>
#include <rmi_msgs/srv/call_command.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <std_msgs/msg/header.hpp>
#include <std_srvs/srv/trigger.hpp>
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
if(!::uwvm_industrial_source_4d6c6529657f3c2ff6fd57d916671fce5ab96eae0d9b74eba980fb6c633e7ba5::validate())throw std::runtime_error{"native source constants differ"};
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
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::AnalogIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::AnalogIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::AnalogIOCmd>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::AnalogIOCmd>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::AnalogIOState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::AnalogIOState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::BoolIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::BoolIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::CollaborativeSpeedScaling>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::CollaborativeSpeedScaling>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::ConnectionStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::ConnectionStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::ForceSensor>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::ForceSensor>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::Frame>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::Frame>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::IOCmd>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::IOCmd>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::IOState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::IOState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::IOType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::IOType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::NumReg>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::NumReg>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::NumRegCmd>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::NumRegCmd>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::NumRegState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::NumRegState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::RMIStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::RMIStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::RobotStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::RobotStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::RobotStatusExt>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::RobotStatusExt>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::CfgForceSensor>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::CfgForceSensor::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::CfgForceSensor::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetAnalogIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetAnalogIO::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetAnalogIO::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetBoolIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetBoolIO::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetBoolIO::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetCartesianPosition>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetCartesianPosition::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetCartesianPosition::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetGroupIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetGroupIO::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetGroupIO::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetNumReg>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetNumReg::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetNumReg::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetPayloadComp>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetPayloadComp::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetPayloadComp::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetPayloadID>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetPayloadID::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetPayloadID::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetPayloadValue>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetPayloadValue::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetPayloadValue::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetPosReg>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetPosReg::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetPosReg::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetTCPSpeed>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetTCPSpeed::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetTCPSpeed::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetUFrameData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetUFrameData::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetUFrameData::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetUFrameUTool>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetUFrameUTool::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetUFrameUTool::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetUToolData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetUToolData::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::GetUToolData::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::ReadError>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::ReadError::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::ReadError::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::Reset>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::Reset::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::Reset::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetAnalogIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetAnalogIO::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetAnalogIO::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetBoolIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetBoolIO::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetBoolIO::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetGenOverride>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetGenOverride::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetGenOverride::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetGroupIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetGroupIO::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetGroupIO::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetNumReg>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetNumReg::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetNumReg::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetPayloadComp>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetPayloadComp::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetPayloadComp::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetPayloadID>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetPayloadID::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetPayloadID::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetPayloadValue>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetPayloadValue::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetPayloadValue::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetPosReg>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetPosReg::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetPosReg::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetUFrameData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetUFrameData::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetUFrameData::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetUFrameUTool>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetUFrameUTool::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetUFrameUTool::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetUToolData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetUToolData::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SetUToolData::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SwitchControlState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SwitchControlState::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::srv::SwitchControlState::Response>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Quaternion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Quaternion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Transform>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Transform>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Twist>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Twist>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::CallParam>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::CallParam>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::Cartesian>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::Cartesian>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::Command>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::Command>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::CommandOptions>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::CommandOptions>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::Joint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::Joint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::MotionInstructionOptions>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::MotionInstructionOptions>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::MotionType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::MotionType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::Representation>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::Representation>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::SpeedType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::SpeedType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::TermType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::TermType>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::rmi_msgs::srv::AddCall>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::srv::AddCall::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::srv::AddCall::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::rmi_msgs::srv::AddMotionInstruction>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::srv::AddMotionInstruction::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::srv::AddMotionInstruction::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::rmi_msgs::srv::AddSetPayload>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::srv::AddSetPayload::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::srv::AddSetPayload::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::rmi_msgs::srv::AddSetUFrame>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::srv::AddSetUFrame::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::srv::AddSetUFrame::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::rmi_msgs::srv::AddSetUTool>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::srv::AddSetUTool::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::srv::AddSetUTool::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::rmi_msgs::srv::AddWaitDIN>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::srv::AddWaitDIN::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::srv::AddWaitDIN::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::rmi_msgs::srv::AddWaitTime>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::srv::AddWaitTime::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::srv::AddWaitTime::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::rmi_msgs::srv::CallCommand>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::srv::CallCommand::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::srv::CallCommand::Response>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::JointState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::sensor_msgs::msg::JointState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::std_srvs::srv::Trigger>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_srvs::srv::Trigger::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_srvs::srv::Trigger::Response>());
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
bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-industrial-native-v1\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"source_profile\":\"uwvm-ros-vendor-complete-bounded-v1\",\"constant_schema_sha256\":\"4d6c6529657f3c2ff6fd57d916671fce5ab96eae0d9b74eba980fb6c633e7ba5\",\"types\":{");
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
quoted("fanuc_msgs/msg/AnalogIO");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::AnalogIO>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::msg::AnalogIO>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/msg/AnalogIOCmd");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::AnalogIOCmd>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::msg::AnalogIOCmd>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/msg/AnalogIOState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::AnalogIOState>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::msg::AnalogIOState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/msg/BoolIO");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::BoolIO>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::msg::BoolIO>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/msg/CollaborativeSpeedScaling");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::CollaborativeSpeedScaling>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::msg::CollaborativeSpeedScaling>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/msg/ConnectionStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::ConnectionStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::msg::ConnectionStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/msg/ForceSensor");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::ForceSensor>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::msg::ForceSensor>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/msg/Frame");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::Frame>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::msg::Frame>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/msg/IOCmd");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::IOCmd>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::msg::IOCmd>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/msg/IOState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::IOState>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::msg::IOState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/msg/IOType");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::IOType>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::msg::IOType>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/msg/NumReg");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::NumReg>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::msg::NumReg>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/msg/NumRegCmd");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::NumRegCmd>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::msg::NumRegCmd>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/msg/NumRegState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::NumRegState>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::msg::NumRegState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/msg/RMIStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::RMIStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::msg::RMIStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/msg/RobotStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::RobotStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::msg::RobotStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/msg/RobotStatusExt");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::fanuc_msgs::msg::RobotStatusExt>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::msg::RobotStatusExt>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/CfgForceSensor");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::CfgForceSensor>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::CfgForceSensor::Request>("request",false);
component<::fanuc_msgs::srv::CfgForceSensor::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/GetAnalogIO");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetAnalogIO>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::GetAnalogIO::Request>("request",false);
component<::fanuc_msgs::srv::GetAnalogIO::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/GetBoolIO");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetBoolIO>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::GetBoolIO::Request>("request",false);
component<::fanuc_msgs::srv::GetBoolIO::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/GetCartesianPosition");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetCartesianPosition>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::GetCartesianPosition::Request>("request",false);
component<::fanuc_msgs::srv::GetCartesianPosition::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/GetGroupIO");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetGroupIO>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::GetGroupIO::Request>("request",false);
component<::fanuc_msgs::srv::GetGroupIO::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/GetNumReg");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetNumReg>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::GetNumReg::Request>("request",false);
component<::fanuc_msgs::srv::GetNumReg::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/GetPayloadComp");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetPayloadComp>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::GetPayloadComp::Request>("request",false);
component<::fanuc_msgs::srv::GetPayloadComp::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/GetPayloadID");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetPayloadID>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::GetPayloadID::Request>("request",false);
component<::fanuc_msgs::srv::GetPayloadID::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/GetPayloadValue");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetPayloadValue>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::GetPayloadValue::Request>("request",false);
component<::fanuc_msgs::srv::GetPayloadValue::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/GetPosReg");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetPosReg>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::GetPosReg::Request>("request",false);
component<::fanuc_msgs::srv::GetPosReg::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/GetTCPSpeed");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetTCPSpeed>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::GetTCPSpeed::Request>("request",false);
component<::fanuc_msgs::srv::GetTCPSpeed::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/GetUFrameData");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetUFrameData>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::GetUFrameData::Request>("request",false);
component<::fanuc_msgs::srv::GetUFrameData::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/GetUFrameUTool");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetUFrameUTool>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::GetUFrameUTool::Request>("request",false);
component<::fanuc_msgs::srv::GetUFrameUTool::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/GetUToolData");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::GetUToolData>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::GetUToolData::Request>("request",false);
component<::fanuc_msgs::srv::GetUToolData::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/ReadError");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::ReadError>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::ReadError::Request>("request",false);
component<::fanuc_msgs::srv::ReadError::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/Reset");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::Reset>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::Reset::Request>("request",false);
component<::fanuc_msgs::srv::Reset::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/SetAnalogIO");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetAnalogIO>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::SetAnalogIO::Request>("request",false);
component<::fanuc_msgs::srv::SetAnalogIO::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/SetBoolIO");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetBoolIO>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::SetBoolIO::Request>("request",false);
component<::fanuc_msgs::srv::SetBoolIO::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/SetGenOverride");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetGenOverride>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::SetGenOverride::Request>("request",false);
component<::fanuc_msgs::srv::SetGenOverride::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/SetGroupIO");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetGroupIO>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::SetGroupIO::Request>("request",false);
component<::fanuc_msgs::srv::SetGroupIO::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/SetNumReg");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetNumReg>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::SetNumReg::Request>("request",false);
component<::fanuc_msgs::srv::SetNumReg::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/SetPayloadComp");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetPayloadComp>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::SetPayloadComp::Request>("request",false);
component<::fanuc_msgs::srv::SetPayloadComp::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/SetPayloadID");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetPayloadID>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::SetPayloadID::Request>("request",false);
component<::fanuc_msgs::srv::SetPayloadID::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/SetPayloadValue");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetPayloadValue>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::SetPayloadValue::Request>("request",false);
component<::fanuc_msgs::srv::SetPayloadValue::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/SetPosReg");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetPosReg>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::SetPosReg::Request>("request",false);
component<::fanuc_msgs::srv::SetPosReg::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/SetUFrameData");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetUFrameData>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::SetUFrameData::Request>("request",false);
component<::fanuc_msgs::srv::SetUFrameData::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/SetUFrameUTool");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetUFrameUTool>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::SetUFrameUTool::Request>("request",false);
component<::fanuc_msgs::srv::SetUFrameUTool::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/SetUToolData");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SetUToolData>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::SetUToolData::Request>("request",false);
component<::fanuc_msgs::srv::SetUToolData::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("fanuc_msgs/srv/SwitchControlState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::fanuc_msgs::srv::SwitchControlState>());
bounded_native_io::print(output(),",\"components\":{");
component<::fanuc_msgs::srv::SwitchControlState::Request>("request",false);
component<::fanuc_msgs::srv::SwitchControlState::Response>("response",true);
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
quoted("rmi_msgs/msg/CallParam");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::CallParam>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::msg::CallParam>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("rmi_msgs/msg/Cartesian");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::Cartesian>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::msg::Cartesian>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("rmi_msgs/msg/Command");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::Command>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::msg::Command>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("rmi_msgs/msg/CommandOptions");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::CommandOptions>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::msg::CommandOptions>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("rmi_msgs/msg/Joint");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::Joint>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::msg::Joint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("rmi_msgs/msg/MotionInstructionOptions");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::MotionInstructionOptions>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::msg::MotionInstructionOptions>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("rmi_msgs/msg/MotionType");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::MotionType>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::msg::MotionType>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("rmi_msgs/msg/Representation");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::Representation>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::msg::Representation>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("rmi_msgs/msg/SpeedType");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::SpeedType>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::msg::SpeedType>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("rmi_msgs/msg/TermType");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::rmi_msgs::msg::TermType>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::msg::TermType>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("rmi_msgs/srv/AddCall");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::rmi_msgs::srv::AddCall>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::srv::AddCall::Request>("request",false);
component<::rmi_msgs::srv::AddCall::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("rmi_msgs/srv/AddMotionInstruction");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::rmi_msgs::srv::AddMotionInstruction>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::srv::AddMotionInstruction::Request>("request",false);
component<::rmi_msgs::srv::AddMotionInstruction::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("rmi_msgs/srv/AddSetPayload");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::rmi_msgs::srv::AddSetPayload>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::srv::AddSetPayload::Request>("request",false);
component<::rmi_msgs::srv::AddSetPayload::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("rmi_msgs/srv/AddSetUFrame");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::rmi_msgs::srv::AddSetUFrame>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::srv::AddSetUFrame::Request>("request",false);
component<::rmi_msgs::srv::AddSetUFrame::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("rmi_msgs/srv/AddSetUTool");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::rmi_msgs::srv::AddSetUTool>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::srv::AddSetUTool::Request>("request",false);
component<::rmi_msgs::srv::AddSetUTool::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("rmi_msgs/srv/AddWaitDIN");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::rmi_msgs::srv::AddWaitDIN>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::srv::AddWaitDIN::Request>("request",false);
component<::rmi_msgs::srv::AddWaitDIN::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("rmi_msgs/srv/AddWaitTime");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::rmi_msgs::srv::AddWaitTime>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::srv::AddWaitTime::Request>("request",false);
component<::rmi_msgs::srv::AddWaitTime::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("rmi_msgs/srv/CallCommand");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::rmi_msgs::srv::CallCommand>());
bounded_native_io::print(output(),",\"components\":{");
component<::rmi_msgs::srv::CallCommand::Request>("request",false);
component<::rmi_msgs::srv::CallCommand::Response>("response",true);
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
