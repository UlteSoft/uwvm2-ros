/** @file @brief Trusted communication owner identity, independent of any guest carrier. */
#pragma once
#include <cstdint>
namespace uwvm2::imported::ros::scene::communication
{
    struct owner
    {
        std::uint64_t environment{},grant{},generation{};
        friend bool operator==(owner const&,owner const&)=default;
        explicit operator bool()const noexcept{return environment&&grant&&generation;}
    };
}
