#include <uwvm2/imported/ros/common/audit_io.h>
#include <atomic>
#include <cstdlib>
#include <new>
#include <string>
#include <thread>
#include <vector>
#include <cstring>

namespace a=uwvm2::imported::ros::audit_io;
std::atomic<bool> measured{};
std::atomic<std::size_t> allocations{};
void* operator new(std::size_t n) {
    if(measured.load()) ++allocations;
    if(auto p=std::malloc(n?n:1)) return p;
#if defined(__cpp_exceptions)
    throw std::bad_alloc();
#else
    std::abort();
#endif
}
void* operator new[](std::size_t n) { return ::operator new(n); }
void operator delete(void* p) noexcept { std::free(p); }
void operator delete[](void* p) noexcept { std::free(p); }
void operator delete(void* p,std::size_t) noexcept { std::free(p); }
void operator delete[](void* p,std::size_t) noexcept { std::free(p); }
bool mask_equal(sigset_t const& x,sigset_t const& y) {
    for(int s=1;s<NSIG;++s) if(sigismember(&x,s)!=sigismember(&y,s)) return false;
    return true;
}
int main(int argc,char** argv) {
    if(argc!=2) return 2;
    ::raise(SIGSTOP); // Actual ELF is pinned while the driver collects main proof.
    std::string mode=argv[1],record="{\"event\":\"audit-io-test\",\"error\":13}";
    if(mode=="limit"||mode=="over"||mode=="long") {
        std::size_t size=mode=="limit"?4095u:mode=="over"?4096u:9000u;
        record="{\"value\":\""+std::string(size-12,'x')+"\"}";
        if(record.size()!=size) return 3;
    }
    if(mode=="empty") record.clear();
    if(mode=="invalid") ::close(STDERR_FILENO);
    int flags=::fcntl(STDERR_FILENO,F_GETFL,0),fdflags=::fcntl(STDERR_FILENO,F_GETFD,0);
    sigset_t original{},before{},pending_before{},after{},pending_after{},blocked{};
    ::pthread_sigmask(SIG_SETMASK,nullptr,&original);
    bool initial_pending=mode=="pending";
    if(initial_pending) {
        ::sigemptyset(&blocked);::sigaddset(&blocked,SIGPIPE);
        if(::pthread_sigmask(SIG_BLOCK,&blocked,nullptr)||::raise(SIGPIPE)) return 4;
    }
    ::pthread_sigmask(SIG_SETMASK,nullptr,&before);::sigpending(&pending_before);
    struct sigaction handler_before{},handler_after{};
    ::sigaction(SIGPIPE,nullptr,&handler_before);
    bool ok{};
    if(mode=="guard"||mode=="pending") {
        int fds[2];if(::pipe(fds)) return 5;::close(fds[0]);
        {
            a::details::pipe_signal_guard guard;if(!guard.valid()) return 6;
            if(::write(fds[1],"x",1)!=-1||errno!=EPIPE) return 7;
            guard.broken_pipe();
        }
        ::close(fds[1]);ok=true;
    } else if(mode=="concurrent") {
        std::vector<std::thread> threads;
        for(unsigned t=0;t<4;++t) threads.emplace_back([&]{for(unsigned i=0;i<16;++i) (void)a::write_record(record);});
        for(auto& t:threads) t.join();ok=a::delivered_records()==64&&a::dropped_records()==0;
    } else {
        if(mode=="saturate-delivered") a::delivered.store(UINT64_MAX-1);
        if(mode=="saturate-dropped") a::dropped.store(UINT64_MAX-1);
        errno=1234;measured=true;ok=a::write_record(record);measured=false;
        if(errno!=1234) return 8;
        if(mode=="saturate-delivered"||mode=="saturate-dropped") (void)a::write_record(record);
    }
    ::pthread_sigmask(SIG_SETMASK,nullptr,&after);::sigpending(&pending_after);
    ::sigaction(SIGPIPE,nullptr,&handler_after);
    if(!mask_equal(before,after)||::sigismember(&pending_before,SIGPIPE)!=::sigismember(&pending_after,SIGPIPE)||handler_before.sa_handler!=handler_after.sa_handler) return 9;
    if(flags!=::fcntl(STDERR_FILENO,F_GETFL,0)||fdflags!=::fcntl(STDERR_FILENO,F_GETFD,0)) return 10;
    if(initial_pending) { timespec zero{};(void)::sigtimedwait(&blocked,nullptr,&zero);::pthread_sigmask(SIG_SETMASK,&original,nullptr); }
    ::fast_io::io::println("PASS audit-io mode=",mode," complete=",ok?1:0," delivered=",a::delivered_records()," dropped=",a::dropped_records(),
        " cpp_allocations=",allocations.load()," fd_flags=",flags," fd_descriptor_flags=",fdflags," preserved_signal_mask_pending_handler=1");
}
