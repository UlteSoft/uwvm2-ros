/** @file
 * Session-owned real Subscription registry and explicit record transactions.
 * This source does not wire a Host import, server action or Runtime classifier.
 * The authenticated caller MUST validate policy and hold the full source lease
 * for every entry, including commit/cancel. Binding values are not authority.
 */
#pragma once
#include "compat/node.h"
#include "native_rmw_subscription.h"
#include <uwvm2/imported/ros/common/rmw_endpoint_history.h>
#include <uwvm2/imported/ros/ipc/rmw_subscription.h>
#include <chrono>
#include <map>
#include <mutex>
#include <utility>
#include <functional>

namespace uwvm2::ros_gateway::native_rmw_subscription
{
    namespace stream=::uwvm2::imported::ros::ipc::rmw_subscription;
    /** Host-only cold creation callable. Captures retain an immutable, complete
     * actual native profile. It must validate the explicit selected grant and
     * commit output only after construction/QoS/GID/receipt all succeed; on an
     * error or exception, local RAII retires partial native state once. The
     * callable never enters take/peek/commit/cancel or the Guest ABI.
     */
    using factory=std::function<std::uint32_t(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&,ros::capability_spec const&,
        wire::binding const&,std::uint64_t,std::shared_ptr<endpoint>&,wire::packet&)>;
    /** This is a real compile-selected native factory seam, not a Guest-provided
     * code address or a legacy listen fallback. Four builtins work through their
     * actual typed endpoint. A custom row needs its genuine compiled shard
     * factory; absent support returns95, never success with an empty endpoint.
     */
    inline std::uint32_t create_builtin(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,ros::capability_spec const& cap,
        wire::binding const& owner,std::uint64_t incarnation,std::shared_ptr<endpoint>& output,wire::packet& actual)
    {
        if(cap.type==native_rmw_builtin::string_codec::type){return builtin<native_rmw_builtin::string_codec>::create(node,cap,owner,incarnation,output,actual);}
        if(cap.type==native_rmw_builtin::bool_codec::type){return builtin<native_rmw_builtin::bool_codec>::create(node,cap,owner,incarnation,output,actual);}
        if(cap.type==native_rmw_builtin::twist_codec::type){return builtin<native_rmw_builtin::twist_codec>::create(node,cap,owner,incarnation,output,actual);}
        if(cap.type==native_rmw_builtin::image_codec::type){return builtin<native_rmw_builtin::image_codec>::create(node,cap,owner,incarnation,output,actual);}
        return 95;
    }
    struct delivery_diagnostics
    {
        std::uint64_t successful_peeks{},committed{},cancelled{},transport_failures{},late_effects{};
        std::uint64_t pending_sequence{},pending_reservation{},last_sequence{},last_reservation{};
        std::uint32_t first_error{},last_error{},last_cleanup_error{},activation_cleanup_error{},
            last_cancel_error{},last_retire_error{},last_commit_error{};
        bool cleanup_attempted{},last_commit_completed{},cancel_attempted{},retire_attempted{},commit_attempted{};
    };
    /** Pure diagnostic recording shared by the actual cleanup call sites and
     * the value-only fault-result contract. Each native operation has its own
     * result slot: a later retirement must never erase a failed cancellation.
     * Summary first/last fields are conveniences, not the complete evidence.
     */
    inline void record_primary_error(delivery_diagnostics& facts,std::uint32_t code) noexcept
    {if(code){if(!facts.first_error){facts.first_error=code;}facts.last_error=code;}}
    inline void record_cancel_result(delivery_diagnostics& facts,std::uint32_t code) noexcept
    {facts.cancel_attempted=true;facts.last_cancel_error=code;facts.last_cleanup_error=code;record_primary_error(facts,code);}
    inline void record_retire_result(delivery_diagnostics& facts,std::uint32_t code) noexcept
    {facts.retire_attempted=true;facts.last_retire_error=code;facts.last_cleanup_error=code;record_primary_error(facts,code);}
    inline void record_commit_result(delivery_diagnostics& facts,std::uint32_t code) noexcept
    {facts.commit_attempted=true;facts.last_commit_error=code;facts.last_commit_completed=!code;record_primary_error(facts,code);}
    class registry
    {
        using history=::uwvm2::imported::ros::rmw_endpoint_history;
        struct object
        {
            std::shared_ptr<endpoint> native;
            delivery_diagnostics audit;
        };
        ::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node_;
        factory create_;
        history history_;
        std::map<std::uint64_t,object> objects_;
        std::mutex mutex_;
        bool sealed_{};
        static void bump(std::uint64_t& n) noexcept {if(n!=UINT64_MAX){++n;}}
        static void note(object& row,std::uint32_t code) noexcept
        {record_primary_error(row.audit,code);}
        static bool expired(ipc::deadline until) noexcept{return std::chrono::steady_clock::now()>=until;}
        static ipc::call_result encode_endpoint(wire::packet const& packet)
        {
            std::array<std::uint8_t,wire::packet_bytes> raw{};
            if(!wire::encode(packet,true,raw)){return {22,{}};}return {0,{raw.begin(),raw.end()}};
        }
        static void clear_pending(object& row) noexcept
        {row.audit.pending_sequence=row.audit.pending_reservation=0;}
        /** Every locally retained endpoint is closed once after the local
         * tombstone, even if replying, native retirement, or cancellation fails.
         * No quota or fresh allocation is required to retire known ownership.
         */
        std::uint32_t retire_one(std::uint64_t source) noexcept
        {
            auto at=objects_.find(source);auto fact=history_.rows().find(source);
            if(at==objects_.end()){return 0;}auto& row=at->second;
            if(row.audit.cleanup_attempted){return 0;}row.audit.cleanup_attempted=true;
            std::uint32_t code{};
            if(row.native&&fact!=history_.rows().end()&&fact->second.observed.incarnation)
            {
                try{code=row.native->retire(fact->second.observed.owner,fact->second.observed.incarnation);}
                catch(...){code=5;}
                record_retire_result(row.audit,code);
            }
            else if(row.native){code=107;}
            row.native.reset();clear_pending(row);row.audit.last_cleanup_error=code;note(row,code);
            if(history_.claim_cleanup(source)){history_.cleanup_result(source,code);}
            return code;
        }
        /** Failed activation may not yet have published its native incarnation
         * into history. Close its actual returned object using the minted local
         * association; never invent a successful receipt for failed creation.
         */
        static void rollback_activation(object& row,wire::binding const& owner,std::uint64_t incarnation) noexcept
        {
            if(!row.native){return;}std::uint32_t code{};
            try{code=row.native->retire(owner,incarnation);}catch(...){code=5;}
            row.native.reset();row.audit.activation_cleanup_error=code;record_retire_result(row.audit,code);
        }
        std::uint32_t find(ros::capability_spec const& cap,wire::binding const& owner,std::uint64_t incarnation,object*& result)
        {
            if(cap.operation!="topic.subscribe"){return 13;}
            history::entry const* fact{};auto error=history_.find(owner,incarnation,wire::role::subscription,fact);
            if(error){return error;}if(fact->alias!=cap.alias){return 13;}
            auto at=objects_.find(owner.source);if(at==objects_.end()||!at->second.native){return 125;}
            result=&at->second;return 0;
        }
        /** A failed output transaction never pops. Retain both the primary
         * serialization/deadline/transport error and the actual cancel result.
         * If cancel cannot release the known token, retire the entire endpoint
         * rather than allowing a later attempt to consume ambiguous state.
         */
        void fail_delivery(object& row,wire::binding const& owner,std::uint64_t incarnation,std::uint32_t primary) noexcept
        {
            note(row,primary);bump(row.audit.transport_failures);
            if(!row.audit.pending_reservation){return;}
            std::uint32_t cancel_error{};
            try{cancel_error=row.native->cancel(owner,incarnation,row.audit.pending_sequence,row.audit.pending_reservation);}
            catch(...){cancel_error=5;}
            record_cancel_result(row.audit,cancel_error);
            if(!cancel_error){clear_pending(row);bump(row.audit.cancelled);return;}
            note(row,cancel_error);history_.abandon(owner.source,primary);(void)retire_one(owner.source);
        }
    public:
        explicit registry(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node,factory selected=&create_builtin):node_{std::move(node)},create_{std::move(selected)}{}
        registry(registry const&)=delete;registry& operator=(registry const&)=delete;
        ~registry(){clear();}
        /** UMRE-v1 is unchanged: same actual subscription object and concrete
         * actual native QoS are independently inspected/retired. Resource/type/
         * codec/QoS policy checks are retained inside the actual typed factory.
         */
        ipc::call_result control(ros::capability_spec const& cap,wire::packet const& request,ipc::deadline until)
        {
            std::lock_guard<std::mutex> lock{mutex_};if(sealed_){return {125,{}};}
            if(!wire::valid_request(request)||request.kind!=wire::role::subscription||cap.operation!="topic.subscribe"){return {22,{}};}
            if(expired(until)){return {110,{}};}
            if(request.operation==wire::command::activate)
            {
                if(!node_||!create_){return {95,{}};}
                auto maximum=limit(cap,"max-subscribers");
                if(!maximum||maximum>history::maximum_entries||limit(cap,"max-message-bytes")>stream::maximum_body_bytes){return {90,{}};}
                // Reserve reply storage before any native side effect. A later
                // response failure still tombstones and retires the object.
                ipc::call_result reply;
                try{reply.payload.resize(wire::packet_bytes);}catch(...){return {12,{}};}
                history::entry const* fact{};auto error=history_.reserve(request,cap.alias,static_cast<std::size_t>(maximum),fact);
                if(error){return {error,{}};}
                decltype(objects_)::iterator position;
                try{position=objects_.emplace(request.owner.source,object{}).first;}
                catch(...){history_.abandon(request.owner.source,12);return {12,{}};}
                auto& row=position->second;auto incarnation=history_.reserve_native_incarnation();
                if(!incarnation){history_.abandon(request.owner.source,90);note(row,90);return {90,{}};}
                wire::packet actual{};
                try{error=create_(node_,cap,request.owner,incarnation,row.native,actual);}catch(std::bad_alloc const&){error=12;}catch(...){error=5;}
                if(!error&&(!row.native||!wire::valid_response(actual)||actual.operation!=wire::command::activate||
                    actual.kind!=wire::role::subscription||!wire::same(actual.owner,request.owner)||actual.incarnation!=incarnation)){error=13;}
                if(error)
                {note(row,error);history_.abandon(request.owner.source,error);rollback_activation(row,request.owner,incarnation);return {error,{}};}
                error=history_.activate(actual);
                if(error)
                {note(row,error);history_.abandon(request.owner.source,error);rollback_activation(row,request.owner,incarnation);return {error,{}};}
                std::array<std::uint8_t,wire::packet_bytes> encoded{};
                if(!wire::encode(actual,true,encoded)||expired(until))
                {
                    error=expired(until)?110u:22u;note(row,error);history_.abandon(request.owner.source,error);
                    (void)retire_one(request.owner.source);return {error,{}};
                }
                std::copy(encoded.begin(),encoded.end(),reply.payload.begin());return reply;
            }
            object* row{};auto error=find(cap,request.owner,request.incarnation,row);if(error){return {error,{}};}
            if(request.operation==wire::command::inspect)
            {
                wire::packet actual{};
                try{error=row->native->inspect(request.owner,request.incarnation,actual);}catch(...){error=5;}
                if(!error){error=history_.inspect(actual);}if(!error&&expired(until)){error=110;}
                if(error){note(*row,error);return {error,{}};}
                try{return encode_endpoint(actual);}catch(...){note(*row,12);return {12,{}};}
            }
            error=history_.retire(request.owner,request.incarnation,request.kind);if(error){return {error,{}};}
            error=retire_one(request.owner.source);if(error){return {error,{}};}
            try{return encode_endpoint(request);}catch(...){note(*row,12);return {12,{}};}
        }
        ipc::call_result transfer(ros::capability_spec const& cap,stream::control const& request,ipc::deadline until)
        {
            std::lock_guard<std::mutex> lock{mutex_};if(sealed_){return {125,{}};}
            if(cap.operation!="topic.subscribe"||!stream::valid(request,false)){return {22,{}};}
            object* row{};auto error=find(cap,request.owner,request.incarnation,row);if(error){return {error,{}};}
            if(expired(until)){note(*row,110);return {110,{}};}
            if(request.operation==stream::command::peek)
            {
                if(row->audit.pending_reservation){return {17,{}};}queue::snapshot snapshot;
                try{error=row->native->peek(request.owner,request.incarnation,request.gid_bytes,snapshot,
                    static_cast<std::size_t>(request.capacity-stream::header_bytes));}catch(...){error=5;}
                if(error){note(*row,error);return {error,{}};}
                // Establish ownership of the returned attempt immediately,
                // before validation, allocations, deadline checks or replies.
                row->audit.pending_sequence=snapshot.sequence;row->audit.pending_reservation=snapshot.reservation;
                row->audit.last_sequence=snapshot.sequence;row->audit.last_reservation=snapshot.reservation;
                auto reply=request;reply.sequence=snapshot.sequence;reply.reservation=snapshot.reservation;reply.record_bytes=snapshot.size;
                std::vector<std::uint8_t> bytes;
                error=stream::pack_delivery(reply,snapshot.bytes.get(),snapshot.size,bytes);
                if(!error&&expired(until)){error=110;}
                if(error){fail_delivery(*row,request.owner,request.incarnation,error);return {error,{}};}
                bump(row->audit.successful_peeks);return {0,std::move(bytes)};
            }
            // Matching full source/incarnation alone is not enough: both the
            // real record and the independent borrowing attempt must match.
            if(row->audit.pending_sequence!=request.sequence||row->audit.pending_reservation!=request.reservation){return {13,{}};}
            std::array<std::uint8_t,stream::header_bytes> encoded{};
            if(!stream::encode(request,true,encoded)){return {22,{}};}
            ipc::call_result reply;
            try{reply.payload.assign(encoded.begin(),encoded.end());}catch(...){note(*row,12);return {12,{}};}
            try
            {
                error=request.operation==stream::command::commit?
                    row->native->commit(request.owner,request.incarnation,request.sequence,request.reservation):
                    row->native->cancel(request.owner,request.incarnation,request.sequence,request.reservation);
            }
            catch(...){error=5;}
            if(request.operation==stream::command::commit){record_commit_result(row->audit,error);}
            else{record_cancel_result(row->audit,error);}
            if(error){note(*row,error);return {error,{}};}
            clear_pending(*row);
            if(request.operation==stream::command::commit){bump(row->audit.committed);}
            else{bump(row->audit.cancelled);}
            if(expired(until))
            {
                // The effect really happened; never replay it. A lost/late ACK
                // is an ambiguous transport outcome, not a rolled-back commit.
                bump(row->audit.late_effects);note(*row,110);return {110,{}};
            }
            return reply;
        }
        /** Trusted transport cleanup, not a Guest operation. Exact owner and
         * current alias still apply. A failed send releases only its pending
         * reservation; a lost ACK requires caller session retirement instead.
         */
        std::uint32_t delivery_failed(ros::capability_spec const& cap,wire::binding const& owner,
            std::uint64_t incarnation,std::uint32_t reason=107) noexcept
        {
            std::lock_guard<std::mutex> lock{mutex_};object* row{};auto error=find(cap,owner,incarnation,row);
            if(error){return error;}fail_delivery(*row,owner,incarnation,reason?reason:107);return row->audit.last_cleanup_error;
        }
        std::uint32_t observe(ros::capability_spec const& cap,wire::binding const& owner,
            std::uint64_t incarnation,delivery_diagnostics& output,queue::statistics& records)
        {
            std::lock_guard<std::mutex> lock{mutex_};object* row{};auto error=find(cap,owner,incarnation,row);if(error){return error;}
            queue::statistics q{};try{error=row->native->observe(owner,incarnation,q);}catch(...){error=5;}
            if(error){return error;}output=row->audit;records=q;return 0;
        }
        /** Retired/failed rows remain finite immutable-key tombstones with
         * diagnostics. This view is for the owning thread after quiescence;
         * it is not a concurrently borrowed public mutable state interface.
         */
        auto const& endpoint_history_after_quiescence() const noexcept{return history_.rows();}
        std::vector<std::pair<std::uint64_t,delivery_diagnostics>> delivery_history_after_quiescence() const
        {
            std::vector<std::pair<std::uint64_t,delivery_diagnostics>> values;values.reserve(objects_.size());
            for(auto const& row:objects_){values.emplace_back(row.first,row.second.audit);}return values;
        }
        void close_alias(std::string_view alias,std::uint32_t reason=125) noexcept
        {
            std::lock_guard<std::mutex> lock{mutex_};
            for(auto const& row:history_.rows()){if(row.second.alias==alias){history_.abandon(row.first,reason);}}
            for(auto const& row:history_.rows()){if(row.second.alias==alias){(void)retire_one(row.first);}}
        }
        void retire_session(std::uint64_t session,std::uint32_t reason=107) noexcept
        {
            std::lock_guard<std::mutex> lock{mutex_};history_.retire_session(session,reason);
            for(auto const& row:history_.rows()){if(row.second.observed.owner.session==session){(void)retire_one(row.first);}}
        }
        void clear() noexcept
        {
            std::lock_guard<std::mutex> lock{mutex_};if(sealed_){return;}sealed_=true;history_.seal();
            for(auto const& row:history_.rows()){(void)retire_one(row.first);}
            // Keep finite diagnostics until actual registry destruction. Native
            // pointers and pending tokens were retired; no successful re-open.
        }
    };
}
