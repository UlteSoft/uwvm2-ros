#include <uwvm2/imported/ros/scene/native/communication_graph_ros2.h>
#include <uwvm2/imported/ros/scene/communication_events.h>
#include <uwvm2/imported/ros/scene/communication_graph_events.h>
#include <cassert>
#include <iostream>

namespace c = uwvm2::imported::ros::scene::communication;

static c::profile selected(char const* name, unsigned byte)
{
    c::profile p;
    p.identity = name;
    p.digest.fill(static_cast<std::uint8_t>(byte));
    return p;
}

static std::vector<std::uint8_t> encoded(std::string const& text)
{
    std::vector<std::uint8_t> b(8 + text.size());
    for(unsigned i{}; i != 8; ++i) { b[i] = static_cast<std::uint8_t>(static_cast<std::uint64_t>(text.size()) >> (8 * i)); }
    std::copy(text.begin(), text.end(), b.begin() + 8);
    return b;
}

int main()
{
    auto policy = c::ros2::local_network::install(true);
    assert(policy && policy->valid());
    auto native = std::make_shared<c::ros2::provider>(c::ros2::builtin_types(), policy, c::ros2::builtin_services());
    auto retained_registry = std::make_shared<c::registry>(native, c::limits{1, 1, 2, 3, 8192, 1024, 2});
    auto& registry = *retained_registry;
    c::owner context_owner{1, 1, 1}, node_owner{1, 2, 1}, publish_owner{1, 3, 1}, take_owner{1, 4, 1};
    auto context = selected("context", 1), node = selected("node", 2), type = selected("string", 3), pub = selected("publisher", 4),
         sub = selected("subscription", 5);
    assert(registry.configure(c::context_plan{context_owner, context, 43, "", c::transport::unauthenticated_loopback}) == c::error::ok);
    assert(registry.configure(c::node_plan{node_owner, node, context, "scene_native", "/scene_qualification", 1}) == c::error::ok);
    assert(registry.configure(c::type_plan{
               type,
               "std_msgs/msg/String",
               "scene-canonical",
               {1},
               &c::ros2::string_codec::validate,
               {1, 5, 1}
    }) == c::error::ok);
    c::qos quality{
        1,
        1,
        2,
        1,
        4,
        {9223372036ULL, 854775807},
        {9223372036ULL, 854775807},
        {9223372036ULL, 854775807}
    };
    assert(registry.configure(c::topic_plan{publish_owner,
                                            pub,
                                            node,
                                            type,
                                            "/scene_qualification/commands",
                                            c::role::publisher,
                                            quality,
                                            1024,
                                            0,
                                            0,
                                            c::overflow::reject,
                                            10000000000ULL}) == c::error::ok);
    assert(registry.configure(c::topic_plan{take_owner,
                                            sub,
                                            node,
                                            type,
                                            "/scene_qualification/events",
                                            c::role::subscription,
                                            quality,
                                            1024,
                                            4,
                                            4096,
                                            c::overflow::reject,
                                            10000000000ULL}) == c::error::ok);
    auto request_type = selected("set-bool-request", 6), response_type = selected("set-bool-response", 7);
    auto service_client = selected("service-client", 8), service_server = selected("service-server", 9);
    c::owner client_owner{1, 6, 1}, server_owner{1, 7, 1};
    assert(registry.configure(c::type_plan{
               request_type,
               "std_srvs/srv/SetBool_Request",
               "scene-canonical",
               {2},
               &c::ros2::set_bool_request_codec::validate,
               {1, 8, 1}
    }) == c::error::ok);
    assert(registry.configure(c::type_plan{
               response_type,
               "std_srvs/srv/SetBool_Response",
               "scene-canonical",
               {3},
               &c::ros2::set_bool_response_codec::validate,
               {1, 9, 1}
    }) == c::error::ok);
    assert(registry.configure(c::service_plan{client_owner,
                                              service_client,
                                              node,
                                              request_type,
                                              response_type,
                                              "/scene_qualification/upstream",
                                              "std_srvs/srv/SetBool",
                                              c::service_role::client,
                                              quality,
                                              1,
                                              8,
                                              1024,
                                              1024,
                                              10000000000ULL}) == c::error::ok);
    assert(registry.configure(c::service_plan{server_owner,
                                              service_server,
                                              node,
                                              request_type,
                                              response_type,
                                              "/scene_qualification/managed",
                                              "std_srvs/srv/SetBool",
                                              c::service_role::server,
                                              quality,
                                              1,
                                              8,
                                              1024,
                                              1024,
                                              10000000000ULL}) == c::error::ok);
    c::handle ctx{}, n{}, p{}, s{};
    assert(registry.open_context({2, 1, 1}, context, ctx) == c::error::denied && ctx == 0);
    auto e = registry.open_context(context_owner, context, ctx);
    assert(e == c::error::ok);
    assert(registry.open_node(node_owner, context_owner, ctx, node, n) == c::error::ok);
    assert(registry.open_topic(publish_owner, node_owner, n, pub, "/scene_qualification/commands", "std_msgs/msg/String", c::role::publisher, p) ==
           c::error::ok);
    assert(registry.open_topic(take_owner, node_owner, n, sub, "/scene_qualification/events", "std_msgs/msg/String", c::role::subscription, s) == c::error::ok);
    namespace ev = uwvm2::imported::ros::scene::events;
    auto event_source = std::make_shared<ev::communication_source>(retained_registry);

    struct observation_grant
    {
        c::owner owner;
        c::handle resource;
    };

    observation_grant observed_grant{take_owner, s};
    auto observe_admit = +[](void* argument, c::owner owner, c::handle resource) noexcept
    {
        auto& grant = *static_cast<observation_grant*>(argument);
        return grant.owner == owner && grant.resource == resource;
    };
    ev::stream events;
    auto event_profile = selected("readiness-events", 10);
    std::array<ev::binding, 1> event_sources{{{take_owner, s, take_owner.generation, event_source}}};
    c::owner event_owner{1, 10, 1};
    assert(events.configure({event_owner, event_profile, 1, 1, 1, 10000000000ULL}, event_sources, &observed_grant, observe_admit) == c::error::ok);
    bool event_ready{};
    auto event_sink = +[](void* output, std::span<ev::event const> values)
    {
        assert(values.size() == 1 && values[0].source_index == 1 && values[0].fields == 1);
        *static_cast<bool*>(output) = values[0].ready;
        return true;
    };
    auto poll_readiness = [&] { return events.poll(event_owner, 1, c::now() + 5000000000ULL, &observed_grant, observe_admit, &event_ready, event_sink); };
    assert(poll_readiness() == c::error::ok && !event_ready);
    c::qos actual;
    std::array<std::uint8_t, 32> gid{};
    std::size_t gid_bytes{};
    assert(registry.describe_qos(publish_owner, p, actual, gid, gid_bytes) == c::error::ok && c::ros2::equivalent(quality, actual) &&
           gid_bytes == RMW_GID_STORAGE_SIZE);
    c::topic_info publisher_description,subscription_description;
    assert(registry.describe(publish_owner,p,publisher_description)==c::error::ok&&
        !publisher_description.native.provider_generation&&publisher_description.native.size==gid_bytes&&
        std::equal(gid.begin(),gid.begin()+gid_bytes,publisher_description.native.native.begin()));
    assert(registry.describe(take_owner,s,subscription_description)==c::error::ok&&
        !subscription_description.native.provider_generation&&!subscription_description.native.size);
    // A subscription never reports a peer publisher's GID as its own identity.

    auto peer_context = std::make_shared<rclcpp::Context>();
    rclcpp::InitOptions init;
    init.set_domain_id(43);
    init.auto_initialize_logging(false);
    init.shutdown_on_signal = false;
    char const* argv[]{"scene-native-peer"};
    peer_context->init(1, argv, init);
    rclcpp::NodeOptions options;
    options.context(peer_context).use_global_arguments(false).enable_rosout(false).start_parameter_services(false).start_parameter_event_publisher(false);
    auto peer = std::make_shared<rclcpp::Node>("peer", "/scene_qualification", options);
    std::size_t received{};
    std::string observed;
    auto publisher = peer->create_publisher<std_msgs::msg::String>("/scene_qualification/events", rclcpp::QoS(4));
    auto subscription = peer->create_subscription<std_msgs::msg::String>("/scene_qualification/commands",
                                                                         rclcpp::QoS(4),
                                                                         [&](std_msgs::msg::String::ConstSharedPtr value)
                                                                         {
                                                                             ++received;
                                                                             observed = value->data;
                                                                         });
    rclcpp::ExecutorOptions execution;
    execution.context = peer_context;
    rclcpp::executors::SingleThreadedExecutor executor{execution};
    executor.add_node(peer);
    auto until = c::now() + 5000000000ULL;
    while(publisher->get_subscription_count() != 1 || subscription->get_publisher_count() != 1)
    {
        assert(c::now() < until);
        executor.spin_some();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    bool graph_ready{};
    until = c::now() + 5000000000ULL;
    while(!graph_ready)
    {
        if(c::now() >= until)
        {
            std::cerr << "graph names:";
            for(auto const& name: peer->get_node_names()) { std::cerr << ' ' << name; }
            std::cerr << '\n';
            auto names = peer->get_topic_names_and_types();
            for(auto const& [name, types]: names) { std::cerr << "topic " << name << '\n'; }
            for(auto const& [name, types]: peer->get_service_names_and_types_by_node("scene_native", "/scene_qualification"))
            {
                std::cerr << "service " << name << '\n';
            }
            for(auto const& [name, types]: peer->get_node_graph_interface()->get_publisher_names_and_types_by_node("scene_native", "/scene_qualification"))
            {
                std::cerr << "publisher " << name << '\n';
            }
            for(auto const& [name, types]: peer->get_node_graph_interface()->get_subscriber_names_and_types_by_node("scene_native", "/scene_qualification"))
            {
                std::cerr << "subscriber " << name << '\n';
            }
            assert(false && "native node graph did not converge");
        }
        try
        {
            auto services = peer->get_service_names_and_types_by_node("scene_native", "/scene_qualification");
            auto actual_publishers = peer->get_node_graph_interface()->get_publisher_names_and_types_by_node("scene_native", "/scene_qualification");
            auto actual_subscriptions = peer->get_node_graph_interface()->get_subscriber_names_and_types_by_node("scene_native", "/scene_qualification");
            graph_ready = services.empty() && actual_publishers.size() == 1 && actual_publishers.begin()->first == "/scene_qualification/commands" &&
                          actual_subscriptions.size() == 1 && actual_subscriptions.begin()->first == "/scene_qualification/events";
        }
        catch(std::runtime_error const&)
        {
        }
        if(!graph_ready)
        {
            executor.spin_some();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    for(std::size_t budget: {std::size_t{0},std::size_t{1}})
    {
        c::node_lease limited_attachment;assert(registry.attach_node(node_owner,n,node,limited_attachment)==c::error::ok);
        c::ros2::graph_source limited{std::move(limited_attachment),budget};
        c::graph::plan query;query.context=ctx;query.nodes={"/scene_qualification/scene_native"};query.fields=2;query.max_rows=8;query.max_bytes=4096;
        uwvm2::utils::container::std_vector<c::graph::entity> rows;
        assert(limited.capture(query,rows)==(budget?c::error::too_large:c::error::denied)&&rows.empty());
    }
    c::node_lease graph_attachment;
    assert(registry.attach_node(node_owner, n, node, graph_attachment) == c::error::ok);
    auto graph_source = uwvm2::utils::container::make_shared<c::ros2::graph_source>(std::move(graph_attachment),65536);
    auto retained_graph = std::make_shared<c::graph::snapshot>();
    auto& graph = *retained_graph;
    auto graph_profile = selected("filtered-native-graph", 11);
    c::owner graph_owner{1, 11, 1};
    assert(graph.configure({graph_owner, graph_profile, ctx, {"/scene_qualification/scene_native"}, 39, 8, 4096, 1, 10000000000ULL, 5000000000ULL},
                           graph_source) == c::error::ok);

    c::graph::watch watcher{retained_graph, graph_owner, 1};
    ev::snapshot watched;
    assert(watcher.inspect(graph_owner, 1, graph_owner.generation, watched) == c::error::ok && watched.ready);

    struct graph_page
    {
        std::optional<c::handle> cursor;
        std::size_t count{};
        bool accept{};
        c::handle revision{};
    } graph_result;

    auto graph_sink = +[](void* argument, c::graph::page const& page)
    {
        auto& out = *static_cast<graph_page*>(argument);
        out.cursor = page.cursor;
        out.count += page.entities.size();
        out.revision = page.revision;
        for(auto const& entity: page.entities) { assert(entity.node == "/scene_qualification/scene_native"); }
        return out.accept;
    };
    assert(graph.read(graph_owner, graph_profile, ctx, {}, 1, &graph_result, graph_sink) == c::error::io);
    graph_result.accept = true;
    graph_result.count = 0;
    assert(graph.read(graph_owner, graph_profile, ctx, {}, 1, &graph_result, graph_sink) == c::error::ok);
    while(graph_result.cursor) { assert(graph.read(graph_owner, graph_profile, ctx, graph_result.cursor, 1, &graph_result, graph_sink) == c::error::ok); }
    assert(graph_result.count == 3 && graph_result.revision == 1);
    assert(watcher.inspect(graph_owner, 1, graph_owner.generation, watched) == c::error::ok && !watched.ready);

    auto bytes = encoded("exact native payload");
    c::receipt receipt{}, retry{};
    auto deadline = c::now() + 5000000000ULL;
    assert(registry.publish(publish_owner, p, {p, 1}, "std_msgs/msg/String", "scene-canonical", bytes, deadline, receipt) == c::error::ok);
    assert(receipt.status == c::error::ok && receipt.observed == c::effect::locally_accepted);
    assert(registry.publish(publish_owner, p, {p, 1}, "std_msgs/msg/String", "scene-canonical", bytes, deadline, retry) == c::error::ok &&
           retry.request == receipt.request);
    while(!received)
    {
        assert(c::now() < until);
        executor.spin_some();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    assert(received == 1 && observed == "exact native payload");
    bytes.back() ^= 1;
    assert(registry.publish(publish_owner, p, {p, 1}, "std_msgs/msg/String", "scene-canonical", bytes, deadline, retry) == c::error::stale);
    assert(registry.publish(take_owner, s, {s, 1}, "std_msgs/msg/String", "scene-canonical", bytes, deadline, retry) == c::error::denied);
    std_msgs::msg::String incoming;
    incoming.data = "native ingress";
    publisher->publish(incoming);
    bool borrowed{};
    until = c::now() + 5000000000ULL;
    while(!borrowed)
    {
        assert(c::now() < until);
        auto result = registry.take(take_owner,
                                    s,
                                    [&](c::delivery const& value)
                                    {
                                        borrowed = true;
                                        assert(value.information.publisher_bytes == RMW_GID_STORAGE_SIZE);
                                        return false;
                                    });
        assert(result == c::error::again || result == c::error::io);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    assert(poll_readiness() == c::error::ok && event_ready);
    assert(registry.take(take_owner,
                         s,
                         [&](c::delivery const& value)
                         {
                             std_msgs::msg::String decoded;
                             assert(c::ros2::string_codec::decode(value.body, decoded));
                             assert(decoded.data == "native ingress" && value.information.sequence == 1);
                             return true;
                         }) == c::error::ok);
    assert(registry.take(take_owner, s, [](auto const&) { return true; }) == c::error::again);
    assert(poll_readiness() == c::error::ok && !event_ready);
    c::handle client{}, server{};
    assert(registry.open_service(client_owner,
                                 node_owner,
                                 n,
                                 service_client,
                                 "/scene_qualification/upstream",
                                 "std_srvs/srv/SetBool",
                                 c::service_role::client,
                                 client) == c::error::ok);
    assert(registry.open_service(server_owner,
                                 node_owner,
                                 n,
                                 service_server,
                                 "/scene_qualification/managed",
                                 "std_srvs/srv/SetBool",
                                 c::service_role::server,
                                 server) == c::error::ok);
    for(auto selected:{std::pair{client_owner,client},std::pair{server_owner,server}})
    {
        c::service_info description;assert(registry.describe(selected.first,selected.second,description)==c::error::ok&&
            !description.native.provider_generation&&!description.native.size&&description.channels[0]&&description.channels[1]&&
            c::ros2::equivalent(*description.channels[0],quality)&&c::ros2::equivalent(*description.channels[1],quality));
        assert(description.direction==(selected.second==client?c::service_role::client:c::service_role::server));
    }
    unsigned remote_calls{};
    auto native_server = peer->create_service<std_srvs::srv::SetBool>(
        "/scene_qualification/upstream",
        [&](std::shared_ptr<std_srvs::srv::SetBool::Request> req, std::shared_ptr<std_srvs::srv::SetBool::Response> response)
        {
            ++remote_calls;
            response->success = req->data;
            response->message = "upstream response";
        });
    auto native_client = peer->create_client<std_srvs::srv::SetBool>("/scene_qualification/managed");
    bool available{};
    until = c::now() + 5000000000ULL;
    do
    {
        assert(c::now() < until);
        executor.spin_some();
        assert(registry.service_available(client_owner, client, available) == c::error::ok);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    while(!available || !native_client->service_is_ready());
    std::array<std::uint8_t, 1> request_body{1};
    deadline = c::now() + 5000000000ULL;
    assert(
        registry
            .service_mutate(client_owner, client, {client, 1}, 0, "std_srvs/srv/SetBool_Request", "scene-canonical", request_body, deadline, false, receipt) ==
        c::error::ok);
    assert(registry.service_mutate(client_owner,
                                   client,
                                   {client, 1},
                                   0,
                                   "std_srvs/srv/SetBool_Request",
                                   "scene-canonical",
                                   request_body,
                                   deadline,
                                   false,
                                   retry) == c::error::ok &&
           retry.request == receipt.request);
    assert(registry
               .service_mutate(client_owner, client, {client, 2}, 0, "std_srvs/srv/SetBool_Request", "scene-canonical", request_body, deadline, false, retry) ==
           c::error::again);
    assert(registry.close(client_owner, client) == c::error::busy);

    struct service_capture
    {
        c::handle correlation{};
        bool response{};
        std::vector<std::uint8_t> body;
        bool accept{};
    } capture;

    auto service_sink = +[](void* argument, c::service_delivery const& record)
    {
        auto& captured = *static_cast<service_capture*>(argument);
        captured.correlation = record.correlation;
        captured.response = record.response;
        captured.body.assign(record.body.begin(), record.body.end());
        return captured.accept;
    };
    until = c::now() + 5000000000ULL;
    while(!capture.correlation)
    {
        assert(c::now() < until);
        executor.spin_some();
        auto result = registry.service_take(client_owner, client, &capture, service_sink);
        assert(result == c::error::again || result == c::error::io);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    assert(capture.correlation == receipt.request && capture.response && remote_calls == 1);
    std_srvs::srv::SetBool::Response decoded_response;
    assert(c::ros2::set_bool_response_codec::decode(capture.body, decoded_response) && decoded_response.success &&
           decoded_response.message == "upstream response");
    capture.accept = true;
    assert(registry.service_take(client_owner, client, &capture, service_sink) == c::error::ok);
    assert(registry.service_take(client_owner, client, &capture, service_sink) == c::error::again);
    assert(
        registry
            .service_mutate(client_owner, client, {client, 2}, 0, "std_srvs/srv/SetBool_Request", "scene-canonical", request_body, deadline, false, receipt) ==
        c::error::ok);
    assert(registry.service_abandon(client_owner, client, receipt.request, retry) == c::error::ok && retry.observed == c::effect::unknown);
    assert(registry.service_abandon(client_owner, client, receipt.request, retry) == c::error::ok);
    auto native_request = std::make_shared<std_srvs::srv::SetBool::Request>();
    native_request->data = true;
    auto native_future = native_client->async_send_request(native_request);
    capture = {};
    until = c::now() + 5000000000ULL;
    while(!capture.correlation)
    {
        assert(c::now() < until);
        executor.spin_some();
        auto result = registry.service_take(server_owner, server, &capture, service_sink);
        assert(result == c::error::again || result == c::error::io);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    assert(!capture.response && capture.body == std::vector<std::uint8_t>{1});
    std_srvs::srv::SetBool::Response answer;
    answer.success = true;
    answer.message = "managed response";
    std::array<std::uint8_t, 1024> response_buffer{};
    std::size_t response_size{};
    assert(c::ros2::set_bool_response_codec::encode(answer, response_buffer, response_size));
    auto response_body = std::span<std::uint8_t const>{response_buffer.data(), response_size};
    deadline = c::now() + 5000000000ULL;
    assert(registry.service_mutate(server_owner,
                                   server,
                                   {server, 1},
                                   capture.correlation,
                                   "std_srvs/srv/SetBool_Response",
                                   "scene-canonical",
                                   response_body,
                                   deadline,
                                   true,
                                   receipt) == c::error::denied);
    capture.accept = true;
    assert(registry.service_take(server_owner, server, &capture, service_sink) == c::error::ok);
    assert(registry.service_mutate(server_owner,
                                   server,
                                   {server, 1},
                                   capture.correlation,
                                   "std_srvs/srv/SetBool_Response",
                                   "scene-canonical",
                                   response_body,
                                   deadline,
                                   true,
                                   receipt) == c::error::ok);
    assert(registry.service_mutate(server_owner,
                                   server,
                                   {server, 1},
                                   capture.correlation,
                                   "std_srvs/srv/SetBool_Response",
                                   "scene-canonical",
                                   response_body,
                                   deadline,
                                   true,
                                   retry) == c::error::ok &&
           retry.request == receipt.request);
    assert(executor.spin_until_future_complete(native_future, std::chrono::seconds(5)) == rclcpp::FutureReturnCode::SUCCESS);
    auto native_response = native_future.get();
    assert(native_response->success && native_response->message == "managed response");
    assert(registry.close(client_owner, client) == c::error::busy && registry.close(server_owner, server) == c::error::ok);
    assert(registry.close(publish_owner, p) == c::error::ok);
    until = c::now() + 5000000000ULL;
    do
    {
        assert(c::now() < until);
        assert(watcher.inspect(graph_owner, 1, graph_owner.generation, watched) == c::error::ok);
        if(!watched.ready) { std::this_thread::sleep_for(std::chrono::milliseconds(5)); }
    }
    while(!watched.ready);
    graph_result.count = 0;
    assert(graph.read(graph_owner, graph_profile, ctx, {}, 1, &graph_result, graph_sink) == c::error::ok);
    while(graph_result.cursor) { assert(graph.read(graph_owner, graph_profile, ctx, graph_result.cursor, 1, &graph_result, graph_sink) == c::error::ok); }
    assert(graph_result.count == 2 && graph_result.revision == 2);
    assert(watcher.inspect(graph_owner, 1, graph_owner.generation, watched) == c::error::ok && !watched.ready);
    assert(registry.shutdown(context_owner, ctx) == c::error::ok);
    assert(registry.publish(publish_owner, p, {p, 2}, "std_msgs/msg/String", "scene-canonical", bytes, deadline, retry) == c::error::denied);
    assert(poll_readiness() == c::error::denied);
    assert(graph.read(graph_owner, graph_profile, ctx, {}, 1, &graph_result, graph_sink) == c::error::cancelled);
    executor.remove_node(peer);
    peer_context->shutdown("test complete");
    std::cout << "passed actual ROS2 context/node/topic publication/subscription/QoS, bounded bidirectional service and replay isolation\n";
}
