/** @file Independent live-slot output alias regression; no ROS or transport. */
#include <platform/unitree/c/roswasm_unitree/topic_rpc.h>
#include <string.h>

int main(void) {
    uwvm_unitree_rpc_tracker tracker, before;
    int64_t output = -1;
    uint32_t result;
    memset(&tracker, 0, sizeof tracker);
    uwvm_unitree_rpc_init(&tracker);
    if (uwvm_unitree_rpc_begin(&tracker, 1, 1008, 1, 10) != 0)
        return 1;
    memcpy(&before, &tracker, sizeof tracker);

    /* Slot0 is live; slot1 is free. The original implementation committed
     * ID2 into slot1 and then overwrote slot0.api_id through this output. */
    result =
        uwvm_unitree_rpc_begin_next(&tracker, 1008, 2, 10, &tracker.slots[0].api_id);
    if (result != 22)
        return 2;
    if (memcmp(&before, &tracker, sizeof tracker) != 0)
        return 3;
    if (uwvm_unitree_rpc_find(&tracker, 1, 1008) == NULL)
        return 4;
    if (uwvm_unitree_rpc_begin_next(&tracker, 1008, 2, 10, &output) != 0 || output != 2)
        return 5;
    if (uwvm_unitree_rpc_accept(&tracker, 1, 1008, 3) != 0)
        return 6;
    if (uwvm_unitree_rpc_accept(&tracker, 2, 1008, 3) != 0)
        return 7;
    return 0;
}
