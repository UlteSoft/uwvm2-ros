#pragma once
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>
#include <fast_io.h>
namespace uwvm2::utils::container
{
    // Borrowed synchronous callback. Never store this view beyond the call
    // receiving it; caller-owned callable lifetime covers every invocation.
    template<typename Signature> class function_ref;
    template<typename Result,typename... Args> class function_ref<Result(Args...)>
    {
        union target { void const* object;Result(*function)(Args...);constexpr target():object{}{} } target_;
        Result(*invoke_)(target const&,Args...){};
    public:
        constexpr function_ref() noexcept=default;
        constexpr function_ref(::std::nullptr_t) noexcept {}
        function_ref(Result(*fn)(Args...)) noexcept
        {
            target_.function=fn;
            if(fn)invoke_=[](target const& t,Args... args)->Result { return t.function(::std::forward<Args>(args)...); };
        }
        template<typename F> requires (!::std::is_same_v<::std::remove_cvref_t<F>,function_ref> &&
            !::std::is_function_v<::std::remove_reference_t<F>> && !::std::is_pointer_v<::std::remove_cvref_t<F>> &&
            ::std::is_invocable_r_v<Result,F&,Args...>)
        function_ref(F&& fn) noexcept
        {
            target_.object=::std::addressof(fn);
            invoke_=[](target const& t,Args... args)->Result {
                auto& fn=*static_cast<::std::remove_reference_t<F>*>(const_cast<void*>(t.object));
                if constexpr(::std::is_void_v<Result>) { ::std::invoke(fn,::std::forward<Args>(args)...); }
                else { return ::std::invoke(fn,::std::forward<Args>(args)...); }
            };
        }
        [[nodiscard]] explicit operator bool() const noexcept { return invoke_!=nullptr; }
        Result operator()(Args... args) const
        { if(!invoke_) { ::fast_io::fast_terminate(); }return invoke_(target_,::std::forward<Args>(args)...); }
    };
}
