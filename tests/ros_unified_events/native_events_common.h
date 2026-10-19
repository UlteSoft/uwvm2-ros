/** @file native_events_common.h
 * @brief Checked finite control protocol shared by independent native peers.
 *
 * The controller proves the stopped ARM mapping before sending I. Every later
 * byte authorizes one fixture phase, not a ROS permission. The peer neither
 * opens Gateway IPC nor supplies Guest identities. Native callbacks update
 * exact counters; the final quiet window detects additional real effects.
 * This fixture uses the C++17 native_io compatibility adapter because official
 * Noetic/Jazzy Gateway SDK builds cannot include C++20 fast_io headers.
 */
#pragma once
#include "../../gateways/common/native_io.h"
#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <poll.h>
#include <stdexcept>
#include <thread>

__attribute__((constructor)) static void unified_events_preinit_stop()
{ if(::raise(SIGSTOP)) { __builtin_trap(); } }

namespace unified_native
{
    namespace io=uwvm2::ros_gateway::native_io;
    using clock=std::chrono::steady_clock;
    inline void require(bool value) {if(!value) {throw std::runtime_error("native seven-event contract refused");}}
    inline void gate(char expected)
    {
        pollfd input{STDIN_FILENO,POLLIN,0};int result;
        do {result=::poll(&input,1,30000);}while(result<0&&errno==EINTR);
        require(result==1&&(input.revents&POLLIN));char byte{};ssize_t read;
        do {read=::read(STDIN_FILENO,&byte,1);}while(read<0&&errno==EINTR);
        require(read==1&&byte==expected);
    }
    inline void emit(std::string_view name,unsigned backend)
    {
        io::fixed_text<256> text;
        require(text.append(name)&&text.append(" backend=")&&text.number(backend)&&
                text.append(" pid=")&&text.number(::getpid())&&text.append("\n"));
        require(io::write_all(STDOUT_FILENO,text.view()));
    }
    template<typename Predicate> void until(Predicate predicate,unsigned seconds=8)
    {
        auto deadline{clock::now()+std::chrono::seconds(seconds)};
        while(!predicate()&&clock::now()<deadline) {std::this_thread::sleep_for(std::chrono::milliseconds(5));}
        require(predicate());
    }
    struct counters
    {
        std::atomic<unsigned> service_calls{},guest_responses{},goals{},feedback{},results{};
        std::atomic<bool> failed{};
        bool exact() const noexcept
        {return !failed&&service_calls==1&&guest_responses==1&&goals==1&&feedback==1&&results==1;}
    };
    inline void quiet(counters const& values,unsigned backend)
    {
        auto begin{clock::now()};unsigned samples{};
        do {require(values.exact());++samples;std::this_thread::sleep_for(std::chrono::milliseconds(20));}
        while(clock::now()-begin<std::chrono::seconds(2));
        io::fixed_text<512> text;
        require(samples>=30&&text.append("PASS_NATIVE_SEVEN_EVENTS backend=")&&text.number(backend)&&
            text.append(" pid=")&&text.number(::getpid())&&text.append(" topic_sent=1 service_calls=1 guest_responses=1 goals=1 feedback=1 results=1 absent_native_requests=0 quiet_samples=")&&
            text.number(samples)&&text.append(" quiet_ns=")&&text.number(std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now()-begin).count())&&text.append("\n"));
        require(io::write_all(STDOUT_FILENO,text.view()));
    }
}
