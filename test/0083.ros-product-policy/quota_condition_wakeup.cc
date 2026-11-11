// Force the portable condition-variable implementation on a Linux test host.
// This qualifies that algorithm, not macOS/Windows kernel or provider behavior.
// Real cancellation queries are paused AFTER reading false; actual stop/release
// and its sole wake complete BEFORE the query returns that old observation.
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <semaphore>
#include <thread>
#include <vector>
#include <uwvm2/utils/control/host_diagnostics.h>
#include <uwvm2/utils/thread/execution_domain.h>
#include <uwvm2/utils/thread/monotonic_clock.h>
#include <uwvm2/utils/thread/process_exit.h>

#if !defined(__linux__)
#error "This fixture explicitly forces only resource_limits.h's portable branch on a Linux host"
#endif
#if !defined(UWVM2TEST_USE_NATIVE_QUOTA_WAIT)
#pragma push_macro("__linux__")
#undef __linux__
#endif
#include <uwvm2/imported/wasi/wasip1/environment/resource_limits.h>
#if !defined(UWVM2TEST_USE_NATIVE_QUOTA_WAIT)
#pragma pop_macro("__linux__")
#endif

namespace quota = uwvm2::imported::wasi::wasip1::environment;
namespace lifetime = uwvm2::utils::thread;
using namespace std::chrono_literals;

struct checkpoint
{
    lifetime::execution_domain& domain;
    std::counting_semaphore<>& entered;
    std::counting_semaphore<>& finished;
    std::binary_semaphore resume{0};
    lifetime::execution_domain::lease* lease{};
    unsigned failed_attempts{}, effects{}, attempts{};
    bool held_snapshot{};
    quota::quota_call_result result{};

    static bool cancelled(void* raw) noexcept
    {
        auto& self = *static_cast<checkpoint*>(raw);
        // Both queries are actual execution-lifetime observations. No fake
        // cancellation flag or production scheduling hook is installed.
        bool const observed = self.lease->stop_requested() || self.domain.host_wait_interrupted();
        if(self.failed_attempts == 2 && !self.held_snapshot)
        {
            self.held_snapshot = true;
            if(observed || !quota::host_wait_scope::current->pending ||
               quota::host_wait_scope::current->wake_at != std::chrono::steady_clock::time_point::max()) std::abort();
            self.entered.release();
            self.resume.acquire();
            // The other thread has now completed real stop/quota release and
            // notification. Returning the earlier false is a valid interleaving.
        }
        return observed;
    }
};

static bool forced_window(bool cancel, unsigned count)
{
    lifetime::execution_domain domain{count};
    quota::resource_limits limits;
    constexpr auto kind = quota::budget_kind::random;
    auto& limit = limits.operations[static_cast<unsigned>(kind)];
    limit.concurrent = 1;
    limit.actions[5] = quota::limit_action::wait;
    auto const before = limits.accounting->counters[static_cast<unsigned>(kind)].total_calls;
    auto occupied = std::make_unique<quota::budget_reservation>(kind, limits, 4);
    if(!*occupied) std::abort();
    // One real owner notification must wake every provider, not just one.
    lifetime::execution_domain::host_wait_registration registration{domain,
        [](void*) noexcept { quota::process_budget_ledger.notify_waiters(); }, nullptr};
    std::counting_semaphore<> entered{0}, finished{0};
    std::vector<std::unique_ptr<checkpoint>> states;
    std::vector<std::thread> workers;
    for(unsigned i{}; i != count; ++i)
    {
        states.push_back(std::make_unique<checkpoint>(domain, entered, finished));
        auto& state = *states.back();
        workers.emplace_back([&state, &domain, &limits]
        {
            auto admitted = domain.try_enter();
            if(!admitted) std::abort();
            state.lease = &admitted;
            state.result = quota::invoke_with_quota_wait([&]() noexcept
            {
                ++state.attempts;
                quota::budget_reservation reservation{kind, limits, 4};
                if(reservation) { ++state.effects; return; }
                if(reservation.failure != quota::budget_failure::waiting) std::abort();
                ++state.failed_attempts;
                // Align the unmodified old fallback and the registered retry:
                // only the FIRST failure has an immediately elapsed deadline.
                // The forced second failure has a genuinely infinite deadline.
                if(state.failed_attempts == 1) quota::host_wait_scope::current->wake_at = std::chrono::steady_clock::now();
            }, checkpoint::cancelled, &state);
            state.finished.release();
        });
    }
    for(unsigned i{}; i != count; ++i)
        if(!entered.try_acquire_for(5s)) { std::fputs("FAIL did not reach the forced infinite-wait window\n", stderr); std::_Exit(2); }
    std::binary_semaphore action_completed{0};
    std::thread management([&]
    {
        if(cancel) domain.request_stop();
        else occupied.reset();
        action_completed.release();
    });
    // Also catches an incorrect fix invoking cancellation while ledger-locked:
    // domain stop must finish while the cancelled() callback is still paused.
    if(!action_completed.try_acquire_for(5s))
    { std::fputs("FAIL stop/release deadlocked against the paused cancellation query\n", stderr); std::_Exit(3); }
    management.join();
    for(auto& state : states) state->resume.release();
    bool lost{};
    for(unsigned i{}; i != count; ++i)
    {
        if(finished.try_acquire_for(2s)) continue;
        lost = true;
        std::fprintf(stderr, "FAIL %s notification lost before CV wait (%u providers); no further release was supplied\n", cancel ? "stop" : "release", count);
        // Rescue only AFTER recording the regression so old code terminates.
        quota::process_budget_ledger.notify_waiters();
        if(!finished.try_acquire_for(5s)) std::_Exit(4);
    }
    for(auto& worker : workers) worker.join();
    occupied.reset();
    bool correct = !lost;
    for(auto const& state : states)
        correct = correct && state->held_snapshot && state->effects == (cancel ? 0u : 1u) &&
            state->result == (cancel ? quota::quota_call_result::cancelled : quota::quota_call_result::completed);
    auto const& counter = limits.accounting->counters[static_cast<unsigned>(kind)];
    return correct && counter.active == 0 && counter.total_calls == before + 1u + (cancel ? 0u : count);
}

int main(int argc, char** argv)
{
    auto const selection = argc == 2 ? argv[1] : "all";
    bool ran{};
    for(bool cancel : {true, false})
    {
        if(std::strcmp(selection, "all") != 0 && std::strcmp(selection, cancel ? "stop" : "release") != 0) continue;
        ran = true;
        for(unsigned count : {1u, 4u}) if(!forced_window(cancel, count)) return 1;
    }
    if(!ran) return 64;
#if defined(UWVM2TEST_USE_NATIVE_QUOTA_WAIT)
    std::puts("PASS native Linux eventfd path: forced stop/release before infinite wait; single and broadcast; exact effects; no lock inversion");
#else
    std::puts("PASS forced portable-CV path on Linux host: stop/release before infinite wait; single and broadcast; exact effects; no lock inversion");
#endif
}
