// SPDX-License-Identifier: Apache-2.0
// Copyright 2026 UWVM contributors.
// Real original public rclcpp APIs and real callback threads. The enclosing
// Root bootstrap supplies actual A/B/C+signal preparation before calling this
// original entry; no mock SignalHandler, Context or pthread is used here.
#include <platform/ros2/cpp/uwvm_rmw/wasi_signal.hpp>
#include <rclcpp/rclcpp.hpp>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <csignal>
#include <mutex>
namespace {
std::mutex facts_mutex;std::condition_variable facts_changed;
std::atomic<unsigned>receiver_checks{},deferred_checks{},failures{};
bool receiver_done{},deferred_done{};
void actual_worker_reentry(bool receiver){
 if(uwvm_rmw::wasi_signal::in_receiver_thread()!=receiver)++failures;
 bool install_refused=false,uninstall_refused=false;
 try{(void)rclcpp::install_signal_handlers(rclcpp::SignalHandlerOptions::All);}
 catch(std::runtime_error const&){install_refused=true;}
 catch(...){++failures;}
 try{(void)rclcpp::uninstall_signal_handlers();}
 catch(std::runtime_error const&){uninstall_refused=true;}
 catch(...){++failures;}
 if(!install_refused||!uninstall_refused)++failures;
 // In particular, the old deferred self-uninstall could mutate installed=false
 // before failing its own join. This real callback must leave that state intact.
 if(!rclcpp::signal_handlers_installed()||
  rclcpp::get_current_signal_handler_options()!=rclcpp::SignalHandlerOptions::All)++failures;
 if(receiver)receiver_checks.fetch_add(2);else deferred_checks.fetch_add(2);
}
void old_interrupt(int signal){
 if(signal!=SIGINT)++failures;
 actual_worker_reentry(true);
 {std::lock_guard<std::mutex>lock(facts_mutex);receiver_done=true;}
 facts_changed.notify_all();
}
}
int uwvm_original_worker_reentry_entry(int argc,char**argv){
 using namespace uwvm_rmw::wasi_signal;
 if(in_receiver_thread())return 41;
 handler previous{};
 if(exchange_handler(SIGINT,&old_interrupt,&previous))return 42;
 rclcpp::init(argc,argv); // actual global Context and default All handler install
 rclcpp::on_shutdown([]{
  actual_worker_reentry(false); // actual original deferred signal worker
  {std::lock_guard<std::mutex>lock(facts_mutex);deferred_done=true;}
  facts_changed.notify_all();
 });
 // Root must separately authorize this genuine logger name and observe the
 // real Host log record before writing one URS1 INT; absence is a real failure.
 RCLCPP_INFO(rclcpp::get_logger("uwvm.signal_reentry"), "UWVM_SIGNAL_REENTRY_READY");
 // Root must not bypass actual pipe delivery by directly invoking handlers.
 bool completed{};
 {std::unique_lock<std::mutex>lock(facts_mutex);
  completed=facts_changed.wait_for(lock,std::chrono::seconds(5),[]{return receiver_done&&deferred_done;});}
 (void)rclcpp::shutdown(); // actual uninstall/join only after callbacks returned
 diagnostics actual{};
 if(observe(&actual)||terminal_error())return 43;
 if(!completed||failures.load()||receiver_checks.load()!=2||deferred_checks.load()!=2)return 44;
 if(!actual.thread_created||!actual.guest_join_completed||actual.active_callbacks)return 45;
 if(rclcpp::signal_handlers_installed()||
  rclcpp::get_current_signal_handler_options()!=rclcpp::SignalHandlerOptions::None)return 46;
 return 0;
}
