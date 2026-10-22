/*************************************************************
 * UWVM2 ROS Runtime Extension. APL-2.0.                     *
 *************************************************************/
#pragma once
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
#ifndef UWVM_MODULE
#include "storage.h"
#endif
#ifndef UWVM_MODULE
#include <uwvm2/uwvm/io/impl.h>
#endif
#ifndef UWVM_MODULE
#include <uwvm2/uwvm/utils/ansies/impl.h>
#endif
#ifndef UWVM_MODULE
#include <uwvm2/runtime/lib/uwvm_runtime.h>
#endif
#ifndef UWVM_MODULE
#include <uwvm2/uwvm/wasm/type/impl.h>
#endif
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/capability/manager.h>
#endif
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/ipc/unix.h>
#endif
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/ipc/event_queue.h>
#endif
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/common/manifest.h>
#endif
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/common/audit_io.h>
#endif
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/common/management.h>
#endif
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/common/service_availability_history.h>
#endif
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/common/rmw_endpoint_history.h>
#endif
#ifndef UWVM_MODULE
#include <uwvm2/uwvm/runtime/storage/builtin_ros_loader.h>
#endif
#include "rmw_service_state.h"
#include <array>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <limits>
#include <span>
#include <string>
#include <vector>
#include <tuple>
#include <type_traits>
#include <utility>


namespace uwvm2::uwvm::imported::ros
{
    namespace core = ::uwvm2::imported::ros;
    namespace runtime = ::uwvm2::runtime::lib;

    namespace host
    {
        struct session;
        inline ::std::uint32_t retire_rmw_endpoints(session&,::std::uint64_t,::std::string_view,bool,::std::uint64_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t rmw_publisher_activate(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o) noexcept;
        inline ::std::uint32_t rmw_publisher_inspect(::std::uint64_t h,::std::uint64_t i,::std::uint32_t m,::std::uint64_t o) noexcept;
        inline ::std::uint32_t rmw_publisher_publish(::std::uint64_t h,::std::uint64_t i,::std::uint32_t m,::std::uint64_t o,::std::uint64_t n) noexcept;
        inline ::std::uint32_t rmw_publisher_retire(::std::uint64_t h,::std::uint64_t i) noexcept;
        inline ::std::uint32_t rmw_subscription_activate(::std::uint64_t,::std::uint32_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t rmw_subscription_inspect(::std::uint64_t,::std::uint64_t,::std::uint32_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t rmw_subscription_peek(::std::uint64_t,::std::uint64_t,::std::uint32_t,::std::uint32_t,::std::uint64_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t rmw_subscription_commit(::std::uint64_t,::std::uint64_t,::std::uint64_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t rmw_subscription_cancel(::std::uint64_t,::std::uint64_t,::std::uint64_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t rmw_subscription_retire(::std::uint64_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t rmw_client_activate(::std::uint64_t,::std::uint64_t,::std::uint32_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t rmw_service_activate(::std::uint64_t,::std::uint64_t,::std::uint32_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t rmw_service_inspect(::std::uint64_t,::std::uint64_t,::std::uint32_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t rmw_client_submit(::std::uint64_t,::std::uint64_t,::std::uint32_t,::std::uint64_t,::std::uint64_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t rmw_client_abandon(::std::uint64_t,::std::uint64_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t rmw_service_respond(::std::uint64_t,::std::uint64_t,::std::uint64_t,::std::uint64_t,::std::uint32_t,::std::uint64_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t rmw_service_peek(::std::uint64_t,::std::uint64_t,::std::uint32_t,::std::uint32_t,::std::uint64_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t rmw_service_commit(::std::uint64_t,::std::uint64_t,::std::uint64_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t rmw_service_cancel(::std::uint64_t,::std::uint64_t,::std::uint64_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t rmw_client_available(::std::uint64_t,::std::uint64_t,::std::uint64_t,::std::uint64_t,::std::uint32_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t rmw_service_retire(::std::uint64_t,::std::uint64_t) noexcept;
        inline ::std::uint32_t code(core::result const& r) noexcept { return static_cast<::std::uint32_t>(r.code); }
        inline ::std::uint32_t audit_denial(core::version version, ::std::uint32_t error);
        struct audit_result
        {
            void const* object{};
            ::std::uint64_t generation{};
            core::version version{};
            ::std::string alias{};
            core::debug::call_result result{};
            ::std::string_view function{};
            ::std::uint32_t finish(::std::uint32_t value) noexcept { return result.finish(value); }
            core::debug::text<255> source_alias{};::std::uint64_t client_session_label{},client_features{};
            bool client_features_observed{};
            ::std::uint64_t debug_started_ns{core::debug::monotonic_nanoseconds()};
            ~audit_result();
        };
        inline bool memory_range(::std::uint32_t memory, ::std::uint64_t offset, ::std::uint64_t length) noexcept
        {
            auto const count{runtime::preload_memory_descriptor_count_host_api()};
            for(::std::size_t i{}; i != count; ++i)
            {
                ::uwvm2::uwvm::wasm::type::uwvm_preload_memory_descriptor_t descriptor{};
                if(runtime::preload_memory_descriptor_at_host_api(i,&descriptor) && descriptor.memory_index == memory)
                { return offset <= descriptor.byte_length && length <= descriptor.byte_length-offset && length <= SIZE_MAX; }
            }
            return false;
        }
        inline bool write_u64(::std::uint32_t memory, ::std::uint64_t offset, ::std::uint64_t value) noexcept
        {
            ::std::array<::std::uint8_t,8> encoded{};
            for(unsigned i{}; i != 8; ++i) { encoded[i] = static_cast<::std::uint8_t>(value >> (8*i)); }
            return runtime::preload_memory_write_host_api(memory,offset,encoded.data(),encoded.size());
        }
        inline bool read_bytes(::std::uint32_t memory, ::std::uint64_t offset, ::std::uint64_t length, ::std::vector<::std::uint8_t>& bytes)
        {
            if(length > core::ipc::max_payload_bytes || !memory_range(memory,offset,length)) { return false; }
            bytes.resize(static_cast<::std::size_t>(length));
            return runtime::preload_memory_read_host_api(memory,offset,bytes.data(),bytes.size());
        }

        struct watched_event
        {
            core::capability_spec observer,source;
            ::std::uint64_t source_handle{},correlation{},instance{},native_session{},native_sequence{},delivered{};
            bool retired{};
            ::std::shared_ptr<core::ipc::event_stream> local;
            ::std::set<::std::uint64_t> requests;
        };
        struct session
        {
            ::std::mutex mutex{};
            core::target_policy const* target{};
            core::ipc::client connection{};
            bool connected{};
            ::std::uint32_t timeout{};
            ::std::map<::std::uint64_t,watched_event> observations;
            ::std::uint64_t next_service_request{1};
            core::service_availability_history availability_history;
            core::rmw_endpoint_history rmw_endpoints;
            // Independent immutable two-party association, query authority and
            // per-source diagnostics. Legacy availability has a different
            // native registry and MUST NOT share this same-client query table.
            core::rmw_service_bindings rmw_service_pairs{};
            core::service_availability_history rmw_service_queries;
            ::std::map<::std::uint64_t,rmw_service_journal> rmw_service_journals;
            void retire_service_session(::std::uint64_t old_session,::std::uint32_t reason) noexcept
            {
                for(auto const& item:rmw_service_pairs.rows())
                {if(item.second.owner.session==old_session){(void)rmw_service_pairs.retire(item.second.owner,item.second.kind,reason);}}
                rmw_service_queries.retire_session(old_session);
            }

            ::std::uint64_t availability_native_session{};
            // Diagnostic label is a local successful-connect ordinal, NEVER
            // the random IPC credential. Exhaustion drops the label, not ROS.
            ::std::uint64_t availability_debug_counter{},availability_debug_session{};
            void retire_observations(::std::uint32_t reason) noexcept
            {
                availability_history.retire_session(availability_native_session);
                retire_service_session(availability_native_session,reason);
                bool owned_native{};
                for(auto const& item:rmw_endpoints.rows())
                {if(item.second.observed.owner.session==availability_native_session&&item.second.state!=core::rmw_endpoint_history::phase::retired){owned_native=true;break;}}
                rmw_endpoints.retire_session(availability_native_session,reason);
                // An ordinary v1 RPC can return a valid native timeout while
                // its socket is still live. Owned RMW retirement must also
                // drop that session, so no hidden publisher outlives its row.
                if(owned_native){connection.close();connected=false;}
                for(auto& item:observations) { item.second.retired=true;item.second.requests.clear();item.second.local->seal(reason,true); }
            }
            // Called with this backend's mutex held by trusted management.
            // Preserve metadata/statistics, but no queued Guest data, pending
            // request identity or live Gateway descriptor survives retirement.
            // This is a local descriptor close, never a capability/event RPC.
            void retire_authority() noexcept
            {
                availability_history.seal();
                rmw_endpoints.seal();
                rmw_service_pairs.seal();rmw_service_queries.seal();
                connection.close();connected=false;
                for(auto& item:observations)
                {
                    item.second.retired=true;item.second.requests.clear();
                    if(item.second.local) { item.second.local->discard(); }
                }
            }

            ::std::uint32_t connect(core::version version,
                ::std::chrono::steady_clock::time_point deadline=::std::chrono::steady_clock::time_point::max())
            {
                if(connected) { return 0; }
                // Reconnect never revives an original full source binding.
                // Close any abandoned descriptor before the first new budget
                // or policy failure could otherwise leave its entity alive.
                bool owned_native{};
                for(auto const& item:rmw_endpoints.rows())
                {if(item.second.observed.owner.session==availability_native_session&&item.second.state!=core::rmw_endpoint_history::phase::retired){owned_native=true;break;}}
                if(owned_native){rmw_endpoints.retire_session(availability_native_session,107);connection.close();}
                retire_service_session(availability_native_session,107);
                if(target == nullptr || !target->enabled) { return 13; }
                ::std::uint64_t uid{}, millis{};
                if(!core::parse_u64(target->connection.at("gateway-uid"),uid) ||
                   !core::parse_u64(target->connection.at("connect-timeout-ms"),millis)) { return 22; }
                timeout=static_cast<::std::uint32_t>(millis);
                // New client queries pass one absolute entry deadline. Ordinary
                // v1 callers keep their existing configured connection budget.
                auto budget=[&]() -> ::std::uint32_t
                {
                    auto now{::std::chrono::steady_clock::now()};if(now>=deadline) { return 0; }
                    auto remaining{::std::chrono::duration_cast<::std::chrono::milliseconds>(deadline-now).count()};
                    return static_cast<::std::uint32_t>(::std::min<::std::uint64_t>(millis,static_cast<::std::uint64_t>(remaining)));
                };
                availability_history.retire_session(availability_native_session);
                auto first{budget()};if(!first) { return 110; }
                auto const error{connection.connect(target->connection.at("gateway-socket"),static_cast<::std::uint8_t>(version),static_cast<::std::uint32_t>(uid),first)};
                if(error) { return error; }
                auto second{budget()};if(!second) { connection.close();return 110; }
                auto bound{connection.call("session.bind","target",core::binding_manifest(*target),second)};
                if(bound.error) { connection.close(); return bound.error; }
                connected=true;availability_native_session=connection.session_id();
                availability_debug_session=availability_debug_counter==UINT64_MAX?0:++availability_debug_counter;
                return 0;
            }
        };
        struct instance
        {
            ::std::uint64_t generation{};
            bool retired{}; // Protected by the Host registry mutex; never reset.
            ::std::array<session,2> backends{};
        };
        inline ::std::mutex mutex{};
        inline core::capability_manager capabilities{};
        inline ::std::map<void const*, ::std::shared_ptr<instance>, ::std::less<>> instances{};
        inline core::debug::trace_store debug_trace{};

        inline void write_audit_record(::std::string_view record) noexcept
        {
            (void)core::audit_io::write_record(record);
        }

        inline audit_result::~audit_result()
        {
            // Diagnostic metadata is independent of the existing audit sink.
            // No payload, address, socket/session credential or audit body is
            // copied; try-lock failure merely marks the label unresolved.
            core::debug::trace_record trace{};trace.backend=static_cast<unsigned>(version);trace.error=result.error;
            trace.result_known=result.known;
            trace.alias.assign(alias);trace.function.assign(function);
            trace.source_alias=source_alias;trace.client_session_label=client_session_label;
            trace.client_features=client_features;trace.client_features_observed=client_features_observed;
            trace.unresolved=!capabilities.debug_identity(object,generation,version,alias,trace);
            auto const finished{core::debug::monotonic_nanoseconds()};trace.started_ns=debug_started_ns;
            trace.duration_known=debug_started_ns&&finished>=debug_started_ns;
            trace.duration_ns=trace.duration_known?finished-debug_started_ns:0;
            debug_trace.append(trace);
            if(result.known)
            {
                auto recorded{capabilities.record_result(object,generation,version,alias,result.error ? "gateway-error" : "gateway-result",static_cast<core::errc>(result.error))};
                if(!recorded && result.error) { audit_denial(version,result.error); }
            }
            ::std::vector<::std::string> records{}; ::std::uint64_t dropped{};
            if(capabilities.drain_audit(object,generation,version,alias,records,dropped))
            { for(auto const& record : records) { write_audit_record(record); } }
        }


        struct denial_sink
        {
            ::std::mutex mutex{};
            core::bounded_audit audit{};
            ::std::uint64_t instance_id{};
        };
        inline ::std::mutex denial_mutex{};
        using denial_identity = ::std::tuple<void const*,::std::uint64_t,core::version>;
        struct denial_less
        {
            bool operator()(denial_identity const& left, denial_identity const& right) const noexcept
            {
                auto less{::std::less<void const*>{}};
                if(less(::std::get<0>(left),::std::get<0>(right))) { return true; }
                if(less(::std::get<0>(right),::std::get<0>(left))) { return false; }
                if(::std::get<1>(left) != ::std::get<1>(right)) { return ::std::get<1>(left) < ::std::get<1>(right); }
                return ::std::get<2>(left) < ::std::get<2>(right);
            }
        };
        inline ::std::map<denial_identity,::std::shared_ptr<denial_sink>,denial_less> denials{};
        inline ::std::uint64_t next_denial_instance{1};
        inline ::std::uint32_t audit_denial(core::version version, ::std::uint32_t error)
        {
            runtime::ros_caller_identity observed{};
            if(!error) {return error;}
            bool const available{runtime::ros_observed_caller_identity_host_api(observed)};
            core::debug::trace_record trace{};trace.backend=static_cast<unsigned>(version);trace.error=error;
            trace.unresolved=true;trace.function.assign("unresolved-denial");
            if(available)
            {
                trace.module.assign({reinterpret_cast<char const*>(observed.module_name.data()),observed.module_name.size()});
                trace.generation=observed.generation;
            }
            debug_trace.append(trace);
            if(!available) {return error;}
            ::std::string_view name{reinterpret_cast<char const*>(observed.module_name.data()),observed.module_name.size()};
            auto const target{runtime_policy.effective(version,name)};
            if(!target || !target->connection.contains("audit-deny-max-rate-hz")) { return error; }
            ::std::shared_ptr<denial_sink> sink{};
            {
                ::std::lock_guard<::std::mutex> registry_lock{denial_mutex};
                auto key{::std::tuple{observed.runtime_instance,observed.generation,version}};
                auto found{denials.find(key)};
                if(found == denials.end())
                {
                    auto item{::std::make_shared<denial_sink>()}; core::capability_spec explicit_budget{};
                    for(auto suffix : {"max-rate-hz","burst","max-records","max-bytes"})
                    {
                        auto field{target->connection.find(::fast_io::concat_std("audit-deny-",::std::string_view{suffix}))};
                        if(field == target->connection.end()) { return error; }
                        explicit_budget.limits.emplace(::fast_io::concat_std("audit-",::std::string_view{suffix}),field->second);
                    }
                    if(!item->audit.configure(explicit_budget) || next_denial_instance == UINT64_MAX) { return error; }
                    item->instance_id=next_denial_instance++;
                    found=denials.emplace(key,::std::move(item)).first;
                }
                sink=found->second;
            }
            ::std::vector<::std::string> records{}; ::std::uint64_t dropped{};
            {
                ::std::lock_guard<::std::mutex> sink_lock{sink->mutex};
                auto wall{::std::chrono::duration_cast<::std::chrono::nanoseconds>(::std::chrono::system_clock::now().time_since_epoch()).count()};
                sink->audit.record(::fast_io::concat_std("{\"wall_ns\":",::fast_io::mnp::dec(wall),
                    ",\"ros\":",core::json_quoted{core::to_string(version)},",\"module\":",core::json_quoted{name},
                    ",\"runtime_generation\":",::fast_io::mnp::dec(observed.generation),
                    ",\"observation_instance\":",::fast_io::mnp::dec(sink->instance_id),
                    ",\"event\":\"unresolved-denial\",\"cross_module\":",::std::string_view{observed.cross_module_ancestry ? "true" : "false"},
                    ",\"error\":",::fast_io::mnp::dec(error),"}"));
                sink->audit.drain(records,dropped);
            }
            for(auto const& record : records) { write_audit_record(record); }
            return error;
        }

        // All enabled targets attest the exact network identity/security and capability manifest
        // before Wasm initialization or start sections can execute. Temporary sessions own no Guest handles.
        inline core::result preflight()
        {
            for(auto target : runtime_policy.configured_targets())
            {
                if(!target->enabled) { continue; }
                session probe{}; probe.target=target;
                auto error{probe.connect(target->ros_version)};
                if(error) { return {static_cast<core::errc>(error),::fast_io::concat_std("gateway preflight failed for ",core::to_string(target->ros_version),"/",core::to_string(target->target_scope),"/",target->name)}; }
            }
            return {};
        }

        inline void trace_retirement(void const* object,instance const& state,core::management::transition reason) noexcept
        {
            // The existing fixed-capacity, try-locked trace owns only detached
            // labels. No payload, handle, object address or launch credential is
            // copied, and a busy diagnostic sink cannot block retirement.
            for(auto version:{core::version::ros1,core::version::ros2})
            {
                core::debug::trace_record trace{};trace.backend=static_cast<unsigned>(version);
                trace.generation=state.generation;trace.error=125;
                trace.function.assign(reason==core::management::transition::function_replacement?
                    "management.function-replacement":"management.runtime-retirement");
                trace.unresolved=!capabilities.debug_identity(object,state.generation,version,{},trace);
                debug_trace.append(trace);
            }
        }

        // Called after Wasm execution/trap drain, before runtime storage can retire. Native ROS callbacks
        // only enqueue in the Gateway; they never reenter the Guest or retain Guest memory pointers.
        inline void shutdown() noexcept
        {
            decltype(instances) retired{};
            {
                ::std::lock_guard<::std::mutex> lock{mutex};
                for(auto const& item:instances) { trace_retirement(item.first,*item.second,core::management::transition::runtime_retirement); }
                retired.swap(instances);
            }
            capabilities.clear();
            { ::std::lock_guard<::std::mutex> denial_lock{denial_mutex}; denials.clear(); }
        }
        /** Trusted post-publication lifecycle hook; never a Wasm import.
         *
         * The Runtime sets its module-local permanent retirement flag while
         * still stopped, then calls this hook OUTSIDE the stopped-domain mutex.
         * Registry -> Host backend -> capability backend is the same order used
         * by normal Host admission. A caller that obtained the instance just
         * before retirement can finish its already admitted operation; waiting
         * requests fail against the now inert capability registration.
         */
        inline void retire_authority(core::management::transition reason,void const* object,::std::uint64_t generation) noexcept
        {
            if(reason==core::management::transition::runtime_retirement) { shutdown();return; }
            if(reason!=core::management::transition::function_replacement||!object||!generation) { return; }
            ::std::lock_guard<::std::mutex> registry{mutex};
            auto found{instances.find(object)};
            // Never instantiate an ROS state for a module that had none. The
            // Runtime's permanent module flag rejects its future first open.
            if(found==instances.end()||found->second->generation!=generation||found->second->retired) { return; }
            auto& state{*found->second};state.retired=true;
            ::std::scoped_lock backends{state.backends[0].mutex,state.backends[1].mutex};
            (void)capabilities.retire_registered(object,generation);
            state.backends[0].retire_authority();state.backends[1].retire_authority();
            trace_retirement(object,state,reason);
        }
        inline ::std::uint32_t identify(runtime::ros_caller_identity& caller, ::std::shared_ptr<instance>& state)
        {
            if(!runtime::ros_caller_identity_host_api(caller)) { return 13; }
            ::std::lock_guard<::std::mutex> registry_lock{mutex};
            auto it{instances.find(caller.runtime_instance)};
            if(it!=instances.end()&&it->second->generation==caller.generation&&it->second->retired) { return 13; }
            if(it != instances.end() && it->second->generation != caller.generation)
            {
                capabilities.unregister_instance(caller.runtime_instance,it->second->generation);
                instances.erase(it); it=instances.end();
            }
            if(it == instances.end())
            {
                auto next{::std::make_shared<instance>()};
                next->generation=caller.generation;
                ::std::string_view name{reinterpret_cast<char const*>(caller.module_name.data()),caller.module_name.size()};
                for(unsigned i{}; i != 2; ++i) { next->backends[i].target=runtime_policy.effective(static_cast<core::version>(i+1),name); }
                auto r{capabilities.register_instance(caller.runtime_instance,caller.generation,name,next->backends[0].target,next->backends[1].target)};
                if(!r) { return code(r); }
                it=instances.emplace(caller.runtime_instance,::std::move(next)).first;
            }
            state=it->second; return 0;
        }

        template <core::version Version>
        inline ::std::uint32_t abi_version() noexcept
        {
            core::debug::trace_record trace{};trace.backend=static_cast<unsigned>(Version);
            trace.function.assign("abi_version");trace.unresolved=true;debug_trace.append(trace);return 1;
        }

        template <core::version Version>
        inline ::std::uint32_t capability_open(::std::uint32_t memory, ::std::uint64_t alias_offset, ::std::uint64_t alias_length, ::std::uint64_t out_offset) noexcept
        {
            runtime::ros_caller_identity caller{}; ::std::shared_ptr<instance> state{};
            if(auto error{identify(caller,state)}) { return audit_denial(Version,error); }
            auto& backend{state->backends[static_cast<unsigned>(Version)-1]};
            ::std::lock_guard<::std::mutex> backend_lock{backend.mutex};
            if(!alias_length || alias_length > 255 || !memory_range(memory,alias_offset,alias_length) || !memory_range(memory,out_offset,8)) { return audit_denial(Version,22); }
            ::std::vector<::std::uint8_t> alias_bytes{};
            if(!read_bytes(memory,alias_offset,alias_length,alias_bytes)) { return audit_denial(Version,22); }
            ::std::string alias{reinterpret_cast<char const*>(alias_bytes.data()),alias_bytes.size()};
            if(!core::valid_identifier(alias)) { return audit_denial(Version,22); }
            audit_result audit{caller.runtime_instance,caller.generation,Version,alias};
            audit.function="capability_open";
            ::std::uint64_t handle{};
            auto r{capabilities.open(caller.runtime_instance,caller.generation,Version,alias,handle)};
            if(!r) { return audit.finish(code(r)); }
            if(!write_u64(memory,out_offset,handle)) { capabilities.close(caller.runtime_instance,caller.generation,Version,handle); return audit.finish(22); }
            return audit.finish(0);
        }

        inline void retire_watches(session& backend,::std::uint64_t handle,::std::string_view alias,bool revoke,::std::uint32_t reason)
        {
            for(auto it=backend.observations.begin();it!=backend.observations.end();)
            {
                auto& observation{it->second};
                if(it->first==handle||(revoke&&observation.observer.alias==alias))
                {
                    observation.local->discard();
                    if(backend.connected) { auto result{backend.connection.call("event.detach",observation.observer.alias,{},backend.timeout)};if(!backend.connection.session_id()) { backend.connected=false;backend.retire_observations(result.error?result.error:107); } }
                    it=backend.observations.erase(it);continue;
                }
                if(!observation.retired&&(observation.source_handle==handle||(revoke&&observation.source.alias==alias)))
                {
                    observation.retired=true;observation.requests.clear();observation.local->seal(reason,true);
                    if(backend.connected)
                    {
                        ::std::vector<::std::uint8_t> data;core::ipc::put(data,reason,4);
                        auto response{backend.connection.call("event.retire",observation.observer.alias,::std::move(data),backend.timeout)};
                        if(response.error==107||response.error==110) { backend.connected=false;backend.retire_observations(response.error); }
                    }
                }
                ++it;
            }
        }
        // This is a PRIVATE Host retirement path, never a Guest import. Core
        // close/revoke has already retired its true handle/alias before entry.
        // Select historical rows even when every query handle closed first;
        // only a row belonging to the current native session is sent remotely.
        inline ::std::uint32_t retire_availability(session& backend,::std::uint64_t handle,
            core::capability_spec const& cap,bool revoke) noexcept
        {
            namespace wire=core::ipc::client_availability;
            if(cap.operation!="service.available"&&cap.operation!="service.call") { return 0; }
            auto scope{cap.operation=="service.available"?(revoke?wire::retirement::availability_alias:wire::retirement::availability_handle):
                (revoke?wire::retirement::source_alias:wire::retirement::source_handle)};
            auto local{backend.availability_history.retirement_reference(handle,cap.alias,scope)};
            if(!local) { return 0; }
            auto remote{backend.availability_history.retirement_reference(handle,cap.alias,scope,backend.availability_native_session)};
            auto binding{local->binding};
            (void)backend.availability_history.retire(binding,scope); // NO allocation or quota.
            if(!backend.connected)
            {
                // Existing v1 error handling can mark connected=false while an
                // error reply left the native fd open. Retiring this source may
                // not leave its new retained client waiting for a future call.
                backend.connection.close();backend.retire_observations(107);return 0;
            }
            if(!remote) { return 0; }
#if defined(__cpp_exceptions)
            try
#endif
            {
                wire::packet request{wire::command::retire,scope,revoke?125u:0u,remote->binding,0};
                auto encoded{wire::encode(request)};
                auto result{backend.connection.call("service.available-retire-v1",remote->availability_alias,
                    {encoded.begin(),encoded.end()},backend.timeout)};
                wire::packet response{};
                if(!result.error&&!wire::decode_response(result.payload.data(),result.payload.size(),request,response)) { result.error=13; }
                if(result.error)
                {
                    // Even quota exhaustion/revocation cannot keep an owned
                    // native client alive. Losing the session abandons ALL its
                    // native registries; no old binding transparently reopens.
                    backend.connection.close();backend.connected=false;backend.retire_observations(result.error);
                }
                return result.error;
            }
#if defined(__cpp_exceptions)
            catch(...)
            { backend.connection.close();backend.connected=false;backend.retire_observations(12);return 12; }
#endif
        }
        template <core::version Version>
        inline ::std::uint32_t capability_close(::std::uint64_t handle) noexcept
        {
            runtime::ros_caller_identity caller{}; ::std::shared_ptr<instance> state{};
            if(auto error{identify(caller,state)}) { return audit_denial(Version,error); }
            auto& backend{state->backends[static_cast<unsigned>(Version)-1]};
            ::std::lock_guard<::std::mutex> backend_lock{backend.mutex};
            core::capability_spec spec{};
            core::authorization description{};
            auto r{capabilities.describe(caller.runtime_instance,caller.generation,Version,handle,description)};
            if(r) { spec=*description.capability(); }
            if(!r) { return audit_denial(Version,code(r)); }
            audit_result audit{caller.runtime_instance,caller.generation,Version,spec.alias};
            audit.function="capability_close";
            bool last_open_handle{};
            r=capabilities.close(caller.runtime_instance,caller.generation,Version,handle,::std::addressof(last_open_handle));
            if(!r) { return audit.finish(code(r)); }
            auto retirement_error{retire_availability(backend,handle,spec,false)};
            auto rmw_retirement_error{retire_rmw_endpoints(backend,handle,spec.alias,false,description.instance_number(),caller.generation)};
            if(!retirement_error){retirement_error=rmw_retirement_error;}
            retire_watches(backend,handle,spec.alias,false,0);
            if(!last_open_handle || !backend.connected) { return audit.finish(retirement_error); }
            auto result{backend.connection.call("capability.close",spec.alias,{},backend.timeout)};
            if(!backend.connection.session_id()) { backend.connected=false;backend.retire_observations(result.error?result.error:107); }
            return audit.finish(retirement_error?retirement_error:result.error);
        }

        template <core::version Version>
        inline ::std::uint32_t capability_revoke(::std::uint64_t handle) noexcept
        {
            runtime::ros_caller_identity caller{}; ::std::shared_ptr<instance> state{};
            if(auto error{identify(caller,state)}) { return audit_denial(Version,error); }
            auto& backend{state->backends[static_cast<unsigned>(Version)-1]};
            ::std::lock_guard<::std::mutex> backend_lock{backend.mutex};
            core::capability_spec spec{};
            core::authorization description{};
            auto r{capabilities.describe(caller.runtime_instance,caller.generation,Version,handle,description)};
            if(r) { spec=*description.capability(); }
            if(!r) { return audit_denial(Version,code(r)); }
            audit_result audit{caller.runtime_instance,caller.generation,Version,spec.alias};
            audit.function="capability_revoke";
            r=capabilities.revoke(caller.runtime_instance,caller.generation,Version,spec.alias);
            if(!r) { return audit.finish(code(r)); }
            auto retirement_error{retire_availability(backend,handle,spec,true)};
            auto rmw_retirement_error{retire_rmw_endpoints(backend,handle,spec.alias,true,description.instance_number(),caller.generation)};
            if(!retirement_error){retirement_error=rmw_retirement_error;}
            retire_watches(backend,handle,spec.alias,true,125);
            if(!backend.connected) { return audit.finish(retirement_error); }
            auto result{backend.connection.call("capability.revoke",spec.alias,{},backend.timeout)};
            if(result.error) { backend.connection.close(); backend.connected=false;backend.retire_observations(result.error); }
            return audit.finish(retirement_error?retirement_error:result.error);
        }

        inline void observe_source_error(session& backend,::std::uint64_t source,::std::uint32_t error,bool native_admitted=false) noexcept
        {
            if(!error||error==11) { return; } // Idle/readiness is not a native fault.
            for(auto& item:backend.observations)
            {
                auto& selected{item.second};if(selected.retired||selected.source_handle!=source||!(selected.local->allowed_kinds()&core::ipc::event_bit(core::ipc::event_kind::resource_error))) { continue; }
                (void)selected.local->enqueue({core::ipc::event_kind::resource_error,static_cast<::std::uint8_t>(native_admitted?core::ipc::event_external_effects_may_remain:0),error,0,{}});
            }
        }
        template <core::version Version>
        inline ::std::uint32_t operation(::std::string_view op, ::std::uint64_t handle, ::std::uint32_t memory,
            ::std::uint64_t offset, ::std::uint64_t length, bool output, ::std::uint64_t out_offset,
            ::std::uint64_t capacity, ::std::uint64_t length_offset) noexcept
        {
            runtime::ros_caller_identity caller{}; ::std::shared_ptr<instance> state{};
            if(auto error{identify(caller,state)}) { return audit_denial(Version,error); }
            auto& backend{state->backends[static_cast<unsigned>(Version)-1]};
            ::std::lock_guard<::std::mutex> backend_lock{backend.mutex};
            core::capability_spec spec{};
            core::authorization description{};
            auto r{capabilities.describe(caller.runtime_instance,caller.generation,Version,handle,description)};
            if(r) { spec=*description.capability(); }
            if(!r) { return audit_denial(Version,code(r)); }
            audit_result audit{caller.runtime_instance,caller.generation,Version,spec.alias};
            audit.function=op;
            bool native_admitted{},native_reported_failure{};
            auto finish=[&](::std::uint32_t error) {if(!native_reported_failure) {observe_source_error(backend,handle,error,native_admitted);}return audit.finish(error);};
            auto const actual_op{op == "event.poll" ? ::std::string_view{spec.operation} : op};
            if(op == "event.poll" && actual_op != "topic.subscribe" && actual_op != "service.advertise" && actual_op != "param.subscribe-events" && actual_op != "action.advertise") { return finish(13); }
            if(spec.operation != actual_op) { return finish(13); }
            if(length > core::ipc::max_payload_bytes || capacity > core::ipc::max_payload_bytes) { return finish(90); }
            if(output && (!memory_range(memory,out_offset,capacity) || !memory_range(memory,length_offset,8))) { return finish(22); }
            ::std::vector<::std::uint8_t> payload{};
            if(length && !read_bytes(memory,offset,length,payload)) { return finish(22); }
            // Check the shared native goal queue before Source quota debit or
            // IPC. Another alias may designate the SAME owned native consumer.
            if(payload.size()==8)
            {
                auto goal{core::ipc::get(payload.data(),8)};
                for(auto const& item:backend.observations)
                {
                    auto const& watch{item.second};
                    if(!watch.retired&&watch.local->active()&&core::ipc::event_action_consumer_conflict(
                        actual_op,spec.resource,spec.type,goal,watch.source.operation,watch.source.resource,
                        watch.source.type,watch.correlation,watch.local->allowed_kinds())) { return finish(17); }
                }
            }
            core::authorization ticket{};
            auto payload_view{::std::as_bytes(::std::span{payload})};
            if(op != "event.poll" && op != "topic.subscribe" && op != "service.advertise" && op != "param.subscribe-events") { r=capabilities.authorize(caller.runtime_instance,caller.generation,Version,handle,actual_op,payload_view,ticket); }
            else { r=capabilities.authorize(caller.runtime_instance,caller.generation,Version,handle,actual_op,{},ticket,false,false); }
            if(!r) { return finish(code(r)); }
            if(auto error{backend.connect(Version)}) { return finish(error); }
            // The capability deadline bounds the Native operation at Gateway.
            // Keep the explicitly configured IPC budget independent so that a
            // Native timeout response can arrive before the control channel
            // expires. Reusing timeout-ms here loses that response and can
            // prevent the Guest's subsequent explicit capability retirement.
            auto const request_timeout{backend.timeout};
            // A receive capability cannot redirect delivery to another owned goal ID.
            // Keep the scalar request identity before moving the bounded copied request to IPC.
            bool const receives_goal{op == "action.receive-feedback" || op == "action.receive-result"};
            ::std::uint64_t const requested_goal{receives_goal && payload.size() == 8 ? core::ipc::get(payload.data(),8) : 0};
            native_admitted=true;auto result{backend.connection.call(op,spec.alias,::std::move(payload),request_timeout)};
            // A well-formed Native reply may itself report ENOTCONN or
            // ETIMEDOUT while the owned IPC session remains usable. Only an
            // actual disconnected transport retires the session; otherwise
            // preserve it for close/revoke and other admitted control calls.
            if(!backend.connection.session_id()) { backend.connected=false;backend.retire_observations(result.error?result.error:107); }
            if(result.error) {native_reported_failure=true; return finish(result.error); }
            if(receives_goal && (!requested_goal || result.payload.size() < 8 || core::ipc::get(result.payload.data(),8) != requested_goal))
            { return finish(13); }
            if(output)
            {
                if(op == "event.poll")
                {
                    // Recheck message structure, quota and revocation at delivery. The admission ticket
                    // pins the original authority; Gateway revocation also drains queued messages.
                    r=capabilities.authorize(caller.runtime_instance,caller.generation,Version,handle,actual_op,
                        ::std::as_bytes(::std::span{result.payload}),ticket,true,true);
                    if(!r) { return finish(code(r)); }
                }
                else
                {
                    r=capabilities.authorize_response(caller.runtime_instance,caller.generation,Version,handle,
                        ::std::as_bytes(::std::span{result.payload}),ticket);
                    if(!r) { return finish(code(r)); }
                }
                if(!write_u64(memory,length_offset,result.payload.size())) { return finish(22); }
                if(result.payload.size() > capacity) { return finish(90); }
                if(!runtime::preload_memory_write_host_api(memory,out_offset,result.payload.data(),result.payload.size())) { return finish(22); }
            }
            return finish(0);
        }
        // Additive event ABI. The observer is separately authorized; source
        // authority is borrowed only under a live trusted Instance/backend lease.
        // No native callback retains a guest address or enters guest code.
        template <core::version Version>
        inline ::std::uint32_t event_watch(::std::uint64_t observer_handle,::std::uint64_t source_handle,::std::uint64_t correlation) noexcept
        {
            runtime::ros_caller_identity caller{};::std::shared_ptr<instance> state{};
            if(auto error=identify(caller,state)) { return audit_denial(Version,error); }
            auto& backend{state->backends[static_cast<unsigned>(Version)-1]};::std::lock_guard guard{backend.mutex};
            core::authorization observer,source;auto granted{capabilities.authorize(caller.runtime_instance,caller.generation,Version,observer_handle,"event.observe",{},observer,false,true)};
            if(!granted) { return audit_denial(Version,code(granted)); }
            auto const& cap{*observer.capability()};audit_result audit{caller.runtime_instance,caller.generation,Version,cap.alias};
            audit.function="event_watch";
            if(backend.observations.contains(observer_handle)) { return audit.finish(17); }
            granted=capabilities.describe(caller.runtime_instance,caller.generation,Version,source_handle,source);
            if(!granted) { return audit.finish(code(granted)); }
            auto const& selected{*source.capability()};core::ipc::event_kind kind{};::std::uint8_t flags{};
            if(observer_handle==source_handle||cap.limits.at("event-source-alias")!=selected.alias||cap.resource!=selected.resource||selected.operation=="event.observe") { return audit.finish(13); }
            bool observable{core::ipc::event_source_kind(selected.operation,kind,flags)};
            bool action{selected.operation=="action.receive-feedback"||selected.operation=="action.receive-result"};
            for(auto const& item:backend.observations) { if(item.second.source.alias==selected.alias) { return audit.finish(17); } }
            granted=capabilities.authorize(caller.runtime_instance,caller.generation,Version,source_handle,selected.operation,{},source,false,false);
            if(!granted) { return audit.finish(code(granted)); }
            if(auto error=backend.connect(Version)) { return audit.finish(error); }
            ::std::uint32_t mask{};core::ipc::event_overflow overflow{};
            if(!core::ipc::parse_event_kinds(cap.limits.at("event-kinds"),mask)||!core::ipc::parse_event_overflow(cap.limits.at("overflow-policy"),overflow)) { return audit.finish(22); }
            auto permitted{core::ipc::event_bit(core::ipc::event_kind::resource_error)|core::ipc::event_bit(core::ipc::event_kind::resource_closed)|(observable?core::ipc::event_bit(kind):0)};
            if(mask&~permitted) { return audit.finish(13); }
            // Metadata-only observation has no native goal/read activation.
            // A correlation is required only when that Action data kind was
            // explicitly selected; all other watches require correlation zero.
            if((action&&observable&&bool(mask&core::ipc::event_bit(kind)))!=bool(correlation)) {return audit.finish(22);}
            if(action&&correlation)
            {
                for(auto const& item:backend.observations)
                {
                    auto const& watch{item.second};
                    if(!watch.retired&&watch.local->active()&&core::ipc::event_action_consumer_conflict(
                        selected.operation,selected.resource,selected.type,correlation,watch.source.operation,
                        watch.source.resource,watch.source.type,watch.correlation,watch.local->allowed_kinds())) {return audit.finish(17);}
                }
            }
            auto number=[&](char const* name) { ::std::uint64_t value{};(void)core::parse_u64(cap.limits.at(name),value);return value; };
            core::ipc::event_stream_settings settings{number("max-event-queue-depth"),number("max-event-queue-bytes"),number("max-event-bytes"),overflow==core::ipc::event_overflow::backpressure?number("max-pending-events"):0,overflow==core::ipc::event_overflow::backpressure?number("max-pending-event-bytes"):0,overflow};
            core::ipc::event_header binding{};binding.backend=static_cast<::std::uint8_t>(Version);binding.source_handle=source_handle;binding.instance=source.instance_number();binding.session=backend.connection.session_id();
            auto local{::std::make_shared<core::ipc::event_stream>(settings,binding,mask)};if(!local->configured()) { return audit.finish(22); }
            ::std::vector<::std::uint8_t> data;data.reserve(24);core::ipc::put(data,source_handle,8);core::ipc::put(data,binding.instance,8);core::ipc::put(data,correlation,8);
            auto result{backend.connection.call("event.watch",cap.alias,::std::move(data),backend.timeout)};
            if(!result.payload.empty()) {backend.connection.close();backend.connected=false;backend.retire_observations(13);return audit.finish(13);}
            if(result.error) { if(!backend.connection.session_id()||result.error==110) {backend.connection.close();backend.connected=false;backend.retire_observations(result.error);}return audit.finish(result.error); }
            backend.observations.emplace(observer_handle,watched_event{cap,selected,source_handle,correlation,binding.instance,binding.session,0,0,false,::std::move(local),{}});
            return audit.finish(0);
        }
        template <core::version Version>
        inline ::std::uint32_t event_poll_v1(::std::uint64_t observer_handle,::std::uint32_t memory,::std::uint64_t out,::std::uint64_t capacity,::std::uint64_t length_out) noexcept
        {
            runtime::ros_caller_identity caller{};::std::shared_ptr<instance> state{};
            if(auto error=identify(caller,state)) { return audit_denial(Version,error); }
            auto& backend{state->backends[static_cast<unsigned>(Version)-1]};::std::lock_guard guard{backend.mutex};
            core::authorization observer,source;auto granted{capabilities.authorize(caller.runtime_instance,caller.generation,Version,observer_handle,"event.observe",{},observer,false,true)};
            if(!granted) { return audit_denial(Version,code(granted)); }
            audit_result audit{caller.runtime_instance,caller.generation,Version,observer.capability()->alias};
            audit.function="event_poll_v1";
            if(capacity>core::ipc::max_payload_bytes) { return audit.finish(90); }
            if(!memory_range(memory,out,capacity)||!memory_range(memory,length_out,8)) { return audit.finish(22); }
            if(core::ipc::event_output_overlaps_length(out,capacity,length_out)) {return audit.finish(22);}
            auto found{backend.observations.find(observer_handle)};if(found==backend.observations.end()) { return audit.finish(9); }auto& watched{found->second};
            auto malformed=[&] {backend.connection.close();backend.connected=false;backend.retire_observations(13);return audit.finish(13);};
            if(!watched.retired)
            {
                granted=capabilities.authorize(caller.runtime_instance,caller.generation,Version,watched.source_handle,watched.source.operation,{},source,false,false);
                if(!granted) { watched.retired=true;watched.requests.clear();watched.local->seal(code(granted),true); }
            }
            core::ipc::call_result result;bool from_local{watched.retired||watched.local->readiness().ready_events!=0};
            if(from_local) { result=watched.local->peek(capacity); }
            else
            {
                ::std::vector<::std::uint8_t> argument;core::ipc::put(argument,capacity,8);
                result=backend.connection.call("event.peek-v1",watched.observer.alias,::std::move(argument),backend.timeout);
                if(!backend.connection.session_id()) { backend.connected=false;backend.retire_observations(result.error?result.error:107);from_local=true;result=watched.local->peek(capacity); }
            }
            if(result.error==90)
            {
                if(result.payload.empty()) {return audit.finish(90);}
                if(result.payload.size()!=8||core::ipc::get(result.payload.data(),8)<core::ipc::event_header_bytes) { return malformed(); }
                return audit.finish(write_u64(memory,length_out,core::ipc::get(result.payload.data(),8))?90u:22u);
            }
            if(result.error) { return audit.finish(result.error); }
            ::std::uint64_t maximum{};if(!core::parse_u64(watched.observer.limits.at("max-event-bytes"),maximum)||result.payload.size()>capacity||result.payload.size()>maximum) { return malformed(); }
            core::ipc::event_header header{};
            if(!core::ipc::decode_event(result.payload.data(),result.payload.size(),header)||header.backend!=static_cast<unsigned>(Version)||header.source_handle!=watched.source_handle||header.instance!=watched.instance||header.session!=watched.native_session||!(watched.local->allowed_kinds()&core::ipc::event_bit(header.kind))||!core::ipc::event_kind_for_source(watched.source.operation,header.kind,header.flags)) { return malformed(); }
            if(!from_local)
            {
                if(header.sequence<=watched.native_sequence||(header.sequence!=watched.native_sequence+1&&(!core::ipc::event_control(header.kind)||!(header.flags&core::ipc::event_source_retired)))) { return malformed(); }

            }
            else if(!core::ipc::event_control(header.kind)||(watched.retired&&(header.flags&core::ipc::event_source_retired)==0)) { return malformed(); }
            if(header.correlation&&watched.source.operation=="service.call"&&!watched.requests.contains(header.correlation)) { return malformed(); }
            if(header.correlation&&watched.correlation&&header.correlation!=watched.correlation) { return malformed(); }
            if(!core::ipc::event_control(header.kind)&&watched.retired) {return malformed();}
            auto body{::std::as_bytes(::std::span{result.payload}).subspan(core::ipc::event_header_bytes)};
            granted=capabilities.authorize_event_delivery(caller.runtime_instance,caller.generation,Version,observer_handle,watched.source_handle,header,body,observer,source);
            if(!granted) {return code(granted)==11?audit.finish(11):malformed();}
            if(watched.delivered==UINT64_MAX) { return audit.finish(90); }
            auto native_sequence{header.sequence};header.sequence=watched.delivered+1;if(!core::ipc::encode_event_header(header,result.payload.data(),result.payload.size())) { return audit.finish(13); }
            if(!runtime::preload_memory_write_host_api(memory,out,result.payload.data(),result.payload.size())) {backend.connection.close();backend.connected=false;backend.retire_observations(22);return audit.finish(22);}
            core::ipc::call_result committed;
            if(from_local) {committed.error=watched.local->commit(native_sequence);}
            else {::std::vector<::std::uint8_t> argument;core::ipc::put(argument,native_sequence,8);committed=backend.connection.call("event.commit",watched.observer.alias,::std::move(argument),backend.timeout);}
            if(committed.error||!committed.payload.empty()) {backend.connection.close();backend.connected=false;backend.retire_observations(committed.error?committed.error:13);return audit.finish(committed.error?committed.error:13);}
            if(!from_local) {watched.native_sequence=native_sequence;}
            if(!write_u64(memory,length_out,result.payload.size())) {backend.connection.close();backend.connected=false;backend.retire_observations(22);return audit.finish(22);}
            ++watched.delivered;if(watched.source.operation=="service.call"&&header.correlation) { watched.requests.erase(header.correlation); }return audit.finish(0);
        }
        template <core::version Version>
        inline ::std::uint32_t event_ready(::std::uint64_t observer_handle,::std::uint32_t memory,::std::uint64_t out) noexcept
        {
            runtime::ros_caller_identity caller{};::std::shared_ptr<instance> state{};
            if(auto error=identify(caller,state)) { return audit_denial(Version,error); }
            auto& backend{state->backends[static_cast<unsigned>(Version)-1]};::std::lock_guard guard{backend.mutex};
            core::authorization observer,source;auto granted{capabilities.authorize(caller.runtime_instance,caller.generation,Version,observer_handle,"event.observe",{},observer,false,true)};
            if(!granted) { return audit_denial(Version,code(granted)); }
            audit_result audit{caller.runtime_instance,caller.generation,Version,observer.capability()->alias};
            audit.function="event_ready";
            if(!memory_range(memory,out,core::ipc::event_ready_bytes)) { return audit.finish(22); }
            auto found{backend.observations.find(observer_handle)};if(found==backend.observations.end()) { return audit.finish(9); }
            auto& watched{found->second};core::ipc::call_result result;
            if(!watched.retired)
            {
                granted=capabilities.authorize(caller.runtime_instance,caller.generation,Version,watched.source_handle,watched.source.operation,{},source,false,false);
                if(!granted) { watched.retired=true;watched.requests.clear();watched.local->seal(code(granted),true); }
            }
            if(watched.retired) { auto ready=core::ipc::encode_event_readiness(watched.local->readiness());result.payload.assign(ready.begin(),ready.end()); }
            else
            {
                result=backend.connection.call("event.ready",watched.observer.alias,{},backend.timeout);
                if(!backend.connection.session_id()) { backend.connected=false;backend.retire_observations(result.error?result.error:107);auto ready=core::ipc::encode_event_readiness(watched.local->readiness());result={0,{ready.begin(),ready.end()}}; }
            }
            if(result.error) { return audit.finish(result.error); }
            if(result.payload.size()!=core::ipc::event_ready_bytes) {backend.connection.close();backend.connected=false;backend.retire_observations(13);return audit.finish(13);}
            if(!watched.retired&&watched.local->readiness().ready_events)
            { auto ready{watched.local->readiness()};auto native{core::ipc::get(result.payload.data(),8)};core::ipc::event_put(result.payload.data(),native>UINT64_MAX-ready.ready_events?UINT64_MAX:native+ready.ready_events,8);core::ipc::event_put(result.payload.data()+8,ready.next_event_bytes,8); }
            granted=capabilities.charge_observation(caller.runtime_instance,caller.generation,Version,observer_handle,result.payload.size(),observer);if(!granted) { return audit.finish(code(granted)); }
            return audit.finish(runtime::preload_memory_write_host_api(memory,out,result.payload.data(),result.payload.size())?0u:22u);
        }
        template <core::version Version>
        inline ::std::uint32_t service_submit(::std::uint64_t source_handle,::std::uint64_t observer_handle,::std::uint32_t memory,::std::uint64_t input,::std::uint64_t length,::std::uint64_t request_out) noexcept
        {
            runtime::ros_caller_identity caller{};::std::shared_ptr<instance> state{};
            if(auto error=identify(caller,state)) { return audit_denial(Version,error); }
            auto& backend{state->backends[static_cast<unsigned>(Version)-1]};::std::lock_guard guard{backend.mutex};
            core::authorization observer,source;auto granted{capabilities.authorize(caller.runtime_instance,caller.generation,Version,observer_handle,"event.observe",{},observer,false,true)};
            if(!granted) { return audit_denial(Version,code(granted)); }
            audit_result audit{caller.runtime_instance,caller.generation,Version,observer.capability()->alias};
            audit.function="service_submit";
            auto found{backend.observations.find(observer_handle)};if(found==backend.observations.end()) { return audit.finish(9); }auto& watched{found->second};
            if(watched.source_handle!=source_handle||watched.source.operation!="service.call") { return audit.finish(13); }
            if(watched.retired) { return audit.finish(125); }
            auto required{core::ipc::event_bit(core::ipc::event_kind::service_response)|core::ipc::event_bit(core::ipc::event_kind::resource_error)};
            if((watched.local->allowed_kinds()&required)!=required) { return audit.finish(13); }
            ::std::uint64_t request_limit{};if(!core::parse_u64(watched.source.limits.at("max-request-bytes"),request_limit)) {return audit.finish(22);}
            if(length>request_limit||length>core::ipc::max_payload_bytes-526) { return audit.finish(90); }
            if(!memory_range(memory,input,length)||!memory_range(memory,request_out,8)) { return audit.finish(22); }
            ::std::vector<::std::uint8_t> bytes;if(!read_bytes(memory,input,length,bytes)) { return audit.finish(22); }
            granted=capabilities.authorize(caller.runtime_instance,caller.generation,Version,source_handle,"service.call",::std::as_bytes(::std::span{bytes}),source,true,true);
            if(!granted) { return audit.finish(code(granted)); }
            ::std::uint64_t maximum{};if(!core::parse_u64(watched.source.limits.at("max-inflight"),maximum)) { return audit.finish(22); }
            if(watched.requests.size()>=maximum) { return audit.finish(11); }
            if(backend.next_service_request==UINT64_MAX) { return audit.finish(90); }auto id{backend.next_service_request++};
            watched.requests.insert(id);
            ::std::vector<::std::uint8_t> data;data.reserve(8+bytes.size());core::ipc::put(data,id,8);data.insert(data.end(),bytes.begin(),bytes.end());
            auto result{backend.connection.call("service.submit",watched.observer.alias,::std::move(data),backend.timeout)};
            if(!backend.connection.session_id()) { backend.connected=false;backend.retire_observations(result.error?result.error:107); }
            if(!result.payload.empty()) {backend.connection.close();backend.connected=false;backend.retire_observations(13);watched.requests.erase(id);return audit.finish(13);}
            if(result.error) {watched.requests.erase(id);return audit.finish(result.error);}
            if(!write_u64(memory,request_out,id)) {backend.connection.close();backend.connected=false;backend.retire_observations(22);return audit.finish(22);}
            return audit.finish(0);
        }

        // Optional client extension. Both true capability leases remain live
        // through native observation, exact response association and Guest copy.
        // No Guest service name/QoS/request body crosses this boundary.
        template <core::version Version>
        inline ::std::uint32_t client_observe(::std::uint64_t availability_handle,::std::uint64_t source_handle,
            ::std::uint64_t wait_ns,::std::uint32_t memory,::std::uint64_t output,bool features) noexcept
        {
            namespace wire=core::ipc::client_availability;
            auto entry_time{::std::chrono::steady_clock::now()};
#if defined(__cpp_exceptions)
            try
#endif
            {
                runtime::ros_caller_identity caller{};::std::shared_ptr<instance> state{};
                if(auto error{identify(caller,state)}) { return audit_denial(Version,error); }
                auto& backend{state->backends[static_cast<unsigned>(Version)-1]};
                // This NEW query never queues behind a long admitted business
                // call. Busy11 is a truthful failure, not false readiness. The
                // existing close/revoke/old v1 locking semantics are unchanged.
                ::std::unique_lock backend_lock{backend.mutex,::std::try_to_lock};
                if(!backend_lock.owns_lock()) { return audit_denial(Version,11); }
                auto extent{features?8u:1u};
                if(wait_ns>wire::maximum_wait_ns||!memory_range(memory,output,extent)) { return audit_denial(Version,22); }
                core::authorization observation,source;
                auto granted{capabilities.authorize(caller.runtime_instance,caller.generation,Version,availability_handle,
                    "service.available",{},observation,false,false)};
                if(!granted) { return audit_denial(Version,code(granted)); }
                audit_result audit{caller.runtime_instance,caller.generation,Version,observation.capability()->alias};
                audit.function=features?"client.features":"client.service_available";
                granted=capabilities.authorize(caller.runtime_instance,caller.generation,Version,source_handle,"service.call",{},source,false,false);
                if(!granted) { return audit.finish(code(granted)); }
                auto const& cap{*observation.capability()};auto const& called{*source.capability()};
                audit.source_alias.assign(called.alias);
                if(!core::service_availability_pair(Version,cap,called)||observation.target()!=source.target()||
                   observation.instance_number()!=source.instance_number()) { return audit.finish(13); }
                ::std::uint64_t maximum{},timeout{},history{};
                if(!core::parse_u64(cap.limits.at("max-wait-ns"),maximum)||!core::parse_u64(cap.limits.at("timeout-ms"),timeout)||
                   !core::parse_u64(cap.limits.at("max-binding-history"),history)||wait_ns>maximum) { return audit.finish(22); }
                wire::packet request{features?wire::command::features:wire::command::query,wire::retirement::none,0,
                    {static_cast<::std::uint8_t>(Version),availability_handle,source_handle,observation.instance_number(),caller.generation},wait_ns};
                if(!wire::valid_request(request)) { return audit.finish(22); }
                auto raw{wire::encode(request)};
                // One NEW authority debit; source.call supplies live independent
                // authority only. No business-call/request quota is consumed.
                core::authorization admitted;
                granted=capabilities.authorize(caller.runtime_instance,caller.generation,Version,availability_handle,
                    "service.available",::std::as_bytes(::std::span{raw}),admitted,true,true);
                if(!granted) { return audit.finish(code(granted)); }
                auto until{entry_time+::std::chrono::milliseconds(timeout)};
                if(auto error{backend.connect(Version,until)}) { return audit.finish(error); }
                audit.client_session_label=backend.availability_debug_session;
                core::service_availability_history::entry const* row{};
                if(auto error{backend.availability_history.bind(request.binding,cap.alias,called.alias,backend.availability_native_session,
                    static_cast<::std::size_t>(history),row)}) { return audit.finish(error); }
                auto remaining{::std::chrono::duration_cast<::std::chrono::milliseconds>(until-::std::chrono::steady_clock::now()).count()};
                if(remaining<=0) { return audit.finish(110); }
                auto result{backend.connection.call("service.available-v1",cap.alias,{raw.begin(),raw.end()},
                    static_cast<::std::uint32_t>(remaining))};
                if(!backend.connection.session_id()) { backend.connected=false;backend.retire_observations(result.error?result.error:107); }
                if(result.error)
                {
                    // A successful extension reply is the only feature proof.
                    // No error or empty/default reply becomes an answer.
                    if(!result.payload.empty()) { backend.connection.close();backend.connected=false;backend.retire_observations(13);return audit.finish(13); }
                    return audit.finish(result.error);
                }
                wire::packet response{};
                if(!wire::decode_response(result.payload.data(),result.payload.size(),request,response))
                { backend.connection.close();backend.connected=false;backend.retire_observations(13);return audit.finish(13); }
                core::authorization response_credit;
                granted=capabilities.authorize_response(caller.runtime_instance,caller.generation,Version,availability_handle,
                    ::std::as_bytes(::std::span{result.payload}),response_credit);
                if(!granted) { return audit.finish(code(granted)); }
                if(::std::chrono::steady_clock::now()>=until) { return audit.finish(110); }
                // Success is atomic from the SDK's perspective. No write on
                // failure; the runtime copy primitive rechecks the live extent.
                if(features)
                {
                    if(!write_u64(memory,output,response.value)) { return audit.finish(22); }
                    audit.client_features=response.value;audit.client_features_observed=true;return audit.finish(0);
                }
                auto value{static_cast<::std::uint8_t>(response.value)};
                return audit.finish(runtime::preload_memory_write_host_api(memory,output,&value,1)?0u:22u);
            }
#if defined(__cpp_exceptions)
            catch(::std::bad_alloc const&) { return audit_denial(Version,12); }
            catch(...) { return audit_denial(Version,22); }
#endif
        }
        template <core::version V> inline ::std::uint32_t client_features(::std::uint64_t a,::std::uint64_t s,::std::uint32_t m,::std::uint64_t o) noexcept
        { return client_observe<V>(a,s,0,m,o,true); }
        template <core::version V> inline ::std::uint32_t client_service_available(::std::uint64_t a,::std::uint64_t s,::std::uint64_t n,::std::uint32_t m,::std::uint64_t o) noexcept
        { return client_observe<V>(a,s,n,m,o,false); }

        template <core::version V> inline ::std::uint32_t topic_publish(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l) noexcept
        { return operation<V>("topic.publish",h,m,o,l,false,0,0,0); }
        template <core::version V> inline ::std::uint32_t topic_subscribe(::std::uint64_t h) noexcept
        { return operation<V>("topic.subscribe",h,0,0,0,false,0,0,0); }
        template <core::version V> inline ::std::uint32_t event_poll(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t c,::std::uint64_t l) noexcept
        { return operation<V>("event.poll",h,m,0,0,true,o,c,l); }
        template <core::version V> inline ::std::uint32_t service_call(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("service.call",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t service_advertise(::std::uint64_t h) noexcept
        { return operation<V>("service.advertise",h,0,0,0,false,0,0,0); }
        template <core::version V> inline ::std::uint32_t service_respond(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("service.respond",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t action_send_goal(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t z) noexcept
        { return operation<V>("action.send-goal",h,m,o,l,true,r,c,z); }
        template <core::version V> inline ::std::uint32_t action_cancel_own_goal(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t z) noexcept
        { return operation<V>("action.cancel-own-goal",h,m,o,l,true,r,c,z); }
        template <core::version V> inline ::std::uint32_t action_cancel_any_goal(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t z) noexcept
        { return operation<V>("action.cancel-any-goal",h,m,o,l,true,r,c,z); }
        template <core::version V> inline ::std::uint32_t action_receive_feedback(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t z) noexcept
        { return operation<V>("action.receive-feedback",h,m,o,l,true,r,c,z); }
        template <core::version V> inline ::std::uint32_t action_receive_result(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t z) noexcept
        { return operation<V>("action.receive-result",h,m,o,l,true,r,c,z); }
        template <core::version V> inline ::std::uint32_t action_advertise(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t z) noexcept
        { return operation<V>("action.advertise",h,m,o,l,true,r,c,z); }
        template <core::version V> inline ::std::uint32_t action_respond(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t z) noexcept
        { return operation<V>("action.respond",h,m,o,l,true,r,c,z); }
        template <core::version V> inline ::std::uint32_t tf_lookup(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("tf.lookup",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t tf_broadcast(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("tf.broadcast",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t parameter_get(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("param.get",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t parameter_set(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("param.set",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t parameter_delete(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("param.delete",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t parameter_exists(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("param.exists",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t parameter_list(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("param.list",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t parameter_search(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("param.search",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t parameter_describe(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("param.describe",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t parameter_declare(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("param.declare",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t parameter_undeclare(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("param.undeclare",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t graph_inspect_topic(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("graph.inspect-topic",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t graph_inspect_node(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("graph.inspect-node",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t graph_inspect_service(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("graph.inspect-service",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t graph_inspect_action(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("graph.inspect-action",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t parameter_subscribe_events(::std::uint64_t h) noexcept
        { return operation<V>("param.subscribe-events",h,0,0,0,false,0,0,0); }
        template <core::version V> inline ::std::uint32_t graph_list_topics(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("graph.list-topics",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t graph_list_nodes(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("graph.list-nodes",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t graph_list_services(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("graph.list-services",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t lifecycle_get_state(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t z) noexcept
        { return operation<V>("lifecycle.get-state",h,m,o,l,true,r,c,z); }
        template <core::version V> inline ::std::uint32_t lifecycle_change_state(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t z) noexcept
        { return operation<V>("lifecycle.change-state",h,m,o,l,true,r,c,z); }
        template <core::version V> inline ::std::uint32_t time_read_ros(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("time.read-ros",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t time_read_wall(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("time.read-wall",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t time_read_system(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("time.read-system",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t time_read_steady(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l,::std::uint64_t r,::std::uint64_t c,::std::uint64_t n) noexcept
        { return operation<V>("time.read-steady",h,m,o,l,true,r,c,n); }
        template <core::version V> inline ::std::uint32_t log_write(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o,::std::uint64_t l) noexcept
        { return operation<V>("log.write",h,m,o,l,false,0,0,0); }
    }

    namespace wrapper
    {
        using features = ::uwvm2::uwvm::wasm::type::binfmt_ver1_feature_list_t;
        using value = ::uwvm2::uwvm::wasm::type::feature_list_final_value_type_t<features>;
        template <typename T> consteval value map_value() noexcept
        { static_assert(::std::is_integral_v<T>); return sizeof(T) <= 4 ? value::i32 : value::i64; }
        template <typename Signature, Signature Fn, auto& Name> struct function;
        template <typename R, typename... A, R (*Fn)(A...) noexcept, auto& Name>
        struct function<R (*)(A...) noexcept,Fn,Name>
        {
            using result_tuple = ::uwvm2::uwvm::wasm::type::import_function_result_tuple_t<features,map_value<R>()>;
            using parameter_tuple = ::uwvm2::uwvm::wasm::type::import_function_parameter_tuple_t<features,map_value<A>()...>;
            using local_imported_function_type = ::uwvm2::uwvm::wasm::type::local_imported_function_type_t<result_tuple,parameter_tuple>;
            inline static constexpr ::uwvm2::utils::container::u8string_view function_name{Name};
            static void call(local_imported_function_type& call) noexcept
            {
                auto result{[&]<::std::size_t... I>(::std::index_sequence<I...>) noexcept
                { return Fn(static_cast<A>(::uwvm2::utils::container::get<I>(call.params))...); }(::std::make_index_sequence<sizeof...(A)>{})};
                ::uwvm2::utils::container::get<0>(call.res)=static_cast<::std::remove_cvref_t<decltype(::uwvm2::utils::container::get<0>(call.res))>>(result);
            }
        };
        template <auto Fn, auto& Name> using native_function = function<decltype(Fn),Fn,Name>;
        inline constexpr char8_t abi_version_name[]=u8"abi_version";
        inline constexpr char8_t rmw_srv_client_activate_name[]=u8"client_activate";
        inline constexpr char8_t rmw_srv_service_activate_name[]=u8"service_activate";
        inline constexpr char8_t rmw_srv_endpoint_inspect_name[]=u8"endpoint_inspect";
        inline constexpr char8_t rmw_srv_client_submit_name[]=u8"client_submit";
        inline constexpr char8_t rmw_srv_client_abandon_name[]=u8"client_abandon";
        inline constexpr char8_t rmw_srv_service_respond_name[]=u8"service_respond";
        inline constexpr char8_t rmw_srv_endpoint_peek_name[]=u8"endpoint_peek";
        inline constexpr char8_t rmw_srv_endpoint_commit_name[]=u8"endpoint_commit";
        inline constexpr char8_t rmw_srv_endpoint_cancel_name[]=u8"endpoint_cancel";
        inline constexpr char8_t rmw_srv_client_available_name[]=u8"client_available";
        inline constexpr char8_t rmw_srv_endpoint_retire_name[]=u8"endpoint_retire";
        inline constexpr char8_t rmw_publisher_activate_name[]=u8"publisher_activate";
        inline constexpr char8_t rmw_publisher_inspect_name[]=u8"publisher_inspect";
        inline constexpr char8_t rmw_publisher_publish_name[]=u8"publisher_publish";
        inline constexpr char8_t rmw_publisher_retire_name[]=u8"publisher_retire";
        inline constexpr char8_t rmw_subscription_activate_name[]=u8"subscription_activate";
        inline constexpr char8_t rmw_subscription_inspect_name[]=u8"subscription_inspect";
        inline constexpr char8_t rmw_subscription_peek_name[]=u8"subscription_peek";
        inline constexpr char8_t rmw_subscription_commit_name[]=u8"subscription_commit";
        inline constexpr char8_t rmw_subscription_cancel_name[]=u8"subscription_cancel";
        inline constexpr char8_t rmw_subscription_retire_name[]=u8"subscription_retire";
        inline constexpr char8_t client_features_name[]=u8"features";
        inline constexpr char8_t client_service_available_name[]=u8"service_available";
        inline constexpr char8_t capability_open_name[]=u8"capability_open";
        inline constexpr char8_t capability_close_name[]=u8"capability_close";
        inline constexpr char8_t capability_revoke_name[]=u8"capability_revoke";
        inline constexpr char8_t topic_publish_name[]=u8"topic_publish";
        inline constexpr char8_t topic_subscribe_name[]=u8"topic_subscribe";
        inline constexpr char8_t event_poll_name[]=u8"event_poll";
        inline constexpr char8_t event_watch_name[]=u8"event_watch";
        inline constexpr char8_t event_poll_v1_name[]=u8"event_poll_v1";
        inline constexpr char8_t event_ready_name[]=u8"event_ready";
        inline constexpr char8_t service_submit_name[]=u8"service_submit";
        inline constexpr char8_t service_call_name[]=u8"service_call";
        inline constexpr char8_t service_advertise_name[]=u8"service_advertise";
        inline constexpr char8_t service_respond_name[]=u8"service_respond";
        inline constexpr char8_t action_send_goal_name[]=u8"action_send_goal";
        inline constexpr char8_t action_cancel_own_goal_name[]=u8"action_cancel_own_goal";
        inline constexpr char8_t action_cancel_any_goal_name[]=u8"action_cancel_any_goal";
        inline constexpr char8_t action_receive_feedback_name[]=u8"action_receive_feedback";
        inline constexpr char8_t action_receive_result_name[]=u8"action_receive_result";
        inline constexpr char8_t action_advertise_name[]=u8"action_advertise";
        inline constexpr char8_t action_respond_name[]=u8"action_respond";
        inline constexpr char8_t tf_lookup_name[]=u8"tf_lookup";
        inline constexpr char8_t tf_broadcast_name[]=u8"tf_broadcast";
        inline constexpr char8_t parameter_get_name[]=u8"parameter_get";
        inline constexpr char8_t parameter_set_name[]=u8"parameter_set";
        inline constexpr char8_t parameter_delete_name[]=u8"parameter_delete";
        inline constexpr char8_t parameter_exists_name[]=u8"parameter_exists";
        inline constexpr char8_t parameter_list_name[]=u8"parameter_list";
        inline constexpr char8_t parameter_search_name[]=u8"parameter_search";
        inline constexpr char8_t parameter_describe_name[]=u8"parameter_describe";
        inline constexpr char8_t parameter_declare_name[]=u8"parameter_declare";
        inline constexpr char8_t parameter_undeclare_name[]=u8"parameter_undeclare";
        inline constexpr char8_t graph_inspect_topic_name[]=u8"graph_inspect_topic";
        inline constexpr char8_t graph_inspect_node_name[]=u8"graph_inspect_node";
        inline constexpr char8_t graph_inspect_service_name[]=u8"graph_inspect_service";
        inline constexpr char8_t graph_inspect_action_name[]=u8"graph_inspect_action";
        inline constexpr char8_t parameter_subscribe_events_name[]=u8"parameter_subscribe_events";
        inline constexpr char8_t graph_list_topics_name[]=u8"graph_list_topics";
        inline constexpr char8_t graph_list_nodes_name[]=u8"graph_list_nodes";
        inline constexpr char8_t graph_list_services_name[]=u8"graph_list_services";
        inline constexpr char8_t lifecycle_get_state_name[]=u8"lifecycle_get_state";
        inline constexpr char8_t lifecycle_change_state_name[]=u8"lifecycle_change_state";
        inline constexpr char8_t time_read_ros_name[]=u8"time_read_ros";
        inline constexpr char8_t time_read_wall_name[]=u8"time_read_wall";
        inline constexpr char8_t time_read_system_name[]=u8"time_read_system";
        inline constexpr char8_t time_read_steady_name[]=u8"time_read_steady";
        inline constexpr char8_t log_write_name[]=u8"log_write";
    }
#if defined(UWVM_IMPORT_ROS1)
    struct ros1_local_imported_module_t
    {
        inline static constexpr ::uwvm2::utils::container::u8string_view module_name{u8"uwvm_ros1_v1"};
        using local_function_tuple = ::uwvm2::utils::container::tuple<
            wrapper::native_function<&host::abi_version<core::version::ros1>,wrapper::abi_version_name>,
            wrapper::native_function<&host::capability_open<core::version::ros1>,wrapper::capability_open_name>,
            wrapper::native_function<&host::capability_close<core::version::ros1>,wrapper::capability_close_name>,
            wrapper::native_function<&host::capability_revoke<core::version::ros1>,wrapper::capability_revoke_name>,
            wrapper::native_function<&host::topic_publish<core::version::ros1>,wrapper::topic_publish_name>,
            wrapper::native_function<&host::topic_subscribe<core::version::ros1>,wrapper::topic_subscribe_name>,
            wrapper::native_function<&host::event_poll<core::version::ros1>,wrapper::event_poll_name>,
            wrapper::native_function<&host::event_watch<core::version::ros1>,wrapper::event_watch_name>,
            wrapper::native_function<&host::event_poll_v1<core::version::ros1>,wrapper::event_poll_v1_name>,
            wrapper::native_function<&host::event_ready<core::version::ros1>,wrapper::event_ready_name>,
            wrapper::native_function<&host::service_submit<core::version::ros1>,wrapper::service_submit_name>,
            wrapper::native_function<&host::service_call<core::version::ros1>,wrapper::service_call_name>,
            wrapper::native_function<&host::service_advertise<core::version::ros1>,wrapper::service_advertise_name>,
            wrapper::native_function<&host::service_respond<core::version::ros1>,wrapper::service_respond_name>,
            wrapper::native_function<&host::action_send_goal<core::version::ros1>,wrapper::action_send_goal_name>,
            wrapper::native_function<&host::action_cancel_own_goal<core::version::ros1>,wrapper::action_cancel_own_goal_name>,
            wrapper::native_function<&host::action_cancel_any_goal<core::version::ros1>,wrapper::action_cancel_any_goal_name>,
            wrapper::native_function<&host::action_receive_feedback<core::version::ros1>,wrapper::action_receive_feedback_name>,
            wrapper::native_function<&host::action_receive_result<core::version::ros1>,wrapper::action_receive_result_name>,
            wrapper::native_function<&host::action_advertise<core::version::ros1>,wrapper::action_advertise_name>,
            wrapper::native_function<&host::action_respond<core::version::ros1>,wrapper::action_respond_name>,
            wrapper::native_function<&host::tf_lookup<core::version::ros1>,wrapper::tf_lookup_name>,
            wrapper::native_function<&host::tf_broadcast<core::version::ros1>,wrapper::tf_broadcast_name>,
            wrapper::native_function<&host::parameter_get<core::version::ros1>,wrapper::parameter_get_name>,
            wrapper::native_function<&host::parameter_set<core::version::ros1>,wrapper::parameter_set_name>,
            wrapper::native_function<&host::parameter_delete<core::version::ros1>,wrapper::parameter_delete_name>,
            wrapper::native_function<&host::parameter_exists<core::version::ros1>,wrapper::parameter_exists_name>,
            wrapper::native_function<&host::parameter_list<core::version::ros1>,wrapper::parameter_list_name>,
            wrapper::native_function<&host::parameter_search<core::version::ros1>,wrapper::parameter_search_name>,
            wrapper::native_function<&host::graph_inspect_topic<core::version::ros1>,wrapper::graph_inspect_topic_name>,
            wrapper::native_function<&host::graph_inspect_node<core::version::ros1>,wrapper::graph_inspect_node_name>,
            wrapper::native_function<&host::graph_inspect_service<core::version::ros1>,wrapper::graph_inspect_service_name>,
            wrapper::native_function<&host::graph_list_topics<core::version::ros1>,wrapper::graph_list_topics_name>,
            wrapper::native_function<&host::graph_list_nodes<core::version::ros1>,wrapper::graph_list_nodes_name>,
            wrapper::native_function<&host::graph_list_services<core::version::ros1>,wrapper::graph_list_services_name>,
            wrapper::native_function<&host::time_read_ros<core::version::ros1>,wrapper::time_read_ros_name>,
            wrapper::native_function<&host::time_read_wall<core::version::ros1>,wrapper::time_read_wall_name>,
            wrapper::native_function<&host::log_write<core::version::ros1>,wrapper::log_write_name>
        >;
    };
    inline constexpr ros1_local_imported_module_t ros1_local_imported_module{};
    struct ros1_client_local_imported_module_t
    {
        inline static constexpr ::uwvm2::utils::container::u8string_view module_name{u8"uwvm_ros1_client_v1"};
        using local_function_tuple = ::uwvm2::utils::container::tuple<
            wrapper::native_function<&host::abi_version<core::version::ros1>,wrapper::abi_version_name>,
            wrapper::native_function<&host::client_features<core::version::ros1>,wrapper::client_features_name>,
            wrapper::native_function<&host::client_service_available<core::version::ros1>,wrapper::client_service_available_name>>;
    };
    inline constexpr ros1_client_local_imported_module_t ros1_client_local_imported_module{};
    static_assert(::uwvm2::uwvm::wasm::type::is_local_imported_module<ros1_client_local_imported_module_t>);
    static_assert(::uwvm2::uwvm::wasm::type::has_local_function_tuple<ros1_client_local_imported_module_t>);
    static_assert(::uwvm2::uwvm::wasm::type::is_local_imported_module<ros1_local_imported_module_t>);
    static_assert(::uwvm2::uwvm::wasm::type::has_local_function_tuple<ros1_local_imported_module_t>);
#endif
#if defined(UWVM_IMPORT_ROS2)
    struct ros2_local_imported_module_t
    {
        inline static constexpr ::uwvm2::utils::container::u8string_view module_name{u8"uwvm_ros2_v1"};
        using local_function_tuple = ::uwvm2::utils::container::tuple<
            wrapper::native_function<&host::abi_version<core::version::ros2>,wrapper::abi_version_name>,
            wrapper::native_function<&host::capability_open<core::version::ros2>,wrapper::capability_open_name>,
            wrapper::native_function<&host::capability_close<core::version::ros2>,wrapper::capability_close_name>,
            wrapper::native_function<&host::capability_revoke<core::version::ros2>,wrapper::capability_revoke_name>,
            wrapper::native_function<&host::topic_publish<core::version::ros2>,wrapper::topic_publish_name>,
            wrapper::native_function<&host::topic_subscribe<core::version::ros2>,wrapper::topic_subscribe_name>,
            wrapper::native_function<&host::event_poll<core::version::ros2>,wrapper::event_poll_name>,
            wrapper::native_function<&host::event_watch<core::version::ros2>,wrapper::event_watch_name>,
            wrapper::native_function<&host::event_poll_v1<core::version::ros2>,wrapper::event_poll_v1_name>,
            wrapper::native_function<&host::event_ready<core::version::ros2>,wrapper::event_ready_name>,
            wrapper::native_function<&host::service_submit<core::version::ros2>,wrapper::service_submit_name>,
            wrapper::native_function<&host::service_call<core::version::ros2>,wrapper::service_call_name>,
            wrapper::native_function<&host::service_advertise<core::version::ros2>,wrapper::service_advertise_name>,
            wrapper::native_function<&host::service_respond<core::version::ros2>,wrapper::service_respond_name>,
            wrapper::native_function<&host::action_send_goal<core::version::ros2>,wrapper::action_send_goal_name>,
            wrapper::native_function<&host::action_cancel_own_goal<core::version::ros2>,wrapper::action_cancel_own_goal_name>,
            wrapper::native_function<&host::action_cancel_any_goal<core::version::ros2>,wrapper::action_cancel_any_goal_name>,
            wrapper::native_function<&host::action_receive_feedback<core::version::ros2>,wrapper::action_receive_feedback_name>,
            wrapper::native_function<&host::action_receive_result<core::version::ros2>,wrapper::action_receive_result_name>,
            wrapper::native_function<&host::action_advertise<core::version::ros2>,wrapper::action_advertise_name>,
            wrapper::native_function<&host::action_respond<core::version::ros2>,wrapper::action_respond_name>,
            wrapper::native_function<&host::tf_lookup<core::version::ros2>,wrapper::tf_lookup_name>,
            wrapper::native_function<&host::tf_broadcast<core::version::ros2>,wrapper::tf_broadcast_name>,
            wrapper::native_function<&host::parameter_get<core::version::ros2>,wrapper::parameter_get_name>,
            wrapper::native_function<&host::parameter_set<core::version::ros2>,wrapper::parameter_set_name>,
            wrapper::native_function<&host::parameter_list<core::version::ros2>,wrapper::parameter_list_name>,
            wrapper::native_function<&host::parameter_describe<core::version::ros2>,wrapper::parameter_describe_name>,
            wrapper::native_function<&host::parameter_declare<core::version::ros2>,wrapper::parameter_declare_name>,
            wrapper::native_function<&host::parameter_undeclare<core::version::ros2>,wrapper::parameter_undeclare_name>,
            wrapper::native_function<&host::graph_inspect_topic<core::version::ros2>,wrapper::graph_inspect_topic_name>,
            wrapper::native_function<&host::graph_inspect_node<core::version::ros2>,wrapper::graph_inspect_node_name>,
            wrapper::native_function<&host::graph_inspect_service<core::version::ros2>,wrapper::graph_inspect_service_name>,
            wrapper::native_function<&host::graph_inspect_action<core::version::ros2>,wrapper::graph_inspect_action_name>,
            wrapper::native_function<&host::parameter_subscribe_events<core::version::ros2>,wrapper::parameter_subscribe_events_name>,
            wrapper::native_function<&host::graph_list_topics<core::version::ros2>,wrapper::graph_list_topics_name>,
            wrapper::native_function<&host::graph_list_nodes<core::version::ros2>,wrapper::graph_list_nodes_name>,
            wrapper::native_function<&host::graph_list_services<core::version::ros2>,wrapper::graph_list_services_name>,
            wrapper::native_function<&host::lifecycle_get_state<core::version::ros2>,wrapper::lifecycle_get_state_name>,
            wrapper::native_function<&host::lifecycle_change_state<core::version::ros2>,wrapper::lifecycle_change_state_name>,
            wrapper::native_function<&host::time_read_ros<core::version::ros2>,wrapper::time_read_ros_name>,
            wrapper::native_function<&host::time_read_system<core::version::ros2>,wrapper::time_read_system_name>,
            wrapper::native_function<&host::time_read_steady<core::version::ros2>,wrapper::time_read_steady_name>,
            wrapper::native_function<&host::log_write<core::version::ros2>,wrapper::log_write_name>
        >;
    };
    inline constexpr ros2_local_imported_module_t ros2_local_imported_module{};
    struct ros2_rmw_local_imported_module_t
    {
        inline static constexpr ::uwvm2::utils::container::u8string_view module_name{u8"uwvm_ros2_rmw_v1"};
        using local_function_tuple = ::uwvm2::utils::container::tuple<
            wrapper::native_function<&host::abi_version<core::version::ros2>,wrapper::abi_version_name>,
            wrapper::native_function<&host::rmw_publisher_activate,wrapper::rmw_publisher_activate_name>,
            wrapper::native_function<&host::rmw_publisher_inspect,wrapper::rmw_publisher_inspect_name>,
            wrapper::native_function<&host::rmw_publisher_publish,wrapper::rmw_publisher_publish_name>,
            wrapper::native_function<&host::rmw_publisher_retire,wrapper::rmw_publisher_retire_name>>;
    };
    inline constexpr ros2_rmw_local_imported_module_t ros2_rmw_local_imported_module{};
    static_assert(::uwvm2::uwvm::wasm::type::is_local_imported_module<ros2_rmw_local_imported_module_t>);
    static_assert(::uwvm2::uwvm::wasm::type::has_local_function_tuple<ros2_rmw_local_imported_module_t>);
    struct ros2_rmw_subscription_local_imported_module_t
    {
        inline static constexpr ::uwvm2::utils::container::u8string_view module_name{u8"uwvm_ros2_rmw_sub_v1"};
        using local_function_tuple = ::uwvm2::utils::container::tuple<
            wrapper::native_function<&host::abi_version<core::version::ros2>,wrapper::abi_version_name>,
            wrapper::native_function<&host::rmw_subscription_activate,wrapper::rmw_subscription_activate_name>,
            wrapper::native_function<&host::rmw_subscription_inspect,wrapper::rmw_subscription_inspect_name>,
            wrapper::native_function<&host::rmw_subscription_peek,wrapper::rmw_subscription_peek_name>,
            wrapper::native_function<&host::rmw_subscription_commit,wrapper::rmw_subscription_commit_name>,
            wrapper::native_function<&host::rmw_subscription_cancel,wrapper::rmw_subscription_cancel_name>,
            wrapper::native_function<&host::rmw_subscription_retire,wrapper::rmw_subscription_retire_name>>;
    };
    inline constexpr ros2_rmw_subscription_local_imported_module_t ros2_rmw_subscription_local_imported_module{};
    static_assert(::uwvm2::uwvm::wasm::type::is_local_imported_module<ros2_rmw_subscription_local_imported_module_t>);
    static_assert(::uwvm2::uwvm::wasm::type::has_local_function_tuple<ros2_rmw_subscription_local_imported_module_t>);
    struct ros2_rmw_service_local_imported_module_t
    {
        inline static constexpr ::uwvm2::utils::container::u8string_view module_name{u8"uwvm_ros2_rmw_srv_v1"};
        using local_function_tuple = ::uwvm2::utils::container::tuple<
            wrapper::native_function<&host::abi_version<core::version::ros2>,wrapper::abi_version_name>,
            wrapper::native_function<&host::rmw_client_activate,wrapper::rmw_srv_client_activate_name>,
            wrapper::native_function<&host::rmw_service_activate,wrapper::rmw_srv_service_activate_name>,
            wrapper::native_function<&host::rmw_service_inspect,wrapper::rmw_srv_endpoint_inspect_name>,
            wrapper::native_function<&host::rmw_client_submit,wrapper::rmw_srv_client_submit_name>,
            wrapper::native_function<&host::rmw_client_abandon,wrapper::rmw_srv_client_abandon_name>,
            wrapper::native_function<&host::rmw_service_respond,wrapper::rmw_srv_service_respond_name>,
            wrapper::native_function<&host::rmw_service_peek,wrapper::rmw_srv_endpoint_peek_name>,
            wrapper::native_function<&host::rmw_service_commit,wrapper::rmw_srv_endpoint_commit_name>,
            wrapper::native_function<&host::rmw_service_cancel,wrapper::rmw_srv_endpoint_cancel_name>,
            wrapper::native_function<&host::rmw_client_available,wrapper::rmw_srv_client_available_name>,
            wrapper::native_function<&host::rmw_service_retire,wrapper::rmw_srv_endpoint_retire_name>>;
    };
    inline constexpr ros2_rmw_service_local_imported_module_t ros2_rmw_service_local_imported_module{};
    static_assert(::uwvm2::uwvm::wasm::type::is_local_imported_module<ros2_rmw_service_local_imported_module_t>);
    static_assert(::uwvm2::uwvm::wasm::type::has_local_function_tuple<ros2_rmw_service_local_imported_module_t>);
    struct ros2_client_local_imported_module_t
    {
        inline static constexpr ::uwvm2::utils::container::u8string_view module_name{u8"uwvm_ros2_client_v1"};
        using local_function_tuple = ::uwvm2::utils::container::tuple<
            wrapper::native_function<&host::abi_version<core::version::ros2>,wrapper::abi_version_name>,
            wrapper::native_function<&host::client_features<core::version::ros2>,wrapper::client_features_name>,
            wrapper::native_function<&host::client_service_available<core::version::ros2>,wrapper::client_service_available_name>>;
    };
    inline constexpr ros2_client_local_imported_module_t ros2_client_local_imported_module{};
    static_assert(::uwvm2::uwvm::wasm::type::is_local_imported_module<ros2_client_local_imported_module_t>);
    static_assert(::uwvm2::uwvm::wasm::type::has_local_function_tuple<ros2_client_local_imported_module_t>);
    static_assert(::uwvm2::uwvm::wasm::type::is_local_imported_module<ros2_local_imported_module_t>);
    static_assert(::uwvm2::uwvm::wasm::type::has_local_function_tuple<ros2_local_imported_module_t>);
#endif
    // Read-only DBG identity issuer, not a namespace/name allowlist. The exact
    // seven immutable Concrete types are the entire construction surface.
    extern "C++"
    {
        class sealed_debug_loader_factory final
        {
            sealed_debug_loader_factory()=delete;
            template<class T> static constexpr bool known = false
#if defined(UWVM_IMPORT_ROS1)
                || ::std::is_same_v<T,ros1_local_imported_module_t>
                || ::std::is_same_v<T,ros1_client_local_imported_module_t>
#endif
#if defined(UWVM_IMPORT_ROS2)
                || ::std::is_same_v<T,ros2_local_imported_module_t>
                || ::std::is_same_v<T,ros2_client_local_imported_module_t>
                || ::std::is_same_v<T,ros2_rmw_local_imported_module_t>
                || ::std::is_same_v<T,ros2_rmw_subscription_local_imported_module_t>
                || ::std::is_same_v<T,ros2_rmw_service_local_imported_module_t>
#endif
                ;
        public:
            template<class T> requires known<::std::remove_cvref_t<T>>
            [[nodiscard]] static bool append() noexcept
            {
                using C=::std::remove_cvref_t<T>;static_assert(::std::is_empty_v<C>);
                namespace identity=::uwvm2::uwvm::runtime::full;
                constexpr auto kind=[]() noexcept {
#if defined(UWVM_IMPORT_ROS1)
                    if constexpr(::std::is_same_v<C,ros1_local_imported_module_t>)return identity::builtin_ros_kind::ros1;
                    else if constexpr(::std::is_same_v<C,ros1_client_local_imported_module_t>)return identity::builtin_ros_kind::ros1_client;
                    else
#endif
#if defined(UWVM_IMPORT_ROS2)
                    if constexpr(::std::is_same_v<C,ros2_local_imported_module_t>)return identity::builtin_ros_kind::ros2;
                    else if constexpr(::std::is_same_v<C,ros2_client_local_imported_module_t>)return identity::builtin_ros_kind::ros2_client;
                    else if constexpr(::std::is_same_v<C,ros2_rmw_local_imported_module_t>)return identity::builtin_ros_kind::ros2_publisher;
                    else if constexpr(::std::is_same_v<C,ros2_rmw_subscription_local_imported_module_t>)return identity::builtin_ros_kind::ros2_subscription;
                    else
#endif
                    return identity::builtin_ros_kind::ros2_service;
                }();
                return identity::builtin_ros_loader_identity::append_typed<C>(kind);
            }
        };
        // ADL enables the existing threaded factory to reuse THIS insertion,
        // without importing ROS Host back into the runtime storage owner.
        template<class C> requires requires {sealed_debug_loader_factory::append<C>();}
        [[nodiscard]] inline bool append_sealed_ros_builtin(C const&) noexcept
        {return sealed_debug_loader_factory::append<C>();}
    }

}
#include "debug_bridge.h"
#include "rmw_publisher_host.h"
#include "rmw_subscription_host.h"
#include "rmw_service_host.h"
#endif
