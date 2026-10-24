/** Cold typed source-constant proof. No Node, socket or authority. */
#pragma once
#include <string_view>
#include <cstdint>
namespace uwvm_industrial_source_d9478ef312f02eff39aab3e326e9bcc36349e4f5cd2f5b4334d3793360c56478 {
inline bool validate() {
if(::booster_interface::msg::ButtonEventMsg::PRESS_DOWN!=INT32_C(0))return false;
if(::booster_interface::msg::ButtonEventMsg::PRESS_UP!=INT32_C(1))return false;
if(::booster_interface::msg::ButtonEventMsg::SINGLE_CLICK!=INT32_C(0))return false;
if(::booster_interface::msg::ButtonEventMsg::DOUBLE_CLICK!=INT32_C(1))return false;
if(::booster_interface::msg::ButtonEventMsg::TRIPLE_CLICK!=INT32_C(0))return false;
if(::booster_interface::msg::ButtonEventMsg::LONG_PRESS_START!=INT32_C(1))return false;
if(::booster_interface::msg::ButtonEventMsg::LONG_PRESS_HOLD!=INT32_C(0))return false;
if(::booster_interface::msg::ButtonEventMsg::LONG_PRESS_END!=INT32_C(1))return false;
if(::booster_interface::msg::FallDownState::IS_READY!=UINT32_C(0))return false;
if(::booster_interface::msg::FallDownState::IS_FALLING!=UINT32_C(1))return false;
if(::booster_interface::msg::FallDownState::HAS_FALLEN!=UINT32_C(2))return false;
if(::booster_interface::msg::FallDownState::IS_GETTING_UP!=UINT32_C(3))return false;
if(::booster_interface::msg::LowCmd::CMD_TYPE_PARALLEL!=INT32_C(0))return false;
if(::booster_interface::msg::LowCmd::CMD_TYPE_SERIAL!=INT32_C(1))return false;
return true;}
}
