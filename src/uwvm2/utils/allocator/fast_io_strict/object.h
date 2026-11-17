/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include "impl.h"
#include <memory>
#include <utility>
#include <limits>
#include <cstring>
namespace uwvm2::utils::allocator::fast_io_strict
{
    [[noreturn]] inline void allocation_failed()
    {
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        throw ::std::bad_alloc{};
#else
        ::fast_io::fast_terminate();
#endif
    }
    // The factory performs lifetime construction only, often in the scope of a
    // private constructor. All backing allocation/release uses the existing
    // strict fast_io global allocator, including constructor-exception rollback.
    template<typename T,typename Factory> [[nodiscard]] T* try_object(Factory&& factory)
    {
        using A=native_strict_typed_global_allocator<T>;
        auto* raw=A::allocate(1);
        if(raw==nullptr) { return nullptr; }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        try { return ::std::forward<Factory>(factory)(raw); }
        catch(...) { A::deallocate(raw); throw; }
#else
        return ::std::forward<Factory>(factory)(raw);
#endif
    }
    template<typename T,typename Factory> [[nodiscard]] T* object(Factory&& factory)
    {
        auto* result=try_object<T>(::std::forward<Factory>(factory));
        if(result==nullptr) { allocation_failed(); }
        return result;
    }
    template<typename T> inline void destroy_object(T* value) noexcept
    {
        if(value==nullptr) { return; }
        ::std::destroy_at(value);
        native_strict_typed_global_allocator<::std::remove_cv_t<T>>::deallocate(const_cast<::std::remove_cv_t<T>*>(value));
    }
    // An explicit count replaces ABI-dependent array-new cookies. The aligned
    // prefix belongs to this allocation, and is never derived from guest data.
    template<typename T> inline constexpr ::std::size_t array_alignment=
        alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__ ? alignof(T) : __STDCPP_DEFAULT_NEW_ALIGNMENT__;
    template<typename T> inline constexpr ::std::size_t array_prefix=
        ((sizeof(::std::size_t)+array_alignment<T>-1)/array_alignment<T>)*array_alignment<T>;
    template<typename T> [[nodiscard]] T* try_array(::std::size_t count,bool value_initialize=true)
    {
        constexpr auto prefix=array_prefix<T>;
        if(count>((::std::numeric_limits<::std::size_t>::max)()-prefix)/sizeof(T)) { return nullptr; }
        auto* raw=static_cast<::std::byte*>(native_strict_global_allocator::allocate_aligned(array_alignment<T>,prefix+count*sizeof(T)));
        if(raw==nullptr) { return nullptr; }
        ::std::memcpy(raw,::std::addressof(count),sizeof(count));
        auto* data=reinterpret_cast<T*>(raw+prefix);
        [[maybe_unused]] ::std::size_t constructed{};
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        try
#endif
        {
            if constexpr(::std::is_nothrow_default_constructible_v<T>)
            {
                if(value_initialize) { ::std::uninitialized_value_construct_n(data,count); }
                else { ::std::uninitialized_default_construct_n(data,count); }
            }
            else
            {
                // The standard uninitialized algorithms may destroy a failed
                // prefix in forward order. Array ownership requires reverse
                // construction order, including constructor-exception rollback.
                for(;constructed!=count;++constructed)
                {
                    if(value_initialize) { ::std::construct_at(data+constructed); }
                    else { ::new(static_cast<void*>(data+constructed)) T; }
                }
            }
        }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        catch(...)
        {
            while(constructed!=0u) { ::std::destroy_at(data+--constructed); }
            native_strict_global_allocator::deallocate_aligned(raw,array_alignment<T>);throw;
        }
#endif
        return data;
    }
    template<typename T> [[nodiscard]] T* array(::std::size_t count,bool value_initialize=true)
    { auto* value=try_array<T>(count,value_initialize);if(value==nullptr) { allocation_failed(); }return value; }
    template<typename T> inline void destroy_array(T* data) noexcept
    {
        if(data==nullptr) { return; }
        auto* raw=reinterpret_cast<::std::byte*>(const_cast<::std::remove_cv_t<T>*>(data))-array_prefix<T>;
        ::std::size_t count{};::std::memcpy(::std::addressof(count),raw,sizeof(count));
        // Match array ownership: destroy the last constructed element first.
        if constexpr(!::std::is_trivially_destructible_v<T>)
        { while(count!=0u) { ::std::destroy_at(data+--count); } }
        native_strict_global_allocator::deallocate_aligned(raw,array_alignment<T>);
    }
}
