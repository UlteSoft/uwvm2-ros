/** Raw numeric Core ABI capability attempt with unchanged output sentinel. */
#include <roswasm/abi/ros2.h>
#include <array>
#include <cstdint>
int main(){
 if(uwvm_ros2_abi_version()!=1)return 95;
 constexpr char alias[]{"nvidia-apriltags"};std::array<unsigned char,8> out{};out.fill(0xff);
 auto error=uwvm_ros2_capability_open(0,static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(alias)),sizeof(alias)-1,static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(out.data())));
 if(error==13){for(auto b:out)if(b!=0xff)return 22;return 0;}
 if(!error){std::uint64_t handle{};for(unsigned i{};i<8;++i)handle|=std::uint64_t{out[i]}<<(i*8);if(handle&&handle!=UINT64_MAX)(void)uwvm_ros2_capability_close(handle);}
 return 22;
}
