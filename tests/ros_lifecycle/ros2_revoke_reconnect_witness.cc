/**
 * @file ros2_revoke_reconnect_witness.cc
 * @brief Independent signed DDS witness for one bounded revoke/fault/reconnect case.
 *
 * Build this C++20 test fixture against the qualified ARM Jazzy SDK. Link rclcpp,
 * rcl, rcutils, rmw, std_msgs, rosidl_typesupport_cpp, fastrtps and pthread. The
 * controller supplies /tmp/revoke-reconnect-keys and a literal domain75 Enforce
 * environment. It owns signing, Guest/Worker/Gateway processes, immutable image
 * qualification, authenticated IPC session evidence and the external deadline.
 * This process opens no IPC connection and spawns no process. Its two native
 * String subscriptions independently observe exact DDS bytes and publisher GIDs.
 *
 * Invoke: ros2-revoke-reconnect-witness /tmp/revoke-reconnect-keys
 * Private stdin gates are I before native initialization, N after the Guest's
 * checked revoke/stale/reopen marker, and F only after Guest success plus exit0.
 * The controller's Guest gates are separate. Each marker below is one LF-ended
 * line with closed public metadata; no payload, exception, key or path is logged.
 *
 * After I, one sixty-second deadline covers old discovery, exact baseline,
 * revocation acknowledgement, fresh discovery and the Guest completion gate.
 * F starts a separate two-second quiet observation, with at least thirty actual
 * executor samples. External owned-pidfd supervision also bounds native teardown.
 * Source/models or prepared ELF bytes alone establish no actual behavior result.
 */
#include <fast_io.h>
#include <fast_io_dsal/string.h>
#include <fastdds/dds/log/Log.hpp>
#include <rcl/context.h>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/node_interfaces/node_graph_interface.hpp>
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
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <poll.h>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <unistd.h>

namespace revoke_reconnect_witness
{
    using message = std_msgs::msg::String;
    using steady_clock = std::chrono::steady_clock;
    using gid = std::array<std::uint8_t, RMW_GID_STORAGE_SIZE>;
    // This selected Jazzy SDK declares sixteen native GID bytes. Every byte is
    // represented once in exactly thirty-two lowercase hexadecimal digits.
    // Neither another width nor padded/truncated native GIDs are admitted.
    static_assert(RMW_GID_STORAGE_SIZE == 16, "qualified Jazzy GIDs must contain exactly sixteen bytes");
    static_assert(sizeof(rmw_gid_t{}.data) == 16);

    constexpr std::string_view node_name{"revoke_witness"};
    constexpr std::string_view node_namespace{"/uwvm_revoke_test"};
    constexpr std::string_view full_node{"/uwvm_revoke_test/revoke_witness"};
    constexpr std::string_view gateway_name{"uwvm_ros2_gateway_probe"};
    constexpr std::string_view type_name{"std_msgs/msg/String"};
    constexpr std::string_view native_identity{"RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18"};
    constexpr std::string_view implementation{"rmw_fastrtps_cpp"};
    constexpr std::string_view keystore{"/tmp/revoke-reconnect-keys"};
    constexpr std::string_view security_root{"/tmp/revoke-reconnect-keys/enclaves/witness"};
    constexpr char dead_topic[]{"/uwvm_revoke_test/dead"};
    constexpr char live_topic[]{"/uwvm_revoke_test/live"};

    // Only the main executor thread evaluates fixture predicates. Record the
    // fixed source-owned refusal site without exposing a payload, exception,
    // credential or dynamically discovered name. This is diagnostic metadata;
    // it neither admits an unknown endpoint nor changes any Boolean condition.
    std::string_view refusal_site{"native_exception"};
    std::string_view refusal_field{"unspecified"};
    void require_at(bool accepted, std::string_view site, std::string_view field)
    {
        if(!accepted)
        {
            refusal_site = site;
            refusal_field = field;
            throw std::runtime_error("revoke/reconnect witness contract failed");
        }
    }
    void require(bool accepted)
    {
        require_at(accepted, "contract", "unspecified");
    }
    bool equal(char const* value, std::string_view expected) noexcept
    {
        return value != nullptr && std::string_view{value} == expected;
    }
    bool nonzero(gid const& value) noexcept
    {
        return std::any_of(value.begin(), value.end(), [](auto byte) { return byte != 0; });
    }
    gid copied(rmw_gid_t const& native)
    {
        require_at(equal(native.implementation_identifier, implementation), "callback_origin", "implementation");
        gid result{};
        std::copy_n(native.data, result.size(), result.begin());
        require_at(nonzero(result), "callback_origin", "zero_gid");
        return result;
    }
    template<typename Bytes> fast_io::string hexadecimal(Bytes const& value)
    {
        fast_io::string text;
        text.reserve(value.size() * 2);
        fast_io::ostring_ref_fast_io output{std::addressof(text)};
        for(auto byte : value)
        {
            fast_io::print(output, fast_io::mnp::right(fast_io::mnp::hex(static_cast<unsigned>(byte)), 2, '0'));
        }
        return text;
    }
    bool exact_hash(rosidl_type_hash_t const& hash)
    {
        if(hash.version != 1) { return false; }
        // A rosidl RIHS01 hash remains thirty-two bytes. It has no relationship
        // to the sixteen-byte RMW endpoint storage width checked above.
        std::array<std::uint8_t, 32> bytes{};
        std::copy_n(hash.value, bytes.size(), bytes.begin());
        auto text{hexadecimal(bytes)};
        return native_identity.substr(7) == std::string_view{text.data(), text.size()};
    }
    void environment()
    {
        require(equal(::getenv("ROS_DOMAIN_ID"), "75"));
        require(equal(::getenv("RMW_IMPLEMENTATION"), implementation));
        require(equal(::getenv("ROS_SECURITY_ENABLE"), "true"));
        require(equal(::getenv("ROS_SECURITY_STRATEGY"), "Enforce"));
        require(equal(::getenv("ROS_SECURITY_KEYSTORE"), keystore));
        for(auto name : {"ROS_SECURITY_ENCLAVE_OVERRIDE", "ROS_NAMESPACE", "ROS_LOCALHOST_ONLY",
                        "ROS_AUTOMATIC_DISCOVERY_RANGE", "ROS_STATIC_PEERS", "FASTRTPS_DEFAULT_PROFILES_FILE",
                        "FASTDDS_DEFAULT_PROFILES_FILE", "RMW_FASTRTPS_USE_QOS_FROM_XML", "LD_PRELOAD"})
        {
            require(::getenv(name) == nullptr);
        }
    }
    bool gate(char expected, int timeout_ms)
    {
        // One private pipe reader consumes exactly one readable byte. A scanner
        // must not read ahead across I/N/F; EOF or an unexpected byte fails.
        pollfd descriptor{STDIN_FILENO, POLLIN, 0};
        int ready{::poll(&descriptor, 1, timeout_ms)};
        if(ready < 0 && errno == EINTR) { return false; }
        require(ready >= 0);
        if(ready == 0) { return false; }
        require(!(descriptor.revents & (POLLERR | POLLNVAL)));
        char value{};
        auto end{fast_io::operations::read_some(fast_io::posix_io_observer{STDIN_FILENO}, &value, &value + 1)};
        require(end == &value + 1 && value == expected);
        return true;
    }
    void initial_gate()
    {
        auto end{steady_clock::now() + std::chrono::seconds{20}};
        while(steady_clock::now() < end)
        {
            if(gate('I', 20)) { return; }
        }
        require(false);
    }
    struct native_logging_owner
    {
        static void discard(rcutils_log_location_t const*, int, char const*, rcutils_time_point_value_t,
                            char const*, va_list*) noexcept {}
        native_logging_owner()
        {
            // Install public process-local sinks before context initialization,
            // which otherwise may print the private selected credential root.
            require(rcutils_logging_initialize() == RCUTILS_RET_OK);
            rcutils_logging_set_output_handler(discard);
            // FastDDS ClearConsumers invokes Flush internally. This sole call
            // is private initial logging-engine setup, before any DDS entity.
            // Never remove consumers or Flush after native context creation.
            eprosima::fastdds::dds::Log::ClearConsumers();
        }
        ~native_logging_owner() noexcept
        {
            auto const result{rcutils_logging_shutdown()};
            (void)result;
        }
    };
    struct context_owner
    {
        std::shared_ptr<rclcpp::Context> value{std::make_shared<rclcpp::Context>()};
        void close()
        {
            if(value->is_valid()) { require(value->shutdown("revoke/reconnect witness completed")); }
        }
        ~context_owner() noexcept
        {
            try { if(value->is_valid()) { value->shutdown("revoke/reconnect witness cleanup"); } } catch(...) {}
        }
    };
    void attest(std::shared_ptr<rclcpp::Context> const& context)
    {
        // These public APIs inspect the options actually used by this native
        // context. Environment text or source-selected settings alone do not
        // replace this attestation or the controller's signed-role verification.
        auto rcl_context{context->get_rcl_context()};
        require(rcl_context != nullptr);
        auto native{rcl_context_get_rmw_context(rcl_context.get())};
        require(native != nullptr && equal(native->implementation_identifier, implementation));
        require(native->actual_domain_id == 75 && equal(native->options.enclave, "/witness"));
        require(equal(native->options.implementation_identifier, implementation));
        require(native->options.security_options.enforce_security == RMW_SECURITY_ENFORCEMENT_ENFORCE);
        require(equal(native->options.security_options.security_root_path, security_root));
        require(equal(rmw_get_implementation_identifier(), implementation));
    }
    rclcpp::QoS configured_qos()
    {
        rmw_qos_profile_t profile{};
        profile.history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
        profile.depth = 8;
        profile.reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
        profile.durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
        profile.deadline = RMW_DURATION_INFINITE;
        profile.lifespan = RMW_DURATION_INFINITE;
        profile.liveliness = RMW_QOS_POLICY_LIVELINESS_AUTOMATIC;
        profile.liveliness_lease_duration = RMW_DURATION_INFINITE;
        profile.avoid_ros_namespace_conventions = false;
        return rclcpp::QoS{rclcpp::QoSInitialization::from_rmw(profile), profile};
    }
    bool actual_qos(rclcpp::QoS const& actual, rclcpp::QoS const& expected)
    {
        auto a{actual.get_rmw_qos_profile()}, b{expected.get_rmw_qos_profile()};
        auto same_time = [](rmw_time_t x, rmw_time_t y) { return x.sec == y.sec && x.nsec == y.nsec; };
        return a.history == b.history && a.depth == b.depth && a.reliability == b.reliability &&
               a.durability == b.durability && same_time(a.deadline, b.deadline) && same_time(a.lifespan, b.lifespan) &&
               a.liveliness == b.liveliness && same_time(a.liveliness_lease_duration, b.liveliness_lease_duration) &&
               a.avoid_ros_namespace_conventions == b.avoid_ros_namespace_conventions;
    }
    gid endpoint(rclcpp::TopicEndpointInfo const& value, std::string_view name, bool publisher,
                 std::string_view site)
    {
        auto qos{value.qos_profile().get_rmw_qos_profile()};
        // GraphCache can report fixed unknown-node placeholders before node
        // association or after retirement. Classify that refusal for diagnosis;
        // all placeholders are still refused by the exact same name predicate.
        require_at(value.node_name() == name, site,
                   value.node_name() == "_NODE_NAME_UNKNOWN_" ? "name_unknown" :
                   value.node_name() == "_CREATED_BY_BARE_DDS_APP_" ? "name_bare_dds" : "name_mismatch");
        require_at(value.node_namespace() == node_namespace, site,
                   value.node_namespace() == "_NODE_NAMESPACE_UNKNOWN_" ? "namespace_unknown" :
                   value.node_namespace() == "_CREATED_BY_BARE_DDS_APP_" ? "namespace_bare_dds" : "namespace_mismatch");
        require_at(value.topic_type() == type_name, site, "type");
        require_at(value.endpoint_type() == (publisher ? rclcpp::EndpointType::Publisher : rclcpp::EndpointType::Subscription),
                   site, "direction");
        require_at(exact_hash(value.topic_type_hash()), site, "hash");
        require_at(nonzero(value.endpoint_gid()), site, "zero_gid");
        // Remote DDS discovery does not supply reliable history/depth evidence.
        // Verify the advertised reliable/volatile interoperability fields here;
        // complete actual local subscription QoS is checked at entity creation.
        require_at(qos.reliability == RMW_QOS_POLICY_RELIABILITY_RELIABLE, site, "reliability");
        require_at(qos.durability == RMW_QOS_POLICY_DURABILITY_VOLATILE, site, "durability");
        return value.endpoint_gid();
    }
    bool own_subscriptions(rclcpp::Node& node, gid& dead, gid& live)
    {
        auto one = [&](char const* topic, gid& output)
        {
            auto entries{node.get_subscriptions_info_by_topic(topic)};
            require_at(entries.size() <= 1, "own_subscription_graph", "shape");
            if(entries.empty()) { return false; }
            output = endpoint(entries.front(), node_name, false, "own_subscription");
            return true;
        };
        bool dead_found{one(dead_topic, dead)}, live_found{one(live_topic, live)};
        return dead_found && live_found;
    }
    bool pending_publisher(rclcpp::TopicEndpointInfo const& value, std::string_view site)
    {
        // The official Jazzy GraphCache publishes these complete literal pairs
        // while its endpoint row lacks a ROS node association. They describe
        // an unresolved observation, never a qualified publisher. Incomplete
        // pairs, foreign names or namespaces still reach the strict endpoint()
        // predicate below and fail. Local witness subscriptions stay strict.
        bool pending{(value.node_name() == "_NODE_NAME_UNKNOWN_" &&
                      value.node_namespace() == "_NODE_NAMESPACE_UNKNOWN_") ||
                     (value.node_name() == "_CREATED_BY_BARE_DDS_APP_" &&
                      value.node_namespace() == "_CREATED_BY_BARE_DDS_APP_")};
        if(!pending) { return false; }
        // GraphCache supplies the remaining endpoint metadata independently of
        // node association. Validate every stable field before returning WAIT;
        // a placeholder never excuses a type/hash/GID/direction/QoS mismatch.
        auto qos{value.qos_profile().get_rmw_qos_profile()};
        require_at(value.topic_type() == type_name, site, "type");
        require_at(value.endpoint_type() == rclcpp::EndpointType::Publisher, site, "direction");
        require_at(exact_hash(value.topic_type_hash()), site, "hash");
        require_at(nonzero(value.endpoint_gid()), site, "zero_gid");
        require_at(qos.reliability == RMW_QOS_POLICY_RELIABILITY_RELIABLE, site, "reliability");
        require_at(qos.durability == RMW_QOS_POLICY_DURABILITY_VOLATILE, site, "durability");
        return true;
    }
    bool publisher(rclcpp::Node& node, char const* topic, gid& output)
    {
        auto entries{node.get_publishers_info_by_topic(topic)};
        require_at(entries.size() <= 1, "old_publisher_graph", "shape");
        if(entries.empty()) { return false; }
        if(pending_publisher(entries.front(), "old_publisher"))
        {
            // Expose only a stable raw identity for this snapshot's duplicate
            // checks. The false return keeps it out of selected public facts.
            output = entries.front().endpoint_gid();
            return false;
        }
        output = endpoint(entries.front(), gateway_name, true, "old_publisher");
        return true;
    }
    template<std::size_t N> bool distinct(std::array<gid, N> const& values)
    {
        for(std::size_t i{}; i != N; ++i)
        {
            if(!nonzero(values[i])) { return false; }
            for(std::size_t j{}; j != i; ++j) { if(values[i] == values[j]) { return false; } }
        }
        return true;
    }
    enum class phase { old_discovery, baseline, await_revoke, fresh_discovery, after_delivery, quiet };
    struct observations
    {
        // Only the main thread calls spin_some and mutates this record. Native
        // callbacks retain no message, payload copy, Guest pointer or ROS entity.
        phase current{phase::old_discovery};
        unsigned dead_before{}, live_before{}, live_after{}, unauthorized_dead{}, fault_live{}, unknown{};
        unsigned warmup_dead{}, warmup_live{}, warmup_new{};
        gid old_dead{}, old_live{}, new_live{}, dead_warmup_source{}, live_warmup_source{}, new_warmup_source{};
        bool old_selected{}, new_selected{};

        gid source(message::ConstSharedPtr const& value, rclcpp::MessageInfo const& info)
        {
            auto const& native{info.get_rmw_message_info()};
            require_at(value != nullptr && !native.from_intra_process, "callback_source", "null_or_intraprocess");
            return copied(native.publisher_gid);
        }
        void dead(message::ConstSharedPtr const& value, rclcpp::MessageInfo const& info)
        {
            auto origin{source(value, info)};
            std::string_view text{value->data.data(), value->data.size()};
            if(text == "WARMUP-DEAD" && (current == phase::old_discovery || current == phase::baseline) &&
               warmup_dead == 0 && (!old_selected || origin == old_dead))
            {
                dead_warmup_source = origin;
                ++warmup_dead;
                return;
            }
            if(text == "BEFORE-DEAD" && current == phase::baseline && old_selected && origin == old_dead && dead_before == 0)
            {
                ++dead_before;
                return;
            }
            // A duplicate baseline, any later warmup, another source or any
            // post-revoke text is an independent native side effect and fails.
            ++unauthorized_dead;
            require_at(false, "dead_delivery", text == "WARMUP-DEAD" ? "warmup_rejected" :
                       text == "BEFORE-DEAD" ? "baseline_rejected" : "unexpected_text");
        }
        void live(message::ConstSharedPtr const& value, rclcpp::MessageInfo const& info)
        {
            auto origin{source(value, info)};
            std::string_view text{value->data.data(), value->data.size()};
            if(text == "FAULT-LIVE") { ++fault_live; require_at(false, "live_delivery", "fault_text"); }
            if(text == "WARMUP-LIVE" && (current == phase::old_discovery || current == phase::baseline) &&
               warmup_live == 0 && (!old_selected || origin == old_live))
            {
                live_warmup_source = origin;
                ++warmup_live;
                return;
            }
            if(text == "BEFORE-LIVE" && current == phase::baseline && old_selected && origin == old_live && live_before == 0)
            {
                ++live_before;
                return;
            }
            if(text == "RECONNECT-WARMUP" && (current == phase::fresh_discovery || current == phase::after_delivery) &&
               warmup_new == 0 && (!new_selected || origin == new_live) && origin != old_dead && origin != old_live)
            {
                new_warmup_source = origin;
                ++warmup_new;
                return;
            }
            if(text == "AFTER-LIVE" && (current == phase::after_delivery || current == phase::quiet) &&
               new_selected && origin == new_live && live_after == 0)
            {
                ++live_after;
                return;
            }
            ++unknown;
            require_at(false, "live_delivery", text == "WARMUP-LIVE" ? "warmup_rejected" :
                       text == "BEFORE-LIVE" ? "baseline_rejected" :
                       text == "RECONNECT-WARMUP" ? "reconnect_rejected" :
                       text == "AFTER-LIVE" ? "after_rejected" : "unexpected_text");
        }
        void clean() const
        {
            require_at(unauthorized_dead == 0 && fault_live == 0 && unknown == 0, "callback_counters", "forbidden_effect");
            require_at(dead_before <= 1 && live_before <= 1 && live_after <= 1, "callback_counters", "positive_count");
            require_at(warmup_dead <= 1 && warmup_live <= 1 && warmup_new <= 1, "callback_counters", "warmup_count");
        }
        void frozen(unsigned dead_warmups, unsigned live_warmups) const
        {
            clean();
            require_at(dead_before == 1 && live_before == 1, "frozen_counters", "baseline_count");
            require_at(warmup_dead == dead_warmups && warmup_live == live_warmups, "frozen_counters", "warmup_count");
        }
    };
    struct public_facts
    {
        pid_t process{::getpid()};
        gid dead_sub{}, live_sub{}, old_dead{}, old_live{}, new_live{};
        fast_io::string dead_sub_text, live_sub_text, old_dead_text, old_live_text, new_live_text;
    };
    template<typename... Tail> void marker(std::string_view name, public_facts const& facts, Tail const&... tail)
    {
        fast_io::println(fast_io::out(), name, " pid=", facts.process, " node=", full_node,
                         " enclave=/witness domain=75 rmw=", implementation, " type=", type_name,
                         " dead_topic=", std::string_view{dead_topic}, " live_topic=", std::string_view{live_topic},
                         " dead_sub_gid=", facts.dead_sub_text, " live_sub_gid=", facts.live_sub_text, tail...);
    }
}

int main(int argc, char** argv)
{
    using namespace revoke_reconnect_witness;
    std::string_view failure_phase{"input"};
    try
    {
        require(argc == 2 && equal(argv[1], keystore));
        environment();
        initial_gate();
        auto exchange_end{steady_clock::now() + std::chrono::seconds{60}};
        native_logging_owner logging;
        context_owner context;
        failure_phase = "initialization";
        rclcpp::InitOptions initialization;
        initialization.auto_initialize_logging(false);
        char const* arguments[]{"revoke-witness", "--ros-args", "--enclave", "/witness"};
        context.value->init(4, arguments, initialization);
        attest(context.value);
        public_facts facts;
        unsigned final_warmup_dead{}, final_warmup_live{}, final_warmup_new{}, quiet_samples{};
        std::uint64_t quiet_ns{};
        {
            observations observed;
            rclcpp::NodeOptions options;
            options.context(context.value).use_global_arguments(true).enable_rosout(false);
            options.start_parameter_services(false).start_parameter_event_publisher(false).use_intra_process_comms(false);
            options.parameter_overrides({rclcpp::Parameter("start_type_description_service", false)});
            auto node{std::make_shared<rclcpp::Node>("revoke_witness", "/uwvm_revoke_test", options)};
            require(equal(node->get_fully_qualified_name(), full_node));
            auto support{rosidl_typesupport_cpp::get_message_type_support_handle<message>()};
            require(support != nullptr && support->get_type_hash_func != nullptr);
            auto hash{support->get_type_hash_func(support)};
            require(hash != nullptr && exact_hash(*hash));
            auto qos{configured_qos()};
            auto dead{node->create_subscription<message>(dead_topic, qos,
                [&](message::ConstSharedPtr value, rclcpp::MessageInfo const& info) { observed.dead(value, info); })};
            auto live{node->create_subscription<message>(live_topic, qos,
                [&](message::ConstSharedPtr value, rclcpp::MessageInfo const& info) { observed.live(value, info); })};
            require(actual_qos(dead->get_actual_qos(), qos) && actual_qos(live->get_actual_qos(), qos));
            rclcpp::ExecutorOptions executor_options;
            executor_options.context = context.value;
            rclcpp::executors::SingleThreadedExecutor executor{executor_options};
            executor.add_node(node);
            auto step = [&]
            {
                require_at(steady_clock::now() < exchange_end, "exchange_deadline", "before_spin");
                executor.spin_some(std::chrono::milliseconds{2});
                observed.clean();
                require_at(steady_clock::now() < exchange_end, "exchange_deadline", "after_spin");
                std::this_thread::sleep_for(std::chrono::milliseconds{2});
                require_at(steady_clock::now() < exchange_end, "exchange_deadline", "after_sleep");
            };
            failure_phase = "own-discovery";
            while(!own_subscriptions(*node, facts.dead_sub, facts.live_sub)) { step(); }
            require(steady_clock::now() < exchange_end);
            require(distinct(std::array<gid, 2>{facts.dead_sub, facts.live_sub}));
            facts.dead_sub_text = hexadecimal(facts.dead_sub);
            facts.live_sub_text = hexadecimal(facts.live_sub);
            marker("READY_REVOKE_WITNESS", facts);

            failure_phase = "old-discovery";
            for(;;)
            {
                step();
                gid raw_dead{}, raw_live{};
                bool dead_found{publisher(*node, dead_topic, raw_dead)};
                bool live_found{publisher(*node, live_topic, raw_live)};
                // Pending names cannot hide a publisher/subscriber alias or a
                // duplicate identity across the two topics. Missing rows may
                // wait; present rows retain these constraints before any retry.
                if(nonzero(raw_dead)) { require(distinct(std::array<gid, 3>{facts.dead_sub, facts.live_sub, raw_dead})); }
                if(nonzero(raw_live)) { require(distinct(std::array<gid, 3>{facts.dead_sub, facts.live_sub, raw_live})); }
                if(nonzero(raw_dead) && nonzero(raw_live)) { require(raw_dead != raw_live); }
                if(dead_found && live_found && dead->get_publisher_count() == 1 && live->get_publisher_count() == 1)
                {
                    facts.old_dead = raw_dead;
                    facts.old_live = raw_live;
                    break;
                }
            }
            require(distinct(std::array<gid, 4>{facts.dead_sub, facts.live_sub, facts.old_dead, facts.old_live}));
            require(observed.warmup_dead == 0 || observed.dead_warmup_source == facts.old_dead);
            require(observed.warmup_live == 0 || observed.live_warmup_source == facts.old_live);
            require(steady_clock::now() < exchange_end);
            observed.old_dead = facts.old_dead;
            observed.old_live = facts.old_live;
            observed.old_selected = true;
            observed.current = phase::baseline;
            facts.old_dead_text = hexadecimal(facts.old_dead);
            facts.old_live_text = hexadecimal(facts.old_live);
            marker("MATCHED_REVOKE_OLD", facts, " old_dead_pub_gid=", facts.old_dead_text,
                   " old_live_pub_gid=", facts.old_live_text, " dead_publishers=1 live_publishers=1");

            failure_phase = "baseline";
            while(observed.dead_before != 1 || observed.live_before != 1) { step(); }
            observed.current = phase::await_revoke;
            marker("BASELINE_REVOKE_WITNESS", facts, " old_dead_pub_gid=", facts.old_dead_text,
                   " old_live_pub_gid=", facts.old_live_text, " dead_before=1 live_before=1");
            failure_phase = "revoke-gate";
            while(!gate('N', 0)) { step(); }
            require(steady_clock::now() < exchange_end);
            executor.spin_some(std::chrono::milliseconds{2});
            observed.clean();
            require(steady_clock::now() < exchange_end);
            unsigned frozen_dead_warmups{observed.warmup_dead}, frozen_live_warmups{observed.warmup_live};
            observed.frozen(frozen_dead_warmups, frozen_live_warmups);
            observed.current = phase::fresh_discovery;
            marker("FROZEN_REVOKE_WITNESS", facts, " old_dead_pub_gid=", facts.old_dead_text,
                   " old_live_pub_gid=", facts.old_live_text, " dead_total=1 live_before=1 unauthorized_dead=0");

            failure_phase = "fresh-discovery";
            for(;;)
            {
                step();
                observed.frozen(frozen_dead_warmups, frozen_live_warmups);
                gid own_dead{}, own_live{}, candidate{};
                require_at(own_subscriptions(*node, own_dead, own_live) && own_dead == facts.dead_sub && own_live == facts.live_sub,
                           "fresh_own_subscriptions", "identity");
                auto dead_writers{node->get_publishers_info_by_topic(dead_topic)};
                require_at(dead_writers.size() <= 1, "fresh_dead_graph", "shape");
                bool dead_writer_present{!dead_writers.empty()};
                // An old advertisement may retire asynchronously. A different
                // dead publisher already resurrects revoked authority and fails.
                if(!dead_writers.empty())
                {
                    auto const& writer{dead_writers.front()};
                    // A pending row must still be the pinned retired writer.
                    // Classify and reject a resurrected dead GID before WAIT;
                    // a recognized placeholder grants no revoked authority.
                    auto site{writer.endpoint_gid() == facts.old_dead ? "fresh_old_dead" : "fresh_dead_candidate"};
                    require_at(writer.endpoint_gid() == facts.old_dead, "fresh_dead_graph", "old_gid");
                    if(!pending_publisher(writer, site))
                    {
                        require_at(endpoint(writer, gateway_name, true, site) == facts.old_dead, "fresh_dead_graph", "old_gid");
                    }
                }
                auto live_writers{node->get_publishers_info_by_topic(live_topic)};
                // Revocation may retire the dead advertisement before the
                // controller kills the old process. Its live advertisement
                // can therefore overlap the replacement until DDS discovery
                // observes retirement. Only this fresh phase permits that
                // bounded transition: the pinned old GID and at most one
                // fully qualified distinct candidate. Final admission still
                // requires the old advertisement to be absent and exactly
                // one actual matched replacement. A third, duplicate or two
                // fresh writers is never a legitimate retirement transition.
                require_at(live_writers.size() <= 2, "fresh_live_graph", "shape");
                bool old_live_present{}, fresh_live_present{}, association_pending{};
                for(auto const& writer : live_writers)
                {
                    // Count and distinguish every raw endpoint GID even while
                    // ROS node association is pending. Duplicates, two fresh
                    // writers or reuse of a witness/dead GID never become WAIT.
                    auto site{writer.endpoint_gid() == facts.old_live ? "fresh_old_live" : "fresh_live_candidate"};
                    auto identity{writer.endpoint_gid()};
                    if(identity == facts.old_live)
                    {
                        require_at(!old_live_present, "fresh_live_graph", "duplicate_old");
                        old_live_present = true;
                    }
                    else
                    {
                        require_at(!fresh_live_present, "fresh_live_graph", "duplicate_fresh");
                        require_at(distinct(std::array<gid, 5>{facts.dead_sub, facts.live_sub, facts.old_dead, facts.old_live, identity}),
                                   "fresh_live_graph", "independent_gid");
                        candidate = identity;
                        fresh_live_present = true;
                    }
                    if(pending_publisher(writer, site)) { association_pending = true; }
                    else { require_at(endpoint(writer, gateway_name, true, site) == identity, "fresh_live_graph", "independent_gid"); }
                }
                // No row with an unresolved node association can contribute to
                // a successful snapshot. The unchanged shared sixty-second
                // deadline and executor/counter checks bound every retry.
                if(dead_writer_present || association_pending) { continue; }
                if(old_live_present || !fresh_live_present) { continue; }
                if(dead->get_publisher_count() != 0 || live->get_publisher_count() != 1) { continue; }
                require_at(observed.warmup_new == 0 || observed.new_warmup_source == candidate, "fresh_live_graph", "warmup_origin");
                facts.new_live = candidate;
                break;
            }
            observed.new_live = facts.new_live;
            observed.new_selected = true;
            observed.current = phase::after_delivery;
            require_at(observed.live_after == 0, "fresh_snapshot", "delivery_count");
            require_at(steady_clock::now() < exchange_end, "exchange_deadline", "before_new_marker");
            facts.new_live_text = hexadecimal(facts.new_live);
            // This snapshots actual new endpoint matching before the Guest's
            // final publication and close. A GID change alone does not attest
            // a new authenticated IPC session: that is controller-owned proof.
            marker("MATCHED_REVOKE_NEW", facts, " old_dead_pub_gid=", facts.old_dead_text,
                   " old_live_pub_gid=", facts.old_live_text, " new_live_pub_gid=", facts.new_live_text,
                   " dead_publishers=0 live_publishers=1 dead_total=1 live_before=1 live_after=0");
            failure_phase = "survivor-delivery";
            while(observed.live_after != 1)
            {
                step();
                observed.frozen(frozen_dead_warmups, frozen_live_warmups);
            }
            // A successful publish call alone does not prove DDS delivery.
            // The Guest remains alive with its publisher retained behind Q.
            // Only this exact callback/GID observation permits the controller
            // to release Q, let the Guest close, and later send native F.
            marker("DELIVERED_REVOKE_NEW", facts, " old_dead_pub_gid=", facts.old_dead_text,
                   " old_live_pub_gid=", facts.old_live_text, " new_live_pub_gid=", facts.new_live_text,
                   " dead_publishers=0 live_publishers=1 dead_total=1 live_before=1 live_after=1");
            failure_phase = "completion-gate";
            while(!gate('F', 0))
            {
                step();
                observed.frozen(frozen_dead_warmups, frozen_live_warmups);
            }
            require(steady_clock::now() < exchange_end);

            failure_phase = "quiet-observation";
            observed.current = phase::quiet;
            auto quiet_begin{steady_clock::now()}, quiet_end{quiet_begin + std::chrono::seconds{2}};
            while(steady_clock::now() < quiet_end)
            {
                executor.spin_some(std::chrono::milliseconds{2});
                observed.frozen(frozen_dead_warmups, frozen_live_warmups);
                ++quiet_samples;
                std::this_thread::sleep_for(std::chrono::milliseconds{10});
            }
            quiet_ns = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(steady_clock::now() - quiet_begin).count());
            require(quiet_samples >= 30 && quiet_ns >= 2000000000ULL);
            observed.frozen(frozen_dead_warmups, frozen_live_warmups);
            require(observed.live_after == 1);
            final_warmup_dead = observed.warmup_dead;
            final_warmup_live = observed.warmup_live;
            final_warmup_new = observed.warmup_new;
            executor.cancel();
            executor.remove_node(node);
        }
        failure_phase = "cleanup";
        context.close();
        marker("PASS_REVOKE_RECONNECT_WITNESS", facts, " old_dead_pub_gid=", facts.old_dead_text,
               " old_live_pub_gid=", facts.old_live_text, " new_live_pub_gid=", facts.new_live_text,
               " dead_publishers=0 live_publishers=1 dead_total=1 live_before=1 live_after=1",
               " unauthorized_dead=0 fault_live=0 unknown=0 quiet_samples=", quiet_samples, " quiet_ns=", quiet_ns,
               " warmup_dead=", final_warmup_dead, " warmup_live=", final_warmup_live, " warmup_new=", final_warmup_new);
        return 0;
    }
    catch(...)
    {
        // Native exception strings can disclose private paths or peer values.
        // Report only fixed source-owned phase/site/field identifiers. A
        // diagnosed refusal still exits one and is never a success marker.
        fast_io::println(fast_io::err(), "FAIL_REVOKE_RECONNECT_WITNESS phase=", failure_phase,
                         " site=", refusal_site, " field=", refusal_field);
        return 1;
    }
}
