// Deterministic shared-OFD flag race at the actual diagnostic write call.
// Only the write call token in crash_policy.h is intercepted; no descriptor,
// fcntl/open/query result, production control flow or syscall is simulated.
#include <fast_io.h>
#include <atomic>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#if defined(__linux__)
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <cerrno>
#include <poll.h>
#include <sys/socket.h>
static int shared_output{-1};
static bool inject{},borrowed{},independent_nonblocking{};
static ssize_t diagnostic_test_write(int fd, void const* data, size_t length) noexcept
{
    if(inject)
    {
        inject=false;
        auto const old=::fcntl(shared_output,F_GETFL);
        if(old<0 || ::fcntl(shared_output,F_SETFL,old&~O_NONBLOCK)!=0) std::_Exit(2);
        borrowed=fd==shared_output;
        independent_nonblocking=(::fcntl(fd,F_GETFL)&O_NONBLOCK)!=0;
        // Prevent a broken implementation from hanging the test process. The
        // borrowed flag below still fails this case before accepting any result.
        if(borrowed) { errno=EAGAIN;return -1; }
    }
    return ::write(fd,data,length);
}
#define write diagnostic_test_write
#include <uwvm2/runtime/diagnostic/crash_policy.h>
#undef write
static void require(bool value)
{ if(!value) { std::fputs("shared diagnostic flag race failed\n",stderr);std::_Exit(1); } }
int main()
{
    int pipefd[2];require(::pipe2(pipefd,O_CLOEXEC|O_NONBLOCK)==0);
    char bytes[4096]{};
    while(::write(pipefd[1],bytes,sizeof(bytes))>0) {}
    require(errno==EAGAIN || errno==EWOULDBLOCK);
    shared_output=pipefd[1];inject=true;errno=EDOM;
    require(!::uwvm2::runtime::diagnostic::crash::write_descriptor(shared_output,"fault",5));
    require(!inject && !borrowed && independent_nonblocking && errno==EDOM);
    // The competing actor's change remains in place; the diagnostic must not
    // restore or otherwise overwrite the launcher's shared file flags.
    require((::fcntl(shared_output,F_GETFL)&O_NONBLOCK)==0);
    ::close(pipefd[0]);::close(pipefd[1]);
    std::puts("crash_diagnostic_shared_flags: PASS independent OFD under concurrent flag change");
}
#else
int main(){return 77;}
#endif
