/** Cold typed source-constant proof. No Node, socket or authority. */
#pragma once
#include <string_view>
#include <cstdint>
namespace uwvm_industrial_source_02d2333c0389d69b82be45096c637e8d5a76d8ecf3f59e2d09854e5718d77cc0 {
inline bool validate() {
if(::franka_msgs::msg::TargetStatus::IDLE!=UINT32_C(0))return false;
if(::franka_msgs::msg::TargetStatus::EXECUTING!=UINT32_C(1))return false;
if(::franka_msgs::msg::TargetStatus::TARGET_REACHED!=UINT32_C(2))return false;
if(::franka_msgs::msg::TargetStatus::ABORTED!=UINT32_C(3))return false;
if(::franka_msgs::msg::FrankaRobotState::ROBOT_MODE_OTHER!=UINT32_C(0))return false;
if(::franka_msgs::msg::FrankaRobotState::ROBOT_MODE_IDLE!=UINT32_C(1))return false;
if(::franka_msgs::msg::FrankaRobotState::ROBOT_MODE_MOVE!=UINT32_C(2))return false;
if(::franka_msgs::msg::FrankaRobotState::ROBOT_MODE_GUIDING!=UINT32_C(3))return false;
if(::franka_msgs::msg::FrankaRobotState::ROBOT_MODE_REFLEX!=UINT32_C(4))return false;
if(::franka_msgs::msg::FrankaRobotState::ROBOT_MODE_USER_STOPPED!=UINT32_C(5))return false;
if(::franka_msgs::msg::FrankaRobotState::ROBOT_MODE_AUTOMATIC_ERROR_RECOVERY!=UINT32_C(6))return false;
return true;}
}
