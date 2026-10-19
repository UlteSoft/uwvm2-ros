/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for mybuddy_communication/MybuddyAngles. */
using Ros1Mybuddy_communicationMybuddyAngles=roswasmc_bounded_1_mybuddy_communication_MybuddyAngles;
/** Complete bounded guest value for mybuddy_communication/MybuddyCoords. */
using Ros1Mybuddy_communicationMybuddyCoords=roswasmc_bounded_1_mybuddy_communication_MybuddyCoords;
/** Complete bounded guest value for mybuddy_communication/MybuddyGripperStatus. */
using Ros1Mybuddy_communicationMybuddyGripperStatus=roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus;
/** Complete bounded guest value for mybuddy_communication/MybuddyPumpStatus. */
using Ros1Mybuddy_communicationMybuddyPumpStatus=roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus;
/** Complete bounded guest value for mybuddy_communication/MybuddySetAngles. */
using Ros1Mybuddy_communicationMybuddySetAngles=roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles;
/** Complete bounded guest value for mybuddy_communication/MybuddySetCoords. */
using Ros1Mybuddy_communicationMybuddySetCoords=roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords;
/** Complete bounded guest value for mybuddy_communication/GetAnglesRequest. */
using Ros1Mybuddy_communicationGetAnglesRequest=roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest;
/** Complete bounded guest value for mybuddy_communication/GetAnglesResponse. */
using Ros1Mybuddy_communicationGetAnglesResponse=roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse;
/** Complete bounded guest value for mybuddy_communication/GetCoordsRequest. */
using Ros1Mybuddy_communicationGetCoordsRequest=roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest;
/** Complete bounded guest value for mybuddy_communication/GetCoordsResponse. */
using Ros1Mybuddy_communicationGetCoordsResponse=roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse;
/** Complete bounded guest value for mybuddy_communication/GripperStatusRequest. */
using Ros1Mybuddy_communicationGripperStatusRequest=roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest;
/** Complete bounded guest value for mybuddy_communication/GripperStatusResponse. */
using Ros1Mybuddy_communicationGripperStatusResponse=roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse;
/** Complete bounded guest value for mybuddy_communication/PumpStatusRequest. */
using Ros1Mybuddy_communicationPumpStatusRequest=roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest;
/** Complete bounded guest value for mybuddy_communication/PumpStatusResponse. */
using Ros1Mybuddy_communicationPumpStatusResponse=roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse;
/** Complete bounded guest value for mybuddy_communication/SetAnglesRequest. */
using Ros1Mybuddy_communicationSetAnglesRequest=roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest;
/** Complete bounded guest value for mybuddy_communication/SetAnglesResponse. */
using Ros1Mybuddy_communicationSetAnglesResponse=roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse;
/** Complete bounded guest value for mybuddy_communication/SetCoordsRequest. */
using Ros1Mybuddy_communicationSetCoordsRequest=roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest;
/** Complete bounded guest value for mybuddy_communication/SetCoordsResponse. */
using Ros1Mybuddy_communicationSetCoordsResponse=roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse;
/** Complete bounded guest value for mecharm_communication/MycobotAngles. */
using Ros1Mecharm_communicationMycobotAngles=roswasmc_bounded_1_mecharm_communication_MycobotAngles;
/** Complete bounded guest value for mecharm_communication/MycobotCoords. */
using Ros1Mecharm_communicationMycobotCoords=roswasmc_bounded_1_mecharm_communication_MycobotCoords;
/** Complete bounded guest value for mecharm_communication/MycobotGripperStatus. */
using Ros1Mecharm_communicationMycobotGripperStatus=roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus;
/** Complete bounded guest value for mecharm_communication/MycobotPumpStatus. */
using Ros1Mecharm_communicationMycobotPumpStatus=roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus;
/** Complete bounded guest value for mecharm_communication/MycobotSetAngles. */
using Ros1Mecharm_communicationMycobotSetAngles=roswasmc_bounded_1_mecharm_communication_MycobotSetAngles;
/** Complete bounded guest value for mecharm_communication/MycobotSetCoords. */
using Ros1Mecharm_communicationMycobotSetCoords=roswasmc_bounded_1_mecharm_communication_MycobotSetCoords;
/** Complete bounded guest value for mecharm_communication/GetAnglesRequest. */
using Ros1Mecharm_communicationGetAnglesRequest=roswasmc_bounded_1_mecharm_communication_GetAnglesRequest;
/** Complete bounded guest value for mecharm_communication/GetAnglesResponse. */
using Ros1Mecharm_communicationGetAnglesResponse=roswasmc_bounded_1_mecharm_communication_GetAnglesResponse;
/** Complete bounded guest value for mecharm_communication/GetCoordsRequest. */
using Ros1Mecharm_communicationGetCoordsRequest=roswasmc_bounded_1_mecharm_communication_GetCoordsRequest;
/** Complete bounded guest value for mecharm_communication/GetCoordsResponse. */
using Ros1Mecharm_communicationGetCoordsResponse=roswasmc_bounded_1_mecharm_communication_GetCoordsResponse;
/** Complete bounded guest value for mecharm_communication/GripperStatusRequest. */
using Ros1Mecharm_communicationGripperStatusRequest=roswasmc_bounded_1_mecharm_communication_GripperStatusRequest;
/** Complete bounded guest value for mecharm_communication/GripperStatusResponse. */
using Ros1Mecharm_communicationGripperStatusResponse=roswasmc_bounded_1_mecharm_communication_GripperStatusResponse;
/** Complete bounded guest value for mecharm_communication/PumpStatusRequest. */
using Ros1Mecharm_communicationPumpStatusRequest=roswasmc_bounded_1_mecharm_communication_PumpStatusRequest;
/** Complete bounded guest value for mecharm_communication/PumpStatusResponse. */
using Ros1Mecharm_communicationPumpStatusResponse=roswasmc_bounded_1_mecharm_communication_PumpStatusResponse;
/** Complete bounded guest value for mecharm_communication/SetAnglesRequest. */
using Ros1Mecharm_communicationSetAnglesRequest=roswasmc_bounded_1_mecharm_communication_SetAnglesRequest;
/** Complete bounded guest value for mecharm_communication/SetAnglesResponse. */
using Ros1Mecharm_communicationSetAnglesResponse=roswasmc_bounded_1_mecharm_communication_SetAnglesResponse;
/** Complete bounded guest value for mecharm_communication/SetCoordsRequest. */
using Ros1Mecharm_communicationSetCoordsRequest=roswasmc_bounded_1_mecharm_communication_SetCoordsRequest;
/** Complete bounded guest value for mecharm_communication/SetCoordsResponse. */
using Ros1Mecharm_communicationSetCoordsResponse=roswasmc_bounded_1_mecharm_communication_SetCoordsResponse;
/** Complete bounded guest value for myarm_communication/MyArmAngles. */
using Ros1Myarm_communicationMyArmAngles=roswasmc_bounded_1_myarm_communication_MyArmAngles;
/** Complete bounded guest value for myarm_communication/MyArmCoords. */
using Ros1Myarm_communicationMyArmCoords=roswasmc_bounded_1_myarm_communication_MyArmCoords;
/** Complete bounded guest value for myarm_communication/MyArmGripperStatus. */
using Ros1Myarm_communicationMyArmGripperStatus=roswasmc_bounded_1_myarm_communication_MyArmGripperStatus;
/** Complete bounded guest value for myarm_communication/MyArmPumpStatus. */
using Ros1Myarm_communicationMyArmPumpStatus=roswasmc_bounded_1_myarm_communication_MyArmPumpStatus;
/** Complete bounded guest value for myarm_communication/MyArmSetAngles. */
using Ros1Myarm_communicationMyArmSetAngles=roswasmc_bounded_1_myarm_communication_MyArmSetAngles;
/** Complete bounded guest value for myarm_communication/MyArmSetCoords. */
using Ros1Myarm_communicationMyArmSetCoords=roswasmc_bounded_1_myarm_communication_MyArmSetCoords;
/** Complete bounded guest value for myarm_communication/GetAnglesRequest. */
using Ros1Myarm_communicationGetAnglesRequest=roswasmc_bounded_1_myarm_communication_GetAnglesRequest;
/** Complete bounded guest value for myarm_communication/GetAnglesResponse. */
using Ros1Myarm_communicationGetAnglesResponse=roswasmc_bounded_1_myarm_communication_GetAnglesResponse;
/** Complete bounded guest value for myarm_communication/GetCoordsRequest. */
using Ros1Myarm_communicationGetCoordsRequest=roswasmc_bounded_1_myarm_communication_GetCoordsRequest;
/** Complete bounded guest value for myarm_communication/GetCoordsResponse. */
using Ros1Myarm_communicationGetCoordsResponse=roswasmc_bounded_1_myarm_communication_GetCoordsResponse;
/** Complete bounded guest value for myarm_communication/GripperStatusRequest. */
using Ros1Myarm_communicationGripperStatusRequest=roswasmc_bounded_1_myarm_communication_GripperStatusRequest;
/** Complete bounded guest value for myarm_communication/GripperStatusResponse. */
using Ros1Myarm_communicationGripperStatusResponse=roswasmc_bounded_1_myarm_communication_GripperStatusResponse;
/** Complete bounded guest value for myarm_communication/PumpStatusRequest. */
using Ros1Myarm_communicationPumpStatusRequest=roswasmc_bounded_1_myarm_communication_PumpStatusRequest;
/** Complete bounded guest value for myarm_communication/PumpStatusResponse. */
using Ros1Myarm_communicationPumpStatusResponse=roswasmc_bounded_1_myarm_communication_PumpStatusResponse;
/** Complete bounded guest value for myarm_communication/SetAnglesRequest. */
using Ros1Myarm_communicationSetAnglesRequest=roswasmc_bounded_1_myarm_communication_SetAnglesRequest;
/** Complete bounded guest value for myarm_communication/SetAnglesResponse. */
using Ros1Myarm_communicationSetAnglesResponse=roswasmc_bounded_1_myarm_communication_SetAnglesResponse;
/** Complete bounded guest value for myarm_communication/SetCoordsRequest. */
using Ros1Myarm_communicationSetCoordsRequest=roswasmc_bounded_1_myarm_communication_SetCoordsRequest;
/** Complete bounded guest value for myarm_communication/SetCoordsResponse. */
using Ros1Myarm_communicationSetCoordsResponse=roswasmc_bounded_1_myarm_communication_SetCoordsResponse;
/** Complete bounded guest value for mycobot_320_communication/MycobotAngles. */
using Ros1Mycobot_320_communicationMycobotAngles=roswasmc_bounded_1_mycobot_320_communication_MycobotAngles;
/** Complete bounded guest value for mycobot_320_communication/MycobotCoords. */
using Ros1Mycobot_320_communicationMycobotCoords=roswasmc_bounded_1_mycobot_320_communication_MycobotCoords;
/** Complete bounded guest value for mycobot_320_communication/MycobotGripperStatus. */
using Ros1Mycobot_320_communicationMycobotGripperStatus=roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus;
/** Complete bounded guest value for mycobot_320_communication/MycobotPumpStatus. */
using Ros1Mycobot_320_communicationMycobotPumpStatus=roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus;
/** Complete bounded guest value for mycobot_320_communication/MycobotSetAngles. */
using Ros1Mycobot_320_communicationMycobotSetAngles=roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles;
/** Complete bounded guest value for mycobot_320_communication/MycobotSetCoords. */
using Ros1Mycobot_320_communicationMycobotSetCoords=roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords;
/** Complete bounded guest value for mycobot_320_communication/GetAnglesRequest. */
using Ros1Mycobot_320_communicationGetAnglesRequest=roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest;
/** Complete bounded guest value for mycobot_320_communication/GetAnglesResponse. */
using Ros1Mycobot_320_communicationGetAnglesResponse=roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse;
/** Complete bounded guest value for mycobot_320_communication/GetCoordsRequest. */
using Ros1Mycobot_320_communicationGetCoordsRequest=roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest;
/** Complete bounded guest value for mycobot_320_communication/GetCoordsResponse. */
using Ros1Mycobot_320_communicationGetCoordsResponse=roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse;
/** Complete bounded guest value for mycobot_320_communication/GripperStatusRequest. */
using Ros1Mycobot_320_communicationGripperStatusRequest=roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest;
/** Complete bounded guest value for mycobot_320_communication/GripperStatusResponse. */
using Ros1Mycobot_320_communicationGripperStatusResponse=roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse;
/** Complete bounded guest value for mycobot_320_communication/PumpStatusRequest. */
using Ros1Mycobot_320_communicationPumpStatusRequest=roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest;
/** Complete bounded guest value for mycobot_320_communication/PumpStatusResponse. */
using Ros1Mycobot_320_communicationPumpStatusResponse=roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse;
/** Complete bounded guest value for mycobot_320_communication/SetAnglesRequest. */
using Ros1Mycobot_320_communicationSetAnglesRequest=roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest;
/** Complete bounded guest value for mycobot_320_communication/SetAnglesResponse. */
using Ros1Mycobot_320_communicationSetAnglesResponse=roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse;
/** Complete bounded guest value for mycobot_320_communication/SetCoordsRequest. */
using Ros1Mycobot_320_communicationSetCoordsRequest=roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest;
/** Complete bounded guest value for mycobot_320_communication/SetCoordsResponse. */
using Ros1Mycobot_320_communicationSetCoordsResponse=roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse;
/** Complete bounded guest value for mycobot_communication/MycobotAngles. */
using Ros1Mycobot_communicationMycobotAngles=roswasmc_bounded_1_mycobot_communication_MycobotAngles;
/** Complete bounded guest value for mycobot_communication/MycobotCoords. */
using Ros1Mycobot_communicationMycobotCoords=roswasmc_bounded_1_mycobot_communication_MycobotCoords;
/** Complete bounded guest value for mycobot_communication/MycobotGripperStatus. */
using Ros1Mycobot_communicationMycobotGripperStatus=roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus;
/** Complete bounded guest value for mycobot_communication/MycobotPumpStatus. */
using Ros1Mycobot_communicationMycobotPumpStatus=roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus;
/** Complete bounded guest value for mycobot_communication/MycobotSetAngles. */
using Ros1Mycobot_communicationMycobotSetAngles=roswasmc_bounded_1_mycobot_communication_MycobotSetAngles;
/** Complete bounded guest value for mycobot_communication/MycobotSetCoords. */
using Ros1Mycobot_communicationMycobotSetCoords=roswasmc_bounded_1_mycobot_communication_MycobotSetCoords;
/** Complete bounded guest value for mycobot_communication/GetAnglesRequest. */
using Ros1Mycobot_communicationGetAnglesRequest=roswasmc_bounded_1_mycobot_communication_GetAnglesRequest;
/** Complete bounded guest value for mycobot_communication/GetAnglesResponse. */
using Ros1Mycobot_communicationGetAnglesResponse=roswasmc_bounded_1_mycobot_communication_GetAnglesResponse;
/** Complete bounded guest value for mycobot_communication/GetCoordsRequest. */
using Ros1Mycobot_communicationGetCoordsRequest=roswasmc_bounded_1_mycobot_communication_GetCoordsRequest;
/** Complete bounded guest value for mycobot_communication/GetCoordsResponse. */
using Ros1Mycobot_communicationGetCoordsResponse=roswasmc_bounded_1_mycobot_communication_GetCoordsResponse;
/** Complete bounded guest value for mycobot_communication/GripperStatusRequest. */
using Ros1Mycobot_communicationGripperStatusRequest=roswasmc_bounded_1_mycobot_communication_GripperStatusRequest;
/** Complete bounded guest value for mycobot_communication/GripperStatusResponse. */
using Ros1Mycobot_communicationGripperStatusResponse=roswasmc_bounded_1_mycobot_communication_GripperStatusResponse;
/** Complete bounded guest value for mycobot_communication/PumpStatusRequest. */
using Ros1Mycobot_communicationPumpStatusRequest=roswasmc_bounded_1_mycobot_communication_PumpStatusRequest;
/** Complete bounded guest value for mycobot_communication/PumpStatusResponse. */
using Ros1Mycobot_communicationPumpStatusResponse=roswasmc_bounded_1_mycobot_communication_PumpStatusResponse;
/** Complete bounded guest value for mycobot_communication/SetAnglesRequest. */
using Ros1Mycobot_communicationSetAnglesRequest=roswasmc_bounded_1_mycobot_communication_SetAnglesRequest;
/** Complete bounded guest value for mycobot_communication/SetAnglesResponse. */
using Ros1Mycobot_communicationSetAnglesResponse=roswasmc_bounded_1_mycobot_communication_SetAnglesResponse;
/** Complete bounded guest value for mycobot_communication/SetCoordsRequest. */
using Ros1Mycobot_communicationSetCoordsRequest=roswasmc_bounded_1_mycobot_communication_SetCoordsRequest;
/** Complete bounded guest value for mycobot_communication/SetCoordsResponse. */
using Ros1Mycobot_communicationSetCoordsResponse=roswasmc_bounded_1_mycobot_communication_SetCoordsResponse;
/** Complete bounded guest value for mypalletizer_communication/MypalAngles. */
using Ros1Mypalletizer_communicationMypalAngles=roswasmc_bounded_1_mypalletizer_communication_MypalAngles;
/** Complete bounded guest value for mypalletizer_communication/MypalCoords. */
using Ros1Mypalletizer_communicationMypalCoords=roswasmc_bounded_1_mypalletizer_communication_MypalCoords;
/** Complete bounded guest value for mypalletizer_communication/MypalGripperStatus. */
using Ros1Mypalletizer_communicationMypalGripperStatus=roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus;
/** Complete bounded guest value for mypalletizer_communication/MypalPumpStatus. */
using Ros1Mypalletizer_communicationMypalPumpStatus=roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus;
/** Complete bounded guest value for mypalletizer_communication/MypalSetAngles. */
using Ros1Mypalletizer_communicationMypalSetAngles=roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles;
/** Complete bounded guest value for mypalletizer_communication/MypalSetCoords. */
using Ros1Mypalletizer_communicationMypalSetCoords=roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords;
/** Complete bounded guest value for mypalletizer_communication/GetAnglesRequest. */
using Ros1Mypalletizer_communicationGetAnglesRequest=roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest;
/** Complete bounded guest value for mypalletizer_communication/GetAnglesResponse. */
using Ros1Mypalletizer_communicationGetAnglesResponse=roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse;
/** Complete bounded guest value for mypalletizer_communication/GetCoordsRequest. */
using Ros1Mypalletizer_communicationGetCoordsRequest=roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest;
/** Complete bounded guest value for mypalletizer_communication/GetCoordsResponse. */
using Ros1Mypalletizer_communicationGetCoordsResponse=roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse;
/** Complete bounded guest value for mypalletizer_communication/GripperStatusRequest. */
using Ros1Mypalletizer_communicationGripperStatusRequest=roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest;
/** Complete bounded guest value for mypalletizer_communication/GripperStatusResponse. */
using Ros1Mypalletizer_communicationGripperStatusResponse=roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse;
/** Complete bounded guest value for mypalletizer_communication/PumpStatusRequest. */
using Ros1Mypalletizer_communicationPumpStatusRequest=roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest;
/** Complete bounded guest value for mypalletizer_communication/PumpStatusResponse. */
using Ros1Mypalletizer_communicationPumpStatusResponse=roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse;
/** Complete bounded guest value for mypalletizer_communication/SetAnglesRequest. */
using Ros1Mypalletizer_communicationSetAnglesRequest=roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest;
/** Complete bounded guest value for mypalletizer_communication/SetAnglesResponse. */
using Ros1Mypalletizer_communicationSetAnglesResponse=roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse;
/** Complete bounded guest value for mypalletizer_communication/SetCoordsRequest. */
using Ros1Mypalletizer_communicationSetCoordsRequest=roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest;
/** Complete bounded guest value for mypalletizer_communication/SetCoordsResponse. */
using Ros1Mypalletizer_communicationSetCoordsResponse=roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse;
/** Complete bounded guest value for ultraarm_communication/ultraArmAngles. */
using Ros1Ultraarm_communicationUltraArmAngles=roswasmc_bounded_1_ultraarm_communication_ultraArmAngles;
/** Complete bounded guest value for ultraarm_communication/ultraArmCoords. */
using Ros1Ultraarm_communicationUltraArmCoords=roswasmc_bounded_1_ultraarm_communication_ultraArmCoords;
/** Complete bounded guest value for ultraarm_communication/ultraArmGripperStatus. */
using Ros1Ultraarm_communicationUltraArmGripperStatus=roswasmc_bounded_1_ultraarm_communication_ultraArmGripperStatus;
/** Complete bounded guest value for ultraarm_communication/ultraArmPumpStatus. */
using Ros1Ultraarm_communicationUltraArmPumpStatus=roswasmc_bounded_1_ultraarm_communication_ultraArmPumpStatus;
/** Complete bounded guest value for ultraarm_communication/ultraArmSetAngles. */
using Ros1Ultraarm_communicationUltraArmSetAngles=roswasmc_bounded_1_ultraarm_communication_ultraArmSetAngles;
/** Complete bounded guest value for ultraarm_communication/ultraArmSetCoords. */
using Ros1Ultraarm_communicationUltraArmSetCoords=roswasmc_bounded_1_ultraarm_communication_ultraArmSetCoords;
/** Complete bounded guest value for ultraarm_communication/GetAnglesRequest. */
using Ros1Ultraarm_communicationGetAnglesRequest=roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest;
/** Complete bounded guest value for ultraarm_communication/GetAnglesResponse. */
using Ros1Ultraarm_communicationGetAnglesResponse=roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse;
/** Complete bounded guest value for ultraarm_communication/GetCoordsRequest. */
using Ros1Ultraarm_communicationGetCoordsRequest=roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest;
/** Complete bounded guest value for ultraarm_communication/GetCoordsResponse. */
using Ros1Ultraarm_communicationGetCoordsResponse=roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse;
/** Complete bounded guest value for ultraarm_communication/GripperStatusRequest. */
using Ros1Ultraarm_communicationGripperStatusRequest=roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest;
/** Complete bounded guest value for ultraarm_communication/GripperStatusResponse. */
using Ros1Ultraarm_communicationGripperStatusResponse=roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse;
/** Complete bounded guest value for ultraarm_communication/PumpStatusRequest. */
using Ros1Ultraarm_communicationPumpStatusRequest=roswasmc_bounded_1_ultraarm_communication_PumpStatusRequest;
/** Complete bounded guest value for ultraarm_communication/PumpStatusResponse. */
using Ros1Ultraarm_communicationPumpStatusResponse=roswasmc_bounded_1_ultraarm_communication_PumpStatusResponse;
/** Complete bounded guest value for ultraarm_communication/SetAnglesRequest. */
using Ros1Ultraarm_communicationSetAnglesRequest=roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest;
/** Complete bounded guest value for ultraarm_communication/SetAnglesResponse. */
using Ros1Ultraarm_communicationSetAnglesResponse=roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse;
/** Complete bounded guest value for ultraarm_communication/SetCoordsRequest. */
using Ros1Ultraarm_communicationSetCoordsRequest=roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest;
/** Complete bounded guest value for ultraarm_communication/SetCoordsResponse. */
using Ros1Ultraarm_communicationSetCoordsResponse=roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse;
}
namespace roswasmcpp {
/** Checked codec for mybuddy_communication/MybuddyAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationMybuddyAngles> {
  static constexpr size_t max_wire_bytes=52;
  static constexpr char const* codec_identity="UWVMROS1_f3c510869207e6d72e868ae35ef95d49a1eea5164c7ef1056cb3d335029563e9";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationMybuddyAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationMybuddyAngles& out) {return !roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mybuddy_communication/MybuddyCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationMybuddyCoords> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_a1df376733f2673e25fed814a038e00f556b82c2b8bce3755644904e50c95384";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationMybuddyCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_MybuddyCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_MybuddyCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_MybuddyCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationMybuddyCoords& out) {return !roswasmc_bounded_1_mybuddy_communication_MybuddyCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mybuddy_communication/MybuddyGripperStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationMybuddyGripperStatus> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_86fdaf7d3397b26155c891e15d2ebdd903d5899a0895128e720276eb35a63854";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationMybuddyGripperStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationMybuddyGripperStatus& out) {return !roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mybuddy_communication/MybuddyPumpStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationMybuddyPumpStatus> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_33b6419700f9baf41b584bb464d0234426a04e1bd86d198fe3d66c06845cd80a";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationMybuddyPumpStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationMybuddyPumpStatus& out) {return !roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mybuddy_communication/MybuddySetAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationMybuddySetAngles> {
  static constexpr size_t max_wire_bytes=53;
  static constexpr char const* codec_identity="UWVMROS1_8f41fddfb715bfaa95c90974cf257181a9ae3e2f36969f6fdfdf3ab03b7d4486";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationMybuddySetAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationMybuddySetAngles& out) {return !roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mybuddy_communication/MybuddySetCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationMybuddySetCoords> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_bf9f2ee073928aafabbd532644eb57eec70d4b864a3097e2ab1a37840abd3669";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationMybuddySetCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationMybuddySetCoords& out) {return !roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mybuddy_communication/GetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationGetAnglesRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_7ba90c12eb91dbcc296c0d61069d0dc12c3ac576c970cd2fe9a0fed29f058e38";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationGetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationGetAnglesRequest& out) {return !roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mybuddy_communication/GetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationGetAnglesResponse> {
  static constexpr size_t max_wire_bytes=52;
  static constexpr char const* codec_identity="UWVMROS1_98085b01a21ae26a74a286d790aab310530fff2a88d7a3c35e071df09abd709d";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationGetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationGetAnglesResponse& out) {return !roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mybuddy_communication/GetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationGetCoordsRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_e5b4bd6f4d17f79bf3477c2577e73355b871523291aace267fea201f5a2b43ef";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationGetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationGetCoordsRequest& out) {return !roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mybuddy_communication/GetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationGetCoordsResponse> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_604a9a58b11d6e51faee74226c4820db7668b2054b2e48949cf9deb5958f5873";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationGetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationGetCoordsResponse& out) {return !roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mybuddy_communication/GripperStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationGripperStatusRequest> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_7816f3fa4c8b7068eeb16e74940d2e26401df47b7bc6881d123f94b554523173";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationGripperStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationGripperStatusRequest& out) {return !roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mybuddy_communication/GripperStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationGripperStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_b329fd687ef106ed366d80c5651fc536ad266d2b16dee8f985f1bad32738330b";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationGripperStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationGripperStatusResponse& out) {return !roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mybuddy_communication/PumpStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationPumpStatusRequest> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_4261e7e11f0c6065de868444a0be45d363465b0913246b06b4a2c413714d77e9";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationPumpStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationPumpStatusRequest& out) {return !roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mybuddy_communication/PumpStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationPumpStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_975f537d88c880f8daa3459f6762dc6ec7d7eee1f580c0e79f3cb581b77cabd4";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationPumpStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationPumpStatusResponse& out) {return !roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mybuddy_communication/SetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationSetAnglesRequest> {
  static constexpr size_t max_wire_bytes=53;
  static constexpr char const* codec_identity="UWVMROS1_47d19578189892ca10e073d8d250839f1e2b39ebbb3525ed9b03a15af18eb240";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationSetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationSetAnglesRequest& out) {return !roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mybuddy_communication/SetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationSetAnglesResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_7371fa369708af552fd68522d7effdd00bf82bf7e2fe21469ceb210f51582f81";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationSetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationSetAnglesResponse& out) {return !roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mybuddy_communication/SetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationSetCoordsRequest> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_81f7315bebd612a9406a2805b393415b8a1956e49f3d1b8efb21f9e703b0bfe0";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationSetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationSetCoordsRequest& out) {return !roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mybuddy_communication/SetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mybuddy_communicationSetCoordsResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_e5c41ea9420e43a5406ddee23cee961347bfad4f83baf9ebac93d16cdb5d0b21";
  static bool encode(uwvm_custom::Ros1Mybuddy_communicationSetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mybuddy_communicationSetCoordsResponse& out) {return !roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/MycobotAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationMycobotAngles> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_4afa3dc66ce6ffe02170c76da688514a04aa7b47008aeb8f79c44f9ff867d189";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationMycobotAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_MycobotAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_MycobotAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_MycobotAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationMycobotAngles& out) {return !roswasmc_bounded_1_mecharm_communication_MycobotAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/MycobotCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationMycobotCoords> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_a6a82e22cd0a2f24657c0a253ed00bf31c0a266e6ebaab071cfd369780818d35";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationMycobotCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_MycobotCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_MycobotCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_MycobotCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationMycobotCoords& out) {return !roswasmc_bounded_1_mecharm_communication_MycobotCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/MycobotGripperStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationMycobotGripperStatus> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_07e04dad52b423c371dc5432b68a2f4272cf55328d6f0a10f091dc389d57a4bf";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationMycobotGripperStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationMycobotGripperStatus& out) {return !roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/MycobotPumpStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationMycobotPumpStatus> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_f559fbf7d5d2c872b3660132eb8c17c79ca3c2d27e975f8fb8fde1d86072a6fa";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationMycobotPumpStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationMycobotPumpStatus& out) {return !roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/MycobotSetAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationMycobotSetAngles> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_bce09b709e806417595c05e0e795d689b8c53008f9916633d934f935f9389de9";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationMycobotSetAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_MycobotSetAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_MycobotSetAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_MycobotSetAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationMycobotSetAngles& out) {return !roswasmc_bounded_1_mecharm_communication_MycobotSetAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/MycobotSetCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationMycobotSetCoords> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_68ffa6950e0934b845b2b45140612997c8276a3ac7ed4303e43cd70417491913";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationMycobotSetCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_MycobotSetCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_MycobotSetCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_MycobotSetCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationMycobotSetCoords& out) {return !roswasmc_bounded_1_mecharm_communication_MycobotSetCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/GetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationGetAnglesRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_6a8f60e71495c9944e1e97c308d5c992674bd46060b6f6abf2df3f9748c13123";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationGetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_GetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_GetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_GetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationGetAnglesRequest& out) {return !roswasmc_bounded_1_mecharm_communication_GetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/GetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationGetAnglesResponse> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_e792a206903dc49fab725e3b3507af3cf4712f10740e9dcceeb90dedfe6ea6e8";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationGetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_GetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_GetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_GetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationGetAnglesResponse& out) {return !roswasmc_bounded_1_mecharm_communication_GetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/GetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationGetCoordsRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_55d4d1b8965ea12f9b15d4bd5a22c43793d04af452fefb84bd98bc3d7d8da8bf";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationGetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_GetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_GetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_GetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationGetCoordsRequest& out) {return !roswasmc_bounded_1_mecharm_communication_GetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/GetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationGetCoordsResponse> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_2230e517685ea625507334067ccceb8424ecf782fcd4163c551d09048e900ca8";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationGetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_GetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_GetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_GetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationGetCoordsResponse& out) {return !roswasmc_bounded_1_mecharm_communication_GetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/GripperStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationGripperStatusRequest> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_98284fbac3bdb61857b9175b1f82adc7265d0ed418088123d79bf75cb04c9a2c";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationGripperStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_GripperStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_GripperStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_GripperStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationGripperStatusRequest& out) {return !roswasmc_bounded_1_mecharm_communication_GripperStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/GripperStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationGripperStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_29cfb0267fbe68d5dafcd3c9d16a8d4dab6b21477921a48ba35135bc0c22bb32";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationGripperStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_GripperStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_GripperStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_GripperStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationGripperStatusResponse& out) {return !roswasmc_bounded_1_mecharm_communication_GripperStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/PumpStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationPumpStatusRequest> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_aa138db91e46b41659dfc5901225aaaae1c1ae0461c35d3f7a90d0cf86c9920d";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationPumpStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_PumpStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_PumpStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_PumpStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationPumpStatusRequest& out) {return !roswasmc_bounded_1_mecharm_communication_PumpStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/PumpStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationPumpStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_876caa88c833bfa180b8747110683c789aa238c0cda162a8e262c66fddefdd8f";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationPumpStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_PumpStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_PumpStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_PumpStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationPumpStatusResponse& out) {return !roswasmc_bounded_1_mecharm_communication_PumpStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/SetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationSetAnglesRequest> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_7ad54edf4730457f2b2124c925b87a99bdbeddd8bfcc1e0d806f7b9dad31507b";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationSetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_SetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_SetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_SetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationSetAnglesRequest& out) {return !roswasmc_bounded_1_mecharm_communication_SetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/SetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationSetAnglesResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_940a294ef91d04afeb65b88bbdd91df3a7db0a07435ced3993db3a9d0f574c6e";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationSetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_SetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_SetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_SetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationSetAnglesResponse& out) {return !roswasmc_bounded_1_mecharm_communication_SetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/SetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationSetCoordsRequest> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_68deaf663a1fe2d3d7434935f10161eb444e7ea2f79e4b746fee9bd0383fc929";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationSetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_SetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_SetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_SetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationSetCoordsRequest& out) {return !roswasmc_bounded_1_mecharm_communication_SetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_communication/SetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mecharm_communicationSetCoordsResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_c66a6e14c4c7eba1b52d7ef57802cb6122a120f692bcc1552c51078301b1b409";
  static bool encode(uwvm_custom::Ros1Mecharm_communicationSetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mecharm_communication_SetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mecharm_communication_SetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mecharm_communication_SetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mecharm_communicationSetCoordsResponse& out) {return !roswasmc_bounded_1_mecharm_communication_SetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/MyArmAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationMyArmAngles> {
  static constexpr size_t max_wire_bytes=28;
  static constexpr char const* codec_identity="UWVMROS1_eff11a03e974b4af030c2dc92d0111cf95e0fb3ce004235bada3fe358b63fd4c";
  static bool encode(uwvm_custom::Ros1Myarm_communicationMyArmAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_MyArmAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_MyArmAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_MyArmAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationMyArmAngles& out) {return !roswasmc_bounded_1_myarm_communication_MyArmAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/MyArmCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationMyArmCoords> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_f0df1605988924f0263805fb2b27ab7ef9e2296691ed15e03a337606362c983c";
  static bool encode(uwvm_custom::Ros1Myarm_communicationMyArmCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_MyArmCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_MyArmCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_MyArmCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationMyArmCoords& out) {return !roswasmc_bounded_1_myarm_communication_MyArmCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/MyArmGripperStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationMyArmGripperStatus> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_7875df5b50417da1cef60d79c872aff0b40a08f9bbfe80c103729d3c4c892c6f";
  static bool encode(uwvm_custom::Ros1Myarm_communicationMyArmGripperStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_MyArmGripperStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_MyArmGripperStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_MyArmGripperStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationMyArmGripperStatus& out) {return !roswasmc_bounded_1_myarm_communication_MyArmGripperStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/MyArmPumpStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationMyArmPumpStatus> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_58765b94f1ecb3bb131f6480ea6830631cde9c96eeef94fd9b22598ccc9ea928";
  static bool encode(uwvm_custom::Ros1Myarm_communicationMyArmPumpStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_MyArmPumpStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_MyArmPumpStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_MyArmPumpStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationMyArmPumpStatus& out) {return !roswasmc_bounded_1_myarm_communication_MyArmPumpStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/MyArmSetAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationMyArmSetAngles> {
  static constexpr size_t max_wire_bytes=29;
  static constexpr char const* codec_identity="UWVMROS1_6acbfd19e3864b66e47b9da5c7c0e8890e7336e8d6c9a5657c4f5eaa5285fe42";
  static bool encode(uwvm_custom::Ros1Myarm_communicationMyArmSetAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_MyArmSetAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_MyArmSetAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_MyArmSetAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationMyArmSetAngles& out) {return !roswasmc_bounded_1_myarm_communication_MyArmSetAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/MyArmSetCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationMyArmSetCoords> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_ce10c29be4cfd832d5076fead55817a0ac7ccde002d5bbbd7a2befd95e9752fd";
  static bool encode(uwvm_custom::Ros1Myarm_communicationMyArmSetCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_MyArmSetCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_MyArmSetCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_MyArmSetCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationMyArmSetCoords& out) {return !roswasmc_bounded_1_myarm_communication_MyArmSetCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/GetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationGetAnglesRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_18ea682d1c475321b67435e9f207a14627113ad237a38c759cfe4e1c92fded37";
  static bool encode(uwvm_custom::Ros1Myarm_communicationGetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_GetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_GetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_GetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationGetAnglesRequest& out) {return !roswasmc_bounded_1_myarm_communication_GetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/GetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationGetAnglesResponse> {
  static constexpr size_t max_wire_bytes=28;
  static constexpr char const* codec_identity="UWVMROS1_0c68b293384d21d06e23d4aba10b4a4c2eff4e1a0eaa81903e026fe28da410c7";
  static bool encode(uwvm_custom::Ros1Myarm_communicationGetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_GetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_GetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_GetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationGetAnglesResponse& out) {return !roswasmc_bounded_1_myarm_communication_GetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/GetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationGetCoordsRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_04d9d79ad983cf4631cf65a48cae60d982cd0f234f0f4ed1afa0cc630bd63d95";
  static bool encode(uwvm_custom::Ros1Myarm_communicationGetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_GetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_GetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_GetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationGetCoordsRequest& out) {return !roswasmc_bounded_1_myarm_communication_GetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/GetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationGetCoordsResponse> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_752564704c08fb2076df8eea4f6f5b89621041372472f0f99b6627fdfda831b7";
  static bool encode(uwvm_custom::Ros1Myarm_communicationGetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_GetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_GetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_GetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationGetCoordsResponse& out) {return !roswasmc_bounded_1_myarm_communication_GetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/GripperStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationGripperStatusRequest> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_adec2e0fbf48862c2245aea4c31c59ea12abdec26b4727b14005b65006c58976";
  static bool encode(uwvm_custom::Ros1Myarm_communicationGripperStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_GripperStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_GripperStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_GripperStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationGripperStatusRequest& out) {return !roswasmc_bounded_1_myarm_communication_GripperStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/GripperStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationGripperStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_d703a0ec6f562f1deb444313e36e6a728b4e521d1196d90a63725753d5fe9659";
  static bool encode(uwvm_custom::Ros1Myarm_communicationGripperStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_GripperStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_GripperStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_GripperStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationGripperStatusResponse& out) {return !roswasmc_bounded_1_myarm_communication_GripperStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/PumpStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationPumpStatusRequest> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_eac2f5c006a0fa86df638c52e9cfb7d2c17818ec10e32ad5277af216c6c944f4";
  static bool encode(uwvm_custom::Ros1Myarm_communicationPumpStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_PumpStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_PumpStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_PumpStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationPumpStatusRequest& out) {return !roswasmc_bounded_1_myarm_communication_PumpStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/PumpStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationPumpStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_28533ec27c01d85c9a4644959ea8e5c3f2480f5203a9545e96e4242158f08044";
  static bool encode(uwvm_custom::Ros1Myarm_communicationPumpStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_PumpStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_PumpStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_PumpStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationPumpStatusResponse& out) {return !roswasmc_bounded_1_myarm_communication_PumpStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/SetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationSetAnglesRequest> {
  static constexpr size_t max_wire_bytes=29;
  static constexpr char const* codec_identity="UWVMROS1_c15c81907fbace209686a98578019d893d86cecf4782427ac2c3858da8cd3e0a";
  static bool encode(uwvm_custom::Ros1Myarm_communicationSetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_SetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_SetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_SetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationSetAnglesRequest& out) {return !roswasmc_bounded_1_myarm_communication_SetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/SetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationSetAnglesResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_dbdbcb001f0478a4bba5ae9d7320d8dbfa4e36a5c1c130215ff5ab864c73af0f";
  static bool encode(uwvm_custom::Ros1Myarm_communicationSetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_SetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_SetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_SetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationSetAnglesResponse& out) {return !roswasmc_bounded_1_myarm_communication_SetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/SetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationSetCoordsRequest> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_86cfc6edb6b86f90a21818e824aea50b697e314687472e1e96e9ce5399ef66c6";
  static bool encode(uwvm_custom::Ros1Myarm_communicationSetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_SetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_SetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_SetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationSetCoordsRequest& out) {return !roswasmc_bounded_1_myarm_communication_SetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for myarm_communication/SetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Myarm_communicationSetCoordsResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_52a54371663141d4384dae8969b729baf097a0aa6105f263e78593743e44272d";
  static bool encode(uwvm_custom::Ros1Myarm_communicationSetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_myarm_communication_SetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_myarm_communication_SetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_myarm_communication_SetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Myarm_communicationSetCoordsResponse& out) {return !roswasmc_bounded_1_myarm_communication_SetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/MycobotAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationMycobotAngles> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_67ec61da0c2bf7ee461c21abbfcab14d74b84cb6c9a12430e2a306abdc65690a";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationMycobotAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationMycobotAngles& out) {return !roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/MycobotCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationMycobotCoords> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_1f2ada8331783764a52ed35c81c3ce9a2fdd260e2dffa2990751ce5156c3ad03";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationMycobotCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_MycobotCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_MycobotCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_MycobotCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationMycobotCoords& out) {return !roswasmc_bounded_1_mycobot_320_communication_MycobotCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/MycobotGripperStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationMycobotGripperStatus> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_efb4ccdb3ce35d67174700f3d3dd7332d0f7a834c5d76b6153e389e031907984";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationMycobotGripperStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationMycobotGripperStatus& out) {return !roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/MycobotPumpStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationMycobotPumpStatus> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_bb1d48067f44b7022820a6175afe05d5ba8caed9f750874a4e703389cb0d83f4";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationMycobotPumpStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationMycobotPumpStatus& out) {return !roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/MycobotSetAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationMycobotSetAngles> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_bf04b1bd24ab2f56c1835d1abe44b73fe023407c3ebdd3d35341242116a5d259";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationMycobotSetAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationMycobotSetAngles& out) {return !roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/MycobotSetCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationMycobotSetCoords> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_761235ef96bcc1c687ad8dd3f6d39982d1b0a7a24eaab3bb54feb9780b9e0092";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationMycobotSetCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationMycobotSetCoords& out) {return !roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/GetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationGetAnglesRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_fe13e75d9def5e6f7f831297c771cd4d319e1e9862b5026382fed23747b721b8";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationGetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationGetAnglesRequest& out) {return !roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/GetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationGetAnglesResponse> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_c0cb03f21add6e566a30129ed7ce3174fa86786859c472a848c6768678adc80a";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationGetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationGetAnglesResponse& out) {return !roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/GetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationGetCoordsRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_82b2ae7ba4bf2aff8cd6c88ae344b376604624612abdd633c930695c1d1650d6";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationGetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationGetCoordsRequest& out) {return !roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/GetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationGetCoordsResponse> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_821bac75a42a37fa06f51e57c09839f3557487dc8e883310601cb2e00bb6b05d";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationGetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationGetCoordsResponse& out) {return !roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/GripperStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationGripperStatusRequest> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_68d072c047f4f026459309282a40c78c5db51571126efb85b7ca9e9f121b5eb5";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationGripperStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationGripperStatusRequest& out) {return !roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/GripperStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationGripperStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_9c1387ec1a93d6674ac6dd735b2811957db57359e9e461dcedc70ae6fccc5544";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationGripperStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationGripperStatusResponse& out) {return !roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/PumpStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationPumpStatusRequest> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_23a6e68b36341b30ab13026cbddfd01b16d352105bb5faecd7c1ec6ad654ab60";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationPumpStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationPumpStatusRequest& out) {return !roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/PumpStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationPumpStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_037f53ebd0750c76a798ff17507759fcf71aa8a55496e5b151b8d977dff2c1c8";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationPumpStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationPumpStatusResponse& out) {return !roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/SetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationSetAnglesRequest> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_644a7c29bae881d5f29708b552dcd8fd4102289aa396614590bca33dbda9f7da";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationSetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationSetAnglesRequest& out) {return !roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/SetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationSetAnglesResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_57578e8427fc6a3a89aa595259e03c3498153482afd716dd44c0d09ee9f59f20";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationSetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationSetAnglesResponse& out) {return !roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/SetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationSetCoordsRequest> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_f398444b45a64155c410a44da67043cee13993d75beabd6bc104b23ccd1d6656";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationSetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationSetCoordsRequest& out) {return !roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_320_communication/SetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_320_communicationSetCoordsResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_3fef1bfe9036381f8ad45bc1490e96b4917df5689ccf3a84d66e5ba5145e56c4";
  static bool encode(uwvm_custom::Ros1Mycobot_320_communicationSetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_320_communicationSetCoordsResponse& out) {return !roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/MycobotAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationMycobotAngles> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_4cbda93a13d78e9c592327caa737dfecf33adf6db898015afd56ecf7732f9aa6";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationMycobotAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_MycobotAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_MycobotAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_MycobotAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationMycobotAngles& out) {return !roswasmc_bounded_1_mycobot_communication_MycobotAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/MycobotCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationMycobotCoords> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_1da0d6342a1f4079244a65ad413d4559047d33b049ce9da04f8ab0bd226410a2";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationMycobotCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_MycobotCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_MycobotCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_MycobotCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationMycobotCoords& out) {return !roswasmc_bounded_1_mycobot_communication_MycobotCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/MycobotGripperStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationMycobotGripperStatus> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_65edcc278c4e21355c62408ff310598b3dbdf03ed426e3da0f2ee0bc6910e649";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationMycobotGripperStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationMycobotGripperStatus& out) {return !roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/MycobotPumpStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationMycobotPumpStatus> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_0c0cb3c0fe97b9f9c65769a09f505ff6c27a60723598d656cd5f16255f36bfbd";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationMycobotPumpStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationMycobotPumpStatus& out) {return !roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/MycobotSetAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationMycobotSetAngles> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_0844a6cf5033b27ad5062741c939a7cfff63edeba34f7e56c8178e1fab85b334";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationMycobotSetAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationMycobotSetAngles& out) {return !roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/MycobotSetCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationMycobotSetCoords> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_39376ad94c0c9b213703a373b6a3b12fd0dad0ec20ff312143d30099842747a9";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationMycobotSetCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationMycobotSetCoords& out) {return !roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/GetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationGetAnglesRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_8915949f81b788e5797f3bc4935e33e9e9eae82c5db3a1211129e7ef03cd2c54";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationGetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationGetAnglesRequest& out) {return !roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/GetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationGetAnglesResponse> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_482e6b845d6913ab7ea12580a395caf0a9815b56f985eb3a0553263fb7fd4ff3";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationGetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationGetAnglesResponse& out) {return !roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/GetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationGetCoordsRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_54acc83a24f2f62d8ba9c14b1cea1703cb6097360720550cc438671282d45876";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationGetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationGetCoordsRequest& out) {return !roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/GetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationGetCoordsResponse> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_275b9bec563bb8578112b3680400523000a08a8e194044164e5334336b73d107";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationGetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationGetCoordsResponse& out) {return !roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/GripperStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationGripperStatusRequest> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_d2c94d1906d311981c72642d82bbea634cfed4eac8c0aef41219bf4f382d4585";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationGripperStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationGripperStatusRequest& out) {return !roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/GripperStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationGripperStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_3a75c0d75882836971ed4c51e50c7b88778fcfeadf9b30df211d5055eb767664";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationGripperStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationGripperStatusResponse& out) {return !roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/PumpStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationPumpStatusRequest> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_561f4bb1f74832111672bbfed1ef1266a91914fb1034437199f730f55324c65c";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationPumpStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationPumpStatusRequest& out) {return !roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/PumpStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationPumpStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_b5a1672fe12d2e2131034bb6da8751130b64ef6d48ea48ac0a046c506da452dd";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationPumpStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationPumpStatusResponse& out) {return !roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/SetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationSetAnglesRequest> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_10bcf04e47b743a985ffefc61dff4522f3718d2ac7170960c8ae6857aaa11cbd";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationSetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationSetAnglesRequest& out) {return !roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/SetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationSetAnglesResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_d407051eb285975d89ad71f39aede021fb564aecca0e0744126c9a599fe396b2";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationSetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationSetAnglesResponse& out) {return !roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/SetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationSetCoordsRequest> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_7a59a4e31eca60caea9f82142225c890a136c60a9e4c821e9463e89139945f7a";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationSetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationSetCoordsRequest& out) {return !roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/SetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationSetCoordsResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_1a936a95cb1bfd0a9158342618c40c587af58388abf3b679eb4bad2518aec6af";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationSetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationSetCoordsResponse& out) {return !roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/MypalAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationMypalAngles> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_1d45a33e05e948c1b58988c84afe359138c896119f82da0d737377f0272892d9";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationMypalAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_MypalAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_MypalAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_MypalAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationMypalAngles& out) {return !roswasmc_bounded_1_mypalletizer_communication_MypalAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/MypalCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationMypalCoords> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_c542d1a1a71224901044bc38ca2934021d811417c7c6937ca194995f67e59b80";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationMypalCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_MypalCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_MypalCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_MypalCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationMypalCoords& out) {return !roswasmc_bounded_1_mypalletizer_communication_MypalCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/MypalGripperStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationMypalGripperStatus> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_e37f608d4499ab2ed49618fc244831d274fadb23bd408d42619e3f8409ad2ee6";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationMypalGripperStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationMypalGripperStatus& out) {return !roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/MypalPumpStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationMypalPumpStatus> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_cdf79e610b5d49b7f73857e02c64688a71517f2fe8e3a96739b16a53ad4d0ebc";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationMypalPumpStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationMypalPumpStatus& out) {return !roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/MypalSetAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationMypalSetAngles> {
  static constexpr size_t max_wire_bytes=17;
  static constexpr char const* codec_identity="UWVMROS1_8d11ba2c705bd577a329a01d039b831162742efe790c93b6c2c2631699d04f98";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationMypalSetAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationMypalSetAngles& out) {return !roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/MypalSetCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationMypalSetCoords> {
  static constexpr size_t max_wire_bytes=18;
  static constexpr char const* codec_identity="UWVMROS1_51a2d485c8d2252f184eb27724e88c1ae06002d54454bd8ebf31962a79f7c448";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationMypalSetCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationMypalSetCoords& out) {return !roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/GetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationGetAnglesRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_d0412d8ccc8f724f2a0660d20e24a39f6445f0cb433e775058b6b092fde65de8";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationGetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationGetAnglesRequest& out) {return !roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/GetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationGetAnglesResponse> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_cb5a18aec2420c030d8193eb40d25691a2491b98606a7722156984e82f77f745";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationGetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationGetAnglesResponse& out) {return !roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/GetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationGetCoordsRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_47d45419f14ce576d9e8c2bb288c7b67bb161115e7b99a2c4752d09cbb8087b8";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationGetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationGetCoordsRequest& out) {return !roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/GetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationGetCoordsResponse> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_7fdad367484b5f89c812c2513e99373844dcd160a115e326fb1c597f221b53fb";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationGetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationGetCoordsResponse& out) {return !roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/GripperStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationGripperStatusRequest> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_48d661f8fd6adfcf1703123464fcfb9351f8f831c0779ffad103a63bcf7ea709";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationGripperStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationGripperStatusRequest& out) {return !roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/GripperStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationGripperStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_00ab8ceb190b588a9420c2e044da64857cfdb76e709e40e57835ab5732074e2b";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationGripperStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationGripperStatusResponse& out) {return !roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/PumpStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationPumpStatusRequest> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_6dd70636864c4450a066c1d75e67643331fac4c3e1625bce1b94c85ce7a35636";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationPumpStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationPumpStatusRequest& out) {return !roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/PumpStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationPumpStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_c2503f89c84c49db8dab2ac9f847ba9d09434d366fd8916a7d5f30ea161bb4c5";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationPumpStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationPumpStatusResponse& out) {return !roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/SetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationSetAnglesRequest> {
  static constexpr size_t max_wire_bytes=17;
  static constexpr char const* codec_identity="UWVMROS1_32072510c6deddb3f372b5f300068ea457916a4e0397b30951e421a1da641d5d";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationSetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationSetAnglesRequest& out) {return !roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/SetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationSetAnglesResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_1472b47adc3171a9f1938e52bc937af6dd6bb720d33e163b70ae066a89a3211c";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationSetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationSetAnglesResponse& out) {return !roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/SetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationSetCoordsRequest> {
  static constexpr size_t max_wire_bytes=18;
  static constexpr char const* codec_identity="UWVMROS1_2d608cac1a7129ca5cd68ff52170ed40258cb6d199f4842a268a1d7570f9bda1";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationSetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationSetCoordsRequest& out) {return !roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/SetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationSetCoordsResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_86900f8772e9609e1d2b882b6ab613ce9609be6c589e5b9574a57b8749231308";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationSetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationSetCoordsResponse& out) {return !roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/ultraArmAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationUltraArmAngles> {
  static constexpr size_t max_wire_bytes=12;
  static constexpr char const* codec_identity="UWVMROS1_8fa11b8169b634ea7c786bef6ee5d8f0d42488b9841f06104ca8fd378c0fa88d";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationUltraArmAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_ultraArmAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_ultraArmAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_ultraArmAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationUltraArmAngles& out) {return !roswasmc_bounded_1_ultraarm_communication_ultraArmAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/ultraArmCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationUltraArmCoords> {
  static constexpr size_t max_wire_bytes=12;
  static constexpr char const* codec_identity="UWVMROS1_4e540a1f7d66110adec19bcf3114c44bbc8574efbfd05e911de8d56cc64f24fa";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationUltraArmCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_ultraArmCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_ultraArmCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_ultraArmCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationUltraArmCoords& out) {return !roswasmc_bounded_1_ultraarm_communication_ultraArmCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/ultraArmGripperStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationUltraArmGripperStatus> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_60d9ded61184b6f51986afe42b42c786ffb71a13ee6af004c3836c6708b41f9c";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationUltraArmGripperStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_ultraArmGripperStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_ultraArmGripperStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_ultraArmGripperStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationUltraArmGripperStatus& out) {return !roswasmc_bounded_1_ultraarm_communication_ultraArmGripperStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/ultraArmPumpStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationUltraArmPumpStatus> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_f765502e0a7c0a4fe18049032fe9d314512de56bc2d52e4b1a8cbaf01fbc68e1";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationUltraArmPumpStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_ultraArmPumpStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_ultraArmPumpStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_ultraArmPumpStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationUltraArmPumpStatus& out) {return !roswasmc_bounded_1_ultraarm_communication_ultraArmPumpStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/ultraArmSetAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationUltraArmSetAngles> {
  static constexpr size_t max_wire_bytes=13;
  static constexpr char const* codec_identity="UWVMROS1_0536de7f7a45a8d0a19b16b78557ec77c4ce5f99e1141df3c617da847cbb079c";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationUltraArmSetAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_ultraArmSetAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_ultraArmSetAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_ultraArmSetAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationUltraArmSetAngles& out) {return !roswasmc_bounded_1_ultraarm_communication_ultraArmSetAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/ultraArmSetCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationUltraArmSetCoords> {
  static constexpr size_t max_wire_bytes=13;
  static constexpr char const* codec_identity="UWVMROS1_4e59dd853f51fe18f9401ec6d0fffaa50a00ba7c96a12b8d09c9bfd5ad68f818";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationUltraArmSetCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_ultraArmSetCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_ultraArmSetCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_ultraArmSetCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationUltraArmSetCoords& out) {return !roswasmc_bounded_1_ultraarm_communication_ultraArmSetCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/GetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationGetAnglesRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_fd6922aabe6ec7f82731d19efb0963753d86a8f412de96f8bc67ea2615a7c860";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationGetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationGetAnglesRequest& out) {return !roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/GetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationGetAnglesResponse> {
  static constexpr size_t max_wire_bytes=12;
  static constexpr char const* codec_identity="UWVMROS1_d7048c8ef416139a11a58bd7d5b865f4b6e0b5431c8e80cdd8c4c79de67cc128";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationGetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationGetAnglesResponse& out) {return !roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/GetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationGetCoordsRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_40a6535b429baf0bbe0bc642ade7a8048e693633388ccd061bf12b6d795bbbd1";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationGetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationGetCoordsRequest& out) {return !roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/GetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationGetCoordsResponse> {
  static constexpr size_t max_wire_bytes=12;
  static constexpr char const* codec_identity="UWVMROS1_9f19d8d2eb7284c4cbe2587ca0438d4680287759553d908340f5053ae4f4f036";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationGetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationGetCoordsResponse& out) {return !roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/GripperStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationGripperStatusRequest> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_c435d39bef04bd7022e048c0cf364d100bf7a92855f04c816d2a22f0e672d586";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationGripperStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationGripperStatusRequest& out) {return !roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/GripperStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationGripperStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_fa0c99ff37acea63c5d507174f9725a9341ffaa007e722e21a8e0b93097e56d4";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationGripperStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationGripperStatusResponse& out) {return !roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/PumpStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationPumpStatusRequest> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_03df5de115870b7679c85e5dc64f996694578f1b7b41387e531a49ee6626e8b2";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationPumpStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_PumpStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_PumpStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_PumpStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationPumpStatusRequest& out) {return !roswasmc_bounded_1_ultraarm_communication_PumpStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/PumpStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationPumpStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_693fd1ad36c534188f959fdb72df4f970432946c5706472c0bf41b27d054a4ee";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationPumpStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_PumpStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_PumpStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_PumpStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationPumpStatusResponse& out) {return !roswasmc_bounded_1_ultraarm_communication_PumpStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/SetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationSetAnglesRequest> {
  static constexpr size_t max_wire_bytes=13;
  static constexpr char const* codec_identity="UWVMROS1_fe8cdfa565c8aa8b77db9743af57d7c1ba14aa9dd31c8d4ca92bb7aec107bb42";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationSetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationSetAnglesRequest& out) {return !roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/SetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationSetAnglesResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_f1ecaff9a60a47ef978b0faf2b9b2b9166ae308b3df718b0536536004dc8a3b4";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationSetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationSetAnglesResponse& out) {return !roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/SetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationSetCoordsRequest> {
  static constexpr size_t max_wire_bytes=13;
  static constexpr char const* codec_identity="UWVMROS1_2a4670c426b6cf566795b0e874018769fb7e852bd104770dff3e735608c3e3d5";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationSetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationSetCoordsRequest& out) {return !roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ultraarm_communication/SetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ultraarm_communicationSetCoordsResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_02aa6c82612c5ffd43e890bb173a59245e00cd91dbc38303dcbdc0fb30683431";
  static bool encode(uwvm_custom::Ros1Ultraarm_communicationSetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ultraarm_communicationSetCoordsResponse& out) {return !roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
}
