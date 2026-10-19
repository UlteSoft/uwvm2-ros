/** Complete typed telemetry ownership lesson, C11, source-only/unexecuted.
 * vendor_binding.h must be a separately reviewed model binding over the ENTIRE
 * freshly generated bounded record. It defines vendor_record,
 * VENDOR_MAX_WIRE_BYTES, vendor_decode(bytes,n,&value), and
 * vendor_observe_record(&value). No default body, projected struct, native
 * pointer, guessed field offset, topic name, or native identity is supplied.
 * The observer must compare/expose the actual model values for the independent
 * native oracle. Its zero status alone cannot establish native delivery.
 * Compile a fresh reactor per ROS major, initialize once, then let the owned
 * controller call poll_once at most4096 times. No timer, retry, thread or grant.
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#if UWVM_VENDOR_ROS_VERSION == 1
#include <roswasm/ros1.h>
#define BACKEND roswasmc_ros1_backend
#elif UWVM_VENDOR_ROS_VERSION == 2
#include <roswasm/ros2.h>
#define BACKEND roswasmc_ros2_backend
#else
#error "Explicitly select one actual ROS backend; no fallback"
#endif
#include <roswasm/session.h>
#include "vendor_binding.h"
#if VENDOR_MAX_WIRE_BYTES < 1 || VENDOR_MAX_WIRE_BYTES > 32768
#error "A complete, source-locked bounded record is required"
#endif
static roswasmc_session session;
static roswasmc_capability storage[1],*subscription;
static unsigned char scratch[VENDOR_MAX_WIRE_BYTES];
static uint32_t started,closed,status,polls,received,denied,cleanup_status,polling;
/** A close attempts the one current owner once even after an earlier error.
 * A trap/kill still requires external Runtime/Gateway ownership cleanup. */
static uint32_t finish(uint32_t first) {
    uint32_t slots[1]={0};roswasmc_cleanup_report report;
    if(closed)return first?first:9;closed=1;
    report=roswasmc_session_close(&session,first,slots,1);
    cleanup_status=report.cleanup_error;status=first?first:report.cleanup_error;return status;
}
/** alias is a trusted example-binding literal, not a native topic selector.
 * Mode0 credits ONLY actual errno13 with no acquired handle, after ABI check.
 * Mode1 requires a separate fresh instance and explicit host subscribe policy. */
uint32_t initialize_case(uint32_t mode,char const* alias,size_t length) {
    uint32_t code;
    if(started||closed)return 9;
    if(mode>1||!alias||!length||length>64)return 22;
    started=1;
    code=roswasmc_session_init(&session,BACKEND(),storage,1);if(code){status=code;closed=1;return code;}
    code=roswasmc_session_check_abi(&session);if(code)return finish(code);
    code=roswasmc_session_open(&session,alias,length,&subscription);
    if(mode==0){denied=(code==13&&session.count==0);return finish(denied?0:(code?code:22));}
    if(code)return finish(code);
    code=roswasmc_subscribe(subscription);return code?finish(code):0;
}
/** Full generated decode precedes the model observer. Idle11 is not delivery;
 * errors90/107/110 never grow a buffer or repeat a consumed native operation. */
uint32_t poll_once(void) {
    roswasmc_result result;vendor_record value;uint32_t code;
    if(polling)return 17;if(!started||closed)return 9;if(polls++>=4096)return finish(110);polling=1;
    result=roswasmc_poll(subscription,scratch,sizeof scratch);
    if(result.error==11){polling=0;return 11;}if(result.error){polling=0;return finish(result.error);}
    if(!result.has_length||result.length>sizeof scratch){polling=0;return finish(22);}
    code=vendor_decode(scratch,(size_t)result.length,&value);if(code){polling=0;return finish(code);}
    code=vendor_observe_record(&value);polling=0;if(code)return finish(code);
    ++received;return 0;
}
uint32_t received_count(void){return received;}
uint32_t denied_verified(void){return denied;}
uint32_t case_status(void){return status;}
uint32_t cleanup_error(void){return cleanup_status;}
uint32_t close_case(void){return polling?17:(!started?9:finish(status));}
