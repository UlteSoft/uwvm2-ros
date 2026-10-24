/** @file recovery_protocol.h
 * @brief Fixed public application values for controlled single-backend recovery.
 * Payloads distinguish stages, so a duplicated baseline cannot be mistaken for
 * delivery after a fault. Bytes are serialized by the official String codec;
 * native structs and connection identities are never copied into Guest memory.
 */
#pragma once
#include <array>
#include <string_view>
namespace recovery_protocol
{
    inline constexpr std::array<std::string_view,3> stages{{"before","during","after"}};
    inline constexpr std::array<std::string_view,3> outbound{{"roswasm-js-relay-out","roswasm-js-relay-out-during","roswasm-js-relay-out-after"}};
    inline constexpr std::array<std::string_view,3> inbound{{"roswasm-js-relay-in","roswasm-js-relay-in-during","roswasm-js-relay-in-after"}};
}
