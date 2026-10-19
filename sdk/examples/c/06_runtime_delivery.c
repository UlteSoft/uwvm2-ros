/** @file
 * @brief Real SDK-to-native String relay using C Node/typed codecs/basic stdio.
 * Build one ROS backend per module and set ROSWASM_EXAMPLE_DEFAULT_DENIED=1 for
 * the separate zero-rights refusal row. Positive mode requires only pub/sub
 * grants specified in tests/ros_sdk_relay/NATIVE.md. A supervisor confirms live
 * native endpoints, sends P, then at most200 R tokens; no implicit guest clock,
 * polling loop pace or stdin argument/environment dependency exists.
 * The real subscriber must decode exactly one canonical reply before PASS.
 */
#include "06_runtime_delivery_support.h"
#ifndef ROSWASM_EXAMPLE_DEFAULT_DENIED
#define ROSWASM_EXAMPLE_DEFAULT_DENIED 0
#endif
typedef struct delivery_state {unsigned callbacks;uint32_t error;} delivery_state;
/** @brief Decode before scratch reuse; copy/count exactly the intended String. */
static void sdk_delivery_callback(void* opaque,unsigned char const* bytes,size_t length) {
    delivery_state* state=(delivery_state*)opaque;roswasmc_string text;size_t n;static char const expected[]="roswasm-js-relay-in";
    state->error=roswasmc_decode_string(bytes,length,&text);
    if(!state->error) {if(text.size!=sizeof expected-1) state->error=UWVM_ROS_INVALID;
        else for(n=0;n<text.size;++n) if(text.data[n]!=expected[n]) state->error=UWVM_ROS_INVALID;}
    if(!state->error) ++state->callbacks;
}
/** @brief Execute real imports; all cleanup paths attempt each owner only once. */
static uint32_t sdk_delivery_main(void) {
    uint32_t error;if(ROSWASM_IMPORT(abi_version)()!=1) return UWVM_ROS_UNSUPPORTED;
#if ROSWASM_EXAMPLE_DEFAULT_DENIED
    error=sdk_delivery_denied("pub",3);if(!error) error=sdk_delivery_denied("sub",3);
    if(!error) error=sdk_delivery_emit("PASS_SDK_DENIED\n",16);return error;
#else
    roswasmc_backend backend=*ROSWASM_BACKEND();roswasmc_node node;roswasmc_capability pub,sub;
    roswasmc_string name={"sdk-delivery",12},pub_alias={"pub",3},sub_alias={"sub",3};
    roswasmc_string warmup={"warmup-js-relay",15},final={"roswasm-js-relay-out",20};
    unsigned char scratch[64],receive[64];uint64_t old_pub=0,old_sub=0;delivery_state state={0};unsigned pass;
    backend.poll=sdk_delivery_observed_poll;roswasmc_node_init(&node,&backend,name);
    roswasmc_capability_init(&pub,&node.context);roswasmc_capability_init(&sub,&node.context);
    error=roswasmc_node_create_publisher(&node,&pub,pub_alias);
    if(!error) error=roswasmc_node_create_subscription(&node,&sub,sub_alias);
    if(!error) error=sdk_delivery_denied("ungranted",9);
    if(!error) error=sdk_delivery_poll_refusal(pub.handle,UWVM_ROS_DENIED);
    if(!error) error=roswasmc_publish_string(&pub,warmup,scratch,sizeof scratch);
    if(!error) error=sdk_delivery_emit("SDK_ENDPOINTS_CREATED\n",22);
    if(!error) error=sdk_delivery_gate('P');
    if(!error) error=roswasmc_publish_string(&pub,final,scratch,sizeof scratch);
    if(!error) error=sdk_delivery_emit("SDK_FINAL_SENT\n",15);
    for(pass=0;!error&&pass<200&&!state.callbacks;++pass) {
        error=sdk_delivery_gate('R');if(error) break;
        {roswasmc_result result=roswasmc_poll(&sub,receive,sizeof receive);error=result.error;
         if(sdk_delivery_poll_error) error=sdk_delivery_poll_error;
         if(error==UWVM_ROS_AGAIN) {error=sdk_delivery_emit("SDK_POLL_AGAIN\n",15);continue;}
         if(!error) sdk_delivery_callback(&state,receive,(size_t)result.length);}
        if(!error) error=state.error;
    }
    if(!error&&state.callbacks!=1) error=UWVM_ROS_INVALID;
    if(!error) error=sdk_delivery_emit("SDK_DELIVERED\n",14);
    old_pub=pub.handle;old_sub=sub.handle;
    {roswasmc_cleanup_report cleanup=roswasmc_server_close(&pub,&sub,error,0);if(!error) error=cleanup.cleanup_error;}
    if(!error) error=sdk_delivery_poll_refusal(old_pub,UWVM_ROS_BAD_HANDLE);
    if(!error) error=sdk_delivery_poll_refusal(old_sub,UWVM_ROS_BAD_HANDLE);
    if(!error) error=sdk_delivery_emit("PASS_SDK_DELIVERY\n",18);return error;
#endif
}
/** @brief Freestanding entry avoids WASI CRT arguments/environment startup. */
__attribute__((export_name("_start"),noreturn)) void _start(void) {sdk_delivery_exit(sdk_delivery_main());}
