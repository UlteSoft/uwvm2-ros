/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once

#include <algorithm>
#include <span>
#include <vector>
#include "../common/message.h"

namespace uwvm2::imported::ros
{
    inline bool valid_identifier(::std::string_view name) noexcept
    {
        if(name.empty() || name.size() > 255) { return false; }
        for(char c : name)
        { if(!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-' || c == '.')) { return false; } }
        return name != "." && name != "..";
    }

    inline bool valid_module_name(::std::string_view name) noexcept
    {
        if(name.empty() || name.size() > 1024) { return false; }
        for(unsigned char c : name) { if(c < 0x20 || c == 0x7f) { return false; } }
        return valid_utf8({reinterpret_cast<::std::byte const*>(name.data()), name.size()});
    }

    inline bool canonical_resource(::std::string_view name, bool allow_root = false) noexcept
    {
        if(name == "/") { return allow_root; }
        if(name.empty() || name.size() > 1024 || name.front() != '/' || name.back() == '/') { return false; }
        bool segment_start{true};
        for(::std::size_t i{1}; i < name.size(); ++i)
        {
            char c{name[i]};
            if(c == '/') { if(segment_start) { return false; } segment_start = true; continue; }
            bool alpha{(c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'};
            if(!alpha && !(c >= '0' && c <= '9' && !segment_start)) { return false; }
            segment_start = false;
        }
        return !segment_start;
    }

    inline ::std::vector<::std::string_view> operation_schema(version v, ::std::string_view operation, ::std::string_view type = {})
    {
        using list = ::std::vector<::std::string_view>;
        list fields{};
        if(operation == "event.observe")
        {
            fields = {"event-source-alias", "event-kinds", "max-event-bytes", "max-event-queue-depth", "max-event-queue-bytes",
                      "max-inflight", "max-rate-hz", "burst", "max-bytes-per-second", "overflow-policy", "audit"};
        }
        else if(operation == "topic.publish" || operation == "topic.subscribe")
        {
            fields = {"max-message-bytes", "max-rate-hz", "burst", "max-bytes-per-second", "max-queue-bytes", "overflow-policy", "validator", "audit"};
            fields.push_back(operation == "topic.publish" ? "max-publishers" : "max-subscribers");
            if(v == version::ros1)
            {
                fields.insert(fields.end(), {"queue-depth", "transport"});
                if(operation == "topic.publish") { fields.push_back("latch"); }
            }
            else
            {
                fields.insert(fields.end(), {"max-inflight", "qos.reliability", "qos.durability", "qos.history", "qos.depth", "qos.deadline", "qos.lifespan", "qos.liveliness", "qos.liveliness-lease-duration"});
            }
        }
        else if(operation == "service.available")
        {
            fields = {"service-source-alias", "timeout-ms", "max-wait-ns", "max-native-clients", "max-binding-history",
                      "max-inflight", "max-request-bytes", "max-response-bytes", "max-rate-hz", "burst", "max-bytes-per-second", "audit"};
            if(v == version::ros2) { fields.insert(fields.end(), {"qos.reliability", "qos.durability", "qos.history", "qos.depth", "qos.deadline", "qos.lifespan", "qos.liveliness", "qos.liveliness-lease-duration"}); }
        }
        else if(operation == "service.call" || operation == "service.advertise" || operation == "service.respond")
        {
            fields = {"timeout-ms", "max-inflight", "max-request-bytes", "max-response-bytes", "max-rate-hz", "burst", "max-bytes-per-second", "validator", "audit"};
            if(operation == "service.advertise") { fields.insert(fields.end(), {"max-services", "max-event-queue-depth", "max-event-queue-bytes", "overflow-policy"}); }
            if(v == version::ros2) { fields.insert(fields.end(), {"qos.reliability", "qos.durability", "qos.history", "qos.depth", "qos.deadline", "qos.lifespan", "qos.liveliness", "qos.liveliness-lease-duration"}); }
        }
        else if(operation.starts_with("param."))
        {
            bool known{operation == "param.get" || operation == "param.set" || operation == "param.list"};
            if(v == version::ros1) { known = known || operation == "param.delete" || operation == "param.exists" || operation == "param.search"; }
            else { known = known || operation == "param.declare" || operation == "param.undeclare" || operation == "param.describe" || operation == "param.subscribe-events"; }
            if(!known) { return {}; }
            fields = {"max-parameter-bytes", "timeout-ms", "max-inflight", "max-rate-hz", "burst", "max-bytes-per-second", "validator", "audit"};
            if(v == version::ros2) { fields.insert(fields.end(), {"parameter-node", "parameter-name"}); if(operation == "param.list") { fields.push_back("list-depth"); } }
            if(operation == "param.list" || operation == "param.search") { fields.push_back("allowed-name-prefix"); }
            if(operation == "param.subscribe-events") { fields.insert(fields.end(), {"max-subscribers", "max-event-fields", "max-event-queue-depth", "max-event-queue-bytes", "overflow-policy", "qos.reliability", "qos.durability", "qos.history", "qos.depth", "qos.deadline", "qos.lifespan", "qos.liveliness", "qos.liveliness-lease-duration"}); }
            else if(v == version::ros2 && operation != "param.declare" && operation != "param.undeclare")
            { fields.insert(fields.end(), {"qos.reliability", "qos.durability", "qos.history", "qos.depth", "qos.deadline", "qos.lifespan", "qos.liveliness", "qos.liveliness-lease-duration"}); }
        }
        else if(operation.starts_with("graph."))
        {
            if(operation != "graph.list-topics" && operation != "graph.list-nodes" && operation != "graph.list-services" && operation != "graph.inspect-topic" && operation != "graph.inspect-node" && operation != "graph.inspect-service" && !(v == version::ros2 && operation == "graph.inspect-action")) { return {}; }
            fields = {"allowed-name-prefix", "max-graph-result-bytes", "timeout-ms", "max-inflight", "max-rate-hz", "burst", "max-bytes-per-second", "audit"};
        }
        else if(operation.starts_with("action."))
        {
            if(operation != "action.send-goal" && operation != "action.cancel-own-goal" && operation != "action.cancel-any-goal" && operation != "action.receive-feedback" && operation != "action.receive-result" && operation != "action.advertise" && operation != "action.respond") { return {}; }
            fields = {"max-active-goals", "max-actions", "timeout-ms", "max-goal-bytes", "max-feedback-bytes", "max-result-bytes", "fibonacci.max-order", "cancel-scope", "max-rate-hz", "burst", "max-bytes-per-second", "max-event-queue-depth", "max-event-queue-bytes", "overflow-policy", "validator", "audit"};
            if(v == version::ros2) { fields.insert(fields.end(), {"qos.goal-service.reliability", "qos.goal-service.durability", "qos.goal-service.history", "qos.goal-service.depth", "qos.goal-service.deadline", "qos.goal-service.lifespan", "qos.goal-service.liveliness", "qos.goal-service.liveliness-lease-duration", "qos.cancel-service.reliability", "qos.cancel-service.durability", "qos.cancel-service.history", "qos.cancel-service.depth", "qos.cancel-service.deadline", "qos.cancel-service.lifespan", "qos.cancel-service.liveliness", "qos.cancel-service.liveliness-lease-duration", "qos.result-service.reliability", "qos.result-service.durability", "qos.result-service.history", "qos.result-service.depth", "qos.result-service.deadline", "qos.result-service.lifespan", "qos.result-service.liveliness", "qos.result-service.liveliness-lease-duration", "qos.feedback.reliability", "qos.feedback.durability", "qos.feedback.history", "qos.feedback.depth", "qos.feedback.deadline", "qos.feedback.lifespan", "qos.feedback.liveliness", "qos.feedback.liveliness-lease-duration", "qos.status.reliability", "qos.status.durability", "qos.status.history", "qos.status.depth", "qos.status.deadline", "qos.status.lifespan", "qos.status.liveliness", "qos.status.liveliness-lease-duration"}); }
        }
        else if(operation == "tf.lookup" || operation == "tf.broadcast")
        {
            fields = {"source-frame", "target-frame", "tf.max-translation-abs", "tf.quaternion-norm-tolerance", "timeout-ms", "max-message-bytes", "max-rate-hz", "burst", "max-bytes-per-second", "max-inflight", "max-queue-bytes", "overflow-policy", "buffer-cache-ms", "validator", "audit"};
            if(v == version::ros1) { fields.insert(fields.end(), {"queue-depth", "transport"}); if(operation == "tf.broadcast") { fields.push_back("latch"); } }
            else { fields.insert(fields.end(), {"qos.reliability", "qos.durability", "qos.history", "qos.depth", "qos.deadline", "qos.lifespan", "qos.liveliness", "qos.liveliness-lease-duration"}); }
        }
        else if(operation == "time.read-ros" || (v == version::ros1 && operation == "time.read-wall") || (v == version::ros2 && (operation == "time.read-steady" || operation == "time.read-system")))
        { fields = {"max-inflight", "max-response-bytes", "max-bytes-per-second", "max-rate-hz", "burst", "audit"}; }
        else if(operation == "log.write")
        { fields = {"max-inflight", "max-message-bytes", "max-bytes-per-second", "max-log-bytes-per-second", "max-rate-hz", "burst", "severity", "audit"}; }
        else if(v == version::ros2 && (operation == "lifecycle.get-state" || operation == "lifecycle.change-state"))
        {
            fields = {"timeout-ms", "max-inflight", "max-request-bytes", "max-response-bytes", "max-rate-hz", "burst", "max-bytes-per-second", "audit"};
            if(operation == "lifecycle.change-state") { fields.push_back("allowed-transition"); }
            fields.insert(fields.end(), {"qos.reliability", "qos.durability", "qos.history", "qos.depth", "qos.deadline", "qos.lifespan", "qos.liveliness", "qos.liveliness-lease-duration"});
        }
        // Generic operation discovery retains its original field list. Once a
        // trusted custom type is explicitly selected, its reviewed codec replaces
        // the builtin Fibonacci contract; no type is inferred from guest bytes.
        if(ipc::find_custom_action(static_cast<unsigned>(v), type) && operation.starts_with("action."))
        { fields.erase(::std::remove(fields.begin(), fields.end(), "fibonacci.max-order"), fields.end()); }
        if(!fields.empty() && (ipc::find_custom_topic_for_operation(static_cast<unsigned>(v), type, operation) ||
           ipc::find_custom_service(static_cast<unsigned>(v), type) || ipc::find_custom_action(static_cast<unsigned>(v), type)))
        { fields.push_back("adapter-codec-identity"); }
        if(!fields.empty()) { fields.insert(fields.end(), {"audit-max-rate-hz", "audit-burst", "audit-max-records", "audit-max-bytes"}); }
        return fields;
    }

    inline bool backend_supported(version v, ::std::string_view operation, ::std::string_view type) noexcept
    {
        bool service_type{type == "std_srvs/Trigger" || type == "std_srvs/SetBool" || type == "std_srvs/srv/Trigger" || type == "std_srvs/srv/SetBool" ||
                          (v == version::ros2 && (type == "std_srvs/srv/Empty" || type == "example_interfaces/srv/AddTwoInts"))};
        bool topic_type{type == "std_msgs/String" || type == "std_msgs/Bool" || type == "geometry_msgs/Twist" || type == "sensor_msgs/Image" || type == "std_msgs/msg/String" || type == "std_msgs/msg/Bool" || type == "geometry_msgs/msg/Twist" || type == "sensor_msgs/msg/Image"};
        if(!valid_version(v)) { return false; }
        if(operation == "event.observe") { return type == ipc::event_type; }
        if(ipc::find_custom_topic_for_operation(static_cast<unsigned>(v), type, operation))
        { return operation == "topic.publish" || operation == "topic.subscribe"; }
        if(ipc::find_custom_service(static_cast<unsigned>(v), type))
        { return operation == "service.call" || operation == "service.advertise" || operation == "service.respond" || operation == "service.available"; }
        if(ipc::find_custom_action(static_cast<unsigned>(v), type))
        { return operation == "action.send-goal" || operation == "action.cancel-own-goal" || operation == "action.cancel-any-goal" ||
                 operation == "action.receive-feedback" || operation == "action.receive-result" || operation == "action.advertise" || operation == "action.respond"; }
        if(operation.starts_with("time."))
        { return type == "builtin/time-v1" && (operation == "time.read-ros" || (v == version::ros1 && operation == "time.read-wall") || (v == version::ros2 && (operation == "time.read-system" || operation == "time.read-steady"))); }
        if(operation == "log.write") { return type == "builtin/log-v1"; }
        if(operation == "tf.lookup" || operation == "tf.broadcast") { return (v == version::ros1 && type == "geometry_msgs/TransformStamped") || (v == version::ros2 && type == "geometry_msgs/msg/TransformStamped"); }
        if(v == version::ros2 && operation == "lifecycle.get-state") { return type == "lifecycle_msgs/srv/GetState"; }
        if(v == version::ros2 && operation == "lifecycle.change-state") { return type == "lifecycle_msgs/srv/ChangeState"; }
        if(operation == "graph.list-topics" || operation == "graph.list-nodes" || operation == "graph.list-services" || operation == "graph.inspect-topic" || operation == "graph.inspect-node" || operation == "graph.inspect-service" || (v == version::ros2 && operation == "graph.inspect-action")) { return type == "builtin/graph-list-v1"; }
        if(ipc::scalar_parameter_operation(v,operation)) { return ipc::parameter_type(v,type) != ipc::parameter_kind::unsupported; }
        if(v == version::ros1 && (operation == "param.delete" || operation == "param.exists" || operation == "param.search" || operation == "param.list")) { return type == "builtin/string-v1"; }
        if(v == version::ros2 && (operation == "param.describe" || operation == "param.list")) { return type == "builtin/string-v1"; }
        if(v == version::ros2 && operation == "param.subscribe-events") { return type == "rcl_interfaces/msg/ParameterEvent" || ipc::parameter_event_type(v,type)!=ipc::parameter_kind::unsupported; }
        if(operation.starts_with("action."))
        {
            bool known{operation == "action.send-goal" || operation == "action.cancel-own-goal" || operation == "action.cancel-any-goal" || operation == "action.receive-feedback" || operation == "action.receive-result" || operation == "action.advertise" || operation == "action.respond"};
            return known && ((v == version::ros1 && type == "actionlib_tutorials/Fibonacci") || (v == version::ros2 && type == "action_tutorials_interfaces/action/Fibonacci"));
        }
        return !builtin_type_identity(v, type).empty() && (((operation == "service.call" || operation == "service.advertise" || operation == "service.respond" || operation == "service.available") && service_type) || ((operation == "topic.publish" || operation == "topic.subscribe") && topic_type));
    }

    inline result validate_capability(version v, capability_spec const& cap)
    {
        if(!valid_version(v) || !valid_identifier(cap.alias)) { return {errc::invalid_argument, "invalid ROS version or capability alias"}; }
        auto const fields{operation_schema(v, cap.operation, cap.type)};
        auto custom{ipc::find_custom_topic_for_operation(static_cast<unsigned>(v), cap.type, cap.operation)};
        auto custom_service{ipc::find_custom_service(static_cast<unsigned>(v), cap.type)};
        auto custom_action{ipc::find_custom_action(static_cast<unsigned>(v), cap.type)};
        if(fields.empty()) { return {errc::unsupported, "unknown operation for this ROS version: " + cap.operation}; }
        if(!canonical_resource(cap.resource, cap.operation.starts_with("graph.") || cap.operation == "param.list"))
        { return {errc::invalid_argument, "resource must be a canonical absolute ROS name"}; }
        for(auto field : fields)
        {
            if(!resource_quota_field(field) && !cap.limits.contains(field)) { return {errc::invalid_argument, "missing explicit field: " + ::std::string(field)}; }
        }
        for(auto const& [key, value] : cap.limits)
        {
            auto const key_view{::std::string_view{key}};
            bool const exhaust_action{key_view.ends_with(".on-exhausted")};
            bool const exit_action{key_view.ends_with(".on-proc-exit")};
            if(exhaust_action || exit_action)
            {
                auto base{key_view.substr(0,key_view.size()-(exhaust_action?13u:13u))};
                bool const native{base=="native-wait"};
                bool const pending{base=="max-pending-events" || base=="max-pending-event-bytes"};
                if((native && !exit_action) || (!native && ((!resource_quota_field(base)) ||
                    (::std::find(fields.begin(),fields.end(),base)==fields.end() && !pending && base!="max-parameter-elements"))))
                { return {errc::invalid_argument,"wait action does not apply to operation: "+key}; }
                if(exit_action)
                { if(value!="cancel" && value!="wait") { return {errc::invalid_argument,"exit action requires cancel or wait: "+key}; } }
                else if(value!="reject" && value!="wait")
                { return {errc::invalid_argument,"quota action requires reject or wait: "+key}; }
                else if(value=="wait" && !replenishing_quota(base))
                { return {errc::invalid_argument,"non-replenishing quota cannot wait: "+key}; }
                continue;
            }
            bool parameter_bound{key == "parameter-min-value" || key == "parameter-max-value"};
            bool parameter_elements{key == "max-parameter-elements"};
            bool bound_field{key == "twist.linear-max-abs" || key == "twist.angular-max-abs"};
            bool parent_search{v == version::ros1 && cap.operation == "param.search" && key == "search-parent-namespaces"};
            bool custom_codec{(custom || custom_service || custom_action) && key == "adapter-codec-identity"};
            bool pending_field{key == "max-pending-events" || key == "max-pending-event-bytes"};
            if(::std::find(fields.begin(), fields.end(), key) == fields.end() && !bound_field && !parameter_bound && !parameter_elements && !parent_search && !custom_codec && !pending_field)
            { return {errc::invalid_argument, "field does not apply to operation: " + key}; }
            if(value.empty() || value.size() > 1024) { return {errc::invalid_argument, "empty or oversized field: " + key}; }
            if(resource_quota_field(key))
            {
                if(value == "unlimited") { continue; }
                if(key == "max-rate-hz" || key == "audit-max-rate-hz")
                {
                    double rate{};
                    if(value!="0" && !parse_positive_double(value,rate)) { return {errc::invalid_argument,"finite nonnegative rate or unlimited required: " + key}; }
                }
                else
                {
                    ::std::uint64_t number{};
                    if(!quota_integer(value,number)) { return {errc::invalid_argument,"uint64 quota or unlimited required: " + key}; }
                }
            }
            else if(key == "tf.max-translation-abs" || key == "tf.quaternion-norm-tolerance" || bound_field)
            {
                double number{};
                if(!parse_positive_double(value, number) || number > 1000000. || (key == "tf.quaternion-norm-tolerance" && number > 0.001)) { return {errc::invalid_argument, "invalid finite positive rate or bound: " + key}; }
            }
            else if(key.starts_with("max-") || key == "burst" || key == "queue-depth" || key == "timeout-ms" || (key.starts_with("qos.") && key.ends_with(".depth")) || key == "audit-burst" || key == "audit-max-records" || key == "audit-max-bytes" || key == "list-depth" || key == "buffer-cache-ms")
            {
                ::std::uint64_t number{};
                if(!parse_u64(value, number) || !number) { return {errc::invalid_argument, "positive integer required: " + key}; }
                auto ceiling{::std::uint64_t{1048576}};
                if(key.find("bytes") != ::std::string::npos) { ceiling = key.find("per-second") != ::std::string::npos ? 1073741824 : 67108864; }
                if(key == "timeout-ms" || key == "buffer-cache-ms") { ceiling = 60000; }
                if(cap.operation == "service.available" && key == "max-wait-ns") { ceiling = ipc::client_availability::maximum_wait_ns; }
                if(key == "list-depth") { ceiling = 32; }
                if(key == "max-message-bytes" || key == "max-request-bytes" || key == "max-response-bytes" || key == "max-parameter-bytes" || key == "max-graph-result-bytes" || key == "max-goal-bytes" || key == "max-feedback-bytes" || key == "max-result-bytes" || key == "max-event-bytes") { ceiling = 4193786; }
                if((key == "max-request-bytes" && cap.operation == "service.advertise") || (key == "max-response-bytes" && cap.operation == "service.respond")) { ceiling -= 8; }
                if(number > ceiling) { return {errc::invalid_argument, "field exceeds protocol safety ceiling: " + key}; }
            }
            else if(key == "parameter-node") { if(!canonical_resource(value)) { return {errc::invalid_argument, "parameter node must be explicit absolute name"}; } }
            else if(key == "service-source-alias") { if(!valid_identifier(value) || value == cap.alias) { return {errc::invalid_argument, "availability needs a distinct explicit service source alias"}; } }
            else if(key == "event-source-alias") { if(!valid_identifier(value) || value == cap.alias) { return {errc::invalid_argument, "event source must be a distinct explicit capability alias"}; } }
            else if(key == "event-kinds") { ::std::uint32_t kinds{}; if(!ipc::parse_event_kinds(value,kinds)) { return {errc::invalid_argument, "event kinds require an explicit ordered nonempty CSV"}; } }
            else if(key == "fibonacci.max-order") { ::std::uint64_t order{};if(!parse_u64(value,order)||order>46) { return {errc::invalid_argument,"Fibonacci maximum order must explicitly be 0..46"}; } }
            else if(key == "parameter-name")
            {
                if(cap.operation == "param.list") { if(value != "not-applicable") { return {errc::invalid_argument, "parameter list name must explicitly be not-applicable"}; } }
                else { auto slash_name{value}; ::std::replace(slash_name.begin(), slash_name.end(), '.', '/'); if(!canonical_resource("/" + slash_name)) { return {errc::invalid_argument, "parameter name requires canonical dotted segments"}; } }
            }
            else if(key == "allowed-name-prefix") { if(!canonical_resource(value, true)) { return {errc::invalid_argument, "name prefix must be canonical absolute"}; } }
            else if(parent_search) { if(value != "true" && value != "false") { return {errc::invalid_argument, "parent parameter search must explicitly be true or false"}; } }
            else if(key == "severity") { if(value != "debug" && value != "info" && value != "warn" && value != "error" && value != "fatal") { return {errc::invalid_argument, "invalid explicit log severity"}; } }
            else if(key == "cancel-scope")
            {
                auto expected{cap.operation == "action.cancel-own-goal" ? "own" : cap.operation == "action.cancel-any-goal" ? "any" : "not-applicable"};
                if(value != expected) { return {errc::invalid_argument, "cancel scope must match independent cancellation operation"}; }
            }
            else if(key == "source-frame" || key == "target-frame") { if(!canonical_resource(::std::string("/") + value)) { return {errc::invalid_argument, "TF frame must be explicit canonical frame name"}; } }
            else if(key == "allowed-transition") { if(value != "configure" && value != "activate" && value != "deactivate" && value != "cleanup" && value != "shutdown") { return {errc::invalid_argument, "invalid explicit lifecycle transition"}; } }
            else if(key == "audit")
            { if(value != "allow-and-deny" && value != "deny-only") { return {errc::invalid_argument, "audit requires allow-and-deny or deny-only"}; } }
            else if(key == "validator")
            { if(value != "none" && value != "finite" && value != "twist-bounds" && value != "transform-bounds") { return {errc::unsupported, "validator has no trusted implementation"}; } }
            else if(key == "overflow-policy")
            {
                if(value != "reject" && value != "wait") { return {errc::invalid_argument, "quota overflow requires reject or synchronous wait"}; }
            }
            else if(key == "transport") { if(v != version::ros1 || value != "tcpros") { return {errc::unsupported, "only explicit TCPROS transport is implemented"}; } }
            else if(key == "latch") { if(value != "true" && value != "false") { return {errc::invalid_argument, "latch must be true or false"}; } }
            else if((key.starts_with("qos.") && key.ends_with(".reliability"))) { if(value != "reliable" && value != "best-effort") { return {errc::invalid_argument, "invalid reliability"}; } }
            else if((key.starts_with("qos.") && key.ends_with(".durability"))) { if(value != "volatile" && value != "transient-local") { return {errc::invalid_argument, "invalid durability"}; } }
            else if((key.starts_with("qos.") && key.ends_with(".history"))) { if(value != "keep-last" && value != "keep-all") { return {errc::invalid_argument, "history requires keep-last or keep-all"}; } }
            else if((key.starts_with("qos.") && key.ends_with(".liveliness"))) { if(value != "automatic" && value != "manual-by-topic") { return {errc::invalid_argument, "invalid liveliness"}; } }
            else if(key.starts_with("qos."))
            {
                ::std::uint64_t ns{};
                if(value != "infinite" && (!parse_u64(value, ns) || !ns || ns > 86400000000000ULL))
                { return {errc::invalid_argument, "QoS duration needs explicit positive nanoseconds or infinite"}; }
            }
        }
        for(auto pair : {::std::pair{::std::string_view{"max-rate-hz"},::std::string_view{"burst"}},
                         ::std::pair{::std::string_view{"audit-max-rate-hz"},::std::string_view{"audit-burst"}}})
        {
            auto const rate{cap.limits.find(pair.first)};
            bool const finite_rate{rate!=cap.limits.end() && rate->second!="unlimited"};
            if(finite_rate != (quota_value(cap,pair.second)!=unlimited_quota))
            { return {errc::invalid_argument,"rate and burst must both be explicitly finite or both unlimited"}; }
        }
        if(!backend_supported(v, cap.operation, cap.type)) { return {errc::unsupported, "operation/type has no implemented trusted backend adapter"}; }
        if(cap.type_identity != trusted_type_identity(v, cap.type, cap.operation)) { return {errc::permission_denied, "type identity differs from trusted adapter"}; }
        if(custom && !custom_topic_codec_matches(v, cap))
        { return {errc::permission_denied, "custom Topic requires its explicit compiled adapter-codec-identity"}; }
        if((custom_service || custom_action) && !custom_interface_codec_matches(v, cap))
        { return {errc::permission_denied, "custom interface requires its explicit compiled adapter-codec-identity"}; }
        for(auto const& [key,value]:cap.limits)
        {
            if(key.starts_with("qos.") && key.ends_with(".history") && value=="keep-last")
            {
                auto const depth{quota_value(cap,key.substr(0,key.size()-7)+"depth")};
                if(!depth || depth==unlimited_quota)
                { return {errc::invalid_argument,"keep-last requires an explicit positive finite depth"}; }
            }
        }
        auto parameter_contract{ipc::validate_parameter_contract(v,cap)};
        if(parameter_contract) { return {static_cast<errc>(parameter_contract), "invalid explicit parameter type, bounds or byte quota"}; }
        auto validator{cap.limits.find("validator")};
        bool twist{cap.type == "geometry_msgs/Twist" || cap.type == "geometry_msgs/msg/Twist"};
        bool transform{cap.type == "geometry_msgs/TransformStamped" || cap.type == "geometry_msgs/msg/TransformStamped"};
        if(validator != cap.limits.end() && validator->second == "transform-bounds" && !transform)
        { return {errc::invalid_argument, "transform-bounds requires TransformStamped"}; }
        bool parameter_float{(ipc::scalar_parameter_operation(v,cap.operation) && ipc::float64_parameter(ipc::parameter_type(v,cap.type))) ||
                             (cap.operation=="param.subscribe-events" && ipc::float64_parameter(ipc::parameter_event_type(v,cap.type)))};
        if(validator != cap.limits.end() && validator->second != "none" && !twist && !(transform && validator->second == "transform-bounds") && !(parameter_float && validator->second == "finite"))
        { return {errc::invalid_argument, "numeric validator only applies to Twist"}; }
        if(validator != cap.limits.end() && validator->second == "twist-bounds")
        {
            if(!cap.limits.contains("twist.linear-max-abs") || !cap.limits.contains("twist.angular-max-abs"))
            { return {errc::invalid_argument, "Twist bounds must both be explicit"}; }
        }
        else if(cap.limits.contains("twist.linear-max-abs") || cap.limits.contains("twist.angular-max-abs"))
        { return {errc::invalid_argument, "Twist bounds require twist-bounds validator"}; }
        if(cap.operation == "tf.lookup" || cap.operation == "tf.broadcast")
        {
            if(cap.resource != "/tf" && cap.resource != "/tf_static") { return {errc::invalid_argument, "TF resource must explicitly be /tf or /tf_static"}; }
            if(cap.limits.at("source-frame") == cap.limits.at("target-frame")) { return {errc::invalid_argument, "TF source and target frames must differ"}; }
            if(cap.limits.at("validator") != "transform-bounds") { return {errc::invalid_argument, "TF requires explicit transform-bounds validator"}; }
        }
        if(v == version::ros2 && cap.operation.starts_with("param."))
        {
            auto resource{cap.limits.at("parameter-node")};
            if(cap.operation != "param.list") { auto name{cap.limits.at("parameter-name")}; ::std::replace(name.begin(), name.end(), '.', '/'); resource += "/" + name; }
            if(resource != cap.resource) { return {errc::invalid_argument, "resource must equal explicit parameter node/name scope"}; }
        }
        // Independent optional quotas impose no synthetic relationship on
        // one another. Each actual request is checked before its native effect.
        return {};
    }
}
