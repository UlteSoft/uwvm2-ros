/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)
 * Licensed under the APL-2.0 License (see LICENSE file).
 *************************************************************/
#pragma once
#include <uwvm2/runtime/compiler/shared/mmap_fault_landing_feature.h>
#if UWVM2_RUNTIME_HAS_LINUX_MMAP_FAULT_LANDING
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <new>
#include <libunwind.h>
#include <ucontext.h>
#include "uwvm_runtime_posix_abi.h"

namespace uwvm2::runtime::lib::memory_guard_landing
{
    // Fault-only scratch stack. It never holds guest bytes or replaces a live
    // guest activation. Kernel sigreturn restores the pre-fault signal mask
    // before the cold callback takes locks or invokes the exit coordinator.
    inline constexpr ::std::size_t stack_bytes{1024u * 1024u};
    struct record;
    using callback = void (*)(record const&) noexcept;
    struct record
    {
        ::std::size_t mapping_bytes{};
        ::std::uintptr_t top{};
        callback deliver{};
        ::std::atomic<unsigned> phase{};
        ::std::size_t memory_index{};
        ::std::uint64_t offset{}, length{};
        ::std::uintptr_t registers[33]{};
    };
    static_assert(::std::atomic<record*>::is_always_lock_free);
    static_assert(::std::atomic<unsigned>::is_always_lock_free);
    inline thread_local ::std::atomic<record*> active{};
    inline thread_local record* cached{};
    inline thread_local bool retired{};
    inline ::pthread_key_t cache_key{};

    inline void release(void* pointer) noexcept
    {
        retired = true;
        auto* value{static_cast<record*>(pointer)};
        if(cached == value) { cached = nullptr; }
        if(value == nullptr) { return; }
        // A late TSD destructor must not free an active crash-delivery stack.
        // Normal runtime scopes clear active before any thread destruction.
        if(active.load(::std::memory_order_acquire) == value) { return; }
        ::uwvm2::runtime::lib::posix_abi::munmap_noexcept(value, value->mapping_bytes);
    }
    [[nodiscard]] inline record* allocate() noexcept
    {
        namespace posix = ::uwvm2::runtime::lib::posix_abi;
        auto const raw_page{posix::sysconf_noexcept(_SC_PAGESIZE)};
        if(raw_page <= 0) { return nullptr; }
        auto const page{static_cast<::std::size_t>(raw_page)};
        if(page < sizeof(record) || page > ((::std::numeric_limits<::std::size_t>::max)() - stack_bytes) / 3u)
        { return nullptr; }
        auto const bytes{stack_bytes + page * 3u};
        auto* mapping{posix::mmap_noexcept(nullptr, bytes, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)};
        if(mapping == MAP_FAILED) { return nullptr; }
        auto* body{static_cast<::std::byte*>(mapping) + page * 2u};
        if(posix::mprotect_noexcept(mapping, page, PROT_READ | PROT_WRITE) != 0 ||
           posix::mprotect_noexcept(body, stack_bytes, PROT_READ | PROT_WRITE) != 0)
        { posix::munmap_noexcept(mapping, bytes); return nullptr; }
        // Metadata | guard | crash stack | guard. No heap allocation is needed
        // on signal delivery, and neither stack shares bytes with guest memory.
        auto* value{::new(mapping) record{}};
        value->mapping_bytes = bytes;
        value->top = reinterpret_cast<::std::uintptr_t>(body) + stack_bytes;
        return value;
    }
    class scope
    {
        record* value_{};
        bool owner_{}, uncached_{};
    public:
        scope() noexcept = default;
        scope(scope const&) = delete;
        scope& operator=(scope const&) = delete;
        [[nodiscard]] bool enter(callback deliver) noexcept
        {
            if(value_ != nullptr) { return true; }
            value_ = active.load(::std::memory_order_acquire);
            if(value_ != nullptr) { return true; } // nested host entry, same OS thread
            value_ = cached;
            if(value_ == nullptr)
            {
                value_ = allocate();
                if(value_ == nullptr) { return false; }
                static bool const have_key{::uwvm2::runtime::lib::posix_abi::pthread_key_create_noexcept(cache_key_address(), release) == 0};
                if(!retired && have_key && ::uwvm2::runtime::lib::posix_abi::pthread_setspecific_noexcept(cache_key, value_) == 0)
                { cached = value_; }
                else { uncached_ = true; }
            }
            value_->deliver = deliver;
            value_->phase.store(0u, ::std::memory_order_relaxed);
            owner_ = true;
            active.store(value_, ::std::memory_order_release);
            return true;
        }
        ~scope()
        {
            if(owner_) { active.store(nullptr, ::std::memory_order_release); }
            if(uncached_) { ::uwvm2::runtime::lib::posix_abi::munmap_noexcept(value_, value_->mapping_bytes); }
        }
    private:
        static ::pthread_key_t* cache_key_address() noexcept { return &cache_key; }
    };

    [[noreturn]] __attribute__((noinline)) inline void land() noexcept
    {
        auto* value{active.load(::std::memory_order_acquire)};
        if(value == nullptr || value->phase.load(::std::memory_order_acquire) != 1u || value->deliver == nullptr)
        { ::uwvm2::runtime::lib::posix_abi::_exit_noexcept(123); }
        // This executes AFTER returning from the signal handler. Never resume
        // or unwind out of this artificial stack: the original stack stays live
        // until process termination, preserving leases and pending-operation TLS.
        value->phase.store(2u, ::std::memory_order_release);
        value->deliver(*value);
        ::uwvm2::runtime::lib::posix_abi::_exit_noexcept(123);
    }
    [[nodiscard]] inline bool redirect(::std::size_t memory, ::std::uint64_t offset,
                                       ::std::uint64_t length, void* context) noexcept
    {
        auto* value{active.load(::std::memory_order_acquire)};
        if(value == nullptr || context == nullptr) { return false; }
        unsigned expected{};
        if(!value->phase.compare_exchange_strong(expected, 1u, ::std::memory_order_acq_rel)) { return false; }
        value->memory_index = memory; value->offset = offset; value->length = length;
        auto* u{static_cast<::ucontext_t*>(context)};
#if defined(__x86_64__)
        constexpr int g[]{REG_RAX,REG_RDX,REG_RCX,REG_RBX,REG_RSI,REG_RDI,REG_RBP,REG_RSP,
                          REG_R8,REG_R9,REG_R10,REG_R11,REG_R12,REG_R13,REG_R14,REG_R15,REG_RIP};
        for(unsigned i{}; i != 17u; ++i) { value->registers[i] = u->uc_mcontext.gregs[g[i]]; }
        auto const sp{(value->top & ~::std::uintptr_t{15u}) - sizeof(::std::uintptr_t)};
        *reinterpret_cast<::std::uintptr_t*>(sp) = 0u;
        u->uc_mcontext.gregs[REG_RSP] = sp; u->uc_mcontext.gregs[REG_RIP] = reinterpret_cast<::std::uintptr_t>(&land);
        u->uc_mcontext.gregs[REG_RBP] = 0u;
#elif defined(__i386__)
        // Linux LLVM libunwind uses DWARF x86 numbering: ESP=4, EBP=5.
        // Its public enum keeps the Darwin EBP/ESP names in the opposite
        // order; Registers_x86 remaps those two names on non-Apple hosts.
        constexpr int g[]{REG_EAX,REG_ECX,REG_EDX,REG_EBX,REG_ESP,REG_EBP,REG_ESI,REG_EDI,REG_EIP};
        for(unsigned i{}; i != 9u; ++i) { value->registers[i] = u->uc_mcontext.gregs[g[i]]; }
        auto const sp{(value->top & ~::std::uintptr_t{15u}) - sizeof(::std::uintptr_t)};
        *reinterpret_cast<::std::uintptr_t*>(sp) = 0u;
        u->uc_mcontext.gregs[REG_ESP] = sp; u->uc_mcontext.gregs[REG_EIP] = reinterpret_cast<::std::uintptr_t>(&land);
        u->uc_mcontext.gregs[REG_EBP] = 0u;
#elif defined(__aarch64__)
        for(unsigned i{}; i != 31u; ++i) { value->registers[i] = u->uc_mcontext.regs[i]; }
        value->registers[31] = u->uc_mcontext.sp; value->registers[32] = u->uc_mcontext.pc;
        u->uc_mcontext.sp = value->top & ~::std::uintptr_t{15u};
        u->uc_mcontext.pc = reinterpret_cast<::std::uintptr_t>(&land);
        u->uc_mcontext.regs[29] = 0u; u->uc_mcontext.regs[30] = 0u;
#elif defined(__arm__)
        value->registers[0]=u->uc_mcontext.arm_r0; value->registers[1]=u->uc_mcontext.arm_r1;
        value->registers[2]=u->uc_mcontext.arm_r2; value->registers[3]=u->uc_mcontext.arm_r3;
        value->registers[4]=u->uc_mcontext.arm_r4; value->registers[5]=u->uc_mcontext.arm_r5;
        value->registers[6]=u->uc_mcontext.arm_r6; value->registers[7]=u->uc_mcontext.arm_r7;
        value->registers[8]=u->uc_mcontext.arm_r8; value->registers[9]=u->uc_mcontext.arm_r9;
        value->registers[10]=u->uc_mcontext.arm_r10; value->registers[11]=u->uc_mcontext.arm_fp;
        value->registers[12]=u->uc_mcontext.arm_ip; value->registers[13]=u->uc_mcontext.arm_sp;
        value->registers[14]=u->uc_mcontext.arm_lr;
        value->registers[15]=u->uc_mcontext.arm_pc | ((u->uc_mcontext.arm_cpsr & 32u) ? 1u : 0u);
        auto const pc{reinterpret_cast<::std::uintptr_t>(&land)};
        u->uc_mcontext.arm_sp = value->top & ~::std::uintptr_t{7u};
        u->uc_mcontext.arm_pc = pc & ~::std::uintptr_t{1u}; u->uc_mcontext.arm_lr = 0u;
        u->uc_mcontext.arm_cpsr = (u->uc_mcontext.arm_cpsr & ~32u) | ((pc & 1u) ? 32u : 0u);
#endif
        return true;
    }
    [[nodiscard]] inline record const* current() noexcept
    {
        auto* value{active.load(::std::memory_order_acquire)};
        return value != nullptr && value->phase.load(::std::memory_order_acquire) == 2u ? value : nullptr;
    }
    [[nodiscard]] inline ::std::uintptr_t instruction(record const& value) noexcept
    {
#if defined(__x86_64__)
        return value.registers[16];
#elif defined(__i386__)
        return value.registers[8];
#elif defined(__aarch64__)
        return value.registers[32];
#else
        return value.registers[15] & ~::std::uintptr_t{1u};
#endif
    }
    // Stream real CFI frames from the saved fault registers. No instruction or
    // frame-pointer guessing, no artificial frame-count cap, no guest reads.
    template<typename Visitor>
    [[nodiscard]] inline bool walk(Visitor&& visit) noexcept
    {
        auto const* value{current()};
        if(value == nullptr) { return false; }
        ::unw_context_t context{}; ::unw_cursor_t cursor{};
        if(::unw_getcontext(&context) != 0 || ::unw_init_local(&cursor, &context) != 0) { return false; }
#if defined(__x86_64__)
        constexpr unsigned last{16u};
#elif defined(__i386__)
        constexpr unsigned last{7u};
#elif defined(__aarch64__)
        constexpr unsigned last{32u};
#else
        constexpr unsigned last{15u};
#endif
        for(unsigned i{}; i <= last; ++i)
        { if(::unw_set_reg(&cursor, static_cast<int>(i), value->registers[i]) != 0) { return false; } }
        // Set the generic IP LAST: LLVM libunwind refreshes CFI only through
        // UNW_REG_IP. Setting the architectural PC alone leaves stale metadata.
#if defined(__arm__)
        auto const pc{value->registers[15]};
#else
        auto const pc{instruction(*value)};
#endif
        if(::unw_set_reg(&cursor, UNW_REG_IP, pc) != 0) { return false; }
        // LLVM's generic IP setter also applies the seed frame's GNU args-size
        // stack adjustment (notably i686). This is a captured fault context,
        // not an exception landing-pad installation: restore its exact SP after
        // refreshing CFI so a frameless leaf cannot silently skip its caller.
#if defined(__x86_64__)
        auto const saved_sp{value->registers[7]};
#elif defined(__i386__)
        auto const saved_sp{value->registers[4]};
#elif defined(__aarch64__)
        auto const saved_sp{value->registers[31]};
#else
        auto const saved_sp{value->registers[13]};
#endif
        if(::unw_set_reg(&cursor, UNW_REG_SP, saved_sp) != 0) { return false; }
        bool first{true};
        ::std::uintptr_t previous_ip{}, previous_sp{};
        for(;;)
        {
            ::unw_word_t ip{}, sp{}; ::unw_proc_info_t info{};
            if(::unw_get_reg(&cursor, UNW_REG_IP, &ip) != 0 || ::unw_get_reg(&cursor, UNW_REG_SP, &sp) != 0)
            { return false; }
            if(ip == 0u) { return true; }
            // Detect a non-progressing corrupt unwind record, without limiting
            // valid recursion or assuming that stack addresses are monotonic.
            if(!first && ip == previous_ip && sp == previous_sp) { return false; }
            previous_ip = ip; previous_sp = sp;
            if(::unw_get_proc_info(&cursor, &info) != 0) { return false; }
#if defined(__arm__)
            ip &= ~::unw_word_t{1u};
#endif
            visit(static_cast<::std::uintptr_t>(first || ip == 0u ? ip : ip - 1u),
                  static_cast<::std::uintptr_t>(sp), static_cast<::std::uintptr_t>(info.start_ip));
            first = false;
            auto const stepped{::unw_step(&cursor)};
            if(stepped <= 0) { return stepped == 0; }
        }
    }
}
#endif
