/** @file
 * @brief Prepared regression for legacy C convenience output-length preservation.
 * These two imports are pure test models, not native ROS or runtime execution.
 */
#include <assert.h>
#include "../../ros1/abi.h"
#include "../../ros2/abi.h"
/** @brief Model error90 before any output slot write. */
uint32_t uwvm_ros1_event_poll(uint64_t h,uint32_t m,uint64_t o,uint64_t c,uint64_t l) {
    (void)h;(void)m;(void)o;(void)c;(void)l;return UWVM_ROS_TOO_LARGE;
}
/** @brief Model error90 before any output slot write for the other backend. */
uint32_t uwvm_ros2_event_poll(uint64_t h,uint32_t m,uint64_t o,uint64_t c,uint64_t l) {
    (void)h;(void)m;(void)o;(void)c;(void)l;return UWVM_ROS_TOO_LARGE;
}
/** @brief Neither backend may replace caller length with uninitialized/default slot bytes. */
int main(void) {
    unsigned char bytes[8];uint64_t length=123;
    assert(uwvm_ros1_poll(1,bytes,sizeof bytes,&length)==90&&length==123);
    assert(uwvm_ros2_poll(1,bytes,sizeof bytes,&length)==90&&length==123);
    return 0;
}
