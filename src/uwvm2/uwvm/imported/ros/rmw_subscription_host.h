/** @file
 * Optional Host transport for one retained native ROS2 Subscription.
 *
 * Only the current Runtime identity and a live topic.subscribe source select
 * the object. UMRE describes its actual QoS; UMRS borrows one complete original
 * URMI/body record. The Host performs one final Guest copy and never confirms
 * consumption implicitly. Commit/cancel use the exact accepted sequence AND
 * independent reservation nonce. No operation grants update/debug authority.
 */
#pragma once
#include "rmw_publisher_host.h"
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/ipc/rmw_subscription.h>
#endif

namespace uwvm2::uwvm::imported::ros::host
{
    namespace rmw_sub_wire=core::ipc::rmw_subscription;
    inline ::std::uint32_t rmw_subscription_activate(::std::uint64_t h,::std::uint32_t m,::std::uint64_t o) noexcept
    {return rmw_endpoint_control(h,0,m,o,rmw_wire::command::activate,rmw_wire::role::subscription);}
    inline ::std::uint32_t rmw_subscription_inspect(::std::uint64_t h,::std::uint64_t i,::std::uint32_t m,::std::uint64_t o) noexcept
    {return rmw_endpoint_control(h,i,m,o,rmw_wire::command::inspect,rmw_wire::role::subscription);}
    inline ::std::uint32_t rmw_subscription_retire(::std::uint64_t h,::std::uint64_t i) noexcept
    {return rmw_endpoint_control(h,i,0,0,rmw_wire::command::retire,rmw_wire::role::subscription);}

    /** Trusted compensation for a known successful borrow, under its ORIGINAL
     * call deadline. A successful cancellation retains the record, not a refund
     * of already admitted traffic. Its result is recorded independently of the
     * primary failure and native retirement. Unknown/late ACK is never replayed.
     * The caller keeps an armed session guard until this returns exact success.
     */
    inline ::std::uint32_t rmw_cancel_subscription_borrow(session& backend,::std::string_view alias,
        rmw_sub_wire::control borrowed,::std::chrono::steady_clock::time_point until) noexcept
    {
        ::std::uint32_t error{115};
#if defined(__cpp_exceptions)
        try
#endif
        {
            borrowed.operation=rmw_sub_wire::command::cancel;
            borrowed.capacity=borrowed.record_bytes=0;borrowed.gid_bytes=0;
            ::std::array<::std::uint8_t,rmw_sub_wire::header_bytes> raw{};
            auto budget{rmw_remaining(until)};
            if(!budget){error=110;}
            else if(!rmw_sub_wire::encode(borrowed,false,raw)){error=22;}
            else
            {
                auto result{backend.connection.call("rmw.subscription-transfer-v1",alias,{raw.begin(),raw.end()},budget)};
                error=result.error;rmw_sub_wire::control reply{};
                if(error&&!result.payload.empty()){error=13;}
                else if(!error&&!rmw_sub_wire::associated_ack(result.payload.data(),result.payload.size(),borrowed,reply)){error=13;}
                if(!error&&!rmw_remaining(until)){error=110;}
            }
        }
#if defined(__cpp_exceptions)
        catch(::std::bad_alloc const&){error=12;}catch(...){error=5;}
#endif
        backend.rmw_endpoints.cancel_result(borrowed.owner.source,error);return error;
    }

    inline ::std::uint32_t rmw_subscription_peek(::std::uint64_t handle,::std::uint64_t incarnation,
        ::std::uint32_t expected_gid,::std::uint32_t memory,::std::uint64_t output,::std::uint64_t capacity) noexcept
    {
        constexpr auto version{core::version::ros2};auto entered{::std::chrono::steady_clock::now()};
#if defined(__cpp_exceptions)
        try
#endif
        {
            runtime::ros_caller_identity caller{};::std::shared_ptr<instance> state{};
            if(auto error{identify(caller,state)}){return audit_denial(version,error);}
            auto& backend{state->backends[1]};::std::unique_lock lock{backend.mutex,::std::try_to_lock};
            if(!lock.owns_lock()){return audit_denial(version,11);}
            core::authorization source;
            auto granted{capabilities.authorize(caller.runtime_instance,caller.generation,version,handle,"topic.subscribe",{},source,false,false)};
            if(!granted){return audit_denial(version,code(granted));}
            auto const& cap{*source.capability()};audit_result audit{caller.runtime_instance,caller.generation,version,cap.alias};
            audit.function="rmw.subscription_peek";
            if(!backend.connected||!backend.connection.session_id()){return audit.finish(125);}
            rmw_wire::binding owner{2,handle,source.instance_number(),caller.generation,backend.connection.session_id()};
            core::rmw_endpoint_history::entry const* row{};
            auto error{backend.rmw_endpoints.find(owner,incarnation,rmw_wire::role::subscription,row)};
            if(error){return audit.finish(error);}
            if(!expected_gid||expected_gid>32){return audit.finish(95);}
            if(capacity<rmw_sub_wire::minimum_delivery_bytes||capacity>core::ipc::max_payload_bytes){return audit.finish(90);}
            if(!memory_range(memory,output,capacity)){return audit.finish(22);}
            ::std::uint64_t timeout{},maximum{};
            if(!source.target()||!core::parse_u64(source.target()->connection.at("connect-timeout-ms"),timeout)||
               !timeout||timeout>core::ipc::max_timeout_ms||!core::parse_u64(cap.limits.at("max-message-bytes"),maximum))
            {return audit.finish(22);}
            auto until{entered+::std::chrono::milliseconds(timeout)};auto budget{rmw_remaining(until)};
            if(!budget){return audit.finish(110);}
            rmw_sub_wire::control request{};request.owner=owner;request.incarnation=incarnation;
            request.capacity=capacity;request.gid_bytes=static_cast<::std::uint8_t>(expected_gid);
            ::std::array<::std::uint8_t,rmw_sub_wire::header_bytes> raw{};
            if(!rmw_sub_wire::encode(request,false,raw)){return audit.finish(22);}
            // Pending is an actual unknown-outcome marker, not invented success
            // when an exception interrupts the RPC. The session guard retires it.
            backend.rmw_endpoints.transfer_result(handle,115);
            auto finish=[&](::std::uint32_t status) noexcept
            {backend.rmw_endpoints.transfer_result(handle,status);return audit.finish(status);};
            rmw_rpc_guard rpc{backend,true,13};
            auto result{backend.connection.call("rmw.subscription-transfer-v1",cap.alias,{raw.begin(),raw.end()},budget)};
            if(result.error)
            {
                rpc.reason=result.payload.empty()?result.error:13;
                // These are explicit non-consuming outcomes of the trusted
                // native queue/transaction, with an empty error payload.
                if(result.payload.empty()&&(result.error==11||result.error==90||result.error==95||result.error==17)){rpc.armed=false;}
                return finish(rpc.reason);
            }
            rmw_sub_wire::control borrowed{};core::ipc::rmw_metadata::header information{};
            if(!rmw_sub_wire::associated_delivery(result.payload.data(),result.payload.size(),request,borrowed,information))
            {return finish(13);}
            auto fail_borrow=[&](::std::uint32_t primary) noexcept
            {
                auto canceled{rmw_cancel_subscription_borrow(backend,cap.alias,borrowed,until)};
                if(!canceled){rpc.armed=false;}else{rpc.reason=canceled;}
                return finish(primary);
            };
            auto const offset{rmw_sub_wire::minimum_delivery_bytes};
            auto body{::std::span<::std::uint8_t const>{result.payload.data()+offset,result.payload.size()-offset}};
            if(body.size()>maximum){return fail_borrow(90);}
            // The same full source lease remains held. Charge only canonical
            // bytes once; UMRS/URMI and later ACKs are control metadata.
            core::authorization admitted;
            granted=capabilities.authorize(caller.runtime_instance,caller.generation,version,handle,"topic.subscribe",::std::as_bytes(body),admitted,true,true);
            if(!granted){return fail_borrow(code(granted));}
            if(!rmw_remaining(until)){return fail_borrow(110);}
            // Runtime's checked writer validates the whole range before writing.
            // There is no second length-pointer write or post-copy error path.
            if(!runtime::preload_memory_write_host_api(memory,output,result.payload.data(),result.payload.size()))
            {return fail_borrow(22);}
            rpc.armed=false;return finish(0);
        }
#if defined(__cpp_exceptions)
        catch(::std::bad_alloc const&){return audit_denial(version,12);}
        catch(...){return audit_denial(version,22);}
#endif
    }

    inline ::std::uint32_t rmw_subscription_ack(::std::uint64_t handle,::std::uint64_t incarnation,
        ::std::uint64_t sequence,::std::uint64_t reservation,rmw_sub_wire::command operation) noexcept
    {
        constexpr auto version{core::version::ros2};auto entered{::std::chrono::steady_clock::now()};
#if defined(__cpp_exceptions)
        try
#endif
        {
            runtime::ros_caller_identity caller{};::std::shared_ptr<instance> state{};
            if(auto error{identify(caller,state)}){return audit_denial(version,error);}
            auto& backend{state->backends[1]};::std::unique_lock lock{backend.mutex,::std::try_to_lock};
            if(!lock.owns_lock()){return audit_denial(version,11);}
            core::authorization source;
            auto granted{capabilities.authorize(caller.runtime_instance,caller.generation,version,handle,"topic.subscribe",{},source,false,false)};
            if(!granted){return audit_denial(version,code(granted));}
            auto const& cap{*source.capability()};audit_result audit{caller.runtime_instance,caller.generation,version,cap.alias};
            audit.function=operation==rmw_sub_wire::command::commit?"rmw.subscription_commit":"rmw.subscription_cancel";
            if(operation!=rmw_sub_wire::command::commit&&operation!=rmw_sub_wire::command::cancel){return audit.finish(22);}
            if(!backend.connected||!backend.connection.session_id()){return audit.finish(125);}
            rmw_sub_wire::control request{};request.operation=operation;
            request.owner={2,handle,source.instance_number(),caller.generation,backend.connection.session_id()};
            request.incarnation=incarnation;request.sequence=sequence;request.reservation=reservation;
            core::rmw_endpoint_history::entry const* row{};
            auto error{backend.rmw_endpoints.find(request.owner,incarnation,rmw_wire::role::subscription,row)};
            if(error){return audit.finish(error);}
            ::std::array<::std::uint8_t,rmw_sub_wire::header_bytes> raw{};
            if(!rmw_sub_wire::encode(request,false,raw)){return audit.finish(22);}
            ::std::uint64_t timeout{};
            if(!source.target()||!core::parse_u64(source.target()->connection.at("connect-timeout-ms"),timeout)||!timeout||timeout>core::ipc::max_timeout_ms)
            {return audit.finish(22);}
            auto until{entered+::std::chrono::milliseconds(timeout)};auto budget{rmw_remaining(until)};
            if(!budget){return audit.finish(110);}
            backend.rmw_endpoints.transfer_result(handle,115);
            // This marks entry to a Host cancellation attempt, not proof that
            // middleware ran. Frame preparation, write and reply may still fail.
            // Unknown progress must not retain an earlier successful cancel.
            if(operation==rmw_sub_wire::command::cancel){backend.rmw_endpoints.cancel_result(handle,115);}
            auto finish=[&](::std::uint32_t status) noexcept
            {
                backend.rmw_endpoints.transfer_result(handle,status);
                if(operation==rmw_sub_wire::command::cancel){backend.rmw_endpoints.cancel_result(handle,status);}
                return audit.finish(status);
            };
            rmw_rpc_guard rpc{backend,true,13};
            auto result{backend.connection.call("rmw.subscription-transfer-v1",cap.alias,{raw.begin(),raw.end()},budget)};
            if(result.error){rpc.reason=result.payload.empty()?result.error:13;return finish(rpc.reason);}
            rmw_sub_wire::control reply{};
            if(!rmw_sub_wire::associated_ack(result.payload.data(),result.payload.size(),request,reply)){return finish(13);}
            if(!rmw_remaining(until)){rpc.reason=110;return finish(110);}
            rpc.armed=false;return finish(0);
        }
#if defined(__cpp_exceptions)
        catch(::std::bad_alloc const&){return audit_denial(version,12);}
        catch(...){return audit_denial(version,22);}
#endif
    }
    inline ::std::uint32_t rmw_subscription_commit(::std::uint64_t h,::std::uint64_t i,::std::uint64_t s,::std::uint64_t r) noexcept
    {return rmw_subscription_ack(h,i,s,r,rmw_sub_wire::command::commit);}
    inline ::std::uint32_t rmw_subscription_cancel(::std::uint64_t h,::std::uint64_t i,::std::uint64_t s,::std::uint64_t r) noexcept
    {return rmw_subscription_ack(h,i,s,r,rmw_sub_wire::command::cancel);}
}
