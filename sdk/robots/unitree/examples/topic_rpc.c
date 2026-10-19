// Typed Unitree example source contract (comments only; executable code below).
// Source-stage contract: copy the selected complete generated family module and
// the actual Guest SDK into fresh owned work as specified by
// sdk/robots/unitree/recipes/closedtyped-example-recipes.json. Never substitute
// a projected schema, fake native identity, mock bridge or generated source stub.
// For exact build/staging and independent native oracles, read TYPED_EXAMPLES.md.
// Role: complete unitree_api Request/Response topic RPC, NOT a ROS service or
// firmware API call. Mode0 opens only unitree-api-request and expects denial.
// Mode1 requires that alias as topic.publish and unitree-api-response as
// topic.subscribe, each with the actual corresponding native message identity.
// After both endpoints match, submit_once publishes exactly one full Request
// with explicit positive int64-compatible ids, parameter="{}" and empty binary.
// The independent SIM fixture must reply with those same ids, status0, exact
// data="typed-example-ack" and empty binary. No physical endpoint is selected.
// Ordered caller protocol: validate the real binary and explicitly initialize
// its selected reactor runtime first; use one fresh instance for mode0 denial
// and another for mode1 positive behavior. Then initialize_case once, explicitly
// submit for topic RPC, poll_once at most4096 times, inspect a received value,
// and close_case after the polling/callback stack has returned. No retry loop,
// clock, thread, goroutine or native endpoint is created by this source itself.
// A successful factory or codec operation does not prove native transport.
// The trusted external Runtime policy supplies rights; this code grants none.
// Status convention:0 means the documented operation succeeded;11 is an idle
// poll;9 rejects unused/closed/repeated lifecycle operations;22 rejects invalid
// configuration or message data;90 is a capacity bound;95 rejects the selected
// ABI when checked;110 exhausts the finite poll budget. Original SDK errors are
// preserved. Rust can additionally return17 for a conflicting state borrow.
// Expected mode0 alias denial13 is translated to0 ONLY with denied_verified=1.
// All acquired owners are explicitly retired on ordinary completion/failure,
// attempting later closes after an earlier close error. Traps/panics/kills still
// require the external process owner to finish actual Runtime/Gateway cleanup.

/** @file C11 finite typed unitree_api topic RPC reactor.
 * The generated full API8 bounded_records.h is an external pinned build input.
 * All dynamic canonical fields use inline caller-owned capacity. These two
 * aliases are topics, never service_call or robot firmware endpoint discovery.
 * No operation occurs at load. initialize_case(0,...) verifies default-denial;
 * explicit mode1 opens two trusted SIM-only aliases, then submit_once sends
 * one request. Root independently observes native Request/Response delivery.
 */
#include <roswasm/ros2.h>
#include "bounded_records.h"
#define EXPORT(name) __attribute__((export_name(name)))
typedef roswasmc_bounded_2_unitree_api_msg_Request Request;
typedef roswasmc_bounded_2_unitree_api_msg_Response Response;
static roswasmc_context context;
static roswasmc_capability publisher,subscription;
static uint32_t started,closed,last_status,polls,received,denied,submitted;
static int64_t request_id,api_id;
static unsigned char input[ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUEST_MAX_WIRE_BYTES];
static unsigned char output[ROSWASMC_BOUNDED_2_UNITREE_API_MSG_RESPONSE_MAX_WIRE_BYTES];
static Request request_value;
static Response response_value;
static uint32_t close_all(uint32_t first) {
    uint32_t code;if(closed)return first?first:9;closed=1;
    if(roswasmc_is_open(&subscription)){code=roswasmc_close(&subscription);if(!first)first=code;}
    if(roswasmc_is_open(&publisher)){code=roswasmc_close(&publisher);if(!first)first=code;}
    last_status=first;return first;
}
// ABI initialize_case(mode,arg0,arg1)->u32 is single-use. Mode0 proves actual
// default denial; mode1 requires the explicit positive aliases stated above.
// LowState args are (family1=GO or2=HG,reserved0); API args are (request_id,
// api_id), each1..0x7fffffff. Return0 is setup/denial success, never delivery.
// An error attempts cleanup; a fresh instance is required for another case.
EXPORT("initialize_case") uint32_t initialize_case(uint32_t mode,uint32_t id,uint32_t api) {
    uint32_t error;if(started||closed)return 9;
    if(mode>1||!id||id>0x7fffffff||!api||api>0x7fffffff)return close_all(22);
    started=1;request_id=id;api_id=api;
    if(uwvm_ros2_abi_version()!=1)return close_all(95);
    context.backend=roswasmc_ros2_backend();roswasmc_capability_init(&publisher,&context);roswasmc_capability_init(&subscription,&context);
    error=roswasmc_open(&publisher,"unitree-api-request",sizeof("unitree-api-request")-1);
    if(mode==0){denied=error==13&&!roswasmc_is_open(&publisher);return close_all(denied?0:error?error:22);}
    if(error)return close_all(error);
    error=roswasmc_open(&subscription,"unitree-api-response",sizeof("unitree-api-response")-1);
    if(!error)error=roswasmc_subscribe(&subscription);
    return error?close_all(error):0;
}
// ABI submit_once()->u32 sends one complete typed Request after successful
// positive initialization. It uses the configured ids, empty binary and "{}".
// Return0 means the SDK accepted publication; only the independent native
// peer and later matching Response prove transport. A repeated call returns9.
EXPORT("submit_once") uint32_t submit_once(void) {
    size_t n=0;uint32_t error;if(!started||closed||submitted)return 9;
    request_value.header.identity.id=request_id;request_value.header.identity.api_id=api_id;
    request_value.parameter.size=2;request_value.parameter.data[0]='{';request_value.parameter.data[1]='}';
    error=roswasmc_bounded_2_unitree_api_msg_Request_encode(&request_value,input,sizeof input,&n);
    if(!error)error=roswasmc_publish(&publisher,input,n);
    if(error)return close_all(error);submitted=1;return 0;
}
// ABI poll_once()->u32 performs one explicit copied SDK poll pass. API must
// have submitted first. Return11 means no new value;0 means a newly validated
// value, and nonzero errors retain their status and attempt cleanup. No hidden
// retry occurs; the4097th attempt closes with110. Inspect count before getters.
EXPORT("poll_once") uint32_t poll_once(void) {
    static char const ack[]="typed-example-ack";size_t i;roswasmc_result result;
    if(!started||closed||!submitted)return 9;if(polls++>=4096)return close_all(110);
    result=roswasmc_poll(&subscription,output,sizeof output);if(result.error==11)return 11;
    if(result.error)return close_all(result.error);
    if(!result.has_length||result.length>sizeof output||roswasmc_bounded_2_unitree_api_msg_Response_decode(output,(size_t)result.length,&response_value))return close_all(22);
    if(response_value.header.identity.id!=request_id||response_value.header.identity.api_id!=api_id||response_value.header.status.code||response_value.binary.size||response_value.data.size!=sizeof ack-1)return close_all(22);
    for(i=0;i<sizeof ack-1;++i)if(response_value.data.data[i]!=(unsigned char)ack[i])return close_all(22);
    ++received;return 0;
}
// ABI received_count()->u32 observes the number of validated complete values.
// Require successful initialization and a poll result0 first. Zero is a valid
// initial count and never proves delivery; this getter performs no native call.
EXPORT("received_count") uint32_t received_count(void){return received;}
// ABI response_code()->u32 observes the accepted Response status after a
// successful poll and received_count>0. This fixture accepts status0 only;
// an initial zero is not an acknowledgement and invokes no native operation.
EXPORT("response_code") uint32_t response_code(void){return (uint32_t)response_value.header.status.code;}
// ABI denied_verified()->u32 is1 ONLY after mode0 observed actual denial13.
// It is0 before that observation or in a positive case. The independent native
// observer must additionally prove zero new endpoints/payload; this flag alone
// cannot establish absence of effects. It grants no permission.
EXPORT("denied_verified") uint32_t denied_verified(void){return denied;}
// ABI case_status()->u32 observes the preserved operation/cleanup status.
// Read it after any ambiguous scalar getter or callback pass. Zero is the
// initial/success value; it cannot replace the delivery count/native witness.
// This getter neither authorizes, polls nor consumes a native queue.
EXPORT("case_status") uint32_t case_status(void){return last_status;}
// ABI close_case()->u32 retires every ordinarily acquired owner after the
// current poll/callback has unwound, attempting remaining closes after errors.
// Return0 means those closes succeeded; later close/poll/submit returns9.
// A denial case was already closed by initialize_case and therefore returns9.
// External ownership remains necessary for an unrecoverable trap/process kill.
EXPORT("close_case") uint32_t close_case(void){return close_all(0);}
