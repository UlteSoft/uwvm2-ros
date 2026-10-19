/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief Bounded Service-only same-take records and nonreusing delivery tokens.
 *
 * This is not the topic queue: Service Request/Response and genuine control
 * records have their own explicit contract. The endpoint serializes this object.
 * All slots and payload storage are allocated BEFORE any native endpoint/effect.
 * Native request identity, signed sequence, Gateway ticket, URMI stream sequence
 * and peek reservation are five separate concepts. None is bearer authority.
 */
#pragma once
#include <uwvm2/imported/ros/ipc/rmw_metadata.h>
#include <rmw/types.h>
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>
#include <limits>
#include <chrono>

namespace uwvm2::ros_gateway::native_rcl_service
{
    namespace wire=::uwvm2::imported::ros::ipc::rmw_endpoint;
    namespace info=::uwvm2::imported::ros::ipc::rmw_metadata;
    namespace events=::uwvm2::imported::ros::ipc;
    struct snapshot
    {
        std::uint64_t sequence{},reservation{};std::size_t size{};
        std::unique_ptr<std::uint8_t[]> bytes;
    };
    enum class state {free,reserved,pending,queued,delivered,completed,abandoned};
    struct record
    {
        state phase{state::free};std::uint64_t ticket{};bool sequence_known{},identity_known{},native_attempted{},response_seen{},response_attempted{},discard{};
        std::chrono::steady_clock::time_point deadline{};
        rmw_request_id_t identity{},response_identity{};rmw_service_info_t native_info{};
        std::array<std::uint8_t,info::header_bytes> header{};
        std::vector<std::uint8_t> body;
        std::size_t length{};std::uint64_t sequence{};
    };
    /** A fixed lifetime history is deliberate: terminal IDs are not evicted and
     * native signed sequences are not assumed monotonic or globally unique.
     * Exhaustion returns90 BEFORE send/take; a new native incarnation is needed.
     * This first provider supports fixed Empty/AddTwoInts bodies (0/16/8 bytes).
     * A future dynamic family must preserve preallocation and complete decoding.
     */
    class records
    {
        std::vector<record> slots_;
        wire::binding owner_{};std::uint64_t incarnation_{},stream_{},nonce_{},reserved_{},reservation_{};
        std::size_t used_{},maximum_{};bool retired_{};
        bool control_ready_{},terminal_error_issued_{};std::uint64_t control_sequence_{};
        std::array<std::uint8_t,info::header_bytes> control_{};
    public:
        static constexpr std::size_t no_slot=SIZE_MAX;
        records(std::size_t count,std::size_t maximum,wire::binding owner,std::uint64_t id):
            slots_(count),owner_{owner},incarnation_{id},maximum_{maximum}
        {for(auto& r:slots_)r.body.resize(maximum);}
        static bool valid_options(std::size_t n,std::size_t maximum) noexcept
        {return n&&n<=256&&maximum<=16;}
        bool owns(wire::binding const& owner,std::uint64_t id)const noexcept
        {return wire::same(owner,owner_)&&id==incarnation_;}
        record& at(std::size_t n)noexcept{return slots_[n];}
        record const& at(std::size_t n)const noexcept{return slots_[n];}
        std::size_t used()const noexcept{return used_;}
        /** No native attempt may begin without both a history slot and sequence
         * headroom. Two sequence positions remain reserved for one endpoint failure and its
         * later actual retirement, even after the failure record was consumed.
         */
        bool can_reserve()const noexcept{return !retired_&&used_<slots_.size()&&stream_<UINT64_MAX-2;}
        bool can_take()const noexcept{return !retired_&&stream_<UINT64_MAX-2;}
        std::size_t reserve(std::uint64_t ticket) noexcept
        {if(!ticket||!can_reserve())return no_slot;auto i=used_++;auto& r=slots_[i];r.phase=state::reserved;r.ticket=ticket;return i;}
        void release_unattempted(std::size_t i)noexcept
        {if(i+1==used_&&slots_[i].phase==state::reserved&&!slots_[i].native_attempted){slots_[i].phase=state::free;slots_[i].ticket=0;--used_;}}
        static bool same(rmw_request_id_t const& a,rmw_request_id_t const& b)noexcept
        {return a.sequence_number==b.sequence_number&&!std::memcmp(a.writer_guid,b.writer_guid,sizeof(a.writer_guid));}
        std::size_t find(rmw_request_id_t const& id,std::size_t except=no_slot)const noexcept
        {for(std::size_t i{};i<used_;++i)if(i!=except&&slots_[i].identity_known&&same(slots_[i].identity,id))return i;return no_slot;}
        std::size_t find_sequence(std::int64_t sequence,std::size_t except=no_slot)const noexcept
        {for(std::size_t i{};i<used_;++i)if(i!=except&&slots_[i].sequence_known&&slots_[i].identity.sequence_number==sequence)return i;return no_slot;}
        std::size_t ticket(std::uint64_t id)const noexcept
        {for(std::size_t i{};i<used_;++i)if(slots_[i].ticket==id)return i;return no_slot;}
        /** The already encoded body and actual native_info belong to this one
         * native take. No allocation occurs here and no second metadata cache is
         * consulted. Caller runs complete codec validation before this commit.
         */
        std::uint32_t enqueue(std::size_t i,info::header h,std::size_t n)noexcept
        {
            if(i>=used_||retired_||!can_take()||n>maximum_)return 90;
            auto& r=slots_[i];h.owner=owner_;h.incarnation=incarnation_;h.sequence=stream_+1;h.correlation=r.ticket;h.payload_bytes=n;
            if((h.kind!=events::event_kind::service_request&&h.kind!=events::event_kind::service_response)||!info::complete(h))return 95;
            // Recheck the actual stored same-take identity/times, not just the
            // validity/presence of an independently well-formed URMI header.
            if(!r.identity_known||!same(r.identity,r.native_info.request_id)||h.writer_size!=sizeof(r.identity.writer_guid)||
                h.request_sequence!=r.native_info.request_id.sequence_number||h.source_timestamp!=r.native_info.source_timestamp||
                h.received_timestamp!=r.native_info.received_timestamp||std::memcmp(h.writer.data(),r.native_info.request_id.writer_guid,h.writer_size))return 22;
            std::array<std::uint8_t,info::header_bytes> encoded{};if(!info::encode(h,encoded))return 22;
            r.header=encoded;r.length=n;r.sequence=h.sequence;r.phase=state::queued;++stream_;return 0;
        }
        std::uint32_t control(std::uint32_t error,std::uint64_t correlation=0,bool retired=false)noexcept
        {
            if(control_ready_)return 17;if(!retired&&terminal_error_issued_)return 125;if(stream_==UINT64_MAX)return 90;
            info::header h{};h.owner=owner_;h.incarnation=incarnation_;h.sequence=stream_+1;h.correlation=correlation;
            h.kind=error?events::event_kind::resource_error:events::event_kind::resource_closed;h.error=error;
            h.control_flags=retired?events::event_source_retired:0;
            if(!info::encode(h,control_))return 22;control_sequence_=++stream_;control_ready_=true;if(!retired)terminal_error_issued_=true;return 0;
        }
        std::uint32_t request_error(std::size_t i,std::uint32_t error)noexcept
        {
            if(i>=used_||!error||retired_||!can_take())return 90;auto& r=slots_[i];
            info::header h{};h.owner=owner_;h.incarnation=incarnation_;h.sequence=stream_+1;h.correlation=r.ticket;
            h.kind=events::event_kind::resource_error;h.error=error;
            if(!info::encode(h,r.header))return 22;
            if(reserved_==r.sequence)reserved_=reservation_=0;
            r.discard=true;r.sequence=++stream_;r.length=0;r.phase=state::queued;return 0;
        }
        std::uint32_t peek(wire::binding const& owner,std::uint64_t id,std::size_t writer_extent,
            snapshot& output,std::size_t capacity=events::max_payload_bytes)noexcept
        {
            if(!owns(owner,id))return 13;if(reserved_)return 17;
            std::size_t selected=no_slot;std::uint64_t sequence=control_sequence_;bool found=control_ready_;
            for(std::size_t i{};i<used_;++i)if(slots_[i].phase==state::queued&&(!found||slots_[i].sequence<sequence)){selected=i;sequence=slots_[i].sequence;found=true;}
            if(!found)return retired_?125:11;
            auto const* header=selected==no_slot?control_.data():slots_[selected].header.data();auto n=selected==no_slot?0:slots_[selected].length;
            info::header h{};if(!info::decode(header,info::header_bytes,n,h))return 5;
            if(!events::event_control(h.kind)&&(!info::complete(h)||writer_extent!=h.writer_size||!writer_extent||writer_extent>16))return 95;
            auto total=info::header_bytes+n;if(total>capacity)return 90;if(nonce_==UINT64_MAX)return 90;
            try{snapshot next{};next.bytes=std::make_unique<std::uint8_t[]>(total);next.size=total;next.sequence=sequence;next.reservation=nonce_+1;
                std::copy(header,header+info::header_bytes,next.bytes.get());
                if(n)std::copy_n(slots_[selected].body.data(),n,next.bytes.get()+info::header_bytes);
                output=std::move(next);reserved_=sequence;reservation_=++nonce_;return 0;
            }catch(std::bad_alloc const&){return 12;}catch(...){return 5;}
        }
        std::uint32_t commit(wire::binding const& owner,std::uint64_t id,std::uint64_t sequence,std::uint64_t token)noexcept
        {
            if(!owns(owner,id))return 13;if(!sequence||reserved_!=sequence||!token||reservation_!=token)return 13;
            if(control_ready_&&control_sequence_==sequence)control_ready_=false;
            else{std::size_t i{};while(i<used_&&(slots_[i].phase!=state::queued||slots_[i].sequence!=sequence))++i;
                if(i==used_)return 13;info::header h{};if(!info::decode(slots_[i].header.data(),info::header_bytes,slots_[i].length,h))return 5;
                slots_[i].phase=h.kind==events::event_kind::service_request?state::delivered:state::completed;}
            reserved_=reservation_=0;return 0;
        }
        std::uint32_t cancel(wire::binding const& owner,std::uint64_t id,std::uint64_t sequence,std::uint64_t token)noexcept
        {if(!owns(owner,id))return 13;if(!sequence||reserved_!=sequence||!token||reservation_!=token)return 13;reserved_=reservation_=0;return 0;}
        std::uint32_t retire(std::uint32_t cleanup_error)noexcept
        {
            if(retired_)return 125;retired_=true;reserved_=reservation_=0;for(std::size_t i{};i<used_;++i)if(slots_[i].phase!=state::free)slots_[i].phase=state::completed;
            // Original native identity/history remains retained for diagnostics.
            // Replace any unconsumed error with a final actual retirement fact.
            control_ready_=false;return control(cleanup_error,0,true);
        }
    };
}
