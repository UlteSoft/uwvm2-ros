#pragma once
#include <algorithm>
#include "snapshot_reader.h"
#include "../../../src/uwvm2/runtime/lib/uwvm_runtime.h"

#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
namespace uwvm_language_debug {
namespace rt = ::uwvm2::runtime::lib;
struct current_snapshot {
    snapshot value{};
    rt::llvm_jit_debug_source_activation_snapshot actual_position{};
};
inline bool same_position(rt::llvm_jit_debug_source_activation_snapshot const& a,
                          rt::llvm_jit_debug_source_activation_snapshot const& b) noexcept {
    if (!a.source_available || !b.source_available ||
        a.source_context_available!=b.source_context_available ||
        !(a.activation.location==b.activation.location) ||
        a.activation.participant!=b.activation.participant ||
        a.source.module!=b.source.module || a.source.function!=b.source.function ||
        a.source.code_offset!=b.source.code_offset || a.source.runtime_epoch!=b.source.runtime_epoch ||
        a.source.function_generation!=b.source.function_generation ||
        a.activation.frames.empty() || a.activation.frames.size()!=b.activation.frames.size()) return false;
    for (std::size_t n{}; n!=a.activation.frames.size(); ++n) {
        auto const& x=a.activation.frames[n]; auto const& y=b.activation.frames[n];
        if (x.incarnation!=y.incarnation || x.parent!=y.parent || x.continuation!=y.continuation ||
            x.module!=y.module || x.function!=y.function || x.function_generation!=y.function_generation ||
            x.runtime_epoch!=y.runtime_epoch) return false;
    }
    return true;
}

/* Caller requirement: root_offset is the result of evaluating the actual
 * snapshot pointer's DWARF location in this SAME canonical memory callback.
 * No source/name/map-file or Guest integer is a substitute. This function is
 * not a public controller registration, Guest import, breakpoint or GC-stop
 * issuer. It cannot construct a view. No borrowed Runtime owner escapes. */
inline status copy_current(rt::llvm_jit_debug_source_memory_view& view,
                           std::uint64_t root_offset,
                           rt::llvm_jit_debug_source_activation_snapshot const& expected,
                           current_snapshot& out) noexcept {
    /* expected may refer to out.actual_position. Preserve it until all input
     * comparisons finish; clear stale output only when returning a refusal. */
    auto refuse=[&](status reason) noexcept { out={}; return reason; };
    try {
        rt::llvm_jit_debug_source_activation_snapshot position{};
        if (!view.copy_position(position) || !same_position(position,expected) ||
            (view.address_bytes()!=4 && view.address_bytes()!=8) ||
            (view.address_bytes()==4 && root_offset>UINT32_MAX)) return refuse(status::unavailable);
        rt::llvm_jit_debug_source_object_copy header{}, bytes{};
        if (!view.copy_guest(root_offset,128,header) || header.bytes.size()!=128 ||
            !same_position(header.position,position) || header.guest_offset!=root_offset ||
            detail::u32(header.bytes,8)!=1 || detail::u32(header.bytes,12)!=59008 ||
            detail::u32(header.bytes,16)!=128 || !detail::u64(header.bytes,24)) return refuse(status::unavailable);
        if (!view.copy_guest(root_offset,59008,bytes) || bytes.bytes.size()!=59008 ||
            !same_position(bytes.position,position) || bytes.guest_offset!=root_offset ||
            bytes.address_bytes!=view.address_bytes() ||
            !std::equal(header.bytes.begin(),header.bytes.end(),bytes.bytes.begin())) return refuse(status::unavailable);
        current_snapshot candidate{}; auto result=parse(bytes.bytes,candidate.value);
        if (result!=status::ok) return refuse(result);
        rt::llvm_jit_debug_source_activation_snapshot terminal{};
        if (!view.copy_position(terminal) || !same_position(terminal,position)) return refuse(status::unavailable);
        candidate.actual_position=std::move(position); out=std::move(candidate);
        return status::ok;
    } catch (...) { out={}; return status::allocation_failure; }
}
}
#endif
