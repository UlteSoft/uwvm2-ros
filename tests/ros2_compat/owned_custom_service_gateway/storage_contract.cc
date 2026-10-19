/* SPDX-License-Identifier: Apache-2.0 */
/** Source-authored finite arithmetic contract, not native delivery evidence.
 * Compile against the actual production header. These independent expected
 * byte counts cover unequal full variable bodies, both directions, a dedicated
 * control slot and integer overflow. No ROS object/provider or grant is faked.
 */
#include "gateways/common/owned_service_storage.h"
#include <uwvm2/imported/ros/ipc/event.h>
namespace s=uwvm2::ros_gateway::owned_service_storage;
static_assert(uwvm2::imported::ros::ipc::event_header_bytes==64,"actual UWRE v1 header");
int main()
{
    s::bodies empty{0,0};s::grant p{1,2,128,1,1,0,0,64,true};
    if(s::admit(empty,p,64)!=0)return 1;
    p.queue_bytes=127;if(s::admit(empty,p,64)!=90)return 2;
    p={1,2,136,1,1,16,8,72,true};if(s::admit({16,8},p,64)!=0)return 3;
    p.client=false;p.queue_bytes=144;if(s::admit({16,8},p,64)!=0)return 4;
    p.queue_bytes=143;if(s::admit({16,8},p,64)!=90)return 5;
    p={4,5,36320,4,4,512,9000,9064,true};if(s::admit({512,9000},p,64)!=0)return 6;
    p.queue_bytes=36319;if(s::admit({512,9000},p,64)!=90)return 7;
    p.queue_bytes=36320;p.observer_max_event_bytes=9063;if(s::admit({512,9000},p,64)!=90)return 8;
    p.observer_max_event_bytes=9064;p.queue_depth=4;if(s::admit({512,9000},p,64)!=90)return 9;
    p.queue_depth=5;p.max_inflight=5;if(s::admit({512,9000},p,64)!=90)return 10;
    p.max_inflight=4;p.qos_depth=5;if(s::admit({512,9000},p,64)!=90)return 11;
    p.qos_depth=4;p.max_request_bytes=511;if(s::admit({512,9000},p,64)!=90)return 12;
    p.max_request_bytes=512;p.max_response_bytes=8999;if(s::admit({512,9000},p,64)!=90)return 13;
    p={256,257,8405056,256,256,32768,32768,32832,true};if(s::admit({32768,32768},p,64)!=0)return 14;
    p.queue_bytes=8405055;if(s::admit({32768,32768},p,64)!=90)return 15;
    p.queue_bytes=UINT64_MAX;if(s::admit({32769,32768},p,64)!=90)return 16;
    if(s::admit({32768,32768},p,UINT64_MAX)!=90)return 17;
    if(s::admit({32768,32768},p,0)!=90)return 18;
    p.history=0;if(s::admit({32768,32768},p,64)!=90)return 19;
    p.history=257;if(s::admit({32768,32768},p,64)!=90)return 20;
    // Actual original ParametersQoS DDS depth1000 is a different owner from
    // four immutable canonical lifetime slots. The trusted bounds selector,
    // not a new policy/packet field, supplies the independent queue ceiling.
    static_assert(s::bodies{512,9000}.separately_owned_native_depth_limit==0,
        "old two-value aggregate retains the original coupled model");
    s::bodies independent{512,9000,1000};
    p={4,5,36320,4,1000,512,9000,9064,true};
    if(s::admit({512,9000},p,64)!=90)return 21; // same old default still refuses
    if(s::admit(independent,p,64)!=0)return 22; // exact client reservation
    p.queue_bytes=36319;if(s::admit(independent,p,64)!=90)return 23;
    p.queue_bytes=36320;p.observer_max_event_bytes=9063;
    if(s::admit(independent,p,64)!=90)return 24;
    p.observer_max_event_bytes=9064;p.queue_depth=4;
    if(s::admit(independent,p,64)!=90)return 25;
    p.queue_depth=5;p.max_inflight=5;
    if(s::admit(independent,p,64)!=90)return 26;
    p.max_inflight=4;p.qos_depth=0;if(s::admit(independent,p,64)!=90)return 27;
    p.qos_depth=1001;if(s::admit(independent,p,64)!=90)return 28;
    p.qos_depth=1000;independent.separately_owned_native_depth_limit=999;
    if(s::admit(independent,p,64)!=90)return 29;
    independent.separately_owned_native_depth_limit=1001;
    if(s::admit(independent,p,64)!=90)return 30;
    independent.separately_owned_native_depth_limit=UINT64_MAX;
    if(s::admit(independent,p,64)!=90)return 31;
    independent.separately_owned_native_depth_limit=1000;
    p.client=false;p.observer_max_event_bytes=0;p.queue_bytes=2368;
    if(s::admit(independent,p,64)!=0)return 32; // 4*(64+512)+64, not responses
    p.queue_bytes=2367;if(s::admit(independent,p,64)!=90)return 33;
    p.queue_bytes=2368;p.max_request_bytes=511;
    if(s::admit(independent,p,64)!=90)return 34;
    p.max_request_bytes=512;p.max_response_bytes=8999;
    if(s::admit(independent,p,64)!=90)return 35;
    p.max_response_bytes=9000;p.history=257;p.queue_depth=258;p.queue_bytes=UINT64_MAX;
    if(s::admit(independent,p,64)!=90)return 36;
    p.history=4;p.queue_depth=5;if(s::admit(independent,p,UINT64_MAX)!=90)return 37;
    if(s::admit({32769,9000,1000},p,64)!=90)return 38;
    // Legacy arithmetic accepted qos_depth0; its actual QoS validator remains
    // independently required. Do not silently change that old helper result.
    p={1,2,128,1,0,0,0,64,true};if(s::admit({0,0},p,64)!=0)return 39;
    if(s::admit({0,0,1},p,64)!=90)return 40;
    return 0;
}
