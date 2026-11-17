/** @file
 * @brief Portable guard, clock, timer and wait resources for robot scenes.
 * @details This registry is a resource-lifetime mechanism, not an authorization
 * boundary. Every grant string and capacity comes from a separately admitted
 * scene capability. Callers hold authority leases across effects, release them
 * before wait(), and revalidate before exposing its result. A numeric resource
 * reference alone confers no authority. No native callback executes guest code.
 *
 * Lock order is authority -> registry -> checked, nonblocking output sink.
 * Cancellation predicates run outside the registry lock. Output sinks must not
 * enter this registry, acquire another authority lease, block or retain a
 * reference to their sample. A false/throwing sink does not consume readiness.
 * The registry must outlive all calls and waiters; trusted teardown calls
 * retire(), joins waiters, then destroys its owning environment.
 *
 * All functions report semantic errors without modifying output parameters,
 * except wait()'s required count on success/too_large. Allocation exceptions
 * propagate to the host boundary, which converts them to a bounded error.
 * APL-2.0.
 */
#pragma once
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <uwvm2/utils/container/function_ref.h>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace uwvm2::imported::ros::resources
{
    using handle = std::uint64_t;
    /** @brief Allocate a process-unique resource identity across scene kinds.
     * @details Identities never wrap or recycle and convey no authority.
     * Artifact and scheduling registries independently check kind and owner.
     */
    inline std::atomic<handle> next_identity{1};
    inline handle allocate_identity() noexcept
    {
        auto value=next_identity.load(std::memory_order_relaxed);
        while(value&&value!=UINT64_MAX)
            if(next_identity.compare_exchange_weak(value,value+1,std::memory_order_relaxed))return value;
        return 0;
    }
    using nanoseconds = std::int64_t;
    /// @brief Portable result codes; failures never imply a successful effect.
    enum class error : std::uint32_t
    { success=0, io=5, bad_handle=9, again=11, no_memory=12, denied=13, conflict=17, invalid=22, too_large=90, unsupported=95, timeout=110, cancelled=125 };
    /// @brief Registry kind checked independently of capability purpose.
    enum class kind : std::uint8_t { guard, clock, timer, wait_set };
    /// @brief Steady/system host clocks or an explicitly updated ROS clock.
    enum class clock_kind : std::uint8_t { steady, system, ros };
    inline constexpr std::uint64_t readable=1, changed=2, expired=4, closed=8;
    inline constexpr std::uint64_t interests=readable|changed|expired|closed;
    /** @brief Nonconsuming readiness observation.
     * @details token is opaque guest correlation. sequence is a guard trigger
     * sequence, ROS-clock update sequence or timer schedule-reset sequence; it
     * is not an authorization token. Another consumer may change readiness
     * before a later take. Clock changes need explicit acknowledgement.
     */
    struct ready_record { handle source{};std::uint64_t token{},events{},sequence{}; };
    /// @brief A sampled clock; now is usable only when started is true.
    struct clock_sample { nanoseconds now{};std::uint64_t epoch{};bool started{};clock_kind source{}; };
    /// @brief One timer delivery with coalesced missed periods and clock epoch.
    struct timer_sample { nanoseconds expected{},actual{};std::uint64_t occurrences{},clock_epoch{}; };
    /// @brief Detached metadata; inspecting it neither consumes nor arms a source.
    struct snapshot { handle id{};kind type{};bool retired{},armed{};std::uint64_t sequence{},members{};::uwvm2::utils::container::std_string grant; };

    /// @brief Checked signed nanosecond addition; output is unchanged on overflow.
    inline bool add(nanoseconds left,nanoseconds right,nanoseconds& result) noexcept
    {
        if((right>0&&left>INT64_MAX-right)||(right<0&&left<INT64_MIN-right))return false;
        result=left+right;return true;
    }
    inline nanoseconds steady_now() noexcept
    { return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(); }
    inline nanoseconds system_now() noexcept
    { return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); }

    /** @brief One registry owned by one explicitly selected policy environment.
     * @details Handles are process-wide monotonic identities, never recycled or
     * shared with another registry. Capacities count all registered resources
     * for the exact grant, including timers retired by parent-clock closure
     * until explicitly closed. Wait memberships separately retain source
     * tombstones and consume their own member capacity until removed/closed.
     * Missing limits are resolved by policy, not guessed here; zero permits
     * no creation or membership. Revocation is a trusted lifetime action; the
     * authority layer separately prevents reopening a revoked grant.
     */
    class registry
    {
        struct object;
        struct member { std::shared_ptr<object> source;std::uint64_t token{},mask{},seen{}; };
        struct object
        {
            handle id{};kind type{};::uwvm2::utils::container::std_string grant;bool retired{},armed{},started{};
            std::uint64_t sequence{},taken{},epoch{};
            clock_kind clock_source{};nanoseconds value{},due{},period{};
            std::shared_ptr<object> clock;
            ::uwvm2::utils::container::std_map<handle,member> members;
        };
        mutable std::mutex mutex;
        std::condition_variable condition;
        ::uwvm2::utils::container::std_map<handle,std::shared_ptr<object>> objects;
        bool retired{};

        static handle allocate() noexcept
        {
            return allocate_identity();
        }
        void notify() noexcept { condition.notify_all(); }
        std::shared_ptr<object> find(handle id,kind type,std::string_view grant) const
        {
            auto found=objects.find(id);
            if(retired||found==objects.end()||found->second->retired||found->second->type!=type||found->second->grant!=grant)return {};
            return found->second;
        }
        error create(kind type,std::string_view grant,std::uint64_t limit,std::shared_ptr<object>& output)
        {
            if(retired||grant.empty())return error::denied;
            std::uint64_t count{};
            for(auto const& [id,value]:objects)if(value->grant==grant)++count;
            if(count>=limit)return error::again;
            auto value=::uwvm2::utils::container::make_shared<object>();value->id=allocate();
            if(!value->id)return error::too_large;
            value->grant=grant;value->type=type;
            objects.emplace(value->id,value);output=std::move(value);notify();return error::success;
        }
        static clock_sample sample(object const& clock) noexcept
        {
            return {clock.clock_source==clock_kind::steady?steady_now():clock.clock_source==clock_kind::system?system_now():clock.value,
                    clock.epoch,clock.started,clock.clock_source};
        }
        static std::chrono::steady_clock::time_point step(
            std::chrono::steady_clock::time_point now,std::chrono::nanoseconds delay) noexcept
        {
            auto delta=std::chrono::duration_cast<std::chrono::steady_clock::duration>(delay);
            auto maximum=std::chrono::steady_clock::time_point::max();
            return now>maximum-delta?maximum:now+delta;
        }
        static std::uint64_t events(member const& entry) noexcept
        {
            auto const& value=*entry.source;
            if(value.retired)return entry.mask&closed;
            std::uint64_t result{};
            if(value.type==kind::guard&&value.sequence!=value.taken)result|=readable;
            if(value.type==kind::clock&&value.sequence!=entry.seen)result|=changed;
            if(value.type==kind::timer&&value.armed&&value.clock&&!value.clock->retired)
            { auto current=sample(*value.clock);if(current.started&&current.now>=value.due)result|=expired; }
            return result&entry.mask;
        }
        void close_object(std::shared_ptr<object> const& value) noexcept
        {
            value->retired=true;value->armed=false;value->members.clear();
            if(value->type==kind::clock)
                for(auto const& [id,child]:objects)
                    if(child->clock==value){child->retired=true;child->armed=false;}
            notify();
        }
    public:
        registry()=default;
        registry(registry const&)=delete;
        registry& operator=(registry const&)=delete;

        /** @brief Create an initially untriggered guard within an admitted limit.
         * @details No trigger is inferred from creation. A failed output copy
         * at the host boundary must close the unpublished handle.
         */
        error guard_create(std::string_view grant,std::uint64_t limit,handle& result)
        {
            std::lock_guard lock{mutex};std::shared_ptr<object> value;
            auto status=create(kind::guard,grant,limit,value);if(status==error::success)result=value->id;return status;
        }
        /// @brief Increment one owned guard and wake waiters; counter overflow refuses the trigger.
        error guard_trigger(std::string_view grant,handle id)
        {
            std::lock_guard lock{mutex};auto value=find(id,kind::guard,grant);if(!value)return error::bad_handle;
            if(value->sequence==UINT64_MAX)return error::too_large;
            ++value->sequence;notify();return error::success;
        }
        /** @brief Deliver accumulated triggers, committing consumption after output succeeds.
         * @details The trusted sink runs once under the registry lock. False
         * returns invalid and preserves all triggers. A thrown exception also
         * preserves triggers. The sink obeys the file-level lock contract.
         */
        template<typename Sink> error guard_deliver(std::string_view grant,handle id,Sink&& sink)
        {
            std::lock_guard lock{mutex};auto value=find(id,kind::guard,grant);if(!value)return error::bad_handle;
            if(value->sequence==value->taken)return error::again;
            if(!sink(value->sequence-value->taken))return error::invalid;
            value->taken=value->sequence;return error::success;
        }
        /// @brief Consume guard triggers into trusted host storage; use guard_deliver for guest output.
        error guard_take(std::string_view grant,handle id,std::uint64_t& count)
        {return guard_deliver(grant,id,[&](auto value){count=value;return true;});}
        /** @brief Create an owned clock of one independently authorized source kind.
         * @details ROS clocks start unstarted. Steady/system clocks reflect the
         * host without granting host-clock mutation. System-clock adjustments
         * are observed in samples; only explicit ROS updates produce changed
         * events. Security timeouts and quota windows must use steady time.
         */
        error clock_create(std::string_view grant,std::uint64_t limit,clock_kind source,handle& result)
        {
            if(source!=clock_kind::steady&&source!=clock_kind::system&&source!=clock_kind::ros)return error::invalid;
            std::lock_guard lock{mutex};std::shared_ptr<object> value;
            auto status=create(kind::clock,grant,limit,value);
            if(status==error::success){value->clock_source=source;value->started=source!=clock_kind::ros;result=value->id;}
            return status;
        }
        /// @brief Sample an owned clock without acknowledging its update sequence.
        error clock_read(std::string_view grant,handle id,clock_sample& result) const
        {
            std::lock_guard lock{mutex};auto value=find(id,kind::clock,grant);if(!value)return error::bad_handle;
            result=sample(*value);return error::success;
        }
        /** @brief Update an explicitly authorized ROS clock, never the host clock.
         * @details The host checks independent clock.update authority before
         * entering. Source activation/deactivation or a backward running-time
         * jump increases the epoch and disarms every dependent timer. Callers
         * needing jump notification must observe the clock's changed event;
         * timer reset is explicit and no catch-up burst is manufactured.
         */
        error clock_update(std::string_view grant,handle id,nanoseconds now,bool started)
        {
            std::lock_guard lock{mutex};auto value=find(id,kind::clock,grant);if(!value)return error::bad_handle;
            if(value->clock_source!=clock_kind::ros)return error::denied;
            if(value->sequence==UINT64_MAX||value->epoch==UINT64_MAX)return error::too_large;
            bool jump=value->started!=started||(started&&value->started&&now<value->value);
            value->value=now;value->started=started;++value->sequence;
            if(jump)
            {
                ++value->epoch;
                // Backward jumps and source activation invalidate timer
                // schedules; explicit reset is required, no catch-up storm.
                for(auto const& [child_id,child]:objects)if(child->clock==value)child->armed=false;
            }
            notify();return error::success;
        }
        /** @brief Create an unarmed timer bound to an independently authorized clock.
         * @details The caller admits both grants and supplies their exact
         * aliases. Closing/revoking the clock permanently retires its timers.
         */
        error timer_create(std::string_view grant,std::uint64_t limit,std::string_view clock_grant,handle clock_id,handle& result)
        {
            std::lock_guard lock{mutex};auto clock=find(clock_id,kind::clock,clock_grant);if(!clock)return error::bad_handle;
            std::shared_ptr<object> value;auto status=create(kind::timer,grant,limit,value);
            if(status==error::success){value->clock=std::move(clock);result=value->id;}
            return status;
        }
        /** @brief Arm/reset an owned timer with an absolute due time in its clock domain.
         * @details period zero is one-shot; a positive period coalesces missed
         * occurrences on delivery. Negative periods and schedule sequence
         * overflow are refused before mutation. An unstarted clock does not
         * make the timer ready until a later explicit update/reset permits it.
         */
        error timer_reset(std::string_view grant,handle id,nanoseconds due,nanoseconds period)
        {
            if(period<0)return error::invalid;
            std::lock_guard lock{mutex};auto value=find(id,kind::timer,grant);if(!value)return error::bad_handle;
            if(!value->clock||value->clock->retired)return error::cancelled;
            if(value->sequence==UINT64_MAX)return error::too_large;
            value->due=due;value->period=period;value->armed=true;++value->sequence;notify();return error::success;
        }
        /// @brief Disarm one timer without destroying its identity or granting any clock access.
        error timer_cancel(std::string_view grant,handle id)
        {
            std::lock_guard lock{mutex};auto value=find(id,kind::timer,grant);if(!value)return error::bad_handle;
            value->armed=false;notify();return error::success;
        }
        /** @brief Deliver one coalesced timer occurrence transactionally.
         * @details Both missed-count and following-deadline arithmetic are
         * checked before output. Sink failure/exception preserves the pending
         * schedule. Successful output advances exactly once or disarms a
         * one-shot timer. No callback executes for each missed interval.
         */
        template<typename Sink> error timer_deliver(std::string_view grant,handle id,Sink&& sink)
        {
            std::lock_guard lock{mutex};auto value=find(id,kind::timer,grant);if(!value)return error::bad_handle;
            auto now=sample(*value->clock);
            if(!value->armed||!now.started||now.now<value->due)return error::again;
            auto count=std::uint64_t{1};
            nanoseconds following{};
            if(value->period)
            {
                auto difference=static_cast<std::uint64_t>(now.now)-static_cast<std::uint64_t>(value->due);
                auto missed=difference/static_cast<std::uint64_t>(value->period);
                if(missed==UINT64_MAX)return error::too_large;
                count=missed+1;
            }
            timer_sample delivered{value->due,now.now,count,now.epoch};
            if(value->period)
            {
                auto difference=static_cast<std::uint64_t>(now.now)-static_cast<std::uint64_t>(value->due);
                auto remaining=value->period-static_cast<nanoseconds>(difference%static_cast<std::uint64_t>(value->period));
                if(!add(now.now,remaining,following))return error::too_large;
            }
            if(!sink(delivered))return error::invalid;
            if(value->period)value->due=following;
            else value->armed=false;
            notify();return error::success;
        }
        /// @brief Consume one timer delivery into trusted storage; use timer_deliver for guest output.
        error timer_take(std::string_view grant,handle id,timer_sample& result)
        {return timer_deliver(grant,id,[&](auto value){result=value;return true;});}
        /// @brief Create an empty owned wait set; creation grants no source observation rights.
        error wait_set_create(std::string_view grant,std::uint64_t limit,handle& result)
        {
            std::lock_guard lock{mutex};std::shared_ptr<object> value;
            auto status=create(kind::wait_set,grant,limit,value);if(status==error::success)result=value->id;return status;
        }
        /** @brief Attach an independently admitted source with exact interests.
         * @details The host checks source observation authority and ownership.
         * Wrong source kind/owner, empty/unknown masks, duplicate attachment
         * and full membership allowance fail without mutation. Wait-set nesting
         * is not supported, preventing source cycles. Clock observation starts
         * at its current sequence; earlier updates are not replayed.
         */
        error wait_set_add(std::string_view grant,handle id,std::string_view source_grant,handle source,
                           std::uint64_t mask,std::uint64_t token,std::uint64_t member_limit)
        {
            if(!mask||(mask&~interests))return error::invalid;
            std::lock_guard lock{mutex};auto set=find(id,kind::wait_set,grant);if(!set)return error::bad_handle;
            auto found=objects.find(source);
            if(found==objects.end()||found->second->grant!=source_grant||found->second->retired)return error::bad_handle;
            auto value=found->second;
            auto permitted=value->type==kind::guard?readable|closed:value->type==kind::timer?expired|closed:value->type==kind::clock?changed|closed:0;
            if(mask&~permitted)return error::invalid;
            if(set->members.contains(source))return error::conflict;
            if(set->members.size()>=member_limit)return error::again;
            set->members.emplace(source,member{value,token,mask,value->sequence});notify();return error::success;
        }
        /// @brief Detach a source and release its tombstone membership without consuming the source.
        error wait_set_remove(std::string_view grant,handle id,handle source)
        {
            std::lock_guard lock{mutex};auto set=find(id,kind::wait_set,grant);if(!set)return error::bad_handle;
            if(!set->members.erase(source))return error::bad_handle;
            notify();return error::success;
        }
        /** @brief Acknowledge clock updates through an existing observed sequence.
         * @details An acknowledgement cannot move backward or exceed the
         * source's published sequence. It affects this membership only.
         * Guard/timer readiness is consumed through their own delivery APIs.
         */
        error wait_set_acknowledge(std::string_view grant,handle id,handle source,std::uint64_t sequence)
        {
            std::lock_guard lock{mutex};auto set=find(id,kind::wait_set,grant);if(!set)return error::bad_handle;
            auto entry=set->members.find(source);if(entry==set->members.end())return error::bad_handle;
            if(entry->second.source->type!=kind::clock||sequence<entry->second.seen||sequence>entry->second.source->sequence)return error::invalid;
            entry->second.seen=sequence;return error::success;
        }
        /** @brief Wait for nonconsuming readiness with a monotonic absolute deadline.
         * @details Registration, scans and sleeping share one mutex, avoiding
         * lost notifications. Output never exceeds capacity. too_large reports
         * the needed count without changing result or source readiness.
         * Closing the set/environment or cancellation wakes/refuses the call.
         * Cancellation is checked outside the lock on 10 ms wake intervals;
         * scheduler latency is not bounded. It is advisory until the host
         * reacquires authority before final delivery.
         * Deadlines are checked after readiness, allowing zero-duration polls.
         */
        error wait(std::string_view grant,handle id,std::chrono::steady_clock::time_point deadline,
                   std::size_t capacity,::uwvm2::utils::container::std_vector<ready_record>& result,std::size_t& required,
                   ::uwvm2::utils::container::function_ref<bool()> const& cancelled={})
        {
            std::unique_lock lock{mutex};auto set=find(id,kind::wait_set,grant);if(!set)return error::bad_handle;
            while(true)
            {
                // Admission callbacks run outside this registry lock. They may
                // acquire the capability registry and trigger trusted retirement.
                bool stop{};
                if(cancelled){lock.unlock();stop=cancelled();lock.lock();}
                if(stop||retired||set->retired)return error::cancelled;
                std::size_t needed{};
                ::uwvm2::utils::container::std_vector<ready_record> output;
                for(auto const& [source,entry]:set->members)
                {
                    if(auto mask=events(entry))
                    {
                        ++needed;
                        // Time may advance while scanning a system/steady
                        // source. A single scan bounds both result size and
                        // allocation before publishing any observation.
                        if(needed==1&&capacity)output.reserve(std::min(capacity,set->members.size()));
                        if(needed<=capacity)output.push_back({source,entry.token,mask,entry.source->sequence});
                    }
                }
                if(needed>capacity){required=needed;return error::too_large;}
                if(needed){required=needed;result=std::move(output);return error::success;}
                auto now=std::chrono::steady_clock::now();if(now>=deadline)return error::timeout;
                auto until=deadline;
                // The bounded wake checks trusted cancellation even when the
                // management source is outside this portable registry.
                if(cancelled)until=std::min(until,step(now,std::chrono::milliseconds(10)));
                for(auto const& [source,entry]:set->members)
                {
                    auto const& value=*entry.source;
                    if(value.retired||value.type!=kind::timer||!value.armed||!(entry.mask&expired))continue;
                    auto clock=sample(*value.clock);
                    if(!clock.started||clock.source==clock_kind::ros)continue;
                    if(clock.now>=value.due){until=now;break;}
                    auto difference=static_cast<std::uint64_t>(value.due)-static_cast<std::uint64_t>(clock.now);
                    // Bounded stepping handles system clock jumps without
                    // overflowing chrono time_point arithmetic.
                    auto delta=std::chrono::nanoseconds(std::min<std::uint64_t>(difference,10000000));
                    until=std::min(until,step(now,delta));
                }
                condition.wait_until(lock,until);
            }
        }
        /** @brief Retire an owned resource and release its registry capacity.
         * @details Wait memberships may retain a metadata tombstone and report
         * closed. A closed clock retires dependent timers, whose registry
         * capacity remains charged until each timer is closed. Retired timer
         * handles can be closed but never rearmed or delivered.
         */
        error close(std::string_view grant,handle id)
        {
            std::lock_guard lock{mutex};auto found=objects.find(id);
            if(retired||found==objects.end()||found->second->grant!=grant)return error::bad_handle;
            close_object(found->second);objects.erase(found);return error::success;
        }
        /** @brief Trusted retirement of every resource created under one grant.
         * @details This does not itself revoke authority; the authority layer
         * must prevent future admission. Two bounded scans retire matching
         * objects and child timers, avoiding quadratic guard-group teardown.
         */
        void revoke(std::string_view grant) noexcept
        {
            std::lock_guard lock{mutex};
            for(auto const& [id,value]:objects)
                if(value->grant==grant){value->retired=true;value->armed=false;value->members.clear();}
            for(auto const& [id,value]:objects)
                if(value->clock&&value->clock->retired){value->retired=true;value->armed=false;}
            std::erase_if(objects,[&](auto const& entry){return entry.second->grant==grant;});notify();
        }
        /// @brief Permanently retire the environment and wake all current waiters.
        void retire() noexcept
        {
            std::lock_guard lock{mutex};retired=true;
            for(auto const& [id,value]:objects){value->retired=true;value->armed=false;value->members.clear();}
            objects.clear();notify();
        }
        /** @brief Copy bounded metadata without waiting for a busy registry.
         * @details Does not sample clocks, trigger sources, refresh limits,
         * acknowledge observations or expose native pointers. Busy/oversized
         * snapshots leave output unchanged; allocation failure propagates.
         */
        bool inspect(::uwvm2::utils::container::std_vector<snapshot>& output,std::size_t limit) const
        {
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock()||objects.size()>limit)return false;
            ::uwvm2::utils::container::std_vector<snapshot> next;next.reserve(objects.size());
            for(auto const& [id,value]:objects)next.push_back({id,value->type,value->retired,value->armed,value->sequence,value->members.size(),value->grant});
            output=std::move(next);return true;
        }
    };
}
