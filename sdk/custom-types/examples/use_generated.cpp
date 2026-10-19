/** @file
 * @brief C++20: use a generated record with the existing roswasmcpp Node API.
 * Build against the bundled guest-only generated-r3 codec. Real delivery
 * additionally requires official identities and a reviewed native adapter.
 * Native resource/type/identity/QoS are selected by the explicitly granted alias.
 * No C++ ROS library is linked; the single publish is the explicit ROS I/O.
 * Guest compilation cannot establish native registration or delivery.
 */
#include <roswasm/ros1.h>
#include "../generated-r3/fixed_records.hpp"

/** @brief Create one RAII publisher; generated Codec performs exact validation. */
int main() {
    // Reject an incompatible ABI before any alias acquisition.
    if (uwvm_ros1_abi_version()!=1) return UWVM_ROS_UNSUPPORTED;
    roswasmcpp::Node node{*roswasmc_ros1_backend(),"fixed-packet"};
    auto publisher=node.create_publisher<uwvm_custom::Ros1Example_interfaces_customPacket>("packet-pub");
    if(publisher.error()) return static_cast<int>(publisher.error());
    uwvm_custom::Ros1Example_interfaces_customPacket packet{};
    packet.sequence=7;packet.active=1;packet.measurement=1.25;
    packet.tag[0]=0x41;packet.samples[0]=-3;
    auto error=publisher.publish(packet);
    auto close_error=publisher.close();return static_cast<int>(error?error:close_error);
}
