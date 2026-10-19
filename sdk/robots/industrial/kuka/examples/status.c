/**
 * Complete, read-only KUKA KssStatus reactor using the portable ROS2 C SDK.
 *
 * Build with the exact generated KUKA bounded_records.h and the shared
 * industrial examples CMake entry. The host must initialize this no-entry
 * reactor before invoking exports. initialize_case(0) checks default denial;
 * initialize_case(1) requests the separately authorized "vendor-status" alias
 * and subscribes once. Neither mode changes Runtime policy or controller state.
 *
 * Call poll_once() repeatedly from the embedding's executor. Error 11 means
 * that no sample is available yet; it preserves the previous complete sample.
 * A zero return publishes one newly decoded sample to the getters below. Other
 * errors close the owned capability and remain visible through primary_status()
 * and cleanup_status(). close_case() explicitly retires a successful session.
 *
 * Inspect sample_ready() before reading fields. sample_count() saturates at
 * UINT32_MAX and is application data, not a freshness or safety guarantee.
 * The original message has no timestamp. A stored sample, including one kept
 * after closure, must never be interpreted as current physical robot state.
 * Emergency stop, motion permission and recovery require independent native
 * and hardware supervision; no single field authorizes a robot action.
 */
#include <roswasm/ros2.h>
#include "bounded_records.h"

#define EXPORT(name) __attribute__((export_name(name)))

static roswasmc_context context;
static roswasmc_capability owner;
static int started, closed, denied;
static uint32_t primary, cleanup, samples;
static roswasmc_bounded_2_kuka_driver_interfaces_msg_KssStatus value;
static unsigned char bytes[
    ROSWASMC_BOUNDED_2_KUKA_DRIVER_INTERFACES_MSG_KSSSTATUS_MAX_WIRE_BYTES];

/** Preserve the first operation failure and the independent close result. */
static uint32_t finish(uint32_t error)
{
    if(error && !primary) primary = error;
    if(!closed)
    {
        closed = 1;
        if(owner.handle) cleanup = roswasmc_close(&owner);
    }
    return primary ? primary : cleanup;
}

EXPORT("initialize_case") uint32_t initialize_case(uint32_t mode)
{
    uint32_t error;
    if(started || closed) return 9;
    if(mode > 1) return 22;
    started = 1;
    if(uwvm_ros2_abi_version() != 1) return finish(95);
    context.backend = roswasmc_ros2_backend();
    roswasmc_capability_init(&owner, &context);
    error = roswasmc_open(&owner, "vendor-status", 13);
    if(mode == 0)
    {
        denied = error == 13;
        return finish(denied ? 0 : error ? error : 22);
    }
    if(!error) error = roswasmc_subscribe(&owner);
    return error ? finish(error) : 0;
}

/** Commit a complete canonical sample only after successful validation. */
static uint32_t accept_sample(unsigned char const* data, size_t length)
{
    uint32_t error;
    roswasmc_bounded_2_kuka_driver_interfaces_msg_KssStatus candidate;
    error = roswasmc_bounded_2_kuka_driver_interfaces_msg_KssStatus_decode(
        data, length, &candidate);
    if(error) return error;
    value = candidate;
    if(samples != UINT32_MAX) ++samples;
    return 0;
}

EXPORT("poll_once") uint32_t poll_once(void)
{
    roswasmc_result result;
    uint32_t error;
    if(!started || closed) return 9;
    result = roswasmc_poll(&owner, bytes, sizeof bytes);
    if(result.error == 11) return 11;
    if(result.error) return finish(result.error);
    if(!result.has_length || result.length > sizeof bytes) return finish(22);
    error = accept_sample(bytes, (size_t)result.length);
    if(error) return finish(error);
    return 0;
}

/** Full original field view. Check sample_ready() before using any getter. */
EXPORT("sample_ready") uint32_t sample_ready(void) { return samples != 0; }
EXPORT("sample_count") uint32_t sample_count(void) { return samples; }
EXPORT("control_mode") uint32_t control_mode(void) { return value.control_mode; }
EXPORT("cycle_time") uint32_t cycle_time(void) { return value.cycle_time; }
EXPORT("drives_powered") uint32_t drives_powered(void) { return value.drives_powered; }
EXPORT("emergency_stop") uint32_t emergency_stop(void) { return value.emergency_stop; }
EXPORT("guard_stop") uint32_t guard_stop(void) { return value.guard_stop; }
EXPORT("in_motion") uint32_t in_motion(void) { return value.in_motion; }
EXPORT("motion_possible") uint32_t motion_possible(void) { return value.motion_possible; }
EXPORT("operation_mode") uint32_t operation_mode(void) { return value.operation_mode; }
EXPORT("robot_stopped") uint32_t robot_stopped(void) { return value.robot_stopped; }

EXPORT("denied_verified") uint32_t denied_verified(void) { return denied; }
EXPORT("primary_status") uint32_t primary_status(void) { return primary; }
EXPORT("cleanup_status") uint32_t cleanup_status(void) { return cleanup; }
EXPORT("close_case") uint32_t close_case(void)
{
    if(closed) return 9;
    return finish(0);
}
