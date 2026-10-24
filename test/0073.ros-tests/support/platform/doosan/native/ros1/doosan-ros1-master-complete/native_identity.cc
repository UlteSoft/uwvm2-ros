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
#include <dsr_msgs/AlignAxis1.h>
#include <dsr_msgs/AlignAxis2.h>
#include <dsr_msgs/AlterMotion.h>
#include <dsr_msgs/AlterMotionStream.h>
#include <dsr_msgs/CalcCoord.h>
#include <dsr_msgs/ChangeOperationSpeed.h>
#include <dsr_msgs/CheckForceCondition.h>
#include <dsr_msgs/CheckMotion.h>
#include <dsr_msgs/CheckOrientationCondition1.h>
#include <dsr_msgs/CheckOrientationCondition2.h>
#include <dsr_msgs/CheckPositionCondition.h>
#include <dsr_msgs/ConfigCreateModbus.h>
#include <dsr_msgs/ConfigCreateTcp.h>
#include <dsr_msgs/ConfigCreateTool.h>
#include <dsr_msgs/ConfigDeleteModbus.h>
#include <dsr_msgs/ConfigDeleteTcp.h>
#include <dsr_msgs/ConfigDeleteTool.h>
#include <dsr_msgs/ConnectRTControl.h>
#include <dsr_msgs/CoordTransform.h>
#include <dsr_msgs/DisableAlterMotion.h>
#include <dsr_msgs/DisconnectRTControl.h>
#include <dsr_msgs/DrlPause.h>
#include <dsr_msgs/DrlResume.h>
#include <dsr_msgs/DrlStart.h>
#include <dsr_msgs/DrlStop.h>
#include <dsr_msgs/EnableAlterMotion.h>
#include <dsr_msgs/Fkin.h>
#include <dsr_msgs/GetControlMode.h>
#include <dsr_msgs/GetControlSpace.h>
#include <dsr_msgs/GetCtrlBoxAnalogInput.h>
#include <dsr_msgs/GetCtrlBoxDigitalInput.h>
#include <dsr_msgs/GetCtrlBoxDigitalOutput.h>
#include <dsr_msgs/GetCurrentPose.h>
#include <dsr_msgs/GetCurrentPosj.h>
#include <dsr_msgs/GetCurrentPosx.h>
#include <dsr_msgs/GetCurrentRotm.h>
#include <dsr_msgs/GetCurrentSolutionSpace.h>
#include <dsr_msgs/GetCurrentTcp.h>
#include <dsr_msgs/GetCurrentTool.h>
#include <dsr_msgs/GetCurrentToolFlangePosx.h>
#include <dsr_msgs/GetCurrentVelj.h>
#include <dsr_msgs/GetCurrentVelx.h>
#include <dsr_msgs/GetDesiredPosj.h>
#include <dsr_msgs/GetDesiredPosx.h>
#include <dsr_msgs/GetDesiredVelj.h>
#include <dsr_msgs/GetDesiredVelx.h>
#include <dsr_msgs/GetDrlState.h>
#include <dsr_msgs/GetExternalTorque.h>
#include <dsr_msgs/GetJointTorque.h>
#include <dsr_msgs/GetLastAlarm.h>
#include <dsr_msgs/GetModbusInput.h>
#include <dsr_msgs/GetOrientationError.h>
#include <dsr_msgs/GetRTControlInputDataList.h>
#include <dsr_msgs/GetRTControlInputVersionList.h>
#include <dsr_msgs/GetRTControlOutputDataList.h>
#include <dsr_msgs/GetRTControlOutputVersionList.h>
#include <dsr_msgs/GetRobotMode.h>
#include <dsr_msgs/GetRobotSpeedMode.h>
#include <dsr_msgs/GetRobotState.h>
#include <dsr_msgs/GetRobotSystem.h>
#include <dsr_msgs/GetSolutionSpace.h>
#include <dsr_msgs/GetToolDigitalInput.h>
#include <dsr_msgs/GetToolDigitalOutput.h>
#include <dsr_msgs/GetToolForce.h>
#include <dsr_msgs/GetUserCartCoord.h>
#include <dsr_msgs/GetWorkpieceWeight.h>
#include <dsr_msgs/Ikin.h>
#include <dsr_msgs/IkinEx.h>
#include <dsr_msgs/IsDoneBoltTightening.h>
#include <dsr_msgs/Jog.h>
#include <dsr_msgs/JogMulti.h>
#include <dsr_msgs/JogMultiAxis.h>
#include <dsr_msgs/LogAlarm.h>
#include <dsr_msgs/ManageAccessControl.h>
#include <dsr_msgs/ModbusState.h>
#include <dsr_msgs/MoveBlending.h>
#include <dsr_msgs/MoveCircle.h>
#include <dsr_msgs/MoveHome.h>
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
#include <dsr_msgs/OverwriteUserCartCoord.h>
#include <dsr_msgs/ParallelAxis1.h>
#include <dsr_msgs/ParallelAxis2.h>
#include <dsr_msgs/ReadDataRT.h>
#include <dsr_msgs/ReleaseComplianceCtrl.h>
#include <dsr_msgs/ReleaseForce.h>
#include <dsr_msgs/ReleaseProtectiveStop.h>
#include <dsr_msgs/ResetWorkpieceWeight.h>
#include <dsr_msgs/RobotError.h>
#include <dsr_msgs/RobotState.h>
#include <dsr_msgs/RobotStateRT.h>
#include <dsr_msgs/RobotStop.h>
#include <dsr_msgs/Robotiq2FClose.h>
#include <dsr_msgs/Robotiq2FMove.h>
#include <dsr_msgs/Robotiq2FOpen.h>
#include <dsr_msgs/SerialSendData.h>
#include <dsr_msgs/ServoJRTStream.h>
#include <dsr_msgs/ServoJStream.h>
#include <dsr_msgs/ServoLRTStream.h>
#include <dsr_msgs/ServoLStream.h>
#include <dsr_msgs/SetAccJRT.h>
#include <dsr_msgs/SetAccXRT.h>
#include <dsr_msgs/SetCtrlBoxAnalogInputType.h>
#include <dsr_msgs/SetCtrlBoxAnalogOutput.h>
#include <dsr_msgs/SetCtrlBoxAnalogOutputType.h>
#include <dsr_msgs/SetCtrlBoxDigitalOutput.h>
#include <dsr_msgs/SetCurrentTcp.h>
#include <dsr_msgs/SetCurrentTool.h>
#include <dsr_msgs/SetDesiredForce.h>
#include <dsr_msgs/SetModbusOutput.h>
#include <dsr_msgs/SetRTControlInput.h>
#include <dsr_msgs/SetRTControlOutput.h>
#include <dsr_msgs/SetRefCoord.h>
#include <dsr_msgs/SetRobotControl.h>
#include <dsr_msgs/SetRobotMode.h>
#include <dsr_msgs/SetRobotSpeedMode.h>
#include <dsr_msgs/SetRobotSystem.h>
#include <dsr_msgs/SetSafeStopResetType.h>
#include <dsr_msgs/SetSingularityHandling.h>
#include <dsr_msgs/SetStiffnessx.h>
#include <dsr_msgs/SetToolDigitalOutput.h>
#include <dsr_msgs/SetToolShape.h>
#include <dsr_msgs/SetUserCartCoord1.h>
#include <dsr_msgs/SetUserCartCoord2.h>
#include <dsr_msgs/SetUserCartCoord3.h>
#include <dsr_msgs/SetVelJRT.h>
#include <dsr_msgs/SetVelXRT.h>
#include <dsr_msgs/SpeedJRTStream.h>
#include <dsr_msgs/SpeedJStream.h>
#include <dsr_msgs/SpeedLRTStream.h>
#include <dsr_msgs/SpeedLStream.h>
#include <dsr_msgs/StartRTControl.h>
#include <dsr_msgs/StopRTControl.h>
#include <dsr_msgs/TaskComplianceCtrl.h>
#include <dsr_msgs/TorqueRTStream.h>
#include <dsr_msgs/Trans.h>
#include <dsr_msgs/WriteDataRT.h>
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
quoted("dsr_msgs/AlignAxis1");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::AlignAxis1>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::AlignAxis1::Request>("request",false);
component<::dsr_msgs::AlignAxis1::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/AlignAxis2");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::AlignAxis2>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::AlignAxis2::Request>("request",false);
component<::dsr_msgs::AlignAxis2::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/AlterMotion");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::AlterMotion>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::AlterMotion::Request>("request",false);
component<::dsr_msgs::AlterMotion::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/AlterMotionStream");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::dsr_msgs::AlterMotionStream>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::AlterMotionStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/CalcCoord");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::CalcCoord>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::CalcCoord::Request>("request",false);
component<::dsr_msgs::CalcCoord::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/ChangeOperationSpeed");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::ChangeOperationSpeed>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ChangeOperationSpeed::Request>("request",false);
component<::dsr_msgs::ChangeOperationSpeed::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/CheckForceCondition");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::CheckForceCondition>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::CheckForceCondition::Request>("request",false);
component<::dsr_msgs::CheckForceCondition::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/CheckMotion");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::CheckMotion>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::CheckMotion::Request>("request",false);
component<::dsr_msgs::CheckMotion::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/CheckOrientationCondition1");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::CheckOrientationCondition1>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::CheckOrientationCondition1::Request>("request",false);
component<::dsr_msgs::CheckOrientationCondition1::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/CheckOrientationCondition2");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::CheckOrientationCondition2>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::CheckOrientationCondition2::Request>("request",false);
component<::dsr_msgs::CheckOrientationCondition2::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/CheckPositionCondition");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::CheckPositionCondition>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::CheckPositionCondition::Request>("request",false);
component<::dsr_msgs::CheckPositionCondition::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
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
quoted("dsr_msgs/ConnectRTControl");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::ConnectRTControl>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ConnectRTControl::Request>("request",false);
component<::dsr_msgs::ConnectRTControl::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/CoordTransform");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::CoordTransform>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::CoordTransform::Request>("request",false);
component<::dsr_msgs::CoordTransform::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/DisableAlterMotion");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::DisableAlterMotion>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::DisableAlterMotion::Request>("request",false);
component<::dsr_msgs::DisableAlterMotion::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/DisconnectRTControl");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::DisconnectRTControl>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::DisconnectRTControl::Request>("request",false);
component<::dsr_msgs::DisconnectRTControl::Response>("response",true);
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
quoted("dsr_msgs/EnableAlterMotion");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::EnableAlterMotion>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::EnableAlterMotion::Request>("request",false);
component<::dsr_msgs::EnableAlterMotion::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/Fkin");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::Fkin>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::Fkin::Request>("request",false);
component<::dsr_msgs::Fkin::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetControlMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetControlMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetControlMode::Request>("request",false);
component<::dsr_msgs::GetControlMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetControlSpace");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetControlSpace>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetControlSpace::Request>("request",false);
component<::dsr_msgs::GetControlSpace::Response>("response",true);
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
quoted("dsr_msgs/GetCtrlBoxDigitalOutput");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetCtrlBoxDigitalOutput>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetCtrlBoxDigitalOutput::Request>("request",false);
component<::dsr_msgs::GetCtrlBoxDigitalOutput::Response>("response",true);
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
quoted("dsr_msgs/GetCurrentPosj");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetCurrentPosj>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetCurrentPosj::Request>("request",false);
component<::dsr_msgs::GetCurrentPosj::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetCurrentPosx");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetCurrentPosx>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetCurrentPosx::Request>("request",false);
component<::dsr_msgs::GetCurrentPosx::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetCurrentRotm");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetCurrentRotm>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetCurrentRotm::Request>("request",false);
component<::dsr_msgs::GetCurrentRotm::Response>("response",true);
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
quoted("dsr_msgs/GetCurrentToolFlangePosx");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetCurrentToolFlangePosx>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetCurrentToolFlangePosx::Request>("request",false);
component<::dsr_msgs::GetCurrentToolFlangePosx::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetCurrentVelj");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetCurrentVelj>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetCurrentVelj::Request>("request",false);
component<::dsr_msgs::GetCurrentVelj::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetCurrentVelx");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetCurrentVelx>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetCurrentVelx::Request>("request",false);
component<::dsr_msgs::GetCurrentVelx::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetDesiredPosj");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetDesiredPosj>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetDesiredPosj::Request>("request",false);
component<::dsr_msgs::GetDesiredPosj::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetDesiredPosx");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetDesiredPosx>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetDesiredPosx::Request>("request",false);
component<::dsr_msgs::GetDesiredPosx::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetDesiredVelj");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetDesiredVelj>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetDesiredVelj::Request>("request",false);
component<::dsr_msgs::GetDesiredVelj::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetDesiredVelx");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetDesiredVelx>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetDesiredVelx::Request>("request",false);
component<::dsr_msgs::GetDesiredVelx::Response>("response",true);
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
quoted("dsr_msgs/GetOrientationError");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetOrientationError>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetOrientationError::Request>("request",false);
component<::dsr_msgs::GetOrientationError::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetRTControlInputDataList");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetRTControlInputDataList>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetRTControlInputDataList::Request>("request",false);
component<::dsr_msgs::GetRTControlInputDataList::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetRTControlInputVersionList");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetRTControlInputVersionList>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetRTControlInputVersionList::Request>("request",false);
component<::dsr_msgs::GetRTControlInputVersionList::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetRTControlOutputDataList");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetRTControlOutputDataList>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetRTControlOutputDataList::Request>("request",false);
component<::dsr_msgs::GetRTControlOutputDataList::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetRTControlOutputVersionList");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetRTControlOutputVersionList>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetRTControlOutputVersionList::Request>("request",false);
component<::dsr_msgs::GetRTControlOutputVersionList::Response>("response",true);
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
quoted("dsr_msgs/GetSolutionSpace");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetSolutionSpace>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetSolutionSpace::Request>("request",false);
component<::dsr_msgs::GetSolutionSpace::Response>("response",true);
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
quoted("dsr_msgs/GetToolDigitalOutput");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetToolDigitalOutput>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetToolDigitalOutput::Request>("request",false);
component<::dsr_msgs::GetToolDigitalOutput::Response>("response",true);
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
quoted("dsr_msgs/GetUserCartCoord");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetUserCartCoord>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetUserCartCoord::Request>("request",false);
component<::dsr_msgs::GetUserCartCoord::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/GetWorkpieceWeight");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::GetWorkpieceWeight>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::GetWorkpieceWeight::Request>("request",false);
component<::dsr_msgs::GetWorkpieceWeight::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/Ikin");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::Ikin>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::Ikin::Request>("request",false);
component<::dsr_msgs::Ikin::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/IkinEx");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::IkinEx>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::IkinEx::Request>("request",false);
component<::dsr_msgs::IkinEx::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/IsDoneBoltTightening");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::IsDoneBoltTightening>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::IsDoneBoltTightening::Request>("request",false);
component<::dsr_msgs::IsDoneBoltTightening::Response>("response",true);
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
quoted("dsr_msgs/ManageAccessControl");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::ManageAccessControl>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ManageAccessControl::Request>("request",false);
component<::dsr_msgs::ManageAccessControl::Response>("response",true);
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
quoted("dsr_msgs/MoveHome");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::MoveHome>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::MoveHome::Request>("request",false);
component<::dsr_msgs::MoveHome::Response>("response",true);
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
quoted("dsr_msgs/OverwriteUserCartCoord");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::OverwriteUserCartCoord>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::OverwriteUserCartCoord::Request>("request",false);
component<::dsr_msgs::OverwriteUserCartCoord::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/ParallelAxis1");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::ParallelAxis1>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ParallelAxis1::Request>("request",false);
component<::dsr_msgs::ParallelAxis1::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/ParallelAxis2");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::ParallelAxis2>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ParallelAxis2::Request>("request",false);
component<::dsr_msgs::ParallelAxis2::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/ReadDataRT");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::ReadDataRT>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ReadDataRT::Request>("request",false);
component<::dsr_msgs::ReadDataRT::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/ReleaseComplianceCtrl");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::ReleaseComplianceCtrl>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ReleaseComplianceCtrl::Request>("request",false);
component<::dsr_msgs::ReleaseComplianceCtrl::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/ReleaseForce");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::ReleaseForce>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ReleaseForce::Request>("request",false);
component<::dsr_msgs::ReleaseForce::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/ReleaseProtectiveStop");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::ReleaseProtectiveStop>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ReleaseProtectiveStop::Request>("request",false);
component<::dsr_msgs::ReleaseProtectiveStop::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/ResetWorkpieceWeight");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::ResetWorkpieceWeight>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ResetWorkpieceWeight::Request>("request",false);
component<::dsr_msgs::ResetWorkpieceWeight::Response>("response",true);
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
quoted("dsr_msgs/RobotStateRT");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::dsr_msgs::RobotStateRT>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::RobotStateRT>("message",false);
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
quoted("dsr_msgs/ServoJRTStream");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::dsr_msgs::ServoJRTStream>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ServoJRTStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/ServoJStream");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::dsr_msgs::ServoJStream>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ServoJStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/ServoLRTStream");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::dsr_msgs::ServoLRTStream>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ServoLRTStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/ServoLStream");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::dsr_msgs::ServoLStream>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::ServoLStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetAccJRT");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetAccJRT>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetAccJRT::Request>("request",false);
component<::dsr_msgs::SetAccJRT::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetAccXRT");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetAccXRT>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetAccXRT::Request>("request",false);
component<::dsr_msgs::SetAccXRT::Response>("response",true);
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
quoted("dsr_msgs/SetDesiredForce");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetDesiredForce>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetDesiredForce::Request>("request",false);
component<::dsr_msgs::SetDesiredForce::Response>("response",true);
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
quoted("dsr_msgs/SetRTControlInput");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetRTControlInput>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetRTControlInput::Request>("request",false);
component<::dsr_msgs::SetRTControlInput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetRTControlOutput");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetRTControlOutput>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetRTControlOutput::Request>("request",false);
component<::dsr_msgs::SetRTControlOutput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetRefCoord");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetRefCoord>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetRefCoord::Request>("request",false);
component<::dsr_msgs::SetRefCoord::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetRobotControl");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetRobotControl>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetRobotControl::Request>("request",false);
component<::dsr_msgs::SetRobotControl::Response>("response",true);
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
quoted("dsr_msgs/SetSingularityHandling");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetSingularityHandling>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetSingularityHandling::Request>("request",false);
component<::dsr_msgs::SetSingularityHandling::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetStiffnessx");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetStiffnessx>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetStiffnessx::Request>("request",false);
component<::dsr_msgs::SetStiffnessx::Response>("response",true);
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
quoted("dsr_msgs/SetToolShape");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetToolShape>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetToolShape::Request>("request",false);
component<::dsr_msgs::SetToolShape::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetUserCartCoord1");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetUserCartCoord1>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetUserCartCoord1::Request>("request",false);
component<::dsr_msgs::SetUserCartCoord1::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetUserCartCoord2");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetUserCartCoord2>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetUserCartCoord2::Request>("request",false);
component<::dsr_msgs::SetUserCartCoord2::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetUserCartCoord3");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetUserCartCoord3>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetUserCartCoord3::Request>("request",false);
component<::dsr_msgs::SetUserCartCoord3::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetVelJRT");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetVelJRT>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetVelJRT::Request>("request",false);
component<::dsr_msgs::SetVelJRT::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SetVelXRT");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::SetVelXRT>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SetVelXRT::Request>("request",false);
component<::dsr_msgs::SetVelXRT::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SpeedJRTStream");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::dsr_msgs::SpeedJRTStream>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SpeedJRTStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SpeedJStream");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::dsr_msgs::SpeedJStream>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SpeedJStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SpeedLRTStream");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::dsr_msgs::SpeedLRTStream>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SpeedLRTStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/SpeedLStream");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::dsr_msgs::SpeedLStream>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::SpeedLStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/StartRTControl");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::StartRTControl>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::StartRTControl::Request>("request",false);
component<::dsr_msgs::StartRTControl::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/StopRTControl");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::StopRTControl>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::StopRTControl::Request>("request",false);
component<::dsr_msgs::StopRTControl::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/TaskComplianceCtrl");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::TaskComplianceCtrl>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::TaskComplianceCtrl::Request>("request",false);
component<::dsr_msgs::TaskComplianceCtrl::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/TorqueRTStream");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::dsr_msgs::TorqueRTStream>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::TorqueRTStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/Trans");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::Trans>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::Trans::Request>("request",false);
component<::dsr_msgs::Trans::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs/WriteDataRT");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::dsr_msgs::WriteDataRT>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs::WriteDataRT::Request>("request",false);
component<::dsr_msgs::WriteDataRT::Response>("response",true);
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
