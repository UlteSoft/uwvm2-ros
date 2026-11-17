/** @file
 * @brief Reject stale process identities before writing user-namespace maps.
 * @details Two actual gated namespace inits model a numeric-PID reuse boundary:
 * the first birth pidfd is already dead while a new, unmapped task is alive.
 * Mapping with that stale identity must leave both maps of the live task empty.
 * A subsequent call with its actual birth identity must succeed. A reference
 * build deliberately uses the prior helper lacking the pidfd argument and
 * must fail this assertion. Neither process executes external code.
 */
#include "support/legacy_process/linux_process_domain.h"
#include <cassert>
#include <cstdio>
#include <cstring>

namespace domain=uwvm2::imported::ros::scene::linux_process_domain;

struct child_record { pid_t pid; int identity; };

static child_record create(int owner)
{
    int identity{-1};bool restored{};
    auto pid=domain::spawn(identity,restored);assert(pid>=0);
    if(!pid)
    {
        if(!domain::enter(owner))::_exit(126);
        for(;;)::pause();
    }
    assert(restored&&identity>=0);return {pid,identity};
}

static void terminate(child_record child)
{
    assert(!::syscall(SYS_pidfd_send_signal,child.identity,SIGKILL,nullptr,0));
    int status{};assert(::waitpid(child.pid,&status,0)==child.pid);
    assert(WIFSIGNALED(status)&&WTERMSIG(status)==SIGKILL);
    pollfd dead{child.identity,POLLIN,0};assert(::poll(&dead,1,0)==1&&(dead.revents&POLLIN));
}

static bool empty_map(pid_t child,char const* name)
{
    std::array<char,96> path{};
    auto length=std::snprintf(path.data(),path.size(),"/proc/%ld/%s",static_cast<long>(child),name);
    assert(length>0&&static_cast<std::size_t>(length)<path.size());
    auto fd=::open(path.data(),O_RDONLY|O_CLOEXEC|O_NOFOLLOW);assert(fd>=0);
    char byte{};auto length_read=::read(fd,&byte,1);::close(fd);assert(length_read>=0);
    return length_read==0;
}

static bool map(child_record child,int identity)
{
#ifdef UWVM_TEST_REFERENCE_MAPPING_WITHOUT_IDENTITY
    (void)identity;return domain::map_identity(child.pid,::geteuid(),::getegid());
#else
    return domain::map_identity(child.pid,identity,::geteuid(),::getegid());
#endif
}

int main()
{
    auto owner=static_cast<int>(::syscall(SYS_pidfd_open,::getpid(),0));assert(owner>=0);
    auto dead=create(owner);terminate(dead);
    auto live=create(owner);
    assert(empty_map(live.pid,"uid_map")&&empty_map(live.pid,"gid_map"));
    auto stale_accepted=map(live,dead.identity);
    auto maps_unchanged=empty_map(live.pid,"uid_map")&&empty_map(live.pid,"gid_map");
    // Always reap before checking the reference negative control: no orphan is
    // rescued by the surrounding test runner to make a result appear correct.
    if(stale_accepted||!maps_unchanged)
    {
        terminate(live);::close(live.identity);::close(dead.identity);::close(owner);
        assert(!"stale birth identity authorized mapping of another process");
    }
    assert(map(live,live.identity));
    assert(!empty_map(live.pid,"uid_map")&&!empty_map(live.pid,"gid_map"));
    assert(!map(live,live.identity));
    terminate(live);::close(live.identity);::close(dead.identity);::close(owner);
    std::puts("PASS stale birth identity leaves live task unmapped; exact identity maps once");
}
