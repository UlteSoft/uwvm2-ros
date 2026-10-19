/** Cold typed source-constant proof. No Node, socket or authority. */
#pragma once
#include <string_view>
#include <cstdint>
namespace uwvm_industrial_source_67ac479248b31b87e28dddf4fa952884a5b4f1264c4be3eecf4d1e5fa60fd893 {
inline bool validate() {
if(::scout_msgs::msg::ScoutLightCmd::LIGHT_CONST_OFF!=UINT32_C(0))return false;
if(::scout_msgs::msg::ScoutLightCmd::LIGHT_CONST_ON!=UINT32_C(1))return false;
if(::scout_msgs::msg::ScoutLightCmd::LIGHT_BREATH!=UINT32_C(2))return false;
if(::scout_msgs::msg::ScoutLightCmd::LIGHT_CUSTOM!=UINT32_C(3))return false;
if(::scout_msgs::msg::ScoutStatus::MOTOR_ID_FRONT_RIGHT!=INT32_C(0))return false;
if(::scout_msgs::msg::ScoutStatus::MOTOR_ID_FRONT_LEFT!=INT32_C(1))return false;
if(::scout_msgs::msg::ScoutStatus::MOTOR_ID_REAR_RIGHT!=INT32_C(2))return false;
if(::scout_msgs::msg::ScoutStatus::MOTOR_ID_REAR_LEFT!=INT32_C(3))return false;
if(::scout_msgs::msg::ScoutStatus::LIGHT_ID_FRONT!=INT32_C(0))return false;
if(::scout_msgs::msg::ScoutStatus::LIGHT_ID_REAR!=INT32_C(1))return false;
return true;}
}
