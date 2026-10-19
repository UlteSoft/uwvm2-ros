/*************************************************************
 * UWVM2 generated, fixed-layout ROS1 topic bindings.         *
 * Copyright (c) 2025-present UlteSoft. APL-2.0 License.      *
 *************************************************************/
#pragma once

#include "../common/server.h"
#include "custom_native_input.h"
#include "complete_vendor_definition.h"
#include <uwvm2/imported/ros/ipc/custom_type.h>
#include <ros/ros.h>
#include <ros/message_traits.h>
#include <cstddef>
#include <cstdint>
#include <atomic>
#include <array>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

// A common descriptor without its native typed binding cannot establish ROS1
// interoperability. Conversely, a native binding without the shared descriptor
// would bypass the Worker's trusted structural validation. The build must select
// both headers together. With neither header selected, the registry is empty.
#if defined(UWVM_ROS_CUSTOM_ROS1_HEADER) != defined(UWVM_ROS_CUSTOM_TOPIC_MANIFEST)
# error "Custom ROS1 topics require both native and common generated headers"
#endif

namespace uwvm2::ros_gateway::ros1_custom
{
    /**
     * Borrow the existing adapter's alias tables. No ROS entity is created by
     * constructing this view, loading a generated provider, or checking a type.
     * The session's normal capability close/revoke path removes these entries.
     */
    struct topic_store
    {
        ::ros::NodeHandle& node;
        ::std::map<::std::string, ::ros::Publisher>& publishers;
        ::std::map<::std::string, ::ros::Subscriber>& subscribers;
        ::std::map<::std::string, ros::capability_spec> retained_grants;
        ::std::map<::std::string, ::std::shared_ptr<::std::atomic<bool>>> subscription_verified;
        ::std::map<::std::string, ::std::weak_ptr<event_queue>> subscription_queues;

        topic_store(::ros::NodeHandle& native_node, ::std::map<::std::string, ::ros::Publisher>& native_publishers,
                    ::std::map<::std::string, ::ros::Subscriber>& native_subscribers)
            : node{native_node}, publishers{native_publishers}, subscribers{native_subscribers} {}
        topic_store(topic_store const&) = delete;
        topic_store& operator=(topic_store const&) = delete;
        topic_store(topic_store&&) = delete;
        topic_store& operator=(topic_store&&) = delete;

        ~topic_store()
        {
            // Borrowed native tables outlive this view in ros1_adapter. Disable
            // custom callbacks before their final subscriber handles disappear.
            for(auto const& gate : subscription_verified) { gate.second->store(false, ::std::memory_order_release); }
            for(auto const& queued : subscription_queues) { if(auto queue{queued.second.lock()}) { queue->deactivate(); } }
        }

        static bool same_grant(ros::capability_spec const& first, ros::capability_spec const& second)
        {
            return first.alias == second.alias && first.operation == second.operation &&
                   first.resource == second.resource && first.type == second.type &&
                   first.type_identity == second.type_identity && first.limits == second.limits;
        }

        bool retained(ros::capability_spec const& capability) const
        {
            auto const previous{retained_grants.find(capability.alias)};
            return previous != retained_grants.end() && same_grant(previous->second, capability);
        }

        void close(::std::string const& alias)
        {
            auto const verified{subscription_verified.find(alias)};
            if(verified != subscription_verified.end()) { verified->second->store(false, ::std::memory_order_release); }
            auto const queued{subscription_queues.find(alias)};
            if(queued != subscription_queues.end()) { if(auto queue{queued->second.lock()}) { queue->deactivate(); } }
            publishers.erase(alias);
            subscribers.erase(alias);
            subscription_verified.erase(alias);
            subscription_queues.erase(alias);
            retained_grants.erase(alias);
        }
    };

    inline bool known(ros::capability_spec const& capability);

    /**
     * Parse the official ROS1 Definition without allocating. Generated message
     * definitions contain a root body followed by '=' separators and
     * 'MSG: package/Type' dependency bodies. Compare every field and dependency;
     * comments and ASCII whitespace have no type semantics. Constants, defaults,
     * unknown types, duplicate bodies and omitted fields refuse admission.
     * Strings/sequences require the exact declared bounded-profile native schema;
     * the retained fixed-v1 projection cannot admit dynamic fields. The complete definition is limited to 16 MiB, 128 records and
     * 4096 fields; no Guest-controlled string reaches this startup-only check.
     */
    namespace definition_details
    {
        inline bool whitespace(char c) noexcept
        { return c == ' ' || c == '\t' || c == '\r' || c == '\f' || c == '\v'; }

        inline ::std::string_view trim(::std::string_view text) noexcept
        {
            while(!text.empty() && whitespace(text.front())) { text.remove_prefix(1); }
            while(!text.empty() && whitespace(text.back())) { text.remove_suffix(1); }
            return text;
        }

        inline bool next_line(::std::string_view& input, ::std::string_view& line) noexcept
        {
            while(!input.empty())
            {
                auto const newline{input.find('\n')};
                line = input.substr(0, newline);
                input.remove_prefix(newline == ::std::string_view::npos ? input.size() : newline + 1);
                auto const comment{line.find('#')};
                if(comment != ::std::string_view::npos) { line = line.substr(0, comment); }
                line = trim(line);
                if(!line.empty()) { return true; }
            }
            return false;
        }

        inline bool identifier(::std::string_view name) noexcept
        {
            if(name.empty() || name.size() > 255) { return false; }
            bool const initial{(name.front() >= 'a' && name.front() <= 'z') ||
                (name.front() >= 'A' && name.front() <= 'Z') || name.front() == '_'};
            if(!initial) { return false; }
            for(char c : name)
            {
                if(!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                     (c >= '0' && c <= '9') || c == '_')) { return false; }
            }
            return true;
        }

        struct field_type
        {
            unsigned primitive{};
            ::std::string_view package, record;
            bool array{};
            unsigned count{};
        };

        inline bool parse_type(::std::string_view text, ::std::string_view section, field_type& out) noexcept
        {
            if(text.empty() || text.size() > 512) { return false; }
            field_type parsed{};
            auto const bracket{text.find('[')};
            if(bracket != ::std::string_view::npos)
            {
                if(!bracket || text.back() != ']') { return false; }
                auto digits{text.substr(bracket + 1, text.size() - bracket - 2)};
                if(digits.size() > 10) { return false; }
                unsigned count{};
                for(char c : digits)
                {
                    if(c < '0' || c > '9') { return false; }
                    unsigned const digit{static_cast<unsigned>(c - '0')};
                    if(count > (4096U - digit) / 10U) { return false; }
                    count = count * 10U + digit;
                }
                if((!digits.empty()&&!count)||count>4096) { return false; }
                parsed.array = true; parsed.count = count;
                text = text.substr(0, bracket);
            }
            // ROS1 byte/char are primitive aliases, not nested record names.
            if(text == "byte") { text = "int8"; }
            else if(text == "char") { text = "uint8"; }
            ::std::array<::std::string_view, 14> const primitives{
                "bool", "int8", "uint8", "int16", "uint16", "int32", "uint32", "int64", "uint64", "float32", "float64", "string", "time", "duration"};
            for(unsigned i{}; i != primitives.size(); ++i)
            { if(text == primitives[i]) { parsed.primitive = i + 1; out = parsed; return true; } }
            // ROS1's unqualified Header is always std_msgs/Header. Preserve
            // official names and native primitive time/duration spelling; this
            // normalizes Definition comparison, never a native MD5 identity.
            if(text == "Header") { text = "std_msgs/Header"; }
            auto const slash{text.find('/')};
            if(slash == ::std::string_view::npos)
            { parsed.package = section.substr(0, section.find('/')); parsed.record = text; }
            else
            { parsed.package = text.substr(0, slash); parsed.record = text.substr(slash + 1); }
            if(!identifier(parsed.package) || !identifier(parsed.record) || parsed.record.front() < 'A' ||
               parsed.record.front() > 'Z' || parsed.package.front() < 'a' || parsed.package.front() > 'z') { return false; }
            out = parsed; return true;
        }

        inline bool field(::std::string_view line, ::std::string_view& type, ::std::string_view& name) noexcept
        {
            ::std::size_t split{};
            while(split < line.size() && !whitespace(line[split])) { ++split; }
            if(!split || split == line.size()) { return false; }
            type = line.substr(0, split); name = trim(line.substr(split));
            return identifier(name); // A third token, '=' or a default is refused.
        }

        inline bool same_field(::std::string_view native, ::std::string_view expected, ::std::string_view section) noexcept
        {
            ::std::string_view native_type, native_name, expected_type, expected_name;
            if(!field(native, native_type, native_name) || !field(expected, expected_type, expected_name) ||
               native_name != expected_name) { return false; }
            field_type first, second;
            return parse_type(native_type, section, first) && parse_type(expected_type, section, second) &&
                first.primitive == second.primitive && first.package == second.package && first.record == second.record &&
                first.array == second.array && first.count == second.count;
        }

        inline bool separator(::std::string_view line) noexcept
        {
            if(line.size() < 3 || line.front() != '=') { return false; }
            for(char c : line) { if(c != '=') { return false; } }
            return true;
        }
    }

    template<typename Entry>
    bool matches_definition(::std::string_view root, ::std::string_view native,
                            Entry const* expected, ::std::size_t count) noexcept
    {
        if(!expected || !count || count > 128 || expected[0].type != root ||
           native.size() > 16U * 1024U * 1024U) { return false; }
        for(::std::size_t i{}; i != count; ++i)
        {
            if(!(ipc::custom_topic_name(1U, expected[i].type) || ipc::official_standard_dependency_name(1U, expected[i].type)) || expected[i].fields.size() > 1024U * 1024U) { return false; }
            for(::std::size_t previous{}; previous != i; ++previous)
            { if(expected[previous].type == expected[i].type) { return false; } }
        }
        ::std::array<bool, 128> visited{}; visited[0] = true;
        ::std::size_t current{}, fields{};
        ::std::string_view remaining{expected[0].fields};
        bool pending_separator{};
        ::std::string_view line, next_expected;
        while(definition_details::next_line(native, line))
        {
            if(definition_details::separator(line))
            {
                if(pending_separator || definition_details::next_line(remaining, next_expected)) { return false; }
                pending_separator = true; continue;
            }
            if(line.substr(0, 4) == "MSG:")
            {
                if(!pending_separator) { return false; }
                auto const dependency{definition_details::trim(line.substr(4))};
                ::std::size_t index{count};
                for(::std::size_t i{1}; i != count; ++i)
                { if(expected[i].type == dependency) { index = i; break; } }
                if(index == count || visited[index]) { return false; }
                visited[index] = true; current = index; remaining = expected[index].fields;
                pending_separator = false; continue;
            }
            if(pending_separator || ++fields > 4096 || !definition_details::next_line(remaining, next_expected) ||
               !definition_details::same_field(line, next_expected, expected[current].type)) { return false; }
        }
        if(pending_separator || definition_details::next_line(remaining, next_expected)) { return false; }
        for(::std::size_t i{}; i != count; ++i) { if(!visited[i]) { return false; } }
        return true;
    }

    /**
     * Bind an official generated ROS1 message to its generated canonical codec.
     * Codec supplies C++17 constants type/native_identity/codec_identity/
     * wire_bytes, allocation-free valid(Native const&), and bounded encode and
     * decode functions using a caller-owned byte buffer. Legacy providers cover
     * fixed values; bounded-v2 providers additionally prove complete dynamic
     * fields, their explicit capacities, UTF-8 and exact active encoded length.
     * This binding never installs an implicit capability or an internal topic.
     */
    template<typename Codec,typename=void> struct bounded_generated_codec : ::std::false_type {};
    template<typename Codec> struct bounded_generated_codec<Codec,::std::void_t<decltype(Codec::bounded_profile)>> : ::std::bool_constant<Codec::bounded_profile> {};
    template<class Codec,class Native> inline bool encoded_size(Native const& value,::std::size_t& size)
    {if constexpr(bounded_generated_codec<Codec>::value) {return Codec::encoded_size(value,size);}else {if(!Codec::valid(value)) {return false;}size=Codec::wire_bytes;return true;}}

    template<typename Native, typename Codec>
    struct binding
    {
        static_assert(::ros::message_traits::IsMessage<Native>::value,
                      "A custom topic binding requires an official ROS1 message");
        static_assert(::ros::message_traits::IsFixedSize<Native>::value||bounded_generated_codec<Codec>::value,
                      "Dynamic official native fields require a complete bounded generated codec");

        static bool native_contract()
        {
            // Official generated traits and the selected codec are immutable.
            // Qualify their complete schema once at startup, then reuse this
            // result on the hot publish/callback path. Capability and descriptor
            // checks below remain per-call and never inherit a previous grant.
            static bool const qualified{[]
            {
                auto const name{::ros::message_traits::DataType<Native>::value()};
                auto const identity{::ros::message_traits::MD5Sum<Native>::value()};
                auto const definition{::ros::message_traits::Definition<Native>::value()};
                if constexpr(ipc::complete_vendor_codec<Codec>::value)
                {
                    return ipc::complete_vendor_native_traits<Native>::enabled&&
                        ipc::complete_vendor_native_traits<Native>::constants()&&
                        name&&identity&&definition&&::std::string_view{name}==Codec::type&&
                        ::std::string_view{identity}==Codec::native_identity&&
                        ros1_complete_vendor::matches(Codec::type,::std::string_view{definition},
                            Codec::definitions,Codec::definition_count,definition_details::same_field);
                }
                else
                {
                    return name && identity && definition && ::std::string_view{name} == Codec::type &&
                        ::std::string_view{identity} == Codec::native_identity &&
                        matches_definition(Codec::type, ::std::string_view{definition}, Codec::definitions, Codec::definition_count);
                }
            }()};
            return qualified;
        }

        static bool known(ros::capability_spec const& capability)
        {
            if(capability.operation != "topic.publish" && capability.operation != "topic.subscribe") { return false; }
            auto const* descriptor{ipc::find_custom_topic(1U, capability.type)};
            if(!::ros::message_traits::IsFixedSize<Native>::value&&(!descriptor||!descriptor->bounded_layout)) {return false;}
            if(!descriptor || descriptor->complete_vendor != ipc::complete_vendor_codec<Codec>::value ||
               descriptor->ros_version != 1U || descriptor->type != Codec::type ||
               descriptor->native_identity != Codec::native_identity || descriptor->codec_identity != Codec::codec_identity ||
               descriptor->wire_bytes != Codec::wire_bytes || capability.type != Codec::type ||
               capability.type_identity != Codec::native_identity) { return false; }
            auto const codec{capability.limits.find("adapter-codec-identity")};
            if(codec == capability.limits.end() || codec->second != Codec::codec_identity) { return false; }
            // These checks run before ros::init as part of known_type. A copied
            // policy MD5 or a provider's claimed name cannot replace the real
            // generated roscpp traits used by the typed publisher/subscriber.
            return native_contract();
        }

        static bool known(ros::capability_spec const& capability, ::std::string_view type,
                          ::std::string_view native_identity, ::std::string_view codec_identity, ::std::size_t wire_bytes)
        {
            // The provider's dispatch row and codec must agree independently of
            // the descriptor. A stale generated native header fails closed.
            return type == Codec::type && native_identity == Codec::native_identity &&
                   codec_identity == Codec::codec_identity && wire_bytes == Codec::wire_bytes && known(capability);
        }

        static ipc::call_result publish(topic_store& store, ros::capability_spec const& capability,
                                        ::std::vector<::std::uint8_t> const& bytes)
        {
            if(capability.operation != "topic.publish" || !ros1_custom::known(capability) || !known(capability)) { return {13, {}}; }
            auto const* descriptor{ipc::find_custom_topic(1U, capability.type)};
            if(bytes.size() > limit(capability, "max-message-bytes")) { return {90, {}}; }
            if(!descriptor || !ipc::validate_custom_topic(*descriptor, bytes.data(), bytes.size())) { return {22, {}}; }
            Native message{};
            if(!Codec::decode(bytes.data(), bytes.size(), message) || !Codec::valid(message)) { return {22, {}}; }
            auto publisher{store.publishers.find(capability.alias)};
            if(publisher == store.publishers.end())
            {
                if(store.retained_grants.count(capability.alias) || store.subscribers.count(capability.alias)) { return {17, {}}; }
                // Prepare the complete immutable grant before creating an
                // entity; native alias reuse cannot silently change quotas.
                auto grant_snapshot{capability};
                auto native{store.node.advertise<Native>(capability.resource,
                    static_cast<::std::uint32_t>(limit(capability, "queue-depth")), field(capability, "latch") == "true")};
                if(!native) { return {17, {}}; }
                if(native.getTopic() != capability.resource) { native.shutdown(); return {13, {}}; }
                store.retained_grants.emplace(capability.alias, ::std::move(grant_snapshot));
                try { publisher = store.publishers.emplace(capability.alias, ::std::move(native)).first; }
                catch(...) { store.retained_grants.erase(capability.alias); throw; }
            }
            if(!store.retained(capability) || publisher->second.getTopic() != capability.resource) { return {13, {}}; }
            publisher->second.publish(message);
            return {};
        }

        static ipc::call_result subscribe(topic_store& store, ros::capability_spec const& capability,
                                          ::std::shared_ptr<event_queue> events)
        {
            if(capability.operation != "topic.subscribe" || !ros1_custom::known(capability) || !known(capability) || !events) { return {13, {}}; }
            auto const& queued{events->capability};
            if(queued.alias != capability.alias || queued.operation != capability.operation ||
               queued.resource != capability.resource || queued.type != capability.type ||
               queued.type_identity != capability.type_identity || queued.limits != capability.limits) { return {13, {}}; }
            auto selected_descriptor{ipc::find_custom_topic(1,capability.type)};
            if(!selected_descriptor||(selected_descriptor->bounded_layout?selected_descriptor->minimum_wire_bytes:Codec::wire_bytes) > limit(capability, "max-message-bytes")) { return {90, {}}; }
            auto const existing{store.subscribers.find(capability.alias)};
            if(existing != store.subscribers.end())
            {
                auto const gate{store.subscription_verified.find(capability.alias)};
                auto const previous_queue{store.subscription_queues.find(capability.alias)};
                return store.retained(capability) && gate != store.subscription_verified.end() &&
                    previous_queue != store.subscription_queues.end() && previous_queue->second.lock().get() == events.get() &&
                    gate->second->load(::std::memory_order_acquire) && existing->second.getTopic() == capability.resource ?
                    ipc::call_result{} : ipc::call_result{13, {}};
            }
            if(store.retained_grants.count(capability.alias) || store.publishers.count(capability.alias) ||
               store.subscription_verified.count(capability.alias) || store.subscription_queues.count(capability.alias)) { return {17, {}}; }
            auto grant_snapshot{capability};
            auto verified{::std::make_shared<::std::atomic<bool>>(false)};
            ::std::weak_ptr<event_queue> weak{events};
            ::boost::function<void(::boost::shared_ptr<Native const> const&)> callback{
                [weak, verified](::boost::shared_ptr<Native const> const& message)
                {
                    if(!message || !verified->load(::std::memory_order_acquire)) { return; }
                    auto queue{weak.lock()};
                    if(!queue) { return; }
                    {
                        ::std::lock_guard<::std::mutex> guard{queue->mutex};
                        if(!queue->active) { return; }
                    }
                    // Measure and validate native fields before copying or
                    // allocating canonical output. Only the active prefix is
                    // charged to the explicit message and queue bounds.
                    ::std::size_t active_bytes{};
                    if(!ros1_custom::known(queue->capability) || !known(queue->capability) ||
                       !encoded_size<Codec>(*message,active_bytes)||active_bytes>limit(queue->capability,"max-message-bytes")) {return;}
                    auto const* descriptor{ipc::find_custom_topic(1U, queue->capability.type)};
                    if(!descriptor) { return; }
                    ::std::vector<::std::uint8_t> bytes;
                    if(!Codec::encode(*message, bytes) ||
                       !ipc::validate_custom_topic(*descriptor, bytes.data(), bytes.size())) { return; }
                    // enqueue rechecks active state, complete structure, token
                    // quotas, queue depth/bytes, and the explicit overflow rule.
                    // A concurrent close/revoke cannot publish a late event.
                    if(verified->load(::std::memory_order_acquire)) { queue->enqueue(::std::move(bytes)); }
                }};
            auto const* input_descriptor{ipc::find_custom_topic(1,capability.type)};auto input_maximum{limit(capability,"max-message-bytes")};
            auto native{ros1_custom_input::subscribe<Native>(store.node,capability.resource,
                static_cast<::std::uint32_t>(limit(capability,"queue-depth")),callback,
                [input_descriptor,input_maximum](std::uint8_t const* bytes,std::size_t size)
                {return input_descriptor&&size<=input_maximum&&ipc::validate_custom_topic(*input_descriptor,bytes,size);})};
            if(!native) { return {17, {}}; }
            if(native.getTopic() != capability.resource) { native.shutdown(); return {13, {}}; }
            // Callback execution is refused until exact name admission and
            // retention both finish. A refused or remapped entity cannot enqueue
            // even if roscpp delivers a callback during subscription creation.
            store.retained_grants.emplace(capability.alias, ::std::move(grant_snapshot));
            try
            {
                store.subscription_verified.emplace(capability.alias, verified);
                store.subscription_queues.emplace(capability.alias, events);
                store.subscribers.emplace(capability.alias, ::std::move(native));
            }
            catch(...)
            {
                verified->store(false, ::std::memory_order_release);
                store.subscription_verified.erase(capability.alias);
                store.subscription_queues.erase(capability.alias);
                store.retained_grants.erase(capability.alias);
                throw;
            }
            verified->store(true, ::std::memory_order_release);
            return {};
        }
    };
}

// The trusted generated header includes the official native package headers and
// codecs, then defines UWVM_ROS_CUSTOM_ROS1_TYPES(M). Each M invocation contains
// (NativeType, type_name, native_identity, codec_identity, wire_bytes, Codec).
// Providers are selected by the trusted build, never by Guest bytes or config.
#if defined(UWVM_ROS_CUSTOM_ROS1_HEADER)
# if defined(UWVM_ROS_CUSTOM_ROS1_TYPES)
#  error "A custom ROS1 native provider macro cannot precede the selected header"
# endif
# include UWVM_ROS_CUSTOM_ROS1_HEADER
# if !defined(UWVM_ROS_CUSTOM_ROS1_TYPES)
#  error "The generated ROS1 native header must define UWVM_ROS_CUSTOM_ROS1_TYPES"
# endif
#elif defined(UWVM_ROS_CUSTOM_ROS1_TYPES)
# error "A custom ROS1 provider macro requires an explicit generated native header"
#else
# define UWVM_ROS_CUSTOM_ROS1_TYPES(M)
# define UWVM_ROS_CUSTOM_ROS1_EMPTY_PROVIDER
#endif

namespace uwvm2::ros_gateway::ros1_custom
{
    inline bool known(ros::capability_spec const& capability)
    {
        ::std::size_t matches{};
        bool qualified{};
        (void)capability;
#define UWVM_ROS1_CUSTOM_KNOWN(Native, Type, Identity, CodecIdentity, WireBytes, Codec) \
        if(::std::string_view{Type} == capability.type) \
        { ++matches; qualified = binding<Native, Codec>::known(capability, Type, Identity, CodecIdentity, WireBytes); }
        UWVM_ROS_CUSTOM_ROS1_TYPES(UWVM_ROS1_CUSTOM_KNOWN)
#undef UWVM_ROS1_CUSTOM_KNOWN
        // Count every row for the requested type, including an invalid row.
        // One valid row cannot conceal a stale or conflicting second binding.
        return matches == 1 && qualified;
    }

    inline ipc::call_result publish(topic_store& store, ros::capability_spec const& capability,
                                    ::std::vector<::std::uint8_t> const& bytes)
    {
        (void)store; (void)capability; (void)bytes;
        if(!known(capability)) { return {13, {}}; }
#define UWVM_ROS1_CUSTOM_PUBLISH(Native, Type, Identity, CodecIdentity, WireBytes, Codec) \
        if(binding<Native, Codec>::known(capability, Type, Identity, CodecIdentity, WireBytes)) \
        { return binding<Native, Codec>::publish(store, capability, bytes); }
        UWVM_ROS_CUSTOM_ROS1_TYPES(UWVM_ROS1_CUSTOM_PUBLISH)
#undef UWVM_ROS1_CUSTOM_PUBLISH
        return {95, {}};
    }

    inline ipc::call_result subscribe(topic_store& store, ros::capability_spec const& capability,
                                      ::std::shared_ptr<event_queue> events)
    {
        (void)store; (void)capability; (void)events;
        if(!known(capability)) { return {13, {}}; }
#define UWVM_ROS1_CUSTOM_SUBSCRIBE(Native, Type, Identity, CodecIdentity, WireBytes, Codec) \
        if(binding<Native, Codec>::known(capability, Type, Identity, CodecIdentity, WireBytes)) \
        { return binding<Native, Codec>::subscribe(store, capability, ::std::move(events)); }
        UWVM_ROS_CUSTOM_ROS1_TYPES(UWVM_ROS1_CUSTOM_SUBSCRIBE)
#undef UWVM_ROS1_CUSTOM_SUBSCRIBE
        return {95, {}};
    }
}

#if defined(UWVM_ROS_CUSTOM_ROS1_EMPTY_PROVIDER)
# undef UWVM_ROS_CUSTOM_ROS1_TYPES
# undef UWVM_ROS_CUSTOM_ROS1_EMPTY_PROVIDER
#endif
