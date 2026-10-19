/* SPDX-License-Identifier: Apache-2.0
 * Complete selected UR I/O values over the real Wasm ROS ABI.
 * Exactly one source variant is required. Constructors grant no authority.
 */
#ifndef UWVM_UR_IO_H
#define UWVM_UR_IO_H
#include <math.h>
#include <roswasm/codec.h>
#include "bounded_records.h"
#if (defined(UWVM_UR_ROS1)+defined(UWVM_UR_ROS2)+defined(UWVM_UR_POLYSCOPEX)) != 1
#error "Select exactly one UR source variant"
#endif
#if defined(UWVM_UR_ROS1)
#include <roswasm/ros1.h>
#define UWVM_UR_BACKEND roswasmc_ros1_backend
#define UWVM_UR_VERSION 1
typedef roswasmc_bounded_1_ur_msgs_IOStates uwvm_ur_io_state;
typedef roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest uwvm_ur_speed_request;
#define UWVM_UR_IO_DECODE roswasmc_bounded_1_ur_msgs_IOStates_decode
#define UWVM_UR_IO_ENCODE roswasmc_bounded_1_ur_msgs_IOStates_encode
#define UWVM_UR_SPEED_MEMBER speed_slider_fraction
#elif defined(UWVM_UR_ROS2)
#include <roswasm/ros2.h>
#define UWVM_UR_BACKEND roswasmc_ros2_backend
#define UWVM_UR_VERSION 2
typedef roswasmc_bounded_2_ur_msgs_msg_IOStates uwvm_ur_io_state;
typedef roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request uwvm_ur_speed_request;
#define UWVM_UR_IO_DECODE roswasmc_bounded_2_ur_msgs_msg_IOStates_decode
#define UWVM_UR_IO_ENCODE roswasmc_bounded_2_ur_msgs_msg_IOStates_encode
#define UWVM_UR_SPEED_MEMBER speed_slider_fraction
#elif defined(UWVM_UR_POLYSCOPEX)
#include <roswasm/ros2.h>
#define UWVM_UR_BACKEND roswasmc_ros2_backend
#define UWVM_UR_VERSION 2
typedef roswasmc_bounded_2_urinterfaces_msg_DigitalIOStateStamped uwvm_ur_io_state;
typedef roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_Request uwvm_ur_speed_request;
#define UWVM_UR_IO_DECODE roswasmc_bounded_2_urinterfaces_msg_DigitalIOStateStamped_decode
#define UWVM_UR_IO_ENCODE roswasmc_bounded_2_urinterfaces_msg_DigitalIOStateStamped_encode
#define UWVM_UR_SPEED_MEMBER value
#endif
/** Caller-owned, serialized, nonoverlapping scratch. Current selected full I/O
 * packets fit in 1 KiB; output is committed only after complete checked decode.
 */
typedef struct uwvm_ur_storage {unsigned char bytes[1024];} uwvm_ur_storage;
/** Pure request construction. This is not publication, a controller assertion,
 * or an implicit resume. Original speed-fraction range is [0,1], finite.
 */
static inline uint32_t uwvm_ur_make_speed(double fraction,uwvm_ur_speed_request* out) {
    uwvm_ur_speed_request next={0};
    if(!out||!isfinite(fraction)||fraction<0||fraction>1)return 22;
    next.UWVM_UR_SPEED_MEMBER=fraction;*out=next;return 0;
}
/** One genuine nonblocking poll. Idle11 is not data, and refusal never retries,
 * enlarges storage, replays an event or publishes a partially decoded output.
 */
static inline uint32_t uwvm_ur_poll_io(roswasmc_capability const* cap,
        uwvm_ur_io_state* out,uwvm_ur_storage* scratch) {
    roswasmc_result result;
    if(!cap||!cap->context||!cap->context->backend||cap->context->backend->version!=UWVM_UR_VERSION||
       !out||!scratch||uwvm_bounded_overlap(out,sizeof *out,scratch,sizeof *scratch))return 22;
    result=roswasmc_poll(cap,scratch->bytes,sizeof scratch->bytes);
    if(result.error)return result.error;
    if(!result.has_length||result.length>sizeof scratch->bytes)return 22;
    return UWVM_UR_IO_DECODE(scratch->bytes,(size_t)result.length,out);
}
#endif
