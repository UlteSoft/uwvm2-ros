/** Actual C11 Guest command AND exported caller-driven reactor entrypoints.
 * Compile a FRESH instance per backend and mode. No default alias grant.
 * The parent telemetry.c is the unchanged current SDK component, not a stub.
 * Add this directory to the real compiler include roots so its vendor_binding.h
 * resolves the concrete complete generated record above.
 */
#include <stdio.h>
#include "vendor_binding.h"
#include "../../../vendors/examples/telemetry.c"
#ifndef UWVM_XARM_CIO_STATE_MODE
#define UWVM_XARM_CIO_STATE_MODE 0
#endif
#if UWVM_XARM_CIO_STATE_MODE != 0 && UWVM_XARM_CIO_STATE_MODE != 1
#error "Mode0 denied or mode1 explicitly authorized; no implicit positive mode"
#endif
uint32_t xarm_cio_state_initialize(uint32_t mode){return initialize_case(mode,"xarm-cio-state",14u);}
uint32_t xarm_cio_state_poll_once(void){return poll_once();}
uint32_t xarm_cio_state_close(void){return close_case();}
/** Every R from actual basic stdin releases ONE poll; an idle11 consumes a
 * caller credit, not a delivered sample. No timer/thread/reopen/grow-and-retry.
 */
int main(void) {
    uint32_t first=xarm_cio_state_initialize(UWVM_XARM_CIO_STATE_MODE),cleanup=0;unsigned pass;
    if(UWVM_XARM_CIO_STATE_MODE==0) {
        if(!first&&(!denied_verified()||received_count()!=0u))first=22;
    } else if(!first) {
        for(pass=0;pass<64u&&received_count()==0u;++pass) {
            if(getchar()!='R'){first=22;break;}
            first=xarm_cio_state_poll_once();if(first==11u){first=0;continue;}if(first)break;
        }
        if(!first&&received_count()!=1u)first=110;
        cleanup=xarm_cio_state_close();if(!first)first=cleanup;
    }
    if(!first&&cleanup_error())first=cleanup_error();
    if(!first&&(fputs(UWVM_XARM_CIO_STATE_MODE?"PASS_XARM_CIO_STATE_FULL_VALUE\n":"PASS_XARM_CIO_STATE_DEFAULT_DENIED\n",stdout)<0||fflush(stdout)))first=107;
    return (int)first;
}
