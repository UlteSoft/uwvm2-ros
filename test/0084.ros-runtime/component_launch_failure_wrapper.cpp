/** @file
 * @brief Test-only inherited kernel refusal for actual VM launch-failure cases.
 * @details No syscall or process implementation is mocked. The selected filter
 * denies pidfd_open and/or the eight-byte authorization send, then execs the
 * recorded compile-time VM. Broker-inserted debugger flags are passed unchanged.
 * Every VM/supervisor/driver child inherits it and the test cgroup.
 * The wrapper is never part of the product or an installed driver launcher.
 */
#include <array>
#include <cassert>
#include <cerrno>
#include <cstddef>
#include <linux/audit.h>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <sys/socket.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <unistd.h>

int main(int argc,char** argv)
{
    assert(argc>=2);
    std::array<sock_filter,24> filter{};std::size_t count{};
    auto add=[&](sock_filter value){assert(count<filter.size());filter[count++]=value;};
    add(BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,arch)));
    add(BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,AUDIT_ARCH_X86_64,1,0));
    add(BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_KILL_PROCESS));
    add(BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,nr)));
#if defined(UWVM_TEST_LAUNCH_PIDFD_FAILURE)
    // Debugger authentication independently pins its already-live parent.
    // Preserve that proof; refuse acquisition of subsequently forked workers.
    add(BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,SYS_pidfd_open,0,4));
    add(BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,args)));
    add(BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,static_cast<unsigned>(::getppid()),1,0));
    add(BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ERRNO|ENOSYS));
    add(BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW));
    add(BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,nr)));
#endif
#if defined(UWVM_TEST_LAUNCH_GATE_FAILURE)
    // Match the launch gate's exact length and flags, preserving debugger
    // traffic which uses its independent nonblocking authenticated channel.
    add(BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,SYS_sendto,0,6));
    add(BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,args)+2*8));
    add(BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,8,0,3));
    add(BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,args)+3*8));
    add(BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,MSG_NOSIGNAL,0,1));
    add(BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ERRNO|EIO));
    add(BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,nr)));
#endif
    add(BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW));
    sock_fprog program{static_cast<unsigned short>(count),filter.data()};
    assert(!::prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0));
    assert(!::prctl(PR_SET_SECCOMP,SECCOMP_MODE_FILTER,&program));
    argv[0]=const_cast<char*>(UWVM_TEST_RUNTIME_PATH);
    ::execv(UWVM_TEST_RUNTIME_PATH,argv);
    return 127;
}
