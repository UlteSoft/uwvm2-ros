/** @file
 * @brief Deny pointer-form legacy mmap while preserving checked direct mappings.
 * @details Executes the actual filter on native x86_64/i386 child processes.
 * Cross-ABI runs also interpret the exact generated classic BPF with the target
 * kernel syscall constants. QEMU runs do not claim kernel seccomp enforcement.
 */
#include "support/legacy_process/model_sandbox.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <sys/mman.h>
namespace x=uwvm2::imported::ros::scene::model_sandbox;
static std::uint32_t decision(x::syscall_filter const& policy,seccomp_data const& data)
{
    std::uint32_t value{};auto program=policy.view();assert(!program.empty());
    for(std::size_t at{},steps{};at<program.size()&&steps<2048;++at,++steps)
    {
        auto instruction=program[at];switch(instruction.code)
        {
        case BPF_LD|BPF_W|BPF_ABS:assert(instruction.k<=sizeof(data)-4);std::memcpy(&value,reinterpret_cast<unsigned char const*>(&data)+instruction.k,4);break;
        case BPF_JMP|BPF_JEQ|BPF_K:at+=value==instruction.k?instruction.jt:instruction.jf;break;
        case BPF_JMP|BPF_JSET|BPF_K:at+=(value&instruction.k)?instruction.jt:instruction.jf;break;
        case BPF_RET|BPF_K:return instruction.k;
        default:assert(false);
        }
    }
    assert(false);return 0;
}
static constexpr unsigned architecture=
#if defined(__x86_64__)
    AUDIT_ARCH_X86_64;
#elif defined(__i386__)
    AUDIT_ARCH_I386;
#elif defined(__aarch64__)
    AUDIT_ARCH_AARCH64;
#elif defined(__arm__)
    AUDIT_ARCH_ARM;
#else
#error unsupported qualification target
#endif
static void explain_filter()
{
    x::syscall_filter startup,final{true};seccomp_data call{};call.arch=architecture;
    auto permits=[&](long number,unsigned long protection)
    {call.nr=static_cast<int>(number);call.args[2]=protection;return decision(startup,call)==SECCOMP_RET_ALLOW&&decision(final,call)==SECCOMP_RET_ALLOW;};
#if defined(SYS_mmap) && (defined(__i386__) || defined(__arm__))
    call.nr=SYS_mmap;call.args[0]=0x10000; // User pointer is opaque, never dereferenced by BPF.
    for(auto misleading:{0ull,1ull,2ull,3ull,4ull,~0ull}){call.args[2]=misleading;assert(decision(final,call)!=(SECCOMP_RET_ALLOW));}
#endif
#ifdef SYS_mmap2
    assert(permits(SYS_mmap2,PROT_READ|PROT_WRITE));assert(!permits(SYS_mmap2,PROT_READ|PROT_EXEC));
#endif
#if defined(SYS_mmap) && !defined(__i386__) && !defined(__arm__)
    assert(permits(SYS_mmap,PROT_READ|PROT_WRITE));assert(!permits(SYS_mmap,PROT_READ|PROT_EXEC));
#endif
    assert(permits(SYS_mprotect,PROT_READ|PROT_WRITE));assert(!permits(SYS_mprotect,PROT_EXEC));
    for(auto syscall:{SYS_socket,SYS_connect,SYS_clone,SYS_execve})assert(!permits(syscall,0));
    call.arch=architecture^1;call.nr=SYS_read;assert(decision(startup,call)==SECCOMP_RET_KILL_PROCESS);
#if defined(__x86_64__)
    call.arch=architecture;call.nr=SYS_read|0x40000000;assert(decision(startup,call)==SECCOMP_RET_KILL_PROCESS);
#endif
}
static void actual_kernel()
{
    auto child=::fork();assert(child>=0);
    if(!child)
    {
        x::syscall_filter startup,final{true};if(!startup.install()||!final.install())::_exit(10);
#if defined(SYS_mmap) && (defined(__i386__) || defined(__arm__))
        struct legacy_arguments{unsigned long address,length,protection,flags,descriptor,offset;};
        legacy_arguments args{0,4096,PROT_READ|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS,~0ul,0};errno=0;
        auto legacy=::syscall(SYS_mmap,&args,0,0);
        if(legacy!=-1)
        {
            constexpr char message[]="legacy executable mapping admitted\n";
            auto ignored=::write(2,message,sizeof(message)-1);(void)ignored;::_exit(11);
        }
        if(errno!=ENOSYS)::_exit(16);
#endif
#ifdef SYS_mmap2
        constexpr auto mapping=SYS_mmap2;
#else
        constexpr auto mapping=SYS_mmap;
#endif
        auto mapped=::syscall(mapping,0,4096,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
        if(mapped==-1)::_exit(12);
        errno=0;if(::syscall(mapping,0,4096,PROT_READ|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS,-1,0)!=-1||errno!=ENOSYS)::_exit(13);
        errno=0;if(::syscall(SYS_mprotect,mapped,4096,PROT_READ|PROT_EXEC)!=-1||errno!=ENOSYS)::_exit(14);
        if(::syscall(SYS_munmap,mapped,4096))::_exit(15);
        ::_exit(0);
    }
    int status{};assert(::waitpid(child,&status,0)==child);if(!WIFEXITED(status)||WEXITSTATUS(status))std::fprintf(stderr,"kernel child status=%d\n",status);
    assert(WIFEXITED(status)&&WEXITSTATUS(status)==0);
}
int main(int argc,char** argv)
{
    assert(argc==1||(argc==2&&(!std::strcmp(argv[1],"--interpret-only")||!std::strcmp(argv[1],"--kernel-only"))));
    bool kernel=argc==1||!std::strcmp(argv[1],"--kernel-only");if(argc==1||!kernel)explain_filter();if(kernel)actual_kernel();
    std::printf("PASS mmap policy target=%x actual_kernel=%u legacy-pointer-denied direct-RW-allowed EXEC-denied\n",architecture,kernel);
}
