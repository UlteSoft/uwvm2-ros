/** @file Generated ROS1 fixed-topic native conversion and dispatch.
 * Native generation/type support remains official. valid() allocates nothing;
 * encode validates all fields before modifying caller storage. decode commits
 * a complete temporary native record only after strict canonical validation.
 * Exact native field types/array shapes are asserted at compile time. Full
 * official native field/dependency descriptions MUST match native_schema(). */
#pragma once
#include "fixed_records.h"
#include <vector>
#include <utility>
#include <array>
#include <type_traits>
#include <string_view>
#include <uwvm2/imported/ros/ipc/custom_type.h>
#include <boost/array.hpp>
#include <uwvm_custom_ros1_fixture/Packet.h>
#ifndef UWVM_CUSTOM_NATIVE_SCHEMA_TRAITS
#define UWVM_CUSTOM_NATIVE_SCHEMA_TRAITS
namespace uwvm_custom_native {
/** @brief Normalized ROS1 definition subsection, preserving every fixed field. */
struct definition_entry {std::string_view type,fields;};
/** @brief Exact fixed array shape; unsupported containers have size zero. */
template<class T> struct fixed_array_shape {using value_type=void;static constexpr size_t size=0;};
template<class T,size_t N> struct fixed_array_shape<std::array<T,N>> {using value_type=T;static constexpr size_t size=N;};
}
#endif
#ifndef UWVM_CUSTOM_NATIVE_BOOST_ARRAY_TRAIT
#define UWVM_CUSTOM_NATIVE_BOOST_ARRAY_TRAIT
namespace uwvm_custom_native {
template<class T,size_t N> struct fixed_array_shape<boost::array<T,N>> {using value_type=T;static constexpr size_t size=N;};
}
#endif
namespace uwvm_custom_native {
using Ros1Uwvm_custom_ros1_fixturePacketCodec_field_0_0=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<::uwvm_custom_ros1_fixture::Packet&>().sequence)>>;
static_assert(std::is_same<Ros1Uwvm_custom_ros1_fixturePacketCodec_field_0_0,int32_t>::value,"native scalar/nested type differs from generated schema");
using Ros1Uwvm_custom_ros1_fixturePacketCodec_field_0_1=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<::uwvm_custom_ros1_fixture::Packet&>().active)>>;
static_assert(std::is_same<Ros1Uwvm_custom_ros1_fixturePacketCodec_field_0_1,uint8_t>::value,"native scalar/nested type differs from generated schema");
using Ros1Uwvm_custom_ros1_fixturePacketCodec_field_0_2=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<::uwvm_custom_ros1_fixture::Packet&>().measurement)>>;
static_assert(std::is_same<Ros1Uwvm_custom_ros1_fixturePacketCodec_field_0_2,double>::value,"native scalar/nested type differs from generated schema");
using Ros1Uwvm_custom_ros1_fixturePacketCodec_field_0_3=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<::uwvm_custom_ros1_fixture::Packet&>().tag)>>;
static_assert(fixed_array_shape<Ros1Uwvm_custom_ros1_fixturePacketCodec_field_0_3>::size==4,"native fixed array length differs from generated schema");
static_assert(std::is_same<typename fixed_array_shape<Ros1Uwvm_custom_ros1_fixturePacketCodec_field_0_3>::value_type,uint8_t>::value,"native fixed array element type differs from generated schema");
using Ros1Uwvm_custom_ros1_fixturePacketCodec_field_0_4=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<::uwvm_custom_ros1_fixture::Packet&>().samples)>>;
static_assert(fixed_array_shape<Ros1Uwvm_custom_ros1_fixturePacketCodec_field_0_4>::size==3,"native fixed array length differs from generated schema");
static_assert(std::is_same<typename fixed_array_shape<Ros1Uwvm_custom_ros1_fixturePacketCodec_field_0_4>::value_type,int16_t>::value,"native fixed array element type differs from generated schema");
/** @brief Fixed converter for official uwvm_custom_ros1_fixture/Packet; native identities require runtime verification. */
struct Ros1Uwvm_custom_ros1_fixturePacketCodec {
    using Native=::uwvm_custom_ros1_fixture::Packet;
    static constexpr char const* type="uwvm_custom_ros1_fixture/Packet";
    static constexpr char const* native_identity="f9f763c3e1630ff9c5652c744e59a68a";
    static constexpr char const* codec_identity="UWVMROS1_eb0eef4515ec546678bae4030eb858c151af98afa1bfa3ca08b3a0e02bd48544";
    static constexpr size_t wire_bytes=23;
    static bool valid(Native const& value) noexcept {
        if(value.active!=0&&value.active!=1)return false;
        {uint64_t bits=0;uwvm_custom_copy(&bits,&value.measurement,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return false;}
        return true;
    }
    static bool validate(Native const& value) noexcept {return valid(value);}
    /** @brief Full normalized native root/dependency definitions; constants/defaults are unsupported. */
    inline static constexpr definition_entry definitions[]={
        {"uwvm_custom_ros1_fixture/Packet","int32 sequence\nbool active\nfloat64 measurement\nuint8[4] tag\nint16[3] samples\n"},
    };
    static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
    inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={
        {"sequence","int32",0,false},
        {"active","bool",0,false},
        {"measurement","float64",0,false},
        {"tag","uint8",4,true},
        {"samples","int16",3,true},
    };
    inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
        {"uwvm_custom_ros1_fixture/Packet",schema_fields_0,sizeof(schema_fields_0)/sizeof(schema_fields_0[0])},
    };
    /** @brief Exact ordered fields for every native record, including the transitive closure. */
    static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
    static bool encode(Native const& value,uint8_t* output,size_t capacity) {
        if(!output||capacity<wire_bytes||!valid(value))return false;roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet record{};
        record.sequence=value.sequence;
        record.active=value.active;
        record.measurement=value.measurement;
        record.tag[0]=value.tag[0];
        record.tag[1]=value.tag[1];
        record.tag[2]=value.tag[2];
        record.tag[3]=value.tag[3];
        record.samples[0]=value.samples[0];
        record.samples[1]=value.samples[1];
        record.samples[2]=value.samples[2];
        size_t length;return !roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet_encode(&record,output,capacity,&length);
    }
    static bool decode(uint8_t const* bytes,size_t size,Native& out) {
        roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet record{};if(roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet_decode(bytes,size,&record))return false;Native next{};
        next.sequence=record.sequence;
        next.active=record.active;
        next.measurement=record.measurement;
        next.tag[0]=record.tag[0];
        next.tag[1]=record.tag[1];
        next.tag[2]=record.tag[2];
        next.tag[3]=record.tag[3];
        next.samples[0]=record.samples[0];
        next.samples[1]=record.samples[1];
        next.samples[2]=record.samples[2];
        out=std::move(next);return true;
    }
    static bool encode(Native const& value,std::vector<uint8_t>& out) {if(!valid(value))return false;std::vector<uint8_t> next(wire_bytes);if(!encode(value,next.data(),next.size()))return false;out=std::move(next);return true;}
};
}
#define UWVM_ROS_CUSTOM_ROS1_TYPES(M) \
    M(::uwvm_custom_ros1_fixture::Packet,"uwvm_custom_ros1_fixture/Packet","f9f763c3e1630ff9c5652c744e59a68a","UWVMROS1_eb0eef4515ec546678bae4030eb858c151af98afa1bfa3ca08b3a0e02bd48544",23,::uwvm_custom_native::Ros1Uwvm_custom_ros1_fixturePacketCodec)
#define UWVM_ROS_CUSTOM_ROS1_TOPIC_TYPES(M) UWVM_ROS_CUSTOM_ROS1_TYPES(M)
