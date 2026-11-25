// Only the faulting instructions of these private leaves are guest accesses.
// Exact PC matching keeps unrelated native faults out of Wasm trap delivery.
#pragma once
#include "mmap_fault_landing_feature.h"
#if UWVM2_RUNTIME_HAS_LINUX_MMAP_FAULT_LANDING && (defined(__x86_64__) || defined(__aarch64__)) && \
    !defined(__ILP32__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define UWVM2_HAS_INT_ADM_GUARD 1
#include <atomic>
#include <cstddef>
#include <cstdint>

namespace uwvm2::runtime::compiler::shared::int_adm_guard
{
    extern "C"
    {
#define UWVM2_ADM_ACCESS_DECL(N) \
        __attribute__((weak,visibility("hidden"))) ::std::uint64_t uwvm_adm_load##N(void const*) noexcept; \
        __attribute__((weak,visibility("hidden"))) void uwvm_adm_store##N(void*,::std::uint64_t) noexcept;
        UWVM2_ADM_ACCESS_DECL(1)
        UWVM2_ADM_ACCESS_DECL(2)
        UWVM2_ADM_ACCESS_DECL(4)
        UWVM2_ADM_ACCESS_DECL(8)
#undef UWVM2_ADM_ACCESS_DECL
    }
    inline ::std::atomic<bool> selected{};
    [[nodiscard]] inline bool available() noexcept { return uwvm_adm_load1 != nullptr; }
    [[nodiscard]] inline bool owns_pc(::std::uintptr_t pc) noexcept
    {
        if(!pc) { return false; }
#define UWVM2_ADM_ACCESS_PC(N) pc == reinterpret_cast<::std::uintptr_t>(uwvm_adm_load##N) || pc == reinterpret_cast<::std::uintptr_t>(uwvm_adm_store##N)
        return UWVM2_ADM_ACCESS_PC(1) || UWVM2_ADM_ACCESS_PC(2) || UWVM2_ADM_ACCESS_PC(4) || UWVM2_ADM_ACCESS_PC(8);
#undef UWVM2_ADM_ACCESS_PC
    }
    template<::std::size_t Bytes>
    [[nodiscard]] inline ::std::uint64_t load(void const* p) noexcept
    {
        if constexpr(Bytes == 1u) { return uwvm_adm_load1(p); }
        else if constexpr(Bytes == 2u) { return uwvm_adm_load2(p); }
        else if constexpr(Bytes == 4u) { return uwvm_adm_load4(p); }
        else { static_assert(Bytes == 8u); return uwvm_adm_load8(p); }
    }
    template<::std::size_t Bytes>
    inline void store(void* p,::std::uint64_t bits) noexcept
    {
        if constexpr(Bytes == 1u) { uwvm_adm_store1(p,bits); }
        else if constexpr(Bytes == 2u) { uwvm_adm_store2(p,bits); }
        else if constexpr(Bytes == 4u) { uwvm_adm_store4(p,bits); }
        else { static_assert(Bytes == 8u); uwvm_adm_store8(p,bits); }
    }
}
#else
#define UWVM2_HAS_INT_ADM_GUARD 0
#endif
