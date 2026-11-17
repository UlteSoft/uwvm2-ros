// Shared snapshot/cache policy DATA. This does not grant runtime authority.
#pragma once
#ifndef UWVM_MODULE
# include <cstdint>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::checkpoint::feature_policy_schema
{
    inline constexpr ::std::uint64_t revision{3u};
    inline constexpr ::std::uint64_t disable_mask{(::std::uint64_t{1u} << 20u)-1u};
    inline constexpr ::std::uint64_t known_feature_mask{(::std::uint64_t{1u} << 19u)-1u};
    [[nodiscard]] inline constexpr bool valid(::std::uint64_t mode, ::std::uint64_t disabled,
        ::std::uint64_t controlled) noexcept
    { return mode <= 5u && (disabled & ~disable_mask) == 0u && controlled <= 3u; }
}
