// Actual Linux MTE admission on the calling native thread. A blocked SIGSEGV
// must be a setup error in BOTH delivery modes, before any Guest access.
// This test neither unblocks on the implementation's behalf nor accepts a
// fatal signal as a correctly delivered managed memory-protection fault.
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/memtag/bridge.h>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <thread>
#if defined(__linux__) && defined(__aarch64__)
#include <csignal>
#include <pthread.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <unistd.h>
namespace mt=uwvm2::extension::memtag;
namespace hw=mt::hardware_mte;
namespace rt=uwvm2::runtime::memtag;
namespace linear=uwvm2::object::memory::linear;
#define CHECK(...) do { if(!(__VA_ARGS__)) { std::fprintf(stderr,"hardware mask assertion %u\n",__LINE__);std::abort(); } } while(false)
static void set_blocked(sigset_t& original)
{
    sigset_t signals{};CHECK(::sigemptyset(&signals)==0);CHECK(::sigaddset(&signals,SIGSEGV)==0);
    CHECK(::pthread_sigmask(SIG_BLOCK,&signals,&original)==0);
}
template<class Error> static constexpr bool blocked_error(Error error)
{
    // The same fixture can demonstrate admission on a pre-fix header without
    // depending on the newly introduced error-enumerator spelling there.
    if constexpr(requires { Error::thread_fault_signal_blocked; })
    { return error==Error::thread_fault_signal_blocked; }
    else { return false; }
}
static void reject_blocked()
{
    sigset_t before{},after{};struct sigaction handler_before{},handler_after{};
    CHECK(::pthread_sigmask(SIG_BLOCK,nullptr,&before)==0);CHECK(::sigismember(&before,SIGSEGV)==1);
    CHECK(::sigaction(SIGSEGV,nullptr,&handler_before)==0);
    auto const control{::prctl(56,0ul,0ul,0ul,0ul)};CHECK(control>=0);
    auto const direct{hw::enable_current_thread_selected()};
    auto const admitted{rt::prepare_hardware_thread()};
    if(direct || admitted)
    {
        std::fputs("FAIL: an actually blocked native thread was admitted to hardware MTE\n",stdout);
        std::fflush(stdout);::_Exit(42);
    }
    CHECK(blocked_error(direct.error));CHECK(blocked_error(admitted.error));
    CHECK(::pthread_sigmask(SIG_BLOCK,nullptr,&after)==0);CHECK(::sigismember(&after,SIGSEGV)==1);
    CHECK(::prctl(56,0ul,0ul,0ul,0ul)==control);
    CHECK(::sigaction(SIGSEGV,nullptr,&handler_after)==0);
    CHECK(handler_before.sa_sigaction==handler_after.sa_sigaction && handler_before.sa_flags==handler_after.sa_flags);
}
static void test_mode(bool asynchronous)
{
    mt::selected_fault_delivery.store(asynchronous?mt::fault_delivery::hardware_async:mt::fault_delivery::hardware_sync);
    CHECK(hw::enable_current_thread_selected());
    linear::mmap_memory_t memory(65536,linear::mmap_memory_status_t::wasm32);
    memory.init_by_page_count(1,2,19);
    CHECK(memory.enable_memtag(mt::address_width::i32,false)==mt::storage_error::none);
    CHECK(rt::activate_owner_hardware(memory));
    // Main thread: querying admission preserves its signal mask, handler, and
    // existing control register. Explicit caller restoration re-enables entry.
    sigset_t original{};set_blocked(original);reject_blocked();
    CHECK(::pthread_sigmask(SIG_SETMASK,&original,nullptr)==0);CHECK(rt::prepare_hardware_thread());
    // A clean main thread does not authorize a worker that blocks SIGSEGV later.
    std::atomic<bool> blocked{},release{};
    std::thread worker{[&]
    {
        sigset_t saved{};set_blocked(saved);blocked.store(true,std::memory_order_release);blocked.notify_all();
        release.wait(false,std::memory_order_acquire);reject_blocked();
        CHECK(::pthread_sigmask(SIG_SETMASK,&saved,nullptr)==0);CHECK(rt::prepare_hardware_thread());
    }};
    blocked.wait(false,std::memory_order_acquire);CHECK(rt::prepare_hardware_thread());
    release.store(true,std::memory_order_release);release.notify_all();worker.join();
    // A newly created worker inherits its creator's mask. Changing the creator
    // afterwards must not accidentally make this child's entry pass.
    blocked.store(false);release.store(false);set_blocked(original);
    std::thread inherited{[&]
    {
        blocked.store(true,std::memory_order_release);blocked.notify_all();
        release.wait(false,std::memory_order_acquire);reject_blocked();
    }};
    blocked.wait(false,std::memory_order_acquire);CHECK(::pthread_sigmask(SIG_SETMASK,&original,nullptr)==0);
    CHECK(rt::prepare_hardware_thread());release.store(true,std::memory_order_release);release.notify_all();inherited.join();
    auto view=memory.memtag_shadow.view<std::uint32_t,false>(65536);
    CHECK(view.store(view.insert(32,7),16));
    auto* good=reinterpret_cast<volatile unsigned char*>(hw::tagged_pointer(memory.memory_begin+32,7));
    *good=55;CHECK(*good==55);CHECK((::prctl(56,0ul,0ul,0ul,0ul)&6)==(asynchronous?4:2));
}
int main()
{
    if(!hw::supported()) {std::puts("UNSUPPORTED: OS checked MTE mappings unavailable");return 77;}
    for(bool asynchronous:{false,true})
    {
        auto const child{::fork()};CHECK(child>=0);
        if(!child) {test_mode(asynchronous);::_Exit(0);}
        int status{};CHECK(::waitpid(child,&status,0)==child);
        if(!WIFEXITED(status)||WEXITSTATUS(status)!=0) {return WIFEXITED(status)?WEXITSTATUS(status):1;}
    }
    std::puts("PASS: sync+async blocked main, independently blocked worker, and inherited blocked worker rejected; mask/handler/control preserved; restored entry and tagged access succeed");
}
#else
int main(){std::puts("UNSUPPORTED: requires AArch64 Linux MTE");return 77;}
#endif
