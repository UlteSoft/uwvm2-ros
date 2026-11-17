/*************************************************************
 * UWVM2 ROS Runtime Extension. APL-2.0.                     *
 *************************************************************/
#pragma once
#include <uwvm2/utils/allocator/fast_io_strict/object.h>
#include <uwvm2/utils/container/unique.h>
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
#ifndef UWVM_MODULE
#include <uwvm2/utils/thread/monotonic_clock.h>
#endif
#include "event.h"
#include "../common/checkpoint_environment.h"
#include "../common/synchronous_wait.h"
#include <algorithm>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <new>
#include <utility>

namespace uwvm2::imported::ros::ipc
{
    enum class event_overflow : std::uint8_t { drop_oldest,drop_newest,reject,backpressure,wait };
    inline bool parse_event_overflow(std::string_view value,event_overflow& result) noexcept
    {
        if(value=="reject") { result=event_overflow::reject; }
        else if(value=="wait") { result=event_overflow::wait; }
        else { return false; }return true;
    }
    struct event_stream_settings
    {
        std::uint64_t depth{UINT64_MAX},bytes{UINT64_MAX},record_bytes{UINT64_MAX},pending_events{},pending_bytes{};
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
        ::uwvm2::utils::container::std_vector<std::uint8_t> payload;
        std::uint64_t wire_bytes() const noexcept { return event_header_bytes+payload.size(); }
        std::uint64_t owned_bytes() const noexcept { return event_header_bytes+payload.capacity(); }
    };
    class event_stream
    {
        mutable std::mutex mutex_;
        struct waiter_link { synchronous_quota_wait* waiter{};waiter_link* next{}; };
        waiter_link* waiters_{};
        void notify_locked() noexcept
        { for(auto* entry=waiters_;entry;entry=entry->next) { entry->waiter->notify(); } }
        std::uint64_t controls() const noexcept
        { return (control_error_?1u:0u)+(control_closed_?1u:0u); }
        std::uint64_t occupied_depth() const noexcept
        { return sum(sum(records_.size(),reservations_.size()),controls()); }
        std::uint64_t occupied_bytes() const noexcept
        { return sum(sum(queued_bytes_,reserved_bytes_),controls()*event_header_bytes); }
        event_stream_settings settings_{};event_header binding_{};std::uint32_t mask_{};
        bool configured_{},active_{true};std::uint64_t sequence_{1},queued_bytes_{},reserved_bytes_{};
        ::uwvm2::utils::container::std_deque<event_record> records_;
        std::optional<event_record> pending_,control_error_,control_closed_;
        struct reserved_credit { std::uint64_t response{},owned{}; std::optional<event_record> completed; };
        ::uwvm2::utils::container::std_map<std::uint64_t,reserved_credit> reservations_;
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
            auto const used{occupied_bytes()};
            return occupied_depth()<settings_.depth && used<=settings_.bytes && bytes<=settings_.bytes-used;
        }
        bool allowed(event_record const& record) const noexcept
        {
            auto header{binding_};header.kind=record.kind;header.flags=record.flags;header.error=record.error;
            header.correlation=record.correlation;header.sequence=sequence_;header.payload_bytes=record.payload.size();
            if(!(mask_&event_bit(record.kind))||!valid_event_header(header)||record.wire_bytes()>settings_.record_bytes||record.wire_bytes()>max_payload_bytes) { return false; }
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
        void append(event_record&& record)
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
            if(size>capacity) { ::uwvm2::utils::container::std_vector<std::uint8_t> required;required.reserve(8);put(required,size,8);return {90,std::move(required)}; }
            auto header{header_for(*record)};
            if(auto error=admission(header)) {return {error,{}};}
            ::uwvm2::utils::container::std_vector<std::uint8_t> bytes(static_cast<std::size_t>(size));if(!encode_event_header(header,bytes.data(),bytes.size())) { return {22,{}}; }
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
            pinned_=front_source::none;pinned_request_=0;++sequence_;increment(totals_.dequeued);notify_locked();return 0;
        }
    public:
        // Missing quotas are unlimited. Explicit ceilings include retained
        // control records, data and reservations. Terminal state remains visible
        // even when a configured zero leaves no room for a control record.
        event_stream(event_stream_settings settings,event_header binding,std::uint32_t mask)
            :settings_{settings},binding_{binding},mask_{mask}
        {
            configured_=binding.backend>=1&&binding.backend<=2&&binding.source_handle&&binding.instance&&binding.session&&
                mask&&!(mask&~std::uint32_t{127})&&
                (settings.overflow==event_overflow::reject||settings.overflow==event_overflow::wait);
        }
        event_stream(event_stream const&)=delete;event_stream& operator=(event_stream const&)=delete;
        bool configured() const noexcept { return configured_; }
        event_header binding() const noexcept { return binding_; }
        std::uint32_t allowed_kinds() const noexcept { return mask_; }
        bool active() const noexcept { std::lock_guard<std::mutex> guard{mutex_};return configured_&&active_; }
        // This primitive never consumes the caller's record on refusal.
        // Owners release their ingress/connection locks before wait_for_space,
        // then retry this same owned record; no native request is replayed.
        std::uint32_t enqueue_owned(event_record& record)
        {
            std::lock_guard<std::mutex> guard{mutex_};
            if(!configured_) { return 22; }if(!active_) { return 125; }
            if(!allowed(record))
            { increment(totals_.rejected);return record.wire_bytes()>settings_.record_bytes||record.wire_bytes()>max_payload_bytes?90:22; }
            if(event_control(record.kind)&&record.correlation==0)
            {
                auto& slot{record.kind==event_kind::resource_error?control_error_:control_closed_};
                if(slot||!room(record.owned_bytes())) { increment(totals_.admission_refused);return 11; }
                slot=std::move(record);increment(totals_.enqueued);high_water();return 0;
            }
            if(!room(record.owned_bytes())) { increment(totals_.rejected);increment(totals_.admission_refused);return 11; }
#if defined(__cpp_exceptions)
            try
#endif
            { append(std::move(record));return 0; }
#if defined(__cpp_exceptions)
            catch(std::bad_alloc const&) { increment(totals_.admission_refused);return 12; }
#endif
        }
        std::uint32_t enqueue(event_record record) { return enqueue_owned(record); }
        void notify_waiters() noexcept { std::lock_guard<std::mutex> guard{mutex_};notify_locked(); }
        std::uint32_t wait_for_space(std::uint64_t bytes,
            std::chrono::steady_clock::time_point deadline,bool wait_records,bool wait_bytes,
            bool (*cancelled)(void*) noexcept=nullptr,void* context=nullptr,int cancellation_fd=-1)
        {
            ::uwvm2::utils::container::std_unique_ptr<synchronous_quota_wait> fallback;
            auto* waiter{synchronous_wait_context::present()?synchronous_wait_context::waiter():nullptr};
            if(!waiter && !synchronous_wait_context::present())
            { fallback.reset(::uwvm2::utils::allocator::fast_io_strict::try_object<synchronous_quota_wait>([&](synchronous_quota_wait* uwvm_allocated) { return ::new(static_cast<void*>(uwvm_allocated)) synchronous_quota_wait{}; }));waiter=fallback.get(); }
            if(!waiter) { return 12; }
            std::unique_lock<std::mutex> guard{mutex_};
            for(;;)
            {
                if(!configured_) { return 22; }
                if(!active_||waiter->cancelled()||(cancelled&&cancelled(context))) { return 125; }
                if(room(bytes)) { return 0; }
                if(bytes>settings_.bytes || !settings_.depth) { return 90; }
                bool const count{occupied_depth()>=settings_.depth};
                auto const used{occupied_bytes()};
                bool const data{used>settings_.bytes || bytes>settings_.bytes-used};
                if((count&&!wait_records)||(data&&!wait_bytes)) { return 11; }
                auto const now{::uwvm2::utils::host_clock::now()};
                if(!::uwvm2::utils::host_clock::available()) { return 5; }
                if(now>=deadline) { return 110; }
                waiter_link pending{waiter,waiters_};waiters_=&pending;
                guard.unlock();auto const outcome{waiter->until(deadline,cancellation_fd)};guard.lock();
                auto** link{&waiters_};while(*link!=&pending) { link=&(*link)->next; }*link=pending.next;
                if(outcome!=quota_wait_result::ready) { return outcome==quota_wait_result::cancelled?125u:5u; }
            }
        }
        // Reserve response storage BEFORE native asynchronous side effects.
        // Input capacity and fixed task-accounting bytes share the same explicit
        // stream budget; queued requests cannot grow outside the byte quota.
        std::uint32_t reserve(std::uint64_t request,std::uint64_t response_bytes,std::uint64_t request_owned_bytes)
        {
            std::lock_guard<std::mutex> guard{mutex_};
            if(!configured_||!request||response_bytes<event_header_bytes||response_bytes>settings_.record_bytes) { return 22; }
            if(!active_) { return 125; }if(reservations_.count(request)) { return 17; }
            if(request_owned_bytes>UINT64_MAX-response_bytes) { return 90; }
            auto total{response_bytes+request_owned_bytes};
            if(!room(total)) { increment(totals_.backpressured);increment(totals_.admission_refused);return 11; }
#if defined(__cpp_exceptions)
            try { reservations_.emplace(request,reserved_credit{response_bytes,total,{}}); }
            catch(std::bad_alloc const&) { increment(totals_.admission_refused);return 12; }
#else
            reservations_.emplace(request,reserved_credit{response_bytes,total,{}});
#endif
            reserved_bytes_+=total;high_water();return 0;
        }
        void cancel_reservation(std::uint64_t request) noexcept
        { std::lock_guard<std::mutex> guard{mutex_};auto found{reservations_.find(request)};if(found!=reservations_.end()) { reserved_bytes_-=found->second.owned;reservations_.erase(found);notify_locked(); } }
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
            increment(totals_.enqueued);increment(totals_.completed);high_water();notify_locked();return 0;
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
                auto used{occupied_bytes()};auto budget{settings_.bytes};
                auto depth{occupied_depth()};value.admission_slots=depth>settings_.depth?0:settings_.depth-depth;
                value.admission_bytes=used>budget?0:budget-used;
            }
            return value;
        }
        // Called only by the trusted closed-cohort collector. Try-lock avoids
        // waiting for a host operation stopped while owning this queue. No
        // peek/promote/consume or waiter state mutation is permitted here.
        bool copy_checkpoint(checkpoint::data::queue& output) const
        {
            std::unique_lock<std::mutex> guard{mutex_,std::try_to_lock};
            if(!guard.owns_lock() || waiters_)return false;
            checkpoint::data::queue next{};
            next.state={settings_.depth,settings_.bytes,settings_.record_bytes,settings_.pending_events,
                settings_.pending_bytes,static_cast<std::uint64_t>(settings_.overflow),mask_,sequence_,
                queued_bytes_,reserved_bytes_,static_cast<std::uint64_t>(pinned_),pinned_request_};
            next.configured=configured_;next.active=active_;
            auto const& t=totals_;
            next.totals={t.depth,t.bytes,t.pending_depth,t.pending_bytes,t.reserved_depth,t.reserved_bytes,
                t.high_water_depth,t.high_water_bytes,t.enqueued,t.dequeued,t.dropped_oldest,t.dropped_newest,
                t.rejected,t.backpressured,t.admission_refused,t.completed,t.closed};
            auto append=[&](std::uint8_t location,event_record const* record,std::uint64_t request=0,
                std::uint64_t response=0,std::uint64_t owned=0)
            {
                checkpoint::data::event value{};value.location=location;value.present=record!=nullptr;
                value.request=request;value.response_credit=response;value.owned_bytes=owned;
                if(record)
                {
                    value.kind=static_cast<std::uint16_t>(record->kind);value.flags=record->flags;
                    value.error=record->error;value.correlation=record->correlation;
                    if(location!=4)value.owned_bytes=record->owned_bytes();
                    value.payload.reserve(record->payload.size());
                    for(auto b:record->payload)value.payload.push_back(static_cast<std::byte>(b));
                }
                next.events.push_back(std::move(value));
            };
            for(auto const& record:records_)append(0,&record);
            if(pending_)append(1,&*pending_);
            if(control_error_)append(2,&*control_error_);
            if(control_closed_)append(3,&*control_closed_);
            for(auto const& [request,credit]:reservations_)append(4,credit.completed?&*credit.completed:nullptr,request,credit.response,credit.owned);
            output=std::move(next);return true;
        }
        // Trusted stopped-world preparation only. The file supplies local DATA;
        // the existing stream supplies the actual session/instance binding and
        // current ceilings. The original queue is never touched on refusal.
        std::shared_ptr<event_stream> prepare_checkpoint(checkpoint::data::queue const& saved) const
        {
            std::unique_lock<std::mutex> guard{mutex_,std::try_to_lock};
            if(!guard.owns_lock() || waiters_ || !configured_ || !saved.configured || saved.active!=active_ ||
               saved.state[0]!=settings_.depth || saved.state[1]!=settings_.bytes || saved.state[2]!=settings_.record_bytes ||
               saved.state[3]!=settings_.pending_events || saved.state[4]!=settings_.pending_bytes ||
               saved.state[5]!=static_cast<std::uint64_t>(settings_.overflow) || saved.state[6]!=mask_ ||
               !saved.state[7] || saved.state[10]>4)return {};
            auto next=::uwvm2::utils::container::make_shared<event_stream>(settings_,binding_,mask_);
            next->active_=saved.active;next->sequence_=saved.state[7];
            unsigned previous{},optional{};std::uint64_t last_request{},encoded_queued{},encoded_reserved{};
            for(auto const& value:saved.events)
            {
                if(value.location>4 || value.location<previous || (!value.present && value.location!=4))return {};
                previous=value.location;
                if(value.location>0 && value.location<4)
                {auto bit=1u<<value.location;if(optional&bit)return {};optional|=bit;}
                if(value.location==4)
                {
                    if(!value.request || value.request<=last_request || value.response_credit<event_header_bytes ||
                       value.response_credit>settings_.record_bytes || (!value.present && value.owned_bytes<value.response_credit))return {};
                    last_request=value.request;
                    // Do not resurrect an external request already delivered,
                    // abandoned or retired in the current real session.
                    auto live=reservations_.find(value.request);
                    if(live==reservations_.end() || live->second.response!=value.response_credit ||
                       live->second.completed.has_value()!=value.present)return {};
                }
                else if(value.request || value.response_credit || !value.present)return {};
                event_record record{};
                if(value.present)
                {
                    if(value.kind<1 || value.kind>7 || value.owned_bytes<event_header_bytes ||
                       value.payload.size()>value.owned_bytes-event_header_bytes)return {};
                    record.kind=static_cast<event_kind>(value.kind);record.flags=value.flags;record.error=value.error;
                    record.correlation=value.correlation;record.payload.reserve(value.payload.size());
                    for(auto b:value.payload)record.payload.push_back(std::to_integer<std::uint8_t>(b));
                    if(!next->allowed(record) ||
                       (value.location==2 && record.kind!=event_kind::resource_error) ||
                       (value.location==3 && record.kind!=event_kind::resource_closed) ||
                       (value.location==4 && (record.wire_bytes()>value.response_credit ||
                           (record.kind!=event_kind::service_response && record.kind!=event_kind::resource_error) || record.correlation!=value.request)))return {};
                }
                else if(value.kind || value.flags || value.error || value.correlation || !value.payload.empty())return {};
                if(value.location==0)
                {
                    if(value.owned_bytes>UINT64_MAX-encoded_queued || record.owned_bytes()>UINT64_MAX-next->queued_bytes_)return {};
                    encoded_queued+=value.owned_bytes;next->queued_bytes_+=record.owned_bytes();next->records_.push_back(std::move(record));
                }
                else if(value.location==1)next->pending_=std::move(record);
                else if(value.location==2)next->control_error_=std::move(record);
                else if(value.location==3)next->control_closed_=std::move(record);
                else
                {
                    if(value.owned_bytes>UINT64_MAX-encoded_reserved)return {};
                    encoded_reserved+=value.owned_bytes;
                    auto charged=std::max(value.owned_bytes,value.present?record.owned_bytes():std::uint64_t{});
                    if(charged>UINT64_MAX-next->reserved_bytes_)return {};
                    next->reserved_bytes_+=charged;
                    reserved_credit credit{value.response_credit,charged,{}};
                    if(value.present)credit.completed=std::move(record);
                    next->reservations_.emplace(value.request,std::move(credit));
                }
            }
            if(encoded_queued!=saved.state[8] || encoded_reserved!=saved.state[9] ||
               next->reservations_.size()!=reservations_.size() || next->occupied_depth()>settings_.depth ||
               next->occupied_bytes()>settings_.bytes || (next->pending_ &&
                   (!settings_.pending_events || next->pending_->owned_bytes()>settings_.pending_bytes)))return {};
            next->pinned_=static_cast<front_source>(saved.state[10]);next->pinned_request_=saved.state[11];
            if((next->pinned_!=front_source::completion && next->pinned_request_) ||
               (next->pinned_!=front_source::none && !next->front()))return {};
            auto const& t=saved.totals;
            next->totals_={t[0],t[1],t[2],t[3],t[4],t[5],t[6],t[7],t[8],t[9],t[10],t[11],t[12],t[13],t[14],t[15],t[16]};
            return next;
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
            if(reason&&(mask_&event_bit(event_kind::resource_error))&&room(event_header_bytes)) { control_error_=event_record{event_kind::resource_error,flags,reason,0,{}};increment(totals_.enqueued); }
            if((mask_&event_bit(event_kind::resource_closed))&&room(event_header_bytes)) { control_closed_=event_record{event_kind::resource_closed,flags,reason,0,{}};increment(totals_.enqueued); }
            increment(totals_.closed);high_water();notify_locked();
        }
        void discard() noexcept
        {
            std::lock_guard<std::mutex> guard{mutex_};active_=false;pinned_=front_source::none;pinned_request_=0;records_.clear();pending_.reset();reservations_.clear();reserved_bytes_=0;
            control_error_.reset();control_closed_.reset();queued_bytes_=0;notify_locked();
        }
    };
}
