// Real host commit refusal, not a Wasm maximum-limit failure or fake kernel.
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/object/memory/linear/mmap.h>
#include <array>
#include <cerrno>
#include <cstdio>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/prctl.h>
#include <sys/mman.h>
#include <linux/audit.h>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <unistd.h>
#if !defined(__linux__) || !defined(__x86_64__) || !defined(UWVM_SUPPORT_MMAP) || !defined(UWVM_CPP_EXCEPTIONS)
#error "This fixture requires real Linux x86_64 mmap memory and exception recovery."
#endif
static bool deny_commit()
{
    // Refuse only writable mprotect, after initial owner allocation. PROT_NONE
    // rollback remains a real allowed kernel operation. No other process gains
    // this additional restriction and no filter is disabled or bypassed.
    std::array<sock_filter,12> filter{{
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,arch)),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,AUDIT_ARCH_X86_64,1,0),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_KILL_PROCESS),
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,nr)),
        BPF_JUMP(BPF_JMP|BPF_JSET|BPF_K,__X32_SYSCALL_BIT,0,1),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_KILL_PROCESS),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,SYS_mprotect,0,4),
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,args[2])),
        BPF_JUMP(BPF_JMP|BPF_JSET|BPF_K,PROT_WRITE,0,2),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ERRNO|ENOMEM),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW)}};
    sock_fprog program{static_cast<unsigned short>(filter.size()),filter.data()};
    return ::syscall(SYS_prctl,PR_SET_NO_NEW_PRIVS,1ul,0ul,0ul,0ul)==0 &&
        ::syscall(SYS_seccomp,SECCOMP_SET_MODE_FILTER,0u,&program)==0;
}
static int run(bool strict)
{
    namespace memory=uwvm2::object::memory::linear;
    auto [page,ok]=uwvm2::object::memory::platform_page::get_platform_page_size();
    if(!ok || !page)return 1;
    memory::mmap_memory_t mem{1u,memory::mmap_memory_status_t::wasm32};
    mem.init_by_page_count(page);
    int proof[2];if(::pipe(proof))return 2;
    auto child=::fork();if(child<0)return 3;
    if(!child)
    {
        ::close(proof[0]);
        if(!deny_commit())::_exit(100);
        void* next=reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(mem.memory_begin)+page);
        errno=0;
        if(::syscall(SYS_mprotect,next,page,PROT_READ|PROT_WRITE)!=-1 || errno!=ENOMEM)::_exit(101);
        if(::write(proof[1],"K",1)!=1)::_exit(102);
        if(strict)
        {
            if(mem.grow_strictly(page*3u) || mem.memory_length_p->load(std::memory_order_acquire)!=page)::_exit(103);
            // A successful write to the original page proves the existing
            // allocation survives this recoverable commit refusal.
            *reinterpret_cast<volatile unsigned char*>(mem.memory_begin)=0x5a;
            if(::write(proof[1],"R",1)!=1)::_exit(104);
            ::_exit(0);
        }
        mem.grow_silently(page*3u);
        ::_exit(105); // fail-fast is required here; returning would be a failure.
    }
    ::close(proof[1]);int status{};if(::waitpid(child,&status,0)!=child)return 4;
    char marker[3]{};auto count=::read(proof[0],marker,sizeof(marker));::close(proof[0]);
    bool passed=strict ? WIFEXITED(status) && WEXITSTATUS(status)==0 && count==2 && marker[0]=='K' && marker[1]=='R' :
        WIFSIGNALED(status) && (WTERMSIG(status)==SIGILL || WTERMSIG(status)==SIGABRT || WTERMSIG(status)==SIGTRAP) && count==1 && marker[0]=='K';
    if(!passed){std::printf("FAIL strict=%d wait=%d marker_count=%zd\n",strict,status,count);return 5;}
    return 0;
}
int main()
{
    rlimit limit{};::setrlimit(RLIMIT_CORE,&limit);
    if(auto result=run(true))return result;
    if(auto result=run(false))return result;
    std::puts("PASS 2/2 real kernel mprotect ENOMEM: strict returns false with old length/data; silent terminates");
}
