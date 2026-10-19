/**
 * One complete ReadSingleIO request through the portable ROS2 C SDK.
 *
 * Build with the exact generated Yaskawa bounded_interfaces.h. Initialize this
 * no-entry reactor through the embedding before invoking exported functions.
 * initialize_case(0) checks default denial. initialize_case(1) requests only the
 * separately authorized "vendor-query" Service alias. query_once() reads the
 * example controller address 10011 exactly once and closes its capability.
 * It never writes an IO element, retries a request, resets errors or changes
 * Runtime, debugger, controller mode or update settings.
 *
 * A transport/decode error leaves response_ready() false. A complete controller
 * response is retained even when success is false or result_code is nonzero;
 * query_once() then returns application status 5. Inspect response_ready(),
 * result_code(), response_success(), io_value() and the complete message before
 * deciding what that result means for the selected physical installation.
 * primary_status() and cleanup_status() preserve operation and close failures
 * separately. close_case() retires an initialized session without a query.
 *
 * message_size() gives the exact diagnostic byte count. copy_message() copies
 * the entire value into caller-owned Guest linear memory, with no terminator,
 * truncation or partial write. The embedding supplies a valid Guest buffer and
 * capacity; insufficient capacity returns 90 before writing any destination
 * byte. A message is application data and never authority to perform recovery.
 */
#include <roswasm/ros2.h>
#include "bounded_interfaces.h"
#include <string.h>

#define EXPORT(name) __attribute__((export_name(name)))

static roswasmc_context context;
static roswasmc_capability owner;
static int started, closed, called, denied, ready;
static uint32_t primary, cleanup;
static roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response response;

/** Record the first failure and close the owned capability exactly once. */
static uint32_t finish(uint32_t status)
{
    if(status && !primary) primary = status;
    if(!closed)
    {
        closed = 1;
        if(roswasmc_is_open(&owner)) cleanup = roswasmc_close(&owner);
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
    error = roswasmc_open(&owner, "vendor-query", 12);
    if(mode == 0)
    {
        denied = error == 13 && !owner.handle;
        return finish(denied ? 0 : error ? error : 22);
    }
    return error ? finish(error) : 0;
}

EXPORT("query_once") uint32_t query_once(void)
{
    roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request request = {0};
    roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response candidate;
    roswasmc_result result;
    if(!started || closed || called) return 9;
    called = 1;
    request.address = 10011;
    result = roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_call(
        &owner, &request, &candidate);
    if(result.error) return finish(result.error);
    response = candidate;
    ready = 1;
    return finish(response.success && response.result_code == 0 ? 0 : 5);
}

/** Read all original response fields, including controller-reported failures. */
EXPORT("response_ready") uint32_t response_ready(void) { return ready; }
EXPORT("result_code") uint32_t result_code(void) { return response.result_code; }
EXPORT("response_success") uint32_t response_success(void) { return response.success; }
EXPORT("io_value") int32_t io_value(void) { return response.value; }
EXPORT("message_size") uint32_t message_size(void) { return response.message.size; }
EXPORT("copy_message") uint32_t copy_message(unsigned char* destination, uint32_t capacity)
{
    if(!ready) return 9;
    if(response.message.size > sizeof response.message.data) return 22;
    if(capacity < response.message.size) return 90;
    if(!destination && response.message.size) return 22;
    if(response.message.size) memcpy(destination, response.message.data, response.message.size);
    return 0;
}

EXPORT("denied_verified") uint32_t denied_verified(void) { return denied; }
EXPORT("primary_status") uint32_t primary_status(void) { return primary; }
EXPORT("cleanup_status") uint32_t cleanup_status(void) { return cleanup; }
EXPORT("close_case") uint32_t close_case(void)
{
    if(closed) return 9;
    return finish(0);
}
