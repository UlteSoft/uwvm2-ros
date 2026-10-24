/** @file
 * Test-only, same-process execution gate for the actual interpreter Worker.
 *
 * Usage: exec_gate /absolute/pinned/Worker [the Worker's ordinary arguments].
 * This fixture is not a deployment launcher and grants no Guest capability.
 * It opens the selected ordinary executable without following a final symlink,
 * retains that descriptor, writes JS_EXEC_GATE <pid> <fd>, and waits at most
 * twenty seconds for exactly G on stdin. The owner must prove this executable,
 * its owned birth/pidfd/cgroup, and the complete held Worker descriptor before
 * G. execveat consumes that exact descriptor atomically; the kernel preserves
 * PID/birth while replacing the gate with the Worker and closing CLOEXEC FDs.
 * The actual JavaScript artifact is then the Worker's main module, so no Wasm
 * wrapper can borrow its ROS permissions across a module ancestry boundary.
 *
 * The guarded producer must retain this source, full compiler dependencies,
 * linker inputs and native ELF product. A printed marker alone proves none of
 * those facts. No shell, daemon, dynamic executable lookup or network is used.
 */
#if __cplusplus >= 202002L
#include <fast_io.h>
#include <fast_io_dsal/string.h>
#else
// Native Noetic/Focal builds select the real C++17 I/O shim. This changes
// only bounded protocol formatting/pipe reads, never ROS calls or authority.
#include "../../../../../src/ros/gateway/common/native_io.h"
#endif
#include <cerrno>
#include <chrono>
#include <fcntl.h>
#include <poll.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

extern char** environ;

int main(int argc, char** argv)
{
    if(argc < 3 || argv[1][0] != '/') { return 125; }
    int target{::open(argv[1], O_RDONLY | O_CLOEXEC | O_NOFOLLOW | O_NONBLOCK)};
    if(target < 0) { return 125; }
    struct stat facts{};
    if(::fstat(target, &facts) || !S_ISREG(facts.st_mode) || !(facts.st_mode & 0111))
    { ::close(target); return 125; }
    try
    {
#if __cplusplus >= 202002L
        auto line{fast_io::concat_fast_io("JS_EXEC_GATE ", ::getpid(), " ", target, "\n")};
        auto end{fast_io::operations::write_some(fast_io::posix_io_observer{STDOUT_FILENO},
                                               line.data(), line.data()+line.size())};
        if(end != line.data()+line.size()) { ::close(target); return 125; }
#else
        namespace io=uwvm2::ros_gateway::native_io;
        io::fixed_text<96> line;
        if(!line.append("JS_EXEC_GATE ") || !line.number(::getpid()) || !line.append(" ") ||
           !line.number(target) || !line.append("\n") || !io::write_all(STDOUT_FILENO,line.view()))
        { ::close(target); return 125; }
#endif
        auto deadline{std::chrono::steady_clock::now()+std::chrono::seconds{20}};
        for(;;)
        {
            auto now{std::chrono::steady_clock::now()};
            if(now >= deadline) { ::close(target); return 125; }
            auto milliseconds{std::chrono::duration_cast<std::chrono::milliseconds>(deadline-now).count()};
            pollfd input{STDIN_FILENO, POLLIN, 0};
            int ready{::poll(&input, 1, static_cast<int>(milliseconds > 100 ? 100 : milliseconds+1))};
            if(ready < 0 && errno == EINTR) { continue; }
            if(ready < 0 || input.revents & (POLLERR | POLLNVAL | POLLHUP))
            { ::close(target); return 125; }
            if(!ready) { continue; }
            char request{};
#if __cplusplus >= 202002L
            auto last{fast_io::operations::read_some(fast_io::posix_io_observer{STDIN_FILENO},
                                                   &request, &request+1)};
            if(last != &request+1 || request != 'G') { ::close(target); return 125; }
#else
            ssize_t count;
            do { count=::read(STDIN_FILENO,&request,1); } while(count<0 && errno==EINTR);
            if(count!=1 || request!='G') { ::close(target); return 125; }
#endif
            break;
        }
        // argv+1 names the actual Worker and retains its exact reviewed argv.
        // An ELF product needs no shebang interpreter or pathname fallback.
        ::syscall(SYS_execveat, target, "", argv+1, environ, AT_EMPTY_PATH);
    }
    catch(...) { ::close(target); return 125; }
    ::close(target);
    return 126; // execveat returned: no Worker/Guest success is possible.
}
