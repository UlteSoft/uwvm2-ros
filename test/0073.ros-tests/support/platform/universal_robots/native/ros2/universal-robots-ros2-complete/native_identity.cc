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
#include <control_msgs/msg/joint_tolerance.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/quaternion.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/vector3.hpp>
#include <geometry_msgs/msg/vector3_stamped.hpp>
#include <geometry_msgs/msg/wrench.hpp>
#include <std_msgs/msg/header.hpp>
#include <trajectory_msgs/msg/joint_trajectory.hpp>
#include <trajectory_msgs/msg/joint_trajectory_point.hpp>
#include <ur_dashboard_msgs/action/set_mode.hpp>
#include <ur_dashboard_msgs/msg/operational_mode.hpp>
#include <ur_dashboard_msgs/msg/program_information.hpp>
#include <ur_dashboard_msgs/msg/program_state.hpp>
#include <ur_dashboard_msgs/msg/robot_mode.hpp>
#include <ur_dashboard_msgs/msg/safety_mode.hpp>
#include <ur_dashboard_msgs/msg/safety_status.hpp>
#include <ur_dashboard_msgs/msg/user_role.hpp>
#include <ur_dashboard_msgs/msg/version_information.hpp>
#include <ur_dashboard_msgs/srv/add_to_log.hpp>
#include <ur_dashboard_msgs/srv/download_program.hpp>
#include <ur_dashboard_msgs/srv/download_support_file.hpp>
#include <ur_dashboard_msgs/srv/generate_flight_report.hpp>
#include <ur_dashboard_msgs/srv/generate_support_file.hpp>
#include <ur_dashboard_msgs/srv/get_loaded_program.hpp>
#include <ur_dashboard_msgs/srv/get_operational_mode.hpp>
#include <ur_dashboard_msgs/srv/get_poly_scope_version.hpp>
#include <ur_dashboard_msgs/srv/get_program_state.hpp>
#include <ur_dashboard_msgs/srv/get_programs.hpp>
#include <ur_dashboard_msgs/srv/get_robot_mode.hpp>
#include <ur_dashboard_msgs/srv/get_robot_model.hpp>
#include <ur_dashboard_msgs/srv/get_safety_mode.hpp>
#include <ur_dashboard_msgs/srv/get_safety_status.hpp>
#include <ur_dashboard_msgs/srv/get_serial_number.hpp>
#include <ur_dashboard_msgs/srv/get_user_role.hpp>
#include <ur_dashboard_msgs/srv/is_in_remote_control.hpp>
#include <ur_dashboard_msgs/srv/is_program_running.hpp>
#include <ur_dashboard_msgs/srv/is_program_saved.hpp>
#include <ur_dashboard_msgs/srv/load.hpp>
#include <ur_dashboard_msgs/srv/popup.hpp>
#include <ur_dashboard_msgs/srv/raw_request.hpp>
#include <ur_dashboard_msgs/srv/set_operational_mode.hpp>
#include <ur_dashboard_msgs/srv/set_user_role.hpp>
#include <ur_dashboard_msgs/srv/upload_program.hpp>
#include <ur_msgs/action/follow_joint_trajectory_until.hpp>
#include <ur_msgs/action/send_script.hpp>
#include <ur_msgs/action/tool_contact.hpp>
#include <ur_msgs/msg/analog.hpp>
#include <ur_msgs/msg/digital.hpp>
#include <ur_msgs/msg/friction_model_parameters.hpp>
#include <ur_msgs/msg/io_states.hpp>
#include <ur_msgs/msg/masterboard_data_msg.hpp>
#include <ur_msgs/msg/robot_mode_data_msg.hpp>
#include <ur_msgs/msg/robot_state_rt_msg.hpp>
#include <ur_msgs/msg/tool_data_msg.hpp>
#include <ur_msgs/srv/get_robot_software_version.hpp>
#include <ur_msgs/srv/set_analog_output.hpp>
#include <ur_msgs/srv/set_force_mode.hpp>
#include <ur_msgs/srv/set_friction_model_parameters.hpp>
#include <ur_msgs/srv/set_gravity.hpp>
#include <ur_msgs/srv/set_io.hpp>
#include <ur_msgs/srv/set_payload.hpp>
#include <ur_msgs/srv/set_speed_slider_fraction.hpp>
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
if(!::uwvm_industrial_source_95ddc5e034e8b74be3b3054ec6668d5ef43f7290930b40da7eb6d79662a3c7e3::validate())throw std::runtime_error{"native source constants differ"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Duration>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Duration>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::msg::JointTolerance>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::msg::JointTolerance>());
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
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3Stamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3Stamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Wrench>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Wrench>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::trajectory_msgs::msg::JointTrajectory>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::trajectory_msgs::msg::JointTrajectory>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::trajectory_msgs::msg::JointTrajectoryPoint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::trajectory_msgs::msg::JointTrajectoryPoint>());
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::ur_dashboard_msgs::action::SetMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::action::SetMode::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::action::SetMode::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::action::SetMode::Feedback>());
auto a14=rosidl_typesupport_cpp::get_action_type_support_handle<::ur_dashboard_msgs::action::SetMode>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_dashboard_msgs::action::SetMode::Impl::SendGoalService>());if(a14->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_dashboard_msgs::action::SetMode::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_dashboard_msgs::action::SetMode::Impl::GetResultService>());if(a14->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_dashboard_msgs::action::SetMode::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_dashboard_msgs::action::SetMode::Impl::CancelGoalService>());if(a14->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_dashboard_msgs::action::SetMode::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::ur_dashboard_msgs::action::SetMode::Impl::FeedbackMessage>());if(a14->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::ur_dashboard_msgs::action::SetMode::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::ur_dashboard_msgs::action::SetMode::Impl::GoalStatusMessage>());if(a14->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::ur_dashboard_msgs::action::SetMode::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::OperationalMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::OperationalMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::ProgramInformation>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::ProgramInformation>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::ProgramState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::ProgramState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::RobotMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::RobotMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::SafetyMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::SafetyMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::SafetyStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::SafetyStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::UserRole>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::UserRole>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::VersionInformation>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::VersionInformation>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::AddToLog>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::AddToLog::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::AddToLog::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::DownloadProgram>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::DownloadProgram::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::DownloadProgram::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::DownloadSupportFile>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::DownloadSupportFile::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::DownloadSupportFile::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GenerateFlightReport>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GenerateFlightReport::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GenerateFlightReport::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GenerateSupportFile>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GenerateSupportFile::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GenerateSupportFile::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetLoadedProgram>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetLoadedProgram::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetLoadedProgram::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetOperationalMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetOperationalMode::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetOperationalMode::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetPolyScopeVersion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetPolyScopeVersion::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetPolyScopeVersion::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetProgramState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetProgramState::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetProgramState::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetPrograms>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetPrograms::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetPrograms::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetRobotMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetRobotMode::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetRobotMode::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetRobotModel>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetRobotModel::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetRobotModel::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetSafetyMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetSafetyMode::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetSafetyMode::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetSafetyStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetSafetyStatus::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetSafetyStatus::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetSerialNumber>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetSerialNumber::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetSerialNumber::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetUserRole>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetUserRole::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::GetUserRole::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::IsInRemoteControl>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::IsInRemoteControl::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::IsInRemoteControl::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::IsProgramRunning>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::IsProgramRunning::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::IsProgramRunning::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::IsProgramSaved>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::IsProgramSaved::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::IsProgramSaved::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::Load>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::Load::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::Load::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::Popup>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::Popup::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::Popup::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::RawRequest>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::RawRequest::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::RawRequest::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::SetOperationalMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::SetOperationalMode::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::SetOperationalMode::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::SetUserRole>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::SetUserRole::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::SetUserRole::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::UploadProgram>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::UploadProgram::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::srv::UploadProgram::Response>());
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::ur_msgs::action::FollowJointTrajectoryUntil>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::action::FollowJointTrajectoryUntil::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::action::FollowJointTrajectoryUntil::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::action::FollowJointTrajectoryUntil::Feedback>());
auto a48=rosidl_typesupport_cpp::get_action_type_support_handle<::ur_msgs::action::FollowJointTrajectoryUntil>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::FollowJointTrajectoryUntil::Impl::SendGoalService>());if(a48->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::FollowJointTrajectoryUntil::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::FollowJointTrajectoryUntil::Impl::GetResultService>());if(a48->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::FollowJointTrajectoryUntil::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::FollowJointTrajectoryUntil::Impl::CancelGoalService>());if(a48->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::FollowJointTrajectoryUntil::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::ur_msgs::action::FollowJointTrajectoryUntil::Impl::FeedbackMessage>());if(a48->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::ur_msgs::action::FollowJointTrajectoryUntil::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::ur_msgs::action::FollowJointTrajectoryUntil::Impl::GoalStatusMessage>());if(a48->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::ur_msgs::action::FollowJointTrajectoryUntil::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::ur_msgs::action::SendScript>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::action::SendScript::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::action::SendScript::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::action::SendScript::Feedback>());
auto a49=rosidl_typesupport_cpp::get_action_type_support_handle<::ur_msgs::action::SendScript>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::SendScript::Impl::SendGoalService>());if(a49->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::SendScript::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::SendScript::Impl::GetResultService>());if(a49->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::SendScript::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::SendScript::Impl::CancelGoalService>());if(a49->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::SendScript::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::ur_msgs::action::SendScript::Impl::FeedbackMessage>());if(a49->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::ur_msgs::action::SendScript::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::ur_msgs::action::SendScript::Impl::GoalStatusMessage>());if(a49->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::ur_msgs::action::SendScript::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::ur_msgs::action::ToolContact>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::action::ToolContact::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::action::ToolContact::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::action::ToolContact::Feedback>());
auto a50=rosidl_typesupport_cpp::get_action_type_support_handle<::ur_msgs::action::ToolContact>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::ToolContact::Impl::SendGoalService>());if(a50->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::ToolContact::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::ToolContact::Impl::GetResultService>());if(a50->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::ToolContact::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::ToolContact::Impl::CancelGoalService>());if(a50->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::ur_msgs::action::ToolContact::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::ur_msgs::action::ToolContact::Impl::FeedbackMessage>());if(a50->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::ur_msgs::action::ToolContact::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::ur_msgs::action::ToolContact::Impl::GoalStatusMessage>());if(a50->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::ur_msgs::action::ToolContact::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::Analog>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::Analog>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::Digital>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::Digital>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::FrictionModelParameters>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::FrictionModelParameters>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::IOStates>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::IOStates>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::MasterboardDataMsg>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::MasterboardDataMsg>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::RobotModeDataMsg>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::RobotModeDataMsg>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::RobotStateRTMsg>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::RobotStateRTMsg>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::ToolDataMsg>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::ToolDataMsg>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_msgs::srv::GetRobotSoftwareVersion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::srv::GetRobotSoftwareVersion::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::srv::GetRobotSoftwareVersion::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_msgs::srv::SetAnalogOutput>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::srv::SetAnalogOutput::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::srv::SetAnalogOutput::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_msgs::srv::SetForceMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::srv::SetForceMode::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::srv::SetForceMode::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_msgs::srv::SetFrictionModelParameters>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::srv::SetFrictionModelParameters::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::srv::SetFrictionModelParameters::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_msgs::srv::SetGravity>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::srv::SetGravity::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::srv::SetGravity::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_msgs::srv::SetIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::srv::SetIO::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::srv::SetIO::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_msgs::srv::SetPayload>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::srv::SetPayload::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::srv::SetPayload::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_msgs::srv::SetSpeedSliderFraction>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::srv::SetSpeedSliderFraction::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::srv::SetSpeedSliderFraction::Response>());
if(::raise(SIGSTOP)!=0)throw std::runtime_error{"owned mapping stop failed"};
bounded_native_io::gate('I');
bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-industrial-native-v1\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"source_profile\":\"uwvm-ros-vendor-complete-bounded-v1\",\"constant_schema_sha256\":\"95ddc5e034e8b74be3b3054ec6668d5ef43f7290930b40da7eb6d79662a3c7e3\",\"types\":{");
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
quoted("control_msgs/msg/JointTolerance");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::control_msgs::msg::JointTolerance>());
bounded_native_io::print(output(),",\"components\":{");
component<::control_msgs::msg::JointTolerance>("message",false);
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
quoted("std_msgs/msg/Header");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::msg::Header>("message",false);
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
quoted("ur_dashboard_msgs/action/SetMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::ur_dashboard_msgs::action::SetMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::action::SetMode::Goal>("goal",false);
component<::ur_dashboard_msgs::action::SetMode::Result>("result",true);
component<::ur_dashboard_msgs::action::SetMode::Feedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/msg/OperationalMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::OperationalMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::msg::OperationalMode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/msg/ProgramInformation");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::ProgramInformation>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::msg::ProgramInformation>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/msg/ProgramState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::ProgramState>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::msg::ProgramState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/msg/RobotMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::RobotMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::msg::RobotMode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/msg/SafetyMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::SafetyMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::msg::SafetyMode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/msg/SafetyStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::SafetyStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::msg::SafetyStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/msg/UserRole");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::UserRole>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::msg::UserRole>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/msg/VersionInformation");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_dashboard_msgs::msg::VersionInformation>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::msg::VersionInformation>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/AddToLog");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::AddToLog>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::AddToLog::Request>("request",false);
component<::ur_dashboard_msgs::srv::AddToLog::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/DownloadProgram");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::DownloadProgram>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::DownloadProgram::Request>("request",false);
component<::ur_dashboard_msgs::srv::DownloadProgram::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/DownloadSupportFile");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::DownloadSupportFile>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::DownloadSupportFile::Request>("request",false);
component<::ur_dashboard_msgs::srv::DownloadSupportFile::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/GenerateFlightReport");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GenerateFlightReport>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::GenerateFlightReport::Request>("request",false);
component<::ur_dashboard_msgs::srv::GenerateFlightReport::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/GenerateSupportFile");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GenerateSupportFile>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::GenerateSupportFile::Request>("request",false);
component<::ur_dashboard_msgs::srv::GenerateSupportFile::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/GetLoadedProgram");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetLoadedProgram>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::GetLoadedProgram::Request>("request",false);
component<::ur_dashboard_msgs::srv::GetLoadedProgram::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/GetOperationalMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetOperationalMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::GetOperationalMode::Request>("request",false);
component<::ur_dashboard_msgs::srv::GetOperationalMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/GetPolyScopeVersion");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetPolyScopeVersion>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::GetPolyScopeVersion::Request>("request",false);
component<::ur_dashboard_msgs::srv::GetPolyScopeVersion::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/GetProgramState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetProgramState>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::GetProgramState::Request>("request",false);
component<::ur_dashboard_msgs::srv::GetProgramState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/GetPrograms");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetPrograms>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::GetPrograms::Request>("request",false);
component<::ur_dashboard_msgs::srv::GetPrograms::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/GetRobotMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetRobotMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::GetRobotMode::Request>("request",false);
component<::ur_dashboard_msgs::srv::GetRobotMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/GetRobotModel");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetRobotModel>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::GetRobotModel::Request>("request",false);
component<::ur_dashboard_msgs::srv::GetRobotModel::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/GetSafetyMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetSafetyMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::GetSafetyMode::Request>("request",false);
component<::ur_dashboard_msgs::srv::GetSafetyMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/GetSafetyStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetSafetyStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::GetSafetyStatus::Request>("request",false);
component<::ur_dashboard_msgs::srv::GetSafetyStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/GetSerialNumber");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetSerialNumber>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::GetSerialNumber::Request>("request",false);
component<::ur_dashboard_msgs::srv::GetSerialNumber::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/GetUserRole");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::GetUserRole>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::GetUserRole::Request>("request",false);
component<::ur_dashboard_msgs::srv::GetUserRole::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/IsInRemoteControl");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::IsInRemoteControl>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::IsInRemoteControl::Request>("request",false);
component<::ur_dashboard_msgs::srv::IsInRemoteControl::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/IsProgramRunning");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::IsProgramRunning>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::IsProgramRunning::Request>("request",false);
component<::ur_dashboard_msgs::srv::IsProgramRunning::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/IsProgramSaved");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::IsProgramSaved>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::IsProgramSaved::Request>("request",false);
component<::ur_dashboard_msgs::srv::IsProgramSaved::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/Load");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::Load>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::Load::Request>("request",false);
component<::ur_dashboard_msgs::srv::Load::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/Popup");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::Popup>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::Popup::Request>("request",false);
component<::ur_dashboard_msgs::srv::Popup::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/RawRequest");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::RawRequest>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::RawRequest::Request>("request",false);
component<::ur_dashboard_msgs::srv::RawRequest::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/SetOperationalMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::SetOperationalMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::SetOperationalMode::Request>("request",false);
component<::ur_dashboard_msgs::srv::SetOperationalMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/SetUserRole");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::SetUserRole>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::SetUserRole::Request>("request",false);
component<::ur_dashboard_msgs::srv::SetUserRole::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/srv/UploadProgram");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_dashboard_msgs::srv::UploadProgram>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::srv::UploadProgram::Request>("request",false);
component<::ur_dashboard_msgs::srv::UploadProgram::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/action/FollowJointTrajectoryUntil");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::ur_msgs::action::FollowJointTrajectoryUntil>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::action::FollowJointTrajectoryUntil::Goal>("goal",false);
component<::ur_msgs::action::FollowJointTrajectoryUntil::Result>("result",true);
component<::ur_msgs::action::FollowJointTrajectoryUntil::Feedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/action/SendScript");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::ur_msgs::action::SendScript>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::action::SendScript::Goal>("goal",false);
component<::ur_msgs::action::SendScript::Result>("result",true);
component<::ur_msgs::action::SendScript::Feedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/action/ToolContact");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::ur_msgs::action::ToolContact>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::action::ToolContact::Goal>("goal",false);
component<::ur_msgs::action::ToolContact::Result>("result",true);
component<::ur_msgs::action::ToolContact::Feedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/msg/Analog");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::Analog>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::msg::Analog>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/msg/Digital");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::Digital>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::msg::Digital>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/msg/FrictionModelParameters");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::FrictionModelParameters>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::msg::FrictionModelParameters>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/msg/IOStates");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::IOStates>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::msg::IOStates>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/msg/MasterboardDataMsg");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::MasterboardDataMsg>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::msg::MasterboardDataMsg>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/msg/RobotModeDataMsg");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::RobotModeDataMsg>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::msg::RobotModeDataMsg>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/msg/RobotStateRTMsg");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::RobotStateRTMsg>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::msg::RobotStateRTMsg>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/msg/ToolDataMsg");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::ur_msgs::msg::ToolDataMsg>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::msg::ToolDataMsg>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/srv/GetRobotSoftwareVersion");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_msgs::srv::GetRobotSoftwareVersion>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::srv::GetRobotSoftwareVersion::Request>("request",false);
component<::ur_msgs::srv::GetRobotSoftwareVersion::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/srv/SetAnalogOutput");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_msgs::srv::SetAnalogOutput>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::srv::SetAnalogOutput::Request>("request",false);
component<::ur_msgs::srv::SetAnalogOutput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/srv/SetForceMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_msgs::srv::SetForceMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::srv::SetForceMode::Request>("request",false);
component<::ur_msgs::srv::SetForceMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/srv/SetFrictionModelParameters");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_msgs::srv::SetFrictionModelParameters>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::srv::SetFrictionModelParameters::Request>("request",false);
component<::ur_msgs::srv::SetFrictionModelParameters::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/srv/SetGravity");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_msgs::srv::SetGravity>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::srv::SetGravity::Request>("request",false);
component<::ur_msgs::srv::SetGravity::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/srv/SetIO");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_msgs::srv::SetIO>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::srv::SetIO::Request>("request",false);
component<::ur_msgs::srv::SetIO::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/srv/SetPayload");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_msgs::srv::SetPayload>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::srv::SetPayload::Request>("request",false);
component<::ur_msgs::srv::SetPayload::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/srv/SetSpeedSliderFraction");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::ur_msgs::srv::SetSpeedSliderFraction>());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::srv::SetSpeedSliderFraction::Request>("request",false);
component<::ur_msgs::srv::SetSpeedSliderFraction::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),"}}\n");return 0;
}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native industrial metadata refused\n");return 1;}}
