/** @file C++ adapters for generated bounded-v2/native ROS2 complete values.
 * No native headers, ROS call, allocation, grant, or transport is introduced.
 * These template adapters copy semantic fields into our explicit layout view;
 * they never inspect a C++ object's padding.  Only crc is committed on success.
 * A successful checksum is not an actuator-range/model/permission validation.
 */
#pragma once
#include "crc.h"
#include <array>
#include <type_traits>
namespace roswasm_unitree {
namespace detail {
template<class T> struct fixed_count;
template<class T,size_t N> struct fixed_count<T[N]> {static constexpr size_t value=N;};
template<class T,size_t N> struct fixed_count<std::array<T,N>> {static constexpr size_t value=N;};
}
template<class Message> uint32_t set_go_lowcmd_crc(Message& message) noexcept {
    static_assert(detail::fixed_count<decltype(message.motor_cmd)>::value==20);
    static_assert(detail::fixed_count<decltype(message.head)>::value==2);
    static_assert(detail::fixed_count<decltype(message.sn)>::value==2);
    static_assert(detail::fixed_count<decltype(message.version)>::value==2);
    static_assert(detail::fixed_count<decltype(message.fan)>::value==2);
    static_assert(detail::fixed_count<decltype(message.wireless_remote)>::value==40);
    static_assert(detail::fixed_count<decltype(message.led)>::value==12);
    static_assert(detail::fixed_count<decltype(message.bms_cmd.reserve)>::value==3);
    static_assert(detail::fixed_count<decltype(message.motor_cmd[0].reserve)>::value==3);
    static_assert(std::is_same_v<decltype(message.motor_cmd[0].q),float>);
    uwvm_unitree_go_lowcmd view{};
    for(unsigned i=0;i<2;++i){view.head[i]=message.head[i];view.sn[i]=message.sn[i];view.version[i]=message.version[i];view.fan[i]=message.fan[i];}
    view.level_flag=message.level_flag;view.frame_reserve=message.frame_reserve;view.bandwidth=message.bandwidth;
    for(unsigned i=0;i<20;++i){auto const& m=message.motor_cmd[i];auto& v=view.motor_cmd[i];v.mode=m.mode;v.q=m.q;v.dq=m.dq;v.tau=m.tau;v.kp=m.kp;v.kd=m.kd;for(unsigned j=0;j<3;++j)v.reserve[j]=m.reserve[j];}
    view.bms_off=message.bms_cmd.off;for(unsigned i=0;i<3;++i)view.bms_reserve[i]=message.bms_cmd.reserve[i];
    for(unsigned i=0;i<40;++i)view.wireless_remote[i]=message.wireless_remote[i];
    for(unsigned i=0;i<12;++i)view.led[i]=message.led[i];view.gpio=message.gpio;view.reserve=message.reserve;
    uint8_t packed[UWVM_UNITREE_GO_LOWCMD_BYTES];size_t used=0;uint32_t crc=0;
    auto status=uwvm_unitree_go_lowcmd_pack_crc(&view,packed,sizeof packed,&used,&crc);
    if(!status)message.crc=crc;return status;
}
template<class Message> uint32_t set_hg_lowcmd_crc(Message& message) noexcept {
    static_assert(detail::fixed_count<decltype(message.motor_cmd)>::value==35);
    static_assert(detail::fixed_count<decltype(message.reserve)>::value==4);
    static_assert(std::is_same_v<decltype(message.motor_cmd[0].q),float>);
    uwvm_unitree_hg_lowcmd view{};view.mode_pr=message.mode_pr;view.mode_machine=message.mode_machine;
    for(unsigned i=0;i<35;++i){auto const& m=message.motor_cmd[i];auto& v=view.motor_cmd[i];v.mode=m.mode;v.q=m.q;v.dq=m.dq;v.tau=m.tau;v.kp=m.kp;v.kd=m.kd;v.reserve=m.reserve;}
    for(unsigned i=0;i<4;++i)view.reserve[i]=message.reserve[i];
    uint8_t packed[UWVM_UNITREE_HG_LOWCMD_BYTES];size_t used=0;uint32_t crc=0;
    auto status=uwvm_unitree_hg_lowcmd_pack_crc(&view,packed,sizeof packed,&used,&crc);
    if(!status)message.crc=crc;return status;
}
} // namespace roswasm_unitree
