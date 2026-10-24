// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <array>
#include <cerrno>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <uwvm2/runtime/lib/uwvm_runtime.h>
#include <uwvm2/runtime/wasm_threads/thread_group.h>

// This is a real Runtime contract component, not a stand-alone fake Runtime.
// Root's existing real source loader supplies initialize_source and the actual
// numeric entry ordinal after parsing the compiled fixture. No predicted index,
// caller-created Source/admission, synthetic Group or success callback is used.
namespace uwvm_signal_host_contract
{
#if UWVM2_HAS_THREADED_PROFILE_HOST_API
namespace runtime=::uwvm2::runtime::lib;
using sender_owner=::std::shared_ptr<runtime::threaded_signal_sender const>;
static_assert(!::std::is_default_constructible_v<runtime::threaded_signal_sender>);
static_assert(!::std::is_copy_constructible_v<runtime::threaded_signal_sender>);
static_assert(!::std::is_constructible_v<runtime::threaded_signal_sender,::std::uint64_t>);
struct source_input
{
    bool (*initialize_source)(void*) noexcept{};
    void* actual_loader_state{};
    ::uwvm2::utils::container::u8string_view actual_module_name{};
    ::std::size_t actual_entry{};
};
struct result
{
    unsigned checks{};
    int failure{};
    ::std::uint64_t first_generation{},second_generation{};
    runtime::threaded_retirement first_join{},second_join{};
    runtime::threaded_signal_report first{},second{};
};
inline bool same_counters(runtime::threaded_signal_report const&a,runtime::threaded_signal_report const&b) noexcept
{return a.sent_frames==b.sent_frames&&a.last_sequence==b.last_sequence&&a.native_write_attempts==b.native_write_attempts;}

// Requires a fresh dedicated Runtime process and two genuinely initialized
// Sources of frame_reader.wat. Each initialize callback is invoked only by the
// existing privileged replace_full_source_after_drain_host_api. Its return value
// alone cannot mint the canonical Source/admission checked by start_main.
inline result two_actual_sources(source_input const&first,source_input const&second,
    ::uwvm2::runtime::wasm_threads::threaded_profile_limits const&finite_limits,
    ::std::chrono::steady_clock::time_point deadline) noexcept
{
    result out{};auto check=[&](bool value,int failure)noexcept
    {++out.checks;if(!value&&!out.failure)out.failure=failure;return value;};
    if(!check(first.initialize_source&&second.initialize_source,1))return out;
    if(!check(!runtime::runtime_threaded_profile_signal_sender_host_api(),2))return out;
    // Invalid partial policy is rejected. This does not enable ordinary threads.
    if(!check(!runtime::runtime_threaded_profile_signal_configure_host_api({3u,0u,4u,1048576u}),3))return out;
    sender_owner old;
    auto run=[&](source_input const&input,runtime::threaded_signal_report&facts,
        runtime::threaded_retirement&joined,::std::uint64_t&generation)noexcept -> bool
    {
        if(!check(::std::chrono::steady_clock::now()<deadline,4))return false;
        if(!check(runtime::replace_full_source_after_drain_host_api(input.initialize_source,input.actual_loader_state),5))return false;
        if(!check(runtime::runtime_threaded_profile_configure_host_api(finite_limits),6))return false;
        // Actual Root request. Cookie is public association and intentionally
        // repeats between Sources; it must never authorize an old sender.
        if(!check(runtime::runtime_threaded_profile_signal_configure_host_api({3u,0x105u,4u,1048576u}),7))return false;
        if(!check(runtime::runtime_threaded_profile_start_main_host_api(input.actual_module_name,input.actual_entry,{},4u)==
            runtime::threaded_main_status::running,8))return false;
        auto current=runtime::runtime_threaded_profile_signal_sender_host_api();
        if(!check(static_cast<bool>(current),9))return false;
        if(!check(runtime::runtime_threaded_profile_signal_observe_host_api(*current,facts)==0&&facts.installed&&!facts.retired&&
            facts.read_fd==3u&&facts.cookie==0x105u&&facts.pipe_capacity>=32u&&facts.sent_frames==0u,10))return false;
        generation=facts.source_generation;
        if(!check(!runtime::runtime_threaded_profile_signal_configure_host_api({}),11))return false;
        auto before=facts;
        if(!check(runtime::runtime_threaded_profile_signal_push_host_api(*current,static_cast<runtime::threaded_signal_event>(0u))==EINVAL,12))return false;
        if(!check(runtime::runtime_threaded_profile_signal_observe_host_api(*current,facts)==0&&same_counters(before,facts),13))return false;
        if(old)
        {
            if(!check(runtime::runtime_threaded_profile_signal_push_host_api(*old,runtime::threaded_signal_event::terminate)==ENOTCONN,14))return false;
            auto untouched=before;untouched.cookie=0xabcdefu;
            if(!check(runtime::runtime_threaded_profile_signal_observe_host_api(*old,untouched)==ENOTCONN&&untouched.cookie==0xabcdefu,15))return false;
            if(!check(runtime::runtime_threaded_profile_signal_observe_host_api(*current,facts)==0&&same_counters(before,facts),16))return false;
        }
        if(!check(runtime::runtime_threaded_profile_signal_push_host_api(*current,runtime::threaded_signal_event::interrupt)==0,17))return false;
        // One real absolute deadline and true Group native/compiler joins.
        // A timeout is a failure, never a forged completion or a detached owner.
        joined=runtime::runtime_threaded_profile_wait_host_api(deadline);
        if(!check(joined.permits(runtime::threaded_management_kind::full_source_reset),18))return false;
        if(!check(runtime::runtime_threaded_profile_signal_observe_host_api(*current,facts)==0&&!facts.installed&&facts.retired&&
            facts.sent_frames==1u&&facts.last_sequence==1u&&facts.native_write_attempts==1u&&
            facts.read_close_attempted&&facts.write_close_attempted&&facts.read_close_error==0&&facts.write_close_error==0,19))return false;
        ::std::array<::std::byte,4> bytes{};::std::uint32_t code{};unsigned kind{};
        if(!check(runtime::runtime_threaded_profile_result_host_api(bytes,&code,&kind)==runtime::threaded_main_status::finished&&
            bytes[0]==::std::byte{1}&&bytes[1]==::std::byte{}&&bytes[2]==::std::byte{}&&bytes[3]==::std::byte{}&&code==0u&&
            kind==static_cast<unsigned>(::uwvm2::runtime::wasm_threads::termination_kind::main_return),20))return false;
        if(!check(runtime::runtime_threaded_profile_signal_push_host_api(*current,runtime::threaded_signal_event::interrupt)==ENOTCONN,21))return false;
        old=::std::move(current);return true;
    };
    if(!run(first,out.first,out.first_join,out.first_generation))return out;
    if(!run(second,out.second,out.second_join,out.second_generation))return out;
    check(out.first_generation!=out.second_generation,22);
    return out;
}

// Dedicated fresh Source for either mutation-import negative fixture. Actual
// parsing/validation may reject it even earlier; this component requires the
// real loader to install it, then proves this profile refuses before a pipe or
// sender is installed. It does not replace the producer's independent MD census.
inline int forbidden_import_source(source_input const&input,
    ::uwvm2::runtime::wasm_threads::threaded_profile_limits const&finite_limits) noexcept
{
    if(!input.initialize_source||!runtime::replace_full_source_after_drain_host_api(input.initialize_source,input.actual_loader_state))return 1;
    if(!runtime::runtime_threaded_profile_configure_host_api(finite_limits)||
       !runtime::runtime_threaded_profile_signal_configure_host_api({3u,0x105u,4u,1048576u}))return 2;
    if(runtime::runtime_threaded_profile_start_main_host_api(input.actual_module_name,input.actual_entry,{},4u)!=runtime::threaded_main_status::rejected)return 3;
    if(runtime::runtime_threaded_profile_signal_sender_host_api())return 4;
    runtime::threaded_signal_report actual{};
    if(runtime::runtime_threaded_profile_signal_status_host_api(actual)!=0||actual.installed||actual.native_write_attempts!=0u)return 5;
    return 0;
}
#endif
}
