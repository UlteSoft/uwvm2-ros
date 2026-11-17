/** @file @brief Actual target-libc finite limit checks, including large-file ABI. */
#include <uwvm2/imported/ros/scene/linux_resource_limits.h>
#include <cassert>
#include <cstdio>
#include <sys/wait.h>
#include <unistd.h>
namespace limits=uwvm2::imported::ros::scene::linux_resource_limits;
int main()
{
    auto infinity=static_cast<std::uint64_t>(RLIM_INFINITY);
    assert(limits::finite(0)&&limits::finite(4096)&&!limits::finite(infinity));
    if constexpr(sizeof(rlim_t)==4)
    {
        assert(!limits::finite(UINT32_MAX));
        assert(!limits::finite(UINT64_C(1)<<32));
        assert(!limits::finite((UINT64_C(1)<<32)+4096));
    }
    else assert(limits::finite(UINT32_MAX)&&limits::finite(UINT64_C(1)<<32));
    rlimit before{};assert(!::getrlimit(RLIMIT_NOFILE,&before));
    auto child=::fork();assert(child>=0);
    if(!child)
    {
        assert(limits::apply(RLIMIT_NOFILE,64));
        assert(!limits::apply(RLIMIT_NOFILE,infinity));
        if constexpr(sizeof(rlim_t)==4)
            assert(!limits::apply(RLIMIT_NOFILE,(UINT64_C(1)<<32)+4096));
        rlimit value{};assert(!::getrlimit(RLIMIT_NOFILE,&value));
        assert(value.rlim_cur==64&&value.rlim_max==64);
        assert(limits::apply(RLIMIT_CORE,0));
        ::_exit(0);
    }
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
    rlimit after{};assert(!::getrlimit(RLIMIT_NOFILE,&after));
    assert(after.rlim_cur==before.rlim_cur&&after.rlim_max==before.rlim_max);
    std::printf("PASS actual libc rlim_t=%zu off_t=%zu finite ceilings; owner unchanged\n",sizeof(rlim_t),sizeof(off_t));
}
