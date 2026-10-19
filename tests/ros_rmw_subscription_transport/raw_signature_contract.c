/** C11 exact numeric declarations against the actual public SDK header.
 * This object requires no fake Host symbols and executes nothing. It proves
 * source declaration types only; use the separate fixture for real imports.
 */
#include <roswasm/rmw_subscription_v1.h>
typedef uint32_t (*version_type)(void);
typedef uint32_t (*activate_type)(uint64_t,uint32_t,uint64_t);
typedef uint32_t (*inspect_type)(uint64_t,uint64_t,uint32_t,uint64_t);
typedef uint32_t (*peek_type)(uint64_t,uint64_t,uint32_t,uint32_t,uint64_t,uint64_t);
typedef uint32_t (*ack_type)(uint64_t,uint64_t,uint64_t,uint64_t);
typedef uint32_t (*retire_type)(uint64_t,uint64_t);
#define EXACT(name,type) _Static_assert(_Generic(&(name),type:1,default:0),"wrong raw RMW Subscription ABI signature")
EXACT(uwvm_ros2_rmw_sub_v1_abi_version,version_type);
EXACT(uwvm_ros2_rmw_sub_v1_subscription_activate,activate_type);
EXACT(uwvm_ros2_rmw_sub_v1_subscription_inspect,inspect_type);
EXACT(uwvm_ros2_rmw_sub_v1_subscription_peek,peek_type);
EXACT(uwvm_ros2_rmw_sub_v1_subscription_commit,ack_type);
EXACT(uwvm_ros2_rmw_sub_v1_subscription_cancel,ack_type);
EXACT(uwvm_ros2_rmw_sub_v1_subscription_retire,retire_type);
#undef EXACT
