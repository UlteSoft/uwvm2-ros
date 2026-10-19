/** @file
 * @brief C11: encode one generated fixed Packet and publish one explicit alias.
 * The bundled guest-only generated-r3 codec is sufficient for compilation. Add the SDK
 * include directory to the Wasm C compiler. The host must grant `packet-pub` as
 * ROS1 example_interfaces_custom/Packet with its official MD5, generated
 * adapter-codec-identity and all normal Topic quotas. Default policy denies it.
 * Real delivery additionally needs official native identities and a reviewed
 * registered adapter. Compilation cannot provide that native registration.
 */
#include <roswasm/ros1.h>
#include "../generated-r3/fixed_records.h"

/** @brief Validate before publication; close once on every successful open. */
int main(void) {
    roswasmc_context context={roswasmc_ros1_backend()};
    roswasmc_capability publisher;
    roswasmc_custom_1_example_interfaces_custom_Packet packet={0};
    unsigned char bytes[ROSWASMC_CUSTOM_1_EXAMPLE_INTERFACES_CUSTOM_PACKET_WIRE_BYTES];
    size_t length;uint32_t error,close_error;
    packet.sequence=7;packet.active=1;packet.measurement=1.25;
    packet.tag[0]=0x41;packet.samples[0]=-3;
    // ABI negotiation creates no resource or permission.
    if (uwvm_ros1_abi_version()!=1) return UWVM_ROS_UNSUPPORTED;
    // Caller-owned encoding scratch does not enlarge the host byte quota.
    error=roswasmc_custom_1_example_interfaces_custom_Packet_encode(&packet,bytes,sizeof bytes,&length);
    if(error) return (int)error;
    roswasmc_capability_init(&publisher,&context);
    error=roswasmc_open(&publisher,"packet-pub",10);if(error) return (int)error;
    error=roswasmc_publish(&publisher,bytes,length);
    close_error=roswasmc_close(&publisher);return (int)(error?error:close_error);
}
