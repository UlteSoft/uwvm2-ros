// Regression for a real lost-wakeup window: stop state must precede the sole
// event notification. No synthetic clock or production scheduling hook is used.
#include <uwvm2/utils/thread/execution_domain.h>
#include <uwvm2/imported/wasi/wasip1/environment/resource_limits.h>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <mutex>
#include <thread>
#include <sys/eventfd.h>
#include <poll.h>
#include <unistd.h>

namespace lifetime = uwvm2::utils::thread;
namespace quota = uwvm2::imported::wasi::wasip1::environment;

struct notification
{
    lifetime::execution_domain::lease const& lease;
    int event{::eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK)};
    std::atomic<unsigned> calls{};
    std::atomic<bool> cancelled_at_first_wake{};
    ~notification() { if(event >= 0) ::close(event); }
    static void wake(void* context) noexcept
    {
        auto& self = *static_cast<notification*>(context);
        // This only reads the actual atomic lifetime predicate and signals a
        // native primitive. It never waits, takes ownership or reenters domain.
        if(self.calls.fetch_add(1, std::memory_order_relaxed) == 0)
            self.cancelled_at_first_wake.store(self.lease.stop_requested(), std::memory_order_release);
        std::uint64_t one{1};
        if(::write(self.event, &one, sizeof(one)) != sizeof(one)) std::abort();
    }
    bool receive()
    {
        ::pollfd watched{event, POLLIN, 0};
        if(::poll(&watched, 1, 3000) != 1 || !(watched.revents & POLLIN)) return false;
        std::uint64_t value{};
        return ::read(event, &value, sizeof(value)) == sizeof(value) && value != 0;
    }
};

static bool stop_order(unsigned kind)
{
    lifetime::execution_domain domain{2};
    auto admitted = domain.try_enter();
    if(!admitted) return false;
    notification observed{admitted};
    if(observed.event < 0) return false;
    lifetime::execution_domain::host_wait_registration registration{domain, notification::wake, &observed};
    std::atomic<bool> administrative_success{};
    std::thread management([&]
    {
        switch(kind)
        {
            case 0: domain.request_stop(); break;
            case 1:
            {
                auto transition = domain.try_maintenance_transition();
                if(!transition || !domain.request_stop_without_callbacks_owned_transition(transition)) std::abort();
                break;
            }
            case 2: domain.stop_and_drain(); break;
            case 3: domain.reset([] {}); break;
            case 4:
            {
                auto transition = domain.try_maintenance_transition();
                if(!transition || !domain.reset_owned_transition(transition, [] {})) std::abort();
                break;
            }
            case 5:
            {
                auto transition = domain.try_maintenance_transition();
                auto prepared = domain.prepare_replacement_generation(transition);
                auto drain = domain.drain_prepared_generation_until(transition, prepared,
                    std::chrono::steady_clock::now() + std::chrono::seconds(5));
                if(drain.status != lifetime::execution_domain::prepared_drain_status::drained || !drain.actual) std::abort();
                break;
            }
            default: std::abort();
        }
        administrative_success.store(true, std::memory_order_release);
    });
    bool const signalled = observed.receive();
    bool const published = observed.cancelled_at_first_wake.load(std::memory_order_acquire);
    admitted.reset();
    management.join();
    bool const reset = kind == 3 || kind == 4;
    auto next = domain.try_enter();
    bool const outcome = signalled && published && administrative_success.load() && static_cast<bool>(next) == reset;
    if(!outcome)
        std::fprintf(stderr, "FAIL stop path %u: notification=%d actual cancellation visible at first wake=%d\n", kind, signalled, published);
    return outcome;
}

static bool update_fence_does_not_cancel()
{
    lifetime::execution_domain domain{2};
    auto admitted = domain.try_enter();
    notification observed{admitted};
    lifetime::execution_domain::host_wait_registration registration{domain, notification::wake, &observed};
    auto transition = domain.try_maintenance_transition();
    auto fence = domain.fence_for_update(transition);
    if(!admitted || !transition || !fence || domain.try_enter() || admitted.stop_requested() || domain.host_wait_interrupted() || observed.calls.load()) return false;
    if(domain.drain_update_until(transition, fence, std::chrono::steady_clock::now())) return false;
    if(!domain.cancel_private_update_fence(transition, fence) || admitted.stop_requested() || domain.host_wait_interrupted() || observed.calls.load()) return false;
    auto reopened = domain.try_enter();
    return static_cast<bool>(reopened);
}

static bool quota_wait(bool cancel)
{
    lifetime::execution_domain domain{1};
    quota::resource_limits limits;
    constexpr auto kind = quota::budget_kind::random;
    auto& limit = limits.operations[static_cast<unsigned>(kind)];
    limit.concurrent = 1;
    limit.actions[5] = quota::limit_action::wait;
    auto const before = limits.accounting->counters[static_cast<unsigned>(kind)].total_calls;
    auto occupied = std::make_unique<quota::budget_reservation>(kind, limits, 4);
    if(!*occupied) return false;
    std::atomic<unsigned> attempts{};
    std::atomic<bool> finished{};
    unsigned effects{};
    quota::quota_call_result result{};
    std::thread worker([&]
    {
        auto admitted = domain.try_enter();
        if(!admitted) std::abort();
        lifetime::execution_domain::host_wait_registration registration{domain,
            [](void*) noexcept { quota::process_budget_ledger.notify_waiters(); }, nullptr};
        struct context { lifetime::execution_domain* domain; lifetime::execution_domain::lease* lease; } actual{&domain, &admitted};
        result = quota::invoke_with_quota_wait([&]() noexcept
        {
            attempts.fetch_add(1, std::memory_order_release);
            quota::budget_reservation reservation{kind, limits, 4};
            if(reservation) ++effects;
            else if(reservation.failure != quota::budget_failure::waiting) std::abort();
        }, [](void* raw) noexcept
        {
            auto& actual = *static_cast<context*>(raw);
            return actual.lease->stop_requested() || actual.domain->host_wait_interrupted();
        }, &actual);
        finished.store(true, std::memory_order_release);
    });
    auto const deadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    for(;;)
    {
        bool pending{};
        {
            std::lock_guard lock{quota::process_budget_ledger.mutex};
            pending = quota::process_budget_ledger.waiters != nullptr && attempts.load(std::memory_order_acquire) >= 2;
        }
        if(pending) break;
        if(finished.load(std::memory_order_acquire) || std::chrono::steady_clock::now() > deadline) std::abort();
        std::this_thread::yield();
    }
    if(cancel) domain.request_stop();
    else occupied.reset();
    // The quota has no window deadline. Successful cancellation requires the
    // actual notification; a periodic timer cannot turn a lost wake into PASS.
    while(!finished.load(std::memory_order_acquire))
    {
        if(std::chrono::steady_clock::now() > deadline) std::abort();
        std::this_thread::yield();
    }
    worker.join();
    occupied.reset();
    auto const& counter = limits.accounting->counters[static_cast<unsigned>(kind)];
    return result == (cancel ? quota::quota_call_result::cancelled : quota::quota_call_result::completed) &&
        effects == (cancel ? 0u : 1u) && counter.active == 0 && counter.total_calls == before + (cancel ? 1u : 2u);
}

int main()
{
    for(unsigned path{}; path != 6; ++path) if(!stop_order(path)) return 1;
    if(!update_fence_does_not_cancel()) return 2;
    if(!quota_wait(true) || !quota_wait(false)) return 3;
    std::puts("PASS stop published before native wake on 6 lifecycle paths; update fence preserves leases; infinite quota wait cancels or resumes exactly once");
}
