/** Industrial native metadata source; no Node, endpoint or grant. */
#include "tests/ros_custom_types/bounded-v2/native_protocol.h"
#include <array>
#include <string_view>
#include <stdexcept>
#include <csignal>
#include <cstdint>
#if UWVM_CUSTOM_NATIVE_ROS_VERSION != 1
#error "Select the exact source-reviewed ROS major"
#endif
#include <mecharm_communication/GetAngles.h>
#include <mecharm_communication/GetCoords.h>
#include <mecharm_communication/GripperStatus.h>
#include <mecharm_communication/MycobotAngles.h>
#include <mecharm_communication/MycobotCoords.h>
#include <mecharm_communication/MycobotGripperStatus.h>
#include <mecharm_communication/MycobotPumpStatus.h>
#include <mecharm_communication/MycobotSetAngles.h>
#include <mecharm_communication/MycobotSetCoords.h>
#include <mecharm_communication/PumpStatus.h>
#include <mecharm_communication/SetAngles.h>
#include <mecharm_communication/SetCoords.h>
#include <myarm_communication/GetAngles.h>
#include <myarm_communication/GetCoords.h>
#include <myarm_communication/GripperStatus.h>
#include <myarm_communication/MyArmAngles.h>
#include <myarm_communication/MyArmCoords.h>
#include <myarm_communication/MyArmGripperStatus.h>
#include <myarm_communication/MyArmPumpStatus.h>
#include <myarm_communication/MyArmSetAngles.h>
#include <myarm_communication/MyArmSetCoords.h>
#include <myarm_communication/PumpStatus.h>
#include <myarm_communication/SetAngles.h>
#include <myarm_communication/SetCoords.h>
#include <mybuddy_communication/GetAngles.h>
#include <mybuddy_communication/GetCoords.h>
#include <mybuddy_communication/GripperStatus.h>
#include <mybuddy_communication/MybuddyAngles.h>
#include <mybuddy_communication/MybuddyCoords.h>
#include <mybuddy_communication/MybuddyGripperStatus.h>
#include <mybuddy_communication/MybuddyPumpStatus.h>
#include <mybuddy_communication/MybuddySetAngles.h>
#include <mybuddy_communication/MybuddySetCoords.h>
#include <mybuddy_communication/PumpStatus.h>
#include <mybuddy_communication/SetAngles.h>
#include <mybuddy_communication/SetCoords.h>
#include <mycobot_320_communication/GetAngles.h>
#include <mycobot_320_communication/GetCoords.h>
#include <mycobot_320_communication/GripperStatus.h>
#include <mycobot_320_communication/MycobotAngles.h>
#include <mycobot_320_communication/MycobotCoords.h>
#include <mycobot_320_communication/MycobotGripperStatus.h>
#include <mycobot_320_communication/MycobotPumpStatus.h>
#include <mycobot_320_communication/MycobotSetAngles.h>
#include <mycobot_320_communication/MycobotSetCoords.h>
#include <mycobot_320_communication/PumpStatus.h>
#include <mycobot_320_communication/SetAngles.h>
#include <mycobot_320_communication/SetCoords.h>
#include <mycobot_communication/GetAngles.h>
#include <mycobot_communication/GetCoords.h>
#include <mycobot_communication/GripperStatus.h>
#include <mycobot_communication/MycobotAngles.h>
#include <mycobot_communication/MycobotCoords.h>
#include <mycobot_communication/MycobotGetGripperValue.h>
#include <mycobot_communication/MycobotGripperStatus.h>
#include <mycobot_communication/MycobotPumpStatus.h>
#include <mycobot_communication/MycobotSetAngles.h>
#include <mycobot_communication/MycobotSetCoords.h>
#include <mycobot_communication/MycobotSetEndType.h>
#include <mycobot_communication/MycobotSetFreshMode.h>
#include <mycobot_communication/MycobotSetToolReference.h>
#include <mycobot_communication/MycobotSetVisionMode.h>
#include <mycobot_communication/PumpStatus.h>
#include <mycobot_communication/SetAngles.h>
#include <mycobot_communication/SetCoords.h>
#include <mycobot_communication/SetEndType.h>
#include <mycobot_communication/SetFreshMode.h>
#include <mycobot_communication/SetToolReference.h>
#include <mycobot_communication/SetVisionMode.h>
#include <mycobot_pro450_communication/GetAngles.h>
#include <mycobot_pro450_communication/GetCoords.h>
#include <mycobot_pro450_communication/GetGripperValue.h>
#include <mycobot_pro450_communication/GripperStatus.h>
#include <mycobot_pro450_communication/MycobotAngles.h>
#include <mycobot_pro450_communication/MycobotCoords.h>
#include <mycobot_pro450_communication/MycobotGetGripperValue.h>
#include <mycobot_pro450_communication/MycobotGripperStatus.h>
#include <mycobot_pro450_communication/MycobotSetAngles.h>
#include <mycobot_pro450_communication/MycobotSetCoords.h>
#include <mycobot_pro450_communication/MycobotSetFreshMode.h>
#include <mycobot_pro450_communication/SetAngles.h>
#include <mycobot_pro450_communication/SetCoords.h>
#include <mycobot_pro450_communication/SetFreshMode.h>
#include <mypalletizer_communication/GetAngles.h>
#include <mypalletizer_communication/GetCoords.h>
#include <mypalletizer_communication/GripperStatus.h>
#include <mypalletizer_communication/MypalAngles.h>
#include <mypalletizer_communication/MypalCoords.h>
#include <mypalletizer_communication/MypalGripperStatus.h>
#include <mypalletizer_communication/MypalPumpStatus.h>
#include <mypalletizer_communication/MypalSetAngles.h>
#include <mypalletizer_communication/MypalSetCoords.h>
#include <mypalletizer_communication/PumpStatus.h>
#include <mypalletizer_communication/SetAngles.h>
#include <mypalletizer_communication/SetCoords.h>
#include <ultraarm_communication/GetAngles.h>
#include <ultraarm_communication/GetCoords.h>
#include <ultraarm_communication/GetGripperValue.h>
#include <ultraarm_communication/GripperStatus.h>
#include <ultraarm_communication/MycobotAngles.h>
#include <ultraarm_communication/MycobotCoords.h>
#include <ultraarm_communication/MycobotGetGripperValue.h>
#include <ultraarm_communication/MycobotGripperStatus.h>
#include <ultraarm_communication/MycobotSetAngles.h>
#include <ultraarm_communication/MycobotSetCoords.h>
#include <ultraarm_communication/MycobotSetFreshMode.h>
#include <ultraarm_communication/SetAngles.h>
#include <ultraarm_communication/SetCoords.h>
#include <ultraarm_communication/SetFreshMode.h>
#include <ros/message_traits.h>
#include <ros/service_traits.h>
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
int main() {try {
if(!::uwvm_industrial_source_ca3d163bab055381827226140568f3bef7eaac187cebd76878e0b63e9e442356::validate())throw std::runtime_error{"native source constants differ"};
if(::raise(SIGSTOP)!=0)throw std::runtime_error{"owned mapping stop failed"};
bounded_native_io::gate('I');
bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-industrial-native-v1\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"source_profile\":\"uwvm-ros-vendor-complete-bounded-v1\",\"constant_schema_sha256\":\"ca3d163bab055381827226140568f3bef7eaac187cebd76878e0b63e9e442356\",\"types\":{");
quoted("mybuddy_communication/MybuddyAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mybuddy_communication::MybuddyAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mybuddy_communication::MybuddyAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mybuddy_communication/MybuddyCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mybuddy_communication::MybuddyCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mybuddy_communication::MybuddyCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mybuddy_communication/MybuddyGripperStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mybuddy_communication::MybuddyGripperStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mybuddy_communication::MybuddyGripperStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mybuddy_communication/MybuddyPumpStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mybuddy_communication::MybuddyPumpStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mybuddy_communication::MybuddyPumpStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mybuddy_communication/MybuddySetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mybuddy_communication::MybuddySetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mybuddy_communication::MybuddySetAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mybuddy_communication/MybuddySetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mybuddy_communication::MybuddySetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mybuddy_communication::MybuddySetCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mybuddy_communication/GetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mybuddy_communication::GetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mybuddy_communication::GetAngles::Request>("request",false);
component<::mybuddy_communication::GetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mybuddy_communication/GetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mybuddy_communication::GetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mybuddy_communication::GetCoords::Request>("request",false);
component<::mybuddy_communication::GetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mybuddy_communication/GripperStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mybuddy_communication::GripperStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mybuddy_communication::GripperStatus::Request>("request",false);
component<::mybuddy_communication::GripperStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mybuddy_communication/PumpStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mybuddy_communication::PumpStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mybuddy_communication::PumpStatus::Request>("request",false);
component<::mybuddy_communication::PumpStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mybuddy_communication/SetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mybuddy_communication::SetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mybuddy_communication::SetAngles::Request>("request",false);
component<::mybuddy_communication::SetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mybuddy_communication/SetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mybuddy_communication::SetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mybuddy_communication::SetCoords::Request>("request",false);
component<::mybuddy_communication::SetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_communication/MycobotAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mecharm_communication::MycobotAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_communication::MycobotAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_communication/MycobotCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mecharm_communication::MycobotCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_communication::MycobotCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_communication/MycobotGripperStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mecharm_communication::MycobotGripperStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_communication::MycobotGripperStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_communication/MycobotPumpStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mecharm_communication::MycobotPumpStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_communication::MycobotPumpStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_communication/MycobotSetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mecharm_communication::MycobotSetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_communication::MycobotSetAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_communication/MycobotSetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mecharm_communication::MycobotSetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_communication::MycobotSetCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_communication/GetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mecharm_communication::GetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_communication::GetAngles::Request>("request",false);
component<::mecharm_communication::GetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_communication/GetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mecharm_communication::GetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_communication::GetCoords::Request>("request",false);
component<::mecharm_communication::GetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_communication/GripperStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mecharm_communication::GripperStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_communication::GripperStatus::Request>("request",false);
component<::mecharm_communication::GripperStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_communication/PumpStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mecharm_communication::PumpStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_communication::PumpStatus::Request>("request",false);
component<::mecharm_communication::PumpStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_communication/SetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mecharm_communication::SetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_communication::SetAngles::Request>("request",false);
component<::mecharm_communication::SetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mecharm_communication/SetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mecharm_communication::SetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mecharm_communication::SetCoords::Request>("request",false);
component<::mecharm_communication::SetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("myarm_communication/MyArmAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::myarm_communication::MyArmAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::myarm_communication::MyArmAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("myarm_communication/MyArmCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::myarm_communication::MyArmCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::myarm_communication::MyArmCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("myarm_communication/MyArmGripperStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::myarm_communication::MyArmGripperStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::myarm_communication::MyArmGripperStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("myarm_communication/MyArmPumpStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::myarm_communication::MyArmPumpStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::myarm_communication::MyArmPumpStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("myarm_communication/MyArmSetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::myarm_communication::MyArmSetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::myarm_communication::MyArmSetAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("myarm_communication/MyArmSetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::myarm_communication::MyArmSetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::myarm_communication::MyArmSetCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("myarm_communication/GetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::myarm_communication::GetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::myarm_communication::GetAngles::Request>("request",false);
component<::myarm_communication::GetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("myarm_communication/GetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::myarm_communication::GetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::myarm_communication::GetCoords::Request>("request",false);
component<::myarm_communication::GetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("myarm_communication/GripperStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::myarm_communication::GripperStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::myarm_communication::GripperStatus::Request>("request",false);
component<::myarm_communication::GripperStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("myarm_communication/PumpStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::myarm_communication::PumpStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::myarm_communication::PumpStatus::Request>("request",false);
component<::myarm_communication::PumpStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("myarm_communication/SetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::myarm_communication::SetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::myarm_communication::SetAngles::Request>("request",false);
component<::myarm_communication::SetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("myarm_communication/SetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::myarm_communication::SetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::myarm_communication::SetCoords::Request>("request",false);
component<::myarm_communication::SetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_320_communication/MycobotAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_320_communication::MycobotAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_320_communication::MycobotAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_320_communication/MycobotCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_320_communication::MycobotCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_320_communication::MycobotCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_320_communication/MycobotGripperStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_320_communication::MycobotGripperStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_320_communication::MycobotGripperStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_320_communication/MycobotPumpStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_320_communication::MycobotPumpStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_320_communication::MycobotPumpStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_320_communication/MycobotSetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_320_communication::MycobotSetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_320_communication::MycobotSetAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_320_communication/MycobotSetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_320_communication::MycobotSetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_320_communication::MycobotSetCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_320_communication/GetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_320_communication::GetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_320_communication::GetAngles::Request>("request",false);
component<::mycobot_320_communication::GetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_320_communication/GetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_320_communication::GetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_320_communication::GetCoords::Request>("request",false);
component<::mycobot_320_communication::GetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_320_communication/GripperStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_320_communication::GripperStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_320_communication::GripperStatus::Request>("request",false);
component<::mycobot_320_communication::GripperStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_320_communication/PumpStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_320_communication::PumpStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_320_communication::PumpStatus::Request>("request",false);
component<::mycobot_320_communication::PumpStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_320_communication/SetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_320_communication::SetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_320_communication::SetAngles::Request>("request",false);
component<::mycobot_320_communication::SetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_320_communication/SetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_320_communication::SetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_320_communication::SetCoords::Request>("request",false);
component<::mycobot_320_communication::SetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/MycobotAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_communication::MycobotAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::MycobotAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/MycobotCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_communication::MycobotCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::MycobotCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/MycobotGetGripperValue");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_communication::MycobotGetGripperValue>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::MycobotGetGripperValue>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/MycobotGripperStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_communication::MycobotGripperStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::MycobotGripperStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/MycobotPumpStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_communication::MycobotPumpStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::MycobotPumpStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/MycobotSetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_communication::MycobotSetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::MycobotSetAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/MycobotSetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_communication::MycobotSetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::MycobotSetCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/MycobotSetEndType");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_communication::MycobotSetEndType>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::MycobotSetEndType>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/MycobotSetFreshMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_communication::MycobotSetFreshMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::MycobotSetFreshMode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/MycobotSetToolReference");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_communication::MycobotSetToolReference>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::MycobotSetToolReference>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/MycobotSetVisionMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_communication::MycobotSetVisionMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::MycobotSetVisionMode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/GetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_communication::GetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::GetAngles::Request>("request",false);
component<::mycobot_communication::GetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/GetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_communication::GetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::GetCoords::Request>("request",false);
component<::mycobot_communication::GetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/GripperStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_communication::GripperStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::GripperStatus::Request>("request",false);
component<::mycobot_communication::GripperStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/PumpStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_communication::PumpStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::PumpStatus::Request>("request",false);
component<::mycobot_communication::PumpStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/SetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_communication::SetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::SetAngles::Request>("request",false);
component<::mycobot_communication::SetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/SetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_communication::SetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::SetCoords::Request>("request",false);
component<::mycobot_communication::SetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/SetEndType");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_communication::SetEndType>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::SetEndType::Request>("request",false);
component<::mycobot_communication::SetEndType::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/SetFreshMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_communication::SetFreshMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::SetFreshMode::Request>("request",false);
component<::mycobot_communication::SetFreshMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/SetToolReference");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_communication::SetToolReference>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::SetToolReference::Request>("request",false);
component<::mycobot_communication::SetToolReference::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_communication/SetVisionMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_communication::SetVisionMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_communication::SetVisionMode::Request>("request",false);
component<::mycobot_communication::SetVisionMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_communication/MycobotAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_pro450_communication::MycobotAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_communication::MycobotAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_communication/MycobotCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_pro450_communication::MycobotCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_communication::MycobotCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_communication/MycobotGetGripperValue");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_pro450_communication::MycobotGetGripperValue>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_communication::MycobotGetGripperValue>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_communication/MycobotGripperStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_pro450_communication::MycobotGripperStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_communication::MycobotGripperStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_communication/MycobotSetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_pro450_communication::MycobotSetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_communication::MycobotSetAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_communication/MycobotSetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_pro450_communication::MycobotSetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_communication::MycobotSetCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_communication/MycobotSetFreshMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mycobot_pro450_communication::MycobotSetFreshMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_communication::MycobotSetFreshMode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_communication/GetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_pro450_communication::GetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_communication::GetAngles::Request>("request",false);
component<::mycobot_pro450_communication::GetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_communication/GetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_pro450_communication::GetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_communication::GetCoords::Request>("request",false);
component<::mycobot_pro450_communication::GetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_communication/GetGripperValue");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_pro450_communication::GetGripperValue>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_communication::GetGripperValue::Request>("request",false);
component<::mycobot_pro450_communication::GetGripperValue::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_communication/GripperStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_pro450_communication::GripperStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_communication::GripperStatus::Request>("request",false);
component<::mycobot_pro450_communication::GripperStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_communication/SetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_pro450_communication::SetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_communication::SetAngles::Request>("request",false);
component<::mycobot_pro450_communication::SetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_communication/SetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_pro450_communication::SetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_communication::SetCoords::Request>("request",false);
component<::mycobot_pro450_communication::SetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mycobot_pro450_communication/SetFreshMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mycobot_pro450_communication::SetFreshMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mycobot_pro450_communication::SetFreshMode::Request>("request",false);
component<::mycobot_pro450_communication::SetFreshMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_communication/MypalAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mypalletizer_communication::MypalAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_communication::MypalAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_communication/MypalCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mypalletizer_communication::MypalCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_communication::MypalCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_communication/MypalGripperStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mypalletizer_communication::MypalGripperStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_communication::MypalGripperStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_communication/MypalPumpStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mypalletizer_communication::MypalPumpStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_communication::MypalPumpStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_communication/MypalSetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mypalletizer_communication::MypalSetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_communication::MypalSetAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_communication/MypalSetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::mypalletizer_communication::MypalSetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_communication::MypalSetCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_communication/GetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mypalletizer_communication::GetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_communication::GetAngles::Request>("request",false);
component<::mypalletizer_communication::GetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_communication/GetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mypalletizer_communication::GetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_communication::GetCoords::Request>("request",false);
component<::mypalletizer_communication::GetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_communication/GripperStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mypalletizer_communication::GripperStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_communication::GripperStatus::Request>("request",false);
component<::mypalletizer_communication::GripperStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_communication/PumpStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mypalletizer_communication::PumpStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_communication::PumpStatus::Request>("request",false);
component<::mypalletizer_communication::PumpStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_communication/SetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mypalletizer_communication::SetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_communication::SetAngles::Request>("request",false);
component<::mypalletizer_communication::SetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("mypalletizer_communication/SetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::mypalletizer_communication::SetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::mypalletizer_communication::SetCoords::Request>("request",false);
component<::mypalletizer_communication::SetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ultraarm_communication/MycobotAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ultraarm_communication::MycobotAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ultraarm_communication::MycobotAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ultraarm_communication/MycobotCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ultraarm_communication::MycobotCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ultraarm_communication::MycobotCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ultraarm_communication/MycobotGetGripperValue");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ultraarm_communication::MycobotGetGripperValue>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ultraarm_communication::MycobotGetGripperValue>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ultraarm_communication/MycobotGripperStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ultraarm_communication::MycobotGripperStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ultraarm_communication::MycobotGripperStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ultraarm_communication/MycobotSetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ultraarm_communication::MycobotSetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ultraarm_communication::MycobotSetAngles>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ultraarm_communication/MycobotSetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ultraarm_communication::MycobotSetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ultraarm_communication::MycobotSetCoords>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ultraarm_communication/MycobotSetFreshMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ultraarm_communication::MycobotSetFreshMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ultraarm_communication::MycobotSetFreshMode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ultraarm_communication/GetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ultraarm_communication::GetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ultraarm_communication::GetAngles::Request>("request",false);
component<::ultraarm_communication::GetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ultraarm_communication/GetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ultraarm_communication::GetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ultraarm_communication::GetCoords::Request>("request",false);
component<::ultraarm_communication::GetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ultraarm_communication/GetGripperValue");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ultraarm_communication::GetGripperValue>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ultraarm_communication::GetGripperValue::Request>("request",false);
component<::ultraarm_communication::GetGripperValue::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ultraarm_communication/GripperStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ultraarm_communication::GripperStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ultraarm_communication::GripperStatus::Request>("request",false);
component<::ultraarm_communication::GripperStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ultraarm_communication/SetAngles");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ultraarm_communication::SetAngles>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ultraarm_communication::SetAngles::Request>("request",false);
component<::ultraarm_communication::SetAngles::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ultraarm_communication/SetCoords");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ultraarm_communication::SetCoords>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ultraarm_communication::SetCoords::Request>("request",false);
component<::ultraarm_communication::SetCoords::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ultraarm_communication/SetFreshMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ultraarm_communication::SetFreshMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ultraarm_communication::SetFreshMode::Request>("request",false);
component<::ultraarm_communication::SetFreshMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),"}}\n");return 0;
}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native industrial metadata refused\n");return 1;}}
