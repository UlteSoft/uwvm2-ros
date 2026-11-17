/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include <uwvm2/utils/allocator/fast_io_strict/std_allocator.h>
#include <memory>
#include <uwvm2/utils/allocator/fast_io_strict/object.h>
#include <type_traits>
#include <utility>

namespace uwvm2::utils::container
{
    template<typename T> struct strict_object_deleter
    { void operator()(T* p) const noexcept { ::uwvm2::utils::allocator::fast_io_strict::destroy_object(p); } };
    // Private constructors and custom retirement need a separate object block.
    // The control block still uses fast_io; allocation failure invokes the same
    // deleter exactly once before propagating, as required by shared_ptr.
    template<typename T,typename Deleter=strict_object_deleter<T>>
    [[nodiscard]] inline ::std::shared_ptr<T> adopt_shared(T* pointer,Deleter deleter={})
    { return ::std::shared_ptr<T>{pointer,::std::move(deleter),
        ::uwvm2::utils::allocator::fast_io_strict::native_std_allocator<::std::remove_cv_t<T>>{}}; }
    // Both the object and its shared/weak control block use fast_io backing.
    // Keep shared_ptr's lifetime and weak-reference protocol without invoking
    // the standard global allocation functions for either allocation.
    template<typename T, typename... Args>
    [[nodiscard]] auto make_shared(Args&&... args)
    {
        return ::std::allocate_shared<T>(
            ::uwvm2::utils::allocator::fast_io_strict::native_std_allocator<::std::remove_cv_t<T>>{},
            ::std::forward<Args>(args)...);
    }
}
