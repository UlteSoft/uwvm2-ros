/*************************************************************
 * UWVM2 ROS Runtime Extension. APL-2.0.                     *
 *************************************************************/
#pragma once
#include "event.h"
#include <algorithm>
#include <deque>
#include <map>
#include <mutex>
#include <optional>
#include <new>
#include <utility>

namespace uwvm2::imported::ros::ipc
{
    enum class event_overflow : std::uint8_t { drop_oldest,drop_newest,reject,backpressure };
    inline bool parse_event_overflow(std::string_view value,event_overflow& result) noexcept
    {
        if(value=="drop-oldest") { result=event_overflow::drop_oldest; }
        else if(value=="drop-newest") { result=event_overflow::drop_newest; }
        else if(value=="reject") { result=event_overflow::reject; }
        else if(value=="backpressure") { result=event_overflow::backpressure; }
        else { return false; }return true;
    }
    struct event_stream_settings
    {
        std::uint64_t depth{},bytes{},record_bytes{},pending_events{},pending_bytes{};
        event_overflow overflow{event_overflow::reject};
    };
    // Trusted native snapshots only. There is no Guest statistics import or
    // implicit observation privilege. Bytes include vector capacity plus64B
    // record accounting; pending/reserved storage is reported separately.
    struct event_queue_statistics
    {
        std::uint64_t depth{},bytes{},pending_depth{},pending_bytes{},reserved_depth{},reserved_bytes{};
        std::uint64_t high_water_depth{},high_water_bytes{},enqueued{},dequeued{},dropped_oldest{},dropped_newest{};
        std::uint64_t rejected{},backpressured{},admission_refused{},completed{},closed{};
    };
    struct event_record
    {
        event_kind kind{};std::uint8_t flags{};std::uint32_t error{};std::uint64_t correlation{};
        std::vector<std::uint8_t> payload;
        std::uint64_t wire_bytes() const noexcept { return event_header_bytes+payload.size(); }
        std::uint64_t owned_bytes() const noexcept { return event_header_bytes+payload.capacity(); }
    };
    class event_stream
    {
        mutable std::mutex mutex_;
        event_stream_settings settings_{};event_header binding_{};std::uint32_t mask_{};
        bool configured_{},active_{true};std::uint64_t sequence_{1},queued_bytes_{},reserved_bytes_{};
        std::deque<event_record> records_;
        std::optional<event_record> pending_,control_error_,control_closed_;
        struct reserved_credit { std::uint64_t response{},owned{}; std::optional<event_record> completed; };
        std::map<std::uint64_t,reserved_credit> reservations_;
        event_queue_statistics totals_{};
        enum class front_source {none,data,completion,error,closed};
        front_source pinned_{front_source::none};std::uint64_t pinned_request_{};
        static void increment(std::uint64_t& value) noexcept { if(value!=UINT64_MAX) { ++value; } }
        static std::uint64_t sum(std::uint64_t a,std::uint64_t b) noexcept
        { return b>UINT64_MAX-a?UINT64_MAX:a+b; }
        std::uint64_t reserved_bytes() const noexcept
        { return reserved_bytes_; }
        bool room(std::uint64_t bytes) const noexcept
        {
            auto reserved{reserved_bytes()};auto available{settings_.bytes-128};
            return records_.size()+reservations_.size()<settings_.depth-2&&
                queued_bytes_<=available&&reserved<=available-queued_bytes_&&bytes<=available-queued_bytes_-reserved;
        }
        bool allowed(event_record const& record) const noexcept
        {
            auto header{binding_};header.kind=record.kind;header.flags=record.flags;header.error=record.error;
            header.correlation=record.correlation;header.sequence=sequence_;header.payload_bytes=record.payload.size();
            if(!(mask_&event_bit(record.kind))||!valid_event_header(header)||record.wire_bytes()>settings_.record_bytes) { return false; }
            if((record.kind==event_kind::service_request||record.kind==event_kind::action_feedback||record.kind==event_kind::action_result)&&
               (record.payload.size()<8||get(record.payload.data(),8)!=record.correlation)) { return false; }
            return true;
        }
        void high_water() noexcept
        {
            auto controls{std::uint64_t{control_error_?1u:0u}+std::uint64_t{control_closed_?1u:0u}};
            auto depth{static_cast<std::uint64_t>(records_.size()+reservations_.size())+controls+(pending_?1:0)};
            auto bytes{sum(sum(queued_bytes_,reserved_bytes()),controls*event_header_bytes)};
            if(pending_) { bytes=sum(bytes,pending_->owned_bytes()); }
            totals_.high_water_depth=std::max(totals_.high_water_depth,depth);
            totals_.high_water_bytes=std::max(totals_.high_water_bytes,bytes);
        }
        void promote()
        {
            if(pending_&&room(pending_->owned_bytes()))
            { auto bytes{pending_->owned_bytes()};records_.push_back(std::move(*pending_));queued_bytes_+=bytes;pending_.reset(); }
        }
        void append(event_record record)
        { auto bytes{record.owned_bytes()};records_.push_back(std::move(record));queued_bytes_+=bytes;increment(totals_.enqueued);high_water(); }
        auto first_completion() const
        { return std::find_if(reservations_.begin(),reservations_.end(),[](auto const& item){return item.second.completed.has_value();}); }
        event_record const* front() const noexcept
        {
            if(pinned_==front_source::data) { return records_.empty()?nullptr:&records_.front(); }
            if(pinned_==front_source::completion) { auto found{reservations_.find(pinned_request_)};return found==reservations_.end()||!found->second.completed?nullptr:&*found->second.completed; }
            if(pinned_==front_source::error) { return control_error_?&*control_error_:nullptr; }
            if(pinned_==front_source::closed) { return control_closed_?&*control_closed_:nullptr; }
            if(!records_.empty()) { return &records_.front(); }
            auto completed{first_completion()};if(completed!=reservations_.end()) { return &*completed->second.completed; }
            if(control_error_) { return &*control_error_; }return control_closed_?&*control_closed_:nullptr;
        }
        event_header header_for(event_record const& record) const noexcept
        {
            auto header{binding_};header.kind=record.kind;header.flags=record.flags;header.error=record.error;
            header.correlation=record.correlation;header.sequence=sequence_;header.payload_bytes=record.payload.size();return header;
        }
        template<typename Admission>
        call_result peek_checked_locked(std::uint64_t capacity,Admission&& admission)
        {
            if(!configured_) { return {22,{}}; }promote();auto record{front()};if(!record) { return {11,{}}; }
            if(sequence_==UINT64_MAX) { return {90,{}}; }auto size{record->wire_bytes()};
            if(size>capacity) { std::vector<std::uint8_t> required;required.reserve(8);put(required,size,8);return {90,std::move(required)}; }
            auto header{header_for(*record)};
            if(auto error=admission(header)) {return {error,{}};}
            std::vector<std::uint8_t> bytes(static_cast<std::size_t>(size));if(!encode_event_header(header,bytes.data(),bytes.size())) { return {22,{}}; }
            if(!record->payload.empty()) { std::copy(record->payload.begin(),record->payload.end(),bytes.begin()+event_header_bytes); }
            if(pinned_==front_source::none)
            {
                if(!records_.empty()) { pinned_=front_source::data; }
                else {auto completed{first_completion()};if(completed!=reservations_.end()) {pinned_=front_source::completion;pinned_request_=completed->first;}else {pinned_=control_error_?front_source::error:front_source::closed;}}
            }
            return {0,std::move(bytes)};
        }
        call_result peek_locked(std::uint64_t capacity)
        {return peek_checked_locked(capacity,[](event_header const&){return std::uint32_t{};});}
        std::uint32_t commit_locked(std::uint64_t sequence)
        {
            if(!configured_) { return 22; }if(pinned_==front_source::none||!front()) { return 9; }
            if(sequence!=sequence_) { return 22; }
            if(pinned_==front_source::data) { queued_bytes_-=records_.front().owned_bytes();records_.pop_front(); }
            else if(pinned_==front_source::completion) { auto found{reservations_.find(pinned_request_)};reserved_bytes_-=found->second.owned;reservations_.erase(found); }
            else if(pinned_==front_source::error) { control_error_.reset(); }else { control_closed_.reset(); }
            pinned_=front_source::none;pinned_request_=0;++sequence_;increment(totals_.dequeued);return 0;
        }
    public:
        // All settings are explicit. Two metadata slots are reserved within the
        // configured depth/bytes so retiring a source cannot hide its permitted
        // error/closed notification behind previously queued sensitive content.
        event_stream(event_stream_settings settings,event_header binding,std::uint32_t mask)
            :settings_{settings},binding_{binding},mask_{mask}
        {
            configured_=settings.depth>=3&&settings.record_bytes>=event_header_bytes&&settings.record_bytes<=max_payload_bytes&&
                settings.bytes>=settings.record_bytes+128&&binding.backend>=1&&binding.backend<=2&&binding.source_handle&&binding.instance&&binding.session&&
                mask&&!(mask&~std::uint32_t{127})&&
                (settings.overflow==event_overflow::backpressure?
                    settings.pending_events==1&&settings.pending_bytes>=settings.record_bytes:
                    settings.pending_events==0&&settings.pending_bytes==0);
        }
        event_stream(event_stream const&)=delete;event_stream& operator=(event_stream const&)=delete;
        bool configured() const noexcept { return configured_; }
        event_header binding() const noexcept { return binding_; }
        std::uint32_t allowed_kinds() const noexcept { return mask_; }
        bool active() const noexcept { std::lock_guard<std::mutex> guard{mutex_};return configured_&&active_; }
        // Return0 for retained admission,11 for a bounded full refusal,90 for an
        // oversized record and125 after source retirement. Never wait for credit
        // inside a native executor/callback. The optional pending push is exactly
        // one separately budgeted record; a second pending push receives11.
        std::uint32_t enqueue(event_record record)
        {
            std::lock_guard<std::mutex> guard{mutex_};
            if(!configured_) { return 22; }if(!active_) { return 125; }
            if(!allowed(record)) { increment(totals_.rejected);return record.wire_bytes()>settings_.record_bytes?90:22; }
            if(event_control(record.kind)&&record.correlation==0)
            {
                auto tag{record.kind==event_kind::resource_error?front_source::error:front_source::closed};
                if(pinned_==tag) { increment(totals_.admission_refused);return 11; }
                auto& slot{record.kind==event_kind::resource_error?control_error_:control_closed_};
                if(slot) { increment(totals_.dropped_oldest); }slot=std::move(record);increment(totals_.enqueued);high_water();return 0;
            }
            promote();
            if(!pending_&&room(record.owned_bytes())) { append(std::move(record));return 0; }
            if(settings_.overflow==event_overflow::drop_oldest)
            {
                while(!room(record.owned_bytes())&&!records_.empty())
                {
                    auto oldest{records_.begin()};if(pinned_==front_source::data) { ++oldest;if(oldest==records_.end()) {break;} }
                    queued_bytes_-=oldest->owned_bytes();records_.erase(oldest);increment(totals_.dropped_oldest);
                }
                if(room(record.owned_bytes())) { append(std::move(record));return 0; }
            }
            else if(settings_.overflow==event_overflow::backpressure)
            {
                increment(totals_.backpressured);
                if(!pending_&&record.owned_bytes()<=settings_.pending_bytes)
                { pending_=std::move(record);increment(totals_.enqueued);high_water();return 0; }
                increment(totals_.admission_refused);return 11;
            }
            if(settings_.overflow==event_overflow::drop_newest) { increment(totals_.dropped_newest); }
            else { increment(totals_.rejected); }increment(totals_.admission_refused);return 11;
        }
        // Reserve response storage BEFORE native asynchronous side effects.
        // Input capacity and fixed task-accounting bytes share the same explicit
        // stream budget; queued requests cannot grow outside the byte quota.
        std::uint32_t reserve(std::uint64_t request,std::uint64_t response_bytes,std::uint64_t request_owned_bytes)
        {
            std::lock_guard<std::mutex> guard{mutex_};
            if(!configured_||!request||response_bytes<event_header_bytes||response_bytes>settings_.record_bytes) { return 22; }
            if(!active_) { return 125; }if(reservations_.count(request)) { return 17; }
            auto total{sum(response_bytes,request_owned_bytes)};
            if(pending_||!room(total)) { increment(totals_.backpressured);increment(totals_.admission_refused);return 11; }
#if defined(__cpp_exceptions)
            try { reservations_.emplace(request,reserved_credit{response_bytes,total,{}}); }
            catch(std::bad_alloc const&) { increment(totals_.admission_refused);return 12; }
#else
            reservations_.emplace(request,reserved_credit{response_bytes,total,{}});
#endif
            reserved_bytes_+=total;high_water();return 0;
        }
        void cancel_reservation(std::uint64_t request) noexcept
        { std::lock_guard<std::mutex> guard{mutex_};auto found{reservations_.find(request)};if(found!=reservations_.end()) { reserved_bytes_-=found->second.owned;reservations_.erase(found); } }
        // Every successful submission completes at most once. Reserved records
        // bypass overflow eviction: capacity already belongs to this request.
        // After seal, no response content can be retained or delivered.
        std::uint32_t complete(std::uint64_t request,event_record record)
        {
            std::lock_guard<std::mutex> guard{mutex_};if(!active_) { return 125; }
            auto found{reservations_.find(request)};if(found==reservations_.end()) { return 9; }
            if(found->second.completed) { return 17; }
            auto maximum{found->second.response};
            if(record.correlation!=request||!allowed(record)||record.owned_bytes()>maximum)
            {
                record={event_kind::resource_error,event_external_effects_may_remain,90,request,{}};
                if(!allowed(record)) { reserved_bytes_-=found->second.owned;reservations_.erase(found);increment(totals_.rejected);return 13; }
            }
            // Reuse the already allocated request entry. Completion never needs
            // a deque/map allocation and overflow never evicts this response.
            reserved_bytes_-=found->second.owned;found->second.owned=record.owned_bytes();
            reserved_bytes_+=found->second.owned;found->second.completed=std::move(record);
            increment(totals_.enqueued);increment(totals_.completed);high_water();return 0;
        }
        // Two-phase Native delivery pins an existing slot, never an extra queue.
        // Source/observer quotas can refuse admission without losing this record
        // or its async request ID. Repeated peeks return the same bytes/sequence;
        // callbacks cannot evict/replace a pinned data or metadata record.
        call_result peek(std::uint64_t capacity)
        { std::lock_guard<std::mutex> guard{mutex_};return peek_locked(capacity); }
        // A trusted Native delivery preview may inspect only immutable header
        // metadata while this queue lock is held. Refusal does not allocate,
        // pin, consume, or debit the record. The callback must not reenter this
        // stream; its quota state belongs to the single authenticated IPC owner.
        template<typename Admission>
        call_result peek_if(std::uint64_t capacity,Admission&& admission)
        {std::lock_guard<std::mutex> guard{mutex_};return peek_checked_locked(capacity,std::forward<Admission>(admission));}
        std::uint32_t commit(std::uint64_t sequence)
        { std::lock_guard<std::mutex> guard{mutex_};return commit_locked(sequence); }
        // Check the exact pinned ticket BEFORE calling admission. A successful
        // callback may atomically debit source and observer credits: the held
        // lock then guarantees commit cannot target a substituted newer front.
        // Wrong/stale tickets and quota refusal preserve both record and debit.
        template<typename Admission>
        std::uint32_t commit_if(std::uint64_t sequence,Admission&& admission)
        {
            std::lock_guard<std::mutex> guard{mutex_};
            if(!configured_) {return 22;}auto record{front()};if(pinned_==front_source::none||!record) {return 9;}
            if(sequence!=sequence_) {return 22;}if(sequence_==UINT64_MAX) {return 90;}
            if(auto error=admission(header_for(*record))) {return error;}
            return commit_locked(sequence);
        }
        // Private legacy one-step poll remains atomic and capacity-retaining.
        // The new Worker always uses peek/commit around its authority/memory copy.
        call_result poll(std::uint64_t capacity)
        { std::lock_guard<std::mutex> guard{mutex_};auto result{peek_locked(capacity)};if(!result.error) {auto error{commit_locked(sequence_)};if(error) {return {error,{}};}}return result; }
        event_readiness readiness() const noexcept
        {
            std::lock_guard<std::mutex> guard{mutex_};event_readiness value{};
            if(!configured_) { return value; }
            value.ready_events=records_.size()+(control_error_?1:0)+(control_closed_?1:0);
            for(auto const& item:reservations_) { if(item.second.completed) { ++value.ready_events; } }
            if(auto record=front()) { value.next_event_bytes=record->wire_bytes(); }
            if(active_&&!pending_)
            {
                auto used{queued_bytes_+reserved_bytes()};auto budget{settings_.bytes-128};
                value.admission_slots=settings_.depth-2-records_.size()-reservations_.size();
                value.admission_bytes=used>budget?0:budget-used;
            }
            return value;
        }
        event_queue_statistics statistics() const noexcept
        {
            std::lock_guard<std::mutex> guard{mutex_};auto value{totals_};
            value.depth=records_.size()+(control_error_?1:0)+(control_closed_?1:0);
            value.bytes=queued_bytes_+(control_error_?event_header_bytes:0)+(control_closed_?event_header_bytes:0);
            value.pending_depth=pending_?1:0;value.pending_bytes=pending_?pending_->owned_bytes():0;
            for(auto const& item:reservations_)
            {if(item.second.completed) {++value.depth;value.bytes+=item.second.owned;}else {++value.reserved_depth;value.reserved_bytes+=item.second.owned;}}return value;
        }
        // Detached debugger diagnostics must not wait for a mutex held by a
        // cooperatively stopped host call. A busy queue leaves output intact;
        // this observation never peeks, promotes, commits or consumes a slot.
        bool try_statistics(event_queue_statistics& output) const noexcept
        {
            std::unique_lock<std::mutex> guard{mutex_,std::try_to_lock};if(!guard.owns_lock()) {return false;}
            auto value{totals_};
            value.depth=records_.size()+(control_error_?1:0)+(control_closed_?1:0);
            value.bytes=queued_bytes_+(control_error_?event_header_bytes:0)+(control_closed_?event_header_bytes:0);
            value.pending_depth=pending_?1:0;value.pending_bytes=pending_?pending_->owned_bytes():0;
            for(auto const& item:reservations_)
            {if(item.second.completed) {++value.depth;value.bytes+=item.second.owned;}else {++value.reserved_depth;value.reserved_bytes+=item.second.owned;}}
            output=value;return true;
        }
        // Close/revoke first discard ALL sensitive data/pending/reservations.
        // Only explicitly permitted metadata remains. The separate observer
        // capability still needs its own live Instance/backend/permission lease
        // at delivery; retiring that observer discards even these notifications.
        void seal(std::uint32_t reason,bool external_effects_may_remain=false) noexcept
        {
            std::lock_guard<std::mutex> guard{mutex_};if(!active_) { return; }
            active_=false;if(pinned_!=front_source::none&&sequence_!=UINT64_MAX) {++sequence_;}pinned_=front_source::none;pinned_request_=0;records_.clear();pending_.reset();reservations_.clear();reserved_bytes_=0;queued_bytes_=0;control_error_.reset();control_closed_.reset();
            auto flags{static_cast<std::uint8_t>(event_source_retired|(external_effects_may_remain?event_external_effects_may_remain:0))};
            if(reason&&(mask_&event_bit(event_kind::resource_error))) { control_error_=event_record{event_kind::resource_error,flags,reason,0,{}};increment(totals_.enqueued); }
            if(mask_&event_bit(event_kind::resource_closed)) { control_closed_=event_record{event_kind::resource_closed,flags,reason,0,{}};increment(totals_.enqueued); }
            increment(totals_.closed);high_water();
        }
        void discard() noexcept
        {
            std::lock_guard<std::mutex> guard{mutex_};active_=false;pinned_=front_source::none;pinned_request_=0;records_.clear();pending_.reset();reservations_.clear();reserved_bytes_=0;
            control_error_.reset();control_closed_.reset();queued_bytes_=0;
        }
    };
}
