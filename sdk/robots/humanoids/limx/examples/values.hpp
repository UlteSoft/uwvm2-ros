#pragma once
#include "basic_io.hpp"
#include <fast_io.h>
#include <cstdint>
#include <string_view>
namespace humanoid_values {
template<class... A> std::uint32_t line(A const&... args){auto text=fast_io::concat_std(args...,"\n");return humanoid_io::output(text.data(),text.size());}
template<class V> std::uint32_t header(V const& h){return line("header.seq=",h.seq," sec=",h.stamp_sec," nsec=",h.stamp_nsec," frame_id=",std::string_view{h.frame_id.data,h.frame_id.size});}
template<class V> std::uint32_t names(V const& v){
 if(auto e=line("na=",v.na," names.size=",v.names.size))return e;
 for(std::size_t i{};i<v.names.size;++i)if(auto e=line("name[",i,"]=",std::string_view{v.names.data[i].data,v.names.data[i].size}))return e;
 return 0;
}
template<class V> std::uint32_t command(V const& v){
 if(auto e=header(v.header))return e;if(auto e=names(v))return e;
 // Canonical decoding bounds every sequence. Model consistency is checked by
 // the actual Native converter too; do not index using an unchecked na value.
 if(v.q.size!=v.na||v.v.size!=v.na||v.tau.size!=v.na||v.kp.size!=v.na||v.kd.size!=v.na||v.mode.size!=v.na)return 22;
 for(std::size_t i{};i<v.na;++i)if(auto e=line("joint[",i,"] q=",v.q.data[i]," v=",v.v.data[i]," tau=",v.tau.data[i]," kp=",v.kp.data[i]," kd=",v.kd.data[i]," mode=",static_cast<unsigned>(v.mode.data[i])))return e;
 return 0;
}
template<class V> std::uint32_t state(V const& v){
 if(auto e=header(v.header))return e;if(auto e=names(v))return e;
 if(v.q.size!=v.na||v.v.size!=v.na||v.vd.size!=v.na||v.tau.size!=v.na)return 22;
 for(std::size_t i{};i<v.na;++i)if(auto e=line("joint[",i,"] q=",v.q.data[i]," v=",v.v.data[i]," vd=",v.vd.data[i]," tau=",v.tau.data[i]))return e;
 return 0;
}
}
