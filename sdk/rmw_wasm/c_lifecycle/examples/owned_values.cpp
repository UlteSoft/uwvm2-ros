// SPDX-License-Identifier: Apache-2.0
// Complete owned-value lesson. Actual generated C functions only; no Node,
// transport, alias open, policy change, clock or Native ROS proof is implied.
#include <uwvm_rmw/c_service_lifecycle.hpp>
#include <rosidl_runtime_c/string_functions.h>
#include <cstdio>
#include <memory>
using Q=type_description_interfaces__srv__GetTypeDescription_Request;
using S=type_description_interfaces__srv__GetTypeDescription_Response;
int main(){
 auto q=uwvm_rmw::c_lifecycle::make_service_object<Q>();
 auto s=uwvm_rmw::c_lifecycle::make_service_object<S>();
 // This value-only lesson acts as the real decoder/handler's single initializer.
 // Never repeat these init calls on already-owned populated objects.
 if(!type_description_interfaces__srv__GetTypeDescription_Request__init(q.get())||
    !type_description_interfaces__srv__GetTypeDescription_Response__init(s.get()))return 12;
 if(!rosidl_runtime_c__String__assign(&q->type_name,"value_only/msg/Example")||
    !rosidl_runtime_c__String__assign(&q->type_hash,"not-an-attested-native-identity")||
    !rosidl_runtime_c__String__assign(&s->failure_reason,"owned by the last shared reference"))return 12;
 q->include_type_sources=true;s->successful=false;
 std::weak_ptr<S> weak=s;std::shared_ptr<void> callback_capture=s;s.reset();
 if(weak.expired())return 1;
 callback_capture.reset(); // actual C fini runs here, not at an earlier encode
 if(!weak.expired())return 1;
 std::puts("OWNED_C_VALUE_ONLY_NO_ROS_ENDPOINT_WAS_CREATED");return 0;
}
