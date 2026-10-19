/** Exact original SDK objects <-> distinctly named NEW ROS2 bridge values.
 * Official robocup ROS2 int8 fields differ from these native SDK uint8 fields.
 * CmdType is a genuine native enum. No width-based identity or cast is claimed.
 */
#pragma once
#include <booster/idl/b1/LowCmd.h>
#include <booster/idl/b1/LowState.h>
#include <booster/robot/b1/b1_api_const.hpp>
#include <uwvm_booster_sdk_bridge/msg/low_cmd.hpp>
#include <uwvm_booster_sdk_bridge/msg/low_state.hpp>
#include <cmath>
#include <cstdint>
#include <new>
#include <type_traits>
#include <utility>
namespace uwvm_booster {
using Command=::uwvm_booster_sdk_bridge::msg::LowCmd;
using State=::uwvm_booster_sdk_bridge::msg::LowState;
using NativeCommand=::booster_interface::msg::LowCmd;
using NativeState=::booster_interface::msg::LowState;
static_assert(::booster::robot::b1::kJointCnt==23,"Actual fixed T1 SDK layout required");
static_assert(static_cast<std::uint32_t>(::booster_interface::msg::PARALLEL)==0&&static_cast<std::uint32_t>(::booster_interface::msg::SERIAL)==1,"Actual SDK enum domain changed");
static_assert(std::is_same_v<decltype(std::declval<::booster_interface::msg::MotorCmd const&>().mode()),std::uint8_t>);
static_assert(std::is_same_v<decltype(std::declval<::booster_interface::msg::MotorState const&>().temperature()),std::uint8_t>);
template<class R> bool finite(R const& rows)noexcept{for(auto x:rows)if(!std::isfinite(x))return false;return true;}
template<class Out,class F> std::uint32_t transaction(Out& out,F&& op)noexcept {
 static_assert(std::is_nothrow_move_assignable_v<Out>);
 try{Out next{};if(!op(next))return 22;out=std::move(next);return 0;}catch(std::bad_alloc const&){return 12;}catch(...){return 22;}
}
inline std::uint32_t copy(NativeCommand const& v,Command& out)noexcept {
 if(v.motor_cmd().size()>29)return 90;
 return transaction(out,[&](auto& n){auto actual=static_cast<std::uint32_t>(v.cmd_type());if(actual>1)return false;n.cmd_type=static_cast<std::int32_t>(actual);n.motor_cmd.resize(v.motor_cmd().size());
 for(std::size_t i{};i<n.motor_cmd.size();++i){auto const& s=v.motor_cmd()[i];auto& d=n.motor_cmd[i];
 if(!std::isfinite(s.q())||!std::isfinite(s.dq())||!std::isfinite(s.tau())||!std::isfinite(s.kp())||!std::isfinite(s.kd())||!std::isfinite(s.weight()))return false;
 d.mode=s.mode();d.q=s.q();d.dq=s.dq();d.tau=s.tau();d.kp=s.kp();d.kd=s.kd();d.weight=s.weight();}return true;});
}
inline std::uint32_t copy(Command const& v,NativeCommand& out)noexcept {
 if(v.motor_cmd.size()>29)return 90;
 return transaction(out,[&](auto& n){if(v.cmd_type!=0&&v.cmd_type!=1)return false;n.cmd_type(static_cast<::booster_interface::msg::CmdType>(v.cmd_type));n.motor_cmd().resize(v.motor_cmd.size());
 for(std::size_t i{};i<v.motor_cmd.size();++i){auto const& s=v.motor_cmd[i];auto& d=n.motor_cmd()[i];
 if(!std::isfinite(s.q)||!std::isfinite(s.dq)||!std::isfinite(s.tau)||!std::isfinite(s.kp)||!std::isfinite(s.kd)||!std::isfinite(s.weight))return false;
 d.mode(s.mode);d.q(s.q);d.dq(s.dq);d.tau(s.tau);d.kp(s.kp);d.kd(s.kd);d.weight(s.weight);}return true;});
}
inline std::uint32_t copy(NativeState const& v,State& out)noexcept {
 if(v.motor_state_parallel().size()>29||v.motor_state_serial().size()>29)return 90;
 return transaction(out,[&](auto& n){if(!finite(v.imu_state().rpy())||!finite(v.imu_state().gyro())||!finite(v.imu_state().acc()))return false;
 for(unsigned i{};i<3;++i){n.imu_state.rpy[i]=v.imu_state().rpy()[i];n.imu_state.gyro[i]=v.imu_state().gyro()[i];n.imu_state.acc[i]=v.imu_state().acc()[i];}
 auto rows=[](auto const& a,auto& b){b.resize(a.size());for(std::size_t i{};i<a.size();++i){auto const& s=a[i];auto& d=b[i];if(!std::isfinite(s.q())||!std::isfinite(s.dq())||!std::isfinite(s.ddq())||!std::isfinite(s.tau_est()))return false;d.mode=s.mode();d.q=s.q();d.dq=s.dq();d.ddq=s.ddq();d.tau_est=s.tau_est();d.temperature=s.temperature();d.lost=s.lost();for(unsigned k{};k<2;++k)d.reserve[k]=s.reserve()[k];}return true;};
 return rows(v.motor_state_parallel(),n.motor_state_parallel)&&rows(v.motor_state_serial(),n.motor_state_serial);});
}
inline std::uint32_t copy(State const& v,NativeState& out)noexcept {
 if(v.motor_state_parallel.size()>29||v.motor_state_serial.size()>29)return 90;
 return transaction(out,[&](auto& n){if(!finite(v.imu_state.rpy)||!finite(v.imu_state.gyro)||!finite(v.imu_state.acc))return false;
 for(unsigned i{};i<3;++i){n.imu_state().rpy()[i]=v.imu_state.rpy[i];n.imu_state().gyro()[i]=v.imu_state.gyro[i];n.imu_state().acc()[i]=v.imu_state.acc[i];}
 auto rows=[](auto const& a,auto& b){b.resize(a.size());for(std::size_t i{};i<a.size();++i){auto const& s=a[i];auto& d=b[i];if(!std::isfinite(s.q)||!std::isfinite(s.dq)||!std::isfinite(s.ddq)||!std::isfinite(s.tau_est))return false;d.mode(s.mode);d.q(s.q);d.dq(s.dq);d.ddq(s.ddq);d.tau_est(s.tau_est);d.temperature(s.temperature);d.lost(s.lost);for(unsigned k{};k<2;++k)d.reserve()[k]=s.reserve[k];}return true;};
 return rows(v.motor_state_parallel,n.motor_state_parallel())&&rows(v.motor_state_serial,n.motor_state_serial());});
}
}
