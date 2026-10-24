/** Non-starting Wasm import-inventory fixture. NEVER execute the exported
 * selector as a robot/application. Its only purpose is keeping all seven REAL
 * SDK imports alive during an actual Wasm link and inspecting their binary
 * signatures. There are no weak substitutes, fabricated receipts or fake Host.
 */
#include <roswasm/rmw_subscription_v1.h>
uint32_t uwvm_rmw_subscription_import_inventory(uint32_t operation,uint64_t source,
  uint64_t incarnation,uint32_t memory,uint64_t output,uint64_t extent,
  uint64_t sequence,uint64_t reservation)
{
  switch(operation){
    case 0:return uwvm_ros2_rmw_sub_v1_abi_version();
    case 1:return uwvm_ros2_rmw_sub_v1_subscription_activate(source,memory,output);
    case 2:return uwvm_ros2_rmw_sub_v1_subscription_inspect(source,incarnation,memory,output);
    case 3:return uwvm_ros2_rmw_sub_v1_subscription_peek(source,incarnation,24,memory,output,extent);
    case 4:return uwvm_ros2_rmw_sub_v1_subscription_commit(source,incarnation,sequence,reservation);
    case 5:return uwvm_ros2_rmw_sub_v1_subscription_cancel(source,incarnation,sequence,reservation);
    case 6:return uwvm_ros2_rmw_sub_v1_subscription_retire(source,incarnation);
    default:return 22;
  }
}
