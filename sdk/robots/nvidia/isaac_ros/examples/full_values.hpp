#pragma once
#include "basic_io.hpp"
#include <fast_io.h>
#include <string_view>
namespace nvidia_values {
template<class... A>std::uint32_t line(A const&... a){auto s=fast_io::concat_std(a...,"\n");return humanoid_io::output(s.data(),s.size());}
template<class H>std::uint32_t header(H const& h){return line("sec=",h.stamp.sec," nanosec=",h.stamp.nanosec," frame_id=",std::string_view{h.frame_id.data,h.frame_id.size});}
template<class P>std::uint32_t point(P const& p){return line("x=",p.x," y=",p.y," z=",p.z);}
template<class A>std::uint32_t detections(A const& v){
 if(auto e=header(v.header))return e;if(auto e=line("detections=",v.detections.size))return e;
 for(std::size_t i{};i<v.detections.size;++i){auto const& d=v.detections.data[i];
  if(auto e=line("family=",std::string_view{d.family.data,d.family.size}," id=",d.id))return e;
  if(auto e=point(d.center))return e;for(auto const& p:d.corners)if(auto e=point(p))return e;
  if(auto e=header(d.pose.header))return e;auto const& pose=d.pose.pose.pose;
  if(auto e=point(pose.position))return e;if(auto e=line("qx=",pose.orientation.x," qy=",pose.orientation.y," qz=",pose.orientation.z," qw=",pose.orientation.w))return e;
  for(auto c:d.pose.pose.covariance)if(auto e=line("covariance=",c))return e;
 }return 0;
}
}
