/** Complete official RobotMsg native fixture, not a robot-control command.
 * Include ONLY an actually native-bound generated header for the selected ROS
 * major. It includes the real installed RobotMsg/Header/Time native definitions.
 * Native identities below are read from the SDK; no MD5/RIHS string is supplied.
 */
#ifndef UWVM_XARM_ROBOT_MSG_NATIVE_FIXTURE_HPP
#define UWVM_XARM_ROBOT_MSG_NATIVE_FIXTURE_HPP
#include <cstdint>
#include <cstring>
#include <string_view>
#include <vector>
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
#include "native_ros1.hpp"
using Native=::xarm_msgs::RobotMsg;
using Codec=::uwvm_custom_bounded_native::Ros1Xarm_msgsRobotMsgCodec;
using HeaderCodec=::uwvm_custom_bounded_native::Ros1Std_msgsHeaderCodec;
#elif UWVM_CUSTOM_NATIVE_ROS_VERSION == 2
#include "native_ros2.hpp"
#include <rosidl_typesupport_cpp/message_type_support.hpp>
using Native=::xarm_msgs::msg::RobotMsg;
using Codec=::uwvm_custom_bounded_native::Ros2Xarm_msgsMsgRobotMsgCodec;
using HeaderCodec=::uwvm_custom_bounded_native::Ros2Std_msgsMsgHeaderCodec;
using TimeCodec=::uwvm_custom_bounded_native::Ros2Builtin_interfacesMsgTimeCodec;
#else
#error "Select exactly one actual native ROS major and matching generated bundle"
#endif
namespace xarm_robot_msg_fixture {
inline Native sample() {
    Native v{};
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    v.header.seq=19;v.header.stamp.sec=42;v.header.stamp.nsec=300000000;
#else
    v.header.stamp.sec=-2;v.header.stamp.nanosec=300000000;
#endif
    v.header.frame_id="xarm-base";v.state=4;v.mode=0;v.cmdnum=3;
    v.mt_brake=85;v.mt_able=42;v.err=7;v.warn=9;
    v.angle={0.0f,0.25f,-0.5f,1.0f,-1.25f,1.5f,-1.75f};
    float const pose[6]={125.5f,-250.25f,375.0f,0.25f,-0.5f,0.75f};
    float const offset[6]={1.0f,2.0f,3.0f,-0.125f,0.25f,-0.5f};
    for(unsigned i{};i!=6;++i){v.pose[i]=pose[i];v.offset[i]=offset[i];}
    return v;
}
inline bool same(Native const& a,Native const& b) {
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    if(a.header.seq!=b.header.seq||a.header.stamp.nsec!=b.header.stamp.nsec)return false;
#else
    if(a.header.stamp.nanosec!=b.header.stamp.nanosec)return false;
#endif
    if(a.header.stamp.sec!=b.header.stamp.sec||a.header.frame_id!=b.header.frame_id||a.state!=b.state||a.mode!=b.mode||a.cmdnum!=b.cmdnum||a.mt_brake!=b.mt_brake||a.mt_able!=b.mt_able||a.err!=b.err||a.warn!=b.warn||a.angle!=b.angle)return false;
    for(unsigned i{};i!=6;++i)if(a.pose[i]!=b.pose[i]||a.offset[i]!=b.offset[i])return false;
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
