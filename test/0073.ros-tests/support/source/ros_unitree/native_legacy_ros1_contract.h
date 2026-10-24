#pragma once
/** @file Compile-time contracts against the ACTUAL installed eleven-message family.
 *
 * Every assertion instantiates the selected catkin headers, rather than a copied
 * native struct or a source-text model. This is compiled in the witness TU.
 * It detects field/case/type/count drift, ROS2 family substitution, and missing
 * actual ROS1 generated traits. It does not execute ROS or authenticate MD5.
 * The real exporter and native map/SDK qualification remain independent gates.
 *
 * Contract source: unitree_ros_to_real/b989870124913091fbe75e0dcfb047eb4ca00a09,
 * all11 original .msg bodies retained in actual passive packet SHA256
 * 7af2979ea61d25e377a2c542ecb7d71ad646a2696cff04ad978376437cb512bf.
 */
#include <ros/message_traits.h>
#include <boost/array.hpp>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>
#include <unitree_legged_msgs/BmsCmd.h>
#include <unitree_legged_msgs/BmsState.h>
#include <unitree_legged_msgs/Cartesian.h>
#include <unitree_legged_msgs/HighCmd.h>
#include <unitree_legged_msgs/HighState.h>
#include <unitree_legged_msgs/IMU.h>
#include <unitree_legged_msgs/LED.h>
#include <unitree_legged_msgs/LowCmd.h>
#include <unitree_legged_msgs/LowState.h>
#include <unitree_legged_msgs/MotorCmd.h>
#include <unitree_legged_msgs/MotorState.h>

namespace uwvm_legacy_ros1_contract {
static_assert(sizeof(float)==4 && std::numeric_limits<float>::is_iec559,"Actual float32 IEEE representation required");
#define UWVM_LEGACY_FIELD(M,N,T) static_assert(std::is_same_v<decltype(std::declval<unitree_legged_msgs::M&>().N),T>,"Official legacy field type mismatch: " #M "." #N)
#define UWVM_LEGACY_ARRAY(M,N,T,C) static_assert(std::is_same_v<decltype(std::declval<unitree_legged_msgs::M&>().N),boost::array<T,C>>,"Official legacy fixed array mismatch: " #M "." #N)

// Original .msg SHA256 27fb6f7506dea1bdb64f67428ac1b10471fe7b874066a6d0e6a45a272399ecd2.
static_assert(ros::message_traits::IsMessage<unitree_legged_msgs::BmsCmd>::value,"Actual generated ROS1 message required");
static_assert(ros::message_traits::IsFixedSize<unitree_legged_msgs::BmsCmd>::value,"Official legacy family has no dynamic fields");
UWVM_LEGACY_FIELD(BmsCmd,off,std::uint8_t);
UWVM_LEGACY_ARRAY(BmsCmd,reserve,std::uint8_t,3);

// Original .msg SHA256 983f9f6b89aaa22f2bd276b0a47c4aae444efb59d645664c4b6673c805845bb2.
static_assert(ros::message_traits::IsMessage<unitree_legged_msgs::BmsState>::value,"Actual generated ROS1 message required");
static_assert(ros::message_traits::IsFixedSize<unitree_legged_msgs::BmsState>::value,"Official legacy family has no dynamic fields");
UWVM_LEGACY_FIELD(BmsState,version_h,std::uint8_t);
UWVM_LEGACY_FIELD(BmsState,version_l,std::uint8_t);
UWVM_LEGACY_FIELD(BmsState,bms_status,std::uint8_t);
UWVM_LEGACY_FIELD(BmsState,SOC,std::uint8_t);
UWVM_LEGACY_FIELD(BmsState,current,std::int32_t);
UWVM_LEGACY_FIELD(BmsState,cycle,std::uint16_t);
UWVM_LEGACY_ARRAY(BmsState,BQ_NTC,std::int8_t,2);
UWVM_LEGACY_ARRAY(BmsState,MCU_NTC,std::int8_t,2);
UWVM_LEGACY_ARRAY(BmsState,cell_vol,std::uint16_t,10);

// Original .msg SHA256 655ba746820eb8d51108feba96950738cfa9146b014344b2ca4b106f057779f6.
static_assert(ros::message_traits::IsMessage<unitree_legged_msgs::Cartesian>::value,"Actual generated ROS1 message required");
static_assert(ros::message_traits::IsFixedSize<unitree_legged_msgs::Cartesian>::value,"Official legacy family has no dynamic fields");
UWVM_LEGACY_FIELD(Cartesian,x,float);
UWVM_LEGACY_FIELD(Cartesian,y,float);
UWVM_LEGACY_FIELD(Cartesian,z,float);

// Original .msg SHA256 25c94f0f6e6d6c21eb7363c7fdc107f447633be8b4a613fee83f30f866cddee9.
static_assert(ros::message_traits::IsMessage<unitree_legged_msgs::HighCmd>::value,"Actual generated ROS1 message required");
static_assert(ros::message_traits::IsFixedSize<unitree_legged_msgs::HighCmd>::value,"Official legacy family has no dynamic fields");
UWVM_LEGACY_ARRAY(HighCmd,head,std::uint8_t,2);
UWVM_LEGACY_FIELD(HighCmd,levelFlag,std::uint8_t);
UWVM_LEGACY_FIELD(HighCmd,frameReserve,std::uint8_t);
UWVM_LEGACY_ARRAY(HighCmd,SN,std::uint32_t,2);
UWVM_LEGACY_ARRAY(HighCmd,version,std::uint32_t,2);
UWVM_LEGACY_FIELD(HighCmd,bandWidth,std::uint16_t);
UWVM_LEGACY_FIELD(HighCmd,mode,std::uint8_t);
UWVM_LEGACY_FIELD(HighCmd,gaitType,std::uint8_t);
UWVM_LEGACY_FIELD(HighCmd,speedLevel,std::uint8_t);
UWVM_LEGACY_FIELD(HighCmd,footRaiseHeight,float);
UWVM_LEGACY_FIELD(HighCmd,bodyHeight,float);
UWVM_LEGACY_ARRAY(HighCmd,position,float,2);
UWVM_LEGACY_ARRAY(HighCmd,euler,float,3);
UWVM_LEGACY_ARRAY(HighCmd,velocity,float,2);
UWVM_LEGACY_FIELD(HighCmd,yawSpeed,float);
UWVM_LEGACY_FIELD(HighCmd,bms,unitree_legged_msgs::BmsCmd);
UWVM_LEGACY_ARRAY(HighCmd,led,unitree_legged_msgs::LED,4);
UWVM_LEGACY_ARRAY(HighCmd,wirelessRemote,std::uint8_t,40);
UWVM_LEGACY_FIELD(HighCmd,reserve,std::uint32_t);
UWVM_LEGACY_FIELD(HighCmd,crc,std::uint32_t);

// Original .msg SHA256 65d43eafa99cffb22481dd78236309d8bbb55db090f61a96af8a20ec63a00d62.
static_assert(ros::message_traits::IsMessage<unitree_legged_msgs::HighState>::value,"Actual generated ROS1 message required");
static_assert(ros::message_traits::IsFixedSize<unitree_legged_msgs::HighState>::value,"Official legacy family has no dynamic fields");
UWVM_LEGACY_ARRAY(HighState,head,std::uint8_t,2);
UWVM_LEGACY_FIELD(HighState,levelFlag,std::uint8_t);
UWVM_LEGACY_FIELD(HighState,frameReserve,std::uint8_t);
UWVM_LEGACY_ARRAY(HighState,SN,std::uint32_t,2);
UWVM_LEGACY_ARRAY(HighState,version,std::uint32_t,2);
UWVM_LEGACY_FIELD(HighState,bandWidth,std::uint16_t);
UWVM_LEGACY_FIELD(HighState,imu,unitree_legged_msgs::IMU);
UWVM_LEGACY_ARRAY(HighState,motorState,unitree_legged_msgs::MotorState,20);
UWVM_LEGACY_FIELD(HighState,bms,unitree_legged_msgs::BmsState);
UWVM_LEGACY_ARRAY(HighState,footForce,std::int16_t,4);
UWVM_LEGACY_ARRAY(HighState,footForceEst,std::int16_t,4);
UWVM_LEGACY_FIELD(HighState,mode,std::uint8_t);
UWVM_LEGACY_FIELD(HighState,progress,float);
UWVM_LEGACY_FIELD(HighState,gaitType,std::uint8_t);
UWVM_LEGACY_FIELD(HighState,footRaiseHeight,float);
UWVM_LEGACY_ARRAY(HighState,position,float,3);
UWVM_LEGACY_FIELD(HighState,bodyHeight,float);
UWVM_LEGACY_ARRAY(HighState,velocity,float,3);
UWVM_LEGACY_FIELD(HighState,yawSpeed,float);
UWVM_LEGACY_ARRAY(HighState,rangeObstacle,float,4);
UWVM_LEGACY_ARRAY(HighState,footPosition2Body,unitree_legged_msgs::Cartesian,4);
UWVM_LEGACY_ARRAY(HighState,footSpeed2Body,unitree_legged_msgs::Cartesian,4);
UWVM_LEGACY_ARRAY(HighState,wirelessRemote,std::uint8_t,40);
UWVM_LEGACY_FIELD(HighState,reserve,std::uint32_t);
UWVM_LEGACY_FIELD(HighState,crc,std::uint32_t);

// Original .msg SHA256 8592dac6be50ed4dc06a1a601bb139e0d17e641b62c912c23fe0c7dc1193dd64.
static_assert(ros::message_traits::IsMessage<unitree_legged_msgs::IMU>::value,"Actual generated ROS1 message required");
static_assert(ros::message_traits::IsFixedSize<unitree_legged_msgs::IMU>::value,"Official legacy family has no dynamic fields");
UWVM_LEGACY_ARRAY(IMU,quaternion,float,4);
UWVM_LEGACY_ARRAY(IMU,gyroscope,float,3);
UWVM_LEGACY_ARRAY(IMU,accelerometer,float,3);
UWVM_LEGACY_ARRAY(IMU,rpy,float,3);
UWVM_LEGACY_FIELD(IMU,temperature,std::int8_t);

// Original .msg SHA256 0123ebbfa35f57cba882f49119571774a51a838ba11dacef05f6e949835a3429.
static_assert(ros::message_traits::IsMessage<unitree_legged_msgs::LED>::value,"Actual generated ROS1 message required");
static_assert(ros::message_traits::IsFixedSize<unitree_legged_msgs::LED>::value,"Official legacy family has no dynamic fields");
UWVM_LEGACY_FIELD(LED,r,std::uint8_t);
UWVM_LEGACY_FIELD(LED,g,std::uint8_t);
UWVM_LEGACY_FIELD(LED,b,std::uint8_t);

// Original .msg SHA256 d8cae6ff55e5f7f6f9915499f5fcf5ba6e4acdcc7d320e285c15b6570217652d.
static_assert(ros::message_traits::IsMessage<unitree_legged_msgs::LowCmd>::value,"Actual generated ROS1 message required");
static_assert(ros::message_traits::IsFixedSize<unitree_legged_msgs::LowCmd>::value,"Official legacy family has no dynamic fields");
UWVM_LEGACY_ARRAY(LowCmd,head,std::uint8_t,2);
UWVM_LEGACY_FIELD(LowCmd,levelFlag,std::uint8_t);
UWVM_LEGACY_FIELD(LowCmd,frameReserve,std::uint8_t);
UWVM_LEGACY_ARRAY(LowCmd,SN,std::uint32_t,2);
UWVM_LEGACY_ARRAY(LowCmd,version,std::uint32_t,2);
UWVM_LEGACY_FIELD(LowCmd,bandWidth,std::uint16_t);
UWVM_LEGACY_ARRAY(LowCmd,motorCmd,unitree_legged_msgs::MotorCmd,20);
UWVM_LEGACY_FIELD(LowCmd,bms,unitree_legged_msgs::BmsCmd);
UWVM_LEGACY_ARRAY(LowCmd,wirelessRemote,std::uint8_t,40);
UWVM_LEGACY_FIELD(LowCmd,reserve,std::uint32_t);
UWVM_LEGACY_FIELD(LowCmd,crc,std::uint32_t);

// Original .msg SHA256 a5fc3a6a7f54d05439f352b3dfc9e5078d25d04691abd1dd5264a51ee20c89e3.
static_assert(ros::message_traits::IsMessage<unitree_legged_msgs::LowState>::value,"Actual generated ROS1 message required");
static_assert(ros::message_traits::IsFixedSize<unitree_legged_msgs::LowState>::value,"Official legacy family has no dynamic fields");
UWVM_LEGACY_ARRAY(LowState,head,std::uint8_t,2);
UWVM_LEGACY_FIELD(LowState,levelFlag,std::uint8_t);
UWVM_LEGACY_FIELD(LowState,frameReserve,std::uint8_t);
UWVM_LEGACY_ARRAY(LowState,SN,std::uint32_t,2);
UWVM_LEGACY_ARRAY(LowState,version,std::uint32_t,2);
UWVM_LEGACY_FIELD(LowState,bandWidth,std::uint16_t);
UWVM_LEGACY_FIELD(LowState,imu,unitree_legged_msgs::IMU);
UWVM_LEGACY_ARRAY(LowState,motorState,unitree_legged_msgs::MotorState,20);
UWVM_LEGACY_FIELD(LowState,bms,unitree_legged_msgs::BmsState);
UWVM_LEGACY_ARRAY(LowState,footForce,std::int16_t,4);
UWVM_LEGACY_ARRAY(LowState,footForceEst,std::int16_t,4);
UWVM_LEGACY_FIELD(LowState,tick,std::uint32_t);
UWVM_LEGACY_ARRAY(LowState,wirelessRemote,std::uint8_t,40);
UWVM_LEGACY_FIELD(LowState,reserve,std::uint32_t);
UWVM_LEGACY_FIELD(LowState,crc,std::uint32_t);
UWVM_LEGACY_ARRAY(LowState,eeForceRaw,unitree_legged_msgs::Cartesian,4);
UWVM_LEGACY_ARRAY(LowState,eeForce,unitree_legged_msgs::Cartesian,4);
UWVM_LEGACY_FIELD(LowState,position,unitree_legged_msgs::Cartesian);
UWVM_LEGACY_FIELD(LowState,velocity,unitree_legged_msgs::Cartesian);
UWVM_LEGACY_FIELD(LowState,velocity_w,unitree_legged_msgs::Cartesian);

// Original .msg SHA256 9a675cdb66ea59ff255bc3b59ccef4f953b6dc0148661e53829434cda5fc9fb4.
static_assert(ros::message_traits::IsMessage<unitree_legged_msgs::MotorCmd>::value,"Actual generated ROS1 message required");
static_assert(ros::message_traits::IsFixedSize<unitree_legged_msgs::MotorCmd>::value,"Official legacy family has no dynamic fields");
UWVM_LEGACY_FIELD(MotorCmd,mode,std::uint8_t);
UWVM_LEGACY_FIELD(MotorCmd,q,float);
UWVM_LEGACY_FIELD(MotorCmd,dq,float);
UWVM_LEGACY_FIELD(MotorCmd,tau,float);
UWVM_LEGACY_FIELD(MotorCmd,Kp,float);
UWVM_LEGACY_FIELD(MotorCmd,Kd,float);
UWVM_LEGACY_ARRAY(MotorCmd,reserve,std::uint32_t,3);

// Original .msg SHA256 0d94cb503bdfa1ee7fe38527fc8efa0aa7e2fc4c3361279615585caf9ec92f8e.
static_assert(ros::message_traits::IsMessage<unitree_legged_msgs::MotorState>::value,"Actual generated ROS1 message required");
static_assert(ros::message_traits::IsFixedSize<unitree_legged_msgs::MotorState>::value,"Official legacy family has no dynamic fields");
UWVM_LEGACY_FIELD(MotorState,mode,std::uint8_t);
UWVM_LEGACY_FIELD(MotorState,q,float);
UWVM_LEGACY_FIELD(MotorState,dq,float);
UWVM_LEGACY_FIELD(MotorState,ddq,float);
UWVM_LEGACY_FIELD(MotorState,tauEst,float);
UWVM_LEGACY_FIELD(MotorState,q_raw,float);
UWVM_LEGACY_FIELD(MotorState,dq_raw,float);
UWVM_LEGACY_FIELD(MotorState,ddq_raw,float);
UWVM_LEGACY_FIELD(MotorState,temperature,std::int8_t);
UWVM_LEGACY_ARRAY(MotorState,reserve,std::uint32_t,2);

#undef UWVM_LEGACY_ARRAY
#undef UWVM_LEGACY_FIELD

// These are actual generated traits observed by the executed native witness.
// Parent qualification must compare all11 to the genuine Noetic exporter;
// the hardcoded type spellings alone are not native identity evidence.
template<class Emit> inline void identities(Emit&& emit) {
    emit(ros::message_traits::DataType<unitree_legged_msgs::BmsCmd>::value(),ros::message_traits::MD5Sum<unitree_legged_msgs::BmsCmd>::value());
    emit(ros::message_traits::DataType<unitree_legged_msgs::BmsState>::value(),ros::message_traits::MD5Sum<unitree_legged_msgs::BmsState>::value());
    emit(ros::message_traits::DataType<unitree_legged_msgs::Cartesian>::value(),ros::message_traits::MD5Sum<unitree_legged_msgs::Cartesian>::value());
    emit(ros::message_traits::DataType<unitree_legged_msgs::HighCmd>::value(),ros::message_traits::MD5Sum<unitree_legged_msgs::HighCmd>::value());
    emit(ros::message_traits::DataType<unitree_legged_msgs::HighState>::value(),ros::message_traits::MD5Sum<unitree_legged_msgs::HighState>::value());
    emit(ros::message_traits::DataType<unitree_legged_msgs::IMU>::value(),ros::message_traits::MD5Sum<unitree_legged_msgs::IMU>::value());
    emit(ros::message_traits::DataType<unitree_legged_msgs::LED>::value(),ros::message_traits::MD5Sum<unitree_legged_msgs::LED>::value());
    emit(ros::message_traits::DataType<unitree_legged_msgs::LowCmd>::value(),ros::message_traits::MD5Sum<unitree_legged_msgs::LowCmd>::value());
    emit(ros::message_traits::DataType<unitree_legged_msgs::LowState>::value(),ros::message_traits::MD5Sum<unitree_legged_msgs::LowState>::value());
    emit(ros::message_traits::DataType<unitree_legged_msgs::MotorCmd>::value(),ros::message_traits::MD5Sum<unitree_legged_msgs::MotorCmd>::value());
    emit(ros::message_traits::DataType<unitree_legged_msgs::MotorState>::value(),ros::message_traits::MD5Sum<unitree_legged_msgs::MotorState>::value());
}
}
