/** Cold typed source-constant proof. No Node, socket or authority. */
#pragma once
#include <string_view>
#include <cstdint>
namespace uwvm_industrial_source_ab4765f6859963c56e54591604b89bfce0aa897d1c2952158247a6be7b93efb4 {
inline bool validate() {
if(::franka_msgs::FrankaState::ROBOT_MODE_OTHER!=UINT32_C(0))return false;
if(::franka_msgs::FrankaState::ROBOT_MODE_IDLE!=UINT32_C(1))return false;
if(::franka_msgs::FrankaState::ROBOT_MODE_MOVE!=UINT32_C(2))return false;
if(::franka_msgs::FrankaState::ROBOT_MODE_GUIDING!=UINT32_C(3))return false;
if(::franka_msgs::FrankaState::ROBOT_MODE_REFLEX!=UINT32_C(4))return false;
if(::franka_msgs::FrankaState::ROBOT_MODE_USER_STOPPED!=UINT32_C(5))return false;
if(::franka_msgs::FrankaState::ROBOT_MODE_AUTOMATIC_ERROR_RECOVERY!=UINT32_C(6))return false;
return true;}
}
