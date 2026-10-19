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
#include <mecharm_interfaces/msg/mecharm_angles.hpp>
#include <mecharm_interfaces/msg/mecharm_coords.hpp>
#include <mecharm_interfaces/msg/mecharm_gripper_status.hpp>
#include <mecharm_interfaces/msg/mecharm_pump_status.hpp>
#include <mecharm_interfaces/msg/mecharm_set_angles.hpp>
#include <mecharm_interfaces/msg/mecharm_set_coords.hpp>
#include <mecharm_interfaces/srv/get_angles.hpp>
#include <mecharm_interfaces/srv/get_coords.hpp>
#include <mecharm_interfaces/srv/gripper_status.hpp>
#include <mecharm_interfaces/srv/pump_status.hpp>
#include <mecharm_interfaces/srv/set_angles.hpp>
#include <mecharm_interfaces/srv/set_coords.hpp>
#include <mycobot_interfaces/msg/mycobot_angles.hpp>
#include <mycobot_interfaces/msg/mycobot_coords.hpp>
#include <mycobot_interfaces/msg/mycobot_gripper_status.hpp>
#include <mycobot_interfaces/msg/mycobot_pump_status.hpp>
#include <mycobot_interfaces/msg/mycobot_set_angles.hpp>
#include <mycobot_interfaces/msg/mycobot_set_coords.hpp>
#include <mycobot_interfaces/srv/get_angles.hpp>
#include <mycobot_interfaces/srv/get_coords.hpp>
#include <mycobot_interfaces/srv/gripper_status.hpp>
#include <mycobot_interfaces/srv/pump_status.hpp>
#include <mycobot_interfaces/srv/set_angles.hpp>
#include <mycobot_interfaces/srv/set_coords.hpp>
#include <mycobot_pro450_interfaces/msg/mycobot_angles.hpp>
#include <mycobot_pro450_interfaces/msg/mycobot_coords.hpp>
#include <mycobot_pro450_interfaces/msg/mycobot_gripper_status.hpp>
#include <mycobot_pro450_interfaces/msg/mycobot_set_angles.hpp>
#include <mycobot_pro450_interfaces/msg/mycobot_set_coords.hpp>
#include <mycobot_pro450_interfaces/srv/get_angles.hpp>
#include <mycobot_pro450_interfaces/srv/get_coords.hpp>
#include <mycobot_pro450_interfaces/srv/get_gripper_value.hpp>
#include <mycobot_pro450_interfaces/srv/gripper_status.hpp>
#include <mycobot_pro450_interfaces/srv/set_angles.hpp>
#include <mycobot_pro450_interfaces/srv/set_coords.hpp>
#include <mypalletizer_interfaces/msg/mypal_angles.hpp>
#include <mypalletizer_interfaces/msg/mypal_coords.hpp>
#include <mypalletizer_interfaces/msg/mypal_gripper_status.hpp>
#include <mypalletizer_interfaces/msg/mypal_pump_status.hpp>
#include <mypalletizer_interfaces/msg/mypal_set_angles.hpp>
#include <mypalletizer_interfaces/msg/mypal_set_coords.hpp>
#include <mypalletizer_interfaces/srv/get_angles.hpp>
#include <mypalletizer_interfaces/srv/get_coords.hpp>
#include <mypalletizer_interfaces/srv/gripper_status.hpp>
#include <mypalletizer_interfaces/srv/pump_status.hpp>
#include <mypalletizer_interfaces/srv/set_angles.hpp>
#include <mypalletizer_interfaces/srv/set_coords.hpp>
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
if(!::uwvm_industrial_source_ca3d163bab055381827226140568f3bef7eaac187cebd76878e0b63e9e442356::validate())throw std::runtime_error{"native source constants differ"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmGripperStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmGripperStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmPumpStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmPumpStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmSetAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmSetAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmSetCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmSetCoords>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mecharm_interfaces::srv::GetAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::srv::GetAngles::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::srv::GetAngles::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mecharm_interfaces::srv::GetCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::srv::GetCoords::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::srv::GetCoords::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mecharm_interfaces::srv::GripperStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::srv::GripperStatus::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::srv::GripperStatus::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mecharm_interfaces::srv::PumpStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::srv::PumpStatus::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::srv::PumpStatus::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mecharm_interfaces::srv::SetAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::srv::SetAngles::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::srv::SetAngles::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mecharm_interfaces::srv::SetCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::srv::SetCoords::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::srv::SetCoords::Response>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotGripperStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotGripperStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotPumpStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotPumpStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotSetAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotSetAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotSetCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotSetCoords>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_interfaces::srv::GetAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::srv::GetAngles::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::srv::GetAngles::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_interfaces::srv::GetCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::srv::GetCoords::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::srv::GetCoords::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_interfaces::srv::GripperStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::srv::GripperStatus::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::srv::GripperStatus::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_interfaces::srv::PumpStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::srv::PumpStatus::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::srv::PumpStatus::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_interfaces::srv::SetAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::srv::SetAngles::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::srv::SetAngles::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_interfaces::srv::SetCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::srv::SetCoords::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::srv::SetCoords::Response>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::msg::MycobotAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::msg::MycobotAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::msg::MycobotCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::msg::MycobotCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::msg::MycobotGripperStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::msg::MycobotGripperStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::msg::MycobotSetAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::msg::MycobotSetAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::msg::MycobotSetCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::msg::MycobotSetCoords>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_pro450_interfaces::srv::GetAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::srv::GetAngles::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::srv::GetAngles::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_pro450_interfaces::srv::GetCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::srv::GetCoords::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::srv::GetCoords::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_pro450_interfaces::srv::GetGripperValue>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::srv::GetGripperValue::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::srv::GetGripperValue::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_pro450_interfaces::srv::GripperStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::srv::GripperStatus::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::srv::GripperStatus::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_pro450_interfaces::srv::SetAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::srv::SetAngles::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::srv::SetAngles::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_pro450_interfaces::srv::SetCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::srv::SetCoords::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::srv::SetCoords::Response>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalGripperStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalGripperStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalPumpStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalPumpStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalSetAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalSetAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalSetCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalSetCoords>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mypalletizer_interfaces::srv::GetAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::srv::GetAngles::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::srv::GetAngles::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mypalletizer_interfaces::srv::GetCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::srv::GetCoords::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::srv::GetCoords::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mypalletizer_interfaces::srv::GripperStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::srv::GripperStatus::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::srv::GripperStatus::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mypalletizer_interfaces::srv::PumpStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::srv::PumpStatus::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::srv::PumpStatus::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mypalletizer_interfaces::srv::SetAngles>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::srv::SetAngles::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::srv::SetAngles::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::mypalletizer_interfaces::srv::SetCoords>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::srv::SetCoords::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::srv::SetCoords::Response>());
if(::raise(SIGSTOP)!=0)throw std::runtime_error{"owned mapping stop failed"};
bounded_native_io::gate('I');
bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-industrial-native-v1\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"source_profile\":\"uwvm-ros-vendor-complete-bounded-v1\",\"constant_schema_sha256\":\"ca3d163bab055381827226140568f3bef7eaac187cebd76878e0b63e9e442356\",\"types\":{");
quoted("mecharm_interfaces/msg/MecharmAngles");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmAngles>());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_interfaces::msg::MecharmAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_interfaces/msg/MecharmCoords");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmCoords>());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_interfaces::msg::MecharmCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_interfaces/msg/MecharmGripperStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmGripperStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_interfaces::msg::MecharmGripperStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_interfaces/msg/MecharmPumpStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmPumpStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_interfaces::msg::MecharmPumpStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_interfaces/msg/MecharmSetAngles");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmSetAngles>());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_interfaces::msg::MecharmSetAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_interfaces/msg/MecharmSetCoords");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mecharm_interfaces::msg::MecharmSetCoords>());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_interfaces::msg::MecharmSetCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_interfaces/srv/GetAngles");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mecharm_interfaces::srv::GetAngles>());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_interfaces::srv::GetAngles::Request>("request",false);
component<::mecharm_interfaces::srv::GetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_interfaces/srv/GetCoords");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mecharm_interfaces::srv::GetCoords>());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_interfaces::srv::GetCoords::Request>("request",false);
component<::mecharm_interfaces::srv::GetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_interfaces/srv/GripperStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mecharm_interfaces::srv::GripperStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_interfaces::srv::GripperStatus::Request>("request",false);
component<::mecharm_interfaces::srv::GripperStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_interfaces/srv/PumpStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mecharm_interfaces::srv::PumpStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_interfaces::srv::PumpStatus::Request>("request",false);
component<::mecharm_interfaces::srv::PumpStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_interfaces/srv/SetAngles");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mecharm_interfaces::srv::SetAngles>());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_interfaces::srv::SetAngles::Request>("request",false);
component<::mecharm_interfaces::srv::SetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_interfaces/srv/SetCoords");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mecharm_interfaces::srv::SetCoords>());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_interfaces::srv::SetCoords::Request>("request",false);
component<::mecharm_interfaces::srv::SetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_interfaces/msg/MycobotAngles");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotAngles>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_interfaces::msg::MycobotAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_interfaces/msg/MycobotCoords");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotCoords>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_interfaces::msg::MycobotCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_interfaces/msg/MycobotGripperStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotGripperStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_interfaces::msg::MycobotGripperStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_interfaces/msg/MycobotPumpStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotPumpStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_interfaces::msg::MycobotPumpStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_interfaces/msg/MycobotSetAngles");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotSetAngles>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_interfaces::msg::MycobotSetAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_interfaces/msg/MycobotSetCoords");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_interfaces::msg::MycobotSetCoords>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_interfaces::msg::MycobotSetCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_interfaces/srv/GetAngles");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_interfaces::srv::GetAngles>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_interfaces::srv::GetAngles::Request>("request",false);
component<::mycobot_interfaces::srv::GetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_interfaces/srv/GetCoords");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_interfaces::srv::GetCoords>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_interfaces::srv::GetCoords::Request>("request",false);
component<::mycobot_interfaces::srv::GetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_interfaces/srv/GripperStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_interfaces::srv::GripperStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_interfaces::srv::GripperStatus::Request>("request",false);
component<::mycobot_interfaces::srv::GripperStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_interfaces/srv/PumpStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_interfaces::srv::PumpStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_interfaces::srv::PumpStatus::Request>("request",false);
component<::mycobot_interfaces::srv::PumpStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_interfaces/srv/SetAngles");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_interfaces::srv::SetAngles>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_interfaces::srv::SetAngles::Request>("request",false);
component<::mycobot_interfaces::srv::SetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_interfaces/srv/SetCoords");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_interfaces::srv::SetCoords>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_interfaces::srv::SetCoords::Request>("request",false);
component<::mycobot_interfaces::srv::SetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_interfaces/msg/MycobotAngles");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::msg::MycobotAngles>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_interfaces::msg::MycobotAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_interfaces/msg/MycobotCoords");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::msg::MycobotCoords>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_interfaces::msg::MycobotCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_interfaces/msg/MycobotGripperStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::msg::MycobotGripperStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_interfaces::msg::MycobotGripperStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_interfaces/msg/MycobotSetAngles");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::msg::MycobotSetAngles>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_interfaces::msg::MycobotSetAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_interfaces/msg/MycobotSetCoords");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mycobot_pro450_interfaces::msg::MycobotSetCoords>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_interfaces::msg::MycobotSetCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_interfaces/srv/GetAngles");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_pro450_interfaces::srv::GetAngles>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_interfaces::srv::GetAngles::Request>("request",false);
component<::mycobot_pro450_interfaces::srv::GetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_interfaces/srv/GetCoords");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_pro450_interfaces::srv::GetCoords>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_interfaces::srv::GetCoords::Request>("request",false);
component<::mycobot_pro450_interfaces::srv::GetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_interfaces/srv/GetGripperValue");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_pro450_interfaces::srv::GetGripperValue>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_interfaces::srv::GetGripperValue::Request>("request",false);
component<::mycobot_pro450_interfaces::srv::GetGripperValue::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_interfaces/srv/GripperStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_pro450_interfaces::srv::GripperStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_interfaces::srv::GripperStatus::Request>("request",false);
component<::mycobot_pro450_interfaces::srv::GripperStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_interfaces/srv/SetAngles");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_pro450_interfaces::srv::SetAngles>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_interfaces::srv::SetAngles::Request>("request",false);
component<::mycobot_pro450_interfaces::srv::SetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_interfaces/srv/SetCoords");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mycobot_pro450_interfaces::srv::SetCoords>());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_interfaces::srv::SetCoords::Request>("request",false);
component<::mycobot_pro450_interfaces::srv::SetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_interfaces/msg/MypalAngles");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalAngles>());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_interfaces::msg::MypalAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_interfaces/msg/MypalCoords");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalCoords>());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_interfaces::msg::MypalCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_interfaces/msg/MypalGripperStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalGripperStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_interfaces::msg::MypalGripperStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_interfaces/msg/MypalPumpStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalPumpStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_interfaces::msg::MypalPumpStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_interfaces/msg/MypalSetAngles");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalSetAngles>());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_interfaces::msg::MypalSetAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_interfaces/msg/MypalSetCoords");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::mypalletizer_interfaces::msg::MypalSetCoords>());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_interfaces::msg::MypalSetCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_interfaces/srv/GetAngles");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mypalletizer_interfaces::srv::GetAngles>());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_interfaces::srv::GetAngles::Request>("request",false);
component<::mypalletizer_interfaces::srv::GetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_interfaces/srv/GetCoords");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mypalletizer_interfaces::srv::GetCoords>());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_interfaces::srv::GetCoords::Request>("request",false);
component<::mypalletizer_interfaces::srv::GetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_interfaces/srv/GripperStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mypalletizer_interfaces::srv::GripperStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_interfaces::srv::GripperStatus::Request>("request",false);
component<::mypalletizer_interfaces::srv::GripperStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_interfaces/srv/PumpStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mypalletizer_interfaces::srv::PumpStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_interfaces::srv::PumpStatus::Request>("request",false);
component<::mypalletizer_interfaces::srv::PumpStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_interfaces/srv/SetAngles");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mypalletizer_interfaces::srv::SetAngles>());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_interfaces::srv::SetAngles::Request>("request",false);
component<::mypalletizer_interfaces::srv::SetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_interfaces/srv/SetCoords");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::mypalletizer_interfaces::srv::SetCoords>());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_interfaces::srv::SetCoords::Request>("request",false);
component<::mypalletizer_interfaces::srv::SetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),"}}\n");return 0;
}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native industrial metadata refused\n");return 1;}}
