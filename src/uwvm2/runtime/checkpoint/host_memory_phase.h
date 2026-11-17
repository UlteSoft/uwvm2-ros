// Licensed under the APL-2.0 License (see LICENSE file).
#pragma once
#include <type_traits>

// Native-only, stack-borrowed connection from an authenticated checkpoint
// import to audited host memory transactions. Never populated from guest or
// snapshot data. The ordinary provider has no installed consumer.
namespace uwvm2::runtime::checkpoint::host_memory_phase
{
    struct consumer
    {
        void* context{};
        void (*begin)(void*, bool) noexcept{};
        void (*end)(void*) noexcept{};
    };
    inline thread_local consumer const* current{};

    class scope final
    {
        consumer const* actual_{};
    public:
        explicit constexpr scope(bool writing) noexcept
        {
            if(!::std::is_constant_evaluated())
            {
                actual_=current;
                if(actual_)actual_->begin(actual_->context,writing);
            }
        }
        constexpr ~scope()
        {
            if(!::std::is_constant_evaluated() && actual_)actual_->end(actual_->context);
        }
        scope(scope const&)=delete;
        scope& operator=(scope const&)=delete;
    };
}
