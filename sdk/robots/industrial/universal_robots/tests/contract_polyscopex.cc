/* Typed C++ whole-value/transactional decode contracts; no ROS I/O. */
#include <cassert>
#include <cstring>
#include <span>
#include "io.hpp"
#define CHECK(T) do {T value={0},output,before;roswasmcpp::wire::bytes bytes;assert(roswasmcpp::Codec<T>::encode(value,bytes));assert(roswasmcpp::Codec<T>::decode(bytes,output));std::memset(&output,0x55,sizeof output);std::memcpy(&before,&output,sizeof output);for(std::size_t n=0;n<bytes.size();++n){assert(!roswasmcpp::Codec<T>::decode(std::span<unsigned char const>(bytes.data(),n),output));assert(!std::memcmp(&output,&before,sizeof output));}bytes.push_back(0xff);assert(!roswasmcpp::Codec<T>::decode(bytes,output));assert(!std::memcmp(&output,&before,sizeof output));}while(0)
int main(){
CHECK(uwvm_custom::Ros2Builtin_interfacesMsgTime);
CHECK(uwvm_custom::Ros2Geometry_msgsMsgPoint);
CHECK(uwvm_custom::Ros2Geometry_msgsMsgQuaternion);
CHECK(uwvm_custom::Ros2Geometry_msgsMsgPose);
CHECK(uwvm_custom::Ros2Geometry_msgsMsgVector3);
CHECK(uwvm_custom::Ros2Geometry_msgsMsgTwist);
CHECK(uwvm_custom::Ros2Std_msgsMsgHeader);
CHECK(uwvm_custom::Ros2Std_msgsMsgString);
CHECK(uwvm_custom::Ros2UrinterfacesMsgRobotMode);
CHECK(uwvm_custom::Ros2UrinterfacesActionSetMode_Goal);
CHECK(uwvm_custom::Ros2UrinterfacesActionSetMode_Result);
CHECK(uwvm_custom::Ros2UrinterfacesActionSetMode_Feedback);
CHECK(uwvm_custom::Ros2UrinterfacesMsgActiveTCP);
CHECK(uwvm_custom::Ros2UrinterfacesMsgAnalog);
CHECK(uwvm_custom::Ros2UrinterfacesMsgAnalogIO);
CHECK(uwvm_custom::Ros2UrinterfacesMsgAnalogIOStateStamped);
CHECK(uwvm_custom::Ros2UrinterfacesMsgAnalogStamped);
CHECK(uwvm_custom::Ros2UrinterfacesMsgBoolStamped);
CHECK(uwvm_custom::Ros2UrinterfacesMsgControlCycle);
CHECK(uwvm_custom::Ros2UrinterfacesMsgCurrent);
CHECK(uwvm_custom::Ros2UrinterfacesMsgDigital);
CHECK(uwvm_custom::Ros2UrinterfacesMsgDigitalIO);
CHECK(uwvm_custom::Ros2UrinterfacesMsgDigitalIOStateStamped);
CHECK(uwvm_custom::Ros2UrinterfacesMsgDigitalStamped);
CHECK(uwvm_custom::Ros2UrinterfacesMsgDoubleMultiArrayStamped);
CHECK(uwvm_custom::Ros2UrinterfacesMsgFloat32Stamped);
CHECK(uwvm_custom::Ros2UrinterfacesMsgFloat64Stamped);
CHECK(uwvm_custom::Ros2UrinterfacesMsgJointMode);
CHECK(uwvm_custom::Ros2UrinterfacesMsgJointTemperature);
CHECK(uwvm_custom::Ros2UrinterfacesMsgJointVoltage);
CHECK(uwvm_custom::Ros2UrinterfacesMsgJsonStringType);
CHECK(uwvm_custom::Ros2UrinterfacesMsgRuntimeState);
CHECK(uwvm_custom::Ros2UrinterfacesMsgSafetyStatus);
CHECK(uwvm_custom::Ros2UrinterfacesMsgSharedVariablesEditResponse);
CHECK(uwvm_custom::Ros2UrinterfacesMsgSpeedSliderFraction);
CHECK(uwvm_custom::Ros2UrinterfacesMsgTCPState);
CHECK(uwvm_custom::Ros2UrinterfacesMsgToolOutputMode);
CHECK(uwvm_custom::Ros2UrinterfacesMsgVariableUpdate);
CHECK(uwvm_custom::Ros2UrinterfacesMsgVoltageCurrentStamped);
CHECK(uwvm_custom::Ros2UrinterfacesMsgWorldModelNames);
CHECK(uwvm_custom::Ros2UrinterfacesSrvSetAnalogOutput_Request);
CHECK(uwvm_custom::Ros2UrinterfacesSrvSetAnalogOutput_Response);
CHECK(uwvm_custom::Ros2UrinterfacesSrvSetDigitalOutput_Request);
CHECK(uwvm_custom::Ros2UrinterfacesSrvSetDigitalOutput_Response);
CHECK(uwvm_custom::Ros2UrinterfacesSrvSetSpeedFraction_Request);
CHECK(uwvm_custom::Ros2UrinterfacesSrvSetSpeedFraction_Response);
CHECK(uwvm_custom::Ros2UrinterfacesSrvSharedVariablesEditor_Request);
CHECK(uwvm_custom::Ros2UrinterfacesSrvSharedVariablesEditor_Response);
CHECK(uwvm_custom::Ros2UrinterfacesSrvVariablesViewer_Request);
CHECK(uwvm_custom::Ros2UrinterfacesSrvVariablesViewer_Response);
return 0;}
