/** Strong entry to the actual source-bound vendor exporter product.
 * The complete original source/constants are not filtered or projected. The
 * selected fresh bundle must contain the genuine generated native_identity.cc
 * and native_constants.hpp produced from the captured current parent/bounded
 * pair + vendor_profile.py. No empty shim or Native identity is supplied here.
 * Its real pre-stop TypeSupport warming, SIGSTOP/I gate and modern full graph
 * output are preserved verbatim by inclusion of that actual generated source.
 */
#include <booster_interface/msg/booster_api_req_msg.hpp>
#include <booster_interface/msg/booster_api_resp_msg.hpp>
#include <booster_interface/msg/button_event_msg.hpp>
static_assert(::booster_interface::msg::ButtonEventMsg::PRESS_DOWN==0,"Actual original constant differs from retained source");
static_assert(::booster_interface::msg::ButtonEventMsg::PRESS_UP==1,"Actual original constant differs from retained source");
static_assert(::booster_interface::msg::ButtonEventMsg::SINGLE_CLICK==0,"Actual original constant differs from retained source");
static_assert(::booster_interface::msg::ButtonEventMsg::DOUBLE_CLICK==1,"Actual original constant differs from retained source");
static_assert(::booster_interface::msg::ButtonEventMsg::TRIPLE_CLICK==0,"Actual original constant differs from retained source");
static_assert(::booster_interface::msg::ButtonEventMsg::LONG_PRESS_START==1,"Actual original constant differs from retained source");
static_assert(::booster_interface::msg::ButtonEventMsg::LONG_PRESS_HOLD==0,"Actual original constant differs from retained source");
static_assert(::booster_interface::msg::ButtonEventMsg::LONG_PRESS_END==1,"Actual original constant differs from retained source");
#include <booster_interface/msg/fall_down_state.hpp>
static_assert(::booster_interface::msg::FallDownState::IS_READY==0,"Actual original constant differs from retained source");
static_assert(::booster_interface::msg::FallDownState::IS_FALLING==1,"Actual original constant differs from retained source");
static_assert(::booster_interface::msg::FallDownState::HAS_FALLEN==2,"Actual original constant differs from retained source");
static_assert(::booster_interface::msg::FallDownState::IS_GETTING_UP==3,"Actual original constant differs from retained source");
#include <booster_interface/msg/hand_command.hpp>
#include <booster_interface/msg/hand_dds_msg.hpp>
#include <booster_interface/msg/hand_param.hpp>
#include <booster_interface/msg/imu_state.hpp>
#include <booster_interface/msg/low_cmd.hpp>
static_assert(::booster_interface::msg::LowCmd::CMD_TYPE_PARALLEL==0,"Actual original constant differs from retained source");
static_assert(::booster_interface::msg::LowCmd::CMD_TYPE_SERIAL==1,"Actual original constant differs from retained source");
#include <booster_interface/msg/low_state.hpp>
#include <booster_interface/msg/motor_cmd.hpp>
#include <booster_interface/msg/motor_state.hpp>
#include <booster_interface/msg/odometer.hpp>
#include <booster_interface/msg/raw_bytes_msg.hpp>
#include <booster_interface/msg/raw_bytes_stamped.hpp>
#include <booster_interface/msg/remote_controller_state.hpp>
#include <booster_interface/srv/agent_service.hpp>
#include <booster_interface/srv/rpc_service.hpp>
#include <std_msgs/msg/header.hpp>
#include <builtin_interfaces/msg/time.hpp>
#include <rosidl_runtime_c/type_description/type_description__struct.h>
#include "native_identity.cc"
