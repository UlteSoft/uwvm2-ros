/** @file
 * @brief Read-only imports available to an explicitly bound ROSv2 caller.
 * @details The ABI query returns a constant. The self report reads the caller's
 * approved policy. Each import admits an independent deny or hidden rule.
 */
#pragma once
#include <array>
#include <string_view>

extern "C++" {
namespace uwvm2::imported::ros::scene
{
    inline constexpr std::array<std::string_view, 2> bound_queries{"abi", "permission.report"};

    inline constexpr bool is_bound_query(std::string_view name) noexcept
    {
        for (auto query : bound_queries)
            if (query == name) return true;
        return false;
    }
}
}
