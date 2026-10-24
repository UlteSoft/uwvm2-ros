/** @file C11 local correlation lifetime scenarios, with no ROS or transport.
 * This tests the public helper against late replies, timeout, ambiguous delivery,
 * finite concurrency, long reuse of storage, and signed ID exhaustion. It does
 * not simulate a successful robot command or establish a native wire identity.
 */
#include <platform/unitree/c/roswasm_unitree/topic_rpc.h>
#include <string.h>

#define CHECK(condition) do { if(!(condition))return __LINE__; } while(0)

int main(void) {
    uwvm_unitree_rpc_tracker tracker, before;
    int64_t allocated=-7;
    size_t i;
    /* Initialize padding too, so byte comparisons below observe real mutation
     * without depending on indeterminate structure padding. */
    memset(&tracker,0,sizeof tracker);
    uwvm_unitree_rpc_init(&tracker);

    /* A completed request can never lend its identity to another operation. */
    CHECK(uwvm_unitree_rpc_begin(&tracker,10,1008,1,20)==0);
    CHECK(uwvm_unitree_rpc_accept(&tracker,10,1008,2)==0);
    memcpy(&before,&tracker,sizeof tracker);
    CHECK(uwvm_unitree_rpc_begin(&tracker,10,1008,3,20)==17);
    CHECK(uwvm_unitree_rpc_begin(&tracker,10,1003,3,20)==17);
    CHECK(uwvm_unitree_rpc_begin(&tracker,9,1008,3,20)==17);
    CHECK(memcmp(&before,&tracker,sizeof tracker)==0);
    CHECK(uwvm_unitree_rpc_begin(&tracker,11,1008,3,20)==0);
    CHECK(uwvm_unitree_rpc_accept(&tracker,10,1008,4)==9);
    CHECK(uwvm_unitree_rpc_accept(&tracker,11,1003,4)==9);
    CHECK(uwvm_unitree_rpc_find(&tracker,11,1008)!=NULL);
    CHECK(uwvm_unitree_rpc_accept(&tracker,11,1008,4)==0);

    /* Delivery may already have occurred when publication reports an error.
     * Forgetting only abandons local ownership; it must still burn that ID. */
    CHECK(uwvm_unitree_rpc_begin(&tracker,12,1008,4,20)==0);
    CHECK(uwvm_unitree_rpc_forget(&tracker,12,1008)==0);
    CHECK(uwvm_unitree_rpc_begin(&tracker,12,1008,5,20)==17);
    CHECK(uwvm_unitree_rpc_begin_next(&tracker,1008,5,20,&allocated)==0);
    CHECK(allocated==13);
    CHECK(uwvm_unitree_rpc_accept(&tracker,12,1008,6)==9);
    CHECK(uwvm_unitree_rpc_accept(&tracker,13,1008,6)==0);

    /* Exactly-at-deadline is expired, not an acknowledgement. An expired
     * response does not silently release ownership; explicit expiry does. */
    CHECK(uwvm_unitree_rpc_begin(&tracker,14,1008,6,10)==0);
    CHECK(uwvm_unitree_rpc_expire(&tracker,14,1008,9)==11);
    CHECK(uwvm_unitree_rpc_accept(&tracker,14,1008,10)==110);
    CHECK(uwvm_unitree_rpc_find(&tracker,14,1008)!=NULL);
    CHECK(uwvm_unitree_rpc_expire(&tracker,14,1008,10)==110);
    CHECK(uwvm_unitree_rpc_begin(&tracker,14,1008,10,20)==17);
    CHECK(uwvm_unitree_rpc_begin(&tracker,15,1008,10,20)==0);
    CHECK(uwvm_unitree_rpc_accept(&tracker,14,1008,11)==9);
    CHECK(uwvm_unitree_rpc_accept(&tracker,15,1008,11)==0);

    /* Capacity is concurrent, not a limit on total requests. Refusal neither
     * consumes the proposed ID nor changes the caller's output. */
    for(i=0;i<UWVM_UNITREE_RPC_CAPACITY;++i)
        CHECK(uwvm_unitree_rpc_begin(&tracker,100+(int64_t)i,1008,10,30)==0);
    memcpy(&before,&tracker,sizeof tracker);
    allocated=-7;
    CHECK(uwvm_unitree_rpc_begin(&tracker,200,1008,10,30)==11);
    CHECK(uwvm_unitree_rpc_begin_next(&tracker,1008,10,30,&allocated)==11);
    CHECK(allocated==-7);
    CHECK(memcmp(&before,&tracker,sizeof tracker)==0);
    CHECK(uwvm_unitree_rpc_forget(&tracker,100,1008)==0);
    CHECK(uwvm_unitree_rpc_begin(&tracker,200,1008,10,30)==0);
    CHECK(uwvm_unitree_rpc_accept(&tracker,100,1008,11)==9);
    CHECK(uwvm_unitree_rpc_find(&tracker,200,1008)!=NULL);
    for(i=1;i<UWVM_UNITREE_RPC_CAPACITY;++i)
        CHECK(uwvm_unitree_rpc_forget(&tracker,100+(int64_t)i,1008)==0);
    CHECK(uwvm_unitree_rpc_forget(&tracker,200,1008)==0);
    for(i=0;i<4096;++i) {
        CHECK(uwvm_unitree_rpc_begin_next(&tracker,1008,10,30,&allocated)==0);
        CHECK(uwvm_unitree_rpc_accept(&tracker,allocated,1008,11)==0);
    }
    CHECK(allocated==4296);

    /* Invalid calls are transactional, including the output parameter. */
    memcpy(&before,&tracker,sizeof tracker);
    allocated=-7;
    CHECK(uwvm_unitree_rpc_begin_next(&tracker,0,10,30,&allocated)==22);
    CHECK(uwvm_unitree_rpc_begin_next(&tracker,1008,30,30,&allocated)==22);
    CHECK(uwvm_unitree_rpc_begin_next(&tracker,1008,10,30,NULL)==22);
    CHECK(uwvm_unitree_rpc_begin_next(NULL,1008,10,30,&allocated)==22);
    CHECK(uwvm_unitree_rpc_begin(&tracker,0,1008,10,30)==22);
    CHECK(allocated==-7);
    CHECK(memcmp(&before,&tracker,sizeof tracker)==0);

    /* Last signed64 identity is valid exactly once; automatic allocation
     * refuses exhaustion rather than overflowing or wrapping to an old ID. */
    CHECK(uwvm_unitree_rpc_begin(&tracker,INT64_MAX,1008,10,30)==0);
    memcpy(&before,&tracker,sizeof tracker);
    CHECK(uwvm_unitree_rpc_begin_next(&tracker,1008,10,30,&allocated)==75);
    CHECK(allocated==-7);
    CHECK(memcmp(&before,&tracker,sizeof tracker)==0);
    CHECK(uwvm_unitree_rpc_accept(&tracker,INT64_MAX,1008,11)==0);
    CHECK(uwvm_unitree_rpc_begin(&tracker,INT64_MAX,1008,11,30)==17);
    CHECK(uwvm_unitree_rpc_begin_next(&tracker,1008,11,30,&allocated)==75);
    uwvm_unitree_rpc_close(&tracker);
    memcpy(&before,&tracker,sizeof tracker);
    CHECK(uwvm_unitree_rpc_begin_next(&tracker,1008,11,30,&allocated)==9);
    CHECK(uwvm_unitree_rpc_begin(&tracker,INT64_MAX,1008,11,30)==9);
    CHECK(uwvm_unitree_rpc_accept(&tracker,INT64_MAX,1008,11)==9);
    CHECK(uwvm_unitree_rpc_forget(&tracker,INT64_MAX,1008)==9);
    CHECK(memcmp(&before,&tracker,sizeof tracker)==0);
    uwvm_unitree_rpc_close(NULL);
    uwvm_unitree_rpc_init(NULL);
    return 0;
}
