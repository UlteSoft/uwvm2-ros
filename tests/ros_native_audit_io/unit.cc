#include "server.h"
#include <atomic>
#include <cstdlib>
#include <new>
#include <string>
#include <thread>
#include <vector>
namespace n=uwvm2::ros_gateway::native_io;
namespace g=uwvm2::ros_gateway;
std::atomic<bool> measured{};
std::atomic<std::size_t> allocations{};
void* operator new(std::size_t size) { if(measured.load()) ++allocations;if(auto p=std::malloc(size?size:1)) return p;
#if defined(__cpp_exceptions)
throw std::bad_alloc{};
#else
std::abort();
#endif
}
void* operator new[](std::size_t size) { return ::operator new(size); }
void operator delete(void* p) noexcept { std::free(p); }
void operator delete[](void* p) noexcept { std::free(p); }
void operator delete(void* p,std::size_t) noexcept { std::free(p); }
void operator delete[](void* p,std::size_t) noexcept { std::free(p); }
bool mask_equal(sigset_t const& a,sigset_t const& b) { for(int s=1;s<NSIG;++s) if(::sigismember(&a,s)!=::sigismember(&b,s)) return false;return true; }
int main(int argc,char** argv) {
    if(argc!=2) return 2;::raise(SIGSTOP);
    std::string mode=argv[1],record="{\"event\":\"audit-io-test\",\"error\":13}\n";
    if(mode=="limit"||mode=="over"||mode=="long") {
        std::size_t size=mode=="limit"?4096u:mode=="over"?4097u:9001u;
        record="{\"value\":\""+std::string(size-13,'x')+"\"}\n";if(record.size()!=size) return 3;
    }
    if(mode=="empty") record="\n";
    if(mode=="invalid") ::close(STDERR_FILENO);
    auto flags=::fcntl(STDERR_FILENO,F_GETFL,0),fdflags=::fcntl(STDERR_FILENO,F_GETFD,0);
    sigset_t original{},before{},pending_before{},after{},pending_after{},blocked{};::pthread_sigmask(SIG_SETMASK,nullptr,&original);
    bool initial_pending=mode=="pending";
    if(initial_pending) { ::sigemptyset(&blocked);::sigaddset(&blocked,SIGPIPE);if(::pthread_sigmask(SIG_BLOCK,&blocked,nullptr)||::raise(SIGPIPE)) return 4; }
    ::pthread_sigmask(SIG_SETMASK,nullptr,&before);::sigpending(&pending_before);struct sigaction handler_before{},handler_after{};::sigaction(SIGPIPE,nullptr,&handler_before);
    bool ok{};std::uint64_t completed_calls{},failed_calls{},queued_records{},suppressed{};
    if(mode=="guard"||mode=="pending") {
        int fds[2];if(::pipe(fds)) return 5;::close(fds[0]);
        { n::audit_details::pipe_signal_guard guard;if(!guard.valid()) return 6;if(::write(fds[1],"x",1)!=-1||errno!=EPIPE) return 7;guard.broken_pipe(); }
        ::close(fds[1]);ok=true;
    } else if(mode=="concurrent") {
        std::atomic<unsigned> completed{},failed{};std::vector<std::thread> threads;
        for(unsigned t=0;t<4;++t) threads.emplace_back([&]{for(unsigned i=0;i<16;++i) { if(n::write_audit_text(record)) ++completed;else ++failed; }});
        for(auto& t:threads) t.join();completed_calls=completed;failed_calls=failed;ok=completed_calls==64&&!failed_calls;
    } else if(mode=="server"||mode=="quota"||mode=="saturated-suppress") {
        g::configuration cfg;cfg.policy.ros_version=g::ros::version::ros2;cfg.worker_uid=1000;cfg.worker_pid=12345;cfg.audit_records_per_second=100;cfg.audit_max_records=mode=="server"?100:0;cfg.audit_max_bytes=1048576;
        g::ipc::request request{"time.read-steady","clock",{}};g::audit_limiter global,grant;
        if(mode=="saturated-suppress") global.suppressed=UINT64_MAX;
        errno=1234;measured=true;g::audit(77,request,nullptr,13,global,grant,cfg);measured=false;if(errno!=1234) return 8;
        queued_records=global.total_records;suppressed=global.suppressed;ok=queued_records==1&&!suppressed;
    } else {
        errno=1234;measured=true;ok=n::write_audit_text(record);measured=false;if(errno!=1234) return 8;
        completed_calls=ok?1:0;failed_calls=ok?0:1;
    }
    ::pthread_sigmask(SIG_SETMASK,nullptr,&after);::sigpending(&pending_after);::sigaction(SIGPIPE,nullptr,&handler_after);
    if(!mask_equal(before,after)||::sigismember(&pending_before,SIGPIPE)!=::sigismember(&pending_after,SIGPIPE)||handler_before.sa_handler!=handler_after.sa_handler) return 9;
    if(flags!=::fcntl(STDERR_FILENO,F_GETFL,0)||fdflags!=::fcntl(STDERR_FILENO,F_GETFD,0)) return 10;
    if(initial_pending) { timespec zero{};(void)::sigtimedwait(&blocked,nullptr,&zero);::pthread_sigmask(SIG_SETMASK,&original,nullptr); }
    n::fixed_text<512> line;
    if(!line.append("PASS native-audit-io mode=")||!line.append(mode)||!line.append(" complete=")||!line.number(ok?1:0)||!line.append(" completed_calls=")||!line.number(completed_calls)||!line.append(" failed_calls=")||!line.number(failed_calls)||!line.append(" queued_records=")||!line.number(queued_records)||!line.append(" suppressed=")||!line.number(suppressed)||!line.append(" cpp_allocations=")||!line.number(allocations.load())||!line.append(" fd_flags=")||!line.number(flags)||!line.append(" fd_descriptor_flags=")||!line.number(fdflags)||!line.append(" preserved_signal_mask_pending_handler=1\n")) return 11;
    if(!n::write_all(STDOUT_FILENO,line.view())) return 12;
}
