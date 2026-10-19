/** Actual Wasm ROS2 ABI81 -> fully generated AprilTag records. No GPU pointer. */
#include <roswasm/ros2.h>
#include "bounded_records.hpp"
#include "full_values.hpp"
#include <new>
#ifndef UWVM_NVIDIA_POSITIVE
#define UWVM_NVIDIA_POSITIVE 0
#endif
static_assert(UWVM_NVIDIA_POSITIVE==0||UWVM_NVIDIA_POSITIVE==1);
using Value=uwvm_custom::Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray;
int main(){
 if(uwvm_ros2_abi_version()!=1)return 95;roswasmc_context context{roswasmc_ros2_backend()};
 roswasmcpp::Subscription<Value> s{context,"nvidia-apriltags",roswasmcpp::Codec<Value>::max_wire_bytes};
 auto primary=s.error();if(!UWVM_NVIDIA_POSITIVE){if(primary==13&&!s.is_open())return 0;if(s.is_open())s.close();return 22;}
 if(primary)return primary;unsigned received{};
 s.on_message([&](Value const& value){try{primary=nvidia_values::detections(value);if(!primary)++received;}catch(std::bad_alloc const&){primary=12;}catch(...){primary=22;}});
 // Actual caller-driven finite observations, not iterations pretending to be ns.
 for(unsigned i{};!primary&&!received&&i<64;++i){char command{};primary=humanoid_io::input(&command,1);if(primary||command!='R'){if(!primary)primary=22;break;}auto n=s.poll_once();if(!primary&&n!=11)primary=n;}
 if(!primary&&!received)primary=110;auto cleanup=s.close();auto out=nvidia_values::line("primary=",primary," close=",cleanup," rollback=",s.rollback_error());return primary?primary:cleanup?cleanup:out;
}
