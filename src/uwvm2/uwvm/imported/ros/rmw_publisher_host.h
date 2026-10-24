/** @file
 * Actual Host operations for additive owned native endpoint namespaces.
 * Include through host.h. The existing Runtime caller guard supplies the true
 * module/instance/generation; no Guest argument selects a management identity.
 */
#pragma once
#include "host.h"
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/ipc/rmw_publish.h>
#endif

namespace uwvm2::uwvm::imported::ros::host
{
    namespace rmw_wire=core::ipc::rmw_endpoint;
    inline void abandon_rmw_transport(session& backend,::std::uint32_t reason) noexcept
    {
        auto native_session{backend.connection.session_id()};
        // connection.session_id() can already be0 after a transport failure.
        // The retained old session, not0, owns every historical cleanup row.
        if(!native_session){native_session=backend.availability_native_session;}
        backend.rmw_endpoints.retire_session(native_session,reason);
        backend.retire_service_session(native_session,reason);
        backend.connection.close();backend.connected=false;backend.retire_observations(reason);
    }
    struct rmw_rpc_guard
    {
        session& backend;bool armed{};::std::uint32_t reason{13};
        ~rmw_rpc_guard(){if(armed){abandon_rmw_transport(backend,reason);}}
    };
    inline ::std::uint32_t rmw_remaining(::std::chrono::steady_clock::time_point until) noexcept
    {
        auto remaining{::std::chrono::duration_cast<::std::chrono::milliseconds>(until-::std::chrono::steady_clock::now()).count()};
        return remaining>0?static_cast<::std::uint32_t>(::std::min<::std::uint64_t>(remaining,core::ipc::max_timeout_ms)):0u;
    }
    /** One row's private cleanup. The caller tombstones ALL matching rows
     * before entering here, then claims this row exactly once. A disconnected
     * old Native session records107; it is never replaced by a new connection.
     * This trusted route does not reauthorize an auxiliary already revoked.
     */
    inline ::std::uint32_t rmw_cleanup_endpoint(session& backend,core::rmw_endpoint_history::entry const& row,
        ::std::chrono::steady_clock::time_point until) noexcept
    {
#if defined(__cpp_exceptions)
        try
#endif
        {
            auto receipt{row.observed};auto budget{rmw_remaining(until)};
            if(!backend.connected||!backend.connection.session_id()||receipt.owner.session!=backend.connection.session_id()){return 107;}
            if(!budget){return 110;}
            if(core::service_endpoint_role(receipt.kind))
            {
                core::rmw_service_bindings::entry const* pair{};
                if(auto error{backend.rmw_service_pairs.lookup(receipt.owner,receipt.kind,pair)}){return error;}
                core::ipc::rmw_service::control request{};request.operation=core::ipc::rmw_service::command::retire;
                request.kind=receipt.kind;request.owner=receipt.owner;request.incarnation=receipt.incarnation;request.peer=pair->peer;
                ::std::array<::std::uint8_t,core::ipc::rmw_service::header_bytes> raw{};
                if(!core::ipc::rmw_service::encode(request,false,raw)){return 22;}
                auto result{backend.connection.call("rmw.service-retire-v1",pair->primary_alias,{raw.begin(),raw.end()},budget)};
                core::ipc::rmw_service::control reply{};rmw_wire::packet observed{};
                if(result.error){return result.payload.empty()?result.error:13u;}
                if(!core::ipc::rmw_service::associated_endpoint(result.payload.data(),result.payload.size(),request,reply,observed)){return 13;}
                return rmw_remaining(until)?0u:110u;
            }
            receipt.operation=rmw_wire::command::retire;receipt.profiles=receipt.gid_size=0;receipt.actual={};receipt.gid.fill(0);
            ::std::array<::std::uint8_t,rmw_wire::packet_bytes> raw{};
            if(!rmw_wire::encode(receipt,false,raw)){return 22;}
            auto action{row.observed.kind==rmw_wire::role::publisher?"rmw.endpoint-retire-v1":
                row.observed.kind==rmw_wire::role::subscription?"rmw.subscription-endpoint-retire-v1":nullptr};
            if(!action){return 95;}
            auto result{backend.connection.call(action,row.alias,{raw.begin(),raw.end()},budget)};
            rmw_wire::packet reply{};
            if(result.error){return result.payload.empty()?result.error:13u;}
            return rmw_wire::associated_response(result.payload.data(),result.payload.size(),receipt,reply)?0u:13u;
        }
#if defined(__cpp_exceptions)
        catch(::std::bad_alloc const&){return 12;}catch(...){return 5;}
#endif
    }
    inline ::std::uint32_t retire_rmw_endpoints(session& backend,::std::uint64_t handle,
        ::std::string_view alias,bool revoke,::std::uint64_t instance,::std::uint64_t generation) noexcept
    {
        // ONE entry deadline covers every local scan and every native retire.
        // No per-row timeout multiplication, quota, allocation or native error
        // can stop the first allocation-free authority tombstone sweep.
        auto until{::std::chrono::steady_clock::now()+::std::chrono::milliseconds(backend.timeout)};
        auto matches=[&](core::rmw_endpoint_history::entry const& row) noexcept
        {
            if(!core::service_endpoint_role(row.observed.kind))
            {return row.observed.owner.source==handle||(revoke&&row.alias==alias);}
            core::rmw_service_bindings::entry const* pair{};
            if(backend.rmw_service_pairs.lookup(row.observed.owner,row.observed.kind,pair)){return false;}
            auto domain{pair->owner};domain.instance=instance;domain.generation=generation;
            return core::rmw_service_bindings::matches_peer(*pair,domain,handle,alias,revoke);
        };
        for(auto const& item:backend.rmw_service_pairs.rows())
        {
            auto domain{item.second.owner};domain.instance=instance;domain.generation=generation;
            if(core::rmw_service_bindings::matches_peer(item.second,domain,handle,alias,revoke))
            {(void)backend.rmw_service_pairs.retire(item.second.owner,item.second.kind,125);backend.rmw_endpoints.abandon(item.first,125);}
        }
        for(auto const& item:backend.rmw_endpoints.rows())
        {if(matches(item.second)){backend.rmw_endpoints.abandon(item.first,125);}}
        // Query authority has no independently created native client here.
        // Retire this local query table, never send a legacy availability retire
        // to the wrong native registry or retire a still-authorized call owner.
        namespace availability=core::ipc::client_availability;
        for(auto scope:{revoke?availability::retirement::availability_alias:availability::retirement::availability_handle,
            revoke?availability::retirement::source_alias:availability::retirement::source_handle})
        {
            auto row{backend.rmw_service_queries.retirement_reference(handle,alias,scope)};
            if(row){(void)backend.rmw_service_queries.retire(row->binding,scope);}
        }
        ::std::uint32_t first{};
        for(auto const& item:backend.rmw_endpoints.rows())
        {
            auto const& row{item.second};
            if(!matches(row)||!backend.rmw_endpoints.claim_cleanup(item.first)){continue;}
            auto journal{backend.rmw_service_journals.find(item.first)};
            if(journal!=backend.rmw_service_journals.end()){journal->second.cleaned(115);}
            // Each helper catches its OWN allocation/transport failure. Every
            // remaining claimed historical row still receives an explicit
            // independent107/110 result even after the first connection loss.
            auto error{rmw_cleanup_endpoint(backend,row,until)};
            backend.rmw_endpoints.cleanup_result(item.first,error);
            if(journal!=backend.rmw_service_journals.end()){journal->second.cleaned(error);}
            if(error&&!first){first=error;}
            if(error){abandon_rmw_transport(backend,error);}
        }
        return first;
    }
    inline ::std::uint32_t rmw_endpoint_control(::std::uint64_t handle,::std::uint64_t incarnation,
        ::std::uint32_t memory,::std::uint64_t output,rmw_wire::command command,rmw_wire::role kind) noexcept
    {
        constexpr auto version{core::version::ros2};auto entered{::std::chrono::steady_clock::now()};
        bool const publisher{kind==rmw_wire::role::publisher};
        if(!publisher&&kind!=rmw_wire::role::subscription){return audit_denial(version,95);}
#if defined(__cpp_exceptions)
        try
#endif
        {
            runtime::ros_caller_identity caller{};::std::shared_ptr<instance> state{};
            if(auto error{identify(version, caller, state)}){return audit_denial(version,error);}
            auto& backend{state->backends[1]};::std::unique_lock lock{backend.mutex,::std::try_to_lock};
            if(!lock.owns_lock()){return audit_denial(version,11);}
            if(command!=rmw_wire::command::retire&&!memory_range(memory,output,rmw_wire::packet_bytes)){return audit_denial(version,22);}
            if((command==rmw_wire::command::activate&&incarnation)||(command!=rmw_wire::command::activate&&!incarnation)){return audit_denial(version,22);}
            core::authorization source;
            auto granted{capabilities.authorize(caller.runtime_instance,caller.generation,version,handle,publisher?"topic.publish":"topic.subscribe",{},source,false,false)};
            if(!granted){return audit_denial(version,code(granted));}
            auto const& cap{*source.capability()};audit_result audit{caller.runtime_instance,caller.generation,version,cap.alias};
            audit.function=publisher?
                (command==rmw_wire::command::activate?"rmw.publisher_activate":command==rmw_wire::command::inspect?"rmw.publisher_inspect":"rmw.publisher_retire"):
                (command==rmw_wire::command::activate?"rmw.subscription_activate":command==rmw_wire::command::inspect?"rmw.subscription_inspect":"rmw.subscription_retire");
            ::std::uint64_t timeout{},maximum{};
            if(!source.target()||!core::parse_u64(source.target()->connection.at("connect-timeout-ms"),timeout)||
               !timeout||timeout>core::ipc::max_timeout_ms||!core::parse_u64(cap.limits.at(publisher?"max-publishers":"max-subscribers"),maximum)||
               !maximum||maximum>core::rmw_endpoint_history::maximum_entries){return audit.finish(22);}
            auto until{entered+::std::chrono::milliseconds(timeout)};
            if(command==rmw_wire::command::activate)
            {if(auto error{backend.connect(version,until)}){return audit.finish(error);}}
            else if(!backend.connected||!backend.connection.session_id()){return audit.finish(125);}
            rmw_wire::packet request{};request.operation=command;request.kind=kind;
            request.owner={2,handle,source.instance_number(),caller.generation,backend.connection.session_id()};request.incarnation=incarnation;
            core::rmw_endpoint_history::entry const* previous{};
            if(command==rmw_wire::command::activate)
            {
                auto error{backend.rmw_endpoints.reserve(request,cap.alias,static_cast<::std::size_t>(maximum),previous)};
                if(error){return audit.finish(error);}
            }
            else
            {
                auto error{backend.rmw_endpoints.find(request.owner,incarnation,request.kind,previous)};
                if(error){return audit.finish(error);}
            }
            ::std::array<::std::uint8_t,rmw_wire::packet_bytes> raw{};
            if(!rmw_wire::encode(request,false,raw)){return audit.finish(22);}
            auto budget{rmw_remaining(until)};
            if(!budget)
            {
                if(command==rmw_wire::command::activate){backend.rmw_endpoints.abandon(handle,110);}
                else if(command==rmw_wire::command::retire){abandon_rmw_transport(backend,110);}
                // Inspect has made no RPC or mutation; its live owner remains
                // available for the caller's explicit subsequent cleanup.
                return audit.finish(110);
            }
            if(command==rmw_wire::command::retire)
            {
                auto error{backend.rmw_endpoints.retire(request.owner,incarnation,request.kind)};
                if(error){return audit.finish(error);}
                if(!backend.rmw_endpoints.claim_cleanup(handle)){return audit.finish(125);}
            }
            rmw_rpc_guard rpc{backend,true,13};
            auto result{backend.connection.call(publisher?"rmw.endpoint-v1":"rmw.subscription-endpoint-v1",cap.alias,{raw.begin(),raw.end()},budget)};
            if(result.error)
            {
                rpc.reason=result.payload.empty()?result.error:13;
                backend.rmw_endpoints.abandon(handle,rpc.reason);
                if(command==rmw_wire::command::retire){backend.rmw_endpoints.cleanup_result(handle,rpc.reason);}
                return audit.finish(rpc.reason);
            }
            rmw_wire::packet reply{};
            if(!rmw_wire::associated_response(result.payload.data(),result.payload.size(),request,reply)){return audit.finish(13);}
            ::std::uint32_t error{};
            if(command==rmw_wire::command::activate){error=backend.rmw_endpoints.activate(reply);}
            else if(command==rmw_wire::command::inspect){error=backend.rmw_endpoints.inspect(reply);}
            else{backend.rmw_endpoints.cleanup_result(handle,0);}
            if(error){return audit.finish(error);}
            if(!rmw_remaining(until)){rpc.reason=110;return audit.finish(110);}
            if(command!=rmw_wire::command::retire&&!runtime::preload_memory_write_host_api(memory,output,result.payload.data(),result.payload.size()))
            {rpc.reason=22;return audit.finish(22);}
            rpc.armed=false;return audit.finish(0);
        }
#if defined(__cpp_exceptions)
        catch(::std::bad_alloc const&){return audit_denial(version,12);}
        catch(...){return audit_denial(version,22);}
#endif
    }
    inline ::std::uint32_t rmw_publisher_activate(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o) noexcept
    {return rmw_endpoint_control(h,0,m,o,rmw_wire::command::activate,rmw_wire::role::publisher);}
    inline ::std::uint32_t rmw_publisher_inspect(::std::uint64_t h,::std::uint64_t i,::std::uint32_t m,::std::uint64_t o) noexcept
    {return rmw_endpoint_control(h,i,m,o,rmw_wire::command::inspect,rmw_wire::role::publisher);}
    inline ::std::uint32_t rmw_publisher_retire(::std::uint64_t h,::std::uint64_t i) noexcept
    {return rmw_endpoint_control(h,i,0,0,rmw_wire::command::retire,rmw_wire::role::publisher);}
    inline ::std::uint32_t rmw_publisher_publish(::std::uint64_t handle,::std::uint64_t incarnation,
        ::std::uint32_t memory,::std::uint64_t input,::std::uint64_t length) noexcept
    {
        constexpr auto version{core::version::ros2};auto entered{::std::chrono::steady_clock::now()};
#if defined(__cpp_exceptions)
        try
#endif
        {
            runtime::ros_caller_identity caller{};::std::shared_ptr<instance> state{};
            if(auto error{identify(version, caller, state)}){return audit_denial(version,error);}
            auto& backend{state->backends[1]};::std::unique_lock lock{backend.mutex,::std::try_to_lock};
            if(!lock.owns_lock()){return audit_denial(version,11);}
            core::authorization source;auto granted{capabilities.authorize(caller.runtime_instance,caller.generation,version,handle,"topic.publish",{},source,false,false)};
            if(!granted){return audit_denial(version,code(granted));}
            auto const& cap{*source.capability()};audit_result audit{caller.runtime_instance,caller.generation,version,cap.alias};audit.function="rmw.publisher_publish";
            if(!backend.connected||!backend.connection.session_id()){return audit.finish(125);}
            rmw_wire::binding owner{2,handle,source.instance_number(),caller.generation,backend.connection.session_id()};
            core::rmw_endpoint_history::entry const* row{};auto error{backend.rmw_endpoints.find(owner,incarnation,rmw_wire::role::publisher,row)};
            if(error){return audit.finish(error);}
            ::std::uint64_t maximum{};
            if(!core::parse_u64(cap.limits.at("max-message-bytes"),maximum)||length>maximum||length>core::ipc::max_payload_bytes-518-core::ipc::rmw_publish::header_bytes){return audit.finish(90);}
            if(!memory_range(memory,input,length)){return audit.finish(22);}
            // One owned allocation/read. Admission borrows the active body,
            // then the fixed envelope is filled without another payload copy.
            ::std::vector<::std::uint8_t> packet(core::ipc::rmw_publish::header_bytes+static_cast<::std::size_t>(length));
            auto body{::std::span<::std::uint8_t>{packet.data()+core::ipc::rmw_publish::header_bytes,static_cast<::std::size_t>(length)}};
            if(!runtime::preload_memory_read_host_api(memory,input,body.data(),body.size())){return audit.finish(22);}
            core::authorization admitted;granted=capabilities.authorize(caller.runtime_instance,caller.generation,version,handle,"topic.publish",::std::as_bytes(body),admitted,true,true);
            if(!granted){return audit.finish(code(granted));}
            ::std::array<::std::uint8_t,core::ipc::rmw_publish::header_bytes> raw{};
            if(!core::ipc::rmw_publish::encode({owner,incarnation,length},raw)){return audit.finish(22);}
            ::std::copy(raw.begin(),raw.end(),packet.begin());
            ::std::uint64_t timeout{};
            if(!core::parse_u64(source.target()->connection.at("connect-timeout-ms"),timeout)||!timeout||timeout>core::ipc::max_timeout_ms){return audit.finish(22);}
            auto until{entered+::std::chrono::milliseconds(timeout)};auto budget{rmw_remaining(until)};if(!budget){return audit.finish(110);}
            rmw_rpc_guard rpc{backend,true,13};auto result{backend.connection.call("rmw.publish-v1",cap.alias,::std::move(packet),budget)};
            if(!result.payload.empty()){return audit.finish(13);}
            if(result.error){rpc.reason=result.error;if(result.error==11){rpc.armed=false;}return audit.finish(result.error);}
            if(!rmw_remaining(until)){rpc.reason=110;return audit.finish(110);}
            rpc.armed=false;return audit.finish(0);
        }
#if defined(__cpp_exceptions)
        catch(::std::bad_alloc const&){return audit_denial(version,12);}
        catch(...){return audit_denial(version,22);}
#endif
    }
}
