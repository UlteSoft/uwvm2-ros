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
#include <geometry_msgs/msg/quaternion.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/vector3.hpp>
#include <std_msgs/msg/header.hpp>
#include <std_msgs/msg/string.hpp>
#include <urinterfaces/action/set_mode.hpp>
#include <urinterfaces/msg/active_tcp.hpp>
#include <urinterfaces/msg/analog.hpp>
#include <urinterfaces/msg/analog_io.hpp>
#include <urinterfaces/msg/analog_io_state_stamped.hpp>
#include <urinterfaces/msg/analog_stamped.hpp>
#include <urinterfaces/msg/bool_stamped.hpp>
#include <urinterfaces/msg/control_cycle.hpp>
#include <urinterfaces/msg/current.hpp>
#include <urinterfaces/msg/digital.hpp>
#include <urinterfaces/msg/digital_io.hpp>
#include <urinterfaces/msg/digital_io_state_stamped.hpp>
#include <urinterfaces/msg/digital_stamped.hpp>
#include <urinterfaces/msg/double_multi_array_stamped.hpp>
#include <urinterfaces/msg/float32_stamped.hpp>
#include <urinterfaces/msg/float64_stamped.hpp>
#include <urinterfaces/msg/joint_mode.hpp>
#include <urinterfaces/msg/joint_temperature.hpp>
#include <urinterfaces/msg/joint_voltage.hpp>
#include <urinterfaces/msg/json_string_type.hpp>
#include <urinterfaces/msg/robot_mode.hpp>
#include <urinterfaces/msg/runtime_state.hpp>
#include <urinterfaces/msg/safety_status.hpp>
#include <urinterfaces/msg/shared_variables_edit_response.hpp>
#include <urinterfaces/msg/speed_slider_fraction.hpp>
#include <urinterfaces/msg/tcp_state.hpp>
#include <urinterfaces/msg/tool_output_mode.hpp>
#include <urinterfaces/msg/variable_update.hpp>
#include <urinterfaces/msg/voltage_current_stamped.hpp>
#include <urinterfaces/msg/world_model_names.hpp>
#include <urinterfaces/srv/set_analog_output.hpp>
#include <urinterfaces/srv/set_digital_output.hpp>
#include <urinterfaces/srv/set_speed_fraction.hpp>
#include <urinterfaces/srv/shared_variables_editor.hpp>
#include <urinterfaces/srv/variables_viewer.hpp>
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
if(!::uwvm_industrial_source_85ebadcedeb01f49364e039d4ca2cb0f3ea131eaaf280869b12fa0e9aadb1685::validate())throw std::runtime_error{"native source constants differ"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Point>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Point>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Pose>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Pose>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Quaternion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Quaternion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Twist>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Twist>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::String>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::String>());
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::urinterfaces::action::SetMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::action::SetMode::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::action::SetMode::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::action::SetMode::Feedback>());
auto a8=rosidl_typesupport_cpp::get_action_type_support_handle<::urinterfaces::action::SetMode>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::urinterfaces::action::SetMode::Impl::SendGoalService>());if(a8->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::urinterfaces::action::SetMode::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::urinterfaces::action::SetMode::Impl::GetResultService>());if(a8->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::urinterfaces::action::SetMode::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::urinterfaces::action::SetMode::Impl::CancelGoalService>());if(a8->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::urinterfaces::action::SetMode::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::urinterfaces::action::SetMode::Impl::FeedbackMessage>());if(a8->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::urinterfaces::action::SetMode::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::urinterfaces::action::SetMode::Impl::GoalStatusMessage>());if(a8->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::urinterfaces::action::SetMode::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::ActiveTCP>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::ActiveTCP>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::Analog>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::Analog>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::AnalogIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::AnalogIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::AnalogIOStateStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::AnalogIOStateStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::AnalogStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::AnalogStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::BoolStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::BoolStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::ControlCycle>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::ControlCycle>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::Current>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::Current>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::Digital>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::Digital>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::DigitalIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::DigitalIO>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::DigitalIOStateStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::DigitalIOStateStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::DigitalStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::DigitalStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::DoubleMultiArrayStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::DoubleMultiArrayStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::Float32Stamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::Float32Stamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::Float64Stamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::Float64Stamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::JointMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::JointMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::JointTemperature>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::JointTemperature>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::JointVoltage>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::JointVoltage>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::JsonStringType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::JsonStringType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::RobotMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::RobotMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::RuntimeState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::RuntimeState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::SafetyStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::SafetyStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::SharedVariablesEditResponse>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::SharedVariablesEditResponse>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::SpeedSliderFraction>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::SpeedSliderFraction>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::TCPState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::TCPState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::ToolOutputMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::ToolOutputMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::VariableUpdate>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::VariableUpdate>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::VoltageCurrentStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::VoltageCurrentStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::WorldModelNames>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::WorldModelNames>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::urinterfaces::srv::SetAnalogOutput>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::srv::SetAnalogOutput::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::srv::SetAnalogOutput::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::urinterfaces::srv::SetDigitalOutput>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::srv::SetDigitalOutput::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::srv::SetDigitalOutput::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::urinterfaces::srv::SetSpeedFraction>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::srv::SetSpeedFraction::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::srv::SetSpeedFraction::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::urinterfaces::srv::SharedVariablesEditor>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::srv::SharedVariablesEditor::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::srv::SharedVariablesEditor::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::urinterfaces::srv::VariablesViewer>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::srv::VariablesViewer::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::srv::VariablesViewer::Response>());
if(::raise(SIGSTOP)!=0)throw std::runtime_error{"owned mapping stop failed"};
bounded_native_io::gate('I');
bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-industrial-native-v1\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"source_profile\":\"uwvm-ros-vendor-complete-bounded-v1\",\"constant_schema_sha256\":\"85ebadcedeb01f49364e039d4ca2cb0f3ea131eaaf280869b12fa0e9aadb1685\",\"types\":{");
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
quoted("std_msgs/msg/Header");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::msg::Header>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/msg/String");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::String>());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::msg::String>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/action/SetMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::urinterfaces::action::SetMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::action::SetMode::Goal>("goal",false);
component<::urinterfaces::action::SetMode::Result>("result",true);
component<::urinterfaces::action::SetMode::Feedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/ActiveTCP");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::ActiveTCP>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::ActiveTCP>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/Analog");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::Analog>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::Analog>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/AnalogIO");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::AnalogIO>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::AnalogIO>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/AnalogIOStateStamped");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::AnalogIOStateStamped>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::AnalogIOStateStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/AnalogStamped");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::AnalogStamped>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::AnalogStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/BoolStamped");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::BoolStamped>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::BoolStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/ControlCycle");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::ControlCycle>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::ControlCycle>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/Current");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::Current>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::Current>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/Digital");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::Digital>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::Digital>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/DigitalIO");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::DigitalIO>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::DigitalIO>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/DigitalIOStateStamped");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::DigitalIOStateStamped>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::DigitalIOStateStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/DigitalStamped");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::DigitalStamped>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::DigitalStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/DoubleMultiArrayStamped");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::DoubleMultiArrayStamped>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::DoubleMultiArrayStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/Float32Stamped");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::Float32Stamped>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::Float32Stamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/Float64Stamped");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::Float64Stamped>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::Float64Stamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/JointMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::JointMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::JointMode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/JointTemperature");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::JointTemperature>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::JointTemperature>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/JointVoltage");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::JointVoltage>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::JointVoltage>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/JsonStringType");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::JsonStringType>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::JsonStringType>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/RobotMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::RobotMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::RobotMode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/RuntimeState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::RuntimeState>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::RuntimeState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/SafetyStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::SafetyStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::SafetyStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/SharedVariablesEditResponse");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::SharedVariablesEditResponse>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::SharedVariablesEditResponse>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/SpeedSliderFraction");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::SpeedSliderFraction>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::SpeedSliderFraction>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/TCPState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::TCPState>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::TCPState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/ToolOutputMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::ToolOutputMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::ToolOutputMode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/VariableUpdate");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::VariableUpdate>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::VariableUpdate>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/VoltageCurrentStamped");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::VoltageCurrentStamped>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::VoltageCurrentStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/msg/WorldModelNames");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::urinterfaces::msg::WorldModelNames>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::msg::WorldModelNames>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/srv/SetAnalogOutput");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::urinterfaces::srv::SetAnalogOutput>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::srv::SetAnalogOutput::Request>("request",false);
component<::urinterfaces::srv::SetAnalogOutput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/srv/SetDigitalOutput");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::urinterfaces::srv::SetDigitalOutput>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::srv::SetDigitalOutput::Request>("request",false);
component<::urinterfaces::srv::SetDigitalOutput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/srv/SetSpeedFraction");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::urinterfaces::srv::SetSpeedFraction>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::srv::SetSpeedFraction::Request>("request",false);
component<::urinterfaces::srv::SetSpeedFraction::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/srv/SharedVariablesEditor");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::urinterfaces::srv::SharedVariablesEditor>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::srv::SharedVariablesEditor::Request>("request",false);
component<::urinterfaces::srv::SharedVariablesEditor::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("urinterfaces/srv/VariablesViewer");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::urinterfaces::srv::VariablesViewer>());
bounded_native_io::print(output(),",\"components\":{");
component<::urinterfaces::srv::VariablesViewer::Request>("request",false);
component<::urinterfaces::srv::VariablesViewer::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),"}}\n");return 0;
}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native industrial metadata refused\n");return 1;}}
