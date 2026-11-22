/** @file
 * @brief Independent authority for robot-scene operations.
 * @details This component has no WASI/ROS-v1 policy dependency. A trusted
 * deployment creates exact grants; a guest can only open an existing alias.
 * Admission checks caller membership, interface exposure, purpose, action and
 * resource ownership before reserving optional call/byte budgets. A held lease
 * linearizes effects with revocation. Blocking operations release the lease
 * and reacquire without charging again before delivering a result.
 *
 * Optional rate and byte limits apply only after authority exists. Missing budgets
 * mean unrestricted execution WITHIN the grant. Explicit zero means no
 * admissions. Fixed capacity reservations are independent of rate limits.
 */
#pragma once
#include "identifier.h"
#include <uwvm2/utils/container/standard.h>
#include <array>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>
#include "../runtime/resources.h"
#include "../runtime/contract.h"
#include "artifact_contract.h"
#include "output_contract.h"
#include "revocation_contract.h"
#include "permission_report.h"
#include "module_exchange_contract.h"

namespace uwvm2::imported::ros::scene
{
    using error=resources::error;
    using handle=std::uint64_t;
    enum class exposure : std::uint8_t { hidden, deny, allow };
    enum class purpose : std::uint8_t
    {
        guard, steady_clock, system_clock, ros_clock, timer, wait,
        package, configuration, calibration, map, model_weights,
        log, recording, cache, sensor, device, topic, service, action,
        discovery, bridge, language_model, vision_model, world_model, lifecycle, buffer,
        context, node, type, event, parameter, transform, control, media, replay, proposal, arbiter, component, diagnostic, module_inspection, module_call, vm_inspection, driver
    };
    /** @brief One shared spelling table for trusted CLI parsing and debug labels. */
    inline constexpr std::array purpose_names{std::string_view{"guard"},std::string_view{"clock.steady"},std::string_view{"clock.system"},
        std::string_view{"clock.ros"},std::string_view{"timer"},std::string_view{"wait"},std::string_view{"package"},std::string_view{"configuration"},
        std::string_view{"calibration"},std::string_view{"map"},std::string_view{"model-weights"},std::string_view{"log"},std::string_view{"recording"},
        std::string_view{"cache"},std::string_view{"sensor"},std::string_view{"device"},std::string_view{"topic"},std::string_view{"service"},
        std::string_view{"action"},std::string_view{"discovery"},std::string_view{"bridge"},std::string_view{"model.language"},
        std::string_view{"model.vision"},std::string_view{"model.world"},std::string_view{"lifecycle"},std::string_view{"buffer"},
        std::string_view{"context"},std::string_view{"node"},std::string_view{"type"},std::string_view{"event"},std::string_view{"parameter"},
        std::string_view{"transform"},std::string_view{"control"},std::string_view{"media"},std::string_view{"replay"},std::string_view{"proposal"},
        std::string_view{"arbiter"},std::string_view{"component"},std::string_view{"diagnostic"},std::string_view{"module"},std::string_view{"module.call"},std::string_view{"vm"},std::string_view{"driver"}};
    static_assert(purpose_names.size()==static_cast<std::size_t>(purpose::driver)+1);
    /** @brief Exact actions accepted by each purpose; no implicit descendants.
     * @details This is a semantic catalogue, not an implementation claim.
     * Provider admission separately rejects unsupported actions. Storage
     * actions cannot be used on a network binding or vice versa. Model tools
     * produce proposals only; device execution always needs a device grant.
     */
    inline bool permits(purpose kind,std::string_view action) noexcept
    {
        if(kind>purpose::driver)return false;
        if(kind==purpose::driver)return action=="driver.connect"||action=="driver.submit"||action=="driver.result"||action=="driver.describe"||action=="driver.disconnect"||action=="capability.revoke";
        if(kind==purpose::vm_inspection)return action==permission_report::vm_import_name||action=="capability.revoke";
        if(kind==purpose::module_call)return action=="module.call.submit"||action=="module.call.take"||action=="module.call.respond"||
            action=="module.call.result"||action=="module.call.acknowledge"||action=="module.call.cancel"||action=="capability.revoke";
        if(kind==purpose::module_inspection)
        {
            for(auto name:permission_report::module_operations)if(action==name)return true;
            return action=="module.permission.report"||action=="capability.revoke";
        }
        if(action=="capability.revoke"||action=="resource.close"||(action=="resource.observe"&&kind!=purpose::buffer))return true;
        if(action=="request.describe"||action=="request.release")
        {
            switch(kind)
            {
            case purpose::context:case purpose::node:case purpose::topic:case purpose::service:case purpose::action:case purpose::parameter:case purpose::lifecycle:
            case purpose::transform:case purpose::calibration:case purpose::map:case purpose::cache:case purpose::device:
            case purpose::control:case purpose::media:case purpose::recording:case purpose::language_model:case purpose::vision_model:
            case purpose::world_model:case purpose::arbiter:case purpose::bridge:case purpose::component:case purpose::log:return true;
            default:return false;
            }
        }
        if(action=="communication.open.take"||action=="communication.open.cancel")
            return kind==purpose::node||kind==purpose::topic||kind==purpose::service||kind==purpose::action;
        if(action=="data.export")return kind==purpose::buffer||kind==purpose::sensor||kind==purpose::media||kind==purpose::topic||
            kind==purpose::service||kind==purpose::action||kind==purpose::language_model||kind==purpose::vision_model||kind==purpose::world_model||
            kind==purpose::package||kind==purpose::configuration||kind==purpose::calibration||kind==purpose::map||kind==purpose::cache||
            kind==purpose::recording||kind==purpose::replay;
        if(action=="qos.describe")return kind==purpose::topic||kind==purpose::service||kind==purpose::action;
        switch(kind)
        {
        case purpose::buffer:return action=="buffer.create"||action=="buffer.write"||action=="buffer.seal"||action=="buffer.read"||action=="buffer.describe";
        case purpose::lifecycle:return action=="lifecycle.open"||action=="lifecycle.state"||action=="lifecycle.transitions"||action=="lifecycle.submit"||action=="lifecycle.take"||action=="lifecycle.respond";
        case purpose::guard:return action=="guard.create"||action=="guard.trigger"||action=="guard.take";
        case purpose::steady_clock:case purpose::system_clock:
            return action=="clock.create"||action=="clock.read";
        case purpose::ros_clock:return action=="clock.create"||action=="clock.read"||action=="clock.update";
        case purpose::timer:return action=="timer.create"||action=="timer.reset"||action=="timer.cancel"||action=="timer.take";
        case purpose::wait:return action=="wait.create"||action=="wait.add"||action=="wait.remove"||action=="wait.acknowledge"||action=="wait.poll";
        case purpose::package:case purpose::configuration:case purpose::model_weights:
            return action=="artifact.open"||action=="artifact.read"||action=="artifact.describe";
        case purpose::calibration:case purpose::map:
            return action=="artifact.open"||action=="artifact.read"||action=="artifact.describe"||action=="artifact.begin"||
                   action=="artifact.append"||action=="artifact.commit"||action=="artifact.abort"||action=="artifact.remove";
        case purpose::cache:
            return action=="artifact.begin"||action=="artifact.append"||action=="artifact.commit"||action=="artifact.abort"||
                   action=="artifact.remove"||action=="artifact.read"||action=="artifact.status"||action=="artifact.receipt"||action=="artifact.publish";
        case purpose::log:
            if(action=="log.emit")return true;
            [[fallthrough]];
        case purpose::recording:
            if(kind==purpose::recording&&(action=="recording.open"||action=="recording.append"||action=="recording.checkpoint"||action=="recording.stop"))return true;
            return action=="artifact.begin"||action=="artifact.append"||action=="artifact.commit"||action=="artifact.abort"||
                   action=="artifact.remove"||action=="artifact.open"||action=="artifact.read"||action=="artifact.describe";
        case purpose::sensor:return action=="sensor.open"||action=="sensor.take"||action=="sensor.describe";
        case purpose::device:return action=="device.open"||action=="device.submit"||action=="device.result"||action=="device.stop"||action=="device.describe";
        case purpose::topic:return action=="topic.open"||action=="topic.publish"||action=="topic.take"||action=="topic.describe"||
            action=="topic.reserve"||action=="topic.acknowledge"||action=="topic.release"||
            action=="qos.liveliness.assert"||action=="qos.acknowledgements.wait";
        case purpose::service:return action=="service.open"||action=="service.submit"||action=="service.take"||action=="service.respond"||action=="service.available"||action=="service.abandon"||action=="service.describe"||
            action=="service.reserve"||action=="service.acknowledge"||action=="service.release";
        case purpose::action:return action=="action.open"||action=="action.submit"||action=="action.take"||action=="action.respond"||action=="action.cancel"||action=="action.cancel.set"||action=="action.describe"||
            action=="action.accept"||action=="action.reject"||action=="action.execute"||action=="action.feedback"||action=="action.succeed"||action=="action.abort"||
            action=="action.finish.cancelled"||action=="action.cancel.respond";
        case purpose::discovery:return action=="graph.snapshot"||action=="graph.watch";
        case purpose::bridge:return action=="bridge.create"||action=="bridge.start"||action=="bridge.stop"||action=="bridge.describe";
        case purpose::language_model:case purpose::vision_model:case purpose::world_model:
            return action=="model.open"||action=="model.submit"||action=="model.take"||action=="model.cancel"||action=="model.describe"||
                   action=="request.describe"||action=="request.release";
        case purpose::context:return action=="context.open"||action=="context.describe"||action=="context.shutdown";
        case purpose::node:return action=="node.open"||action=="node.describe";
        case purpose::type:return action=="type.describe";
        case purpose::event:return action=="event.stream.open"||action=="event.stream.poll"||action=="event.stream.close";
        case purpose::parameter:return action=="parameter.open"||action=="parameter.declare"||action=="parameter.undeclare"||action=="parameter.get"||
            action=="parameter.describe"||action=="parameter.list"||action=="parameter.set"||action=="parameter.set.atomic"||action=="parameter.take"||action=="parameter.respond"||
            action=="parameter.exists"||action=="parameter.search";
        case purpose::transform:return action=="transform.open"||action=="transform.publish"||action=="transform.lookup"||action=="transform.available"||
            action=="transform.velocity"||action=="transform.submit"||action=="transform.take"||action=="transform.cancel"||action=="transform.clear"||action=="transform.static.replace";
        case purpose::control:return action=="control.open"||action=="control.sample"||action=="control.submit"||action=="control.result"||action=="control.stop";
        case purpose::media:return action=="media.open"||action=="media.submit"||action=="media.take"||action=="media.close";
        case purpose::replay:return action=="replay.open"||action=="replay.read"||action=="replay.seek";
        case purpose::proposal:return action=="proposal.read"||action=="proposal.capture"||action=="proposal.publish";
        case purpose::arbiter:return action=="proposal.accept";
        case purpose::component:return false; // Native driver lifecycle belongs to the deployment layer.
        case purpose::diagnostic:return action=="diagnostic.read";
        case purpose::module_inspection:return false; // Closed observation actions were handled above.
        case purpose::module_call:return false;
        case purpose::vm_inspection:case purpose::driver:return false;
        }
        return false;
    }
    /** @brief Closed secondary-permission names; none is a callable import. */
    inline bool grant_only_action(std::string_view action) noexcept
    {
        return action=="resource.observe"||action=="data.export"||action=="transform.static.replace"||
            action=="action.accept"||action=="action.reject"||action=="action.execute"||action=="action.feedback"||
            action=="action.succeed"||action=="action.abort"||action=="action.finish.cancelled"||action=="action.cancel.respond";
    }
    /** @brief Closed, independently exposed cleanup operations with reserved capacity.
     * @details These release an owned record or close an owned resource; they
     * cannot disclose payloads, acknowledge consumption or create resources. Their explicit capability and
     * exposure remain mandatory, and ordinary traffic limits leave reserved cleanup capacity available.
     */
    inline bool cleanup_action(std::string_view action)noexcept
    {return action=="driver.disconnect"||action=="topic.release"||action=="service.release"||action=="resource.close"||action=="communication.open.cancel"||action=="request.release"||action=="bridge.stop"||action=="device.stop"||action=="control.stop"||action=="component.stop";}
    /** @brief Optional restrictions on a previously granted scope.
     * @details Call and byte rates independently reserve their entire admitted
     * cost. Individual operations cost one call unit; complete artifact.publish
     * costs three units so it cannot bypass the equivalent component work
     * budget. Admission counters still count admitted carrier attempts. A zero
     * window is invalid. No implicit clipping, drop,
     * sleep or partial acceptance is permitted by this admission component.
     * `uncertain` explicitly acknowledges possible quota rejection. It never
     * affects the authorization decision or broadens a resource set.
     */
    struct attenuation
    {
        std::optional<std::uint64_t> calls,bytes;
        std::chrono::nanoseconds window{std::chrono::seconds(1)};
        bool uncertain{};
    };
    /** @brief Immutable deployment grant; aliases and bindings are logical IDs.
     * @details Purpose and action set are mandatory. Resource and binding IDs
     * are deployment metadata, not filesystem paths or connection strings.
     * Capacity defaults to unlimited within this scope; a trusted provider
     * may require an explicit finite reservation before activation.
     */
    struct grant
    {
        ::uwvm2::utils::container::std_string alias,resource,binding;
        purpose kind{};
        ::uwvm2::utils::container::std_set<::uwvm2::utils::container::std_string,std::less<>> actions;
        std::uint64_t max_handles{UINT64_MAX},max_resources{UINT64_MAX},max_wait_members{UINT64_MAX},max_wait_ns{UINT64_MAX};
        bool infinite_wait{};
        attenuation limits;
    };
    /** @brief An independently revocable readiness grant for exactly one client grant.
     * @details The operator sets resource to the exact client grant alias. This
     * registration confers no ability to open the endpoint or send requests.
     */
    inline bool service_observer(grant const& value) noexcept
    {
        return value.kind==purpose::service && value.binding=="service-observer" && value.alias!=value.resource &&
               value.actions.size()==1 && value.actions.contains("service.available");
    }
    inline bool valid(grant const& value) noexcept
    {
        if(!identifier(value.alias)||!identifier(value.resource)||!identifier(value.binding)||value.actions.empty())return false;
        if(value.binding=="service-observer"&&!service_observer(value))return false;
        for(auto const& action:value.actions)if(!permits(value.kind,action))return false;
        bool limited=value.limits.calls.has_value()||value.limits.bytes.has_value();
        return value.limits.window.count()>0&&(!limited||value.limits.uncertain);
    }
    /** @brief Runtime membership is issued by the VM, never supplied by Wasm.
     * @details Pointer identity and generation come from the checked runtime
     * caller API. Generation zero and retired memberships are denied. Group
     * sharing requires explicit enrollment in this same environment.
     */
    struct caller
    {
        void const* instance{};std::uint64_t generation{};
        friend bool operator<(caller const& a,caller const& b) noexcept
        {return std::less<void const*>{}(a.instance,b.instance)||(a.instance==b.instance&&a.generation<b.generation);}
    };
    class authority;
    /** @brief Move-only admission lease held across a nonblocking side effect.
     * @details Release before blocking; reacquire after wake. Destruction
     * releases only synchronization, never persistent storage capacity.
     */
    class lease
    {
        friend class authority;
        std::unique_lock<std::recursive_mutex> lock;
        grant const* value{};
    public:
        lease()=default;lease(lease const&)=delete;lease& operator=(lease const&)=delete;
        lease(lease&&)=default;lease& operator=(lease&&)=default;
        explicit operator bool() const noexcept{return lock.owns_lock()&&value;}
        grant const& specification() const noexcept{return *value;}
        void release() noexcept{value=nullptr;if(lock.owns_lock())lock.unlock();}
    };
    /** @brief One independently authorized resource participating in an operation.
     * @details Constructed by a trusted broker after canonical decoding. The
     * action is exact; each charged claim costs one work unit and its full byte cost.
     * Repeated capabilities do not collapse these costs. Owning no input data,
     * this record is borrowed only for the synchronous admission call.
     * grant_only permits only the closed secondary-action list and has no
     * import exposure. validation_only requires zero bytes and checks a held
     * resource without consuming its creation rate again. It still requires
     * current membership, exact action, exposure (unless grant_only) and grant.
     * cleanup_only requires a zero-byte closed cleanup action, keeps its import
     * exposure and yields a one-use publication lease without debiting traffic.
     * A charged group must contain a charged action or explicit cleanup. The
     * trusted broker determines these roles; guest data cannot select them.
     */
    struct admission_claim {handle capability{};std::string_view action;std::uint64_t bytes{};bool grant_only{},validation_only{},cleanup_only{};};
    inline constexpr std::size_t maximum_admission_claims=128;
    /** @brief One lock covering every independently admitted resource scope.
     * @details Fixed storage makes admission allocation-free. Specifications
     * remain ordered like the claims, even for repeated grants. Retain this
     * lease through nonblocking buffer pinning and job publication; release
     * before provider waits and revalidate the whole set before delivery.
     * This object cannot be constructed by a guest and grants no new rights.
     */
    class group_lease
    {
        friend class authority;
        std::unique_lock<std::recursive_mutex> lock;
        std::array<grant const*,maximum_admission_claims> values{};
        std::array<admission_claim,maximum_admission_claims> accepted{};
        authority const* issuer{};
        caller admitted_caller{};
        std::thread::id native_thread{};
        std::size_t count{};
        bool charge_available{};
    public:
        group_lease()=default;group_lease(group_lease const&)=delete;group_lease& operator=(group_lease const&)=delete;
        group_lease(group_lease&&)=default;group_lease& operator=(group_lease&&)=default;
        explicit operator bool() const noexcept;
        std::size_t size() const noexcept{return native_thread==std::this_thread::get_id()&&lock.owns_lock()?count:0;}
        grant const* specification(std::size_t index) const noexcept
        {return index<size()?values[index]:nullptr;}
        /** @brief Verify the exact ordered admission, including caller, costs and claim roles.
         * @details Action views refer to immutable grant-owned strings, never the
         * caller's temporary request storage. Current membership and every live
         * handle are checked again: recursive same-thread close/revoke invalidates
         * a previously held group. This method performs no allocation, charging,
         * clock sampling or callback. A lease stays on its acquiring native thread
         * until release; another thread cannot use it as authority. The issuer
         * must outlive the lease. Resource identities still need broker validation.
         */
        bool matches(caller who,std::span<admission_claim const> claims) const noexcept;
        /** @brief Consume one successful charged admission for one queue publication.
         * @details Preflight and validation-only admissions have no consumable
         * charge. Consuming prevents a single charged group from publishing two
         * different requests. Exact retries inspect the existing receipt instead.
         */
        bool consume(caller who,std::span<admission_claim const> claims) noexcept
        {if(!charge_available||!matches(who,claims))return false;charge_available=false;return true;}
        void release() noexcept{count=0;issuer=nullptr;charge_available=false;if(lock.owns_lock())lock.unlock();}
    };
    struct authority_snapshot
    {::uwvm2::utils::container::std_string alias,resource,binding;purpose kind{};bool revoked{},attenuated{};std::uint64_t open_handles{},admissions{},refused{};};
    /** @brief A separate, default-empty scene authority environment.
     * @details `configure`, `expose`, `enroll`, `revoke`, `retire` are trusted
     * management methods, not guest imports. Configuration freezes on first
     * enrollment. Handles are monotonic across environments and cannot be
     * reopened after revocation. This class invokes no native provider.
     */
    class authority
    {
        friend class group_lease;
        struct account
        {
            grant policy;bool revoked{};std::uint64_t calls{},bytes{},admissions{},refused{},open_handles{};
            std::chrono::steady_clock::time_point began{};
            std::uint64_t indexed_actions{};
            // Pre-reserved, one-shot guest reduction receipt; never a rate budget.
            caller revoker{};
            revocation_contract::receipt revocation{};
        };
        mutable std::recursive_mutex mutex;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,account,std::less<>> grants;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,exposure,std::less<>> interfaces;
        ::uwvm2::utils::container::std_set<caller> members;
        ::uwvm2::utils::container::std_set<caller> retired_members;
        ::uwvm2::utils::container::std_map<handle,account*> handles;
        inline static std::atomic<handle> next{1};
        static constexpr auto artifact_offset=runtime_contract::operations.size();
        static constexpr auto output_offset=artifact_offset+artifact_contract::operations.size();
        static constexpr auto exchange_offset=output_offset+output_contract::operations.size();
        static_assert(exchange_offset+module_exchange::names.size()<=64);
        static std::uint64_t indexed_bit(std::string_view name) noexcept
        {
            for(std::size_t n{};n<runtime_contract::operations.size();++n)if(name==runtime_contract::operations[n].name)return std::uint64_t{1}<<n;
            for(std::size_t n{};n<artifact_contract::operations.size();++n)if(name==artifact_contract::operations[n].name)return std::uint64_t{1}<<(artifact_offset+n);
            for(std::size_t n{};n<output_contract::operations.size();++n)if(name==output_contract::operations[n].name)return std::uint64_t{1}<<(output_offset+n);
            for(std::size_t n{};n<module_exchange::names.size();++n)if(name==module_exchange::names[n])return std::uint64_t{1}<<(exchange_offset+n);
            return 0;
        }
        std::uint64_t indexed_exposure{};
        bool frozen{},retired{};
#if defined(__linux__)
        ::pid_t revocation_process{::getpid()};
#endif
        bool member(caller who) const noexcept{return !retired&&who.instance&&who.generation&&members.contains(who);}
        bool group_current(group_lease const& held) const noexcept
        {
            if(!member(held.admitted_caller))return false;
            for(std::size_t i{};i<held.count;++i)
            {
                auto found=handles.find(held.accepted[i].capability);
                if(found==handles.end()||found->second->revoked||&found->second->policy!=held.values[i])return false;
            }
            return true;
        }
        exposure visible_locked(std::string_view action) const noexcept
        {auto i=interfaces.find(action);return i==interfaces.end()?exposure::hidden:i->second;}
        bool erase_handle(handle id)
        {
            auto i=handles.find(id);if(i==handles.end())return false;
            --i->second->open_handles;handles.erase(i);return true;
        }
        static void increment(std::uint64_t& value) noexcept{if(value!=UINT64_MAX)++value;}
        static handle allocate() noexcept
        {
            auto id=next.load(std::memory_order_relaxed);
            while(id&&id!=UINT64_MAX)if(next.compare_exchange_weak(id,id+1,std::memory_order_relaxed))return id;
            return 0;
        }
        template<bool Indexed>
        error admit_checked(caller who,handle id,std::string_view action,std::uint64_t action_bit,
                            std::uint64_t bytes,lease& output,bool charge,bool check_budget=false,std::uint64_t work_units=1)
        {
            output.release();std::unique_lock lock{mutex};
            if(!member(who))return error::denied;
            if constexpr(Indexed){if(!action_bit||(indexed_exposure&action_bit)!=action_bit)return error::denied;}
            else if(visible_locked(action)!=exposure::allow)return error::denied;
            auto it=handles.find(id);if(it==handles.end())return error::bad_handle;
            auto& state=*it->second;
            if(state.revoked)return error::denied;
            if constexpr(Indexed){if((state.indexed_actions&action_bit)!=action_bit)return error::denied;}
            else if(!state.policy.actions.contains(action))return error::denied;
            if(charge||check_budget)
            {
                auto const& bounds=state.policy.limits;
                if(bounds.calls||bounds.bytes)
                {
                    auto now=std::chrono::steady_clock::now();
                    auto calls=state.calls,transferred=state.bytes;
                    if(now-state.began>=bounds.window)
                    {
                        calls=transferred=0;
                        if(charge){state.began=now;state.calls=state.bytes=0;}
                    }
                    if((bounds.calls&&(calls>*bounds.calls||work_units>*bounds.calls-calls))||(bounds.bytes&&(transferred>*bounds.bytes||bytes>*bounds.bytes-transferred)))
                    {if(charge)increment(state.refused);return error::again;}
                    if(charge)
                    {
                        if(bounds.calls)state.calls+=work_units;
                        if(bounds.bytes)state.bytes+=bytes;
                    }
                }
                if(charge)increment(state.admissions);
            }
            output.value=&state.policy;output.lock=std::move(lock);return error::success;
        }
    public:
        authority()=default;authority(authority const&)=delete;authority& operator=(authority const&)=delete;
        /** @brief Atomically authorize and debit all scopes of a compound operation.
         * @details Every claim requires exact live capability and import
         * exposure. Costs for aliases of the same grant are accumulated with
         * checked arithmetic before any quota mutation. Failure never charges
         * earlier claims or refreshes their windows. Only the refusing account
         * gains a refusal diagnostic on a charged attempt. Preflight and
         * post-wait revalidation change no counters or clocks. Rates absent
         * from a grant remain unlimited within that scope; explicit zero is
         * still zero. Artifact publication uses its dedicated four-action
         * admission and is rejected here to prevent work-cost substitution.
         */
        error admit_group(caller who,std::span<admission_claim const> claims,group_lease& output,
                          bool charge=true,bool check_budget=false)
        {
            output.release();
            if(claims.empty()||claims.size()>maximum_admission_claims)return error::invalid;
            std::unique_lock lock{mutex};if(!member(who))return error::denied;
            struct pending_account
            {account* state{};std::uint64_t work{},bytes{},calls{},transferred{};bool elapsed{};};
            std::array<pending_account,maximum_admission_claims> pending{};
            std::array<grant const*,maximum_admission_claims> policies{};std::size_t accounts{};
            bool timed{},cleanup{};
            for(std::size_t index{};index<claims.size();++index)
            {
                auto const& claim=claims[index];
                if(claim.action=="artifact.publish")return error::unsupported;
                if(claim.validation_only&&claim.bytes)return error::invalid;
                if(claim.cleanup_only&&(claim.bytes||claim.grant_only||claim.validation_only||!cleanup_action(claim.action)))return error::invalid;
                if(claim.grant_only?!grant_only_action(claim.action):visible_locked(claim.action)!=exposure::allow)return error::denied;
                auto found=handles.find(claim.capability);if(found==handles.end())return error::bad_handle;
                auto* state=found->second;
                auto action=state->policy.actions.find(claim.action);
                if(state->revoked||action==state->policy.actions.end())return error::denied;
                policies[index]=&state->policy;
                output.accepted[index]=claim;output.accepted[index].action=*action;
                if(claim.cleanup_only){cleanup=true;continue;}
                if(claim.validation_only)continue;
                std::size_t slot{};while(slot<accounts&&pending[slot].state!=state)++slot;
                if(slot==accounts){pending[slot].state=state;++accounts;}
                auto& cost=pending[slot];
                if(claim.bytes>UINT64_MAX-cost.bytes)return error::too_large;
                cost.bytes+=claim.bytes;++cost.work;
                timed=timed||state->policy.limits.calls.has_value()||state->policy.limits.bytes.has_value();
            }
            if(charge&&!accounts&&!cleanup)return error::invalid;
            if(charge||check_budget)
            {
                auto now=timed?std::chrono::steady_clock::now():std::chrono::steady_clock::time_point{};
                for(std::size_t slot{};slot<accounts;++slot)
                {
                    auto& cost=pending[slot];auto& state=*cost.state;auto const& bounds=state.policy.limits;
                    cost.elapsed=(bounds.calls||bounds.bytes)&&now-state.began>=bounds.window;
                    cost.calls=cost.elapsed?0:state.calls;cost.transferred=cost.elapsed?0:state.bytes;
                    if((bounds.calls&&(cost.calls>*bounds.calls||cost.work>*bounds.calls-cost.calls))||
                       (bounds.bytes&&(cost.transferred>*bounds.bytes||cost.bytes>*bounds.bytes-cost.transferred)))
                    {if(charge)increment(state.refused);return error::again;}
                }
                if(charge)for(std::size_t slot{};slot<accounts;++slot)
                {
                    auto const& cost=pending[slot];auto& state=*cost.state;auto const& bounds=state.policy.limits;
                    if(cost.elapsed){state.began=now;state.calls=state.bytes=0;}
                    if(bounds.calls)state.calls=cost.calls+cost.work;
                    if(bounds.bytes)state.bytes=cost.transferred+cost.bytes;
                    state.admissions=cost.work>UINT64_MAX-state.admissions?UINT64_MAX:state.admissions+cost.work;
                }
            }
            output.values=policies;output.count=claims.size();output.issuer=this;output.admitted_caller=who;
            output.native_thread=std::this_thread::get_id();output.charge_available=charge;
            output.lock=std::move(lock);return error::success;
        }
        /** @brief Atomically admit complete publication and all three component actions.
         * @param request_bytes Owned carrier extent including its 16-byte header.
         * @details The four exact exposures and actions must all belong to this
         * live capability. One lease covers the final effect. Optional limits
         * reserve three logical call units and payload+280 bytes together;
         * insufficient allowance never partially debits the composite call.
         * Empty payloads conservatively pay the same fixed work cost. Missing
         * limits remain unlimited within authority; explicit zero denies work.
         * With charge=false/check_budget=false this is pure authority validation;
         * a second lease can revalidate under an already-held recursive lease
         * without releasing it or waiting after a cancellation observation.
         */
        error admit_publish(caller who,handle id,std::uint64_t request_bytes,lease& output,bool charge=true,bool check_budget=false)
        {
            output.release();
            if(request_bytes<output_contract::publication_framing)return error::invalid;
            auto payload=request_bytes-output_contract::publication_framing;
            if(payload>UINT64_MAX-output_contract::publication_byte_overhead)return error::too_large;
            constexpr auto bit=[](output_contract::operation action){return std::uint64_t{1}<<(output_offset+static_cast<unsigned>(action));};
            constexpr auto mask=bit(output_contract::operation::publish)|bit(output_contract::operation::begin)|
                bit(output_contract::operation::append)|bit(output_contract::operation::commit);
            return admit_checked<true>(who,id,{},mask,payload+output_contract::publication_byte_overhead,output,charge,check_budget,
                                      output_contract::publication_work_units);
        }
    private:
        /** @brief Preserve generic admission's request-plus-receipt cost convention.
         * @details Generic publish calls must use their actual carrier bytes
         * plus 72, except a zero-cost authority-only revalidation. No generic
         * string or enum route may reduce the required dependency action set.
         */
        error admit_publish_wire(caller who,handle id,std::uint64_t wire_bytes,lease& output,bool charge,bool check_budget)
        {
            if(!charge&&!check_budget&&wire_bytes==0)return admit_publish(who,id,output_contract::publication_framing,output,false,false);
            if(wire_bytes<output_contract::publication_framing+output_contract::receipt_bytes){output.release();return error::invalid;}
            return admit_publish(who,id,wire_bytes-output_contract::receipt_bytes,output,charge,check_budget);
        }
    public:
        /** @brief Add one exact, immutable grant before enrolling any callers. */
        error configure(grant value)
        {
            std::lock_guard lock{mutex};if(frozen||retired||!valid(value))return error::invalid;
            auto key=value.alias;
            account pending{std::move(value)};
            for(auto const& action:pending.policy.actions)pending.indexed_actions|=indexed_bit(action);
            if(!grants.emplace(std::move(key),std::move(pending)).second)return error::conflict;
            return error::success;
        }
        /** @brief Attenuate an existing live grant without creating or widening authority.
         * @details The trusted manager may move stable authority to explicitly
         * may-reject traffic policy. The first transition starts a prospective
         * window: effects already admitted under the stable grant retain their
         * reservations. Further changes may only lower existing limits or add
         * a new meter, and must preserve the original window duration/start.
         * Usage, handles, queued receipts, controller leases and stop capacity
         * are never reset. Identical requests are idempotent. A new meter starts
         * counting at its explicit installation; earlier unmetered traffic is
         * not retroactively denied. Zero means no new metered traffic.
         *
         * This method performs no allocation, provider call, sleep or effect.
         * It shares the authority lock with complete admission groups, so a
         * compound call cannot observe partly installed limits. It cannot
         * revive a revoked grant, unenroll a caller, broaden a scope or remove
         * a meter. Removing attenuation requires a new reviewed deployment.
         * Reserved cleanup is still admitted by the provider's exact cleanup
         * operation; an attenuation update itself never emits a physical stop.
         */
        error attenuate(std::string_view alias,attenuation const& next)
        {
#if defined(__linux__)
            if(revocation_process!=::getpid())return error::denied;
#endif
            std::lock_guard lock{mutex};
            if(retired||!frozen)return error::denied;
            auto found=grants.find(alias);if(found==grants.end()||found->second.revoked)return error::denied;
            if(!next.uncertain||next.window.count()<=0||(!next.calls&&!next.bytes))return error::invalid;
            auto& state=found->second;auto const& previous=state.policy.limits;
            auto narrows=[](auto const& old,auto const& value)noexcept{return !old||(value&&*value<=*old);};
            if(!narrows(previous.calls,next.calls)||!narrows(previous.bytes,next.bytes))return error::denied;
            bool metered=previous.calls.has_value()||previous.bytes.has_value();
            if(metered&&previous.window!=next.window)return error::denied;
            if(!metered){state.began=std::chrono::steady_clock::now();state.calls=state.bytes=0;}
            state.policy.limits=next;return error::success;
        }
        /** @brief Configure one interface; repeated settings are rejected. */
        error expose(::uwvm2::utils::container::std_string action,exposure state)
        {
            std::lock_guard lock{mutex};if(frozen||retired||!identifier(action)||state>exposure::allow)return error::invalid;
            auto [entry,inserted]=interfaces.emplace(std::move(action),state);
            if(!inserted)return error::conflict;
            if(state==exposure::allow)indexed_exposure|=indexed_bit(entry->first);
            return error::success;
        }
        /** @brief Enroll one trusted runtime generation without inheriting old policies. */
        error enroll(caller who)
        {
            std::lock_guard lock{mutex};if(retired||!who.instance||!who.generation||retired_members.contains(who))return error::denied;
            members.insert(who);frozen=true;return error::success;
        }
        exposure visible(std::string_view action)
        {std::lock_guard lock{mutex};return visible_locked(action);}
        /** @brief Open an existing alias; opening never creates or upgrades a grant.
         * @details Per-account handle counts are maintained under the same lock
         * as publication, close and rollback; opening never scans peer handles.
         */
        error open(caller who,std::string_view alias,handle& output)
        {
            std::lock_guard lock{mutex};if(!member(who)||visible_locked("capability.open")!=exposure::allow)return error::denied;
            auto i=grants.find(alias);if(i==grants.end()||i->second.revoked)return error::denied;
            if(i->second.open_handles>=i->second.policy.max_handles)return error::again;
            auto id=allocate();if(!id)return error::too_large;
            handles.emplace(id,&i->second);++i->second.open_handles;output=id;return error::success;
        }
        /** @brief Verify action and atomically reserve its full admission budget.
         * @param charge False only for trusted post-wait revalidation; the
         * guest cannot select it. Revalidation still checks every permission.
         * @details A refusal has no provider effect. A successful reservation
         * is not refunded when the provider later fails; retries cost another
         * admission. This fixed-window policy explicitly permits edge bursts.
         * Grants without attenuation still perform every authorization check
         * and increment admissions, but do not sample an unused quota clock.
         */
        error admit(caller who,handle id,std::string_view action,std::uint64_t bytes,lease& output,bool charge=true)
        {
            if(action=="artifact.publish")return admit_publish_wire(who,id,bytes,output,charge,false);
            return admit_checked<false>(who,id,action,0,bytes,output,charge);
        }
        /** @brief Check exact authority and available attenuation before host work.
         * @details This trusted preflight never reserves capacity, refreshes a
         * window or changes admission/refusal counters. An elapsed window is
         * evaluated as empty without modifying it. A successful result is not
         * a promise of later admission: after owned request copying, admit()
         * must repeat every check and atomically charge before any effect.
         */
        error preflight(caller who,handle id,std::string_view action,std::uint64_t bytes,lease& output)
        {
            if(action=="artifact.publish")return admit_publish_wire(who,id,bytes,output,false,true);
            return admit_checked<false>(who,id,action,0,bytes,output,false,true);
        }
        /** @brief Admit a registered-message operation using exact frozen action bits.
         * @details The trusted wrapper supplies the checked enum. Mapping is
         * derived from exact deployed names; unknown values fail before shifts.
         * Membership, exposure, revocation, handles, quota and lease locking use
         * the same implementation as string admission. Preflight observes the
         * budget without changing it and final admission still rechecks it.
         * Cleanup callers explicitly disable charging, never authorization.
         */
        error admit(caller who,handle id,module_exchange::operation action,std::uint64_t bytes,lease& output,bool charge=true,bool check_budget=false)
        {
            auto index=static_cast<std::size_t>(action);
            if(index>=module_exchange::names.size()){output.release();return error::unsupported;}
            return admit_checked<true>(who,id,{},std::uint64_t{1}<<(exchange_offset+index),bytes,output,charge,check_budget);
        }
        /** @brief Admit an exact, checked local operation using frozen policy indices.
         * @details Each bit is derived only from the exact runtime descriptor
         * and exact configured name. No prefix, fallback, ordinal truncation
         * or guest-supplied mask is accepted. Unknown enum values release any
         * previous lease and return unsupported before a shift. This path uses
         * the same caller, revocation, quota and lease checks as string actions;
         * it removes only repeated string-tree lookup after policy freezing.
         */
        error admit(caller who,handle id,runtime_contract::operation action,std::uint64_t bytes,lease& output,bool charge=true)
        {
            auto index=static_cast<std::size_t>(action);
            if(index>=runtime_contract::operations.size()){output.release();return error::unsupported;}
            return admit_checked<true>(who,id,{},std::uint64_t{1}<<index,bytes,output,charge);
        }
        /** @brief Admit a checked artifact enum using exact frozen action bits.
         * @details The mask is derived only from the deployed exact spelling;
         * unknown enum values fail before shifting. All caller, revocation and
         * quota checks are identical to string admission, without tree lookup.
         */
        error admit(caller who,handle id,artifact_contract::operation action,std::uint64_t bytes,lease& output,bool charge=true)
        {
            auto index=static_cast<std::size_t>(action);
            if(index>=artifact_contract::operations.size()){output.release();return error::unsupported;}
            return admit_checked<true>(who,id,{},std::uint64_t{1}<<(artifact_offset+index),bytes,output,charge);
        }
        /** @brief Admit or preflight an exact cache operation without string lookup.
         * @details A trusted budget preflight observes but does not alter the
         * rate window or counters. It is not a reservation; final admission
         * repeats all checks and charges before any provider effect.
         */
        error admit(caller who,handle id,output_contract::operation action,std::uint64_t bytes,lease& output,bool charge=true,bool check_budget=false)
        {
            auto index=static_cast<std::size_t>(action);
            if(index>=output_contract::operations.size()){output.release();return error::unsupported;}
            if(action==output_contract::operation::publish)return admit_publish_wire(who,id,bytes,output,charge,check_budget);
            return admit_checked<true>(who,id,{},std::uint64_t{1}<<(output_offset+index),bytes,output,charge,check_budget);
        }
        /** @brief Check a second resource grant within an already admitted call.
         * @details This trusted-only entry does not expose a guest interface or
         * debit a second call budget. It checks live membership, the exact
         * handle and action. The initiating operation's interface and budget
         * must already be admitted. Holding both leases prevents revocation
         * between dependency validation and resource attachment.
         */
        error dependency(caller who,handle id,std::string_view action,lease& output)
        {
            output.release();std::unique_lock lock{mutex};if(!member(who))return error::denied;
            auto it=handles.find(id);if(it==handles.end())return error::bad_handle;
            if(it->second->revoked||!it->second->policy.actions.contains(action))return error::denied;
            output.value=&it->second->policy;output.lock=std::move(lock);return error::success;
        }
        /** @brief Close a capability handle only; persistent resource lifetime is explicit. */
        error close(caller who,handle id)
        {
            std::lock_guard lock{mutex};if(!member(who)||visible_locked("capability.close")!=exposure::allow)return error::denied;
            return erase_handle(id)?error::success:error::bad_handle;
        }
        /** @brief Trusted rollback when a newly opened handle was not delivered.
         * @details This is never a guest import and is only used for the exact
         * unpublished handle returned by open, independent of close exposure.
         */
        void abandon_open(handle id){std::lock_guard lock{mutex};erase_handle(id);}
        /** @brief Check exact self-revocation authority without charging traffic.
         * @details An already revoked account is reachable only for its original
         * caller/capability receipt. The later revoke_granted call compares the
         * complete fixed request before returning or changing anything.
         */
        error preflight_revoke(caller who,handle cap)
        {
#if defined(__linux__)
            if(revocation_process!=::getpid())return error::denied;
#endif
            std::lock_guard lock{mutex};
            if(!member(who)||visible_locked("capability.revoke")!=exposure::allow)return error::denied;
            auto found=handles.find(cap);if(found==handles.end())return error::bad_handle;
            auto const& value=*found->second;
            if(!value.policy.actions.contains("capability.revoke"))return error::denied;
            if(value.revoked&&(!value.revocation.identity||value.revocation.target!=cap||
                value.revoker.instance!=who.instance||value.revoker.generation!=who.generation))return error::denied;
            return error::success;
        }
        /** @brief Atomically fence one explicitly self-revocable grant and retain its receipt.
         * @details No rate check, allocation, wait or authority expansion occurs.
         * The trusted retirement callback follows authority -> resource lock
         * order and must be bounded/nonblocking. Callback failure leaves the
         * grant permanently fenced with pending retirement; it is never reported
         * as physical/native completion. Exact retry never repeats notifications.
         * A reentrant retry sees the reserved pending receipt, not an empty slot.
         */
        template<typename Retire> error revoke_granted(caller who,handle cap,std::uint64_t key,handle target,
                                                       revocation_contract::receipt& output,Retire&& notify)
        {
#if defined(__linux__)
            if(revocation_process!=::getpid())return error::denied;
#endif
            std::lock_guard lock{mutex};auto status=preflight_revoke(who,cap);if(status!=error::success)return status;
            if(!key||!target)return error::invalid;
            if(target!=cap)return error::denied;
            auto& value=*handles.find(cap)->second;
            if(value.revocation.identity)
            {
                if(value.revocation.key!=key||value.revocation.target!=target||value.revoker.instance!=who.instance||
                    value.revoker.generation!=who.generation)return error::conflict;
                output=value.revocation;return error::success;
            }
            auto identity=resources::allocate_identity();if(!identity)return error::too_large;
            value.revocation={key,target,identity,revocation_contract::retirement::pending};value.revoker=who;
            value.revoked=true;increment(value.admissions);
            try{notify(std::string_view{value.policy.alias});value.revocation.state=revocation_contract::retirement::notified;}
            catch(...){/* The irreversible fence and pending receipt survive. */}
            output=value.revocation;return error::success;
        }
        /** @brief Revoke exact alias and run retirement while excluding new effects.
         * @details The callback must be nonblocking and follow authority ->
         * resource lock order. It must wake waiters and retire source resources.
         * Revocation cannot be undone; new deployment requires a new environment.
         */
        template<typename Retire> error revoke(std::string_view alias,Retire&& retire_resources)
        {
            std::lock_guard lock{mutex};auto i=grants.find(alias);if(i==grants.end())return error::bad_handle;
            i->second.revoked=true;retire_resources(alias);return error::success;
        }
        /** @brief Irreversibly fence a complete provider fault domain in one authority transaction.
         * @details This host-only operation grants nothing and allocates nothing.
         * Every exact alias must exist before any account changes. All accounts
         * are denied before the nonthrowing retirement callback runs under the
         * authority lock. The callback must follow authority -> resource order,
         * retain uncertain effects and keep the independently reserved stop lane.
         * Restarting an executable cannot undo these tombstones; a replacement
         * requires fresh deployment authority and fresh resource generations.
         */
        template<typename Retire> error revoke_group(std::span<std::string_view const> aliases,Retire&& retire_resources)
        {
            static_assert(noexcept(retire_resources()));
#if defined(__linux__)
            if(revocation_process!=::getpid())return error::denied;
#endif
            if(aliases.empty()||aliases.size()>maximum_admission_claims)return error::invalid;
            std::lock_guard lock{mutex};
            for(auto alias:aliases)if(!grants.contains(alias))return error::bad_handle;
            for(auto alias:aliases)grants.find(alias)->second.revoked=true;
            retire_resources();return error::success;
        }
        /** @brief Permanently retire one runtime incarnation without allocating for a live member.
         * @details The existing membership node becomes a tombstone. A trusted
         * retirement received before enrollment must still prevent subsequent
         * enrollment; if its tombstone cannot be allocated, this authority
         * becomes permanently denied instead of retaining an ambiguous member.
         * Existing wait cancellation checks observe that denial on their next
         * bounded wake. No provider effects occur during this transition.
         */
        void retire_member(caller who)
        {
            std::lock_guard lock{mutex};auto current=members.extract(who);
            if(!current.empty())retired_members.insert(std::move(current));
            else if(!retired_members.contains(who))
            {
                try{retired_members.insert(who);}
                catch(std::bad_alloc const&){retired=true;}
            }
        }
        template<typename Retire> void retire(Retire&& retire_resources)
        {
            std::lock_guard lock{mutex};retired=true;members.clear();handles.clear();
            for(auto& [alias,a]:grants)a.open_handles=0;
            retire_resources();
        }
        /** @brief Bounded metadata-only inspection; never refreshes quota state.
         * @details At most maximum grant records are visited. Handle totals are
         * maintained at mutation, so inspection work is independent of the
         * number of guest-opened handles. Busy/oversized output stays unchanged.
         */
        bool inspect(::uwvm2::utils::container::std_vector<authority_snapshot>& output,std::size_t maximum)
        {
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock()||grants.size()>maximum)return false;
            ::uwvm2::utils::container::std_vector<authority_snapshot> rows;rows.reserve(grants.size());
            for(auto const& [alias,a]:grants)
            {
                rows.push_back({alias,a.policy.resource,a.policy.binding,a.policy.kind,a.revoked,
                    a.policy.limits.calls.has_value()||a.policy.limits.bytes.has_value(),a.open_handles,a.admissions,a.refused});
            }
            output=std::move(rows);return true;
        }
        /** @brief Observe complete grant policy without opening handles or refreshing quotas.
         * @details The observer has no admission lease and receives no account
         * reference. Busy state returns again. Values are copied while holding
         * the authority lock; no callback, clock read or resource operation is
         * executed. Live attenuation and revocation are reported as stored.
         */
        error report_permissions(permission_report::page& out)const
        {
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(!out.add("environment","retired",retired))return error::too_large;
            for(auto const& [alias,entry]:grants)
            {
                auto const& spec=entry.policy;
                if(!out.add(alias,"purpose",static_cast<unsigned>(spec.kind),purpose_names[static_cast<std::size_t>(spec.kind)])||
                   !out.add(alias,"resource",0,spec.resource)||!out.add(alias,"binding",0,spec.binding)||
                   !out.add(alias,"revoked",entry.revoked)||!out.add(alias,"max-handles",spec.max_handles)||
                   !out.add(alias,"max-resources",spec.max_resources)||!out.add(alias,"max-wait-members",spec.max_wait_members)||
                   !out.add(alias,"max-wait-ns",spec.max_wait_ns)||!out.add(alias,"infinite-wait",spec.infinite_wait)||
                   !out.add(alias,"calls-per-window",spec.limits.calls.value_or(UINT64_MAX))||
                   !out.add(alias,"bytes-per-window",spec.limits.bytes.value_or(UINT64_MAX))||
                   !out.add(alias,"window-ns",static_cast<std::uint64_t>(spec.limits.window.count()))||
                   !out.add(alias,"may-reject",spec.limits.uncertain)||!out.add(alias,"open-handles",entry.open_handles)||
                   !out.add(alias,"admissions",entry.admissions)||!out.add(alias,"refused",entry.refused))return error::too_large;
                for(auto const& action:spec.actions)if(!out.add(alias,"action",!retired&&!entry.revoked,action))return error::too_large;
            }
            return error::success;
        }
        /** @brief Check a registered observation grant without admission accounting.
         * @details This reads an immutable alias plus its live revocation bit.
         * It never returns a capability or charges, refreshes or resets limits.
         */
        error observes(std::string_view alias,std::string_view action,purpose kind,lease& held)const
        {
            held.release();
            if(kind!=purpose::module_inspection&&kind!=purpose::vm_inspection)return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            auto it=grants.find(alias);
            if(retired||it==grants.end()||it->second.revoked||it->second.policy.kind!=kind||
                !it->second.policy.actions.contains(action))return error::denied;
            held.value=&it->second.policy;held.lock=std::move(lock);return error::success;
        }
    };
    inline group_lease::operator bool() const noexcept
    {return native_thread==std::this_thread::get_id()&&lock.owns_lock()&&count&&issuer&&issuer->group_current(*this);}
    inline bool group_lease::matches(caller who,std::span<admission_claim const> claims) const noexcept
    {
        if(who.instance!=admitted_caller.instance||who.generation!=admitted_caller.generation||claims.size()!=count||!static_cast<bool>(*this))return false;
        for(std::size_t i{};i<count;++i)
        {
            auto const& expected=claims[i];auto const& actual=accepted[i];
            if(expected.capability!=actual.capability||expected.action!=actual.action||expected.bytes!=actual.bytes||
               expected.grant_only!=actual.grant_only||expected.validation_only!=actual.validation_only||expected.cleanup_only!=actual.cleanup_only)return false;
        }
        return true;
    }
}
