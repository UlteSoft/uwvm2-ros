// Explicit quota failures must stay distinct from permission denials. Missing
// quotas remain unlimited; impossible requests must never enter an endless wait.
#include <uwvm2/imported/wasi/wasip1/environment/resource_limits.h>
#include <cassert>
#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

namespace policy = uwvm2::imported::wasi::wasip1::environment;
static bool cancelled(void*) noexcept { return false; }

int main()
{
    policy::resource_limits defaults;
    assert(defaults.max_poll_subscriptions == policy::unlimited);
    assert(defaults.max_poll_wait_ns == policy::unlimited);
    assert(defaults.max_args_env_bytes == policy::unlimited);
    assert(defaults.filesystem_base_rights == 0 && defaults.filesystem_inheriting_rights == 0);
    for(auto const& limit : defaults.operations)
    {
        assert(limit.request_bytes == policy::unlimited && limit.window_bytes == policy::unlimited);
        assert(limit.window_calls == policy::unlimited && limit.total_bytes == policy::unlimited);
        assert(limit.total_calls == policy::unlimited && limit.concurrent == policy::unlimited);
    }
    constexpr auto kind = policy::budget_kind::socket_write;
    policy::budget_ledger ledger;
    // No hidden finite counter ceiling appears after saturation. A later
    // explicit finite total does reject, instead of wrapping into allowance.
    assert(ledger.reserve_checked(kind, {}, UINT64_MAX) == policy::budget_failure::none);
    ledger.release(kind);
    assert(ledger.reserve_checked(kind, {}, 1) == policy::budget_failure::none);
    ledger.release(kind);
    policy::operation_limit finite;
    finite.total_bytes = UINT64_MAX - 1;
    finite.actions[3] = policy::limit_action::wait;
    assert(ledger.reserve_checked(kind, finite, 1) == policy::budget_failure::exhausted);
    policy::budget_ledger empty;
    for(unsigned field : {2u, 4u, 5u})
    {
        policy::operation_limit zero;
        (field == 2 ? zero.window_calls : field == 4 ? zero.total_calls : zero.concurrent) = 0;
        zero.actions[field] = policy::limit_action::wait;
        assert(empty.reserve_checked(kind, zero, 0) == policy::budget_failure::exhausted);
    }
    for(unsigned field : {0u, 1u})
    {
        policy::operation_limit too_small;
        (field == 0 ? too_small.request_bytes : too_small.window_bytes) = 3;
        too_small.actions[field] = policy::limit_action::wait;
        assert(empty.reserve_checked(kind, too_small, 4) == policy::budget_failure::request_too_large);
    }
    // Exercise the actual synchronous wait wrapper and real release notification.
    // Failed attempts may not charge a second call or execute native work.
    auto& limit = defaults.operations[static_cast<unsigned>(kind)];
    limit.concurrent = 1;
    limit.actions[5] = policy::limit_action::wait;
    auto occupied = std::make_unique<policy::budget_reservation>(kind, defaults, 4);
    assert(*occupied);
    std::atomic<unsigned> attempts{};
    unsigned effects{};
    auto const begin = std::chrono::steady_clock::now();
    std::thread release([&]
    {
        unsigned count{};
        while((count = attempts.load(std::memory_order_acquire)) < 2u) attempts.wait(count);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        occupied.reset();
    });
    auto outcome = policy::invoke_with_quota_wait([&]
    {
        attempts.fetch_add(1, std::memory_order_release);
        attempts.notify_one();
        policy::budget_reservation granted{kind, defaults, 4};
        if(granted) ++effects;
        else assert(granted.failure == policy::budget_failure::waiting);
    }, cancelled, nullptr);
    release.join();
    assert(outcome == policy::quota_call_result::completed && effects == 1);
    assert(attempts.load() >= 3u && std::chrono::steady_clock::now() - begin >= std::chrono::milliseconds(5));
    auto const& counter = defaults.accounting->counters[static_cast<unsigned>(kind)];
    assert(counter.active == 0 && counter.total_calls == 2 && counter.total_bytes == 8);
    std::cout << "PASS unlimited defaults, saturating totals, impossible wait rejection and exactly-once synchronous admission\n";
}
