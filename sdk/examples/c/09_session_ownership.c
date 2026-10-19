/** @file
 * @brief Runnable bounded Session ownership and explicit optional revocation.
 * Default opens one pub grant, publishes once and checks reverse cleanup. Set
 * ROSWASM_EXAMPLE_REVOKE=1 to deliberately revoke that configured alias instead
 * and check subsequent open125. Revocation is an intentional destructive alias
 * operation; it is not part of ordinary Context shutdown or a hidden cleanup.
 * A successful revocation-negative check is not native message delivery proof.
 */
#include "example_support.h"
#ifndef ROSWASM_EXAMPLE_REVOKE
#define ROSWASM_EXAMPLE_REVOKE 0
#endif
int main(void) {
    roswasmc_local_context context;roswasmc_session session;roswasmc_capability slots[1];roswasmc_capability* owner=0;
    unsigned char scratch[64];roswasmc_string message={"session publication",19};uint32_t error,statuses[1];roswasmc_cleanup_report cleanup;
    roswasmc_local_context_init(&context);error=roswasmc_session_init_context(&session,ROSWASM_BACKEND(),&context,slots,1);
    if(error) return (int)error;error=roswasmc_session_check_abi(&session);
    if(!error) error=roswasmc_session_open(&session,"pub",3,&owner);
    if(!error) {
#if ROSWASM_EXAMPLE_REVOKE
        error=roswasmc_revoke(owner);
        if(!error) {roswasmc_capability next;roswasmc_capability_init(&next,&session.context);
            uint32_t refusal=roswasmc_open(&next,"pub",3);if(refusal!=UWVM_ROS_REVOKED) error=UWVM_ROS_INVALID;
            if(next.handle) error=example_finish(&next,error);}
#else
        error=roswasmc_publish_string(owner,message,scratch,sizeof scratch);
#endif
    }
    roswasmc_local_context_shutdown(&context);cleanup=roswasmc_session_close(&session,error,statuses,1);
    return (int)(error?error:cleanup.cleanup_error);
}
