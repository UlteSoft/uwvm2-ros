/** @file
 * @brief Executable C local Context/Future/CallbackGroup/Timer control flow.
 * Default mode exercises local scheduling only and acquires no ROS alias. It
 * is not native ROS acceptance. ROSWASM_EXAMPLE_DEFER_RPC=1 instead opens the
 * explicit trigger-call alias and defers one bounded synchronous Trigger call;
 * its real result13/95/110 is returned unchanged, never a fabricated success.
 * Caller provides timer times100/110; no system/ROS/WASI clock is requested.
 */
#include "example_support.h"
#ifndef ROSWASM_EXAMPLE_DEFER_RPC
#define ROSWASM_EXAMPLE_DEFER_RPC 0
#endif
/** @brief Stable task storage owns results independently of scheduler scratch. */
typedef struct application {roswasmc_capability* client;unsigned calls,completed,ticks;unsigned char response[512];roswasmc_service_response result;} application;
static uint32_t operation(void* opaque) {
    application* app=(application*)opaque;++app->calls;
#if ROSWASM_EXAMPLE_DEFER_RPC
    return roswasmc_trigger_call(app->client,app->response,sizeof app->response,&app->result);
#else
    return 0;
#endif
}
static void completed(void* opaque,roswasmc_future const* status) {application* app=(application*)opaque;++app->completed;(void)status;}
static uint32_t timer_callback(void* opaque) {++((application*)opaque)->ticks;return 0;}
int main(void) {
    roswasmc_local_context context;roswasmc_executor executor={0};roswasmc_deferred_task slot[1];roswasmc_future future;
    roswasmc_callback_group group;roswasmc_timer timer;roswasmc_node node;roswasmc_capability owner;application app={0};uint32_t error;size_t done=0;
    roswasmc_local_context_init(&context);roswasmc_future_init(&future);roswasmc_callback_group_init(&group,1);
    error=example_node(&node);if(error) return (int)error;roswasmc_capability_init(&owner,&node.context);app.client=&owner;
#if ROSWASM_EXAMPLE_DEFER_RPC
    {roswasmc_string alias={"trigger-call",12};error=roswasmc_node_create_client(&node,&owner,alias);}if(error) return (int)error;
#endif
    error=roswasmc_executor_init(&executor,&context,slot,1);
    if(!error) error=roswasmc_executor_post(&executor,&future,operation,&app,completed,&group);
    if(!error&&app.calls) error=UWVM_ROS_INVALID;
    if(!error) error=roswasmc_executor_spin(&executor,&done);
    if(!error&&(done!=1||app.calls!=1||app.completed!=1)) error=UWVM_ROS_INVALID;
    if(!error) error=roswasmc_future_take(&future);
    if(!error) error=roswasmc_timer_init(&timer,10,100,timer_callback,&app,&group,&context);
    if(!error) {uint32_t idle=roswasmc_timer_tick(&timer,109);if(idle!=UWVM_ROS_AGAIN) error=UWVM_ROS_INVALID;}
    if(!error) error=roswasmc_timer_tick(&timer,110);
    if(!error&&app.ticks!=1) error=UWVM_ROS_INVALID;
    roswasmc_executor_cancel_pending(&executor);roswasmc_local_context_shutdown(&context);
    return (int)example_finish(&owner,error);
}
