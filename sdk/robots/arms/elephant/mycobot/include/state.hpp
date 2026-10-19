/* SPDX-License-Identifier: Apache-2.0
 * Borrowed whole-state reader plus all original generated service/action facades.
 */
#ifndef UWVM_ELEPHANT_STATE_HPP
#define UWVM_ELEPHANT_STATE_HPP
#include "state.h"
#include "bounded_interfaces.hpp"
namespace roswasm_elephant {
using State=uwvm_elephant_state;

/** Borrow an already explicitly owned capability; serialized poll, no auto-open. */
class Reader {
 roswasmc_capability const* cap_;uwvm_elephant_storage scratch_{};
public:
 explicit Reader(roswasmc_capability const& cap) noexcept:cap_(&cap){}
 [[nodiscard]] std::uint32_t poll(State& value) noexcept {return uwvm_elephant_poll_state(cap_,&value,&scratch_);}
};
}
#endif
