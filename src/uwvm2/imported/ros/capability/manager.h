/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once

#include <chrono>
#include <memory>
#include <mutex>
#include <utility>
#include "../policy/policy.h"
#include "../common/audit.h"
#include "../common/debug.h"

namespace uwvm2::imported::ros
{
    class capability_manager;

    // Keep this lease alive until the side effect or event delivery has completed.
    // Revocation waits for earlier admitted operations of this instance/backend.
    class authorization
    {
        friend class capability_manager;
        ::std::shared_ptr<void> lifetime{};
        ::std::unique_lock<::std::recursive_mutex> lock{};
        capability_spec const* cap{};
        target_policy const* bound_target{};
        ::std::uint64_t identity{};
    public:
        authorization() = default;
        authorization(authorization const&) = delete;
        authorization& operator=(authorization const&) = delete;
        authorization(authorization&& other) noexcept { *this = ::std::move(other); }
        authorization& operator=(authorization&& other) noexcept
        {
            if(this != &other)
            {
                release(); lifetime = ::std::move(other.lifetime); lock = ::std::move(other.lock);
                cap = ::std::exchange(other.cap, nullptr); bound_target = ::std::exchange(other.bound_target, nullptr); identity=::std::exchange(other.identity,0);
            }
            return *this;
        }
        capability_spec const* capability() const noexcept { return lock.owns_lock() ? cap : nullptr; }
        target_policy const* target() const noexcept { return lock.owns_lock() ? bound_target : nullptr; }
        ::std::uint64_t instance_number() const noexcept { return lock.owns_lock()?identity:0; }
        explicit operator bool() const noexcept { return lock.owns_lock() && cap; }
        void release() noexcept { cap = nullptr; bound_target = nullptr; identity=0; if(lock.owns_lock()) { lock.unlock(); } lifetime.reset(); }
    };

    class capability_manager
    {
        struct alias_state
        {
            capability_spec cap{};
            bounded_audit audit{};
            bool revoked{};
            ::std::uint64_t open_count{};
            double rate{}, burst{}, bandwidth{}, tokens{}, creation_tokens{}, byte_tokens{};
            ::std::chrono::steady_clock::time_point updated{::std::chrono::steady_clock::now()};
        };
        struct handle_entry
        {
            ::std::uint32_t generation{1};
            ::std::string alias{};
            bool active{}, retired{};
        };
        struct backend_state
        {
            ::std::recursive_mutex mutex{};
            bool alive{true};
            target_policy policy{};
            ::std::string module{};
            ::std::uint64_t instance_id{};
            ::std::map<::std::string, alias_state, ::std::less<>> aliases{};
            ::std::map<::std::uint32_t, handle_entry> handles{};
        };
        struct instance_state
        {
            ::std::string module{};
            backend_state ros1{}, ros2{};
        };
        using identity = ::std::pair<void const*, ::std::uint64_t>;
        struct identity_less
        {
            bool operator()(identity const& a, identity const& b) const noexcept
            {
                if(::std::less<void const*>{}(a.first, b.first)) { return true; }
                if(::std::less<void const*>{}(b.first, a.first)) { return false; }
                return a.second < b.second;
            }
        };
        ::std::mutex registry_mutex{};
        ::std::map<identity, ::std::shared_ptr<instance_state>, identity_less> instances{};
        // Slots are never transferred to a different instance/backend after teardown.
        ::std::uint64_t next_slot{1};
        ::std::uint64_t next_instance_id{1};

        static backend_state& backend(instance_state& state, version v) noexcept { return v == version::ros1 ? state.ros1 : state.ros2; }
        ::std::shared_ptr<instance_state> find_instance(void const* object, ::std::uint64_t generation)
        {
            ::std::lock_guard guard{registry_mutex};
            auto it{instances.find({object, generation})};
            return it == instances.end() ? nullptr : it->second;
        }
        static void initialize_bucket(alias_state& state)
        {
            parse_positive_double(state.cap.limits.at("max-rate-hz"), state.rate);
            ::std::uint64_t burst{}, bandwidth{};
            parse_u64(state.cap.limits.at("burst"), burst); parse_u64(state.cap.limits.at("max-bytes-per-second"), bandwidth);
            if(state.cap.operation == "log.write") { ::std::uint64_t log_bytes{}; parse_u64(state.cap.limits.at("max-log-bytes-per-second"), log_bytes); bandwidth = ::std::min(bandwidth, log_bytes); }
            state.burst = static_cast<double>(burst); state.bandwidth = static_cast<double>(bandwidth);
            state.tokens = state.creation_tokens = state.burst; state.byte_tokens = state.bandwidth;
            state.audit.configure(state.cap);
        }
        static void refresh_quota(alias_state& state)
        {
            auto now{::std::chrono::steady_clock::now()};
            auto seconds{::std::clamp(::std::chrono::duration<double>(now - state.updated).count(), 0., 86400.)};
            state.tokens = ::std::min(state.burst, state.tokens + seconds * state.rate);
            state.creation_tokens = ::std::min(state.burst, state.creation_tokens + seconds * state.rate);
            state.byte_tokens = ::std::min(state.bandwidth, state.byte_tokens + seconds * state.bandwidth); state.updated = now;
        }
        static bool available(alias_state& state, ::std::size_t bytes, bool operation)
        {refresh_quota(state);return (!operation||state.tokens>=1.)&&static_cast<double>(bytes)<=state.byte_tokens;}
        static result consume(alias_state& state, ::std::size_t bytes, bool creation = false, bool operation = true)
        {
            refresh_quota(state);
            double& tokens{creation ? state.creation_tokens : state.tokens};
            if((operation && tokens < 1.) || static_cast<double>(bytes) > state.byte_tokens) { return {errc::rate_limited, "explicit per-instance capability quota exhausted"}; }
            if(operation) { tokens -= 1.; } state.byte_tokens -= static_cast<double>(bytes);
            return {};
        }
        static result lookup(backend_state& state, ::std::uint64_t handle, handle_entry*& entry, alias_state*& cap)
        {
            if(!state.alive) { return {errc::permission_denied, "runtime instance was torn down"}; }
            auto slot{static_cast<::std::uint32_t>(handle)}; auto generation{static_cast<::std::uint32_t>(handle >> 32)};
            auto found{state.handles.find(slot)};
            if(!slot || !generation || found == state.handles.end() || !found->second.active || found->second.retired || found->second.generation != generation)
            { return {errc::bad_handle, "forged, stale, cross-instance or cross-version handle"}; }
            entry = &found->second;
            auto alias{state.aliases.find(entry->alias)};
            if(alias == state.aliases.end() || alias->second.revoked || !state.policy.enabled) { return {errc::revoked, "capability revoked or target disabled"}; }
            cap = &alias->second;
            return {};
        }
        static void audit_decision(backend_state& state, alias_state& cap, ::std::string_view event, errc error)
        {
            if(cap.cap.limits.at("audit") == "deny-only" && error == errc::success && (event == "allow" || event == "gateway-result")) { return; }
            auto timestamp{::std::chrono::duration_cast<::std::chrono::nanoseconds>(::std::chrono::system_clock::now().time_since_epoch()).count()};
            cap.audit.record(::fast_io::concat_std("{\"time_unix_ns\":",::fast_io::mnp::dec(timestamp),
                ",\"ros_version\":",json_quoted{to_string(state.policy.ros_version)},",\"module\":",json_quoted{state.module},
                ",\"instance\":",::fast_io::mnp::dec(state.instance_id),",\"event\":",json_quoted{event},
                ",\"operation\":",json_quoted{cap.cap.operation},",\"resource\":",json_quoted{cap.cap.resource},
                ",\"result\":",json_quoted{error == errc::success ? "allow" : "deny"},
                ",\"error\":",::fast_io::mnp::dec(static_cast<unsigned>(error)),"}"));
        }
        static void invalidate(handle_entry& entry) noexcept
        {
            entry.active = false;
            if(entry.generation == ::std::numeric_limits<::std::uint32_t>::max()) { entry.retired = true; }
            else { ++entry.generation; }
        }
        static void retire(instance_state& state) noexcept
        {
            ::std::scoped_lock guard{state.ros1.mutex, state.ros2.mutex};
            // A management tombstone can be retired again by normal shutdown.
            // Do not consume another handle generation on repeated retirement.
            if(!state.ros1.alive&&!state.ros2.alive) { return; }
            state.ros1.alive = state.ros2.alive = false;
            for(auto& item : state.ros1.handles) { invalidate(item.second); }
            for(auto& item : state.ros2.handles) { invalidate(item.second); }
        }
        static void set_lease(authorization& out, ::std::shared_ptr<instance_state> const& lifetime, backend_state& state,
                              alias_state& cap, ::std::unique_lock<::std::recursive_mutex> lock)
        {
            out.lifetime = lifetime; out.cap = &cap.cap; out.bound_target = &state.policy; out.identity=state.instance_id; out.lock = ::std::move(lock);
        }
    public:
        capability_manager() = default;
        capability_manager(capability_manager const&) = delete;
        capability_manager& operator=(capability_manager const&) = delete;

        /** Copy bounded diagnostic metadata without opening, authorizing,
         * refreshing or consuming a capability. Every mutex is try-locked:
         * a stopped call holding a backend lease cannot deadlock its debugger.
         * Registry keys, raw handles, target connections and audit bodies are
         * deliberately absent from the detached public row. */
        void debug_snapshot(debug::request const& query,debug::view& output) noexcept
        {
            output={};output.selected=query.selected;
            if(!debug::valid(query)) {output.result=debug::status::invalid_request;return;}
            std::unique_lock registry{registry_mutex,std::try_to_lock};
            if(!registry.owns_lock()) {output.result=debug::status::busy;return;}
            std::size_t scanned{};
            for(auto const& entry:instances)
            {
                if(++scanned>debug::maximum_scan) {output={};output.selected=query.selected;output.result=debug::status::resource_limit;return;}
                auto const& instance{*entry.second};
                for(auto v:{version::ros1,version::ros2})
                {
                    if(query.backend&&query.backend!=static_cast<unsigned>(v)) {continue;}
                    auto& bound{backend(*entry.second,v)};
                    std::unique_lock lease{bound.mutex,std::try_to_lock};
                    if(!lease.owns_lock()) {output={};output.selected=query.selected;output.result=debug::status::busy;return;}
                    if(query.instance&&query.instance!=bound.instance_id) {continue;}
                    for(auto const& configured:bound.aliases)
                    {
                        if(++scanned>debug::maximum_scan) {output={};output.selected=query.selected;output.result=debug::status::resource_limit;return;}
                        auto const& state{configured.second};auto const& cap{state.cap};
                        if(!debug::matches(query,static_cast<unsigned>(v),bound.instance_id,cap.alias,cap.operation,instance.module)) {continue;}
                        debug::row value{};value.kind=debug::row_kind::capability;value.backend=static_cast<unsigned>(v);
                        value.instance=bound.instance_id;value.generation=entry.first.second;value.module.assign(instance.module);value.alias.assign(cap.alias);
                        value.operation.assign(cap.operation);value.type.assign(cap.type);
                        if(debug::public_resource(cap.resource)) {value.resource.assign(cap.resource);}else {value.resource_redacted=true;}
                        value.enabled=bound.policy.enabled;value.alive=bound.alive;value.retired=!bound.alive;value.revoked=state.revoked;value.open_count=state.open_count;
                        value.operation_tokens=state.tokens;value.creation_tokens=state.creation_tokens;value.byte_tokens=state.byte_tokens;
                        value.audit_records=state.audit.retained_records();value.audit_bytes=state.audit.retained_bytes();value.audit_suppressed=state.audit.dropped_records();
                        for(auto const& handle:bound.handles)
                        {
                            if(++scanned>debug::maximum_scan) {output={};output.selected=query.selected;output.result=debug::status::resource_limit;return;}
                            if(handle.second.active&&!handle.second.retired&&handle.second.alias==cap.alias) {++value.active_handles;}
                        }
                        debug::add(query,output,value);
                    }
                }
            }
            output.result=debug::status::ok;
        }
        // Host-only label lookup for the trace/session bridge. The trusted
        // caller supplies its existing internal identity; it is never copied
        // into the request or reply and creates no authorization lease.
        bool debug_identity(void const* object,std::uint64_t generation,version v,
            std::string_view alias,debug::trace_record& output) noexcept
        {
            std::unique_lock registry{registry_mutex,std::try_to_lock};if(!registry.owns_lock()) {return false;}
            auto found{instances.find({object,generation})};if(found==instances.end()) {return false;}
            auto& bound{backend(*found->second,v)};
            std::unique_lock lease{bound.mutex,std::try_to_lock};if(!lease.owns_lock()) {return false;}
            output.instance=bound.instance_id;output.generation=generation;output.module.assign(found->second->module);
            if(!alias.empty())
            {
                auto selected{bound.aliases.find(alias)};if(selected==bound.aliases.end()) {return false;}
                output.operation.assign(selected->second.cap.operation);
            }
            return true;
        }

        result register_instance(void const* object, ::std::uint64_t generation, ::std::string_view module,
                                 target_policy const* ros1, target_policy const* ros2)
        {
            if(!object || !valid_module_name(module)) { return {errc::invalid_argument, "runtime identity and validated module name required"}; }
            auto state{::std::make_shared<instance_state>()}; state->module = module;
            state->ros1.module = state->ros2.module = module;
            state->ros2.policy.ros_version = version::ros2;
            if(ros1) { if(ros1->ros_version != version::ros1) { return {errc::invalid_argument, "ROS1 target version mismatch"}; } state->ros1.policy = *ros1; }
            if(ros2) { if(ros2->ros_version != version::ros2) { return {errc::invalid_argument, "ROS2 target version mismatch"}; } state->ros2.policy = *ros2; }
            for(auto v : {version::ros1, version::ros2})
            {
                auto& bound{backend(*state, v)};
                if(auto r{validate_target(bound.policy)}; !r) { return r; }
                if(!bound.policy.enabled) { continue; }
                for(auto const& [alias, cap] : bound.policy.capabilities)
                { alias_state item{}; item.cap = cap; initialize_bucket(item); bound.aliases.emplace(alias, ::std::move(item)); }
            }
            ::std::lock_guard guard{registry_mutex};
            state->ros1.instance_id = state->ros2.instance_id = next_instance_id++;
            if(!instances.emplace(identity{object, generation}, state).second) { return {errc::conflict, "runtime instance already registered"}; }
            return {};
        }
        /** Permanently retire one existing registration without erasing it.
         *
         * Only trusted native lifecycle code uses this entry. Keeping the
         * original registration prevents an update from recreating aliases,
         * quotas or handle slots for the same (object, Runtime epoch). Original
         * revocation flags, counters and audit storage remain diagnostic facts.
         * No registration or allocation is made for an absent module. Both
         * backend leases drain before this function returns; old handles and
         * fresh open attempts subsequently fail closed through alive=false.
         */
        bool retire_registered(void const* object, ::std::uint64_t generation) noexcept
        {
            if(!object||!generation) { return false; }
            auto state{find_instance(object,generation)};
            if(!state) { return false; }
            retire(*state);
            return true;
        }
        result unregister_instance(void const* object, ::std::uint64_t generation)
        {
            ::std::shared_ptr<instance_state> state{};
            {
                ::std::lock_guard guard{registry_mutex}; auto it{instances.find({object, generation})};
                if(it == instances.end()) { return {errc::unknown_module, "runtime instance not registered"}; }
                state = it->second; instances.erase(it);
            }
            retire(*state);
            return {};
        }
        void clear()
        {
            decltype(instances) retired{};
            { ::std::lock_guard guard{registry_mutex}; retired.swap(instances); }
            for(auto const& item : retired) { retire(*item.second); }
        }
        result open(void const* object, ::std::uint64_t generation, version v, ::std::string_view alias, ::std::uint64_t& handle)
        {
            handle = 0;
            if(!object || !valid_version(v)) { return {errc::permission_denied, "invalid trusted instance or version"}; }
            auto instance{find_instance(object, generation)};
            if(!instance) { return {errc::permission_denied, "unregistered trusted instance"}; }
            auto& bound{backend(*instance, v)}; ::std::lock_guard guard{bound.mutex};
            if(!bound.alive || !bound.policy.enabled) { return {errc::permission_denied, "ROS target disabled or instance torn down"}; }
            auto found{bound.aliases.find(alias)};
            if(found == bound.aliases.end()) { return {errc::permission_denied, "alias absent from effective target"}; }
            auto& cap{found->second};
            if(cap.revoked) { audit_decision(bound, cap, "deny", errc::revoked); return {errc::revoked, "revoked capability cannot be reopened"}; }
            ::std::uint64_t maximum{};
            auto count_field{cap.cap.operation == "topic.publish" ? "max-publishers" : (cap.cap.operation == "topic.subscribe" || cap.cap.operation == "param.subscribe-events") ? "max-subscribers" : cap.cap.operation == "service.advertise" ? "max-services" : cap.cap.operation.starts_with("action.") ? "max-actions" : "max-inflight"};
            if(!parse_u64(cap.cap.limits.at(count_field), maximum) || cap.open_count >= maximum) { audit_decision(bound, cap, "deny", errc::rate_limited); return {errc::rate_limited, "explicit resource count exhausted"}; }
            if(auto r{consume(cap, 0, true)}; !r) { audit_decision(bound, cap, "deny", r.code); return r; }
            ::std::uint32_t slot{}; handle_entry* entry{};
            for(auto& [candidate, value] : bound.handles) { if(!value.active && !value.retired) { slot = candidate; entry = &value; break; } }
            if(!entry)
            {
                { ::std::lock_guard slot_guard{registry_mutex}; if(next_slot > ::std::numeric_limits<::std::uint32_t>::max()) { return {errc::too_large, "handle namespace exhausted"}; } slot = static_cast<::std::uint32_t>(next_slot++); }
                entry = &bound.handles.emplace(slot, handle_entry{}).first->second;
            }
            entry->alias = alias; entry->active = true; ++cap.open_count; handle = (::std::uint64_t{entry->generation} << 32) | slot;
            audit_decision(bound, cap, "resource-create", errc::success);
            return {};
        }
        result describe(void const* object, ::std::uint64_t generation, version v, ::std::uint64_t handle, authorization& out)
        {
            out.release();
            if(!object || !valid_version(v)) { return {errc::permission_denied, "invalid trusted instance or version"}; }
            auto instance{find_instance(object, generation)};
            if(!instance) { return {errc::permission_denied, "unregistered trusted instance"}; }
            auto& bound{backend(*instance, v)}; ::std::unique_lock guard{bound.mutex};
            handle_entry* entry{}; alias_state* cap{};
            if(auto r{lookup(bound, handle, entry, cap)}; !r) { return r; }
            set_lease(out, instance, bound, *cap, ::std::move(guard));
            return {};
        }
        result authorize(void const* object, ::std::uint64_t generation, version v, ::std::uint64_t handle,
                         ::std::string_view operation, ::std::span<::std::byte const> payload, authorization& out,
                         bool validate_payload = true, bool consume_quota = true)
        {
            out.release();
            if(!object || !valid_version(v)) { return {errc::permission_denied, "invalid trusted instance or version"}; }
            auto instance{find_instance(object, generation)};
            if(!instance) { return {errc::permission_denied, "unregistered trusted instance"}; }
            auto& bound{backend(*instance, v)}; ::std::unique_lock guard{bound.mutex};
            handle_entry* entry{}; alias_state* cap{};
            if(auto r{lookup(bound, handle, entry, cap)}; !r) { return r; }
            if(cap->cap.operation != operation) { audit_decision(bound, *cap, "deny", errc::permission_denied); return {errc::permission_denied, "operation differs from explicit capability"}; }
            if(validate_payload) { if(auto r{validate_message(v, cap->cap, payload)}; !r) { audit_decision(bound, *cap, "deny", r.code); return r; } }
            if(consume_quota) { if(auto r{consume(*cap, payload.size())}; !r) { audit_decision(bound, *cap, "deny", r.code); return r; } audit_decision(bound, *cap, "allow", errc::success); }
            set_lease(out, instance, bound, *cap, ::std::move(guard));
            return {};
        }
        /** Admit a trusted ROS2 RMW service ticket envelope.
         *
         * This host-only entry keeps the actual capability lease and validates
         * the complete existing ticket/body format. The fixed positive ticket
         * is transport metadata, so only canonical body bytes debit bandwidth.
         * It never authorizes a client, observer, availability query or control
         * packet. Original authorize() and its legacy byte accounting stay
         * unchanged. Call this before a native response or borrowed delivery;
         * an unsuccessful admission must not acknowledge that delivery.
         */
        result authorize_rmw_service_envelope(void const* object, ::std::uint64_t generation, version v,
            ::std::uint64_t handle, ::std::string_view operation, ::std::span<::std::byte const> envelope,
            authorization& out, bool consume_operation = true)
        {
            out.release();
            if(!object || v != version::ros2) { return {errc::permission_denied, "ROS2 trusted service instance required"}; }
            auto instance{find_instance(object, generation)};
            if(!instance) { return {errc::permission_denied, "unregistered trusted instance"}; }
            auto& bound{backend(*instance, v)}; ::std::unique_lock guard{bound.mutex};
            handle_entry* entry{}; alias_state* cap{};
            if(auto r{lookup(bound, handle, entry, cap)}; !r) { return r; }
            if((operation != "service.advertise" && operation != "service.respond") || cap->cap.operation != operation)
            { audit_decision(bound, *cap, "deny", errc::permission_denied); return {errc::permission_denied, "explicit server service capability required"}; }
            // Unlike legacy advertise activation, this entry always represents
            // one real ticket/body envelope, including an empty request body.
            if(envelope.size() < 8)
            { audit_decision(bound, *cap, "deny", errc::invalid_argument); return {errc::invalid_argument, "RMW service ticket envelope truncated"}; }
            if(auto r{validate_message(v, cap->cap, envelope)}; !r)
            { audit_decision(bound, *cap, "deny", r.code); return r; }
            if(auto r{consume(*cap, envelope.size() - 8, false, consume_operation)}; !r)
            { audit_decision(bound, *cap, "deny", r.code); return r; }
            audit_decision(bound, *cap, "allow", errc::success);
            set_lease(out, instance, bound, *cap, ::std::move(guard));
            return {};
        }
        result authorize_response(void const* object, ::std::uint64_t generation, version v, ::std::uint64_t handle,
                                  ::std::span<::std::byte const> payload, authorization& out)
        {
            out.release();
            if(!object || !valid_version(v)) { return {errc::permission_denied, "invalid trusted instance or version"}; }
            auto instance{find_instance(object, generation)}; if(!instance) { return {errc::permission_denied, "unregistered trusted instance"}; }
            auto& bound{backend(*instance, v)}; ::std::unique_lock guard{bound.mutex};
            handle_entry* entry{}; alias_state* cap{};
            if(auto r{lookup(bound, handle, entry, cap)}; !r) { return r; }
            if(auto r{validate_response_payload(v, cap->cap, payload)}; !r) { audit_decision(bound, *cap, "deny", r.code); return r; }
            if(auto r{consume(*cap, payload.size(), false, false)}; !r) { audit_decision(bound, *cap, "deny", r.code); return r; }
            set_lease(out, instance, bound, *cap, ::std::move(guard)); return {};
        }
        // Source and observer byte/rate admission is atomic. A refused preview
        // changes no debit, so a retained Native peek can be retried without
        // losing a completion or charging one side twice. Both leases survive
        // until controlled Guest copy and exact Native commit have completed.
        result authorize_event_delivery(void const* object, ::std::uint64_t generation, version v,
            ::std::uint64_t observer_handle, ::std::uint64_t source_handle, ipc::event_header const& header,
            ::std::span<::std::byte const> body, authorization& observer, authorization& source)
        {
            observer.release();source.release();if(!object||!valid_version(v)) {return {errc::permission_denied,"invalid event Instance"};}
            auto instance{find_instance(object,generation)};if(!instance) {return {errc::permission_denied,"unregistered event Instance"};}
            auto& bound{backend(*instance,v)};::std::unique_lock guard{bound.mutex};handle_entry* entry{};alias_state* observing{};
            if(auto r{lookup(bound,observer_handle,entry,observing)};!r) {return r;}
            if(!ipc::valid_event_header(header)||observing->cap.operation!="event.observe"||header.instance!=bound.instance_id||header.backend!=static_cast<unsigned>(v)||header.source_handle!=source_handle||header.payload_bytes!=body.size()) {return {errc::permission_denied,"event binding differs from explicit observer"};}
            ::std::uint32_t mask{};::std::uint64_t maximum{};
            if(!ipc::parse_event_kinds(observing->cap.limits.at("event-kinds"),mask)||!(mask&ipc::event_bit(header.kind))||!parse_u64(observing->cap.limits.at("max-event-bytes"),maximum)||body.size()>maximum-ipc::event_header_bytes) {return {errc::permission_denied,"event exceeds explicit observer kind/bytes"};}
            alias_state* producing{};bool operation{};
            if(!ipc::event_control(header.kind))
            {
                if(auto r{lookup(bound,source_handle,entry,producing)};!r) {return r;}
                if(observing->cap.limits.at("event-source-alias")!=producing->cap.alias||!ipc::event_kind_for_source(producing->cap.operation,header.kind,header.flags)) {return {errc::permission_denied,"event source operation mismatch"};}
                bool response{header.kind==ipc::event_kind::service_response||header.kind==ipc::event_kind::action_feedback||header.kind==ipc::event_kind::action_result};
                // service_submit already debited its request operation. Action
                // reads are independent receive operations and still consume
                // their explicitly granted per-delivery rate plus body bytes.
                operation=header.kind!=ipc::event_kind::service_response;
                auto r{response?validate_response_payload(v,producing->cap,body):validate_message(v,producing->cap,body)};if(!r) {return r;}
                if(!available(*producing,body.size(),operation)) {return {errc::rate_limited,"source delivery credit exhausted"};}
            }
            auto wire{body.size()+ipc::event_header_bytes};if(!available(*observing,wire,false)) {return {errc::rate_limited,"observer delivery credit exhausted"};}
            if(producing) {(void)consume(*producing,body.size(),false,operation);set_lease(source,instance,bound,*producing,::std::unique_lock<::std::recursive_mutex>{bound.mutex});}
            (void)consume(*observing,wire,false,false);set_lease(observer,instance,bound,*observing,::std::move(guard));return {};
        }
        // Dedicated trusted-envelope byte charge; it cannot grant native source
        // authority or reinterpret the envelope as a topic/service payload.
        result charge_observation(void const* object, ::std::uint64_t generation, version v, ::std::uint64_t handle,
                                  ::std::size_t bytes, authorization& out)
        {
            out.release();if(!object||!valid_version(v)) { return {errc::permission_denied,"invalid observer identity"}; }
            auto instance{find_instance(object,generation)};if(!instance) { return {errc::permission_denied,"unregistered observer Instance"}; }
            auto& bound{backend(*instance,v)};::std::unique_lock guard{bound.mutex};handle_entry* entry{};alias_state* cap{};
            if(auto r{lookup(bound,handle,entry,cap)};!r) { return r; }
            if(cap->cap.operation!="event.observe") { return {errc::permission_denied,"observer operation required"}; }
            if(auto r{consume(*cap,bytes,false,false)};!r) { return r; }
            set_lease(out,instance,bound,*cap,::std::move(guard));return {};
        }
        result close(void const* object, ::std::uint64_t generation, version v, ::std::uint64_t handle,
                     bool* last_open_handle = nullptr)
        {
            if(last_open_handle) { *last_open_handle = false; }
            if(!object || !valid_version(v)) { return {errc::permission_denied, "invalid trusted instance or version"}; }
            auto instance{find_instance(object, generation)};
            if(!instance) { return {errc::permission_denied, "unregistered trusted instance"}; }
            auto& bound{backend(*instance, v)}; ::std::lock_guard guard{bound.mutex};
            handle_entry* entry{}; alias_state* cap{};
            if(auto r{lookup(bound, handle, entry, cap)}; !r) { return r; }
            invalidate(*entry); if(cap->open_count) { --cap->open_count; }
            if(last_open_handle) { *last_open_handle = cap->open_count == 0; }
            audit_decision(bound, *cap, "resource-close", errc::success);
            return {};
        }
        result revoke(void const* object, ::std::uint64_t generation, version v, ::std::string_view alias)
        {
            if(!object || !valid_version(v)) { return {errc::permission_denied, "invalid trusted instance or version"}; }
            auto instance{find_instance(object, generation)};
            if(!instance) { return {errc::permission_denied, "unregistered trusted instance"}; }
            auto& bound{backend(*instance, v)}; ::std::lock_guard guard{bound.mutex};
            if(!bound.alive) { return {errc::permission_denied, "runtime instance torn down"}; }
            auto cap{bound.aliases.find(alias)};
            if(cap == bound.aliases.end()) { return {errc::permission_denied, "unknown effective capability"}; }
            cap->second.revoked = true; cap->second.open_count = 0; audit_decision(bound, cap->second, "capability-revoke", errc::success);
            for(auto& [slot, entry] : bound.handles) { if(entry.active && entry.alias == alias) { invalidate(entry); } }
            return {};
        }
        result record_result(void const* object, ::std::uint64_t generation, version v, ::std::string_view alias,
                             ::std::string_view event, errc error)
        {
            if(!object || !valid_version(v)) { return {errc::permission_denied, "invalid trusted identity"}; }
            auto instance{find_instance(object, generation)}; if(!instance) { return {errc::permission_denied, "unregistered instance"}; }
            auto& bound{backend(*instance, v)}; ::std::lock_guard guard{bound.mutex};
            auto cap{bound.aliases.find(alias)}; if(!bound.alive || cap == bound.aliases.end()) { return {errc::permission_denied, "unknown configured capability"}; }
            if(event != "gateway-result" && event != "gateway-error" && event != "resource-create" && event != "resource-close") { return {errc::invalid_argument, "unknown audit event"}; }
            audit_decision(bound, cap->second, event, error); return {};
        }
        result drain_audit(void const* object, ::std::uint64_t generation, version v, ::std::string_view alias,
                           ::std::vector<::std::string>& records, ::std::uint64_t& dropped)
        {
            if(!object || !valid_version(v)) { return {errc::permission_denied, "invalid trusted identity"}; }
            auto instance{find_instance(object, generation)}; if(!instance) { return {errc::permission_denied, "unregistered instance"}; }
            auto& bound{backend(*instance, v)}; ::std::lock_guard guard{bound.mutex};
            auto cap{bound.aliases.find(alias)}; if(!bound.alive || cap == bound.aliases.end()) { return {errc::permission_denied, "unknown configured capability"}; }
            cap->second.audit.drain(records, dropped); return {};
        }
    };
}
