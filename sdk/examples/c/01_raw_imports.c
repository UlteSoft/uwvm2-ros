/** @file
 * @brief Layer1: see the exact raw import and memory ABI without a resource wrapper.
 * Host setup: explicitly grant alias `pub` for a String topic.publish operation
 * with its exact builtin type identity and byte/rate/QoS limits. No alias exists
 * by default. With no grant this example returns13 and publishes nothing.
 * Build with -I sdk/include and -DROSWASM_EXAMPLE_ROS_VERSION=1 or2.
 */
#include "select_backend.h"
/** @brief Open, publish one canonical String, and close exactly once.
 * ABI offsets/lengths are i64 even on wasm32. Eight-byte output slots avoid the
 * incorrect four-byte size_t/handle assumptions. The String is u32LE length3
 * followed by bytes `raw`; no terminator is sent. Inputs are copied by the host.
 */
int main(void) {
    static char const alias[]="pub";
    unsigned char slot[8]={0};
    unsigned char payload[]={3,0,0,0,'r','a','w'};
    uint64_t handle;uint32_t error,close_error;
    if(ROSWASM_IMPORT(abi_version)()!=1) return UWVM_ROS_UNSUPPORTED;
    error=ROSWASM_IMPORT(capability_open)(0,uwvm_ros_address(alias),3,uwvm_ros_address(slot));
    if(error) return (int)error; /* A failed open leaves slot unusable. */
    handle=uwvm_ros_load_u64(slot);
    if(!handle) return UWVM_ROS_INVALID;
    error=ROSWASM_IMPORT(topic_publish)(handle,0,uwvm_ros_address(payload),sizeof(payload));
    close_error=ROSWASM_IMPORT(capability_close)(handle);
    return (int)(error?error:close_error);
}
