/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for franka_example_controllers/JointTorqueComparison. */
using Ros1Franka_example_controllersJointTorqueComparison=roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison;
/** Complete bounded guest value for franka_gripper/GraspEpsilon. */
using Ros1Franka_gripperGraspEpsilon=roswasmc_bounded_1_franka_gripper_GraspEpsilon;
/** Complete bounded guest value for franka_gripper/GraspGoal. */
using Ros1Franka_gripperGraspGoal=roswasmc_bounded_1_franka_gripper_GraspGoal;
/** Complete bounded guest value for franka_gripper/GraspResult. */
using Ros1Franka_gripperGraspResult=roswasmc_bounded_1_franka_gripper_GraspResult;
/** Complete bounded guest value for franka_gripper/GraspFeedback. */
using Ros1Franka_gripperGraspFeedback=roswasmc_bounded_1_franka_gripper_GraspFeedback;
/** Complete bounded guest value for franka_gripper/HomingGoal. */
using Ros1Franka_gripperHomingGoal=roswasmc_bounded_1_franka_gripper_HomingGoal;
/** Complete bounded guest value for franka_gripper/HomingResult. */
using Ros1Franka_gripperHomingResult=roswasmc_bounded_1_franka_gripper_HomingResult;
/** Complete bounded guest value for franka_gripper/HomingFeedback. */
using Ros1Franka_gripperHomingFeedback=roswasmc_bounded_1_franka_gripper_HomingFeedback;
/** Complete bounded guest value for franka_gripper/MoveGoal. */
using Ros1Franka_gripperMoveGoal=roswasmc_bounded_1_franka_gripper_MoveGoal;
/** Complete bounded guest value for franka_gripper/MoveResult. */
using Ros1Franka_gripperMoveResult=roswasmc_bounded_1_franka_gripper_MoveResult;
/** Complete bounded guest value for franka_gripper/MoveFeedback. */
using Ros1Franka_gripperMoveFeedback=roswasmc_bounded_1_franka_gripper_MoveFeedback;
/** Complete bounded guest value for franka_gripper/StopGoal. */
using Ros1Franka_gripperStopGoal=roswasmc_bounded_1_franka_gripper_StopGoal;
/** Complete bounded guest value for franka_gripper/StopResult. */
using Ros1Franka_gripperStopResult=roswasmc_bounded_1_franka_gripper_StopResult;
/** Complete bounded guest value for franka_gripper/StopFeedback. */
using Ros1Franka_gripperStopFeedback=roswasmc_bounded_1_franka_gripper_StopFeedback;
/** Complete bounded guest value for franka_msgs/ErrorRecoveryGoal. */
using Ros1Franka_msgsErrorRecoveryGoal=roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal;
/** Complete bounded guest value for franka_msgs/ErrorRecoveryResult. */
using Ros1Franka_msgsErrorRecoveryResult=roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult;
/** Complete bounded guest value for franka_msgs/ErrorRecoveryFeedback. */
using Ros1Franka_msgsErrorRecoveryFeedback=roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback;
/** Complete bounded guest value for franka_msgs/Errors. */
using Ros1Franka_msgsErrors=roswasmc_bounded_1_franka_msgs_Errors;
/** Complete bounded guest value for time. */
using Ros1Time=roswasmc_bounded_1_time;
/** Complete bounded guest value for std_msgs/Header. */
using Ros1Std_msgsHeader=roswasmc_bounded_1_std_msgs_Header;
/** Complete bounded guest value for franka_msgs/FrankaState. */
using Ros1Franka_msgsFrankaState=roswasmc_bounded_1_franka_msgs_FrankaState;
/** Complete bounded guest value for franka_msgs/SetCartesianImpedanceRequest. */
using Ros1Franka_msgsSetCartesianImpedanceRequest=roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest;
/** Complete bounded guest value for franka_msgs/SetCartesianImpedanceResponse. */
using Ros1Franka_msgsSetCartesianImpedanceResponse=roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse;
/** Complete bounded guest value for franka_msgs/SetEEFrameRequest. */
using Ros1Franka_msgsSetEEFrameRequest=roswasmc_bounded_1_franka_msgs_SetEEFrameRequest;
/** Complete bounded guest value for franka_msgs/SetEEFrameResponse. */
using Ros1Franka_msgsSetEEFrameResponse=roswasmc_bounded_1_franka_msgs_SetEEFrameResponse;
/** Complete bounded guest value for franka_msgs/SetForceTorqueCollisionBehaviorRequest. */
using Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest=roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest;
/** Complete bounded guest value for franka_msgs/SetForceTorqueCollisionBehaviorResponse. */
using Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse=roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse;
/** Complete bounded guest value for franka_msgs/SetFullCollisionBehaviorRequest. */
using Ros1Franka_msgsSetFullCollisionBehaviorRequest=roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest;
/** Complete bounded guest value for franka_msgs/SetFullCollisionBehaviorResponse. */
using Ros1Franka_msgsSetFullCollisionBehaviorResponse=roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse;
/** Complete bounded guest value for franka_msgs/SetJointImpedanceRequest. */
using Ros1Franka_msgsSetJointImpedanceRequest=roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest;
/** Complete bounded guest value for franka_msgs/SetJointImpedanceResponse. */
using Ros1Franka_msgsSetJointImpedanceResponse=roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse;
/** Complete bounded guest value for franka_msgs/SetKFrameRequest. */
using Ros1Franka_msgsSetKFrameRequest=roswasmc_bounded_1_franka_msgs_SetKFrameRequest;
/** Complete bounded guest value for franka_msgs/SetKFrameResponse. */
using Ros1Franka_msgsSetKFrameResponse=roswasmc_bounded_1_franka_msgs_SetKFrameResponse;
/** Complete bounded guest value for franka_msgs/SetLoadRequest. */
using Ros1Franka_msgsSetLoadRequest=roswasmc_bounded_1_franka_msgs_SetLoadRequest;
/** Complete bounded guest value for franka_msgs/SetLoadResponse. */
using Ros1Franka_msgsSetLoadResponse=roswasmc_bounded_1_franka_msgs_SetLoadResponse;
}
namespace roswasmcpp {
/** Checked codec for franka_example_controllers/JointTorqueComparison; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_example_controllersJointTorqueComparison> {
  static constexpr size_t max_wire_bytes=176;
  static constexpr char const* codec_identity="UWVMROS1_dde6eaf404002642e64af59eb5367948b00bc07c0b2fd8a57e359e9df3084254";
  static bool encode(uwvm_custom::Ros1Franka_example_controllersJointTorqueComparison const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_example_controllersJointTorqueComparison& out) {return !roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_gripper/GraspEpsilon; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_gripperGraspEpsilon> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_538ac801e9874841f232909d2d9a16893a8d29c8380cd623c00c408690f978f9";
  static bool encode(uwvm_custom::Ros1Franka_gripperGraspEpsilon const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_gripper_GraspEpsilon_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_gripper_GraspEpsilon_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_gripper_GraspEpsilon_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_gripperGraspEpsilon& out) {return !roswasmc_bounded_1_franka_gripper_GraspEpsilon_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_gripper/GraspGoal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_gripperGraspGoal> {
  static constexpr size_t max_wire_bytes=40;
  static constexpr char const* codec_identity="UWVMROS1_d3dd0d3f47e49e06a95d907a6219335b35234749518ab645e102257595ed7bcf";
  static bool encode(uwvm_custom::Ros1Franka_gripperGraspGoal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_gripper_GraspGoal_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_gripper_GraspGoal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_gripper_GraspGoal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_gripperGraspGoal& out) {return !roswasmc_bounded_1_franka_gripper_GraspGoal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_gripper/GraspResult; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_gripperGraspResult> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_9ce1e4f95f16fd6c1603f0b30a9fcc33e4f8436cdb96710ea91eb20b38a1e02e";
  static bool encode(uwvm_custom::Ros1Franka_gripperGraspResult const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_gripper_GraspResult_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_gripper_GraspResult_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_gripper_GraspResult_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_gripperGraspResult& out) {return !roswasmc_bounded_1_franka_gripper_GraspResult_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_gripper/GraspFeedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_gripperGraspFeedback> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_24ca23625df0bd9fb984e65c8ecd7a3af162a3db00c15951225df5e18d3a4d0d";
  static bool encode(uwvm_custom::Ros1Franka_gripperGraspFeedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_gripper_GraspFeedback_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_gripper_GraspFeedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_gripper_GraspFeedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_gripperGraspFeedback& out) {return !roswasmc_bounded_1_franka_gripper_GraspFeedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_gripper/HomingGoal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_gripperHomingGoal> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_4b4f861fa9ff44afda4217cf613720e90353b0d43c1fc3425d5937157353910f";
  static bool encode(uwvm_custom::Ros1Franka_gripperHomingGoal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_gripper_HomingGoal_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_gripper_HomingGoal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_gripper_HomingGoal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_gripperHomingGoal& out) {return !roswasmc_bounded_1_franka_gripper_HomingGoal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_gripper/HomingResult; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_gripperHomingResult> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_1ddf202b4e7dbf100d304b4eb32aa0ce8dca6ffadcf720820f2d7932af056477";
  static bool encode(uwvm_custom::Ros1Franka_gripperHomingResult const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_gripper_HomingResult_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_gripper_HomingResult_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_gripper_HomingResult_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_gripperHomingResult& out) {return !roswasmc_bounded_1_franka_gripper_HomingResult_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_gripper/HomingFeedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_gripperHomingFeedback> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_4a1118d8ab544b2bc99ff878356aae49b4d3ecb4cb5ba7e2f93f7c0c7a29512e";
  static bool encode(uwvm_custom::Ros1Franka_gripperHomingFeedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_gripper_HomingFeedback_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_gripper_HomingFeedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_gripper_HomingFeedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_gripperHomingFeedback& out) {return !roswasmc_bounded_1_franka_gripper_HomingFeedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_gripper/MoveGoal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_gripperMoveGoal> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_c83296f2540854e78cc159691c229f5d50267b3fdb31789e325a55fcd7d6830a";
  static bool encode(uwvm_custom::Ros1Franka_gripperMoveGoal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_gripper_MoveGoal_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_gripper_MoveGoal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_gripper_MoveGoal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_gripperMoveGoal& out) {return !roswasmc_bounded_1_franka_gripper_MoveGoal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_gripper/MoveResult; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_gripperMoveResult> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_527adf0720dabcc91866a0858cddfbfae1ed4203d4eb34a3965ebe085d0ac3b3";
  static bool encode(uwvm_custom::Ros1Franka_gripperMoveResult const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_gripper_MoveResult_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_gripper_MoveResult_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_gripper_MoveResult_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_gripperMoveResult& out) {return !roswasmc_bounded_1_franka_gripper_MoveResult_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_gripper/MoveFeedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_gripperMoveFeedback> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_8e02ca1d66ae38a5d71ca5f708aa2c57bb3df573d37b692e24986dba384c5f80";
  static bool encode(uwvm_custom::Ros1Franka_gripperMoveFeedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_gripper_MoveFeedback_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_gripper_MoveFeedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_gripper_MoveFeedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_gripperMoveFeedback& out) {return !roswasmc_bounded_1_franka_gripper_MoveFeedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_gripper/StopGoal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_gripperStopGoal> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_ecb09ccc03cb2140a457a11c174a614392ce1f9263e46c5b82427a99f7ff0245";
  static bool encode(uwvm_custom::Ros1Franka_gripperStopGoal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_gripper_StopGoal_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_gripper_StopGoal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_gripper_StopGoal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_gripperStopGoal& out) {return !roswasmc_bounded_1_franka_gripper_StopGoal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_gripper/StopResult; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_gripperStopResult> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_343e16d9b839b61ed8f8c1ce93005cb3a21511a220095200eb075dfb62a40c61";
  static bool encode(uwvm_custom::Ros1Franka_gripperStopResult const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_gripper_StopResult_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_gripper_StopResult_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_gripper_StopResult_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_gripperStopResult& out) {return !roswasmc_bounded_1_franka_gripper_StopResult_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_gripper/StopFeedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_gripperStopFeedback> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_62b637081e95f59df491f71b0ca84394a7e1f36137ca389a8f6d0d3a0f19038d";
  static bool encode(uwvm_custom::Ros1Franka_gripperStopFeedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_gripper_StopFeedback_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_gripper_StopFeedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_gripper_StopFeedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_gripperStopFeedback& out) {return !roswasmc_bounded_1_franka_gripper_StopFeedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/ErrorRecoveryGoal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsErrorRecoveryGoal> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_b4f11555bc4919bf59aa548bd21046bdc7b1e2ed5cce6dc46298eb7486ef7b64";
  static bool encode(uwvm_custom::Ros1Franka_msgsErrorRecoveryGoal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsErrorRecoveryGoal& out) {return !roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/ErrorRecoveryResult; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsErrorRecoveryResult> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_6f06297f59f06dc46e3f983b72101ae3c25cd054bca44cea1f6dd11a9aaa7794";
  static bool encode(uwvm_custom::Ros1Franka_msgsErrorRecoveryResult const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsErrorRecoveryResult& out) {return !roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/ErrorRecoveryFeedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsErrorRecoveryFeedback> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_3faee6d8afdc0524598cec0fe30fd51bd723e8e44849f035368723e59be87a21";
  static bool encode(uwvm_custom::Ros1Franka_msgsErrorRecoveryFeedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsErrorRecoveryFeedback& out) {return !roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/Errors; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsErrors> {
  static constexpr size_t max_wire_bytes=41;
  static constexpr char const* codec_identity="UWVMROS1_80e3d1157d53bf80d4db3621f9f22b35a1ef665382974599ff016709c6a629ca";
  static bool encode(uwvm_custom::Ros1Franka_msgsErrors const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_Errors_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_Errors_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_Errors_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsErrors& out) {return !roswasmc_bounded_1_franka_msgs_Errors_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for time; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Time> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_90b75571d2bcc21b824eb993b485a001bf97e281adda7907e1b2e3357e4804b0";
  static bool encode(uwvm_custom::Ros1Time const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_time_valid(&value))return false;size_t n=roswasmc_bounded_1_time_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_time_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Time& out) {return !roswasmc_bounded_1_time_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for std_msgs/Header; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Std_msgsHeader> {
  static constexpr size_t max_wire_bytes=144;
  static constexpr char const* codec_identity="UWVMROS1_ee2b4e736d36ebcdb562d3ef7832777233d39d366837e9148a76aa2c36756435";
  static bool encode(uwvm_custom::Ros1Std_msgsHeader const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_std_msgs_Header_valid(&value))return false;size_t n=roswasmc_bounded_1_std_msgs_Header_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_std_msgs_Header_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Std_msgsHeader& out) {return !roswasmc_bounded_1_std_msgs_Header_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/FrankaState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsFrankaState> {
  static constexpr size_t max_wire_bytes=2619;
  static constexpr char const* codec_identity="UWVMROS1_79f22f36ce2c11f519ee29e65a248ec1b67240e27712b2667e9b803a2562ee71";
  static bool encode(uwvm_custom::Ros1Franka_msgsFrankaState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_FrankaState_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_FrankaState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_FrankaState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsFrankaState& out) {return !roswasmc_bounded_1_franka_msgs_FrankaState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/SetCartesianImpedanceRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsSetCartesianImpedanceRequest> {
  static constexpr size_t max_wire_bytes=48;
  static constexpr char const* codec_identity="UWVMROS1_ff604aae9ab1ef36b5af107966e64ee2115ee17618a4ece6f15997c889cacbdf";
  static bool encode(uwvm_custom::Ros1Franka_msgsSetCartesianImpedanceRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsSetCartesianImpedanceRequest& out) {return !roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/SetCartesianImpedanceResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsSetCartesianImpedanceResponse> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_b091d2fe98c84528460ba3d84524c38355e725c161456edf0034e62c6f63ab5b";
  static bool encode(uwvm_custom::Ros1Franka_msgsSetCartesianImpedanceResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsSetCartesianImpedanceResponse& out) {return !roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/SetEEFrameRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsSetEEFrameRequest> {
  static constexpr size_t max_wire_bytes=128;
  static constexpr char const* codec_identity="UWVMROS1_1bc740d3998a14dd6c0604d5024ebda2b4916a8497950841b14a8073c6c1ecfc";
  static bool encode(uwvm_custom::Ros1Franka_msgsSetEEFrameRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_SetEEFrameRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_SetEEFrameRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_SetEEFrameRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsSetEEFrameRequest& out) {return !roswasmc_bounded_1_franka_msgs_SetEEFrameRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/SetEEFrameResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsSetEEFrameResponse> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_b40c9595d57d6ed52ea4eca78e21f2c3d81f5a0afaad2420511211931f7d1c67";
  static bool encode(uwvm_custom::Ros1Franka_msgsSetEEFrameResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_SetEEFrameResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_SetEEFrameResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_SetEEFrameResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsSetEEFrameResponse& out) {return !roswasmc_bounded_1_franka_msgs_SetEEFrameResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/SetForceTorqueCollisionBehaviorRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest> {
  static constexpr size_t max_wire_bytes=208;
  static constexpr char const* codec_identity="UWVMROS1_2513f0ea74bf6a871a85ce00c0f822891f2c0bed890b87e35d25a637aa62c4b5";
  static bool encode(uwvm_custom::Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest& out) {return !roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/SetForceTorqueCollisionBehaviorResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_72418d73d504ae385d7ddce7e3cef859a104346dbfea7fc9da82e0f7e33ca2f5";
  static bool encode(uwvm_custom::Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse& out) {return !roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/SetFullCollisionBehaviorRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsSetFullCollisionBehaviorRequest> {
  static constexpr size_t max_wire_bytes=416;
  static constexpr char const* codec_identity="UWVMROS1_47a86620ec5abdd6cc820b095d8f5f4214c1d9af00ac1ff0740137bbdda7217c";
  static bool encode(uwvm_custom::Ros1Franka_msgsSetFullCollisionBehaviorRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsSetFullCollisionBehaviorRequest& out) {return !roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/SetFullCollisionBehaviorResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsSetFullCollisionBehaviorResponse> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_26f46e286c42d51a8ef45db7c8b3bf315fb02faf7b2be5d3741563d43169a0e2";
  static bool encode(uwvm_custom::Ros1Franka_msgsSetFullCollisionBehaviorResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsSetFullCollisionBehaviorResponse& out) {return !roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/SetJointImpedanceRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsSetJointImpedanceRequest> {
  static constexpr size_t max_wire_bytes=56;
  static constexpr char const* codec_identity="UWVMROS1_29b5419f698b39452d38fbc58a0e3c572b8d285b70c8acf7d326c440eb417c53";
  static bool encode(uwvm_custom::Ros1Franka_msgsSetJointImpedanceRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsSetJointImpedanceRequest& out) {return !roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/SetJointImpedanceResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsSetJointImpedanceResponse> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_1ae0cd7c4ad89de09ccffd6c4999435fa6965509e99a9adb98b7f3ebb3ea4c55";
  static bool encode(uwvm_custom::Ros1Franka_msgsSetJointImpedanceResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsSetJointImpedanceResponse& out) {return !roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/SetKFrameRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsSetKFrameRequest> {
  static constexpr size_t max_wire_bytes=128;
  static constexpr char const* codec_identity="UWVMROS1_36afcdc6402259fb5a71ae1b27be0bb8623a64dd93caffa376adea7862c86446";
  static bool encode(uwvm_custom::Ros1Franka_msgsSetKFrameRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_SetKFrameRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_SetKFrameRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_SetKFrameRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsSetKFrameRequest& out) {return !roswasmc_bounded_1_franka_msgs_SetKFrameRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/SetKFrameResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsSetKFrameResponse> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_4ea77d3e827a9b9eaf3c3f9805f78c38bfe91789555d0469cffd4ff4266cd443";
  static bool encode(uwvm_custom::Ros1Franka_msgsSetKFrameResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_SetKFrameResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_SetKFrameResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_SetKFrameResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsSetKFrameResponse& out) {return !roswasmc_bounded_1_franka_msgs_SetKFrameResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/SetLoadRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsSetLoadRequest> {
  static constexpr size_t max_wire_bytes=104;
  static constexpr char const* codec_identity="UWVMROS1_d7c08dd9105cb7b83cb251e80c7d2dd1e9f7bb2a43f614bff6d00d808cccbcb5";
  static bool encode(uwvm_custom::Ros1Franka_msgsSetLoadRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_SetLoadRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_SetLoadRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_SetLoadRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsSetLoadRequest& out) {return !roswasmc_bounded_1_franka_msgs_SetLoadRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/SetLoadResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Franka_msgsSetLoadResponse> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_f18952407bf9e2813deeaa3734a781e90b74cb79892919c1218812ad71255cf3";
  static bool encode(uwvm_custom::Ros1Franka_msgsSetLoadResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_franka_msgs_SetLoadResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_franka_msgs_SetLoadResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_franka_msgs_SetLoadResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Franka_msgsSetLoadResponse& out) {return !roswasmc_bounded_1_franka_msgs_SetLoadResponse_decode(bytes.data(),bytes.size(),&out);}
};
}
