/* SPDX-License-Identifier: Apache-2.0 */
#pragma once
#include "../../compat/node.h"
#include "service_proof.hpp"
#include "selection.hpp"
#include "native_rcl_service_registry.h"
#include "custom_owned_service_records.h"
/** Dynamic full-native Service provider, all sources private and additive.
 * It retains ONE actual rcl client/service and original Node ownership. Every
 * canonical history slab/native object is reserved before init. Middleware can
 * allocate while deserializing an ORIGINAL unbounded field: canonical caps do
 * not preempt foreign code; real RSS/cgroup/deadline/drain remain mandatory.
 * No old81/ephemeral client, projected native schema, fake timestamp/GID or
 * auto replay/cancel is used. Positive Native wait uses the actual integrated shared graph domain and
 * authenticated borrowed transport lifetime. Missing that admission remains95.
 */
namespace uwvm2::ros_gateway::default_control_gateway::detail
{
    using namespace ::uwvm2::ros_gateway::native_rcl_service;
    using endpoint=::uwvm2::ros_gateway::native_rcl_service_registry::endpoint;
    using records=::uwvm2::ros_gateway::custom_owned_services::detail::records;
    using state=::uwvm2::ros_gateway::native_rcl_service::state;
    template<class Service,bool Client>class typed_endpoint final:public endpoint
    {
        using Request=typename Service::Request;using Response=typename Service::Response;
        using Proof=ServiceProof<Service>;
        ::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node_;std::shared_ptr<rcl_node_t> rcl_node_;
        std::shared_ptr<uwvm_rmw_cpp::Registry const> original_registry_;
        std::shared_ptr<Proof const> codecs_;uwvm_rmw_codec const* root_{};
        ros::capability_spec cap_;wire::packet association_{};records records_;std::shared_ptr<report> report_;
        std::mutex mutex_;bool initialized_{},finalized_{},retired_{};std::uint32_t failed_{};
        rcl_client_t client_{rcl_get_zero_initialized_client()};rcl_service_t service_{rcl_get_zero_initialized_service()};
        // Every history value has its ORIGINAL generated type. Indirection is
        // deliberate: string/vector copies can throw; unique_ptr swaps cannot.
        std::vector<std::unique_ptr<Request>> requests_;
        std::vector<std::unique_ptr<Response>> responses_;std::unique_ptr<Response> response_;
        std::vector<std::uint8_t> encoded_;
        typed_endpoint(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node,std::shared_ptr<rcl_node_t> native,
            std::shared_ptr<uwvm_rmw_cpp::Registry const> original,std::shared_ptr<Proof const> codecs,uwvm_rmw_codec const* root,
            ros::capability_spec cap,wire::binding owner,std::uint64_t id,std::size_t history,std::shared_ptr<report> diagnostics):
            node_{std::move(node)},rcl_node_{std::move(native)},original_registry_{std::move(original)},codecs_{std::move(codecs)},root_{root},cap_{std::move(cap)},
            records_{history,std::max(root->request->maximum_wire_bytes,root->response->maximum_wire_bytes),owner,id},report_{std::move(diagnostics)},requests_(history),responses_(history),response_{std::make_unique<Response>()},
            encoded_(std::max(root->request->maximum_wire_bytes,root->response->maximum_wire_bytes))
        {for(std::size_t i{};i<history;++i){requests_[i]=std::make_unique<Request>();responses_[i]=std::make_unique<Response>();}
         association_.owner=owner;association_.incarnation=id;association_.kind=Client?wire::role::client:wire::role::service;}
        bool owns(wire::binding const& owner,std::uint64_t id)const noexcept{return records_.owns(owner,id);}
        std::uint32_t live(wire::binding const& owner,std::uint64_t id)const noexcept
        {if(!owns(owner,id))return 13;if(retired_||!initialized_||!rcl_node_is_valid(rcl_node_.get()))return 125;return failed_;}
        std::uint32_t fail(std::uint32_t n,std::uint64_t ticket=0)noexcept
        {if(!failed_){failed_=n;report_->error(n);(void)records_.control(n,ticket);}return n;}
        static std::uint64_t mint_ticket()noexcept
        {try{return new_service_id();}catch(...){return 0;}}
        std::size_t pending()const noexcept
        {std::size_t n{};for(std::size_t i{};i<records_.used();++i){auto const& r=records_.at(i);if(!r.discard&&!r.response_seen&&!r.response_attempted&&r.phase!=state::completed)++n;}return n;}
        void expire()noexcept
        {if(failed_)return;auto now=std::chrono::steady_clock::now();for(std::size_t i{};i<records_.used();++i){auto& r=records_.at(i);
            if(!r.discard&&!r.response_seen&&!r.response_attempted&&r.phase!=state::completed&&now>=r.deadline){auto error=records_.request_error(i,110);if(error){fail(error,r.ticket);return;}}}}
        std::uint32_t query(wire::packet& output)noexcept
        {
#if UWVM_NATIVE_RCL_SERVICE_API && UWVM_ROS2_HAVE_COMPLETE_RMW_QOS
            auto p=association_;p.profiles=2;p.gid_size=0;p.gid.fill(0);
            auto a=Client?rcl_client_request_publisher_get_actual_qos(&client_):rcl_service_request_subscription_get_actual_qos(&service_);
            auto b=Client?rcl_client_response_subscription_get_actual_qos(&client_):rcl_service_response_publisher_get_actual_qos(&service_);
            if(!a||!b||!native_rmw_metadata::actual_profile(*a,p.actual[0])||!native_rmw_metadata::actual_profile(*b,p.actual[1]))return 95;
            if constexpr(Client){rmw_gid_t gid{};auto rmw=rcl_client_get_rmw_handle(&client_);if(!rmw)return 5;auto result=rmw_get_gid_for_client(rmw,&gid);
                if(result==RMW_RET_UNSUPPORTED)return 95;if(result!=RMW_RET_OK)return 5;
                if(!native_rmw_metadata::actual_gid(gid,p.gid,p.gid_size)||p.gid_size!=sizeof(rmw_gid_t::data))return 95;}
            if(!wire::valid_response(p))return 22;output=p;return 0;
#else
            (void)output;return 95;
#endif
        }
        std::uint32_t finalize()noexcept
        {
            if(finalized_||!initialized_)return 0;finalized_=true; // set BEFORE the one real native fini
#if UWVM_NATIVE_RCL_SERVICE_API
            try{auto result=Client?rcl_client_fini(&client_,rcl_node_.get()):rcl_service_fini(&service_,rcl_node_.get());
                report_->call(operation::fini,result);report_->cleanup(true,result);initialized_=false;return result==RCL_RET_OK?0u:5u;
            }catch(...){report_->cleanup(false,0);initialized_=false;return 5;}
#else
            return 95;
#endif
        }
    public:
        bool owns_native()const noexcept override{return initialized_&&!finalized_;}
        typed_endpoint(typed_endpoint const&)=delete;typed_endpoint& operator=(typed_endpoint const&)=delete;
        ~typed_endpoint(){auto error=finalize();if(error)report_->error(error);}
        /** Publish outputs only after actual native init, exact resolved name,
         * both actual native QoS getters and native client writer-GID association.
         * Every local owner, report, canonical history/body buffer and ORIGINAL native object exists
         * BEFORE init. The caller reserves its own registry slot beforehand.
         * Unsupported compiled SDK returns95 before native initialization.
         */
        static std::uint32_t create(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,
            std::shared_ptr<uwvm_rmw_cpp::Registry const> const& original,std::shared_ptr<Proof const> const& codecs,ros::capability_spec const& cap,
            wire::binding const& owner,std::uint64_t id,std::size_t history,std::shared_ptr<report> const& diagnostics,
            std::shared_ptr<endpoint>& output,wire::packet& observed)noexcept
        {
#if !UWVM_NATIVE_RCL_SERVICE_API || !UWVM_ROS2_HAVE_COMPLETE_RMW_QOS
            (void)node;(void)original;(void)codecs;(void)cap;(void)owner;(void)id;(void)history;(void)diagnostics;(void)output;(void)observed;return 95;
#else
            if(!diagnostics||!diagnostics->begin())return 17;
            if(!node||!codecs||!wire::valid(owner)||!id||cap.operation!=(Client?"service.call":"service.advertise")){diagnostics->error(13);return 13;}
            try{
                auto root=codecs->root();auto actual=rosidl_typesupport_cpp::get_service_type_support_handle<Service>();
                if(!root||root->generated_support!=actual){diagnostics->error(13);return 13;}
                auto validation=root->validate(root,actual);if(validation){diagnostics->error(validation);return validation;}
                if(!codecs->selected(cap)){diagnostics->error(13);return 13;}
                auto until=std::chrono::steady_clock::now()+std::chrono::milliseconds(limit(cap,"timeout-ms"));
                auto maximum=std::max(root->request->maximum_wire_bytes,root->response->maximum_wire_bytes);
                if(!records::valid_options(history,maximum)||history<limit(cap,"max-inflight")){diagnostics->error(90);return 90;}
                auto native=node->get_node_base_interface()->get_shared_rcl_node_handle();if(!native||!rcl_node_is_valid(native.get())){diagnostics->error(125);return 125;}
                auto holder=std::shared_ptr<typed_endpoint>{new typed_endpoint{node,native,original,codecs,root,cap,owner,id,history,diagnostics}};
                auto configured=ros2_qos::configured(cap);auto requested=native_rmw_metadata::profile(configured);
                if(std::chrono::steady_clock::now()>=until){diagnostics->error(110);return 110;}
                rcl_ret_t result;
                if constexpr(Client){auto options=rcl_client_get_default_options();options.qos=requested;result=rcl_client_init(&holder->client_,native.get(),actual,cap.resource.c_str(),&options);}
                else{auto options=rcl_service_get_default_options();options.qos=requested;result=rcl_service_init(&holder->service_,native.get(),actual,cap.resource.c_str(),&options);}
                diagnostics->call(operation::init,result);if(result!=RCL_RET_OK){diagnostics->error(5);return 5;}
                holder->initialized_=true;diagnostics->initialized();
                if(std::chrono::steady_clock::now()>=until){diagnostics->error(110);return 110;}
                auto name=Client?rcl_client_get_service_name(&holder->client_):rcl_service_get_service_name(&holder->service_);
                if(!name||std::string_view{name}!=cap.resource){diagnostics->error(13);return 13;}
                wire::packet p{};auto error=holder->query(p);if(error){diagnostics->error(error);return error;}
                wire::qos explicit_requested{};
                if(!native_rmw_metadata::actual_profile(requested,explicit_requested)||!wire::same(explicit_requested,p.actual[0])||!wire::same(explicit_requested,p.actual[1])){diagnostics->error(95);return 95;}
                if(std::chrono::steady_clock::now()>=until){diagnostics->error(110);return 110;}
                holder->association_=p;observed=p;output=std::move(holder);return 0;
            }catch(std::bad_alloc const&){diagnostics->error(12);return 12;}catch(...){diagnostics->error(5);return 5;}
#endif
        }
        std::uint32_t inspect(wire::binding const& owner,std::uint64_t id,wire::packet& output)noexcept
        {
            std::unique_lock<std::mutex> g{mutex_,std::try_to_lock};if(!g.owns_lock())return 17;auto error=live(owner,id);if(error)return error;wire::packet p{};error=query(p);if(error)return error;
            if(!wire::same(p.actual[0],association_.actual[0])||!wire::same(p.actual[1],association_.actual[1])||p.gid_size!=association_.gid_size||p.gid!=association_.gid)return 13;
            p.operation=wire::command::inspect;output=p;return 0;
        }
        /** One genuine native readiness observation on THIS retained client.
         * No new client, graph-list lookup, request send, sleep or callback spin.
         * Caller must authenticate/lease BOTH current handles before entry; the
         * source binding/current incarnation and the independent availability
         * binding must remain live for the whole call. service.call grants none
         * of this authority. Outputs commit only after a successful native query.
         * Missing compiled API or unsupported native RMW returns95 unchanged.
         * Finite waiting, Host routing and per-grant quotas are separate work. */
        std::uint32_t available(wire::binding const& owner,std::uint64_t id,
            wire::binding const& availability_owner,ros::capability_spec const& availability_cap,bool& output)noexcept
        {
            if constexpr(!Client){(void)owner;(void)id;(void)availability_owner;(void)availability_cap;(void)output;return 95;}
            else{
#if UWVM_NATIVE_RCL_SERVICE_API && UWVM_NATIVE_RCL_AVAILABILITY_API && UWVM_ROS2_HAVE_COMPLETE_RMW_QOS
                std::unique_lock<std::mutex> g{mutex_,std::try_to_lock};if(!g.owns_lock())return 17;auto error=live(owner,id);if(error)return error;
                try{
                    if(!same_domain(owner,availability_owner)||availability_owner.source==owner.source||!uwvm_default_control_gateway_detail::availability_selected(availability_cap,cap_))return 13;
                    // Reinspect the same retained native instance, not a probe
                    // endpoint. Independently retain requested/actual QoS facts.
                    auto until=std::chrono::steady_clock::now()+std::chrono::milliseconds(limit(availability_cap,"timeout-ms"));
                    wire::packet observed{};error=query(observed);if(error)return error;
                    if(!wire::same(observed.actual[0],association_.actual[0])||!wire::same(observed.actual[1],association_.actual[1])||
                        observed.gid_size!=association_.gid_size||observed.gid!=association_.gid)return 13;
                    auto configured=ros2_qos::configured(availability_cap);wire::qos declared{};
                    if(!native_rmw_metadata::actual_profile(native_rmw_metadata::profile(configured),declared)||
                        !wire::same(declared,observed.actual[0])||!wire::same(declared,observed.actual[1]))return 13;
                    if(std::chrono::steady_clock::now()>=until)return 110;
                    bool actual{};auto result=rcl_service_server_is_available(rcl_node_.get(),&client_,&actual);report_->call(operation::available,result);
                    if(result==RCL_RET_UNSUPPORTED)return 95;if(result==RCL_RET_BAD_ALLOC)return 12;if(result!=RCL_RET_OK)return 5;
                    if(std::chrono::steady_clock::now()>=until)return 110;
                    output=actual;return 0;
                }catch(std::bad_alloc const&){return 12;}catch(...){return 5;}
#else
                (void)owner;(void)id;(void)availability_owner;(void)availability_cap;(void)output;return 95;
#endif
            }
        }
        /** Genuine finite graph-event wait on THIS retained original RCL client.
         * Wait/recheck ordering follows ros2/rclcpp ClientBase::wait_for_service_nanoseconds
         * (Copyright2015 Open Source Robotics Foundation, Apache-2.0); its complete
         * original source/license and fixed Git provenance accompany this cohort.
         * The independent capability/lifetime/deadline/error logic is added here.
         * Zero duration delegates byte-for-byte to the original one-query API.
         * Positive duration requires independently admitted actual native socket,
         * worker pidfd/stop lifetime and the SAME actual Node/profile domain.
         * The real predicate, not graph names/events, determines readiness.
         * Requested elapsed time with a final false predicate is success/false;
         * RPC deadline, disconnect or shutdown is an error with untouched output.
         * No second client, request send, sleep or poll-count clock exists here.
         */
        std::uint32_t available_until(wire::binding const& owner,std::uint64_t id,
            wire::binding const& availability_owner,ros::capability_spec const& availability_cap,
            std::uint64_t wait_ns,ipc::deadline transport_until,
            native_rcl_graph_wait::admission const& waiting,bool& output)noexcept
        {
            if(!wait_ns)return available(owner,id,availability_owner,availability_cap,output);
            if constexpr(!Client){(void)owner;(void)id;(void)availability_owner;(void)availability_cap;(void)transport_until;(void)waiting;(void)output;return 95;}
            else{
#if UWVM_NATIVE_RCL_SERVICE_API && UWVM_NATIVE_RCL_AVAILABILITY_API && UWVM_ROS2_HAVE_COMPLETE_RMW_QOS && UWVM_NATIVE_RCL_FINITE_GRAPH_WAIT_API
                std::unique_lock<std::mutex> g{mutex_,std::try_to_lock};if(!g.owns_lock())return 17;
                auto error=live(owner,id);if(error)return error;
                try
                {
                    if(!same_domain(owner,availability_owner)||availability_owner.source==owner.source||!uwvm_default_control_gateway_detail::availability_selected(availability_cap,cap_))return 13;
                    if(wait_ns>limit(availability_cap,"max-wait-ns")||wait_ns>60000000000ULL)return 22;
                    if(!waiting.graph||!waiting.graph->same_node(node_)||!waiting.cancel.valid())return 95;
                    error=waiting.graph->status();if(error)return error;
                    auto entered=std::chrono::steady_clock::now();
                    auto until=std::min(transport_until,entered+std::chrono::milliseconds(limit(availability_cap,"timeout-ms")));
                    auto requested=entered+std::chrono::nanoseconds{static_cast<std::int64_t>(wait_ns)};
                    error=native_rcl_graph_wait::check(waiting.cancel,node_,until);if(error)return error;
                    // Full SAME-instance actual QoS/GID reinspection precedes
                    // the first native readiness query and graph registration.
                    wire::packet observed{};error=query(observed);
                    auto lifetime_error=native_rcl_graph_wait::check(waiting.cancel,node_,until);if(lifetime_error)return lifetime_error;
                    if(error)return error;
                    if(!wire::same(observed.actual[0],association_.actual[0])||!wire::same(observed.actual[1],association_.actual[1])||
                        observed.gid_size!=association_.gid_size||observed.gid!=association_.gid)return 13;
                    auto configured=ros2_qos::configured(availability_cap);wire::qos declared{};
                    if(!native_rmw_metadata::actual_profile(native_rmw_metadata::profile(configured),declared)||
                        !wire::same(declared,observed.actual[0])||!wire::same(declared,observed.actual[1]))return 13;
                    auto sample=[&](bool& actual)->std::uint32_t
                    {
                        auto e=native_rcl_graph_wait::check(waiting.cancel,node_,until);if(e)return e;
                        e=waiting.graph->status();if(e)return e;
                        auto result=rcl_service_server_is_available(rcl_node_.get(),&client_,&actual);
                        report_->call(operation::available,result);
                        auto after=native_rcl_graph_wait::check(waiting.cancel,node_,until);if(after)return after;
                        if(result==RCL_RET_UNSUPPORTED)return 95;if(result==RCL_RET_BAD_ALLOC)return 12;if(result!=RCL_RET_OK)return 5;
                        return 0;
                    };
                    auto finish=[&](bool actual)->std::uint32_t
                    {
                        auto e=native_rcl_graph_wait::check(waiting.cancel,node_,until);if(e)return e;
                        e=waiting.graph->status();if(e)return e;output=actual;return 0;
                    };
                    bool actual{};error=sample(actual);if(error)return error;
                    if(actual)return finish(true);
                    if(std::chrono::steady_clock::now()>=requested)return finish(false);
                    rclcpp::Event::SharedPtr event;
                    error=waiting.graph->loan(node_,event);
                    lifetime_error=native_rcl_graph_wait::check(waiting.cancel,node_,until);if(lifetime_error)return lifetime_error;
                    if(error)return error;
                    for(;;)
                    {
                        // Recheck after registration as well as EVERY actual
                        // graph wait, including timeout/spurious/missed events.
                        error=sample(actual);if(error)return error;
                        if(actual)return finish(true);
                        auto now=std::chrono::steady_clock::now();
                        if(now>=requested)return finish(false);
                        auto duration=native_rcl_graph_wait::slice(now,requested,until);
                        error=waiting.graph->wait(event,duration);
                        lifetime_error=native_rcl_graph_wait::check(waiting.cancel,node_,until);if(lifetime_error)return lifetime_error;
                        if(error)return error;
                    }
                }
                catch(std::bad_alloc const&){return 12;}catch(...){return 5;}
#else
                (void)owner;(void)id;(void)availability_owner;(void)availability_cap;(void)transport_until;(void)waiting;(void)output;return 95;
#endif
            }
        }
        /** Client-only: reserve ticket, history, body and decode before the single
         * rcl_send_request. Error/timeout is never permission to replay it. */
        std::uint32_t submit(wire::binding const& owner,std::uint64_t id,std::uint8_t const* bytes,std::size_t size,std::uint64_t& ticket)noexcept
        {
            if constexpr(!Client){(void)owner;(void)id;(void)bytes;(void)size;(void)ticket;return 95;}
            else{
#if UWVM_NATIVE_RCL_SERVICE_API
                std::unique_lock<std::mutex> g{mutex_,std::try_to_lock};if(!g.owns_lock())return 17;auto error=live(owner,id);if(error)return error;expire();if(failed_)return failed_;
                if(!bytes&&size)return 22;if(size>root_->request->maximum_wire_bytes)return 90;
                if(!records_.can_reserve()||pending()>=limit(cap_,"max-inflight"))return 90;
                error=codecs_->validate(false,bytes,size);if(error)return error;
                error=root_->request->decode(bytes,size,requests_[records_.used()].get());if(error)return error;
                auto selected_ticket=mint_ticket();if(!selected_ticket)return 90;auto i=records_.reserve(selected_ticket);if(i==records::no_slot)return 90;
                auto& r=records_.at(i);r.deadline=std::chrono::steady_clock::now()+std::chrono::milliseconds(limit(cap_,"timeout-ms"));
                r.native_attempted=true;report_->ticket(selected_ticket);std::int64_t native_sequence{};
                auto result=rcl_send_request(&client_,requests_[i].get(),&native_sequence);report_->call(operation::send,result);
                if(result!=RCL_RET_OK)return fail(result==RCL_RET_TIMEOUT?110:5,selected_ticket);
                // rcl_send_request supplies only this actual signed sequence.
                // Do NOT reinterpret the independently queried client GID as a
                // request writer GUID. This private rcl_client is the exclusive
                // response consumer; the same native take later supplies the
                // complete request_id. Refuse sequence reuse across history.
                r.identity.sequence_number=native_sequence;r.sequence_known=true;
                if(records_.find_sequence(native_sequence,i)!=records::no_slot)return fail(22,selected_ticket);
                r.phase=state::pending;
                if(std::chrono::steady_clock::now()>=r.deadline){(void)records_.request_error(i,110);return 110;}
                ticket=selected_ticket;return 0;
#else
                (void)owner;(void)id;(void)bytes;(void)size;(void)ticket;return 95;
#endif
            }
        }
        /** At most ONE direct native take. No rclcpp executor sees these handles.
         * Every native value is complete-codec encoded before tombstone/expiry
         * discard; no old accepted record is substituted for this take's info. */
        std::uint32_t poll(wire::binding const& owner,std::uint64_t id)noexcept
        {
#if UWVM_NATIVE_RCL_SERVICE_API
            std::unique_lock<std::mutex> g{mutex_,std::try_to_lock};if(!g.owns_lock())return 17;auto error=live(owner,id);if(error)return error;expire();if(failed_)return failed_;
            if(!records_.can_take())return 90;rmw_service_info_t native{};std::size_t i=records::no_slot;
            if constexpr(!Client){if(!records_.can_reserve()||pending()>=limit(cap_,"max-inflight"))return 90;auto t=mint_ticket();if(!t)return 90;i=records_.reserve(t);if(i==records::no_slot)return 90;}
            else{bool waiting{};for(std::size_t n{};n<records_.used();++n)if(records_.at(n).sequence_known&&!records_.at(n).response_seen)waiting=true;if(!waiting)return 11;}
            rcl_ret_t result;
            if constexpr(Client)result=rcl_take_response_with_info(&client_,&native,response_.get());
            else result=rcl_take_request_with_info(&service_,&native,requests_[i].get());
            report_->call(operation::take,result);
            if(result==(Client?RCL_RET_CLIENT_TAKE_FAILED:RCL_RET_SERVICE_TAKE_FAILED)){if constexpr(!Client)records_.release_unattempted(i);return 11;}
            if(result!=RCL_RET_OK){if constexpr(!Client)records_.at(i).native_attempted=true;return fail(5);}
            if constexpr(Client)i=records_.find_sequence(native.request_id.sequence_number);
            std::size_t length=SIZE_MAX;auto part=Client?root_->response:root_->request;
            error=part->encode(Client?static_cast<void const*>(response_.get()):static_cast<void const*>(requests_[i].get()),encoded_.data(),part->maximum_wire_bytes,&length);
            if(error||length==SIZE_MAX||length>part->maximum_wire_bytes){report_->unmatched(native);return fail(error?error:22,i==records::no_slot?0:records_.at(i).ticket);}
            error=codecs_->validate(Client,encoded_.data(),length);
            if(error){report_->unmatched(native);return fail(error,i==records::no_slot?0:records_.at(i).ticket);}
            if(i==records::no_slot){report_->unmatched(native);return fail(22);}
            auto& r=records_.at(i);r.native_attempted=true;
            if constexpr(!Client){r.identity=native.request_id;r.sequence_known=true;r.identity_known=true;r.deadline=std::chrono::steady_clock::now()+std::chrono::milliseconds(limit(cap_,"timeout-ms"));}
            // The full actual object has now been validated even for a tombstone.
            if constexpr(Client){if(r.response_seen){report_->unmatched(native);return fail(22,r.ticket);}responses_[i].swap(response_);r.identity=native.request_id;r.identity_known=true;r.response_seen=true;}
            else if(records_.find(r.identity,i)!=records::no_slot){report_->unmatched(native);return fail(22,r.ticket);}
            r.native_info=native;
            if(r.discard)return 0;
            if(length)std::copy_n(encoded_.data(),length,r.body.data());
            info::header h{};h.kind=Client?events::event_kind::service_response:events::event_kind::service_request;h.owner=owner;h.incarnation=id;h.sequence=1;h.correlation=r.ticket;h.payload_bytes=length;
            error=native_rmw_metadata::service_record(native,h);if(error)return fail(error,r.ticket);
            error=records_.enqueue(i,h,length);return error?fail(error,r.ticket):0u;
#else
            (void)owner;(void)id;return 95;
#endif
        }
        /** Local cancellation only. Native cancellation does not exist in this
         * API. Retain the complete identity until late response/full decode or
         * endpoint retirement. Whole history never reuses this ticket/native ID. */
        std::uint32_t abandon(wire::binding const& owner,std::uint64_t id,std::uint64_t ticket)noexcept
        {
            if constexpr(!Client){(void)owner;(void)id;(void)ticket;return 95;}
            else{std::unique_lock<std::mutex> g{mutex_,std::try_to_lock};if(!g.owns_lock())return 17;auto error=live(owner,id);if(error)return error;auto i=records_.ticket(ticket);if(i==records::no_slot)return 9;
                auto& r=records_.at(i);if(r.discard||r.phase==state::completed)return 125;return records_.request_error(i,125);}
        }
        /** Server-only. A separately authenticated service.respond owner is
         * REQUIRED in the same live session/instance/generation and exact native
         * service/type/QoS. The advertise owner is not response authority. */
        std::uint32_t respond(wire::binding const& owner,std::uint64_t id,wire::binding const& responder,
            ros::capability_spec const& response_cap,std::uint64_t ticket,std::uint8_t const* bytes,std::size_t size)noexcept
        {
            if constexpr(Client){(void)owner;(void)id;(void)responder;(void)response_cap;(void)ticket;(void)bytes;(void)size;return 95;}
            else{
#if UWVM_NATIVE_RCL_SERVICE_API
                std::unique_lock<std::mutex> g{mutex_,std::try_to_lock};if(!g.owns_lock())return 17;auto error=live(owner,id);if(error)return error;expire();if(failed_)return failed_;
                try{
                if(!same_domain(owner,responder)||responder.source==owner.source||response_cap.operation!="service.respond"||response_cap.alias==cap_.alias||
                    response_cap.resource!=cap_.resource||!codecs_->selected(response_cap))return 13;
                auto configured=ros2_qos::configured(response_cap);wire::qos declared{};
                if(!native_rmw_metadata::actual_profile(native_rmw_metadata::profile(configured),declared)||!wire::same(declared,association_.actual[0])||!wire::same(declared,association_.actual[1]))return 13;
                if(!bytes&&size)return 22;if(size>root_->response->maximum_wire_bytes||size>limit(response_cap,"max-response-bytes"))return 90;
                auto i=records_.ticket(ticket);if(i==records::no_slot)return 9;auto& r=records_.at(i);
                if(r.discard||r.response_attempted)return 125;if(r.phase!=state::delivered)return 17;
                error=codecs_->validate(true,bytes,size);if(error)return error;
                error=root_->response->decode(bytes,size,responses_[i].get());if(error)return error;
                auto delivery_until=std::min(r.deadline,std::chrono::steady_clock::now()+std::chrono::milliseconds(limit(response_cap,"timeout-ms")));
                if(std::chrono::steady_clock::now()>=delivery_until)return 110;
                // Set attempted BEFORE entering middleware: even a native timeout
                 // or uncertain failure cannot cause a second response send.
                r.response_attempted=true;r.response_identity=r.identity;
                auto result=rcl_send_response(&service_,&r.response_identity,responses_[i].get());report_->call(operation::respond,result);r.phase=state::completed;
                if(result!=RCL_RET_OK){auto code=result==RCL_RET_TIMEOUT?110u:5u;(void)records_.request_error(i,code);report_->error(code);return code;}
                return std::chrono::steady_clock::now()>=delivery_until?110u:0u;
                }catch(std::bad_alloc const&){return 12;}catch(...){return 5;}
#else
                (void)owner;(void)id;(void)responder;(void)response_cap;(void)ticket;(void)bytes;(void)size;return 95;
#endif
            }
        }
        std::uint32_t peek(wire::binding const& owner,std::uint64_t id,std::size_t writer,snapshot& out,std::size_t capacity=events::max_payload_bytes)noexcept
        {std::unique_lock<std::mutex> g{mutex_,std::try_to_lock};if(!g.owns_lock())return 17;if(!owns(owner,id))return 13;if(!retired_)expire();return records_.peek(owner,id,writer,out,capacity);}
        std::uint32_t commit(wire::binding const& owner,std::uint64_t id,std::uint64_t sequence,std::uint64_t token)noexcept
        {std::unique_lock<std::mutex> g{mutex_,std::try_to_lock};if(!g.owns_lock())return 17;if(!owns(owner,id))return 13;if(!retired_)expire();return records_.commit(owner,id,sequence,token);}
        std::uint32_t cancel(wire::binding const& owner,std::uint64_t id,std::uint64_t sequence,std::uint64_t token)noexcept
        {std::unique_lock<std::mutex> g{mutex_,std::try_to_lock};if(!g.owns_lock())return 17;return records_.cancel(owner,id,sequence,token);}
        std::uint32_t retire(wire::binding const& owner,std::uint64_t id)noexcept
        {std::unique_lock<std::mutex> g{mutex_,std::try_to_lock};if(!g.owns_lock())return 17;if(!owns(owner,id))return 13;if(retired_)return 125;retired_=true;auto error=finalize();auto queued=records_.retire(error);if(!error)error=queued;if(error)report_->error(error);return error;}
    };
    template<class Service>using client=typed_endpoint<Service,true>;
    template<class Service>using server=typed_endpoint<Service,false>;
}
