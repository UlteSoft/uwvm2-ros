/** Cold typed source-constant proof. No Node, socket or authority. */
#pragma once
#include <string_view>
#include <cstdint>
namespace uwvm_industrial_source_dcedf5b42a6bf683255f97c231af66872381ece18cdc26237e07c4b39b3cbd21 {
inline bool validate() {
if(::scout_msgs::ScoutLightCmd::LIGHT_CONST_OFF!=UINT32_C(0))return false;
if(::scout_msgs::ScoutLightCmd::LIGHT_CONST_ON!=UINT32_C(1))return false;
if(::scout_msgs::ScoutLightCmd::LIGHT_BREATH!=UINT32_C(2))return false;
if(::scout_msgs::ScoutLightCmd::LIGHT_CUSTOM!=UINT32_C(3))return false;
if(::scout_msgs::ScoutStatus::MOTOR_ID_FRONT_RIGHT!=INT32_C(0))return false;
if(::scout_msgs::ScoutStatus::MOTOR_ID_FRONT_LEFT!=INT32_C(1))return false;
if(::scout_msgs::ScoutStatus::MOTOR_ID_REAR_RIGHT!=INT32_C(2))return false;
if(::scout_msgs::ScoutStatus::MOTOR_ID_REAR_LEFT!=INT32_C(3))return false;
if(::scout_msgs::ScoutStatus::LIGHT_ID_FRONT!=INT32_C(0))return false;
if(::scout_msgs::ScoutStatus::LIGHT_ID_REAR!=INT32_C(1))return false;
return true;}
}
