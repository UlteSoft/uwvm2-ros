/* SPDX-License-Identifier: Apache-2.0
 * Borrowed whole-state reader plus all original generated service/action facades.
 */
#ifndef UWVM_TURTLEBOT3_STATE_HPP
#define UWVM_TURTLEBOT3_STATE_HPP
#include "state.h"
#include "bounded_interfaces.hpp"
namespace roswasm_turtlebot3 {
using State=uwvm_turtlebot3_state;

/** Borrow an already explicitly owned capability; serialized poll, no auto-open. */
class Reader {
 roswasmc_capability const* cap_;uwvm_turtlebot3_storage scratch_{};
public:
 explicit Reader(roswasmc_capability const& cap) noexcept:cap_(&cap){}
 [[nodiscard]] std::uint32_t poll(State& value) noexcept {return uwvm_turtlebot3_poll_state(cap_,&value,&scratch_);}
};
}
#endif
