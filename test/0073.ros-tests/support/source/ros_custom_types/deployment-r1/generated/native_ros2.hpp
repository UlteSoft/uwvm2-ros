/** @file Generated ROS2 fixed-topic native conversion and dispatch.
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
#include <uwvm_ros_custom_test_msgs/msg/fixed_sample.hpp>
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
namespace uwvm_custom_native {
using Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec_field_0_0=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<::uwvm_ros_custom_test_msgs::msg::FixedSample&>().sequence)>>;
static_assert(std::is_same<Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec_field_0_0,uint32_t>::value,"native scalar/nested type differs from generated schema");
using Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec_field_0_1=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<::uwvm_ros_custom_test_msgs::msg::FixedSample&>().enabled)>>;
static_assert(std::is_same<Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec_field_0_1,bool>::value,"native scalar/nested type differs from generated schema");
using Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec_field_0_2=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<::uwvm_ros_custom_test_msgs::msg::FixedSample&>().temperature)>>;
static_assert(std::is_same<Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec_field_0_2,float>::value,"native scalar/nested type differs from generated schema");
using Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec_field_0_3=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<::uwvm_ros_custom_test_msgs::msg::FixedSample&>().acceleration)>>;
static_assert(fixed_array_shape<Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec_field_0_3>::size==3,"native fixed array length differs from generated schema");
static_assert(std::is_same<typename fixed_array_shape<Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec_field_0_3>::value_type,double>::value,"native fixed array element type differs from generated schema");
using Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec_field_0_4=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<::uwvm_ros_custom_test_msgs::msg::FixedSample&>().tag)>>;
static_assert(fixed_array_shape<Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec_field_0_4>::size==8,"native fixed array length differs from generated schema");
static_assert(std::is_same<typename fixed_array_shape<Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec_field_0_4>::value_type,uint8_t>::value,"native fixed array element type differs from generated schema");
using Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec_field_0_5=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<::uwvm_ros_custom_test_msgs::msg::FixedSample&>().thresholds)>>;
static_assert(fixed_array_shape<Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec_field_0_5>::size==2,"native fixed array length differs from generated schema");
static_assert(std::is_same<typename fixed_array_shape<Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec_field_0_5>::value_type,int16_t>::value,"native fixed array element type differs from generated schema");
/** @brief Fixed converter for official uwvm_ros_custom_test_msgs/msg/FixedSample; native identities require runtime verification. */
struct Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec {
    using Native=::uwvm_ros_custom_test_msgs::msg::FixedSample;
    static constexpr char const* type="uwvm_ros_custom_test_msgs/msg/FixedSample";
    static constexpr char const* native_identity="RIHS01_5026d3427fd5649d1cb11a7cf9e79429d2775096e9bec98669dde689b4d20fc2";
    static constexpr char const* codec_identity="UWVMROS1_3f390efa2dccaf91a53c14a50fcead8790bb1bbc103c6ac6b29f176d045e2516";
    static constexpr size_t wire_bytes=45;
    static bool valid(Native const& value) noexcept {
        if(value.enabled!=0&&value.enabled!=1)return false;
        {uint32_t bits=0;uwvm_custom_copy(&bits,&value.temperature,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return false;}
        {uint64_t bits=0;uwvm_custom_copy(&bits,&value.acceleration[0],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return false;}
        {uint64_t bits=0;uwvm_custom_copy(&bits,&value.acceleration[1],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return false;}
        {uint64_t bits=0;uwvm_custom_copy(&bits,&value.acceleration[2],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return false;}
        return true;
    }
    static bool validate(Native const& value) noexcept {return valid(value);}
    /** @brief Full normalized native root/dependency definitions; constants/defaults are unsupported. */
    inline static constexpr definition_entry definitions[]={
        {"uwvm_ros_custom_test_msgs/msg/FixedSample","uint32 sequence\nbool enabled\nfloat32 temperature\nfloat64[3] acceleration\nuint8[8] tag\nint16[2] thresholds\n"},
    };
    static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);
    inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_0[]={
        {"sequence","uint32",0,false},
        {"enabled","bool",0,false},
        {"temperature","float32",0,false},
        {"acceleration","float64",3,true},
        {"tag","uint8",8,true},
        {"thresholds","int16",2,true},
    };
    inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={
        {"uwvm_ros_custom_test_msgs/msg/FixedSample",schema_fields_0,sizeof(schema_fields_0)/sizeof(schema_fields_0[0])},
    };
    /** @brief Exact ordered fields for every native record, including the transitive closure. */
    static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}
    static bool encode(Native const& value,uint8_t* output,size_t capacity) {
        if(!output||capacity<wire_bytes||!valid(value))return false;roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample record{};
        record.sequence=value.sequence;
        record.enabled=value.enabled;
        record.temperature=value.temperature;
        record.acceleration[0]=value.acceleration[0];
        record.acceleration[1]=value.acceleration[1];
        record.acceleration[2]=value.acceleration[2];
        record.tag[0]=value.tag[0];
        record.tag[1]=value.tag[1];
        record.tag[2]=value.tag[2];
        record.tag[3]=value.tag[3];
        record.tag[4]=value.tag[4];
        record.tag[5]=value.tag[5];
        record.tag[6]=value.tag[6];
        record.tag[7]=value.tag[7];
        record.thresholds[0]=value.thresholds[0];
        record.thresholds[1]=value.thresholds[1];
        size_t length;return !roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample_encode(&record,output,capacity,&length);
    }
    static bool decode(uint8_t const* bytes,size_t size,Native& out) {
        roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample record{};if(roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample_decode(bytes,size,&record))return false;Native next{};
        next.sequence=record.sequence;
        next.enabled=record.enabled;
        next.temperature=record.temperature;
        next.acceleration[0]=record.acceleration[0];
        next.acceleration[1]=record.acceleration[1];
        next.acceleration[2]=record.acceleration[2];
        next.tag[0]=record.tag[0];
        next.tag[1]=record.tag[1];
        next.tag[2]=record.tag[2];
        next.tag[3]=record.tag[3];
        next.tag[4]=record.tag[4];
        next.tag[5]=record.tag[5];
        next.tag[6]=record.tag[6];
        next.tag[7]=record.tag[7];
        next.thresholds[0]=record.thresholds[0];
        next.thresholds[1]=record.thresholds[1];
        out=std::move(next);return true;
    }
    static bool encode(Native const& value,std::vector<uint8_t>& out) {if(!valid(value))return false;std::vector<uint8_t> next(wire_bytes);if(!encode(value,next.data(),next.size()))return false;out=std::move(next);return true;}
};
}
#define UWVM_ROS_CUSTOM_ROS2_TYPES(M) \
    M(::uwvm_ros_custom_test_msgs::msg::FixedSample,"uwvm_ros_custom_test_msgs/msg/FixedSample","RIHS01_5026d3427fd5649d1cb11a7cf9e79429d2775096e9bec98669dde689b4d20fc2","UWVMROS1_3f390efa2dccaf91a53c14a50fcead8790bb1bbc103c6ac6b29f176d045e2516",45,::uwvm_custom_native::Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec)
#define UWVM_ROS_CUSTOM_ROS2_TOPIC_TYPES(M) UWVM_ROS_CUSTOM_ROS2_TYPES(M)
