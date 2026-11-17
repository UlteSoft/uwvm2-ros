/** @file
 * @brief Fixed-capacity device command ledger with retained source pins and stop debt.
 * @details This host-only core never grants authority or performs I/O. A provider
 * authenticates the actual source witnesses and controller lease before submit,
 * and again holds the complete authority group across cross_effect and each
 * bounded nonblocking native write. Claiming work is not crossing that boundary.
 * The receiver independently checks provider/controller fencing and deadlines.
 * A partial write, missing ACK or ambiguous cleanup is unknown, never a retry.
 *
 * Every command, observation pin and recovery record has fixed reserved storage.
 * Source pin visits check the real sealed allocation; source identity and capture
 * time must additionally come from a trusted ingress adapter. None is inferred
 * from guest metadata. No callback or native operation runs under this mutex.
 * Stop has its own pre-reserved identity and does not need a normal request slot.
 * This ledger has no watchdog thread or crash persistence, and never proves a
 * physical machine stopped solely from a transport acknowledgement.
 */
#pragma once
#include "device_command.h"
#include "control_arbitration.h"
namespace uwvm2::imported::ros::scene::devices
{
    inline constexpr std::size_t maximum_handles=16,maximum_requests=16,maximum_observations=8,maximum_command_bytes=8192;
    struct owner
    {handle environment{},grant{},generation{};explicit operator bool()const noexcept{return environment&&grant&&generation;}friend bool operator==(owner const&,owner const&)=default;};
    enum class state:std::uint8_t{admitted,queued,running,succeeded,failed,cancelled,outcome_unknown};
    enum class effect:std::uint8_t{none,locally_accepted,committed,observed_remote,partial,unknown};
    enum class reason:std::uint8_t{none,deadline,closed,retired,stopped,provider_changed,source_unavailable,native_rejected,native_unknown};
    struct receipt
    {handle request{};state status{};effect outcome{};handle provider_generation{};reason why{};friend bool operator==(receipt const&,receipt const&)=default;};
    /** @brief Trusted ingress witness accompanying an independently pinned allocation.
     * @details source/capture/authority fields are compared with the actual source
     * facade by the provider. The core verifies the buffer identity, SHA, frame
     * and bounded host-steady freshness; possession alone confers no permission.
     */
    struct observation
    {
        handle source{},source_generation{},capture{},authority_epoch{},buffer{};
        std::uint64_t captured_ns{};
        buffers::digest digest{};buffers::text<96> frame;
        friend bool operator==(observation const&,observation const&)=default;
    };
    struct observations
    {std::array<observation,maximum_observations> values{};std::array<buffers::registry::sealed_lease,maximum_observations> pins;std::size_t count{};};
    struct plan
    {
        buffers::profile profile,stop_profile;scalar_schema schema;
        std::uint64_t handles{},requests{},body_bytes{},observations{},minimum_observations{},maximum_age_ns{},maximum_deadline_ns{},storage_bytes{};
    };
    struct submission
    {handle device{},key{};std::uint64_t deadline{};control::lease controller;buffers::profile schema;bytes body;};
    /** @brief Detached work metadata; original pins stay in the retained request. */
    struct work
    {
        handle ticket{},device{},key{};receipt result;std::uint64_t deadline{};
        control::lease controller;scalar_command command;
        std::array<observation,maximum_observations> observations{};std::size_t observation_count{};
    };
    enum class completion:std::uint8_t{no_effect,executed,unknown};
    struct usage
    {std::uint64_t handles{},retained_handles{},requests{},queued{},running{},unknown{},pins{},reserved_bytes{};bool latched{},retired{},stop_pending{},stop_acknowledged{},stopped_observed{};handle stop_reservation{};};
    class core
    {
        struct opened
        {
            handle id{},last_key{};buffers::digest last_digest{};receipt last_receipt;
            handle stop_key{};receipt stop_receipt;
            bool published{},closing{},last_released{},stop_released{};
        };
        struct record
        {
            handle device{},key{},ticket{};receipt result;std::uint64_t deadline{};
            control::lease controller;scalar_command command;observations evidence;
            bool crossed{},worker{},cleanup{};
        };
        mutable std::mutex mutex_;
        owner owner_;plan policy_;handle provider_{},reserved_stop_{},stopped_witness_{},last_reconciliation_{};
        receipt stop_;std::array<opened,maximum_handles> handles_{};std::array<record,maximum_requests> records_{};
        bool configured_{},retired_{},latched_{},stop_ack_{};
#if defined(__linux__)
        ::pid_t process_{::getpid()};
#endif
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return process_==::getpid();
#else
            return true;
#endif
        }
        bool owned(owner value)const noexcept{return configured_&&value==owner_;}
        opened* opened_for(handle id)noexcept{for(auto& row:handles_)if(id&&row.id==id)return &row;return nullptr;}
        record* request_for(handle id)noexcept{for(auto& row:records_)if(id&&row.result.request==id)return &row;return nullptr;}
        record* work_for(handle ticket)noexcept{for(auto& row:records_)if(ticket&&row.ticket==ticket&&row.worker)return &row;return nullptr;}
        static bool terminal(record const& row)noexcept
        {return row.result.status==state::succeeded||row.result.status==state::failed||row.result.status==state::cancelled;}
        void remember(record const& row)noexcept
        {auto open=opened_for(row.device);if(open&&open->last_key==row.key)open->last_receipt=row.result;}
        void cancel_unstarted(reason why)noexcept
        {for(auto& row:records_)if(row.result.request&&!row.crossed&&!row.cleanup){row.result.status=state::cancelled;row.result.outcome=effect::none;row.result.why=why;row.cleanup=!row.worker;remember(row);}}
        void release_closed(opened& open)noexcept
        {
            if(!open.closing)return;
            for(auto const& row:records_)if(row.result.request&&row.device==open.id)return;
            if(open.stop_receipt.request&&!open.stop_released)return;
            open={};
        }
        bool current(observations const& value,std::uint64_t now)const noexcept
        {
            if(value.count<policy_.minimum_observations||value.count>policy_.observations)return false;
            for(std::size_t i{};i<value.count;++i)
            {
                auto const& witness=value.values[i];
                if(!witness.source||!witness.source_generation||!witness.capture||!witness.authority_epoch||!witness.buffer||
                   !witness.captured_ns||witness.captured_ns>now||now-witness.captured_ns>policy_.maximum_age_ns||witness.frame!=policy_.schema.frame||
                   value.pins[i].identity()!=witness.buffer)return false;
                bool matches{};
                auto status=value.pins[i].visit([&](auto,auto const&,auto const& sha)noexcept{matches=sha==witness.digest;return matches;});
                if(status!=error::success||!matches)return false;
                for(std::size_t n{};n<i;++n)if(value.values[n].source==witness.source)return false;
            }
            return true;
        }
        static bool write_profile(wire::writer& out,buffers::profile const& value)noexcept
        {return out.text(value.identity.view())&&out.blob(value.sha256);}
        static buffers::digest fingerprint(submission const& value,bytes copied,observations const& evidence)noexcept
        {
            std::array<std::uint8_t,16384> storage;wire::writer out{storage};auto const& lease=value.controller;
            out.u64(value.device);out.u64(value.key);out.u64(value.deadline);out.u64(lease.id);out.u64(lease.epoch);out.u64(lease.provider_generation);
            out.u64(lease.owner.environment);out.u64(lease.owner.grant);out.u64(lease.owner.generation);out.u64(lease.owner.authority_epoch);
            out.u64(lease.priority);out.u64(lease.expires);write_profile(out,value.schema);out.blob(copied);out.u64(evidence.count);
            for(std::size_t i{};i<evidence.count;++i)
            {
                auto const& source=evidence.values[i];out.u64(source.source);out.u64(source.source_generation);out.u64(source.capture);out.u64(source.authority_epoch);
                out.u64(source.buffer);out.u64(source.captured_ns);out.blob(source.digest);out.text(source.frame.view());
            }
            // 8 KiB body + eight bounded witnesses + fixed lease/profile < 16 KiB.
            return buffers::hash(out.written());
        }
        void start_stop()noexcept
        {
            latched_=true;cancel_unstarted(reason::stopped);
            if(!stop_.request&&reserved_stop_){stop_={std::exchange(reserved_stop_,0),state::queued,effect::locally_accepted,provider_,reason::none};stop_ack_=false;stopped_witness_=0;}
        }
    public:
        core()=default;core(core const&)=delete;core&operator=(core const&)=delete;
        /** @brief Fixed storage includes all maximum-sized records even with lower limits. */
        static bool required_storage(plan const& value,std::uint64_t& bytes)noexcept
        {
            if(!valid_text(value.profile.identity)||!valid_text(value.stop_profile.identity)||!valid(value.schema)||value.handles>maximum_handles||
               value.requests>maximum_requests||value.body_bytes>maximum_command_bytes||value.observations>maximum_observations||
               value.minimum_observations>value.observations||!value.minimum_observations||!value.maximum_age_ns||value.maximum_age_ns>INT64_MAX||
               !value.maximum_deadline_ns||value.maximum_deadline_ns>INT64_MAX)return false;
            bytes=sizeof(core);return true;
        }
        error configure(owner principal,handle provider,plan const& value)noexcept
        {
            if(!same_process())return error::denied;
            std::uint64_t required{};if(!principal||!provider||!required_storage(value,required))return error::invalid;
            if(value.storage_bytes<required)return error::too_large;
            std::lock_guard lock{mutex_};if(configured_||retired_)return error::denied;
            auto stop=resources::allocate_identity();if(!stop)return error::too_large;
            owner_=principal;provider_=provider;policy_=value;reserved_stop_=stop;configured_=true;return error::success;
        }
        /** @brief Reserve before handle delivery; failed/reentrant delivery rolls back. */
        template<typename Sink>error open(owner principal,buffers::profile const& profile,std::uint64_t grant_capacity,Sink&& sink)
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex_};if(!owned(principal)||retired_)return error::denied;
            if(profile!=policy_.profile)return error::denied;
            std::uint64_t used{};opened* selected{};for(auto& row:handles_)if(row.id)++used;else if(!selected)selected=&row;
            if(!selected||used>=policy_.handles||used>=grant_capacity)return error::too_large;
            auto id=resources::allocate_identity();if(!id)return error::too_large;selected->id=id;
            lock.unlock();bool copied{};try{copied=sink(id);}catch(...){copied=false;}lock.lock();
            if(!copied||retired_||selected->closing){*selected={};return !copied?error::io:error::cancelled;}
            selected->published=true;return error::success;
        }
        /** @brief Commit one complete inert command; exact last replay does not re-execute.
         * @details Historical replay is checked before freshness/expiry and never
         * consumes new pins. The next successful key acknowledges the previous
         * replay receipt. Rejected submissions do not advance that key. The caller
         * owns the complete copied carrier and holds current grouped authority.
         */
        error submit(owner principal,submission const& value,observations& evidence,std::uint64_t now,receipt& output)noexcept
        {
            if(!same_process())return error::denied;
            if(value.body.size()>maximum_command_bytes||evidence.count>maximum_observations)return error::too_large;
            if(!valid_text(value.schema.identity))return error::invalid;
            for(std::size_t i{};i<evidence.count;++i)if(!valid_text(evidence.values[i].frame))return error::invalid;
            std::array<std::uint8_t,maximum_command_bytes> copied;std::copy(value.body.begin(),value.body.end(),copied.begin());
            bytes body{copied.data(),value.body.size()};auto digest=fingerprint(value,body,evidence);
            std::lock_guard lock{mutex_};if(!owned(principal))return error::denied;
            auto open=opened_for(value.device);if(!open||!open->published)return error::bad_handle;
            if(value.key&&value.key==open->last_key)
            {if(digest!=open->last_digest)return error::conflict;output=open->last_receipt;return error::success;}
            if(retired_||open->closing)return error::denied;
            if(latched_)return error::cancelled;
            if(!value.key||open->last_key==UINT64_MAX||value.key!=open->last_key+1)return error::conflict;
            if(value.schema!=policy_.schema.profile||value.body.size()>policy_.body_bytes)return error::denied;
            if(!now||now>INT64_MAX||value.deadline<=now||value.deadline-now>policy_.maximum_deadline_ns||value.deadline>INT64_MAX)return error::timeout;
            auto const& lease=value.controller;
            if(!lease.id||!lease.epoch||!lease.owner||lease.owner.environment!=owner_.environment||lease.owner.generation!=owner_.generation||
                lease.provider_generation!=provider_||lease.expires<value.deadline)return error::denied;
            scalar_command command;auto decoded=decode(body,policy_.schema,command);if(decoded!=error::success)return decoded;
            if(!current(evidence,now))return error::denied;
            std::uint64_t used{};record* selected{};for(auto& row:records_)if(row.result.request)++used;else if(!selected)selected=&row;
            if(!selected||used>=policy_.requests)return error::again;
            auto id=resources::allocate_identity();if(!id)return error::too_large;
            selected->device=value.device;selected->key=value.key;selected->deadline=value.deadline;selected->controller=lease;selected->command=command;
            selected->evidence=std::move(evidence);evidence.count=0;selected->result={id,state::queued,effect::locally_accepted,provider_,reason::none};
            open->last_key=value.key;open->last_digest=digest;open->last_receipt=selected->result;open->last_released=false;output=selected->result;return error::success;
        }
        /** @brief Select one queued command without claiming that any effect occurred. */
        error claim(std::uint64_t now,work& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!configured_||retired_)return error::denied;if(latched_)return error::cancelled;
            for(auto const& row:records_)if(row.worker||row.result.status==state::outcome_unknown)return error::again;
            record* next{};for(auto& row:records_)if(row.result.request&&row.result.status==state::queued&&(!next||row.result.request<next->result.request))next=&row;
            if(!next)return error::again;
            if(!now||now>=next->deadline||!current(next->evidence,now))
            {next->result.status=state::cancelled;next->result.outcome=effect::none;next->result.why=(!now||now>=next->deadline)?reason::deadline:reason::source_unavailable;next->cleanup=true;remember(*next);return error::timeout;}
            auto ticket=resources::allocate_identity();if(!ticket)return error::too_large;next->ticket=ticket;next->worker=true;
            next->result.status=state::running;remember(*next);
            output={ticket,next->device,next->key,next->result,next->deadline,next->controller,next->command,next->evidence.values,next->evidence.count};return error::success;
        }
        /** @brief Final local boundary immediately before native I/O under held authority.
         * @details The provider first validates the entire actual source chain and
         * current controller arbitration. This call rechecks local cancellation,
         * boot, expiry and real pins. Subsequent failure is conservatively unknown
         * unless the provider can prove no effect or has a qualified remote ACK.
         */
        error cross_effect(handle ticket,std::uint64_t now)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto row=work_for(ticket);if(!row)return error::bad_handle;
            if(row->crossed)return error::conflict;
            if(retired_||latched_||row->result.status!=state::running)return error::cancelled;
            if(!now||now>=row->deadline||now>=row->controller.expires)return error::timeout;
            if(row->result.provider_generation!=provider_||!current(row->evidence,now))return error::denied;
            row->crossed=true;return error::success;
        }
        /** @brief Record real native completion; only qualified evidence may say executed. */
        error finish(handle ticket,completion outcome)noexcept
        {
            if(!same_process())return error::denied;
            if(static_cast<unsigned>(outcome)>2)return error::invalid;
            std::lock_guard lock{mutex_};auto row=work_for(ticket);if(!row)return error::bad_handle;
            if(outcome!=completion::no_effect&&!row->crossed)return error::conflict;
            if(row->crossed&&row->result.provider_generation!=provider_&&outcome!=completion::unknown)return error::conflict;
            row->worker=false;row->ticket=0;
            if(outcome==completion::unknown)
            {row->result.status=state::outcome_unknown;row->result.outcome=effect::unknown;row->result.why=reason::native_unknown;row->cleanup=false;start_stop();}
            else
            {
                row->result.status=outcome==completion::executed?state::succeeded:row->result.status==state::cancelled?state::cancelled:state::failed;
                row->result.outcome=outcome==completion::executed?effect::committed:effect::none;
                if(outcome==completion::no_effect&&row->result.why==reason::none)row->result.why=reason::native_rejected;
                row->cleanup=true;
            }
            remember(*row);return error::success;
        }
        /** @brief Historical result and explicit terminal release remain available after retire. */
        error result(owner principal,handle device,handle request,receipt& output)const noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!owned(principal))return error::denied;
            for(auto const& row:records_)if(row.result.request==request&&request&&row.device==device){output=row.result;return error::success;}
            for(auto const& open:handles_)if(open.id==device&&device)
            {if(!open.last_released&&open.last_receipt.request==request&&request){output=open.last_receipt;return error::success;}if(!open.stop_released&&open.stop_receipt.request==request&&request){output=open.stop_receipt;return error::success;}}
            return error::bad_handle;
        }
        /** @brief Observe retained ownership without advancing deadlines or releasing receipts. */
        bool retains(owner principal,handle device)const noexcept
        {
            if(!same_process())return false;
            std::lock_guard lock{mutex_};if(!owned(principal))return false;
            for(auto const& row:handles_)if(device&&row.id==device)return true;
            return false;
        }
        error release(owner principal,handle request)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!owned(principal))return error::denied;auto row=request_for(request);
            if(!row)
            {
                bool found{};
                for(auto const& open:handles_)if(request&&open.stop_receipt.request==request&&!open.stop_released)
                {if(open.stop_receipt.status!=state::succeeded)return error::again;found=true;}
                if(!found)return error::bad_handle;
                for(auto& open:handles_)if(open.stop_receipt.request==request){open.stop_released=true;release_closed(open);}
                return error::success;
            }
            if(row->worker||!terminal(*row)||!row->cleanup)return error::again;
            auto open=opened_for(row->device);if(open&&open->last_key==row->key)open->last_released=true;*row={};if(open)release_closed(*open);return error::success;
        }
        error close(owner principal,handle device)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!owned(principal))return error::denied;auto open=opened_for(device);if(!open||!open->published||open->closing)return error::bad_handle;
            open->closing=true;
            bool effect_pending{};for(auto& row:records_)if(row.result.request&&row.device==device&&!terminal(row))
            {if(row.crossed)effect_pending=true;else{row.result.status=state::cancelled;row.result.outcome=effect::none;row.result.why=reason::closed;row.cleanup=!row.worker;remember(row);}}
            if(effect_pending){start_stop();}
            release_closed(*open);return error::success;
        }
        /** @brief Independent stop reservation; it succeeds even when normal storage is full. */
        error stop(owner principal,handle device,handle key,buffers::profile const& profile,receipt& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!owned(principal)||profile!=policy_.stop_profile)return error::denied;
            auto open=opened_for(device);if(!open||!open->published)return error::bad_handle;
            if(key&&key==open->stop_key){output=open->stop_receipt;return error::success;}
            if(!key||open->stop_key==UINT64_MAX||key!=open->stop_key+1)return error::conflict;
            start_stop();if(!stop_.request)return error::conflict;open->stop_key=key;open->stop_receipt=stop_;open->stop_released=false;output=stop_;return error::success;
        }
        error pending_stop(receipt& output)const noexcept
        {if(!same_process())return error::denied;std::lock_guard lock{mutex_};if(!stop_.request)return error::again;output=stop_;return error::success;}
        /** @brief Trusted safety path uses the independent reserved stop without a guest key.
         * @details Used only by the owning deployment watchdog/control provider.
         * It grants no native write authority and does not assert physical stop.
         * It cannot be disabled by exhaustion of ordinary guest request slots.
         */
        error safety_stop(reason why,receipt& output)noexcept
        {
            if(!same_process())return error::denied;
            if(why!=reason::deadline&&why!=reason::closed&&why!=reason::retired&&why!=reason::stopped)return error::invalid;
            std::lock_guard lock{mutex_};if(!configured_)return error::denied;
            start_stop();if(!stop_.request)return error::conflict;
            if(stop_.why==reason::none)stop_.why=why;
            output=stop_;return error::success;
        }
        /** @brief Observe the real open device profile and next key without submitting work. */
        error observe_submission(owner principal,handle device,plan& policy,handle& provider,handle& next_key)const noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!owned(principal)||retired_)return error::denied;if(latched_)return error::cancelled;
            for(auto const& open:handles_)if(open.id==device&&device&&open.published&&!open.closing)
            {
                if(open.last_key==UINT64_MAX)return error::too_large;
                policy=policy_;provider=provider_;next_key=open.last_key+1;return error::success;
            }
            return error::bad_handle;
        }
        /** @brief ACK means the fixed stop protocol was acknowledged, not a stopped witness. */
        error acknowledge_stop(handle provider,handle request)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(provider!=provider_||!request||request!=stop_.request)return error::conflict;
            stop_ack_=true;stop_.status=state::succeeded;stop_.outcome=effect::observed_remote;
            for(auto& open:handles_)if(open.stop_receipt.request==request)open.stop_receipt=stop_;
            return error::success;
        }
        /** @brief Retain unresolved stop delivery without claiming cancellation or stopped state.
         * @details The owning typed executor invokes this on a missing, invalid
         * or rejected ACK. Only explicit trusted reconciliation can subsequently
         * establish the remote state; normal command slots remain latched.
         */
        error stop_failed(handle provider,handle request)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(provider!=provider_||!request||request!=stop_.request||stop_ack_)return error::conflict;
            stop_.status=state::outcome_unknown;stop_.outcome=effect::unknown;stop_.why=reason::native_unknown;
            for(auto& open:handles_)if(open.stop_receipt.request==request)open.stop_receipt=stop_;
            return error::success;
        }
        error observe_stopped(handle provider,handle request,handle trusted_witness)noexcept
        {if(!same_process())return error::denied;std::lock_guard lock{mutex_};if(provider!=provider_||!request||request!=stop_.request||!trusted_witness)return error::conflict;stopped_witness_=trusted_witness;return error::success;}
        /** @brief Trusted reconciliation frees unknown debt, retaining the stop latch. */
        error reconcile(handle provider,handle request,handle trusted_evidence,completion outcome)noexcept
        {
            if(!same_process())return error::denied;
            if(outcome!=completion::no_effect&&outcome!=completion::executed)return error::invalid;
            std::lock_guard lock{mutex_};auto row=request_for(request);
            if(provider!=provider_||!row||row->result.status!=state::outcome_unknown||row->worker||!trusted_evidence||trusted_evidence<=last_reconciliation_)return error::conflict;
            last_reconciliation_=trusted_evidence;row->cleanup=true;row->result.status=outcome==completion::executed?state::succeeded:state::failed;
            row->result.outcome=outcome==completion::executed?effect::committed:effect::none;row->result.why=reason::none;remember(*row);return error::success;
        }
        /** @brief Host-only rearm; no guest open/lifecycle action can invoke this implicitly. */
        error rearm(handle provider,handle stopped_request,handle trusted_witness)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(retired_||provider!=provider_||!stop_ack_||!stopped_request||stopped_request!=stop_.request||
                !trusted_witness||trusted_witness!=stopped_witness_)return error::denied;
            for(auto const& row:records_)if(row.worker||row.result.status==state::outcome_unknown)return error::again;
            auto next=resources::allocate_identity();if(!next)return error::too_large;
            reserved_stop_=next;stop_={};stop_ack_=latched_=false;stopped_witness_=0;return error::success;
        }
        error provider_changed(handle next)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!configured_||!next||next<=provider_)return error::conflict;
            provider_=next;start_stop();stop_.provider_generation=next;stop_.status=state::queued;stop_.outcome=effect::locally_accepted;stop_ack_=false;stopped_witness_=0;
            for(auto& row:records_)if(row.result.request&&row.crossed&&!terminal(row))
            {row.result.status=state::outcome_unknown;row.result.outcome=effect::unknown;row.result.why=reason::provider_changed;row.cleanup=false;remember(row);}
            return error::success;
        }
        error retire()noexcept
        {if(!same_process())return error::denied;std::lock_guard lock{mutex_};if(!configured_)return error::denied;retired_=true;start_stop();return error::success;}
        error inspect(owner principal,usage& output)const noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!owned(principal))return error::denied;usage value;value.reserved_bytes=sizeof(core);value.latched=latched_;value.retired=retired_;
            value.stop_pending=bool(stop_.request);value.stop_acknowledged=stop_ack_;value.stopped_observed=bool(stopped_witness_);
            value.stop_reservation=stop_.request?stop_.request:reserved_stop_;
            for(auto const& row:handles_)if(row.id){++value.retained_handles;if(row.published&&!row.closing)++value.handles;}
            for(auto const& row:records_)if(row.result.request){++value.requests;value.pins+=row.evidence.count;if(row.result.status==state::queued)++value.queued;if(row.worker)++value.running;if(row.result.status==state::outcome_unknown)++value.unknown;}
            output=value;return error::success;
        }
    };
}
