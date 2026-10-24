/** @file
 * @brief Pure C11 tests of the actual Action bundle with a test-owned backend.
 *
 * Root compiles/runs this translation unit serially. It creates no native ROS,
 * Wasm, Gateway or DDS side effect. Model permission denial/support/retirement
 * and canonical bytes are not native authentication/interoperability proof.
 * Every failure assertion uses one exact code; no fallback accepts another code.
 */
#include "../support/ownership_backend.h"
#include <platform/ros/c/roswasm/action_bundle.h>
#include <assert.h>
#include <stdio.h>

static unsigned checks;
#define CHECK(value) do {++checks;assert(value);} while(0)
static unsigned close_order[128],close_count,revokes,revoked_roles,duplicate_at;
static unsigned handle_role[128],last_operation;
static uint64_t last_goal;
static int unwritten90,written90;
/** @brief Only these literal test aliases exist; unknown inputs are not printed. */
static unsigned model_role(uint64_t alias,uint64_t length) {
    char const* names[5]={"send","own","feedback","result","any"};unsigned i;
    for(i=0;i<5;++i) if(length==strlen(names[i])&&!memcmp((void const*)(uintptr_t)alias,names[i],(size_t)length)) return i;
    return 5;
}
/** @brief Explicit model revoke makes a later matching alias open125. */
static uint32_t model_open(uint32_t memory,uint64_t alias,uint64_t length,uint64_t slot) {
    unsigned role=model_role(alias,length);uint64_t next=fixture_next;uint32_t code;
    if(role<5&&(revoked_roles&(1u<<role))) {++fixture_opens;return 125;}
    if(duplicate_at&&fixture_opens+1==duplicate_at) {++fixture_opens;fixture_store64(slot,1);return 0;}
    code=fixture_open(memory,alias,length,slot);if(!code) handle_role[next]=role;return code;
}
/** @brief Record every actual cleanup attempt in exact reverse-handle order. */
static uint32_t model_close(uint64_t handle) {
    close_order[close_count++]=(unsigned)handle;return fixture_close(handle);
}
/** @brief Destructive alias mutation is separate from plain close in this model. */
static uint32_t model_revoke(uint64_t handle) {
    ++revokes;if(handle<128&&handle_role[handle]<5) revoked_roles|=1u<<handle_role[handle];return model_close(handle);
}
/** @brief Model one real dispatch role; error90 slot variants write no payload. */
static uint32_t model_request(unsigned role,uint64_t handle,uint32_t memory,uint64_t input,uint64_t input_size,
    uint64_t output,uint64_t capacity,uint64_t slot) {
    assert(handle<128&&fixture_live[handle]&&handle_role[handle]==role);last_operation=role;
    if(role) {assert(input_size==8);last_goal=uwvm_ros_load_u64((unsigned char const*)(uintptr_t)input);}
    else assert(input_size==4);
    if(unwritten90||written90) {++fixture_requests;if(written90) fixture_store64(slot,UINT64_C(9007199254740993));return 90;}
    return fixture_request(handle,memory,input,input_size,output,capacity,slot);
}
#define MODEL_REQUEST(name,role) \
static uint32_t name(uint64_t h,uint32_t m,uint64_t a,uint64_t n,uint64_t b,uint64_t c,uint64_t z) {return model_request(role,h,m,a,n,b,c,z);}
MODEL_REQUEST(model_send,0)
MODEL_REQUEST(model_own,1)
MODEL_REQUEST(model_feedback,2)
MODEL_REQUEST(model_result,3)
MODEL_REQUEST(model_any,4)
/** @brief Reset only pure fixture memory; no external process or resource exists. */
static roswasmc_backend* model_reset(void) {
    roswasmc_backend* backend=fixture_backend();fixture_opens=fixture_closes=fixture_activations=fixture_requests=fixture_polls=fixture_publishes=0;
    fixture_next=1;fixture_request_error=fixture_activate_error=0;fixture_payload_size=0;fixture_denied="denied";
    memset(fixture_live,0,sizeof(fixture_live));memset(fixture_close_errors,0,sizeof(fixture_close_errors));memset(fixture_payload,0,sizeof(fixture_payload));
    memset(handle_role,0,sizeof(handle_role));memset(close_order,0,sizeof(close_order));close_count=revokes=revoked_roles=duplicate_at=0;unwritten90=written90=0;
    last_goal=0;last_operation=99;backend->open=model_open;backend->close=model_close;backend->revoke=model_revoke;
    backend->request[ROSWASMC_ACTION_SEND_GOAL]=model_send;backend->request[ROSWASMC_ACTION_CANCEL_OWN_GOAL]=model_own;
    backend->request[ROSWASMC_ACTION_RECEIVE_FEEDBACK]=model_feedback;backend->request[ROSWASMC_ACTION_RECEIVE_RESULT]=model_result;
    backend->request[ROSWASMC_ACTION_CANCEL_ANY_GOAL]=model_any;return backend;
}
/** @brief Exact canonical native reply ID, never a locally guessed permission. */
static void model_id(uint64_t goal) {fixture_store64((uint64_t)(uintptr_t)fixture_payload,goal);fixture_payload_size=8;}
/** @brief One complete feedback/result envelope with bounded signed values. */
static void model_sequence(uint64_t goal,unsigned char kind,int terminal) {
    unsigned char values[8]={0,0,0,0,1,0,0,0};size_t length;
    roswasmc_action_sequence value={goal,kind,2,values};assert(roswasmc_encode_action_sequence(&value,terminal,fixture_payload,sizeof(fixture_payload),&length)==0);fixture_payload_size=length;
}
/** @brief All roles are supplied by the test, including explicitly absent any. */
static roswasmc_action_aliases model_aliases(int any) {
    roswasmc_action_aliases aliases={{"send",4},{"own",3},{"feedback",8},{"result",6},{0,0}};
    if(any) {aliases.cancel_any.data="any";aliases.cancel_any.size=3;}return aliases;
}
/** @brief Verify preflight/rollback, then request/cleanup lifetime state machines. */
int main(void) {
    roswasmc_backend* backend=model_reset();roswasmc_context context={backend};roswasmc_action_bundle bundle={0};
    roswasmc_action_aliases aliases=model_aliases(0),bad;uint64_t goals[2]={77,88},goal=55;
    uint32_t statuses[5]={71,72,73,74,75};roswasmc_cleanup_report report;roswasmc_result result;
    unsigned before;int accepted=77;unsigned char output[64];roswasmc_action_sequence sequence={66,4,1,(unsigned char const*)(uintptr_t)1};

    CHECK(roswasmc_action_bundle_init(&context,&bundle,&aliases,goals,0)==22&&fixture_opens==0);
    CHECK(roswasmc_action_bundle_init(&context,&bundle,&aliases,goals,1025u)==22&&fixture_opens==0);
    bad=aliases;bad.result=bad.send;
    CHECK(roswasmc_action_bundle_init(&context,&bundle,&bad,goals,2)==22&&fixture_opens==0);
    bad=aliases;bad.send.data=".";bad.send.size=1;
    CHECK(roswasmc_action_bundle_init(&context,&bundle,&bad,goals,2)==22&&fixture_opens==0);
    bad=aliases;bad.send.data="..";bad.send.size=2;
    CHECK(roswasmc_action_bundle_init(&context,&bundle,&bad,goals,2)==22&&fixture_opens==0);
    bad=aliases;bad.cancel_any.data="";
    CHECK(roswasmc_action_bundle_init(&context,&bundle,&bad,goals,2)==22&&fixture_opens==0);
    bad=aliases;bad.feedback.data="bad\0name";bad.feedback.size=8;
    CHECK(roswasmc_action_bundle_init(&context,&bundle,&bad,goals,2)==22&&fixture_opens==0);
    backend->request[ROSWASMC_ACTION_RECEIVE_RESULT]=0;
    CHECK(roswasmc_action_bundle_init(&context,&bundle,&aliases,goals,2)==95&&fixture_opens==0);
    CHECK(bundle.rollback.primary_error==95&&bundle.rollback.attempted==0);

    {uint64_t maximum_goals[1024];backend=model_reset();context.backend=backend;
        CHECK(roswasmc_action_bundle_init(&context,&bundle,&aliases,maximum_goals,1024)==0&&fixture_opens==4&&bundle.count==0);
        report=roswasmc_action_bundle_close_checked(&bundle,0,0,0);
        CHECK(report.attempted==4&&!report.cleanup_error&&!bundle.active);}

    backend=model_reset();context.backend=backend;fixture_denied="feedback";fixture_close_errors[1]=107;fixture_close_errors[2]=110;
    CHECK(roswasmc_action_bundle_init(&context,&bundle,&aliases,goals,2)==13);
    CHECK(fixture_opens==3&&fixture_closes==2&&close_order[0]==2&&close_order[1]==1);
    CHECK(bundle.rollback.primary_error==13&&bundle.rollback.cleanup_error==110&&bundle.rollback.attempted==2&&bundle.rollback.failed==2);
    CHECK(bundle.rollback_status[0]==107&&bundle.rollback_status[1]==110&&bundle.rollback_status[2]==0);
    CHECK(!bundle.active&&bundle.count==0&&bundle.cap[0].handle==0&&bundle.cap[1].handle==0);

    backend=model_reset();context.backend=backend;duplicate_at=2;
    CHECK(roswasmc_action_bundle_init(&context,&bundle,&aliases,goals,2)==17);
    CHECK(fixture_opens==2&&fixture_closes==1&&close_order[0]==1&&bundle.rollback.attempted==1);
    CHECK(bundle.cap[0].handle==0&&bundle.cap[1].handle==0&&!bundle.active);

    backend=model_reset();context.backend=backend;
    CHECK(roswasmc_action_bundle_init(&context,&bundle,&aliases,goals,2)==0&&fixture_opens==4&&bundle.roles==4);
    before=fixture_opens;CHECK(roswasmc_action_bundle_init(&context,&bundle,&aliases,goals,2)==17&&fixture_opens==before&&bundle.active);
    before=fixture_requests;result=roswasmc_action_bundle_cancel_all(&bundle,0);
    CHECK(result.error==13&&!result.has_length&&fixture_requests==before);
    result=roswasmc_action_bundle_cancel_any(&bundle,0,0);
    CHECK(result.error==13&&!result.has_length&&fixture_requests==before);
    result=roswasmc_action_bundle_cancel_own(&bundle,9,&accepted);
    CHECK(result.error==9&&accepted==77&&fixture_requests==before);
    result=roswasmc_action_bundle_feedback(&bundle,9,output,sizeof(output),&sequence);
    CHECK(result.error==9&&sequence.goal==66&&fixture_requests==before);
    result=roswasmc_action_bundle_send_goal(&bundle,-1,&goal);
    CHECK(result.error==22&&goal==55&&fixture_requests==before);
    result=roswasmc_action_bundle_send_goal(&bundle,47,&goal);
    CHECK(result.error==22&&fixture_requests==before);
    result=roswasmc_action_bundle_send_goal(&bundle,5,goals);
    CHECK(result.error==22&&fixture_requests==before&&goals[0]==77);
    fixture_request_error=110;result=roswasmc_action_bundle_send_goal(&bundle,5,&goal);
    CHECK(result.error==110&&!result.has_length&&goal==55&&bundle.count==0);
    fixture_request_error=0;model_id(0);result=roswasmc_action_bundle_send_goal(&bundle,5,&goal);
    CHECK(result.error==22&&result.has_length&&result.length==8&&goal==55&&bundle.count==0);
    model_id(UINT64_C(9223372036854775813));result=roswasmc_action_bundle_send_goal(&bundle,5,&goal);
    CHECK(result.error==0&&result.has_length&&result.length==8&&goal==UINT64_C(9223372036854775813));
    CHECK(bundle.count==1&&goals[0]==goal&&roswasmc_action_bundle_owns(&bundle,goal));
    before=fixture_requests;{uint64_t unchanged=91;result=roswasmc_action_bundle_send_goal(&bundle,5,&unchanged);
        CHECK(result.error==17&&unchanged==91&&bundle.count==1&&fixture_requests==before+1);}
    model_id(7);{uint64_t second=0;result=roswasmc_action_bundle_send_goal(&bundle,5,&second);CHECK(!result.error&&second==7&&bundle.count==2);}
    before=fixture_requests;result=roswasmc_action_bundle_send_goal(&bundle,5,&goal);
    CHECK(result.error==90&&!result.has_length&&fixture_requests==before&&bundle.count==2);

    memset(output,0xa5,sizeof(output));fixture_request_error=11;
    result=roswasmc_action_bundle_feedback(&bundle,goal,output,sizeof(output),&sequence);
    CHECK(result.error==11&&!result.has_length&&bundle.count==2&&sequence.goal==66&&output[0]==0xa5);
    fixture_request_error=110;result=roswasmc_action_bundle_result(&bundle,goal,output,sizeof(output),&sequence);
    CHECK(result.error==110&&!result.has_length&&bundle.count==2&&sequence.goal==66&&output[0]==0xa5);
    fixture_request_error=0;unwritten90=1;result=roswasmc_action_bundle_feedback(&bundle,goal,output,sizeof(output),&sequence);
    CHECK(result.error==90&&!result.has_length&&result.length==0&&bundle.count==2&&sequence.goal==66);
    unwritten90=0;written90=1;result=roswasmc_action_bundle_result(&bundle,goal,output,sizeof(output),&sequence);
    CHECK(result.error==90&&result.has_length&&result.length==UINT64_C(9007199254740993)&&bundle.count==2&&sequence.goal==66);
    written90=0;model_sequence(99,1,0);result=roswasmc_action_bundle_feedback(&bundle,goal,output,sizeof(output),&sequence);
    CHECK(result.error==22&&bundle.count==2&&sequence.goal==66);
    model_sequence(goal,1,0);result=roswasmc_action_bundle_feedback(&bundle,goal,output,sizeof(output),&sequence);
    CHECK(!result.error&&sequence.goal==goal&&sequence.count==2&&bundle.count==2&&last_operation==2&&last_goal==goal);
    before=fixture_requests;result=roswasmc_action_bundle_feedback(&bundle,goal,goals,sizeof(goals),&sequence);
    CHECK(result.error==22&&fixture_requests==before&&bundle.count==2);
    result=roswasmc_action_bundle_feedback(&bundle,goal,output,sizeof(output),(roswasmc_action_sequence*)(void*)output);
    CHECK(result.error==22&&fixture_requests==before&&bundle.count==2);
    result=roswasmc_action_bundle_cancel_own(&bundle,goal,(int*)(void*)goals);
    CHECK(result.error==22&&fixture_requests==before&&goals[0]==goal);
    fixture_payload[0]=2;fixture_payload_size=1;accepted=77;
    result=roswasmc_action_bundle_cancel_own(&bundle,goal,&accepted);
    CHECK(result.error==22&&accepted==77&&bundle.count==2);
    fixture_payload[0]=1;fixture_payload_size=1;accepted=77;
    result=roswasmc_action_bundle_cancel_own(&bundle,goal,&accepted);
    CHECK(!result.error&&accepted==1&&bundle.count==2&&last_operation==1&&last_goal==goal);
    model_sequence(goal,1,1);result=roswasmc_action_bundle_result(&bundle,goal,output,sizeof(output),&sequence);
    CHECK(result.error==22&&bundle.count==2);
    model_sequence(goal,2,1);result=roswasmc_action_bundle_result(&bundle,goal,output,sizeof(output),&sequence);
    CHECK(!result.error&&sequence.goal==goal&&sequence.kind==2&&bundle.count==1);
    CHECK(!roswasmc_action_bundle_owns(&bundle,goal)&&roswasmc_action_bundle_owns(&bundle,7)&&goals[0]==7&&goals[1]==0);
    before=fixture_requests;result=roswasmc_action_bundle_result(&bundle,goal,output,sizeof(output),&sequence);
    CHECK(result.error==9&&fixture_requests==before);
    before=fixture_closes;report=roswasmc_action_bundle_close_checked(&bundle,22,statuses,3);
    CHECK(report.primary_error==22&&report.cleanup_error==22&&bundle.active&&bundle.count==1&&fixture_closes==before);
    fixture_close_errors[4]=110;fixture_close_errors[3]=107;
    report=roswasmc_action_bundle_close_checked(&bundle,22,statuses,5);
    CHECK(report.primary_error==22&&report.cleanup_error==110&&report.attempted==4&&report.failed==2);
    CHECK(close_order[0]==4&&close_order[1]==3&&close_order[2]==2&&close_order[3]==1);
    CHECK(statuses[0]==0&&statuses[1]==0&&statuses[2]==107&&statuses[3]==110);
    CHECK(!bundle.active&&bundle.count==0&&goals[0]==0&&bundle.error==9);
    before=fixture_closes;report=roswasmc_action_bundle_close_checked(&bundle,0,statuses,5);
    CHECK(report.cleanup_error==9&&report.attempted==0&&fixture_closes==before);
    result=roswasmc_action_bundle_cancel_own(&bundle,7,&accepted);CHECK(result.error==9);

    backend=model_reset();context.backend=backend;aliases=model_aliases(1);
    CHECK(roswasmc_action_bundle_init(&context,&bundle,&aliases,goals,2)==0&&bundle.roles==5&&fixture_opens==5);
    model_id(10);result=roswasmc_action_bundle_send_goal(&bundle,0,&goal);CHECK(!result.error&&bundle.count==1);
    fixture_payload_size=1;fixture_payload[0]=0;accepted=77;
    before=fixture_requests;result=roswasmc_action_bundle_cancel_any(&bundle,0,&accepted);
    CHECK(result.error==22&&!result.has_length&&fixture_requests==before&&accepted==77&&bundle.count==1);
    fixture_payload[0]=1;result=roswasmc_action_bundle_cancel_any(&bundle,999,&accepted);
    CHECK(!result.error&&accepted==1&&!roswasmc_action_bundle_owns(&bundle,999)&&bundle.count==1&&goals[0]==10);
    CHECK(last_goal==999&&last_operation==4&&fixture_requests==before+1);
    fixture_payload[0]=0;
    result=roswasmc_action_bundle_cancel_all(&bundle,&accepted);
    CHECK(!result.error&&accepted==0&&last_goal==0&&last_operation==4&&bundle.count==1);
    fixture_close_errors[5]=107;
    report=roswasmc_action_bundle_revoke_checked(&bundle,110,statuses,5);
    CHECK(report.primary_error==110&&report.cleanup_error==107&&report.attempted==5&&report.failed==1);
    CHECK(revokes==5&&revoked_roles==31&&close_order[0]==5&&close_order[4]==1&&bundle.count==0&&!bundle.active);
    before=revokes;report=roswasmc_action_bundle_revoke_checked(&bundle,0,statuses,5);
    CHECK(report.cleanup_error==9&&revokes==before);
    CHECK(roswasmc_action_bundle_init(&context,&bundle,&aliases,goals,2)==125&&bundle.rollback.attempted==0);
    CHECK(!bundle.active&&bundle.error==125);
    printf("PASS C Action bundle contracts checks=%u\n",checks);return 0;
}
