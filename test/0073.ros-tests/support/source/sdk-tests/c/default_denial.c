/** Real import/Runtime denial fixture. Run ONLY with empty default ROS policy.
 * This source provides no import definitions and does not invoke native ROS.
 * A success result requires actual ABI1 and both raw/typed alias denial13.
 */
#include "select_backend.h"
#include <platform/ros/c/roswasm/roswasm.h>
int main(void) {
    roswasmc_context context;roswasmc_capability owner;roswasmc_node node;
    roswasmc_string alias;unsigned char slot[8]={0};uwvm_ros_u32 error;
    if(ROSWASM_IMPORT(abi_version)()!=1) return 1;
    error=ROSWASM_IMPORT(capability_open)(0,uwvm_ros_address("absent"),6,uwvm_ros_address(slot));
    if(error!=UWVM_ROS_DENIED||uwvm_ros_load_u64(slot)) return 2;
    context.backend=ROSWASM_BACKEND();roswasmc_capability_init(&owner,&context);
    if(roswasmc_open(&owner,"absent",6)!=UWVM_ROS_DENIED||owner.handle) return 3;
    alias=roswasmc_string_init("absent",6);roswasmc_node_init(&node,context.backend,alias);
    if(roswasmc_node_create_publisher(&node,&owner,alias)!=UWVM_ROS_DENIED||owner.handle) return 4;
    if(roswasmc_publish_bool(&owner,1)!=UWVM_ROS_BAD_HANDLE) return 5;
    return 0;
}
