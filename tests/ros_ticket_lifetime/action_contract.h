#pragma once
/** @file
 * @brief Exercise the actual native Action allocation entry point without a Node.
 * This supplements the shared sequence and Service lifecycle regressions. It
 * proves the selected Action wrapper uses the maintained no-reuse allocator;
 * it does not claim native goal execution, callback, cancel or DDS coverage.
 */
#include "../ros_ipc/probe_common.h"
#include <algorithm>
#include <array>
#include <atomic>
#include <csignal>
#include <exception>
#include <thread>
#include <vector>

namespace ticket_action_test
{
    constexpr std::size_t completed{1048576+32};
    constexpr unsigned threads{8},per_thread{16384};
    inline void need(bool value,char const* message) { ros_probe::require(value,message); }
    template<class Allocate> void domain(Allocate allocate)
    {
        // Preserve every returned ID in this bounded test oracle, including
        // the sequential prefix. The production allocator retains no oracle.
        std::vector<std::uint64_t> observed(completed+threads*per_thread);
        for(std::size_t i{};i<completed;++i) { observed[i]=allocate(); }
        std::atomic<bool> go{};std::array<std::exception_ptr,threads> errors{};
        std::vector<std::thread> actors;
        try
        {
            for(unsigned t{};t<threads;++t) { actors.emplace_back([&,t] {
                while(!go.load(std::memory_order_acquire)) { std::this_thread::yield(); }
                try { for(unsigned i{};i<per_thread;++i) { observed[completed+t*per_thread+i]=allocate(); } }
                catch(...) { errors[t]=std::current_exception(); }
            }); }
        }
        catch(...) { go.store(true,std::memory_order_release);for(auto& actor:actors) {actor.join();}throw; }
        go.store(true,std::memory_order_release);for(auto& actor:actors) {actor.join();}
        for(auto const& error:errors) { if(error) { std::rethrow_exception(error); } }
        std::sort(observed.begin(),observed.end());
        need(observed.front()&&std::adjacent_find(observed.begin(),observed.end())==observed.end(),
            "actual Action allocator exhausted at the old cap or reused sequential/parallel IDs");
    }
    inline void options(int argc,char** argv)
    {
        if(argc==2&&std::string_view{argv[1]}=="--owned-stop") { ::raise(SIGSTOP); }
        else { need(argc==1,"only explicit --owned-stop fixture option is supported"); }
    }
}
