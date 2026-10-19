// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <rosidl_runtime_c/string.h>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#if __has_include(<rosidl_runtime_cpp/bounded_vector.hpp>)
#include <rosidl_runtime_cpp/bounded_vector.hpp>
#elif __has_include(<rosidl_generator_cpp/bounded_vector.hpp>)
#include <rosidl_generator_cpp/bounded_vector.hpp>
#else
#error "Real installed original bounded sequence container required"
#endif
namespace uwvm_default_control_gateway_detail {
template<class T>struct native_sequence_bound;
template<class T,class A>struct native_sequence_bound<std::vector<T,A>>{static constexpr std::size_t value=0;};
#if __has_include(<rosidl_runtime_cpp/bounded_vector.hpp>)
template<class T,std::size_t N,class A>struct native_sequence_bound<rosidl_runtime_cpp::BoundedVector<T,N,A>>{static constexpr std::size_t value=N;};
#else
template<class T,std::size_t N,class A>struct native_sequence_bound<rosidl_generator_cpp::BoundedVector<T,N,A>>{static constexpr std::size_t value=N;};
#endif
inline std::uint32_t copy_text(rosidl_runtime_c__String const&v,unsigned char*out,std::size_t bound,std::uint32_t&count)noexcept {
 if(v.size>v.capacity||(!v.data&&(v.size||v.capacity)))return 22;
 if(v.data&&(v.capacity<=v.size||v.data[v.size]!=0))return 22;
 if(v.size>bound)return 90;if(v.size)std::memcpy(out,v.data,v.size);count=static_cast<std::uint32_t>(v.size);return 0;
}
inline std::uint32_t copy_text(std::string const&v,unsigned char*out,std::size_t bound,std::uint32_t&count)noexcept {
 if(v.size()>bound)return 90;if(!v.empty())std::memcpy(out,v.data(),v.size());count=static_cast<std::uint32_t>(v.size());return 0;
}
// Actual complete canonical validator performs strict UTF-8/NUL/bool/finite
// checks after full object conversion and before any public byte/length write.
}
