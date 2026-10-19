/** C11 complete typed clients for this project's five Spot ROS bridge schemas.
 * Requires ACTUAL generated bounded_records.h from the complete vendor IR.
 * These are not Boston Dynamics native C layouts or arbitrary byte facades.
 * The native-only ROS bridge owns all vendor networking/credentials/SDK leases.
 */
#ifndef UWVM_SPOT_TYPED_SDK_H
#define UWVM_SPOT_TYPED_SDK_H
#include <roswasm/roswasmc.h>
#include "bounded_records.h"
typedef roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request mm_spot_State_request;
typedef roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response mm_spot_State_response;
typedef roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request mm_spot_Lease_request;
typedef roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response mm_spot_Lease_response;
typedef roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request mm_spot_Command_request;
typedef roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response mm_spot_Command_response;
typedef roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request mm_spot_Velocity_request;
typedef roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response mm_spot_Velocity_response;
typedef roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request mm_spot_Feedback_request;
typedef roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response mm_spot_Feedback_response;
typedef struct mm_spot_buffers {
    unsigned char input[32768],output[32768];
    union {mm_spot_State_response State;mm_spot_Lease_response Lease;mm_spot_Command_response Command;
           mm_spot_Velocity_response Velocity;mm_spot_Feedback_response Feedback;} next;
    uint32_t busy;
} mm_spot_buffers;
/** Preallocate buffers and typed response before opening an owner. Caller must
 * serialize their lifetime and raw owner retirement; no callback is invoked.
 * Returned roswasmc_result.error is TRANSPORT status only. On transport0 inspect
 * response.error/unknown independently; provider errors never become retry grants.
 * Malformed/correlation-failed responses preserve the caller's entire object.
 */
#define MM_SPOT_TYPED_CALL(Name) \
static inline roswasmc_result mm_spot_##Name##_call(roswasmc_capability const* source, \
    mm_spot_##Name##_request const* request,mm_spot_##Name##_response* response,mm_spot_buffers* storage) { \
    roswasmc_result result={22,0,0};size_t length=0;uint32_t code;uint64_t token,nonce; \
    if(!source||!source->context||!source->context->backend||source->context->backend->version!=2||!request||!response||!storage)return result; \
    if(storage->busy){result.error=17;return result;} \
    if(uwvm_bounded_overlap(response,sizeof *response,storage,sizeof *storage))return result; \
    token=request->token;nonce=request->nonce; \
    if(!token||!nonce||!request->budget_ns||request->budget_ns>5000000000ull)return result; \
    code=roswasmc_bounded_2_uwvm_spot_bridge_srv_##Name##_Request_encode(request,storage->input,sizeof storage->input,&length); \
    if(code){result.error=code;return result;} \
    storage->busy=1; \
    result=roswasmc_request(source,ROSWASMC_SERVICE_CALL,storage->input,length,storage->output,sizeof storage->output); \
    if(!result.error){ \
        if(!result.has_length)result.error=22; \
        else {code=roswasmc_bounded_2_uwvm_spot_bridge_srv_##Name##_Response_decode(storage->output,(size_t)result.length,&storage->next.Name); \
          if(code)result.error=code; \
          else if(storage->next.Name.token!=token||storage->next.Name.nonce!=nonce)result.error=22; \
          else *response=storage->next.Name;} \
    } \
    storage->busy=0;return result; \
}
MM_SPOT_TYPED_CALL(State)
MM_SPOT_TYPED_CALL(Lease)
MM_SPOT_TYPED_CALL(Command)
MM_SPOT_TYPED_CALL(Velocity)
MM_SPOT_TYPED_CALL(Feedback)
#undef MM_SPOT_TYPED_CALL
#endif
