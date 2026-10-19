/** @file
 * @brief Runnable allocation-free service/Action server, one bounded poll.
 * Compile ROSWASM_EXAMPLE_CASE=0 Trigger,1 Fibonacci,2 SetBool. Every server
 * acquires distinct advertise/respond aliases before activation and closes both
 * once on every exit. A queue-empty poll returns11, never successful delivery.
 * Retain owners in a real application's externally paced loop rather than
 * assuming this one pass guarantees native discovery. No thread/clock is opened.
 * Full grants/native prerequisites are listed in ADVANCED.md.
 */
#include "example_support.h"
#ifndef ROSWASM_EXAMPLE_CASE
#define ROSWASM_EXAMPLE_CASE 0
#endif
#if ROSWASM_EXAMPLE_CASE < 0 || ROSWASM_EXAMPLE_CASE > 2
#error "ROSWASM_EXAMPLE_CASE must be 0..2"
#endif
/** @brief Decode exactly the selected request and reply with its gateway-owned ID. */
int main(void) {
    roswasmc_node node;roswasmc_capability advertise,respond;roswasmc_cleanup_report setup;
    unsigned char event[64],scratch[512];uint32_t error;roswasmc_result polled;
    error=example_node(&node);if(error) return (int)error;
    roswasmc_capability_init(&advertise,&node.context);roswasmc_capability_init(&respond,&node.context);
#if ROSWASM_EXAMPLE_CASE == 1
    {roswasmc_string a={"fib-advertise",13},r={"fib-respond",11};
     error=roswasmc_node_create_action_server_checked(&node,&advertise,&respond,a,r,&setup);}
#else
    {roswasmc_string a={ROSWASM_EXAMPLE_CASE==0?"trigger-advertise":"set-bool-advertise",ROSWASM_EXAMPLE_CASE==0?17:18};
     roswasmc_string r={ROSWASM_EXAMPLE_CASE==0?"trigger-respond":"set-bool-respond",ROSWASM_EXAMPLE_CASE==0?15:16};
     error=roswasmc_node_create_service_checked(&node,&advertise,&respond,a,r,&setup);}
#endif
    if(!error) {
        polled=roswasmc_poll(&advertise,event,sizeof event);error=polled.error;
        if(!error) {
#if ROSWASM_EXAMPLE_CASE == 1
            roswasmc_fibonacci_goal goal;error=roswasmc_decode_fibonacci_goal(event,(size_t)polled.length,&goal);
            if(!error&&(goal.order<0||goal.order>12)) error=UWVM_ROS_INVALID;
            if(!error) {
                int32_t sequence[12];unsigned char wire_values[48];int32_t n;roswasmc_writer writer=roswasmc_writer_init(wire_values,sizeof wire_values);
                for(n=0;n<goal.order;++n) {uint32_t bits;sequence[n]=n<2?n:sequence[n-1]+sequence[n-2];roswasmc_copy_object(&bits,&sequence[n],4);roswasmc_write_integer(&writer,bits,4);}
                {roswasmc_action_sequence result={goal.id,2,(uint32_t)goal.order,wire_values};error=roswasmc_action_reply(&respond,&result,scratch,sizeof scratch);}
            }
#else
            roswasmc_service_request request;int accepted=1;error=roswasmc_decode_service_request(event,(size_t)polled.length,&request);
            if(!error) {
                if(ROSWASM_EXAMPLE_CASE==0) {if(request.payload.size) error=UWVM_ROS_INVALID;}
                else error=roswasmc_decode_bool(request.payload.data,request.payload.size,&accepted);
            }
            if(!error) {roswasmc_string message={"guest service handler",21};roswasmc_service_response response={accepted,message};
                error=roswasmc_service_reply(&respond,request.id,response,scratch,sizeof scratch);}
#endif
        }
    }
    return (int)example_finish_pair(&advertise,&respond,error);
}
