/* SPDX-License-Identifier: Apache-2.0 */
/** Private authenticated-Host service routing. This C++17 value layer creates
 * no ROS entity and grants no capability. Peer integers are associations from
 * the already authenticated Host, whose actual current handle leases remain
 * mandatory; aliases and grants ALWAYS come from the immutable bound config.
 * A connection owns this table until all Native objects have retired. There is
 * no reconnect, eviction, replay, discovery grant or legacy-service fallback.
 */
#pragma once
#include "config.h"
#include <uwvm2/imported/ros/ipc/rmw_service.h>
#include <uwvm2/imported/ros/common/rmw_service_bindings.h>
#include <uwvm2/imported/ros/common/service_availability.h>
#include <algorithm>
#include <set>
#include <functional>
#include <csignal>
#include <atomic>

namespace uwvm2::ros_gateway
{
    struct owned_service_admission
    {
        ros::capability_spec const* source{};
        ros::capability_spec const* paired{};
        ipc::rmw_endpoint::binding paired_owner{};
        ros::capability_spec const* availability{};
        ipc::rmw_endpoint::binding availability_owner{};
        // Trusted Native session-only lifetime. Never serialized or accepted
        // from Guest UMRC fields; the synchronous server retains all three.
        int session_fd{-1},worker_pidfd{-1};
        std::atomic<std::sig_atomic_t> const* stopping{};
    };
    inline bool owned_service_body(ros::capability_spec const&,bool,std::uint8_t const*,std::size_t)noexcept;
    struct owned_service_backend
    {
        virtual ~owned_service_backend()=default;
        // Zero is absence of the explicitly loaded Native profile, never a
        // default capacity or authority. No old adapter creates a native client.
        virtual std::size_t owned_rmw_service_capacity() const noexcept{return 0;}
        virtual ipc::call_result owned_rmw_service(owned_service_admission const&,
            ipc::rmw_service::control const&,std::uint8_t const*,std::size_t,ipc::deadline){return {95,{}};}
        virtual std::uint32_t owned_rmw_service_delivery_failed(ipc::rmw_endpoint::binding const&,
            std::uint64_t,std::uint32_t) noexcept{return 95;}
        virtual void close_owned_rmw_service(ipc::rmw_endpoint::binding const&,std::uint64_t,
            std::string_view,bool,std::uint32_t) noexcept{}
        virtual void clear_owned_rmw_services() noexcept{}
        /** Complete canonical body validation before traffic charge or Native
         * send. Default preserves the existing closed builtin family. A custom
         * override must use its independently proved selected codec, without
         * constructing Native objects, creating rights or mutating ownership. */
        virtual std::uint32_t validate_owned_rmw_service_body(ros::capability_spec const& cap,
            bool response,std::uint8_t const* bytes,std::size_t size)noexcept
        {return owned_service_body(cap,response,bytes,size)?0u:22u;}
    };
    inline bool owned_service_operation(std::string_view op) noexcept
    {return op=="rmw.service-v1"||op=="rmw.service-retire-v1";}
    /** Both public retire and private cleanup use the same closed command.
     * Only the PRIVATE lane bypasses alias revocation using a known saved row;
     * it can never open/query/respond or reinterpret another command. */
    inline bool owned_service_control_lane(std::string_view operation,ipc::rmw_service::command command) noexcept
    {
        using c=ipc::rmw_service::command;
        if(operation=="rmw.service-retire-v1")return command==c::retire;
        if(operation!="rmw.service-v1")return false;
        switch(command)
        {
            case c::activate:case c::inspect:case c::retire:case c::submit:case c::peek:
            case c::commit:case c::cancel:case c::respond:case c::available:case c::abandon:return true;
            default:return false;
        }
    }
    inline bool owned_service_idle_reply(ipc::rmw_service::command command,ipc::call_result const& reply) noexcept
    {return command==ipc::rmw_service::command::peek&&reply.error==11&&reply.payload.empty();}
    inline bool owned_service_body(ros::capability_spec const& source,bool response,
        std::uint8_t const* bytes,std::size_t size) noexcept
    {
        // This finite bridge does NOT infer a custom codec from a width. Full
        // original Native graph/hash/codec proof is independently required by
        // the genuine typed Registry. Explicit custom IDs have no such proof
        // here and are refused, not dropped or treated as builtin permission.
        if((!bytes&&size)||source.limits.count("adapter-codec-identity")){return false;}
        if(source.type=="std_srvs/srv/Empty"){return size==0;}
        return source.type=="example_interfaces/srv/AddTwoInts"&&size==(response?8u:16u);
    }
    inline ros::capability_spec const* owned_service_pair(configuration const& cfg,
        ros::capability_spec const& source,ipc::rmw_endpoint::role role) noexcept
    {
        ros::capability_spec const* result{};
        for(auto const& row:cfg.policy.capabilities)
        {
            if(ros::service_endpoint_pair(source,row.second,role)){continue;}
            if(result){return nullptr;}result=&row.second;
        }
        return result; // absent OR ambiguous is refused, never first-match grant
    }
    struct owned_service_delivery_facts
    {
        std::uint32_t primary{},cancel_result{};bool cancel_attempted{};
    };
    class owned_service_route
    {
        using binding=ipc::rmw_endpoint::binding;
        using control=ipc::rmw_service::control;
        using command=ipc::rmw_service::command;
        using role=ipc::rmw_endpoint::role;
        using history=ros::rmw_service_bindings;
        configuration const& config_;
        owned_service_backend& backend_;
        int session_fd_{-1},worker_pidfd_{-1};
        std::atomic<std::sig_atomic_t> const* stopping_{};
        history history_;
        bool sealed_{},delivery_armed_{},close_session_{};
        binding delivered_owner_{};std::uint64_t delivered_incarnation_{};
        owned_service_delivery_facts facts_{};
        static binding peer(binding source,std::uint64_t handle) noexcept
        {source.source=handle;return source;}
        std::uint32_t resolve(ros::capability_spec const& alias,control const& request,
            std::set<std::string> const& revoked,owned_service_admission& admission,
            history::entry const*& row,bool cleanup) const
        {
            auto source=config_.policy.capabilities.end(),paired=source;
            if(request.operation==command::activate)
            {
                if(alias.operation!=(request.kind==role::client?"service.call":"service.advertise")){return 13;}
                auto auxiliary=owned_service_pair(config_,alias,request.kind);if(!auxiliary){return 13;}
                source=config_.policy.capabilities.find(alias.alias);paired=config_.policy.capabilities.find(auxiliary->alias);
            }
            else
            {
                auto error=cleanup?history_.lookup(request.owner,request.kind,row):history_.find_live(request.owner,request.kind,row);
                if(error){return error;}source=config_.policy.capabilities.find(row->primary_alias);
                paired=config_.policy.capabilities.find(row->peer_alias);
                if(source==config_.policy.capabilities.end()||paired==config_.policy.capabilities.end()){return 13;}
                if(request.operation!=command::available&&request.peer!=row->peer){return 13;}
                auto expected=request.operation==command::respond?row->peer_alias:row->primary_alias;
                if(request.operation!=command::available&&alias.alias!=expected){return 13;}
            }
            if(source==config_.policy.capabilities.end()||paired==config_.policy.capabilities.end()||
                ros::service_endpoint_pair(source->second,paired->second,request.kind)){return 13;}
            admission.source=&source->second;admission.paired=&paired->second;
            admission.paired_owner=peer(request.owner,row?row->peer:request.peer);
            if(!cleanup&&(revoked.count(source->first)||revoked.count(paired->first))){return 125;}
            if(request.operation==command::available)
            {
                if(alias.operation!="service.available"||!ros::service_availability_pair(ros::version::ros2,alias,source->second))return 13;
                if(revoked.count(alias.alias))return 125;
                admission.availability=&alias;admission.availability_owner=peer(request.owner,request.peer);
            }
            else if(alias.alias!=(request.operation==command::respond?paired->first:source->first))return 13;
            return 0;
        }
    public:
        enum class charge_kind {activate,operation,bytes,preview_operation,preview_bytes};
        owned_service_route(configuration const& cfg,owned_service_backend& backend,
            int session_fd=-1,int worker_pidfd=-1,std::atomic<std::sig_atomic_t> const* stopping=nullptr):
            config_{cfg},backend_{backend},session_fd_{session_fd},worker_pidfd_{worker_pidfd},stopping_{stopping},
            history_{backend.owned_rmw_service_capacity()}{}
        owned_service_route(owned_service_route const&)=delete;
        ~owned_service_route(){clear();}
        bool close_session() const noexcept{return close_session_;}
        owned_service_delivery_facts const& delivery_facts() const noexcept{return facts_;}
        bool alias_live(std::string_view alias) const noexcept
        {
            for(auto const& p:history_.rows())if(!p.second.retired&&
                (p.second.primary_alias==alias||p.second.peer_alias==alias))return true;
            return false;
        }
        /** Cancel THIS successful borrow only. Native Registry retains primary,
         * cancel and retire independently. Attempt here means actual interface
         * invocation, not an invented successful Native cancellation. */
        void delivery_failed(std::uint32_t primary) noexcept
        {
            if(!delivery_armed_)return;delivery_armed_=false;facts_.primary=primary?primary:107;
            facts_.cancel_attempted=true;facts_.cancel_result=115;
            facts_.cancel_result=backend_.owned_rmw_service_delivery_failed(delivered_owner_,delivered_incarnation_,facts_.primary);
            if(facts_.cancel_result)close_session_=true;
        }
        void delivered() noexcept{delivery_armed_=false;}
        void close_alias(std::string_view alias,bool revoke) noexcept
        {
            // The Host emits alias-close only after its last real handle closes.
            // Preserve every saved old session/domain; never consult session0.
            for(auto const& p:history_.rows())if(p.second.primary_alias==alias||p.second.peer_alias==alias)
                (void)history_.retire(p.second.owner,p.second.kind,revoke?125u:0u);
            for(auto const& p:history_.rows())if(p.second.primary_alias==alias||p.second.peer_alias==alias)
                backend_.close_owned_rmw_service(p.second.owner,0,alias,true,revoke?125u:0u);
        }
        void clear() noexcept
        {if(sealed_)return;sealed_=true;delivery_failed(107);history_.seal();backend_.clear_owned_rmw_services();}
        template<class Charge>ipc::call_result execute(ipc::request const& original,
            ros::capability_spec const& alias,std::set<std::string> const& revoked,
            std::uint64_t session,ipc::deadline started,ipc::deadline& until,Charge&& charge)
        {
            if(sealed_)return {125,{}};if(!backend_.owned_rmw_service_capacity())return {95,{}};
            control request{};if(config_.policy.ros_version!=ros::version::ros2||
                !ipc::rmw_service::request(original.data.data(),original.data.size(),request)||request.owner.session!=session)return {22,{}};
            bool cleanup=original.operation=="rmw.service-retire-v1";
            if(!owned_service_control_lane(original.operation,request.operation))return {22,{}};
            owned_service_admission admission{};history::entry const* row{};
            auto error=resolve(alias,request,revoked,admission,row,cleanup);if(error)return {error,{}};
            admission.session_fd=session_fd_;admission.worker_pidfd=worker_pidfd_;admission.stopping=stopping_;
            auto const& source=*admission.source;
            for(auto cap:{admission.source,admission.paired,admission.availability})if(cap&&cap->limits.count("timeout-ms"))
                until=std::min(until,started+std::chrono::milliseconds(limit(*cap,"timeout-ms")));
            if(std::chrono::steady_clock::now()>=until)return {110,{}};
            if(request.operation==command::activate)
            {
                // Client pending max-inflight is NOT an endpoint count. The
                // explicit profile/table capacity bounds client instances;
                // each real client independently enforces its pending requests.
                if(request.kind==role::service)
                {
                    std::size_t live{};for(auto const& p:history_.rows())
                        if(!p.second.retired&&p.second.primary_alias==source.alias)++live;
                    if(live>=limit(source,"max-services"))return {11,{}};
                }
                if(!charge(source,0,charge_kind::activate))return {11,{}};
                error=history_.reserve(request.owner,request.kind,admission.paired_owner.source,source,*admission.paired,row);
                if(error)return {error,{}};
            }
            else if(request.operation==command::submit||request.operation==command::respond)
            {
                auto const& grant=request.operation==command::respond?*admission.paired:source;
                auto bytes=original.data.data()+ipc::rmw_service::header_bytes;
                if(request.body_bytes>limit(grant,request.operation==command::respond?"max-response-bytes":"max-request-bytes"))return {90,{}};
                auto body_error=backend_.validate_owned_rmw_service_body(source,request.operation==command::respond,bytes,static_cast<std::size_t>(request.body_bytes));
                if(body_error)return {body_error,{}};
                if(!charge(grant,static_cast<std::size_t>(request.body_bytes),charge_kind::operation))return {11,{}};
            }
            else if(request.operation==command::available)
            {
                if(request.wait_ns>limit(alias,"max-wait-ns"))return {22,{}};
                if(request.wait_ns&&(session_fd_<0||worker_pidfd_<0||!stopping_))return {95,{}};
                if(!charge(alias,ipc::client_availability::bytes,charge_kind::operation))return {11,{}};
            }
            if(std::chrono::steady_clock::now()>=until)
            {if(request.operation==command::activate&&row)(void)history_.retire(row->owner,row->kind,110);return {110,{}};}
            auto result=backend_.owned_rmw_service(admission,request,
                original.data.data()+ipc::rmw_service::header_bytes,static_cast<std::size_t>(request.body_bytes),until);
            // Arm before ANY validation/quota/allocation following a successful
            // genuine peek. The result vector moves without another body copy.
            if(!result.error&&request.operation==command::peek)
            {delivery_armed_=true;delivered_owner_=request.owner;delivered_incarnation_=request.incarnation;facts_={};}
            if(result.error)
            {
                if(request.operation==command::activate&&row)(void)history_.retire(row->owner,row->kind,result.error);
                // Empty peek11 guarantees no reservation/accepted visible item,
                // although a fully validated late abandoned response may have
                // been disposed by one Native take. It does not replay a send.
                if(!owned_service_idle_reply(request.operation,result))close_session_=true;
                return result;
            }
            control reply{};
            if(ipc::rmw_service::endpoint_command(request.operation))
            {
                ipc::rmw_endpoint::packet endpoint{};
                if(!ipc::rmw_service::associated_endpoint(result.payload.data(),result.payload.size(),request,reply,endpoint))error=13;
                if(request.operation==command::retire&&row)(void)history_.retire(row->owner,row->kind,125);
            }
            else if(request.operation==command::peek)
            {
                ipc::rmw_metadata::header information{};
                if(!ipc::rmw_service::associated_delivery(result.payload.data(),result.payload.size(),request,reply,information))error=13;
                else
                {
                    bool control_record=ipc::event_control(information.kind);
                    auto bytes=result.payload.data()+ipc::rmw_service::minimum_delivery_bytes;
                    auto count=static_cast<std::size_t>(information.payload_bytes);
                    if(!control_record)error=backend_.validate_owned_rmw_service_body(source,request.kind==role::client,bytes,count);
                    if(!error&&!control_record&&count>limit(source,request.kind==role::client?"max-response-bytes":"max-request-bytes"))error=90;
                    else if(!error&&request.kind==role::client)
                    {
                        auto const& observer=*admission.paired;
                        auto uwre=count+ipc::event_header_bytes;
                        std::uint32_t mask{};error=ros::service_observer_kinds(observer,mask);
                        if(!error&&(!(mask&ipc::event_bit(information.kind))||uwre>limit(observer,"max-event-bytes")))error=13;
                        if(!error&&(!charge(observer,uwre,charge_kind::preview_operation)||
                            (!control_record&&!charge(source,count,charge_kind::preview_bytes))))error=11;
                        if(!error){if(!control_record)(void)charge(source,count,charge_kind::bytes);(void)charge(observer,uwre,charge_kind::operation);}
                    }
                    else if(!error&&!control_record&&!charge(source,count,charge_kind::operation))error=11;
                }
            }
            else if(!ipc::rmw_service::associated_ack(result.payload.data(),result.payload.size(),request,reply))error=13;
            if(!error&&std::chrono::steady_clock::now()>=until)error=110;
            if(error)
            {
                delivery_failed(error);
                if(request.operation!=command::peek)close_session_=true;
                return {error,{}};
            }
            return result;
        }
    };
}
