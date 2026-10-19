/* Optional same-Wasm scalar bridge. No dynamic native ROS library or pointers are marshalled. */
#include "roswasm_csharp_owned_bridge.h"
#if !defined(__wasm__) || !defined(__wasm32__)
#error This object is restricted to wasm32
#endif
#if !defined(ROSWASM_BACKEND) || ROSWASM_BACKEND != 2 || !defined(ROSWASM_OWNED_SERVICE) || ROSWASM_OWNED_SERVICE != 1
#error Explicit ROS2 optional owned-service selection required
#endif
uint32_t roswasm_owned_abi_version(void){return uwvm_ros2_rmw_srv_v1_abi_version();}
uint32_t roswasm_owned_client_activate(uint64_t h,uint64_t p,uint32_t m,uint64_t o){return uwvm_ros2_rmw_srv_v1_client_activate(h,p,m,o);}
uint32_t roswasm_owned_service_activate(uint64_t h,uint64_t p,uint32_t m,uint64_t o){return uwvm_ros2_rmw_srv_v1_service_activate(h,p,m,o);}
uint32_t roswasm_owned_endpoint_inspect(uint64_t h,uint64_t i,uint32_t m,uint64_t o){return uwvm_ros2_rmw_srv_v1_endpoint_inspect(h,i,m,o);}
uint32_t roswasm_owned_client_submit(uint64_t h,uint64_t i,uint32_t m,uint64_t p,uint64_t n,uint64_t o){return uwvm_ros2_rmw_srv_v1_client_submit(h,i,m,p,n,o);}
uint32_t roswasm_owned_client_abandon(uint64_t h,uint64_t i,uint64_t t){return uwvm_ros2_rmw_srv_v1_client_abandon(h,i,t);}
uint32_t roswasm_owned_service_respond(uint64_t h,uint64_t i,uint64_t r,uint64_t t,uint32_t m,uint64_t p,uint64_t n){return uwvm_ros2_rmw_srv_v1_service_respond(h,i,r,t,m,p,n);}
uint32_t roswasm_owned_endpoint_peek(uint64_t h,uint64_t i,uint32_t w,uint32_t m,uint64_t o,uint64_t n){return uwvm_ros2_rmw_srv_v1_endpoint_peek(h,i,w,m,o,n);}
uint32_t roswasm_owned_endpoint_commit(uint64_t h,uint64_t i,uint64_t s,uint64_t n){return uwvm_ros2_rmw_srv_v1_endpoint_commit(h,i,s,n);}
uint32_t roswasm_owned_endpoint_cancel(uint64_t h,uint64_t i,uint64_t s,uint64_t n){return uwvm_ros2_rmw_srv_v1_endpoint_cancel(h,i,s,n);}
uint32_t roswasm_owned_client_available(uint64_t h,uint64_t i,uint64_t q,uint64_t w,uint32_t m,uint64_t o){return uwvm_ros2_rmw_srv_v1_client_available(h,i,q,w,m,o);}
uint32_t roswasm_owned_endpoint_retire(uint64_t h,uint64_t i){return uwvm_ros2_rmw_srv_v1_endpoint_retire(h,i);}
