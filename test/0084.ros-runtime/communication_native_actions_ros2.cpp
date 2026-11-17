#include <uwvm2/imported/ros/scene/native/communication_actions_ros2.h>
#include <rclcpp_action/rclcpp_action.hpp>
#include <cassert>
#include <iostream>
#include <thread>
namespace c = uwvm2::imported::ros::scene::communication;
namespace a = c::actions;
using Action = tf2_msgs::action::LookupTransform;

static c::profile profile(char const* name)
{
    c::profile p;
    p.identity = name;
    p.digest.fill(1);
    return p;
}

static a::plan plan(a::role role)
{
    a::plan p;
    p.authority = {1, static_cast<std::uint64_t>(role == a::role::server ? 10 : 11), 1};
    p.selected = profile(role == a::role::server ? "server" : "client");
    p.resolved_name = role == a::role::server ? "/scene_action/managed" : "/scene_action/upstream";
    p.action_type = "tf2_msgs/action/LookupTransform";
    p.direction = role;
    p.goal_quality = p.result_quality = p.cancel_quality = p.feedback_quality = {
        1,
        1,
        2,
        1,
        10,
        {9223372036ULL, 854775807},
        {9223372036ULL, 854775807},
        {9223372036ULL, 854775807}
    };
    p.status_quality = p.feedback_quality;
    p.status_quality.depth = 1;
    p.status_quality.durability = 1;
    p.goal = {
        profile("goal"),
        "tf2_msgs/action/LookupTransform_Goal",
        "scene-canonical",
        {},
        c::ros2::lookup_goal_codec::validate,
        {1, 20, 1}
    };
    p.feedback = {
        profile("feedback"),
        "tf2_msgs/action/LookupTransform_Feedback",
        "scene-canonical",
        {},
        c::ros2::lookup_feedback_codec::validate,
        {1, 21, 1}
    };
    p.result = {
        profile("result"),
        "tf2_msgs/action/LookupTransform_Result",
        "scene-canonical",
        {},
        c::ros2::lookup_result_codec::validate,
        {1, 22, 1}
    };
    p.max_goals = 2;
    p.max_cancels_per_goal = 2;
    p.feedback_slots = 2;
    p.max_goal_bytes = 1024;
    p.max_result_bytes = 1024;
    p.max_feedback_bytes = 0;
    p.reserved_bytes = 65536;
    p.max_deadline_ns = 30000000000ULL;
    p.retention_ns = 10000000000ULL;
    p.transition_mask = 510;
    return p;
}

struct capture
{
    a::delivery value;
    std::vector<std::uint8_t> body;
    bool accept{};
};

static bool deliver(void* p, a::delivery const& value)
{
    auto& capture = *static_cast<struct capture*>(p);
    capture.value = value;
    capture.body.assign(value.body.begin(), value.body.end());
    return capture.accept;
}

int main()
{
    auto policy = c::ros2::local_network::install(true);
    assert(policy && policy->valid());
    auto backend = std::make_shared<c::ros2::provider>(c::ros2::builtin_types(), policy, c::ros2::builtin_services());
    c::registry registry{
        backend,
        {1, 1, 0, 0, 0, 0, 0}
    };
    c::owner context_owner{1, 1, 1}, node_owner{1, 2, 1};
    auto context_profile = profile("context"), node_profile = profile("node");
    assert(registry.configure(c::context_plan{context_owner, context_profile, 44, "", c::transport::unauthenticated_loopback}) == c::error::ok);
    assert(registry.configure(c::node_plan{node_owner, node_profile, context_profile, "managed", "/scene_action", 2}) == c::error::ok);
    c::handle context{}, node{};
    assert(registry.open_context(context_owner, context_profile, context) == c::error::ok);
    assert(registry.open_node(node_owner, context_owner, context, node_profile, node) == c::error::ok);
    c::node_lease server_lease, client_lease;
    assert(registry.attach_node(node_owner, node, node_profile, server_lease) == c::error::ok);
    assert(registry.attach_node(node_owner, node, node_profile, client_lease) == c::error::ok);
    auto server_plan = plan(a::role::server), client_plan = plan(a::role::client);
    std::shared_ptr<a::endpoint> server, client;
    assert(c::ros2::open_lookup_action(server_plan, std::move(server_lease), 10, server) == c::error::ok);
    assert(c::ros2::open_lookup_action(client_plan, std::move(client_lease), 11, client) == c::error::ok);
    auto peer_context = std::make_shared<rclcpp::Context>();
    rclcpp::InitOptions init;
    init.set_domain_id(44);
    init.auto_initialize_logging(false);
    init.shutdown_on_signal = false;
    char const* argv[]{"native-action-peer"};
    peer_context->init(1, argv, init);
    rclcpp::NodeOptions options;
    options.context(peer_context).use_global_arguments(false).enable_rosout(false).start_parameter_services(false).start_parameter_event_publisher(false);
    auto peer = std::make_shared<rclcpp::Node>("peer", "/scene_action", options);
    rclcpp::ExecutorOptions execution;
    execution.context = peer_context;
    rclcpp::executors::SingleThreadedExecutor executor{execution};
    executor.add_node(peer);
    auto wait = [&](auto&& ready)
    {
        auto until = c::now() + 15000000000ULL;
        while(!ready())
        {
            assert(c::now() < until);
            executor.spin_some();
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    };
    auto native_client = rclcpp_action::create_client<Action>(peer, "/scene_action/managed");
    std::shared_ptr<rclcpp_action::ServerGoalHandle<Action>> native_goal;
    unsigned goals{}, cancels{}, feedbacks{};
    auto native_server = rclcpp_action::create_server<Action>(
        peer,
        "/scene_action/upstream",
        [&](rclcpp_action::GoalUUID const&, std::shared_ptr<Action::Goal const> incoming)
        {
            assert(incoming->target_frame == "map" && incoming->source_frame == "base");
            ++goals;
            return rclcpp_action::GoalResponse::ACCEPT_AND_DEFER;
        },
        [&](std::shared_ptr<rclcpp_action::ServerGoalHandle<Action>>)
        {
            ++cancels;
            return cancels == 1 ? rclcpp_action::CancelResponse::REJECT : rclcpp_action::CancelResponse::ACCEPT;
        },
        [&](std::shared_ptr<rclcpp_action::ServerGoalHandle<Action>> accepted) { native_goal = std::move(accepted); });
    wait([&] { return native_client->action_server_is_ready(); });
    Action::Goal goal;
    goal.target_frame = "map";
    goal.source_frame = "base";
    goal.timeout.sec = 1;
    typename rclcpp_action::Client<Action>::SendGoalOptions send;
    send.feedback_callback = [&](auto, std::shared_ptr<Action::Feedback const>) { ++feedbacks; };
    auto pending = native_client->async_send_goal(goal, send);
    capture incoming;
    auto receive = [&](a::endpoint& endpoint, c::owner owner, a::event_kind kind, bool accept)
    {
        incoming.accept = accept;
        wait(
            [&]
            {
                auto status = endpoint.take(owner, &incoming, deliver);
                assert(status == c::error::ok || status == c::error::again || status == c::error::io);
                return (status == c::error::ok || status == c::error::io) && incoming.value.kind == kind;
            });
    };
    receive(*server, server_plan.authority, a::event_kind::proposal, false);
    auto managed_goal = incoming.value.goal, proposal = incoming.value.request;
    Action::Goal decoded_goal;
    assert(c::ros2::lookup_goal_codec::decode(incoming.body, decoded_goal) && decoded_goal.target_frame == "map");
    c::receipt receipt, retry;
    auto deadline = c::now() + 20000000000ULL;
    a::response response{proposal, managed_goal, a::transition::accept, {}, false};
    assert(server->respond(server_plan.authority, {10, 1}, response, deadline, receipt) == c::error::denied);
    assert(pending.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready);
    receive(*server, server_plan.authority, a::event_kind::proposal, true);
    assert(server->respond(server_plan.authority, {10, 1}, response, deadline, receipt) == c::error::ok);
    assert(server->respond(server_plan.authority, {10, 1}, response, deadline, retry) == c::error::ok);
    wait([&] { return pending.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready; });
    auto managed_handle = pending.get();
    assert(managed_handle);
    auto result_future = native_client->async_get_result(managed_handle);
    response.change = a::transition::execute;
    assert(server->respond(server_plan.authority, {10, 2}, response, deadline, receipt) == c::error::ok);
    response.change = a::transition::feedback;
    assert(server->respond(server_plan.authority, {10, 3}, response, deadline, receipt) == c::error::ok);
    wait([&] { return feedbacks == 1; });
    auto cancel_first = native_client->async_cancel_goal(managed_handle);
    receive(*server, server_plan.authority, a::event_kind::cancel_request, true);
    auto first_cancel = incoming.value.request;
    response = {first_cancel, managed_goal, a::transition::cancel_response, {}, false};
    assert(server->respond(server_plan.authority, {10, 4}, response, deadline, receipt) == c::error::ok);
    wait([&] { return cancel_first.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready; });
    assert(cancel_first.get()->return_code == action_msgs::srv::CancelGoal::Response::ERROR_REJECTED);
    auto cancel_second = native_client->async_cancel_goal(managed_handle);
    receive(*server, server_plan.authority, a::event_kind::cancel_request, true);
    assert(incoming.value.request != first_cancel);
    response = {incoming.value.request, managed_goal, a::transition::cancel_response, {}, true};
    assert(server->respond(server_plan.authority, {10, 5}, response, deadline, receipt) == c::error::ok);
    wait([&] { return cancel_second.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready; });
    assert(cancel_second.get()->return_code == action_msgs::srv::CancelGoal::Response::ERROR_NONE);
    Action::Result result;
    result.transform.header.frame_id = "map";
    result.transform.child_frame_id = "base";
    result.transform.transform.rotation.w = 1;
    result.transform.transform.translation.x = 4.25;
    result.error.error_string = "cancelled by scene";
    std::array<std::uint8_t, 1024> encoded_result;
    std::size_t result_size{};
    assert(c::ros2::lookup_result_codec::encode(result, encoded_result, result_size));
    response = {
        proposal,
        managed_goal,
        a::transition::cancelled,
        {encoded_result.data(), result_size},
        false
    };
    assert(server->respond(server_plan.authority, {10, 6}, response, deadline, receipt) == c::error::ok);
    wait([&] { return result_future.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready; });
    auto finished = result_future.get();
    assert(finished.code == rclcpp_action::ResultCode::CANCELED && finished.result->transform.transform.translation.x == 4.25);
    assert(server->close(server_plan.authority) == c::error::ok);

    std::array<std::uint8_t, 1024> encoded_goal;
    std::size_t goal_size{};
    assert(c::ros2::lookup_goal_codec::encode(goal, encoded_goal, goal_size));
    a::uuid uuid{};
    uuid[0] = 42;
    deadline = c::now() + 20000000000ULL;
    assert(client->submit(client_plan.authority, {11, 1}, uuid, {encoded_goal.data(), goal_size}, deadline, receipt) == c::error::ok);
    auto owned_goal = receipt.request;
    assert(client->submit(client_plan.authority, {11, 1}, uuid, {encoded_goal.data(), goal_size}, deadline, retry) == c::error::ok);
    wait([&] { return bool(native_goal); });
    assert(goals == 1 && native_goal->get_goal_id() == uuid);
    native_goal->execute();
    native_goal->publish_feedback(std::make_shared<Action::Feedback>());
    receive(*client, client_plan.authority, a::event_kind::acceptance, true);
    assert(client->cancel(client_plan.authority, {11, 2}, owned_goal, deadline, receipt) == c::error::ok);
    receive(*client, client_plan.authority, a::event_kind::cancel_response, true);
    assert(incoming.value.observed == a::state::accepted || incoming.value.observed == a::state::executing);
    assert(client->cancel(client_plan.authority, {11, 3}, owned_goal, deadline, receipt) == c::error::ok);
    receive(*client, client_plan.authority, a::event_kind::cancel_response, true);
    assert(incoming.value.observed == a::state::cancelling && cancels == 2 && native_goal->is_canceling());
    result.error.error_string = "cancelled upstream";
    native_goal->canceled(std::make_shared<Action::Result>(result));
    receive(*client, client_plan.authority, a::event_kind::result, false);
    auto result_sequence = incoming.value.information.sequence;
    receive(*client, client_plan.authority, a::event_kind::result, true);
    assert(incoming.value.information.sequence == result_sequence && incoming.value.observed == a::state::cancelled);
    Action::Result decoded_result;
    assert(c::ros2::lookup_result_codec::decode(incoming.body, decoded_result) && decoded_result.error.error_string == "cancelled upstream");
    assert(client->close(client_plan.authority) == c::error::ok);
    server.reset();
    client.reset();
    assert(registry.close(node_owner, node) == c::error::ok);
    assert(registry.shutdown(context_owner, context) == c::error::ok);
    executor.remove_node(peer);
    peer_context->shutdown("action test complete");
    std::cout << "passed actual ROS2 action client/server deferred accept, execute, feedback, repeated cancel, terminal result, UUID and copy recovery\n";
}
