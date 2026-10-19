/** @file
 * @brief Real C++ typed Node/Publisher/Subscription relay and held native match.
 * One backend and compile-time denied mode follow the C delivery protocol.
 * The public SDK factory/callback path owns and decodes String values; only a
 * reviewed actual-import poll observer checks raw sentinels/canonical bytes.
 * Direct WASI gates use basicstdio only. Link with a custom _start entry to avoid
 * CRT argument/environment startup; see ../c/ADVANCED.md for exact recipes.
 */
#include "../c/06_runtime_delivery_support.h"
#include "example_support.hpp"
#ifndef ROSWASM_EXAMPLE_DEFAULT_DENIED
#define ROSWASM_EXAMPLE_DEFAULT_DENIED 0
#endif
static uint32_t sdk_delivery_main() {
    auto error=example_abi();if(error) return error;
#if ROSWASM_EXAMPLE_DEFAULT_DENIED
    error=sdk_delivery_denied("pub",3);if(!error) error=sdk_delivery_denied("sub",3);
    if(!error) error=sdk_delivery_emit("PASS_SDK_DENIED\n",16);return error;
#else
    auto backend=*ROSWASM_BACKEND();backend.poll=sdk_delivery_observed_poll;
    roswasmcpp::Context context{backend};roswasmcpp::Node node{context,"sdk-delivery"};roswasmcpp::Executor executor{context};
    auto pub=node.create_publisher<roswasmcpp::msg::String>("pub");error=pub.error();if(error) return error;
    auto sub=node.create_subscription<roswasmcpp::msg::String>("sub",64);error=sub.error();unsigned callbacks{};uint32_t callback_error{};
    sub.on_message([&](roswasmcpp::msg::String const& message){if(message.data!="roswasm-js-relay-in") callback_error=UWVM_ROS_INVALID;else ++callbacks;});
    auto registration=error?0:executor.add(sub);if(!error&&!registration) error=UWVM_ROS_INVALID;
    if(!error) error=sdk_delivery_denied("ungranted",9);
    if(!error) error=sdk_delivery_poll_refusal(pub.handle(),UWVM_ROS_DENIED);
    if(!error) error=pub.publish({"warmup-js-relay"});
    if(!error) error=sdk_delivery_emit("SDK_ENDPOINTS_CREATED\n",22);
    if(!error) error=sdk_delivery_gate('P');
    if(!error) error=pub.publish({"roswasm-js-relay-out"});
    if(!error) error=sdk_delivery_emit("SDK_FINAL_SENT\n",15);
    for(unsigned pass=0;!error&&pass<200&&!callbacks;++pass) {
        error=sdk_delivery_gate('R');if(error) break;size_t delivered{};
        error=executor.spin_some(1,delivered);if(sdk_delivery_poll_error) error=sdk_delivery_poll_error;
        if(!error) error=callback_error;
        if(!error&&!delivered) error=sdk_delivery_emit("SDK_POLL_AGAIN\n",15);
    }
    if(!error&&callbacks!=1) error=UWVM_ROS_INVALID;
    if(!error) error=sdk_delivery_emit("SDK_DELIVERED\n",14);
    executor.remove(registration);auto old_pub=pub.handle(),old_sub=sub.handle();
    error=example_finish(sub,error);error=example_finish(pub,error);
    if(!error) error=sdk_delivery_poll_refusal(old_pub,UWVM_ROS_BAD_HANDLE);
    if(!error) error=sdk_delivery_poll_refusal(old_sub,UWVM_ROS_BAD_HANDLE);
    if(!error) error=sdk_delivery_emit("PASS_SDK_DELIVERY\n",18);return error;
#endif
}
/** @brief Objects destruct before checked proc_exit; no argument access is required. */
extern "C" __attribute__((export_name("_start"),noreturn)) void _start() {sdk_delivery_exit(sdk_delivery_main());}
