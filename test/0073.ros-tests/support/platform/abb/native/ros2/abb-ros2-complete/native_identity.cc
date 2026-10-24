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
#include <abb_egm_msgs/msg/egm_channel_state.hpp>
#include <abb_egm_msgs/msg/egm_state.hpp>
#include <abb_rapid_msgs/msg/conf_data.hpp>
#include <abb_rapid_msgs/msg/ext_joint.hpp>
#include <abb_rapid_msgs/msg/joint_target.hpp>
#include <abb_rapid_msgs/msg/load_data.hpp>
#include <abb_rapid_msgs/msg/orient.hpp>
#include <abb_rapid_msgs/msg/pos.hpp>
#include <abb_rapid_msgs/msg/pose.hpp>
#include <abb_rapid_msgs/msg/rob_joint.hpp>
#include <abb_rapid_msgs/msg/rob_target.hpp>
#include <abb_rapid_msgs/msg/tool_data.hpp>
#include <abb_rapid_msgs/msg/w_obj_data.hpp>
#include <abb_rapid_sm_addin_msgs/msg/egm_activate_settings.hpp>
#include <abb_rapid_sm_addin_msgs/msg/egm_run_settings.hpp>
#include <abb_rapid_sm_addin_msgs/msg/egm_settings.hpp>
#include <abb_rapid_sm_addin_msgs/msg/egm_setup_settings.hpp>
#include <abb_rapid_sm_addin_msgs/msg/egm_stop_settings.hpp>
#include <abb_rapid_sm_addin_msgs/msg/runtime_state.hpp>
#include <abb_rapid_sm_addin_msgs/msg/state_machine_state.hpp>
#include <abb_rapid_sm_addin_msgs/srv/get_egm_settings.hpp>
#include <abb_rapid_sm_addin_msgs/srv/set_egm_settings.hpp>
#include <abb_rapid_sm_addin_msgs/srv/set_rapid_routine.hpp>
#include <abb_rapid_sm_addin_msgs/srv/set_sg_command.hpp>
#include <abb_robot_msgs/msg/mechanical_unit_state.hpp>
#include <abb_robot_msgs/msg/rapid_symbol_path.hpp>
#include <abb_robot_msgs/msg/rapid_task_state.hpp>
#include <abb_robot_msgs/msg/service_responses.hpp>
#include <abb_robot_msgs/msg/system_state.hpp>
#include <abb_robot_msgs/srv/get_file_contents.hpp>
#include <abb_robot_msgs/srv/get_io_signal.hpp>
#include <abb_robot_msgs/srv/get_rapid_bool.hpp>
#include <abb_robot_msgs/srv/get_rapid_dnum.hpp>
#include <abb_robot_msgs/srv/get_rapid_num.hpp>
#include <abb_robot_msgs/srv/get_rapid_string.hpp>
#include <abb_robot_msgs/srv/get_rapid_symbol.hpp>
#include <abb_robot_msgs/srv/get_robot_controller_description.hpp>
#include <abb_robot_msgs/srv/get_speed_ratio.hpp>
#include <abb_robot_msgs/srv/set_file_contents.hpp>
#include <abb_robot_msgs/srv/set_io_signal.hpp>
#include <abb_robot_msgs/srv/set_rapid_bool.hpp>
#include <abb_robot_msgs/srv/set_rapid_dnum.hpp>
#include <abb_robot_msgs/srv/set_rapid_num.hpp>
#include <abb_robot_msgs/srv/set_rapid_string.hpp>
#include <abb_robot_msgs/srv/set_rapid_symbol.hpp>
#include <abb_robot_msgs/srv/set_speed_ratio.hpp>
#include <abb_robot_msgs/srv/trigger_with_result_code.hpp>
#include <builtin_interfaces/msg/duration.hpp>
#include <builtin_interfaces/msg/time.hpp>
#include <control_msgs/action/follow_joint_trajectory.hpp>
#include <control_msgs/msg/joint_component_tolerance.hpp>
#include <control_msgs/msg/joint_tolerance.hpp>
#include <geometry_msgs/msg/quaternion.hpp>
#include <geometry_msgs/msg/transform.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/vector3.hpp>
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
if(!::uwvm_industrial_source_575c7e779a9e0a42c93e50f0d378e3f26a719b29402aa96e0cf29b1e7b91b901::validate())throw std::runtime_error{"native source constants differ"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_egm_msgs::msg::EGMChannelState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_egm_msgs::msg::EGMChannelState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_egm_msgs::msg::EGMState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_egm_msgs::msg::EGMState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::ConfData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::ConfData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::ExtJoint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::ExtJoint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::JointTarget>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::JointTarget>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::LoadData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::LoadData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::Orient>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::Orient>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::Pos>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::Pos>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::Pose>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::Pose>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::RobJoint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::RobJoint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::RobTarget>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::RobTarget>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::ToolData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::ToolData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::WObjData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::WObjData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::EGMActivateSettings>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::EGMActivateSettings>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::EGMRunSettings>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::EGMRunSettings>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::EGMSettings>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::EGMSettings>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::EGMSetupSettings>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::EGMSetupSettings>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::EGMStopSettings>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::EGMStopSettings>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::RuntimeState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::RuntimeState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::StateMachineState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::StateMachineState>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_rapid_sm_addin_msgs::srv::GetEGMSettings>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::srv::GetEGMSettings::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::srv::GetEGMSettings::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_rapid_sm_addin_msgs::srv::SetEGMSettings>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::srv::SetEGMSettings::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::srv::SetEGMSettings::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_rapid_sm_addin_msgs::srv::SetRAPIDRoutine>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::srv::SetRAPIDRoutine::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::srv::SetRAPIDRoutine::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_rapid_sm_addin_msgs::srv::SetSGCommand>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::srv::SetSGCommand::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::srv::SetSGCommand::Response>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::msg::MechanicalUnitState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::msg::MechanicalUnitState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::msg::RAPIDSymbolPath>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::msg::RAPIDSymbolPath>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::msg::RAPIDTaskState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::msg::RAPIDTaskState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::msg::ServiceResponses>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::msg::ServiceResponses>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::msg::SystemState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::msg::SystemState>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetFileContents>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetFileContents::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetFileContents::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetIOSignal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetIOSignal::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetIOSignal::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetRAPIDBool>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetRAPIDBool::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetRAPIDBool::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetRAPIDDnum>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetRAPIDDnum::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetRAPIDDnum::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetRAPIDNum>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetRAPIDNum::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetRAPIDNum::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetRAPIDString>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetRAPIDString::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetRAPIDString::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetRAPIDSymbol>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetRAPIDSymbol::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetRAPIDSymbol::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetRobotControllerDescription>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetRobotControllerDescription::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetRobotControllerDescription::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetSpeedRatio>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetSpeedRatio::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::GetSpeedRatio::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::SetFileContents>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::SetFileContents::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::SetFileContents::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::SetIOSignal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::SetIOSignal::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::SetIOSignal::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::SetRAPIDBool>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::SetRAPIDBool::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::SetRAPIDBool::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::SetRAPIDDnum>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::SetRAPIDDnum::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::SetRAPIDDnum::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::SetRAPIDNum>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::SetRAPIDNum::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::SetRAPIDNum::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::SetRAPIDString>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::SetRAPIDString::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::SetRAPIDString::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::SetRAPIDSymbol>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::SetRAPIDSymbol::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::SetRAPIDSymbol::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::SetSpeedRatio>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::SetSpeedRatio::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::SetSpeedRatio::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::TriggerWithResultCode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::TriggerWithResultCode::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::srv::TriggerWithResultCode::Response>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Duration>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Duration>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::control_msgs::action::FollowJointTrajectory>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::action::FollowJointTrajectory::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::action::FollowJointTrajectory::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::action::FollowJointTrajectory::Feedback>());
auto a49=rosidl_typesupport_cpp::get_action_type_support_handle<::control_msgs::action::FollowJointTrajectory>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::SendGoalService>());if(a49->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::GetResultService>());if(a49->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::CancelGoalService>());if(a49->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::FeedbackMessage>());if(a49->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::GoalStatusMessage>());if(a49->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::control_msgs::action::FollowJointTrajectory::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::msg::JointComponentTolerance>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::msg::JointComponentTolerance>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::msg::JointTolerance>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::msg::JointTolerance>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Quaternion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Quaternion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Transform>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Transform>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Twist>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Twist>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
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
bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-industrial-native-v1\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"source_profile\":\"uwvm-ros-vendor-complete-bounded-v1\",\"constant_schema_sha256\":\"575c7e779a9e0a42c93e50f0d378e3f26a719b29402aa96e0cf29b1e7b91b901\",\"types\":{");
quoted("abb_egm_msgs/msg/EGMChannelState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_egm_msgs::msg::EGMChannelState>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_egm_msgs::msg::EGMChannelState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_egm_msgs/msg/EGMState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_egm_msgs::msg::EGMState>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_egm_msgs::msg::EGMState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/msg/ConfData");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::ConfData>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::msg::ConfData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/msg/ExtJoint");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::ExtJoint>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::msg::ExtJoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/msg/JointTarget");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::JointTarget>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::msg::JointTarget>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/msg/LoadData");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::LoadData>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::msg::LoadData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/msg/Orient");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::Orient>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::msg::Orient>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/msg/Pos");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::Pos>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::msg::Pos>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/msg/Pose");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::Pose>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::msg::Pose>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/msg/RobJoint");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::RobJoint>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::msg::RobJoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/msg/RobTarget");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::RobTarget>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::msg::RobTarget>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/msg/ToolData");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::ToolData>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::msg::ToolData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/msg/WObjData");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_msgs::msg::WObjData>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::msg::WObjData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/msg/EGMActivateSettings");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::EGMActivateSettings>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::msg::EGMActivateSettings>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/msg/EGMRunSettings");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::EGMRunSettings>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::msg::EGMRunSettings>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/msg/EGMSettings");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::EGMSettings>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::msg::EGMSettings>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/msg/EGMSetupSettings");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::EGMSetupSettings>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::msg::EGMSetupSettings>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/msg/EGMStopSettings");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::EGMStopSettings>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::msg::EGMStopSettings>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/msg/RuntimeState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::RuntimeState>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::msg::RuntimeState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/msg/StateMachineState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_rapid_sm_addin_msgs::msg::StateMachineState>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::msg::StateMachineState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/srv/GetEGMSettings");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_rapid_sm_addin_msgs::srv::GetEGMSettings>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::srv::GetEGMSettings::Request>("request",false);
component<::abb_rapid_sm_addin_msgs::srv::GetEGMSettings::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/srv/SetEGMSettings");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_rapid_sm_addin_msgs::srv::SetEGMSettings>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::srv::SetEGMSettings::Request>("request",false);
component<::abb_rapid_sm_addin_msgs::srv::SetEGMSettings::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_rapid_sm_addin_msgs::srv::SetRAPIDRoutine>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::srv::SetRAPIDRoutine::Request>("request",false);
component<::abb_rapid_sm_addin_msgs::srv::SetRAPIDRoutine::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/srv/SetSGCommand");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_rapid_sm_addin_msgs::srv::SetSGCommand>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::srv::SetSGCommand::Request>("request",false);
component<::abb_rapid_sm_addin_msgs::srv::SetSGCommand::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/msg/MechanicalUnitState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::msg::MechanicalUnitState>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::msg::MechanicalUnitState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/msg/RAPIDSymbolPath");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::msg::RAPIDSymbolPath>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::msg::RAPIDSymbolPath>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/msg/RAPIDTaskState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::msg::RAPIDTaskState>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::msg::RAPIDTaskState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/msg/ServiceResponses");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::msg::ServiceResponses>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::msg::ServiceResponses>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/msg/SystemState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::abb_robot_msgs::msg::SystemState>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::msg::SystemState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/GetFileContents");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetFileContents>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::GetFileContents::Request>("request",false);
component<::abb_robot_msgs::srv::GetFileContents::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/GetIOSignal");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetIOSignal>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::GetIOSignal::Request>("request",false);
component<::abb_robot_msgs::srv::GetIOSignal::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/GetRAPIDBool");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetRAPIDBool>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::GetRAPIDBool::Request>("request",false);
component<::abb_robot_msgs::srv::GetRAPIDBool::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/GetRAPIDDnum");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetRAPIDDnum>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::GetRAPIDDnum::Request>("request",false);
component<::abb_robot_msgs::srv::GetRAPIDDnum::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/GetRAPIDNum");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetRAPIDNum>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::GetRAPIDNum::Request>("request",false);
component<::abb_robot_msgs::srv::GetRAPIDNum::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/GetRAPIDString");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetRAPIDString>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::GetRAPIDString::Request>("request",false);
component<::abb_robot_msgs::srv::GetRAPIDString::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/GetRAPIDSymbol");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetRAPIDSymbol>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::GetRAPIDSymbol::Request>("request",false);
component<::abb_robot_msgs::srv::GetRAPIDSymbol::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/GetRobotControllerDescription");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetRobotControllerDescription>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::GetRobotControllerDescription::Request>("request",false);
component<::abb_robot_msgs::srv::GetRobotControllerDescription::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/GetSpeedRatio");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::GetSpeedRatio>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::GetSpeedRatio::Request>("request",false);
component<::abb_robot_msgs::srv::GetSpeedRatio::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/SetFileContents");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::SetFileContents>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::SetFileContents::Request>("request",false);
component<::abb_robot_msgs::srv::SetFileContents::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/SetIOSignal");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::SetIOSignal>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::SetIOSignal::Request>("request",false);
component<::abb_robot_msgs::srv::SetIOSignal::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/SetRAPIDBool");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::SetRAPIDBool>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::SetRAPIDBool::Request>("request",false);
component<::abb_robot_msgs::srv::SetRAPIDBool::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/SetRAPIDDnum");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::SetRAPIDDnum>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::SetRAPIDDnum::Request>("request",false);
component<::abb_robot_msgs::srv::SetRAPIDDnum::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/SetRAPIDNum");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::SetRAPIDNum>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::SetRAPIDNum::Request>("request",false);
component<::abb_robot_msgs::srv::SetRAPIDNum::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/SetRAPIDString");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::SetRAPIDString>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::SetRAPIDString::Request>("request",false);
component<::abb_robot_msgs::srv::SetRAPIDString::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/SetRAPIDSymbol");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::SetRAPIDSymbol>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::SetRAPIDSymbol::Request>("request",false);
component<::abb_robot_msgs::srv::SetRAPIDSymbol::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/SetSpeedRatio");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::SetSpeedRatio>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::SetSpeedRatio::Request>("request",false);
component<::abb_robot_msgs::srv::SetSpeedRatio::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/srv/TriggerWithResultCode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::abb_robot_msgs::srv::TriggerWithResultCode>());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::srv::TriggerWithResultCode::Request>("request",false);
component<::abb_robot_msgs::srv::TriggerWithResultCode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
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
