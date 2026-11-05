// A bounded Guest lifecycle result. It never exits the embedding host process.
#pragma once
#include <cstdint>
namespace uwvm2::runtime::lib::guest_exit
{
    struct outcome { bool requested{}, denied{}; ::std::uint32_t code{}; };
    inline thread_local outcome last{};
    inline void clear() noexcept { last = {}; }
#if defined(__cpp_exceptions)
    [[noreturn]] inline void request(bool, ::std::uint32_t);
    class signal final
    {
        outcome result_;
        explicit signal(outcome value) noexcept : result_{value} {}
        friend void request(bool, ::std::uint32_t);
    public:
        [[nodiscard]] outcome result() const noexcept { return result_; }
    };
    [[noreturn]] inline void request(bool denied, ::std::uint32_t code)
    { throw signal{{true, denied, denied ? 126u : code}}; }
#endif
}
