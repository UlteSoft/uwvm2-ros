/** Authored generator-export adversarial fixture; no ROS/native identity claim. */
#ifndef UWVM_ROS_BOUNDED_RECORDS_H
#define UWVM_ROS_BOUNDED_RECORDS_H
#include <stdint.h>
#include <string.h>
#include <platform/ros/c/roswasm/codec.h>
#if defined(__cplusplus)
#define UWVM_BOUNDED_ASSERT(x) static_assert(x,"canonical representation required")
#else
#define UWVM_BOUNDED_ASSERT(x) _Static_assert(x,"canonical representation required")
#endif
UWVM_BOUNDED_ASSERT(sizeof(uint64_t)==8&&sizeof(float)==4);
typedef struct portable_generated_value {
    uint64_t uint64_t;
    int64_t signed_min;
    uint32_t uint32_t[3];
    double scalar;
    float rounded;
    double negative_zero;
    char literal[64];
} portable_generated_value;
static inline void roswasmc_bounded_portable_generated_default(portable_generated_value* value) {
    static char const text[]="uint64_t static inline _Static_assert(x) UINT64_C(42)";
    size_t i;value->uint64_t=UINT64_C(0xfedcba9876543210);
    value->signed_min=(-INT64_C(9223372036854775807)-INT64_C(1));
    value->uint32_t[0]=1;value->uint32_t[1]=UINT32_C(0xffffffff);value->uint32_t[2]=0;
    value->scalar=-0x1.0p-3;value->rounded=0x1.99999ap-4F;value->negative_zero=-0x0.0p+0;
    for(i=0;i<sizeof text;++i)value->literal[i]=text[i];
}
static inline uint32_t roswasmc_bounded_portable_generated_encode(portable_generated_value const* value,void* bytes,size_t capacity,size_t* length) {
    roswasmc_writer writer=roswasmc_writer_init(bytes,capacity);size_t i;uint32_t single;
    if(!length||!value||capacity<48)return UWVM_ROS_INVALID;
    roswasmc_write_integer(&writer,value->uint64_t,8);roswasmc_write_integer(&writer,(uint64_t)value->signed_min,8);
    for(i=0;i<3;++i)roswasmc_write_integer(&writer,value->uint32_t[i],4);
    roswasmc_write_float64(&writer,value->scalar);memcpy(&single,&value->rounded,4);roswasmc_write_integer(&writer,single,4);
    roswasmc_write_float64(&writer,value->negative_zero);if(!writer.valid)return UWVM_ROS_INVALID;
    *length=writer.size;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
