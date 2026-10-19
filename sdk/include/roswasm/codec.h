/** @file
 * @brief Allocation-free canonical ABI codecs for C11 and freestanding guests.
 *
 * These encode ROS ABI values, not native ROS serialization or C struct layout.
 * All fixed-width integers use little endian; strings use u32 byte length plus
 * bytes without a terminator. Callers own every buffer and choose its maximum.
 * Encoding never obtains authority. The runtime independently validates types,
 * UTF-8, finite numbers, quotas, frames, bounds, ownership and operation rights.
 */
#ifndef ROSWASMC_CODEC_H
#define ROSWASMC_CODEC_H
#include "roswasmc.h"
/** @brief Copy object representation bytes without requiring a hosted C library.
 * Only small fixed-width bit representations use this helper; it performs no I/O.
 */
static inline void roswasmc_copy_object(void* output,void const* input,size_t size) {
    size_t i;for(i=0;i<size;++i) ((unsigned char*)output)[i]=((unsigned char const*)input)[i];
}
/** @brief Borrowed canonical byte range; its backing storage must outlive use. */
typedef struct roswasmc_bytes { unsigned char const* data;size_t size; } roswasmc_bytes;
/** @brief Borrowed UTF-8 range. It is not NUL terminated and may be empty. */
typedef struct roswasmc_string { char const* data;size_t size; } roswasmc_string;
/** @brief Twist fields: linear XYZ followed by angular XYZ. */
typedef struct roswasmc_twist { double linear[3];double angular[3]; } roswasmc_twist;
/** @brief Bounded wire writer. Once failed, later writes remain failed. */
typedef struct roswasmc_writer { unsigned char* data;size_t capacity;size_t size;int valid; } roswasmc_writer;
/** @brief Bounded wire reader. Failure never advances past the supplied range. */
typedef struct roswasmc_reader { unsigned char const* data;size_t size;size_t position;int valid; } roswasmc_reader;
/** @brief Initialize caller-owned output storage; a NULL nonempty buffer is invalid. */
static inline roswasmc_writer roswasmc_writer_init(void* bytes,size_t capacity) {
    roswasmc_writer out={(unsigned char*)bytes,capacity,0,bytes!=0||capacity==0};return out;
}
/** @brief Initialize a reader over the exact import-returned bytes. */
static inline roswasmc_reader roswasmc_reader_init(void const* bytes,size_t length) {
    roswasmc_reader in={(unsigned char const*)bytes,length,0,bytes!=0||length==0};return in;
}
/** @brief Append raw bytes; buffer overflow changes valid to zero. */
static inline int roswasmc_write_bytes(roswasmc_writer* out,void const* bytes,size_t length) {
    size_t i;if(!out->valid||length>out->capacity-out->size||(!bytes&&length)) {out->valid=0;return 0;}
    for(i=0;i<length;++i) out->data[out->size+i]=((unsigned char const*)bytes)[i];out->size+=length;return 1;
}
/** @brief Append an unsigned LE integer of width 1..8, rejecting truncation. */
static inline int roswasmc_write_integer(roswasmc_writer* out,uint64_t value,unsigned width) {
    unsigned char bytes[8];unsigned i;
    if(!width||width>8||(width<8&&(value>>(width*8)))) {out->valid=0;return 0;}
    for(i=0;i<width;++i) bytes[i]=(unsigned char)(value>>(i*8));return roswasmc_write_bytes(out,bytes,width);
}
/** @brief Append one canonical u32-length UTF-8 string; host validates UTF-8 separately. */
static inline int roswasmc_write_string(roswasmc_writer* out,roswasmc_string text) {
    if(text.size>UINT32_MAX||out->capacity-out->size<4||text.size>out->capacity-out->size-4||(!text.data&&text.size)) {out->valid=0;return 0;}
    return roswasmc_write_integer(out,(uint32_t)text.size,4)&&roswasmc_write_bytes(out,text.data,text.size);
}
/** @brief Append one IEEE754 binary64 value using portable byte copies.
 * The WebAssembly target must use eight-byte IEEE754 doubles. Finite values are
 * required by the host; this codec rejects exponent-all-ones before writing.
 */
static inline int roswasmc_write_float64(roswasmc_writer* out,double value) {
    uint64_t bits=0;if(sizeof(double)!=8) {out->valid=0;return 0;}roswasmc_copy_object(&bits,&value,8);
    if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000)) {out->valid=0;return 0;}
    return roswasmc_write_integer(out,bits,8);
}
/** @brief Read one LE integer; preserve the destination when truncated. */
static inline int roswasmc_read_integer(roswasmc_reader* in,uint64_t* value,unsigned width) {
    uint64_t next=0;unsigned i;if(!in->valid||!width||width>8||width>in->size-in->position||!value) {in->valid=0;return 0;}
    for(i=0;i<width;++i) next|=(uint64_t)in->data[in->position+i]<<(i*8);in->position+=width;*value=next;return 1;
}
/** @brief Read one borrowed length-prefixed string; no allocation or terminator is added. */
static inline int roswasmc_read_string(roswasmc_reader* in,roswasmc_string* text) {
    uint64_t length;if(!text||!roswasmc_read_integer(in,&length,4)||length>in->size-in->position) {in->valid=0;return 0;}
    text->data=(char const*)(in->data+in->position);text->size=(size_t)length;in->position+=(size_t)length;return 1;
}
/** @brief Read a finite IEEE754 binary64 field, preserving destination on failure. */
static inline int roswasmc_read_float64(roswasmc_reader* in,double* value) {
    uint64_t bits;if(!value||sizeof(double)!=8||!roswasmc_read_integer(in,&bits,8)||(bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000)) {in->valid=0;return 0;}
    roswasmc_copy_object(value,&bits,8);return 1;
}
/** @brief Require exact consumption: trailing bytes are invalid canonical values. */
static inline int roswasmc_reader_done(roswasmc_reader const* in) {return in->valid&&in->position==in->size;}
/** @brief Validate well-formed UTF-8, optionally allowing scalar-string NUL.
 * This performs bounded borrowed reads and rejects overlong/surrogate sequences.
 */
static inline int roswasmc_utf8(roswasmc_string value,int allow_nul) {
    size_t pos=0;if(!value.data&&value.size) return 0;
    while(pos<value.size) {
        unsigned lead=(unsigned char)value.data[pos++],n=0;uint32_t cp=0,min=0;
        if(lead<128) {if(!lead&&!allow_nul) return 0;continue;}
        if(lead>=0xc2&&lead<=0xdf) {n=1;cp=lead&31;min=128;}
        else if(lead>=0xe0&&lead<=0xef) {n=2;cp=lead&15;min=2048;}
        else if(lead>=0xf0&&lead<=0xf4) {n=3;cp=lead&7;min=65536;}
        else return 0;
        if(n>value.size-pos) return 0;
        while(n--) {unsigned b=(unsigned char)value.data[pos++];if((b&0xc0)!=0x80) return 0;cp=(cp<<6)|(b&63);}
        if(cp<min||cp>0x10ffff||(cp>=0xd800&&cp<=0xdfff)) return 0;
    }
    return 1;
}
/** @brief Encode std_msgs String; on failure length is preserved but scratch bytes may change. */
static inline uint32_t roswasmc_encode_string(roswasmc_string value,void* output,size_t capacity,size_t* length) {
    roswasmc_writer out=roswasmc_writer_init(output,capacity);if(!length||!roswasmc_utf8(value,1)||!roswasmc_write_string(&out,value)) return UWVM_ROS_INVALID;*length=out.size;return 0;
}
/** @brief Decode std_msgs String and preserve the destination on failure. */
static inline uint32_t roswasmc_decode_string(void const* bytes,size_t length,roswasmc_string* value) {
    roswasmc_reader in=roswasmc_reader_init(bytes,length);roswasmc_string next;
    if(!value||!roswasmc_read_string(&in,&next)||!roswasmc_utf8(next,1)||!roswasmc_reader_done(&in)) return UWVM_ROS_INVALID;*value=next;return 0;
}
/** @brief Encode Bool as exactly one byte (0 or 1), rejecting other C integer values. */
static inline uint32_t roswasmc_encode_bool(int value,void* output,size_t capacity,size_t* length) {
    if((value!=0&&value!=1)||!output||!capacity||!length) return UWVM_ROS_INVALID;((unsigned char*)output)[0]=(unsigned char)value;*length=1;return 0;
}
/** @brief Decode canonical Bool; destination remains unchanged on failure. */
static inline uint32_t roswasmc_decode_bool(void const* bytes,size_t length,int* value) {
    if(!bytes||length!=1||!value||((unsigned char const*)bytes)[0]>1) return UWVM_ROS_INVALID;*value=((unsigned char const*)bytes)[0]!=0;return 0;
}
/** @brief Encode six finite Twist doubles; capacity must cover exactly 48 bytes. */
static inline uint32_t roswasmc_encode_twist(roswasmc_twist const* value,void* output,size_t capacity,size_t* length) {
    roswasmc_writer out=roswasmc_writer_init(output,capacity);unsigned i;if(!value||!length) return UWVM_ROS_INVALID;
    for(i=0;i<3;++i) if(!roswasmc_write_float64(&out,value->linear[i])) return UWVM_ROS_INVALID;
    for(i=0;i<3;++i) if(!roswasmc_write_float64(&out,value->angular[i])) return UWVM_ROS_INVALID;
    *length=out.size;return 0;
}
/** @brief Decode a Twist transactionally; exact length and all finite fields are required. */
static inline uint32_t roswasmc_decode_twist(void const* bytes,size_t length,roswasmc_twist* value) {
    roswasmc_reader in=roswasmc_reader_init(bytes,length);roswasmc_twist next;unsigned i;if(!value) return UWVM_ROS_INVALID;
    for(i=0;i<3;++i) if(!roswasmc_read_float64(&in,&next.linear[i])) return UWVM_ROS_INVALID;
    for(i=0;i<3;++i) if(!roswasmc_read_float64(&in,&next.angular[i])) return UWVM_ROS_INVALID;
    if(!roswasmc_reader_done(&in)) return UWVM_ROS_INVALID;*value=next;return 0;
}
/** @brief Publish a typed String using caller-owned encoding scratch storage. */
static inline uint32_t roswasmc_publish_string(roswasmc_capability const* cap,roswasmc_string value,void* scratch,size_t capacity) {
    size_t length;uint32_t error=roswasmc_encode_string(value,scratch,capacity,&length);return error?error:roswasmc_publish(cap,scratch,length);
}
/** @brief Publish a typed Bool; the one-byte scratch value is copied during the import. */
static inline uint32_t roswasmc_publish_bool(roswasmc_capability const* cap,int value) {
    unsigned char bytes[1];size_t length;uint32_t error=roswasmc_encode_bool(value,bytes,1,&length);return error?error:roswasmc_publish(cap,bytes,length);
}
/** @brief Publish typed Twist using a fixed 48-byte local scratch array. */
static inline uint32_t roswasmc_publish_twist(roswasmc_capability const* cap,roswasmc_twist const* value) {
    unsigned char bytes[48];size_t length;uint32_t error=roswasmc_encode_twist(value,bytes,48,&length);return error?error:roswasmc_publish(cap,bytes,length);
}
/** @brief Supported Trigger/SetBool response with borrowed UTF-8 message. */
typedef struct roswasmc_service_response {int success;roswasmc_string message;} roswasmc_service_response;
/** @brief Decode the common Trigger/SetBool response (Bool followed by String). */
static inline uint32_t roswasmc_decode_service_response(void const* bytes,size_t length,roswasmc_service_response* response) {
    roswasmc_reader in=roswasmc_reader_init(bytes,length);roswasmc_service_response next;uint64_t success;
    if(!response||!roswasmc_read_integer(&in,&success,1)||success>1||!roswasmc_read_string(&in,&next.message)||!roswasmc_utf8(next.message,1)||!roswasmc_reader_done(&in)) return UWVM_ROS_INVALID;
    next.success=(int)success;*response=next;return 0;
}
/** @brief Reply to one owned service request using its eight-byte ID prefix.
 * The separate response alias must match the advertise resource and type.
 */
static inline uint32_t roswasmc_service_reply(roswasmc_capability const* respond,uint64_t request_id,
    roswasmc_service_response response,void* scratch,size_t capacity) {
    roswasmc_writer out=roswasmc_writer_init(scratch,capacity);roswasmc_result result;
    if(!request_id||(response.success!=0&&response.success!=1)||!roswasmc_utf8(response.message,1)||!roswasmc_write_integer(&out,request_id,8)||
       !roswasmc_write_integer(&out,(uint64_t)response.success,1)||!roswasmc_write_string(&out,response.message)) return UWVM_ROS_INVALID;
    result=roswasmc_service_respond(respond,out.data,out.size,0,0);return result.error;
}
/** @brief Send a Fibonacci goal; the goal output is changed only after a valid successful reply. */
static inline uint32_t roswasmc_fibonacci_send_goal(roswasmc_capability const* send,int32_t order,uint64_t* goal) {
    unsigned char request[4],response[8];roswasmc_result result;uwvm_ros_store_u32(request,(uint32_t)order);
    if(!goal) return UWVM_ROS_INVALID;result=roswasmc_action_send_goal(send,request,4,response,8);
    if(result.error) return result.error;if(!result.has_length||result.length!=8||!uwvm_ros_load_u64(response)) return UWVM_ROS_INVALID;
    *goal=uwvm_ros_load_u64(response);return 0;
}
/** @brief Read a granted ROS/system/wall/steady clock; no WASI clock grant is inferred. */
static inline uint32_t roswasmc_clock_read(roswasmc_capability const* cap,roswasmc_operation clock,uint64_t* nanoseconds) {
    unsigned char response[8];roswasmc_result result;if(!nanoseconds) return UWVM_ROS_INVALID;
    if(clock!=ROSWASMC_TIME_READ_ROS&&clock!=ROSWASMC_TIME_READ_WALL&&clock!=ROSWASMC_TIME_READ_SYSTEM&&clock!=ROSWASMC_TIME_READ_STEADY) return UWVM_ROS_INVALID;
    result=roswasmc_request(cap,clock,0,0,response,8);if(result.error) return result.error;
    if(!result.has_length||result.length!=8) return UWVM_ROS_INVALID;*nanoseconds=uwvm_ros_load_u64(response);return 0;
}
/** @brief Complete borrowed canonical Image fields. ROS2 requires sequence zero.
 * encoding/frame_id/data remain borrowed from caller or the decode buffer.
 */
typedef struct roswasmc_image {
    uint32_t sequence,seconds,nanoseconds,height,width,step;
    roswasmc_string frame_id,encoding;int bigendian;roswasmc_bytes data;
} roswasmc_image;
/** @brief Complete borrowed TransformStamped fields; host policy owns allowed frame pairs. */
typedef struct roswasmc_transform {
    uint32_t sequence,seconds,nanoseconds;roswasmc_string parent_frame,child_frame;
    double translation[3],quaternion[4];
} roswasmc_transform;
/** @brief Encode Image fields in canonical order. Host still checks geometry/stride/encoding quotas. */
static inline uint32_t roswasmc_encode_image(roswasmc_image const* value,void* bytes,size_t capacity,size_t* length) {
    roswasmc_writer out=roswasmc_writer_init(bytes,capacity);
    if(!value||!length||(value->bigendian!=0&&value->bigendian!=1)||value->data.size>UINT32_MAX) return UWVM_ROS_INVALID;
    if(!roswasmc_write_integer(&out,value->sequence,4)||!roswasmc_write_integer(&out,value->seconds,4)||!roswasmc_write_integer(&out,value->nanoseconds,4)||
       !roswasmc_write_string(&out,value->frame_id)||!roswasmc_write_integer(&out,value->height,4)||!roswasmc_write_integer(&out,value->width,4)||
       !roswasmc_write_string(&out,value->encoding)||!roswasmc_write_integer(&out,(uint64_t)value->bigendian,1)||!roswasmc_write_integer(&out,value->step,4)||
       !roswasmc_write_integer(&out,value->data.size,4)||!roswasmc_write_bytes(&out,value->data.data,value->data.size)) return UWVM_ROS_INVALID;
    *length=out.size;return 0;
}
/** @brief Decode Image transactionally into views borrowed from the exact input buffer. */
static inline uint32_t roswasmc_decode_image(void const* bytes,size_t length,roswasmc_image* value) {
    roswasmc_reader in=roswasmc_reader_init(bytes,length);roswasmc_image next;uint64_t n;
    if(!value||!roswasmc_read_integer(&in,&n,4)) return UWVM_ROS_INVALID;next.sequence=(uint32_t)n;
    if(!roswasmc_read_integer(&in,&n,4)) return UWVM_ROS_INVALID;next.seconds=(uint32_t)n;
    if(!roswasmc_read_integer(&in,&n,4)) return UWVM_ROS_INVALID;next.nanoseconds=(uint32_t)n;
    if(!roswasmc_read_string(&in,&next.frame_id)||!roswasmc_read_integer(&in,&n,4)) return UWVM_ROS_INVALID;next.height=(uint32_t)n;
    if(!roswasmc_read_integer(&in,&n,4)) return UWVM_ROS_INVALID;next.width=(uint32_t)n;
    if(!roswasmc_read_string(&in,&next.encoding)||!roswasmc_read_integer(&in,&n,1)||n>1) return UWVM_ROS_INVALID;next.bigendian=(int)n;
    if(!roswasmc_read_integer(&in,&n,4)) return UWVM_ROS_INVALID;next.step=(uint32_t)n;
    if(!roswasmc_read_integer(&in,&n,4)||n>in.size-in.position) return UWVM_ROS_INVALID;
    next.data.data=in.data+in.position;next.data.size=(size_t)n;in.position+=(size_t)n;
    if(!roswasmc_reader_done(&in)) return UWVM_ROS_INVALID;*value=next;return 0;
}
/** @brief Encode all Transform fields; rejects nonfinite doubles before import. */
static inline uint32_t roswasmc_encode_transform(roswasmc_transform const* value,void* bytes,size_t capacity,size_t* length) {
    roswasmc_writer out=roswasmc_writer_init(bytes,capacity);unsigned i;if(!value||!length) return UWVM_ROS_INVALID;
    if(!roswasmc_write_integer(&out,value->sequence,4)||!roswasmc_write_integer(&out,value->seconds,4)||!roswasmc_write_integer(&out,value->nanoseconds,4)||
       !roswasmc_write_string(&out,value->parent_frame)||!roswasmc_write_string(&out,value->child_frame)) return UWVM_ROS_INVALID;
    for(i=0;i<3;++i) if(!roswasmc_write_float64(&out,value->translation[i])) return UWVM_ROS_INVALID;
    for(i=0;i<4;++i) if(!roswasmc_write_float64(&out,value->quaternion[i])) return UWVM_ROS_INVALID;
    *length=out.size;return 0;
}
/** @brief Decode Transform transactionally; destination strings borrow input storage. */
static inline uint32_t roswasmc_decode_transform(void const* bytes,size_t length,roswasmc_transform* value) {
    roswasmc_reader in=roswasmc_reader_init(bytes,length);roswasmc_transform next;uint64_t n;unsigned i;
    if(!value||!roswasmc_read_integer(&in,&n,4)) return UWVM_ROS_INVALID;next.sequence=(uint32_t)n;
    if(!roswasmc_read_integer(&in,&n,4)) return UWVM_ROS_INVALID;next.seconds=(uint32_t)n;
    if(!roswasmc_read_integer(&in,&n,4)) return UWVM_ROS_INVALID;next.nanoseconds=(uint32_t)n;
    if(!roswasmc_read_string(&in,&next.parent_frame)||!roswasmc_read_string(&in,&next.child_frame)) return UWVM_ROS_INVALID;
    for(i=0;i<3;++i) if(!roswasmc_read_float64(&in,&next.translation[i])) return UWVM_ROS_INVALID;
    for(i=0;i<4;++i) if(!roswasmc_read_float64(&in,&next.quaternion[i])) return UWVM_ROS_INVALID;
    if(!roswasmc_reader_done(&in)) return UWVM_ROS_INVALID;*value=next;return 0;
}
/** @brief Parameter tag values match typed-event v2 tags; int32=10 is scalar ROS1-only. */
typedef enum roswasmc_parameter_tag {
    ROSWASMC_PARAM_DELETION=0,ROSWASMC_PARAM_BOOL=1,ROSWASMC_PARAM_INT64=2,ROSWASMC_PARAM_FLOAT64=3,
    ROSWASMC_PARAM_STRING=4,ROSWASMC_PARAM_BYTES=5,ROSWASMC_PARAM_BOOLS=6,ROSWASMC_PARAM_INT64S=7,
    ROSWASMC_PARAM_FLOAT64S=8,ROSWASMC_PARAM_STRINGS=9,ROSWASMC_PARAM_INT32=10,ROSWASMC_PARAM_INT32S=11
} roswasmc_parameter_tag;
/** @brief Borrowed typed parameter value. integer_bits preserves signed two's-complement bits.
 * Arrays use complete canonical count-prefixed bytes in array_wire. Construct
 * them using write_integer/write_string/write_float64, then decode to validate.
 * They remain borrowed; this keeps C freestanding code allocation-free.
 */
typedef struct roswasmc_parameter_value {
    roswasmc_parameter_tag tag;int boolean;uint64_t integer_bits;double real;
    roswasmc_string text;roswasmc_bytes array_wire;uint32_t element_count;
} roswasmc_parameter_value;
/** @brief Decode all supported scalar/array values, with exact size, finite and UTF-8 checks.
 * Destination is preserved on failure. Tag is supplied from the granted type,
 * never inferred from arbitrary bytes. Array elements remain canonical LE bytes.
 */
static inline uint32_t roswasmc_decode_parameter(roswasmc_parameter_tag tag,void const* bytes,size_t length,roswasmc_parameter_value* value) {
    roswasmc_reader in=roswasmc_reader_init(bytes,length);roswasmc_parameter_value next={ROSWASMC_PARAM_DELETION};uint64_t n,i;unsigned width=0;
    if(!value||length>4193786u) return UWVM_ROS_INVALID;next.tag=tag;
    if(tag==ROSWASMC_PARAM_BOOL) {if(!roswasmc_read_integer(&in,&n,1)||n>1) return UWVM_ROS_INVALID;next.boolean=(int)n;}
    else if(tag==ROSWASMC_PARAM_INT64||tag==ROSWASMC_PARAM_INT32) {if(!roswasmc_read_integer(&in,&next.integer_bits,tag==ROSWASMC_PARAM_INT32?4:8)) return UWVM_ROS_INVALID;}
    else if(tag==ROSWASMC_PARAM_FLOAT64) {if(!roswasmc_read_float64(&in,&next.real)) return UWVM_ROS_INVALID;}
    else if(tag==ROSWASMC_PARAM_STRING) {if(!roswasmc_read_string(&in,&next.text)||!roswasmc_utf8(next.text,1)) return UWVM_ROS_INVALID;}
    else if((tag>=ROSWASMC_PARAM_BYTES&&tag<=ROSWASMC_PARAM_STRINGS)||tag==ROSWASMC_PARAM_INT32S) {
        if(!roswasmc_read_integer(&in,&n,4)||n>1048576u) return UWVM_ROS_INVALID;next.element_count=(uint32_t)n;
        width=(tag==ROSWASMC_PARAM_BYTES||tag==ROSWASMC_PARAM_BOOLS)?1:(tag==ROSWASMC_PARAM_INT32S?4:8);
        if(tag!=ROSWASMC_PARAM_STRINGS&&(n>(in.size-in.position)/width||n*width!=in.size-in.position)) return UWVM_ROS_INVALID;
        for(i=0;i<n;++i) {
            uint64_t bits;double real;roswasmc_string text;
            if(tag==ROSWASMC_PARAM_STRINGS) {if(!roswasmc_read_string(&in,&text)||!roswasmc_utf8(text,0)) return UWVM_ROS_INVALID;}
            else if(tag==ROSWASMC_PARAM_FLOAT64S) {if(!roswasmc_read_float64(&in,&real)) return UWVM_ROS_INVALID;}
            else {if(!roswasmc_read_integer(&in,&bits,width)||(tag==ROSWASMC_PARAM_BOOLS&&bits>1)) return UWVM_ROS_INVALID;}
        }
        next.array_wire.data=(unsigned char const*)bytes;next.array_wire.size=length;
    } else return UWVM_ROS_INVALID;
    if(!roswasmc_reader_done(&in)) return UWVM_ROS_INVALID;*value=next;return 0;
}
/** @brief Encode a complete scalar/array parameter value into caller-owned storage.
 * Array wire is validated before copying. Failure preserves length, while scratch
 * bytes are unspecified. Policy still enforces backend-specific type support.
 */
static inline uint32_t roswasmc_encode_parameter(roswasmc_parameter_value const* value,void* bytes,size_t capacity,size_t* length) {
    roswasmc_writer out=roswasmc_writer_init(bytes,capacity);roswasmc_parameter_value checked;
    if(!value||!length) return UWVM_ROS_INVALID;
    if(value->tag==ROSWASMC_PARAM_BOOL) {if(value->boolean!=0&&value->boolean!=1) return UWVM_ROS_INVALID;roswasmc_write_integer(&out,(uint64_t)value->boolean,1);}
    else if(value->tag==ROSWASMC_PARAM_INT32||value->tag==ROSWASMC_PARAM_INT64) roswasmc_write_integer(&out,value->integer_bits,value->tag==ROSWASMC_PARAM_INT32?4:8);
    else if(value->tag==ROSWASMC_PARAM_FLOAT64) roswasmc_write_float64(&out,value->real);
    else if(value->tag==ROSWASMC_PARAM_STRING) {if(!roswasmc_utf8(value->text,1)) return UWVM_ROS_INVALID;roswasmc_write_string(&out,value->text);}
    else {if(roswasmc_decode_parameter(value->tag,value->array_wire.data,value->array_wire.size,&checked)) return UWVM_ROS_INVALID;roswasmc_write_bytes(&out,value->array_wire.data,value->array_wire.size);}
    if(!out.valid||out.size>4193786u) return UWVM_ROS_INVALID;*length=out.size;return 0;
}
/** @brief Typed ROS2 parameter event v2; values and strings borrow the input buffer.
 * Deletion has kind3 and an empty outer payload. Tag0 is deletion-only and carries
 * no assertion about a prior native value. Types1..9 keep their exact tags.
 */
typedef struct roswasmc_typed_parameter_event {
    uint32_t seconds,nanoseconds;unsigned char kind;roswasmc_string node,name;
    roswasmc_parameter_value value;
} roswasmc_typed_parameter_event;
/** @brief Validate the existing canonical ROS2 event node/parameter name grammar. */
static inline int roswasmc_parameter_name(roswasmc_string name,int node) {
    size_t i=node?1:0;int first=1;
    if(!name.data||!name.size||name.size>(node?1024u:255u)||(node&&(name.size<2||name.data[0]!='/'))) return 0;
    for(;i<name.size;++i) {
        char c=name.data[i];int alpha=(c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_';
        if(c==(node?'/':'.')) {if(first) return 0;first=1;continue;}
        if(!alpha&&!(c>='0'&&c<='9'&&!first)) return 0;first=0;
    }
    return !first;
}
/** @brief Decode the complete typed/deletion parameter-event envelope transactionally. */
static inline uint32_t roswasmc_decode_typed_parameter_event(void const* bytes,size_t length,roswasmc_typed_parameter_event* event) {
    roswasmc_reader in=roswasmc_reader_init(bytes,length);roswasmc_typed_parameter_event next={0};uint64_t n,version,tag,payload;
    if(!event||length>4193786u||!roswasmc_read_integer(&in,&n,4)||n>2147483647u) return UWVM_ROS_INVALID;next.seconds=(uint32_t)n;
    if(!roswasmc_read_integer(&in,&n,4)||n>=1000000000u) return UWVM_ROS_INVALID;next.nanoseconds=(uint32_t)n;
    if(!roswasmc_read_integer(&in,&n,1)||n<1||n>3) return UWVM_ROS_INVALID;next.kind=(unsigned char)n;
    if(!roswasmc_read_integer(&in,&version,1)||version!=2||!roswasmc_read_integer(&in,&tag,1)||tag>9||(tag==0&&next.kind!=3)||
       !roswasmc_read_string(&in,&next.node)||!roswasmc_read_string(&in,&next.name)||!roswasmc_read_integer(&in,&payload,4)||payload!=in.size-in.position) return UWVM_ROS_INVALID;
    if(!roswasmc_parameter_name(next.node,1)||!roswasmc_parameter_name(next.name,0)) return UWVM_ROS_INVALID;
    next.value.tag=(roswasmc_parameter_tag)tag;
    if(next.kind==3) {if(payload) return UWVM_ROS_INVALID;}
    else if(roswasmc_decode_parameter((roswasmc_parameter_tag)tag,in.data+in.position,(size_t)payload,&next.value)) return UWVM_ROS_INVALID;
    *event=next;return 0;
}
/** @brief Legacy String parameter event; node/name/value views borrow exact input bytes. */
typedef struct roswasmc_parameter_event {uint32_t seconds,nanoseconds;unsigned char kind;roswasmc_string node,name,value;} roswasmc_parameter_event;
/** @brief Decode the legacy String event shape; this grants no mutation authority. */
static inline uint32_t roswasmc_decode_parameter_event(void const* bytes,size_t length,roswasmc_parameter_event* event) {
    roswasmc_reader in=roswasmc_reader_init(bytes,length);roswasmc_parameter_event next;uint64_t n;
    if(!event||!roswasmc_read_integer(&in,&n,4)) return UWVM_ROS_INVALID;next.seconds=(uint32_t)n;
    if(!roswasmc_read_integer(&in,&n,4)) return UWVM_ROS_INVALID;next.nanoseconds=(uint32_t)n;
    if(!roswasmc_read_integer(&in,&n,1)||n<1||n>3) return UWVM_ROS_INVALID;next.kind=(unsigned char)n;
    if(!roswasmc_read_string(&in,&next.node)||!roswasmc_read_string(&in,&next.name)||!roswasmc_read_string(&in,&next.value)||!roswasmc_reader_done(&in)) return UWVM_ROS_INVALID;*event=next;return 0;
}
/** @brief Fibonacci feedback/result sequence with borrowed canonical i32 element bytes. */
typedef struct roswasmc_action_sequence {uint64_t goal;unsigned char kind;uint32_t count;unsigned char const* values_le;} roswasmc_action_sequence;
/** @brief Decode feedback without kind, or result with terminal kind2..4; at most48 elements. */
static inline uint32_t roswasmc_decode_action_sequence(void const* bytes,size_t length,int has_kind,roswasmc_action_sequence* value) {
    roswasmc_reader in=roswasmc_reader_init(bytes,length);roswasmc_action_sequence next;uint64_t n;next.kind=0;
    if(!value||!roswasmc_read_integer(&in,&next.goal,8)||!next.goal) return UWVM_ROS_INVALID;
    if(has_kind) {if(!roswasmc_read_integer(&in,&n,1)||n<1||n>4) return UWVM_ROS_INVALID;next.kind=(unsigned char)n;}
    if(!roswasmc_read_integer(&in,&n,4)||n>48||n*4!=in.size-in.position) return UWVM_ROS_INVALID;
    next.count=(uint32_t)n;next.values_le=in.data+in.position;*value=next;return 0;
}
/** @brief Encode server feedback1/terminalresult2..4, preserving supplied i32 element bits. */
static inline uint32_t roswasmc_encode_action_sequence(roswasmc_action_sequence const* value,int include_kind,void* bytes,size_t capacity,size_t* length) {
    roswasmc_writer out=roswasmc_writer_init(bytes,capacity);
    if(!value||!length||!value->goal||value->count>48||(include_kind&&(value->kind<1||value->kind>4))) return UWVM_ROS_INVALID;
    if(!roswasmc_write_integer(&out,value->goal,8)||(include_kind&&!roswasmc_write_integer(&out,value->kind,1))||!roswasmc_write_integer(&out,value->count,4)||
       !roswasmc_write_bytes(&out,value->values_le,(size_t)value->count*4)) return UWVM_ROS_INVALID;*length=out.size;return 0;
}
/** @brief ROS2 Lifecycle GetState value with a borrowed label. */
typedef struct roswasmc_lifecycle_state {unsigned char id;roswasmc_string label;} roswasmc_lifecycle_state;
/** @brief Decode canonical state byte plus String; unsupported ROS1 is never emulated. */
static inline uint32_t roswasmc_decode_lifecycle_state(void const* bytes,size_t length,roswasmc_lifecycle_state* value) {
    roswasmc_reader in=roswasmc_reader_init(bytes,length);roswasmc_lifecycle_state next;uint64_t id;
    if(!value||!roswasmc_read_integer(&in,&id,1)||!roswasmc_read_string(&in,&next.label)||!roswasmc_utf8(next.label,1)||!roswasmc_reader_done(&in)) return UWVM_ROS_INVALID;next.id=(unsigned char)id;*value=next;return 0;
}
/** @brief Encode String and log at the host-selected severity using bounded caller storage. */
static inline uint32_t roswasmc_log_string(roswasmc_capability const* cap,roswasmc_string text,void* scratch,size_t capacity) {
    size_t length;uint32_t error=roswasmc_encode_string(text,scratch,capacity,&length);return error?error:roswasmc_log(cap,(char const*)scratch,length);
}
/** @brief Encode a complete typed/deletion ROS2 parameter-event v2 with caller scratch.
 * @param payload_scratch Distinct storage for the selected scalar/array payload.
 * @param bytes Final output storage, which must not overlap payload_scratch.
 * Only tags0..9 exist for typed events; ROS1 int32 tags10/11 are refused.
 * Failure preserves length; caller scratch bytes are unspecified on failure.
 */
static inline uint32_t roswasmc_encode_typed_parameter_event(roswasmc_typed_parameter_event const* event,
    void* payload_scratch,size_t payload_capacity,void* bytes,size_t capacity,size_t* length) {
    roswasmc_writer out=roswasmc_writer_init(bytes,capacity);size_t payload_length=0;uint32_t error;
    if(!event||!length||event->seconds>2147483647u||event->nanoseconds>=1000000000u||event->kind<1||event->kind>3||event->value.tag>9||
       (event->value.tag==0&&event->kind!=3)||!roswasmc_parameter_name(event->node,1)||!roswasmc_parameter_name(event->name,0)) return UWVM_ROS_INVALID;
    if(event->kind!=3) {error=roswasmc_encode_parameter(&event->value,payload_scratch,payload_capacity,&payload_length);if(error) return error;}
    if(payload_length>UINT32_MAX||!roswasmc_write_integer(&out,event->seconds,4)||!roswasmc_write_integer(&out,event->nanoseconds,4)||
       !roswasmc_write_integer(&out,event->kind,1)||!roswasmc_write_integer(&out,2,1)||!roswasmc_write_integer(&out,event->value.tag,1)||
       !roswasmc_write_string(&out,event->node)||!roswasmc_write_string(&out,event->name)||!roswasmc_write_integer(&out,payload_length,4)||
       !roswasmc_write_bytes(&out,payload_scratch,payload_length)||out.size>4193786u) return UWVM_ROS_INVALID;*length=out.size;return 0;
}
/** @brief Encode the legacy String parameter-event shape; a deletion value must be empty. */
static inline uint32_t roswasmc_encode_parameter_event(roswasmc_parameter_event const* event,void* bytes,size_t capacity,size_t* length) {
    roswasmc_writer out=roswasmc_writer_init(bytes,capacity);
    if(!event||!length||event->kind<1||event->kind>3||(event->kind==3&&event->value.size)||!roswasmc_utf8(event->node,0)||!roswasmc_utf8(event->name,0)||!roswasmc_utf8(event->value,1)||
       !roswasmc_write_integer(&out,event->seconds,4)||!roswasmc_write_integer(&out,event->nanoseconds,4)||!roswasmc_write_integer(&out,event->kind,1)||
       !roswasmc_write_string(&out,event->node)||!roswasmc_write_string(&out,event->name)||!roswasmc_write_string(&out,event->value)) return UWVM_ROS_INVALID;*length=out.size;return 0;
}
/** @brief Encode a canonical Trigger/SetBool response without a request-ID envelope. */
static inline uint32_t roswasmc_encode_service_response(roswasmc_service_response response,void* bytes,size_t capacity,size_t* length) {
    roswasmc_writer out=roswasmc_writer_init(bytes,capacity);
    if(!length||(response.success!=0&&response.success!=1)||!roswasmc_utf8(response.message,1)||!roswasmc_write_integer(&out,(uint64_t)response.success,1)||!roswasmc_write_string(&out,response.message)) return UWVM_ROS_INVALID;
    *length=out.size;return 0;
}
/** @brief Encode a gateway-owned service request/response envelope around supplied canonical bytes.
 * This is a wire helper only. It does not make a fabricated request ID valid.
 */
static inline uint32_t roswasmc_encode_service_envelope(uint64_t request_id,roswasmc_bytes payload,void* bytes,size_t capacity,size_t* length) {
    roswasmc_writer out=roswasmc_writer_init(bytes,capacity);
    if(!length||!request_id||!roswasmc_write_integer(&out,request_id,8)||!roswasmc_write_bytes(&out,payload.data,payload.size)) return UWVM_ROS_INVALID;*length=out.size;return 0;
}
/** @brief Encode a signed Fibonacci order into exactly four canonical bytes. */
static inline uint32_t roswasmc_encode_fibonacci_order(int32_t order,void* bytes,size_t capacity,size_t* length) {
    roswasmc_writer out=roswasmc_writer_init(bytes,capacity);if(!length||!roswasmc_write_integer(&out,(uint32_t)order,4)) return UWVM_ROS_INVALID;*length=out.size;return 0;
}
/** @brief Encode a goal ID or nanosecond time into exactly eight canonical bytes.
 * Goal ownership/nonzero rules are operation-specific and still enforced by host.
 */
static inline uint32_t roswasmc_encode_u64(uint64_t value,void* bytes,size_t capacity,size_t* length) {
    roswasmc_writer out=roswasmc_writer_init(bytes,capacity);if(!length||!roswasmc_write_integer(&out,value,8)) return UWVM_ROS_INVALID;*length=out.size;return 0;
}
/** @brief Encode a Lifecycle state byte followed by a canonical label String. */
static inline uint32_t roswasmc_encode_lifecycle_state(roswasmc_lifecycle_state state,void* bytes,size_t capacity,size_t* length) {
    roswasmc_writer out=roswasmc_writer_init(bytes,capacity);
    if(!length||!roswasmc_utf8(state.label,1)||!roswasmc_write_integer(&out,state.id,1)||!roswasmc_write_string(&out,state.label)) return UWVM_ROS_INVALID;*length=out.size;return 0;
}
#endif
