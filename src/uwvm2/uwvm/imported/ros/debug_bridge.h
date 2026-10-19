/*************************************************************
 * UWVM2 ROS Host read-only debugger bridge. APL-2.0.       *
 *************************************************************/
#pragma once
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
#include "host.h"
namespace uwvm2::uwvm::imported::ros::host
{
    inline void debug_snapshot(core::debug::request const& query,core::debug::view& output) noexcept
    {
        using namespace core::debug;
        if(query.selected==selection::trace) {debug_trace.read(query,output);return;}
        if(query.selected==selection::calls) {debug_trace.read_calls(query,output);return;}
        bool const sessions{query.selected==selection::sessions||query.selected==selection::statistics};
        bool const queues{query.selected==selection::queues||query.selected==selection::statistics};
        if((!sessions&&!queues)||query.selected==selection::statistics)
        {capabilities.debug_snapshot(query,output);if(output.result!=status::ok) {return;}if(!sessions&&!queues) {return;}}
        auto fail=[&](status reason) {output={};output.selected=query.selected;output.result=reason;};
        std::unique_lock registry{mutex,std::try_to_lock};if(!registry.owns_lock()) {fail(status::busy);return;}
        std::size_t scanned{};
        for(auto const& entry:instances)
        {
            if(++scanned>maximum_scan) {fail(status::resource_limit);return;}
            for(unsigned index{};index!=2;++index)
            {
                auto version{static_cast<core::version>(index+1)};
                if(query.backend&&query.backend!=index+1) {continue;}
                auto& backend{entry.second->backends[index]};
                std::unique_lock lease{backend.mutex,std::try_to_lock};if(!lease.owns_lock()) {fail(status::busy);return;}
                trace_record label{};
                if(!capabilities.debug_identity(entry.first,entry.second->generation,version,{},label)) {fail(status::busy);return;}
                if((query.instance&&query.instance!=label.instance)||
                   (query.module.size&&(label.module.truncated||query.module.view()!=label.module.view()))) {continue;}
                row value{};value.backend=index+1;value.instance=label.instance;value.generation=label.generation;value.module=label.module;
                value.enabled=backend.target&&backend.target->enabled;value.connected=backend.connected;
                value.alive=!entry.second->retired;value.retired=entry.second->retired;
                value.timeout_ms=backend.timeout;value.observations=backend.observations.size();
                value.client_bindings=backend.availability_history.rows().size();
                value.client_session_label=value.client_bindings?backend.availability_debug_session:0;
                if(sessions&&matches(query,value.backend,value.instance,{},{},value.module.view()))
                {value.kind=row_kind::session;add(query,output,value);}
                if(!queues) {continue;}
                for(auto const& observation:backend.observations)
                {
                    if(++scanned>maximum_scan) {fail(status::resource_limit);return;}
                    auto const& watched{observation.second};
                    if(!matches(query,value.backend,value.instance,watched.observer.alias,watched.source.operation,value.module.view())) {continue;}
                    core::ipc::event_queue_statistics current{};
                    if(!watched.local||!watched.local->try_statistics(current)) {fail(status::busy);return;}
                    value.kind=row_kind::queue;value.alias.assign(watched.observer.alias);value.operation.assign(watched.source.operation);
                    value.type.assign(watched.source.type);value.retired=watched.retired;value.requests=watched.requests.size();value.delivered=watched.delivered;
                    value.resource={};value.resource_redacted=!public_resource(watched.source.resource);
                    if(!value.resource_redacted) {value.resource.assign(watched.source.resource);}
#define UWVM_ROS_DEBUG_COPY_STAT(name) value.name=current.name
                    UWVM_ROS_DEBUG_COPY_STAT(depth);UWVM_ROS_DEBUG_COPY_STAT(bytes);
                    UWVM_ROS_DEBUG_COPY_STAT(pending_depth);UWVM_ROS_DEBUG_COPY_STAT(pending_bytes);
                    UWVM_ROS_DEBUG_COPY_STAT(reserved_depth);UWVM_ROS_DEBUG_COPY_STAT(reserved_bytes);
                    UWVM_ROS_DEBUG_COPY_STAT(high_water_depth);UWVM_ROS_DEBUG_COPY_STAT(high_water_bytes);
                    UWVM_ROS_DEBUG_COPY_STAT(enqueued);UWVM_ROS_DEBUG_COPY_STAT(dequeued);
                    UWVM_ROS_DEBUG_COPY_STAT(dropped_oldest);UWVM_ROS_DEBUG_COPY_STAT(dropped_newest);
                    UWVM_ROS_DEBUG_COPY_STAT(rejected);UWVM_ROS_DEBUG_COPY_STAT(backpressured);
                    UWVM_ROS_DEBUG_COPY_STAT(admission_refused);UWVM_ROS_DEBUG_COPY_STAT(completed);UWVM_ROS_DEBUG_COPY_STAT(closed);
#undef UWVM_ROS_DEBUG_COPY_STAT
                    add(query,output,value);
                }
            }
        }
        output.result=status::ok;
    }
    // The callback has process lifetime and carries no owner pointer. This
    // guard retires it before the Host's registries are destroyed at exit.
    struct debug_hook_registration
    {
        debug_hook_registration() noexcept
        {
            core::debug::snapshot_hook.store(&debug_snapshot,std::memory_order_release);
            core::management::retirement_hook.store(&retire_authority,std::memory_order_release);
        }
        ~debug_hook_registration()
        {
            auto expected{&debug_snapshot};
            core::debug::snapshot_hook.compare_exchange_strong(expected,nullptr,std::memory_order_acq_rel);
            auto expected_retirement{&retire_authority};
            core::management::retirement_hook.compare_exchange_strong(expected_retirement,nullptr,std::memory_order_acq_rel);
        }
    };
    inline debug_hook_registration debug_hook{};
}
#endif
