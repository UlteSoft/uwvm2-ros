/** Checked finite WASIp1 binary/text I/O. Exact real imports, no host stub. */
#pragma once
#include <cstdint>
#include <cstddef>
#if !defined(__wasm__) || !defined(__clang__) || UINTPTR_MAX != UINT32_MAX
#error "These Guest applications require the real Clang wasm32 WASIp1 lane"
#endif
namespace humanoid_io {
struct Vec {void* pointer;std::size_t bytes;};
extern "C" __attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_read")))
std::uint32_t read(std::uint32_t,Vec const*,std::uint32_t,std::uint32_t*);
extern "C" __attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_write")))
std::uint32_t write(std::uint32_t,Vec const*,std::uint32_t,std::uint32_t*);
inline std::uint32_t input(void* data,std::size_t size) {
 if(size>32772)return 90;std::size_t at{};
 for(unsigned attempts{};at<size&&attempts<4096;++attempts){std::uint32_t n=UINT32_MAX;Vec v{static_cast<unsigned char*>(data)+at,size-at};auto e=read(0,&v,1,&n);if(e==27)continue;if(e)return e;if(!n||n>size-at)return 22;at+=n;}
 return at==size?0:110;
}
inline std::uint32_t output(char const* data,std::size_t size) {
 if(size>4096)return 90;std::size_t at{};
 for(unsigned attempts{};at<size&&attempts<4096;++attempts){std::uint32_t n=UINT32_MAX;Vec v{const_cast<char*>(data+at),size-at};auto e=write(1,&v,1,&n);if(e==27)continue;if(e)return e;if(!n||n>size-at)return 107;at+=n;}
 return at==size?0:110;
}
inline std::uint32_t little32(unsigned char const* p){return p[0]|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
}
