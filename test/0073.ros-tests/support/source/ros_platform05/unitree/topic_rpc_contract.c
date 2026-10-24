/** Local lifecycle model only; no ROS Service, DDS RPC, network or clock API. */
#include <platform/unitree/c/roswasm_unitree/topic_rpc.h>
#define CHECK(x)                                                                       \
    do {                                                                               \
        ++checks;                                                                      \
        if (!(x))                                                                      \
            return __LINE__;                                                           \
    } while (0)
static unsigned checks;
unsigned uwvm_unitree_completed_checks(void) {
    return checks;
}
int main(void) {
    uwvm_unitree_rpc_tracker tracker;
    unsigned i;
    uwvm_unitree_rpc_init(&tracker);
    CHECK(uwvm_unitree_rpc_begin(&tracker, 1, 1008, 5, 10) == 0);
    CHECK(uwvm_unitree_rpc_begin(&tracker, 1, 1001, 5, 10) == 17);
    CHECK(uwvm_unitree_rpc_accept(&tracker, 1, 1001, 6) == 9);
    CHECK(uwvm_unitree_rpc_accept(&tracker, 2, 1008, 6) == 9);
    CHECK(uwvm_unitree_rpc_expire(&tracker, 1, 1008, 9) == 11);
    CHECK(uwvm_unitree_rpc_accept(&tracker, 1, 1008, 9) == 0);
    CHECK(uwvm_unitree_rpc_accept(&tracker, 1, 1008, 9) == 9);
    CHECK(uwvm_unitree_rpc_begin(&tracker, 2, 1003, 10, 20) == 0);
    CHECK(uwvm_unitree_rpc_accept(&tracker, 2, 1003, 20) == 110);
    CHECK(uwvm_unitree_rpc_expire(&tracker, 2, 1003, 20) == 110);
    CHECK(uwvm_unitree_rpc_expire(&tracker, 2, 1003, 20) == 9);
    CHECK(uwvm_unitree_rpc_begin(&tracker, 0, 1003, 10, 20) == 22);
    CHECK(uwvm_unitree_rpc_begin(&tracker, 3, 0, 10, 20) == 22);
    CHECK(uwvm_unitree_rpc_begin(&tracker, 3, 1003, 20, 20) == 22);
    for (i = 0; i < 16; ++i)
        if (uwvm_unitree_rpc_begin(&tracker, 100 + i, 1008, 10, 20))
            return __LINE__;
    CHECK(uwvm_unitree_rpc_begin(&tracker, 200, 1008, 10, 20) == 11);
    CHECK(uwvm_unitree_rpc_forget(&tracker, 107, 1008) == 0);
    CHECK(uwvm_unitree_rpc_begin(&tracker, 200, 1008, 10, 20) == 0);
    uwvm_unitree_rpc_close(&tracker);
    CHECK(uwvm_unitree_rpc_accept(&tracker, 200, 1008, 11) == 9);
    CHECK(uwvm_unitree_rpc_begin(&tracker, 300, 1008, 10, 20) == 9);
    return 0;
}
