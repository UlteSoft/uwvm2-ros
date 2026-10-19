/** A disabled native Crypto dependency must keep RIHS derivation unavailable.
 * Compile the actual production header without OpenSSL, with assertions active.
 * This pure refusal contract creates no native ROS/Runtime process or entity.
 */
#if defined(UWVM_ROS2_HAVE_OPENSSL) && UWVM_ROS2_HAVE_OPENSSL
#error "This refusal fixture requires the actual no-Crypto profile"
#endif
#ifndef UWVM_ROS2_HAVE_OPENSSL
#define UWVM_ROS2_HAVE_OPENSSL 0
#endif
#include "../../gateways/ros2/compat/rihs.h"
#include <array>
#include <cassert>
#ifdef NDEBUG
#error "Compile the native identity contract with assertions enabled (-UNDEBUG)"
#endif

int main()
{
    namespace ipc=uwvm2::imported::ros::ipc;
    namespace compat=uwvm2::ros_gateway::ros2_compat;
    constexpr ipc::custom_native_field fields[]{{"value","int32"}};
    ipc::custom_native_record const rows[]{{"example/msg/Value",fields,1}};
    std::array<unsigned char,32> output{};output.fill(0xa5);auto before=output;
    assert(!compat::rihs_digest({rows,1},rows[0].type,output)&&output==before);
    assert(compat::rihs_identity({rows,1},rows[0].type).empty());
    assert(!compat::matches_rihs({rows,1},rows[0].type,
        "RIHS01_0000000000000000000000000000000000000000000000000000000000000000"));
}
