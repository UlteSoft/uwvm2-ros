/** @file
 * @brief Runnable stable Session ownership and deliberate optional alias revoke.
 * ROSWASM_EXAMPLE_REVOKE=1 intentionally revokes pub and checks reopen125.
 * This mode is separate from ordinary checked close/local Context shutdown.
 * Every acquired slot is closed once and each cleanup status is inspected.
 */
#include "example_support.hpp"
#ifndef ROSWASM_EXAMPLE_REVOKE
#define ROSWASM_EXAMPLE_REVOKE 0
#endif
int main() {
    roswasmcpp::Context context{*ROSWASM_BACKEND()};roswasmcpp::Session session{context,1};auto error=session.check_abi();
    if(!error) {
        auto opened=session.open("pub");error=opened.error;
        if(!error) {
#if ROSWASM_EXAMPLE_REVOKE
            error=opened.owner->revoke();if(!error) {roswasmcpp::Capability next{session.context(),"pub"};
                if(next.error()!=UWVM_ROS_REVOKED) error=UWVM_ROS_INVALID;error=example_finish(next,error);}
#else
            auto bytes=roswasmcpp::wire::string("session publication");error=opened.owner->publish_bytes(bytes);
#endif
        }
    }
    context.shutdown();uint32_t statuses[1]{};auto cleanup=session.close(statuses,error);
    return static_cast<int>(error?error:cleanup.cleanup_error);
}
