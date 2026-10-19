/** Complete caller-supplied canonical command, one real publish, no replay.
 * Default0 verifies denied authority before reading input or publishing.
 * Positive mode is an explicitly selected simulator/application lane; it is
 * not a controller, guessed posture, mode transition or hardware stop request.
 */
#include <roswasm/ros2.h>
#include "bounded_records.hpp"
#include "basic_io.hpp"
#include "values.hpp"
#include <fast_io.h>
#include <new>
#include <array>
#include <span>
#ifndef UWVM_HUMANOID_POSITIVE
#define UWVM_HUMANOID_POSITIVE 0
#endif
static_assert(UWVM_HUMANOID_POSITIVE==0||UWVM_HUMANOID_POSITIVE==1);
using Command=uwvm_custom::Ros2Uwvm_limx_bridgeMsgJointCmd;
int main(){
 if(uwvm_ros2_abi_version()!=1)return 95;

 if(!UWVM_HUMANOID_POSITIVE){
  constexpr char alias[]{"limx-command"};std::array<unsigned char,8> slot{};slot.fill(0xff);
  auto e=uwvm_ros2_capability_open(0,static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(alias)),sizeof(alias)-1,
   static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(slot.data())));
  if(e==13){for(auto b:slot)if(b!=0xff)return 22;return 0;}
  if(!e){std::uint64_t h{};for(unsigned i{};i<8;++i)h|=static_cast<std::uint64_t>(slot[i])<<(8*i);if(h&&h!=UINT64_MAX)(void)uwvm_ros2_capability_close(h);}
  return 22;
 }
 std::array<unsigned char,32768> bytes{};unsigned char length[4]{};
 auto primary=humanoid_io::input(length,4);auto n=humanoid_io::little32(length);
 if(primary)return primary;if(!n||n>bytes.size()||n>roswasmcpp::Codec<Command>::max_wire_bytes)return 90;
 primary=humanoid_io::input(bytes.data(),n);if(primary)return primary;
 Command command{};if(!roswasmcpp::Codec<Command>::decode({bytes.data(),n},command)||!(command.na==6))return 22;
 try{if(auto e=humanoid_values::command(command))return static_cast<int>(e);}catch(std::bad_alloc const&){return 12;}catch(...){return 22;}
 // Raw ABI uses full-width handles/offsets. It never receives a native C++
 // object. Only the complete generated canonical bytes cross this import.
 std::array<unsigned char,8> slot{};slot.fill(0xff);
 constexpr char alias[]{"limx-command"};
 primary=uwvm_ros2_capability_open(0,static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(alias)),sizeof(alias)-1,
  static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(slot.data())));
 std::uint64_t handle{};for(unsigned i{};i<8;++i)handle|=static_cast<std::uint64_t>(slot[i])<<(8*i);
 if(primary)return static_cast<int>(primary);
 if(!handle||handle==UINT64_MAX)return 22; // protocol failure; outer Source owns unknown retirement
 primary=uwvm_ros2_topic_publish(handle,0,static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(bytes.data())),n);
 // An error can follow a real publish; no replay. Closing also retires locally
 // before cleanup, so this is one attempt, with a separate retained result.
 auto cleanup=uwvm_ros2_capability_close(handle);
 auto report=fast_io::concat_std("HUMANOID_RAW_COMMAND primary=",primary," close=",cleanup,"\n");
 auto output=humanoid_io::output(report.data(),report.size());
 return static_cast<int>(primary?primary:cleanup?cleanup:output);
}
