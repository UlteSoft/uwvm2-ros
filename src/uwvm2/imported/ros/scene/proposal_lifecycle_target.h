/** @file
 * @brief Strict inert proposal validation against an actual local lifecycle registry.
 * @details This adapter binds only the existing local-lifecycle state graph.
 * It never submits a transition, consumes a callback or extends a deadline.
 * Native ROS nodes, actuators and arbitrary lifecycle bridges are unsupported.
 * The immutable tool is explicitly non_physical: a future physical lifecycle
 * adapter needs independent device/control/source fencing and may not reuse it.
 *
 * Arguments are exactly the lifecycle.submit record: handle, key, transition,
 * expected_generation, absolute_steady_deadline. The whole authenticated model
 * envelope binds the target handle/profile. Admission checks current cached
 * state, the next submission key, the exact transition mask and finite deadline.
 * A returned proposal receipt remains inert; real execution independently invokes
 * lifecycle.submit under its current capability and repeats every registry check.
 * All binding/validation storage is fixed; no operation allocates or performs I/O.
 */
#pragma once
#include "managed_lifecycle.h"
#include "proposal_dispatch.h"
namespace uwvm2::imported::ros::scene
{
    class proposal_lifecycle_target
    {
    public:
        inline static constexpr std::string_view argument_type="ros.lifecycle.submit",encoding="le64",frame="logical";
        inline static constexpr std::size_t maximum_bindings=32;
        struct configuration
        {buffers::text<255> alias;buffers::profile profile;managed_lifecycle::plan bounds;};
    private:
        struct binding{configuration policy;handle owner{},provider{};bool retired{};};
        struct arguments{handle target{},key{},transition{},generation{},deadline{};};
        std::shared_ptr<authority> permissions_;
        managed_lifecycle::registry& nodes_;
        handle environment_{},generation_{};
        std::uint64_t(*clock_)()noexcept;
        mutable std::mutex mutex_;
        std::array<binding,maximum_bindings> bindings_{};
        std::size_t count_{};
        bool retired_{};
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
        static std::uint64_t steady()noexcept{auto value=resources::steady_now();return value>0?static_cast<std::uint64_t>(value):0;}
        static bool decode(wire::bytes input,arguments& output)noexcept
        {
            wire::reader reader{input};arguments value;
            if(!reader.u64(value.target)||!value.target||!reader.u64(value.key)||!value.key||!reader.u64(value.transition)||
                !value.transition||value.transition>7||!reader.u64(value.generation)||!value.generation||!reader.u64(value.deadline)||
                !value.deadline||value.deadline>INT64_MAX||!reader.done())return false;
            output=value;return true;
        }
        static bool valid_arguments(wire::bytes bytes)noexcept{arguments out;return decode(bytes,out);}
        static bool same(managed_lifecycle::plan const& a,managed_lifecycle::plan const& b)noexcept
        {return a.max_handles==b.max_handles&&a.max_deadline_ns==b.max_deadline_ns&&a.transitions_mask==b.transitions_mask&&a.roles_mask==b.roles_mask;}
        binding const* find(proposals::tool_rule const& rule)const noexcept
        {
            if(retired_||!proposals::valid(rule)||rule.action!=proposal_contract::target_action::lifecycle_submit||rule.effect!=proposals::admission::effect_class::non_physical||
                rule.argument_type.view()!=argument_type||rule.encoding.view()!=encoding||rule.frame.view()!=frame||rule.schema!=argument_schema())return nullptr;
            for(std::size_t i{};i<count_;++i)
            {auto const& row=bindings_[i];if(!row.retired&&row.policy.alias==rule.target_alias&&row.policy.profile==rule.target_profile)return &row;}
            return nullptr;
        }
        proposal_dispatch::target_evidence evidence(binding const& value,handle resource,managed_lifecycle::submission_observation const& observed)const noexcept
        {
            return {environment_,value.owner,generation_,resource,value.provider,observed.current.generation,0,0,
                value.policy.profile,proposals::admission::effect_class::non_physical,valid_arguments};
        }
        static bool equal(proposal_dispatch::target_evidence const& a,proposal_dispatch::target_evidence const& b)noexcept
        {
            return a.environment==b.environment&&a.owner==b.owner&&a.owner_generation==b.owner_generation&&a.resource==b.resource&&
                a.provider_generation==b.provider_generation&&a.authority_epoch==b.authority_epoch&&a.physical_scope==b.physical_scope&&
                a.controller_epoch==b.controller_epoch&&a.profile==b.profile&&a.effect==b.effect&&a.validate_arguments==b.validate_arguments;
        }
        static error resolve_adapter(void* context,proposals::tool_rule const& rule,handle resource,proposal_dispatch::target_evidence& out)
        {return static_cast<proposal_lifecycle_target*>(context)->resolve(rule,resource,out);}
        static error validate_adapter(void* context,caller who,handle cap,proposals::tool_rule const& rule,
                                      proposal_dispatch::target_evidence const& actual,wire::bytes arguments,
                                      std::span<proposals::admission::observation const> observations)
        {return static_cast<proposal_lifecycle_target*>(context)->validate(who,cap,rule,actual,arguments,observations);}
    public:
        explicit proposal_lifecycle_target(std::shared_ptr<authority> permissions,managed_lifecycle::registry& nodes,handle environment,handle generation,
                                           std::uint64_t(*clock)()noexcept=steady)
            :permissions_(std::move(permissions)),nodes_(nodes),environment_(environment),generation_(generation),clock_(clock?clock:steady){}
        /** @brief Exact argument profile; fields and allowed public ROS transition IDs are fixed. */
        static buffers::profile argument_schema()noexcept
        {
            buffers::profile result;result.identity.assign(argument_type);
            constexpr std::string_view definition="uwvm.ros.local-lifecycle.submit:le64(handle>0,key>0,transition=1..7,expected_generation>0,steady_deadline=1..INT64_MAX)";
            result.sha256=buffers::hash({reinterpret_cast<std::uint8_t const*>(definition.data()),definition.size()});return result;
        }
        /** @brief Bind the profile digest to its identity, exact alias and all actual limits. */
        static buffers::digest profile_digest(std::string_view identity,std::string_view alias,managed_lifecycle::plan const& limits)noexcept
        {
            std::array<std::uint8_t,512> storage;wire::writer out{storage};
            if(!proposals::envelope_text(identity,96)||!identifier(alias)||!managed_lifecycle::valid(limits))return {};
            if(!out.text("uwvm.ros.local-lifecycle.target")||!out.text(identity)||!out.text(alias)||!out.u64(limits.max_handles)||
                !out.u64(limits.max_deadline_ns)||!out.u64(limits.transitions_mask)||!out.u64(limits.roles_mask))return {};
            return buffers::hash(out.written());
        }
        error bind(configuration const& policy)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(retired_||!permissions_||!environment_||!generation_)return error::denied;
            if(!proposals::policy_text(policy.alias,true)||!proposals::envelope_profile(policy.profile)||!managed_lifecycle::valid(policy.bounds)||
                !(policy.bounds.roles_mask&managed_lifecycle::client)||
                policy.profile.sha256!=profile_digest(policy.profile.identity.view(),policy.alias.view(),policy.bounds))return error::invalid;
            for(std::size_t i{};i<count_;++i)if(bindings_[i].policy.alias==policy.alias)return error::conflict;
            if(count_==maximum_bindings)return error::too_large;
            auto owner=resources::allocate_identity(),provider=resources::allocate_identity();if(!owner||!provider)return error::too_large;
            bindings_[count_++]={policy,owner,provider,false};return error::success;
        }
        proposal_dispatch::target_access access()noexcept{return {this,resolve_adapter,validate_adapter};}
        /** @brief Resolve an actual current client resource without observing/expiring its clock. */
        error resolve(proposals::tool_rule const& rule,handle resource,proposal_dispatch::target_evidence& output)const noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto row=find(rule);if(!row)return error::denied;
            managed_lifecycle::submission_observation observed;auto status=nodes_.observe_submission(row->policy.alias.view(),resource,observed);
            if(status!=error::success)return status;
            if(!same(row->policy.bounds,observed.bounds))return error::conflict;
            output=evidence(*row,resource,observed);return error::success;
        }
        /** @brief Validate an inert transition proposal under exact current target authority.
         * @details This performs no submission, expiration, callback, quota debit
         * or permission widening. The caller's proposal admission owns its own
         * optional budget. Source proofs are checked by proposal_dispatch before
         * this typed adapter; this target cannot invent missing observations.
         */
        error validate(caller who,handle cap,proposals::tool_rule const& rule,proposal_dispatch::target_evidence const& actual,
                       wire::bytes input,std::span<proposals::admission::observation const> observations)const noexcept
        {
            if(!same_process()||!permissions_)return error::denied;
            arguments wanted;if(!decode(input,wanted)||wanted.target!=actual.resource)return error::invalid;
            auto now=clock_();
            lease held;auto status=permissions_->admit(who,cap,"lifecycle.submit",0,held,false);if(status!=error::success)return status;
            auto const& grant=held.specification();if(grant.kind!=purpose::lifecycle||grant.binding!="local-lifecycle"||grant.alias!=rule.target_alias.view())return error::denied;
            std::lock_guard lock{mutex_};auto row=find(rule);if(!row)return error::denied;
            managed_lifecycle::submission_observation observed;status=nodes_.observe_submission(row->policy.alias.view(),wanted.target,observed);
            if(status!=error::success)return status;
            if(!same(row->policy.bounds,observed.bounds)||!equal(actual,evidence(*row,wanted.target,observed)))return error::conflict;
            if(wanted.generation!=observed.current.generation||wanted.key!=observed.next_key)return error::conflict;
            if(!now||now>INT64_MAX||wanted.deadline<=now||wanted.deadline-now>observed.bounds.max_deadline_ns||
                (observed.current.request&&now>=observed.current.deadline))return error::timeout;
            if(observed.current.request)return error::again;
            if(!(observed.allowed&(std::uint64_t{1}<<wanted.transition)))return error::denied;
            if(observations.size()<rule.minimum_observations||observations.size()>8)return error::denied;
            return error::success;
        }
        void revoke(std::string_view alias)noexcept
        {if(!same_process())return;std::lock_guard lock{mutex_};for(std::size_t i{};i<count_;++i)if(bindings_[i].policy.alias.view()==alias)bindings_[i].retired=true;}
        void retire()noexcept{if(!same_process())return;std::lock_guard lock{mutex_};retired_=true;}
    };
}
