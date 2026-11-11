// Real processes exercise the production cold policy and process-exit owners.
// This component does not claim to validate a generated Guest unwind; the
// maintained signed-module-state VM runner covers the actual managed boundary.
#define UWVM_ROS_ZERO_HOST_DEFAULTS 1
#include <uwvm2/runtime/diagnostic/crash_policy.h>
#include <uwvm2/utils/thread/process_exit.h>
#include <uwvm2/runtime/lib/uwvm_runtime_managed_fault_policy.h>
#include <atomic>
#include <cerrno>
#include <cstdio>
#include <signal.h>
#include <poll.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

namespace crash = uwvm2::runtime::diagnostic::crash;
namespace process_exit = uwvm2::utils::thread::process_exit;

static int wait_code(pid_t child)
{
    int status{};
    pid_t got;
    do { got = ::waitpid(child, &status, 0); } while(got < 0 && errno == EINTR);
    return got == child && WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}

static bool simple(unsigned mode, unsigned category)
{
    auto child = ::fork();
    if(child < 0) return false;
    if(child == 0)
    {
        ::alarm(5); ::close(2);
        if(mode != 0 && !crash::process_policy.seal(mode == 2 ? crash::action::pause : crash::action::none)) ::_exit(90);
        if(mode != 1) crash::report(static_cast<crash::category>(category), "fixture classified fault");
        if(mode == 4) crash::report(crash::category::execution, "later classified fault");
        runtime_managed_fault_detail::finish_cli_fault_after_unwind();
        ::_exit(0);
    }
    auto expected = mode <= 2 ? 0 : static_cast<int>(category);
    auto actual = wait_code(child);
    std::printf("mode=%u category=%u expected=%d actual=%d %s\n", mode, category, expected, actual, actual == expected ? "PASS" : "FAIL");
    return actual == expected;
}

static bool wait_owner(bool prior_exit)
{
    int notice[2], release[2];
    if(::pipe(notice) || ::pipe(release)) return false;
    auto child = ::fork();
    if(child < 0) return false;
    if(child == 0)
    {
        ::alarm(5); ::close(2); ::close(notice[0]); ::close(release[1]);
        if(!crash::process_policy.seal(crash::action::none)) ::_exit(91);
        crash::report(crash::category::memtag, "fixture classified fault");
        std::atomic<bool> ready{};
        std::thread owner{[&]
        {
            process_exit::operation pending{[](void* pointer) noexcept
            {
                auto fd = *static_cast<int*>(pointer);
                if(::write(fd, "W", 1) != 1) ::_exit(92);
            }, &notice[1]};
            if(!pending.select(process_exit::wait_action::wait)) ::_exit(93);
            ready.store(true, std::memory_order_release);
            char byte{};
            if(::read(release[0], &byte, 1) != 1 || byte != 'R') ::_exit(94);
            // This exact native wait owner must leave scope before process exit.
        }};
        while(!ready.load(std::memory_order_acquire)) std::this_thread::yield();
        std::thread first;
        if(prior_exit)
        {
            first = std::thread{[] { process_exit::request_process_exit(37); }};
            while(!process_exit::exit_claimed.load(std::memory_order_acquire)) std::this_thread::yield();
        }
        runtime_managed_fault_detail::finish_cli_fault_after_unwind();
        ::_exit(95);
    }
    ::close(notice[1]); ::close(release[0]);
    unsigned received{};
    auto required = prior_exit ? 2u : 1u;
    bool valid = true;
    while(received < required)
    {
        pollfd descriptor{notice[0], POLLIN, 0};
        if(::poll(&descriptor, 1, 2000) != 1 || !(descriptor.revents & POLLIN)) { valid = false; break; }
        char byte{};
        if(::read(notice[0], &byte, 1) != 1 || byte != 'W') { valid = false; break; }
        ++received;
    }
    int status{};
    // The helper has actually entered the coordinator (twice for first-exit),
    // yet an outstanding wait owner must still prevent terminal process exit.
    auto before = ::waitpid(child, &status, WNOHANG);
    valid = valid && before == 0;
    if(before == 0)
    {
        if(::write(release[1], "R", 1) != 1) valid = false;
    }
    ::close(notice[0]); ::close(release[1]);
    auto actual = before == 0 ? wait_code(child) : (before == child && WIFEXITED(status) ? WEXITSTATUS(status) : -1);
    auto expected = prior_exit ? 37 : 122;
    valid = valid && actual == expected;
    std::printf("wait-owner prior=%u notifications=%u held=%u expected=%d actual=%d %s\n",
                prior_exit, received, before == 0, expected, actual, valid ? "PASS" : "FAIL");
    return valid;
}

int main()
{
    rlimit core{}; ::setrlimit(RLIMIT_CORE, &core);
    unsigned passed{}, total{};
    auto count = [&](bool result) { ++total; passed += result; };
    count(simple(0, 122)); // Unsealed embedding records but returns.
    count(simple(1, 122)); // Sealed terminate with no fault is not a fault.
    count(simple(2, 122)); // Explicit pause retains the managed result.
    for(unsigned category = 120; category <= 123; ++category) count(simple(3, category));
    count(simple(4, 122)); // The first recorded category remains authoritative.
    count(wait_owner(false));
    count(wait_owner(true));
    std::printf("managed-fault-exit-policy %u/%u\n", passed, total);
    return passed == total ? 0 : 1;
}
