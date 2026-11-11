#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/imported/wasi/wasip1/environment/resource_limits.h>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
namespace quota=uwvm2::imported::wasi::wasip1::environment;
namespace hc=uwvm2::utils::host_clock;
static void require(bool value,char const* why){if(!value){std::fprintf(stderr,"FAIL: %s\n",why);std::abort();}}
int main(){
#if defined(__linux__)
 quota::quota_native_wait own;require(own.prepare(),"register");
 quota::process_budget_ledger.notify_waiters(false);
 auto start=hc::now();require(own.wait(start+std::chrono::milliseconds(80),-1),"timer wait");
 require(hc::now()-start>=std::chrono::milliseconds(65),"own FD release must not wake its quota waiter");
 // A different thread's real release must remain sticky, including before poll.
 std::thread other([]{quota::process_budget_ledger.notify_waiters(false);});other.join();
 start=hc::now();require(own.wait(start+std::chrono::seconds(2),-1),"peer wake");
 require(hc::now()-start<std::chrono::milliseconds(500),"peer notification lost");
 // Stop/exit notifications must still wake the current thread immediately.
 quota::process_budget_ledger.notify_waiters();
 start=hc::now();require(own.wait(start+std::chrono::seconds(2),-1),"cancellation wake");
 require(hc::now()-start<std::chrono::milliseconds(500),"self cancellation notification lost");
 std::puts("PASS: quota timer blocks after own FD unlock; peer and cancellation wakes retained");
#else
 return 77;
#endif
}
