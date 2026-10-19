/** Cold typed source-constant proof. No Node, socket or authority. */
#pragma once
#include <string_view>
#include <cstdint>
namespace uwvm_industrial_source_21e5abf791a9368095f80da1f2e9771085c51b525adaf2f3d055dbd61b6c412c {
inline bool validate() {
if(::control_msgs::msg::JointComponentTolerance::X_AXIS!=UINT32_C(1))return false;
if(::control_msgs::msg::JointComponentTolerance::Y_AXIS!=UINT32_C(2))return false;
if(::control_msgs::msg::JointComponentTolerance::Z_AXIS!=UINT32_C(3))return false;
if(::control_msgs::msg::JointComponentTolerance::TRANSLATION!=UINT32_C(4))return false;
if(::control_msgs::msg::JointComponentTolerance::ROTATION!=UINT32_C(5))return false;
if(::control_msgs::action::FollowJointTrajectory::Result::SUCCESSFUL!=INT32_C(0))return false;
if(::control_msgs::action::FollowJointTrajectory::Result::INVALID_GOAL!=-INT32_C(1))return false;
if(::control_msgs::action::FollowJointTrajectory::Result::INVALID_JOINTS!=-INT32_C(2))return false;
if(::control_msgs::action::FollowJointTrajectory::Result::OLD_HEADER_TIMESTAMP!=-INT32_C(3))return false;
if(::control_msgs::action::FollowJointTrajectory::Result::PATH_TOLERANCE_VIOLATED!=-INT32_C(4))return false;
if(::control_msgs::action::FollowJointTrajectory::Result::GOAL_TOLERANCE_VIOLATED!=-INT32_C(5))return false;
return true;}
}
