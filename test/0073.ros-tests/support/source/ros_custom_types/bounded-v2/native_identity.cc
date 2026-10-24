/** @file
 * @brief Export actual official native root/component identity and schema metadata.
 *
 * Build this C++17 tool only after installing the official ROS1 or ROS2 fixture
 * package. It creates no Node, endpoint, capability, service or Action. Redirect
 * stdout to a fresh receipt under the unified acceptance controller's verified
 * cgroup. The output must be pinned together with the tool, installed headers,
 * linked type-support libraries and official generator inputs. Source preparation
 * alone does not establish any identity. ROS1 component MD5 values come directly
 * from generated message_traits, including service-component MD5 conventions.
 */
#include "native_protocol.h"
#include <array>
#include <string_view>
#include <stdexcept>
#include <csignal>
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
#include <uwvm_ros_bounded_v2/Point.h>
#include <uwvm_ros_bounded_v2/Envelope.h>
#include <uwvm_ros_bounded_v2/Echo.h>
#include <uwvm_ros_bounded_v2/Empty.h>
#include <uwvm_ros_bounded_v2/DigestAction.h>
#include <ros/message_traits.h>
#include <ros/service_traits.h>
#elif UWVM_CUSTOM_NATIVE_ROS_VERSION == 2
#include <uwvm_ros_bounded_v2/msg/point.hpp>
#include <uwvm_ros_bounded_v2/msg/envelope.hpp>
#include <uwvm_ros_bounded_v2/srv/echo.hpp>
#include <uwvm_ros_bounded_v2/srv/empty.hpp>
#include <uwvm_ros_bounded_v2/action/digest.hpp>
#include <rosidl_typesupport_cpp/message_type_support.hpp>
#include <rosidl_typesupport_cpp/service_type_support.hpp>
#include <rosidl_typesupport_cpp/action_type_support.hpp>
#else
#error "Select exactly one official native backend"
#endif
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
int main()
{
    try
    {
        if(::raise(SIGSTOP)!=0) {throw std::runtime_error{"exporter mapping stop failed"};}
        bounded_native_io::gate('I');
        bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-official-bounded-native-v2\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"types\":{");
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
        message<uwvm_ros_bounded_v2::Point>("uwvm_ros_bounded_v2/Point",false);message<uwvm_ros_bounded_v2::Envelope>("uwvm_ros_bounded_v2/Envelope",true);
        service<uwvm_ros_bounded_v2::Echo>("uwvm_ros_bounded_v2/Echo");service<uwvm_ros_bounded_v2::Empty>("uwvm_ros_bounded_v2/Empty");
        bounded_native_io::print(output(),',');quoted("uwvm_ros_bounded_v2/Digest");bounded_native_io::print(output(),":{\"native_identity\":");quoted(::ros::message_traits::MD5Sum<uwvm_ros_bounded_v2::DigestAction>::value());bounded_native_io::print(output(),",\"components\":{");component<uwvm_ros_bounded_v2::DigestGoal>("goal",false);component<uwvm_ros_bounded_v2::DigestResult>("result",true);component<uwvm_ros_bounded_v2::DigestFeedback>("feedback",true);bounded_native_io::print(output(),"}}");
#else
        message<uwvm_ros_bounded_v2::msg::Point>("uwvm_ros_bounded_v2/msg/Point",false);message<uwvm_ros_bounded_v2::msg::Envelope>("uwvm_ros_bounded_v2/msg/Envelope",true);
        service<uwvm_ros_bounded_v2::srv::Echo>("uwvm_ros_bounded_v2/srv/Echo");service<uwvm_ros_bounded_v2::srv::Empty>("uwvm_ros_bounded_v2/srv/Empty");
        bounded_native_io::print(output(),',');quoted("uwvm_ros_bounded_v2/action/Digest");bounded_native_io::print(output(),":{\"official\":");metadata(rosidl_typesupport_cpp::get_action_type_support_handle<uwvm_ros_bounded_v2::action::Digest>());bounded_native_io::print(output(),",\"components\":{");component<uwvm_ros_bounded_v2::action::Digest::Goal>("goal",false);component<uwvm_ros_bounded_v2::action::Digest::Result>("result",true);component<uwvm_ros_bounded_v2::action::Digest::Feedback>("feedback",true);bounded_native_io::print(output(),"}}");
#endif
        bounded_native_io::print(output(),"}}\n");return 0;
    }
    catch(std::exception const& error) {bounded_native_io::print(STDERR_FILENO,"native identity export refused\n");return 1;}
}
