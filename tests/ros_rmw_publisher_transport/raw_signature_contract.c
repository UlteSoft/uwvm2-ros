/** C11 exact numeric import declarations. Compile this translation unit with
 * the actual SDK header; it executes nothing and needs no fake Host symbols.
 * An actual Wasm link/import inventory and policy/native runs are independent.
 */
#include <roswasm/rmw_publisher_v1.h>
typedef uint32_t (*version_type)(void);
typedef uint32_t (*activate_type)(uint64_t,uint32_t,uint64_t);
typedef uint32_t (*inspect_type)(uint64_t,uint64_t,uint32_t,uint64_t);
typedef uint32_t (*publish_type)(uint64_t,uint64_t,uint32_t,uint64_t,uint64_t);
typedef uint32_t (*retire_type)(uint64_t,uint64_t);
#define EXACT(name,type) _Static_assert(_Generic(&(name),type:1,default:0),"wrong raw RMW ABI signature")
EXACT(uwvm_ros2_rmw_v1_abi_version,version_type);
EXACT(uwvm_ros2_rmw_v1_publisher_activate,activate_type);
EXACT(uwvm_ros2_rmw_v1_publisher_inspect,inspect_type);
EXACT(uwvm_ros2_rmw_v1_publisher_publish,publish_type);
EXACT(uwvm_ros2_rmw_v1_publisher_retire,retire_type);
#undef EXACT
