// Licensed under the APL-2.0 License (see LICENSE file).
#pragma once
#ifndef UWVM_MODULE
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <atomic>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <new>
#include <string>
#include <string_view>
#include <vector>
#include <uwvm2/utils/thread/monotonic_clock.h>
#endif

// Detached DEV-only observations. No native address, guest memory borrow,
// capability, management callback or executable object is retained.
#ifndef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::utils::debug::host_calls
{
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
    inline constexpr bool supported=true;
#else
    inline constexpr bool supported=false;
#endif
    enum class channel : unsigned { wasi=1, ros1=2, ros2=4 };
    inline std::atomic<unsigned> enabled{};
    inline bool observing(channel c) noexcept {return supported&&(enabled.load(std::memory_order_relaxed)&static_cast<unsigned>(c))!=0;}
    inline std::uint64_t now() noexcept
    {
        auto t=host_clock::now();
        return host_clock::available()?static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count()):0;
    }
    struct segment
    {
        std::uint32_t memory{};
        std::uint64_t offset{};
        bool output{},snapshot{};
        std::vector<std::byte> bytes;
    };
    struct record
    {
        std::uint64_t id{},participant{},module{},generation{},environment{},started{},duration{},result{};
        channel family{};
        std::string name_space,name,module_name;
        std::array<std::uint64_t,16> arguments{};
        std::array<unsigned char,16> widths{};
        std::size_t count{};
        bool returned{},has_result{},timed{},incomplete{};
        std::vector<segment> segments;
    };
    inline std::mutex mutex;
    inline std::vector<std::shared_ptr<record>> records;
    inline std::uint64_t sequence{},epoch{1},cleared_through{};
    inline std::atomic<std::uint64_t> allocation_failures{},invalid_ranges{},sequence_failures{};
    inline thread_local record* active{};
    inline void increment(std::atomic<std::uint64_t>& c) noexcept
    {auto v=c.load(std::memory_order_relaxed);while(v!=UINT64_MAX&&!c.compare_exchange_weak(v,v+1,std::memory_order_relaxed)) {}}
    inline void missing_range() noexcept
    {
        if(!active) {return;}
        std::lock_guard lock{mutex};active->incomplete=true;increment(invalid_ranges);
    }
    // The supplied bytes must be owned by the host, or protected by the original
    // linear-memory read lease. A failed optional copy never changes the ABI call.
    inline void capture(std::uint32_t memory,std::uint64_t offset,void const* bytes,std::size_t size,bool output,bool snapshot=false) noexcept
    {
        if(!active) {return;}
        std::lock_guard lock{mutex};
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        try
#endif
        {
            segment s{memory,offset,output,snapshot,{}};
            s.bytes.resize(size);
            if(size) {std::memcpy(s.bytes.data(),bytes,size);}
            active->segments.push_back(std::move(s));
        }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        catch(...) {active->incomplete=true;increment(allocation_failures);}
#endif
    }
    struct scope
    {
        std::shared_ptr<record> value;
        record* previous{active};
        scope(channel c,std::string_view ns,std::string_view name) noexcept
        {
            active=nullptr;
            if(!observing(c)) {return;}
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
            try
#endif
            {
                value=std::make_shared<record>();value->family=c;
                value->name_space=ns;value->name=name;
                std::lock_guard lock{mutex};
                if(sequence==UINT64_MAX) {increment(sequence_failures);value.reset();return;}
                records.push_back(value);value->id=++sequence;value->started=now();
                active=value.get();
            }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
            catch(...) {value.reset();increment(allocation_failures);}
#endif
        }
        scope(scope const&)=delete;
        ~scope() {active=previous;}
        void identity(std::string_view module_name,std::uint64_t generation) noexcept
        {
            if(!value) {return;}
            std::lock_guard lock{mutex};
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
            try
#endif
            {value->module_name=module_name;value->generation=generation;}
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
            catch(...) {value->incomplete=true;increment(allocation_failures);}
#endif
        }
        void finish(bool has_result,std::uint64_t result) noexcept
        {
            if(!value) {return;}
            std::lock_guard lock{mutex};value->has_result=has_result;value->result=result;value->returned=true;
            auto end=now();value->timed=value->started&&end>=value->started;
            value->duration=value->timed?end-value->started:0;
        }
    };
    inline void clear() noexcept
    {
        std::lock_guard lock{mutex};records.clear();cleared_through=sequence;
        if(epoch!=UINT64_MAX) {++epoch;}
        // In-flight scopes own their retired record until they leave. Their
        // completion never republishes it into a newer recording episode.
    }
}
