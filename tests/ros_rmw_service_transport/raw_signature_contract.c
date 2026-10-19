/** Exact C11 types from the actual optional SDK header; no Host definitions. */
#include <roswasm/rmw_service_v1.h>
typedef uint32_t(*version_type)(void);
typedef uint32_t(*activate_type)(uint64_t,uint64_t,uint32_t,uint64_t);
typedef uint32_t(*inspect_type)(uint64_t,uint64_t,uint32_t,uint64_t);
typedef uint32_t(*submit_type)(uint64_t,uint64_t,uint32_t,uint64_t,uint64_t,uint64_t);
typedef uint32_t(*abandon_type)(uint64_t,uint64_t,uint64_t);
typedef uint32_t(*respond_type)(uint64_t,uint64_t,uint64_t,uint64_t,uint32_t,uint64_t,uint64_t);
typedef uint32_t(*peek_type)(uint64_t,uint64_t,uint32_t,uint32_t,uint64_t,uint64_t);
typedef uint32_t(*ack_type)(uint64_t,uint64_t,uint64_t,uint64_t);
typedef uint32_t(*available_type)(uint64_t,uint64_t,uint64_t,uint64_t,uint32_t,uint64_t);
typedef uint32_t(*retire_type)(uint64_t,uint64_t);
#define EXACT(n,t) _Static_assert(_Generic(&(n),t:1,default:0),"wrong owned service raw signature")
EXACT(uwvm_ros2_rmw_srv_v1_abi_version,version_type);
EXACT(uwvm_ros2_rmw_srv_v1_client_activate,activate_type);
EXACT(uwvm_ros2_rmw_srv_v1_service_activate,activate_type);
EXACT(uwvm_ros2_rmw_srv_v1_endpoint_inspect,inspect_type);
EXACT(uwvm_ros2_rmw_srv_v1_client_submit,submit_type);
EXACT(uwvm_ros2_rmw_srv_v1_client_abandon,abandon_type);
EXACT(uwvm_ros2_rmw_srv_v1_service_respond,respond_type);
EXACT(uwvm_ros2_rmw_srv_v1_endpoint_peek,peek_type);
EXACT(uwvm_ros2_rmw_srv_v1_endpoint_commit,ack_type);
EXACT(uwvm_ros2_rmw_srv_v1_endpoint_cancel,ack_type);
EXACT(uwvm_ros2_rmw_srv_v1_client_available,available_type);
EXACT(uwvm_ros2_rmw_srv_v1_endpoint_retire,retire_type);
#undef EXACT
