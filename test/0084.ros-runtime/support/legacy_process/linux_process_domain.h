// Test-only historical process owner. Never include or link into the VM product.
/** @file
 * @brief Atomically owned PID-namespace birth before executing a sealed launcher.
 * @details clone3 returns the exact namespace-init pidfd in the parent as part
 * of process creation. No later pidfd_open or launcher-reported numeric child
 * identity supplies the cleanup boundary. A one-use gate precedes every
 * external instruction. Killing and observing this init covers daemonized and
 * nested descendants. The child immediately execs the sealed launcher; no
 * forked guardian retains VM memory throughout the worker lifetime.
 *
 * Missing clone3, namespaces, identity mappings or gate delivery fail closed.
 * There is no weaker fork fallback, implicit resource grant, cgroup migration,
 * per-instance cgroup claim or physical-stop claim. Callers still retain
 * ownership until actual wait/reap and account all admitted image resources.
 */
#pragma once
#if defined(__linux__)
#include <array>
#include <cerrno>
#include <charconv>
#include <climits>
#include <cstdint>
#include <fcntl.h>
#include <linux/sched.h>
#include <poll.h>
#include <signal.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>

namespace uwvm2::imported::ros::scene::linux_process_domain
{
    /** @brief Create a direct, gated namespace init; zero is the child return.
     * @details No shared address space or descriptor table and no native code
     * has executed. The caller owns wait/reap even if later preparation fails.
     * Only async-signal-safe fixed code may run in the child before exec/exit;
     * libc atfork handlers are deliberately not invoked by this raw clone.
     */
    inline pid_t spawn(int& identity,bool& parent_restored) noexcept
    {
#ifdef SYS_clone3
        identity=-1;parent_restored=true;
        sigset_t blocked{},previous{};
        if(::sigfillset(&blocked)||::sigprocmask(SIG_SETMASK,&blocked,&previous))return -1;
        clone_args args{};
        args.flags=CLONE_NEWUSER|CLONE_NEWPID|CLONE_PIDFD;
        args.pidfd=reinterpret_cast<std::uintptr_t>(&identity);
        args.exit_signal=SIGCHLD;
        auto child=static_cast<pid_t>(::syscall(SYS_clone3,&args,sizeof(args)));
        auto saved=errno;
        if(child!=0)parent_restored=!::sigprocmask(SIG_SETMASK,&previous,nullptr);
        errno=saved;return child;
#else
        identity=-1;parent_restored=true;errno=ENOSYS;return -1;
#endif
    }

    /** @brief Write one bounded kernel identity mapping; no allocation or paths from guests. */
    inline bool map_file(int directory,char const* name,char const* data,std::size_t size) noexcept
    {
        auto fd=::openat(directory,name,O_WRONLY|O_CLOEXEC|O_NOFOLLOW);
        if(fd<0)return false;
        ssize_t written;
        do{written=::write(fd,data,size);}while(written<0&&errno==EINTR);
        auto saved=errno;::close(fd);errno=saved;
        return written==static_cast<ssize_t>(size);
    }

    /** @brief Map exactly the preparing effective UID/GID, before launch authorization.
     * @details The birth pidfd must remain live across opening the proc directory.
     * Its inode then pins the exact task for all mapping writes, even if the
     * child exits or is automatically reaped before a write. The
     * new namespace has no supplementary-group or host-root authority. Failure
     * after any write still requires killing/reaping the owned init; mappings
     * are never reused for another incarnation.
     */
    inline bool map_identity(pid_t child,int identity,uid_t uid,gid_t gid) noexcept
    {
        if(child<=0||identity<0)return false;
        pollfd live{identity,POLLIN,0};if(::poll(&live,1,0)!=0)return false;
        std::array<char,64> path{};
        constexpr char prefix[]{"/proc/"};
        for(std::size_t i{};i<sizeof(prefix)-1;++i)path[i]=prefix[i];
        auto end=std::to_chars(path.data()+sizeof(prefix)-1,path.data()+path.size()-1,child);
        if(end.ec!=std::errc{})return false;
        auto directory=::open(path.data(),O_PATH|O_CLOEXEC|O_DIRECTORY|O_NOFOLLOW);
        if(directory<0)return false;
        // Pin procfs while the birth pidfd is still live. If SIGCHLD is ignored
        // or another observer reaps a rapidly failed child, a recycled numeric
        // PID cannot redirect identity-map writes to an unrelated process.
        live.revents=0;
        if(::poll(&live,1,0)!=0){::close(directory);return false;}
        auto one=[&](char const* name,std::uint64_t id) noexcept
        {
            std::array<char,64> mapping{};
            auto first=std::to_chars(mapping.data(),mapping.data()+24,id);if(first.ec!=std::errc{})return false;
            *first.ptr++=' ';
            auto value=std::to_chars(first.ptr,mapping.data()+mapping.size()-4,id);
            if(value.ec!=std::errc{})return false;
            *value.ptr++=' ';*value.ptr++='1';*value.ptr++='\n';
            return map_file(directory,name,mapping.data(),static_cast<std::size_t>(value.ptr-mapping.data()));
        };
        bool success=map_file(directory,"setgroups","deny\n",5)&&one("uid_map",uid)&&one("gid_map",gid);
        ::close(directory);return success;
    }

    /** @brief Reset inherited VM signal handlers and bind namespace init to its owner.
     * @details This runs only in the fixed raw-clone child before an immediate
     * sealed-image exec. It allocates no guardian and retains no long-lived VM
     * address space. The pinned owner closes the pre-PDEATHSIG death window;
     * SIGKILL from the parent namespace remains effective against PID 1.
     */
    inline bool enter(int parent_identity) noexcept
    {
        struct sigaction action{};action.sa_handler=SIG_DFL;
        if(::sigemptyset(&action.sa_mask))return false;
        for(int signal=1;signal<NSIG;++signal)
            if(signal!=SIGKILL&&signal!=SIGSTOP&&::sigaction(signal,&action,nullptr)&&errno!=EINVAL)return false;
        if(::getpid()!=1||::getppid()!=0||parent_identity<0||::syscall(SYS_prctl,PR_SET_PDEATHSIG,SIGKILL,0,0,0))return false;
        pollfd parent{parent_identity,POLLIN,0};
        if(::poll(&parent,1,0)!=0)return false;
        return !::sigprocmask(SIG_SETMASK,&action.sa_mask,nullptr);
    }
}
#endif
