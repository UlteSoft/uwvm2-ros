/** @file Explicit complete-family selection for copied telemetry examples.
 * Select exactly one generated complete-domain header directory and define
 * UWVM_UNITREE_COMPLETE_FAMILY: 1=GO, 2=HG, 3=API topics, 4=legacy ROS1.
 * This chooses Source types and imports only. It creates no grant or endpoint,
 * identifies no physical robot, and never combines independent registries.
 */
#ifndef UWVM_ROBOTS_UNITREE_COMPLETE_FAMILY_H
#define UWVM_ROBOTS_UNITREE_COMPLETE_FAMILY_H
#if UWVM_UNITREE_COMPLETE_FAMILY == 1
#include <roswasm/ros2.h>
#include "bounded_records.h"
typedef roswasmc_bounded_2_unitree_go_msg_LowState uwvm_unitree_complete_value;
#define UWVM_UNITREE_COMPLETE_DECODE roswasmc_bounded_2_unitree_go_msg_LowState_decode
#define UWVM_UNITREE_COMPLETE_CAPACITY ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_LOWSTATE_MAX_WIRE_BYTES
#define UWVM_UNITREE_COMPLETE_ALIAS "unitree-lowstate"
#define UWVM_UNITREE_COMPLETE_BACKEND roswasmc_ros2_backend
#define UWVM_UNITREE_COMPLETE_VERSION uwvm_ros2_abi_version
#elif UWVM_UNITREE_COMPLETE_FAMILY == 2
#include <roswasm/ros2.h>
#include "bounded_records.h"
typedef roswasmc_bounded_2_unitree_hg_msg_LowState uwvm_unitree_complete_value;
#define UWVM_UNITREE_COMPLETE_DECODE roswasmc_bounded_2_unitree_hg_msg_LowState_decode
#define UWVM_UNITREE_COMPLETE_CAPACITY ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_LOWSTATE_MAX_WIRE_BYTES
#define UWVM_UNITREE_COMPLETE_ALIAS "unitree-lowstate"
#define UWVM_UNITREE_COMPLETE_BACKEND roswasmc_ros2_backend
#define UWVM_UNITREE_COMPLETE_VERSION uwvm_ros2_abi_version
#elif UWVM_UNITREE_COMPLETE_FAMILY == 3
#include <roswasm/ros2.h>
#include "bounded_records.h"
typedef roswasmc_bounded_2_unitree_api_msg_Response uwvm_unitree_complete_value;
#define UWVM_UNITREE_COMPLETE_DECODE roswasmc_bounded_2_unitree_api_msg_Response_decode
#define UWVM_UNITREE_COMPLETE_CAPACITY ROSWASMC_BOUNDED_2_UNITREE_API_MSG_RESPONSE_MAX_WIRE_BYTES
#define UWVM_UNITREE_COMPLETE_ALIAS "unitree-api-response"
#define UWVM_UNITREE_COMPLETE_BACKEND roswasmc_ros2_backend
#define UWVM_UNITREE_COMPLETE_VERSION uwvm_ros2_abi_version
#elif UWVM_UNITREE_COMPLETE_FAMILY == 4
#include <roswasm/ros1.h>
#include "bounded_records.h"
typedef roswasmc_bounded_1_unitree_legged_msgs_LowState uwvm_unitree_complete_value;
#define UWVM_UNITREE_COMPLETE_DECODE roswasmc_bounded_1_unitree_legged_msgs_LowState_decode
#define UWVM_UNITREE_COMPLETE_CAPACITY ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_LOWSTATE_MAX_WIRE_BYTES
#define UWVM_UNITREE_COMPLETE_ALIAS "unitree-legacy-lowstate"
#define UWVM_UNITREE_COMPLETE_BACKEND roswasmc_ros1_backend
#define UWVM_UNITREE_COMPLETE_VERSION uwvm_ros1_abi_version
#else
#error "Select exactly one complete Unitree family: 1=GO, 2=HG, 3=API, 4=legacy ROS1"
#endif
#endif
