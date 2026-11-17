/** @file @brief Finite ABI limits and real child-only kernel ceiling checks. */
#include <uwvm2/imported/ros/scene/linux_resource_limits.h>
#include <cassert>
#include <cerrno>
#include <cstdio>
#include <sys/wait.h>
#include <unistd.h>
namespace limits=uwvm2::imported::ros::scene::linux_resource_limits;
static_assert(limits::representable<std::uint32_t>(0,UINT32_MAX));
static_assert(limits::representable<std::uint32_t>(UINT32_MAX-1,UINT32_MAX));
static_assert(!limits::representable<std::uint32_t>(UINT32_MAX,UINT32_MAX));
static_assert(!limits::representable<std::uint32_t>(UINT64_C(1)<<32,UINT32_MAX));
static_assert(!limits::representable<std::uint32_t>(UINT64_MAX,UINT32_MAX));
static_assert(limits::representable<std::uint64_t>(UINT32_MAX,UINT64_MAX));
static_assert(limits::representable<std::uint64_t>(UINT64_MAX-1,UINT64_MAX));
static_assert(!limits::representable<std::uint64_t>(UINT64_MAX,UINT64_MAX));
int main()
{
    rlimit before{};assert(!::getrlimit(RLIMIT_NOFILE,&before));
    auto child=::fork();assert(child>=0);
    if(!child)
    {
        assert(before.rlim_cur>=64&&before.rlim_max>=64);
        assert(limits::apply(RLIMIT_NOFILE,64));
        rlimit observed{};assert(!::getrlimit(RLIMIT_NOFILE,&observed));
        assert(observed.rlim_cur==64&&observed.rlim_max==64);
        assert(!limits::apply(RLIMIT_NOFILE,static_cast<std::uint64_t>(RLIM_INFINITY)));
        assert(!::getrlimit(RLIMIT_NOFILE,&observed)&&observed.rlim_cur==64&&observed.rlim_max==64);
        assert(!limits::apply(-1,1));
        assert(limits::apply(RLIMIT_CORE,0));
        assert(!::getrlimit(RLIMIT_CORE,&observed)&&!observed.rlim_cur&&!observed.rlim_max);
        ::_exit(0);
    }
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
    rlimit after{};assert(!::getrlimit(RLIMIT_NOFILE,&after));
    assert(before.rlim_cur==after.rlim_cur&&before.rlim_max==after.rlim_max);
    std::puts("PASS finite 32/64-bit conversion boundaries and child-only kernel ceilings");
}
