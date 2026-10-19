/** @file
 * @brief Layer1: raw imports in C++, with explicit i64 offsets and 8LE handle slots.
 * Host setup must grant `pub` as a String topic.publish alias. The guest supplies
 * no native topic name or node identity. Build this example for exactly one
 * backend with ROSWASM_EXAMPLE_ROS_VERSION=1 or2.
 */
#include "select_backend.h"
/** @brief Publish a minimal canonical String and close the returned handle once. */
int main() {
    if(ROSWASM_IMPORT(abi_version)()!=1) return UWVM_ROS_UNSUPPORTED;
    char const alias[]{"pub"};unsigned char slot[8]{};
    unsigned char bytes[]{3,0,0,0,'c','p','p'};
    auto error=ROSWASM_IMPORT(capability_open)(0,uwvm_ros_address(alias),3,uwvm_ros_address(slot));
    if(error) return static_cast<int>(error);
    auto handle=uwvm_ros_load_u64(slot);
    if(!handle) return UWVM_ROS_INVALID;
    error=ROSWASM_IMPORT(topic_publish)(handle,0,uwvm_ros_address(bytes),sizeof bytes);
    auto close_error=ROSWASM_IMPORT(capability_close)(handle);
    return static_cast<int>(error?error:close_error);
}
