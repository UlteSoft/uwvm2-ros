/** @file Complete C++20 passive LowCmd publication through real Wasm imports.
 * Compile with -DUWVM_UNITREE_FAMILY=1 (GO) or2 (HG) and the selected external
 * generated bounded_records.hpp include directory.  The default build verifies
 * empty authority: topic open must return13 and no publication is attempted.
 * An explicitly selected -DUWVM_UNITREE_SEND=1 build sends exactly one all-zero
 * motor-mode command, with a checked official-layout CRC, and checks cleanup.
 *
 * This process does not choose a ROS domain/interface or grant an alias.  The
 * independent Root driver must restrict the Gateway to domain1/loopback SIM,
 * prove its peer ready, and grant exactly unitree-lowcmd topic.publish.  A CRC
 * does not validate a robot's joint mapping, torque limits, safety, or authority.
 * There is no timer, hidden retry, background thread or physical-device setup.
 */
#include <roswasm/ros2.h>
#include <roswasm_unitree/crc.hpp>
#include "bounded_records.hpp"
#ifndef UWVM_UNITREE_SEND
#define UWVM_UNITREE_SEND 0
#endif
#if UWVM_UNITREE_FAMILY == 1
using Command=uwvm_custom::Ros2Unitree_goMsgLowCmd;
#elif UWVM_UNITREE_FAMILY == 2
using Command=uwvm_custom::Ros2Unitree_hgMsgLowCmd;
#else
#error "Explicit GO1 or HG2 family required; no robot/distro inference"
#endif
int main() {
    if(uwvm_ros2_abi_version()!=1)return 95;
    roswasmc_context context{roswasmc_ros2_backend()};
    roswasmcpp::Publisher<Command> publisher{context,"unitree-lowcmd"};
    auto error=publisher.error();
    if(!UWVM_UNITREE_SEND){
        if(error==13&&!publisher.is_open())return 0;
        if(publisher.is_open()){auto cleanup=publisher.close();(void)cleanup;}
        return 22;
    }
    if(error)return static_cast<int>(error);
    Command command{};
#if UWVM_UNITREE_FAMILY == 1
    command.head[0]=0xfe;command.head[1]=0xef;command.level_flag=0xff;
    error=roswasm_unitree::set_go_lowcmd_crc(command);
#else
    error=roswasm_unitree::set_hg_lowcmd_crc(command);
#endif
    if(!error)error=publisher.publish(command);
    auto cleanup=publisher.close();if(!error)error=cleanup;
    return static_cast<int>(error);
}
