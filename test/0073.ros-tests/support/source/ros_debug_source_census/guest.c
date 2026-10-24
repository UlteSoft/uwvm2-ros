// Real C11 Guest source for an actual WASI/DWARF producer, not run here.
// ABI version queries issue no native ROS business effect or resource grant.
#include <stdint.h>
#include <roswasm/ros2.h>
struct reading {uint32_t version;uint64_t iterations;uint8_t bytes[8];};
volatile struct reading observed={0,0,{0,1,2,3,4,5,6,7}};
int main(void)
{
    observed.version=uwvm_ros2_abi_version();
    // The genuine VM debugger instruments/pauses this original loop. The Host
    // must stop and drain the actual Guest before any view is read. Volatile
    // is ordinary Guest semantics, not a cross-native-thread synchronization.
    while(observed.version==1u){observed.iterations+=1u;observed.bytes[3]=(uint8_t)observed.iterations;}
    return 0;
}
