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
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Wrench.h>
#include <std_msgs/Header.h>
#include <ur_dashboard_msgs/AddToLog.h>
#include <ur_dashboard_msgs/GetLoadedProgram.h>
#include <ur_dashboard_msgs/GetProgramState.h>
#include <ur_dashboard_msgs/GetRobotMode.h>
#include <ur_dashboard_msgs/GetSafetyMode.h>
#include <ur_dashboard_msgs/IsInRemoteControl.h>
#include <ur_dashboard_msgs/IsProgramRunning.h>
#include <ur_dashboard_msgs/IsProgramSaved.h>
#include <ur_dashboard_msgs/Load.h>
#include <ur_dashboard_msgs/Popup.h>
#include <ur_dashboard_msgs/ProgramState.h>
#include <ur_dashboard_msgs/RawRequest.h>
#include <ur_dashboard_msgs/RobotMode.h>
#include <ur_dashboard_msgs/SafetyMode.h>
#include <ur_dashboard_msgs/SetModeAction.h>
#include <ur_msgs/Analog.h>
#include <ur_msgs/Digital.h>
#include <ur_msgs/GetRobotSoftwareVersion.h>
#include <ur_msgs/IOStates.h>
#include <ur_msgs/MasterboardDataMsg.h>
#include <ur_msgs/RobotModeDataMsg.h>
#include <ur_msgs/RobotStateRTMsg.h>
#include <ur_msgs/SetAnalogOutput.h>
#include <ur_msgs/SetForceMode.h>
#include <ur_msgs/SetIO.h>
#include <ur_msgs/SetPayload.h>
#include <ur_msgs/SetSpeedSliderFraction.h>
#include <ur_msgs/ToolDataMsg.h>
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
if(!::uwvm_industrial_source_606428f9f784caaf974491d44dd404d3d098b4464399c589f2bb8114902bf41b::validate())throw std::runtime_error{"native source constants differ"};
if(::raise(SIGSTOP)!=0)throw std::runtime_error{"owned mapping stop failed"};
bounded_native_io::gate('I');
bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-industrial-native-v1\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"source_profile\":\"uwvm-ros-vendor-complete-bounded-v1\",\"constant_schema_sha256\":\"606428f9f784caaf974491d44dd404d3d098b4464399c589f2bb8114902bf41b\",\"types\":{");
quoted("geometry_msgs/Point");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::Point>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::Point>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/Pose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::Pose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::Pose>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/PoseStamped");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::PoseStamped>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::PoseStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/Quaternion");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::Quaternion>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::Quaternion>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/Twist");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::Twist>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::Twist>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/Vector3");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::Vector3>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::Vector3>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/Wrench");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::Wrench>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::Wrench>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/Header");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::std_msgs::Header>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::Header>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/AddToLog");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ur_dashboard_msgs::AddToLog>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::AddToLog::Request>("request",false);
component<::ur_dashboard_msgs::AddToLog::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/GetLoadedProgram");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ur_dashboard_msgs::GetLoadedProgram>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::GetLoadedProgram::Request>("request",false);
component<::ur_dashboard_msgs::GetLoadedProgram::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/GetProgramState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ur_dashboard_msgs::GetProgramState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::GetProgramState::Request>("request",false);
component<::ur_dashboard_msgs::GetProgramState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/GetRobotMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ur_dashboard_msgs::GetRobotMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::GetRobotMode::Request>("request",false);
component<::ur_dashboard_msgs::GetRobotMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/GetSafetyMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ur_dashboard_msgs::GetSafetyMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::GetSafetyMode::Request>("request",false);
component<::ur_dashboard_msgs::GetSafetyMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/IsInRemoteControl");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ur_dashboard_msgs::IsInRemoteControl>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::IsInRemoteControl::Request>("request",false);
component<::ur_dashboard_msgs::IsInRemoteControl::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/IsProgramRunning");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ur_dashboard_msgs::IsProgramRunning>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::IsProgramRunning::Request>("request",false);
component<::ur_dashboard_msgs::IsProgramRunning::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/IsProgramSaved");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ur_dashboard_msgs::IsProgramSaved>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::IsProgramSaved::Request>("request",false);
component<::ur_dashboard_msgs::IsProgramSaved::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/Load");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ur_dashboard_msgs::Load>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::Load::Request>("request",false);
component<::ur_dashboard_msgs::Load::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/Popup");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ur_dashboard_msgs::Popup>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::Popup::Request>("request",false);
component<::ur_dashboard_msgs::Popup::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/ProgramState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ur_dashboard_msgs::ProgramState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::ProgramState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/RawRequest");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ur_dashboard_msgs::RawRequest>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::RawRequest::Request>("request",false);
component<::ur_dashboard_msgs::RawRequest::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/RobotMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ur_dashboard_msgs::RobotMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::RobotMode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/SafetyMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ur_dashboard_msgs::SafetyMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::SafetyMode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_dashboard_msgs/SetMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ur_dashboard_msgs::SetModeAction>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_dashboard_msgs::SetModeGoal>("goal",false);
component<::ur_dashboard_msgs::SetModeResult>("result",true);
component<::ur_dashboard_msgs::SetModeFeedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/Analog");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ur_msgs::Analog>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::Analog>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/Digital");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ur_msgs::Digital>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::Digital>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/GetRobotSoftwareVersion");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ur_msgs::GetRobotSoftwareVersion>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::GetRobotSoftwareVersion::Request>("request",false);
component<::ur_msgs::GetRobotSoftwareVersion::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/IOStates");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ur_msgs::IOStates>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::IOStates>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/MasterboardDataMsg");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ur_msgs::MasterboardDataMsg>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::MasterboardDataMsg>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/RobotModeDataMsg");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ur_msgs::RobotModeDataMsg>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::RobotModeDataMsg>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/RobotStateRTMsg");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ur_msgs::RobotStateRTMsg>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::RobotStateRTMsg>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/SetAnalogOutput");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ur_msgs::SetAnalogOutput>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::SetAnalogOutput::Request>("request",false);
component<::ur_msgs::SetAnalogOutput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/SetForceMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ur_msgs::SetForceMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::SetForceMode::Request>("request",false);
component<::ur_msgs::SetForceMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/SetIO");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ur_msgs::SetIO>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::SetIO::Request>("request",false);
component<::ur_msgs::SetIO::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/SetPayload");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ur_msgs::SetPayload>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::SetPayload::Request>("request",false);
component<::ur_msgs::SetPayload::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/SetSpeedSliderFraction");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ur_msgs::SetSpeedSliderFraction>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::SetSpeedSliderFraction::Request>("request",false);
component<::ur_msgs::SetSpeedSliderFraction::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ur_msgs/ToolDataMsg");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ur_msgs::ToolDataMsg>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ur_msgs::ToolDataMsg>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),"}}\n");return 0;
}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native industrial metadata refused\n");return 1;}}
