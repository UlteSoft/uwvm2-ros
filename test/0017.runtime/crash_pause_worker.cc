// Real Linux thread-directed fault stops, including a real signal handler and
// a stricter seccomp policy refusing SIGSTOP. This is a native component test;
// run_fault_pause.py separately exercises actual INT/AOT Guest execution.
#include <uwvm2/runtime/diagnostic/crash_policy.h>
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <signal.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

namespace crash = uwvm2::runtime::diagnostic::crash;
static volatile sig_atomic_t expected_state;

[[noreturn]] static void finish(bool signal_context) noexcept
{
    if(signal_context)
    {
        crash::report_signal(crash::category::memtag, "native signal pause regression");
        crash::pause_process_signal();
    }
    else
    {
        crash::report(crash::category::memtag, "native worker pause regression");
        crash::pause_process();
    }
    if(crash::stop_state.load(std::memory_order_acquire) != static_cast<unsigned>(expected_state)) ::_exit(91);
    if(expected_state == 3 && errno != EPERM) ::_exit(92);
    // Deliberately immediate: a process-directed kill used to let this native
    // terminal worker exit before a different thread handled the group stop.
    ::_exit(static_cast<int>(crash::exit_status()));
}

static void signal_handler(int) noexcept { finish(true); }

static bool refuse_current_thread_stop() noexcept
{
    // Only deny SIGSTOP via the self-thread delivery calls. All other calls,
    // including delivery of our real SIGUSR1 handler, retain their policy.
    sock_filter rules[]{
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, offsetof(seccomp_data, nr)),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_tgkill, 0, 3),
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, offsetof(seccomp_data, args[2])),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SIGSTOP, 4, 5),
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_tkill, 0, 3),
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, offsetof(seccomp_data, args[1])),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SIGSTOP, 0, 1),
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ERRNO | EPERM),
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),
    };
    sock_fprog program{static_cast<unsigned short>(sizeof(rules) / sizeof(*rules)), rules};
    return ::prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) == 0 &&
        ::prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &program) == 0;
}

static bool exercise(bool worker, bool signal_context, bool pause, bool denied, unsigned repetition)
{
    auto const child = ::fork();
    if(child < 0) return false;
    if(child == 0)
    {
        ::alarm(5); ::close(2);
        expected_state = !pause ? 0 : denied ? 3 : 2;
        if(!crash::process_policy.seal(pause ? crash::action::pause : crash::action::none)) ::_exit(93);
        struct sigaction action{};
        action.sa_handler = signal_handler;
        if(::sigemptyset(&action.sa_mask) != 0 || ::sigaction(SIGUSR1, &action, nullptr) != 0) ::_exit(94);
        auto terminal = [=]() noexcept
        {
            if(denied && !refuse_current_thread_stop()) ::_exit(77);
            if(signal_context)
            {
                static_cast<void>(::raise(SIGUSR1));
                ::_exit(95);
            }
            finish(false);
        };
        if(worker) { std::thread thread{terminal}; thread.join(); }
        else terminal();
        ::_exit(96);
    }
    int status{};
    pid_t result;
    do { result = ::waitpid(child, &status, WUNTRACED); } while(result < 0 && errno == EINTR);
    bool const stopped = result == child && WIFSTOPPED(status) && WSTOPSIG(status) == SIGSTOP;
    if(result == child && WIFSTOPPED(status))
    {
        if(::kill(child, SIGCONT) != 0) { ::kill(child, SIGKILL); }
        do { result = ::waitpid(child, &status, 0); } while(result < 0 && errno == EINTR);
    }
    bool const expected_stop = pause && !denied;
    int const code = result == child && WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    bool const passed = result == child && stopped == expected_stop && code == 122;
    std::printf("worker=%u signal=%u pause=%u denied=%u repetition=%u stopped=%u expected_stop=%u exit=%d %s\n",
        worker, signal_context, pause, denied, repetition, stopped, expected_stop, code, passed ? "PASS" : "FAIL");
    return passed;
}

int main()
{
    rlimit core{}; ::setrlimit(RLIMIT_CORE, &core);
    unsigned passed{}, total{};
    auto count = [&](bool value) { ++total; passed += value; };
    for(bool signal_context : {false, true})
    {
        count(exercise(false, signal_context, true, false, 0));
        count(exercise(true, signal_context, false, false, 0));
        for(unsigned repeat{}; repeat != 32u; ++repeat) count(exercise(true, signal_context, true, false, repeat));
        count(exercise(true, signal_context, true, true, 0));
    }
    std::printf("crash-pause-worker %u/%u\n", passed, total);
    return passed == total ? 0 : 1;
}
