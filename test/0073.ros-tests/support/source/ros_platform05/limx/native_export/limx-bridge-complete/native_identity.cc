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
#include <uwvm_limx_bridge/msg/imu_data.hpp>
#include <uwvm_limx_bridge/msg/joint_cmd.hpp>
#include <uwvm_limx_bridge/msg/joint_cmd_limx.hpp>
#include <uwvm_limx_bridge/msg/joint_state.hpp>
#include <uwvm_limx_bridge/msg/native_header.hpp>
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
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::uwvm_limx_bridge::msg::NativeHeader>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::uwvm_limx_bridge::msg::NativeHeader>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::uwvm_limx_bridge::msg::ImuData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::uwvm_limx_bridge::msg::ImuData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::uwvm_limx_bridge::msg::JointState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::uwvm_limx_bridge::msg::JointState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::uwvm_limx_bridge::msg::JointCmd>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::uwvm_limx_bridge::msg::JointCmd>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::uwvm_limx_bridge::msg::JointCmdLimx>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::uwvm_limx_bridge::msg::JointCmdLimx>());
if(::raise(SIGSTOP)!=0)throw std::runtime_error{"owned mapping stop failed"};
bounded_native_io::gate('I');
bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-industrial-native-v1\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"source_profile\":\"uwvm-ros-vendor-complete-bounded-v1\",\"constant_schema_sha256\":\"ca3d163bab055381827226140568f3bef7eaac187cebd76878e0b63e9e442356\",\"types\":{");
quoted("uwvm_limx_bridge/msg/NativeHeader");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::uwvm_limx_bridge::msg::NativeHeader>());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_limx_bridge::msg::NativeHeader>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_limx_bridge/msg/ImuData");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::uwvm_limx_bridge::msg::ImuData>());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_limx_bridge::msg::ImuData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_limx_bridge/msg/JointState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::uwvm_limx_bridge::msg::JointState>());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_limx_bridge::msg::JointState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_limx_bridge/msg/JointCmd");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::uwvm_limx_bridge::msg::JointCmd>());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_limx_bridge::msg::JointCmd>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_limx_bridge/msg/JointCmdLimx");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::uwvm_limx_bridge::msg::JointCmdLimx>());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_limx_bridge::msg::JointCmdLimx>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),"}}\n");return 0;
}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native industrial metadata refused\n");return 1;}}
