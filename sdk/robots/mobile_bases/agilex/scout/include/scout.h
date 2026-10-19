/* SPDX-License-Identifier: Apache-2.0
 * Complete selected SCOUT client values over the existing controlled ROS ABI.
 * Select exactly one UWVM_SCOUT_ROS_VERSION and its matching generated include
 * directory. This header opens no owner and never accesses a CAN device.
 */
#ifndef UWVM_AGILEX_SCOUT_H
#define UWVM_AGILEX_SCOUT_H
#include <math.h>
#include <roswasm/codec.h>
#include "bounded_records.h"

#if UWVM_SCOUT_ROS_VERSION == 1
#include <roswasm/ros1.h>
#define UWVM_SCOUT_BACKEND roswasmc_ros1_backend
#define UWVM_SCOUT_RECORD_IMPL(Name) roswasmc_bounded_1_scout_msgs_##Name
#define UWVM_SCOUT_ODOMETRY roswasmc_bounded_1_nav_msgs_Odometry
#define UWVM_SCOUT_REMOTE ScoutRsStatus
#elif UWVM_SCOUT_ROS_VERSION == 2
#include <roswasm/ros2.h>
#define UWVM_SCOUT_BACKEND roswasmc_ros2_backend
#define UWVM_SCOUT_RECORD_IMPL(Name) roswasmc_bounded_2_scout_msgs_msg_##Name
#define UWVM_SCOUT_ODOMETRY roswasmc_bounded_2_nav_msgs_msg_Odometry
#define UWVM_SCOUT_REMOTE ScoutRCState
#else
#error "Explicit UWVM_SCOUT_ROS_VERSION=1 or 2 is required; no backend fallback"
#endif
#define UWVM_SCOUT_RECORD(Name) UWVM_SCOUT_RECORD_IMPL(Name)
#define UWVM_SCOUT_JOIN_IMPL(A,B) A##B
#define UWVM_SCOUT_JOIN(A,B) UWVM_SCOUT_JOIN_IMPL(A,B)
typedef UWVM_SCOUT_RECORD(ScoutStatus) uwvm_scout_status;
typedef UWVM_SCOUT_RECORD(ScoutLightCmd) uwvm_scout_light;
typedef UWVM_SCOUT_RECORD(UWVM_SCOUT_REMOTE) uwvm_scout_remote;
typedef UWVM_SCOUT_ODOMETRY uwvm_scout_odometry;

/** Reject a capability from a different backend before dequeue or publication.
 * Alias/type authority still belongs to the Runtime; this local version check
 * prevents this selected source codec from consuming another backend's event.
 */
static inline int uwvm_scout_matches_backend(roswasmc_capability const* source) {
    return source&&source->context&&source->context->backend&&
        source->context->backend->version==UWVM_SCOUT_ROS_VERSION;
}

/** Fixed allocation shared by one serialized operation, never by callbacks.
 * The largest selected complete record is less than 1 KiB. Its exact returned
 * length is checked before publishing a decoded value; stale storage is ignored.
 */
typedef struct uwvm_scout_storage { unsigned char wire[1024]; } uwvm_scout_storage;

/** Model selection affects only admitted motion axes. It is not discovery,
 * protocol negotiation, a firmware assertion, or a grant to a robot resource.
 */
enum uwvm_scout_model { UWVM_SCOUT=1, UWVM_SCOUT_MINI=2, UWVM_SCOUT_MINI_OMNI=3 };
typedef struct uwvm_scout_motion_limits {
    uint32_t model;
    double forward, lateral, yaw;
} uwvm_scout_motion_limits;

/** Construct the existing builtin Twist, leaving the entire destination intact
 * on refusal. Limits are explicit operator inputs, not guessed vendor maxima.
 * Zero-initialized limits refuse. Non-Omni models reject lateral motion rather
 * than allowing their original driver to silently ignore it. All unused axes
 * are exactly zero. Native controller limits/watchdogs remain independent.
 */
static inline uint32_t uwvm_scout_make_motion(uwvm_scout_motion_limits const* limits,
        double forward,double lateral,double yaw,roswasmc_twist* output) {
    roswasmc_twist next={{0,0,0},{0,0,0}};
    if(!limits||!output||limits->model<UWVM_SCOUT||limits->model>UWVM_SCOUT_MINI_OMNI||
       !isfinite(limits->forward)||!isfinite(limits->lateral)||!isfinite(limits->yaw)||
       limits->forward<=0||limits->yaw<=0||limits->lateral<0||
       !isfinite(forward)||!isfinite(lateral)||!isfinite(yaw)||
       fabs(forward)>limits->forward||fabs(lateral)>limits->lateral||fabs(yaw)>limits->yaw||
       (limits->model!=UWVM_SCOUT_MINI_OMNI&&(limits->lateral!=0||lateral!=0)))return 22;
    next.linear[0]=forward;next.linear[1]=lateral;next.angular[2]=yaw;*output=next;return 0;
}

/** Preserve both original light-control boolean spellings, which differ by ROS
 * major. Mode values are the four original constants; custom values are bytes.
 * This constructor has no transport effect and preserves output on refusal.
 */
static inline uint32_t uwvm_scout_make_light(unsigned enabled,unsigned front_mode,
        unsigned front_value,unsigned rear_mode,unsigned rear_value,uwvm_scout_light* output) {
    uwvm_scout_light next={0};
    if(!output||enabled>1||front_mode>3||rear_mode>3||front_value>255||rear_value>255)return 22;
#if UWVM_SCOUT_ROS_VERSION == 1
    next.enable_cmd_light_control=(uint8_t)enabled;
#else
    next.cmd_ctrl_allowed=(uint8_t)enabled;
#endif
    next.front_mode=(uint8_t)front_mode;next.front_custom_value=(uint8_t)front_value;
    next.rear_mode=(uint8_t)rear_mode;next.rear_custom_value=(uint8_t)rear_value;*output=next;return 0;
}

/** One nonblocking event read, followed by the genuine whole generated decoder.
 * Idle 11 is not delivery. There is no sleep, retry, prefix decode or partial
 * assignment. The caller owns capability, output and nonoverlapping storage.
 */
#define UWVM_SCOUT_POLL(Name,Type,Prefix) \
static inline uint32_t uwvm_scout_poll_##Name(roswasmc_capability const* source, \
        Type* output,uwvm_scout_storage* storage) { \
    roswasmc_result result; \
    if(!uwvm_scout_matches_backend(source)||!output||!storage||uwvm_bounded_overlap(output,sizeof *output,storage,sizeof *storage))return 22; \
    result=roswasmc_poll(source,storage->wire,sizeof storage->wire); \
    if(result.error)return result.error; \
    if(!result.has_length||result.length>sizeof storage->wire)return 22; \
    return UWVM_SCOUT_JOIN(Prefix,_decode)(storage->wire,(size_t)result.length,output); \
}
UWVM_SCOUT_POLL(status,uwvm_scout_status,UWVM_SCOUT_RECORD(ScoutStatus))
UWVM_SCOUT_POLL(remote,uwvm_scout_remote,UWVM_SCOUT_RECORD(UWVM_SCOUT_REMOTE))
UWVM_SCOUT_POLL(odometry,uwvm_scout_odometry,UWVM_SCOUT_ODOMETRY)
#if UWVM_SCOUT_ROS_VERSION == 1
typedef UWVM_SCOUT_RECORD(ScoutBmsStatus) uwvm_scout_bms;
UWVM_SCOUT_POLL(bms,uwvm_scout_bms,UWVM_SCOUT_RECORD(ScoutBmsStatus))
#endif
#undef UWVM_SCOUT_POLL

/** Encode one complete original light value, then publish once through the
 * separately granted alias. A successful publish is not controller feedback.
 */
static inline uint32_t uwvm_scout_publish_light(roswasmc_capability const* source,
        uwvm_scout_light const* value,uwvm_scout_storage* storage) {
    size_t used=0;uint32_t code;
    if(!uwvm_scout_matches_backend(source)||!value||!storage||uwvm_bounded_overlap(value,sizeof *value,storage,sizeof *storage))return 22;
    code=UWVM_SCOUT_JOIN(UWVM_SCOUT_RECORD(ScoutLightCmd),_encode)(value,storage->wire,sizeof storage->wire,&used);
    return code?code:roswasmc_publish(source,storage->wire,used);
}
/** Publish an ordinary zero-velocity command once. This is neither a physical
 * E-Stop nor rollback; absence of feedback cannot establish physical stopping.
 * It still needs the exact independent topic.publish authority.
 */
static inline uint32_t uwvm_scout_stop(roswasmc_capability const* source) {
    roswasmc_twist value={{0,0,0},{0,0,0}};
    return uwvm_scout_matches_backend(source)?roswasmc_publish_twist(source,&value):22;
}
#endif
