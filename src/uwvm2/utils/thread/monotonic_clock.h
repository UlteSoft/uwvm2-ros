// Licensed under the APL-2.0 License (see LICENSE file).
#pragma once
#ifndef UWVM_MODULE
#include <chrono>
#include <cstdint>
#include <limits>
#if !defined(UWVM_HOST_CLOCK_CXX17_BRIDGE)
#include "monotonic_sample.h"
#endif
#if defined(__linux__)
#include <sys/timerfd.h>
#endif
#endif
#ifndef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
#endif
#if defined(UWVM_HOST_CLOCK_CXX17_BRIDGE)
extern "C" bool uwvm_host_clock_read_monotonic_ns(::std::int64_t*) noexcept;
#endif
UWVM_MODULE_EXPORT namespace uwvm2::utils::host_clock
{
    // Linux steady-clock deadlines and CLOCK_MONOTONIC use the same epoch.
    // Chrono supplies duration arithmetic only; every observation comes from
    // the same fast_io POSIX clock used to arm the kernel timer. Guest clock
    // grants, wall-clock changes and process CPU time cannot affect this clock.
    using clock=::std::chrono::steady_clock;
    inline thread_local bool read_failed{};
    inline thread_local clock::time_point last_sample{};
    [[nodiscard]] inline bool available() noexcept {return !read_failed;}
    [[nodiscard]] inline clock::time_point now() noexcept
    {
        // Failure belongs to this observation. A later successful sample can
        // resume the existing ledger, without resetting it or inventing time.
        read_failed=false;
        ::std::int64_t ns{};
#if defined(UWVM_HOST_CLOCK_CXX17_BRIDGE)
        bool const sampled{::uwvm_host_clock_read_monotonic_ns(&ns)};
#else
        bool const sampled{details::read_monotonic_ns(ns)};
#endif
        if(!sampled){read_failed=true;return last_sample;}
        auto const result{clock::time_point{::std::chrono::duration_cast<clock::duration>(::std::chrono::nanoseconds{ns})}};
        if(result<last_sample){read_failed=true;return last_sample;}
        last_sample=result;return result;
    }
#if defined(__linux__)
    [[nodiscard]] inline bool arm_timer(int fd,clock::time_point deadline) noexcept
    {
        if(!available())return false;
        ::itimerspec setting{};
        if(deadline!=clock::time_point::max())
        {
            auto stamp{::std::chrono::duration_cast<::std::chrono::nanoseconds>(deadline.time_since_epoch()).count()};
            if(stamp<=0)stamp=1; // zero disarms timerfd, even for an expired deadline.
            setting.it_value.tv_sec=static_cast<::time_t>(stamp/1000000000);
            setting.it_value.tv_nsec=static_cast<long>(stamp%1000000000);
        }
        return ::timerfd_settime(fd,TFD_TIMER_ABSTIME,&setting,nullptr)==0;
    }
#endif
}
