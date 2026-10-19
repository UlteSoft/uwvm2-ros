#include "policy_store_before.h"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <new>

namespace allocation_probe
{
    thread_local bool active{};
    thread_local bool reject{};
    std::size_t calls{}, bytes{};
    struct totals { std::size_t calls{}, bytes{}; };
    struct interval
    {
        interval() { calls = bytes = 0; active = true; }
        ~interval() { active = reject = false; }
        totals finish() { active = reject = false; return {calls, bytes}; }
    };
}
void* operator new(std::size_t size)
{
    if(allocation_probe::active)
    {
        ++allocation_probe::calls;
        allocation_probe::bytes += size;
        if(allocation_probe::reject) { throw std::bad_alloc{}; }
    }
    if(auto p = std::malloc(size ? size : 1)) { return p; }
    throw std::bad_alloc{};
}
void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }
void* operator new[](std::size_t size) { return ::operator new(size); }
void operator delete[](void* p) noexcept { ::operator delete(p); }
void operator delete[](void* p, std::size_t) noexcept { ::operator delete(p); }

namespace
{
    using namespace uwvm2::imported::ros;
    using legacy_policy_store = policy_test::legacy_policy_store;
    unsigned checks{}, dumps{};
    void require(bool condition, char const* message)
    {
        ++checks;
        if(!condition) { std::fprintf(stderr, "FAIL policy lookup: %s\n", message); std::exit(1); }
    }
    void equal(result const& before, result const& after)
    {
        require(before.code == after.code, "result code differs from frozen implementation");
        require(before.detail == after.detail, "result detail differs from frozen implementation");
    }
    void equal(target_policy const* before, target_policy const* after)
    {
        require(bool(before) == bool(after), "lookup presence differs from frozen implementation");
        if(!before) { return; }
        require(before->ros_version == after->ros_version && before->target_scope == after->target_scope && before->name == after->name && before->enabled == after->enabled, "lookup identity differs from frozen implementation");
        require(before->connection == after->connection, "lookup connection differs from frozen implementation");
        require(before->capabilities.size() == after->capabilities.size(), "lookup capability count differs");
        auto right = after->capabilities.begin();
        for(auto const& [alias, cap] : before->capabilities)
        {
            require(right->first == alias && right->second.alias == cap.alias && right->second.operation == cap.operation && right->second.resource == cap.resource && right->second.type == cap.type && right->second.type_identity == cap.type_identity && right->second.limits == cap.limits, "lookup capability differs");
            ++right;
        }
    }
    void dump_equal(legacy_policy_store const& before, policy_store const& after, version v, std::span<std::string const> modules)
    {
        auto old = before.canonical_dump(v, modules);
        auto current = after.canonical_dump(v, modules);
        require(old == current, "canonical JSON bytes differ");
        ++dumps;
    }
    capability_spec clock_cap(std::string alias)
    {
        return {std::move(alias), "time.read-ros", "/clock", "builtin/time-v1", "builtin:time-nanoseconds-v1",
            {{"max-inflight", "1"}, {"max-response-bytes", "8"}, {"max-bytes-per-second", "64"}, {"max-rate-hz", "10"}, {"burst", "2"}, {"audit", "deny-only"},
             {"audit-max-rate-hz", "10"}, {"audit-burst", "2"}, {"audit-max-records", "32"}, {"audit-max-bytes", "4096"}}};
    }
    void report(char const* name, allocation_probe::totals before, allocation_probe::totals after)
    {
        std::printf("PASS allocation %s before_calls=%zu before_requested_bytes=%zu after_calls=%zu after_requested_bytes=%zu\n", name, before.calls, before.bytes, after.calls, after.bytes);
    }
}

int main()
{
    using namespace uwvm2::imported::ros;
    legacy_policy_store before;
    policy_store after;
    std::vector<std::string> names{"a", "A", "a/b", "a\\b", "a\"b", "0", "_", "-", ".module", "mod-2", "z", "\xe4\xb8\xad", "\xc2\xa3", "\xf0\x9f\x8c\x8d", std::string(80, 'm'), std::string(1024, 'z')};
    std::vector<std::string> loaded = names;
    loaded.emplace_back("unbound");
    for(auto v : {version::ros1, version::ros2})
    {
        for(auto const& name : names)
        {
            equal(before.create_target(v, scope::single, name), after.create_target(v, scope::single, name));
            equal(before.create_target(v, scope::single, name), after.create_target(v, scope::single, name));
            equal(before.set_enabled(v, scope::single, name, false), after.set_enabled(v, scope::single, name, false));
            equal(before.set_connection(v, scope::single, name, "node-name", "node"), after.set_connection(v, scope::single, name, "node-name", "node"));
            equal(before.set_connection(v, scope::single, name, "node-name", "duplicate"), after.set_connection(v, scope::single, name, "node-name", "duplicate"));
            equal(before.grant(v, scope::single, name, clock_cap("clock")), after.grant(v, scope::single, name, clock_cap("clock")));
            equal(before.grant(v, scope::single, name, clock_cap("clock")), after.grant(v, scope::single, name, clock_cap("clock")));
            equal(before.set_capability(v, scope::single, name, "clock", "max-inflight", "2"), after.set_capability(v, scope::single, name, "clock", "max-inflight", "2"));
            equal(before.set_capability(v, scope::single, name, "clock", "unknown", "1"), after.set_capability(v, scope::single, name, "clock", "unknown", "1"));
        }
        for(auto const& name : names)
        { equal(before.create_target(v, scope::group, name), after.create_target(v, scope::group, name)); }
        equal(before.bind_module(v, "a", "bound"), after.bind_module(v, "a", "bound"));
        loaded.emplace_back(v == version::ros1 ? "ros1_bound" : "ros2_bound");
        equal(before.bind_module(v, "a", loaded.back()), after.bind_module(v, "a", loaded.back()));
        equal(before.bind_module(v, "a", "bound"), after.bind_module(v, "a", "bound"));
        equal(before.bind_module(v, "missing", "never"), after.bind_module(v, "missing", "never"));
        equal(before.bind_module(v, "a", "a"), after.bind_module(v, "a", "a"));
        equal(before.create_target(v, scope::single, "bound"), after.create_target(v, scope::single, "bound"));
        equal(before.create_target(v, scope::group, "absent_members"), after.create_target(v, scope::group, "absent_members"));
        equal(before.bind_module(v, names[14], "long_group_member"), after.bind_module(v, names[14], "long_group_member"));
        dump_equal(before, after, v, loaded);
    }
    loaded.emplace_back("bound");
    loaded.emplace_back("long_group_member");
    equal(before.create_target(static_cast<version>(255), scope::single, "invalid_version"), after.create_target(static_cast<version>(255), scope::single, "invalid_version"));
    equal(before.create_target(version::ros2, scope::global, "global"), after.create_target(version::ros2, scope::global, "global"));
    auto old_targets = before.configured_targets();
    auto current_targets = after.configured_targets();
    require(old_targets.size() == current_targets.size(), "map size differs");
    for(std::size_t i{}; i < old_targets.size(); ++i) { equal(old_targets[i], current_targets[i]); }
    std::vector<std::string> queries = names;
    queries.insert(queries.end(), {"", "bound", "long_group_member", "unbound", "missing", std::string("a\0b", 3), "\xff", std::string(1025, 'x')});
    for(auto v : {version::ros1, version::ros2, static_cast<version>(0), static_cast<version>(255)})
    {
        for(auto s : {scope::global, scope::single, scope::group, static_cast<scope>(255)})
        {
            for(auto const& name : queries)
            {
                equal(before.find_target(v, s, name), after.find_target(v, s, name));
                equal(before.set_enabled(v, s, name, false), after.set_enabled(v, s, name, false));
            }
        }
        for(auto const& name : queries) { equal(before.effective(v, name), after.effective(v, name)); }
        dump_equal(before, after, v, queries);
        dump_equal(before, after, v, {});
        auto reversed = queries;
        std::reverse(reversed.begin(), reversed.end());
        reversed.insert(reversed.end(), queries.begin(), queries.end());
        dump_equal(before, after, v, reversed);
        require(after.canonical_dump(v, queries) == after.canonical_dump(v, reversed), "deduplication/sort depends on input order");
    }
    equal(before.validate(loaded), after.validate(loaded));
    require(bool(after.validate(loaded)), "valid explicit bindings and capabilities were rejected");
    for(auto const& invalid : std::vector<std::string>{"", ".", "..", "a\nb", std::string("a\0b", 3), "\xff", std::string(1025, 'x')})
    {
        equal(before.create_target(version::ros2, scope::single, invalid), after.create_target(version::ros2, scope::single, invalid));
        equal(before.create_target(version::ros2, scope::group, invalid), after.create_target(version::ros2, scope::group, invalid));
        equal(before.bind_module(version::ros2, "a", invalid), after.bind_module(version::ros2, "a", invalid));
        std::array<std::string, 1> invalid_loaded{invalid};
        equal(before.validate(invalid_loaded), after.validate(invalid_loaded));
    }
    equal(before.validate(loaded), after.validate(loaded));
    auto missing_binding = after.validate(loaded);
    require(missing_binding.code == errc::unknown_module && missing_binding.detail == "ROS binding refers to absent module: .", "valid dot module creation did not require its loaded binding");
    equal(before.validate({}), after.validate({}));
    equal(before.validate({}, false), after.validate({}, false));
    require(bool(after.validate({}, false)), "binding verification bypass changed");
    auto duplicated = loaded;
    duplicated.push_back(loaded.front());
    equal(before.validate(duplicated), after.validate(duplicated));
    std::string identity(80, 'm');
    auto preserved = after.find_target(version::ros2, scope::single, identity);
    identity.assign(80, 'n');
    require(after.find_target(version::ros2, scope::single, names[14]) == preserved && !after.find_target(version::ros2, scope::single, identity), "map keys did not preserve independent ownership");
    auto retained_json = after.canonical_dump(version::ros2, queries);
    auto retained_copy = retained_json;
    queries.clear(); queries.shrink_to_fit();
    require(retained_json == retained_copy, "canonical output retained input views");

    legacy_policy_store old_measure;
    policy_store current_measure;
    std::string module(80, 'm'), missing(80, 'x');
    equal(old_measure.create_target(version::ros2, scope::single, module), current_measure.create_target(version::ros2, scope::single, module));
    auto old_lookup = [&](unsigned kind) { return kind == 0 ? old_measure.find_target(version::ros2, scope::single, module) : kind == 1 ? old_measure.effective(version::ros2, module) : old_measure.effective(version::ros1, missing); };
    auto current_lookup = [&](unsigned kind) { return kind == 0 ? current_measure.find_target(version::ros2, scope::single, module) : kind == 1 ? current_measure.effective(version::ros2, module) : current_measure.effective(version::ros1, missing); };
    for(unsigned kind{}; kind < 3; ++kind)
    {
        target_policy const* old_pointer{};
        allocation_probe::totals old_counts{}, current_counts{};
        { allocation_probe::interval scoped; old_pointer = old_lookup(kind); old_counts = scoped.finish(); }
        target_policy const* current_pointer{};
        { allocation_probe::interval scoped; allocation_probe::reject = true; current_pointer = current_lookup(kind); current_counts = scoped.finish(); }
        equal(old_pointer, current_pointer);
        require(old_counts.calls > 0 && current_counts.calls == 0, "long-name lookup still allocates");
        report(kind == 0 ? "find_target_80_bytes" : kind == 1 ? "effective_bound_80_bytes" : "effective_unbound_80_bytes", old_counts, current_counts);
    }
    {
        allocation_probe::interval scoped;
        allocation_probe::reject = true;
        auto absent = current_measure.find_target(version::ros2, scope::single, missing);
        auto invalid = current_measure.find_target(static_cast<version>(255), static_cast<scope>(255), missing);
        auto normalized = current_measure.find_target(version::ros2, scope::global, missing);
        auto invalid_effective = current_measure.effective(static_cast<version>(255), missing);
        auto counts = scoped.finish();
        require(!absent && !invalid && normalized && !invalid_effective && !counts.calls, "missing/invalid/global-normalized lookup allocates or changes identity");
    }
    std::vector<std::string> measured_modules;
    for(unsigned i{}; i < 16; ++i) { measured_modules.emplace_back(80, static_cast<char>('a' + i)); }
    allocation_probe::totals old_counts{}, current_counts{};
    std::string old_json, current_json;
    { allocation_probe::interval scoped; old_json = old_measure.canonical_dump(version::ros2, measured_modules); old_counts = scoped.finish(); }
    { allocation_probe::interval scoped; current_json = current_measure.canonical_dump(version::ros2, measured_modules); current_counts = scoped.finish(); }
    require(old_json == current_json && current_counts.calls < old_counts.calls && current_counts.bytes < old_counts.bytes, "canonical dump allocation reduction or bytes failed");
    report("canonical_dump_16x80_bytes", old_counts, current_counts);
    legacy_policy_store old_loaded;
    policy_store current_loaded;
    result old_result, current_result;
    { allocation_probe::interval scoped; old_result = old_loaded.validate(measured_modules); old_counts = scoped.finish(); }
    { allocation_probe::interval scoped; current_result = current_loaded.validate(measured_modules); current_counts = scoped.finish(); }
    equal(old_result, current_result);
    require(current_counts.calls < old_counts.calls && current_counts.bytes < old_counts.bytes, "loaded module validation still copies strings");
    report("validate_16x80_bytes", old_counts, current_counts);
    equal(old_measure.set_enabled(version::ros2, scope::single, module, true), current_measure.set_enabled(version::ros2, scope::single, module, true));
    { allocation_probe::interval scoped; old_result = old_measure.validate({}, false); old_counts = scoped.finish(); }
    { allocation_probe::interval scoped; current_result = current_measure.validate({}, false); current_counts = scoped.finish(); }
    equal(old_result, current_result);
    require(old_result.code == errc::invalid_argument && current_counts.calls < old_counts.calls, "fast_io diagnostic allocation reduction failed");
    report("validation_long_target_diagnostic", old_counts, current_counts);
    std::printf("PASS policy lookup differential checks=%u canonical_byte_comparisons=%u canonical_measured_bytes=%zu\n", checks, dumps + 1, current_json.size());
}
