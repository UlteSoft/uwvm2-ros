// SPDX-License-Identifier: Apache-2.0
#include <cstddef>
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/runtime/lib/uwvm_runtime_threaded_profile_feature.h>
#if !UWVM2_HAS_THREADED_PROFILE_HOST_API
#error "This explicit module contract requires the genuine Linux/TLS/native-thread/C++-exception backend profile."
#endif
import uwvm2.uwvm.cmdline.params;
import uwvm2.uwvm.imported.ros.policy_state;
extern "C" void const* named_module_signal_state() noexcept
{return &::uwvm2::uwvm::cmdline::params::details::wasi_thread_signal_config;}
extern "C" void const* named_module_ros_state() noexcept
{return &::uwvm2::uwvm::imported::ros::runtime_policy;}
