/** Complete value conversion between actual LimX mros mirrors and our NEW ROS2
 * bridge types. No type-support cast, network operation, identity or grant.
 * SDK stamp uint32 pairs and all names/arrays/status fields are retained.
 * A failed conversion leaves the destination unchanged. Finite quotas narrow
 * accepted values; they do not change native capacities or truncate a value.
 */
#pragma once
#include <limxsdk/msg/controller_msgs/IMUData.h>
#include <limxsdk/msg/controller_msgs/JointCmd.h>
#include <limxsdk/msg/controller_msgs/JointCmdLimx.h>
#include <limxsdk/msg/controller_msgs/JointState.h>
#include <uwvm_limx_bridge/msg/imu_data.hpp>
#include <uwvm_limx_bridge/msg/joint_cmd.hpp>
#include <uwvm_limx_bridge/msg/joint_cmd_limx.hpp>
#include <uwvm_limx_bridge/msg/joint_state.hpp>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <new>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace uwvm_limx {
namespace native = ::limxsdk::msg::controller_msgs;
namespace bridge = ::uwvm_limx_bridge::msg;
inline constexpr std::size_t maximum_joints=64, maximum_name_bytes=64,
    maximum_frame_bytes=128, maximum_native_packet=32768;

/** Canonical text is full UTF-8 without NUL, unlike arbitrary native strings. */
inline bool text(std::string_view s,std::size_t bound) noexcept {
    if(s.size()>bound)return false;
    for(std::size_t at{};at<s.size();) {
        auto a=static_cast<unsigned char>(s[at++]);
        if(a==0)return false;
        if(a<0x80)continue;
        unsigned n{},value{},minimum{};
        if(a>=0xc2&&a<=0xdf){n=1;value=a&31u;minimum=0x80;}
        else if(a>=0xe0&&a<=0xef){n=2;value=a&15u;minimum=0x800;}
        else if(a>=0xf0&&a<=0xf4){n=3;value=a&7u;minimum=0x10000;}
        else return false;
        if(n>s.size()-at)return false;
        for(unsigned i{};i<n;++i){auto b=static_cast<unsigned char>(s[at++]);if((b&0xc0)!=0x80)return false;value=(value<<6)|(b&63u);}
        if(value<minimum||value>0x10ffff||(value>=0xd800&&value<=0xdfff))return false;
    }
    return true;
}
template<class Range> inline bool finite(Range const& values) noexcept {
    for(auto value:values)if(!std::isfinite(value))return false;
    return true;
}
template<class Range> inline bool names(Range const& values,std::size_t n) noexcept {
    // The actual SDK permits nameless controllers. Empty stays empty; it is
    // never filled with guessed names or silently discarded on commands.
    if(!values.empty()&&values.size()!=n)return false;
    for(auto const& value:values)if(!text(value,maximum_name_bytes))return false;
    return true;
}
inline bool header(::limxsdk::msg::std_msgs::Header const& in,
    bridge::NativeHeader& out) {
    if(in.stamp.nsec>=1000000000u||!text(in.frame_id,maximum_frame_bytes))return false;
    out.seq=in.seq;out.stamp_sec=in.stamp.sec;out.stamp_nsec=in.stamp.nsec;out.frame_id=in.frame_id;return true;
}
inline bool header(bridge::NativeHeader const& in,
    ::limxsdk::msg::std_msgs::Header& out) {
    if(in.stamp_nsec>=1000000000u||!text(in.frame_id,maximum_frame_bytes))return false;
    out.seq=in.seq;out.stamp.sec=in.stamp_sec;out.stamp.nsec=in.stamp_nsec;out.frame_id=in.frame_id;return true;
}
/** Quota refusal is distinct from an invalid value. Check all actual source
 * extents before allocation; no prefix is copied on error90. */
template<class V> inline std::uint32_t quota(V const& in) noexcept {
    if(in.header.frame_id.size()>maximum_frame_bytes)return 90;
    if constexpr(requires{in.na;}) {
        if(in.na>maximum_joints||in.names.size()>maximum_joints)return 90;
        for(auto const& name:in.names)if(name.size()>maximum_name_bytes)return 90;
        if(in.q.size()>maximum_joints||in.v.size()>maximum_joints||in.tau.size()>maximum_joints)return 90;
        if constexpr(requires{in.vd;})if(in.vd.size()>maximum_joints)return 90;
        if constexpr(requires{in.kp;})if(in.kp.size()>maximum_joints||in.kd.size()>maximum_joints||in.mode.size()>maximum_joints)return 90;
        if constexpr(requires{in.parallel_solver_mode;})if(in.parallel_solver_mode.size()>maximum_joints)return 90;
    }
    return 0;
}
template<class Out,class Operation> inline std::uint32_t transaction(Out& out,Operation&& operation) noexcept {
    static_assert(std::is_nothrow_move_assignable_v<Out>,"Actual generated object must commit without allocation");
    try {Out next{};if(!operation(next))return 22;out=std::move(next);return 0;}
    catch(std::bad_alloc const&){return 12;}catch(...){return 22;}
}
inline std::uint32_t copy(native::IMUData const& in,bridge::ImuData& out) noexcept {
    if(auto e=quota(in))return e;
    return transaction(out,[&](auto& n){
        if(!finite(in.euler)||!finite(in.quat)||!finite(in.acc)||!finite(in.gyro)||!header(in.header,n.header))return false;
        n.imustamp=in.imustamp;n.status=in.status;
        std::copy(in.euler.begin(),in.euler.end(),n.euler.begin());
        std::copy(in.quat.begin(),in.quat.end(),n.quat.begin());
        std::copy(in.acc.begin(),in.acc.end(),n.acc.begin());
        std::copy(in.gyro.begin(),in.gyro.end(),n.gyro.begin());return true;
    });
}
inline std::uint32_t copy(bridge::ImuData const& in,native::IMUData& out) noexcept {
    if(auto e=quota(in))return e;
    return transaction(out,[&](auto& n){
        if(!finite(in.euler)||!finite(in.quat)||!finite(in.acc)||!finite(in.gyro)||!header(in.header,n.header))return false;
        n.imustamp=in.imustamp;n.status=in.status;
        std::copy(in.euler.begin(),in.euler.end(),n.euler.begin());
        std::copy(in.quat.begin(),in.quat.end(),n.quat.begin());
        std::copy(in.acc.begin(),in.acc.end(),n.acc.begin());
        std::copy(in.gyro.begin(),in.gyro.end(),n.gyro.begin());return true;
    });
}
template<class Value> inline bool state_shape(Value const& in) noexcept {
    auto n=static_cast<std::size_t>(in.na);
    return n<=maximum_joints&&names(in.names,n)&&in.q.size()==n&&in.v.size()==n&&
        in.vd.size()==n&&in.tau.size()==n&&finite(in.q)&&finite(in.v)&&finite(in.vd)&&finite(in.tau);
}
template<class Value> inline bool command_shape(Value const& in) noexcept {
    auto n=static_cast<std::size_t>(in.na);
    return n<=maximum_joints&&names(in.names,n)&&in.q.size()==n&&in.v.size()==n&&
        in.tau.size()==n&&in.kp.size()==n&&in.kd.size()==n&&in.mode.size()==n&&
        finite(in.q)&&finite(in.v)&&finite(in.tau)&&finite(in.kp)&&finite(in.kd);
}
template<class From,class To> inline bool state_fields(From const& in,To& n) {
    if(!state_shape(in)||!header(in.header,n.header))return false;
    n.na=in.na;n.names=in.names;n.q=in.q;n.v=in.v;n.vd=in.vd;n.tau=in.tau;return true;
}
template<class From,class To> inline bool command_fields(From const& in,To& n) {
    if(!command_shape(in)||!header(in.header,n.header))return false;
    n.na=in.na;n.names=in.names;n.q=in.q;n.v=in.v;n.tau=in.tau;
    n.kp=in.kp;n.kd=in.kd;n.mode=in.mode;return true;
}
inline std::uint32_t copy(native::JointState const& in,bridge::JointState& out) noexcept {
    if(auto e=quota(in))return e;
    return transaction(out,[&](auto& n){return state_fields(in,n);});
}
inline std::uint32_t copy(bridge::JointState const& in,native::JointState& out) noexcept {
    if(auto e=quota(in))return e;
    return transaction(out,[&](auto& n){return state_fields(in,n);});
}
inline std::uint32_t copy(native::JointCmd const& in,bridge::JointCmd& out) noexcept {
    if(auto e=quota(in))return e;
    return transaction(out,[&](auto& n){return command_fields(in,n);});
}
inline std::uint32_t copy(bridge::JointCmd const& in,native::JointCmd& out) noexcept {
    if(auto e=quota(in))return e;
    return transaction(out,[&](auto& n){return command_fields(in,n);});
}
inline std::uint32_t copy(native::JointCmdLimx const& in,bridge::JointCmdLimx& out) noexcept {
    if(auto e=quota(in))return e;
    return transaction(out,[&](auto& n){if(in.parallel_solver_mode.size()!=in.na||!command_fields(in,n))return false;n.parallel_solver_mode=in.parallel_solver_mode;return true;});
}
inline std::uint32_t copy(bridge::JointCmdLimx const& in,native::JointCmdLimx& out) noexcept {
    if(auto e=quota(in))return e;
    return transaction(out,[&](auto& n){if(in.parallel_solver_mode.size()!=in.na||!command_fields(in,n))return false;n.parallel_solver_mode=in.parallel_solver_mode;return true;});
}
} // namespace uwvm_limx
