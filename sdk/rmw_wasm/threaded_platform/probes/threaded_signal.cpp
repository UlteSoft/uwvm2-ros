// SPDX-License-Identifier: Apache-2.0
// Compile AND LINK only. This source must never be run by CMake.
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <csignal>
#include <mutex>
#include <semaphore.h>
#include <stdexcept>
#include <thread>
#include <wasi/api.h>
#if !defined(__wasi__) || !defined(__wasm32__) || !defined(__wasm_atomics__) || !defined(__wasm_exception_handling__)
#error "A genuine wasm32 WASI pthread+Wasm-EH producer is required"
#endif
#if defined(_LIBCPP_HAS_NO_THREADS) || (defined(_LIBCPP_HAS_THREADS) && !_LIBCPP_HAS_THREADS)
#error "The selected libc++ was not built with threading"
#endif
#if !defined(_LIBCPP_VERSION)
#error "This fixed producer contract selects its genuine LLVM libc++"
#endif
static_assert(sizeof(void*)==4);
static_assert(SIGINT!=SIGTERM);
static std::atomic<unsigned>destroyed{};
struct tls_value {~tls_value(){destroyed.fetch_add(1,std::memory_order_relaxed);}};
static thread_local tls_value local;
int main(int argc,char**){
 std::mutex mutex;std::recursive_mutex recursive;std::condition_variable changed;
 std::once_flag once;bool ready=false;sem_t signal{};
 if(sem_init(&signal,0,0)!=0)return 1;
 std::thread actual([&]{
  (void)&local;
  std::call_once(once,[&]{std::lock_guard<std::recursive_mutex>lock(recursive);});
  {std::lock_guard<std::mutex>lock(mutex);ready=true;}
  changed.notify_all();
  if(sem_post(&signal)!=0)std::terminate();
 });
 {std::unique_lock<std::mutex>lock(mutex);changed.wait(lock,[&]{return ready;});}
 if(sem_wait(&signal)!=0)std::terminate();actual.join();
 {std::unique_lock<std::mutex>lock(mutex);
  (void)changed.wait_for(lock,std::chrono::nanoseconds(1));}
 (void)std::chrono::steady_clock::now();
 if(sem_destroy(&signal)!=0)return 2;
 // Keep the genuine declarations/symbols and member layout live at final link.
 // There are no handwritten imports, C structs or weak success replacements.
 auto volatile poll=&__wasi_poll_oneoff;
 auto volatile read=&__wasi_fd_read;
 if(argc==3162217){
  __wasi_subscription_t sub{};__wasi_event_t event{};__wasi_size_t count{};
  sub.u.tag=__WASI_EVENTTYPE_FD_READ;sub.u.u.fd_read.file_descriptor=3;
  __wasi_iovec_t vector{};
  if(poll(&sub,&event,1,&count))return 3;
  if(read(3,&vector,1,&count))return 4;
 }
 try{if(argc>1)throw std::runtime_error("actual Wasm C++ exception link closure");}
 catch(std::exception const&){return destroyed.load(std::memory_order_relaxed)==1?0:5;}
 return destroyed.load(std::memory_order_relaxed)==1?0:6;
}
