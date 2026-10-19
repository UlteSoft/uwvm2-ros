/** Concrete C++20 use of the current complete builtin Twist SDK type.
 * This is bounded telemetry, not a command publisher or a robot stop policy.
 * Supply the actually selected ROS backend, an explicitly granted simulator
 * subscription alias and an independently reviewed full-value native oracle.
 * Construct a separate fresh instance for default-denied mode0 and positive
 * mode1. The observer may inspect all six finite fields; it must not reenter
 * this active reader. The owned external controller drives at most4096 polls.
 */
#pragma once
#include "telemetry.hpp"
namespace uwvm_vendor {
using TwistTelemetry=Telemetry<roswasmcpp::msg::Twist>;
/** The exact current canonical Twist extent is48 bytes, independent of C++
 * struct padding. No native pointer, field offset, permission or SDK/RDK model
 * inference follows from selecting this existing Guest codec. */
inline TwistTelemetry make_twist_telemetry(roswasmc_backend const& backend,
    uint32_t (*observe)(roswasmcpp::msg::Twist const&)) {
    return TwistTelemetry{backend,48,observe};
}
}
