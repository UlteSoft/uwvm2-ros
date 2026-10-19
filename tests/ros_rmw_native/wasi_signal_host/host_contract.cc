// SPDX-License-Identifier: Apache-2.0
#include "host_contract.hpp"
#if !UWVM2_HAS_THREADED_PROFILE_HOST_API
#error This real Host contract requires the actual Linux threaded Runtime feature.
#endif
// Link this component into Root's genuine Native Runtime driver. These wrappers
// retain actual strong Runtime references; there is deliberately no local main,
// fake initializer, replacement admission constructor or weak success fallback.
extern "C++" uwvm_signal_host_contract::result uwvm_signal_host_two_actual_sources(
    uwvm_signal_host_contract::source_input const&a,
    uwvm_signal_host_contract::source_input const&b,
    ::uwvm2::runtime::wasm_threads::threaded_profile_limits const&limits,
    ::std::chrono::steady_clock::time_point deadline) noexcept
{return uwvm_signal_host_contract::two_actual_sources(a,b,limits,deadline);}
extern "C++" int uwvm_signal_host_forbidden_import(
    uwvm_signal_host_contract::source_input const&input,
    ::uwvm2::runtime::wasm_threads::threaded_profile_limits const&limits) noexcept
{return uwvm_signal_host_contract::forbidden_import_source(input,limits);}
