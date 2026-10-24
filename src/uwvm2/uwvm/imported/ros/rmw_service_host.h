/** @file
 * Additive owned ROS2 service transport. Include only through host.h.
 *
 * The genuine Runtime caller, both original capability leases, retained native
 * session and immutable pairing select one native object. UMRC/UMRE/URMI are
 * associated values, never bearer authority. No graph, clock, logging, debugger
 * or update grant follows from this module. Default deployments have no grants.
 * A successful peek borrows; only a distinct successful ACK consumes it.
 */
#pragma once
#include "rmw_subscription_host.h"
#include "rmw_service_state.h"

namespace uwvm2::uwvm::imported::ros::host
{
    namespace rmw_srv_wire=core::ipc::rmw_service;
    /** Function-scope leases, not an invented lease around upstream callbacks.
     * All metadata allocation needed by a first activation precedes native
     * creation. Later operations only find an already prepared journal.
     */
    struct rmw_service_access
    {
        ::std::chrono::steady_clock::time_point entered{::std::chrono::steady_clock::now()},until{};
        runtime::ros_caller_identity caller{};::std::shared_ptr<instance> state{};
        session* backend{};::std::unique_lock<::std::mutex> lock{};
        core::authorization source{},paired{};
        core::rmw_service_bindings::entry const* pair{};
        core::rmw_endpoint_history::entry const* endpoint{};
        rmw_service_journal* journal{};::std::unique_ptr<audit_result> audit{};
        rmw_srv_wire::control request{};
        ::std::uint32_t prepare(::std::uint64_t handle,::std::uint64_t incarnation,
            ::std::uint64_t activation_peer,::std::string_view function)
        {
            if(auto error{identify(core::version::ros2, caller, state)}){return error;}
            backend=&state->backends[1];lock=::std::unique_lock<::std::mutex>{backend->mutex,::std::try_to_lock};
            if(!lock.owns_lock()){return 11;}
            core::authorization description;
            auto result{capabilities.describe(caller.runtime_instance,caller.generation,core::version::ros2,handle,description)};
            if(!result){return code(result);}
            auto const& original{*description.capability()};
            auto kind{original.operation=="service.call"?rmw_wire::role::client:
                original.operation=="service.advertise"?rmw_wire::role::service:static_cast<rmw_wire::role>(0)};
            if(kind!=rmw_wire::role::client&&kind!=rmw_wire::role::service){return 95;}
            result=capabilities.authorize(caller.runtime_instance,caller.generation,core::version::ros2,
                handle,kind==rmw_wire::role::client?"service.call":"service.advertise",{},source,false,false);
            if(!result){return code(result);}
            // A true current source is now known. Attach/reset its API ledger
            // BEFORE audit allocation, timeout parsing, disconnection and row
            // lookups can fail. A previous successful RPC stays historical;
            // this entry starts with api_rpc_attempted=false. Unidentified or
            // unauthorized callers never touch another source's journal.
            auto existing{backend->rmw_service_journals.find(handle)};
            if(existing!=backend->rmw_service_journals.end()){journal=&existing->second;}
            else if(activation_peer)
            {
                if(backend->rmw_service_journals.size()>=core::rmw_service_bindings::maximum_entries){return 11;}
                journal=&backend->rmw_service_journals.emplace(handle,rmw_service_journal{}).first->second;
            }
            if(journal){journal->begin_api();}
            audit.reset(new audit_result{caller.runtime_instance,caller.generation,core::version::ros2,source.capability()->alias});
            audit->function=function;
            ::std::uint64_t timeout{},source_timeout{};
            if(!source.target()||!core::parse_u64(source.target()->connection.at("connect-timeout-ms"),timeout)||
                !timeout||timeout>core::ipc::max_timeout_ms||
                !core::parse_u64(source.capability()->limits.at("timeout-ms"),source_timeout)){return 22;}
            if(auto error{rmw_service_values::entry_deadline(entered,timeout,source_timeout,until)}){return error;}
            request.kind=kind;request.owner={2,handle,source.instance_number(),caller.generation,backend->connection.session_id()};
            request.incarnation=incarnation;
            if(activation_peer)
            {
                if(incarnation){return 22;}request.peer=activation_peer;
            }
            else
            {
                if(!incarnation){return 22;}
                if(!backend->connected||!request.owner.session){return 125;}
                auto error{backend->rmw_service_pairs.find_live(request.owner,kind,pair)};if(error){return error;}
                request.peer=pair->peer;
                error=backend->rmw_endpoints.find(request.owner,incarnation,kind,endpoint);if(error){return error;}
            }
            if(!journal){return 13;}
            result=capabilities.authorize(caller.runtime_instance,caller.generation,core::version::ros2,request.peer,
                kind==rmw_wire::role::client?"event.observe":"service.respond",{},paired,false,false);
            if(!result){return code(result);}
            if(source.target()!=paired.target()||source.instance_number()!=paired.instance_number()){return 13;}
            if(auto error{core::service_endpoint_pair(*source.capability(),*paired.capability(),kind)}){return error;}
            if(pair&&(pair->primary_alias!=source.capability()->alias||pair->peer_alias!=paired.capability()->alias)){return 13;}
            return 0;
        }
        ::std::uint32_t finish(::std::uint32_t status) noexcept
        {if(journal){journal->api_result(status);}return audit?audit->finish(status):audit_denial(core::version::ros2,status);}
        ::std::uint32_t begin() noexcept
        {return rmw_remaining(until);}
    };
    template<typename Function>
    inline ::std::uint32_t rmw_service_invoke(Function&& function) noexcept
    {
        rmw_service_access call{};
#if defined(__cpp_exceptions)
        try{return function(call);}catch(::std::bad_alloc const&){return call.finish(12);}catch(...){return call.finish(22);}
#else
        return function(call);
#endif
    }
    inline bool rmw_service_encode(rmw_srv_wire::control const& request,::std::vector<::std::uint8_t>& packet) noexcept
    {
        ::std::array<::std::uint8_t,rmw_srv_wire::header_bytes> raw{};
        if(packet.size()!=rmw_srv_wire::header_bytes+request.body_bytes||!rmw_srv_wire::encode(request,false,raw)){return false;}
        ::std::copy(raw.begin(),raw.end(),packet.begin());return true;
    }
    /** Cancellation for a fully validated known borrow uses its ORIGINAL
     * deadline. Primary, transfer, cancel, commit and retirement remain separate
     * observations, including115 if an exception interrupts an attempted call.
     */
    inline ::std::uint32_t rmw_service_cancel_borrow(rmw_service_access& call,
        rmw_srv_wire::control borrowed) noexcept
    {
        ::std::uint32_t error{115};call.journal->cancelled(115);
#if defined(__cpp_exceptions)
        try
#endif
        {
            borrowed.operation=rmw_srv_wire::command::cancel;borrowed.ticket=0;
            borrowed.body_bytes=borrowed.capacity=borrowed.record_bytes=borrowed.wait_ns=borrowed.value=0;borrowed.writer_bytes=0;
            ::std::array<::std::uint8_t,rmw_srv_wire::header_bytes> raw{};auto budget{rmw_remaining(call.until)};
            if(!budget){error=110;}else if(!rmw_srv_wire::encode(borrowed,false,raw)){error=22;}
            else
            {
                auto result{call.backend->connection.call("rmw.service-v1",call.source.capability()->alias,{raw.begin(),raw.end()},budget)};
                error=result.error;rmw_srv_wire::control reply{};
                if(error&&!result.payload.empty()){error=13;}
                else if(!error&&!rmw_srv_wire::associated_ack(result.payload.data(),result.payload.size(),borrowed,reply)){error=13;}
                if(!error&&!rmw_remaining(call.until)){error=110;}
            }
        }
#if defined(__cpp_exceptions)
        catch(::std::bad_alloc const&){error=12;}catch(...){error=5;}
#endif
        call.journal->cancelled(error);call.backend->rmw_endpoints.cancel_result(borrowed.owner.source,error);
        if(!error){call.journal->clear_borrow();}return error;
    }
    inline ::std::uint32_t rmw_service_endpoint_control(::std::uint64_t handle,::std::uint64_t incarnation,
        ::std::uint64_t peer,::std::uint32_t memory,::std::uint64_t output,
        rmw_srv_wire::command operation,rmw_wire::role required) noexcept
    {
        return rmw_service_invoke([&](rmw_service_access& call)->::std::uint32_t
        {
            if(operation!=rmw_srv_wire::command::activate&&operation!=rmw_srv_wire::command::inspect&&operation!=rmw_srv_wire::command::retire){return call.finish(22);}
            if(operation!=rmw_srv_wire::command::retire&&!memory_range(memory,output,rmw_wire::packet_bytes)){return call.finish(22);}
            if(operation==rmw_srv_wire::command::activate&&!peer){return call.finish(22);}
            if(auto error{call.prepare(handle,incarnation,operation==rmw_srv_wire::command::activate?peer:0,"rmw.service_endpoint")}){return call.finish(error);}
            if(required!=static_cast<rmw_wire::role>(0)&&call.request.kind!=required){return call.finish(95);}
            auto& backend{*call.backend};call.request.operation=operation;
            // Reserve the complete RPC packet before connect/native effects.
            ::std::vector<::std::uint8_t> raw(rmw_srv_wire::header_bytes);
            if(operation==rmw_srv_wire::command::activate)
            {
                ::std::uint64_t maximum{core::rmw_endpoint_history::maximum_entries};
                if(call.request.kind==rmw_wire::role::service&&(!core::parse_u64(call.source.capability()->limits.at("max-services"),maximum)||
                    !maximum||maximum>core::rmw_endpoint_history::maximum_entries)){return call.finish(22);}
                if(auto error{backend.connect(core::version::ros2,call.until)}){return call.finish(error);}
                call.request.owner.session=backend.connection.session_id();
                auto error{backend.rmw_service_pairs.reserve(call.request.owner,call.request.kind,peer,
                    *call.source.capability(),*call.paired.capability(),call.pair)};if(error){return call.finish(error);}
                rmw_wire::packet reservation{};reservation.owner=call.request.owner;reservation.kind=call.request.kind;
                error=backend.rmw_endpoints.reserve(reservation,call.source.capability()->alias,static_cast<::std::size_t>(maximum),call.endpoint);
                if(error){backend.rmw_service_pairs.retire(call.request.owner,call.request.kind,error);return call.finish(error);}
            }
            if(!rmw_service_encode(call.request,raw)){return call.finish(22);}
            auto budget{call.begin()};if(!budget)
            {
                if(operation==rmw_srv_wire::command::activate)
                {backend.rmw_service_pairs.retire(call.request.owner,call.request.kind,110);backend.rmw_endpoints.abandon(handle,110);}
                return call.finish(110);
            }
            if(operation==rmw_srv_wire::command::retire)
            {
                // Retire locally before the first private native effect. No
                // failed/lost reply revives either side or permits a retry.
                backend.rmw_service_pairs.retire(call.request.owner,call.request.kind,125);
                backend.rmw_endpoints.abandon(handle,125);
                if(!backend.rmw_endpoints.claim_cleanup(handle)){return call.finish(125);}
                call.journal->cleaned(115);
            }
            rmw_rpc_guard guard{backend,true,13};
            call.journal->begin_rpc();
            auto result{backend.connection.call("rmw.service-v1",call.source.capability()->alias,::std::move(raw),budget)};
            call.journal->end_rpc(result.error);
            if(result.error)
            {
                guard.reason=result.payload.empty()?result.error:13;
                if(operation==rmw_srv_wire::command::retire)
                {backend.rmw_endpoints.cleanup_result(handle,guard.reason);call.journal->cleaned(guard.reason);}
                return call.finish(guard.reason);
            }
            rmw_srv_wire::control reply{};rmw_wire::packet observed{};
            if(!rmw_srv_wire::associated_endpoint(result.payload.data(),result.payload.size(),call.request,reply,observed)){return call.finish(13);}
            ::std::uint32_t error{};
            if(operation==rmw_srv_wire::command::activate){error=backend.rmw_endpoints.activate(observed);}
            else if(operation==rmw_srv_wire::command::inspect){error=backend.rmw_endpoints.inspect(observed);}
            else{backend.rmw_endpoints.cleanup_result(handle,0);call.journal->cleaned(0);}
            if(error){return call.finish(error);}call.journal->receipt_observed=true;
            if(!rmw_remaining(call.until)){guard.reason=110;return call.finish(110);}
            // Public receipt is UMRE256 only; UMRC is independently checked.
            if(operation!=rmw_srv_wire::command::retire&&!runtime::preload_memory_write_host_api(memory,output,
                result.payload.data()+rmw_srv_wire::header_bytes,rmw_wire::packet_bytes)){guard.reason=22;return call.finish(22);}
            guard.armed=false;return call.finish(0);
        });
    }
    inline ::std::uint32_t rmw_client_activate(::std::uint64_t h,::std::uint64_t p,::std::uint32_t m,::std::uint64_t o) noexcept
    {return rmw_service_endpoint_control(h,0,p,m,o,rmw_srv_wire::command::activate,rmw_wire::role::client);}
    inline ::std::uint32_t rmw_service_activate(::std::uint64_t h,::std::uint64_t p,::std::uint32_t m,::std::uint64_t o) noexcept
    {return rmw_service_endpoint_control(h,0,p,m,o,rmw_srv_wire::command::activate,rmw_wire::role::service);}
    inline ::std::uint32_t rmw_service_inspect(::std::uint64_t h,::std::uint64_t i,::std::uint32_t m,::std::uint64_t o) noexcept
    {return rmw_service_endpoint_control(h,i,0,m,o,rmw_srv_wire::command::inspect,static_cast<rmw_wire::role>(0));}
    inline ::std::uint32_t rmw_service_retire(::std::uint64_t h,::std::uint64_t i) noexcept
    {return rmw_service_endpoint_control(h,i,0,0,0,rmw_srv_wire::command::retire,static_cast<rmw_wire::role>(0));}

    /** One owned canonical request/response copy. Respond authorizes the true
     * distinct responder. The private prefix view is restored BEFORE encoding
     * or transport; only canonical body bytes consume service byte credit.
     */
    inline ::std::uint32_t rmw_service_send(::std::uint64_t handle,::std::uint64_t incarnation,
        ::std::uint64_t responder,::std::uint64_t ticket,::std::uint32_t memory,::std::uint64_t input,
        ::std::uint64_t length,::std::uint64_t ticket_output,bool respond) noexcept
    {
        return rmw_service_invoke([&](rmw_service_access& call)->::std::uint32_t
        {
            if(length>rmw_srv_wire::maximum_input_bytes||!memory_range(memory,input,length)||
                (!respond&&!memory_range(memory,ticket_output,8))){return call.finish(length>rmw_srv_wire::maximum_input_bytes?90u:22u);}
            if(auto error{call.prepare(handle,incarnation,0,respond?"rmw.service_respond":"rmw.client_submit")}){return call.finish(error);}
            if(call.request.kind!=(respond?rmw_wire::role::service:rmw_wire::role::client)){return call.finish(95);}
            if(respond&&(responder!=call.request.peer||!ticket)){return call.finish(13);}
            if(respond)
            {
                ::std::uint64_t response_timeout{};
                if(!core::parse_u64(call.paired.capability()->limits.at("timeout-ms"),response_timeout)){return call.finish(22);}
                if(auto error{rmw_service_values::clamp_deadline(call.entered,response_timeout,call.until)}){return call.finish(error);}
            }
            ::std::uint64_t maximum{};auto const& cap{respond?*call.paired.capability():*call.source.capability()};
            auto const bound{cap.limits.find(rmw_service_values::send_limit_key(respond))};
            if(bound==cap.limits.end()){return call.finish(22);}
            if(!core::parse_u64(bound->second,maximum)||length>maximum){return call.finish(90);}
            ::std::vector<::std::uint8_t> raw(rmw_srv_wire::header_bytes+static_cast<::std::size_t>(length));
            auto body{::std::span<::std::uint8_t>{raw.data()+rmw_srv_wire::header_bytes,static_cast<::std::size_t>(length)}};
            if(!runtime::preload_memory_read_host_api(memory,input,body.data(),body.size())){return call.finish(22);}
            core::authorization admitted;core::result granted;
            if(respond)
            {
                rmw_service_values::ticket_prefix prefix{raw.data()+rmw_srv_wire::header_bytes-8,ticket};
                granted=capabilities.authorize_rmw_service_envelope(call.caller.runtime_instance,call.caller.generation,core::version::ros2,
                    responder,"service.respond",::std::as_bytes(::std::span<::std::uint8_t const>{raw.data()+rmw_srv_wire::header_bytes-8,body.size()+8}),admitted,true);
            }
            else{granted=capabilities.authorize(call.caller.runtime_instance,call.caller.generation,core::version::ros2,
                handle,"service.call",::std::as_bytes(body),admitted,true,true);}
            if(!granted){return call.finish(code(granted));}
            call.request.operation=respond?rmw_srv_wire::command::respond:rmw_srv_wire::command::submit;
            call.request.body_bytes=length;call.request.ticket=ticket;
            if(!rmw_service_encode(call.request,raw)){return call.finish(22);}
            auto budget{call.begin()};if(!budget){return call.finish(110);}
            rmw_rpc_guard guard{*call.backend,true,13};
            call.journal->begin_rpc();
            auto result{call.backend->connection.call("rmw.service-v1",cap.alias,::std::move(raw),budget)};
            call.journal->end_rpc(result.error);
            if(result.error)
            {
                guard.reason=result.payload.empty()?result.error:13;
                return call.finish(guard.reason);
            }
            rmw_srv_wire::control reply{};
            if(!rmw_srv_wire::associated_ack(result.payload.data(),result.payload.size(),call.request,reply)){return call.finish(13);}
            if(!rmw_remaining(call.until)){guard.reason=110;return call.finish(110);}
            if(!respond&&!write_u64(memory,ticket_output,reply.ticket)){guard.reason=22;return call.finish(22);}
            guard.armed=false;return call.finish(0);
        });
    }
    inline ::std::uint32_t rmw_client_submit(::std::uint64_t h,::std::uint64_t i,::std::uint32_t m,::std::uint64_t p,::std::uint64_t n,::std::uint64_t o) noexcept
    {return rmw_service_send(h,i,0,0,m,p,n,o,false);}
    inline ::std::uint32_t rmw_service_respond(::std::uint64_t h,::std::uint64_t i,::std::uint64_t r,::std::uint64_t t,::std::uint32_t m,::std::uint64_t p,::std::uint64_t n) noexcept
    {return rmw_service_send(h,i,r,t,m,p,n,0,true);}
    inline ::std::uint32_t rmw_service_peek(::std::uint64_t handle,::std::uint64_t incarnation,
        ::std::uint32_t expected_writer,::std::uint32_t memory,::std::uint64_t output,::std::uint64_t capacity) noexcept
    {
        return rmw_service_invoke([&](rmw_service_access& call)->::std::uint32_t
        {
            if(!expected_writer||expected_writer>16){return call.finish(95);}
            if(capacity<rmw_srv_wire::minimum_delivery_bytes||capacity>core::ipc::max_payload_bytes){return call.finish(90);}
            if(!memory_range(memory,output,capacity)){return call.finish(22);}
            if(auto error{call.prepare(handle,incarnation,0,"rmw.service_peek")}){return call.finish(error);}
            if(call.journal->pending_sequence){return call.finish(17);}
            // The old event_poll_v1 contract charges one observer poll operation
            // even when native returns idle11. Header/body credit is separate.
            core::authorization polling;
            if(call.request.kind==rmw_wire::role::client)
            {
                auto admitted{capabilities.authorize(call.caller.runtime_instance,call.caller.generation,core::version::ros2,
                    call.request.peer,"event.observe",{},polling,false,true)};
                if(!admitted){return call.finish(code(admitted));}
            }
            call.request.operation=rmw_srv_wire::command::peek;call.request.capacity=capacity;
            call.request.writer_bytes=static_cast<::std::uint8_t>(expected_writer);
            ::std::vector<::std::uint8_t> raw(rmw_srv_wire::header_bytes);
            if(!rmw_service_encode(call.request,raw)){return call.finish(22);}
            auto budget{call.begin()};if(!budget){return call.finish(110);}
            rmw_rpc_guard guard{*call.backend,true,13};
            call.journal->begin_rpc();
            auto result{call.backend->connection.call("rmw.service-v1",call.source.capability()->alias,::std::move(raw),budget)};
            call.journal->end_rpc(result.error);
            if(result.error)
            {
                guard.reason=result.payload.empty()?result.error:13;
                if(rmw_service_values::peek_idle(result.error,result.payload.empty())){guard.armed=false;}
                return call.finish(guard.reason);
            }
            rmw_srv_wire::control borrowed{};core::ipc::rmw_metadata::header information{};
            if(!rmw_srv_wire::associated_delivery(result.payload.data(),result.payload.size(),call.request,borrowed,information)){return call.finish(13);}
            // Only a completely associated response establishes a cancellable
            // borrow. An untrusted/malformed response retires the session.
            auto fail_borrow=[&](::std::uint32_t primary) noexcept
            {
                call.journal->primary(primary);
                auto cancel{rmw_service_cancel_borrow(call,borrowed)};
                if(!cancel){guard.armed=false;}else{guard.reason=cancel;}
                return call.finish(primary);
            };
            return rmw_service_values::guarded_borrow([&]()->::std::uint32_t
            {
                if(borrowed.sequence<=call.journal->last_committed||borrowed.sequence<call.journal->last_sequence||
                    borrowed.reservation<=call.journal->last_reservation){return fail_borrow(13);}
                call.journal->pending_sequence=borrowed.sequence;call.journal->pending_reservation=borrowed.reservation;
                call.journal->last_sequence=borrowed.sequence;call.journal->last_reservation=borrowed.reservation;
                auto body{::std::span<::std::uint8_t const>{result.payload.data()+rmw_srv_wire::minimum_delivery_bytes,
                    result.payload.size()-rmw_srv_wire::minimum_delivery_bytes}};
                core::authorization delivery_source,delivery_peer;
                if(call.request.kind==rmw_wire::role::client)
                {
                    core::ipc::event_header logical{};
                    if(!rmw_service_values::event(information,logical)){return fail_borrow(13);}
                    auto admitted{capabilities.authorize_event_delivery(call.caller.runtime_instance,call.caller.generation,
                        core::version::ros2,call.request.peer,handle,logical,::std::as_bytes(body),delivery_peer,delivery_source)};
                    if(!admitted){return fail_borrow(code(admitted));}
                }
                else if(!core::ipc::event_control(information.kind))
                {
                    // Real advertise validation consumes canonical bytes ONLY. The
                    // ticket is the genuine positive Gateway request correlation;
                    // private URMI reserved bytes are restored on every outcome.
                    core::result admitted;
                    {
                        rmw_service_values::ticket_prefix prefix{result.payload.data()+rmw_srv_wire::minimum_delivery_bytes-8,borrowed.ticket};
                        auto envelope{::std::span<::std::uint8_t const>{result.payload.data()+rmw_srv_wire::minimum_delivery_bytes-8,body.size()+8}};
                        admitted=capabilities.authorize_rmw_service_envelope(call.caller.runtime_instance,call.caller.generation,
                            core::version::ros2,handle,"service.advertise",::std::as_bytes(envelope),delivery_source,true);
                    }
                    if(!admitted){return fail_borrow(code(admitted));}
                }
                // Controls have no canonical body/advertise operation token. Their
                // full structural/source association is still verified above. A
                // client additionally requires its actual observer control mask.
                if(!rmw_remaining(call.until)){return fail_borrow(110);}
                if(!runtime::preload_memory_write_host_api(memory,output,result.payload.data(),result.payload.size())){return fail_borrow(22);}
                // No fallible action follows the one complete checked Guest write.
                guard.armed=false;return call.finish(0);
            },fail_borrow);
        });
    }
    inline ::std::uint32_t rmw_service_ack(::std::uint64_t handle,::std::uint64_t incarnation,
        ::std::uint64_t sequence,::std::uint64_t reservation,rmw_srv_wire::command operation) noexcept
    {
        return rmw_service_invoke([&](rmw_service_access& call)->::std::uint32_t
        {
            if(operation!=rmw_srv_wire::command::commit&&operation!=rmw_srv_wire::command::cancel){return call.finish(22);}
            if(auto error{call.prepare(handle,incarnation,0,operation==rmw_srv_wire::command::commit?"rmw.service_commit":"rmw.service_cancel")}){return call.finish(error);}
            if(!sequence||!reservation||sequence!=call.journal->pending_sequence||reservation!=call.journal->pending_reservation){return call.finish(13);}
            call.request.operation=operation;call.request.sequence=sequence;call.request.reservation=reservation;
            ::std::vector<::std::uint8_t> raw(rmw_srv_wire::header_bytes);
            if(!rmw_service_encode(call.request,raw)){return call.finish(22);}
            auto budget{call.begin()};if(!budget){return call.finish(110);}
            if(operation==rmw_srv_wire::command::commit){call.journal->committed(115);}
            else{call.journal->cancelled(115);call.backend->rmw_endpoints.cancel_result(handle,115);}
            auto finish=[&](::std::uint32_t error) noexcept
            {
                if(operation==rmw_srv_wire::command::commit){call.journal->committed(error);}
                else{call.journal->cancelled(error);call.backend->rmw_endpoints.cancel_result(handle,error);}
                return call.finish(error);
            };
            rmw_rpc_guard guard{*call.backend,true,13};
            call.journal->begin_rpc();
            auto result{call.backend->connection.call("rmw.service-v1",call.source.capability()->alias,::std::move(raw),budget)};
            call.journal->end_rpc(result.error);
            if(result.error){guard.reason=result.payload.empty()?result.error:13;return finish(guard.reason);}
            rmw_srv_wire::control reply{};
            if(!rmw_srv_wire::associated_ack(result.payload.data(),result.payload.size(),call.request,reply)){return finish(13);}
            if(!rmw_remaining(call.until)){guard.reason=110;return finish(110);}
            if(operation==rmw_srv_wire::command::commit){call.journal->last_committed=sequence;}
            call.journal->clear_borrow();guard.armed=false;return finish(0);
        });
    }
    inline ::std::uint32_t rmw_service_commit(::std::uint64_t h,::std::uint64_t i,::std::uint64_t s,::std::uint64_t r) noexcept
    {return rmw_service_ack(h,i,s,r,rmw_srv_wire::command::commit);}
    inline ::std::uint32_t rmw_service_cancel(::std::uint64_t h,::std::uint64_t i,::std::uint64_t s,::std::uint64_t r) noexcept
    {return rmw_service_ack(h,i,s,r,rmw_srv_wire::command::cancel);}
    inline ::std::uint32_t rmw_client_abandon(::std::uint64_t handle,::std::uint64_t incarnation,::std::uint64_t ticket) noexcept
    {
        return rmw_service_invoke([&](rmw_service_access& call)->::std::uint32_t
        {
            if(!ticket){return call.finish(22);}
            if(auto error{call.prepare(handle,incarnation,0,"rmw.client_abandon")}){return call.finish(error);}
            if(call.request.kind!=rmw_wire::role::client){return call.finish(95);}
            call.request.operation=rmw_srv_wire::command::abandon;call.request.ticket=ticket;
            ::std::vector<::std::uint8_t> raw(rmw_srv_wire::header_bytes);
            if(!rmw_service_encode(call.request,raw)){return call.finish(22);}
            auto budget{call.begin()};if(!budget){return call.finish(110);}
            rmw_rpc_guard guard{*call.backend,true,13};
            call.journal->begin_rpc();
            auto result{call.backend->connection.call("rmw.service-v1",call.source.capability()->alias,::std::move(raw),budget)};
            call.journal->end_rpc(result.error);
            if(result.error){guard.reason=result.payload.empty()?result.error:13;return call.finish(guard.reason);}
            rmw_srv_wire::control reply{};
            if(!rmw_srv_wire::associated_ack(result.payload.data(),result.payload.size(),call.request,reply)){return call.finish(13);}
            if(!rmw_remaining(call.until)){guard.reason=110;return call.finish(110);}
            guard.armed=false;return call.finish(0);
        });
    }
    inline ::std::uint32_t rmw_client_available(::std::uint64_t handle,::std::uint64_t incarnation,
        ::std::uint64_t availability,::std::uint64_t wait_ns,::std::uint32_t memory,::std::uint64_t output) noexcept
    {
        return rmw_service_invoke([&](rmw_service_access& call)->::std::uint32_t
        {
            if(!memory_range(memory,output,1)){return call.finish(22);}
            if(auto error{call.prepare(handle,incarnation,0,"rmw.client_available")}){return call.finish(error);}
            if(call.request.kind!=rmw_wire::role::client){return call.finish(95);}
            if(availability==handle||availability==call.request.peer){return call.finish(13);}
            core::authorization query;
            auto grant{capabilities.authorize(call.caller.runtime_instance,call.caller.generation,core::version::ros2,
                availability,"service.available",{},query,false,false)};
            if(!grant){return call.finish(code(grant));}
            if(query.target()!=call.source.target()||query.instance_number()!=call.source.instance_number()||
                !core::service_availability_pair(core::version::ros2,*query.capability(),*call.source.capability())){return call.finish(13);}
            ::std::uint64_t maximum_wait{},maximum_history{},query_timeout{};
            if(!core::parse_u64(query.capability()->limits.at("max-wait-ns"),maximum_wait)||
                !core::parse_u64(query.capability()->limits.at("max-binding-history"),maximum_history)||
                !core::parse_u64(query.capability()->limits.at("timeout-ms"),query_timeout)||!query_timeout||query_timeout>core::ipc::max_timeout_ms||
                !maximum_wait||maximum_wait>rmw_srv_wire::maximum_wait_ns||!maximum_history||maximum_history>65536){return call.finish(22);}
            if(wait_ns>maximum_wait||wait_ns>rmw_srv_wire::maximum_wait_ns){return call.finish(22);}
            if(auto error{rmw_service_values::clamp_deadline(call.entered,query_timeout,call.until)}){return call.finish(error);}
            // This query's independent64-byte semantic observation envelope is
            // not UMRC/control overhead and is not charged to service.call.
            core::ipc::client_availability::context association{2,availability,handle,call.source.instance_number(),call.caller.generation};
            core::ipc::client_availability::packet observation{core::ipc::client_availability::command::query,
                core::ipc::client_availability::retirement::none,0,association,wait_ns};
            if(!core::ipc::client_availability::valid_request(observation)){return call.finish(22);}
            auto envelope{core::ipc::client_availability::encode(observation)};
            core::authorization admitted;
            grant=capabilities.authorize(call.caller.runtime_instance,call.caller.generation,core::version::ros2,
                availability,"service.available",::std::as_bytes(::std::span{envelope}),admitted,true,true);
            if(!grant){return call.finish(code(grant));}
            core::service_availability_history::entry const* history{};
            if(auto error{call.backend->rmw_service_queries.bind(association,query.capability()->alias,call.source.capability()->alias,
                call.request.owner.session,static_cast<::std::size_t>(maximum_history),history)}){return call.finish(error);}
            call.request.operation=rmw_srv_wire::command::available;call.request.peer=availability;call.request.wait_ns=wait_ns;
            ::std::vector<::std::uint8_t> raw(rmw_srv_wire::header_bytes);
            if(!rmw_service_encode(call.request,raw)){return call.finish(22);}
            auto budget{call.begin()};if(!budget){return call.finish(110);}
            // No sleep or graph fallback. The original deadline must cover the
            // real native finite query AND its response; never mint a new one.
            auto needed{wait_ns/1000000+(wait_ns%1000000?1:0)};
            if(needed>=budget){return call.finish(110);}
            rmw_rpc_guard guard{*call.backend,true,13};
            call.journal->begin_rpc();
            auto result{call.backend->connection.call("rmw.service-v1",query.capability()->alias,::std::move(raw),budget)};
            call.journal->end_rpc(result.error);
            if(result.error)
            {
                guard.reason=result.payload.empty()?result.error:13;
                return call.finish(guard.reason);
            }
            rmw_srv_wire::control reply{};
            if(!rmw_srv_wire::associated_ack(result.payload.data(),result.payload.size(),call.request,reply)){return call.finish(13);}
            observation.value=reply.value;auto response_envelope{core::ipc::client_availability::encode(observation)};
            core::authorization response_credit;
            grant=capabilities.authorize_response(call.caller.runtime_instance,call.caller.generation,core::version::ros2,
                availability,::std::as_bytes(::std::span{response_envelope}),response_credit);
            if(!grant){guard.armed=false;return call.finish(code(grant));}
            if(!rmw_remaining(call.until)){guard.reason=110;return call.finish(110);}
            auto value{static_cast<::std::uint8_t>(reply.value)};
            if(!runtime::preload_memory_write_host_api(memory,output,&value,1)){guard.reason=22;return call.finish(22);}
            guard.armed=false;return call.finish(0);
        });
    }
}
