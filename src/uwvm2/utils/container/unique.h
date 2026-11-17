/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include <uwvm2/utils/allocator/fast_io_strict/object.h>
#include <concepts>
namespace uwvm2::utils::container
{
    // Recoverable counterpart of basic_owned_ptr. Retain the concrete allocation
    // and destructor across derived-to-base moves, including multiple inheritance
    // and over-aligned derived objects. No allocation occurs in the pointer itself.
    template<typename T> class strict_owned_ptr
    {
        template<typename> friend class strict_owned_ptr;
        using value_type=::std::remove_extent_t<T>;
        value_type* pointer_{};
        void const* allocation_{};
        void (*dispose_)(void const*) noexcept{};
        void clear() noexcept
        {
            auto* const allocation=allocation_;auto const dispose=dispose_;
            pointer_=nullptr;allocation_=nullptr;dispose_=nullptr;
            if(allocation!=nullptr) { dispose(allocation); }
        }
        template<typename U> static constexpr bool compatible=
            ::std::is_array_v<T> ? ::std::is_convertible_v<U(*)[],value_type(*)[]> : ::std::is_convertible_v<U*,T*>;
    public:
        using element_type=value_type;
        using pointer=value_type*;
        constexpr strict_owned_ptr() noexcept=default;
        constexpr strict_owned_ptr(::std::nullptr_t) noexcept {}
        template<typename U> requires compatible<U>
        explicit strict_owned_ptr(U* value) noexcept { reset(value); }
        strict_owned_ptr(strict_owned_ptr const&)=delete;
        strict_owned_ptr& operator=(strict_owned_ptr const&)=delete;
        strict_owned_ptr(strict_owned_ptr&& other) noexcept
            :pointer_(::std::exchange(other.pointer_,nullptr)),allocation_(::std::exchange(other.allocation_,nullptr)),dispose_(::std::exchange(other.dispose_,nullptr)) {}
        template<typename U> requires (!::std::is_array_v<T> && !::std::is_array_v<U> && ::std::is_convertible_v<U*,T*>)
        strict_owned_ptr(strict_owned_ptr<U>&& other) noexcept
            :pointer_(::std::exchange(other.pointer_,nullptr)),allocation_(::std::exchange(other.allocation_,nullptr)),dispose_(::std::exchange(other.dispose_,nullptr)) {}
        strict_owned_ptr& operator=(strict_owned_ptr&& other) noexcept
        { if(this!=::std::addressof(other)) { strict_owned_ptr{::std::move(other)}.swap(*this); }return *this; }
        template<typename U> requires (!::std::is_array_v<T> && !::std::is_array_v<U> && ::std::is_convertible_v<U*,T*>)
        strict_owned_ptr& operator=(strict_owned_ptr<U>&& other) noexcept
        { strict_owned_ptr{::std::move(other)}.swap(*this);return *this; }
        strict_owned_ptr& operator=(::std::nullptr_t) noexcept { clear();return *this; }
        ~strict_owned_ptr() { clear(); }
        [[nodiscard]] pointer get() const noexcept { return pointer_; }
        [[nodiscard]] explicit operator bool() const noexcept { return pointer_!=nullptr; }
        [[nodiscard]] bool operator==(::std::nullptr_t) const noexcept { return pointer_==nullptr; }
        [[nodiscard]] value_type& operator*() const noexcept requires (!::std::is_array_v<T>) { return *pointer_; }
        [[nodiscard]] pointer operator->() const noexcept requires (!::std::is_array_v<T>) { return pointer_; }
        [[nodiscard]] value_type& operator[](::std::size_t index) const noexcept requires ::std::is_array_v<T> { return pointer_[index]; }
        [[nodiscard]] pointer release() noexcept
        { auto* result=pointer_;pointer_=nullptr;allocation_=nullptr;dispose_=nullptr;return result; }
        void reset(::std::nullptr_t=nullptr) noexcept { clear(); }
        template<typename U> requires compatible<U> void reset(U* value) noexcept
        {
            if(pointer_==value) { return; }
            clear();pointer_=value;allocation_=value;
            dispose_=[](void const* raw) noexcept {
                auto* actual=static_cast<U*>(const_cast<void*>(raw));
                if constexpr(::std::is_array_v<T>) { ::uwvm2::utils::allocator::fast_io_strict::destroy_array(actual); }
                else { ::uwvm2::utils::allocator::fast_io_strict::destroy_object(actual); }
            };
        }
        void swap(strict_owned_ptr& other) noexcept
        { ::std::swap(pointer_,other.pointer_);::std::swap(allocation_,other.allocation_);::std::swap(dispose_,other.dispose_); }
    };
    // Arrays retain their original pointer-sized representation; their explicit
    // count lives in the allocation prefix, never in the hot GC object layout.
    template<typename T> class strict_owned_ptr<T[]>
    {
        T* pointer_{};
    public:
        using element_type=T;using pointer=T*;
        constexpr strict_owned_ptr() noexcept=default;
        constexpr strict_owned_ptr(::std::nullptr_t) noexcept {}
        explicit strict_owned_ptr(T* p) noexcept : pointer_(p) {}
        strict_owned_ptr(strict_owned_ptr const&)=delete;
        strict_owned_ptr& operator=(strict_owned_ptr const&)=delete;
        strict_owned_ptr(strict_owned_ptr&& other) noexcept : pointer_(other.release()) {}
        strict_owned_ptr& operator=(strict_owned_ptr&& other) noexcept
        { if(this!=::std::addressof(other)) { reset(other.release()); }return *this; }
        strict_owned_ptr& operator=(::std::nullptr_t) noexcept { reset();return *this; }
        ~strict_owned_ptr() { reset(); }
        [[nodiscard]] pointer get() const noexcept { return pointer_; }
        [[nodiscard]] explicit operator bool() const noexcept { return pointer_!=nullptr; }
        [[nodiscard]] bool operator==(::std::nullptr_t) const noexcept { return pointer_==nullptr; }
        [[nodiscard]] T& operator[](::std::size_t i) const noexcept { return pointer_[i]; }
        [[nodiscard]] pointer release() noexcept { return ::std::exchange(pointer_,nullptr); }
        void reset(T* p=nullptr) noexcept
        { if(pointer_!=p) { auto* old=::std::exchange(pointer_,p);::uwvm2::utils::allocator::fast_io_strict::destroy_array(old); } }
        void swap(strict_owned_ptr& other) noexcept { ::std::swap(pointer_,other.pointer_); }
    };
    template<typename T,typename D=void>
    using std_unique_ptr=::std::conditional_t<::std::is_void_v<D>,strict_owned_ptr<T>,::std::unique_ptr<T,D>>;
    template<typename T,typename... Args> requires (!::std::is_array_v<T>)
    [[nodiscard]] inline strict_owned_ptr<T> make_unique(Args&&... args)
    {
        return strict_owned_ptr<T>{::uwvm2::utils::allocator::fast_io_strict::object<T>([&](T* p) {
            return ::std::construct_at(p,::std::forward<Args>(args)...); })};
    }
    template<typename T> requires ::std::is_unbounded_array_v<T>
    [[nodiscard]] inline strict_owned_ptr<T> make_unique(::std::size_t count)
    { return strict_owned_ptr<T>{::uwvm2::utils::allocator::fast_io_strict::array<::std::remove_extent_t<T>>(count)}; }
    template<typename T> requires ::std::is_unbounded_array_v<T>
    [[nodiscard]] inline strict_owned_ptr<T> make_unique_for_overwrite(::std::size_t count)
    { return strict_owned_ptr<T>{::uwvm2::utils::allocator::fast_io_strict::array<::std::remove_extent_t<T>>(count,false)}; }

}
