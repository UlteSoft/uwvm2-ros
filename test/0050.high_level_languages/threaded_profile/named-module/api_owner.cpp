// SPDX-License-Identifier: Apache-2.0
#include <cstddef>
#include <type_traits>
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/runtime/lib/uwvm_runtime_threaded_profile_feature.h>
#if !UWVM2_HAS_THREADED_PROFILE_HOST_API
#error "This explicit module contract requires the genuine Linux/TLS/native-thread/C++-exception backend profile."
#endif
import uwvm2.runtime;
import uwvm2.uwvm.runtime.initializer.limits;
import uwvm2.uwvm.runtime.storage;
namespace runtime=::uwvm2::runtime::lib;
namespace threads=::uwvm2::runtime::wasm_threads;
using actual_limits=::uwvm2::uwvm::runtime::initializer::initializer_limit_t;
static_assert(::std::is_same_v<decltype(threads::threaded_profile_limits{}.initializer),actual_limits>);
static_assert(!::std::is_default_constructible_v<threads::ThreadedSpawnAdmission>);
static_assert(!::std::is_default_constructible_v<runtime::threaded_signal_sender>);
static_assert(!::std::is_copy_constructible_v<runtime::threaded_management_exclusion>);
static_assert(!::std::is_move_constructible_v<runtime::threaded_management_exclusion>);
// Volatile address slots retain each REAL external definition, including when
// no Guest is admitted. An omitted bridge/implementation must fail final link.
auto volatile anchor_runtime_threaded_profile_configure_host_api=&runtime::runtime_threaded_profile_configure_host_api;
auto volatile anchor_runtime_threaded_profile_policy_host_api=&runtime::runtime_threaded_profile_policy_host_api;
auto volatile anchor_runtime_threaded_profile_builtin_requested_host_api=&runtime::runtime_threaded_profile_builtin_requested_host_api;
auto volatile anchor_runtime_threaded_profile_start_main_host_api=&runtime::runtime_threaded_profile_start_main_host_api;
auto volatile anchor_runtime_threaded_profile_wait_host_api=&runtime::runtime_threaded_profile_wait_host_api;
auto volatile anchor_runtime_threaded_profile_retire_host_api=&runtime::runtime_threaded_profile_retire_host_api;
auto volatile anchor_runtime_threaded_profile_result_host_api=&runtime::runtime_threaded_profile_result_host_api;
auto volatile anchor_runtime_threaded_profile_try_management_host_api=&runtime::runtime_threaded_profile_try_management_host_api;
auto volatile anchor_runtime_threaded_profile_try_management_under_publication_host_api=&runtime::runtime_threaded_profile_try_management_under_publication_host_api;
auto volatile anchor_runtime_threaded_profile_recheck_management_host_api=&runtime::runtime_threaded_profile_recheck_management_host_api;
auto volatile anchor_runtime_threaded_profile_debug_observation_configure_host_api=&runtime::runtime_threaded_profile_debug_observation_configure_host_api;
auto volatile anchor_runtime_threaded_profile_debug_session_host_api=&runtime::runtime_threaded_profile_debug_session_host_api;
auto volatile anchor_runtime_threaded_profile_debug_control_configure_host_api=&runtime::runtime_threaded_profile_debug_control_configure_host_api;
auto volatile anchor_runtime_threaded_profile_debug_control_session_host_api=&runtime::runtime_threaded_profile_debug_control_session_host_api;
auto volatile anchor_runtime_threaded_profile_signal_configure_host_api=&runtime::runtime_threaded_profile_signal_configure_host_api;
auto volatile anchor_runtime_threaded_profile_signal_sender_host_api=&runtime::runtime_threaded_profile_signal_sender_host_api;
auto volatile anchor_runtime_threaded_profile_signal_push_host_api=&runtime::runtime_threaded_profile_signal_push_host_api;
auto volatile anchor_runtime_threaded_profile_signal_observe_host_api=&runtime::runtime_threaded_profile_signal_observe_host_api;
auto volatile anchor_runtime_threaded_profile_signal_status_host_api=&runtime::runtime_threaded_profile_signal_status_host_api;

extern "C" bool named_module_default_policy() noexcept
{
    threads::threaded_profile_limits observed{};
    return runtime::runtime_threaded_profile_policy_host_api(observed)==runtime::threaded_policy_status::disabled &&
        !runtime::runtime_threaded_profile_builtin_requested_host_api() &&
        !observed.max_active_children && !observed.max_spawn_attempts && !observed.max_live_storage_bytes;
}
