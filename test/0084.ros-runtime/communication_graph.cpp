#include <uwvm2/imported/ros/scene/communication_graph_events.h>
#include <uwvm2/imported/ros/scene/communication_graph_contract.h>
#include <cassert>
#include <iostream>
namespace c = uwvm2::imported::ros::scene::communication;
namespace g = c::graph;

struct native_source : g::source
{
    bool active{true};
    unsigned calls{};
    ::uwvm2::utils::container::std_vector<g::entity> rows;

    bool live() const noexcept override { return active; }

    c::handle context() const noexcept override { return 7; }

    c::owner authority() const noexcept override { return {1, 3, 1}; }

    c::error capture(g::plan const&, ::uwvm2::utils::container::std_vector<g::entity>& output) noexcept override
    {
        ++calls;
        try
        {
            output = rows;
            return c::error::ok;
        }
        catch(...)
        {
            return c::error::no_memory;
        }
    }
};

struct capture
{
    g::snapshot* snapshot{};
    c::profile profile;
    c::handle revision{};
    std::optional<c::handle> cursor;
    bool accept{};
    std::size_t count{};
};

static bool copy_page(void* ptr, g::page const& page)
{
    auto& out = *static_cast<capture*>(ptr);
    out.revision = page.revision;
    out.cursor = page.cursor;
    out.count = page.entities.size();
    return out.accept;
}

int main()
{
    auto native = std::make_shared<native_source>();
    native->rows = {
        {g::kind::node,         "/scene/node", "",           ""            },
        {g::kind::publisher,    "/scene/node", "/scene/one", "test/Message"},
        {g::kind::subscription, "/scene/node", "/scene/two", "test/Message"}
    };
    c::profile profile;
    profile.identity = "selected graph";
    profile.digest.fill(3);
    namespace contract = uwvm2::imported::ros::graph_contract;
    namespace wire = uwvm2::imported::ros::scene::wire;
    std::array<std::uint8_t, 1024> encoded;
    wire::writer writer{encoded};
    assert(writer.u64(6) && writer.u64(7) && writer.u64(11)&&writer.u64(12)&&writer.text(profile.identity) && writer.blob(profile.digest) && writer.boolean(true) && writer.u64(9) && writer.u64(2));
    contract::request decoded;
    auto request = writer.written();
    assert(contract::decode(contract::operation::snapshot, request, decoded) && contract::matches(decoded, profile) && decoded.cursor == 9 &&
           decoded.limit == 2&&decoded.observer_node_capability==11&&decoded.observer_node==12);
    for(std::size_t i{}; i < request.size(); ++i) { assert(!contract::decode(contract::operation::snapshot, request.first(i), decoded)); }
    assert(!contract::decode(contract::operation::watch, request, decoded));
    std::array<std::uint8_t, 1024> page_bytes;
    wire::writer page_writer{page_bytes};
    assert(contract::encode(page_writer, {1, 2, native->rows}));
    wire::reader page_reader{page_writer.written()};
    std::uint64_t number{};
    bool present{};
    assert(page_reader.u64(number) && number == 1 && page_reader.boolean(present) && present && page_reader.u64(number) && number == 2 &&
           page_reader.u64(number) && number == 3);
    for(auto const& row: native->rows)
    {
        std::string_view text;
        wire::bytes body;
        assert(page_reader.text(text) && text == "uwvm/graph/Entity" && page_reader.text(text) && text == "scene-canonical" && page_reader.blob(body));
        wire::reader fields{body};
        assert(fields.u64(number) && number == static_cast<std::uint64_t>(row.type) && fields.text(text) && text == row.node && fields.text(text) &&
               text == row.endpoint && fields.text(text) && text == row.message_type && fields.done());
    }
    assert(page_reader.done());
    wire::writer short_page{std::span<std::uint8_t>{page_bytes}.first(page_writer.size() - 1)};
    assert(!contract::encode(short_page, {1, 2, native->rows}));
    g::plan plan{
        {1, 2, 1},
        profile,
        7,
        {"/scene/node"},
        39,
        4,
        1024,
        2,
        5000000000ULL,
        1000000000ULL
    };
    g::snapshot snapshot;
    assert(snapshot.configure(plan, native) == c::error::ok);
    capture copied;
    copied.profile = profile;
    auto read = [&](std::optional<c::handle> cursor, std::size_t limit) { return snapshot.read({1, 2, 1}, profile, 7, cursor, limit, &copied, copy_page); };
    assert(read({}, 2) == c::error::io && native->calls == 1 && copied.count == 2 && copied.revision == 1 && copied.cursor);
    auto cursor = copied.cursor;
    native->rows.pop_back();
    assert(read({}, 1) == c::error::busy && native->calls == 1);
    copied.accept = true;
    assert(read({}, 2) == c::error::ok && copied.cursor == cursor && native->calls == 1);
    assert(read(cursor, 2) == c::error::ok && copied.count == 1 && !copied.cursor && copied.revision == 1);
    assert(read(cursor, 2) == c::error::stale);
    assert(read({}, 2) == c::error::ok && native->calls == 2 && copied.count == 2 && copied.revision == 2);
    c::handle observed{};
    bool ready{};
    assert(snapshot.observe({1, 2, 1}, observed, ready) == c::error::ok && observed == 2 && !ready);
    native->rows.push_back({g::kind::subscription, "/scene/node", "/scene/new", "test/Message"});
    assert(snapshot.observe({1, 2, 1}, observed, ready) == c::error::ok && observed == 3 && ready);
    assert(read({}, 2) == c::error::ok && copied.cursor && copied.revision == 3);
    auto pinned = copied.cursor;
    native->rows.pop_back();
    assert(snapshot.observe({1, 2, 1}, observed, ready) == c::error::ok && observed == 4 && ready);
    assert(read(pinned, 2) == c::error::ok && copied.count == 1 && copied.revision == 3);
    assert(snapshot.observe({1, 2, 1}, observed, ready) == c::error::ok && ready);  // unread revision survives old page acknowledgement
    assert(read({}, 2) == c::error::ok && copied.revision == 4);
    assert(snapshot.observe({1, 2, 1}, observed, ready) == c::error::ok && !ready);
    native->rows[0].node = "/secret/node";
    assert(read({}, 2) == c::error::invalid);
    native->rows[0].node = "/scene/node";
    native->active = false;
    assert(read({}, 2) == c::error::cancelled);
    std::cout << "passed graph exact filter, bounded snapshot, revision, cursor and failed-copy recovery groups\n";
}
