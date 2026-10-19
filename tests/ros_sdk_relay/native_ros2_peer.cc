/**
 * @file native_ros2_peer.cc
 * @brief Independent exact-String DDS peer shared by every Wasm SDK language.
 *
 * Build only in the parent's qualified Linux lane with the original Jazzy SDK,
 * C++20, fast_io, rclcpp/rcl/rcutils/rmw/std_msgs/rosidl_typesupport_cpp and
 * fastrtps. This executable never starts a Worker/Gateway, opens their IPC, or
 * supplies a Guest identity. The external controller owns immutable products,
 * process births/pidfds, ARM/QEMU mappings, explicit grants and all deadlines.
 *
 * CLI: native_ros2_peer positive|absent. A constructor stops before main; after
 * proving its live target mapping the controller resumes it and writes I to
 * private stdin. Q is written only after actual SDK success and Worker exit0.
 * The four positive stdout lines and two absent lines are specified in NATIVE.md.
 * A successful publish return is insufficient: matching and the received final
 * message are independently checked before the sole native reply is sent.
 */
#include <fast_io.h>
#include <fast_io_dsal/string.h>
#include <fastdds/dds/log/Log.hpp>
#include <rcl/context.h>
#include <rclcpp/rclcpp.hpp>
#include <rcutils/logging.h>
#include <rmw/init.h>
#include <rmw/rmw.h>
#include <rosidl_runtime_c/type_hash.h>
#include <rosidl_typesupport_cpp/message_type_support.hpp>
#include <std_msgs/msg/string.hpp>
#include <algorithm>
#include <array>
#include <cerrno>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <fcntl.h>
#include <memory>
#include <poll.h>
#include <stdexcept>
#include <string_view>
#include <thread>
#include <unistd.h>

__attribute__((constructor)) static void sdk_peer_preinit_stop()
{ if(::raise(SIGSTOP) != 0) { __builtin_trap(); } }

namespace sdk_ros2_peer
{
    using message = std_msgs::msg::String;
    using clock = std::chrono::steady_clock;
    using gid = std::array<std::uint8_t, RMW_GID_STORAGE_SIZE>;
    static_assert(RMW_GID_STORAGE_SIZE == 16, "qualified Jazzy16-byte GID protocol required");
    static_assert(sizeof(rmw_gid_t{}.data) == 16);
    constexpr char out_topic[]{"/uwvm_sdk_relay/pub"}, in_topic[]{"/uwvm_sdk_relay/sub"};
    constexpr std::string_view namespc{"/uwvm_sdk_relay"}, peer_name{"sdk_peer"}, gateway_name{"sdk_gateway"};
    constexpr std::string_view full_peer{"/uwvm_sdk_relay/sdk_peer"}, full_gateway{"/uwvm_sdk_relay/sdk_gateway"};
    constexpr std::string_view native_type{"std_msgs/msg/String"};
    constexpr std::string_view identity{"RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18"};
    constexpr std::string_view implementation{"rmw_fastrtps_cpp"};
    // These names remain fixed for C, C++, Rust, TinyGo, AssemblyScript and JS.
    // The historical "js" substring is a test payload, not a language claim.
    constexpr std::string_view warmup_text{"warmup-js-relay"}, final_text{"roswasm-js-relay-out"}, reply_text{"roswasm-js-relay-in"};
    static_assert(warmup_text.size() == 15 && final_text.size() == 20 && reply_text.size() == 19);
    std::string_view phase{"input"}, field{"exception"};
    void require(bool value, std::string_view fixed_field = "contract")
    { if(!value) { field = fixed_field; throw std::runtime_error("fixed SDK peer refusal"); } }
    bool equal(char const* value, std::string_view expected) noexcept
    { return value && std::string_view{value} == expected; }
    bool nonzero(gid const& value) noexcept
    { return std::any_of(value.begin(), value.end(), [](auto byte) { return byte != 0; }); }
    gid copied(rmw_gid_t const& value)
    {
        require(equal(value.implementation_identifier, implementation), "implementation");
        gid result{}; std::copy_n(value.data, result.size(), result.begin());
        require(nonzero(result), "zero_gid"); return result;
    }
    template<typename Bytes> fast_io::string hexadecimal(Bytes const& bytes)
    {
        fast_io::string value; value.reserve(bytes.size() * 2);
        fast_io::ostring_ref_fast_io output{std::addressof(value)};
        for(auto byte : bytes) { fast_io::print(output, fast_io::mnp::right(fast_io::mnp::hex(static_cast<unsigned>(byte)), 2, '0')); }
        return value;
    }
    bool exact_hash(rosidl_type_hash_t const& hash)
    {
        if(hash.version != 1) { return false; }
        std::array<std::uint8_t, 32> value{}; std::copy_n(hash.value, value.size(), value.begin());
        auto text{hexadecimal(value)}; return identity.substr(7) == std::string_view{text.data(), text.size()};
    }
    void environment()
    {
        require(equal(::getenv("ROS_DOMAIN_ID"), "205") && equal(::getenv("RMW_IMPLEMENTATION"), implementation) &&
                equal(::getenv("ROS_SECURITY_ENABLE"), "false"), "environment");
        for(auto name : {"ROS_NAMESPACE", "ROS_SECURITY_STRATEGY", "ROS_SECURITY_KEYSTORE", "ROS_SECURITY_ENCLAVE_OVERRIDE",
                        "ROS_LOCALHOST_ONLY", "ROS_AUTOMATIC_DISCOVERY_RANGE", "ROS_STATIC_PEERS", "LD_PRELOAD",
                        "FASTRTPS_DEFAULT_PROFILES_FILE", "FASTDDS_DEFAULT_PROFILES_FILE", "RMW_FASTRTPS_USE_QOS_FROM_XML"})
        { require(::getenv(name) == nullptr, "environment"); }
    }
    void control_descriptors()
    {
        // Preserve message ordering without scanner read-ahead. Nonblocking
        // descriptors plus poll bound our own I/O; the parent also bounds DDS
        // creation/teardown and signals only retained owned process births.
        require(::signal(SIGPIPE, SIG_IGN) != SIG_ERR, "control");
        for(int fd : {STDIN_FILENO, STDOUT_FILENO})
        { int flags{::fcntl(fd, F_GETFL, 0)}; require(flags >= 0 && ::fcntl(fd, F_SETFL, flags | O_NONBLOCK) == 0, "control"); }
    }
    bool gate(char expected)
    {
        pollfd fd{STDIN_FILENO, POLLIN, 0}; int result{::poll(&fd, 1, 0)};
        if(result < 0 && errno == EINTR) { return false; }
        require(result >= 0 && !(fd.revents & (POLLERR | POLLNVAL)), "control");
        if(!result) { return false; }
        require(fd.revents & POLLIN, "control");
        char byte{}; auto end{fast_io::operations::read_some(fast_io::posix_io_observer{STDIN_FILENO}, &byte, &byte + 1)};
        require(end == &byte + 1 && byte == expected, "control"); return true;
    }
    void initial_gate()
    {
        auto end{clock::now() + std::chrono::seconds{20}};
        while(clock::now() < end) { if(gate('I')) { return; } std::this_thread::sleep_for(std::chrono::milliseconds{2}); }
        require(false, "deadline");
    }
    template<typename... Args> void emit(Args const&... args)
    {
        // Format into one fast_io-owned bounded string, then make one atomic
        // small pipe write. No remote name, payload or exception body is logged.
        auto line{fast_io::concat_fast_io(args..., "\n")}; require(line.size() <= 1024, "io");
        pollfd fd{STDOUT_FILENO, POLLOUT, 0}; int ready{::poll(&fd, 1, 1000)};
        require(ready > 0 && (fd.revents & POLLOUT) && !(fd.revents & (POLLERR | POLLHUP | POLLNVAL)), "io");
        auto end{fast_io::operations::write_some(fast_io::posix_io_observer{STDOUT_FILENO}, line.data(), line.data() + line.size())};
        require(end == line.data() + line.size(), "io");
    }
    struct logging_owner
    {
        static void discard(rcutils_log_location_t const*, int, char const*, rcutils_time_point_value_t, char const*, va_list*) noexcept {}
        logging_owner()
        {
            require(rcutils_logging_initialize() == RCUTILS_RET_OK, "logging"); rcutils_logging_set_output_handler(discard);
            // ClearConsumers internally Flushes. Only call it before any DDS
            // participant; the parent deadline also bounds this library call.
            eprosima::fastdds::dds::Log::ClearConsumers();
        }
        ~logging_owner() noexcept { auto result{rcutils_logging_shutdown()}; (void)result; }
    };
    struct context_owner
    {
        std::shared_ptr<rclcpp::Context> value{std::make_shared<rclcpp::Context>()};
        void close() { if(value->is_valid()) { require(value->shutdown("SDK peer complete"), "cleanup"); } }
        ~context_owner() noexcept { try { if(value->is_valid()) { value->shutdown("SDK peer cleanup"); } } catch(...) {} }
    };
    rclcpp::QoS qos()
    {
        rmw_qos_profile_t value{}; value.history = RMW_QOS_POLICY_HISTORY_KEEP_LAST; value.depth = 4;
        value.reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE; value.durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
        value.deadline = value.lifespan = value.liveliness_lease_duration = RMW_DURATION_INFINITE;
        value.liveliness = RMW_QOS_POLICY_LIVELINESS_AUTOMATIC; value.avoid_ros_namespace_conventions = false;
        return rclcpp::QoS{rclcpp::QoSInitialization::from_rmw(value), value};
    }
    bool same_qos(rclcpp::QoS const& a, rclcpp::QoS const& b)
    {
        auto x{a.get_rmw_qos_profile()}, y{b.get_rmw_qos_profile()};
        auto time = [](rmw_time_t first, rmw_time_t second) { return first.sec == second.sec && first.nsec == second.nsec; };
        return x.history == y.history && x.depth == y.depth && x.reliability == y.reliability && x.durability == y.durability &&
            time(x.deadline,y.deadline) && time(x.lifespan,y.lifespan) && time(x.liveliness_lease_duration,y.liveliness_lease_duration) &&
            x.liveliness == y.liveliness && x.avoid_ros_namespace_conventions == y.avoid_ros_namespace_conventions;
    }
    struct facts { gid own_pub{}, own_sub{}, gateway_pub{}, gateway_sub{}; };
    bool pending(rclcpp::TopicEndpointInfo const& info)
    {
        return (info.node_name() == "_NODE_NAME_UNKNOWN_" && info.node_namespace() == "_NODE_NAMESPACE_UNKNOWN_") ||
               (info.node_name() == "_CREATED_BY_BARE_DDS_APP_" && info.node_namespace() == "_CREATED_BY_BARE_DDS_APP_");
    }
    bool endpoint(rclcpp::TopicEndpointInfo const& info, std::string_view name, bool publisher, gid& output)
    {
        auto profile{info.qos_profile().get_rmw_qos_profile()}; output = info.endpoint_gid();
        // Stable metadata and raw GIDs remain mandatory even while the complete
        // official name/namespace pair is unresolved. WAIT never qualifies it.
        require(info.topic_type() == native_type && exact_hash(info.topic_type_hash()), "type_hash");
        require(info.endpoint_type() == (publisher ? rclcpp::EndpointType::Publisher : rclcpp::EndpointType::Subscription), "direction");
        require(nonzero(output), "zero_gid");
        require(profile.reliability == RMW_QOS_POLICY_RELIABILITY_RELIABLE && profile.durability == RMW_QOS_POLICY_DURABILITY_VOLATILE, "remote_qos");
        if(pending(info)) { return false; }
        require(info.node_name() == name && info.node_namespace() == namespc, "identity"); return true;
    }
    bool graph(rclcpp::Node& node, facts& value, bool positive)
    {
        auto own_pub{node.get_publishers_info_by_topic(in_topic)}, own_sub{node.get_subscriptions_info_by_topic(out_topic)};
        auto gateway_pub{node.get_publishers_info_by_topic(out_topic)}, gateway_sub{node.get_subscriptions_info_by_topic(in_topic)};
        require(own_pub.size() <= 1 && own_sub.size() <= 1 && gateway_pub.size() <= 1 && gateway_sub.size() <= 1, "graph_count");
        facts raw{}; bool a{}, b{}, c{}, d{};
        if(!own_pub.empty()) { a = endpoint(own_pub.front(), peer_name, true, raw.own_pub); }
        if(!own_sub.empty()) { b = endpoint(own_sub.front(), peer_name, false, raw.own_sub); }
        if(!gateway_pub.empty()) { c = endpoint(gateway_pub.front(), gateway_name, true, raw.gateway_pub); }
        if(!gateway_sub.empty()) { d = endpoint(gateway_sub.front(), gateway_name, false, raw.gateway_sub); }
        std::array<gid,4> all{raw.own_pub,raw.own_sub,raw.gateway_pub,raw.gateway_sub};
        for(std::size_t i{}; i != all.size(); ++i)
        { if(nonzero(all[i])) { for(std::size_t j{}; j != i; ++j) { require(all[i] != all[j], "duplicate_gid"); } } }
        if(!positive) { require(gateway_pub.empty() && gateway_sub.empty(), "unexpected_endpoint"); }
        if(a && b) { value.own_pub = raw.own_pub; value.own_sub = raw.own_sub; }
        if(a && b && c && d) { value = raw; return true; }
        return !positive && a && b;
    }
    struct observations
    {
        bool positive{}, matched{}; unsigned warmup{}, final_out{}, reply_send{}, excess{};
        gid gateway{}, warmup_source{};
        void receive(message::ConstSharedPtr const& value, rclcpp::MessageInfo const& information)
        {
            require(value != nullptr && !information.get_rmw_message_info().from_intra_process, "message_origin");
            auto origin{copied(information.get_rmw_message_info().publisher_gid)};
            if(!positive) { ++excess; require(false, "unexpected_message"); }
            std::string_view text{value->data.data(), value->data.size()};
            if(text == warmup_text && warmup == 0 && final_out == 0 && (!matched || origin == gateway))
            { ++warmup; warmup_source = origin; return; }
            if(text == final_text && matched && origin == gateway && final_out == 0)
            { ++final_out; return; }
            ++excess; require(false, "unexpected_message");
        }
    };
    template<typename... Tail> void marker(std::string_view name, std::string_view mode, facts const& value, Tail const&... tail)
    {
        emit(name," backend=2 mode=",mode," pid=",::getpid()," node=",full_peer," gateway_node=",full_gateway,
             " type=",native_type," out_topic=",std::string_view{out_topic}," in_topic=",std::string_view{in_topic},
             " domain=205 rmw=",implementation," hash=",identity," own_pub_gid=",hexadecimal(value.own_pub),
             " own_sub_gid=",hexadecimal(value.own_sub),tail...);
    }
}

int main(int argc, char** argv)
{
    using namespace sdk_ros2_peer;
    try
    {
        require(argc == 2 && (equal(argv[1],"positive") || equal(argv[1],"absent")), "mode");
        bool positive{equal(argv[1],"positive")}; std::string_view mode{positive ? "positive" : "absent"};
        environment(); control_descriptors(); initial_gate(); auto end{clock::now() + std::chrono::seconds{60}};
        phase = "initialization"; logging_owner logging; context_owner context;
        rclcpp::InitOptions init; init.auto_initialize_logging(false); init.set_domain_id(205);
        char const* arguments[]{"sdk-native-ros2"}; context.value->init(1,arguments,init);
        auto native{rcl_context_get_rmw_context(context.value->get_rcl_context().get())};
        require(native && equal(native->implementation_identifier,implementation) && native->actual_domain_id == 205 &&
                equal(rmw_get_implementation_identifier(),implementation), "context");
        facts selected; observations observed; observed.positive = positive; unsigned samples{}; std::uint64_t quiet_ns{};
        {
            rclcpp::NodeOptions options; options.context(context.value).use_global_arguments(false).enable_rosout(false);
            options.start_parameter_services(false).start_parameter_event_publisher(false).use_intra_process_comms(false);
            options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
            auto node{std::make_shared<rclcpp::Node>("sdk_peer","/uwvm_sdk_relay",options)};
            require(equal(node->get_fully_qualified_name(),full_peer), "identity");
            auto support{rosidl_typesupport_cpp::get_message_type_support_handle<message>()};
            require(support && support->get_type_hash_func && support->get_type_hash_func(support) && exact_hash(*support->get_type_hash_func(support)), "type_hash");
            auto configured{qos()}; auto publisher{node->create_publisher<message>(in_topic,configured)};
            auto subscriber{node->create_subscription<message>(out_topic,configured,
                [&](message::ConstSharedPtr value,rclcpp::MessageInfo const& information) { observed.receive(value,information); })};
            require(same_qos(publisher->get_actual_qos(),configured) && same_qos(subscriber->get_actual_qos(),configured), "local_qos");
            auto own_publisher{copied(publisher->get_gid())};
            // Jazzy accepts one ExecutorOptions object. Bind our independent
            // context explicitly rather than selecting the global context.
            rclcpp::ExecutorOptions executor_options; executor_options.context = context.value;
            rclcpp::executors::SingleThreadedExecutor executor{executor_options}; executor.add_node(node);
            auto step = [&]
            {
                require(clock::now() < end, "deadline"); executor.spin_some(std::chrono::milliseconds{2});
                require(clock::now() < end, "deadline"); std::this_thread::sleep_for(std::chrono::milliseconds{2});
            };
            phase = "own-discovery";
            // READY qualifies only our two endpoints. Positive Gateway endpoints
            // may still be absent because its publisher is deliberately lazy.
            while(!nonzero(selected.own_pub) || !nonzero(selected.own_sub))
            { facts next; graph(*node,next,positive); selected = next; step(); }
            require(selected.own_pub == own_publisher, "own_gid"); marker("READY_SDK_PEER",mode,selected);
            if(positive)
            {
                phase = "matching";
                while(true)
                {
                    facts next; bool complete{graph(*node,next,true)};
                    require(subscriber->get_publisher_count() <= 1 && publisher->get_subscription_count() <= 1, "match_count");
                    if(complete && subscriber->get_publisher_count() == 1 && publisher->get_subscription_count() == 1)
                    { require(next.own_pub == selected.own_pub && next.own_sub == selected.own_sub, "own_gid"); selected = next; break; }
                    step();
                }
                require(observed.warmup == 0 || observed.warmup_source == selected.gateway_pub, "warmup_source");
                observed.gateway = selected.gateway_pub; observed.matched = true;
                marker("MATCHED_SDK_PEER",mode,selected," gateway_pub_gid=",hexadecimal(selected.gateway_pub),
                       " gateway_sub_gid=",hexadecimal(selected.gateway_sub)," gateway_publishers=1 gateway_subscribers=1");
                phase = "delivery"; while(observed.final_out != 1) { step(); }
                facts current; require(graph(*node,current,true) && current.own_pub == selected.own_pub && current.own_sub == selected.own_sub &&
                    current.gateway_pub == selected.gateway_pub && current.gateway_sub == selected.gateway_sub &&
                    publisher->get_subscription_count() == 1 && subscriber->get_publisher_count() == 1, "reply_match");
                message reply; reply.data.assign(reply_text.data(),reply_text.size()); publisher->publish(reply); ++observed.reply_send;
                marker("DELIVERED_SDK_PEER",mode,selected," gateway_pub_gid=",hexadecimal(selected.gateway_pub),
                    " gateway_sub_gid=",hexadecimal(selected.gateway_sub)," gateway_publishers=1 gateway_subscribers=1 warmup=",observed.warmup,
                    " final_out=1 reply_send=1 excess=0");
            }
            phase = "completion";
            while(!gate('Q'))
            { if(!positive) { facts current; require(graph(*node,current,false) && subscriber->get_publisher_count() == 0 && publisher->get_subscription_count() == 0,"unexpected_endpoint"); } step(); }
            require(clock::now() < end,"deadline"); phase = "quiet"; auto begin{clock::now()}, quiet_end{begin + std::chrono::seconds{2}};
            while(clock::now() < quiet_end)
            {
                executor.spin_some(std::chrono::milliseconds{2});
                if(!positive) { facts current; require(graph(*node,current,false) && subscriber->get_publisher_count() == 0 && publisher->get_subscription_count() == 0,"unexpected_endpoint"); }
                require(observed.excess == 0 && observed.final_out == (positive ? 1U : 0U) && observed.reply_send == (positive ? 1U : 0U),"count");
                ++samples; std::this_thread::sleep_for(std::chrono::milliseconds{10});
            }
            quiet_ns = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now()-begin).count());
            require(samples >= 30 && samples <= 10000 && quiet_ns >= 2000000000ULL && quiet_ns <= 10000000000ULL,"quiet_budget");
            executor.cancel(); executor.remove_node(node);
        }
        phase = "cleanup"; context.close();
        if(positive)
        { marker("PASS_SDK_PEER",mode,selected," gateway_pub_gid=",hexadecimal(selected.gateway_pub)," gateway_sub_gid=",hexadecimal(selected.gateway_sub),
                 " gateway_publishers=1 gateway_subscribers=1 warmup=",observed.warmup," final_out=1 reply_send=1 excess=0 quiet_samples=",samples," quiet_ns=",quiet_ns); }
        else
        { marker("PASS_SDK_PEER",mode,selected," gateway_publishers=0 gateway_subscribers=0 warmup=0 final_out=0 reply_send=0 excess=0 quiet_samples=",samples," quiet_ns=",quiet_ns); }
        return 0;
    }
    catch(...)
    { try { fast_io::println(fast_io::err(),"FAIL_SDK_PEER backend=2 phase=",phase," field=",field); } catch(...) {} return 1; }
}
