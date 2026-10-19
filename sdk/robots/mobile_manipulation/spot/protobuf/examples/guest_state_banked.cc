/* SPDX-License-Identifier: Apache-2.0
 * Complete C++20 Spot State caller using the genuine bounded C field decoder.
 *
 * This is a C++ application linked to the actual nanopb C SDK. It is separate
 * from the protobuf Lite C++ class API, whose WASI profile requires the real
 * threaded/EH toolchain. No mutex, exception, ROS or native SDK stub is used.
 * argv[1] is one independently authorized State service alias. Standard input
 * contains token, nonce and budget_ns as three little-endian u64 values.
 * No credentials, robot address, lease or motion operation is selected here.
 */
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <wasi/api.h>
#include <roswasm/ros2.h>
#include "../../sdk.hpp"
#include "robot_state.h"

namespace
{
    alignas(std::max_align_t) unsigned char bank0[1048576], bank1[1048576];
    uwvm_spot_proto_document document{};
    uwvm_mobile::spot::Client client;
    uwvm_mobile::spot::StateResponse response{};
    struct outcome
    {
        std::uint32_t primary{}, transport{}, provider{}, unknown{}, decode{}, cleanup{};
    } report;

    // Direct WASI input keeps the basic stdin authority explicit. Partial reads
    // are ordinary input progress; they never retry a ROS request or acquire I/O.
    bool exact(unsigned char* destination, std::size_t remaining) noexcept
    {
        while(remaining)
        {
            __wasi_iovec_t part{destination, remaining};
            __wasi_size_t received{};
            if(__wasi_fd_read(0, &part, 1, &received) || !received || received > remaining) { return false; }
            destination += received;
            remaining -= received;
        }
        return true;
    }
}

int main(int argc, char** argv)
{
    unsigned char input[24];
    if(argc != 2 || !argv[1] || !argv[1][0] || std::strlen(argv[1]) > 255 || !exact(input, sizeof input)) { return 22; }
    uwvm_mobile::spot::StateRequest request{};
    request.token = uwvm_ros_load_u64(input);
    request.nonce = uwvm_ros_load_u64(input + 8);
    request.budget_ns = uwvm_ros_load_u64(input + 16);
    if(!request.token || !request.nonce || !request.budget_ns || request.budget_ns > 5000000000ull) { return 22; }

    // Both banks and the complete response exist before any capability open.
    // The caller owns these linear-memory objects; they are not Host pointers.
    report.primary = uwvm_spot_proto_init(&document, bank0, bank1, sizeof bank0);
    if(report.primary) { return static_cast<int>(report.primary); }
    roswasmc_context context{};
    context.backend = roswasmc_ros2_backend();
    roswasmc_capability source{};
    roswasmc_capability_init(&source, &context);
    report.primary = roswasmc_open(&source, argv[1], std::strlen(argv[1]));
    if(!report.primary)
    {
        auto const result{client.state(source, request, response)};
        report.transport = result.error;
        if(result.error) { report.primary = result.error; }
        else
        {
            // Provider refusal and uncertain outcome are independent of ROS
            // transport success. Neither permits interpreting stale payloads.
            report.provider = response.error;
            report.unknown = response.unknown;
            report.primary = response.error ? response.error : response.unknown ? 115u : 0u;
            if(!report.primary)
            {
                report.decode = uwvm_spot_robot_state_decode(&document, response.native_proto.data, response.native_proto.size);
                report.primary = report.decode;
                // uwvm_spot_robot_state_value(&document) exposes every genuine
                // generated C field after success. Its borrow survives a failed
                // decode, and ends on successful replacement or explicit clear.
            }
        }
    }
    if(source.handle) { report.cleanup = roswasmc_close(&source); }
    if(!report.primary) { report.primary = report.cleanup; }
    return report.primary ? static_cast<int>(report.primary < 126 ? report.primary : 125) : 0;
}
