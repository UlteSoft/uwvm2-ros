/** @file
 * @brief Complete C++20 caller-driven event application with checked cleanup.
 * Select ROSWASM_EXAMPLE_ROS_VERSION=1/2; explicitly grant String source sub and
 * subEvents observer with topic-message,resource-error,resource-closed. A native
 * String publisher may deliver before the single poll; idle11 is normal.
 * No background thread/clock, automatic retry or implicit alias is installed.
 * See ../../docs/unified-events.md for the precise explicit policy contract.
 */
#include "select_backend.h"
#include <roswasm/roswasmcpp.hpp>
int main(){
    roswasmc_context context{ROSWASM_BACKEND()};roswasmcpp::Capability source{context,"sub"};if(source.error())return int(source.error());
    roswasmcpp::Capability observer{context,"subEvents"};uint32_t primary{observer.error()},cleanup{};
#if ROSWASM_EXAMPLE_ROS_VERSION == 1
    auto const& api=*roswasmc_ros1_event_backend();
#else
    auto const& api=*roswasmc_ros2_event_backend();
#endif
    roswasmcpp::EventStream events{observer,source,api};if(!primary)primary=events.error();
    roswasmc_event_header last{};
    roswasmcpp::EventSubscription entity{events,256,[&](auto const& header,auto body){
        last=header; // Body is a callback-lifetime span; copy if retained.
        (void)body; // Select the actual granted codec before decoding this body.
    }};
    roswasmcpp::Executor executor;auto registration=executor.add(entity);size_t delivered{};
    if(!primary)primary=entity.error();if(!primary&&!registration)primary=UWVM_ROS_INVALID;
    roswasmc_event_readiness ready{};
    if(!primary)primary=events.ready(ready);
    if(!primary)primary=executor.spin_some(1,delivered);
    cleanup=source.close();
    if(!primary&&!cleanup){primary=entity.poll_once();if(!primary&&last.kind!=ROSWASMC_RESOURCE_CLOSED)primary=UWVM_ROS_INVALID;}
    executor.remove(registration); // Unregister before entity/stream/owner destruction.
    if(observer.is_open()){auto error=events.close_checked();if(!cleanup)cleanup=error;}
    return int(primary?primary:cleanup);
}
