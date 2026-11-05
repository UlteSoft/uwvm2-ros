// Licensed under the APL-2.0 License (see LICENSE file).
#pragma once
#ifndef UWVM_MODULE
#include <cstdint>
#include <limits>
#include <fast_io.h>
#endif
#ifndef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::utils::host_clock::details
{
    // The only host monotonic sample implementation, also used by the explicit
    // C++20 bridge for native SDK dependencies that still require C++17.
    [[nodiscard]] inline bool read_monotonic_ns(::std::int64_t& value) noexcept
    {
#if defined(__cpp_exceptions)
        try
#endif
        {
            auto const sample{::fast_io::posix_clock_gettime(::fast_io::posix_clock_id::monotonic)};
            constexpr auto scale{::fast_io::uint_least64_subseconds_per_second/1000000000u};
            constexpr auto maximum{::std::numeric_limits<::std::int64_t>::max()};
            auto const fraction{sample.subseconds/scale};
            if(sample.seconds<0 || sample.subseconds>=::fast_io::uint_least64_subseconds_per_second ||
               sample.seconds>maximum/1000000000 ||
               (sample.seconds==maximum/1000000000 && fraction>static_cast<::std::uint64_t>(maximum%1000000000)))return false;
            value=static_cast<::std::int64_t>(sample.seconds)*1000000000+static_cast<::std::int64_t>(fraction);
            return true;
        }
#if defined(__cpp_exceptions)
        catch(...){return false;}
#endif
    }
}
