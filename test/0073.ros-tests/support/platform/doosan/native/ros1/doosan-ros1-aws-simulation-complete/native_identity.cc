/** Industrial native metadata source; no Node, endpoint or grant. */
#include "test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/native_protocol.h"
#include <array>
#include <string_view>
#include <stdexcept>
#include <csignal>
#include <cstdint>
#if UWVM_CUSTOM_NATIVE_ROS_VERSION != 1
#error "Select the exact source-reviewed ROS major"
#endif
#include <dsr_msgs/ConfigCreateModbus.h>
#include <dsr_msgs/ConfigCreateTcp.h>
#include <dsr_msgs/ConfigCreateTool.h>
#include <dsr_msgs/ConfigDeleteModbus.h>
#include <dsr_msgs/ConfigDeleteTcp.h>
#include <dsr_msgs/ConfigDeleteTool.h>
#include <dsr_msgs/DrlPause.h>
#include <dsr_msgs/DrlResume.h>
#include <dsr_msgs/DrlStart.h>
#include <dsr_msgs/DrlStop.h>
#include <dsr_msgs/GetCtrlBoxAnalogInput.h>
#include <dsr_msgs/GetCtrlBoxDigitalInput.h>
#include <dsr_msgs/GetCurrentPose.h>
#include <dsr_msgs/GetCurrentSolutionSpace.h>
#include <dsr_msgs/GetCurrentTcp.h>
#include <dsr_msgs/GetCurrentTool.h>
#include <dsr_msgs/GetDrlState.h>
#include <dsr_msgs/GetExternalTorque.h>
#include <dsr_msgs/GetJointTorque.h>
#include <dsr_msgs/GetLastAlarm.h>
#include <dsr_msgs/GetModbusInput.h>
#include <dsr_msgs/GetRobotMode.h>
#include <dsr_msgs/GetRobotSpeedMode.h>
#include <dsr_msgs/GetRobotState.h>
#include <dsr_msgs/GetRobotSystem.h>
#include <dsr_msgs/GetToolDigitalInput.h>
#include <dsr_msgs/GetToolForce.h>
#include <dsr_msgs/Jog.h>
#include <dsr_msgs/JogMulti.h>
#include <dsr_msgs/JogMultiAxis.h>
#include <dsr_msgs/LogAlarm.h>
#include <dsr_msgs/ModbusState.h>
#include <dsr_msgs/MoveBlending.h>
#include <dsr_msgs/MoveCircle.h>
#include <dsr_msgs/MoveJoint.h>
#include <dsr_msgs/MoveJointx.h>
#include <dsr_msgs/MoveLine.h>
#include <dsr_msgs/MovePause.h>
#include <dsr_msgs/MovePeriodic.h>
#include <dsr_msgs/MoveResume.h>
#include <dsr_msgs/MoveSpiral.h>
#include <dsr_msgs/MoveSplineJoint.h>
#include <dsr_msgs/MoveSplineTask.h>
#include <dsr_msgs/MoveStop.h>
#include <dsr_msgs/MoveWait.h>
#include <dsr_msgs/RobotError.h>
#include <dsr_msgs/RobotState.h>
#include <dsr_msgs/RobotStop.h>
#include <dsr_msgs/Robotiq2FClose.h>
#include <dsr_msgs/Robotiq2FMove.h>
#include <dsr_msgs/Robotiq2FOpen.h>
#include <dsr_msgs/SerialSendData.h>
#include <dsr_msgs/SetCtrlBoxAnalogInputType.h>
#include <dsr_msgs/SetCtrlBoxAnalogOutput.h>
#include <dsr_msgs/SetCtrlBoxAnalogOutputType.h>
#include <dsr_msgs/SetCtrlBoxDigitalOutput.h>
#include <dsr_msgs/SetCurrentTcp.h>
#include <dsr_msgs/SetCurrentTool.h>
#include <dsr_msgs/SetModbusOutput.h>
#include <dsr_msgs/SetRobotMode.h>
#include <dsr_msgs/SetRobotSpeedMode.h>
#include <dsr_msgs/SetRobotSystem.h>
#include <dsr_msgs/SetSafeStopResetType.h>
#include <dsr_msgs/SetToolDigitalOutput.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/MultiArrayLayout.h>
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
quoted("dsr_msgs/ConfigCreateModbus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::ConfigCreateModbus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ConfigCreateModbus::Request>("request",false);
component<::dsr_msgs::ConfigCreateModbus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/ConfigCreateTcp");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::ConfigCreateTcp>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ConfigCreateTcp::Request>("request",false);
component<::dsr_msgs::ConfigCreateTcp::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/ConfigCreateTool");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::ConfigCreateTool>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ConfigCreateTool::Request>("request",false);
component<::dsr_msgs::ConfigCreateTool::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/ConfigDeleteModbus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::ConfigDeleteModbus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ConfigDeleteModbus::Request>("request",false);
component<::dsr_msgs::ConfigDeleteModbus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/ConfigDeleteTcp");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::ConfigDeleteTcp>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ConfigDeleteTcp::Request>("request",false);
component<::dsr_msgs::ConfigDeleteTcp::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/ConfigDeleteTool");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::ConfigDeleteTool>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ConfigDeleteTool::Request>("request",false);
component<::dsr_msgs::ConfigDeleteTool::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/DrlPause");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::DrlPause>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::DrlPause::Request>("request",false);
component<::dsr_msgs::DrlPause::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/DrlResume");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::DrlResume>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::DrlResume::Request>("request",false);
component<::dsr_msgs::DrlResume::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/DrlStart");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::DrlStart>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::DrlStart::Request>("request",false);
component<::dsr_msgs::DrlStart::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/DrlStop");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::DrlStop>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::DrlStop::Request>("request",false);
component<::dsr_msgs::DrlStop::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetCtrlBoxAnalogInput");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetCtrlBoxAnalogInput>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetCtrlBoxAnalogInput::Request>("request",false);
component<::dsr_msgs::GetCtrlBoxAnalogInput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetCtrlBoxDigitalInput");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetCtrlBoxDigitalInput>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetCtrlBoxDigitalInput::Request>("request",false);
component<::dsr_msgs::GetCtrlBoxDigitalInput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetCurrentPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetCurrentPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetCurrentPose::Request>("request",false);
component<::dsr_msgs::GetCurrentPose::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetCurrentSolutionSpace");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetCurrentSolutionSpace>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetCurrentSolutionSpace::Request>("request",false);
component<::dsr_msgs::GetCurrentSolutionSpace::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetCurrentTcp");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetCurrentTcp>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetCurrentTcp::Request>("request",false);
component<::dsr_msgs::GetCurrentTcp::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetCurrentTool");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetCurrentTool>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetCurrentTool::Request>("request",false);
component<::dsr_msgs::GetCurrentTool::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetDrlState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetDrlState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetDrlState::Request>("request",false);
component<::dsr_msgs::GetDrlState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetExternalTorque");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetExternalTorque>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetExternalTorque::Request>("request",false);
component<::dsr_msgs::GetExternalTorque::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetJointTorque");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetJointTorque>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetJointTorque::Request>("request",false);
component<::dsr_msgs::GetJointTorque::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetLastAlarm");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetLastAlarm>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetLastAlarm::Request>("request",false);
component<::dsr_msgs::GetLastAlarm::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetModbusInput");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetModbusInput>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetModbusInput::Request>("request",false);
component<::dsr_msgs::GetModbusInput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetRobotMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetRobotMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetRobotMode::Request>("request",false);
component<::dsr_msgs::GetRobotMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetRobotSpeedMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetRobotSpeedMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetRobotSpeedMode::Request>("request",false);
component<::dsr_msgs::GetRobotSpeedMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetRobotState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetRobotState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetRobotState::Request>("request",false);
component<::dsr_msgs::GetRobotState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetRobotSystem");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetRobotSystem>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetRobotSystem::Request>("request",false);
component<::dsr_msgs::GetRobotSystem::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetToolDigitalInput");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetToolDigitalInput>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetToolDigitalInput::Request>("request",false);
component<::dsr_msgs::GetToolDigitalInput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetToolForce");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetToolForce>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetToolForce::Request>("request",false);
component<::dsr_msgs::GetToolForce::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/Jog");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::Jog>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::Jog::Request>("request",false);
component<::dsr_msgs::Jog::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/JogMulti");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::JogMulti>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::JogMulti::Request>("request",false);
component<::dsr_msgs::JogMulti::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/JogMultiAxis");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::dsr_msgs::JogMultiAxis>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::JogMultiAxis>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/LogAlarm");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::dsr_msgs::LogAlarm>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::LogAlarm>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/ModbusState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::dsr_msgs::ModbusState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ModbusState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/MoveBlending");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::MoveBlending>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::MoveBlending::Request>("request",false);
component<::dsr_msgs::MoveBlending::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/MoveCircle");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::MoveCircle>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::MoveCircle::Request>("request",false);
component<::dsr_msgs::MoveCircle::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/MoveJoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::MoveJoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::MoveJoint::Request>("request",false);
component<::dsr_msgs::MoveJoint::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/MoveJointx");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::MoveJointx>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::MoveJointx::Request>("request",false);
component<::dsr_msgs::MoveJointx::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/MoveLine");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::MoveLine>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::MoveLine::Request>("request",false);
component<::dsr_msgs::MoveLine::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/MovePause");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::MovePause>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::MovePause::Request>("request",false);
component<::dsr_msgs::MovePause::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/MovePeriodic");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::MovePeriodic>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::MovePeriodic::Request>("request",false);
component<::dsr_msgs::MovePeriodic::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/MoveResume");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::MoveResume>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::MoveResume::Request>("request",false);
component<::dsr_msgs::MoveResume::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/MoveSpiral");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::MoveSpiral>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::MoveSpiral::Request>("request",false);
component<::dsr_msgs::MoveSpiral::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/MoveSplineJoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::MoveSplineJoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::MoveSplineJoint::Request>("request",false);
component<::dsr_msgs::MoveSplineJoint::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/MoveSplineTask");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::MoveSplineTask>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::MoveSplineTask::Request>("request",false);
component<::dsr_msgs::MoveSplineTask::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/MoveStop");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::MoveStop>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::MoveStop::Request>("request",false);
component<::dsr_msgs::MoveStop::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/MoveWait");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::MoveWait>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::MoveWait::Request>("request",false);
component<::dsr_msgs::MoveWait::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/RobotError");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::dsr_msgs::RobotError>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::RobotError>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/RobotState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::dsr_msgs::RobotState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::RobotState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/RobotStop");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::dsr_msgs::RobotStop>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::RobotStop>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/Robotiq2FClose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::Robotiq2FClose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::Robotiq2FClose::Request>("request",false);
component<::dsr_msgs::Robotiq2FClose::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/Robotiq2FMove");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::Robotiq2FMove>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::Robotiq2FMove::Request>("request",false);
component<::dsr_msgs::Robotiq2FMove::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/Robotiq2FOpen");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::Robotiq2FOpen>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::Robotiq2FOpen::Request>("request",false);
component<::dsr_msgs::Robotiq2FOpen::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SerialSendData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SerialSendData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SerialSendData::Request>("request",false);
component<::dsr_msgs::SerialSendData::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetCtrlBoxAnalogInputType");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetCtrlBoxAnalogInputType>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetCtrlBoxAnalogInputType::Request>("request",false);
component<::dsr_msgs::SetCtrlBoxAnalogInputType::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetCtrlBoxAnalogOutput");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetCtrlBoxAnalogOutput>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetCtrlBoxAnalogOutput::Request>("request",false);
component<::dsr_msgs::SetCtrlBoxAnalogOutput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetCtrlBoxAnalogOutputType");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetCtrlBoxAnalogOutputType>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetCtrlBoxAnalogOutputType::Request>("request",false);
component<::dsr_msgs::SetCtrlBoxAnalogOutputType::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetCtrlBoxDigitalOutput");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetCtrlBoxDigitalOutput>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetCtrlBoxDigitalOutput::Request>("request",false);
component<::dsr_msgs::SetCtrlBoxDigitalOutput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetCurrentTcp");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetCurrentTcp>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetCurrentTcp::Request>("request",false);
component<::dsr_msgs::SetCurrentTcp::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetCurrentTool");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetCurrentTool>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetCurrentTool::Request>("request",false);
component<::dsr_msgs::SetCurrentTool::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetModbusOutput");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetModbusOutput>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetModbusOutput::Request>("request",false);
component<::dsr_msgs::SetModbusOutput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetRobotMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetRobotMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetRobotMode::Request>("request",false);
component<::dsr_msgs::SetRobotMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetRobotSpeedMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetRobotSpeedMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetRobotSpeedMode::Request>("request",false);
component<::dsr_msgs::SetRobotSpeedMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetRobotSystem");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetRobotSystem>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetRobotSystem::Request>("request",false);
component<::dsr_msgs::SetRobotSystem::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetSafeStopResetType");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetSafeStopResetType>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetSafeStopResetType::Request>("request",false);
component<::dsr_msgs::SetSafeStopResetType::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetToolDigitalOutput");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetToolDigitalOutput>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetToolDigitalOutput::Request>("request",false);
component<::dsr_msgs::SetToolDigitalOutput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/Float64MultiArray");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::std_msgs::Float64MultiArray>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::Float64MultiArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/MultiArrayDimension");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::std_msgs::MultiArrayDimension>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::MultiArrayDimension>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/MultiArrayLayout");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::std_msgs::MultiArrayLayout>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::MultiArrayLayout>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),"}}\n");return 0;
}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native industrial metadata refused\n");return 1;}}
