/** @file
 * @brief Prepared C11 golden-wire fixture covering all57 positive and11 negative cases.
 * Expected bytes come independently from sdk/abi/wire-vectors-v1.json, SHA256 e4b1070d1a579019950b34aaf558abc7888449a46db0d5d4bb430515973a2d75.
 * This fixture is not executed here and does not establish native ROS or ARM behavior.
 */
#include <assert.h>
#include <string.h>
#include <roswasm/node.h>
/** @brief Encode complete builtin values and compare independently specified canonical bytes. */
int main(void) {
{ /* string-empty */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_string value={(char const*)"",0};assert(roswasmc_encode_string(value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={0,0,0,0};assert(length==4&&memcmp(encoded,expected,length)==0);
}
{ /* string-ascii */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_string value={(char const*)"\x68\x65\x6c\x6c\x6f",5};assert(roswasmc_encode_string(value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={5,0,0,0,104,101,108,108,111};assert(length==9&&memcmp(encoded,expected,length)==0);
}
{ /* string-utf8 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_string value={(char const*)"\x72\x6f\x62\x6f\x74\x20\xf0\x9f\x8c\x8d",10};assert(roswasmc_encode_string(value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={10,0,0,0,114,111,98,111,116,32,240,159,140,141};assert(length==14&&memcmp(encoded,expected,length)==0);
}
{ /* bool-false */
unsigned char encoded[512],payload[512];size_t length=0;
assert(roswasmc_encode_bool(0,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={0};assert(length==1&&memcmp(encoded,expected,length)==0);
}
{ /* bool-true */
unsigned char encoded[512],payload[512];size_t length=0;
assert(roswasmc_encode_bool(1,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1};assert(length==1&&memcmp(encoded,expected,length)==0);
}
{ /* int32--2147483648 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_parameter_value value={0};value.tag=(roswasmc_parameter_tag)10;
value.integer_bits=UINT64_C(2147483648);
assert(roswasmc_encode_parameter(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={0,0,0,128};assert(length==4&&memcmp(encoded,expected,length)==0);
}
{ /* int32--1 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_parameter_value value={0};value.tag=(roswasmc_parameter_tag)10;
value.integer_bits=UINT64_C(4294967295);
assert(roswasmc_encode_parameter(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={255,255,255,255};assert(length==4&&memcmp(encoded,expected,length)==0);
}
{ /* int32-2147483647 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_parameter_value value={0};value.tag=(roswasmc_parameter_tag)10;
value.integer_bits=UINT64_C(2147483647);
assert(roswasmc_encode_parameter(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={255,255,255,127};assert(length==4&&memcmp(encoded,expected,length)==0);
}
{ /* int64--9223372036854775808 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_parameter_value value={0};value.tag=(roswasmc_parameter_tag)2;
value.integer_bits=UINT64_C(9223372036854775808);
assert(roswasmc_encode_parameter(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={0,0,0,0,0,0,0,128};assert(length==8&&memcmp(encoded,expected,length)==0);
}
{ /* int64--1 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_parameter_value value={0};value.tag=(roswasmc_parameter_tag)2;
value.integer_bits=UINT64_C(18446744073709551615);
assert(roswasmc_encode_parameter(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={255,255,255,255,255,255,255,255};assert(length==8&&memcmp(encoded,expected,length)==0);
}
{ /* int64-9223372036854775807 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_parameter_value value={0};value.tag=(roswasmc_parameter_tag)2;
value.integer_bits=UINT64_C(9223372036854775807);
assert(roswasmc_encode_parameter(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={255,255,255,255,255,255,255,127};assert(length==8&&memcmp(encoded,expected,length)==0);
}
{ /* float64-zero */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_parameter_value value={0};value.tag=(roswasmc_parameter_tag)3;
value.real=0.0;
assert(roswasmc_encode_parameter(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={0,0,0,0,0,0,0,0};assert(length==8&&memcmp(encoded,expected,length)==0);
}
{ /* float64-negative-zero */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_parameter_value value={0};value.tag=(roswasmc_parameter_tag)3;
value.real=-0.0;
assert(roswasmc_encode_parameter(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={0,0,0,0,0,0,0,128};assert(length==8&&memcmp(encoded,expected,length)==0);
}
{ /* float64-fraction */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_parameter_value value={0};value.tag=(roswasmc_parameter_tag)3;
value.real=1.25;
assert(roswasmc_encode_parameter(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={0,0,0,0,0,0,244,63};assert(length==8&&memcmp(encoded,expected,length)==0);
}
{ /* twist */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_twist value={{1.0,-2.0,3.0},{0.0,-0.0,0.5}};assert(roswasmc_encode_twist(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,192,0,0,0,0,0,0,8,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,0,0,0,0,0,0,224,63};assert(length==48&&memcmp(encoded,expected,length)==0);
}
{ /* array-bool */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_writer writer=roswasmc_writer_init(payload,sizeof payload);roswasmc_parameter_value value;assert(roswasmc_write_integer(&writer,3,4));
assert(roswasmc_write_integer(&writer,UINT64_C(0),1));
assert(roswasmc_write_integer(&writer,UINT64_C(1),1));
assert(roswasmc_write_integer(&writer,UINT64_C(0),1));
assert(roswasmc_decode_parameter((roswasmc_parameter_tag)6,payload,writer.size,&value)==0);assert(roswasmc_encode_parameter(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={3,0,0,0,0,1,0};assert(length==7&&memcmp(encoded,expected,length)==0);
}
{ /* array-byte */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_writer writer=roswasmc_writer_init(payload,sizeof payload);roswasmc_parameter_value value;assert(roswasmc_write_integer(&writer,3,4));
assert(roswasmc_write_integer(&writer,UINT64_C(0),1));
assert(roswasmc_write_integer(&writer,UINT64_C(127),1));
assert(roswasmc_write_integer(&writer,UINT64_C(255),1));
assert(roswasmc_decode_parameter((roswasmc_parameter_tag)5,payload,writer.size,&value)==0);assert(roswasmc_encode_parameter(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={3,0,0,0,0,127,255};assert(length==7&&memcmp(encoded,expected,length)==0);
}
{ /* array-int32 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_writer writer=roswasmc_writer_init(payload,sizeof payload);roswasmc_parameter_value value;assert(roswasmc_write_integer(&writer,2,4));
assert(roswasmc_write_integer(&writer,UINT64_C(4294967295),4));
assert(roswasmc_write_integer(&writer,UINT64_C(2147483647),4));
assert(roswasmc_decode_parameter((roswasmc_parameter_tag)11,payload,writer.size,&value)==0);assert(roswasmc_encode_parameter(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={2,0,0,0,255,255,255,255,255,255,255,127};assert(length==12&&memcmp(encoded,expected,length)==0);
}
{ /* array-int64 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_writer writer=roswasmc_writer_init(payload,sizeof payload);roswasmc_parameter_value value;assert(roswasmc_write_integer(&writer,2,4));
assert(roswasmc_write_integer(&writer,UINT64_C(9223372036854775808),8));
assert(roswasmc_write_integer(&writer,UINT64_C(9223372036854775807),8));
assert(roswasmc_decode_parameter((roswasmc_parameter_tag)7,payload,writer.size,&value)==0);assert(roswasmc_encode_parameter(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={2,0,0,0,0,0,0,0,0,0,0,128,255,255,255,255,255,255,255,127};assert(length==20&&memcmp(encoded,expected,length)==0);
}
{ /* array-float64 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_writer writer=roswasmc_writer_init(payload,sizeof payload);roswasmc_parameter_value value;assert(roswasmc_write_integer(&writer,2,4));
assert(roswasmc_write_float64(&writer,1.25));
assert(roswasmc_write_float64(&writer,-0.0));
assert(roswasmc_decode_parameter((roswasmc_parameter_tag)8,payload,writer.size,&value)==0);assert(roswasmc_encode_parameter(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={2,0,0,0,0,0,0,0,0,0,244,63,0,0,0,0,0,0,0,128};assert(length==20&&memcmp(encoded,expected,length)==0);
}
{ /* array-string */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_writer writer=roswasmc_writer_init(payload,sizeof payload);roswasmc_parameter_value value;assert(roswasmc_write_integer(&writer,3,4));
{roswasmc_string text={(char const*)"",0};assert(roswasmc_write_string(&writer,text));}
{roswasmc_string text={(char const*)"\x68\x65\x6c\x6c\x6f",5};assert(roswasmc_write_string(&writer,text));}
{roswasmc_string text={(char const*)"\xce\xbb",2};assert(roswasmc_write_string(&writer,text));}
assert(roswasmc_decode_parameter((roswasmc_parameter_tag)9,payload,writer.size,&value)==0);assert(roswasmc_encode_parameter(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={3,0,0,0,0,0,0,0,5,0,0,0,104,101,108,108,111,2,0,0,0,206,187};assert(length==23&&memcmp(encoded,expected,length)==0);
}
{ /* array-empty */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_writer writer=roswasmc_writer_init(payload,sizeof payload);roswasmc_parameter_value value;assert(roswasmc_write_integer(&writer,0,4));
assert(roswasmc_decode_parameter((roswasmc_parameter_tag)9,payload,writer.size,&value)==0);assert(roswasmc_encode_parameter(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={0,0,0,0};assert(length==4&&memcmp(encoded,expected,length)==0);
}
{ /* image-rgb8 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_image value={0};
value.sequence=0;
value.seconds=1;
value.nanoseconds=2;
{roswasmc_string text={(char const*)"\x63\x61\x6d\x65\x72\x61",6};value.frame_id=text;}
value.height=1;
value.width=1;
{roswasmc_string text={(char const*)"\x72\x67\x62\x38",4};value.encoding=text;}
value.bigendian=0;
value.step=3;
unsigned char pixels[]={255,0,7};value.data.data=pixels;value.data.size=sizeof pixels;
assert(roswasmc_encode_image(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={0,0,0,0,1,0,0,0,2,0,0,0,6,0,0,0,99,97,109,101,114,97,1,0,0,0,1,0,0,0,4,0,0,0,114,103,98,56,0,3,0,0,0,3,0,0,0,255,0,7};assert(length==50&&memcmp(encoded,expected,length)==0);
}
{ /* transform */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_transform value={0};
value.sequence=0;
value.seconds=1;
value.nanoseconds=2;
{roswasmc_string text={(char const*)"\x6d\x61\x70",3};value.parent_frame=text;}
{roswasmc_string text={(char const*)"\x62\x61\x73\x65",4};value.child_frame=text;}
value.translation[0]=1.0;
value.translation[1]=2.0;
value.translation[2]=3.0;
value.quaternion[0]=0.0;
value.quaternion[1]=0.0;
value.quaternion[2]=0.0;
value.quaternion[3]=1.0;
assert(roswasmc_encode_transform(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={0,0,0,0,1,0,0,0,2,0,0,0,3,0,0,0,109,97,112,4,0,0,0,98,97,115,101,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,64,0,0,0,0,0,0,8,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,240,63};assert(length==83&&memcmp(encoded,expected,length)==0);
}
{ /* trigger-request */
unsigned char encoded[512],payload[512];size_t length=0;
length=0;
unsigned char expected[]={0};assert(length==0&&memcmp(encoded,expected,length)==0);
}
{ /* set-bool-request */
unsigned char encoded[512],payload[512];size_t length=0;
assert(roswasmc_encode_bool(1,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1};assert(length==1&&memcmp(encoded,expected,length)==0);
}
{ /* service-response */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_service_response response={1,{(char const*)"\x6f\x6b",2}};assert(roswasmc_encode_service_response(response,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,2,0,0,0,111,107};assert(length==7&&memcmp(encoded,expected,length)==0);
}
{ /* service-request-event */
unsigned char encoded[512],payload[512];size_t length=0;
unsigned char inner[]={1};roswasmc_bytes inner_view={inner,sizeof inner};assert(roswasmc_encode_service_envelope(UINT64_C(9223372036854775813),inner_view,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={5,0,0,0,0,0,0,128,1};assert(length==9&&memcmp(encoded,expected,length)==0);
}
{ /* service-response-envelope */
unsigned char encoded[512],payload[512];size_t length=0;
unsigned char inner[]={1,2,0,0,0,111,107};roswasmc_bytes inner_view={inner,sizeof inner};assert(roswasmc_encode_service_envelope(UINT64_C(9223372036854775813),inner_view,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={5,0,0,0,0,0,0,128,1,2,0,0,0,111,107};assert(length==15&&memcmp(encoded,expected,length)==0);
}
{ /* fibonacci-order */
unsigned char encoded[512],payload[512];size_t length=0;
assert(roswasmc_encode_fibonacci_order(5,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={5,0,0,0};assert(length==4&&memcmp(encoded,expected,length)==0);
}
{ /* goal-id */
unsigned char encoded[512],payload[512];size_t length=0;
assert(roswasmc_encode_u64(UINT64_C(9223372036854775813),encoded,sizeof encoded,&length)==0);
unsigned char expected[]={5,0,0,0,0,0,0,128};assert(length==8&&memcmp(encoded,expected,length)==0);
}
{ /* incoming-goal */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_writer writer=roswasmc_writer_init(encoded,sizeof encoded);assert(roswasmc_write_integer(&writer,UINT64_C(9223372036854775813),8));assert(roswasmc_write_integer(&writer,5,4));length=writer.size;
unsigned char expected[]={5,0,0,0,0,0,0,128,5,0,0,0};assert(length==12&&memcmp(encoded,expected,length)==0);
}
{ /* feedback */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_writer writer=roswasmc_writer_init(payload,sizeof payload);roswasmc_action_sequence value={0};
assert(roswasmc_write_integer(&writer,0,4));
assert(roswasmc_write_integer(&writer,1,4));
assert(roswasmc_write_integer(&writer,1,4));
assert(roswasmc_write_integer(&writer,2,4));
assert(roswasmc_write_integer(&writer,3,4));
assert(roswasmc_write_integer(&writer,5,4));
value.goal=UINT64_C(9223372036854775813);value.kind=0;value.count=6;value.values_le=payload;assert(roswasmc_encode_action_sequence(&value,0,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={5,0,0,0,0,0,0,128,6,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,2,0,0,0,3,0,0,0,5,0,0,0};assert(length==36&&memcmp(encoded,expected,length)==0);
}
{ /* success */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_writer writer=roswasmc_writer_init(payload,sizeof payload);roswasmc_action_sequence value={0};
assert(roswasmc_write_integer(&writer,0,4));
assert(roswasmc_write_integer(&writer,1,4));
assert(roswasmc_write_integer(&writer,1,4));
assert(roswasmc_write_integer(&writer,2,4));
assert(roswasmc_write_integer(&writer,3,4));
assert(roswasmc_write_integer(&writer,5,4));
value.goal=UINT64_C(9223372036854775813);value.kind=2;value.count=6;value.values_le=payload;assert(roswasmc_encode_action_sequence(&value,1,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={5,0,0,0,0,0,0,128,2,6,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,2,0,0,0,3,0,0,0,5,0,0,0};assert(length==37&&memcmp(encoded,expected,length)==0);
}
{ /* server-feedback */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_writer writer=roswasmc_writer_init(payload,sizeof payload);roswasmc_action_sequence value={0};
assert(roswasmc_write_integer(&writer,0,4));
assert(roswasmc_write_integer(&writer,1,4));
assert(roswasmc_write_integer(&writer,1,4));
assert(roswasmc_write_integer(&writer,2,4));
assert(roswasmc_write_integer(&writer,3,4));
assert(roswasmc_write_integer(&writer,5,4));
value.goal=UINT64_C(9223372036854775813);value.kind=1;value.count=6;value.values_le=payload;assert(roswasmc_encode_action_sequence(&value,1,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={5,0,0,0,0,0,0,128,1,6,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,2,0,0,0,3,0,0,0,5,0,0,0};assert(length==37&&memcmp(encoded,expected,length)==0);
}
{ /* lifecycle-state */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_lifecycle_state value={3,{(char const*)"\x61\x63\x74\x69\x76\x65",6}};assert(roswasmc_encode_lifecycle_state(value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={3,6,0,0,0,97,99,116,105,118,101};assert(length==11&&memcmp(encoded,expected,length)==0);
}
{ /* time-nanoseconds */
unsigned char encoded[512],payload[512];size_t length=0;
assert(roswasmc_encode_u64(UINT64_C(9223372036854775813),encoded,sizeof encoded,&length)==0);
unsigned char expected[]={5,0,0,0,0,0,0,128};assert(length==8&&memcmp(encoded,expected,length)==0);
}
{ /* legacy-parameter-event */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_parameter_event value={1,2,2,{(char const*)"\x2f\x64\x65\x6d\x6f",5},{(char const*)"\x76\x61\x6c\x75\x65",5},{(char const*)"\x74\x72\x75\x65",4}};assert(roswasmc_encode_parameter_event(&value,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,2,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,4,0,0,0,116,114,117,101};assert(length==35&&memcmp(encoded,expected,length)==0);
}
{ /* typed-event-bool */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=2;value.value.tag=(roswasmc_parameter_tag)1;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
value.value.boolean=1;
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,2,2,1,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,1,0,0,0,1};assert(length==34&&memcmp(encoded,expected,length)==0);
}
{ /* typed-event-int64 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=2;value.value.tag=(roswasmc_parameter_tag)2;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
value.value.integer_bits=UINT64_C(9223372036854775807);
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,2,2,2,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,8,0,0,0,255,255,255,255,255,255,255,127};assert(length==41&&memcmp(encoded,expected,length)==0);
}
{ /* typed-event-float64 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=2;value.value.tag=(roswasmc_parameter_tag)3;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
value.value.real=1.25;
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,2,2,3,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,8,0,0,0,0,0,0,0,0,0,244,63};assert(length==41&&memcmp(encoded,expected,length)==0);
}
{ /* typed-event-string */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=2;value.value.tag=(roswasmc_parameter_tag)4;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
{roswasmc_string text={(char const*)"\x68\x65\x6c\x6c\x6f",5};value.value.text=text;}
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,2,2,4,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,9,0,0,0,5,0,0,0,104,101,108,108,111};assert(length==42&&memcmp(encoded,expected,length)==0);
}
{ /* typed-event-byte-array */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=2;value.value.tag=(roswasmc_parameter_tag)5;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
unsigned char array[512];roswasmc_writer writer=roswasmc_writer_init(array,sizeof array);assert(roswasmc_write_integer(&writer,2,4));
assert(roswasmc_write_integer(&writer,UINT64_C(0),1));
assert(roswasmc_write_integer(&writer,UINT64_C(255),1));
assert(roswasmc_decode_parameter((roswasmc_parameter_tag)5,array,writer.size,&value.value)==0);
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,2,2,5,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,6,0,0,0,2,0,0,0,0,255};assert(length==39&&memcmp(encoded,expected,length)==0);
}
{ /* typed-event-bool-array */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=2;value.value.tag=(roswasmc_parameter_tag)6;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
unsigned char array[512];roswasmc_writer writer=roswasmc_writer_init(array,sizeof array);assert(roswasmc_write_integer(&writer,2,4));
assert(roswasmc_write_integer(&writer,UINT64_C(0),1));
assert(roswasmc_write_integer(&writer,UINT64_C(1),1));
assert(roswasmc_decode_parameter((roswasmc_parameter_tag)6,array,writer.size,&value.value)==0);
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,2,2,6,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,6,0,0,0,2,0,0,0,0,1};assert(length==39&&memcmp(encoded,expected,length)==0);
}
{ /* typed-event-int64-array */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=2;value.value.tag=(roswasmc_parameter_tag)7;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
unsigned char array[512];roswasmc_writer writer=roswasmc_writer_init(array,sizeof array);assert(roswasmc_write_integer(&writer,2,4));
assert(roswasmc_write_integer(&writer,UINT64_C(18446744073709551615),8));
assert(roswasmc_write_integer(&writer,UINT64_C(9223372036854775807),8));
assert(roswasmc_decode_parameter((roswasmc_parameter_tag)7,array,writer.size,&value.value)==0);
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,2,2,7,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,20,0,0,0,2,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,127};assert(length==53&&memcmp(encoded,expected,length)==0);
}
{ /* typed-event-float64-array */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=2;value.value.tag=(roswasmc_parameter_tag)8;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
unsigned char array[512];roswasmc_writer writer=roswasmc_writer_init(array,sizeof array);assert(roswasmc_write_integer(&writer,1,4));
assert(roswasmc_write_float64(&writer,1.25));
assert(roswasmc_decode_parameter((roswasmc_parameter_tag)8,array,writer.size,&value.value)==0);
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,2,2,8,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,12,0,0,0,1,0,0,0,0,0,0,0,0,0,244,63};assert(length==45&&memcmp(encoded,expected,length)==0);
}
{ /* typed-event-string-array */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=2;value.value.tag=(roswasmc_parameter_tag)9;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
unsigned char array[512];roswasmc_writer writer=roswasmc_writer_init(array,sizeof array);assert(roswasmc_write_integer(&writer,2,4));
{roswasmc_string text={(char const*)"\x68\x65\x6c\x6c\x6f",5};assert(roswasmc_write_string(&writer,text));}
{roswasmc_string text={(char const*)"",0};assert(roswasmc_write_string(&writer,text));}
assert(roswasmc_decode_parameter((roswasmc_parameter_tag)9,array,writer.size,&value.value)==0);
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,2,2,9,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,17,0,0,0,2,0,0,0,5,0,0,0,104,101,108,108,111,0,0,0,0};assert(length==50&&memcmp(encoded,expected,length)==0);
}
{ /* typed-deletion-tag-0 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=3;value.value.tag=(roswasmc_parameter_tag)0;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,3,2,0,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(length==33&&memcmp(encoded,expected,length)==0);
}
{ /* typed-deletion-tag-1 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=3;value.value.tag=(roswasmc_parameter_tag)1;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,3,2,1,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(length==33&&memcmp(encoded,expected,length)==0);
}
{ /* typed-deletion-tag-2 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=3;value.value.tag=(roswasmc_parameter_tag)2;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,3,2,2,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(length==33&&memcmp(encoded,expected,length)==0);
}
{ /* typed-deletion-tag-3 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=3;value.value.tag=(roswasmc_parameter_tag)3;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,3,2,3,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(length==33&&memcmp(encoded,expected,length)==0);
}
{ /* typed-deletion-tag-4 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=3;value.value.tag=(roswasmc_parameter_tag)4;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,3,2,4,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(length==33&&memcmp(encoded,expected,length)==0);
}
{ /* typed-deletion-tag-5 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=3;value.value.tag=(roswasmc_parameter_tag)5;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,3,2,5,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(length==33&&memcmp(encoded,expected,length)==0);
}
{ /* typed-deletion-tag-6 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=3;value.value.tag=(roswasmc_parameter_tag)6;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,3,2,6,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(length==33&&memcmp(encoded,expected,length)==0);
}
{ /* typed-deletion-tag-7 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=3;value.value.tag=(roswasmc_parameter_tag)7;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,3,2,7,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(length==33&&memcmp(encoded,expected,length)==0);
}
{ /* typed-deletion-tag-8 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=3;value.value.tag=(roswasmc_parameter_tag)8;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,3,2,8,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(length==33&&memcmp(encoded,expected,length)==0);
}
{ /* typed-deletion-tag-9 */
unsigned char encoded[512],payload[512];size_t length=0;
roswasmc_typed_parameter_event value={0};value.seconds=1;value.nanoseconds=2;value.kind=3;value.value.tag=(roswasmc_parameter_tag)9;{roswasmc_string node={(char const*)"\x2f\x64\x65\x6d\x6f",5},name={(char const*)"\x76\x61\x6c\x75\x65",5};value.node=node;value.name=name;}
assert(roswasmc_encode_typed_parameter_event(&value,payload,sizeof payload,encoded,sizeof encoded,&length)==0);
unsigned char expected[]={1,0,0,0,2,0,0,0,3,2,9,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(length==33&&memcmp(encoded,expected,length)==0);
}
{ /* Negative: Bool must be exactly 0 or 1. */
unsigned char bad[]={2};
int value=77;assert(roswasmc_decode_bool(bad,sizeof bad,&value)==22&&value==77);
}
{ /* Negative: Reject trailing bytes. */
unsigned char bad[]={1,0};
int value=77;assert(roswasmc_decode_bool(bad,sizeof bad,&value)==22&&value==77);
}
{ /* Negative: Declared UTF-8 byte length exceeds the remaining buffer. */
unsigned char bad[]={5,0,0,0,104,105};
roswasmc_string value={"keep",4};assert(roswasmc_decode_string(bad,sizeof bad,&value)==22&&value.size==4);
}
{ /* Negative: Overlong UTF-8 is invalid. */
unsigned char bad[]={2,0,0,0,192,175};
roswasmc_string value={"keep",4};assert(roswasmc_decode_string(bad,sizeof bad,&value)==22&&value.size==4);
}
{ /* Negative: Float64 must be finite. */
unsigned char bad[]={0,0,0,0,0,0,240,127};
roswasmc_parameter_value value={0};value.integer_bits=77;assert(roswasmc_decode_parameter((roswasmc_parameter_tag)3,bad,sizeof bad,&value)==22&&value.integer_bits==77);
}
{ /* Negative: Every Float64 array element must be finite. */
unsigned char bad[]={1,0,0,0,0,0,0,0,0,0,248,127};
roswasmc_parameter_value value={0};value.integer_bits=77;assert(roswasmc_decode_parameter((roswasmc_parameter_tag)8,bad,sizeof bad,&value)==22&&value.integer_bits==77);
}
{ /* Negative: Every Bool array element must be 0 or 1. */
unsigned char bad[]={1,0,0,0,2};
roswasmc_parameter_value value={0};value.integer_bits=77;assert(roswasmc_decode_parameter((roswasmc_parameter_tag)6,bad,sizeof bad,&value)==22&&value.integer_bits==77);
}
{ /* Negative: String-array elements are NUL-free. */
unsigned char bad[]={1,0,0,0,3,0,0,0,97,0,98};
roswasmc_parameter_value value={0};value.integer_bits=77;assert(roswasmc_decode_parameter((roswasmc_parameter_tag)9,bad,sizeof bad,&value)==22&&value.integer_bits==77);
}
{ /* Negative: An owned Goal ID is nonzero. */
unsigned char bad[]={0,0,0,0,0,0,0,0,2,0,0,0,0};
roswasmc_action_sequence value={0};value.goal=77;assert(roswasmc_decode_action_sequence(bad,sizeof bad,1,&value)==22&&value.goal==77);
}
{ /* Negative: Deletion-only tag 0 requires deleted kind 3. */
unsigned char bad[]={1,0,0,0,2,0,0,0,1,2,0,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};
roswasmc_typed_parameter_event value={0};value.seconds=77;assert(roswasmc_decode_typed_parameter_event(bad,sizeof bad,&value)==22&&value.seconds==77);
}
{ /* Negative: Deleted events have an empty selected payload. */
unsigned char bad[]={1,0,0,0,2,0,0,0,3,2,1,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,1,0,0,0,1};
roswasmc_typed_parameter_event value={0};value.seconds=77;assert(roswasmc_decode_typed_parameter_event(bad,sizeof bad,&value)==22&&value.seconds==77);
}
return 0;
}
