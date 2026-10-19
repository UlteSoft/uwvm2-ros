/** @file
 * @brief Layer3: guest-local Node construction and caller-driven typed callbacks.
 * Host setup requires separate aliases: `pub` String topic.publish and `sub`
 * String topic.subscribe with a complete event byte limit at most128 bytes.
 * This example makes one bounded polling pass. An empty asynchronous queue is
 * normal; a real application schedules later passes using its own permitted
 * event loop. The SDK neither acquires a clock/thread grant nor busy-spins.
 */
#include "example_support.h"
/** @brief Application-owned callback state; no native thread can access this state. */
struct state {size_t received;uint32_t error;};
/** @brief Decode one borrowed event and use it only for the duration of this callback. */
static void on_string(void* user,unsigned char const* bytes,size_t length) {
    roswasmc_string message;struct state* state=(struct state*)user;
    uint32_t error=roswasmc_decode_string(bytes,length,&message);
    if(error) {if(!state->error) state->error=error;}else ++state->received;
    /* Copy message.data if it must outlive the next poll; this example just counts. */
}
/** @brief Create two explicitly granted resources and run one bounded guest executor pass. */
int main(void) {
    roswasmc_node node;roswasmc_capability pub,sub;struct state state={0,0};
    unsigned char scratch[128];size_t delivered;uint32_t error;
    roswasmc_string label={"c-example",9},pub_alias={"pub",3},sub_alias={"sub",3},message={"hello",5};
    (void)label;error=example_node(&node);if(error) return (int)error;
    error=roswasmc_node_create_publisher(&node,&pub,pub_alias);if(error) return (int)error;
    error=roswasmc_node_create_subscription(&node,&sub,sub_alias);
    if(!error) error=roswasmc_publish_string(&pub,message,scratch,sizeof scratch);
    if(!error) {
        roswasmc_poll_task task={&sub,scratch,sizeof scratch,on_string,&state};
        error=roswasmc_spin_some(&task,1,1,&delivered);
        if(!error) error=state.error;
    }
    return (int)example_finish_pair(&pub,&sub,error);
}
