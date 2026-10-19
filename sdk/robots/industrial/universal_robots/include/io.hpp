/* SPDX-License-Identifier: Apache-2.0
 * Typed whole I/O reader. The generated interfaces header also exposes every
 * selected original service/action facade; there is no native UR binary ABI.
 */
#ifndef UWVM_UR_IO_HPP
#define UWVM_UR_IO_HPP
#include "io.h"
#include "bounded_interfaces.hpp"
namespace roswasm_ur {
using IOState=uwvm_ur_io_state;
using SpeedRequest=uwvm_ur_speed_request;
/** Borrow an explicitly owned capability; construction opens no resource.
 * Access is serialized. Poll uses real generated Codec<T>, not a prefix view.
 */
class Reader {
    roswasmc_capability const* cap_;
    uwvm_ur_storage scratch_{};
public:
    explicit Reader(roswasmc_capability const& cap) noexcept:cap_(&cap){}
    [[nodiscard]] std::uint32_t poll(IOState& value) noexcept {
        return uwvm_ur_poll_io(cap_,&value,&scratch_);
    }
};
}
#endif
