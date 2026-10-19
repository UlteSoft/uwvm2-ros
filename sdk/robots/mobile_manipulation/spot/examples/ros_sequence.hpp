/** C++ uses the SAME full generated records and checked C path, not native casts. */
#pragma once
#include "ros_sequence.h"
#include "../sdk.hpp"
namespace uwvm_mobile::spot {
using SequenceReport=mm_spot_sequence_report;
inline std::uint32_t sequence(roswasmc_context const& context,
    char const* const (&aliases)[8],std::uint64_t token,std::uint64_t nonce,
    double x_mps,double y_mps,double yaw_rps,std::uint64_t duration_ns,
    SequenceReport& report,mm_spot_buffers& buffers) noexcept {
    return mm_spot_sequence(&context,aliases,token,nonce,x_mps,y_mps,yaw_rps,duration_ns,&report,&buffers);
}
}
