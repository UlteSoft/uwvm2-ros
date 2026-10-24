/**
 * @file native_ros1_peer.cc
 * @brief Independent bounded TCPROS observer for all six SDK languages.
 *
 * CLI: native_ros1_peer positive|absent. The controller reserves Master port
 * 11384, supplies only ROS_MASTER_URI and ROS_IP=127.0.0.1, proves the stopped
 * ARM image, then sends I. Q follows actual Guest/SDK success and Worker exit0.
 * This executable never creates a Gateway, opens IPC, supplies a Wasm identity
 * or grants authority. Its separate process prevents roscpp intra-process
 * delivery from being mistaken for TCPROS. Master graph, connection header and
 * actual byte counters are all required; a publish return alone is insufficient.
 *
 * Noetic uses C++17. The shared checked native_io helper is used because the
 * vendored fast_io native library requires C++20. Logging contains only fixed
 * public protocol metadata and counters, never remote payload/exception bodies.
 */
#include "../../../../../src/ros/gateway/ros1/resources.h"
#include <std_msgs/String.h>
#include <ros/message_event.h>
#include <ros/message_traits.h>
#include <ros/xmlrpc_manager.h>
#include <array>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <fcntl.h>
#include <poll.h>
#include <stdexcept>
#include <string_view>
#include <thread>
#include <type_traits>
#include <unistd.h>

__attribute__((constructor)) static void sdk_ros1_preinit_stop()
{ if(::raise(SIGSTOP) != 0) { __builtin_trap(); } }

namespace sdk_ros1_peer
{
    namespace io = uwvm2::ros_gateway::native_io;
    using clock = std::chrono::steady_clock;
    constexpr std::string_view peer{"/uwvm_sdk_relay/sdk_peer"}, gateway{"/uwvm_sdk_relay/sdk_gateway"};
    constexpr char out_topic[]{"/uwvm_sdk_relay/pub"}, in_topic[]{"/uwvm_sdk_relay/sub"};
    constexpr std::string_view md5{"992ce8a1687cec8c8bd883ec73ca41d1"};
    constexpr std::string_view warmup{"warmup-js-relay"}, final_text{"roswasm-js-relay-out"}, reply{"roswasm-js-relay-in"};
    std::string_view phase{"input"}, field{"contract"};
    void require(bool value, std::string_view fixed = "contract")
    { if(!value) { field = fixed; throw std::runtime_error("fixed SDK TCPROS refusal"); } }
    bool equal(char const* actual, std::string_view expected) noexcept
    { return actual && std::string_view{actual} == expected; }
    template<typename T> bool append(io::fixed_text<2048>& output, T const& value)
    {
        if constexpr(std::is_integral_v<T>) { return output.number(value); }
        else { return output.append(std::string_view{value}); }
    }
    template<typename... Args> void emit(Args const&... args)
    {
        io::fixed_text<2048> line;
        require((append(line, args) && ...) && line.append("\n"), "output_bound");
        pollfd descriptor{1, POLLOUT, 0};
        int ready{::poll(&descriptor, 1, 1000)};
        require(ready > 0 && (descriptor.revents & POLLOUT) && !(descriptor.revents & (POLLERR|POLLHUP|POLLNVAL)), "output_ready");
        auto count{::write(1, line.view().data(), line.view().size())};
        require(count == static_cast<ssize_t>(line.view().size()), "output_short");
    }
    bool gate(char expected)
    {
        pollfd descriptor{0, POLLIN, 0}; int ready{::poll(&descriptor, 1, 0)};
        if(ready < 0 && errno == EINTR) { return false; }
        require(ready >= 0 && !(descriptor.revents & (POLLERR|POLLNVAL)), "input_poll");
        if(!ready) { return false; }
        require(descriptor.revents & POLLIN, "input_eof");
        char value{}; auto count{::read(0, &value, 1)};
        if(count < 0 && (errno == EINTR || errno == EAGAIN)) { return false; }
        require(count == 1 && value == expected, "input_token"); return true;
    }
    void environment()
    {
        require(equal(::getenv("ROS_MASTER_URI"), "http://127.0.0.1:11384") && equal(::getenv("ROS_IP"), "127.0.0.1"), "environment");
        for(auto name : {"ROS_HOSTNAME", "ROS_NAMESPACE", "LD_PRELOAD", "ROS_SECURITY_ENABLE",
                        "ROS_SECURITY_KEYSTORE", "ROS_SECURITY_ENCLAVE_OVERRIDE"})
        { require(::getenv(name) == nullptr, "environment"); }
        require(::signal(SIGPIPE, SIG_IGN) != SIG_ERR, "signal");
        for(int fd : {0, 1})
        { int flags{::fcntl(fd, F_GETFL, 0)}; require(flags >= 0 && ::fcntl(fd, F_SETFL, flags|O_NONBLOCK) == 0, "descriptor"); }
    }
    XmlRpc::XmlRpcValue query(char const* method, XmlRpc::XmlRpcValue const& arguments)
    {
        uwvm2::ros_gateway::ros1_resources::bounded_xmlrpc client{"127.0.0.1", 11384, 65536};
        XmlRpc::XmlRpcValue response;
        require(client.execute_bounded(method, arguments, response, 2000) == 0, "master_query");
        require(response.getType() == XmlRpc::XmlRpcValue::TypeArray && response.size() == 3 &&
                response[0].getType() == XmlRpc::XmlRpcValue::TypeInt && static_cast<int>(response[0]) == 1,
                "master_response"); return response[2];
    }
    unsigned registered(XmlRpc::XmlRpcValue& rows, std::string_view topic, std::string_view node)
    {
        require(rows.getType() == XmlRpc::XmlRpcValue::TypeArray && rows.size() <= 128, "master_rows");
        unsigned count{};
        for(int n{}; n != rows.size(); ++n)
        {
            auto& item{rows[n]};
            require(item.getType() == XmlRpc::XmlRpcValue::TypeArray && item.size() == 2 &&
                    item[0].getType() == XmlRpc::XmlRpcValue::TypeString && item[1].getType() == XmlRpc::XmlRpcValue::TypeArray,
                    "master_entry");
            if(static_cast<std::string&>(item[0]) != topic) { continue; }
            require(item[1].size() <= 2, "foreign_endpoint");
            for(int k{}; k != item[1].size(); ++k)
            {
                require(item[1][k].getType() == XmlRpc::XmlRpcValue::TypeString, "master_node");
                auto const& name{static_cast<std::string&>(item[1][k])};
                require(name == peer || name == gateway, "foreign_endpoint");
                if(name == node) { ++count; }
            }
        }
        require(count <= 1, "duplicate_endpoint"); return count;
    }
    struct graph_counts { unsigned gateway_publishers{}, gateway_subscribers{}, own_publishers{}, own_subscribers{}; };
    graph_counts graph()
    {
        XmlRpc::XmlRpcValue args; args[0] = std::string{peer}; auto rows{query("getSystemState", args)};
        require(rows.getType() == XmlRpc::XmlRpcValue::TypeArray && rows.size() == 3, "master_state");
        return {registered(rows[0], out_topic, gateway), registered(rows[1], in_topic, gateway),
                registered(rows[0], in_topic, peer), registered(rows[1], out_topic, peer)};
    }
    struct observations
    {
        bool positive{}, matched{}; unsigned warmup_count{}, final_out{}, reply_send{}, excess{};
        void receive(ros::MessageEvent<std_msgs::String const> const& event)
        {
            auto value{event.getMessage()}; auto header{event.getConnectionHeaderPtr()};
            require(value && header && event.getPublisherName() == gateway, "callerid");
            auto type{header->find("type")}, digest{header->find("md5sum")}, caller{header->find("callerid")};
            require(type != header->end() && type->second == "std_msgs/String" && digest != header->end() &&
                    digest->second == md5 && caller != header->end() && caller->second == gateway, "tcpros_header");
            if(!positive) { ++excess; require(false, "unexpected_message"); }
            if(value->data == warmup && !warmup_count && !final_out) { ++warmup_count; return; }
            if(value->data == final_text && matched && !final_out) { ++final_out; return; }
            ++excess; require(false, "unexpected_message");
        }
    };
    template<typename... Tail> void marker(std::string_view name, std::string_view mode, Tail const&... tail)
    {
        emit(name, " backend=1 mode=", mode, " pid=", ::getpid(), " node=", peer, " gateway_node=", gateway,
             " type=std_msgs/String out_topic=", std::string_view{out_topic}, " in_topic=", std::string_view{in_topic},
             " master=http://127.0.0.1:11384 transport=tcpros md5=", md5, tail...);
    }
}

int main(int argc, char** argv)
{
    using namespace sdk_ros1_peer;
    try
    {
        require(argc == 2 && (equal(argv[1], "positive") || equal(argv[1], "absent")), "mode");
        bool positive{equal(argv[1], "positive")}; std::string_view mode{positive ? "positive" : "absent"}; environment();
        auto initial{clock::now() + std::chrono::seconds{20}};
        while(!gate('I')) { require(clock::now() < initial, "deadline"); std::this_thread::sleep_for(std::chrono::milliseconds{2}); }
        auto end{clock::now() + std::chrono::seconds{60}}; phase = "initialization";
        require(equal(ros::message_traits::MD5Sum<std_msgs::String>::value(), md5) &&
                equal(ros::message_traits::DataType<std_msgs::String>::value(), "std_msgs/String"), "native_type");
        // Fixed local remappings select our own node only; caller argv and
        // environment cannot rename a peer to impersonate the Gateway.
        ros::M_string remappings; remappings["__ns"] = "/uwvm_sdk_relay";
        ros::init(remappings, "sdk_peer", ros::init_options::NoSigintHandler|ros::init_options::NoRosout);
        ros::NodeHandle node{"/uwvm_sdk_relay"}; require(ros::this_node::getName() == peer, "node_identity");
        observations observed; observed.positive = positive;
        auto publisher{node.advertise<std_msgs::String>(in_topic, 4, false)};
        ros::SubscribeOptions options; options.initByFullCallbackType<ros::MessageEvent<std_msgs::String const> const&>(
            out_topic, 4, [&](ros::MessageEvent<std_msgs::String const> const& event) { observed.receive(event); });
        options.transport_hints = ros::TransportHints{}.tcpNoDelay().reliable(); auto subscriber{node.subscribe(options)};
        auto step = [&] { require(clock::now() < end && ros::ok(), "deadline"); ros::spinOnce(); std::this_thread::sleep_for(std::chrono::milliseconds{2}); };
        phase = "own-discovery";
        for(;;) { auto value{graph()}; if(value.own_publishers == 1 && value.own_subscribers == 1) { break; } step(); }
        marker("READY_SDK_PEER", mode);
        if(positive)
        {
            phase = "matching";
            for(;;)
            {
                auto value{graph()}; require(subscriber.getNumPublishers() <= 1 && publisher.getNumSubscribers() <= 1, "connection_count");
                if(value.gateway_publishers == 1 && value.gateway_subscribers == 1 && subscriber.getNumPublishers() == 1 && publisher.getNumSubscribers() == 1) { break; }
                step();
            }
            observed.matched = true; marker("MATCHED_SDK_PEER", mode, " gateway_publishers=1 gateway_subscribers=1");
            phase = "delivery"; while(observed.final_out != 1) { step(); }
            require(publisher.getNumSubscribers() == 1 && subscriber.getNumPublishers() == 1, "reply_match");
            std_msgs::String value; value.data.assign(reply.data(), reply.size()); publisher.publish(value); ++observed.reply_send;
            marker("DELIVERED_SDK_PEER", mode, " gateway_publishers=1 gateway_subscribers=1 warmup=", observed.warmup_count,
                   " final_out=1 reply_send=1 excess=0");
        }
        phase = "completion";
        while(!gate('Q'))
        {
            if(!positive) { auto value{graph()}; require(!value.gateway_publishers && !value.gateway_subscribers && !subscriber.getNumPublishers() && !publisher.getNumSubscribers(), "unexpected_endpoint"); }
            step();
        }
        phase = "quiet"; auto begin{clock::now()}; unsigned samples{};
        while(clock::now()-begin < std::chrono::seconds{2})
        {
            ros::spinOnce(); require(observed.final_out == (positive ? 1U : 0U) && observed.reply_send == (positive ? 1U : 0U) && !observed.excess, "count");
            if(!positive) { auto value{graph()}; require(!value.gateway_publishers && !value.gateway_subscribers && !subscriber.getNumPublishers() && !publisher.getNumSubscribers(), "unexpected_endpoint"); }
            ++samples; std::this_thread::sleep_for(std::chrono::milliseconds{10});
        }
        auto elapsed{std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now()-begin).count()};
        require(samples >= 30 && samples <= 10000 && elapsed >= 2000000000LL && elapsed <= 10000000000LL, "quiet_budget");
        publisher.shutdown(); subscriber.shutdown(); node.shutdown(); ros::shutdown();
        marker("PASS_SDK_PEER", mode, " gateway_publishers=", positive ? 1U : 0U, " gateway_subscribers=", positive ? 1U : 0U,
               " warmup=", observed.warmup_count, " final_out=", observed.final_out, " reply_send=", observed.reply_send,
               " excess=0 quiet_samples=", samples, " quiet_ns=", elapsed);
        return 0;
    }
    catch(...)
    { try { emit("FAIL_SDK_PEER backend=1 phase=", phase, " field=", field); } catch(...) {} ros::shutdown(); return 1; }
}
