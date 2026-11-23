// Terminal diagnostics must never wait for a launcher to drain its pipe.
#include <uwvm2/runtime/diagnostic/crash_policy.h>
#include <cstdio>
#if defined(__linux__)
#include <cstring>
#include <sys/wait.h>
namespace crash=::uwvm2::runtime::diagnostic::crash;
static void require(bool valid, char const* message)
{
    if(!valid) { std::fprintf(stderr,"crash diagnostic: %s\n",message); std::_Exit(1); }
}
static void fill(int fd, bool socket=false)
{
    char bytes[4096]{};
    auto const flags=fcntl(fd,F_GETFL);
    require(flags>=0 && fcntl(fd,F_SETFL,flags|O_NONBLOCK)==0,"select fill mode");
    for(;;)
    {
        auto n=socket ? send(fd,bytes,sizeof(bytes),MSG_NOSIGNAL):write(fd,bytes,sizeof(bytes));
        if(n>0)continue;
        require(n<0 && (errno==EAGAIN || errno==EWOULDBLOCK),"fill backpressure");break;
    }
    require(fcntl(fd,F_SETFL,flags)==0,"restore launcher blocking mode");
}
static void unchanged_mask(sigset_t const& expected)
{
    sigset_t actual{};require(sigprocmask(SIG_SETMASK,nullptr,&actual)==0,"read signal mask");
    for(int n=1;n<NSIG;++n)require(sigismember(&actual,n)==sigismember(&expected,n),"preserve signal mask");
}
int main()
{
    alarm(10); // A regression must fail finitely even without a draining peer.
    sigset_t original{};require(sigprocmask(SIG_SETMASK,nullptr,&original)==0,"initial mask");
    int pipefd[2];require(pipe2(pipefd,O_CLOEXEC)==0,"pipe");
    auto flags=fcntl(pipefd[1],F_GETFL);fill(pipefd[1]);
    errno=EDOM;require(!crash::write_descriptor(pipefd[1],"lost",4),"full pipe reports failure");
    require(errno==EDOM && fcntl(pipefd[1],F_GETFL)==flags,"preserve errno and shared pipe flags");
    unchanged_mask(original);
    // Shared descriptions remain unchanged across a real child fault path.
    auto pid=fork();require(pid>=0,"fork");
    if(pid==0)
    {
        require(dup2(pipefd[1],2)==2,"child stderr");
        require(crash::process_policy.seal(crash::action::exit),"seal exit");
        crash::report(crash::category::execution,"nonblocking regression");
        crash::finish_unhandled_fault();
    }
    int status{};require(waitpid(pid,&status,0)==pid && WIFEXITED(status) && WEXITSTATUS(status)==121,"child stable exit without draining");
    require(fcntl(pipefd[1],F_GETFL)==flags,"parent descriptor flags untouched");
    char buffer[16384];auto const read_flags=fcntl(pipefd[0],F_GETFL);
    require(read_flags>=0 && fcntl(pipefd[0],F_SETFL,read_flags|O_NONBLOCK)==0,"select drain mode");
    while(read(pipefd[0],buffer,sizeof(buffer))>0){}
    require(errno==EAGAIN && fcntl(pipefd[0],F_SETFL,read_flags)==0,"drained complete pipe");
    require(crash::write_descriptor(pipefd[1],"OK",2),"available blocking pipe writes");
    require(read(pipefd[0],buffer,sizeof(buffer))==2 && buffer[0]=='O' && buffer[1]=='K',"pipe bytes");
    close(pipefd[0]);
    require(!crash::write_descriptor(pipefd[1],"broken",6),"closed peer fails without SIGPIPE death");
    unchanged_mask(original);
    sigset_t blocked=original;require(sigaddset(&blocked,SIGPIPE)==0 && sigprocmask(SIG_SETMASK,&blocked,nullptr)==0,"block pending signal");
    require(raise(SIGPIPE)==0,"preexisting SIGPIPE");
    require(!crash::write_descriptor(pipefd[1],"broken",6),"preexisting pending path");
    sigset_t pending{};require(sigpending(&pending)==0 && sigismember(&pending,SIGPIPE)==1,"preserve preexisting SIGPIPE");
    unchanged_mask(blocked);
    sigset_t consume{};sigemptyset(&consume);sigaddset(&consume,SIGPIPE);timespec immediate{};
    require(sigtimedwait(&consume,nullptr,&immediate)==SIGPIPE,"consume test signal");
    require(sigprocmask(SIG_SETMASK,&original,nullptr)==0,"restore original mask");close(pipefd[1]);
    int sockets[2];require(socketpair(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC,0,sockets)==0,"sockets");
    flags=fcntl(sockets[0],F_GETFL);fill(sockets[0],true);
    require(!crash::write_descriptor(sockets[0],"lost",4) && fcntl(sockets[0],F_GETFL)==flags,"full socket does not block or change flags");
    close(sockets[1]);require(!crash::write_descriptor(sockets[0],"broken",6),"socket no SIGPIPE");close(sockets[0]);
    int full=open("/dev/full",O_WRONLY|O_CLOEXEC);require(full>=0,"full device");
    errno=EDOM;require(!crash::write_descriptor(full,"lost",4) && errno==EDOM,"device error preserved");close(full);
    char name[]="crash-output-XXXXXX";int file=mkstemp(name);require(file>=0,"regular file");unlink(name);
    require(write(file,"abcd",4)==4 && lseek(file,1,SEEK_SET)==1,"file cursor");
    require(crash::write_descriptor(file,"X",1) && lseek(file,0,SEEK_CUR)==2,"regular-file cursor retained");
    require(pread(file,buffer,4,0)==4 && std::memcmp(buffer,"aXcd",4)==0,"regular file not implicitly appended");close(file);
    errno=EDOM;require(!crash::write_descriptor(-1,"invalid",7) && errno==EDOM,"invalid descriptor");
    unchanged_mask(original);alarm(0);std::puts("crash_diagnostic_nonblocking: PASS");
}
#else
int main(){return 77;}
#endif
