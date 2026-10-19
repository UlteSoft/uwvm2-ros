/** One complete standard JointState to an explicitly selected simulation topic.
 * Does not create a real robot owner, mode switch or safety-stop capability. */
#include <roswasm/ros2.h>
#include "bounded_records.hpp"
#include "basic_io.hpp"
#include <fast_io.h>
#include <array>
#include <string_view>
#ifndef UWVM_NVIDIA_POSITIVE
#define UWVM_NVIDIA_POSITIVE 0
#endif
static_assert(UWVM_NVIDIA_POSITIVE==0||UWVM_NVIDIA_POSITIVE==1);
using Value=uwvm_custom::Ros2Sensor_msgsMsgJointState;
int main(){
 if(uwvm_ros2_abi_version()!=1)return 95;roswasmc_context context{roswasmc_ros2_backend()};
 // The whole private value is prepared before any positive capability open.
 Value value{};std::array<unsigned char,16384> bytes{};unsigned char length[4]{};std::uint32_t primary{};
 if(UWVM_NVIDIA_POSITIVE){primary=humanoid_io::input(length,4);if(primary)return primary;auto n=humanoid_io::little32(length);if(!n||n>bytes.size()||n>roswasmcpp::Codec<Value>::max_wire_bytes)return 90;primary=humanoid_io::input(bytes.data(),n);if(primary)return primary;if(!roswasmcpp::Codec<Value>::decode({bytes.data(),n},value))return 22;
  // Genuine current Isaac Sim JointState subscriber requires all three arrays
  // to cover all named actuators. Do not silently project a partial command.
  if(!value.name.size||value.position.size!=value.name.size||value.velocity.size!=value.name.size||value.effort.size!=value.name.size)return 22;
  for(std::size_t i{};i<value.name.size;++i){auto text=fast_io::concat_std("joint=",std::string_view{value.name.data[i].data,value.name.data[i].size}," q=",value.position.data[i]," v=",value.velocity.data[i]," effort=",value.effort.data[i],"\n");if(auto e=humanoid_io::output(text.data(),text.size()))return e;}
 }
 roswasmcpp::Publisher<Value> p{context,"nvidia-sim-joint-command"};primary=p.error();
 if(!UWVM_NVIDIA_POSITIVE){if(primary==13&&!p.is_open())return 0;if(p.is_open())p.close();return 22;}
 if(!primary)primary=p.publish(value);auto cleanup=p.close();auto report=fast_io::concat_std("SIM_COMMAND primary=",primary," close=",cleanup,"\n");auto out=humanoid_io::output(report.data(),report.size());return primary?primary:cleanup?cleanup:out;
}
