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
#include <clearpath_control_msgs/msg/device_info.hpp>
#include <clearpath_control_msgs/msg/stream_control.hpp>
#include <clearpath_control_msgs/srv/power_control.hpp>
#include <clearpath_motor_msgs/action/lynx_calibrate.hpp>
#include <clearpath_motor_msgs/action/lynx_update.hpp>
#include <clearpath_motor_msgs/msg/lynx_debug.hpp>
#include <clearpath_motor_msgs/msg/lynx_feedback.hpp>
#include <clearpath_motor_msgs/msg/lynx_motor_protection.hpp>
#include <clearpath_motor_msgs/msg/lynx_multi_debug.hpp>
#include <clearpath_motor_msgs/msg/lynx_multi_feedback.hpp>
#include <clearpath_motor_msgs/msg/lynx_multi_status.hpp>
#include <clearpath_motor_msgs/msg/lynx_status.hpp>
#include <clearpath_motor_msgs/msg/lynx_system_protection.hpp>
#include <clearpath_motor_msgs/msg/puma_feedback.hpp>
#include <clearpath_motor_msgs/msg/puma_multi_feedback.hpp>
#include <clearpath_motor_msgs/msg/puma_multi_status.hpp>
#include <clearpath_motor_msgs/msg/puma_status.hpp>
#include <clearpath_platform_msgs/msg/display_status.hpp>
#include <clearpath_platform_msgs/msg/drive.hpp>
#include <clearpath_platform_msgs/msg/drive_feedback.hpp>
#include <clearpath_platform_msgs/msg/fans.hpp>
#include <clearpath_platform_msgs/msg/feedback.hpp>
#include <clearpath_platform_msgs/msg/lights.hpp>
#include <clearpath_platform_msgs/msg/pinout_command.hpp>
#include <clearpath_platform_msgs/msg/pinout_state.hpp>
#include <clearpath_platform_msgs/msg/power.hpp>
#include <clearpath_platform_msgs/msg/rgb.hpp>
#include <clearpath_platform_msgs/msg/status.hpp>
#include <clearpath_platform_msgs/msg/stop_status.hpp>
#include <clearpath_platform_msgs/msg/temperature.hpp>
#include <clearpath_platform_msgs/srv/configure_mcu.hpp>
#include <clearpath_platform_msgs/srv/set_pinout.hpp>
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
if(!::uwvm_industrial_source_262469ae5a140e193f2d8dfa9c71d8bc5038a3c751e3e5bbd45c572b69d2dcc4::validate())throw std::runtime_error{"native source constants differ"};
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::clearpath_motor_msgs::action::LynxCalibrate>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::action::LynxCalibrate::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::action::LynxCalibrate::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::action::LynxCalibrate::Feedback>());
auto a0=rosidl_typesupport_cpp::get_action_type_support_handle<::clearpath_motor_msgs::action::LynxCalibrate>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::clearpath_motor_msgs::action::LynxCalibrate::Impl::SendGoalService>());if(a0->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::clearpath_motor_msgs::action::LynxCalibrate::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::clearpath_motor_msgs::action::LynxCalibrate::Impl::GetResultService>());if(a0->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::clearpath_motor_msgs::action::LynxCalibrate::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::clearpath_motor_msgs::action::LynxCalibrate::Impl::CancelGoalService>());if(a0->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::clearpath_motor_msgs::action::LynxCalibrate::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::clearpath_motor_msgs::action::LynxCalibrate::Impl::FeedbackMessage>());if(a0->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::clearpath_motor_msgs::action::LynxCalibrate::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::clearpath_motor_msgs::action::LynxCalibrate::Impl::GoalStatusMessage>());if(a0->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::clearpath_motor_msgs::action::LynxCalibrate::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::clearpath_motor_msgs::action::LynxUpdate>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::action::LynxUpdate::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::action::LynxUpdate::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::action::LynxUpdate::Feedback>());
auto a1=rosidl_typesupport_cpp::get_action_type_support_handle<::clearpath_motor_msgs::action::LynxUpdate>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::clearpath_motor_msgs::action::LynxUpdate::Impl::SendGoalService>());if(a1->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::clearpath_motor_msgs::action::LynxUpdate::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::clearpath_motor_msgs::action::LynxUpdate::Impl::GetResultService>());if(a1->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::clearpath_motor_msgs::action::LynxUpdate::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::clearpath_motor_msgs::action::LynxUpdate::Impl::CancelGoalService>());if(a1->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::clearpath_motor_msgs::action::LynxUpdate::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::clearpath_motor_msgs::action::LynxUpdate::Impl::FeedbackMessage>());if(a1->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::clearpath_motor_msgs::action::LynxUpdate::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::clearpath_motor_msgs::action::LynxUpdate::Impl::GoalStatusMessage>());if(a1->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::clearpath_motor_msgs::action::LynxUpdate::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Duration>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Duration>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_control_msgs::msg::DeviceInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_control_msgs::msg::DeviceInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_control_msgs::msg::StreamControl>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_control_msgs::msg::StreamControl>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxDebug>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxDebug>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxFeedback>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxFeedback>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxMotorProtection>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxMotorProtection>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxMultiDebug>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxMultiDebug>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxMultiFeedback>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxMultiFeedback>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxMultiStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxMultiStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxSystemProtection>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxSystemProtection>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::PumaFeedback>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::PumaFeedback>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::PumaMultiFeedback>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::PumaMultiFeedback>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::PumaMultiStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::PumaMultiStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::PumaStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::PumaStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::DisplayStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::DisplayStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Drive>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Drive>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::DriveFeedback>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::DriveFeedback>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Fans>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Fans>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Feedback>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Feedback>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Lights>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Lights>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::PinoutCommand>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::PinoutCommand>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::PinoutState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::PinoutState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Power>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Power>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::RGB>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::RGB>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Status>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Status>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::StopStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::StopStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Temperature>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Temperature>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::clearpath_control_msgs::srv::PowerControl>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_control_msgs::srv::PowerControl::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_control_msgs::srv::PowerControl::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::clearpath_platform_msgs::srv::ConfigureMcu>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::srv::ConfigureMcu::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::srv::ConfigureMcu::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::clearpath_platform_msgs::srv::SetPinout>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::srv::SetPinout::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::srv::SetPinout::Response>());
if(::raise(SIGSTOP)!=0)throw std::runtime_error{"owned mapping stop failed"};
bounded_native_io::gate('I');
bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-industrial-native-v1\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"source_profile\":\"uwvm-ros-vendor-complete-bounded-v1\",\"constant_schema_sha256\":\"262469ae5a140e193f2d8dfa9c71d8bc5038a3c751e3e5bbd45c572b69d2dcc4\",\"types\":{");
quoted("clearpath_motor_msgs/action/LynxCalibrate");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::clearpath_motor_msgs::action::LynxCalibrate>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_motor_msgs::action::LynxCalibrate::Goal>("goal",false);
component<::clearpath_motor_msgs::action::LynxCalibrate::Result>("result",true);
component<::clearpath_motor_msgs::action::LynxCalibrate::Feedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_motor_msgs/action/LynxUpdate");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::clearpath_motor_msgs::action::LynxUpdate>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_motor_msgs::action::LynxUpdate::Goal>("goal",false);
component<::clearpath_motor_msgs::action::LynxUpdate::Result>("result",true);
component<::clearpath_motor_msgs::action::LynxUpdate::Feedback>("feedback",true);
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
quoted("clearpath_control_msgs/msg/DeviceInfo");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_control_msgs::msg::DeviceInfo>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_control_msgs::msg::DeviceInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_control_msgs/msg/StreamControl");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_control_msgs::msg::StreamControl>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_control_msgs::msg::StreamControl>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_motor_msgs/msg/LynxDebug");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxDebug>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_motor_msgs::msg::LynxDebug>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_motor_msgs/msg/LynxFeedback");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxFeedback>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_motor_msgs::msg::LynxFeedback>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_motor_msgs/msg/LynxMotorProtection");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxMotorProtection>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_motor_msgs::msg::LynxMotorProtection>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_motor_msgs/msg/LynxMultiDebug");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxMultiDebug>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_motor_msgs::msg::LynxMultiDebug>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_motor_msgs/msg/LynxMultiFeedback");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxMultiFeedback>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_motor_msgs::msg::LynxMultiFeedback>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_motor_msgs/msg/LynxMultiStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxMultiStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_motor_msgs::msg::LynxMultiStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_motor_msgs/msg/LynxStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_motor_msgs::msg::LynxStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_motor_msgs/msg/LynxSystemProtection");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::LynxSystemProtection>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_motor_msgs::msg::LynxSystemProtection>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_motor_msgs/msg/PumaFeedback");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::PumaFeedback>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_motor_msgs::msg::PumaFeedback>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_motor_msgs/msg/PumaMultiFeedback");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::PumaMultiFeedback>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_motor_msgs::msg::PumaMultiFeedback>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_motor_msgs/msg/PumaMultiStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::PumaMultiStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_motor_msgs::msg::PumaMultiStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_motor_msgs/msg/PumaStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_motor_msgs::msg::PumaStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_motor_msgs::msg::PumaStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_platform_msgs/msg/DisplayStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::DisplayStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_platform_msgs::msg::DisplayStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_platform_msgs/msg/Drive");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Drive>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_platform_msgs::msg::Drive>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_platform_msgs/msg/DriveFeedback");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::DriveFeedback>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_platform_msgs::msg::DriveFeedback>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_platform_msgs/msg/Fans");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Fans>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_platform_msgs::msg::Fans>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_platform_msgs/msg/Feedback");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Feedback>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_platform_msgs::msg::Feedback>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_platform_msgs/msg/Lights");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Lights>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_platform_msgs::msg::Lights>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_platform_msgs/msg/PinoutCommand");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::PinoutCommand>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_platform_msgs::msg::PinoutCommand>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_platform_msgs/msg/PinoutState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::PinoutState>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_platform_msgs::msg::PinoutState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_platform_msgs/msg/Power");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Power>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_platform_msgs::msg::Power>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_platform_msgs/msg/RGB");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::RGB>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_platform_msgs::msg::RGB>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_platform_msgs/msg/Status");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Status>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_platform_msgs::msg::Status>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_platform_msgs/msg/StopStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::StopStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_platform_msgs::msg::StopStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_platform_msgs/msg/Temperature");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::clearpath_platform_msgs::msg::Temperature>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_platform_msgs::msg::Temperature>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/msg/Header");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::msg::Header>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_control_msgs/srv/PowerControl");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::clearpath_control_msgs::srv::PowerControl>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_control_msgs::srv::PowerControl::Request>("request",false);
component<::clearpath_control_msgs::srv::PowerControl::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_platform_msgs/srv/ConfigureMcu");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::clearpath_platform_msgs::srv::ConfigureMcu>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_platform_msgs::srv::ConfigureMcu::Request>("request",false);
component<::clearpath_platform_msgs::srv::ConfigureMcu::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("clearpath_platform_msgs/srv/SetPinout");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::clearpath_platform_msgs::srv::SetPinout>());
bounded_native_io::print(output(),",\"components\":{");
component<::clearpath_platform_msgs::srv::SetPinout::Request>("request",false);
component<::clearpath_platform_msgs::srv::SetPinout::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),"}}\n");return 0;
}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native industrial metadata refused\n");return 1;}}
