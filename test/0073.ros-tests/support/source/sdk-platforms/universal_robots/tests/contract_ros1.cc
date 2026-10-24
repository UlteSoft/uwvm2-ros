/* Typed C++ whole-value/transactional decode contracts; no ROS I/O. */
#include <cassert>
#include <cstring>
#include <span>
#include "io.hpp"
#define CHECK(T) do {T value={0},output,before;roswasmcpp::wire::bytes bytes;assert(roswasmcpp::Codec<T>::encode(value,bytes));assert(roswasmcpp::Codec<T>::decode(bytes,output));std::memset(&output,0x55,sizeof output);std::memcpy(&before,&output,sizeof output);for(std::size_t n=0;n<bytes.size();++n){assert(!roswasmcpp::Codec<T>::decode(std::span<unsigned char const>(bytes.data(),n),output));assert(!std::memcmp(&output,&before,sizeof output));}bytes.push_back(0xff);assert(!roswasmcpp::Codec<T>::decode(bytes,output));assert(!std::memcmp(&output,&before,sizeof output));}while(0)
int main(){
CHECK(uwvm_custom::Ros1Geometry_msgsPoint);
CHECK(uwvm_custom::Ros1Geometry_msgsQuaternion);
CHECK(uwvm_custom::Ros1Geometry_msgsPose);
CHECK(uwvm_custom::Ros1Time);
CHECK(uwvm_custom::Ros1Std_msgsHeader);
CHECK(uwvm_custom::Ros1Geometry_msgsPoseStamped);
CHECK(uwvm_custom::Ros1Geometry_msgsVector3);
CHECK(uwvm_custom::Ros1Geometry_msgsTwist);
CHECK(uwvm_custom::Ros1Geometry_msgsWrench);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsAddToLogRequest);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsAddToLogResponse);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsGetLoadedProgramRequest);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsGetLoadedProgramResponse);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsGetProgramStateRequest);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsProgramState);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsGetProgramStateResponse);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsGetRobotModeRequest);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsRobotMode);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsGetRobotModeResponse);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsGetSafetyModeRequest);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsSafetyMode);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsGetSafetyModeResponse);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsIsInRemoteControlRequest);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsIsInRemoteControlResponse);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsIsProgramRunningRequest);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsIsProgramRunningResponse);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsIsProgramSavedRequest);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsIsProgramSavedResponse);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsLoadRequest);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsLoadResponse);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsPopupRequest);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsPopupResponse);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsRawRequestRequest);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsRawRequestResponse);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsSetModeGoal);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsSetModeResult);
CHECK(uwvm_custom::Ros1Ur_dashboard_msgsSetModeFeedback);
CHECK(uwvm_custom::Ros1Ur_msgsAnalog);
CHECK(uwvm_custom::Ros1Ur_msgsDigital);
CHECK(uwvm_custom::Ros1Ur_msgsGetRobotSoftwareVersionRequest);
CHECK(uwvm_custom::Ros1Ur_msgsGetRobotSoftwareVersionResponse);
CHECK(uwvm_custom::Ros1Ur_msgsIOStates);
CHECK(uwvm_custom::Ros1Ur_msgsMasterboardDataMsg);
CHECK(uwvm_custom::Ros1Ur_msgsRobotModeDataMsg);
CHECK(uwvm_custom::Ros1Ur_msgsRobotStateRTMsg);
CHECK(uwvm_custom::Ros1Ur_msgsSetAnalogOutputRequest);
CHECK(uwvm_custom::Ros1Ur_msgsSetAnalogOutputResponse);
CHECK(uwvm_custom::Ros1Ur_msgsSetForceModeRequest);
CHECK(uwvm_custom::Ros1Ur_msgsSetForceModeResponse);
CHECK(uwvm_custom::Ros1Ur_msgsSetIORequest);
CHECK(uwvm_custom::Ros1Ur_msgsSetIOResponse);
CHECK(uwvm_custom::Ros1Ur_msgsSetPayloadRequest);
CHECK(uwvm_custom::Ros1Ur_msgsSetPayloadResponse);
CHECK(uwvm_custom::Ros1Ur_msgsSetSpeedSliderFractionRequest);
CHECK(uwvm_custom::Ros1Ur_msgsSetSpeedSliderFractionResponse);
CHECK(uwvm_custom::Ros1Ur_msgsToolDataMsg);
return 0;}
