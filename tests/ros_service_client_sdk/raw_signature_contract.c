/* SPDX-License-Identifier: Apache-2.0
 * Source-only C11 contract against the actual separate raw extension header.
 * This does not invoke an import or establish a Wasm/native runtime result.
 */
#include <roswasm/service_client_ros1.h>
#include <roswasm/service_client_ros2.h>
typedef uint32_t (*version_signature)(void);
typedef uint32_t (*features_signature)(uint64_t,uint64_t,uint32_t,uint64_t);
typedef uint32_t (*query_signature)(uint64_t,uint64_t,uint64_t,uint32_t,uint64_t);
#define SAME_TYPE(expression,type) _Generic((expression),type:1,default:0)
_Static_assert(SAME_TYPE(&uwvm_ros1_client_v1_abi_version,version_signature),"ROS1 optional version tuple");
_Static_assert(SAME_TYPE(&uwvm_ros2_client_v1_abi_version,version_signature),"ROS2 optional version tuple");
_Static_assert(SAME_TYPE(&uwvm_ros1_client_v1_features,features_signature),"ROS1 feature8 tuple");
_Static_assert(SAME_TYPE(&uwvm_ros2_client_v1_features,features_signature),"ROS2 feature8 tuple");
_Static_assert(SAME_TYPE(&uwvm_ros1_client_v1_service_available,query_signature),"ROS1 bool1 tuple");
_Static_assert(SAME_TYPE(&uwvm_ros2_client_v1_service_available,query_signature),"ROS2 bool1 tuple");
_Static_assert(sizeof(uint64_t)==8&&sizeof(uint32_t)==4,"full-width handles and memory selector");
int main(void){return 0;}
