#pragma once
// Private runtime owner. No constructor or issuer is exposed to Guest imports.
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS)
class runtime_managed_fault final
{
    inline static ::std::atomic<::std::uint_least64_t> enabled_epoch_{};
    inline static ::std::atomic<::std::uint_least64_t> fault_epoch_{};
    inline static ::std::atomic<::std::uint_least64_t> ordinary_epoch_{};
    inline static thread_local bool catchable_{};
public:
    class boundary final
    {
        bool previous_{catchable_};
    public:
        boundary() noexcept { catchable_ = true; }
        ~boundary() { catchable_ = previous_; }
        boundary(boundary const&) = delete;
    };
    class signal final
    {
        friend class runtime_managed_fault;
        ::std::uint_least64_t epoch_, participant_;
        signal(::std::uint_least64_t epoch, ::std::uint_least64_t participant) noexcept : epoch_{epoch}, participant_{participant} {}
    };
    static void enable(::std::uint_least64_t epoch, bool ordinary = false) noexcept
    {
        ordinary_epoch_.store(ordinary ? epoch : 0u, ::std::memory_order_release);
        enabled_epoch_.store(epoch, ::std::memory_order_release);
    }
    [[nodiscard]] static bool ordinary_enabled() noexcept;
    [[nodiscard]] static bool faulted(::std::uint_least64_t epoch) noexcept
    { return epoch != 0u && fault_epoch_.load(::std::memory_order_acquire) == epoch; }
    static bool raise(llvm_jit_trap_kind);
    [[nodiscard]] static bool accept(signal const&) noexcept;
    [[nodiscard]] static bool stop_after_unwind() noexcept;
    [[nodiscard]] static bool accept_uncaught(::uwvm2::runtime::exception::guest_exception const&) noexcept;
};
#endif
