/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * Retained, explicitly paired direct-RCL service transport.
 *
 * This does not install a Host import, Gateway handler or capability. Every
 * caller must hold the actual source AND original peer leases for the WHOLE
 * call. Availability needs a third independent current lease. The values below
 * associate those already authenticated objects; they are not credentials.
 *
 * One registry belongs to one real instance/generation and its native sessions.
 * It retains failed/retired keys and diagnostics until destruction. Rebuilding
 * a registry while its original raw owners survive is forbidden. All physical
 * teardown must run under the external owner's finite process deadline: a
 * post-call clock check does not preempt foreign middleware.
 */
#pragma once
#include "compat/node.h"
#include "native_rcl_service.h"
#include "../common/owned_service_storage.h"
#include <uwvm2/imported/ros/common/rmw_service_bindings.h>
#include <uwvm2/imported/ros/ipc/rmw_service.h>
#include <map>
#include <mutex>
#include <memory>
#include <utility>
#include <functional>

namespace uwvm2::ros_gateway::native_rcl_service_registry
{
    namespace transport=::uwvm2::imported::ros::ipc::rmw_service;
    namespace native=::uwvm2::ros_gateway::native_rcl_service;
    namespace wire=::uwvm2::imported::ros::ipc::rmw_endpoint;
    namespace metadata=::uwvm2::imported::ros::ipc::rmw_metadata;
    using pair_history=::uwvm2::imported::ros::rmw_service_bindings;

    struct admitted
    {
        ros::capability_spec const* source{};
        ros::capability_spec const* paired{};
        wire::binding paired_owner{};
        ros::capability_spec const* availability{};
        wire::binding availability_owner{};
        // Native-only borrowed facts. Never decoded from UMRC or Guest memory.
        native_rcl_graph_wait::admission waiting{};
    };
    struct limits
    {
        // These are finite local storage limits, never policy grants. The
        // owning process must separately admit their complete physical memory.
        std::size_t maximum_entries{},history_per_endpoint{};
        bool valid()const noexcept
        {return maximum_entries&&maximum_entries<=pair_history::maximum_entries&&
            history_per_endpoint&&history_per_endpoint<=256;}
    };
    struct diagnostics
    {
        std::uint32_t first_error{},last_error{},last_submit_error{},last_respond_error{},
            last_abandon_error{},last_commit_error{},last_cancel_error{},last_retire_error{};
        bool submit_attempted{},respond_attempted{},abandon_attempted{},commit_attempted{},
            cancel_attempted{},retire_attempted{},cleanup_claimed{},last_commit_completed{};
        std::uint64_t pending_sequence{},pending_reservation{},last_ticket{},
            successful_peeks{},late_effects{};
    };
    inline void record_error(diagnostics& d,std::uint32_t error)noexcept
    {if(error){if(!d.first_error)d.first_error=error;d.last_error=error;}}
    inline void record_cancel(diagnostics& d,std::uint32_t error)noexcept
    {d.cancel_attempted=true;d.last_cancel_error=error;record_error(d,error);}
    inline void record_commit(diagnostics& d,std::uint32_t error)noexcept
    {d.commit_attempted=true;d.last_commit_error=error;d.last_commit_completed=!error;record_error(d,error);}
    inline void record_retire(diagnostics& d,std::uint32_t error)noexcept
    {d.retire_attempted=true;d.last_retire_error=error;record_error(d,error);}
    inline bool exact_cap(ros::capability_spec const& a,ros::capability_spec const& b)noexcept
    {return a.alias==b.alias&&a.operation==b.operation&&a.resource==b.resource&&
        a.type==b.type&&a.type_identity==b.type_identity&&a.limits==b.limits;}

    /** A genuinely implemented typed factory may extend the finite family.
     * It must retain the supplied actual Registry, validate the complete Native
     * graph and any explicit canonical identity, allocate before native create,
     * and return the actual SAME-object receipt. Empty callback is never support.
     */
    class endpoint
    {
    public:
        virtual ~endpoint()=default;
        virtual bool owns_native()const noexcept=0;
        virtual std::uint32_t inspect(wire::binding const&,std::uint64_t,wire::packet&)noexcept=0;
        virtual std::uint32_t submit(wire::binding const&,std::uint64_t,std::uint8_t const*,std::size_t,std::uint64_t&)noexcept=0;
        virtual std::uint32_t respond(wire::binding const&,std::uint64_t,wire::binding const&,ros::capability_spec const&,std::uint64_t,std::uint8_t const*,std::size_t)noexcept=0;
        virtual std::uint32_t abandon(wire::binding const&,std::uint64_t,std::uint64_t)noexcept=0;
        virtual std::uint32_t poll(wire::binding const&,std::uint64_t)noexcept=0;
        virtual std::uint32_t peek(wire::binding const&,std::uint64_t,std::size_t,native::snapshot&,std::size_t)noexcept=0;
        virtual std::uint32_t commit(wire::binding const&,std::uint64_t,std::uint64_t,std::uint64_t)noexcept=0;
        virtual std::uint32_t cancel(wire::binding const&,std::uint64_t,std::uint64_t,std::uint64_t)noexcept=0;
        virtual std::uint32_t available(wire::binding const&,std::uint64_t,wire::binding const&,ros::capability_spec const&,bool&)noexcept=0;
        virtual std::uint32_t available_until(wire::binding const& owner,std::uint64_t incarnation,wire::binding const& query,
            ros::capability_spec const& cap,std::uint64_t wait_ns,ipc::deadline,
            native_rcl_graph_wait::admission const&,bool& output)noexcept
        {return wait_ns?95u:available(owner,incarnation,query,cap,output);}
        virtual std::uint32_t retire(wire::binding const&,std::uint64_t)noexcept=0;
    };
    template<class Service,bool Client>class typed_endpoint final:public endpoint
    {
        using real_type=native::typed_endpoint<Service,Client>;
        std::shared_ptr<real_type> real_;
    public:
        static std::uint32_t create(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,
            std::shared_ptr<uwvm_rmw_cpp::Registry const> const& codecs,ros::capability_spec const& cap,
            wire::binding const& owner,std::uint64_t incarnation,std::size_t history,
            std::shared_ptr<native::report> const& report,std::shared_ptr<endpoint>& output,wire::packet& observed)
        {
            // The type-erased owner itself is allocated/retained BEFORE init.
            auto next=std::make_shared<typed_endpoint>();output=next;
            return real_type::create(node,codecs,cap,owner,incarnation,history,report,next->real_,observed);
        }
        bool owns_native()const noexcept override{return bool(real_);}
        std::uint32_t inspect(wire::binding const& o,std::uint64_t i,wire::packet& p)noexcept override{return real_?real_->inspect(o,i,p):125;}
        std::uint32_t submit(wire::binding const& o,std::uint64_t i,std::uint8_t const* b,std::size_t n,std::uint64_t& t)noexcept override{return real_?real_->submit(o,i,b,n,t):125;}
        std::uint32_t respond(wire::binding const& o,std::uint64_t i,wire::binding const& r,ros::capability_spec const& c,std::uint64_t t,std::uint8_t const* b,std::size_t n)noexcept override{return real_?real_->respond(o,i,r,c,t,b,n):125;}
        std::uint32_t abandon(wire::binding const& o,std::uint64_t i,std::uint64_t t)noexcept override{return real_?real_->abandon(o,i,t):125;}
        std::uint32_t poll(wire::binding const& o,std::uint64_t i)noexcept override{return real_?real_->poll(o,i):125;}
        std::uint32_t peek(wire::binding const& o,std::uint64_t i,std::size_t w,native::snapshot& s,std::size_t c)noexcept override{return real_?real_->peek(o,i,w,s,c):125;}
        std::uint32_t commit(wire::binding const& o,std::uint64_t i,std::uint64_t s,std::uint64_t t)noexcept override{return real_?real_->commit(o,i,s,t):125;}
        std::uint32_t cancel(wire::binding const& o,std::uint64_t i,std::uint64_t s,std::uint64_t t)noexcept override{return real_?real_->cancel(o,i,s,t):125;}
        std::uint32_t available(wire::binding const& o,std::uint64_t i,wire::binding const& a,ros::capability_spec const& c,bool& r)noexcept override{return real_?real_->available(o,i,a,c,r):125;}
        std::uint32_t available_until(wire::binding const& o,std::uint64_t i,wire::binding const& a,ros::capability_spec const& c,
            std::uint64_t wait_ns,ipc::deadline until,native_rcl_graph_wait::admission const& waiting,bool& r)noexcept override
        {return real_?real_->available_until(o,i,a,c,wait_ns,until,waiting,r):125;}
        std::uint32_t retire(wire::binding const& o,std::uint64_t i)noexcept override{return real_?real_->retire(o,i):0;}
    };
    /** Host-selected cold activation factory. A retained immutable Native profile
     * may own the callable's captures. Guest data never supplies code or grants;
     * the selected factory must prove the exact type/identity/QoS and commit its
     * output only after complete native construction. Failure must retire any
     * locally created native object once. Existing fixed CPP10 remains default.
     */
    using factory=std::function<std::uint32_t(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&,
        std::shared_ptr<uwvm_rmw_cpp::Registry const> const&,ros::capability_spec const&,
        wire::binding const&,std::uint64_t,std::size_t,std::shared_ptr<native::report> const&,
        std::shared_ptr<endpoint>&,wire::packet&)>;
    inline std::uint32_t create_fixed(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,
        std::shared_ptr<uwvm_rmw_cpp::Registry const> const& codecs,ros::capability_spec const& cap,
        wire::binding const& owner,std::uint64_t incarnation,std::size_t history,
        std::shared_ptr<native::report> const& report,std::shared_ptr<endpoint>& output,wire::packet& observed)
    {
        // The immutable CPP10 descriptor has no canonical-identity member. Do
        // not ignore an explicit constraint or pretend Native RIHS proves it.
        // A future genuine compiled canonical adapter must provide a separate
        // complete proof/factory; this finite implementation deliberately95s.
        if(cap.limits.find("adapter-codec-identity")!=cap.limits.end())return 95;
        if(!codecs||codecs->error())return 95;
        bool client=cap.operation=="service.call";
        if(!client&&cap.operation!="service.advertise")return 13;
        if(cap.type=="std_srvs/srv/Empty")
            return client?typed_endpoint<std_srvs::srv::Empty,true>::create(node,codecs,cap,owner,incarnation,history,report,output,observed):
                typed_endpoint<std_srvs::srv::Empty,false>::create(node,codecs,cap,owner,incarnation,history,report,output,observed);
        if(cap.type=="example_interfaces/srv/AddTwoInts")
            return client?typed_endpoint<example_interfaces::srv::AddTwoInts,true>::create(node,codecs,cap,owner,incarnation,history,report,output,observed):
                typed_endpoint<example_interfaces::srv::AddTwoInts,false>::create(node,codecs,cap,owner,incarnation,history,report,output,observed);
        return 95;
    }

    using canonical_sizes=owned_service_storage::bodies;
    // Called only during cold admission under the registry lock. A separately
    // selected profile proves its own actual roots; fixed_bounds below still
    // requires the existing complete CPP10 Registry and fails closed without it.
    using bounds_selector=std::function<std::uint32_t(std::shared_ptr<uwvm_rmw_cpp::Registry const> const&,
        ros::capability_spec const&,canonical_sizes&)>;
    /** Genuine CPP10 root bounds only. Presence of a separately constrained
     * canonical identity requires an explicitly selected custom proof instead.
     * The output is committed only after full original root selection. */
    inline std::uint32_t fixed_bounds(std::shared_ptr<uwvm_rmw_cpp::Registry const> const& codecs,
        ros::capability_spec const& cap,canonical_sizes& output)noexcept
    {
        try{
            if(cap.limits.find("adapter-codec-identity")!=cap.limits.end()||!codecs||codecs->error())return 95;
            auto root=cap.type=="std_srvs/srv/Empty"?codecs->codec(uwvm_rmw_cpp::Codec::empty_service):
                cap.type=="example_interfaces/srv/AddTwoInts"?codecs->codec(uwvm_rmw_cpp::Codec::add_service):nullptr;
            if(!root||!root->request||!root->response)return 95;
            // Preserve the original queue-admission short-capacity error before
            // the later full selected-grant predicate. No Native effect occurs.
            if(limit(cap,"max-request-bytes")<root->request->maximum_wire_bytes||
                limit(cap,"max-response-bytes")<root->response->maximum_wire_bytes)return 90;
            if(!native::selected(cap,*root,native::identity_text(root->native_rihs01)))return 13;
            canonical_sizes next{root->request->maximum_wire_bytes,root->response->maximum_wire_bytes};output=next;return 0;
        }catch(std::bad_alloc const&){return 12;}catch(...){return 13;}
    }

    class registry
    {
        struct object
        {
            ros::capability_spec source,paired;
            wire::binding peer_owner{};
            wire::packet observed{};
            std::shared_ptr<native::report> report;
            std::shared_ptr<endpoint> real;
            diagnostics facts;
        };
        ::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node_;
        std::shared_ptr<uwvm_rmw_cpp::Registry const> codecs_;
        limits limits_;
        factory create_;
        bounds_selector bounds_;
        pair_history pairs_;
        std::map<std::uint64_t,object> objects_;
        std::mutex mutex_;
        bool sealed_{};
        inline static nonreusing_id_allocator incarnations_;
        static bool expired(ipc::deadline d)noexcept{return std::chrono::steady_clock::now()>=d;}
        static void bump(std::uint64_t& n)noexcept{if(n!=UINT64_MAX)++n;}
        static void clear_pending(object& o)noexcept{o.facts.pending_sequence=o.facts.pending_reservation=0;}
        /** One actual native fini attempt, with diagnostic storage surviving the
         * final shared object. This never needs quota, allocation or a live RPC.
         */
        std::uint32_t retire_one(std::uint64_t source)noexcept
        {
            auto at=objects_.find(source);if(at==objects_.end())return 0;auto& row=at->second;
            if(row.facts.cleanup_claimed)return 0;row.facts.cleanup_claimed=true;
            std::uint32_t error{};
            if(row.real&&row.real->owns_native()){try{error=row.real->retire(row.observed.owner,row.observed.incarnation);}catch(...){error=5;}record_retire(row.facts,error);}
            row.real.reset();clear_pending(row);return error;
        }
        void tombstone(object& row,std::uint32_t reason)noexcept
        {(void)pairs_.retire(row.observed.owner,row.observed.kind,reason);record_error(row.facts,reason);}
        std::uint32_t fail_delivery(object& row,std::uint32_t primary)noexcept
        {
            record_error(row.facts,primary);if(!row.facts.pending_reservation)return 0;
            std::uint32_t error{};
            try{error=row.real->cancel(row.observed.owner,row.observed.incarnation,row.facts.pending_sequence,row.facts.pending_reservation);}catch(...){error=5;}
            record_cancel(row.facts,error);if(!error){clear_pending(row);return 0;}
            tombstone(row,primary);(void)retire_one(row.observed.owner.source);return error;
        }
        static std::uint32_t validate_admission(admitted const& a,transport::control const& p)noexcept
        {
            if(!a.source||!a.paired||!wire::valid(a.paired_owner)||
                !ros::service_endpoint_domain(p.owner,a.paired_owner)||a.paired_owner.source==p.owner.source)return 13;
            auto error=ros::service_endpoint_pair(*a.source,*a.paired,p.kind);if(error)return error;
            if(p.operation==transport::command::available)
            {
                if(!a.availability||!wire::valid(a.availability_owner)||
                    !ros::service_endpoint_domain(p.owner,a.availability_owner)||a.availability_owner.source==p.owner.source||
                    a.availability_owner.source==a.paired_owner.source||p.peer!=a.availability_owner.source||
                    !ros::service_availability_pair(ros::version::ros2,*a.availability,*a.source))return 13;
            }
            else if(p.peer!=a.paired_owner.source)return 13;
            return 0;
        }
        std::uint32_t find(admitted const& a,transport::control const& p,object*& output)noexcept
        {
            pair_history::entry const* pair{};auto error=pairs_.find_live(p.owner,p.kind,pair);if(error)return error;
            if(pair->peer!=a.paired_owner.source||pair->primary_alias!=a.source->alias||pair->peer_alias!=a.paired->alias)return 13;
            auto at=objects_.find(p.owner.source);if(at==objects_.end())return 125;auto& row=at->second;
            if(!row.real||row.observed.incarnation!=p.incarnation||!wire::same(row.peer_owner,a.paired_owner)||
                !exact_cap(row.source,*a.source)||!exact_cap(row.paired,*a.paired))return 13;
            output=&row;return 0;
        }
        /** Both policy dimensions remain original UWRE envelope/body quotas.
         * Actual native URMI/object/immutable history overhead is separately
         * finite admitted process memory, not a fabricated Guest traffic charge.
         */
        std::uint32_t queue_limits(admitted const& a)const
        {
            if(!bounds_)return 95;
            auto const& source=*a.source;auto const& queue=source.operation=="service.call"?*a.paired:source;
            canonical_sizes actual{};auto error=bounds_(codecs_,source,actual);if(error)return error;
            owned_service_storage::grant policy{limits_.history_per_endpoint,
                limit(queue,"max-event-queue-depth"),limit(queue,"max-event-queue-bytes"),
                limit(source,"max-inflight"),limit(source,"qos.depth"),
                limit(source,"max-request-bytes"),limit(source,"max-response-bytes"),
                source.operation=="service.call"?limit(*a.paired,"max-event-bytes"):0,
                source.operation=="service.call"};
            return owned_service_storage::admit(actual,policy,ipc::event_header_bytes);
        }
        static bool same_receipt(wire::packet const& a,wire::packet const& b)noexcept
        {
            if(!wire::same(a.owner,b.owner)||a.kind!=b.kind||a.incarnation!=b.incarnation||a.profiles!=b.profiles||
                a.gid_size!=b.gid_size||a.gid!=b.gid)return false;
            for(unsigned n=0;n<a.profiles;++n)if(!wire::same(a.actual[n],b.actual[n]))return false;
            return true;
        }
        static bool write_header(transport::control const& p,std::vector<std::uint8_t>& bytes)noexcept
        {
            std::array<std::uint8_t,transport::header_bytes> h{};if(!transport::encode(p,true,h)||bytes.size()<h.size())return false;
            std::copy(h.begin(),h.end(),bytes.begin());return true;
        }
        static bool write_endpoint(transport::control const& p,wire::packet const& actual,std::vector<std::uint8_t>& bytes)noexcept
        {
            std::array<std::uint8_t,wire::packet_bytes> packet{};
            if(bytes.size()!=transport::endpoint_response_bytes||!wire::encode(actual,true,packet)||!write_header(p,bytes))return false;
            std::copy(packet.begin(),packet.end(),bytes.begin()+transport::header_bytes);return true;
        }
    public:
        registry(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node,std::shared_ptr<uwvm_rmw_cpp::Registry const> codecs,limits storage,
            factory selected=&create_fixed,bounds_selector selected_bounds=&fixed_bounds):
            node_{std::move(node)},codecs_{std::move(codecs)},limits_{storage},create_{std::move(selected)},bounds_{std::move(selected_bounds)},pairs_{storage.maximum_entries}{}
        registry(registry const&)=delete;registry& operator=(registry const&)=delete;
        ~registry(){clear();}
        ipc::call_result execute(admitted const& a,transport::control const& request,
            std::uint8_t const* body,std::size_t length,ipc::deadline until)
        {
            std::lock_guard<std::mutex> g{mutex_};if(sealed_)return {125,{}};
            if(!limits_.valid()||!transport::valid(request,false)||request.body_bytes!=length||(!body&&length))return {22,{}};
            auto error=validate_admission(a,request);if(error)return {error,{}};
            if(expired(until))return {110,{}};
            // Every fixed successful reply is reserved before its native effect.
            // Peek reserves the WHOLE admitted maximum packet before even poll.
            ipc::call_result result;
            auto extent=transport::endpoint_command(request.operation)?transport::endpoint_response_bytes:
                request.operation==transport::command::peek?static_cast<std::size_t>(request.capacity):transport::header_bytes;
            try{result.payload.resize(extent);}catch(std::bad_alloc const&){return {12,{}};}catch(...){return {5,{}};}
            auto reply=request;
            if(request.operation==transport::command::activate)
            {
                // Each selected factory/bounds pair admits its own actual native graph.
                // Requiring unrelated CPP10 roots here would defeat a separately
                // compiled complete control profile. Default fixed selectors
                // retain their original nonnull/error checks before effects.
                if(!node_||!create_)return {95,{}};
                try{error=queue_limits(a);}catch(...){return {22,{}};}if(error)return {error,{}};
                pair_history::entry const* pair{};
                error=pairs_.reserve(request.owner,request.kind,a.paired_owner.source,*a.source,*a.paired,pair);if(error)return {error,{}};
                // Preserve both original grants and all result/report storage
                // before native creation. Allocation failure leaves a tombstone.
                decltype(objects_)::iterator at;
                try{
                    object next;next.source=*a.source;next.paired=*a.paired;next.peer_owner=a.paired_owner;
                    next.report=std::make_shared<native::report>();next.observed.owner=request.owner;next.observed.kind=request.kind;
                    auto inserted=objects_.emplace(request.owner.source,std::move(next));if(!inserted.second){(void)pairs_.retire(request.owner,request.kind,17);return {17,{}};}at=inserted.first;
                }catch(...){(void)pairs_.retire(request.owner,request.kind,12);return {12,{}};}
                auto& row=at->second;std::uint64_t incarnation{};
                try{incarnation=incarnations_.allocate(ipc::random_session);}catch(...){error=5;}
                if(!incarnation){if(!error)error=90;tombstone(row,error);return {error,{}};}
                row.observed.incarnation=incarnation;wire::packet actual{};
                if(expired(until)){tombstone(row,110);return {110,{}};}
                try{error=create_(node_,codecs_,row.source,request.owner,incarnation,limits_.history_per_endpoint,row.report,row.real,actual);}
                catch(std::bad_alloc const&){error=12;}catch(...){error=5;}
                if(!error&&(!row.real||!wire::valid_response(actual)||actual.operation!=wire::command::activate||
                    !wire::same(actual.owner,request.owner)||actual.kind!=request.kind||actual.incarnation!=incarnation))error=13;
                if(!error&&expired(until))error=110;
                if(error){tombstone(row,error);(void)retire_one(request.owner.source);return {error,{}};}
                row.observed=actual;reply.incarnation=incarnation;
                if(!write_endpoint(reply,actual,result.payload)){tombstone(row,22);(void)retire_one(request.owner.source);return {22,{}};}
                return result;
            }
            object* selected{};error=find(a,request,selected);if(error)return {error,{}};auto& row=*selected;
            if(expired(until)){record_error(row.facts,110);return {110,{}};}
            if(request.operation==transport::command::inspect)
            {
                wire::packet actual{};error=row.real->inspect(request.owner,request.incarnation,actual);
                if(!error&&(!same_receipt(row.observed,actual)||actual.operation!=wire::command::inspect))error=13;
                if(!error&&expired(until))error=110;if(error){record_error(row.facts,error);return {error,{}};}
                if(!write_endpoint(reply,actual,result.payload))return {22,{}};return result;
            }
            if(request.operation==transport::command::retire)
            {
                (void)pairs_.retire(request.owner,request.kind,125);error=retire_one(request.owner.source);
                if(error)return {error,{}};wire::packet actual{};actual.operation=wire::command::retire;
                actual.owner=request.owner;actual.kind=request.kind;actual.incarnation=request.incarnation;
                if(!write_endpoint(reply,actual,result.payload))return {22,{}};
                if(expired(until)){bump(row.facts.late_effects);record_error(row.facts,110);return {110,{}};}return result;
            }
            if(request.operation==transport::command::peek)
            {
                if(row.facts.pending_reservation)return {17,{}};native::snapshot snapshot;
                error=row.real->peek(request.owner,request.incarnation,request.writer_bytes,snapshot,
                    static_cast<std::size_t>(request.capacity-transport::header_bytes));
                if(error==11&&expired(until)){record_error(row.facts,110);return {110,{}};}
                if(error==11){error=row.real->poll(request.owner,request.incarnation);if(!error||error==11)
                    error=row.real->peek(request.owner,request.incarnation,request.writer_bytes,snapshot,static_cast<std::size_t>(request.capacity-transport::header_bytes));}
                if(error){record_error(row.facts,error);return {error,{}};}
                if(!snapshot.sequence||!snapshot.reservation)
                {tombstone(row,13);(void)retire_one(request.owner.source);return {13,{}};}
                // Establish this borrowing obligation before any validation,
                // quota/deadline check or output formation can fail.
                row.facts.pending_sequence=snapshot.sequence;row.facts.pending_reservation=snapshot.reservation;
                reply.sequence=snapshot.sequence;reply.reservation=snapshot.reservation;reply.record_bytes=snapshot.size;
                metadata::header info{};
                if(!snapshot.bytes||snapshot.size<metadata::header_bytes||
                    !metadata::decode(snapshot.bytes.get(),metadata::header_bytes,snapshot.size-metadata::header_bytes,info))error=22;
                if(!error){reply.ticket=info.correlation;if(!transport::associated_record(reply,snapshot.bytes.get(),snapshot.size,info))error=22;}
                if(!error&&request.kind==wire::role::client)
                {
                    std::uint32_t mask{};error=ros::service_observer_kinds(row.paired,mask);
                    if(!error&&!(mask&ipc::event_bit(info.kind)))error=13;
                }
                if(!error&&expired(until))error=110;
                if(!error&&(snapshot.size>result.payload.size()-transport::header_bytes||!write_header(reply,result.payload)))error=90;
                if(error){fail_delivery(row,error);return {error,{}};}
                result.payload.resize(transport::header_bytes+snapshot.size);
                std::copy_n(snapshot.bytes.get(),snapshot.size,result.payload.begin()+transport::header_bytes);
                if(expired(until)){fail_delivery(row,110);return {110,{}};}
                bump(row.facts.successful_peeks);return result;
            }
            if(request.operation==transport::command::commit||request.operation==transport::command::cancel)
            {
                if(request.sequence!=row.facts.pending_sequence||request.reservation!=row.facts.pending_reservation)return {13,{}};
                if(!write_header(reply,result.payload))return {22,{}};
                if(request.operation==transport::command::commit){error=row.real->commit(request.owner,request.incarnation,request.sequence,request.reservation);record_commit(row.facts,error);}
                else{error=row.real->cancel(request.owner,request.incarnation,request.sequence,request.reservation);record_cancel(row.facts,error);}
                if(error){tombstone(row,error);(void)retire_one(request.owner.source);return {error,{}};}clear_pending(row);
            }
            else if(request.operation==transport::command::submit)
            {
                std::uint64_t ticket{};row.facts.submit_attempted=true;row.facts.last_submit_error=115;
                auto calls=row.report->observe().native[static_cast<unsigned>(native::operation::send)].calls;
                error=row.real->submit(request.owner,request.incarnation,body,length,ticket);row.facts.last_submit_error=error;
                if(error)
                {
                    record_error(row.facts,error);
                    if(row.report->observe().native[static_cast<unsigned>(native::operation::send)].calls!=calls)
                    {tombstone(row,error);(void)retire_one(request.owner.source);}
                    return {error,{}};
                }
                reply.ticket=ticket;reply.body_bytes=0;row.facts.last_ticket=ticket;
                if(!write_header(reply,result.payload)){tombstone(row,22);(void)retire_one(request.owner.source);return {22,{}};}
            }
            else if(request.operation==transport::command::respond)
            {
                row.facts.respond_attempted=true;row.facts.last_respond_error=115;
                auto calls=row.report->observe().native[static_cast<unsigned>(native::operation::respond)].calls;
                error=row.real->respond(request.owner,request.incarnation,a.paired_owner,*a.paired,request.ticket,body,length);
                row.facts.last_respond_error=error;
                if(error)
                {
                    record_error(row.facts,error);
                    if(row.report->observe().native[static_cast<unsigned>(native::operation::respond)].calls!=calls)
                    {tombstone(row,error);(void)retire_one(request.owner.source);}
                    return {error,{}};
                }
                reply.body_bytes=0;if(!write_header(reply,result.payload))return {22,{}};
            }
            else if(request.operation==transport::command::abandon)
            {
                row.facts.abandon_attempted=true;row.facts.last_abandon_error=115;
                error=row.real->abandon(request.owner,request.incarnation,request.ticket);row.facts.last_abandon_error=error;
                if(error){record_error(row.facts,error);return {error,{}};}if(!write_header(reply,result.payload))return {22,{}};
            }
            else if(request.operation==transport::command::available)
            {
                // Both paths query THIS retained client. Positive wait uses
                // the actual registered graph event and borrowed native lifetime;
                // absent actual compiled support/admission remains honest95.
                bool value{};
                error=row.real->available_until(request.owner,request.incarnation,a.availability_owner,*a.availability,
                    request.wait_ns,until,a.waiting,value);
                if(error){record_error(row.facts,error);return {error,{}};}reply.value=value?1:0;
                if(!write_header(reply,result.payload))return {22,{}};
            }
            else return {22,{}};
            if(expired(until))
            {
                bump(row.facts.late_effects);record_error(row.facts,110);
                if(request.operation!=transport::command::available){tombstone(row,110);(void)retire_one(request.owner.source);}
                return {110,{}};
            }
            return result;
        }
        std::uint32_t delivery_failed(wire::binding const& owner,std::uint64_t incarnation,std::uint32_t reason=107)noexcept
        {
            std::lock_guard<std::mutex> g{mutex_};auto at=objects_.find(owner.source);if(at==objects_.end())return 9;
            auto& row=at->second;if(!wire::same(row.observed.owner,owner)||row.observed.incarnation!=incarnation)return 13;
            if(!row.real)return 125;return fail_delivery(row,reason?reason:107);
        }
        /** Trusted close/revoke, not a Guest endpoint operation. Use actual old
         * saved session after disconnect. Tombstone every match BEFORE cleanup.
         */
        void close_owner(wire::binding const& domain,std::uint64_t handle,std::string_view alias,bool revoke=false,std::uint32_t reason=125)noexcept
        {
            std::lock_guard<std::mutex> g{mutex_};(void)pairs_.retire_matching(domain,handle,alias,revoke,reason);
            for(auto const& p:pairs_.rows())if(pair_history::matches_peer(p.second,domain,handle,alias,revoke))(void)retire_one(p.first);
        }
        void retire_session(std::uint64_t session,std::uint32_t reason=107)noexcept
        {
            std::lock_guard<std::mutex> g{mutex_};
            for(auto const& p:pairs_.rows())if(p.second.owner.session==session)(void)pairs_.retire(p.second.owner,p.second.kind,reason);
            for(auto const& p:pairs_.rows())if(p.second.owner.session==session)(void)retire_one(p.first);
        }
        void clear()noexcept
        {std::lock_guard<std::mutex> g{mutex_};if(sealed_)return;sealed_=true;pairs_.seal();for(auto const& p:pairs_.rows())(void)retire_one(p.first);}
        /** Owning thread only, after quiescence. Native report and finite
         * summary remain alive even after all actual endpoints have retired.
         */
        auto const& bindings_after_quiescence()const noexcept{return pairs_.rows();}
        std::uint32_t observe_after_quiescence(std::uint64_t source,diagnostics& output,native::diagnostic& native_output)const noexcept
        {auto at=objects_.find(source);if(at==objects_.end())return 9;output=at->second.facts;native_output=at->second.report->observe();return 0;}
    };
}
