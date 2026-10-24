/** Cold typed source-constant proof. No Node, socket or authority. */
#pragma once
#include <string_view>
#include <cstdint>
namespace uwvm_industrial_source_45c8fffcc11cf6f47cd7234d410f27e691d3aa21287f185b551f639e464ae70f {
inline bool validate() {
if(::aimdk_msgs::msg::CommonState::UNKNOWN!=INT32_C(0))return false;
if(::aimdk_msgs::msg::CommonState::SUCCESS!=INT32_C(1))return false;
if(::aimdk_msgs::msg::CommonState::FAILURE!=INT32_C(2))return false;
if(::aimdk_msgs::msg::CommonState::ABORTED!=INT32_C(3))return false;
if(::aimdk_msgs::msg::CommonState::TIMEOUT!=INT32_C(4))return false;
if(::aimdk_msgs::msg::CommonState::INVALID!=INT32_C(5))return false;
if(::aimdk_msgs::msg::CommonState::IN_MANUAL!=INT32_C(6))return false;
if(::aimdk_msgs::msg::CommonState::NOT_READY!=INT32_C(100))return false;
if(::aimdk_msgs::msg::CommonState::PENDING!=INT32_C(200))return false;
if(::aimdk_msgs::msg::CommonState::CREATED!=INT32_C(300))return false;
if(::aimdk_msgs::msg::CommonState::RUNNING!=INT32_C(400))return false;
if(::aimdk_msgs::msg::DomainErrorState::NONE!=UINT32_C(0))return false;
if(::aimdk_msgs::msg::DomainErrorState::DAMPING!=UINT32_C(1))return false;
if(::aimdk_msgs::msg::DomainErrorState::POWERDOWN!=UINT32_C(2))return false;
if(::aimdk_msgs::msg::DomainErrorState::DISABLE!=UINT32_C(3))return false;
if(::aimdk_msgs::msg::DomainErrorState::DISCONNECT!=UINT32_C(4))return false;
return true;}
}
