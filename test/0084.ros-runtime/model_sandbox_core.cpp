/** @file @brief Closed sandbox policy, immutable files and deployment bounds. */
#include "model_worker_fixture.h"
#include <cstdio>
#include <sys/mman.h>
static std::uint32_t decision(x::syscall_filter const& filter,seccomp_data const& data)
{
    std::uint32_t accumulator{};auto instructions=filter.view();assert(!instructions.empty());
    for(std::size_t offset{},steps{};offset<instructions.size()&&steps<2048;++offset,++steps)
    {
        auto instruction=instructions[offset];
        switch(instruction.code)
        {
            case BPF_LD|BPF_W|BPF_ABS:assert(instruction.k<=sizeof(data)-4);std::memcpy(&accumulator,reinterpret_cast<std::uint8_t const*>(&data)+instruction.k,4);break;
            case BPF_JMP|BPF_JEQ|BPF_K:offset+=accumulator==instruction.k?instruction.jt:instruction.jf;break;
            case BPF_JMP|BPF_JSET|BPF_K:offset+=(accumulator&instruction.k)?instruction.jt:instruction.jf;break;
            case BPF_RET|BPF_K:return instruction.k;
            default:assert(false);
        }
    }
    assert(false);return 0;
}
int main(int argc,char** argv)
{
    assert(argc==10);uwvm2::utils::container::std_vector<x::descriptor> ownership;auto manifest=worker_manifest(argv+1,ownership);assert(x::valid(manifest));
    auto invalid=manifest;invalid.bound={};assert(!x::valid(invalid));
    for(auto path:{"/worker/../evil","/worker//evil","/worker/.","/root/evil","relative","/worker/trailing/"})assert(!x::destination(path));
    invalid=manifest;invalid.files[1].destination=invalid.files[0].destination;assert(!x::valid(invalid));
    invalid=manifest;assert(invalid.files[1].destination.assign("/worker/bootstrap/nested"));assert(!x::valid(invalid));
    invalid=manifest;invalid.files[1].role=x::image_role::weights;assert(!x::valid(invalid));
    std::uint64_t exact=manifest.launcher.bytes;for(auto const& file:manifest.files)exact+=file.bytes*(file.role==x::image_role::weights?1:2);
    invalid=manifest;invalid.bound.image_bytes=exact;assert(x::valid(invalid));invalid.bound.image_bytes=exact-1;assert(!x::valid(invalid));
    invalid=manifest;invalid.bound.scratch_slots=17;assert(!x::valid(invalid));invalid=manifest;invalid.bound.open_files=1;assert(!x::valid(invalid));
    auto weights=manifest.files[2];x::descriptor sealed;assert(x::seal(weights,sealed)==x::error::success);
    auto seals=::fcntl(sealed.get(),F_GET_SEALS);assert(seals>=0&&(seals&(1|2|4|8|32))==(1|2|4|8|32));
    struct stat information{};assert(!::fstat(sealed.get(),&information)&&!(information.st_mode&0111));
    std::uint8_t byte{1};assert(::pwrite(sealed.get(),&byte,1,0)<0&&::ftruncate(sealed.get(),0)<0&&::fchmod(sealed.get(),0700)<0);
    x::descriptor rejected;weights.sha256[0]^=1;assert(x::seal(weights,rejected)==x::error::conflict&&!rejected);
    weights=manifest.files[2];weights.bytes++;assert(x::seal(weights,rejected)==x::error::invalid&&!rejected);
    x::descriptor directory{::open("/",O_PATH|O_DIRECTORY|O_CLOEXEC)};assert(directory);weights.source=directory.get();assert(x::seal(weights,rejected)==x::error::invalid);
    weights=manifest.files[2];weights.role=x::image_role::unconfigured;assert(x::seal(weights,rejected)==x::error::invalid);

    x::syscall_filter startup,final{true};seccomp_data call{};call.arch=AUDIT_ARCH_X86_64;
    auto allowed=[&](int syscall,bool after=true){call.nr=syscall;auto a=decision(startup,call);return a==SECCOMP_RET_ALLOW&&(!after||decision(final,call)==SECCOMP_RET_ALLOW);};
    for(auto syscall:{SYS_socket,SYS_connect,SYS_sendmsg,SYS_ptrace,SYS_process_vm_readv,SYS_process_vm_writev,SYS_mount,SYS_unshare,SYS_setns,SYS_clone,SYS_fork,SYS_vfork,SYS_io_uring_setup,SYS_bpf,SYS_perf_event_open,SYS_memfd_create,SYS_mkdir,SYS_link,SYS_symlink,SYS_chmod,SYS_fchmod,SYS_ioctl,SYS_pidfd_getfd,SYS_seccomp})assert(!allowed(syscall));
    assert(allowed(SYS_read)&&allowed(SYS_write)&&allowed(SYS_execve,false)&&!allowed(SYS_execve)&&!allowed(SYS_execveat));
    call.args[0]=3;assert(allowed(SYS_recvmsg));call.args[0]=4;assert(!allowed(SYS_recvmsg));call.args[0]=0;
    call.args[2]=O_RDONLY;assert(allowed(SYS_openat));call.args[2]=O_CREAT|O_RDONLY;assert(!allowed(SYS_openat));call.args[2]=O_TMPFILE|O_RDWR;assert(!allowed(SYS_openat));
    call.args[2]=PROT_READ|PROT_WRITE;assert(allowed(SYS_mmap));call.args[2]=PROT_READ|PROT_EXEC;assert(allowed(SYS_mmap,false)&&!allowed(SYS_mmap)&&!allowed(SYS_mprotect));
    call.args[2]=0;call.args[1]=F_GET_SEALS;assert(allowed(SYS_fcntl));call.args[1]=F_SETFL;assert(!allowed(SYS_fcntl));
    call.args[1]=RLIMIT_FSIZE;call.args[2]=1;assert(allowed(SYS_prlimit64,false)&&!allowed(SYS_prlimit64));call.args[1]=RLIMIT_NOFILE;assert(!allowed(SYS_prlimit64,false));
    call.args[2]=0;assert(allowed(SYS_prlimit64));call.nr=SYS_read;call.arch=AUDIT_ARCH_I386;assert(decision(startup,call)==SECCOMP_RET_KILL_PROCESS);
    call.arch=AUDIT_ARCH_X86_64;call.nr=SYS_read|0x40000000;assert(decision(startup,call)==SECCOMP_RET_KILL_PROCESS);
    std::puts("passed 4 sandbox policy and immutable image groups");
}
