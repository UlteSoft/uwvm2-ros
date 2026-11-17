/** @file
 * @brief Verify optional Intel JIT metadata registration without a collector.
 * @details Fake only the official collector boundary; exercise the production
 * registry's validation, default denial, concurrent IDs and retirement order.
 * Run exclusively in the existing 64 GiB cgroup via run_intel_jit_profile.py.
 */
#include <uwvm2/runtime/lib/uwvm_runtime_intel_jit_profile.h>
#include <array>
#include <atomic>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <set>
#if defined(__linux__)
#include <sys/wait.h>
#include <unistd.h>
#endif
namespace profile = uwvm2::runtime::lib::details::intel_jit_profile;
void check(bool condition, char const* text) { if(!condition) { std::cerr << text << '\n'; std::abort(); } }

#if defined(UWVM_RUNTIME_INTEL_JIT_PROFILE) && UWVM_RUNTIME_INTEL_JIT_PROFILE == 1
namespace fake
{
    std::atomic<unsigned> in_api{};
    unsigned queries{}, ids{}, loads{}, unloads{}, next_id{1u};
    bool active{true}, reject_next{};
    struct record { std::string name, module, source; std::vector<LineNumberInfo> lines; };
    std::map<unsigned, record> live;
    std::set<unsigned> used;
    struct lock
    {
        lock() { check(in_api.fetch_add(1u) == 0u, "all SDK calls serialized"); }
        ~lock() { check(in_api.fetch_sub(1u) == 1u, "SDK serial scope balanced"); }
    };
}
extern "C" iJIT_IsProfilingActiveFlags JITAPI iJIT_IsProfilingActive()
{ fake::lock guard; ++fake::queries; return fake::active ? iJIT_SAMPLING_ON : iJIT_NOTHING_RUNNING; }
extern "C" unsigned JITAPI iJIT_GetNewMethodID()
{ fake::lock guard; ++fake::ids; return fake::next_id++; }
extern "C" int JITAPI iJIT_NotifyEvent(iJIT_JVM_EVENT event, void* payload)
{
    fake::lock guard;
    if(event == iJVM_EVENT_TYPE_METHOD_LOAD_FINISHED_V2)
    {
        auto const& value{*static_cast<iJIT_Method_Load_V2*>(payload)};
        check(fake::used.insert(value.method_id).second && value.method_id != 0u, "unique nonzero method ID");
        // Real code owner's storage is still live while the SDK copies bytes.
        volatile unsigned char byte{*static_cast<unsigned char const*>(value.method_load_address)};
        static_cast<void>(byte);
        auto& copy{fake::live[value.method_id]};
        copy.name = value.method_name;
        if(value.module_name) { copy.module = value.module_name; }
        if(value.source_file_name) { copy.source = value.source_file_name; }
        if(value.line_number_size) { copy.lines.assign(value.line_number_table, value.line_number_table + value.line_number_size); }
        ++fake::loads;
        if(fake::reject_next) { fake::reject_next = false; return 0; }
        return 1;
    }
    check(event == iJVM_EVENT_TYPE_METHOD_UNLOAD_START, "no implicit shutdown/unknown event");
    auto const& value{*static_cast<iJIT_Method_Load*>(payload)};
    check(fake::live.erase(value.method_id) == 1u, "each load retired exactly once");
    volatile unsigned char byte{*static_cast<unsigned char const*>(value.method_load_address)};
    static_cast<void>(byte);
    ++fake::unloads;
    return 1;
}
#endif

int main()
{
    using status = profile::registration_status;
    std::array<unsigned char, 128u> code{};
    profile::method_view method{reinterpret_cast<std::uintptr_t>(code.data()), code.size(), "kernel", "probe"};
    {
        profile::registry disabled;
        check(disabled.state() == profile::activation::disabled, "default disabled");
        std::array invalid_rows{profile::line_boundary{999u, 0u}};
        auto invalid{method}; invalid.lines = invalid_rows;
        check(disabled.report(1u, invalid) == status::disabled, "disabled does not admit metadata");
        disabled.retire(1u); disabled.retire_all();
    }
#if !defined(UWVM_RUNTIME_INTEL_JIT_PROFILE) || UWVM_RUNTIME_INTEL_JIT_PROFILE != 1
    profile::registry unsupported{true};
    check(unsupported.report(1u, method) == status::unavailable, "build without SDK stays unavailable");
    std::cout << "PASS absent SDK build, no link dependency and default disabled\n";
#else
    check(fake::queries == 0u && fake::loads == 0u && fake::ids == 0u && fake::unloads == 0u, "off has zero SDK calls");
    fake::active = false;
    { profile::registry absent{true}; check(absent.report(1u, method) == status::unavailable, "missing collector"); }
    check(fake::queries == 1u && fake::ids == 0u, "missing collector cannot allocate method IDs");
    fake::active = true;
    {
        profile::registry records{true};
        auto invalid{method}; invalid.address = UINTPTR_MAX - 4u;
        check(records.report(1u, invalid) == status::invalid_metadata, "address overflow");
        invalid = method; invalid.size = 0;
        check(records.report(1u, invalid) == status::invalid_metadata, "zero extent");
        invalid = method; invalid.name = "bad\nlabel";
        check(records.report(1u, invalid) == status::invalid_metadata, "control label");
        std::array rows{profile::line_boundary{8u, 7u}, profile::line_boundary{128u, 11u}};
        invalid = method; invalid.lines = rows;
        check(records.report(1u, invalid) == status::invalid_metadata, "source required for lines");
        invalid.source = "controlled/probe.c"; rows[1].end_offset = 127u;
        check(records.report(1u, invalid) == status::invalid_metadata, "table must end at actual method end");
        rows[1].end_offset = 8u;
        check(records.report(1u, invalid) == status::invalid_metadata, "monotonic end offsets");
        rows[1].end_offset = 129u;
        check(records.report(1u, invalid) == status::invalid_metadata, "out of bounds row");
        rows[1].end_offset = 128u;
        std::string name{"borrowed-name"}; invalid.name = name;
        check(fake::ids == 0u && records.report(7u, invalid) == status::reported, "invalid metadata had no SDK side effect");
        name.assign("mutated"); rows[0].source_line = 99u;
        check(fake::live.begin()->second.name == "borrowed-name" && fake::live.begin()->second.lines[0].LineNumber == 7u,
              "SDK consumes borrowed metadata synchronously");
        check(records.report(8u, method) == status::invalid_metadata, "duplicate range rejected");
        auto overlap{method}; ++overlap.address; --overlap.size;
        check(records.report(8u, overlap) == status::invalid_metadata, "interior overlap rejected");
        check(records.size() == 1u, "failed report preserves earlier registration");
        records.retire(8u); check(records.size() == 1u, "retirement scoped to actual object");
        records.retire(7u); check(records.size() == 0u && fake::live.empty(), "explicit unload before code release");
        check(records.report(8u, method) == status::reported, "reused address gets fresh ID");
    }
    check(fake::live.empty(), "destructor retires still-live methods");
    fake::reject_next = true;
    { profile::registry records{true}; check(records.report(1u, method) == status::collector_rejected && records.size() == 0u, "partial collector failure cleaned"); }
    std::vector<std::thread> workers;
    for(unsigned thread{}; thread != 8u; ++thread)
    {
        workers.emplace_back([thread]
        {
            std::array<unsigned char, 16u> bytes{};
            profile::registry records{true};
            profile::method_view own{reinterpret_cast<std::uintptr_t>(bytes.data()), bytes.size(), "parallel", "probe"};
            for(unsigned i{}; i != 128u; ++i)
            { check(records.report(thread, own) == status::reported, "parallel report"); records.retire(thread); }
        });
    }
    for(auto& thread : workers) { thread.join(); }
    check(fake::live.empty() && fake::loads == fake::unloads, "parallel lifecycle complete");
#if defined(__linux__)
    {
        profile::registry inherited{true};
        check(inherited.report(91u, method) == status::reported, "parent owns a live fork witness");
        std::atomic<bool> locked{}, release{};
        std::thread holder{[&]
        {
            std::lock_guard guard{profile::sdk::mutex}; locked.store(true, std::memory_order_release);
            while(!release.load(std::memory_order_acquire)) { std::this_thread::yield(); }
        }};
        while(!locked.load(std::memory_order_acquire)) { std::this_thread::yield(); }
        auto const child{::fork()}; check(child >= 0, "fork succeeds");
        if(child == 0)
        {
            ::alarm(3);
            auto const calls{fake::queries + fake::ids + fake::loads + fake::unloads};
            {
                profile::registry new_child{true};
                check(new_child.state() == profile::activation::unavailable, "child cannot activate inherited SDK");
                check(new_child.report(92u, method) == status::unavailable, "child new owner report denied");
            }
            check(inherited.state() == profile::activation::unavailable, "inherited owner is unavailable");
            check(inherited.report(92u, method) == status::unavailable, "inherited report denied before SDK lock");
            inherited.retire(91u); inherited.retire_all(); inherited.~registry();
            check(calls == fake::queries + fake::ids + fake::loads + fake::unloads, "fork child performs zero SDK calls");
            ::_exit(0);
        }
        int result{}; check(::waitpid(child, &result, 0) == child, "child joined");
        release.store(true, std::memory_order_release); holder.join();
        check(WIFEXITED(result) && WEXITSTATUS(result) == 0, "child never waits for parent-held SDK mutex");
        check(inherited.size() == 1u && fake::live.size() == 1u, "parent registration survives child refusal");
    }
#endif
    fake::next_id = UINT_MAX;
    {
        profile::registry records{true};
        check(records.report(2u, method) == status::reported, "final nonzero ID usable");
        records.retire(2u);
        auto before{fake::ids};
        check(records.report(2u, method) == status::exhausted_ids && fake::ids == before, "exhaustion never wraps or calls SDK generator again");
    }
    check(fake::live.empty() && fake::loads == fake::unloads, "all tests leave zero registrations");
    std::cout << "PASS default off, bounds, end-offset lines, copy lifetime, rejection cleanup, concurrent unique IDs, unload and exhaustion\n";
#endif
}
