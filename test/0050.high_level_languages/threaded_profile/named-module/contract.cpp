// SPDX-License-Identifier: Apache-2.0
// Genuine module consumer, no project .h ownership escape or alternate provider.
#include <array>
#include <cassert>
#include <cstdint>
#include <limits>
#include <type_traits>
#ifdef NDEBUG
#error "Assertions are the contract; compile without NDEBUG."
#endif
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/runtime/lib/uwvm_runtime_threaded_profile_feature.h>
#if !UWVM2_HAS_THREADED_PROFILE_HOST_API
#error "This explicit module contract requires the genuine Linux/TLS/native-thread/C++-exception backend profile."
#endif
import uwvm2.runtime;
import uwvm2.uwvm.cmdline.params;
import uwvm2.uwvm.cmdline.callback;
import uwvm2.uwvm.cmdline;
import uwvm2.uwvm.run;
import uwvm2.uwvm.runtime.initializer;
import uwvm2.runtime.compiler.uwvm_int.threaded;
import uwvm2.imported.ros;
import uwvm2.uwvm.imported.ros;
import uwvm2.uwvm.imported.ros.policy_state;
extern "C" bool named_module_default_policy() noexcept;
extern "C" void const* named_module_signal_state() noexcept;
extern "C" void const* named_module_ros_state() noexcept;
namespace runtime=::uwvm2::runtime::lib;
namespace detail=::uwvm2::uwvm::cmdline::params::details;
namespace ros=::uwvm2::uwvm::imported::ros;
static_assert(!::std::is_default_constructible_v<::uwvm2::uwvm::runtime::initializer::ThreadedChildWorld>);
static_assert(!::std::is_default_constructible_v<::uwvm2::runtime::compiler::uwvm_int::threaded::engine>);
// Strong table references check registration and actual global callback identity.
auto volatile module_thread_callback=&detail::wasi_threads_callback;
auto volatile module_signal_callback=&detail::wasi_thread_signals_callback;
auto volatile module_run=&::uwvm2::uwvm::run::run;
auto volatile module_ros_activate=&ros::host::rmw_client_activate;
auto volatile module_ros_peek=&ros::host::rmw_subscription_peek;
int main()
{
    assert(named_module_default_policy());
    assert(named_module_signal_state()==&detail::wasi_thread_signal_config);
    assert(named_module_ros_state()==&ros::runtime_policy);
    assert(!detail::wasi_thread_signal_config.selected());
    assert(!ros::policy_configured && !ros::policy_any_enabled);
    assert(!detail::wasi_threads_existed && !detail::wasi_thread_signals_existed);
    detail::wasi_thread_signal_policy result{};
    assert(detail::wasi_thread_signals_derive({3u,UINT64_MAX,1u,4096u,100u,100u},result));
    assert(result.channel.cookie==UINT64_MAX && result.channel.read_fd==3u);
    auto const old=result;
    assert(!detail::wasi_thread_signals_derive({2u,UINT64_MAX,1u,4096u,100u,100u},result));
    assert(result.channel.cookie==old.channel.cookie && result.channel.read_fd==old.channel.read_fd);
    ::std::uint64_t parsed=73u;
    assert(detail::wasi_thread_signals_number(u8"00018446744073709551615",parsed) && parsed==UINT64_MAX);
    assert(!detail::wasi_thread_signals_number(u8"18446744073709551616",parsed) && parsed==UINT64_MAX);
    // No module import or purely local parser can issue a Source signal sender.
    assert(!runtime::runtime_threaded_profile_signal_sender_host_api());
    assert(named_module_default_policy());
}
