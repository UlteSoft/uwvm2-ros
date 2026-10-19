/** @file
 * Independent native typed-String witness for embedded-JavaScript tests.
 *
 * Build with UWVM_JS_NATIVE_BACKEND=1 (roscpp/std_msgs) or2
 * (rclcpp/std_msgs/rmw). The guarded native producer owns the exact official
 * interface definitions, compiler/full-MD/link/SDK provenance and capability
 * feature report. This witness does not manufacture RIHS/type-description
 * support on older ROS2 releases. ROS2 writer GIDs come from actual native
 * endpoint metadata and MessageInfo; their observed byte width is retained.
 *
 * CLI: positive|absent|event|ownership. BOOT precedes I and actual ROS context
 * creation. The owner proves this live executable before sending I. Positive
 * waits for native discovery, accepts one exact Guest message, then replies
 * once. Event publishes only after explicit T. Q follows actual Guest success;
 * at least two seconds/30 observations detect extra effects before PASS.
 * No physical-robot, performance, ROS-signature or cryptographic claim is made.
 */
#if __cplusplus >= 202002L
#include <fast_io.h>
#include <fast_io_dsal/string.h>
#else
// Native Noetic/Focal builds select the real C++17 I/O shim. This changes
// only bounded protocol formatting/pipe reads, never ROS calls or authority.
#include "../../gateways/common/native_io.h"
#endif
#include <array>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <cerrno>
#include <poll.h>
#include <unistd.h>
#if UWVM_JS_NATIVE_BACKEND == 2
#include <rclcpp/rclcpp.hpp>
#include <rmw/rmw.h>
#include <std_msgs/msg/string.hpp>
#elif UWVM_JS_NATIVE_BACKEND == 1
#include <ros/ros.h>
#include <ros/message_traits.h>
#include <ros/master.h>
#include <std_msgs/String.h>
#else
#error "Explicit UWVM_JS_NATIVE_BACKEND=1 or2 is required"
#endif

namespace fixture
{
using clock = std::chrono::steady_clock;
constexpr std::string_view namespc{"/uwvm_js_guest"};
constexpr char outgoing[]{"/uwvm_js_guest/out"}, incoming[]{"/uwvm_js_guest/in"}, events[]{"/uwvm_js_guest/event"};
constexpr std::string_view warmup{"warmup-js-relay"}, final_message{"roswasm-js-relay-out"},
    reply_message{"roswasm-js-relay-in"}, event_message{"roswasm-js-event"};
void require(bool value) { if(!value) { throw std::runtime_error("native witness contract"); } }
template<typename... T> void emit(T const&... values)
{
    // All fields are fixed protocol literals, integers or checked hexadecimal
    // GIDs. Formatting never inserts unescaped remote strings into JSON.
#if __cplusplus >= 202002L
    // A decayed C string is not a printable fast_io value. Preserve borrowed
    // literal/owned-string arguments, and turn only internal C-string fields
    // into an explicit bounded scatter before the synchronous concatenation.
    auto printable=[](auto const& value) -> decltype(auto)
    {
        using value_type=std::remove_cv_t<std::remove_reference_t<decltype(value)>>;
        if constexpr(std::is_pointer_v<value_type> &&
                     std::is_same_v<std::remove_cv_t<std::remove_pointer_t<value_type>>,char>)
        {
            require(value != nullptr);
            auto length{fast_io::cstr_nlen(value,4096)};
            require(length < 4096);
            return fast_io::mnp::strvw(value,value+length);
        }
        else { return (value); }
    };
    auto line{fast_io::concat_fast_io(printable(values)..., "\n")};
    require(line.size() <= 4096);
    auto end{fast_io::operations::write_some(fast_io::posix_io_observer{STDOUT_FILENO},
                                           line.data(), line.data()+line.size())};
    require(end == line.data()+line.size());
#else
    static_assert(UWVM_JS_NATIVE_BACKEND == 1,
                  "The C++17 compatibility path is the actual ROS1 native witness");
    namespace io=uwvm2::ros_gateway::native_io;
    io::fixed_text<4096> line;
    auto append=[&line](auto const& value)
    {
        using value_type=std::decay_t<decltype(value)>;
        if constexpr(std::is_integral_v<value_type> && !std::is_same_v<value_type,bool>)
        { return line.number(value); }
        else { return line.append(std::string_view{value}); }
    };
    require((append(values) && ...));
    require(line.append("\n"));
    require(io::write_all(STDOUT_FILENO,line.view()));
#endif
}
char input()
{
    pollfd descriptor{STDIN_FILENO, POLLIN, 0};
    int ready{::poll(&descriptor, 1, 0)};
    if(ready < 0 && errno == EINTR) { return 0; }
    require(ready >= 0 && !(descriptor.revents & (POLLERR | POLLNVAL)));
    if(!ready) { return 0; }
    require(descriptor.revents & POLLIN);
    char value{};
#if __cplusplus >= 202002L
    auto end{fast_io::operations::read_some(fast_io::posix_io_observer{STDIN_FILENO}, &value, &value+1)};
    require(end == &value+1); return value;
#else
    ssize_t count;
    do { count=::read(STDIN_FILENO,&value,1); } while(count<0 && errno==EINTR);
    require(count==1); return value;
#endif
}
void initial_gate()
{
    auto deadline{clock::now()+std::chrono::seconds{20}};
    while(clock::now() < deadline)
    {
        auto value{input()}; if(value) { require(value == 'I'); return; }
        std::this_thread::sleep_for(std::chrono::milliseconds{2});
    }
    require(false);
}
struct counters { unsigned warmup{}, final{}, reply{}, event{}, excess{}; };

#if UWVM_JS_NATIVE_BACKEND == 2
using message = std_msgs::msg::String;
using gid = std::array<std::uint8_t, RMW_GID_STORAGE_SIZE>;
fast_io::string hex(gid const& value)
{
    fast_io::string text; text.reserve(value.size()*2);
    fast_io::ostring_ref_fast_io output{std::addressof(text)};
    for(auto byte : value) { fast_io::print(output, fast_io::mnp::right(fast_io::mnp::hex(static_cast<unsigned>(byte)), 2, '0')); }
    return text;
}
bool nonzero(gid const& value)
{ for(auto byte : value) { if(byte) { return true; } } return false; }
struct ros_owner
{
    std::shared_ptr<rclcpp::Context> context{std::make_shared<rclcpp::Context>()};
    std::shared_ptr<rclcpp::Node> node;
    rclcpp::Publisher<message>::SharedPtr publisher;
    rclcpp::Subscription<message>::SharedPtr subscriber;
    gid gateway_writer{}, own_writer{}, observed_writer{};
    explicit ros_owner(bool event, counters& counts)
    {
        rclcpp::InitOptions init; init.auto_initialize_logging(false);
        context->init(0, nullptr, init);
        rclcpp::NodeOptions options; options.context(context).use_global_arguments(false)
            .enable_rosout(false).start_parameter_services(false).start_parameter_event_publisher(false);
        node=std::make_shared<rclcpp::Node>("js_native_peer", std::string{namespc}, options);
        auto qos{rclcpp::QoS{rclcpp::KeepLast{4}}.reliable().durability_volatile()};
        publisher=node->create_publisher<message>(event ? events : incoming, qos);
        if(!event)
        {
            subscriber=node->create_subscription<message>(outgoing, qos,
                [this,&counts](message const& value, rclcpp::MessageInfo const& info)
                {
                    auto actual{info.get_rmw_message_info().publisher_gid};
                    require(actual.implementation_identifier &&
                            std::string_view{actual.implementation_identifier} == rmw_get_implementation_identifier());
                    gid writer{}; std::copy_n(actual.data, writer.size(), writer.begin());
                    require(nonzero(writer) && (!nonzero(observed_writer) || writer == observed_writer) &&
                            (!nonzero(gateway_writer) || writer == gateway_writer));
                    observed_writer=writer;
                    if(std::string_view{value.data} == warmup && !counts.warmup && !counts.final) { ++counts.warmup; }
                    else if(std::string_view{value.data} == final_message && !counts.final) { ++counts.final; }
                    else { ++counts.excess; require(false); }
                });
        }
        // Executor must use the exact explicit context, never the global one.
        rclcpp::ExecutorOptions selected; selected.context=context;
        owned_executor=std::make_unique<rclcpp::executors::SingleThreadedExecutor>(selected);
        owned_executor->add_node(node);
    }
    std::unique_ptr<rclcpp::executors::SingleThreadedExecutor> owned_executor;
    void spin() { owned_executor->spin_some(std::chrono::milliseconds{1}); }
    bool matched(bool event, bool absent)
    {
        auto publications{node->get_publishers_info_by_topic(event ? events : outgoing)};
        auto subscriptions{node->get_subscriptions_info_by_topic(event ? events : incoming)};
        unsigned gateway_pub{}, gateway_sub{}, own_pub{}, own_sub{}; bool pending{};
        auto unresolved=[](auto const& item)
        { return (item.node_name() == "_NODE_NAME_UNKNOWN_" && item.node_namespace() == "_NODE_NAMESPACE_UNKNOWN_") ||
                 (item.node_name() == "_CREATED_BY_BARE_DDS_APP_" && item.node_namespace() == "_CREATED_BY_BARE_DDS_APP_"); };
        for(auto const& item : publications)
        {
            require(item.topic_type() == "std_msgs/msg/String");
            if(unresolved(item)) { pending=true; continue; }
            if(item.node_name() == "js_gateway" && item.node_namespace() == namespc)
            { ++gateway_pub; gateway_writer=item.endpoint_gid(); require(nonzero(gateway_writer) &&
              (!nonzero(observed_writer) || gateway_writer == observed_writer)); }
            else if(event && item.node_name() == "js_native_peer" && item.node_namespace() == namespc)
            { ++own_pub; own_writer=item.endpoint_gid(); require(nonzero(own_writer)); }
            else { require(false); }
        }
        for(auto const& item : subscriptions)
        {
            require(item.topic_type() == "std_msgs/msg/String");
            if(unresolved(item)) { pending=true; continue; }
            if(item.node_name() == "js_gateway" && item.node_namespace() == namespc) { ++gateway_sub; }
            else { require(false); }
        }
        // The positive pub/sub peer additionally observes its own reverse
        // endpoints. No second native source may masquerade as the Gateway.
        if(!event)
        {
            for(auto const& item : node->get_publishers_info_by_topic(incoming))
            { if(unresolved(item)) { pending=true; continue; }
              require(item.node_name() == "js_native_peer" && item.node_namespace() == namespc);
              ++own_pub; own_writer=item.endpoint_gid(); require(nonzero(own_writer)); }
            for(auto const& item : node->get_subscriptions_info_by_topic(outgoing))
            { if(unresolved(item)) { pending=true; continue; }
              require(item.node_name() == "js_native_peer" && item.node_namespace() == namespc); ++own_sub; }
        }
        require(gateway_pub <= 1 && gateway_sub <= 1 && own_pub <= 1 && own_sub <= 1);
        if(absent) { require(!pending && !gateway_pub && !gateway_sub); return own_pub == 1 && (event || own_sub == 1); }
        if(pending) { return false; }
        return event ? own_pub == 1 && gateway_sub == 1 && !gateway_pub :
                       own_pub == 1 && own_sub == 1 && gateway_pub == 1 && gateway_sub == 1;
    }
    void publish(std::string_view value)
    { message message_; message_.data.assign(value.data(), value.size()); publisher->publish(message_); }
    void facts(char const* phase)
    { emit("{\"kind\":\"",phase,"\",\"backend\":2,\"pid\":",::getpid(),
           ",\"runtime_type_hash\":\"unavailable-not-inferred\",\"gid_bytes\":",own_writer.size(),
           ",\"native_writer\":\"",hex(own_writer),"\",\"gateway_writer\":\"",hex(gateway_writer),"\"}"); }
    ~ros_owner()
    { try { if(owned_executor && node) { owned_executor->remove_node(node); }
            subscriber.reset(); publisher.reset(); node.reset();
            if(context->is_valid()) { context->shutdown("native witness complete"); } } catch(...) {} }
};
#else
using message = std_msgs::String;
struct ros_owner
{
    ros::NodeHandle node;
    ros::Publisher publisher;
    ros::Subscriber subscriber;
    explicit ros_owner(bool event, counters& counts)
    {
        require(ros::message_traits::MD5Sum<message>::value() == std::string_view{"992ce8a1687cec8c8bd883ec73ca41d1"});
        publisher=node.advertise<message>(event ? events : incoming, 4, false);
        if(!event) {
            ros::SubscribeOptions options;
            options.initByFullCallbackType<ros::MessageEvent<message const> const&>(outgoing, 4,
            [&counts](ros::MessageEvent<message const> const& packet)
            {
                auto const headers{packet.getConnectionHeaderPtr()};
                require(headers && packet.getPublisherName() == "/uwvm_js_guest/js_gateway" &&
                        headers->count("callerid") && headers->at("callerid") == "/uwvm_js_guest/js_gateway" &&
                        headers->count("md5sum") && headers->at("md5sum") == "992ce8a1687cec8c8bd883ec73ca41d1" &&
                        headers->count("type") && headers->at("type") == "std_msgs/String");
                auto const& value{packet.getMessage()->data};
                if(std::string_view{value} == warmup && !counts.warmup && !counts.final) { ++counts.warmup; }
                else if(std::string_view{value} == final_message && !counts.final) { ++counts.final; }
                else { ++counts.excess; require(false); }
            });
            options.transport_hints=ros::TransportHints{}.tcpNoDelay().reliable();
            subscriber=node.subscribe(options);
        }
    }
    void spin() { ros::spinOnce(); }
    bool matched(bool event, bool absent)
    {
        // getNumPublishers/getNumSubscribers are real TCPROS connections, not
        // a local publish-success return. The callback independently binds the
        // actual native writer's callerid. The isolated Master is owner-held.
        unsigned writers{event ? 0u : subscriber.getNumPublishers()}, readers{publisher.getNumSubscribers()};
        require(writers <= 1 && readers <= 1);
        if(absent) { require(!writers && !readers); return true; }
        return readers == 1 && (event || writers == 1);
    }
    void publish(std::string_view value)
    { message message_; message_.data.assign(value.data(), value.size()); publisher.publish(message_); }
    void facts(char const* phase)
    { emit("{\"kind\":\"",phase,"\",\"backend\":1,\"pid\":",::getpid(),
           ",\"native_md5\":\"992ce8a1687cec8c8bd883ec73ca41d1\",\"gateway_caller\":\"/uwvm_js_guest/js_gateway\"}"); }
};
#endif
int run(int argc, char** argv)
{
    require(argc == 2);
    std::string_view mode{argv[1]}; require(mode == "positive" || mode == "absent" || mode == "event" || mode == "ownership");
    emit("{\"kind\":\"boot\",\"backend\":",UWVM_JS_NATIVE_BACKEND,",\"pid\":",::getpid(),"}");
    initial_gate();
#if UWVM_JS_NATIVE_BACKEND == 1
    ros::M_string remappings{{"__ns", "/uwvm_js_guest"}};
    ros::init(remappings, "js_native_peer", ros::init_options::NoSigintHandler | ros::init_options::NoRosout);
#endif
    counters counts; ros_owner native{mode == "event" || mode == "ownership", counts}; native.facts("ready");
    bool matched{}, delivered{}, finishing{}; unsigned quiet_samples{};
    auto deadline{clock::now()+std::chrono::seconds{120}}, quiet_start{deadline};
    while(clock::now() < deadline)
    {
        native.spin();
        bool current{native.matched(mode == "event" || mode == "ownership", mode == "absent")};
        if(!matched && current && mode != "absent" && mode != "ownership") { matched=true; native.facts("matched"); }
        if(counts.final && !delivered)
        { require(mode == "positive" && matched && !finishing); native.publish(reply_message);
          ++counts.reply; delivered=true; native.facts("delivered"); }
        auto command{input()};
        if(command == 'T')
        { require(mode == "event" && matched && !counts.event && !finishing);
          native.publish(event_message); ++counts.event; delivered=true; native.facts("event-sent"); }
        else if(command == 'Q')
        { require(!finishing && (mode == "absent" || mode == "ownership" || delivered)); finishing=true; quiet_start=clock::now(); }
        else { require(command == 0); }
        if(finishing)
        {
            ++quiet_samples;
            auto elapsed{std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now()-quiet_start).count()};
            if(elapsed >= 2000000000LL && quiet_samples >= 30)
            {
                require(!counts.excess && (mode != "positive" || counts.final == 1 && counts.reply == 1) &&
                        (mode != "event" || counts.event == 1) &&
                        ((mode != "absent" && mode != "ownership") || !counts.warmup && !counts.final && !counts.reply && !counts.event));
                emit("{\"kind\":\"complete\",\"backend\":",UWVM_JS_NATIVE_BACKEND,",\"pid\":",::getpid(),
                    ",\"warmup\":",counts.warmup,",\"final\":",counts.final,",\"reply\":",counts.reply,
                    ",\"event\":",counts.event,",\"excess\":",counts.excess,
                    ",\"quiet_samples\":",quiet_samples,",\"quiet_ns\":",elapsed,"}");
                return 0;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{5});
    }
    require(false); return 1;
}
}
int main(int argc, char** argv)
{
    try { return fixture::run(argc, argv); }
    catch(...) { fixture::emit("{\"kind\":\"failure\",\"backend\":",UWVM_JS_NATIVE_BACKEND,"}"); return 1; }
}
