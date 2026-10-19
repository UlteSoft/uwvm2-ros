// SPDX-License-Identifier: Apache-2.0
// Include INSIDE uwvm2::runtime::lib after the real bridge/function definitions.
extern "C++" bool runtime_threaded_profile_signal_configure_host_api(threaded_signal_policy const&value) noexcept
{return runtime_threaded_profile_bridge::configure_signal(value);}
extern "C++" ::std::shared_ptr<threaded_signal_sender const> runtime_threaded_profile_signal_sender_host_api() noexcept
{return runtime_threaded_profile_bridge::signal_sender();}
extern "C++" int runtime_threaded_profile_signal_push_host_api(threaded_signal_sender const&sender,threaded_signal_event event) noexcept
{return runtime_threaded_profile_bridge::push_signal(sender,event);}
extern "C++" int runtime_threaded_profile_signal_observe_host_api(threaded_signal_sender const&sender,threaded_signal_report&out) noexcept
{return runtime_threaded_profile_bridge::observe_signal(sender,out);}
extern "C++" int runtime_threaded_profile_signal_status_host_api(threaded_signal_report&out) noexcept
{return runtime_threaded_profile_bridge::signal_status(out);}
