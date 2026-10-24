/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include "../common/format.h"

#include <array>
#include <set>
#include <tuple>
#include "schema.h"
#include "../common/manifest.h"
#include "../common/action_qos_contract.h"
#include "../common/service_availability.h"

namespace uwvm2::imported::ros
{
    inline bool connection_key(version v, ::std::string_view key) noexcept
    {
        if(key == "call-boundary") { return true; }
        if(key == "audit-deny-max-rate-hz" || key == "audit-deny-burst" || key == "audit-deny-max-records" || key == "audit-deny-max-bytes") { return true; }
        if(key == "gateway-socket" || key == "gateway-uid" || key == "node-name" || key == "namespace" || key == "connect-timeout-ms") { return true; }
        if(v == version::ros1) { return key == "master-uri" || key == "advertise-host" || key == "transport"; }
        return key == "domain-id" || key == "rmw-implementation" || key == "security-mode" || key == "keystore" || key == "enclave";
    }

    inline result validate_connection_value(version v, ::std::string_view key, ::std::string_view value)
    {
        if(!valid_version(v) || !connection_key(v, key) || value.empty() || value.size() > 1024 || value.find('\0') != ::std::string_view::npos)
        {
            return {errc::invalid_argument, "unknown or empty connection field"};
        }
        if(key == "call-boundary")
        {
            if(value != "same-module" && value != "module-exports") { return {errc::invalid_argument, "call-boundary requires same-module or module-exports"}; }
        }
        else if(key == "audit-deny-max-rate-hz")
        {
            double n{};
            if(!parse_positive_double(value, n) || n > 1000000.) { return {errc::invalid_argument, "denial audit rate must be finite positive <=1000000"}; }
        }
        else if(key == "audit-deny-burst" || key == "audit-deny-max-records" || key == "audit-deny-max-bytes")
        {
            ::std::uint64_t n{}; auto maximum{key == "audit-deny-max-bytes" ? 67108864ULL : 1048576ULL};
            if(!parse_u64(value, n) || !n || n > maximum) { return {errc::invalid_argument, "denial audit integer limit outside public range"}; }
        }
        else if(key == "gateway-socket")
        {
            if(value.front() != '/' || value.size() > 107 || value.find('\0') != ::std::string_view::npos || value.find("/../") != ::std::string_view::npos || value.ends_with("/..") || value.find("/./") != ::std::string_view::npos || value.find("//") != ::std::string_view::npos)
            { return {errc::invalid_argument, "gateway socket needs a canonical absolute Unix socket path"}; }
        }
        else if(key == "node-name")
        {
            if(value.find('/') != ::std::string_view::npos || !canonical_resource(::fast_io::concat_std("/",value))) { return {errc::invalid_argument, "invalid ROS node basename"}; }
        }
        else if(key == "namespace")
        { if(!canonical_resource(value, true)) { return {errc::invalid_argument, "namespace must be canonical and absolute"}; } }
        else if(key == "gateway-uid" || key == "domain-id" || key == "connect-timeout-ms")
        {
            ::std::uint64_t n{};
            if(!parse_u64(value, n) || (key == "gateway-uid" && n > 4294967294ULL) || (key == "domain-id" && n > 232) || (key == "connect-timeout-ms" && (!n || n > 60000)))
            { return {errc::invalid_argument, "connection number outside explicit range"}; }
        }
        else if(key == "transport") { if(value != "tcpros") { return {errc::unsupported, "only TCPROS is implemented"}; } }
        else if(key == "security-mode") { if(value != "enforce" && value != "disabled-test") { return {errc::invalid_argument, "security-mode requires enforce or disabled-test"}; } }
        else if(key == "keystore")
        { if(value != "not-applicable" && (value.front() != '/' || value.find('\0') != ::std::string_view::npos)) { return {errc::invalid_argument, "keystore requires absolute path or explicit not-applicable"}; } }
        else if(key == "enclave")
        { if(value != "not-applicable" && !canonical_resource(value, true)) { return {errc::invalid_argument, "enclave requires canonical absolute name or not-applicable"}; } }
        else if(key == "master-uri")
        {
            if(!value.starts_with("http://") || value.find_first_of(" \t\r\n?#@") != ::std::string_view::npos)
            { return {errc::invalid_argument, "Master URI must be explicit http://host:port"}; }
            auto authority{value.substr(7)};
            if(authority.ends_with('/')) { authority.remove_suffix(1); }
            auto colon{authority.rfind(':')};
            ::std::uint64_t port{};
            if(colon == ::std::string_view::npos || !colon || authority.substr(0, colon).find('/') != ::std::string_view::npos || !parse_u64(authority.substr(colon + 1), port) || !port || port > 65535)
            { return {errc::invalid_argument, "Master URI requires an explicit port"}; }
        }
        else if(value.find_first_of(" \t\r\n/\\\0") != ::std::string_view::npos)
        { return {errc::invalid_argument, "invalid connection identity"}; }
        return {};
    }

    inline result validate_target(target_policy const& target)
    {
        if(!valid_version(target.ros_version) || (target.target_scope == scope::single ? !valid_module_name(target.name) : target.target_scope == scope::group ? !valid_identifier(target.name) : target.target_scope != scope::global))
        { return {errc::invalid_argument, "invalid target identity"}; }
        // Delegation belongs to one explicitly named library module. A Global
        // or Group policy cannot turn every member's exports into ROS authority.
        if(target.connection.contains("call-boundary") && target.target_scope != scope::single)
        {
            return {errc::invalid_argument, "call-boundary requires a Single module target"};
        }
        for(auto const& [key, value]: target.connection)
        {
            if(auto r{validate_connection_value(target.ros_version, key, value)}; !r) { return r; }
        }
        if(target.enabled)
        {
            ::std::vector<::std::string_view> required{"gateway-socket",
                                                       "gateway-uid",
                                                       "node-name",
                                                       "namespace",
                                                       "connect-timeout-ms",
                                                       "audit-deny-max-rate-hz",
                                                       "audit-deny-burst",
                                                       "audit-deny-max-records",
                                                       "audit-deny-max-bytes"};
            if(target.ros_version == version::ros1) { required.insert(required.end(), {"master-uri", "advertise-host", "transport"}); }
            else
            {
                required.insert(required.end(), {"domain-id", "rmw-implementation", "security-mode", "keystore", "enclave"});
            }
            for(auto key: required)
            {
                if(!target.connection.contains(key))
                {
                    return {errc::invalid_argument, ::fast_io::concat_std("enabled target missing explicit connection field: ", ::std::string_view{key})};
                }
            }
            if(target.ros_version == version::ros2)
            {
                bool enforce{target.connection.at("security-mode") == "enforce"};
                bool keystore_unused{target.connection.at("keystore") == "not-applicable"};
                bool enclave_unused{target.connection.at("enclave") == "not-applicable"};
                if((enforce && (keystore_unused || enclave_unused)) || (!enforce && (!keystore_unused || !enclave_unused)))
                {
                    return {errc::invalid_argument, "ROS2 security mode and explicit keystore/enclave conflict"};
                }
            }
        }
        if(!target.enabled)
        {
            unsigned explicit_audit{};
            for(auto key : {"audit-deny-max-rate-hz", "audit-deny-burst", "audit-deny-max-records", "audit-deny-max-bytes"}) { explicit_audit += target.connection.contains(key); }
            if(explicit_audit && explicit_audit != 4) { return {errc::invalid_argument, "disabled target denial audit fields must be supplied all together"}; }
        }
        ::std::set<::std::tuple<::std::string, ::std::string, ::std::string>> duplicates{};
        ::std::map<::std::string, ::std::string, ::std::less<>> resource_types{};
        for(auto const& [alias, cap] : target.capabilities)
        {
            if(alias != cap.alias) { return {errc::invalid_argument, "capability alias key mismatch"}; }
            auto r{validate_capability(target.ros_version, cap)};
            if(!r && (target.enabled || r.code != errc::unsupported)) { return r; }
            auto duplicate_resource{cap.resource};
            if(cap.operation == "event.observe")
            {
                auto source{target.capabilities.find(cap.limits.at("event-source-alias"))};
                if(source == target.capabilities.end() || source->second.operation == "event.observe" || source->second.resource != cap.resource)
                { return {errc::invalid_argument, "event observer requires a present source on the exact same resource"}; }
                ::std::uint32_t kinds{};ipc::parse_event_kinds(cap.limits.at("event-kinds"),kinds);
                ipc::event_kind payload{};::std::uint8_t flags{};
                bool has_payload{ipc::event_source_kind(source->second.operation,payload,flags)};
                auto allowed{ipc::event_bit(ipc::event_kind::resource_error)|ipc::event_bit(ipc::event_kind::resource_closed)};
                if(has_payload) { allowed |= ipc::event_bit(payload); }
                if(kinds & ~allowed) { return {errc::permission_denied, "event payload kind exceeds its independently granted source operation"}; }
                ::fast_io::print(::fast_io::ostring_ref_std{::std::addressof(duplicate_resource)},"|",source->first);
            }
            if(cap.operation == "service.available")
            {
                auto source{target.capabilities.find(cap.limits.at("service-source-alias"))};
                if(source==target.capabilities.end()||!service_availability_pair(target.ros_version,cap,source->second))
                { return {errc::permission_denied, "availability requires its independently granted exact typed service.call source and QoS"}; }
            }
            if(!duplicates.emplace(cap.operation, duplicate_resource, cap.type).second) { return {errc::conflict, "duplicate resource grant under another alias"}; }
            if(cap.operation.starts_with("topic.") || cap.operation.starts_with("service.") || cap.operation.starts_with("action."))
            {
                auto const resource_key{::fast_io::concat_std(::std::string_view{cap.operation}.substr(0,cap.operation.find('.')),":",cap.resource)};
                auto type_key{::fast_io::concat_std(cap.type,"|",cap.type_identity)};
                // try_emplace leaves type_key intact when the key exists, so
                // the same owned bytes serve insertion or conflict checking.
                auto [it, inserted]{resource_types.try_emplace(resource_key,::std::move(type_key))};
                if(!inserted && it->second != type_key) { return {errc::conflict, "resource has incompatible trusted types"}; }
            }
        }
        if(target.ros_version == version::ros2)
        {
            for(auto const& [response_alias, response] : target.capabilities)
            {
                if(response.operation != "service.respond") { continue; }
                for(auto const& [server_alias, server] : target.capabilities)
                {
                    if(server.operation != "service.advertise" || server.resource != response.resource || server.type != response.type || server.type_identity != response.type_identity) { continue; }
                    for(auto field : {"qos.reliability", "qos.durability", "qos.history", "qos.depth", "qos.deadline", "qos.lifespan", "qos.liveliness", "qos.liveliness-lease-duration"})
                    {
                        if(server.limits.at(field) != response.limits.at(field))
                        { return {errc::conflict, "service response QoS differs from its explicit server contract"}; }
                    }
                }
            }
            for(auto const& [response_alias, response] : target.capabilities)
            {
                if(response.operation != "action.respond") { continue; }
                for(auto const& [server_alias, server] : target.capabilities)
                {
                    if(server.operation != "action.advertise" || server.resource != response.resource || server.type != response.type || server.type_identity != response.type_identity) { continue; }
                    if(!equal_action_server_qos_contract(server, response))
                    { return {errc::conflict, "action response QoS differs from its explicit server contract"}; }
                }
            }
        }
        return {};
    }

    inline ::std::string json_quote(::std::string_view text)
    { return ::fast_io::concat_std(json_quoted{text}); }

    class policy_store
    {
        using key_type = ::std::tuple<version, scope, ::std::string>;
        using key_view = ::std::tuple<version, scope, ::std::string_view>;
        struct target_less
        {
            using is_transparent = void;
            template<typename LeftName, typename RightName>
            bool operator()(::std::tuple<version, scope, LeftName> const& left, ::std::tuple<version, scope, RightName> const& right) const noexcept
            {
                return key_view{::std::get<0>(left), ::std::get<1>(left), ::std::string_view{::std::get<2>(left)}} <
                       key_view{::std::get<0>(right), ::std::get<1>(right), ::std::string_view{::std::get<2>(right)}};
            }
        };
        struct binding_less
        {
            using is_transparent = void;
            template<typename LeftName, typename RightName>
            bool operator()(::std::pair<version, LeftName> const& left, ::std::pair<version, RightName> const& right) const noexcept
            {
                return ::std::pair{left.first, ::std::string_view{left.second}} < ::std::pair{right.first, ::std::string_view{right.second}};
            }
        };
        ::std::map<key_type, target_policy, target_less> targets{};
        ::std::map<::std::pair<version, ::std::string>, ::std::pair<scope, ::std::string>, binding_less> bindings{};

        static ::std::string_view target_name(scope s, ::std::string_view name) noexcept { return s == scope::global ? ::std::string_view{} : name; }
        target_policy* mutable_target(version v, scope s, ::std::string_view name)
        {
            auto it{targets.find(key_view{v, s, target_name(s, name)})};
            return it == targets.end() ? nullptr : &it->second;
        }
    public:
        policy_store()
        {
            for(auto v : {version::ros1, version::ros2}) { targets.emplace(key_type{v, scope::global, ""}, target_policy{v, scope::global, "", false, {}, {}}); }
        }
        result create_target(version v, scope s, ::std::string_view name)
        {
            if(!valid_version(v) || (s != scope::single && s != scope::group) || (s == scope::single ? !valid_module_name(name) : !valid_identifier(name)))
            { return {errc::invalid_argument, "only explicitly named Single/Group targets can be created"}; }
            if(targets.contains(key_view{v, s, name})) { return {errc::conflict, "target already exists"}; }
            if(s == scope::single && bindings.contains(::std::pair{v, name})) { return {errc::conflict, "module already has an explicit target in this ROS version"}; }
            targets.emplace(key_type{v, s, ::std::string(name)}, target_policy{v, s, ::std::string(name), false, {}, {}});
            if(s == scope::single) { bindings.emplace(::std::pair{v, ::std::string(name)}, ::std::pair{s, ::std::string(name)}); }
            return {};
        }
        result bind_module(version v, ::std::string_view group, ::std::string_view module)
        {
            if(!find_target(v, scope::group, group) || !valid_module_name(module)) { return {errc::invalid_argument, "unknown Group or invalid module binding"}; }
            if(!bindings.emplace(::std::pair{v, ::std::string(module)}, ::std::pair{scope::group, ::std::string(group)}).second)
            { return {errc::conflict, "module already explicitly bound in this ROS version"}; }
            return {};
        }
        result set_enabled(version v, scope s, ::std::string_view name, bool enabled)
        {
            auto target{mutable_target(v, s, name)};
            if(!target) { return {errc::invalid_argument, "unknown target"}; }
            target->enabled = enabled;
            return {};
        }
        result set_connection(version v, scope s, ::std::string_view name, ::std::string_view key, ::std::string_view value)
        {
            auto target{mutable_target(v, s, name)};
            if(!target) { return {errc::invalid_argument, "unknown target"}; }
            if(auto r{validate_connection_value(v, key, value)}; !r) { return r; }
            if(!target->connection.emplace(key, value).second) { return {errc::conflict, "connection field specified more than once"}; }
            return {};
        }
        result grant(version v, scope s, ::std::string_view name, capability_spec cap)
        {
            auto target{mutable_target(v, s, name)};
            if(!target || !valid_identifier(cap.alias) || !canonical_resource(cap.resource, cap.operation.starts_with("graph.") || cap.operation == "param.list") || operation_schema(v, cap.operation).empty())
            { return {errc::invalid_argument, "invalid target, alias, resource or operation"}; }
            if(cap.type.empty() || cap.type_identity.empty()) { return {errc::invalid_argument, "type and trusted identity must be explicit"}; }
            auto alias{cap.alias};
            if(!target->capabilities.emplace(::std::move(alias), ::std::move(cap)).second) { return {errc::conflict, "capability alias already granted"}; }
            return {};
        }
        result set_capability(version v, scope s, ::std::string_view name, ::std::string_view alias, ::std::string_view key, ::std::string_view value)
        {
            auto target{mutable_target(v, s, name)};
            if(!target) { return {errc::invalid_argument, "unknown target"}; }
            auto it{target->capabilities.find(alias)};
            if(it == target->capabilities.end()) { return {errc::invalid_argument, "unknown capability alias"}; }
            auto fields{operation_schema(v, it->second.operation, it->second.type)};
            bool parent_search{v == version::ros1 && it->second.operation == "param.search" && key == "search-parent-namespaces"};
            bool custom_codec{key == "adapter-codec-identity" &&
                              (ipc::find_custom_topic_for_operation(static_cast<unsigned>(v), it->second.type, it->second.operation) ||
                               ipc::find_custom_service(static_cast<unsigned>(v), it->second.type) ||
                               ipc::find_custom_action(static_cast<unsigned>(v), it->second.type))};
            bool pending_field{(key == "max-pending-events" || key == "max-pending-event-bytes") && ::std::find(fields.begin(),fields.end(),"overflow-policy") != fields.end()};
            if(value.empty() || value.size() > 1024 || (::std::find(fields.begin(), fields.end(), key) == fields.end() && key != "twist.linear-max-abs" && key != "twist.angular-max-abs" && key != "parameter-min-value" && key != "parameter-max-value" && key != "max-parameter-elements" && !parent_search && !custom_codec && !pending_field))
            { return {errc::invalid_argument, "unknown/inapplicable capability field"}; }
            if(parent_search && value != "true" && value != "false")
            { return {errc::invalid_argument, "parent parameter search must explicitly be true or false"}; }
            if(!it->second.limits.emplace(key, value).second) { return {errc::conflict, "capability field specified more than once"}; }
            return {};
        }
        target_policy const* find_target(version v, scope s, ::std::string_view name) const noexcept
        {
            auto it{targets.find(key_view{v, s, target_name(s, name)})};
            return it == targets.end() ? nullptr : &it->second;
        }
        target_policy const* effective(version v, ::std::string_view module) const noexcept
        {
            auto it{bindings.find(::std::pair{v, module})};
            if(it == bindings.end()) { return find_target(v, scope::global, ""); }
            return find_target(v, it->second.first, it->second.second);
        }
        ::std::vector<target_policy const*> configured_targets() const
        {
            ::std::vector<target_policy const*> out{};
            out.reserve(targets.size());
            for(auto const& item : targets) { out.push_back(&item.second); }
            return out;
        }
        result validate(::std::span<::std::string const> loaded_modules, bool verify_bindings = true) const
        {
            // These views only live until validation returns to the span's owner.
            ::std::set<::std::string_view, ::std::less<>> loaded{};
            for(auto const& name : loaded_modules)
            {
                if(!valid_module_name(name)) { return {errc::invalid_argument, "invalid loaded module name"}; }
                if(!loaded.emplace(name).second) { return {errc::conflict, "duplicate loaded module name"}; }
            }
            for(auto const& [key, target] : targets) { if(auto r{validate_target(target)}; !r) { return {r.code, ::fast_io::concat_std(to_string(target.ros_version), "/", to_string(target.target_scope), "/", target.name, ": ", r.detail)}; } }
            if(verify_bindings)
            {
                for(auto const& [module, target] : bindings)
                { if(!loaded.contains(module.second)) { return {errc::unknown_module, "ROS binding refers to absent module: " + module.second}; } }
            }
            return {};
        }
        ::std::string canonical_dump(version v, ::std::span<::std::string const> modules) const
        {
            ::std::string out{};
            ::fast_io::ostring_ref_std stream{::std::addressof(out)};
            ::fast_io::print(stream,"{\"version\":",json_quoted{to_string(v)},",\"targets\":[");
            bool first_target{true};
            for(auto const& [key, target] : targets)
            {
                if(target.ros_version != v) { continue; }
                if(!first_target) { ::fast_io::print(stream,","); } first_target = false;
                ::fast_io::print(stream,"{\"scope\":",json_quoted{to_string(target.target_scope)},
                    ",\"name\":",json_quoted{target.name},",\"enabled\":",::std::string_view{target.enabled ? "true" : "false"},",\"connection\":{");
                bool first{true};
                for(auto const& [field, value] : target.connection)
                { if(!first) { ::fast_io::print(stream,","); } first = false; ::fast_io::print(stream,json_quoted{field},":",json_quoted{value}); }
                ::fast_io::print(stream,"},\"capabilities\":["); first = true;
                for(auto const& [alias, cap] : target.capabilities)
                {
                    if(!first) { ::fast_io::print(stream,","); } first = false;
                    ::fast_io::print(stream,"{\"alias\":",json_quoted{alias},",\"operation\":",json_quoted{cap.operation},
                        ",\"resource\":",json_quoted{cap.resource},",\"type\":",json_quoted{cap.type},
                        ",\"type_identity\":",json_quoted{cap.type_identity},",\"limits\":{");
                    bool first_limit{true};
                    for(auto const& [field, value] : cap.limits)
                    { if(!first_limit) { ::fast_io::print(stream,","); } first_limit = false; ::fast_io::print(stream,json_quoted{field},":",json_quoted{value}); }
                    ::fast_io::print(stream,"}}");
                }
                ::fast_io::print(stream,"]}");
            }
            ::fast_io::print(stream,"],\"modules\":[");
            // The caller's strings outlive this local set; no borrowed key is retained.
            ::std::set<::std::string_view, ::std::less<>> sorted{modules.begin(), modules.end()};
            bool first{true};
            for(auto const& name : sorted)
            {
                if(!first) { ::fast_io::print(stream,","); } first = false;
                auto target{effective(v, name)};
                ::fast_io::print(stream,"{\"module\":",json_quoted{name},",\"scope\":",json_quoted{target ? to_string(target->target_scope) : "global"},
                    ",\"target\":",json_quoted{target ? ::std::string_view{target->name} : ::std::string_view{}},
                    ",\"enabled\":",::std::string_view{target && target->enabled ? "true" : "false"},"}");
            }
            ::fast_io::print(stream,"]}");
            return out;
        }

    };
}
