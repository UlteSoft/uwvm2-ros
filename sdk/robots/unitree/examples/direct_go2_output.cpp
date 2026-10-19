/** @file fast_io formatter for the real C Go2 executable, in the same Wasm.
 * A non-null36-word array emits the exact STEP/STATE observer protocol; null
 * emits the final PASS count. There are no ROS headers, handles, imports or
 * callbacks here. The C11 main remains the sole C API capability owner.
 */
#include "basic_io.h"
#include <fast_io.h>
extern "C" uint32_t uwvm_unitree_direct_record(uint32_t step,uint32_t tick,
    uint32_t command_crc,uint32_t state_crc,uint32_t const* words) {
    // Both wire records contain only uint32 words. The final PASS branch
    // is shorter. Keep the existing output bytes and WASIp1 short-write policy,
    // but format directly into one stack owner instead of a heap string.
    char text[2048];
    static_assert((sizeof("UNITREE_STEP ")-1u)+4u*10u+3u+1u+
        (sizeof("UNITREE_STATE ")-1u)+2u*10u+1u+36u*11u+1u<=sizeof text);
    static_assert((sizeof("PASS_UNITREE_DIRECT steps=")-1u)+10u+1u<=sizeof text);
    fast_io::obuffer_view sink{text,text+sizeof text};
    if(words){
        fast_io::print(sink,"UNITREE_STEP ",step," ",tick," ",command_crc," ",state_crc,"\nUNITREE_STATE ",step," ",tick);
        for(unsigned i=0;i<36;++i)fast_io::print(sink," ",words[i]);
        fast_io::print(sink,"\n");
    }else fast_io::print(sink,"PASS_UNITREE_DIRECT steps=",step,"\n");
    return uwvm_unitree_example_output(sink.data(),sink.size());
}
