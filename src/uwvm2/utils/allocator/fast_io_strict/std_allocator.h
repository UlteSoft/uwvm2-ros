/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include "impl.h"
#include <memory>
#include <new>
#include <limits>
#include <type_traits>

namespace uwvm2::utils::allocator::fast_io_strict
{
    // Standard-library protocol adapter for the existing recoverable fast_io
    // global allocator. The backing allocator still returns nullptr; containers
    // receive bad_alloc so their existing rollback/ENOMEM handlers remain valid.
    template<typename T> struct native_std_allocator
    {
        using value_type = T;
        using is_always_equal = ::std::true_type;
        using propagate_on_container_move_assignment = ::std::true_type;
        constexpr native_std_allocator() noexcept = default;
        template<typename U> constexpr native_std_allocator(native_std_allocator<U> const&) noexcept {}
        [[nodiscard]] constexpr T* allocate(::std::size_t count)
        {
            if(count <= (::std::numeric_limits<::std::size_t>::max)()/sizeof(T))
            {
                if(auto* result = native_strict_typed_global_allocator<T>::allocate(count)) { return result; }
            }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
            throw ::std::bad_alloc{};
#else
            ::fast_io::fast_terminate();
#endif
        }
        constexpr void deallocate(T* pointer, ::std::size_t count) noexcept
        { native_strict_typed_global_allocator<T>::deallocate_n(pointer, count); }
        template<typename U> constexpr bool operator==(native_std_allocator<U> const&) const noexcept { return true; }
    };
}
