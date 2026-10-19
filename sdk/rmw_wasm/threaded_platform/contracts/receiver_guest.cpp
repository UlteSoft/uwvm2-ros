// SPDX-License-Identifier: Apache-2.0
// Actual Guest-only pipe receiver contract. Root owns the only writer and the
// complete finite execution deadline; no data is written to that pipe here.
#include <uwvm_rmw/wasi_signal.hpp>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <csignal>
#include <cstdint>
#include <mutex>
#if !defined(__wasi__)
#error "This is a real WASI Guest contract, not a native model"
#endif
#ifndef UWVM_SIGNAL_READ_FD
#error "Root must supply its actual admitted fd role"
#endif
#ifndef UWVM_SIGNAL_SOURCE_COOKIE
#error "Root must supply the actual immutable Source/channel association"
#endif
#ifndef UWVM_SIGNAL_EXPECTED_ERROR
#define UWVM_SIGNAL_EXPECTED_ERROR 0
#endif
#ifndef UWVM_SIGNAL_EXPECTED_SIGINT
#define UWVM_SIGNAL_EXPECTED_SIGINT 1
#endif
#ifndef UWVM_SIGNAL_EXPECTED_SIGTERM
#define UWVM_SIGNAL_EXPECTED_SIGTERM 0
#endif
#ifndef UWVM_SIGNAL_FRAME_LIMIT
#define UWVM_SIGNAL_FRAME_LIMIT 8
#endif
using namespace uwvm_rmw::wasi_signal;
static std::mutex changed_mutex;static std::condition_variable changed;
static std::atomic<unsigned>int_count{},term_count{},unexpected{};
static void received(int signal){
 std::lock_guard<std::mutex>lock(changed_mutex);
 if(!in_receiver_thread())unexpected.fetch_add(1);
 if(signal==SIGINT)int_count.fetch_add(1);else if(signal==SIGTERM)term_count.fetch_add(1);
 else unexpected.fetch_add(1);
 changed.notify_all();
}
static void failed()noexcept{std::lock_guard<std::mutex>lock(changed_mutex);changed.notify_all();}
int main(){
 if(in_receiver_thread())return 35;
 profile selected{profile_version,UWVM_SIGNAL_READ_FD,UWVM_SIGNAL_SOURCE_COOKIE,
  UWVM_SIGNAL_FRAME_LIMIT,maximum_slice_ns};
 auto code=prepare(selected);if(code)return static_cast<int>(code);
 handler old_int{},old_term{};
 if(exchange_handler(SIGINT,&received,&old_int)||exchange_handler(SIGTERM,&received,&old_term))return 21;
#if defined(UWVM_SIGNAL_STOP_BEFORE_ACTIVATE)
 if(stop_and_join())return 31;
 if(activate(interrupt_mask|terminate_mask,&failed)!=17)return 32;
 diagnostics stopped{};
 if(observe(&stopped)||stopped.thread_created||!stopped.guest_join_completed||
  stopped.state!=phase::retired)return 33;
 return int_count.load()||term_count.load()?34:0;
#endif
 if(activate(interrupt_mask|terminate_mask,&failed))return 22;
 bool observed{};
 {
  std::unique_lock<std::mutex>lock(changed_mutex);
  observed=changed.wait_for(lock,std::chrono::seconds(5),[]{
#if UWVM_SIGNAL_EXPECTED_ERROR
   return terminal_error()!=0;
#else
   return terminal_error()!=0||(int_count.load()==UWVM_SIGNAL_EXPECTED_SIGINT&&
    term_count.load()==UWVM_SIGNAL_EXPECTED_SIGTERM);
#endif
  });
 }
 // Both success and finite failure paths discharge the actual receiver owner.
 auto cleanup=stop_and_join();diagnostics actual{};auto snapshot=observe(&actual);
 auto terminal=terminal_error();handler retired_int{},retired_term{};
 auto restore_int=exchange_handler(SIGINT,old_int,&retired_int);
 auto restore_term=exchange_handler(SIGTERM,old_term,&retired_term);
 if(!observed)return 110;
 if(cleanup||snapshot||restore_int||restore_term)return 23;
 if(!actual.guest_join_completed||actual.active_callbacks||unexpected.load())return 24;
 if(terminal!=UWVM_SIGNAL_EXPECTED_ERROR)return 25;
 if(int_count.load()!=UWVM_SIGNAL_EXPECTED_SIGINT||term_count.load()!=UWVM_SIGNAL_EXPECTED_SIGTERM)return 26;
 if(actual.interrupts!=int_count.load()||actual.terminations!=term_count.load())return 27;
 if(actual.accepted_frames!=int_count.load()+term_count.load())return 28;
 if(UWVM_SIGNAL_EXPECTED_ERROR&&(!actual.failure_notified||actual.state!=phase::poisoned))return 29;
 if(!UWVM_SIGNAL_EXPECTED_ERROR&&actual.state!=phase::retired)return 30;
 return 0;
}
