/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include <uwvm2/utils/allocator/fast_io_strict/object.h>
#include <uwvm2/utils/allocator/fast_io_strict/impl.h>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <utility>
#include <new>
#include <memory>
#include <type_traits>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#if defined(__linux__)
# include <cerrno>
# include <fcntl.h>
# include <signal.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <unistd.h>
#endif

namespace uwvm2::utils::allocator::resource
{
    // Host deployment policy, independent of import permissions. Configure only
    // before loading modules or starting guest threads. Imported aliases retain
    // the original backing's charge; creating another module does not reset it.
    enum class pool : unsigned { gc, table, memory, memtag, auxiliary, validation, threads, count };
    inline constexpr ::std::size_t host_bytes(::std::uint64_t bytes) noexcept
    {
        constexpr auto maximum{static_cast<::std::uint64_t>((::std::numeric_limits<::std::ptrdiff_t>::max)())};
        return static_cast<::std::size_t>(bytes < maximum ? bytes : maximum);
    }
    struct limits
    {
        ::std::size_t single;
        ::std::size_t live;
    };
    inline limits policies[] = {
        {host_bytes(256ull << 20u), host_bytes(1ull << 30u)},
        {host_bytes(128ull << 20u), host_bytes(512ull << 20u)},
        {host_bytes(4ull << 30u), host_bytes(8ull << 30u)},
        {host_bytes(256ull << 20u), host_bytes(512ull << 20u)},
        {host_bytes(256ull << 20u), host_bytes(1ull << 30u)},
        {host_bytes(256ull << 20u), host_bytes(512ull << 20u)},
        {host_bytes(256ull << 20u), host_bytes(4ull << 30u)}
    };
    static_assert(sizeof(policies)/sizeof(policies[0]) == static_cast<unsigned>(pool::count));
    inline ::std::atomic_size_t live_bytes[static_cast<unsigned>(pool::count)]{};
    inline ::std::size_t parser_metadata_bytes{host_bytes(512ull << 20u)};
    inline ::std::size_t module_file_bytes{host_bytes(256ull << 20u)};
    inline ::std::size_t initializer_allocation_bytes{host_bytes(4ull << 30u)};
    inline ::std::size_t function_body_bytes{host_bytes(8ull << 20u)};
    inline ::std::size_t module_code_bytes{host_bytes(64ull << 20u)};

    [[nodiscard]] inline bool acquire(pool kind, ::std::size_t bytes) noexcept
    {
        auto const index{static_cast<unsigned>(kind)};
        auto const limit{policies[index]};
        if(bytes > limit.single) { return false; }
        auto used{live_bytes[index].load(::std::memory_order_relaxed)};
        for(;;)
        {
            if(used > limit.live || bytes > limit.live - used) { return false; }
            if(live_bytes[index].compare_exchange_weak(used, used + bytes,
                ::std::memory_order_relaxed, ::std::memory_order_relaxed)) { return true; }
        }
    }
    inline void release(pool kind, ::std::size_t bytes) noexcept
    {
        if(bytes != 0uz && live_bytes[static_cast<unsigned>(kind)].fetch_sub(bytes, ::std::memory_order_relaxed) < bytes)
        { ::std::abort(); } // Broken host ownership accounting, never a guest limit rejection.
    }
    [[nodiscard]] inline constexpr bool checked_bytes(::std::size_t count, ::std::size_t width,
        ::std::size_t& bytes) noexcept
    {
        constexpr auto maximum{static_cast<::std::size_t>((::std::numeric_limits<::std::ptrdiff_t>::max)())};
        if(width != 0uz && count > maximum/width) { return false; }
        bytes=count*width; return true;
    }
    // This function always exits: a broken/full diagnostic pipe must neither
    // replace status 127 with SIGPIPE nor block a resource refusal indefinitely.
    // No allocation, guest text, or process-global signal disposition is used.
    [[noreturn]] inline void reject_allocation(char const* message) noexcept
    {
#if defined(__linux__)
        struct stat original{};
        if(::fstat(2,&original)==0)
        {
            auto const length{::std::strlen(message)};
            if(S_ISSOCK(original.st_mode))
            { static_cast<void>(::send(2,message,length,MSG_DONTWAIT|MSG_NOSIGNAL)); }
            else if(S_ISREG(original.st_mode) || S_ISFIFO(original.st_mode) || ::isatty(2))
            {
                // A new OFD keeps O_NONBLOCK private; dup/F_SETFL would also
                // change the deploying parent's inherited diagnostic stream.
                int descriptor{::open("/proc/self/fd/2",O_WRONLY|O_APPEND|O_NONBLOCK|O_CLOEXEC|O_NOCTTY)};
                if(descriptor>=0)
                {
                    struct stat actual{};sigset_t mask{};
                    if(::fstat(descriptor,&actual)==0 && original.st_dev==actual.st_dev &&
                       original.st_ino==actual.st_ino && original.st_mode==actual.st_mode &&
                       ::sigemptyset(&mask)==0 && ::sigaddset(&mask,SIGPIPE)==0 &&
                       ::sigprocmask(SIG_BLOCK,&mask,nullptr)==0)
                    { static_cast<void>(::write(descriptor,message,length)); }
                    static_cast<void>(::close(descriptor));
                }
            }
        }
#else
        ::std::fputs(message,stderr);
#endif
        ::std::_Exit(127);
    }
    [[noreturn]] inline void validation_limit() noexcept
    {
        reject_allocation("uwvm: [error] validation: native metadata allocation refused; check --wasm-set-resource-limit validation single-bytes/live-bytes and available host memory.\n");
    }
    [[noreturn]] inline void compiler_memory_limit() noexcept
    {
        reject_allocation("uwvm: [error] LLVM compilation: native allocation failed; available host memory or virtual address space is exhausted. Reduce concurrent memory instances or use a 64-bit host with sufficient memory.\n");
    }
    // Standard containers return denied host scratch allocations through their
    // existing bad_alloc -> guest ENOMEM path. The guest cannot alter budgets.
    template<typename T, pool Kind = pool::auxiliary> struct std_allocator
    {
        using value_type=T;
        using is_always_equal=::std::true_type;
        using propagate_on_container_move_assignment=::std::true_type;
        template<typename U> struct rebind { using other=std_allocator<U,Kind>; };
        constexpr std_allocator() noexcept=default;
        template<typename U> constexpr std_allocator(std_allocator<U,Kind> const&) noexcept {}
        [[nodiscard]] constexpr T* allocate(::std::size_t count)
        {
            if consteval { return ::uwvm2::utils::allocator::fast_io_strict::native_strict_typed_global_allocator<T>::allocate(count); }
            else
            {
                ::std::size_t bytes{};
                if(!checked_bytes(count,sizeof(T),bytes) || !acquire(Kind,bytes))
                {
                    if constexpr(Kind == pool::validation) { validation_limit(); }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
                    throw ::std::bad_alloc{};
#else
                    reject_allocation("uwvm: [error] native scratch allocation exceeds resource budget.\n");
#endif
                }
                auto* pointer{::uwvm2::utils::allocator::fast_io_strict::native_strict_typed_global_allocator<T>::allocate(count)};
                if(pointer != nullptr) { return pointer; }
                release(Kind,bytes);
                if constexpr(Kind == pool::validation) { validation_limit(); }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
                throw ::std::bad_alloc{}; // Existing caller maps recoverable scratch failure to guest ENOMEM.
#else
                reject_allocation("uwvm: [error] native scratch allocation failed.\n");
#endif
            }
        }
        constexpr void deallocate(T* pointer,::std::size_t count) noexcept
        {
            ::uwvm2::utils::allocator::fast_io_strict::native_strict_typed_global_allocator<T>::deallocate_n(pointer,count);
            if consteval {} else { release(Kind,count*sizeof(T)); }
        }
        template<typename U> constexpr bool operator==(std_allocator<U,Kind> const&) const noexcept { return true; }
    };
    template<typename T, pool Kind> struct array_deleter
    {
        ::std::size_t bytes{};
        void operator()(T* pointer) const noexcept
        {
            if constexpr(!::std::is_trivially_destructible_v<T>)
            { auto count=bytes/sizeof(T);while(count!=0u) { ::std::destroy_at(pointer+--count); } }
            ::uwvm2::utils::allocator::fast_io_strict::native_strict_typed_global_allocator<T>::deallocate_n(pointer,bytes/sizeof(T));
            release(Kind,bytes);
        }
    };
    template<typename T, pool Kind = pool::auxiliary>
    using owned_array=::std::unique_ptr<T[],array_deleter<T,Kind>>;
    template<typename T, pool Kind = pool::auxiliary>
    [[nodiscard]] owned_array<T,Kind> try_array(::std::size_t count) noexcept
    {
        ::std::size_t bytes{};
        if(!checked_bytes(count,sizeof(T),bytes) || !acquire(Kind,bytes)) { return {}; }
        auto* pointer{::uwvm2::utils::allocator::fast_io_strict::native_strict_typed_global_allocator<T>::allocate(count)};
        if(pointer == nullptr) { release(Kind,bytes); return {}; }
        ::std::uninitialized_value_construct_n(pointer,count);
        return owned_array<T,Kind>{pointer,array_deleter<T,Kind>{bytes}};
    }
    // Ownership follows the actual allocation, including failed construction,
    // move, reset and hot-update retirement. No counting on loads/stores.
    template<pool Kind> class reservation
    {
        ::std::size_t bytes_{};
    public:
        constexpr reservation() noexcept = default;
        reservation(reservation const&) = delete;
        reservation& operator=(reservation const&) = delete;
        constexpr reservation(reservation&& other) noexcept : bytes_{::std::exchange(other.bytes_,0uz)} {}
        constexpr reservation& operator=(reservation&& other) noexcept
        {
            if(this != &other) { reset(); bytes_ = ::std::exchange(other.bytes_,0uz); }
            return *this;
        }
        constexpr ~reservation() { reset(); }
        [[nodiscard]] constexpr ::std::size_t bytes() const noexcept { return bytes_; }
        [[nodiscard]] constexpr bool resize(::std::size_t next) noexcept
        {
            if consteval { bytes_=next; return true; }
            else
            {
                if(next > policies[static_cast<unsigned>(Kind)].single) { return false; }
                if(next > bytes_ && !acquire(Kind,next-bytes_)) { return false; }
                if(next < bytes_) { release(Kind,bytes_-next); }
                bytes_=next; return true;
            }
        }
        constexpr void reset() noexcept
        {
            if consteval {} else { release(Kind,bytes_); }
            bytes_=0uz;
        }
    };
    // Fixed-size registry nodes own a charge just like variable-size backing.
    // Reserve before new; release remains tied to the real node destructor.
    template<typename T, typename... Args>
    [[nodiscard]] T* try_charged_object(Args&&... args) noexcept
    {
        reservation<pool::auxiliary> charge;
        if(!charge.resize(sizeof(T))) { return nullptr; }
        auto* result{::uwvm2::utils::allocator::fast_io_strict::try_object<T>([&](T* uwvm_allocated) { return ::new(static_cast<void*>(uwvm_allocated)) T{::std::forward<Args>(args)...}; })};
        if(result != nullptr) { result->allocation_charge=::std::move(charge); }
        return result;
    }
    template<pool Kind> class resize_transaction
    {
        reservation<Kind>* owner_;
        ::std::size_t previous_;
        bool ready_, committed_{};
    public:
        constexpr resize_transaction(reservation<Kind>& owner, ::std::size_t next) noexcept
            : owner_{&owner}, previous_{owner.bytes()}, ready_{owner.resize(next)} {}
        resize_transaction(resize_transaction const&) = delete;
        constexpr ~resize_transaction()
        { if(ready_ && !committed_) { static_cast<void>(owner_->resize(previous_)); } }
        [[nodiscard]] constexpr explicit operator bool() const noexcept { return ready_; }
        constexpr void commit() noexcept { committed_=true; }
    };
    [[noreturn]] inline void initializer_memory_limit() noexcept
    {
        // Diagnostics must not turn a closed stderr into an unrelated crash.
        reject_allocation("uwvm: [error] initializer: linear memory exceeds the configured byte budget. Use --wasm-set-resource-limit runtime memory-single-bytes/memory-live-bytes to configure it.\n");
    }
    [[noreturn]] inline void initializer_memory_mapping_failure() noexcept
    {
        reject_allocation("uwvm: [error] initializer: native linear-memory address-space reservation or page commitment failed; reduce concurrent memory instances or initial sizes, or use a 64-bit host with sufficient memory.\n");
    }
    // A byte allocation retains its exact charge in its fast_io backing block.
    // The same strict global allocator releases that block after owner teardown.
    struct alignas(::std::max_align_t) byte_header { ::std::size_t bytes; };
    template<pool Kind> [[nodiscard]] inline ::std::byte* try_bytes(::std::size_t bytes) noexcept
    {
        constexpr auto max{static_cast<::std::size_t>((::std::numeric_limits<::std::ptrdiff_t>::max)())};
        if(bytes > max-sizeof(byte_header) || !acquire(Kind,bytes+sizeof(byte_header))) { return nullptr; }
        auto* raw{static_cast<::std::byte*>(::uwvm2::utils::allocator::fast_io_strict::native_strict_global_allocator::allocate(bytes+sizeof(byte_header)))};
        if(raw == nullptr) { release(Kind,bytes+sizeof(byte_header)); return nullptr; }
        ::new(static_cast<void*>(raw)) byte_header{bytes+sizeof(byte_header)};
        return raw+sizeof(byte_header);
    }
    template<pool Kind> inline void free_bytes(::std::byte* pointer) noexcept
    {
        if(pointer == nullptr) { return; }
        // This is the retained cursor returned by try_bytes, never a guest token.
        auto* raw{pointer-sizeof(byte_header)};
        auto* header{reinterpret_cast<byte_header*>(raw)};
        auto const bytes{header->bytes};
        header->~byte_header(); ::uwvm2::utils::allocator::fast_io_strict::native_strict_global_allocator::deallocate(raw); release(Kind,bytes);
    }
    [[nodiscard]] inline ::std::byte* try_gc_bytes(::std::size_t bytes) noexcept { return try_bytes<pool::gc>(bytes); }
    inline void free_gc_bytes(::std::byte* pointer) noexcept { free_bytes<pool::gc>(pointer); }

    template<pool Kind> struct bounded_allocator
    {
        [[nodiscard]] static void* allocate(::std::size_t bytes) noexcept
        {
            auto* result{try_bytes<Kind>(bytes)};
            if(result==nullptr)
            {
                validation_limit();
            }
            return result;
        }
        [[nodiscard]] static void* allocate_zero(::std::size_t bytes) noexcept
        { auto* result{allocate(bytes)}; ::std::memset(result,0,bytes); return result; }
        static void deallocate(void* pointer) noexcept { free_bytes<Kind>(static_cast<::std::byte*>(pointer)); }
    };

}
