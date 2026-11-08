// SPDX-License-Identifier: Apache-2.0
#pragma once
#if defined(UWVM_IMPORT_ROS2)
#include <uwvm2/imported/ros/portable/memory.h>
#include <uwvm2/imported/ros/portable/native_client.h>
#include <uwvm2/imported/ros/portable/runtime_audit.h>
#include <uwvm2/runtime/lib/uwvm_runtime.h>
#include <uwvm2/uwvm/runtime/storage/builtin_ros_loader.h>
#include <mutex>
#include <uwvm2/utils/debug/host_calls.h>

namespace uwvm2::uwvm::imported::ros::portable_host
{
    namespace model=::uwvm2::imported::ros::portable;
    namespace api=::uwvm2::runtime::lib;
    using model::error;
    inline bool observed_read(::std::uint32_t memory,::std::uint64_t offset,void* bytes,::std::size_t size) noexcept
    {
        bool ok=api::preload_memory_read_host_api(memory,offset,bytes,size);
        if(ok) {::uwvm2::utils::debug::host_calls::capture(memory,offset,bytes,size,false);}
        return ok;
    }
    inline bool observed_write(::std::uint32_t memory,::std::uint64_t offset,void const* bytes,::std::size_t size) noexcept
    {
        bool ok=api::preload_memory_write_host_api(memory,offset,bytes,size);
        if(ok) {::uwvm2::utils::debug::host_calls::capture(memory,offset,bytes,size,true);}
        return ok;
    }
    struct owner_state
    {
        model::owner_key owner{};
        model::deployment_selection::owner deployment{};
        model::operation_binding binding{};
        model::joint_native_client native{};
        model::command_state command{};
        model::state latest{};
        ::std::array<::std::uint8_t,16> native_goal_uuid{};
        ::std::uint64_t handle{},command_id{},deadline_ns{};
        bool opened{},closed{},retired{};
    };
    // One explicitly selected deployment may bind one actual runtime owner.
    // Retain its tombstone and consumed command through close/retirement, so
    // pointer reuse, reopen or a rollback cannot silently reissue a goal.
    inline ::std::mutex registry_mutex{};
    inline ::std::unique_ptr<owner_state> selected_owner{};
    inline ::std::uint64_t next_handle{1};

    inline error caller(unsigned index,api::ros_caller_source_facts& facts,model::owner_key& owner,
                        model::deployment_selection::owner& deployment,model::runtime_audit::call_context* audit=nullptr) noexcept
    {
        if(!api::ros_caller_source_facts_host_api(facts)||facts.builtin_function_index!=index||
           facts.builtin_kind!=static_cast<unsigned>(::uwvm2::uwvm::runtime::full::builtin_ros_kind::portable_robot)) { return error::denied; }
        owner.instance=facts.caller.runtime_instance;owner.generation=facts.caller.generation;
        for(::std::size_t i{};i!=owner.wasm_sha256.size();++i) { owner.wasm_sha256[i]=::std::to_integer<::std::uint8_t>(facts.whole_wasm_sha256[i]); }
        ::std::string_view module{reinterpret_cast<char const*>(facts.caller.module_name.data()),facts.caller.module_name.size()};
        deployment=model::deployment_selection::current();
        bool const permitted{deployment&&owner.wasm_sha256==deployment->wasm_sha256&&module==deployment->module};
        if(model::runtime_audit::runtime.enabled())
        {
            // Identity comes from the sealed active import and actual source
            // owner. Observing an absent/mismatched grant creates no owner.
            auto call{model::runtime_audit::runtime.enter_call(owner.instance,owner.generation,owner.wasm_sha256,
                index,deployment.get(),permitted)};
            if(audit) { *audit=call; }
            if(!permitted) { (void)model::runtime_audit::runtime.emit_binding(call); }
        }
        if(!permitted) { return error::denied; }
        return error::ok;
    }
    inline model::memory_shape memory(api::ros_caller_source_facts const& facts) noexcept
    { return {facts.memory_bytes,1,0,false}; }
    inline bool write_word(::std::uint64_t offset,::std::uint64_t value) noexcept
    { ::std::array<::std::uint8_t,8> bytes{};model::wire_detail::put(bytes,0,value,8);return observed_write(0,offset,bytes.data(),bytes.size()); }
    // Canonical offsets are only for range/alias preflight. The eventual copy
    // must retain the original tagged pointer so the actual owner rechecks it.
    inline bool normalized_span(model::memory_span raw,model::memory_span& normalized) noexcept
    {
        normalized={};
        if(raw.bytes>(::std::numeric_limits<::std::size_t>::max)()) { return false; }
        ::std::uint64_t offset{};
        if(!api::preload_memory_normalize_range_host_api(0,raw.offset,static_cast<::std::size_t>(raw.bytes),offset)) { return false; }
        normalized={offset,raw.bytes};return true;
    }
    inline error open_preflight(api::ros_caller_source_facts const& facts,::std::uint32_t selector,
        ::std::uint64_t alias_at,::std::uint64_t alias_bytes,::std::uint64_t handle_out) noexcept
    {
        auto const shape{memory(facts)};
        if(auto result{model::valid_memory(shape,selector)};result!=error::ok) { return result; }
        if(alias_bytes==0u || alias_bytes>255u) { return error::malformed; }
        model::memory_span alias{},handle{};
        if(!normalized_span({alias_at,alias_bytes},alias) || !normalized_span({handle_out,8u},handle))
        { return error::malformed; }
        return model::preflight_open(shape,selector,alias,handle.offset);
    }
    inline error output_preflight(api::ros_caller_source_facts const& facts,::std::uint32_t selector,
        ::std::uint64_t offset,::std::uint64_t capacity,::std::uint64_t length,::std::size_t required,model::memory_span request={}) noexcept
    {
        auto const shape{memory(facts)};
        if(auto result{model::valid_memory(shape,selector)};result!=error::ok) { return result; }
        model::memory_span canonical_output{},canonical_length{},canonical_request{};
        if(!normalized_span({offset,capacity},canonical_output) || !normalized_span({length,8u},canonical_length) ||
           !normalized_span(request,canonical_request)) { return error::malformed; }
        auto e{model::preflight_output(shape,selector,canonical_output,canonical_length.offset,required,canonical_request)};
        if(e!=error::ok&&e!=error::too_small) { return e; }
        if(!observed_write(0,offset,nullptr,0)||!write_word(length,required)) { return error::malformed; }
        return e;
    }
    inline error admitted(::std::uint64_t handle,model::owner_key const& owner,model::deployment_selection::owner const& deployment) noexcept
    {
        if(!selected_owner||!selected_owner->opened||selected_owner->closed||selected_owner->retired||
           selected_owner->handle!=handle||selected_owner->owner!=owner) { return error::bad_handle; }
        if(selected_owner->deployment!=deployment||!selected_owner->native.connected()) { return error::stale; }
        return error::ok;
    }
    template<typename Function> inline ::std::uint32_t guarded(Function&& function) noexcept
    {
#if defined(UWVM_CPP_EXCEPTIONS)
        try { return static_cast<::std::uint32_t>(function()); }
        catch(...)
        {
            ::std::lock_guard lock{registry_mutex};
            if(selected_owner)
            {
                if(selected_owner->command.consumed()) { (void)selected_owner->command.mark_indeterminate(selected_owner->binding); }
                selected_owner->native.close();
            }
            return static_cast<::std::uint32_t>(error::unavailable);
        }
#else
        return static_cast<::std::uint32_t>(function());
#endif
    }
    inline ::std::uint32_t abi_version() noexcept { return 1u; }
    inline ::std::uint32_t open(::std::uint32_t selector,::std::uint64_t alias_at,::std::uint64_t alias_bytes,::std::uint64_t handle_out) noexcept
    {
        return guarded([&]() -> error {
            api::ros_caller_source_facts facts{};model::owner_key owner{};model::deployment_selection::owner deployment{};
            if(auto e{caller(1,facts,owner,deployment)};e!=error::ok) { return e; }
            if(auto e{open_preflight(facts,selector,alias_at,alias_bytes,handle_out)};e!=error::ok) { return e; }
            ::std::array<char,255> alias{};
            if(!observed_read(0,alias_at,alias.data(),static_cast<::std::size_t>(alias_bytes))||
               ::std::string_view{alias.data(),static_cast<::std::size_t>(alias_bytes)}!=deployment->alias||
               !observed_write(0,handle_out,nullptr,0)) { return error::denied; }
            ::std::lock_guard lock{registry_mutex};
            if(selected_owner||next_handle==UINT64_MAX) { return error::bad_handle; }
            selected_owner=::std::make_unique<owner_state>();auto& state{*selected_owner};
            state.owner=owner;state.deployment=deployment;state.handle=next_handle++;
            state.binding={owner,deployment->mapping.epoch,deployment->policy_epoch,deployment->native_generation,
                           deployment->adapter_sha256,deployment->adapter_sha256,deployment->native_provenance};
            auto now{model::monotonic_ns()};if(!now||now>UINT64_MAX-model::command_state::maximum_wait_ns) { state.closed=true;return error::deadline; }
            auto e{state.native.connect(*deployment,owner,state.handle,now+model::command_state::maximum_wait_ns)};
            if(e!=error::ok) { state.closed=true;return e; }
            state.opened=true;
            if(!write_word(handle_out,state.handle)) { state.closed=true;state.native.close();return error::malformed; }
            return error::ok;
        });
    }
    inline ::std::uint32_t describe(::std::uint64_t handle,::std::uint32_t selector,::std::uint64_t output,::std::uint64_t capacity,::std::uint64_t length) noexcept
    {
        return guarded([&]() -> error {
            api::ros_caller_source_facts facts{};model::owner_key owner{};model::deployment_selection::owner deployment{};
            if(auto e{caller(2,facts,owner,deployment)};e!=error::ok) { return e; }
            ::std::lock_guard lock{registry_mutex};if(auto e{admitted(handle,owner,deployment)};e!=error::ok) { return e; }
            auto required{64u+40u*deployment->mapping.count};
            if(auto e{output_preflight(facts,selector,output,capacity,length,required)};e!=error::ok) { return e; }
            ::std::array<::std::uint8_t,704> bytes{};auto encoded{model::encode(deployment->mapping,bytes)};
            return encoded.status==error::ok&&observed_write(0,output,bytes.data(),encoded.bytes)?error::ok:error::malformed;
        });
    }
    inline ::std::uint32_t read_state(::std::uint64_t handle,::std::uint32_t selector,::std::uint64_t output,::std::uint64_t capacity,::std::uint64_t length) noexcept
    {
        return guarded([&]() -> error {
            api::ros_caller_source_facts facts{};model::owner_key owner{};model::deployment_selection::owner deployment{};
            model::runtime_audit::call_context audit_call{};
            if(auto e{caller(3,facts,owner,deployment,&audit_call)};e!=error::ok) { return e; }
            ::std::lock_guard lock{registry_mutex};if(auto e{admitted(handle,owner,deployment)};e!=error::ok) { return e; }
            auto required{64u+16u*deployment->mapping.count};
            if(auto e{output_preflight(facts,selector,output,capacity,length,required)};e!=error::ok) { return e; }
            auto& state{*selected_owner};auto now{model::monotonic_ns()};if(!now||now>UINT64_MAX-500000000u) { return error::deadline; }
            ::std::vector<::std::uint8_t> response{};
            if(auto e{state.native.request(model::native_operation::read_state,{},response,now+500000000u)};e!=error::ok) { return e; }
            model::state sample{};now=model::monotonic_ns();
            if(!model::decode(response,sample)||sample.epoch!=deployment->mapping.epoch||sample.count!=deployment->mapping.count||
               sample.sequence<state.latest.sequence||sample.observed_ns>now||now>=sample.valid_until_ns||
               sample.valid_until_ns-sample.observed_ns>250000000u) { return error::no_fresh_state; }
            for(::std::uint32_t i{};i!=sample.count;++i)
            { auto const& bound{deployment->mapping.joints[i]};if(sample.positions[i]<bound.lower||sample.positions[i]>bound.upper) { return error::unsupported; } }
            if(!observed_write(0,output,response.data(),response.size())) { return error::malformed; }
            state.latest=sample;
            if(model::runtime_audit::runtime.enabled())
            {
                // Only successful real copies can produce delivery evidence.
                // Snapshot observation must not change the import result, retry
                // a command, or manufacture a baseline after a consumed command.
                model::receipt command{};
                if(!state.command.consumed())
                { (void)model::runtime_audit::runtime.emit_state_delivery(audit_call,*deployment,handle,output,response); }
                else if(state.command.snapshot(state.binding,model::monotonic_ns(),command)==error::ok)
                { (void)model::runtime_audit::runtime.emit_state_delivery(audit_call,*deployment,handle,output,response,&command,&state.native_goal_uuid); }
            }
            return error::ok;
        });
    }
    inline ::std::uint32_t submit(::std::uint64_t handle,::std::uint32_t selector,::std::uint64_t input,::std::uint64_t input_bytes,
                                  ::std::uint64_t output,::std::uint64_t capacity,::std::uint64_t length) noexcept
    {
        return guarded([&]() -> error {
            api::ros_caller_source_facts facts{};model::owner_key owner{};model::deployment_selection::owner deployment{};
            model::runtime_audit::call_context audit_call{};
            if(auto e{caller(4,facts,owner,deployment,&audit_call)};e!=error::ok) { return e; }
            ::std::lock_guard lock{registry_mutex};if(auto e{admitted(handle,owner,deployment)};e!=error::ok) { return e; }
            if(input_bytes!=80u+16u*deployment->mapping.count) { return error::malformed; }
            if(auto e{output_preflight(facts,selector,output,capacity,length,96,{input,input_bytes})};e!=error::ok) { return e; }
            ::std::array<::std::uint8_t,336> bytes{};auto size{static_cast<::std::size_t>(input_bytes)};
            if(!observed_read(0,input,bytes.data(),size)) { return error::malformed; }
            model::command request{};if(!model::decode({bytes.data(),size},request)) { return error::malformed; }
            auto& state{*selected_owner};auto now{model::monotonic_ns()};auto hash{model::hash_bytes({reinterpret_cast<char const*>(bytes.data()),size})};
            if(auto e{state.command.reserve(state.binding,request,deployment->mapping,state.latest,hash,now,model::command_state::maximum_wait_ns)};e!=error::ok) { return e; }
            state.command_id=request.id;state.deadline_ns=now+model::command_state::maximum_wait_ns;
            // Reservation survives every exception, socket error, late reply
            // and failed output copy. There is no command retry or re-open.
            ::std::vector<::std::uint8_t> response{};
            if(auto e{state.native.request(model::native_operation::submit,{bytes.data(),size},response,state.deadline_ns)};e!=error::ok)
            { (void)state.command.mark_indeterminate(state.binding);state.native.close();return e; }
            model::native_event accepted{};now=model::monotonic_ns();
            if(!model::decode_native_event(response,accepted)||accepted.result!=model::outcome::accepted||accepted.status||accepted.observed_ns<state.latest.observed_ns||accepted.observed_ns>now)
            { (void)state.command.mark_indeterminate(state.binding);state.native.close();return error::malformed; }
            state.native_goal_uuid=accepted.goal_uuid;
            if(auto e{state.command.accepted(state.binding,1u,accepted.observed_ns)};e!=error::ok) { state.native.close();return e; }
            model::receipt receipt{};if(auto e{state.command.snapshot(state.binding,now,receipt)};e!=error::ok) { return e; }
            ::std::array<::std::uint8_t,96> encoded{};auto result{model::encode(receipt,encoded)};
            if(result.status!=error::ok||!observed_write(0,output,encoded.data(),encoded.size()))
            { (void)state.command.mark_indeterminate(state.binding);state.native.close();return error::malformed; }
            if(model::runtime_audit::runtime.enabled())
            {
                (void)model::runtime_audit::runtime.emit(*deployment,
                    {model::runtime_audit::event::vm_goal_accepted,owner.generation,handle,receipt.command_id,
                     accepted.observed_ns,receipt.observed_sequence,accepted.goal_uuid,receipt.result,receipt.native_status,receipt.command_sha256});
                (void)model::runtime_audit::runtime.emit_binding(audit_call,&receipt.command_sha256,&accepted.goal_uuid);
            }
            return error::ok;
        });
    }
    inline ::std::uint32_t wait(::std::uint64_t handle,::std::uint64_t command_id,::std::uint32_t selector,
                               ::std::uint64_t output,::std::uint64_t capacity,::std::uint64_t length) noexcept
    {
        return guarded([&]() -> error {
            api::ros_caller_source_facts facts{};model::owner_key owner{};model::deployment_selection::owner deployment{};
            if(auto e{caller(5,facts,owner,deployment)};e!=error::ok) { return e; }
            ::std::lock_guard lock{registry_mutex};if(auto e{admitted(handle,owner,deployment)};e!=error::ok) { return e; }
            auto& state{*selected_owner};if(!command_id||command_id!=state.command_id||state.command.current_phase()!=model::command_state::phase::accepted) { return error::bad_handle; }
            if(auto e{output_preflight(facts,selector,output,capacity,length,96)};e!=error::ok) { return e; }
            ::std::array<::std::uint8_t,24> request{};::std::copy(state.native_goal_uuid.begin(),state.native_goal_uuid.end(),request.begin());model::wire_detail::put(request,16,command_id,8);
            ::std::vector<::std::uint8_t> response{};
            if(auto e{state.native.request(model::native_operation::wait,request,response,state.deadline_ns)};e!=error::ok)
            { (void)state.command.mark_indeterminate(state.binding);state.native.close();return e; }
            model::native_event event{};
            if(response.size()<32u||!model::decode_native_event({response.data(),32},event)||event.goal_uuid!=state.native_goal_uuid||event.result==model::outcome::accepted)
            { (void)state.command.mark_indeterminate(state.binding);state.native.close();return error::malformed; }
            model::state confirmation{};
            if(event.result==model::outcome::succeeded)
            {
                if(!model::decode({response.data()+32,response.size()-32},confirmation)||confirmation.valid_until_ns-confirmation.observed_ns>250000000u)
                { (void)state.command.mark_indeterminate(state.binding);state.native.close();return error::malformed; }
            }
            else if(response.size()!=32u)
            { (void)state.command.mark_indeterminate(state.binding);state.native.close();return error::malformed; }
            auto result{event.result==model::outcome::succeeded?model::native_terminal::succeeded:event.result==model::outcome::cancelled?model::native_terminal::cancelled:model::native_terminal::rejected};
            auto now{model::monotonic_ns()};
            if(auto e{state.command.finish(state.binding,{1u,event.observed_ns,result,event.status},confirmation,now)};e!=error::ok)
            { (void)state.command.mark_indeterminate(state.binding);state.native.close();return e; }
            model::receipt receipt{};if(auto e{state.command.snapshot(state.binding,now,receipt)};e!=error::ok) { return e; }
            ::std::array<::std::uint8_t,96> encoded{};auto e{model::encode(receipt,encoded)};
            if(e.status!=error::ok||!observed_write(0,output,encoded.data(),encoded.size())) { return error::malformed; }
            if(model::runtime_audit::runtime.enabled())
            {
                (void)model::runtime_audit::runtime.emit(*deployment,
                    {model::runtime_audit::event::vm_terminal,owner.generation,handle,receipt.command_id,
                     event.observed_ns,receipt.observed_sequence,event.goal_uuid,receipt.result,receipt.native_status,receipt.command_sha256});
            }
            return error::ok;
        });
    }
    inline ::std::uint32_t close(::std::uint64_t handle) noexcept
    {
        return guarded([&]() -> error {
            api::ros_caller_source_facts facts{};model::owner_key owner{};model::deployment_selection::owner deployment{};
            if(auto e{caller(6,facts,owner,deployment)};e!=error::ok) { return e; }
            ::std::lock_guard lock{registry_mutex};
            if(!selected_owner||selected_owner->owner!=owner||selected_owner->handle!=handle||selected_owner->closed) { return error::bad_handle; }
            selected_owner->closed=true;selected_owner->native.close();
            if(selected_owner->command.consumed()) { (void)selected_owner->command.close(owner); }return error::ok;
        });
    }
    inline void retire(void const* object,::std::uint64_t generation,bool all) noexcept
    {
        ::std::lock_guard lock{registry_mutex};
        if(!selected_owner||(!all&&(selected_owner->owner.instance!=object||selected_owner->owner.generation!=generation))) { return; }
        selected_owner->retired=true;selected_owner->native.close();
        if(selected_owner->command.consumed()) { (void)selected_owner->command.revoke(selected_owner->owner); }
    }
}
#endif
