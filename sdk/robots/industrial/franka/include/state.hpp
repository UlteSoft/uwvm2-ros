/* SPDX-License-Identifier: Apache-2.0
 * Borrowed whole-state reader plus all original generated service/action facades.
 */
#ifndef UWVM_FRANKA_STATE_HPP
#define UWVM_FRANKA_STATE_HPP
#include "state.h"
#include "bounded_interfaces.hpp"
namespace roswasm_franka {
using State=uwvm_franka_state;
using GraspGoal=uwvm_franka_grasp_goal;
/** Borrow an already explicitly owned capability; serialized poll, no auto-open. */
class Reader {
 roswasmc_capability const* cap_;uwvm_franka_storage scratch_{};
public:
 explicit Reader(roswasmc_capability const& cap) noexcept:cap_(&cap){}
 [[nodiscard]] std::uint32_t poll(State& value) noexcept {return uwvm_franka_poll_state(cap_,&value,&scratch_);}
};
}
#endif
