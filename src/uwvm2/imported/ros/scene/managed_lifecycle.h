/** @file
 * @brief Bounded local managed-node lifecycle with deferred guest callbacks.
 * @details This provider implements the ROS lifecycle primary/intermediate
 * state graph in process; it neither creates a native ROS node nor controls
 * a process, publisher or actuator. Exact capability admission is the broker's
 * duty. A state number or numeric handle never confers device authority.
 *
 * Trusted deployment binds each alias once with finite handle/deadline limits,
 * a transition allow-mask and client/server roles. An empty registry and zero
 * fields allow nothing. Fixed capacity is independent of optional broker rate
 * attenuation. Open/close never reset state, generation or request history.
 * There is one pending callback, one last submission and one last response
 * per alias. No native callback reenters guest code and no queue grows.
 *
 * Lock order is authority then registry. take() reserves one callback under
 * the registry lock, releases it before the delivery sink, then revalidates
 * ownership, request, generation and deadline before consuming. The sink may
 * reenter but must not retain the record. Failed/throwing delivery releases
 * its reservation without consuming. Mutations return fixed receipts;
 * exact retry recovers delivery failure without repeating the state effect.
 * Deadline expiry is checked on provider calls and finalizes pending work;
 * revocation cancels pending work and denies access, while idle state remains
 * cached for diagnostics. Neither stops the physical robot. A separately
 * enforced device watchdog/safe-stop is still required; no background timer or
 * deadline-time hardware action is implied by this local state machine.
 */
#pragma once
#include <uwvm2/utils/container/standard.h>
#include "authority.h"
#include <array>
#include <limits>
#if defined(__linux__)
#include <unistd.h>
#endif

namespace uwvm2::imported::ros::scene::managed_lifecycle
{
    /// @brief Public ROS lifecycle transition and state identifiers are retained.
    enum class state : std::uint64_t
    {unconfigured=1,inactive=2,active=3,finalized=4,configuring=10,cleaning_up=11,shutting_down=12,activating=13,deactivating=14,error_processing=15};
    enum class outcome : std::uint64_t {success=97,failure=98,error=99};
    inline constexpr std::uint64_t client=1,server=2,all_transitions=0xfe;
    /** @brief Explicit finite capacity and disclosure scope of one local node.
     * @details transitions uses bit N for public transition N in [1,7]. Roles
     * are client=1/server=2. max_deadline_ns bounds each absolute steady deadline
     * relative to admission. Zero handles/deadline/masks provide no allowance.
     */
    struct plan
    {std::uint64_t max_handles{},max_deadline_ns{},transitions_mask{},roles_mask{};};
    inline bool valid(plan const& limits) noexcept
    {return limits.max_handles!=UINT64_MAX&&limits.max_deadline_ns<=INT64_MAX&&
        !(limits.transitions_mask&~all_transitions)&&!(limits.roles_mask&~(client|server));}
    /** @brief Current logical state; status is the last completion result code.
     * @details generation starts at one and advances for every state change.
     * pending request/transition are zero after completion. Deadline is an
     * absolute host steady-clock nanosecond value, never wall-clock time.
     */
    struct snapshot
    {
        std::uint64_t state{},generation{},request{},transition{},deadline{},status{},delivered{};
        friend bool operator==(snapshot const&,snapshot const&)=default;
    };
    /** @brief Fixed recoverable result: key, request, generation, state, status.
     * @details Submission replay returns its original accepted receipt; use
     * state() to observe subsequent completion. Response replay returns its
     * last exact response receipt. Success means local state transition only.
     */
    struct receipt
    {
        std::uint64_t key{},request{},generation{},state{},status{};
        friend bool operator==(receipt const&,receipt const&)=default;
    };
    /** @brief Deferred callback with no native pointers or implicit authority.
     * @details phase is the ROS intermediate state (10..15); transition remains
     * the original public transition even for the follow-up error callback.
     * respond must match request and generation and a successfully delivered
     * callback. All phases share the original finite deadline.
     */
    struct callback
    {std::uint64_t request{},transition{},generation{},phase{},deadline{};};
    struct inspection
    {::uwvm2::utils::container::std_string alias; snapshot current;std::uint64_t handles{};bool retired{};};
    /** @brief Cached client submission facts for a typed, observational adapter.
     * @details Reading never expires a request or schedules a callback. The
     * adapter must reject elapsed deadlines itself; actual submit repeats all
     * transition, generation, key and time checks before any state mutation.
     */
    struct submission_observation
    {snapshot current;plan bounds;std::uint64_t allowed{},next_key{};};

    class registry
    {
        struct submission
        {
            std::uint64_t key{},transition{},generation{},deadline{};
            friend bool operator==(submission const&,submission const&)=default;
        };
        struct response
        {
            std::uint64_t request{},generation{},outcome{};
            friend bool operator==(response const&,response const&)=default;
        };
        struct binding
        {
            plan bounds;
            std::uint64_t current{1},generation{1},request{},transition{},deadline{},status{},origin{},handles{};
            bool delivered{},delivering{},retired{};
            submission last_submission;
            receipt submitted;
            response last_response;
            receipt responded;
        };
        struct opened {binding* owner;std::uint64_t role;};
        mutable std::mutex mutex;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,binding,std::less<>> bindings;
        ::uwvm2::utils::container::std_map<handle,opened> handles;
        bool retired{};
#if defined(__linux__)
        ::pid_t process{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process==::getpid();
#else
            return true;
#endif
        }
        static constexpr std::uint64_t code(managed_lifecycle::state value) noexcept{return static_cast<std::uint64_t>(value);}
        static std::uint64_t now() noexcept
        {auto value=resources::steady_now();return value<0?0:static_cast<std::uint64_t>(value);}
        binding* find(std::string_view alias) noexcept
        {auto it=bindings.find(alias);return retired||it==bindings.end()||it->second.retired?nullptr:&it->second;}
        binding* owned(std::string_view alias,handle id,std::uint64_t role=client|server) noexcept
        {
            auto owner=find(alias);if(!owner)return nullptr;
            auto it=handles.find(id);return it!=handles.end()&&it->second.owner==owner&&(it->second.role&role)?owner:nullptr;
        }
        static std::uint64_t legal(std::uint64_t current) noexcept
        {
            switch(current)
            {case 1:return (1u<<1)|(1u<<5);case 2:return (1u<<2)|(1u<<3)|(1u<<6);case 3:return (1u<<4)|(1u<<7);default:return 0;}
        }
        static std::uint64_t intermediate(std::uint64_t transition) noexcept
        {constexpr std::array<std::uint64_t,8> states{0,10,11,13,14,12,12,12};return states[transition];}
        static std::uint64_t successful(std::uint64_t transition) noexcept
        {constexpr std::array<std::uint64_t,8> states{0,2,1,3,2,4,4,4};return states[transition];}
        static snapshot sample(binding const& value) noexcept
        {return {value.current,value.generation,value.request,value.transition,value.deadline,value.status,value.delivered?1u:0u};}
        static void finish(binding& value,std::uint64_t next,error result) noexcept
        {
            value.current=next;++value.generation;value.status=static_cast<unsigned>(result);
            value.request=value.transition=value.deadline=0;value.delivered=value.delivering=false;
        }
        static bool expire(binding& value,std::uint64_t time) noexcept
        {
            if(value.request&&time>=value.deadline){finish(value,code(managed_lifecycle::state::finalized),error::timeout);return true;}
            return false;
        }
    public:
        /** @brief Prepare one immutable local lifecycle scope; never execute a callback.
         * @details Allocation is confined to trusted bind/open/inspection. bind
         * does not expose an import or confer authority. Allocation exceptions
         * leave the alias absent and are converted by the host boundary.
         */
        error bind(::uwvm2::utils::container::std_string alias,plan limits)
        {
            if(!same_process())return error::denied;
            if(!identifier(alias)||!valid(limits))return error::invalid;
            std::lock_guard lock{mutex};if(retired)return error::denied;
            binding pending;pending.bounds=limits;
            return bindings.emplace(std::move(alias),std::move(pending)).second?error::success:error::conflict;
        }
        /** @brief Open a client or callback-server handle without resetting state.
         * @details role is exactly client or server, never a combined guest mask.
         * Handles share one alias state and consume its explicit finite count
         * and the caller grant's max-resources bound. The broker supplies the
         * latter from trusted policy, never from a guest request word.
         * Failed handle delivery must call abandon(); the identity is not reused.
         */
        error open(std::string_view alias,std::uint64_t role,std::uint64_t max_resources,handle& output)
        {
            if(!same_process())return error::denied;
            if(role!=client&&role!=server)return error::invalid;
            std::lock_guard lock{mutex};auto value=find(alias);if(!value)return error::denied;
            if(!(value->bounds.roles_mask&role))return error::denied;
            if(value->handles>=value->bounds.max_handles||value->handles>=max_resources)return error::again;
            auto id=resources::allocate_identity();if(!id)return error::too_large;
            handles.emplace(id,opened{value,role});++value->handles;output=id;return error::success;
        }
        /** @brief Roll back an unpublished handle; does not alter pending work. */
        void abandon(std::string_view alias,handle id)
        {
            if(!same_process())return;
            std::lock_guard lock{mutex};auto owner=bindings.find(alias);auto it=handles.find(id);
            if(owner!=bindings.end()&&it!=handles.end()&&it->second.owner==&owner->second)
            {--owner->second.handles;handles.erase(it);}
        }
        /** @brief Close only this handle; state/history and pending work survive. */
        error close(std::string_view alias,handle id)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=owned(alias,id);if(!value)return error::bad_handle;
            handles.erase(id);--value->handles;return error::success;
        }
        /** @brief Observe owned logical state, enforcing an expired finite deadline. */
        error state(std::string_view alias,handle id,snapshot& output)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=owned(alias,id);if(!value)return error::bad_handle;
            expire(*value,now());output=sample(*value);return error::success;
        }
        /** @brief Return only currently legal and explicitly allowed transition bits. */
        error transitions(std::string_view alias,handle id,std::uint64_t& output)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=owned(alias,id);if(!value)return error::bad_handle;
            expire(*value,now());output=legal(value->current)&value->bounds.transitions_mask;return error::success;
        }
        /** @brief Observe an actual client handle without advancing logical state or time. */
        error observe_submission(std::string_view alias,handle id,submission_observation& output)const noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto binding=bindings.find(alias);auto opened=handles.find(id);
            if(retired||binding==bindings.end()||binding->second.retired)return error::denied;
            if(opened==handles.end()||opened->second.owner!=&binding->second||!(opened->second.role&client))return error::bad_handle;
            auto const& value=binding->second;
            output={sample(value),value.bounds,legal(value.current)&value.bounds.transitions_mask,
                value.last_submission.key==UINT64_MAX?0:value.last_submission.key+1};return error::success;
        }
        /** @brief Queue one allowed transition under expected state generation.
         * @details Keys start at one; next successful submission acknowledges
         * the previous submission and response receipts. Exact last-request
         * replay is recoverable; changed/old/skipped keys conflict. Rejections
         * never consume a key. An accepted request atomically advances state
         * into its ROS intermediate phase. A single pending request prevents
         * overlapping transitions. Deadline/ID/generation validation precedes
         * effects and reserves enough generation range for an error callback.
         * A zero deadline is invalid; no implicit infinite wait exists.
         */
        error submit(std::string_view alias,handle id,std::uint64_t key,std::uint64_t transition,
                     std::uint64_t expected_generation,std::uint64_t deadline,receipt& output)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=owned(alias,id,client);if(!value)return error::bad_handle;
            auto time=now();expire(*value,time);
            submission request{key,transition,expected_generation,deadline};
            if(key&&key==value->last_submission.key)
            {if(request!=value->last_submission)return error::conflict;output=value->submitted;return error::success;}
            if(!key||value->last_submission.key==UINT64_MAX||key!=value->last_submission.key+1)return error::conflict;
            if(transition<1||transition>7)return error::invalid;
            if(!(value->bounds.transitions_mask&(std::uint64_t{1}<<transition)))return error::denied;
            if(value->request)return error::again;
            if(expected_generation!=value->generation||!(legal(value->current)&(std::uint64_t{1}<<transition)))return error::conflict;
            if(deadline<=time||deadline-time>value->bounds.max_deadline_ns||deadline>INT64_MAX)return error::invalid;
            if(value->generation>UINT64_MAX-3)return error::too_large;
            auto request_id=resources::allocate_identity();if(!request_id)return error::too_large;
            value->origin=value->current;value->current=intermediate(transition);++value->generation;
            value->request=request_id;value->transition=transition;value->deadline=deadline;value->status=0;value->delivered=false;
            value->last_submission=request;value->last_response={};value->responded={};
            value->submitted={key,request_id,value->generation,value->current,0};output=value->submitted;return error::success;
        }
        /** @brief Deliver one bounded callback, marking it delivered only after success.
         * @details Reserves one callback, unlocks for delivery and revalidates
         * after relocking. Reentrant/concurrent takes return again. A false
         * result or exception releases the reservation without consumption.
         * Closure, revocation, timeout or generation change during delivery
         * prevents completion; already copied bytes convey no response right.
         * Once delivered, another take returns again until an error phase.
         */
        template<typename Sink> error take(std::string_view alias,handle id,Sink&& deliver)
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex};auto value=owned(alias,id,server);if(!value)return error::bad_handle;
            if(expire(*value,now()))return error::timeout;
            if(!value->request||value->delivered||value->delivering)return error::again;
            callback event{value->request,value->transition,value->generation,value->current,value->deadline};
            value->delivering=true;lock.unlock();bool copied{};
            try{copied=deliver(event);}
            catch(...)
            {
                lock.lock();
                if(value->request==event.request&&value->generation==event.generation)value->delivering=false;
                throw;
            }
            lock.lock();
            if(value->request==event.request&&value->generation==event.generation)value->delivering=false;
            if(owned(alias,id,server)!=value)return error::cancelled;
            if(expire(*value,now()))return error::timeout;
            if(value->request!=event.request||value->generation!=event.generation)return error::cancelled;
            if(!copied)return error::invalid;
            value->delivered=true;return error::success;
        }
        /** @brief Resolve exactly the delivered callback or recover its last reply.
         * @details ROS callback outcomes are 97 success, 98 failure, 99 error.
         * Error queues on_error under the same request/deadline with a new
         * generation. Successful on_error returns unconfigured; failure/error
         * finalizes. Late, wrong-owner, stale-generation or undelivered replies
         * have no response effect. Exact last response replay never executes a
         * callback twice and retains its fixed receipt after delivery failure.
         */
        error respond(std::string_view alias,handle id,std::uint64_t request,std::uint64_t expected_generation,
                      std::uint64_t result,receipt& output)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=owned(alias,id,server);if(!value)return error::bad_handle;
            auto expired=expire(*value,now());response supplied{request,expected_generation,result};
            if(request&&supplied==value->last_response){output=value->responded;return error::success;}
            if(expired)return error::timeout;
            if(result<97||result>99)return error::invalid;
            if(!request||request!=value->request||expected_generation!=value->generation||!value->delivered)return error::conflict;
            auto key=value->last_submission.key;
            if(value->current==code(managed_lifecycle::state::error_processing))
                finish(*value,result==97?1:4,error::success);
            else if(result==99)
            {value->current=15;++value->generation;value->delivered=false;}
            else finish(*value,result==97?successful(value->transition):(value->transition>=5?4:value->origin),error::success);
            value->last_response=supplied;value->responded={key,request,value->generation,value->current,0};
            output=value->responded;return error::success;
        }
        /** @brief Permanently revoke one alias and stop its logical transition queue.
         * @details No physical device stop is implied. Retained handles remain
         * charged for diagnostics until destruction; nothing can reopen this
         * binding. Completion callbacks cannot revive a retired scope.
         */
        void revoke(std::string_view alias)
        {
            if(!same_process())return;
            std::lock_guard lock{mutex};auto it=bindings.find(alias);if(it==bindings.end()||it->second.retired)return;
            auto& value=it->second;if(value.request)finish(value,4,error::cancelled);
            value.retired=true;
        }
        /** @brief Retire the entire provider without executing a guest callback. */
        void retire()
        {
            if(!same_process())return;
            std::lock_guard lock{mutex};retired=true;
            for(auto& [alias,value]:bindings){if(value.request)finish(value,4,error::cancelled);value.retired=true;}
        }
        /** @brief Copy bounded cached diagnostics without advancing time or callbacks. */
        bool inspect(::uwvm2::utils::container::std_vector<inspection>& output,std::size_t maximum) const
        {
            if(!same_process())return false;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock()||bindings.size()>maximum)return false;
            ::uwvm2::utils::container::std_vector<inspection> pending;pending.reserve(bindings.size());
            for(auto const& [alias,value]:bindings)pending.push_back({alias,sample(value),value.handles,value.retired||retired});
            output=std::move(pending);return true;
        }
    };
}
