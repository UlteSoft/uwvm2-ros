/** @file
 * @brief Pure C11 Session/factory rollback and checked cleanup contracts.
 * Root compiles this single translation unit serially into /tmp. No ROS/native
 * side effects or Guest delivery follow from a model pass. Errors13/95 are
 * refusal/support-boundary assertions, never accepted functional results.
 */
#include "ownership_backend.h"
#include <assert.h>
#include <stdio.h>
static unsigned checks;
#define CHECK(value) do {++checks;assert(value);} while(0)
/** @brief Exercise failures after acquisition and transactional caller output. */
int main(void) {
    roswasmc_backend* backend=fixture_backend();roswasmc_node node;
    roswasmc_capability cap={0},advertise={0},respond={0},storage[2];
    roswasmc_session session;roswasmc_capability* borrowed=(roswasmc_capability*)(uintptr_t)1;
    roswasmc_cleanup_report report;uint32_t codes[2],before;uint64_t nanos=77;
    roswasmc_string allowed={"allowed",7},denied={"denied",6},ad={"advertise",9},reply={"respond",7};
    roswasmc_node_init(&node,backend,(roswasmc_string){"pure-C",6});
    CHECK(fixture_opens==0&&fixture_activations==0);
    CHECK(roswasmc_check_operation(&node.context,(roswasmc_operation)-1)==22);
    CHECK(roswasmc_session_init(&session,backend,storage,1)==0);
    CHECK(roswasmc_session_check_abi(&session)==0&&fixture_opens==0);
    CHECK(roswasmc_session_open(&session,denied.data,denied.size,&borrowed)==13);
    CHECK(borrowed==(roswasmc_capability*)(uintptr_t)1&&session.count==0);
    CHECK(roswasmc_session_open(&session,allowed.data,allowed.size,&borrowed)==0);
    CHECK(borrowed==storage&&session.count==1&&borrowed->handle!=0);
    before=fixture_opens;
    CHECK(roswasmc_session_open(&session,allowed.data,allowed.size,&borrowed)==90&&fixture_opens==before);
    fixture_close_errors[storage[0].handle]=107;
    report=roswasmc_session_close(&session,110,codes,1);
    CHECK(report.primary_error==110&&report.cleanup_error==107&&report.attempted==1&&report.failed==1);
    CHECK(codes[0]==107&&session.count==0&&storage[0].handle==0);
    before=fixture_closes;
    CHECK(roswasmc_close(storage)==9&&fixture_closes==before);
    fixture_activate_error=110;fixture_close_errors[fixture_next]=107;
    CHECK(roswasmc_node_create_subscription_checked(&node,&cap,allowed,&report)==110);
    CHECK(report.primary_error==110&&report.cleanup_error==107&&report.attempted==1&&cap.handle==0);
    fixture_activate_error=0;backend->subscribe_parameter_events=0;before=fixture_opens;
    CHECK(roswasmc_node_create_parameter_events_checked(&node,&cap,allowed,&report)==95);
    CHECK(fixture_opens==before&&report.attempted==0&&cap.handle==0);
    backend->subscribe_parameter_events=fixture_activate;
    before=fixture_opens;
    CHECK(roswasmc_node_create_service_checked(&node,&advertise,&respond,ad,ad,&report)==22&&fixture_opens==before);
    fixture_close_errors[fixture_next]=107;fixture_denied="respond";before=fixture_activations;
    CHECK(roswasmc_node_create_service_checked(&node,&advertise,&respond,ad,reply,&report)==13);
    CHECK(report.primary_error==13&&report.cleanup_error==107&&report.attempted==1&&fixture_activations==before);
    CHECK(advertise.handle==0&&respond.handle==0);
    fixture_denied="denied";
    CHECK(roswasmc_node_create_service_checked(&node,&advertise,&respond,ad,reply,&report)==0);
    fixture_close_errors[advertise.handle]=107;fixture_close_errors[respond.handle]=110;
    report=roswasmc_server_close(&advertise,&respond,22,codes);
    CHECK(report.primary_error==22&&report.cleanup_error==110&&report.attempted==2&&report.failed==2);
    CHECK(codes[0]==107&&codes[1]==110&&advertise.handle==0&&respond.handle==0);
    fixture_activate_error=110;
    CHECK(roswasmc_node_create_action_server_checked(&node,&advertise,&respond,ad,reply,&report)==110);
    CHECK(report.primary_error==110&&report.attempted==2&&advertise.handle==0&&respond.handle==0);
    fixture_activate_error=0;backend->request[ROSWASMC_LIFECYCLE_GET_STATE]=0;before=fixture_opens;
    CHECK(roswasmc_node_create_lifecycle(&node,&cap,allowed,ROSWASMC_LIFECYCLE_GET_STATE)==95&&fixture_opens==before);
    CHECK(roswasmc_node_create_clock(&node,&cap,allowed,ROSWASMC_PARAMETER_GET)==22&&fixture_opens==before);
    CHECK(roswasmc_node_create_clock(&node,&cap,allowed,ROSWASMC_TIME_READ_ROS)==0);
    fixture_request_error=110;before=fixture_requests;
    CHECK(roswasmc_clock_read(&cap,ROSWASMC_TIME_READ_ROS,&nanos)==110);
    CHECK(nanos==77&&fixture_requests==before+1);
    fixture_request_error=0;fixture_payload_size=9;
    CHECK(roswasmc_clock_read(&cap,ROSWASMC_TIME_READ_ROS,&nanos)==90&&nanos==77);
    fixture_payload_size=8;fixture_payload[0]=123;
    CHECK(roswasmc_clock_read(&cap,ROSWASMC_TIME_READ_ROS,&nanos)==0&&nanos==123);
    CHECK(roswasmc_close(&cap)==0&&cap.handle==0);
    CHECK(roswasmc_session_init(&session,backend,storage,2)==0);
    CHECK(roswasmc_session_open(&session,allowed.data,allowed.size,&borrowed)==0);
    CHECK(roswasmc_session_open(&session,allowed.data,allowed.size,&borrowed)==0);
    before=fixture_closes;report=roswasmc_session_close(&session,0,codes,1);
    CHECK(report.cleanup_error==22&&fixture_closes==before&&session.count==2);
    report=roswasmc_session_close(&session,0,codes,2);
    CHECK(report.cleanup_error==0&&report.attempted==2&&session.count==0);
    printf("PASS C resource lifetime models checks=%u\n",checks);return 0;
}
