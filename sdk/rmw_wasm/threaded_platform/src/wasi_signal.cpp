// SPDX-License-Identifier: Apache-2.0
// Copyright 2026 UWVM contributors.
// All buffers and the dispatch owner outlive the actual receiver join. This
// finite target port borrows a trusted preinstalled pipe; it opens no resource.
#include <uwvm_rmw/wasi_signal.hpp>
#include "uwvm_rmw_wasi_signal_features.h"
#if UWVM_RMW_WASI_SIGNAL_P1_API
#include <atomic>
#include <exception>
#include <csignal>
#include <cstddef>
#include <cstring>
#include <limits>
#include <mutex>
#include <new>
#include <system_error>
#include <thread>
#include <wasi/api.h>
static_assert(sizeof(void*)==4,"The admitted Runtime profile is wasm32 only");
#endif

namespace uwvm_rmw::wasi_signal {
#if UWVM_RMW_WASI_SIGNAL_P1_API
namespace {
struct state {
 std::mutex mutex;
 profile selected{};
 diagnostics facts{};
 handler callbacks[2]{};
 handler active[2]{};
 failure_notification failure_wake{};
 std::thread receiver;
 std::atomic<bool> stop{},published{};
 std::atomic<std::uint32_t> terminal{};
 std::uint64_t next_userdata{1},next_sequence{1};
 std::uint32_t selected_mask{};
 bool join_attempted{};
 unsigned char frame[frame_bytes]{};
 __wasi_subscription_t subscriptions[2]{};
 __wasi_event_t events[2]{};
 __wasi_size_t event_count{};
};
// A genuine source-lifetime owner, deliberately not an atexit-destructed thread
// object. Runtime Group/Source retirement must join before releasing its memory.
// In particular, a forced terminal trap is not a successful library uninstall.
state& owner() { static state* value=new state;return *value; }
thread_local state const* actual_receiver{};
struct receiver_scope final {
 state const* previous{};
 explicit receiver_scope(state const* value) noexcept:previous{actual_receiver}{actual_receiver=value;}
 ~receiver_scope(){actual_receiver=previous;}
 receiver_scope(receiver_scope const&)=delete;
};
void bump(std::uint64_t&n) noexcept {if(n!=UINT64_MAX)++n;}
void record_locked(state&s,std::uint32_t code) noexcept {
 if(code){if(!s.facts.first_error)s.facts.first_error=code;s.facts.last_error=code;}
}
std::uint32_t wasi_error(__wasi_errno_t code) noexcept {
 if(code==__WASI_ERRNO_SUCCESS)return 0;
 if(code==__WASI_ERRNO_NOTCAPABLE||code==__WASI_ERRNO_ACCES)return 13;
 if(code==__WASI_ERRNO_INVAL)return 22;
 if(code==__WASI_ERRNO_AGAIN)return 11;
 if(code==__WASI_ERRNO_BADF)return 9;
 return 5;
}
std::uint64_t little(unsigned char const*p,unsigned width) noexcept {
 std::uint64_t value{};for(unsigned i{};i<width;++i)value|=std::uint64_t(p[i])<<(8*i);return value;
}
int index_for(int value) noexcept {return value==SIGINT?0:value==SIGTERM?1:-1;}

// Only the unpublished prepare caller or the one real receiver calls poll.
// Concurrent diagnostics/stop touch facts under mutex and the stop atomic only.
std::uint32_t poll(state&s,std::uint64_t duration,bool&fd_ready) {
 if(s.next_userdata>UINT64_MAX-2)return 90;
 std::uint64_t const fd_token=s.next_userdata++,clock_token=s.next_userdata++;
 std::memset(s.subscriptions,0,sizeof s.subscriptions);
 std::memset(s.events,0,sizeof s.events);s.event_count=static_cast<__wasi_size_t>(-1);
 s.subscriptions[0].userdata=fd_token;
 s.subscriptions[0].u.tag=__WASI_EVENTTYPE_FD_READ;
 s.subscriptions[0].u.u.fd_read.file_descriptor=s.selected.read_fd;
 s.subscriptions[1].userdata=clock_token;
 s.subscriptions[1].u.tag=__WASI_EVENTTYPE_CLOCK;
 s.subscriptions[1].u.u.clock.id=__WASI_CLOCKID_MONOTONIC;
 s.subscriptions[1].u.u.clock.timeout=duration;
 s.subscriptions[1].u.u.clock.precision=0;s.subscriptions[1].u.u.clock.flags=0;
 {
  std::lock_guard<std::mutex>lock(s.mutex);
  bump(s.facts.polls);s.facts.last_userdata=clock_token;
 }
 // This is the real Preview1 declaration, not a handwritten import signature.
 __wasi_errno_t const actual=__wasi_poll_oneoff(s.subscriptions,s.events,2,&s.event_count);
 {
  std::lock_guard<std::mutex>lock(s.mutex);s.facts.last_wasi_errno=actual;
 }
 if(actual)return wasi_error(actual);
 if(s.event_count==0||s.event_count>2)return 22;
 bool seen_fd=false,seen_clock=false;
 for(__wasi_size_t i{};i<s.event_count;++i){
  auto const&e=s.events[i];
  if(e.error){std::lock_guard<std::mutex>lock(s.mutex);s.facts.last_wasi_errno=e.error;return wasi_error(e.error);}
  if(e.userdata==fd_token&&e.type==__WASI_EVENTTYPE_FD_READ&&!seen_fd){
   seen_fd=true;
   // A trusted live writer must remain present. HUP is not a graceful signal.
   if(e.fd_readwrite.flags&__WASI_EVENTRWFLAGS_FD_READWRITE_HANGUP)return 5;
   if(e.fd_readwrite.flags!=0)return 22;
  }else if(e.userdata==clock_token&&e.type==__WASI_EVENTTYPE_CLOCK&&!seen_clock){seen_clock=true;}
  else return 22;
 }
 fd_ready=seen_fd;return 0;
}

void terminal_failure(state&s,std::uint32_t code) noexcept {
 failure_notification wake{};
 try{
  std::lock_guard<std::mutex>lock(s.mutex);
  record_locked(s,code);s.facts.state=phase::poisoned;
  std::uint32_t empty{};s.terminal.compare_exchange_strong(empty,code,std::memory_order_release,std::memory_order_relaxed);
  if(!s.facts.failure_notified){s.facts.failure_notified=true;wake=s.failure_wake;}
 }catch(...){std::terminate();}
 // No signal_received flag is set here. The original deferred worker must
 // distinguish this actual platform failure from a valid SIGINT/SIGTERM.
 if(wake)wake();
}

std::uint32_t receive_frame(state&s) {
 unsigned held{};
 {std::lock_guard<std::mutex>lock(s.mutex);held=s.facts.partial_bytes;}
 if(held>=frame_bytes)return 22;
 __wasi_iovec_t vector{};vector.buf=s.frame+held;vector.buf_len=frame_bytes-held;
 __wasi_size_t received=static_cast<__wasi_size_t>(-1);
 {std::lock_guard<std::mutex>lock(s.mutex);bump(s.facts.reads);}
 __wasi_errno_t const actual=__wasi_fd_read(s.selected.read_fd,&vector,1,&received);
 {std::lock_guard<std::mutex>lock(s.mutex);s.facts.last_wasi_errno=actual;}
 if(actual)return wasi_error(actual);
 if(received==0)return 5; // EOF never authorizes a fabricated shutdown signal.
 if(received>frame_bytes-held)return 22;
 held+=received;
 {std::lock_guard<std::mutex>lock(s.mutex);s.facts.partial_bytes=held;}
 if(held<frame_bytes)return 0;
 unsigned char const*p=s.frame;
 if(std::memcmp(p,"URS1",4)!=0||little(p+4,2)!=1||little(p+6,2)!=frame_bytes||
  little(p+8,8)!=s.selected.source_cookie||little(p+16,8)!=s.next_sequence||little(p+28,4)!=0)return 22;
 auto const event=little(p+24,4);
 if(event!=1&&event!=2)return 22;
 auto const index=static_cast<unsigned>(event-1);
 if(!(s.selected_mask&(1u<<index)))return 13;
 handler callback{};
 {
  std::lock_guard<std::mutex>lock(s.mutex);
  if(s.facts.accepted_frames>=s.selected.maximum_frames||s.next_sequence==UINT64_MAX)return 90;
  // A complete valid old frame may be consumed during uninstall, but no
  // callback may begin after stop admission. No new activation reuses it.
  if(s.stop.load(std::memory_order_acquire)||s.facts.state!=phase::active)return 0;
  callback=s.active[index];if(!callback)return 13;
  s.facts.last_sequence=s.next_sequence++;
  ++s.facts.accepted_frames;s.facts.partial_bytes=0;
  if(index==0)++s.facts.interrupts;else ++s.facts.terminations;
  ++s.facts.active_callbacks;
 }
 try{callback(index==0?SIGINT:SIGTERM);}
 catch(...){std::lock_guard<std::mutex>lock(s.mutex);--s.facts.active_callbacks;return 5;}
 {std::lock_guard<std::mutex>lock(s.mutex);--s.facts.active_callbacks;}
 return 0;
}

void run(state*s) noexcept {
 receiver_scope entered{s};
 try{
  // activate holds this same real mutex until the thread owner and publication
  // are installed. The child blocks in genuine pthread synchronization; there
  // is no spin/yield loop or import before ownership publication.
  bool published{};
  {std::lock_guard<std::mutex>lock(s->mutex);published=s->published.load(std::memory_order_acquire);}
  if(!published){terminal_failure(*s,5);return;}
  while(!s->stop.load(std::memory_order_acquire)){
   bool ready=false;auto code=poll(*s,s->selected.poll_slice_ns,ready);
   if(code){terminal_failure(*s,code);return;}
   if(s->stop.load(std::memory_order_acquire))return;
   if(!ready)continue; // A genuine clock event, not a signal or elapsed credit.
   code=receive_frame(*s);
   if(code==11)continue; // Actual nonblocking EAGAIN; no consumed frame replay.
   if(code){terminal_failure(*s,code);return;}
  }
 }catch(...){terminal_failure(*s,5);}
}
}
#endif

std::uint32_t prepare(profile const&p) noexcept {
#if !UWVM_RMW_WASI_SIGNAL_P1_API
 (void)p;return 95;
#else
 try{
  state&s=owner();std::unique_lock<std::mutex>lock(s.mutex);
  if(s.facts.state!=phase::cold)return 17;
  if(p.version!=profile_version||p.read_fd<3||p.read_fd>INT32_MAX||p.source_cookie==0||!p.maximum_frames||
   p.maximum_frames>maximum_frame_count||!p.poll_slice_ns||p.poll_slice_ns>maximum_slice_ns)return 22;
  s.selected=p;s.facts.state=phase::starting;
  lock.unlock();bool ready=false;auto code=poll(s,0,ready);lock.lock();
  if(code){record_locked(s,code);s.facts.state=phase::poisoned;s.terminal.store(code,std::memory_order_release);return code;}
  s.facts.state=phase::prepared;return 0;
 }catch(...){return 5;}
#endif
}
std::uint32_t exchange_handler(int signal,handler desired,handler*previous) noexcept {
 if(!previous)return 22;
#if !UWVM_RMW_WASI_SIGNAL_P1_API
 (void)signal;(void)desired;return 95;
#else
 try{
  state&s=owner();std::lock_guard<std::mutex>lock(s.mutex);int const index=index_for(signal);
  if(index<0)return 22;
  if(s.facts.state!=phase::prepared&&s.facts.state!=phase::retired&&
   !(s.facts.state==phase::poisoned&&s.facts.guest_join_completed))return 17;
  auto const old=s.callbacks[index];s.callbacks[index]=desired;*previous=old;return 0;
 }catch(...){return 5;}
#endif
}
std::uint32_t activate(std::uint32_t mask,failure_notification wake) noexcept {
#if !UWVM_RMW_WASI_SIGNAL_P1_API
 (void)mask;(void)wake;return 95;
#else
 try{
  state&s=owner();std::unique_lock<std::mutex>lock(s.mutex);
  if(s.facts.state!=phase::prepared||s.facts.start_attempted)return 17;
  if(!mask||(mask&~(interrupt_mask|terminate_mask))||!wake)return 22;
  for(unsigned i{};i<2;++i)if((mask&(1u<<i))&&!s.callbacks[i])return 13;
  s.selected_mask=mask;s.active[0]=s.callbacks[0];s.active[1]=s.callbacks[1];s.failure_wake=wake;
  s.facts.start_attempted=true;s.facts.state=phase::starting;
  // The receiver cannot pass publication until its actual thread owner is
  // stored. No raw pipe read or callback runs in the constructor interval.
  try{s.receiver=std::thread(run,&s);}
  catch(std::system_error const&e){s.facts.last_native_error=static_cast<std::uint32_t>(e.code().value());record_locked(s,5);s.facts.state=phase::poisoned;s.facts.guest_join_completed=true;s.terminal.store(5,std::memory_order_release);return 5;}
  catch(...){record_locked(s,12);s.facts.state=phase::poisoned;s.facts.guest_join_completed=true;s.terminal.store(12,std::memory_order_release);return 12;}
  s.facts.thread_created=true;s.facts.state=phase::active;
  s.published.store(true,std::memory_order_release);return 0;
 }catch(...){return 5;}
#endif
}
std::uint32_t stop_and_join() noexcept {
#if !UWVM_RMW_WASI_SIGNAL_P1_API
 return 95;
#else
 try{
  state&s=owner();std::unique_lock<std::mutex>lock(s.mutex);
  if(s.facts.state==phase::cold)return 13;
  if(s.facts.state==phase::starting)return 17;
  if(s.facts.guest_join_completed)return 0;
  if(!s.facts.thread_created){
   // Success also closes the pre-activation domain. It cannot be followed by
   // a later activate after the caller has begun retiring its handler storage.
   s.stop.store(true,std::memory_order_release);s.facts.guest_join_completed=true;
   s.facts.state=s.terminal.load(std::memory_order_acquire)?phase::poisoned:phase::retired;
   return 0; // thread_created=false distinguishes no owner from a real join.
  }
  if(s.receiver.get_id()==std::this_thread::get_id())return 17;
  if(s.facts.state==phase::stopping)return 17;
  if(s.join_attempted)return 5; // Failed physical ownership is never retried.
  s.join_attempted=true;
  s.stop.store(true,std::memory_order_release);s.facts.state=phase::stopping;
  lock.unlock();
  try{s.receiver.join();}
  catch(std::system_error const&e){lock.lock();s.facts.last_native_error=static_cast<std::uint32_t>(e.code().value());record_locked(s,5);s.facts.state=phase::poisoned;s.terminal.store(5,std::memory_order_release);return 5;}
  lock.lock();
  if(s.facts.active_callbacks!=0){record_locked(s,5);s.facts.state=phase::poisoned;s.terminal.store(5,std::memory_order_release);return 5;}
  s.facts.guest_join_completed=true;s.facts.state=s.terminal.load(std::memory_order_acquire)?phase::poisoned:phase::retired;
  return 0;
 }catch(...){return 5;}
#endif
}
bool in_receiver_thread() noexcept {
#if !UWVM_RMW_WASI_SIGNAL_P1_API
 return false;
#else
 return actual_receiver!=nullptr;
#endif
}
std::uint32_t terminal_error() noexcept {
#if !UWVM_RMW_WASI_SIGNAL_P1_API
 return 95;
#else
 try{return owner().terminal.load(std::memory_order_acquire);}catch(...){return 5;}
#endif
}
std::uint32_t observe(diagnostics*out) noexcept {
 if(!out)return 22;
#if !UWVM_RMW_WASI_SIGNAL_P1_API
 return 95;
#else
 try{state&s=owner();std::lock_guard<std::mutex>lock(s.mutex);*out=s.facts;return 0;}catch(...){return 5;}
#endif
}
}
