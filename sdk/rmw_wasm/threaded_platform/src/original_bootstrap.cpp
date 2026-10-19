// SPDX-License-Identifier: Apache-2.0
// Copyright 2026 UWVM contributors.
#include <uwvm_rmw/wasi_signal_bootstrap.hpp>
#include <rclcpp/utilities.hpp>
namespace uwvm_rmw::wasi_signal {
int run_original(profile const&p,original_application application,int argc,char**argv,
 bootstrap_report&report) noexcept {
 bootstrap_report result{};
 if(!application){result.preparation=22;report=result;return 22;}
 result.preparation=prepare(p);
 if(result.preparation){result.observation=observe(&result.channel);report=result;return static_cast<int>(result.preparation);}
 result.application_entered=true;
 try{result.application_result=application(argc,argv);}catch(...){result.application_threw=true;}
 // Always join the original deferred worker via its genuine public API first.
 // Its implementation joins the channel receiver before semaphore retirement.
 result.uninstall_attempted=true;
 try{(void)rclcpp::uninstall_signal_handlers();}catch(...){result.uninstall_threw=true;}
 // Also handles an entry that never reached original init. This is idempotent
 // only after a successful actual join, never a retry of a failed join.
 result.receiver_join=stop_and_join();
 result.terminal=terminal_error();result.observation=observe(&result.channel);
 report=result;
 if(result.application_result)return result.application_result;
 if(result.application_threw||result.uninstall_threw)return 5;
 if(result.receiver_join)return static_cast<int>(result.receiver_join);
 if(result.terminal)return static_cast<int>(result.terminal);
 if(result.observation)return static_cast<int>(result.observation);
 return 0;
}
}
