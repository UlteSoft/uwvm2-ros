// Frozen pre-optimization policy_store for byte/contract differential tests.
// Original policy.h SHA256: a2e1e3ae535cc463fdacb460ae7cc06c4feae43b32961113a511bca573d39327
// Only the class and constructor names differ from the frozen class body.
#pragma once
#include <uwvm2/imported/ros/policy/policy.h>

namespace uwvm2::imported::ros::policy_test
{
    class legacy_policy_store
    {
        using key_type = ::std::tuple<version, scope, ::std::string>;
        ::std::map<key_type, target_policy> targets{};
        ::std::map<::std::pair<version, ::std::string>, ::std::pair<scope, ::std::string>> bindings{};

        static ::std::string target_name(scope s, ::std::string_view name) { return s == scope::global ? ::std::string{} : ::std::string{name}; }
        target_policy* mutable_target(version v, scope s, ::std::string_view name)
        {
            auto it{targets.find({v, s, target_name(s, name)})};
            return it == targets.end() ? nullptr : &it->second;
        }
    public:
        legacy_policy_store()
        {
            for(auto v : {version::ros1, version::ros2}) { targets.emplace(key_type{v, scope::global, ""}, target_policy{v, scope::global, "", false, {}, {}}); }
        }
        result create_target(version v, scope s, ::std::string_view name)
        {
            if(!valid_version(v) || (s != scope::single && s != scope::group) || (s == scope::single ? !valid_module_name(name) : !valid_identifier(name)))
            { return {errc::invalid_argument, "only explicitly named Single/Group targets can be created"}; }
            if(targets.contains({v, s, ::std::string(name)})) { return {errc::conflict, "target already exists"}; }
            if(s == scope::single && bindings.contains({v, ::std::string(name)})) { return {errc::conflict, "module already has an explicit target in this ROS version"}; }
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
            auto fields{operation_schema(v, it->second.operation)};
            if(value.empty() || value.size() > 1024 || (::std::find(fields.begin(), fields.end(), key) == fields.end() && key != "twist.linear-max-abs" && key != "twist.angular-max-abs" && key != "parameter-min-value" && key != "parameter-max-value" && key != "max-parameter-elements"))
            { return {errc::invalid_argument, "unknown/inapplicable capability field"}; }
            if(!it->second.limits.emplace(key, value).second) { return {errc::conflict, "capability field specified more than once"}; }
            return {};
        }
        target_policy const* find_target(version v, scope s, ::std::string_view name) const noexcept
        {
            auto it{targets.find({v, s, target_name(s, name)})};
            return it == targets.end() ? nullptr : &it->second;
        }
        target_policy const* effective(version v, ::std::string_view module) const noexcept
        {
            auto it{bindings.find({v, ::std::string(module)})};
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
            ::std::set<::std::string, ::std::less<>> loaded{};
            for(auto const& name : loaded_modules)
            {
                if(!valid_module_name(name)) { return {errc::invalid_argument, "invalid loaded module name"}; }
                if(!loaded.emplace(name).second) { return {errc::conflict, "duplicate loaded module name"}; }
            }
            for(auto const& [key, target] : targets) { if(auto r{validate_target(target)}; !r) { return {r.code, ::std::string(to_string(target.ros_version)) + "/" + ::std::string(to_string(target.target_scope)) + "/" + target.name + ": " + r.detail}; } }
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
            ::std::set<::std::string, ::std::less<>> sorted{modules.begin(), modules.end()};
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
