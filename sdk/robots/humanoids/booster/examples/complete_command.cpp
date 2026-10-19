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
using Command=uwvm_custom::Ros2Uwvm_booster_sdk_bridgeMsgLowCmd;
int main(){
 if(uwvm_ros2_abi_version()!=1)return 95;
 roswasmc_context context{roswasmc_ros2_backend()};
 if(!UWVM_HUMANOID_POSITIVE){roswasmcpp::Publisher<Command> publisher{context,"booster-sdk-lowcmd"};auto e=publisher.error();if(e==13&&!publisher.is_open())return 0;if(publisher.is_open())publisher.close();return 22;}
 std::array<unsigned char,32768> bytes{};unsigned char length[4]{};
 auto primary=humanoid_io::input(length,4);auto n=humanoid_io::little32(length);
 if(primary)return primary;if(!n||n>bytes.size()||n>roswasmcpp::Codec<Command>::max_wire_bytes)return 90;
 primary=humanoid_io::input(bytes.data(),n);if(primary)return primary;
 Command command{};if(!roswasmcpp::Codec<Command>::decode({bytes.data(),n},command)||!(command.motor_cmd.size==23&&(command.cmd_type==0||command.cmd_type==1)))return 22;
 try{if(auto e=humanoid_values::command(command))return static_cast<int>(e);}catch(std::bad_alloc const&){return 12;}catch(...){return 22;}
 roswasmcpp::Publisher<Command> publisher{context,"booster-sdk-lowcmd"};primary=publisher.error();
 if(!primary)primary=publisher.publish(command); // exactly one attempt
 auto cleanup=publisher.is_open()?publisher.close():0u;
 auto report=fast_io::concat_std("HUMANOID_COMMAND primary=",primary," close=",cleanup,"\n");
 auto output=humanoid_io::output(report.data(),report.size());
 return static_cast<int>(primary?primary:cleanup?cleanup:output);
}
