#pragma once
namespace uwvm2::runtime::compiler::uwvm_int::threaded
{
    // One canonical raw slot also carries v128 through locals, branch tuples,
    // host ABI packets and immutable exception payloads without truncation.
    struct alignas(16) stack_value
    {
        ::std::uint64_t low{},high{};
        constexpr stack_value() noexcept = default;
        constexpr stack_value(::std::uint64_t scalar) noexcept:low{scalar} {}
        friend bool operator==(stack_value const&,stack_value const&) = default;
    };
    static_assert(sizeof(stack_value)==16u && ::std::is_trivially_copyable_v<stack_value>);
}
