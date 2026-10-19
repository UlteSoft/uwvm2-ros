/** Complete official CIOState native fixture, not a robot-control command.
 * Include ONLY an actually native-bound generated header for the selected ROS
 * major. It includes the real installed CIOState/Header/Time native definitions.
 * Native identities below are read from the SDK; no MD5/RIHS string is supplied.
 */
#ifndef UWVM_XARM_CIO_STATE_NATIVE_FIXTURE_HPP
#define UWVM_XARM_CIO_STATE_NATIVE_FIXTURE_HPP
#include <cstdint>
#include <cstring>
#include <string_view>
#include <vector>
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
#include "native_ros1.hpp"
using Native=::xarm_msgs::CIOState;
using Codec=::uwvm_custom_bounded_native::Ros1Xarm_msgsCIOStateCodec;
using HeaderCodec=::uwvm_custom_bounded_native::Ros1Std_msgsHeaderCodec;
#elif UWVM_CUSTOM_NATIVE_ROS_VERSION == 2
#include "native_ros2.hpp"
#include <rosidl_typesupport_cpp/message_type_support.hpp>
using Native=::xarm_msgs::msg::CIOState;
using Codec=::uwvm_custom_bounded_native::Ros2Xarm_msgsMsgCIOStateCodec;
using HeaderCodec=::uwvm_custom_bounded_native::Ros2Std_msgsMsgHeaderCodec;
using TimeCodec=::uwvm_custom_bounded_native::Ros2Builtin_interfacesMsgTimeCodec;
#else
#error "Select exactly one actual native ROS major and matching generated bundle"
#endif
namespace xarm_cio_state_fixture {
inline Native sample() {
    Native v{};
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    v.header.seq=19;v.header.stamp.sec=42;v.header.stamp.nsec=300000000;
#else
    v.header.stamp.sec=-2;v.header.stamp.nanosec=300000000;
#endif
    v.header.frame_id="xarm-gpio";v.state=1;v.code=0;
    v.input_digitals[0]=165;v.input_digitals[1]=32769;v.output_digitals[0]=90;v.output_digitals[1]=16386;
    v.input_analogs[0]=1.25f;v.input_analogs[1]=2.5f;v.output_analogs[0]=3.75f;v.output_analogs[1]=0.5f;
    for(unsigned i{};i!=16;++i){v.input_conf[i]=static_cast<std::uint8_t>(i);v.output_conf[i]=static_cast<std::uint8_t>(15-i);}
    return v;
}
inline bool same(Native const& a,Native const& b) {
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    if(a.header.seq!=b.header.seq||a.header.stamp.nsec!=b.header.stamp.nsec)return false;
#else
    if(a.header.stamp.nanosec!=b.header.stamp.nanosec)return false;
#endif
    if(a.header.stamp.sec!=b.header.stamp.sec||a.header.frame_id!=b.header.frame_id||a.state!=b.state||a.code!=b.code)return false;
    for(unsigned i{};i!=2;++i)if(a.input_digitals[i]!=b.input_digitals[i]||a.output_digitals[i]!=b.output_digitals[i]||a.input_analogs[i]!=b.input_analogs[i]||a.output_analogs[i]!=b.output_analogs[i])return false;
    for(unsigned i{};i!=16;++i)if(a.input_conf[i]!=b.input_conf[i]||a.output_conf[i]!=b.output_conf[i])return false;
    return true;
}
template<class Message> inline bool actual_identity_for(char const* expected) {
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    return std::string_view{expected}==::ros::message_traits::MD5Sum<Message>::value();
#else
    // This concrete branch targets the pinned Jazzy source family, not older
    // introspection-only ROS2. That separate reviewed exporter must be selected
    // explicitly before any future old-version native lesson is claimed.
    auto support=rosidl_typesupport_cpp::get_message_type_support_handle<Message>();
    if(!support||!support->get_type_hash_func)return false;
    auto hash=support->get_type_hash_func(support);if(!hash||hash->version!=1)return false;
    char text[71]{};constexpr char hex[]{"0123456789abcdef"};std::memcpy(text,"RIHS01_",7);
    for(unsigned i{};i!=32;++i){text[7+2*i]=hex[hash->value[i]>>4];text[8+2*i]=hex[hash->value[i]&15];}
    return std::string_view{text,sizeof text}==expected;
#endif
}
inline bool actual_identities() {
    if(!actual_identity_for<Native>(Codec::native_identity))return false;
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    return std::string_view{Codec::type}==::ros::message_traits::DataType<Native>::value()&&actual_identity_for<::std_msgs::Header>(HeaderCodec::native_identity);
#else
    return actual_identity_for<::std_msgs::msg::Header>(HeaderCodec::native_identity)&&actual_identity_for<::builtin_interfaces::msg::Time>(TimeCodec::native_identity);
#endif
}
}
#endif
