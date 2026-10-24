/** Compile against original C11 and the C89 export of the exact same fixture. */
#include "portable_generated_input.h"
int main(void) {
    portable_generated_value value;unsigned char bytes[64];size_t length=0,i;
    static char const expected[]="uint64_t static inline _Static_assert(x) UINT64_C(42)";
    roswasmc_bounded_portable_generated_default(&value);
    for(i=0;i<sizeof expected;++i)if(value.literal[i]!=expected[i])return 1;
    if(value.uint64_t!=UWVM_ROS_U64_C(0xfedcba9876543210)||value.uint32_t[1]!=UWVM_ROS_U32_MAX)return 2;
    if(roswasmc_bounded_portable_generated_encode(&value,bytes,sizeof bytes,&length)||length!=48)return 3;
    if(uwvm_ros_load_u64(bytes)!=UWVM_ROS_U64_C(0xfedcba9876543210)||uwvm_ros_load_u64(bytes+8)!=UWVM_ROS_U64_C(0x8000000000000000))return 4;
    if(bytes[16]!=1||bytes[20]!=255||bytes[23]!=255||bytes[35]!=0xbf)return 5;
    if(bytes[36]!=0xcd||bytes[37]!=0xcc||bytes[38]!=0xcc||bytes[39]!=0x3d||uwvm_ros_load_u64(bytes+40)!=UWVM_ROS_U64_C(0x8000000000000000))return 6;
    return 0;
}
