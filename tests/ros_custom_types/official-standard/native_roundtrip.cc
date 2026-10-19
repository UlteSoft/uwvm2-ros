/** Actual official native Header/JointState conversion contract, source only.
 * Compile only against actual freshly exported identities and generated source
 * from the matching real ROS SDK. FullMD/ELF/link/maps and native traits remain
 * required; no mock message/provider or hand-written native hash is supplied.
 * No ROS Node, endpoint, command publication, clock or robot process is created.
 */
#include "tests/ros_custom_types/bounded-v2/native_protocol.h"
#include <cstdint>
#include <cstring>
#include <string_view>
#include <vector>
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
#include "native_ros1.hpp"
using Native=::sensor_msgs::JointState;
using Codec=::uwvm_custom_bounded_native::Ros1Sensor_msgsJointStateCodec;
using HeaderCodec=::uwvm_custom_bounded_native::Ros1Std_msgsHeaderCodec;
#elif UWVM_CUSTOM_NATIVE_ROS_VERSION == 2
#include "native_ros2.hpp"
#include <rosidl_typesupport_cpp/message_type_support.hpp>
using Native=::sensor_msgs::msg::JointState;
using Codec=::uwvm_custom_bounded_native::Ros2Sensor_msgsMsgJointStateCodec;
using HeaderCodec=::uwvm_custom_bounded_native::Ros2Std_msgsMsgHeaderCodec;
using TimeCodec=::uwvm_custom_bounded_native::Ros2Builtin_interfacesMsgTimeCodec;
#else
#error "Select one matching official native ROS major"
#endif
namespace {
unsigned checks{},failures{};
void check(bool ok,char const* what) {++checks;if(!ok){++failures;bounded_native_io::print(STDERR_FILENO,"FAIL_OFFICIAL_STANDARD ",what,'\n');}}
bool same(Native const& a,Native const& b) {
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    if(a.header.seq!=b.header.seq||a.header.stamp.nsec!=b.header.stamp.nsec)return false;
#else
    if(a.header.stamp.nanosec!=b.header.stamp.nanosec)return false;
#endif
    return a.header.stamp.sec==b.header.stamp.sec&&a.header.frame_id==b.header.frame_id&&
        a.name==b.name&&a.position==b.position&&a.velocity==b.velocity&&a.effort==b.effort;
}
template<class Message> bool actual_identity_for(char const* wanted) {
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    return std::string_view{wanted}==::ros::message_traits::MD5Sum<Message>::value();
#else
    auto support=rosidl_typesupport_cpp::get_message_type_support_handle<Message>();
    if(!support||!support->get_type_hash_func)return false;
    auto hash=support->get_type_hash_func(support);if(!hash||hash->version!=1)return false;
    char text[71]{};constexpr char hex[]{"0123456789abcdef"};std::memcpy(text,"RIHS01_",7);
    for(unsigned i{};i!=32;++i){text[7+2*i]=hex[hash->value[i]>>4];text[8+2*i]=hex[hash->value[i]&15];}
    return std::string_view{text,sizeof text}==wanted;
#endif
}
}
int main() {
    check(actual_identity_for<Native>(Codec::native_identity),"original actual official JointState identity");
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    check(std::string_view{Codec::type}==::ros::message_traits::DataType<Native>::value(),"actual native JointState type spelling");
    check(actual_identity_for<::std_msgs::Header>(HeaderCodec::native_identity),"original actual official Header identity");
#else
    check(actual_identity_for<::std_msgs::msg::Header>(HeaderCodec::native_identity),"original actual official Header identity");
    check(actual_identity_for<::builtin_interfaces::msg::Time>(TimeCodec::native_identity),"original actual official Time message identity");
#endif
    Native value{};
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    value.header.seq=19;value.header.stamp.sec=42;value.header.stamp.nsec=300000000;
#else
    value.header.stamp.sec=-2;value.header.stamp.nanosec=300000000;
#endif
    value.header.frame_id="base";value.name={"left","right"};value.position={1.25,-2.5};
    value.velocity={};value.effort={0.5,-0.25};
    std::vector<std::uint8_t> wire;check(Codec::encode(value,wire),"complete native encode");
    Native decoded{};check(Codec::decode(wire.data(),wire.size(),decoded),"complete native decode");
    check(same(value,decoded),"all Header/time/joint/optional-array fields preserved");
    auto original=wire;Native sentinel=value;sentinel.header.frame_id="unchanged";auto before=sentinel;
    wire.push_back(0);check(!Codec::decode(wire.data(),wire.size(),sentinel),"trailing byte refused");
    check(same(sentinel,before),"trailing-byte failure preserves destination");
    wire=original;
    // Canonical records preserve the official declaration order, never native
    // struct padding. ROS1 Header has seq before time; ROS2 Header has no seq.
    constexpr unsigned nanos_offset=UWVM_CUSTOM_NATIVE_ROS_VERSION==1?8:4;
    check(wire.size()>=nanos_offset+4,"actual canonical timestamp extent");
    if(wire.size()>=nanos_offset+4){for(unsigned i{};i!=4;++i)wire[nanos_offset+i]=static_cast<std::uint8_t>(UINT32_C(1000000000)>>(8*i));
        check(!Codec::decode(wire.data(),wire.size(),sentinel),"nonnormalized nanoseconds refused");
        check(same(sentinel,before),"nanosecond failure preserves destination");}
    Native bad=value;bad.position.push_back(9);auto unchanged=original;
    check(!Codec::encode(bad,unchanged),"nonempty joint arrays must have equal length");
    check(unchanged==original,"invalid full native value preserves output");
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    bad=value;bad.header.stamp.nsec=1000000000;
#else
    bad=value;bad.header.stamp.nanosec=1000000000;
#endif
    std::size_t extent=777;check(!Codec::encoded_size(bad,extent)&&extent==777,"native normalized time check before allocation");
    Native empty{};empty.header=value.header;wire.clear();check(Codec::encode(empty,wire),"all optional arrays empty is preserved");
    check(Codec::decode(wire.data(),wire.size(),decoded)&&same(empty,decoded),"empty-array full native roundtrip");
    bounded_native_io::print(STDOUT_FILENO,"PASS_OFFICIAL_STANDARD_NATIVE groups=4 checks=",checks," failures=",failures,'\n');
    return failures?1:0;
}
