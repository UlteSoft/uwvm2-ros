/** @file Generated ROS1 bounded-v2 native value conversion.
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
#include <boost/array.hpp>
#include <ros/message_traits.h>
#include <ros/service_traits.h>
#include <uwvm_ros_bounded_v2/DigestAction.h>
#include <uwvm_ros_bounded_v2/Echo.h>
#include <uwvm_ros_bounded_v2/Empty.h>
#include <uwvm_ros_bounded_v2/Envelope.h>
#include <uwvm_ros_bounded_v2/Point.h>
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
#ifndef UWVM_CUSTOM_BOUNDED_BOOST_TRAITS
#define UWVM_CUSTOM_BOUNDED_BOOST_TRAITS
namespace uwvm_custom_bounded_native {
template<class T,size_t N> struct fixed_shape<boost::array<T,N>> {using element=T;static constexpr size_t size=N;};
}
#endif
namespace uwvm_custom_bounded_native {
/** Complete official uwvm_ros_bounded_v2/Point converter, including all nested dependencies. */
struct Ros1Uwvm_ros_bounded_v2PointCodec {
using Native=::uwvm_ros_bounded_v2::Point;using Record=roswasmc_bounded_1_uwvm_ros_bounded_v2_Point;
static constexpr char const* type="uwvm_ros_bounded_v2/Point";
static constexpr char const* native_identity="d01b93b38572826dbde8043bc3a87849";
static constexpr char const* codec_identity="UWVMROS1_adb4b0b058e3491e31d00d4c0c5914bdeeabe7ba8ce7a4de798ac358c8fecf28";
static constexpr size_t wire_bytes=5;static constexpr bool bounded_profile=true;
using raw_0=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().value)>>;
using field_0=raw_0;
static_assert(std::is_same<field_0,int32_t>::value,"official scalar/nested field type differs");
using raw_1=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().enabled)>>;
using field_1=raw_1;
static_assert(std::is_same<field_1,uint8_t>::value,"official scalar/nested field type differs");
/** Validate native fields without copying and commit the exact active canonical size. */
static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;
{
n+=4;
}
{
if(value.enabled!=0&&value.enabled!=1)return false;
n+=1;
}
if(n>5)return false;out=n;return true;}
/** Copy a fully bounded native projection into a private C value, never into wire output. */
static bool to_record(Native const& value,Record& record) noexcept {
record.value=value.value;
if(value.enabled!=0&&value.enabled!=1)return false;
record.enabled=value.enabled;
return roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_valid(&record);
}
/** Populate only a fresh native temporary after complete canonical decoding. */
static bool from_record(Record const& record,Native& value) {
value.value=record.value;
value.enabled=record.enabled;
return true;}
static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}
static bool validate(Native const& value) noexcept {return valid(value);}
/** This pointer overload succeeds only when the capacity is the exact active wire length. */
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/Point","int32 value\nbool enabled\n"},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={{"value","int32",0,false,0,false,0},{"enabled","bool",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/Point",schema_fields_0,2},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
/** Complete official uwvm_ros_bounded_v2/Envelope converter, including all nested dependencies. */
struct Ros1Uwvm_ros_bounded_v2EnvelopeCodec {
using Native=::uwvm_ros_bounded_v2::Envelope;using Record=roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope;
static constexpr char const* type="uwvm_ros_bounded_v2/Envelope";
static constexpr char const* native_identity="76b87d274fb061da1933bd67ff4757f6";
static constexpr char const* codec_identity="UWVMROS1_18a098b82136b0c52c84df1c74e3dba78c63a877feabb3b14a750cbb2d7e3da4";
static constexpr size_t wire_bytes=120;static constexpr bool bounded_profile=true;
using raw_0=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().label)>>;
using field_0=raw_0;
static_assert(string_shape<field_0>::supported,"official field must be an 8-bit allocator-aware string");
using raw_1=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().samples)>>;
static_assert(sequence_shape<raw_1>::supported&&sequence_shape<raw_1>::bound==0,"official sequence shape/bound differs");
using field_1=typename sequence_shape<raw_1>::element;
static_assert(std::is_same<field_1,uint16_t>::value,"official scalar/nested field type differs");
using raw_2=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().points)>>;
static_assert(sequence_shape<raw_2>::supported&&sequence_shape<raw_2>::bound==0,"official sequence shape/bound differs");
using field_2=typename sequence_shape<raw_2>::element;
static_assert(std::is_same<field_2,::uwvm_ros_bounded_v2::Point>::value,"official scalar/nested field type differs");
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
size_t nested=0;if(!Ros1Uwvm_ros_bounded_v2PointCodec::encoded_size(value.points[i],nested))return false;n+=nested;
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
if(!Ros1Uwvm_ros_bounded_v2PointCodec::to_record(value.points[i],record.points.data[i]))return false;
}
for(size_t i=0;i<2;++i){
if(value.labels[i].size()>16||!uwvm_bounded_utf8(reinterpret_cast<unsigned char const*>(value.labels[i].data()),value.labels[i].size()))return false;
record.labels[i].size=static_cast<uint32_t>(value.labels[i].size());uwvm_bounded_copy(record.labels[i].data,value.labels[i].data(),value.labels[i].size());
}
return roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_valid(&record);
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
if(!Ros1Uwvm_ros_bounded_v2PointCodec::from_record(record.points.data[i],value.points[i]))return false;
}
for(size_t i=0;i<2;++i){
value.labels[i].assign(reinterpret_cast<char const*>(record.labels[i].data),record.labels[i].size);
}
return true;}
static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}
static bool validate(Native const& value) noexcept {return valid(value);}
/** This pointer overload succeeds only when the capacity is the exact active wire length. */
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/Envelope","string label\nuint16[] samples\nuwvm_ros_bounded_v2/Point[] points\nstring[2] labels\n"},
{"uwvm_ros_bounded_v2/Point","int32 value\nbool enabled\n"},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={{"label","string",0,false,0,false,0},{"samples","uint16",0,false,0,true,0},{"points","uwvm_ros_bounded_v2/Point",0,false,0,true,0},{"labels","string",2,true,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_1[]={{"value","int32",0,false,0,false,0},{"enabled","bool",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/Envelope",schema_fields_0,4},
{"uwvm_ros_bounded_v2/Point",schema_fields_1,2},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
/** Complete official uwvm_ros_bounded_v2/EchoRequest converter, including all nested dependencies. */
struct Ros1Uwvm_ros_bounded_v2EchoRequestCodec {
using Native=::uwvm_ros_bounded_v2::Echo::Request;using Record=roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest;
static constexpr char const* type="uwvm_ros_bounded_v2/EchoRequest";
static constexpr char const* native_identity="4c849610dc717f5e0adb9b8210f4cd69";
static constexpr char const* codec_identity="UWVMROS1_fc0c655e5a6a780160b2953a6fba67e28745d60b46c80f55be5ffb8b82da4420";
static constexpr size_t wire_bytes=124;static constexpr bool bounded_profile=true;
using raw_0=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().sample)>>;
using field_0=raw_0;
static_assert(std::is_same<field_0,::uwvm_ros_bounded_v2::Envelope>::value,"official scalar/nested field type differs");
using raw_1=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().limit)>>;
using field_1=raw_1;
static_assert(std::is_same<field_1,uint32_t>::value,"official scalar/nested field type differs");
/** Validate native fields without copying and commit the exact active canonical size. */
static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;
{
size_t nested=0;if(!Ros1Uwvm_ros_bounded_v2EnvelopeCodec::encoded_size(value.sample,nested))return false;n+=nested;
}
{
n+=4;
}
if(n>124)return false;out=n;return true;}
/** Copy a fully bounded native projection into a private C value, never into wire output. */
static bool to_record(Native const& value,Record& record) noexcept {
if(!Ros1Uwvm_ros_bounded_v2EnvelopeCodec::to_record(value.sample,record.sample))return false;
record.limit=value.limit;
return roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_valid(&record);
}
/** Populate only a fresh native temporary after complete canonical decoding. */
static bool from_record(Record const& record,Native& value) {
if(!Ros1Uwvm_ros_bounded_v2EnvelopeCodec::from_record(record.sample,value.sample))return false;
value.limit=record.limit;
return true;}
static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}
static bool validate(Native const& value) noexcept {return valid(value);}
/** This pointer overload succeeds only when the capacity is the exact active wire length. */
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/EchoRequest","uwvm_ros_bounded_v2/Envelope sample\nuint32 limit\n"},
{"uwvm_ros_bounded_v2/Envelope","string label\nuint16[] samples\nuwvm_ros_bounded_v2/Point[] points\nstring[2] labels\n"},
{"uwvm_ros_bounded_v2/Point","int32 value\nbool enabled\n"},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={{"sample","uwvm_ros_bounded_v2/Envelope",0,false,0,false,0},{"limit","uint32",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_1[]={{"label","string",0,false,0,false,0},{"samples","uint16",0,false,0,true,0},{"points","uwvm_ros_bounded_v2/Point",0,false,0,true,0},{"labels","string",2,true,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_2[]={{"value","int32",0,false,0,false,0},{"enabled","bool",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/EchoRequest",schema_fields_0,2},
{"uwvm_ros_bounded_v2/Envelope",schema_fields_1,4},
{"uwvm_ros_bounded_v2/Point",schema_fields_2,2},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
/** Complete official uwvm_ros_bounded_v2/EchoResponse converter, including all nested dependencies. */
struct Ros1Uwvm_ros_bounded_v2EchoResponseCodec {
using Native=::uwvm_ros_bounded_v2::Echo::Response;using Record=roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse;
static constexpr char const* type="uwvm_ros_bounded_v2/EchoResponse";
static constexpr char const* native_identity="40c160c971220283eb5e33795de6b070";
static constexpr char const* codec_identity="UWVMROS1_b06920091695407488787551c23510de5a76cf0368bbed5ea82d833d36c8385b";
static constexpr size_t wire_bytes=121;static constexpr bool bounded_profile=true;
using raw_0=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().accepted)>>;
using field_0=raw_0;
static_assert(std::is_same<field_0,uint8_t>::value,"official scalar/nested field type differs");
using raw_1=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().echoed)>>;
using field_1=raw_1;
static_assert(std::is_same<field_1,::uwvm_ros_bounded_v2::Envelope>::value,"official scalar/nested field type differs");
/** Validate native fields without copying and commit the exact active canonical size. */
static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;
{
if(value.accepted!=0&&value.accepted!=1)return false;
n+=1;
}
{
size_t nested=0;if(!Ros1Uwvm_ros_bounded_v2EnvelopeCodec::encoded_size(value.echoed,nested))return false;n+=nested;
}
if(n>121)return false;out=n;return true;}
/** Copy a fully bounded native projection into a private C value, never into wire output. */
static bool to_record(Native const& value,Record& record) noexcept {
if(value.accepted!=0&&value.accepted!=1)return false;
record.accepted=value.accepted;
if(!Ros1Uwvm_ros_bounded_v2EnvelopeCodec::to_record(value.echoed,record.echoed))return false;
return roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_valid(&record);
}
/** Populate only a fresh native temporary after complete canonical decoding. */
static bool from_record(Record const& record,Native& value) {
value.accepted=record.accepted;
if(!Ros1Uwvm_ros_bounded_v2EnvelopeCodec::from_record(record.echoed,value.echoed))return false;
return true;}
static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}
static bool validate(Native const& value) noexcept {return valid(value);}
/** This pointer overload succeeds only when the capacity is the exact active wire length. */
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/EchoResponse","bool accepted\nuwvm_ros_bounded_v2/Envelope echoed\n"},
{"uwvm_ros_bounded_v2/Envelope","string label\nuint16[] samples\nuwvm_ros_bounded_v2/Point[] points\nstring[2] labels\n"},
{"uwvm_ros_bounded_v2/Point","int32 value\nbool enabled\n"},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={{"accepted","bool",0,false,0,false,0},{"echoed","uwvm_ros_bounded_v2/Envelope",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_1[]={{"label","string",0,false,0,false,0},{"samples","uint16",0,false,0,true,0},{"points","uwvm_ros_bounded_v2/Point",0,false,0,true,0},{"labels","string",2,true,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_2[]={{"value","int32",0,false,0,false,0},{"enabled","bool",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/EchoResponse",schema_fields_0,2},
{"uwvm_ros_bounded_v2/Envelope",schema_fields_1,4},
{"uwvm_ros_bounded_v2/Point",schema_fields_2,2},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
/** Complete official uwvm_ros_bounded_v2/EmptyRequest converter, including all nested dependencies. */
struct Ros1Uwvm_ros_bounded_v2EmptyRequestCodec {
using Native=::uwvm_ros_bounded_v2::Empty::Request;using Record=roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest;
static constexpr char const* type="uwvm_ros_bounded_v2/EmptyRequest";
static constexpr char const* native_identity="d41d8cd98f00b204e9800998ecf8427e";
static constexpr char const* codec_identity="UWVMROS1_3f00a65933d0d7b1bc1848910b3d2a503b9f2b8a9f09ef58a83fc27e534dcf40";
static constexpr size_t wire_bytes=0;static constexpr bool bounded_profile=true;
/** Validate native fields without copying and commit the exact active canonical size. */
static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;
if(n>0)return false;out=n;return true;}
/** Copy a fully bounded native projection into a private C value, never into wire output. */
static bool to_record(Native const& value,Record& record) noexcept {
return roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_valid(&record);
}
/** Populate only a fresh native temporary after complete canonical decoding. */
static bool from_record(Record const& record,Native& value) {
return true;}
static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}
static bool validate(Native const& value) noexcept {return valid(value);}
/** This pointer overload succeeds only when the capacity is the exact active wire length. */
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/EmptyRequest",""},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/EmptyRequest",nullptr,0},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
/** Complete official uwvm_ros_bounded_v2/EmptyResponse converter, including all nested dependencies. */
struct Ros1Uwvm_ros_bounded_v2EmptyResponseCodec {
using Native=::uwvm_ros_bounded_v2::Empty::Response;using Record=roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse;
static constexpr char const* type="uwvm_ros_bounded_v2/EmptyResponse";
static constexpr char const* native_identity="d41d8cd98f00b204e9800998ecf8427e";
static constexpr char const* codec_identity="UWVMROS1_9ddb085437fd7750f10f08121d4cfd64d83bd293553a6ed9090044ec0625a817";
static constexpr size_t wire_bytes=0;static constexpr bool bounded_profile=true;
/** Validate native fields without copying and commit the exact active canonical size. */
static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;
if(n>0)return false;out=n;return true;}
/** Copy a fully bounded native projection into a private C value, never into wire output. */
static bool to_record(Native const& value,Record& record) noexcept {
return roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_valid(&record);
}
/** Populate only a fresh native temporary after complete canonical decoding. */
static bool from_record(Record const& record,Native& value) {
return true;}
static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}
static bool validate(Native const& value) noexcept {return valid(value);}
/** This pointer overload succeeds only when the capacity is the exact active wire length. */
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/EmptyResponse",""},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/EmptyResponse",nullptr,0},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
/** Complete official uwvm_ros_bounded_v2/DigestGoal converter, including all nested dependencies. */
struct Ros1Uwvm_ros_bounded_v2DigestGoalCodec {
using Native=::uwvm_ros_bounded_v2::DigestGoal;using Record=roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal;
static constexpr char const* type="uwvm_ros_bounded_v2/DigestGoal";
static constexpr char const* native_identity="305359dd474a0c87b3bcb9e6e0bb0a74";
static constexpr char const* codec_identity="UWVMROS1_08876b8b3f345f7baa27b2e371458f77eddb4f636cb00f03b6725d91d523accc";
static constexpr size_t wire_bytes=120;static constexpr bool bounded_profile=true;
using raw_0=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().sample)>>;
using field_0=raw_0;
static_assert(std::is_same<field_0,::uwvm_ros_bounded_v2::Envelope>::value,"official scalar/nested field type differs");
/** Validate native fields without copying and commit the exact active canonical size. */
static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;
{
size_t nested=0;if(!Ros1Uwvm_ros_bounded_v2EnvelopeCodec::encoded_size(value.sample,nested))return false;n+=nested;
}
if(n>120)return false;out=n;return true;}
/** Copy a fully bounded native projection into a private C value, never into wire output. */
static bool to_record(Native const& value,Record& record) noexcept {
if(!Ros1Uwvm_ros_bounded_v2EnvelopeCodec::to_record(value.sample,record.sample))return false;
return roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_valid(&record);
}
/** Populate only a fresh native temporary after complete canonical decoding. */
static bool from_record(Record const& record,Native& value) {
if(!Ros1Uwvm_ros_bounded_v2EnvelopeCodec::from_record(record.sample,value.sample))return false;
return true;}
static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}
static bool validate(Native const& value) noexcept {return valid(value);}
/** This pointer overload succeeds only when the capacity is the exact active wire length. */
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/DigestGoal","uwvm_ros_bounded_v2/Envelope sample\n"},
{"uwvm_ros_bounded_v2/Envelope","string label\nuint16[] samples\nuwvm_ros_bounded_v2/Point[] points\nstring[2] labels\n"},
{"uwvm_ros_bounded_v2/Point","int32 value\nbool enabled\n"},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={{"sample","uwvm_ros_bounded_v2/Envelope",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_1[]={{"label","string",0,false,0,false,0},{"samples","uint16",0,false,0,true,0},{"points","uwvm_ros_bounded_v2/Point",0,false,0,true,0},{"labels","string",2,true,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_2[]={{"value","int32",0,false,0,false,0},{"enabled","bool",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/DigestGoal",schema_fields_0,1},
{"uwvm_ros_bounded_v2/Envelope",schema_fields_1,4},
{"uwvm_ros_bounded_v2/Point",schema_fields_2,2},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
/** Complete official uwvm_ros_bounded_v2/DigestResult converter, including all nested dependencies. */
struct Ros1Uwvm_ros_bounded_v2DigestResultCodec {
using Native=::uwvm_ros_bounded_v2::DigestResult;using Record=roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult;
static constexpr char const* type="uwvm_ros_bounded_v2/DigestResult";
static constexpr char const* native_identity="838fa8c88aa04e915d1c18a54bfb5d7f";
static constexpr char const* codec_identity="UWVMROS1_4753d6deda29277eabe2e0b2b53ee10cbbf1a546eaf41ddf6a56174850339bcd";
static constexpr size_t wire_bytes=41;static constexpr bool bounded_profile=true;
using raw_0=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().accepted)>>;
using field_0=raw_0;
static_assert(std::is_same<field_0,uint8_t>::value,"official scalar/nested field type differs");
using raw_1=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().sum)>>;
using field_1=raw_1;
static_assert(std::is_same<field_1,uint32_t>::value,"official scalar/nested field type differs");
using raw_2=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().note)>>;
using field_2=raw_2;
static_assert(string_shape<field_2>::supported,"official field must be an 8-bit allocator-aware string");
/** Validate native fields without copying and commit the exact active canonical size. */
static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;
{
if(value.accepted!=0&&value.accepted!=1)return false;
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
if(value.accepted!=0&&value.accepted!=1)return false;
record.accepted=value.accepted;
record.sum=value.sum;
if(value.note.size()>32||!uwvm_bounded_utf8(reinterpret_cast<unsigned char const*>(value.note.data()),value.note.size()))return false;
record.note.size=static_cast<uint32_t>(value.note.size());uwvm_bounded_copy(record.note.data,value.note.data(),value.note.size());
return roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_valid(&record);
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
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/DigestResult","bool accepted\nuint32 sum\nstring note\n"},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={{"accepted","bool",0,false,0,false,0},{"sum","uint32",0,false,0,false,0},{"note","string",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/DigestResult",schema_fields_0,3},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
/** Complete official uwvm_ros_bounded_v2/DigestFeedback converter, including all nested dependencies. */
struct Ros1Uwvm_ros_bounded_v2DigestFeedbackCodec {
using Native=::uwvm_ros_bounded_v2::DigestFeedback;using Record=roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback;
static constexpr char const* type="uwvm_ros_bounded_v2/DigestFeedback";
static constexpr char const* native_identity="7c0ff39ed386c4a45e9aaf430115a49d";
static constexpr char const* codec_identity="UWVMROS1_1ff8a8acc544c733fe0719dc89ecbb3c99da08b156d1af7d29c5931006bee619";
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
return roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_valid(&record);
}
/** Populate only a fresh native temporary after complete canonical decoding. */
static bool from_record(Record const& record,Native& value) {
value.processed=record.processed;
value.phase.assign(reinterpret_cast<char const*>(record.phase.data),record.phase.size);
return true;}
static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}
static bool validate(Native const& value) noexcept {return valid(value);}
/** This pointer overload succeeds only when the capacity is the exact active wire length. */
static bool encode(Native const& value,uint8_t* out,size_t capacity) {Record record{};size_t n=0;if(!to_record(value,record)||roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_wire_size(&record)!=capacity)return false;return !roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_encode(&record,out,capacity,&n);}
/** Validate and size before the sole canonical output allocation; commit output once. */
static bool encode(Native const& value,std::vector<uint8_t>& out) {Record record{};if(!to_record(value,record))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_wire_size(&record),used=0;std::vector<uint8_t> next(n);if(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
static bool decode(uint8_t const* bytes,size_t size,Native& out) {Record record{};if(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_decode(bytes,size,&record))return false;Native next{};if(!from_record(record,next))return false;out=std::move(next);return true;}
inline static constexpr definition_entry definitions[]={
{"uwvm_ros_bounded_v2/DigestFeedback","uint32 processed\nstring phase\n"},
};
static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={{"processed","uint32",0,false,0,false,0},{"phase","string",0,false,0,false,0}};
inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
{"uwvm_ros_bounded_v2/DigestFeedback",schema_fields_0,2},
};
static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
};
}
#define UWVM_ROS_CUSTOM_ROS1_TYPES(M) \
  M(::uwvm_ros_bounded_v2::Point,"uwvm_ros_bounded_v2/Point","d01b93b38572826dbde8043bc3a87849","UWVMROS1_adb4b0b058e3491e31d00d4c0c5914bdeeabe7ba8ce7a4de798ac358c8fecf28",5,::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2PointCodec) \
  M(::uwvm_ros_bounded_v2::Envelope,"uwvm_ros_bounded_v2/Envelope","76b87d274fb061da1933bd67ff4757f6","UWVMROS1_18a098b82136b0c52c84df1c74e3dba78c63a877feabb3b14a750cbb2d7e3da4",120,::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2EnvelopeCodec)
#define UWVM_ROS_CUSTOM_ROS1_SERVICE_TYPES(M) \
  M(::uwvm_ros_bounded_v2::Echo,"uwvm_ros_bounded_v2/Echo","b03988b8955d560e020d9dde463a8cfb","UWVMROS1_82964f2f9649d7066eca130ee0eed9500fd90ca4744518a4c4bb12cf79aae64e",::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2EchoRequestCodec,::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2EchoResponseCodec) \
  M(::uwvm_ros_bounded_v2::Empty,"uwvm_ros_bounded_v2/Empty","d41d8cd98f00b204e9800998ecf8427e","UWVMROS1_297cd9938f8c2dc7d89d2a439d4e5a82aa5e591c379f8ff42692bf53b3b3cd33",::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2EmptyRequestCodec,::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2EmptyResponseCodec)
#define UWVM_ROS_CUSTOM_ROS1_ACTION_TYPES(M) \
  M(::uwvm_ros_bounded_v2::DigestAction,"uwvm_ros_bounded_v2/Digest","0091d83f95485b037b22ed41ee04754f","UWVMROS1_36823c6a605aaac10b1b0f8d289209ca802e1c2227c7cb0ee2bebc0431b3508e",::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2DigestGoalCodec,::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2DigestResultCodec,::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2DigestFeedbackCodec)
#define UWVM_ROS_CUSTOM_ROS1_TOPIC_TYPES(M) UWVM_ROS_CUSTOM_ROS1_TYPES(M)
