/** @file
 * @brief Real kernel launch refusal must not leak a child or stable reservation.
 * @details Inject after sealing, before fork, using an inherited seccomp filter.
 * Both pidfd and permit failures are actual kernel errors, not mocked workers.
 * An unapproved child must be positively reaped before cleanup is confirmed;
 * lack of namespace identity must not masquerade as such identity. Retry after
 * failure or retirement preserves the original cause and never launches again.
 */
#define UWVM_COMMUNICATION_SANDBOX_NO_MAIN
#include "communication_worker_sandbox.cpp"
#include <uwvm2/imported/ros/scene/communication_worker_stage.h>
#include <cstdarg>
namespace c=s::communication;

static void inject(std::string_view mode)
{
    std::array<sock_filter,64> filter{};std::size_t count{};
    auto add=[&](sock_filter value){assert(count<filter.size());filter[count++]=value;};
    add(BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,arch)));
    add(BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,AUDIT_ARCH_X86_64,1,0));
    add(BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_KILL_PROCESS));
    add(BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,nr)));
    auto argument=[&](unsigned number,unsigned index,unsigned value)
    {
        add(BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,number,0,4));
        add(BPF_STMT(BPF_LD|BPF_W|BPF_ABS,static_cast<unsigned>(offsetof(seccomp_data,args)+index*8)));
        add(BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,value,0,1));
        add(BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ERRNO|EIO));
        add(BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,nr)));
    };
    if(mode=="pidfd"||mode=="both")
    {
        add(BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,SYS_pidfd_open,0,1));
        add(BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ERRNO|ENOSYS));
    }
    if(mode=="gate"||mode=="both")
    {
        argument(SYS_sendto,2,8);
        argument(SYS_write,2,8); // Negative control: the previous eventfd gate.
    }
    if(mode=="nonblocking")argument(SYS_fcntl,1,F_SETFL);
    add(BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW));
    sock_fprog program{static_cast<unsigned short>(count),filter.data()};
    assert(!::prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0));
    assert(!::syscall(SYS_seccomp,SECCOMP_SET_MODE_FILTER,SECCOMP_FILTER_FLAG_TSYNC,&program));
}

static bool inject_parent_setup{};
static unsigned nonblocking_setups{};
extern "C" int __real_fcntl(int,int,...);
extern "C" int __wrap_fcntl(int descriptor,int command,...)
{
    if(command==F_GETFL||command==F_GETFD)return __real_fcntl(descriptor,command);
    va_list args;va_start(args,command);auto value=va_arg(args,int);va_end(args);
    // The first three setups belong to child protocol/diagnostic endpoints.
    // Install an actual kernel refusal at the first parent endpoint setup.
    // The old implementation reaches this point only after authorizing exec.
    if(inject_parent_setup&&command==F_SETFL&&++nonblocking_setups==4)inject("nonblocking");
    return __real_fcntl(descriptor,command,value);
}

struct retained_context final:c::native_context
{
    bool live()const noexcept override{return true;}
    c::error shutdown()noexcept override{return c::error::ok;}
};

int main(int argc,char** argv)
{
    assert(argc==10);std::string_view mode{argv[9]};
    assert(mode=="pidfd"||mode=="gate"||mode=="both"||mode=="nonblocking"||mode=="parent-nonblocking"||mode=="cancel-before");
    assert(::signal(SIGPIPE,SIG_DFL)!=SIG_ERR);
    uwvm2::utils::container::std_vector<x::descriptor> owned;
    auto manifest=make_manifest(argv+1,owned,11);
    c::node_plan plan;plan.authority={1,5,2};plan.selected.identity="node";plan.selected.digest[0]=1;
    plan.context.identity="context";plan.context.digest[0]=2;plan.name="robot";plan.name_space="/scene";plan.max_observers=1;
    auto context=uwvm2::utils::container::make_shared<retained_context>();int caller{};
    c::node_stage_request request{&caller,3,51,52,101,{1,6,7},{}};request.request_digest[0]=1;
    w::node_stage value;assert(value.configure(manifest,plan)==c::error::ok);
    if(mode=="cancel-before")value.request_stop();
    else if(mode=="parent-nonblocking")inject_parent_setup=true;
    else inject(mode);
    auto expected=mode=="pidfd"||mode=="both"?c::error::unsupported:
        mode=="cancel-before"?c::error::cancelled:c::error::io;
    auto actual=value.prepare_node(plan,context,request);
    assert(actual==expected||(mode=="both"&&actual==c::error::io));expected=actual;
    bool forked=mode!="nonblocking"&&mode!="parent-nonblocking"&&mode!="cancel-before";
    auto before=value.inspect();
    assert(before.failure==expected&&before.launches==unsigned(forked)&&before.cleanup_debt==forked&&!before.identity_confirmed);
    assert(value.retained_process()->input_fd()==-1&&value.retained_process()->output_fd()==-1);
    auto until=uwvm2::imported::ros::resources::steady_now()+2000000000;
    for(;;)
    {
        auto result=value.cleanup_progress();if(result==c::error::ok)break;
        assert(result==c::error::again&&uwvm2::imported::ros::resources::steady_now()<until);::poll(nullptr,0,1);
    }
    auto check=[&]()
    {
        auto final=value.inspect();
        assert(final.phase==w::stage_phase::retired&&!final.cleanup_debt&&!final.identity_confirmed);
        assert(final.failure==expected&&final.launches==unsigned(forked));
    };
    check();
    for(unsigned retry{};retry<3;++retry)
    {
        assert(value.prepare_node(plan,context,request)==(mode=="cancel-before"?c::error::denied:c::error::cancelled));
        check();assert(value.cleanup_progress()==c::error::ok);check();
    }
    int status{};errno=0;assert(::waitpid(-1,&status,WNOHANG)==-1&&errno==ECHILD);
    std::printf("PASS communication launch failure %s: no publication, first cause retained, child reaped, reservation released, retry inert\n",argv[9]);
}
