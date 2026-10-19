/** @file
 * @brief Checked setup/cleanup shared by runnable C application examples.
 * Host CLI policy owns every alias, endpoint, quota and deadline. Denial13 and
 * unsupported95 remain failures; neither is functional ROS acceptance.
 */
#ifndef ROSWASM_C_EXAMPLE_SUPPORT_H
#define ROSWASM_C_EXAMPLE_SUPPORT_H
#include "select_backend.h"
#include <roswasm/node.h>
/** @brief Select one backend, check ABI1, initialize a stable local Node. */
static inline uint32_t example_node(roswasmc_node* node) {
    roswasmc_string name={"c-application",13};if(!node) return UWVM_ROS_INVALID;
    if(!ROSWASM_BACKEND()->abi_version||ROSWASM_BACKEND()->abi_version()!=1) return UWVM_ROS_UNSUPPORTED;
    roswasmc_node_init(node,ROSWASM_BACKEND(),name);return 0;
}
/** @brief Inspect one close without replay, preserving primary failure. */
static inline uint32_t example_finish(roswasmc_capability* owner,uint32_t primary) {
    roswasmc_cleanup_report report=roswasmc_cleanup_init(primary);roswasmc_cleanup_owner(owner,&report);
    return primary?primary:report.cleanup_error;
}
/** @brief Close every acquired owner in reverse order, even after an error. */
static inline uint32_t example_finish_pair(roswasmc_capability* first,roswasmc_capability* second,uint32_t primary) {
    roswasmc_cleanup_report report=roswasmc_server_close(first,second,primary,0);return primary?primary:report.cleanup_error;
}
#endif
