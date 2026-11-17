/** @file
 * @brief Concrete ROS 2 communication objects for the scene provider worker.
 * @details Compile this file only in an explicitly deployed ROS native worker.
 * It creates real independent rclcpp contexts and nodes, with no global argument
 * inheritance, rosout, parameter services/events, type-description/logger service or ROS-time
 * thread. Trusted transport/security/RMW configuration belongs to the worker's
 * deployment. Guests cannot choose URLs, libraries, ROS arguments or credentials.
 * Complete generated message codecs are registered by trusted native code;
 * unknown types are refused before an endpoint is created. No old ROS-v1 policy
 * or protocol is inherited. The callback queue is bounded, but middleware receive
 * allocation occurs before callbacks: native process/DDS budgets remain mandatory.
 */
#pragma once
#include "external_process_only.h"
#include "../communication.h"
#include "../communication_codecs.h"
#include "communication_network.h"
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/node_interfaces/node_base.hpp>
#include <rclcpp/node_interfaces/node_graph.hpp>
#include <rclcpp/node_interfaces/node_topics.hpp>
#include <rclcpp/node_interfaces/node_services.hpp>
#include <rclcpp/node_interfaces/node_timers.hpp>
#include <rclcpp/node_interfaces/node_waitables.hpp>
#include <rmw/rmw.h>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_srvs/srv/set_bool.hpp>
#include <fast_io.h>

namespace uwvm2::imported::ros::scene::communication::ros2
{
    inline rmw_qos_profile_t native_qos(qos const& q)
    {
        rmw_qos_profile_t result = rmw_qos_profile_default;
        result.history = q.history == 1 ? RMW_QOS_POLICY_HISTORY_KEEP_LAST : RMW_QOS_POLICY_HISTORY_KEEP_ALL;
        result.depth = q.depth;
        result.reliability = q.reliability == 1 ? RMW_QOS_POLICY_RELIABILITY_RELIABLE : RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT;
        result.durability = q.durability == 1 ? RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL : RMW_QOS_POLICY_DURABILITY_VOLATILE;
        result.liveliness = q.liveliness == 1 ? RMW_QOS_POLICY_LIVELINESS_AUTOMATIC : RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC;
        result.deadline = {q.deadline.seconds, q.deadline.nanoseconds};
        result.lifespan = {q.lifespan.seconds, q.lifespan.nanoseconds};
        result.liveliness_lease_duration = {q.lease.seconds, q.lease.nanoseconds};
        result.avoid_ros_namespace_conventions = false;
        return result;
    }

    inline bool observed_qos(rmw_qos_profile_t const& p, qos& output) noexcept
    {
        qos q;
        if(p.history == RMW_QOS_POLICY_HISTORY_KEEP_LAST) { q.history = 1; }
        else if(p.history == RMW_QOS_POLICY_HISTORY_KEEP_ALL) { q.history = 2; }
        else
        {
            return false;
        }
        if(p.reliability == RMW_QOS_POLICY_RELIABILITY_RELIABLE) { q.reliability = 1; }
        else if(p.reliability == RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT) { q.reliability = 2; }
        else
        {
            return false;
        }
        if(p.durability == RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL) { q.durability = 1; }
        else if(p.durability == RMW_QOS_POLICY_DURABILITY_VOLATILE) { q.durability = 2; }
        else
        {
            return false;
        }
        if(p.liveliness == RMW_QOS_POLICY_LIVELINESS_AUTOMATIC) { q.liveliness = 1; }
        else if(p.liveliness == RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC) { q.liveliness = 2; }
        else
        {
            return false;
        }
        q.depth = p.depth;
        q.deadline = {p.deadline.sec, p.deadline.nsec};
        q.lifespan = {p.lifespan.sec, p.lifespan.nsec};
        q.lease = {p.liveliness_lease_duration.sec, p.liveliness_lease_duration.nsec};
        if(p.avoid_ros_namespace_conventions || q.deadline.nanoseconds >= 1000000000 || q.lifespan.nanoseconds >= 1000000000 ||
           q.lease.nanoseconds >= 1000000000)
        {
            return false;
        }
        output = q;
        return true;
    }

    inline bool equivalent(qos const& a, qos const& b) noexcept
    {
        auto right = b;
        if(a.history == 2) { right.depth = a.depth; }
        return a == right;
    }

    struct context final : native_context
    {
        std::shared_ptr<rclcpp::Context> value;
        std::shared_ptr<rclcpp::executors::SingleThreadedExecutor> executor;
        ::fast_io::native_thread worker;
        std::mutex stopping;
        std::atomic_bool functioning{true};

        bool live() const noexcept override { return functioning.load(std::memory_order_acquire) && value && value->is_valid(); }

        error shutdown() noexcept override
        {
            std::lock_guard lock{stopping};
            functioning.store(false, std::memory_order_release);
            try
            {
                if(executor) { executor->cancel(); }
                if(value && value->is_valid()) { value->shutdown("scene context shutdown"); }
                if(worker.joinable()) { worker.join(); }
                return error::ok;
            }
            catch(...)
            {
                return error::io;
            }
        }

        ~context() override { (void)shutdown(); }
    };

    /** @brief Minimal native interfaces: no implicit ROS-time/parameter endpoint.
     * @details rclcpp::Node always attaches TimeSource, which subscribes to
     * /parameter_events even when parameter services and publishers are disabled.
     * This bundle constructs only the explicitly needed base/graph/topics/services
     * interfaces. QoS override parameters and topic statistics remain disabled.
     */
    struct quiet_node
    {
        rclcpp::node_interfaces::NodeBaseInterface::SharedPtr base;
        rclcpp::node_interfaces::NodeGraphInterface::SharedPtr graph;
        rclcpp::node_interfaces::NodeTimersInterface::SharedPtr timers;
        rclcpp::node_interfaces::NodeTopicsInterface::SharedPtr topics;
        rclcpp::node_interfaces::NodeServicesInterface::SharedPtr services;

        quiet_node(node_plan const& plan, std::shared_ptr<rclcpp::Context> const& context)
        {
            rclcpp::NodeOptions options;
            options.context(context).use_global_arguments(false).enable_rosout(false).enable_topic_statistics(false).use_intra_process_comms(false);
            base = ::uwvm2::utils::container::make_shared<rclcpp::node_interfaces::NodeBase>(plan.name.c_str(),
                                                                                             plan.name_space.c_str(),
                                                                                             context,
                                                                                             *options.get_rcl_node_options(),
                                                                                             false,
                                                                                             false);
            graph = ::uwvm2::utils::container::make_shared<rclcpp::node_interfaces::NodeGraph>(base.get());
            timers = ::uwvm2::utils::container::make_shared<rclcpp::node_interfaces::NodeTimers>(base.get());
            topics = ::uwvm2::utils::container::make_shared<rclcpp::node_interfaces::NodeTopics>(base.get(), timers.get());
            services = ::uwvm2::utils::container::make_shared<rclcpp::node_interfaces::NodeServices>(base.get());
        }

        auto get_node_topics_interface() { return topics; }

        template <class Native>
        auto create_publisher(char const* name, rclcpp::QoS const& q)
        { return rclcpp::create_publisher<Native>(*this, name, q); }

        template <class Native, class Callback>
        auto create_subscription(char const* name, rclcpp::QoS const& q, Callback&& callback)
        { return rclcpp::create_subscription<Native>(*this, name, q, std::forward<Callback>(callback)); }

        char const* get_name() const noexcept { return base->get_name(); }

        char const* get_namespace() const noexcept { return base->get_namespace(); }
    };

    struct node final : native_node
    {
        std::shared_ptr<context> parent;
        std::shared_ptr<quiet_node> value;
        std::mutex graph_queries;

        ~node() override
        {
            try
            {
                if(parent && parent->executor && value) { parent->executor->remove_node(value->base); }
            }
            catch(...)
            {
            }
        }
    };

    template <class Native, class Codec>
    class topic final : public native_topic
    {
        std::shared_ptr<node> parent;
        std::shared_ptr<rclcpp::Publisher<Native>> publisher;
        std::shared_ptr<rclcpp::Subscription<Native>> subscription;
        qos configured;
        endpoint_identity evidence;
        Codec codec;

        struct capture
        {
            std::shared_ptr<receive_queue> queue;
            ::uwvm2::utils::container::std_vector<std::uint8_t> scratch;
            std::mutex mutex;
            handle source{}, generation{};
            std::size_t maximum_message{};
            Codec codec;

            void receive(std::shared_ptr<Native const> const& value, rclcpp::MessageInfo const& information) noexcept
            {
                std::lock_guard lock{mutex};
                std::size_t size{};
                if(!value || !codec.encode(*value, scratch, size)||size>maximum_message) { return; }
                store({scratch.data(),size},information);
            }
            void receive_serialized(std::shared_ptr<rclcpp::SerializedMessage const> const& value,rclcpp::MessageInfo const& information)noexcept
            {
                if(!value)return;
                auto const& buffer=value->get_rcl_serialized_message();
                if(!buffer.buffer||buffer.buffer_length>buffer.buffer_capacity)return;
                std::size_t size{};
                if(!codec.native_extent({buffer.buffer,buffer.buffer_length},size)||size>maximum_message)return;
                store({buffer.buffer,size},information);
            }
            void store(wire::bytes message,rclcpp::MessageInfo const& information)noexcept
            {
                auto const& actual = information.get_rmw_message_info();
                metadata m;
                m.source = source;
                m.generation = generation;
                if(actual.source_timestamp) { m.source_nanoseconds = actual.source_timestamp; }
                if(actual.received_timestamp) { m.received_nanoseconds = actual.received_timestamp; }
                static_assert(RMW_GID_STORAGE_SIZE <= 32);
                m.publisher_bytes = RMW_GID_STORAGE_SIZE;
                std::copy_n(actual.publisher_gid.data, RMW_GID_STORAGE_SIZE, m.publisher.begin());
                (void)queue->push(m,message);
            }
        };

        std::shared_ptr<capture> ingress;

    public:
        /** @brief Detach native QoS waitables before releasing an endpoint.
         * @details Jazzy's executor can retain event handlers (and therefore
         * their rcl publisher handles) until its entity collection is rebuilt.
         * Explicit removal plus guard notification makes close/revocation stop
         * discovery participation without waiting for an unrelated ROS event.
         */
        ~topic() override
        {
            if(!parent || !parent->value) { return; }
            auto base = parent->value->base;
            auto group = base->get_default_callback_group();
            rclcpp::node_interfaces::NodeWaitables waitables{base.get()};
            if(publisher)
            {
                for(auto const& [type, event]: publisher->get_event_handlers())
                {
                    static_cast<void>(type);
                    waitables.remove_waitable(event, group);
                }
            }
            if(subscription)
            {
                for(auto const& [type, event]: subscription->get_event_handlers())
                {
                    static_cast<void>(type);
                    waitables.remove_waitable(event, group);
                }
            }
            publisher.reset();
            subscription.reset();
            try
            {
                base->trigger_notify_guard_condition();
                group->trigger_notify_guard_condition();
            }
            catch(...)
            {
            }
        }

        static error create(topic_plan const& plan,
                            std::shared_ptr<node> selected,
                            handle id,
                            std::shared_ptr<receive_queue> const& queue,
                            std::shared_ptr<native_topic>& output) noexcept
        { return create_bound(plan,std::move(selected),id,queue,Codec{},output); }

        /** @brief Create one endpoint using an already prepared immutable codec.
         * @details Native type/schema approval happens before this call. The
         * dispatcher independently admits the complete node/type/topic authority
         * group. Failed creation never publishes the output handle; subscription
         * callbacks retain their own codec ownership for their complete lifetime.
         */
        static error create_bound(topic_plan const& plan,
                                  std::shared_ptr<node> selected,
                                  handle id,
                                  std::shared_ptr<receive_queue> const& queue,
                                  Codec prepared,
                                  std::shared_ptr<native_topic>& output) noexcept
        {
            try
            {
                auto holder = ::uwvm2::utils::container::make_shared<topic>();
                holder->codec=std::move(prepared);
                holder->parent = selected;
                holder->configured = plan.quality;
                auto profile = native_qos(plan.quality);
                rclcpp::QoS q{rclcpp::QoSInitialization::from_rmw(profile), profile};
                if(plan.direction == role::publisher) { holder->publisher = selected->value->create_publisher<Native>(plan.resolved_name.c_str(), q); }
                else
                {
                    if(!queue) { return error::invalid; }
                    auto state = ::uwvm2::utils::container::make_shared<capture>();
                    state->queue = queue;
                    if constexpr(!requires{Codec::serialized_wire;})
                    {
                        std::size_t extra{};
                        if constexpr(requires{Codec::native_workspace_extra;})extra=Codec::native_workspace_extra;
                        if(extra>SIZE_MAX-plan.max_message_bytes)return error::too_large;
                        state->scratch.resize(plan.max_message_bytes+extra);
                    }
                    state->maximum_message=plan.max_message_bytes;
                    state->source = id;
                    state->generation = plan.authority.generation;
                    state->codec=holder->codec;
                    holder->ingress = state;
                    if constexpr(requires{Codec::serialized_wire;})
                        holder->subscription=selected->value->create_subscription<Native>(plan.resolved_name.c_str(),q,
                            [state](std::shared_ptr<rclcpp::SerializedMessage const> value,rclcpp::MessageInfo const& info){state->receive_serialized(value,info);});
                    else holder->subscription = selected->value->create_subscription<Native>(
                        plan.resolved_name.c_str(),
                        q,
                        [state](std::shared_ptr<Native const> value, rclcpp::MessageInfo const& info) { state->receive(value, info); });
                }
                auto actual_name = holder->publisher ? holder->publisher->get_topic_name() : holder->subscription->get_topic_name();
                if(!actual_name || plan.resolved_name != actual_name) { return error::denied; }
                qos actual;
                std::array<std::uint8_t, 32> gid{};
                std::size_t size{};
                auto e = holder->inspect(actual, gid, size);
                if(e != error::ok) { return e; }
                if(!equivalent(plan.quality, actual)) { return error::unsupported; }
                // A real nonzero publisher GID is cached from this exact object.
                // No middleware/process incarnation is inferred from its grant.
                if(size>gid.size())return error::io;
                if(size&&std::any_of(gid.begin(),gid.begin()+size,[](auto byte){return byte!=0;}))
                {holder->evidence.native=gid;holder->evidence.size=size;}
                output = std::move(holder);
                return error::ok;
            }
            catch(std::bad_alloc const&)
            {
                return error::no_memory;
            }
            catch(...)
            {
                return error::io;
            }
        }

        native_result publish(std::span<std::uint8_t const> body, std::uint64_t deadline) noexcept override
        {
            if(!publisher) { return {error::denied, effect::none}; }
            if(deadline <= now()) { return {error::timeout, effect::none}; }
            if constexpr(requires{Codec::serialized_wire;})return codec.publish(*publisher,body,deadline);
            else
            {
            try
            {
                Native value;
                if(!codec.decode(body, value)) { return {error::invalid, effect::none}; }
                if(deadline <= now()) { return {error::timeout, effect::none}; }
                // Once the native call begins, an exception cannot prove that
                // no remote effect occurred. It is never retried implicitly.
                try
                {
                    publisher->publish(value);
                }
                catch(...)
                {
                    return {error::io, effect::unknown};
                }
                return {deadline <= now() ? error::timeout : error::ok, effect::locally_accepted};
            }
            catch(std::bad_alloc const&)
            {
                return {error::no_memory, effect::none};
            }
            catch(...)
            {
                return {error::invalid, effect::none};
            }
            }
        }

        endpoint_identity identity()const noexcept override{return evidence;}
        error inspect(qos& output, std::array<std::uint8_t, 32>& gid, std::size_t& size) noexcept override
        {
            try
            {
                qos value;
                auto profile = (publisher ? publisher->get_actual_qos() : subscription->get_actual_qos()).get_rmw_qos_profile();
                if(!observed_qos(profile, value)) { return error::unsupported; }
                std::array<std::uint8_t, 32> actual{};
                std::size_t extent{};
                if(publisher)
                {
                    auto native = publisher->get_gid();
                    static_assert(RMW_GID_STORAGE_SIZE<=32);
                    extent = RMW_GID_STORAGE_SIZE;
                    std::copy_n(native.data, extent, actual.begin());
                }
                output = value;
                gid = actual;
                size = extent;
                return error::ok;
            }
            catch(...)
            {
                return error::io;
            }
        }

        native_result assert_liveliness() noexcept override
        {
            if(!publisher) { return {error::denied, effect::none}; }
            try
            {
                return publisher->assert_liveliness() ? native_result{error::ok, effect::locally_accepted} : native_result{error::io, effect::unknown};
            }
            catch(...)
            {
                return {error::io, effect::unknown};
            }
        }

        error wait_acknowledged(std::uint64_t deadline) noexcept override
        {
            if(!publisher) { return error::denied; }
            if(configured.reliability != 1) { return error::unsupported; }
            auto time = now();
            if(deadline <= time || deadline > INT64_MAX) { return error::timeout; }
            try
            {
                return publisher->wait_for_all_acked(std::chrono::nanoseconds{deadline - time}) ? error::ok : error::timeout;
            }
            catch(...)
            {
                return error::unsupported;
            }
        }
    };

    inline bool utf8(std::span<std::uint8_t const> bytes) noexcept
    {
        for(std::size_t i{}; i < bytes.size();)
        {
            auto c = bytes[i++];
            if(c < 0x80)
            {
                if(!c) { return false; }
                continue;
            }
            unsigned count{};
            std::uint32_t point{}, minimum{};
            if(c >= 0xc2 && c <= 0xdf)
            {
                count = 1;
                point = c & 31;
                minimum = 0x80;
            }
            else if(c >= 0xe0 && c <= 0xef)
            {
                count = 2;
                point = c & 15;
                minimum = 0x800;
            }
            else if(c >= 0xf0 && c <= 0xf4)
            {
                count = 3;
                point = c & 7;
                minimum = 0x10000;
            }
            else
            {
                return false;
            }
            if(count > bytes.size() - i) { return false; }
            while(count--)
            {
                auto next = bytes[i++];
                if((next & 0xc0) != 0x80) { return false; }
                point = (point << 6) | (next & 63);
            }
            if(point < minimum || point > 0x10ffff || (point >= 0xd800 && point <= 0xdfff)) { return false; }
        }
        return true;
    }

    /** @brief Complete String payload: LE u64 byte length followed by UTF-8.
     * @details Embedded NUL is rejected explicitly because native serializers
     * differ in representability. No truncation or silent conversion is allowed.
     */
    struct string_codec : codecs::string
    {

        static bool decode(std::span<std::uint8_t const> bytes, std_msgs::msg::String& value)
        {
            if(!validate(bytes)) { return false; }
            value.data.assign(reinterpret_cast<char const*>(bytes.data() + 8), bytes.size() - 8);
            return true;
        }

        static bool encode(std_msgs::msg::String const& value, std::span<std::uint8_t> out, std::size_t& size) noexcept
        {
            if(out.size() < 8 || value.data.size() > out.size() - 8) { return false; }
            auto data = std::span{reinterpret_cast<std::uint8_t const*>(value.data.data()), value.data.size()};
            if(!utf8(data)) { return false; }
            for(unsigned i{}; i != 8; ++i) { out[i] = static_cast<std::uint8_t>(static_cast<std::uint64_t>(value.data.size()) >> (8 * i)); }
            std::copy(data.begin(), data.end(), out.begin() + 8);
            size = value.data.size() + 8;
            return true;
        }
    };

    struct bool_codec : codecs::boolean
    {

        static bool decode(std::span<std::uint8_t const> bytes, std_msgs::msg::Bool& value)
        {
            if(!validate(bytes)) { return false; }
            value.data = bytes[0] != 0;
            return true;
        }

        static bool encode(std_msgs::msg::Bool const& value, std::span<std::uint8_t> out, std::size_t& size) noexcept
        {
            if(out.empty()) { return false; }
            out[0] = value.data ? 1 : 0;
            size = 1;
            return true;
        }
    };

    /** @brief Deferred real ROS service with a finite, non-overwriting correlation ledger.
     * @details Client responses retain their slot until checked guest delivery.
     * Server responses require successful request delivery. Failed copies leave
     * bytes/header/correlation intact. Timeout never claims remote cancellation;
     * client abandon explicitly removes only local interest. Native ingress that
     * exceeds approved schema/size/capacity is refused without fabricating a reply.
     */
    template <class Native, class RequestCodec, class ResponseCodec>
    class service final : public native_service
    {
        enum class phase : unsigned char
        {
            empty,
            waiting,
            ready,
            delivering,
            delivered,
            unknown
        };

        struct pending
        {
            handle correlation{}, sequence{};
            std::uint64_t deadline{};
            phase state{};
            std::int64_t native_request{};
            bool native_request_known{};
            rmw_request_id_t header{};
            ::uwvm2::utils::container::std_vector<std::uint8_t> bytes;
            std::size_t size{};
        };

        struct state
        {
            std::mutex mutex;
            ::uwvm2::utils::container::std_vector<pending> rows;
            handle endpoint{}, generation{}, sequence{}, next_request{};
            std::uint64_t max_deadline{};
            bool retired{};
            handle last_abandoned{};
            delivery_consumer consumer{},settled_consumer{};
            delivery_reference reservation{},settled_reference{};
            std::uint64_t reservation_deadline{},reservation_nonce{};
            delivery_receipt settlement{};
            bool settled_ack{};

            pending* find(handle id)
            {
                for(auto& row: rows)
                {
                    if(row.state != phase::empty && row.correlation == id) { return &row; }
                }
                return nullptr;
            }

            pending* empty()
            {
                for(auto& row: rows)
                {
                    if(row.state == phase::empty) { return &row; }
                }
                return nullptr;
            }

            static void clear(pending& row) noexcept
            {
                row.correlation = row.sequence = row.deadline = row.size = 0;
                row.native_request = 0;
                row.native_request_known = false;
                row.state = phase::empty;
                row.header = {};
            }

            void response(handle id, typename rclcpp::Client<Native>::SharedFuture future) noexcept
            {
                std::lock_guard lock{mutex};
                if(retired) { return; }
                auto row = find(id);
                if(!row || row->state != phase::waiting) { return; }
                try
                {
                    auto value = future.get();
                    std::size_t size{};
                    if(!value || !ResponseCodec::encode(*value, row->bytes, size) || sequence == UINT64_MAX)
                    {
                        row->state = phase::unknown;
                        return;
                    }
                    row->size = size;
                    row->sequence = ++sequence;
                    row->state = phase::ready;
                }
                catch(...)
                {
                    row->state = phase::unknown;
                }
            }

            void request(std::shared_ptr<rmw_request_id_t> header, std::shared_ptr<typename Native::Request> value) noexcept
            {
                std::lock_guard lock{mutex};
                if(retired || !header || !value || next_request == UINT64_MAX || sequence == UINT64_MAX) { return; }
                auto row = empty();
                if(!row) { return; }
                std::size_t size{};
                if(!RequestCodec::encode(*value, row->bytes, size)) { return; }
                auto time = now();
                if(time > INT64_MAX - max_deadline) { return; }
                row->correlation = ++next_request;
                row->sequence = ++sequence;
                row->deadline = time + max_deadline;
                row->header = *header;
                row->size = size;
                row->state = phase::ready;
            }
        };

        std::shared_ptr<node> parent;
        std::shared_ptr<state> storage;
        std::shared_ptr<rclcpp::Client<Native>> client;
        std::shared_ptr<rclcpp::Service<Native>> server;
        std::array<std::optional<qos>,2> channel_evidence;

    public:
        static error create(service_plan const& plan, std::shared_ptr<node> parent, handle id, std::shared_ptr<native_service>& output) noexcept
        {
            try
            {
                auto result = ::uwvm2::utils::container::make_shared<service>();
                result->parent = std::move(parent);
                auto data = ::uwvm2::utils::container::make_shared<state>();
                data->endpoint = id;
                data->generation = plan.authority.generation;
                data->max_deadline = plan.max_deadline_ns;
                data->rows.resize(plan.pending);
                for(auto& row: data->rows) { row.bytes.resize(plan.direction == service_role::client ? plan.max_response_bytes : plan.max_request_bytes); }
                result->storage = data;
                auto node = result->parent->value;
                if(plan.direction == service_role::client)
                {
                    result->client =
                        rclcpp::create_client<Native>(node->base, node->graph, node->services, plan.resolved_name.c_str(), native_qos(plan.quality), nullptr);
                }
                else
                {
                    result->server = rclcpp::create_service<Native>(
                        node->base,
                        node->services,
                        plan.resolved_name.c_str(),
                        [data](std::shared_ptr<rmw_request_id_t> header, std::shared_ptr<typename Native::Request> request)
                        { data->request(std::move(header), std::move(request)); },
                        native_qos(plan.quality),
                        nullptr);
                }
                auto actual = result->client ? result->client->get_service_name() : result->server->get_service_name();
                if(!actual || plan.resolved_name != actual) { return error::denied; }
                auto observed=result->read_channels(result->channel_evidence);
                if(observed!=error::ok)return observed;
                if(!equivalent(plan.quality,*result->channel_evidence[0])||!equivalent(plan.quality,*result->channel_evidence[1]))return error::unsupported;
                output = std::move(result);
                return error::ok;
            }
            catch(std::bad_alloc const&)
            {
                return error::no_memory;
            }
            catch(...)
            {
                return error::io;
            }
        }

        ~service() override { retire(); }
        std::array<std::optional<qos>,2> channels()const noexcept override{return channel_evidence;}

        /** @brief Read each actual middleware leg independently before caching it. */
        error read_channels(std::array<std::optional<qos>,2>& output)const noexcept
        {
            try
            {
                if(!parent||!parent->parent->live()||(!client&&!server))return error::cancelled;
                auto request=client?client->get_request_publisher_actual_qos():server->get_request_subscription_actual_qos();
                auto response=client?client->get_response_subscription_actual_qos():server->get_response_publisher_actual_qos();
                qos first,second;
                if(!observed_qos(request.get_rmw_qos_profile(),first)||!observed_qos(response.get_rmw_qos_profile(),second))return error::unsupported;
                output={first,second};return error::ok;
            }
            catch(...){return error::io;}
        }
        /** @brief A collapsed QoS observation requires equality of both actual legs. */
        error inspect(qos& output)noexcept override
        {
            std::array<std::optional<qos>,2> actual;auto status=read_channels(actual);if(status!=error::ok)return status;
            if(!equivalent(*actual[0],*actual[1]))return error::unsupported;
            output=*actual[0];return error::ok;
        }

        native_result submit(handle id, std::span<std::uint8_t const> body, std::uint64_t deadline) noexcept override
        {
            if(!client || !id) { return {error::denied, effect::none}; }
            try
            {
                auto request = ::uwvm2::utils::container::make_shared<typename Native::Request>();
                if(!RequestCodec::decode(body, *request)) { return {error::invalid, effect::none}; }
                std::lock_guard lock{storage->mutex};
                if(storage->retired || !parent->parent->live()) { return {error::cancelled, effect::none}; }
                if(deadline <= now()) { return {error::timeout, effect::none}; }
                if(storage->find(id)) { return {error::stale, effect::none}; }
                auto row = storage->empty();
                if(!row) { return {error::again, effect::none}; }
                row->correlation = id;
                row->deadline = deadline;
                row->state = phase::waiting;
                try
                {
                    auto shared = storage;
                    auto future = client->async_send_request(request,
                                                             [shared, id](typename rclcpp::Client<Native>::SharedFuture value)
                                                             { shared->response(id, std::move(value)); });
                    row->native_request = future.request_id;
                    row->native_request_known = true;
                    return {deadline <= now() ? error::timeout : error::ok, effect::locally_accepted};
                }
                catch(...)
                {
                    row->state = phase::unknown;
                    return {error::io, effect::unknown};
                }
            }
            catch(std::bad_alloc const&)
            {
                return {error::no_memory, effect::none};
            }
            catch(...)
            {
                return {error::invalid, effect::none};
            }
        }

        native_result respond(handle id, std::span<std::uint8_t const> body, std::uint64_t deadline) noexcept override
        {
            if(!server || !id) { return {error::denied, effect::none}; }
            try
            {
                typename Native::Response response;
                if(!ResponseCodec::decode(body, response)) { return {error::invalid, effect::none}; }
                std::unique_lock lock{storage->mutex};
                if(storage->retired || !parent->parent->live()) { return {error::cancelled, effect::none}; }
                auto row = storage->find(id);
                if(!row) { return {error::bad_handle, effect::none}; }
                if(row->state != phase::delivered) { return {error::denied, effect::none}; }
                if(deadline <= now() || row->deadline <= now()) { return {error::timeout, effect::none}; }
                auto header = row->header;
                row->state = phase::unknown;
                lock.unlock();
                try
                {
                    server->send_response(header, response);
                }
                catch(...)
                {
                    return {error::io, effect::unknown};
                }
                lock.lock();
                state::clear(*row);
                return {deadline <= now() ? error::timeout : error::ok, effect::locally_accepted};
            }
            catch(std::bad_alloc const&)
            {
                return {error::no_memory, effect::none};
            }
            catch(...)
            {
                return {error::invalid, effect::none};
            }
        }

        error take(void* argument, service_sink sink) noexcept override
        {
            std::unique_lock lock{storage->mutex};
            if(storage->retired || !parent->parent->live()) { return error::cancelled; }
            if(storage->reservation.nonce)return error::again;
            pending* first{};
            bool expired{}, unknown{};
            for(auto& row: storage->rows)
            {
                if(row.state == phase::delivering) { return error::again; }
                if(row.state == phase::ready && (!first || row.sequence < first->sequence)) { first = &row; }
                expired |= row.state == phase::waiting && row.deadline <= now();
                unknown |= row.state == phase::unknown;
            }
            if(!first) { return unknown ? error::io : expired ? error::timeout : error::again; }
            first->state = phase::delivering;
            metadata info;
            info.source = storage->endpoint;
            info.generation = storage->generation;
            info.sequence = first->sequence;
            service_delivery record{
                info,
                first->correlation,
                bool(client),
                {first->bytes.data(), first->size}
            };
            lock.unlock();
            bool copied{};
            try
            {
                copied = sink(argument, record);
            }
            catch(...)
            {
            }
            lock.lock();
            if(storage->retired)
            {
                first->state = phase::ready;
                return error::cancelled;
            }
            if(!copied)
            {
                first->state = phase::ready;
                return error::io;
            }
            if(client) { state::clear(*first); }
            else
            {
                first->state = phase::delivered;
            }
            return error::ok;
        }

        /** @brief Pin the original native service record until explicit acknowledgement.
         * @details A server request stays undelivered and cannot be answered
         * before acknowledgement. A client response remains in its original
         * preallocated slot. Failed output, timeout and local release never erase
         * native correlation, request headers or payload. The trusted worker
         * must independently authenticate each reserve/ack/release operation.
         */
        error reserve(delivery_consumer consumer,std::uint64_t deadline,void* argument,service_reservation_sink sink)noexcept override
        {
            if(!consumer||!sink)return error::denied;
            std::unique_lock lock{storage->mutex};auto time=now();
            if(storage->retired||!parent->parent->live())return error::cancelled;
            if(deadline>INT64_MAX||deadline<=time||deadline-time>storage->max_deadline)return error::timeout;
            pending* first{};
            for(auto& row:storage->rows)
            {
                if(row.state==phase::delivering)return error::again;
                if(storage->reservation.nonce)
                {if(row.sequence==storage->reservation.sequence&&row.state==phase::ready)first=&row;}
                else if(row.state==phase::ready&&(!first||row.sequence<first->sequence))first=&row;
            }
            if(storage->reservation.nonce)
            {
                if(consumer!=storage->consumer)return error::busy;
                if(deadline!=storage->reservation_deadline)return error::stale;
                if(!first)return error::io;
            }
            else
            {
                if(!first)return error::again;
                if(storage->reservation_nonce==UINT64_MAX)return error::too_large;
                storage->reservation={storage->endpoint,storage->generation,first->sequence,++storage->reservation_nonce};
                storage->consumer=consumer;storage->reservation_deadline=deadline;
            }
            auto reference=storage->reservation;first->state=phase::delivering;
            metadata info;info.source=storage->endpoint;info.generation=storage->generation;info.sequence=first->sequence;
            service_delivery record{info,first->correlation,bool(client),{first->bytes.data(),first->size}};
            lock.unlock();bool copied{};
            try{copied=sink(argument,reference,deadline,record);}catch(...){}
            lock.lock();first->state=phase::ready;
            if(storage->retired)return error::cancelled;
            if(storage->reservation!=reference)return error::stale;
            return copied?error::ok:error::io;
        }

        /** @brief Settle one exact service borrow; no ROS response is sent by acknowledgement.
         * @details Server acknowledgement only enables the separately authorized
         * service.respond operation. Release leaves the original record ready.
         * Expired/revoked parents deny acknowledgement but permit local release.
         * These local receipts do not establish remote execution or cancellation.
         */
        error settle(delivery_consumer consumer,delivery_reference reference,request_key key,bool acknowledge,delivery_receipt& output)noexcept override
        {
            std::lock_guard lock{storage->mutex};
            if(!consumer||key.session!=storage->endpoint||!key.sequence)return error::denied;
            if(storage->settlement.request&&key==storage->settlement.key)
            {
                if(consumer!=storage->settled_consumer||reference!=storage->settled_reference||acknowledge!=storage->settled_ack)return error::stale;
                output=storage->settlement;return error::ok;
            }
            if(!storage->reservation.nonce||consumer!=storage->consumer||reference!=storage->reservation)return error::denied;
            if(storage->settlement.key.sequence==UINT64_MAX||key.sequence!=storage->settlement.key.sequence+1)return error::stale;
            pending* selected{};
            for(auto& row:storage->rows)if(row.sequence==reference.sequence&&row.state!=phase::empty){selected=&row;break;}
            if(!selected)return error::io;
            if(selected->state==phase::delivering)return error::again;
            if(selected->state!=phase::ready)return error::io;
            if(acknowledge&&(storage->retired||!parent->parent->live()))return error::cancelled;
            if(acknowledge&&now()>=storage->reservation_deadline)return error::timeout;
            auto request=resources::allocate_identity();if(!request)return error::too_large;
            if(acknowledge){if(client)state::clear(*selected);else selected->state=phase::delivered;}
            storage->settlement={request,storage->generation,key,error::ok,effect::locally_accepted};
            storage->settled_consumer=consumer;storage->settled_reference=reference;storage->settled_ack=acknowledge;
            storage->consumer={};storage->reservation={};storage->reservation_deadline=0;
            output=storage->settlement;return error::ok;
        }

        error available(bool& output) noexcept override
        {
            if(!client) { return error::denied; }
            std::lock_guard lock{storage->mutex};
            if(storage->retired || !parent->parent->live()) { return error::cancelled; }
            try
            {
                output = client->service_is_ready();
                return error::ok;
            }
            catch(...)
            {
                return error::io;
            }
        }

        native_result abandon(handle id) noexcept override
        {
            if(!client || !id) { return {error::denied, effect::none}; }
            std::lock_guard lock{storage->mutex};
            if(storage->retired) { return {error::cancelled, effect::none}; }
            auto row = storage->find(id);
            if(!row) { return storage->last_abandoned == id ? native_result{error::ok, effect::unknown} : native_result{error::bad_handle, effect::none}; }
            if(row->state == phase::delivering||(storage->reservation.nonce&&storage->reservation.sequence==row->sequence)) { return {error::busy, effect::none}; }
            try
            {
                if(row->native_request_known) { client->remove_pending_request(row->native_request); }
            }
            catch(...)
            {
                return {error::io, effect::unknown};
            }
            state::clear(*row);
            storage->last_abandoned = id;
            return {error::ok, effect::unknown};
        }

        bool occupied() noexcept override
        {
            std::lock_guard lock{storage->mutex};
            for(auto const& row: storage->rows)
            {
                if(row.state != phase::empty) { return true; }
            }
            return false;
        }

        void retire() noexcept override
        {
            if(storage)
            {
                std::lock_guard lock{storage->mutex};
                storage->retired = true;
            }
        }
    };

    struct set_bool_request_codec : codecs::set_bool_request
    {

        static bool decode(std::span<std::uint8_t const> body, std_srvs::srv::SetBool::Request& output)
        {
            if(!validate(body)) { return false; }
            output.data = body[0] != 0;
            return true;
        }

        static bool encode(std_srvs::srv::SetBool::Request const& value, std::span<std::uint8_t> out, std::size_t& size) noexcept
        {
            if(out.empty()) { return false; }
            out[0] = value.data ? 1 : 0;
            size = 1;
            return true;
        }
    };

    struct set_bool_response_codec : codecs::set_bool_response
    {

        static bool decode(std::span<std::uint8_t const> body, std_srvs::srv::SetBool::Response& output)
        {
            if(!validate(body)) { return false; }
            output.success = body[0] != 0;
            output.message.assign(reinterpret_cast<char const*>(body.data() + 9), body.size() - 9);
            return true;
        }

        static bool encode(std_srvs::srv::SetBool::Response const& value, std::span<std::uint8_t> out, std::size_t& size) noexcept
        {
            if(out.size() < 9 || value.message.size() > out.size() - 9) { return false; }
            std::span<std::uint8_t const> text{reinterpret_cast<std::uint8_t const*>(value.message.data()), value.message.size()};
            if(!utf8(text)) { return false; }
            out[0] = value.success ? 1 : 0;
            for(unsigned i{}; i != 8; ++i) { out[i + 1] = static_cast<std::uint8_t>(static_cast<std::uint64_t>(value.message.size()) >> (8 * i)); }
            std::copy(text.begin(), text.end(), out.begin() + 9);
            size = 9 + text.size();
            return true;
        }
    };

    using service_factory = error (*)(service_plan const&, std::shared_ptr<node>, handle, std::shared_ptr<native_service>&) noexcept;

    struct service_binding
    {
        ::uwvm2::utils::container::std_string identity, request_type, response_type, encoding;
        bool (*request_validator)(std::span<std::uint8_t const>) noexcept;
        bool (*response_validator)(std::span<std::uint8_t const>) noexcept;
        service_factory create;
    };

    using factory = error (*)(topic_plan const&, std::shared_ptr<node>, handle, std::shared_ptr<receive_queue> const&, std::shared_ptr<native_topic>&) noexcept;

    struct type_binding
    {
        ::uwvm2::utils::container::std_string identity, encoding;
        bool (*validate)(std::span<std::uint8_t const>) noexcept;
        factory create;
    };

    /** @brief Prepared, exact-schema native factory owned by the isolated worker.
     * @details A factory is trusted native code selected by a pinned deployment
     * closure. It grants nothing, cannot be supplied as a guest pointer, and must
     * match the full frozen profile/schema before any native endpoint is created.
     */
    class bounded_topic_factory
    {
    public:
        virtual ~bounded_topic_factory()=default;
        virtual bool matches(type_plan const&)const noexcept=0;
        virtual error create(topic_plan const&,std::shared_ptr<node>,handle,
                             std::shared_ptr<receive_queue> const&,std::shared_ptr<native_topic>&)const noexcept=0;
    };

    /** @brief Explicit finite native type registry; no guest-directed dlopen. */
    /** @brief Optional trusted diagnostic sink; views are valid only for the call.
     * @details The worker routes bounded text to its separately budgeted stderr.
     * This reports native startup failures without granting any new authority.
     */
    using failure_reporter=void(*)(std::string_view,std::string_view) noexcept;
    class provider final : public backend
    {
        ::uwvm2::utils::container::std_vector<type_binding> bindings;
        std::shared_ptr<network_policy> network;
        failure_reporter report{};
        ::uwvm2::utils::container::std_vector<service_binding> services;
        ::uwvm2::utils::container::std_vector<std::shared_ptr<bounded_topic_factory>> bounded_topics;

    public:
        explicit provider(::uwvm2::utils::container::std_vector<type_binding> approved,
                          std::shared_ptr<network_policy> policy,
                          ::uwvm2::utils::container::std_vector<service_binding> service_types = {},failure_reporter diagnostics = nullptr,
                          ::uwvm2::utils::container::std_vector<std::shared_ptr<bounded_topic_factory>> bounded = {}) :
            bindings(std::move(approved)), network(std::move(policy)), report(diagnostics), services(std::move(service_types)),bounded_topics(std::move(bounded))
        {
        }

        error create_context(context_plan const& plan, std::shared_ptr<native_context>& output) noexcept override
        {
            if(plan.network != transport::unauthenticated_loopback || !network || !network->permits_domain(plan.domain)) { return error::denied; }
            try
            {
                auto result = ::uwvm2::utils::container::make_shared<context>();
                result->value = ::uwvm2::utils::container::make_shared<rclcpp::Context>();
                rclcpp::InitOptions options;
                options.shutdown_on_signal = false;
                options.auto_initialize_logging(false);
                options.set_domain_id(plan.domain);
                std::array<char const*, 4> args{"scene-provider", "--ros-args", "--enclave", plan.enclave.c_str()};
                if(!plan.enclave.empty() && plan.enclave != "/" && !absolute_name(plan.enclave)) { return error::invalid; }
                result->value->init(plan.enclave.empty() ? 1 : 4, args.data(), options);
                rclcpp::ExecutorOptions execution;
                execution.context = result->value;
                result->executor = ::uwvm2::utils::container::make_shared<rclcpp::executors::SingleThreadedExecutor>(execution);
                auto executor = result->executor;
                auto state = result.get();
                result->worker = ::fast_io::native_thread(
                    [executor, state]
                    {
                        try
                        {
                            executor->spin();
                        }
                        catch(...)
                        {
                        }
                        state->functioning.store(false, std::memory_order_release);
                    });
                output = std::move(result);
                return error::ok;
            }
            catch(std::bad_alloc const&)
            {
                return error::no_memory;
            }
            catch(std::exception const& failure)
            {
                if(report)report("context",failure.what());
                return error::io;
            }
            catch(...)
            {
                if(report)report("context","unknown native startup exception");
                return error::io;
            }
        }

        error create_node(node_plan const& plan, std::shared_ptr<native_context> const& base, std::shared_ptr<native_node>& output) noexcept override
        {
            auto parent = std::dynamic_pointer_cast<context>(base);
            if(!parent || !parent->live()) { return error::denied; }
            try
            {
                auto result = ::uwvm2::utils::container::make_shared<node>();
                result->parent = parent;
                result->value = ::uwvm2::utils::container::make_shared<quiet_node>(plan, parent->value);
                if(plan.name != result->value->get_name() || plan.name_space != result->value->get_namespace()) { return error::denied; }
                parent->executor->add_node(result->value->base);
                output = std::move(result);
                return error::ok;
            }
            catch(std::bad_alloc const&)
            {
                return error::no_memory;
            }
            catch(std::exception const& failure)
            {
                if(report)report("node",failure.what());
                return error::io;
            }
            catch(...)
            {
                if(report)report("node","unknown native startup exception");
                return error::io;
            }
        }

        error create_topic(topic_plan const& plan,
                           type_plan const& schema,
                           std::shared_ptr<native_node> const& base,
                           handle id,
                           std::shared_ptr<receive_queue> const& queue,
                           std::shared_ptr<native_topic>& output) noexcept override
        {
            auto selected = std::dynamic_pointer_cast<node>(base);
            if(!selected) { return error::denied; }
            if(schema.validate.bounded())
            {
                for(auto const& binding:bounded_topics)
                    if(binding&&binding->matches(schema))return binding->create(plan,std::move(selected),id,queue,output);
                return error::unsupported;
            }
            for(auto const& binding: bindings)
            {
                if(binding.identity == schema.type_identity && binding.encoding == schema.encoding && binding.validate == schema.validate)
                {
                    return binding.create(plan, std::move(selected), id, queue, output);
                }
            }
            return error::unsupported;
        }

        error create_service(service_plan const& plan,
                             type_plan const& request,
                             type_plan const& response,
                             std::shared_ptr<native_node> const& base,
                             handle id,
                             std::shared_ptr<native_service>& output) noexcept override
        {
            auto selected = std::dynamic_pointer_cast<node>(base);
            if(!selected || !selected->parent->live()) { return error::denied; }
            for(auto const& binding: services)
            {
                if(binding.identity == plan.service_type && binding.request_type == request.type_identity && binding.response_type == response.type_identity &&
                   binding.encoding == request.encoding && binding.encoding == response.encoding && binding.request_validator == request.validate &&
                   binding.response_validator == response.validate)
                {
                    return binding.create(plan, std::move(selected), id, output);
                }
            }
            return error::unsupported;
        }
    };

    inline ::uwvm2::utils::container::std_vector<type_binding> builtin_types()
    {
        return {
            {"std_msgs/msg/String", "scene-canonical", &string_codec::validate, &topic<std_msgs::msg::String, string_codec>::create},
            {"std_msgs/msg/Bool",   "scene-canonical", &bool_codec::validate,   &topic<std_msgs::msg::Bool,   bool_codec>::create  }
        };
    }

    inline ::uwvm2::utils::container::std_vector<service_binding> builtin_services()
    {
        return {
            {"std_srvs/srv/SetBool",
             "std_srvs/srv/SetBool_Request", "std_srvs/srv/SetBool_Response",
             "scene-canonical", &set_bool_request_codec::validate,
             &set_bool_response_codec::validate,
             &service<std_srvs::srv::SetBool, set_bool_request_codec, set_bool_response_codec>::create}
        };
    }
}  // namespace uwvm2::imported::ros::scene::communication::ros2
