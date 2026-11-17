/** @file
 * @brief Independent communication-worker syscall fences and exact FD directions.
 * @details Startup may load only the sealed namespace image closure and create
 * same-process pthreads. Before any broker traffic, confined is installed with
 * TSYNC across every thread. It denies new code, files, threads and networking.
 * Active adds the verified descriptor directions after four broker pipes arrive.
 * Filters are monotonic: no stage restores a syscall denied by an earlier stage.
 * This is a native worker policy, never a guest API or a model-worker exception.
 */
#pragma once
#include "communication_worker_backend.h"
#include <cerrno>
#include <cstddef>
#include <cstdint>
#if defined(__linux__)
#include <fcntl.h>
#include <linux/audit.h>
#include <linux/filter.h>
#include <linux/sched.h>
#include <linux/seccomp.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <unistd.h>
#endif
namespace uwvm2::imported::ros::scene::communication_sandbox
{
#if defined(__linux__)
    enum class syscall_phase : std::uint8_t { startup, confined, active };
    class syscall_filter
    {
        std::array<sock_filter,2048> code{};std::size_t count{};bool invalid{};
        static constexpr std::uint32_t deny=SECCOMP_RET_ERRNO|ENOSYS,allow=SECCOMP_RET_ALLOW;
        void emit(unsigned short op,unsigned char yes,unsigned char no,std::uint32_t value) noexcept
        {if(count==code.size()){invalid=true;return;}code[count++]={op,yes,no,value};}
        void statement(unsigned short op,std::uint32_t value)noexcept{emit(op,0,0,value);}
        void load(unsigned offset)noexcept{statement(BPF_LD|BPF_W|BPF_ABS,offset);}
        void arg(unsigned number,bool high=false)noexcept{load(offsetof(seccomp_data,args)+number*8+(high?4:0));}
        void plain(unsigned number)noexcept{emit(BPF_JMP|BPF_JEQ|BPF_K,0,1,number);statement(BPF_RET|BPF_K,allow);}
        void reject(unsigned number)noexcept{emit(BPF_JMP|BPF_JEQ|BPF_K,0,1,number);statement(BPF_RET|BPF_K,deny);}
        void no_flags(unsigned number,unsigned argument,std::uint32_t forbidden)noexcept
        {
            emit(BPF_JMP|BPF_JEQ|BPF_K,0,4,number);arg(argument);
            emit(BPF_JMP|BPF_JSET|BPF_K,0,1,forbidden);statement(BPF_RET|BPF_K,deny);statement(BPF_RET|BPF_K,allow);
        }
        void descriptor_call(unsigned number,std::span<int const> descriptors)noexcept
        {
            auto length=5+2*descriptors.size();if(length>255){invalid=true;return;}
            emit(BPF_JMP|BPF_JEQ|BPF_K,0,static_cast<unsigned char>(length),number);
            arg(0,true);emit(BPF_JMP|BPF_JEQ|BPF_K,1,0,0);statement(BPF_RET|BPF_K,deny);arg(0);
            for(auto fd:descriptors){emit(BPF_JMP|BPF_JEQ|BPF_K,0,1,static_cast<unsigned>(fd));statement(BPF_RET|BPF_K,allow);}
            statement(BPF_RET|BPF_K,deny);
        }
    public:
        /** @brief Build a closed phase policy; invalid role lists produce no installable program. */
        explicit syscall_filter(syscall_phase phase,std::span<communication_worker::role_fd const> pipes={},
                                std::span<communication_worker::internal_fd const> notifications={})noexcept
        {
            using namespace communication_worker;
            if(phase>syscall_phase::active||notifications.size()>32||(phase==syscall_phase::active&&pipes.size()!=4))
            {invalid=true;return;}
            load(offsetof(seccomp_data,arch));
#if defined(__x86_64__)
            constexpr unsigned arch=AUDIT_ARCH_X86_64;
#elif defined(__aarch64__)
            constexpr unsigned arch=AUDIT_ARCH_AARCH64;
#elif defined(__i386__)
            constexpr unsigned arch=AUDIT_ARCH_I386;
#elif defined(__arm__)
            constexpr unsigned arch=AUDIT_ARCH_ARM;
#else
            constexpr unsigned arch=0;invalid=true;
#endif
            emit(BPF_JMP|BPF_JEQ|BPF_K,1,0,arch);statement(BPF_RET|BPF_K,SECCOMP_RET_KILL_PROCESS);load(offsetof(seccomp_data,nr));
#if defined(__x86_64__)
            emit(BPF_JMP|BPF_JSET|BPF_K,0,1,0x40000000u);statement(BPF_RET|BPF_K,SECCOMP_RET_KILL_PROCESS);
#endif
            if(phase==syscall_phase::active)
            {
                std::array<int,40> reads{},writes{},polls{};std::size_t rn=1,wn=2,pn{};reads[0]=0;writes[0]=1;writes[1]=2;
                std::array<int,36> seen{};std::size_t used{};
                auto unique=[&](int fd){if(fd<4){invalid=true;return false;}for(std::size_t i{};i<used;++i)if(seen[i]==fd){invalid=true;return false;}seen[used++]=fd;return true;};
                for(std::size_t i{};i<pipes.size();++i)
                {auto p=pipes[i];if(i>=roles.size()||p.role!=roles[i]||!unique(p.descriptor)){invalid=true;continue;}
                    if(receiving(p.role))reads[rn++]=p.descriptor;else writes[wn++]=p.descriptor;}
                for(auto n:notifications)
                {
                    if(!unique(n.descriptor)||n.direction<io_direction::read||n.direction>io_direction::both||n.kind>notification_kind::pipe){invalid=true;continue;}
                    if(n.kind==notification_kind::poll){if(n.direction!=io_direction::both){invalid=true;continue;}polls[pn++]=n.descriptor;continue;}
                    if((static_cast<unsigned>(n.direction)&1u)!=0)reads[rn++]=n.descriptor;
                    if((static_cast<unsigned>(n.direction)&2u)!=0)writes[wn++]=n.descriptor;
                }
                descriptor_call(SYS_read,{reads.data(),rn});descriptor_call(SYS_readv,{reads.data(),rn});
                descriptor_call(SYS_write,{writes.data(),wn});descriptor_call(SYS_writev,{writes.data(),wn});
#ifdef SYS_epoll_wait
                descriptor_call(SYS_epoll_wait,{polls.data(),pn});
#endif
#ifdef SYS_epoll_pwait
                descriptor_call(SYS_epoll_pwait,{polls.data(),pn});
#endif
#ifdef SYS_epoll_pwait2
                descriptor_call(SYS_epoll_pwait2,{polls.data(),pn});
#endif
                descriptor_call(SYS_epoll_ctl,{polls.data(),pn});
                reject(SYS_recvmsg);reject(SYS_pread64);reject(SYS_pwrite64);
            }
            else
            {
                plain(SYS_read);plain(SYS_write);plain(SYS_readv);plain(SYS_writev);plain(SYS_pread64);plain(SYS_pwrite64);
                constexpr std::array receiver{3};descriptor_call(SYS_recvmsg,receiver);
                plain(SYS_epoll_ctl);
#ifdef SYS_epoll_wait
                plain(SYS_epoll_wait);
#endif
#ifdef SYS_epoll_pwait
                plain(SYS_epoll_pwait);
#endif
#ifdef SYS_epoll_pwait2
                plain(SYS_epoll_pwait2);
#endif
            }
            if(phase==syscall_phase::confined)
            {
                // Only already-open private /proc descriptors are used to verify
                // the now-stable FD/thread closure before the final direction fence.
                plain(SYS_getdents64);plain(SYS_readlinkat);
#ifdef SYS_readlink
                plain(SYS_readlink);
#endif
            }
            if(phase!=syscall_phase::startup)
            {
                // Moving an existing executable mapping while /proc/self/maps
                // is inspected could hide it behind the reader's cursor.
                reject(SYS_mremap);
                reject(SYS_execve);
#ifdef SYS_execveat
                reject(SYS_execveat);
#endif
                reject(SYS_clone);
#ifdef SYS_clone3
                reject(SYS_clone3);
#endif
                reject(SYS_openat);
#ifdef SYS_open
                reject(SYS_open);
#endif
#ifdef SYS_mmap
# if defined(__i386__) || defined(__arm__)
                // Legacy mmap takes a pointer argument; BPF cannot inspect its protection field.
                reject(SYS_mmap);
# else
                no_flags(SYS_mmap,2,4);
# endif
#endif
#ifdef SYS_mmap2
                no_flags(SYS_mmap2,2,4);
#endif
                no_flags(SYS_mprotect,2,4);
                if(phase==syscall_phase::active){reject(SYS_seccomp);reject(SYS_prctl);}
            }
            else
            {
                plain(SYS_mremap);
#ifdef SYS_fstatfs
                plain(SYS_fstatfs);
#endif
#ifdef SYS_fstatfs64
                plain(SYS_fstatfs64);
#endif
#ifdef SYS_personality
                // Query only. READ_IMPLIES_EXEC would invalidate protection-bit checks.
                emit(BPF_JMP|BPF_JEQ|BPF_K,0,7,SYS_personality);arg(0,true);emit(BPF_JMP|BPF_JEQ|BPF_K,1,0,0);statement(BPF_RET|BPF_K,deny);
                arg(0);emit(BPF_JMP|BPF_JEQ|BPF_K,0,1,0xffffffffu);statement(BPF_RET|BPF_K,allow);statement(BPF_RET|BPF_K,deny);
#endif
#ifdef SYS_open
                no_flags(SYS_open,1,O_CREAT|020000000u);
#endif
                no_flags(SYS_openat,2,O_CREAT|020000000u);
                // clone3 has pointer arguments. ENOSYS permits libc's inspectable clone fallback.
#ifdef SYS_clone3
                reject(SYS_clone3);
#endif
                constexpr unsigned required=CLONE_VM|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD;
                constexpr unsigned permitted=required|CLONE_FS|CLONE_SYSVSEM|CLONE_SETTLS|CLONE_PARENT_SETTID|CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID;
                emit(BPF_JMP|BPF_JEQ|BPF_K,0,10,SYS_clone);arg(0,true);emit(BPF_JMP|BPF_JEQ|BPF_K,1,0,0);statement(BPF_RET|BPF_K,deny);
                arg(0);emit(BPF_JMP|BPF_JSET|BPF_K,0,1,~permitted);statement(BPF_RET|BPF_K,deny);
                statement(BPF_ALU|BPF_AND|BPF_K,required);emit(BPF_JMP|BPF_JEQ|BPF_K,1,0,required);statement(BPF_RET|BPF_K,deny);statement(BPF_RET|BPF_K,allow);
                plain(SYS_execve);plain(SYS_getdents64);plain(SYS_readlinkat);
#ifdef SYS_readlink
                plain(SYS_readlink);
#endif
#ifdef SYS_epoll_create1
                plain(SYS_epoll_create1);
#endif
#ifdef SYS_eventfd2
                plain(SYS_eventfd2);
#endif
#ifdef SYS_pipe2
                plain(SYS_pipe2);
#endif
            }
            if(phase!=syscall_phase::active)
            {
                // Installation may only add no_new_privs and all-thread seccomp fences.
                emit(BPF_JMP|BPF_JEQ|BPF_K,0,4,SYS_prctl);arg(0);emit(BPF_JMP|BPF_JEQ|BPF_K,0,1,PR_SET_NO_NEW_PRIVS);statement(BPF_RET|BPF_K,allow);statement(BPF_RET|BPF_K,deny);
                emit(BPF_JMP|BPF_JEQ|BPF_K,0,7,SYS_seccomp);arg(0);emit(BPF_JMP|BPF_JEQ|BPF_K,1,0,SECCOMP_SET_MODE_FILTER);statement(BPF_RET|BPF_K,deny);
                arg(1);emit(BPF_JMP|BPF_JEQ|BPF_K,0,1,SECCOMP_FILTER_FLAG_TSYNC);statement(BPF_RET|BPF_K,allow);statement(BPF_RET|BPF_K,deny);
            }
            // All phases have no socket/connect/bind/sendto/sendmsg/io_uring/ptrace path.
#ifdef SYS_prlimit64
            // Resource-limit reads are harmless; no active worker may raise a hard limit.
            emit(BPF_JMP|BPF_JEQ|BPF_K,0,6,SYS_prlimit64);arg(2);emit(BPF_JMP|BPF_JEQ|BPF_K,0,2,0);
            arg(2,true);emit(BPF_JMP|BPF_JEQ|BPF_K,1,0,0);statement(BPF_RET|BPF_K,deny);statement(BPF_RET|BPF_K,allow);
#endif
            emit(BPF_JMP|BPF_JEQ|BPF_K,0,6,SYS_fcntl);arg(1);emit(BPF_JMP|BPF_JEQ|BPF_K,3,0,F_GETFL);emit(BPF_JMP|BPF_JEQ|BPF_K,2,0,F_GETFD);
            emit(BPF_JMP|BPF_JEQ|BPF_K,1,0,F_GET_SEALS);statement(BPF_RET|BPF_K,deny);statement(BPF_RET|BPF_K,allow);
#define UWVM_COMMUNICATION_SYSCALL(name) plain(SYS_##name)
            UWVM_COMMUNICATION_SYSCALL(close);UWVM_COMMUNICATION_SYSCALL(fstat);UWVM_COMMUNICATION_SYSCALL(lseek);
            UWVM_COMMUNICATION_SYSCALL(mprotect);UWVM_COMMUNICATION_SYSCALL(munmap);
            UWVM_COMMUNICATION_SYSCALL(brk);UWVM_COMMUNICATION_SYSCALL(madvise);UWVM_COMMUNICATION_SYSCALL(futex);
            UWVM_COMMUNICATION_SYSCALL(rt_sigaction);UWVM_COMMUNICATION_SYSCALL(rt_sigprocmask);UWVM_COMMUNICATION_SYSCALL(rt_sigreturn);
            UWVM_COMMUNICATION_SYSCALL(sigaltstack);UWVM_COMMUNICATION_SYSCALL(clock_gettime);UWVM_COMMUNICATION_SYSCALL(nanosleep);
            UWVM_COMMUNICATION_SYSCALL(getpid);UWVM_COMMUNICATION_SYSCALL(gettid);UWVM_COMMUNICATION_SYSCALL(getuid);
            UWVM_COMMUNICATION_SYSCALL(geteuid);UWVM_COMMUNICATION_SYSCALL(getgid);UWVM_COMMUNICATION_SYSCALL(getegid);
            UWVM_COMMUNICATION_SYSCALL(sched_getaffinity);UWVM_COMMUNICATION_SYSCALL(sched_yield);UWVM_COMMUNICATION_SYSCALL(uname);
            UWVM_COMMUNICATION_SYSCALL(getrandom);UWVM_COMMUNICATION_SYSCALL(set_tid_address);UWVM_COMMUNICATION_SYSCALL(set_robust_list);
            UWVM_COMMUNICATION_SYSCALL(exit);UWVM_COMMUNICATION_SYSCALL(exit_group);
#ifdef SYS_mmap
            UWVM_COMMUNICATION_SYSCALL(mmap);
#endif
#ifdef SYS_mmap2
            UWVM_COMMUNICATION_SYSCALL(mmap2);
#endif
#ifdef SYS_arch_prctl
            UWVM_COMMUNICATION_SYSCALL(arch_prctl);
#endif
#ifdef SYS_rseq
            UWVM_COMMUNICATION_SYSCALL(rseq);
#endif
#ifdef SYS_newfstatat
            UWVM_COMMUNICATION_SYSCALL(newfstatat);
#endif
#ifdef SYS_statx
            UWVM_COMMUNICATION_SYSCALL(statx);
#endif
#ifdef SYS_access
            UWVM_COMMUNICATION_SYSCALL(access);
#endif
#ifdef SYS_faccessat
            UWVM_COMMUNICATION_SYSCALL(faccessat);
#endif
#ifdef SYS_clock_nanosleep
            UWVM_COMMUNICATION_SYSCALL(clock_nanosleep);
#endif
#ifdef SYS_getrlimit
            UWVM_COMMUNICATION_SYSCALL(getrlimit);
#endif
#ifdef SYS_poll
            UWVM_COMMUNICATION_SYSCALL(poll);
#endif
#ifdef SYS_ppoll
            UWVM_COMMUNICATION_SYSCALL(ppoll);
#endif
#undef UWVM_COMMUNICATION_SYSCALL
            statement(BPF_RET|BPF_K,deny);
        }
        std::span<sock_filter const> view()const noexcept{return invalid?std::span<sock_filter const>{}:std::span<sock_filter const>{code.data(),count};}
        /** @brief Install across the entire thread group; a partial TSYNC failure is fatal to activation. */
        bool install()const noexcept
        {
            if(invalid||!count)return false;
            sock_fprog p{static_cast<unsigned short>(count),const_cast<sock_filter*>(code.data())};
            return !::prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0)&&::syscall(SYS_seccomp,SECCOMP_SET_MODE_FILTER,SECCOMP_FILTER_FLAG_TSYNC,&p)==0;
        }
    };
#endif
}
