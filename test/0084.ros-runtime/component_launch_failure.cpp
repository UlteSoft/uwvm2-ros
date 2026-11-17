/** @file
 * @brief Kernel-injected launch failures retain and reap actual child ownership.
 * @details A per-test seccomp filter applies to all existing supervisor threads.
 * It denies pidfd acquisition, the authorization send, both, or pre-fork fcntl.
 * No guest can install this filter. Real fork/IPC/reap paths execute; no driver
 * publication is allowed. First failure and exact start retries remain stable.
 */
#define UWVM_COMPONENT_DISPATCH_TEST_MAIN previous_component_main
#include "component_dispatch.cpp"
#include <cstddef>

static void inject(std::string_view mode)
{
    std::array<sock_filter,64> filter{};std::size_t count{};
    auto add=[&](sock_filter value){assert(count<filter.size());filter[count++]=value;};
    add(BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,arch)));
    add(BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,AUDIT_ARCH_X86_64,1,0));
    add(BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_KILL_PROCESS));
    add(BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,nr)));
    auto deny=[&](unsigned number,unsigned code)
    {
        add(BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,number,0,1));
        add(BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ERRNO|code));
    };
    auto argument=[&](unsigned number,unsigned index,unsigned value,unsigned code)
    {
        add(BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,number,0,4));
        add(BPF_STMT(BPF_LD|BPF_W|BPF_ABS,static_cast<unsigned>(offsetof(seccomp_data,args)+index*8)));
        add(BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,value,0,1));
        add(BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ERRNO|code));
        add(BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,nr)));
    };
    if(mode=="pidfd"||mode=="both")deny(SYS_pidfd_open,ENOSYS);
    if(mode=="gate"||mode=="both")
    {
        argument(SYS_sendto,2,8,EIO);
        // Also exercises the previous eventfd gate as a negative control.
        argument(SYS_write,2,8,EIO);
    }
    if(mode=="nonblocking")argument(SYS_fcntl,1,F_SETFL,EIO);
    add(BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW));
    sock_fprog program{static_cast<unsigned short>(count),filter.data()};
    assert(!::prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0));
    assert(!::syscall(SYS_seccomp,SECCOMP_SET_MODE_FILTER,SECCOMP_FILTER_FLAG_TSYNC,&program));
}

int main(int argc,char** argv)
{
    assert(argc==11);std::string_view mode{argv[10]};
    assert(mode=="pidfd"||mode=="gate"||mode=="both"||mode=="nonblocking");
    // Preserve default SIGPIPE behavior: the gate must suppress it itself.
    assert(::signal(SIGPIPE,SIG_DFL)!=SIG_ERR);
    component_fixture fixture{argv+1,1};fixture.open();auto start=fixture.keyed();
    inject(mode);
    assert(fixture.call(cop::component_start,start)==error::success);
    bool forked=mode!="nonblocking";
    auto until=c::now()+2000000000;
    for(;;)
    {
        auto usage=fixture.usage();
        if(usage.failed&&(!forked||usage.exit_observed))break;
        assert(c::now()<until);::usleep(1000);
    }
    auto check=[&]()
    {
        auto usage=fixture.usage();
        assert(usage.failed&&usage.first_failure==s::components::fault::launch_failed);
        assert(usage.exit_observed==forked&&!usage.processes&&!usage.ready&&!usage.messages&&!usage.unknown);
        assert(!fixture.native->publisher->publications);
        assert(fixture.call(cop::component_describe,resource(fixture.instance_id))==error::success);
        robot::response response;
        assert(robot::decode_response(cop::component_describe,fixture.response,{2048,2048,255,16},response));
        assert(response.execution.state=="failed"&&response.execution.first_failure==2&&response.execution.exit_observed==forked);
    };
    check();
    for(unsigned attempt{};attempt<3;++attempt)
    {
        assert(fixture.call(cop::component_start,start)==error::success);robot::response response;
        assert(robot::decode_response(cop::component_start,fixture.response,{2048,2048,255,16},response));
        assert(response.outcome.state=="failed"&&response.outcome.effect=="none");check();
    }
    assert(fixture.call(cop::component_stop,fixture.keyed(true))==error::success);
    check();fixture.component->retire();
    int status{};errno=0;
    assert(::waitpid(-1,&status,WNOHANG)==-1&&errno==ECHILD);
    std::printf("PASS launch failure %s: retained cause, no publication, exact retry, confirmed cleanup, no child leak\n",argv[10]);
}
