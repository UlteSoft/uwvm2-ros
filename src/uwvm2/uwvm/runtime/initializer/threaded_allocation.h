// Cold, explicit child-construction allocation accounting. SOURCE ONLY.
#pragma once
#include <uwvm2/utils/allocator/fast_io_strict/object.h>
#include <uwvm2/utils/allocator/resource_limit.h>
#ifndef UWVM_MODULE
#include <atomic>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <limits>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>
#include <fast_io.h>

#endif
#ifndef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::runtime::initializer
{
    // Counts actual requested allocation bytes, including control blocks and
    // explicit array-prefix bytes. This is not RSS or malloc overhead.
    // The count is monotonic through construction (freed scratch is not reused
    // as an excuse to exceed a per-child request budget).
    class threaded_allocation_account final
    {
        ::std::atomic_size_t references_{1uz};
        ::std::atomic_size_t requested_{};
        ::std::size_t maximum_{};
        explicit threaded_allocation_account(::std::size_t maximum) noexcept
            : requested_{sizeof(threaded_allocation_account)}, maximum_{maximum} {}
    public:
        [[nodiscard]] static threaded_allocation_account* create(::std::size_t maximum) noexcept
        {
            if(maximum < sizeof(threaded_allocation_account)) { return nullptr; }
            namespace budget=::uwvm2::utils::allocator::resource;
            if(!budget::acquire(budget::pool::threads,sizeof(threaded_allocation_account))) { return nullptr; }
            auto* result{::uwvm2::utils::allocator::fast_io_strict::try_object<threaded_allocation_account>([&](threaded_allocation_account* uwvm_allocated) { return ::new(static_cast<void*>(uwvm_allocated)) threaded_allocation_account{maximum}; })};
            if(result==nullptr) { budget::release(budget::pool::threads,sizeof(threaded_allocation_account)); }
            return result;
        }
        ~threaded_allocation_account()
        { ::uwvm2::utils::allocator::resource::release(::uwvm2::utils::allocator::resource::pool::threads,
            requested_.load(::std::memory_order_relaxed)); }
        // Called only by the dedicated compiler before publication. The account
        // is never widened after a guest begins; subsequent charges are atomic.
        [[nodiscard]] bool reserve_other_storage(::std::size_t bytes) noexcept
        {
            auto const used{requested_.load(::std::memory_order_relaxed)};
            if(used > maximum_ || bytes > maximum_ - used) { return false; }
            maximum_ -= bytes;
            return true;
        }
        void retain() noexcept { references_.fetch_add(1uz, ::std::memory_order_relaxed); }
        void release() noexcept
        { if(references_.fetch_sub(1uz, ::std::memory_order_acq_rel) == 1uz) { ::uwvm2::utils::allocator::fast_io_strict::destroy_object(this); } }
        [[nodiscard]] bool charge(::std::size_t bytes) noexcept
        {
            namespace budget=::uwvm2::utils::allocator::resource;
            if(!budget::acquire(budget::pool::threads,bytes)) { return false; }
            auto previous{requested_.load(::std::memory_order_relaxed)};
            for(;;)
            {
                if(previous>maximum_ || bytes > maximum_ - previous) { budget::release(budget::pool::threads,bytes); return false; }
                if(requested_.compare_exchange_weak(previous, previous + bytes,
                    ::std::memory_order_relaxed, ::std::memory_order_relaxed)) { return true; }
            }
        }
        [[nodiscard]] ::std::size_t requested_bytes() const noexcept
        { return requested_.load(::std::memory_order_relaxed); }
        // Requested backing bytes are charged before allocating. The paired
        // native-global vector allocator chooses the same backend/alignment.
        template<typename T> [[nodiscard]] T* try_vector_storage(::std::size_t count) noexcept
        {
            if(count==0u || count>SIZE_MAX/sizeof(T) || !charge(count*sizeof(T))) { return nullptr; }
            return ::uwvm2::utils::allocator::fast_io_strict::native_strict_typed_global_allocator<T>::allocate(count);
        }
        template<typename T> [[nodiscard]] T* try_array(::std::size_t count) noexcept
        {
            constexpr auto prefix=::uwvm2::utils::allocator::fast_io_strict::array_prefix<T>;
            if(count>(SIZE_MAX-prefix)/sizeof(T) || !charge(prefix+count*sizeof(T))) { return nullptr; }
            return ::uwvm2::utils::allocator::fast_io_strict::try_array<T>(count);
        }

    };

    class threaded_allocation_pin final
    {
        threaded_allocation_account* account_{};
        struct adopt_tag {};
        explicit threaded_allocation_pin(threaded_allocation_account* value, adopt_tag) noexcept : account_{value} {}
    public:
        [[nodiscard]] static threaded_allocation_pin create(::std::size_t maximum) noexcept
        { return threaded_allocation_pin{threaded_allocation_account::create(maximum), adopt_tag{}}; }
        threaded_allocation_pin() noexcept = default;
        threaded_allocation_pin(threaded_allocation_pin const& other) noexcept : account_{other.account_}
        { if(account_ != nullptr) { account_->retain(); } }
        threaded_allocation_pin(threaded_allocation_pin&& other) noexcept : account_{::std::exchange(other.account_, nullptr)} {}
        threaded_allocation_pin& operator=(threaded_allocation_pin other) noexcept
        { ::std::swap(account_, other.account_); return *this; }
        ~threaded_allocation_pin() { if(account_ != nullptr) { account_->release(); } }
        [[nodiscard]] explicit operator bool() const noexcept { return account_ != nullptr; }
        [[nodiscard]] threaded_allocation_account& get() const noexcept { return *account_; }
    };

    template<typename T> class threaded_std_allocator
    {
        template<typename> friend class threaded_std_allocator;
        threaded_allocation_pin pin_;
    public:
        using value_type = T;
        explicit threaded_std_allocator(threaded_allocation_pin pin) noexcept : pin_{::std::move(pin)} {}
        template<typename U> threaded_std_allocator(threaded_std_allocator<U> const& other) noexcept : pin_{other.pin_} {}
        [[nodiscard]] T* allocate(::std::size_t count)
        {
            void* raw{};
            if(count <= (::std::numeric_limits<::std::size_t>::max)() / sizeof(T) && pin_)
            {
                auto const bytes{count * sizeof(T)};
                if(pin_.get().charge(bytes))
                {
                    raw = ::uwvm2::utils::allocator::fast_io_strict::native_strict_typed_global_allocator<T>::allocate(count);
                }
            }
            if(raw == nullptr)
            {
#ifdef UWVM_CPP_EXCEPTIONS
                throw ::std::bad_alloc{};
#else
                ::std::terminate(); // create() rejects this profile before any allocation.
#endif
            }
            return static_cast<T*>(raw);
        }
        void deallocate(T* pointer, ::std::size_t) noexcept
        {
            ::uwvm2::utils::allocator::fast_io_strict::native_strict_typed_global_allocator<T>::deallocate(pointer);
        }
        template<typename U, typename... Args> void construct(U* pointer, Args&&... args)
        { ::new(const_cast<void*>(static_cast<void const*>(pointer))) U(::std::forward<Args>(args)...); }
        template<typename U> [[nodiscard]] bool operator==(threaded_std_allocator<U> const& other) const noexcept
        { return ::std::addressof(pin_.get()) == ::std::addressof(other.pin_.get()); }
    };

}
