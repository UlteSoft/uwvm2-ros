#include <uwvm2/imported/ros/scene/communication_events.h>
#include <uwvm2/imported/ros/scene/event_contract.h>
#include <uwvm2/imported/ros/scene/event_broker.h>
#include <cassert>
#include <atomic>
#include <iostream>
#include <vector>
#include <thread>
#include <set>
#if defined(__linux__)
# include <sys/wait.h>
#endif
namespace e = uwvm2::imported::ros::scene::events;
namespace c = uwvm2::imported::ros::scene::communication;

struct observed final : e::source
{
    std::mutex mutex;
    e::snapshot current{1, 1, false, false};
    c::owner expected{1, 2, 1};

    c::error inspect(c::owner owner, c::handle resource, c::handle generation, e::snapshot& out) noexcept override
    {
        std::lock_guard lock{mutex};
        if(owner != expected || resource != 9 || generation != 1) { return c::error::denied; }
        out = current;
        return c::error::ok;
    }

    void set(bool ready, bool terminal = false)
    {
        std::lock_guard lock{mutex};
        ++current.revision;
        current.ready = ready;
        current.terminal = terminal;
    }
};

struct authority
{ std::atomic_bool allowed{true}; };

static bool admit(void* arg, c::owner owner, c::handle resource) noexcept
{ return static_cast<authority*>(arg)->allowed.load() && owner == c::owner{1, 2, 1} && resource == 9; }

struct capture
{
    std::vector<e::event> values;
    bool accept{};
    e::stream* stream{};
    authority* permission{};
    bool reenter{}, close{}, revoke{};
    observed* reincarnate{};
};

static bool deliver(void* arg, std::span<e::event const> events)
{
    auto& out = *static_cast<capture*>(arg);
    out.values.assign(events.begin(), events.end());
    if(out.reenter) { assert(out.stream->poll({1, 1, 1}, 1, c::now() + 1000000000ULL, out.permission, admit, &out, deliver) == c::error::again); }
    if(out.close) { assert(out.stream->close({1, 1, 1}) == c::error::ok); }
    if(out.revoke) { out.permission->allowed = false; }
    if(out.reincarnate)
    {
        std::lock_guard lock{out.reincarnate->mutex};
        ++out.reincarnate->current.generation;
    }
    return out.accept;
}

int main()
{
    e::stream stream;
    c::profile profile;
    profile.identity = "readiness";
    profile.digest.fill(1);
    e::plan plan{
        {1, 1, 1},
        profile,
        1,
        1,
        1,
        2000000000ULL
    };
    authority permission;
    auto source = std::make_shared<observed>();
    std::array<e::binding, 1> bindings{{{{1, 2, 1}, 9, 1, source}}};
    assert(stream.configure({}, bindings, &permission, admit) == c::error::invalid);
    permission.allowed = false;
    assert(stream.configure(plan, bindings, &permission, admit) == c::error::denied);
    permission.allowed = true;
    assert(stream.configure(plan, bindings, &permission, admit) == c::error::ok);
    capture out;
    out.stream = &stream;
    out.permission = &permission;
    out.reenter = true;
    auto poll = [&] { return stream.poll({1, 1, 1}, 1, c::now() + 1000000000ULL, &permission, admit, &out, deliver); };
    assert(poll() == c::error::io && out.values.size() == 1 && out.values[0].source_index == 1 && !out.values[0].ready);
    source->set(true);
    out.accept = true;
    assert(poll() == c::error::ok && !out.values[0].ready && out.values[0].sequence == 1);
    assert(poll() == c::error::ok && out.values[0].ready && out.values[0].sequence == 2);
    assert(poll() == c::error::ok && out.values[0].ready && out.values[0].sequence == 3);
    source->set(false);
    assert(poll() == c::error::ok && !out.values[0].ready && out.values[0].sequence == 4);
    source->set(false, true);
    assert(poll() == c::error::again);  // terminal is not authorized
    source->set(true, true);
    out.revoke = true;
    assert(poll() == c::error::denied);
    out.revoke = false;
    permission.allowed = true;
    assert(poll() == c::error::ok && out.values[0].sequence == 5 && !out.values[0].terminal);
    source->set(true, true);
    out.close = true;
    assert(poll() == c::error::cancelled);
    assert(poll() == c::error::cancelled && source->current.ready);
    e::stream fenced;
    assert(fenced.configure(plan, bindings, &permission, admit) == c::error::ok);
    capture changed;
    changed.accept = true;
    changed.reincarnate = source.get();
    assert(fenced.poll({1, 1, 1}, 1, c::now() + 1000000000ULL, &permission, admit, &changed, deliver) == c::error::stale);
    {
        std::lock_guard lock{source->mutex};
        source->current.generation = 1;
    }
    changed.reincarnate = nullptr;
    assert(fenced.poll({1, 1, 1}, 1, c::now() + 1000000000ULL, &permission, admit, &changed, deliver) == c::error::ok && changed.values[0].sequence == 1);
    e::registry handles{1};
    assert(handles.configure(plan, 1) == c::error::ok);
    c::handle id{};
    bool closed{};
    assert(handles.open({1, 1, 1}, profile, bindings, &permission, admit, id) == c::error::ok);
    c::handle unchanged{444};
    assert(handles.open({1, 1, 1}, profile, bindings, &permission, admit, unchanged) == c::error::again && unchanged == 444);
    assert(handles.close({2, 1, 1}, id, closed) == c::error::denied && !closed);
    assert(handles.close({1, 1, 1}, id, closed) == c::error::ok && closed);
    assert(handles.close({1, 1, 1}, id, closed) == c::error::ok);
    assert(handles.open({1, 1, 1}, profile, bindings, &permission, admit, unchanged) == c::error::ok && unchanged != id);
    handles.revoke({1, 1, 1});
    assert(handles.open({1, 1, 1}, profile, bindings, &permission, admit, id) == c::error::denied);
    namespace ec = uwvm2::imported::ros::event_contract;
    namespace wire = uwvm2::imported::ros::scene::wire;
    std::array<std::uint8_t, 256> encoded{};
    wire::writer writer{encoded};
    assert(writer.u64(1) && writer.u64(2) && writer.u64(9) && writer.text("readiness") && writer.blob(profile.digest) && writer.u64(1024) && writer.u64(1) &&
           writer.u64(512));
    ec::request parsed;
    assert(ec::decode(ec::operation::open, writer.written(), parsed) && parsed.bytes == 1024 && parsed.items == 1 && parsed.workspace == 512 &&
           ec::matches(parsed, profile));
    assert(!ec::decode(ec::operation::open, writer.written().first(writer.size() - 1), parsed));
    std::array<e::event, 1> record{{{1, 1, 1, 0, 1, true, true}}};
    wire::writer output{encoded};
    assert(ec::encode(output, record));
    wire::reader reader{output.written()};
    std::uint64_t count{}, word{};
    bool present{};
    std::string_view text;
    wire::bytes payload;
    assert(reader.u64(count) && count == 1 && reader.u64(word) && word == 1 && reader.u64(word) && word == 1 && reader.u64(word) && word == 1);
    assert(reader.text(text) && text == "readiness.snapshot" && reader.boolean(present) && !present && reader.u64(word) && word == 0);
    assert(reader.text(text) && text == "uwvm/events/Readiness" && reader.text(text) && text == "scene-canonical" && reader.blob(payload) && reader.done());
    wire::reader detail{payload};
    assert(detail.u64(word) && word == 1 && detail.boolean(present) && present && detail.boolean(present) && !present && detail.done());
    e::stream concurrent;
    assert(concurrent.configure(plan, bindings, &permission, admit) == c::error::ok);
    source->set(true);
    std::mutex collected_mutex;
    std::set<c::handle> sequences;
    auto run = [&]
    {
        for(unsigned i{}; i < 100; ++i)
        {
            capture own;
            own.accept = true;
            auto status = concurrent.poll({1, 1, 1}, 1, c::now() + 1000000000ULL, &permission, admit, &own, deliver);
            assert(status == c::error::ok || status == c::error::again);
            if(status == c::error::ok)
            {
                std::lock_guard lock{collected_mutex};
                assert(sequences.insert(own.values[0].sequence).second);
            }
        }
    };
    std::thread first{run}, second{run};
    first.join();
    second.join();
    assert(!sequences.empty());
#if defined(__linux__)
    auto child = ::fork();
    assert(child >= 0);
    if(!child)
    {
        capture own;
        own.accept = true;
        auto status = concurrent.poll({1, 1, 1}, 1, c::now() + 1000000000ULL, &permission, admit, &own, deliver);
        ::_exit(status == c::error::denied ? 0 : 1);
    }
    int child_status{};
    assert(::waitpid(child, &child_status, 0) == child && WIFEXITED(child_status) && WEXITSTATUS(child_status) == 0);
#endif
    std::cout << "passed event readiness authority, snapshot transaction, visibility, replay and close groups\n";
}
