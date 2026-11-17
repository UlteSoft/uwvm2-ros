/** @file @brief Actual fork/exec placement, unchanged parent and fail-closed policy tests. */
#include "support/legacy_process/execution_placement.h"
#include <cassert>
#include <cstdio>
#include <sys/wait.h>
namespace p=uwvm2::imported::ros::scene::execution_placement;
using p::error;
static p::mask affinity()
{p::resolved current;assert(current.prepare({})==error::success);return {current.cpus().begin(),current.cpus().end()};}
static void waited(pid_t pid,int expected=0)
{int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&WEXITSTATUS(status)==expected);}
int main(int argc,char** argv)
{
    if(argc==3&&std::string_view{argv[1]}=="--exec-cpu")
    {
        p::mask expected;assert(p::parse_mask(argv[2],expected)==error::success);auto actual=affinity();
        assert(p::subset(expected,actual)&&p::subset(actual,expected));return 0;
    }
    assert(argc==1);
    p::policy policy;assert(p::parse("0-2,7,63-65,1025","bind","0-1",policy)==error::success&&p::valid(policy));
    assert(policy.cpus.size()==1025/p::word_bits+1&&policy.cpus[0]&1);
    auto original=policy.cpus;
    for(auto text:{"","-1","+1"," 1","1 ","1,","1,,2","1-","3-1","2,1","1,1","1-3,3","1-3,2-4","1:3","18446744073709551616"})
    {assert(p::parse_mask(text,policy.cpus)==error::invalid&&policy.cpus==original);}
    assert(p::parse_mask("8388608",policy.cpus)==error::too_large&&policy.cpus==original);
    assert(p::parse("inherit","bind","none",policy)==error::invalid);
    assert(p::parse("inherit","inherit","0",policy)==error::invalid);
    assert(p::parse("inherit","local","none",policy)==error::success&&p::valid(policy));
    assert(p::parse("inherit","unknown","none",policy)==error::invalid);
    policy.cpus={1,0};assert(!p::valid(policy));p::resolved malformed;assert(malformed.prepare(policy)==error::invalid);
    auto parent=affinity();std::size_t first=SIZE_MAX,second=SIZE_MAX;
    for(std::size_t i{};i<parent.size()*p::word_bits;++i)if(parent[i/p::word_bits]&(p::word{1}<<(i%p::word_bits)))
    {if(first==SIZE_MAX)first=i;else if(second==SIZE_MAX)second=i;}
    assert(first!=SIZE_MAX&&second!=SIZE_MAX); // This qualification reserves two actual allowed logical CPUs.
    std::array<char,32> cpu{};auto rendered=std::to_chars(cpu.data(),cpu.data()+cpu.size()-1,first);assert(rendered.ec==std::errc{});
    assert(p::parse({cpu.data(),static_cast<std::size_t>(rendered.ptr-cpu.data())},"inherit","none",policy)==error::success);
    p::resolved selected;assert(selected.prepare(policy)==error::success&&selected.prepare(policy)==error::conflict);
    assert(!selected.apply()&&affinity()==parent); // Accidental application in the VM is refused.
    auto child=::fork();assert(child>=0);
    if(!child)
    {
        if(!selected.apply())::_exit(11);
        char* arguments[]{argv[0],const_cast<char*>("--exec-cpu"),cpu.data(),nullptr};::execv(argv[0],arguments);::_exit(12);
    }
    waited(child);assert(affinity()==parent);
    p::resolved inherited;assert(inherited.prepare({})==error::success);
    child=::fork();assert(child>=0);
    if(!child)
    {
        // Narrow after preparation; apply must refuse to broaden the child's current mask.
        if(::syscall(SYS_sched_setaffinity,0,policy.cpus.size()*sizeof(p::word),policy.cpus.data()))::_exit(13);
        if(inherited.apply())::_exit(14);
        ::_exit(0);
    }
    waited(child);assert(affinity()==parent);
    assert(p::parse("8388607","inherit","none",policy)==error::success);p::resolved outside;
    assert(outside.prepare(policy)==error::denied&&!outside.apply());
    for(auto memory:{"local","bind","interleave"})
    {
        assert(p::parse("inherit",memory,std::string_view{memory}=="local"?"none":"0",policy)==error::success);
        p::resolved numa;auto prepared=numa.prepare(policy);
        // Host seccomp/cpuset support is evidence, never a reason to change or bypass the gate.
        if(prepared==error::denied||prepared==error::unsupported)
        {std::printf("NUMA %s prepare-refused %u\n",memory,static_cast<unsigned>(prepared));continue;}
        assert(prepared==error::success);child=::fork();assert(child>=0);
        if(!child)::_exit(numa.apply()?0:20);
        int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status));
        assert(WEXITSTATUS(status)==0||WEXITSTATUS(status)==20);
        std::printf("NUMA %s %s\n",memory,WEXITSTATUS(status)==0?"applied-and-verified":"apply-refused");
        assert(affinity()==parent);
    }
    std::puts("PASS execution placement: canonical masks, bounded metadata, zero authority, parent refusal, real child/exec affinity, changed allowed set, unavailable CPU, NUMA apply-or-deny");
    return 0;
}
