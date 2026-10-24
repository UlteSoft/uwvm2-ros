/** Allocation-free scalar/codec/local-owner checks, valid C89 and later.
 * No native ROS, fake import, host-language Guest or network is used.
 * Running this natively proves only local C behavior. A separate actual Wasm
 * Runtime row is required for Guest execution evidence.
 */
#include <platform/ros/c/roswasm/roswasm.h>
#include <platform/ros/c/roswasm/events.h>
#define CHECK(condition) do {if(!(condition)) return __LINE__;} while(0)
int main(void) {
    unsigned char bytes[256],array[64];size_t length=77;uwvm_ros_u64 high=UWVM_ROS_U64_C(0xfedcba9876543210),bits;
    roswasmc_writer writer;roswasmc_reader reader;roswasmc_string text,decoded;
    roswasmc_image image={0},image_copy;roswasmc_transform transform={0},transform_copy;
    roswasmc_parameter_value parameter={ROSWASMC_PARAM_DELETION},element={ROSWASMC_PARAM_DELETION};
    roswasmc_action_sequence sequence;uwvm_ros_i32 signed_element=42;
    roswasmc_backend backend={0};roswasmc_context context;roswasmc_capability source,destination;
    CHECK(sizeof(uwvm_ros_u32)==4&&sizeof(uwvm_ros_u64)==8);
    writer=roswasmc_writer_init(bytes,sizeof bytes);CHECK(roswasmc_write_integer(&writer,high,8));
    CHECK(bytes[0]==0x10&&bytes[7]==0xfe&&uwvm_ros_load_u64(bytes)==high);
    reader=roswasmc_reader_init(bytes,8);bits=0;CHECK(roswasmc_read_integer(&reader,&bits,8)&&bits==high&&roswasmc_reader_done(&reader));
    CHECK(!roswasmc_read_integer(&reader,&bits,1)&&bits==high);
    CHECK(!roswasmc_write_integer(&writer,high,7));CHECK(!roswasmc_write_integer(&writer,0,1));
    writer=roswasmc_writer_init(bytes,sizeof bytes);CHECK(!roswasmc_write_integer(&writer,0,9));
    CHECK(roswasmc_event_integer(bytes,9)==0&&roswasmc_event_integer(0,8)==0);
    writer=roswasmc_writer_init(bytes,1);writer.size=2;CHECK(!roswasmc_write_bytes(&writer,bytes,1));
    reader=roswasmc_reader_init(bytes,1);reader.position=2;CHECK(!roswasmc_read_integer(&reader,&bits,1));
    CHECK(!roswasmc_read_integer(0,&bits,8)&&!roswasmc_write_integer(0,0,8));
    text=roswasmc_string_init("a\0b",3);CHECK(!roswasmc_encode_string(text,bytes,sizeof bytes,&length)&&length==7);
    CHECK(!roswasmc_decode_string(bytes,length,&decoded)&&decoded.size==3&&decoded.data[1]==0);
    CHECK(roswasmc_decode_string(bytes,length-1,&decoded)==22&&decoded.size==3);
    image.sequence=7;image.seconds=8;image.nanoseconds=9;image.height=1;image.width=2;image.step=2;
    image.frame_id=roswasmc_string_init("camera",6);image.encoding=roswasmc_string_init("mono8",5);image.bigendian=1;
    image.data=roswasmc_bytes_init("\001\377",2);
    CHECK(!roswasmc_encode_image(&image,bytes,sizeof bytes,&length));CHECK(!roswasmc_decode_image(bytes,length,&image_copy));
    CHECK(image_copy.sequence==7&&image_copy.nanoseconds==9&&image_copy.step==2&&image_copy.bigendian==1&&image_copy.data.size==2&&image_copy.data.data[1]==255);
    transform.parent_frame=roswasmc_string_init("world",5);transform.child_frame=roswasmc_string_init("sensor",6);transform.quaternion[3]=1;
    transform.translation[0]=-0.25;transform.translation[1]=1.5;transform.translation[2]=3;
    CHECK(!roswasmc_encode_transform(&transform,bytes,sizeof bytes,&length)&&!roswasmc_decode_transform(bytes,length,&transform_copy));
    CHECK(transform_copy.translation[0]==-0.25&&transform_copy.quaternion[3]==1&&transform_copy.child_frame.size==6);
    writer=roswasmc_writer_init(array,sizeof array);CHECK(roswasmc_write_integer(&writer,2,4)&&roswasmc_write_integer(&writer,high,8)&&roswasmc_write_integer(&writer,UWVM_ROS_U64_MAX,8));
    CHECK(!roswasmc_decode_parameter(ROSWASMC_PARAM_INT64S,array,writer.size,&parameter));
    CHECK(!roswasmc_parameter_element(&parameter,0,&element)&&element.tag==ROSWASMC_PARAM_INT64&&element.integer_bits==high);
    CHECK(!roswasmc_parameter_element(&parameter,1,&element)&&element.integer_bits==UWVM_ROS_U64_MAX);
    CHECK(roswasmc_parameter_element(&parameter,2,&element)==22&&element.integer_bits==UWVM_ROS_U64_MAX);
    writer=roswasmc_writer_init(array,sizeof array);CHECK(roswasmc_write_integer(&writer,2,4)&&roswasmc_write_string(&writer,roswasmc_string_init("first",5))&&roswasmc_write_string(&writer,roswasmc_string_init("last",4)));
    CHECK(!roswasmc_decode_parameter(ROSWASMC_PARAM_STRINGS,array,writer.size,&parameter)&&!roswasmc_parameter_element(&parameter,1,&element)&&element.text.size==4);
    writer=roswasmc_writer_init(array,sizeof array);CHECK(roswasmc_write_integer(&writer,1,4)&&roswasmc_write_float64(&writer,-0.125));
    CHECK(!roswasmc_decode_parameter(ROSWASMC_PARAM_FLOAT64S,array,writer.size,&parameter)&&!roswasmc_parameter_element(&parameter,0,&element)&&element.real==-0.125);
    writer=roswasmc_writer_init(array,sizeof array);CHECK(roswasmc_write_integer(&writer,1,4)&&roswasmc_write_integer(&writer,1,1));
    CHECK(!roswasmc_decode_parameter(ROSWASMC_PARAM_BOOLS,array,writer.size,&parameter)&&!roswasmc_parameter_element(&parameter,0,&element)&&element.boolean==1);
    uwvm_ros_store_u32(bytes,2147483648UL);sequence.goal=1;sequence.kind=0;sequence.count=1;sequence.values_le=bytes;
    CHECK(!roswasmc_action_sequence_element(&sequence,0,&signed_element)&&signed_element==(-2147483647-1));
    CHECK(roswasmc_action_sequence_element(&sequence,1,&signed_element)==22&&signed_element==(-2147483647-1));
    context.backend=&backend;roswasmc_capability_init(&source,&context);roswasmc_capability_init(&destination,&context);
    CHECK(roswasmc_capability_take(&destination,&source)==9);source.handle=high;
    CHECK(!roswasmc_capability_take(&destination,&source)&&destination.handle==high&&!source.handle);
    CHECK(roswasmc_capability_take(&destination,&source)==17&&destination.handle==high);
    CHECK(roswasmc_revoke(&destination)==UWVM_ROS_UNSUPPORTED&&destination.handle==high);
    CHECK(roswasmc_close(&destination)==UWVM_ROS_UNSUPPORTED&&destination.handle==high);
    CHECK(roswasmc_parameter_erase(&source,ROSWASMC_PARAMETER_GET,bytes,sizeof bytes).error==22);
    return 0;
}
