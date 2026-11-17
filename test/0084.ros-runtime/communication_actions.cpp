#include <uwvm2/imported/ros/scene/communication_actions.h>
#include <uwvm2/imported/ros/scene/communication_action_contract.h>
#include <uwvm2/imported/ros/scene/schema_contract.h>
#include <cassert>
#include <iostream>
namespace c = uwvm2::imported::ros::scene::communication;
namespace a = c::actions;

static bool valid(std::span<std::uint8_t const> b) noexcept { return b.size() == 1 && b[0] < 128; }

static c::profile profile(char const* n)
{
    c::profile p;
    p.identity = n;
    p.digest.fill(1);
    return p;
}

struct backend : a::native
{
    bool active{true}, unknown{}, reject{};
    unsigned calls{};

    bool live() const noexcept override { return active; }

    c::native_result effect() noexcept
    {
        if(reject) { return {c::error::again, c::effect::none}; }
        ++calls;
        return {unknown ? c::error::io : c::error::ok, unknown ? c::effect::unknown : c::effect::locally_accepted};
    }

    c::native_result submit(c::handle, a::uuid const&, std::span<std::uint8_t const>, std::uint64_t) noexcept override { return effect(); }

    c::native_result respond(a::response const&, a::uuid const&, a::state, std::uint64_t) noexcept override { return effect(); }

    c::native_result cancel(c::handle, c::handle, a::uuid const&, std::uint64_t) noexcept override { return effect(); }

    void retire() noexcept override { active = false; }
};

struct capture
{
    bool accept{};
    a::delivery record;
    uwvm2::utils::container::std_vector<std::uint8_t> body;
};

static bool take(void* p, a::delivery const& d)
{
    auto& c = *static_cast<capture*>(p);
    c.record = d;
    c.body.assign(d.body.begin(), d.body.end());
    return c.accept;
}

static a::plan plan(a::role role)
{
    a::plan p;
    p.authority = {1, 1, 1};
    p.selected = profile("action");
    p.resolved_name = "/scene/action";
    p.action_type = "test/action/One";
    p.direction = role;
    p.goal_quality = p.result_quality = p.cancel_quality = p.feedback_quality = p.status_quality = {
        1,
        1,
        2,
        1,
        10,
        {0, 0},
        {0, 0},
        {0, 0}
    };
    p.goal = {
        profile("goal"),
        "test/action/One_Goal",
        "scene-canonical",
        {},
        valid,
        {1, 2, 1}
    };
    p.feedback = {
        profile("feedback"),
        "test/action/One_Feedback",
        "scene-canonical",
        {},
        valid,
        {1, 3, 1}
    };
    p.result = {
        profile("result"),
        "test/action/One_Result",
        "scene-canonical",
        {},
        valid,
        {1, 4, 1}
    };
    p.max_goals = 2;
    p.max_cancels_per_goal = 2;
    p.feedback_slots = 1;
    p.max_goal_bytes = p.max_feedback_bytes = p.max_result_bytes = 1;
    p.reserved_bytes = 2048;
    p.transition_mask = 510;
    p.max_deadline_ns = 10000000000ULL;
    p.retention_ns = 1000000000ULL;
    return p;
}

static void retained_ingress_delivery()
{
    auto native=uwvm2::utils::container::make_shared<backend>();a::endpoint endpoint;
    assert(endpoint.configure(plan(a::role::client),81,native)==c::error::ok);
    a::uuid uuid{31};std::array<std::uint8_t,1> body{7};c::receipt receipt;
    assert(endpoint.submit({1,1,1},{81,1},uuid,body,c::now()+1000000000,receipt)==c::error::ok);
    auto goal=receipt.request;
    assert(endpoint.observe(goal,a::event_kind::acceptance,a::state::accepted)==c::error::ok);
    assert(endpoint.observe(goal,a::event_kind::status,a::state::accepted,body)==c::error::invalid);
    assert(endpoint.observe(goal,a::event_kind::feedback,a::state::accepted,body,0,a::ingress_ownership::retained)==c::error::ok);
    for(unsigned retry{};retry<3;++retry)
        assert(endpoint.observe(goal,a::event_kind::feedback,a::state::accepted,body,0,a::ingress_ownership::retained)==c::error::again);
    capture copied{true,{},{}};
    assert(endpoint.take({1,1,1},&copied,take)==c::error::ok&&copied.record.kind==a::event_kind::acceptance);
    assert(endpoint.take({1,1,1},&copied,take)==c::error::ok&&copied.record.kind==a::event_kind::feedback&&!copied.record.information.loss_before);
    assert(endpoint.observe(goal,a::event_kind::feedback,a::state::accepted,body,0,a::ingress_ownership::retained)==c::error::ok);
    assert(endpoint.take({1,1,1},&copied,take)==c::error::ok&&!copied.record.information.loss_before);
    assert(endpoint.observe(goal,a::event_kind::result,a::state::succeeded,body,0,a::ingress_ownership::retained)==c::error::ok);
    a::goal_info info;assert(endpoint.describe({1,1,1},goal,info)==c::error::ok&&info.observed==a::state::succeeded&&!info.uncertain);
    assert(endpoint.take({1,1,1},&copied,take)==c::error::ok&&copied.record.observed==a::state::succeeded);
    assert(endpoint.close({1,1,1})==c::error::ok);
    c::receive_queue queue{1,1,c::overflow::reject};c::metadata metadata;metadata.source=91;metadata.generation=71;
    assert(queue.push(metadata,body,true)==c::error::ok);
    for(unsigned retry{};retry<3;++retry)assert(queue.push(metadata,body,true)==c::error::again);
    assert(queue.take([](c::delivery const& item){return item.information.sequence==1&&!item.information.loss_before;})==c::error::ok);
    assert(queue.push(metadata,body,true)==c::error::ok);
    assert(queue.take([](c::delivery const& item){return item.information.sequence==2&&!item.information.loss_before;})==c::error::ok);
}

int main()
{
    retained_ingress_delivery();
    auto server = uwvm2::utils::container::make_shared<backend>();
    a::endpoint endpoint;
    assert(endpoint.configure({}, 7, server) == c::error::invalid);
    assert(endpoint.configure(plan(a::role::server), 7, server) == c::error::ok);
    a::uuid uuid{};
    uuid[0] = 1;
    std::array<std::uint8_t, 1> bytes{3};
    c::handle goal{};
    assert(endpoint.offer(uuid, 22, bytes, goal) == c::error::ok && goal);
    c::handle unchanged = 777;
    assert(endpoint.offer(uuid, 23, bytes, unchanged) == c::error::stale && unchanged == 777);
    c::receipt receipt, retry;
    auto deadline = c::now() + 5000000000ULL;
    a::response response{22, goal, a::transition::accept, {}, false};
    assert(endpoint.respond({1, 1, 1}, {7, 1}, response, deadline, receipt) == c::error::denied && !server->calls);
    capture copied;
    assert(endpoint.take({1, 1, 1}, &copied, take) == c::error::io && copied.record.kind == a::event_kind::proposal);
    namespace contract = uwvm2::imported::ros::action_contract;
    namespace wire = uwvm2::imported::ros::scene::wire;
    namespace schema = uwvm2::imported::ros::scene::schema_contract;
    std::array<std::uint8_t, 2048> encoded;
    wire::writer event_output{encoded};
    auto event = copied.record;
    event.body = copied.body;
    assert(event.goal_uuid == uuid && contract::encode_delivery(event_output, event, plan(a::role::server)));
    assert(schema::validate(event_output.written(), schema::type::action_take__result, {2048, 100, 20, 10}));
    auto event_size = event_output.size();
    wire::writer short_event{std::span<std::uint8_t>{encoded}.first(event_size - 1)};
    assert(!contract::encode_delivery(short_event, event, plan(a::role::server)));
    wire::writer response_bytes{encoded};
    assert(response_bytes.u64(7) && response_bytes.u64(1) && response_bytes.u64(7) && response_bytes.u64(22) && response_bytes.u64(goal) &&
           response_bytes.text("accept") && response_bytes.boolean(false) && response_bytes.u64(0) && response_bytes.u64(deadline));
    assert(schema::validate(response_bytes.written(), schema::type::action_respond__request, {2048, 100, 20, 10}));
    contract::request decoded_request;
    a::response decoded_response;
    assert(contract::decode(contract::operation::respond, response_bytes.written(), decoded_request) &&
           contract::exact_response(decoded_request, plan(a::role::server), decoded_response) && decoded_response.request == 22 &&
           decoded_response.goal == goal);
    for(std::size_t i{}; i < response_bytes.size(); ++i)
    {
        assert(!contract::decode(contract::operation::respond, response_bytes.written().first(i), decoded_request));
    }
    assert(endpoint.respond({1, 1, 1}, {7, 1}, response, deadline, receipt) == c::error::denied);
    copied.accept = true;
    assert(endpoint.take({1, 1, 1}, &copied, take) == c::error::ok);
    assert(endpoint.respond({1, 1, 1}, {7, 1}, response, deadline, receipt) == c::error::ok && server->calls == 1);
    assert(endpoint.respond({1, 1, 1}, {7, 1}, response, deadline, retry) == c::error::ok && server->calls == 1);
    response.change = a::transition::cancelled;
    response.body = bytes;
    assert(endpoint.respond({1, 1, 1}, {7, 2}, response, deadline, retry) == c::error::denied);
    response.change = a::transition::execute;
    response.body = {};
    assert(endpoint.respond({1, 1, 1}, {7, 2}, response, deadline, retry) == c::error::ok);
    response.change = a::transition::feedback;
    response.body = bytes;
    assert(endpoint.respond({1, 1, 1}, {7, 3}, response, deadline, retry) == c::error::ok);
    assert(endpoint.offer_cancel(uuid, 33, unchanged) == c::error::ok && unchanged == goal);
    response = {33, goal, a::transition::cancel_response, {}, true};
    assert(endpoint.respond({1, 1, 1}, {7, 4}, response, deadline, retry) == c::error::denied);
    assert(endpoint.take({1, 1, 1}, &copied, take) == c::error::ok && copied.record.kind == a::event_kind::cancel_request);
    assert(endpoint.respond({1, 1, 1}, {7, 4}, response, deadline, retry) == c::error::ok);
    response = {22, goal, a::transition::cancelled, bytes, false};
    assert(endpoint.respond({1, 1, 1}, {7, 5}, response, deadline, retry) == c::error::ok);
    a::goal_info info;
    assert(endpoint.describe({1, 1, 1}, goal, info) == c::error::ok && info.observed == a::state::cancelled);
    a::endpoint_info endpoint_info;
    assert(endpoint.describe({1, 1, 1}, endpoint_info) == c::error::ok);
    wire::writer described{encoded};
    assert(contract::encode_state(described, endpoint_info, info));
    assert(schema::validate(described.written(), schema::type::action_describe__result, {2048, 100, 20, 10}));
    assert(endpoint.close({1, 1, 1}) == c::error::ok && !server->active);

    auto client = uwvm2::utils::container::make_shared<backend>();
    a::endpoint submitting;
    assert(submitting.configure(plan(a::role::client), 8, client) == c::error::ok);
    client->reject = true;
    assert(submitting.submit({1, 1, 1}, {8, 1}, uuid, bytes, deadline, receipt) == c::error::again && !client->calls);
    client->reject = false;
    assert(submitting.submit({1, 1, 1}, {8, 1}, uuid, bytes, deadline, receipt) == c::error::ok);
    goal = receipt.request;
    assert(submitting.submit({1, 1, 1}, {8, 1}, uuid, bytes, deadline, retry) == c::error::ok && client->calls == 1);
    assert(submitting.observe(goal, a::event_kind::acceptance, a::state::accepted) == c::error::ok);
    assert(submitting.observe(goal, a::event_kind::status, a::state::executing) == c::error::ok);
    assert(submitting.observe(goal, a::event_kind::feedback, a::state::executing, bytes) == c::error::ok);
    assert(submitting.observe(goal, a::event_kind::feedback, a::state::executing, bytes) == c::error::again);
    assert(submitting.cancel({1, 1, 1}, {8, 2}, goal, deadline, retry) == c::error::ok && client->calls == 2);
    auto cancel_request = retry.request;
    std::array<std::uint8_t, 8> rejected_cancel{1}, accepted_cancel{};
    assert(submitting.observe(goal, a::event_kind::cancel_response, a::state::executing, rejected_cancel, cancel_request) == c::error::ok);
    assert(submitting.observe(goal, a::event_kind::cancel_response, a::state::executing, rejected_cancel, cancel_request) == c::error::invalid);
    assert(submitting.cancel({1, 1, 1}, {8, 3}, goal, deadline, retry) == c::error::ok && retry.request != cancel_request);
    assert(submitting.observe(goal, a::event_kind::cancel_response, a::state::cancelling, accepted_cancel, retry.request) == c::error::ok);
    assert(submitting.observe(goal, a::event_kind::result, a::state::cancelled, bytes) == c::error::ok);
    unsigned count{};
    while(submitting.take({1, 1, 1}, &copied, take) == c::error::ok) { ++count; }
    assert(count == 6 && copied.record.information.loss_before == 1 && copied.record.kind == a::event_kind::result);
    assert(submitting.close({1, 1, 1}) == c::error::ok);
    auto uncertain = uwvm2::utils::container::make_shared<backend>();
    uncertain->unknown = true;
    a::endpoint debt;
    assert(debt.configure(plan(a::role::client), 9, uncertain) == c::error::ok);
    assert(debt.submit({1, 1, 1}, {9, 1}, uuid, bytes, deadline, receipt) == c::error::ok && receipt.observed == c::effect::unknown);
    assert(debt.close({1, 1, 1}) == c::error::busy);
    auto other = uuid;
    other[1] = 1;
    assert(debt.submit({1, 1, 1}, {9, 2}, other, bytes, deadline, retry) == c::error::busy);
    uncertain->unknown = false;
    assert(debt.cancel({1, 1, 1}, {9, 2}, receipt.request, deadline, retry) == c::error::ok);
    assert(debt.close({1, 1, 1}) == c::error::busy);
    std::cout << "passed action deferred decision, legal transition, critical reserve, exact replay, unknown outcome and cancel isolation groups\n";
    return 0;
}
