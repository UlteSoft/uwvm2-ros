#pragma once
#include "basic_io.hpp"
#include <fast_io.h>
#include <cstdint>
#include <string_view>
namespace humanoid_values {
template<class... A> std::uint32_t line(A const&... args){auto text=fast_io::concat_std(args...,"\n");return humanoid_io::output(text.data(),text.size());}
template<class V> std::uint32_t command(V const& v){
 if(auto e=line("cmd_type=",v.cmd_type," motor_cmd.size=",v.motor_cmd.size))return e;
 for(std::size_t i{};i<v.motor_cmd.size;++i){auto const& m=v.motor_cmd.data[i];if(auto e=line("motor[",i,"] mode=",static_cast<unsigned>(m.mode)," q=",m.q," dq=",m.dq," tau=",m.tau," kp=",m.kp," kd=",m.kd," weight=",m.weight))return e;}
 return 0;
}
template<class V> std::uint32_t state(V const& v){
 for(unsigned i{};i<3;++i)if(auto e=line("imu[",i,"] rpy=",v.imu_state.rpy[i]," gyro=",v.imu_state.gyro[i]," acc=",v.imu_state.acc[i]))return e;
 auto rows=[](char const* name,auto const& rows)->std::uint32_t{
  if(auto e=line(name,".size=",rows.size))return e;
  for(std::size_t i{};i<rows.size;++i){auto const& m=rows.data[i];if(auto e=line(name,"[",i,"] mode=",static_cast<unsigned>(m.mode)," q=",m.q," dq=",m.dq," ddq=",m.ddq," tau_est=",m.tau_est," temperature=",static_cast<unsigned>(m.temperature)," lost=",m.lost," reserve[0]=",m.reserve[0]," reserve[1]=",m.reserve[1]))return e;}
  return 0;
 };
 if(auto e=rows("parallel",v.motor_state_parallel))return e;
 return rows("serial",v.motor_state_serial);
}
}
