/** @file
 * @brief C11 caller-driven custom Echo/Digest servers with complete typed components.
 *
 * Select UWVM_BOUNDED_ROS_VERSION=1/2 and fresh generated-bounded-v2 products.
 * Four independent aliases are required: bounded-service-events/replies for
 * service.advertise/respond, bounded-action-events/replies for action.advertise/
 * respond. Resources must be /uwvm_bounded_v2/guest_echo and guest_digest, with
 * actual official root/component identities and all finite queue/goal/time/QoS
 * limits. An incoming native ticket is not an opaque capability handle.
 *
 * This runnable entry advertises both endpoints, then at most200 basic stdin R
 * gates release one bounded pass. It consumes exactly one native Echo request
 * and one Digest Goal, emits exactly one typed feedback, waits for checked E
 * after independent native feedback delivery, then sends one terminal result,
 * checks every response, and closes all four owners in reverse. Idle11 leaves
 * the external caller-driven loop active; it never counts as delivered success.
 * No implicit clock, thread, random, environment or ROS permission is requested.
 */
#include <stdio.h>
#include <string.h>
#if UWVM_BOUNDED_ROS_VERSION == 1
#include <roswasm/ros1.h>
#define BACKEND roswasmc_ros1_backend
#define ABI uwvm_ros1_abi_version
#define ENV roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope
#define REQUEST roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest
#define RESPONSE roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse
#define GOAL roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal
#define FEEDBACK roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback
#define RESULT roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult
#define SERVICE_POLL roswasmc_bounded_1_uwvm_ros_bounded_v2_Echo_poll
#define SERVICE_RESPOND roswasmc_bounded_1_uwvm_ros_bounded_v2_Echo_respond
#define ACTION_POLL roswasmc_bounded_1_uwvm_ros_bounded_v2_Digest_poll
#define ACTION_FEEDBACK roswasmc_bounded_1_uwvm_ros_bounded_v2_Digest_respond_feedback
#define ACTION_RESULT roswasmc_bounded_1_uwvm_ros_bounded_v2_Digest_respond_result
#elif UWVM_BOUNDED_ROS_VERSION == 2
#include <roswasm/ros2.h>
#define BACKEND roswasmc_ros2_backend
#define ABI uwvm_ros2_abi_version
#define ENV roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope
#define REQUEST roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request
#define RESPONSE roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response
#define GOAL roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal
#define FEEDBACK roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback
#define RESULT roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result
#define SERVICE_POLL roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_poll
#define SERVICE_RESPOND roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_respond
#define ACTION_POLL roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_poll
#define ACTION_FEEDBACK roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_respond_feedback
#define ACTION_RESULT roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_respond_result
#else
#error "Select one explicit native backend"
#endif
#include "../generated-bounded-v2/bounded_interfaces.h"
static int equal(ENV const* value) {
    return value->label.size==14&&!memcmp(value->label.data,"bounded-native",14)&&value->samples.size==3&&value->samples.data[0]==3&&value->samples.data[1]==5&&value->samples.data[2]==8&&value->points.size==2&&value->points.data[0].value==-7&&value->points.data[0].enabled==1&&value->points.data[1].value==11&&value->points.data[1].enabled==0&&value->labels[0].size==5&&!memcmp(value->labels[0].data,"first",5)&&value->labels[1].size==6&&!memcmp(value->labels[1].data,"second",6);
}
static uint32_t marker(char const* text){return fputs(text,stdout)<0||fflush(stdout)?107:0;}
int main(void){
    roswasmc_context context={BACKEND()};roswasmc_capability caps[4];uint32_t error=0;size_t i,opened=0;unsigned pass;int service_done=0,action_done=0;uint64_t ticket=0;
    char const* aliases[]={"bounded-service-events","bounded-service-replies","bounded-action-events","bounded-action-replies"};
    REQUEST request={0};RESPONSE response={0};GOAL goal={0};FEEDBACK feedback={0};RESULT result={0};roswasmc_result reply;
    if(ABI()!=1)return 95;for(i=0;i<4;++i)roswasmc_capability_init(&caps[i],&context);
    // Validate role availability before acquiring any alias; each open remains
    // individually authorized by Runtime policy. All-or-nothing cleanup follows.
    error=roswasmc_check_operation(&context,ROSWASMC_SERVICE_RESPOND);if(error)goto done;
    error=roswasmc_check_operation(&context,ROSWASMC_ACTION_ADVERTISE);if(error)goto done;
    error=roswasmc_check_operation(&context,ROSWASMC_ACTION_RESPOND);if(error)goto done;
    for(i=0;i<4;++i){error=roswasmc_open(&caps[i],aliases[i],strlen(aliases[i]));if(error)goto done;++opened;}
    error=roswasmc_service_advertise(&caps[0]);if(error)goto done;reply=roswasmc_action_advertise(&caps[2],NULL,0,NULL,0);error=reply.error;if(error)goto done;if(!reply.has_length||reply.length){error=22;goto done;}
    error=marker("SDK_BOUNDED_SERVERS_READY\n");if(error)goto done;
    for(pass=0;pass<200&&(!service_done||!action_done);++pass){
        if(getchar()!='R'){error=22;goto done;}
        if(!service_done){reply=SERVICE_POLL(&caps[0],&ticket,&request);if(reply.error!=11){error=reply.error;if(error)goto done;if(request.limit!=32||!equal(&request.sample)){error=22;goto done;}response.accepted=1;response.echoed=request.sample;reply=SERVICE_RESPOND(&caps[1],ticket,&response);error=reply.error;if(error)goto done;service_done=1;}}
        if(!action_done){reply=ACTION_POLL(&caps[2],&ticket,&goal);if(reply.error!=11){error=reply.error;if(error)goto done;if(!equal(&goal.sample)){error=22;goto done;}feedback.processed=3;feedback.phase.size=8;memcpy(feedback.phase.data,"accepted",8);reply=ACTION_FEEDBACK(&caps[3],ticket,&feedback);error=reply.error;if(error)goto done;error=marker("SDK_BOUNDED_FEEDBACK_SENT\n");if(error)goto done;if(getchar()!='E'){error=22;goto done;}result.accepted=1;result.sum=16;result.note.size=21;memcpy(result.note.data,"bounded-native-result",21);reply=ACTION_RESULT(&caps[3],ticket,2,&result);error=reply.error;if(error)goto done;action_done=1;}}
        if(!service_done||!action_done){error=marker("SDK_BOUNDED_AGAIN\n");if(error)goto done;}
    }
    if(!service_done||!action_done)error=110;
    if(!error)error=marker("SDK_BOUNDED_REPLIES_SENT\n");
    /* Keep native response entities alive until the independent client confirms delivery. */
    if(!error&&getchar()!='D')error=22;
 done:
    for(i=opened;i>0;--i){uint32_t cleanup=roswasmc_close(&caps[i-1]);if(!error)error=cleanup;}
    if(!error)error=marker("PASS_BOUNDED_CUSTOM_SERVER\n");return (int)error;
}
