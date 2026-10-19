/** @file Generated ROS2 bounded-v2 native value conversion.
 * Includes only concrete officially generated message/service/action headers.
 * Native identities and complete metadata must be qualified by the Gateway before
 * any entity creation. valid() allocates nothing; vector encode allocates only
 * after complete validation/measurement. Native decode builds a bounded temporary
 * and commits once. CDR/TCPROS bytes are never forwarded as untyped guest data. */
#pragma once
#include "bounded_records.h"
#include <vector>
#include <array>
#include <string>
#include <type_traits>
#include <utility>
#include <uwvm2/imported/ros/ipc/custom_type.h>
#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <uwvm_ros_bounded_v2/action/digest.hpp>
#include <uwvm_ros_bounded_v2/msg/envelope.hpp>
#include <uwvm_ros_bounded_v2/msg/point.hpp>
#include <uwvm_ros_bounded_v2/srv/echo.hpp>
#include <uwvm_ros_bounded_v2/srv/empty.hpp>
#ifndef UWVM_CUSTOM_BOUNDED_NATIVE_TRAITS
#define UWVM_CUSTOM_BOUNDED_NATIVE_TRAITS
namespace uwvm_custom_bounded_native {
/** Normalized complete ROS1 declaration subsection; never a native hash algorithm. */
struct definition_entry {std::string_view type,fields;};
template<class T> struct fixed_shape {using element=void;static constexpr size_t size=0;};
template<class T,size_t N> struct fixed_shape<std::array<T,N>> {using element=T;static constexpr size_t size=N;};
template<class T> struct sequence_shape {using element=void;static constexpr bool supported=false;static constexpr size_t bound=0;};
template<class T,class A> struct sequence_shape<std::vector<T,A>> {using element=T;static constexpr bool supported=true;static constexpr size_t bound=0;};
template<class T> struct string_shape {static constexpr bool supported=false;};
template<class A> struct string_shape<std::basic_string<char,std::char_traits<char>,A>> {static constexpr bool supported=true;};
}
#endif
#ifndef UWVM_CUSTOM_BOUNDED_ROSIDL_TRAITS
#define UWVM_CUSTOM_BOUNDED_ROSIDL_TRAITS
namespace uwvm_custom_bounded_native {
template<class T,size_t N,class A> struct sequence_shape<rosidl_runtime_cpp::BoundedVector<T,N,A>> {using element=T;static constexpr bool supported=true;static constexpr size_t bound=N;};
}
#endif
namespace uwvm_custom_bounded_native {
/** Complete official uwvm_ros_bounded_v2/msg/Point converter, including all nested dependencies. */
struct Ros2Uwvm_ros_bounded_v2MsgPointCodec {
using Native=::uwvm_ros_bounded_v2::msg::Point;using Record=roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point;
static constexpr char const* type="uwvm_ros_bounded_v2/msg/Point";
static constexpr char const* native_identity="RIHS01_7f58e859cebde43421f1dc78d6f7c71effd3adad3bd3620f64c48405a2f1cc91";
static constexpr char const* codec_identity="UWVMROS1_60309848e4857d663e4b90a42790335a74352469d0d30bc3bf33083b529fb333";
static constexpr size_t wire_bytes=5;static constexpr bool bounded_profile=true;
using raw_0=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().value)>>;
using field_0=raw_0;
static_assert(std::is_same<field_0,int32_t>::value,"official scalar/nested field type differs");
using raw_1=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().enabled)>>;
using field_1=raw_1;
static_assert(std::is_same<field_1,bool>::value,"official scalar/nested field type differs");
/** Validate native fields without copying and commit the exact active canonical size. */
static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;
{
n+=4;
}
{
n+=1;
}
if(n>5)return false;out=n;return true;}
/** Copy a fully bounded native projection into a private C value, never into wire output. */
static bool to_record(Native const& value,Record& record) noexcept {
record.value=value.value;
record.enabled=value.enabled;
return roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_valid(&record);
}
/** Populate only a fresh native temporary after complete canonical decoding. */
static bool from_record(Record const& record,Native& value) {
value.value=record.value;
value.enabled=record.enabled;
return true;}
static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}
static bool validate(Native const& value) noexcept {return valid(value);}
/** This pointer overload succeeds only when the capacity is the exact active wire length. */
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/msg/Point","int32 value\nbool enabled\n"},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={{"value","int32",0,false,0,false,0},{"enabled","bool",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/msg/Point",schema_fields_0,2},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
/** Complete official uwvm_ros_bounded_v2/msg/Envelope converter, including all nested dependencies. */
struct Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec {
using Native=::uwvm_ros_bounded_v2::msg::Envelope;using Record=roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope;
static constexpr char const* type="uwvm_ros_bounded_v2/msg/Envelope";
static constexpr char const* native_identity="RIHS01_db84bfb7426f4f7bbd57581e2ec9774e6d69693f18194f18138d2bb435b384ca";
static constexpr char const* codec_identity="UWVMROS1_0a1a27d41a3aa4946fd3e118be4a76e32b496b4aff9206ba7c45f69f85374c36";
static constexpr size_t wire_bytes=120;static constexpr bool bounded_profile=true;
using raw_0=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().label)>>;
using field_0=raw_0;
static_assert(string_shape<field_0>::supported,"official field must be an 8-bit allocator-aware string");
using raw_1=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().samples)>>;
static_assert(sequence_shape<raw_1>::supported&&sequence_shape<raw_1>::bound==8,"official sequence shape/bound differs");
using field_1=typename sequence_shape<raw_1>::element;
static_assert(std::is_same<field_1,uint16_t>::value,"official scalar/nested field type differs");
using raw_2=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().points)>>;
static_assert(sequence_shape<raw_2>::supported&&sequence_shape<raw_2>::bound==4,"official sequence shape/bound differs");
using field_2=typename sequence_shape<raw_2>::element;
static_assert(std::is_same<field_2,::uwvm_ros_bounded_v2::msg::Point>::value,"official scalar/nested field type differs");
using raw_3=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().labels)>>;
static_assert(fixed_shape<raw_3>::size==2,"official fixed array length differs");
using field_3=typename fixed_shape<raw_3>::element;
static_assert(string_shape<field_3>::supported,"official field must be an 8-bit allocator-aware string");
/** Validate native fields without copying and commit the exact active canonical size. */
static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;
{
if(value.label.size()>32||!uwvm_bounded_utf8(reinterpret_cast<unsigned char const*>(value.label.data()),value.label.size()))return false;n+=4+value.label.size();
}
{
if(value.samples.size()>8)return false;n+=4;
for(size_t i=0;i<value.samples.size();++i){
n+=2;
}
}
{
if(value.points.size()>4)return false;n+=4;
for(size_t i=0;i<value.points.size();++i){
size_t nested=0;if(!Ros2Uwvm_ros_bounded_v2MsgPointCodec::encoded_size(value.points[i],nested))return false;n+=nested;
}
}
{
for(size_t i=0;i<2;++i){
if(value.labels[i].size()>16||!uwvm_bounded_utf8(reinterpret_cast<unsigned char const*>(value.labels[i].data()),value.labels[i].size()))return false;n+=4+value.labels[i].size();
}
}
if(n>120)return false;out=n;return true;}
/** Copy a fully bounded native projection into a private C value, never into wire output. */
static bool to_record(Native const& value,Record& record) noexcept {
if(value.label.size()>32||!uwvm_bounded_utf8(reinterpret_cast<unsigned char const*>(value.label.data()),value.label.size()))return false;
record.label.size=static_cast<uint32_t>(value.label.size());uwvm_bounded_copy(record.label.data,value.label.data(),value.label.size());
if(value.samples.size()>8)return false;record.samples.size=static_cast<uint32_t>(value.samples.size());
for(size_t i=0;i<value.samples.size();++i){
record.samples.data[i]=value.samples[i];
}
if(value.points.size()>4)return false;record.points.size=static_cast<uint32_t>(value.points.size());
for(size_t i=0;i<value.points.size();++i){
if(!Ros2Uwvm_ros_bounded_v2MsgPointCodec::to_record(value.points[i],record.points.data[i]))return false;
}
for(size_t i=0;i<2;++i){
if(value.labels[i].size()>16||!uwvm_bounded_utf8(reinterpret_cast<unsigned char const*>(value.labels[i].data()),value.labels[i].size()))return false;
record.labels[i].size=static_cast<uint32_t>(value.labels[i].size());uwvm_bounded_copy(record.labels[i].data,value.labels[i].data(),value.labels[i].size());
}
return roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_valid(&record);
}
/** Populate only a fresh native temporary after complete canonical decoding. */
static bool from_record(Record const& record,Native& value) {
value.label.assign(reinterpret_cast<char const*>(record.label.data),record.label.size);
value.samples.resize(record.samples.size);
for(size_t i=0;i<record.samples.size;++i){
value.samples[i]=record.samples.data[i];
}
value.points.resize(record.points.size);
for(size_t i=0;i<record.points.size;++i){
if(!Ros2Uwvm_ros_bounded_v2MsgPointCodec::from_record(record.points.data[i],value.points[i]))return false;
}
for(size_t i=0;i<2;++i){
value.labels[i].assign(reinterpret_cast<char const*>(record.labels[i].data),record.labels[i].size);
}
return true;}
static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}
static bool validate(Native const& value) noexcept {return valid(value);}
/** This pointer overload succeeds only when the capacity is the exact active wire length. */
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/msg/Envelope","string label\nuint16[] samples\nuwvm_ros_bounded_v2/msg/Point[] points\nstring[2] labels\n"},
{"uwvm_ros_bounded_v2/msg/Point","int32 value\nbool enabled\n"},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={{"label","string",0,false,32,false,0},{"samples","uint16",0,false,0,true,8},{"points","uwvm_ros_bounded_v2/msg/Point",0,false,0,true,4},{"labels","string",2,true,16,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_1[]={{"value","int32",0,false,0,false,0},{"enabled","bool",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/msg/Envelope",schema_fields_0,4},
{"uwvm_ros_bounded_v2/msg/Point",schema_fields_1,2},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
/** Complete official uwvm_ros_bounded_v2/srv/Echo_Request converter, including all nested dependencies. */
struct Ros2Uwvm_ros_bounded_v2SrvEcho_RequestCodec {
using Native=::uwvm_ros_bounded_v2::srv::Echo::Request;using Record=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request;
static constexpr char const* type="uwvm_ros_bounded_v2/srv/Echo_Request";
static constexpr char const* native_identity="RIHS01_5a1e070c6f3fc429af33a615c113758249b109679742936c419a778425823087";
static constexpr char const* codec_identity="UWVMROS1_dd30d845fcb865e94b268c77fc18bc7906d226a4ca3274224cc6e202e69b93f3";
static constexpr size_t wire_bytes=124;static constexpr bool bounded_profile=true;
using raw_0=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().sample)>>;
using field_0=raw_0;
static_assert(std::is_same<field_0,::uwvm_ros_bounded_v2::msg::Envelope>::value,"official scalar/nested field type differs");
using raw_1=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().limit)>>;
using field_1=raw_1;
static_assert(std::is_same<field_1,uint32_t>::value,"official scalar/nested field type differs");
/** Validate native fields without copying and commit the exact active canonical size. */
static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;
{
size_t nested=0;if(!Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec::encoded_size(value.sample,nested))return false;n+=nested;
}
{
n+=4;
}
if(n>124)return false;out=n;return true;}
/** Copy a fully bounded native projection into a private C value, never into wire output. */
static bool to_record(Native const& value,Record& record) noexcept {
if(!Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec::to_record(value.sample,record.sample))return false;
record.limit=value.limit;
return roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_valid(&record);
}
/** Populate only a fresh native temporary after complete canonical decoding. */
static bool from_record(Record const& record,Native& value) {
if(!Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec::from_record(record.sample,value.sample))return false;
value.limit=record.limit;
return true;}
static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}
static bool validate(Native const& value) noexcept {return valid(value);}
/** This pointer overload succeeds only when the capacity is the exact active wire length. */
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/srv/Echo_Request","uwvm_ros_bounded_v2/msg/Envelope sample\nuint32 limit\n"},
{"uwvm_ros_bounded_v2/msg/Envelope","string label\nuint16[] samples\nuwvm_ros_bounded_v2/msg/Point[] points\nstring[2] labels\n"},
{"uwvm_ros_bounded_v2/msg/Point","int32 value\nbool enabled\n"},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={{"sample","uwvm_ros_bounded_v2/msg/Envelope",0,false,0,false,0},{"limit","uint32",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_1[]={{"label","string",0,false,32,false,0},{"samples","uint16",0,false,0,true,8},{"points","uwvm_ros_bounded_v2/msg/Point",0,false,0,true,4},{"labels","string",2,true,16,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_2[]={{"value","int32",0,false,0,false,0},{"enabled","bool",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/srv/Echo_Request",schema_fields_0,2},
{"uwvm_ros_bounded_v2/msg/Envelope",schema_fields_1,4},
{"uwvm_ros_bounded_v2/msg/Point",schema_fields_2,2},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
/** Complete official uwvm_ros_bounded_v2/srv/Echo_Response converter, including all nested dependencies. */
struct Ros2Uwvm_ros_bounded_v2SrvEcho_ResponseCodec {
using Native=::uwvm_ros_bounded_v2::srv::Echo::Response;using Record=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response;
static constexpr char const* type="uwvm_ros_bounded_v2/srv/Echo_Response";
static constexpr char const* native_identity="RIHS01_0e6b9146373ac0ba493a1e779e6b56f2d9e44104a4157b99f2592e73939bc12f";
static constexpr char const* codec_identity="UWVMROS1_995cb48a672a99327ed71ffb5af6d7518e11aae2f4068e17801b38799a52e5e5";
static constexpr size_t wire_bytes=121;static constexpr bool bounded_profile=true;
using raw_0=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().accepted)>>;
using field_0=raw_0;
static_assert(std::is_same<field_0,bool>::value,"official scalar/nested field type differs");
using raw_1=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().echoed)>>;
using field_1=raw_1;
static_assert(std::is_same<field_1,::uwvm_ros_bounded_v2::msg::Envelope>::value,"official scalar/nested field type differs");
/** Validate native fields without copying and commit the exact active canonical size. */
static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;
{
n+=1;
}
{
size_t nested=0;if(!Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec::encoded_size(value.echoed,nested))return false;n+=nested;
}
if(n>121)return false;out=n;return true;}
/** Copy a fully bounded native projection into a private C value, never into wire output. */
static bool to_record(Native const& value,Record& record) noexcept {
record.accepted=value.accepted;
if(!Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec::to_record(value.echoed,record.echoed))return false;
return roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_valid(&record);
}
/** Populate only a fresh native temporary after complete canonical decoding. */
static bool from_record(Record const& record,Native& value) {
value.accepted=record.accepted;
if(!Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec::from_record(record.echoed,value.echoed))return false;
return true;}
static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}
static bool validate(Native const& value) noexcept {return valid(value);}
/** This pointer overload succeeds only when the capacity is the exact active wire length. */
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/srv/Echo_Response","bool accepted\nuwvm_ros_bounded_v2/msg/Envelope echoed\n"},
{"uwvm_ros_bounded_v2/msg/Envelope","string label\nuint16[] samples\nuwvm_ros_bounded_v2/msg/Point[] points\nstring[2] labels\n"},
{"uwvm_ros_bounded_v2/msg/Point","int32 value\nbool enabled\n"},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={{"accepted","bool",0,false,0,false,0},{"echoed","uwvm_ros_bounded_v2/msg/Envelope",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_1[]={{"label","string",0,false,32,false,0},{"samples","uint16",0,false,0,true,8},{"points","uwvm_ros_bounded_v2/msg/Point",0,false,0,true,4},{"labels","string",2,true,16,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_2[]={{"value","int32",0,false,0,false,0},{"enabled","bool",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/srv/Echo_Response",schema_fields_0,2},
{"uwvm_ros_bounded_v2/msg/Envelope",schema_fields_1,4},
{"uwvm_ros_bounded_v2/msg/Point",schema_fields_2,2},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
/** Complete official uwvm_ros_bounded_v2/srv/Empty_Request converter, including all nested dependencies. */
struct Ros2Uwvm_ros_bounded_v2SrvEmpty_RequestCodec {
using Native=::uwvm_ros_bounded_v2::srv::Empty::Request;using Record=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request;
static constexpr char const* type="uwvm_ros_bounded_v2/srv/Empty_Request";
static constexpr char const* native_identity="RIHS01_0237d4d58b26c8beed49f27147aee6b7f8c23ee700bb872ff665703a0c387232";
static constexpr char const* codec_identity="UWVMROS1_60e31c2bafbb5c26e57de70bd2f26cea80c8f1d2299ed064aa6d74f51b25a8f4";
static constexpr size_t wire_bytes=0;static constexpr bool bounded_profile=true;
static_assert(std::is_same<std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().structure_needs_at_least_one_member)>>,uint8_t>::value,"official empty component placeholder differs");
/** Validate native fields without copying and commit the exact active canonical size. */
static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;
if(value.structure_needs_at_least_one_member!=0)return false;
if(n>0)return false;out=n;return true;}
/** Copy a fully bounded native projection into a private C value, never into wire output. */
static bool to_record(Native const& value,Record& record) noexcept {
if(value.structure_needs_at_least_one_member!=0)return false;
return roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_valid(&record);
}
/** Populate only a fresh native temporary after complete canonical decoding. */
static bool from_record(Record const& record,Native& value) {
value.structure_needs_at_least_one_member=0;
return true;}
static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}
static bool validate(Native const& value) noexcept {return valid(value);}
/** This pointer overload succeeds only when the capacity is the exact active wire length. */
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/srv/Empty_Request",""},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={{"structure_needs_at_least_one_member","uint8",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/srv/Empty_Request",schema_fields_0,1},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
/** Complete official uwvm_ros_bounded_v2/srv/Empty_Response converter, including all nested dependencies. */
struct Ros2Uwvm_ros_bounded_v2SrvEmpty_ResponseCodec {
using Native=::uwvm_ros_bounded_v2::srv::Empty::Response;using Record=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response;
static constexpr char const* type="uwvm_ros_bounded_v2/srv/Empty_Response";
static constexpr char const* native_identity="RIHS01_7a12e06d8035fdd26321858cb66f62c1cac5c69c49a57848bc8c6b7da29d3ca6";
static constexpr char const* codec_identity="UWVMROS1_159ef50f81d76904657508db066fc6ffdb7229581aaeb5e2d425cd9b1e31cf2b";
static constexpr size_t wire_bytes=0;static constexpr bool bounded_profile=true;
static_assert(std::is_same<std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().structure_needs_at_least_one_member)>>,uint8_t>::value,"official empty component placeholder differs");
/** Validate native fields without copying and commit the exact active canonical size. */
static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;
if(value.structure_needs_at_least_one_member!=0)return false;
if(n>0)return false;out=n;return true;}
/** Copy a fully bounded native projection into a private C value, never into wire output. */
static bool to_record(Native const& value,Record& record) noexcept {
if(value.structure_needs_at_least_one_member!=0)return false;
return roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_valid(&record);
}
/** Populate only a fresh native temporary after complete canonical decoding. */
static bool from_record(Record const& record,Native& value) {
value.structure_needs_at_least_one_member=0;
return true;}
static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}
static bool validate(Native const& value) noexcept {return valid(value);}
/** This pointer overload succeeds only when the capacity is the exact active wire length. */
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/srv/Empty_Response",""},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={{"structure_needs_at_least_one_member","uint8",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/srv/Empty_Response",schema_fields_0,1},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
/** Complete official uwvm_ros_bounded_v2/action/Digest_Goal converter, including all nested dependencies. */
struct Ros2Uwvm_ros_bounded_v2ActionDigest_GoalCodec {
using Native=::uwvm_ros_bounded_v2::action::Digest::Goal;using Record=roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal;
static constexpr char const* type="uwvm_ros_bounded_v2/action/Digest_Goal";
static constexpr char const* native_identity="RIHS01_b9c606f67c29d3578fe594dac6423a4e2aa14ac2c469ec3552b82bb23368b699";
static constexpr char const* codec_identity="UWVMROS1_b853f639f7516b2fd475fdd1e238f182f34423a38f7e3fa0430dc952f76bb58c";
static constexpr size_t wire_bytes=120;static constexpr bool bounded_profile=true;
using raw_0=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().sample)>>;
using field_0=raw_0;
static_assert(std::is_same<field_0,::uwvm_ros_bounded_v2::msg::Envelope>::value,"official scalar/nested field type differs");
/** Validate native fields without copying and commit the exact active canonical size. */
static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;
{
size_t nested=0;if(!Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec::encoded_size(value.sample,nested))return false;n+=nested;
}
if(n>120)return false;out=n;return true;}
/** Copy a fully bounded native projection into a private C value, never into wire output. */
static bool to_record(Native const& value,Record& record) noexcept {
if(!Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec::to_record(value.sample,record.sample))return false;
return roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_valid(&record);
}
/** Populate only a fresh native temporary after complete canonical decoding. */
static bool from_record(Record const& record,Native& value) {
if(!Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec::from_record(record.sample,value.sample))return false;
return true;}
static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}
static bool validate(Native const& value) noexcept {return valid(value);}
/** This pointer overload succeeds only when the capacity is the exact active wire length. */
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/action/Digest_Goal","uwvm_ros_bounded_v2/msg/Envelope sample\n"},
{"uwvm_ros_bounded_v2/msg/Envelope","string label\nuint16[] samples\nuwvm_ros_bounded_v2/msg/Point[] points\nstring[2] labels\n"},
{"uwvm_ros_bounded_v2/msg/Point","int32 value\nbool enabled\n"},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={{"sample","uwvm_ros_bounded_v2/msg/Envelope",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_1[]={{"label","string",0,false,32,false,0},{"samples","uint16",0,false,0,true,8},{"points","uwvm_ros_bounded_v2/msg/Point",0,false,0,true,4},{"labels","string",2,true,16,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_2[]={{"value","int32",0,false,0,false,0},{"enabled","bool",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/action/Digest_Goal",schema_fields_0,1},
{"uwvm_ros_bounded_v2/msg/Envelope",schema_fields_1,4},
{"uwvm_ros_bounded_v2/msg/Point",schema_fields_2,2},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
/** Complete official uwvm_ros_bounded_v2/action/Digest_Result converter, including all nested dependencies. */
struct Ros2Uwvm_ros_bounded_v2ActionDigest_ResultCodec {
using Native=::uwvm_ros_bounded_v2::action::Digest::Result;using Record=roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result;
static constexpr char const* type="uwvm_ros_bounded_v2/action/Digest_Result";
static constexpr char const* native_identity="RIHS01_61d9dc6f97d23f339408e3a36ffab93c446689b7552d5b7f732caa4e65b6dac3";
static constexpr char const* codec_identity="UWVMROS1_9a80357b1963e18b15441487bc1ca7dc876faa47c4a97df377dbc4f98cacd3da";
static constexpr size_t wire_bytes=41;static constexpr bool bounded_profile=true;
using raw_0=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().accepted)>>;
using field_0=raw_0;
static_assert(std::is_same<field_0,bool>::value,"official scalar/nested field type differs");
using raw_1=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().sum)>>;
using field_1=raw_1;
static_assert(std::is_same<field_1,uint32_t>::value,"official scalar/nested field type differs");
using raw_2=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().note)>>;
using field_2=raw_2;
static_assert(string_shape<field_2>::supported,"official field must be an 8-bit allocator-aware string");
/** Validate native fields without copying and commit the exact active canonical size. */
static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;
{
n+=1;
}
{
n+=4;
}
{
if(value.note.size()>32||!uwvm_bounded_utf8(reinterpret_cast<unsigned char const*>(value.note.data()),value.note.size()))return false;n+=4+value.note.size();
}
if(n>41)return false;out=n;return true;}
/** Copy a fully bounded native projection into a private C value, never into wire output. */
static bool to_record(Native const& value,Record& record) noexcept {
record.accepted=value.accepted;
record.sum=value.sum;
if(value.note.size()>32||!uwvm_bounded_utf8(reinterpret_cast<unsigned char const*>(value.note.data()),value.note.size()))return false;
record.note.size=static_cast<uint32_t>(value.note.size());uwvm_bounded_copy(record.note.data,value.note.data(),value.note.size());
return roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_valid(&record);
}
/** Populate only a fresh native temporary after complete canonical decoding. */
static bool from_record(Record const& record,Native& value) {
value.accepted=record.accepted;
value.sum=record.sum;
value.note.assign(reinterpret_cast<char const*>(record.note.data),record.note.size);
return true;}
static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}
static bool validate(Native const& value) noexcept {return valid(value);}
/** This pointer overload succeeds only when the capacity is the exact active wire length. */
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/action/Digest_Result","bool accepted\nuint32 sum\nstring note\n"},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={{"accepted","bool",0,false,0,false,0},{"sum","uint32",0,false,0,false,0},{"note","string",0,false,32,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/action/Digest_Result",schema_fields_0,3},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
/** Complete official uwvm_ros_bounded_v2/action/Digest_Feedback converter, including all nested dependencies. */
struct Ros2Uwvm_ros_bounded_v2ActionDigest_FeedbackCodec {
using Native=::uwvm_ros_bounded_v2::action::Digest::Feedback;using Record=roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback;
static constexpr char const* type="uwvm_ros_bounded_v2/action/Digest_Feedback";
static constexpr char const* native_identity="RIHS01_ab462fc88a43bac4c8e109915bc58a5e9cfbeaf42255e7778dbd682154a2be2c";
static constexpr char const* codec_identity="UWVMROS1_940051bd38fe0ab7f0dc08355ff3da10782ee3d9705adb60ac218ea8f82180c1";
static constexpr size_t wire_bytes=24;static constexpr bool bounded_profile=true;
using raw_0=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().processed)>>;
using field_0=raw_0;
static_assert(std::is_same<field_0,uint32_t>::value,"official scalar/nested field type differs");
using raw_1=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().phase)>>;
using field_1=raw_1;
static_assert(string_shape<field_1>::supported,"official field must be an 8-bit allocator-aware string");
/** Validate native fields without copying and commit the exact active canonical size. */
static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;
{
n+=4;
}
{
if(value.phase.size()>16||!uwvm_bounded_utf8(reinterpret_cast<unsigned char const*>(value.phase.data()),value.phase.size()))return false;n+=4+value.phase.size();
}
if(n>24)return false;out=n;return true;}
/** Copy a fully bounded native projection into a private C value, never into wire output. */
static bool to_record(Native const& value,Record& record) noexcept {
record.processed=value.processed;
if(value.phase.size()>16||!uwvm_bounded_utf8(reinterpret_cast<unsigned char const*>(value.phase.data()),value.phase.size()))return false;
record.phase.size=static_cast<uint32_t>(value.phase.size());uwvm_bounded_copy(record.phase.data,value.phase.data(),value.phase.size());
return roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_valid(&record);
}
/** Populate only a fresh native temporary after complete canonical decoding. */
static bool from_record(Record const& record,Native& value) {
value.processed=record.processed;
value.phase.assign(reinterpret_cast<char const*>(record.phase.data),record.phase.size);
return true;}
static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}
static bool validate(Native const& value) noexcept {return valid(value);}
/** This pointer overload succeeds only when the capacity is the exact active wire length. */
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/action/Digest_Feedback","uint32 processed\nstring phase\n"},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={{"processed","uint32",0,false,0,false,0},{"phase","string",0,false,16,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/action/Digest_Feedback",schema_fields_0,2},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
}
#define UWVM_ROS_CUSTOM_ROS2_TYPES(M) \
  M(::uwvm_ros_bounded_v2::msg::Point,"uwvm_ros_bounded_v2/msg/Point","RIHS01_7f58e859cebde43421f1dc78d6f7c71effd3adad3bd3620f64c48405a2f1cc91","UWVMROS1_60309848e4857d663e4b90a42790335a74352469d0d30bc3bf33083b529fb333",5,::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2MsgPointCodec) \
  M(::uwvm_ros_bounded_v2::msg::Envelope,"uwvm_ros_bounded_v2/msg/Envelope","RIHS01_db84bfb7426f4f7bbd57581e2ec9774e6d69693f18194f18138d2bb435b384ca","UWVMROS1_0a1a27d41a3aa4946fd3e118be4a76e32b496b4aff9206ba7c45f69f85374c36",120,::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec)
#define UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(M) \
  M(::uwvm_ros_bounded_v2::srv::Echo,"uwvm_ros_bounded_v2/srv/Echo","RIHS01_b6a5373556a7278835d767fffd496e993656833bf4c72d008e10f88669e7ff46","UWVMROS1_f8a4ed1cffd983c44306305ca4a3d4836d05d182fd3e42bdb15e8ca8043be2f4",::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2SrvEcho_RequestCodec,::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2SrvEcho_ResponseCodec) \
  M(::uwvm_ros_bounded_v2::srv::Empty,"uwvm_ros_bounded_v2/srv/Empty","RIHS01_a0f5e07741124efbd845dab80bdb5cc355cabe501c2718b51d2377a3cfa6527b","UWVMROS1_958622d138e27210d360895e1d132afb01e3050ca2da9ff6cf5652100ced504a",::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2SrvEmpty_RequestCodec,::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2SrvEmpty_ResponseCodec)
#define UWVM_ROS_CUSTOM_ROS2_ACTION_TYPES(M) \
  M(::uwvm_ros_bounded_v2::action::Digest,"uwvm_ros_bounded_v2/action/Digest","RIHS01_2b3b1c551cb6a47adad62f07b8b1e56ed08509eccb07a3515e23db1730647a86","UWVMROS1_a4a829b06fdfc8a0cab20854f410c3366c85b9b8113fe8e80e459bf08a43c3a5",::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2ActionDigest_GoalCodec,::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2ActionDigest_ResultCodec,::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2ActionDigest_FeedbackCodec)
#define UWVM_ROS_CUSTOM_ROS2_TOPIC_TYPES(M) UWVM_ROS_CUSTOM_ROS2_TYPES(M)
