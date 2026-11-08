// Actual cooperative domain admission/parking. No VM/source/read capability
// is fabricated by these domain-only tests.
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>
#include <source_location>
#include <fast_io.h>
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/utils/thread/cooperative_pause_domain.h>
#if !defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
#error Actual native threads are required for this test.
#endif
using domain=uwvm2::utils::thread::cooperative_pause_domain;
using status=uwvm2::utils::thread::cooperative_pause_result;
static void check(bool ok,::std::source_location at=::std::source_location::current())
{ if(!ok) { ::fast_io::io::perrln("dynamic cooperative domain line ",at.line());::fast_io::fast_terminate(); } }
static auto deadline() { return ::std::chrono::steady_clock::now()+::std::chrono::seconds{30}; }
int main()
{
    {
        domain actual{};::std::vector<domain::participant> leases{};
        for(::std::size_t i{};i!=4097u;++i) { leases.push_back(actual.enter());check(bool(leases.back())); }
        auto const bytes{actual.requested_storage_bytes()};check(bytes>domain::fixed_native_payload_bytes(4096u));
        leases.clear();actual.close();actual.drain();
    }
    {
        domain actual{};auto own{actual.enter()};auto const id{own.identifier()};
        ::std::atomic_bool done{};
        ::std::thread observer{[&]
        { while(!done.load(::std::memory_order_acquire)) { check(own.identifier()==id);check(actual.requested_storage_bytes()!=0u); } }};
        ::std::vector<domain::participant> growing{};
        for(::std::size_t i{};i!=8193u;++i) { growing.push_back(actual.enter());check(bool(growing.back())); }
        done.store(true,::std::memory_order_release);observer.join();
    }
    {
        domain limited{1u};auto first{limited.enter()};check(bool(first));check(!limited.enter());
        first={};auto next{limited.enter()};check(bool(next));
    }
    {
        domain actual{};::std::mutex mutex{};::std::condition_variable changed{};
        ::std::size_t ready{};bool poll{};::std::vector<::std::thread> guests{};
        constexpr ::std::size_t count{257u};
        for(::std::size_t i{};i!=count;++i)
        {
            guests.emplace_back([&,i]
            {
                auto lease{actual.enter()};check(bool(lease));
                { ::std::unique_lock lock{mutex};++ready;changed.notify_all();check(changed.wait_until(lock,deadline(),[&]{return poll;})); }
                lease.poll({1u,i,0u,1u});
            });
        }
        { ::std::unique_lock lock{mutex};check(changed.wait_until(lock,deadline(),[&]{return ready==count;})); }
        auto ticket{actual.request_pause()};check(bool(ticket));
        { ::std::lock_guard lock{mutex};poll=true;changed.notify_all(); }
        check(actual.wait_until_paused(ticket,deadline())==status::paused);
        auto snapshot{actual.capture(ticket)};check(snapshot.result==status::paused && snapshot.participants.size()==count);
        ::std::vector<bool> seen(count);
        for(auto const& item:snapshot.participants)
        { check(item.id!=0u && item.location.function<count && !seen[item.location.function]);seen[item.location.function]=true; }
        check(actual.resume(ticket));for(auto& guest:guests){guest.join();}
        check(actual.capture(ticket).result==status::stale_ticket);
    }
    // Resume permits a waiting new participant to grow slots before the old
    // parked waiter reacquires its mutex. The waiter must retain only its index.
    for(unsigned iteration{};iteration!=200u;++iteration)
    {
        domain actual{};::std::mutex mutex{};::std::condition_variable changed{};bool ready{},poll{};
        ::std::thread original{[&]
        {
            auto lease{actual.enter()};check(bool(lease));
            {::std::unique_lock lock{mutex};ready=true;changed.notify_all();check(changed.wait_until(lock,deadline(),[&]{return poll;}));}
            lease.poll({1u,1u,0u,1u});
        }};
        {::std::unique_lock lock{mutex};check(changed.wait_until(lock,deadline(),[&]{return ready;}));}
        auto ticket{actual.request_pause()};check(bool(ticket));
        {::std::lock_guard lock{mutex};poll=true;changed.notify_all();}
        check(actual.wait_until_paused(ticket,deadline())==status::paused);
        ::std::thread joining{[&]{auto lease{actual.enter()};check(bool(lease));}};
        check(actual.resume(ticket));joining.join();original.join();
        actual.close();actual.drain();
    }
    ::fast_io::io::println("PASS dynamic domain 4097 leases, 257 actual parked native participants, explicit quota, stale ticket and resume/growth lifecycle");
}
