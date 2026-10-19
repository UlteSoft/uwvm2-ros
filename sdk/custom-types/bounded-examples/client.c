/** @file
 * @brief C11 complete bounded custom Service/Action client lesson.
 *
 * Prepare a fresh generated-bounded-v2 directory from both official catalogs,
 * or from a combined catalog, before compilation. Select exactly one backend
 * with -DUWVM_BOUNDED_ROS_VERSION=1/2. This entry uses existing C capability and
 * checked role-bundle ownership with generated typed custom RPCs, rather than
 * pretending the builtin Trigger/Fibonacci factories support custom components.
 *
 * Required aliases: bounded-echo, bounded-empty, bounded-send, bounded-cancel,
 * bounded-feedback, bounded-result. The host selects the exact official type,
 * identity, canonical fingerprint, resource, timeouts, five ROS2 QoS channels and
 * quotas. There are no default grants. -DUWVM_BOUNDED_DENIED=1 asserts the default
 * empty authority using all six independent open attempts and zero live handles.
 *
 * Positive flow calls Echo and Empty once, sends one Digest Goal, then consumes
 * exactly one feedback and terminal result. Basic stdin R releases each bounded
 * poll, at most200 passes. After validated feedback, the checked E gate prevents
 * terminal polling until the independent native peer releases its result. The
 * external controller owns pacing/deadline. Idle11
 * retains the same Goal and all owners; send/cancel/call are never replayed.
 * stdout markers prove only checked Guest observations; an independent native
 * peer and qualified actual product/process map remain acceptance requirements.
 */
#include <stdio.h>
#include <string.h>
#if UWVM_BOUNDED_ROS_VERSION == 1
#include <roswasm/ros1.h>
#define CONTEXT roswasmc_ros1_backend
#define ABI uwvm_ros1_abi_version
#define ENV roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope
#define ECHO_RQ roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest
#define ECHO_RP roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse
#define EMPTY_RQ roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest
#define EMPTY_RP roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse
#define GOAL roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal
#define FEEDBACK roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback
#define RESULT roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult
#define ECHO_CALL roswasmc_bounded_1_uwvm_ros_bounded_v2_Echo_call
#define EMPTY_CALL roswasmc_bounded_1_uwvm_ros_bounded_v2_Empty_call
#define SEND roswasmc_bounded_1_uwvm_ros_bounded_v2_Digest_send
#define TAKE_FEEDBACK roswasmc_bounded_1_uwvm_ros_bounded_v2_Digest_feedback
#define TAKE_RESULT roswasmc_bounded_1_uwvm_ros_bounded_v2_Digest_result
#elif UWVM_BOUNDED_ROS_VERSION == 2
#include <roswasm/ros2.h>
#define CONTEXT roswasmc_ros2_backend
#define ABI uwvm_ros2_abi_version
#define ENV roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope
#define ECHO_RQ roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request
#define ECHO_RP roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response
#define EMPTY_RQ roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request
#define EMPTY_RP roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response
#define GOAL roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal
#define FEEDBACK roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback
#define RESULT roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result
#define ECHO_CALL roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_call
#define EMPTY_CALL roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_call
#define SEND roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_send
#define TAKE_FEEDBACK roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_feedback
#define TAKE_RESULT roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_result
#else
#error "Select exactly one bounded custom backend"
#endif
#include "../generated-bounded-v2/bounded_interfaces.h"
#ifndef UWVM_BOUNDED_DENIED
#define UWVM_BOUNDED_DENIED 0
#endif
/** Build guest-owned inline capacities; no native struct or native pointer crosses the ABI. */
static ENV sample(void) {
    ENV value={0};value.label.size=14;memcpy(value.label.data,"bounded-native",14);
    value.samples.size=3;value.samples.data[0]=3;value.samples.data[1]=5;value.samples.data[2]=8;
    value.points.size=2;value.points.data[0].value=-7;value.points.data[0].enabled=1;value.points.data[1].value=11;
    value.labels[0].size=5;memcpy(value.labels[0].data,"first",5);value.labels[1].size=6;memcpy(value.labels[1].data,"second",6);return value;
}
static int equal(ENV const* value) {
    return value->label.size==14&&!memcmp(value->label.data,"bounded-native",14)&&value->samples.size==3&&value->samples.data[0]==3&&value->samples.data[1]==5&&value->samples.data[2]==8&&value->points.size==2&&value->points.data[0].value==-7&&value->points.data[0].enabled==1&&value->points.data[1].value==11&&value->points.data[1].enabled==0&&value->labels[0].size==5&&!memcmp(value->labels[0].data,"first",5)&&value->labels[1].size==6&&!memcmp(value->labels[1].data,"second",6);
}
/** Check every marker write/flush, including closed or short basic stdout. */
static uint32_t marker(char const* value) {return fputs(value,stdout)<0||fflush(stdout)?UWVM_ROS_DISCONNECTED:0;}
int main(void) {
    roswasmc_context context={CONTEXT()};roswasmc_capability echo,empty;roswasmc_action_bundle action={0};uint64_t table[8]={0},id=0;uint32_t error=0,cleanup[5]={0};roswasmc_result reply;
    roswasmc_action_aliases aliases={{"bounded-send",12},{"bounded-cancel",14},{"bounded-feedback",16},{"bounded-result",14},{NULL,0}};
    ECHO_RQ request={0};ECHO_RP response={0};EMPTY_RQ empty_request={0};EMPTY_RP empty_response={0};GOAL goal={0};FEEDBACK feedback={0};RESULT result={0};uint8_t kind=0;unsigned pass;int seen_feedback=0,seen_result=0;
    if(ABI()!=1)return UWVM_ROS_UNSUPPORTED;roswasmc_capability_init(&echo,&context);roswasmc_capability_init(&empty,&context);
    if(UWVM_BOUNDED_DENIED) {
        char const* names[]={"bounded-echo","bounded-empty","bounded-send","bounded-cancel","bounded-feedback","bounded-result"};
        for(pass=0;pass<6;++pass) {roswasmc_capability denied;roswasmc_capability_init(&denied,&context);reply.error=roswasmc_open(&denied,names[pass],strlen(names[pass]));if(reply.error!=13||roswasmc_is_open(&denied)){if(roswasmc_is_open(&denied))roswasmc_close(&denied);return UWVM_ROS_INVALID;}}
        return (int)marker("PASS_BOUNDED_DENIED\n");
    }
    error=roswasmc_open(&echo,"bounded-echo",12);if(error)goto done;error=roswasmc_open(&empty,"bounded-empty",13);if(error)goto done;
    error=roswasmc_action_bundle_init(&context,&action,&aliases,table,8);if(error)goto done;
    request.sample=sample();request.limit=32;reply=ECHO_CALL(&echo,&request,&response);error=reply.error;if(error)goto done;if(!response.accepted||!equal(&response.echoed)){error=22;goto done;}
    reply=EMPTY_CALL(&empty,&empty_request,&empty_response);error=reply.error;if(error)goto done;
    goal.sample=sample();reply=SEND(&action,&goal,&id);error=reply.error;if(error)goto done;error=marker("SDK_BOUNDED_GOAL_SENT\n");if(error)goto done;
    for(pass=0;pass<200&&!seen_result;++pass) {
        if(getchar()!='R'){error=22;goto done;}
        if(!seen_feedback) {reply=TAKE_FEEDBACK(&action,id,&feedback);if(reply.error==11){error=marker("SDK_BOUNDED_AGAIN\n");if(error)goto done;continue;}error=reply.error;if(error)goto done;if(feedback.processed!=3||feedback.phase.size!=8||memcmp(feedback.phase.data,"accepted",8)){error=22;goto done;}seen_feedback=1;error=marker("SDK_BOUNDED_FEEDBACK_DELIVERED\n");if(error)goto done;if(getchar()!='E'){error=22;goto done;}}
        reply=TAKE_RESULT(&action,id,&kind,&result);if(reply.error==11){error=marker("SDK_BOUNDED_AGAIN\n");if(error)goto done;continue;}error=reply.error;if(error)goto done;if(kind!=2||!result.accepted||result.sum!=16||result.note.size!=21||memcmp(result.note.data,"bounded-native-result",21)){error=22;goto done;}seen_result=1;
    }
    if(!seen_feedback||!seen_result)error=110;
 done:
    if(action.active) {roswasmc_cleanup_report report=roswasmc_action_bundle_close_checked(&action,error,cleanup,5);if(!error)error=report.cleanup_error;}
    if(roswasmc_is_open(&empty)){uint32_t code=roswasmc_close(&empty);if(!error)error=code;}
    if(roswasmc_is_open(&echo)){uint32_t code=roswasmc_close(&echo);if(!error)error=code;}
    if(!error)error=marker("PASS_BOUNDED_CUSTOM_CLIENT\n");return (int)error;
}
