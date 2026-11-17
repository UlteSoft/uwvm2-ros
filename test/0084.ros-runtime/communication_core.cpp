#include <uwvm2/imported/ros/scene/communication.h>
#include <uwvm2/imported/ros/scene/communication_contract.h>
#include <uwvm2/imported/ros/scene/schema_contract.h>
#include <cassert>
#include <iostream>
#include <thread>
#if defined(__linux__)
# include <sys/wait.h>
#endif

namespace c = uwvm2::imported::ros::scene::communication;

static c::profile profile(char const* name, unsigned byte)
{
    c::profile p;
    p.identity = name;
    p.digest.fill(static_cast<std::uint8_t>(byte));
    return p;
}

static bool bytes_valid(std::span<std::uint8_t const> b) noexcept { return !b.empty() && b[0] != 255; }

static void canonical_contract()
{
    namespace contract = uwvm2::imported::ros::communication_contract;
    namespace wire = uwvm2::imported::ros::scene::wire;
    namespace schema = uwvm2::imported::ros::scene::schema_contract;
    auto selected = profile("selected", 1);
    std::array<std::uint8_t, 2048> storage;
    std::array<std::uint8_t, 1> body{3};
    for(auto const& descriptor: contract::operations)
    {
        wire::writer output{storage};
        auto key = [&] { return output.u64(4) && output.u64(1); };
        auto resource = [&] { return output.u64(4); };
        auto message = [&] { return contract::encode_message(output, {"test/Bytes", "scene-canonical", body}); };
        auto endpoint = [&](std::string_view role,std::size_t count)
        {
            if(!output.u64(2)||!output.u64(3)||!output.u64(count))return false;
            for(std::size_t i{};i<count;++i)if(!output.u64(10+i))return false;
            return contract::encode_profile(output, selected) && output.text("/scene/name") && output.text("test/Bytes") && output.text(role);
        };
        bool okay{};
        switch(descriptor.id)
        {
            case contract::operation::context_open: okay = contract::encode_profile(output, selected); break;
            case contract::operation::node_open: okay = output.u64(2) && resource() && contract::encode_profile(output, selected); break;
            case contract::operation::context_shutdown:
            case contract::operation::qos_liveliness_assert: okay = key() && resource(); break;
            case contract::operation::topic_open: okay = endpoint("publisher",1); break;
            case contract::operation::service_open: okay = endpoint("client",2); break;
            case contract::operation::topic_publish:
            case contract::operation::service_submit: okay = key() && resource() && message() && output.u64(99); break;
            case contract::operation::service_respond: okay = key() && resource() && output.u64(7) && message() && output.u64(99); break;
            case contract::operation::service_abandon: okay = resource() && output.u64(7); break;
            case contract::operation::qos_acknowledgements_wait: okay = resource() && output.u64(99); break;
            case contract::operation::type_describe: okay = output.text("test/Bytes") && output.boolean(false) && output.u64(8); break;
            default: okay = resource(); break;
        }
        assert(okay);
        auto declared = schema::find(descriptor.name);
        assert(declared);
        auto semantic=output.written();
        // Creation's full public carrier appends the separately decoded exact
        // key/deadline trailer; this helper checks its semantic prefix only.
        if(descriptor.id==contract::operation::node_open||descriptor.id==contract::operation::topic_open||descriptor.id==contract::operation::service_open)
            assert(output.u64(1)&&output.u64(99));
        assert(schema::validate(output.written(), declared->request, {2048, 100, 20, 10}));
        contract::request input;
        assert(contract::decode(descriptor.id, semantic, input));
        for(std::size_t i{}; i < semantic.size(); ++i) { assert(!contract::decode(descriptor.id, semantic.first(i), input)); }
        auto size = semantic.size();
        storage[size] = 0;
        assert(!contract::decode(descriptor.id, std::span<std::uint8_t const>{storage}.first(size + 1), input));
    }
    c::metadata metadata;
    metadata.source = 3;
    metadata.generation = 4;
    metadata.sequence = 5;
    metadata.source_nanoseconds = 10;
    metadata.received_nanoseconds = 11;
    wire::writer output{storage};
    assert(contract::encode_metadata(output, metadata));
    wire::reader input{output.written()};
    std::uint64_t value{};
    bool present{};
    assert(input.u64(value) && value == 3 && input.u64(value) && value == 4 && input.u64(value) && value == 5 && input.boolean(present) && !present &&
           input.boolean(present) && !present && input.boolean(present) && !present && input.u64(value) && !value && input.done());
    wire::writer mapped{storage};
    assert(contract::encode_metadata(mapped,
                                     metadata,
                                     {
                                         {8,  9 },
                                         {10, 11}
    }));
    assert(schema::validate(mapped.written(), schema::type::metadata, {2048, 100, 20, 10}));
    c::qos quality{
        1,
        1,
        2,
        1,
        10,
        {1, 2},
        {3, 4},
        {5, 6}
    };
    wire::writer qos_output{storage};
    assert(contract::encode_qos(qos_output, quality, quality));
    assert(schema::validate(qos_output.written(), schema::type::qos_info, {2048, 100, 20, 10}));
}

struct test_context final : c::native_context
{
    bool closed{};

    bool live() const noexcept override { return !closed; }

    c::error shutdown() noexcept override
    {
        closed = true;
        return c::error::ok;
    }
};

struct test_node final : c::native_node
{
#if defined(__linux__)
    ::pid_t process{::getpid()};

    ~test_node() override
    {
        if(process != ::getpid()) { ::_exit(77); }
    }
#endif
};

struct test_topic final : c::native_topic
{
    unsigned publications{}, assertions{};
    bool fail{};
    std::vector<std::uint8_t> bytes;
    c::qos quality;
    c::endpoint_identity evidence;
    c::endpoint_identity identity()const noexcept override{return evidence;}

    c::native_result publish(std::span<std::uint8_t const> body, std::uint64_t) noexcept override
    {
        ++publications;
        bytes.assign(body.begin(), body.end());
        return fail ? c::native_result{c::error::io, c::effect::unknown} : c::native_result{c::error::ok, c::effect::locally_accepted};
    }

    c::error inspect(c::qos& out, std::array<std::uint8_t, 32>& id, std::size_t& n) noexcept override
    {
        out = quality;
        id.fill(12);
        n = 16;
        return c::error::ok;
    }

    c::native_result assert_liveliness() noexcept override
    {
        ++assertions;
        return {c::error::ok, c::effect::locally_accepted};
    }

    c::error wait_acknowledged(std::uint64_t) noexcept override { return c::error::ok; }
};

struct test_service final : c::native_service
{
    unsigned calls{};
    bool reject{}, unknown{}, retired{};
    c::endpoint_identity evidence;
    std::array<std::optional<c::qos>,2> actual_channels;
    c::endpoint_identity identity()const noexcept override{return evidence;}
    std::array<std::optional<c::qos>,2> channels()const noexcept override{return actual_channels;}

    c::native_result submit(c::handle, std::span<std::uint8_t const>, std::uint64_t) noexcept override
    {
        if(reject) { return {c::error::again, c::effect::none}; }
        ++calls;
        return {unknown ? c::error::io : c::error::ok, unknown ? c::effect::unknown : c::effect::locally_accepted};
    }

    c::native_result respond(c::handle id, std::span<std::uint8_t const> b, std::uint64_t d) noexcept override { return submit(id, b, d); }

    c::error take(void*, c::service_sink) noexcept override { return c::error::again; }

    c::error available(bool& out) noexcept override
    {
        out = true;
        return c::error::ok;
    }

    c::native_result abandon(c::handle) noexcept override { return {c::error::ok, c::effect::unknown}; }

    bool occupied() noexcept override { return false; }

    void retire() noexcept override { retired = true; }
};

struct test_backend final : c::backend
{
    unsigned creations{};
    std::shared_ptr<test_topic> publisher;
    std::shared_ptr<test_service> service;
    std::shared_ptr<c::receive_queue> queue;

    c::error create_context(c::context_plan const&, std::shared_ptr<c::native_context>& out) noexcept override
    {
        ++creations;
        out = std::make_shared<test_context>();
        return c::error::ok;
    }

    c::error create_node(c::node_plan const&, std::shared_ptr<c::native_context> const&, std::shared_ptr<c::native_node>& out) noexcept override
    {
        ++creations;
        out = std::make_shared<test_node>();
        return c::error::ok;
    }

    c::error create_topic(c::topic_plan const& p,
                          c::type_plan const&,
                          std::shared_ptr<c::native_node> const&,
                          c::handle,
                          std::shared_ptr<c::receive_queue> const& q,
                          std::shared_ptr<c::native_topic>& out) noexcept override
    {
        ++creations;
        auto t = std::make_shared<test_topic>();
        t->quality = p.quality;
        out = t;
        if(p.direction == c::role::publisher) { publisher = t; }
        else
        {
            queue = q;
        }
        return c::error::ok;
    }

    c::error create_service(c::service_plan const&,
                            c::type_plan const&,
                            c::type_plan const&,
                            std::shared_ptr<c::native_node> const&,
                            c::handle,
                            std::shared_ptr<c::native_service>& out) noexcept override
    {
        ++creations;
        service = std::make_shared<test_service>();
        out = service;
        return c::error::ok;
    }
};

static void queue_checks()
{
    c::receive_queue queue(2, 8, c::overflow::drop_oldest);
    std::array<std::uint8_t, 3> bytes{1, 2, 3};
    c::metadata info;
    info.source = 1;
    info.generation = 2;
    assert(queue.push(info, bytes) == c::error::ok);
    assert(queue.take(
               [&](auto const& first)
               {
                   assert(first.information.sequence == 1);
                   assert(queue.take([](auto const&) { return true; }) == c::error::again);
                   assert(queue.push(info, bytes) == c::error::ok);
                   assert(queue.push(info, bytes) == c::error::again);
                   return false;
               }) == c::error::io);
    assert(queue.take(
               [](auto const& first)
               {
                   assert(first.information.sequence == 1);
                   throw 42;
                   return true;
               }) == c::error::io);
    assert(queue.take(
               [](auto const& first)
               {
                   assert(first.information.sequence == 1);
                   return true;
               }) == c::error::ok);
    assert(queue.push(info, bytes) == c::error::ok);
    assert(queue.push(info, bytes) == c::error::ok);
    assert(queue.take(
               [](auto const& current)
               {
                   assert(current.information.sequence == 3 && current.information.loss_before == 1);
                   return true;
               }) == c::error::ok);
    assert(queue.take(
               [](auto const& current)
               {
                   assert(current.information.sequence == 4 && current.information.loss_before == 2);
                   return true;
               }) == c::error::ok);
    assert(queue.take([](auto const&) { return true; }) == c::error::again);
    assert(queue.push(info, bytes) == c::error::ok);
    assert(queue.take(
               [&](auto const&)
               {
                   queue.retire();
                   return true;
               }) == c::error::cancelled);
    assert(queue.push(info, bytes) == c::error::cancelled);
}

static void provider_checks()
{
    auto backend = std::make_shared<test_backend>();
    c::registry registry{
        backend,
        {1, 1, 2, 1, 16, 8}
    };
    auto ctx = profile("context", 1), node = profile("node", 2), type = profile("type", 3), pub = profile("pub", 4), sub = profile("sub", 5);
    c::owner a{1, 1, 1}, b{1, 2, 1}, p{1, 3, 1}, s{1, 4, 1}, t{1, 5, 1};
    assert(registry.configure(c::context_plan{{}, ctx, 0, {}}) == c::error::invalid);
    assert(registry.configure(c::context_plan{a, ctx, 4, {}, c::transport::unauthenticated_loopback}) == c::error::ok);
    assert(registry.configure(c::node_plan{b, node, ctx, "worker", "/scene", 1}) == c::error::ok);
    assert(registry.configure(c::type_plan{type, "test/Bytes", "canonical", {3}, &bytes_valid, t}) == c::error::ok);
    c::qos qos{
        1,
        1,
        2,
        1,
        2,
        {0, 0},
        {0, 0},
        {0, 0}
    };
    assert(registry.configure(c::topic_plan{p, pub, node, type, "/scene/out", c::role::publisher, qos, 8, 0, 0, c::overflow::reject, 10000000000ULL}) ==
           c::error::ok);
    assert(registry.configure(c::topic_plan{s, sub, node, type, "/scene/in", c::role::subscription, qos, 8, 2, 16, c::overflow::reject, 10000000000ULL}) ==
           c::error::ok);
    c::handle context{}, native_node{}, publisher{}, subscriber{}, unmodified = 123;
    assert(registry.open_context({2, 1, 1}, ctx, unmodified) == c::error::denied && unmodified == 123 && backend->creations == 0);
    assert(registry.open_context(a, ctx, context) == c::error::ok);
    assert(registry.configure(c::context_plan{a, ctx, 0, {}}) == c::error::denied);
    assert(registry.open_node(b, {1, 1, 2}, context, node, unmodified) == c::error::denied && backend->creations == 1);
    assert(registry.open_node(b, a, context, node, native_node) == c::error::ok);
    c::node_lease observer, extra;
    assert(registry.attach_node(b, native_node, node, observer) == c::error::ok && observer.live() && observer.get());
    assert(registry.attach_node(b, native_node, node, extra) == c::error::again && !extra);
    assert(registry.close(b, native_node) == c::error::busy);
#if defined(__linux__)
    auto child = ::fork();
    assert(child >= 0);
    if(!child)
    {
        assert(!observer.live() && !observer.get());
        observer.reset();
        std::destroy_at(std::addressof(registry));
        ::_exit(0);
    }
    int child_status{};
    assert(::waitpid(child, &child_status, 0) == child && WIFEXITED(child_status) && !WEXITSTATUS(child_status));
    assert(observer.live() && registry.close(b, native_node) == c::error::busy);
#endif
    assert(registry.open_topic(p, b, native_node, pub, "/scene/wrong", "test/Bytes", c::role::publisher, unmodified) == c::error::denied &&
           backend->creations == 2);
    assert(registry.open_topic(p, b, native_node, pub, "/scene/out", "test/Bytes", c::role::publisher, publisher) == c::error::ok);
    assert(registry.open_topic(s, b, native_node, sub, "/scene/in", "test/Bytes", c::role::subscription, subscriber) == c::error::ok);
    c::type_info description;
    assert(registry.describe_type({}, "test/Bytes", description) == c::error::denied);
    assert(registry.describe_type(t, "test/Bytes", description) == c::error::ok && std::ranges::equal(description.descriptor, std::array<std::uint8_t, 1>{3}));
    std::array<std::uint8_t, 3> data{1, 2, 3};
    c::receipt first{}, retry{};
    auto deadline = c::now() + 1000000000;
    assert(registry.publish({1, 3, 2}, publisher, {publisher, 1}, "test/Bytes", "canonical", data, deadline, first) == c::error::denied);
    assert(registry.publish(p, publisher, {publisher, 1}, "test/Bytes", "canonical", data, c::now() - 1, first) == c::error::timeout);
    assert(registry.publish(p, publisher, {publisher, 1}, "test/Bytes", "canonical", data, deadline, first) == c::error::ok &&
           backend->publisher->publications == 1);
    assert(registry.publish(p, publisher, {publisher, 1}, "test/Bytes", "canonical", data, deadline, retry) == c::error::ok && retry.request == first.request &&
           backend->publisher->publications == 1);
    assert(registry.assert_liveliness(p, publisher, {publisher, 1}, retry) == c::error::stale);
    data[1] = 8;
    assert(registry.publish(p, publisher, {publisher, 1}, "test/Bytes", "canonical", data, deadline, retry) == c::error::stale);
    assert(registry.assert_liveliness(p, publisher, {publisher, 2}, retry) == c::error::ok && backend->publisher->assertions == 1);
    assert(registry.assert_liveliness(p, publisher, {publisher, 2}, retry) == c::error::ok && backend->publisher->assertions == 1);
    assert(registry.wait_acknowledged(p, publisher, deadline, retry) == c::error::ok && retry.observed == c::effect::observed_remote);
    backend->publisher->fail = true;
    assert(registry.publish(p, publisher, {publisher, 3}, "test/Bytes", "canonical", data, deadline, retry) == c::error::ok &&
           retry.observed == c::effect::unknown);
    assert(registry.publish(p, publisher, {publisher, 3}, "test/Bytes", "canonical", data, deadline, retry) == c::error::ok &&
           backend->publisher->publications == 2);
    assert(registry.assert_liveliness(p, publisher, {publisher, 4}, retry) == c::error::busy);
    assert(registry.close(p, publisher) == c::error::busy);
    c::metadata information;
    information.source = subscriber;
    information.generation = 1;
    assert(backend->queue->push(information, data) == c::error::ok);
    int consumer_instance{};c::delivery_consumer consumer{&consumer_instance,1,91};c::delivery_reference reserved;
    c::delivery_receipt settled;unsigned delivered{};std::uint64_t expected_sequence{1};
    auto borrow=[&](c::delivery_reference reference,std::uint64_t,c::delivery const& record)
    {reserved=reference;++delivered;assert(record.information.source==subscriber&&record.information.sequence==expected_sequence);return false;};
    assert(registry.reserve_topic(p,subscriber,consumer,deadline,borrow)==c::error::denied&&!delivered);
    assert(registry.reserve_topic(s,subscriber,consumer,deadline,borrow)==c::error::io&&delivered==1);
    assert(registry.close(s,subscriber)==c::error::busy);
    assert(registry.take(s,subscriber,[](auto const&){assert(false);return true;})==c::error::again);
    assert(registry.settle_topic(p,subscriber,consumer,reserved,{subscriber,1},false,settled)==c::error::denied);
    assert(registry.settle_topic(s,subscriber,consumer,reserved,{subscriber,1},false,settled)==c::error::ok);
    assert(registry.take(p, subscriber, [](auto const&) { return true; }) == c::error::denied);
    assert(registry.take(s, subscriber, [](auto const&) { return false; }) == c::error::io);
    assert(registry.take(s,
                         subscriber,
                         [](auto const& d)
                         {
                             assert(d.information.sequence == 1 && d.body[1] == 8);
                             return true;
                         }) == c::error::ok);
    assert(registry.close(b, native_node) == c::error::busy);
    assert(registry.close(s, subscriber) == c::error::ok);
    auto old_subscriber = subscriber;
    assert(registry.open_topic(s, b, native_node, sub, "/scene/in", "test/Bytes", c::role::subscription, subscriber) == c::error::ok &&
           subscriber != old_subscriber);
    assert(backend->queue->push(information, data) == c::error::stale);
    information.source=subscriber;expected_sequence=2;assert(backend->queue->push(information,data)==c::error::ok);
    assert(registry.reserve_topic(s,subscriber,consumer,deadline,borrow)==c::error::io);
    assert(registry.shutdown(a, context, {context, 1}, first) == c::error::ok);
    assert(!observer.live() && !observer.get());
    observer.reset();
    assert(registry.shutdown(a, context, {context, 1}, retry) == c::error::ok && retry.request == first.request);
    assert(registry.shutdown(a, context, {context, 2}, retry) == c::error::stale);
    assert(registry.publish(p, publisher, {publisher, 4}, "test/Bytes", "canonical", data, deadline, retry) == c::error::denied);
    assert(registry.take(s, subscriber, [](auto const&) { return true; }) == c::error::denied);
    assert(registry.settle_topic(s,subscriber,consumer,reserved,{subscriber,1},true,settled)==c::error::denied);
    assert(registry.settle_topic(s,subscriber,consumer,reserved,{subscriber,1},false,settled)==c::error::ok);
    registry.retire();
    assert(registry.open_context(a, ctx, unmodified) == c::error::cancelled);
}

static void service_checks()
{
    auto backend = std::make_shared<test_backend>();
    c::registry registry{
        backend,
        {1, 1, 0, 2, 32, 8, 1}
    };
    c::owner a{1, 1, 1}, b{1, 2, 1}, c_owner{1, 3, 1}, schema_owner{1, 4, 1};
    auto ctx = profile("context", 1), node = profile("node", 2), request = profile("request", 3), response = profile("response", 4),
         service = profile("service", 5);
    assert(registry.configure(c::context_plan{a, ctx, 4, {}, c::transport::unauthenticated_loopback}) == c::error::ok);
    assert(registry.configure(c::node_plan{b, node, ctx, "worker", "/scene", 1}) == c::error::ok);
    assert(registry.configure(c::type_plan{request, "test/Request", "canonical", {1}, &bytes_valid, schema_owner}) == c::error::ok);
    assert(registry.configure(c::type_plan{response, "test/Response", "canonical", {2}, &bytes_valid, schema_owner}) == c::error::ok);
    c::qos q{1, 1, 2, 1, 2, {}, {}, {}};
    assert(registry.configure(c::service_plan{c_owner,
                                              service,
                                              node,
                                              request,
                                              response,
                                              "/scene/service",
                                              "test/Service",
                                              c::service_role::client,
                                              q,
                                              2,
                                              8,
                                              8,
                                              16,
                                              10000000000ULL}) == c::error::ok);
    c::handle context{}, n{}, client{}, untouched{999};
    assert(registry.open_context(a, ctx, context) == c::error::ok && registry.open_node(b, a, context, node, n) == c::error::ok);
    assert(registry.open_service(c_owner, b, n, service, "/scene/wrong", "test/Service", c::service_role::client, untouched) == c::error::denied &&
           untouched == 999);
    assert(registry.open_service(c_owner, b, n, service, "/scene/service", "test/Service", c::service_role::server, untouched) == c::error::denied);
    assert(registry.open_service(c_owner, b, n, service, "/scene/service", "test/Service", c::service_role::client, client) == c::error::ok);
    assert(registry.close(b, n) == c::error::busy);
    bool ready{};
    assert(registry.service_available(c_owner, client, ready) == c::error::ok && ready);
    std::array<std::uint8_t, 1> old_body{1}, new_body{2}, invalid{255};
    c::receipt first{}, retry{};
    auto deadline = c::now() + 5000000000ULL;
    auto invoke = [&](c::request_key key, std::span<std::uint8_t const> bytes, c::receipt& output)
    { return registry.service_mutate(c_owner, client, key, 0, "test/Request", "canonical", bytes, deadline, false, output); };
    assert(invoke({client, 1}, invalid, first) == c::error::invalid && backend->service->calls == 0);
    assert(invoke({client, 1}, old_body, first) == c::error::ok && backend->service->calls == 1);
    backend->service->reject = true;
    assert(invoke({client, 2}, new_body, retry) == c::error::again);
    assert(invoke({client, 1}, old_body, retry) == c::error::ok && retry.request == first.request && backend->service->calls == 1);
    assert(invoke({client, 1}, new_body, retry) == c::error::stale);
    backend->service->reject = false;
    backend->service->unknown = true;
    assert(invoke({client, 2}, new_body, retry) == c::error::ok && retry.observed == c::effect::unknown && backend->service->calls == 2);
    assert(invoke({client, 2}, new_body, retry) == c::error::ok && backend->service->calls == 2);
    assert(invoke({client, 3}, old_body, retry) == c::error::busy && registry.close(c_owner, client) == c::error::busy);
    registry.revoke(schema_owner);
    assert(backend->service->retired);
    assert(invoke({client, 2}, new_body, retry) == c::error::denied);
    assert(registry.service_available(c_owner, client, ready) == c::error::denied);
    registry.retire();
}

int main()
{
    canonical_contract();
    queue_checks();
    provider_checks();
    service_checks();
    std::cout << "passed communication capacity, authority, replay, delivery and shutdown groups\n";
    return 0;
}
