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
#include <abb_egm_msgs/EGMChannelState.h>
#include <abb_egm_msgs/EGMState.h>
#include <abb_rapid_msgs/ConfData.h>
#include <abb_rapid_msgs/ExtJoint.h>
#include <abb_rapid_msgs/JointTarget.h>
#include <abb_rapid_msgs/LoadData.h>
#include <abb_rapid_msgs/Orient.h>
#include <abb_rapid_msgs/Pos.h>
#include <abb_rapid_msgs/Pose.h>
#include <abb_rapid_msgs/RobJoint.h>
#include <abb_rapid_msgs/RobTarget.h>
#include <abb_rapid_msgs/ToolData.h>
#include <abb_rapid_msgs/WObjData.h>
#include <abb_rapid_sm_addin_msgs/EGMActivateSettings.h>
#include <abb_rapid_sm_addin_msgs/EGMRunSettings.h>
#include <abb_rapid_sm_addin_msgs/EGMSettings.h>
#include <abb_rapid_sm_addin_msgs/EGMSetupSettings.h>
#include <abb_rapid_sm_addin_msgs/EGMStopSettings.h>
#include <abb_rapid_sm_addin_msgs/GetEGMSettings.h>
#include <abb_rapid_sm_addin_msgs/RuntimeState.h>
#include <abb_rapid_sm_addin_msgs/SetEGMSettings.h>
#include <abb_rapid_sm_addin_msgs/SetRAPIDRoutine.h>
#include <abb_rapid_sm_addin_msgs/SetSGCommand.h>
#include <abb_rapid_sm_addin_msgs/StateMachineState.h>
#include <abb_robot_msgs/GetFileContents.h>
#include <abb_robot_msgs/GetIOSignal.h>
#include <abb_robot_msgs/GetRAPIDBool.h>
#include <abb_robot_msgs/GetRAPIDDnum.h>
#include <abb_robot_msgs/GetRAPIDNum.h>
#include <abb_robot_msgs/GetRAPIDString.h>
#include <abb_robot_msgs/GetRAPIDSymbol.h>
#include <abb_robot_msgs/GetRobotControllerDescription.h>
#include <abb_robot_msgs/GetSpeedRatio.h>
#include <abb_robot_msgs/MechanicalUnitState.h>
#include <abb_robot_msgs/RAPIDSymbolPath.h>
#include <abb_robot_msgs/RAPIDTaskState.h>
#include <abb_robot_msgs/ServiceResponses.h>
#include <abb_robot_msgs/SetFileContents.h>
#include <abb_robot_msgs/SetIOSignal.h>
#include <abb_robot_msgs/SetRAPIDBool.h>
#include <abb_robot_msgs/SetRAPIDDnum.h>
#include <abb_robot_msgs/SetRAPIDNum.h>
#include <abb_robot_msgs/SetRAPIDString.h>
#include <abb_robot_msgs/SetRAPIDSymbol.h>
#include <abb_robot_msgs/SetSpeedRatio.h>
#include <abb_robot_msgs/SystemState.h>
#include <abb_robot_msgs/TriggerWithResultCode.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <control_msgs/JointTolerance.h>
#include <sensor_msgs/JointState.h>
#include <std_msgs/Header.h>
#include <trajectory_msgs/JointTrajectory.h>
#include <trajectory_msgs/JointTrajectoryPoint.h>
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
if(!::uwvm_industrial_source_467219762b8fa636c81fe3b57b998405cb85a8d3335606654766add1a740ac22::validate())throw std::runtime_error{"native source constants differ"};
if(::raise(SIGSTOP)!=0)throw std::runtime_error{"owned mapping stop failed"};
bounded_native_io::gate('I');
bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-industrial-native-v1\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"source_profile\":\"uwvm-ros-vendor-complete-bounded-v1\",\"constant_schema_sha256\":\"467219762b8fa636c81fe3b57b998405cb85a8d3335606654766add1a740ac22\",\"types\":{");
quoted("abb_egm_msgs/EGMChannelState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_egm_msgs::EGMChannelState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_egm_msgs::EGMChannelState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_egm_msgs/EGMState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_egm_msgs::EGMState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_egm_msgs::EGMState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/ConfData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_msgs::ConfData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::ConfData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/ExtJoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_msgs::ExtJoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::ExtJoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/JointTarget");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_msgs::JointTarget>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::JointTarget>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/LoadData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_msgs::LoadData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::LoadData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/Orient");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_msgs::Orient>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::Orient>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/Pos");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_msgs::Pos>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::Pos>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/Pose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_msgs::Pose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::Pose>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/RobJoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_msgs::RobJoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::RobJoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/RobTarget");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_msgs::RobTarget>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::RobTarget>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/ToolData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_msgs::ToolData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::ToolData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_msgs/WObjData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_msgs::WObjData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_msgs::WObjData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/EGMActivateSettings");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_sm_addin_msgs::EGMActivateSettings>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::EGMActivateSettings>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/EGMRunSettings");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_sm_addin_msgs::EGMRunSettings>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::EGMRunSettings>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/EGMSettings");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_sm_addin_msgs::EGMSettings>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::EGMSettings>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/EGMSetupSettings");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_sm_addin_msgs::EGMSetupSettings>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::EGMSetupSettings>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/EGMStopSettings");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_sm_addin_msgs::EGMStopSettings>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::EGMStopSettings>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/GetEGMSettings");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_rapid_sm_addin_msgs::GetEGMSettings>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::GetEGMSettings::Request>("request",false);
component<::abb_rapid_sm_addin_msgs::GetEGMSettings::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/RuntimeState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_sm_addin_msgs::RuntimeState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::RuntimeState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/SetEGMSettings");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_rapid_sm_addin_msgs::SetEGMSettings>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::SetEGMSettings::Request>("request",false);
component<::abb_rapid_sm_addin_msgs::SetEGMSettings::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/SetRAPIDRoutine");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_rapid_sm_addin_msgs::SetRAPIDRoutine>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::SetRAPIDRoutine::Request>("request",false);
component<::abb_rapid_sm_addin_msgs::SetRAPIDRoutine::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/SetSGCommand");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_rapid_sm_addin_msgs::SetSGCommand>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::SetSGCommand::Request>("request",false);
component<::abb_rapid_sm_addin_msgs::SetSGCommand::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_rapid_sm_addin_msgs/StateMachineState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_rapid_sm_addin_msgs::StateMachineState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_rapid_sm_addin_msgs::StateMachineState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/GetFileContents");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::GetFileContents>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::GetFileContents::Request>("request",false);
component<::abb_robot_msgs::GetFileContents::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/GetIOSignal");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::GetIOSignal>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::GetIOSignal::Request>("request",false);
component<::abb_robot_msgs::GetIOSignal::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/GetRAPIDBool");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::GetRAPIDBool>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::GetRAPIDBool::Request>("request",false);
component<::abb_robot_msgs::GetRAPIDBool::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/GetRAPIDDnum");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::GetRAPIDDnum>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::GetRAPIDDnum::Request>("request",false);
component<::abb_robot_msgs::GetRAPIDDnum::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/GetRAPIDNum");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::GetRAPIDNum>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::GetRAPIDNum::Request>("request",false);
component<::abb_robot_msgs::GetRAPIDNum::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/GetRAPIDString");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::GetRAPIDString>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::GetRAPIDString::Request>("request",false);
component<::abb_robot_msgs::GetRAPIDString::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/GetRAPIDSymbol");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::GetRAPIDSymbol>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::GetRAPIDSymbol::Request>("request",false);
component<::abb_robot_msgs::GetRAPIDSymbol::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/GetRobotControllerDescription");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::GetRobotControllerDescription>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::GetRobotControllerDescription::Request>("request",false);
component<::abb_robot_msgs::GetRobotControllerDescription::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/GetSpeedRatio");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::GetSpeedRatio>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::GetSpeedRatio::Request>("request",false);
component<::abb_robot_msgs::GetSpeedRatio::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/MechanicalUnitState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_robot_msgs::MechanicalUnitState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::MechanicalUnitState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/RAPIDSymbolPath");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_robot_msgs::RAPIDSymbolPath>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::RAPIDSymbolPath>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/RAPIDTaskState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_robot_msgs::RAPIDTaskState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::RAPIDTaskState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/ServiceResponses");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_robot_msgs::ServiceResponses>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::ServiceResponses>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/SetFileContents");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::SetFileContents>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::SetFileContents::Request>("request",false);
component<::abb_robot_msgs::SetFileContents::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/SetIOSignal");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::SetIOSignal>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::SetIOSignal::Request>("request",false);
component<::abb_robot_msgs::SetIOSignal::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/SetRAPIDBool");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::SetRAPIDBool>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::SetRAPIDBool::Request>("request",false);
component<::abb_robot_msgs::SetRAPIDBool::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/SetRAPIDDnum");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::SetRAPIDDnum>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::SetRAPIDDnum::Request>("request",false);
component<::abb_robot_msgs::SetRAPIDDnum::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/SetRAPIDNum");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::SetRAPIDNum>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::SetRAPIDNum::Request>("request",false);
component<::abb_robot_msgs::SetRAPIDNum::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/SetRAPIDString");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::SetRAPIDString>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::SetRAPIDString::Request>("request",false);
component<::abb_robot_msgs::SetRAPIDString::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/SetRAPIDSymbol");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::SetRAPIDSymbol>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::SetRAPIDSymbol::Request>("request",false);
component<::abb_robot_msgs::SetRAPIDSymbol::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/SetSpeedRatio");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::SetSpeedRatio>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::SetSpeedRatio::Request>("request",false);
component<::abb_robot_msgs::SetSpeedRatio::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/SystemState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::abb_robot_msgs::SystemState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::SystemState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("abb_robot_msgs/TriggerWithResultCode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::abb_robot_msgs::TriggerWithResultCode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::abb_robot_msgs::TriggerWithResultCode::Request>("request",false);
component<::abb_robot_msgs::TriggerWithResultCode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("control_msgs/FollowJointTrajectory");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::control_msgs::FollowJointTrajectoryAction>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::control_msgs::FollowJointTrajectoryGoal>("goal",false);
component<::control_msgs::FollowJointTrajectoryResult>("result",true);
component<::control_msgs::FollowJointTrajectoryFeedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("control_msgs/JointTolerance");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::control_msgs::JointTolerance>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::control_msgs::JointTolerance>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("sensor_msgs/JointState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::sensor_msgs::JointState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::sensor_msgs::JointState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/Header");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::std_msgs::Header>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::Header>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("trajectory_msgs/JointTrajectory");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::trajectory_msgs::JointTrajectory>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::trajectory_msgs::JointTrajectory>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("trajectory_msgs/JointTrajectoryPoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::trajectory_msgs::JointTrajectoryPoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::trajectory_msgs::JointTrajectoryPoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),"}}\n");return 0;
}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native industrial metadata refused\n");return 1;}}
