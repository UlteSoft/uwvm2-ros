/** @file C++20: the same explicitly selected, complete copied telemetry value.
 * Progress from complete_telemetry.c to the real typed Subscription/Codec
 * facade. Build with one matching generated complete directory. The default
 * proves denial13; UWVM_UNITREE_COMPLETE_RECEIVE=1 allows one caller-driven
 * poll only when the operator separately grants the real subscription alias.
 * This is a portable Wasm client Source API, with no native rclcpp ABI or SDK
 * object, device address, lease acquisition, network discovery or command send.
 */
#include <roswasm_unitree/complete_family.h>
#include "bounded_records.hpp"
#ifndef UWVM_UNITREE_COMPLETE_RECEIVE
#define UWVM_UNITREE_COMPLETE_RECEIVE 0
#endif

int main() {
    if(UWVM_UNITREE_COMPLETE_VERSION()!=1)return 95;
    roswasmc_context context{UWVM_UNITREE_COMPLETE_BACKEND()};
    using Message=uwvm_unitree_complete_value;
    roswasmcpp::Subscription<Message> subscription{
        context,UWVM_UNITREE_COMPLETE_ALIAS,UWVM_UNITREE_COMPLETE_CAPACITY};
    auto code=subscription.error();
    if(!UWVM_UNITREE_COMPLETE_RECEIVE) {
        if(code==13&&!subscription.is_open())return 0;
        if(subscription.is_open())(void)subscription.close();
        return 22;
    }
    if(code)return static_cast<int>(code);
    Message latest{};
    bool received{};
    // The callback borrows its argument for this call only. Copy the entire
    // value before returning; close after polling and the callback unwind.
    subscription.on_message([&](Message const& value) noexcept {
        latest=value;
        received=true;
    });
    code=subscription.poll_once();
    if(!code&&!received)code=22;
    auto cleanup=subscription.close();
    if(!code)code=cleanup;
    return static_cast<int>(code);
}
