/** Cold typed source-constant proof. No Node, socket or authority. */
#pragma once
#include <string_view>
#include <cstdint>
namespace uwvm_industrial_source_914fbb8df370c81fa64abb00a596b8c0fa1a71711765bf0ab2c7bd79d074f03f {
inline bool validate() {
if(::dsr_msgs2::srv::ServoOff::Request::STOP_TYPE_QUICK_STO!=INT32_C(0))return false;
if(::dsr_msgs2::srv::ServoOff::Request::STOP_TYPE_QUICK!=INT32_C(1))return false;
if(::dsr_msgs2::srv::ServoOff::Request::STOP_TYPE_SLOW!=INT32_C(2))return false;
if(::dsr_msgs2::srv::ServoOff::Request::STOP_TYPE_HOLD!=INT32_C(3))return false;
if(::dsr_msgs2::srv::ServoOff::Request::STOP_TYPE_EMERGENCY!=INT32_C(3))return false;
return true;}
}
