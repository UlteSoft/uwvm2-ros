/** @file Real ROS1 C11 legacy application; link the SAME-module command entry.
 * No ROS implementation, generated vendor struct, hardware CRC or authority is
 * supplied here. Full newly Noetic-bound eleven-type records are build inputs.
 * initialize calls real topic_subscribe BEFORE parent M; open alone is not
 * subscription creation. Poll/submit/close occur only at explicit main steps.
 */
#include <roswasm/ros1.h>
#include "values_c.h"
#define EXPORT(N) __attribute__((export_name(N)))
static roswasmc_context context;
static roswasmc_capability publisher,subscription;
static uint32_t started,closed,last_status,polls,received,denials,submitted;
static unsigned char incoming[1536],observed[1536],expected[1536],command_bytes[1536];
static size_t observed_size;
static uint32_t close_all(uint32_t first) {
    uint32_t code;if(closed)return first?first:9;closed=1;
    if(roswasmc_is_open(&subscription)){code=roswasmc_close(&subscription);if(!first)first=code;}
    if(roswasmc_is_open(&publisher)){code=roswasmc_close(&publisher);if(!first)first=code;}
    last_status=first;return first;
}
EXPORT("initialize_case") uint32_t initialize_case(uint32_t mode,uint32_t family,uint32_t reserved) {
    uint32_t code;if(started||closed)return 9;if(mode>1||family!=4||reserved)return close_all(22);started=1;
    if(uwvm_ros1_abi_version()!=1)return close_all(95);
    context.backend=roswasmc_ros1_backend();roswasmc_capability_init(&publisher,&context);roswasmc_capability_init(&subscription,&context);
    code=roswasmc_open(&publisher,"legacy-lowcmd",sizeof("legacy-lowcmd")-1);
    if(!mode){
        if(code!=13||roswasmc_is_open(&publisher))return close_all(code?code:22);++denials;
        code=roswasmc_open(&subscription,"legacy-lowstate",sizeof("legacy-lowstate")-1);
        if(code!=13||roswasmc_is_open(&subscription))return close_all(code?code:22);++denials;return close_all(0);
    }
    if(code)return close_all(code);
    code=roswasmc_open(&subscription,"legacy-lowstate",sizeof("legacy-lowstate")-1);
    if(!code)code=roswasmc_subscribe(&subscription);
    return code?close_all(code):0;
}
EXPORT("poll_once") uint32_t poll_once(void) {
    LegacyState value,reference;size_t n=0,wanted=0,i;roswasmc_result result;
    if(!started||closed||received)return 9;if(polls++>=4096)return close_all(110);
    result=roswasmc_poll(&subscription,incoming,sizeof incoming);if(result.error==11)return 11;
    if(result.error)return close_all(result.error);
    if(!result.has_length||result.length>sizeof incoming||roswasmc_bounded_1_unitree_legged_msgs_LowState_decode(incoming,(size_t)result.length,&value))return close_all(22);
    reference=legacy_expected_state();
    if(roswasmc_bounded_1_unitree_legged_msgs_LowState_encode(&reference,expected,sizeof expected,&wanted)||
       roswasmc_bounded_1_unitree_legged_msgs_LowState_encode(&value,observed,sizeof observed,&n)||n!=wanted||n!=result.length)return close_all(22);
    for(i=0;i<n;++i)if(observed[i]!=expected[i]||observed[i]!=incoming[i])return close_all(22);
    observed_size=n;++received;return 0;
}
EXPORT("submit_once") uint32_t submit_once(void) {
    LegacyCmd value;size_t n=0;uint32_t code;if(!started||closed||received!=1||submitted)return 9;
    value=legacy_expected_command();code=roswasmc_bounded_1_unitree_legged_msgs_LowCmd_encode(&value,command_bytes,sizeof command_bytes,&n);
    if(!code)code=roswasmc_publish(&publisher,command_bytes,n);if(code)return close_all(code);submitted=1;return 0;
}
/* Scalar observations are Guest-owned bytes only; read status after a getter.
 * A zero byte/length may be real data and is never an error sentinel. */
EXPORT("observed_length") uint32_t observed_length(void){return received?(uint32_t)observed_size:0;}
EXPORT("observed_byte") uint32_t observed_byte(uint32_t index){if(!received||index>=observed_size){last_status=22;return 0;}return observed[index];}
EXPORT("received_count") uint32_t received_count(void){return received;}
EXPORT("denied_count") uint32_t denied_count(void){return denials;}
EXPORT("case_status") uint32_t case_status(void){return last_status;}
EXPORT("close_case") uint32_t close_case(void){return close_all(0);}
