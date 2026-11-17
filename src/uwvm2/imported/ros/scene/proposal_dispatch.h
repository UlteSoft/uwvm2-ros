/** @file
 * @brief Independent proposal disclosure and inert task admission under complete authority.
 * @details Native model output cannot create a grant or invoke a robot target.
 * Deployment binds exact profiles and typed, side-effect-free source/target
 * adapters. Captured input tokens retain actual sealed allocations and finite
 * slots; complete model final bytes are authenticated before strict envelope
 * decoding. Acceptance returns admitted/locally_accepted historical data only.
 * Actual execution always needs a separate current target transaction.
 */
#pragma once
#include "proposal_policy.h"
#include "proposal_codec.h"
#include "proposal_workspace.h"
#include <uwvm2/utils/container/function_ref.h>

namespace uwvm2::imported::ros::scene
{
    class proposal_dispatch
    {
    public:
        using operation=proposal_contract::operation;
        using bytes=wire::bytes;
        using sink=::uwvm2::utils::container::function_ref<bool(bytes)>;
        using cancellation=::uwvm2::utils::container::function_ref<bool()>;
        using publication_admission=::uwvm2::utils::container::function_ref<error()>;
        struct limits {std::size_t proposals{},arbiters{};};
        /** @brief Real target provenance returned only by a typed host provider.
         * @details resolve is observational and cannot open/execute a target.
         * The argument validator is a trusted registered schema function, never
         * a symbol selected by configuration or model output.
         */
        struct target_evidence
        {
            handle environment{},owner{},owner_generation{},resource{},provider_generation{},authority_epoch{},physical_scope{},controller_epoch{};
            buffers::profile profile;
            proposals::admission::effect_class effect{};
            bool (*validate_arguments)(bytes) noexcept{};
        };
        struct target_access
        {
            void* context{};
            error (*resolve)(void*,proposals::tool_rule const&,handle,target_evidence&){};
            error (*validate)(void*,caller,handle,proposals::tool_rule const&,target_evidence const&,bytes,
                              std::span<proposals::admission::observation const>){};
        };
        /** @brief Current host source sample, not a guest-writable provenance object. */
        struct source_sample
        {
            handle environment{},owner{},owner_generation{};
            buffers::profile profile;
            buffers::text<96> frame;
            proposals::admission::observation observation;
        };
        struct source_access
        {
            void* context{};
            error (*capture)(void*,caller,handle,handle,proposals::source_rule const&,source_sample&){};
            error (*validate)(void*,caller,handle,proposals::source_rule const&,source_sample const&,
                              proposals::admission::observation const&,proposals::admission::observation_policy){};
        };
        /** @brief Explicit caller source and buffer rights; no capability is opened implicitly.
         * @details The source facade supplies the real sealed buffer identity.
         * buffer_capability is independently checked against its exact configured
         * buffer alias/profile; it supplies authority, never source provenance.
         */
        struct source_reference {std::size_t index{};handle capability{},resource{},buffer_capability{};};
        struct usage
        {std::uint64_t proposals{},readers{},sources{},tools{},captures{},workspace_bytes{},workspace_calls{};bool retired{};};
        using workspace_lease=proposals::workspace::lease;
    private:
        struct reader_state
        {
            caller who;handle capability{},actor{},scope{};
            bool arbiter{},released{};
            buffers::text<255> arbiter_alias;
            proposals::admission::receipt last;
        };
        struct source_state
        {handle id{},capability{},frame{};source_sample current;};
        struct tool_state
        {handle id{},profile{},schema{},frame{},target_scope{};std::size_t target_index{};proposals::tool_profile selected;};
        struct target_state {handle scope{};bool attached{};target_evidence current;};
        struct binding
        {
            ::uwvm2::utils::container::std_string alias;
            proposals::plan policy;
            proposals::provider provider;
            proposals::workspace workspace;
            handle scope{},producer{};
            std::array<reader_state,8> readers{};
            std::array<source_state,16> sources{};
            std::array<tool_state,8> tools{};
            std::array<target_state,8> targets{};
            std::array<buffers::text<96>,24> frame_names{};
            std::array<handle,24> frame_ids{};
            std::size_t frame_count{};
            std::atomic<std::uint64_t> captures{};
            bool busy{},retired{};
            explicit binding(std::uint64_t(*clock)() noexcept):provider(clock){}
        };
        struct arbiter_binding
        {::uwvm2::utils::container::std_string alias;proposals::arbiter_plan policy;std::shared_ptr<binding> source;handle target_scope{};bool retired{};};
        struct capture_slot
        {
            std::shared_ptr<binding> owner;
            capture_slot()=default;capture_slot(capture_slot const&)=delete;capture_slot& operator=(capture_slot const&)=delete;
            capture_slot(capture_slot&&)=default;capture_slot& operator=(capture_slot&&)=delete;
            ~capture_slot(){if(owner)owner->captures.fetch_sub(1,std::memory_order_release);}
        };
        struct capture_record
        {
            capture_slot slot; // Last: source pins/storage, workspace, then retained capture slot.
            workspace_lease reservation;
            std::array<proposals::provider::snapshot_token,8> inputs{};
            caller who;
            std::size_t count{};
        };
    public:
        /** @brief Opaque shared capture; copies retain one counted physical record. */
        class captured_inputs
        {
            friend class proposal_dispatch;
            std::shared_ptr<capture_record> value;
        public:
            explicit operator bool() const noexcept{return bool(value);}
            std::size_t size() const noexcept{return value?value->count:0;}
            handle buffer(std::size_t index) const noexcept{return value&&index<value->count?value->inputs[index].buffer():0;}
            handle capability(std::size_t index) const noexcept
            {return value&&index<value->count&&value->inputs[index].value?value->inputs[index].value->metadata.capability:0;}
            void reset() noexcept{value.reset();}
        };
    private:
        std::shared_ptr<authority> permissions_;
        model_dispatch& model_;
        buffers::registry& buffers_;
        handle environment_{},generation_{};
        limits limits_;
        source_access source_;
        target_access target_;
        std::uint64_t(*clock_)() noexcept;
        mutable std::mutex mutex_;
        ::uwvm2::utils::container::std_vector<std::shared_ptr<binding>> bindings_;
        ::uwvm2::utils::container::std_vector<arbiter_binding> arbiters_;
        bool ready_{},retired_{};
#if defined(__linux__)
        ::pid_t process_{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process_==::getpid();
#else
            return true;
#endif
        }
        static bool same(caller a,caller b) noexcept{return !(a<b)&&!(b<a);}
        static error convert(proposals::result status) noexcept
        {
            using R=proposals::result;
            switch(status)
            {
            case R::success:return error::success;case R::denied:return error::denied;case R::invalid:return error::invalid;
            case R::bad_handle:return error::bad_handle;case R::conflict:return error::conflict;case R::again:return error::again;
            case R::too_large:return error::too_large;case R::timeout:return error::timeout;case R::cancelled:return error::cancelled;
            case R::delivery_failed:return error::io;
            }
            return error::io;
        }
        static proposals::result convert(error status) noexcept
        {
            using R=proposals::result;
            switch(status)
            {
            case error::success:return R::success;case error::denied:return R::denied;case error::invalid:return R::invalid;
            case error::bad_handle:return R::bad_handle;case error::conflict:return R::conflict;case error::again:return R::again;
            case error::too_large:return R::too_large;case error::timeout:return R::timeout;case error::cancelled:return R::cancelled;
            default:return R::delivery_failed;
            }
        }
        std::shared_ptr<binding> named(std::string_view alias) const noexcept
        {for(auto const& value:bindings_)if(value->alias==alias) { return value; }return {};}
        arbiter_binding* arbiter(std::string_view alias) noexcept
        {for(auto& value:arbiters_)if(value.alias==alias) { return &value; }return nullptr;}
        error resolve(grant const& grant,operation action,std::shared_ptr<binding>& output,arbiter_binding*& arbitration)
        {
            std::lock_guard lock{mutex_};arbitration=nullptr;if(!ready_||retired_) { return error::denied; }
            if(action==operation::read)
            {
                if(grant.kind!=purpose::proposal||grant.binding!="model-proposals") { return error::denied; }
                output=named(grant.alias);
            }
            else
            {
                if(grant.kind!=purpose::arbiter||grant.binding!="proposal-arbiter") { return error::denied; }
                arbitration=arbiter(grant.alias);if(!arbitration||arbitration->retired) { return error::denied; }output=arbitration->source;
            }
            return !output||output->retired?error::denied:error::success;
        }
        error live(binding const& value) const
        {std::lock_guard lock{mutex_};return retired_||value.retired?error::cancelled:error::success;}
        handle frame(binding& value,buffers::text<96> const& name)
        {
            for(std::size_t i{};i<value.frame_count;++i)if(value.frame_names[i]==name) { return value.frame_ids[i]; }
            if(value.frame_count==value.frame_ids.size()) { return 0; }
            auto id=resources::allocate_identity();if(!id) { return 0; }auto index=value.frame_count++;
            value.frame_names[index]=name;value.frame_ids[index]=id;return id;
        }
        error enter(binding& value)
        {std::lock_guard lock{mutex_};if(retired_||value.retired) { return error::cancelled; }if(value.busy) { return error::again; }value.busy=true;return error::success;}
        struct finish_call
        {proposal_dispatch& self;binding& selected;~finish_call(){std::lock_guard lock{self.mutex_};selected.busy=false;}};
        error reader(binding& value,caller who,handle capability,arbiter_binding const* arbiter,reader_state*& output)
        {
            std::lock_guard lock{mutex_};if(retired_||value.retired||(arbiter&&arbiter->retired)) { return error::cancelled; }
            handle actor{};reader_state* available{};
            for(std::size_t i{};i<value.policy.storage.readers;++i)
            {
                auto& item=value.readers[i];if(item.capability&&same(item.who,who))actor=item.actor;
                if(item.capability==capability)
                {if(!same(item.who,who)||item.arbiter!=bool(arbiter)) { return error::denied; }output=&item;return error::success;}
                if(!item.capability&&!available)available=&item;
            }
            if(!available) { return error::too_large; }
            if(!actor) { actor=resources::allocate_identity(); }
            auto scope=resources::allocate_identity();if(!actor||!scope) { return error::too_large; }
            proposals::admission::grant access;access.actor=actor;access.capability=scope;access.proposal_capability=value.scope;
            access.authority_epoch=generation_;access.read=true;access.accept=bool(arbiter);
            if(arbiter)
            {
                access.target_capability=arbiter->target_scope;
                for(std::size_t i{};i<value.policy.tool_count;++i)if(arbiter->policy.tool_mask&(std::uint64_t{1}<<i))
                    access.target_actions|=proposal_contract::action_bit(value.policy.tools[i].action);
            }
            handle id{};auto status=convert(value.provider.bind_reader(access,capability,id));if(status!=error::success) { return status; }
            *available={who,capability,actor,scope,bool(arbiter),false,{},{}};
            if(arbiter)available->arbiter_alias.assign(arbiter->alias);
            output=available;return error::success;
        }
        static purpose target_kind(proposal_contract::target_action action) noexcept
        {
            using A=proposal_contract::target_action;
            switch(action)
            {
            case A::device_submit:return purpose::device;case A::control_submit:return purpose::control;
            case A::topic_publish:return purpose::topic;case A::service_submit:return purpose::service;case A::action_submit:return purpose::action;
            case A::parameter_set:case A::parameter_set_atomic:return purpose::parameter;case A::lifecycle_submit:return purpose::lifecycle;
            }
            return purpose::arbiter;
        }
        bool target_valid(proposals::tool_rule const& rule,handle resource,target_evidence const& value) const noexcept
        {
            using C=proposals::admission::effect_class;
            return value.environment==environment_&&value.owner&&value.owner_generation==generation_&&value.resource==resource&&resource&&
                value.profile==rule.target_profile&&value.provider_generation&&value.authority_epoch&&value.effect==rule.effect&&value.validate_arguments&&
                (rule.effect==C::physical?(value.physical_scope&&value.controller_epoch):(!value.physical_scope&&!value.controller_epoch));
        }
        static bool same_target(target_evidence const& a,target_evidence const& b) noexcept
        {
            return a.environment==b.environment&&a.owner==b.owner&&a.owner_generation==b.owner_generation&&a.resource==b.resource&&a.profile==b.profile&&
                a.provider_generation==b.provider_generation&&a.authority_epoch==b.authority_epoch&&a.physical_scope==b.physical_scope&&a.controller_epoch==b.controller_epoch&&a.effect==b.effect;
        }
        error attach_tool(binding& value,std::size_t index,handle resource)
        {
            auto const& rule=value.policy.tools[index];target_evidence actual;
            auto status=target_.resolve(target_.context,rule,resource,actual);if(status!=error::success) { return status; }
            if(!target_valid(rule,resource,actual)) { return error::denied; }
            std::lock_guard lock{mutex_};if(retired_||value.retired) { return error::cancelled; }
            auto& tool=value.tools[index];auto& target=value.targets[tool.target_index];
            if(tool.id&&tool.selected.validate_arguments!=actual.validate_arguments) { return error::conflict; }
            if(target.attached)
            {
                auto const& old=target.current;
                if(old.resource!=actual.resource||old.owner!=actual.owner||old.physical_scope!=actual.physical_scope||old.effect!=actual.effect||
                   actual.provider_generation<old.provider_generation||actual.authority_epoch<old.authority_epoch||actual.controller_epoch<old.controller_epoch) { return error::conflict; }
                if(!same_target(old,actual))
                {
                    for(std::size_t i{};i<value.policy.tool_count;++i)if(value.tools[i].id&&value.tools[i].target_index==tool.target_index)
                    {
                        auto& changed=value.tools[i];status=convert(value.provider.fence_target(changed.id,actual.provider_generation,actual.authority_epoch,actual.controller_epoch));
                        if(status!=error::success) { return status; }
                        changed.selected.semantics.provider_generation=actual.provider_generation;changed.selected.semantics.target_authority_epoch=actual.authority_epoch;
                        changed.selected.semantics.controller_lease_epoch=actual.controller_epoch;
                    }
                }
            }
            target.attached=true;target.current=actual;
            if(tool.id)
            {return tool.selected.validate_arguments==actual.validate_arguments?error::success:error::conflict;}
            proposals::tool_profile selected;selected.profile=rule.profile;selected.schema=rule.schema;selected.argument_type=rule.argument_type;selected.encoding=rule.encoding;
            selected.validate_arguments=actual.validate_arguments;auto& semantics=selected.semantics;
            semantics.profile=tool.profile;semantics.schema=tool.schema;semantics.target_capability=tool.target_scope;semantics.target=resource;
            semantics.provider_generation=actual.provider_generation;semantics.frame=tool.frame;semantics.operation=rule.action;semantics.max_observation_age_ns=rule.max_observation_age_ns;
            semantics.effect=rule.effect;semantics.target_authority_epoch=actual.authority_epoch;semantics.physical_scope=actual.physical_scope;
            semantics.controller_lease_epoch=actual.controller_epoch;semantics.minimum_observations=static_cast<std::size_t>(rule.minimum_observations);semantics.observations=rule.observations;
            handle id{};status=convert(value.provider.bind_tool(selected,id));if(status==error::success){tool.id=id;tool.selected=selected;}return status;
        }
        std::size_t tool_index(binding const& row,proposals::core::record const& record) const noexcept
        {for(std::size_t i{};i<row.policy.tool_count;++i)if(row.tools[i].profile==record.selected_tool.profile) { return i; }return row.policy.tool_count;}
        /** @brief Validate retained source rights and actual provider lineage without rate charges. */
        error source_group(caller who,binding& row,std::span<proposals::admission::observation const> observations,
                           proposals::admission::observation_policy policy,group_lease& held)
        {
            if(observations.size()>8) { return error::too_large; }
            std::array<admission_claim,16> claims{};std::array<std::size_t,8> indices{};std::array<source_state,8> current{};
            {
                std::lock_guard lock{mutex_};if(retired_||row.retired) { return error::cancelled; }
                for(std::size_t i{};i<observations.size();++i)
                {
                    auto found=row.policy.source_count;
                    for(std::size_t n{};n<row.policy.source_count;++n)if(row.sources[n].id&&row.sources[n].current.observation.source==observations[i].source){found=n;break;}
                    if(found==row.policy.source_count) { return error::denied; }
                    indices[i]=found;current[i]=row.sources[found];
                    claims[2*i]={current[i].capability,"resource.observe",0,true,true};claims[2*i+1]={observations[i].capability,"buffer.read",0,false,true};
                }
            }
            if(observations.empty()) { return live(row); }
            auto status=permissions_->admit_group(who,{claims.data(),observations.size()*2},held,false);if(status!=error::success) { return status; }
            for(std::size_t i{};i<observations.size();++i)
            {
                auto const& rule=row.policy.sources[indices[i]];auto* source=held.specification(2*i);auto* buffer=held.specification(2*i+1);
                if(!source||source->alias!=rule.alias.view()||!buffer||buffer->alias!=rule.buffer_alias.view()||buffer->kind!=purpose::buffer||buffer->binding!="sealed-buffer") { return error::denied; }
                status=source_.validate(source_.context,who,current[i].capability,rule,current[i].current,observations[i],policy);if(status!=error::success) { return status; }
            }
            // A trusted adapter may synchronously revoke a capability while
            // validating lineage. Repeat authority admission after callbacks.
            group_lease rechecked;
            status=permissions_->admit_group(who,{claims.data(),observations.size()*2},rechecked,false);
            return status==error::success?live(row):status;
        }
        static std::size_t receipt_bytes() noexcept
        {auto measure=wire::writer::measuring(512);proposals::admission::receipt value;value.request=value.provider_generation=1;return proposal_contract::encode_receipt(measure,value)?measure.size():0;}
        error preflight_sources(caller who,binding& row,std::span<source_reference const> requested)
        {
            if(requested.size()>row.policy.storage.observations||requested.size()>8)return error::too_large;
            std::array<admission_claim,16> claims{};
            for(std::size_t index{};index<requested.size();++index)
            {
                auto const& ref=requested[index];if(ref.index>=row.policy.source_count||!ref.capability||!ref.resource||!ref.buffer_capability)return error::invalid;
                for(std::size_t previous{};previous<index;++previous)if(requested[previous].index==ref.index)return error::invalid;
                claims[2*index]={ref.capability,"resource.observe",0,true,true};claims[2*index+1]={ref.buffer_capability,"buffer.read",0,false,true};
            }
            if(requested.empty())return live(row);
            group_lease held;auto status=permissions_->admit_group(who,{claims.data(),requested.size()*2},held,false);if(status!=error::success)return status;
            for(std::size_t index{};index<requested.size();++index)
            {
                auto const& ref=requested[index];auto const& rule=row.policy.sources[ref.index];auto source=held.specification(2*index),buffer=held.specification(2*index+1);
                if(!source||source->alias!=rule.alias.view()||!buffer||buffer->kind!=purpose::buffer||buffer->binding!="sealed-buffer"||buffer->alias!=rule.buffer_alias.view())return error::denied;
                source_sample actual;status=source_.capture(source_.context,who,ref.capability,ref.resource,rule,actual);if(status!=error::success)return status;
                auto& metadata=actual.observation;metadata.capability=ref.buffer_capability;metadata.frame=row.sources[ref.index].frame;
                if(actual.environment!=environment_||!actual.owner||actual.owner_generation!=generation_||actual.profile!=rule.profile||actual.frame!=rule.frame||
                    metadata.source!=ref.resource||!metadata.buffer)return error::denied;
                buffers::information info;status=buffers_.describe(rule.buffer_alias.view(),metadata.buffer,info);if(status!=error::success)return status;
                if(!info.sealed||info.value.schema!=rule.buffer_profile)return error::denied;
                status=source_.validate(source_.context,who,ref.capability,rule,actual,metadata,proposals::admission::observation_policy::latest_required);if(status!=error::success)return status;
            }
            group_lease checked;status=permissions_->admit_group(who,{claims.data(),requested.size()*2},checked,false);return status==error::success?live(row):status;
        }
    public:
        proposal_dispatch(std::shared_ptr<authority> permissions,model_dispatch& models,buffers::registry& buffers,handle environment,handle generation,limits limits,
                          std::uint64_t(*clock)() noexcept=proposals::admission::steady_now)
            :permissions_(std::move(permissions)),model_(models),buffers_(buffers),environment_(environment),generation_(generation),limits_(limits),clock_(clock?clock:proposals::admission::steady_now){}
        proposal_dispatch(proposal_dispatch const&)=delete;proposal_dispatch& operator=(proposal_dispatch const&)=delete;
        ~proposal_dispatch(){retire();}
        /** @brief Bind fixed storage and immutable semantic references, with no runtime handles. */
        error bind(::uwvm2::utils::container::std_string alias,proposals::plan const& policy)
        {
            if(!same_process()) { return error::denied; }
            try
            {
                std::lock_guard lock{mutex_};if(ready_||retired_||!permissions_||!environment_||!generation_) { return error::denied; }
                if(!identifier(alias)||!proposals::valid(policy)||limits_.proposals>32||limits_.arbiters>32) { return error::invalid; }
                if(named(alias)||arbiter(alias)) { return error::conflict; }if(bindings_.size()>=limits_.proposals) { return error::too_large; }
                auto value=::uwvm2::utils::container::make_shared<binding>(clock_);value->alias=std::move(alias);value->policy=policy;
                value->scope=resources::allocate_identity();value->producer=resources::allocate_identity();if(!value->scope||!value->producer) { return error::too_large; }
                auto status=convert(value->provider.configure(value->alias,::uwvm2::utils::container::std_string{policy.model_alias.view()},policy.storage,value->producer,value->scope));
                if(status!=error::success) { return status; }status=value->workspace.configure(policy.workspace_bytes,policy.workspace_calls);if(status!=error::success) { return status; }
                for(std::size_t i{};i<policy.source_count;++i)if(!(value->sources[i].frame=frame(*value,policy.sources[i].frame))) { return error::too_large; }
                for(std::size_t i{};i<policy.tool_count;++i)
                {
                    auto& tool=value->tools[i];tool.profile=resources::allocate_identity();tool.schema=resources::allocate_identity();tool.frame=frame(*value,policy.tools[i].frame);
                    tool.target_index=i;
                    for(std::size_t n{};n<i;++n)if(policy.tools[n].target_alias==policy.tools[i].target_alias){tool.target_index=value->tools[n].target_index;break;}
                    auto& target=value->targets[tool.target_index];if(!target.scope)target.scope=resources::allocate_identity();tool.target_scope=target.scope;
                    if(!tool.profile||!tool.schema||!tool.frame||!tool.target_scope) { return error::too_large; }
                }
                bindings_.push_back(std::move(value));return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}
        }
        error bind_arbiter(::uwvm2::utils::container::std_string alias,proposals::arbiter_plan const& policy)
        {
            if(!same_process()) { return error::denied; }
            try
            {
                std::lock_guard lock{mutex_};if(ready_||retired_) { return error::denied; }auto source=named(policy.proposal_alias.view());
                if(!source||!identifier(alias)||!proposals::valid(policy,source->policy)) { return error::invalid; }
                if(named(alias)||arbiter(alias)) { return error::conflict; }if(arbiters_.size()>=limits_.arbiters) { return error::too_large; }
                handle scope{};for(std::size_t i{};i<source->policy.tool_count;++i)if(policy.tool_mask&(std::uint64_t{1}<<i)){scope=source->tools[i].target_scope;break;}
                arbiters_.push_back({std::move(alias),policy,std::move(source),scope,false});return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}
        }
        error prepare(source_access source,target_access target)
        {
            if(!same_process()) { return error::denied; }std::lock_guard lock{mutex_};if(ready_||retired_) { return error::denied; }
            for(auto const& row:bindings_)
            {
                if(row->policy.source_count&&(!source.capture||!source.validate)) { return error::invalid; }
                if(row->policy.tool_count&&(!target.resolve||!target.validate)) { return error::invalid; }
            }
            source_=source;target_=target;ready_=true;return error::success;
        }
        /** @brief Capture real, authorized source snapshots before model submission.
         * @details The adapter may only observe an existing typed resource. It
         * supplies metadata, never user text. The buffer capability, alias,
         * layout profile and actual sealed allocation are independently checked.
         * No source is implicitly opened; old snapshots remain charged while a
         * model/capture/proposal holds them. Failed allocation leaves output empty.
         */
        error capture(caller who,std::string_view alias,std::span<source_reference const> requested,captured_inputs& output)
        {
            if(!same_process()) { return error::denied; }if(output) { return error::conflict; }
            try
            {
                std::shared_ptr<binding> row;{std::lock_guard lock{mutex_};if(!ready_||retired_) { return error::denied; }row=named(alias);}
                if(!row) { return error::denied; }if(requested.size()>row->policy.storage.observations) { return error::too_large; }
                auto status=enter(*row);if(status!=error::success) { return status; }finish_call done{*this,*row};
                status=preflight_sources(who,*row,requested);if(status!=error::success)return status;
                auto count=row->captures.load();while(count<row->policy.captures&&!row->captures.compare_exchange_weak(count,count+1)){}
                if(count>=row->policy.captures) { return error::too_large; }capture_slot slot;slot.owner=row;
                workspace_lease reservation;status=row->workspace.reserve(sizeof(capture_record),reservation);if(status!=error::success) { return status; }
                auto retained=::uwvm2::utils::container::make_shared<capture_record>();retained->slot.owner=std::move(slot.owner);retained->reservation=std::move(reservation);retained->who=who;
                for(std::size_t index{};index<requested.size();++index)
                {
                    auto reference=requested[index];if(reference.index>=row->policy.source_count||!reference.capability||!reference.resource||!reference.buffer_capability) { return error::invalid; }
                    for(std::size_t previous{};previous<index;++previous)if(requested[previous].index==reference.index) { return error::invalid; }
                    auto const& rule=row->policy.sources[reference.index];lease source_grant;status=permissions_->dependency(who,reference.capability,"resource.observe",source_grant);
                    if(status!=error::success) { return status; }if(source_grant.specification().alias!=rule.alias.view()) { return error::denied; }
                    source_sample sample;status=source_.capture(source_.context,who,reference.capability,reference.resource,rule,sample);if(status!=error::success) { return status; }
                    auto& metadata=sample.observation;
                    metadata.capability=reference.buffer_capability;
                    if(sample.environment!=environment_||!sample.owner||sample.owner_generation!=generation_||sample.profile!=rule.profile||sample.frame!=rule.frame||
                       metadata.source!=reference.resource||!metadata.capability||!metadata.buffer) { return error::denied; }
                    std::array claims{admission_claim{reference.capability,"resource.observe",0,true,true},admission_claim{metadata.capability,"buffer.read",0,false,true}};
                    group_lease all;status=permissions_->admit_group(who,claims,all,false);if(status!=error::success) { return status; }
                    auto* selected=all.specification(1);if(!selected||selected->kind!=purpose::buffer||selected->binding!="sealed-buffer"||selected->alias!=rule.buffer_alias.view()) { return error::denied; }
                    buffers::information info;status=buffers_.describe(rule.buffer_alias.view(),metadata.buffer,info);if(status!=error::success) { return status; }
                    if(!info.sealed||info.value.schema!=rule.buffer_profile) { return error::denied; }
                    buffers::registry::sealed_lease bytes;status=buffers_.pin(rule.buffer_alias.view(),metadata.buffer,bytes);if(status!=error::success) { return status; }
                    source_state prior;{std::lock_guard lock{mutex_};prior=row->sources[reference.index];}
                    metadata.frame=prior.frame;
                    if(prior.id&&(prior.capability!=reference.capability||prior.current.owner!=sample.owner)&&metadata.source_authority_epoch<=prior.current.observation.source_authority_epoch) { return error::conflict; }
                    status=source_.validate(source_.context,who,reference.capability,rule,sample,metadata,proposals::admission::observation_policy::latest_required);if(status!=error::success) { return status; }
                    group_lease rechecked;
                    status=permissions_->admit_group(who,claims,rechecked,false);if(status!=error::success)return status;
                    if(live(*row)!=error::success) { return error::cancelled; }
                    handle id=prior.id;
                    if(id&&prior.current.observation==metadata)
                    {if(prior.capability!=reference.capability||prior.current.owner!=sample.owner) { return error::conflict; }}
                    else
                    {
                        status=convert(id?row->provider.update_source(id,metadata,rule.frame.view(),bytes):row->provider.bind_source(metadata,rule.frame.view(),bytes,id));
                        if(status!=error::success) { return status; }
                    }
                    {std::lock_guard lock{mutex_};auto& state=row->sources[reference.index];state.id=id;state.capability=reference.capability;state.current=sample;}
                    status=convert(row->provider.capture(id,retained->inputs[index]));if(status!=error::success) { return status; }
                    ++retained->count;
                }
                // A later source callback can retire an earlier source. The
                // final opaque capture is published only after a complete group
                // and actual-pin recheck, with no intervening external callback.
                std::array<proposals::admission::observation,8> captured_observations{};
                for(std::size_t i{};i<retained->count;++i)captured_observations[i]=retained->inputs[i].value->metadata;
                group_lease final_sources;
                status=source_group(who,*row,{captured_observations.data(),retained->count},proposals::admission::observation_policy::latest_required,final_sources);
                if(status!=error::success)return status;
                for(std::size_t i{};i<retained->count;++i)
                {
                    auto const& snapshot=retained->inputs[i].value;
                    status=snapshot->bytes.visit([&](auto,auto const&,auto const& digest)noexcept{return digest==snapshot->evidence.sha256;});
                    if(status!=error::success)return status;
                }
                output.value=std::move(retained);return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        /** @brief Check every source and actual sealed layout before capture allocation.
         * @details This is not a reservation or authority issue. Capture repeats
         * all checks; callbacks may change state and invalidate this preflight.
         */
        error preflight_capture(caller who,std::string_view alias,std::span<source_reference const> requested)
        {
            if(!same_process()||!permissions_)return error::denied;
            std::shared_ptr<binding> row;{std::lock_guard lock{mutex_};if(!ready_||retired_)return error::denied;row=named(alias);}
            return row?preflight_sources(who,*row,requested):error::denied;
        }
        /** @brief Revalidate an opaque capture's original caller, source group and real pins. */
        error validate_capture(caller who,std::string_view alias,captured_inputs const& captured,
                               proposals::admission::observation_policy mode=proposals::admission::observation_policy::bounded_age)
        {
            if(!same_process()||!permissions_||!captured.value)return error::denied;
            auto held=captured.value;auto row=held->slot.owner;
            {std::lock_guard lock{mutex_};if(!ready_||retired_||!row||named(alias)!=row||!same(who,held->who))return error::denied;}
            std::array<proposals::admission::observation,8> observations{};
            for(std::size_t index{};index<held->count;++index)
            {if(!held->inputs[index].value)return error::bad_handle;observations[index]=held->inputs[index].value->metadata;}
            group_lease current;auto status=source_group(who,*row,{observations.data(),held->count},mode,current);if(status!=error::success)return status;
            for(std::size_t index{};index<held->count;++index)
            {auto const& snapshot=held->inputs[index].value;status=snapshot->bytes.visit([&](auto,auto const&,auto const& digest)noexcept{return digest==snapshot->evidence.sha256;});if(status!=error::success)return status;}
            return live(*row);
        }
        /** @brief Publish only a fully authenticated, policy-matched final model envelope.
         * @details No worker or target execution is allocated here. Captures
         * must be the exact input sequence adopted by this completed model job.
         * Final bytes are copied into reserved owned storage before hashing so
         * later caller changes cannot change arguments or their outer meaning.
         */
        error publish(captured_inputs const& captured,handle model_request,bytes final,std::uint64_t expiry,handle& output,
                      publication_admission final_admission={},cancellation cancelled={})
        {
            if(!same_process()) { return error::denied; }
            if(!captured.value||!model_request||final.size()>proposals::maximum_envelope_bytes) { return error::invalid; }
            try
            {
                auto held=captured.value;auto row=held->slot.owner;
                {std::lock_guard lock{mutex_};if(!ready_||retired_||!row||named(row->alias)!=row)return error::denied;}
                auto status=enter(*row);if(status!=error::success) { return status; }finish_call done{*this,*row};
                workspace_lease reservation;status=row->workspace.reserve(final.size(),reservation);if(status!=error::success) { return status; }
                byte_buffer owned;owned.resize_for_overwrite(final.size());std::copy(final.begin(),final.end(),owned.writable().begin());
                std::array<models::input_evidence,8> expected{};std::array<proposals::admission::observation,8> observations{};
                for(std::size_t i{};i<held->count;++i)
                {auto const& snapshot=held->inputs[i].value;if(!snapshot) { return error::bad_handle; }expected[i]=snapshot->evidence;observations[i]=snapshot->metadata;}
                return model_.with_completed_output(row->policy.model_alias.view(),model_request,{expected.data(),held->count},owned.view(),[&](model_dispatch::completion const& completed)
                {
                    if(!same(completed.origin(),held->who)||completed.policy().model!=row->policy.model||completed.policy().task!=row->policy.task||
                       completed.policy().output_type.view()!=proposals::model_output_type||completed.policy().encoding.view()!=proposals::model_output_encoding) { return error::denied; }
                    proposals::envelope decoded;if(!proposals::decode_envelope(completed.output(),decoded)) { return error::invalid; }
                    std::size_t index=row->policy.tool_count;
                    for(std::size_t i{};i<row->policy.tool_count;++i)if(proposals::matches(decoded,row->policy.tools[i])){index=i;break;}
                    if(index==row->policy.tool_count||decoded.arguments.size()>row->policy.storage.argument_bytes) { return error::denied; }
                    auto status=attach_tool(*row,index,decoded.target);if(status!=error::success) { return status; }
                    group_lease sources;status=source_group(completed.origin(),*row,{observations.data(),held->count},row->policy.tools[index].observations,sources);if(status!=error::success) { return status; }
                    auto& tool=row->tools[index];if(!tool.selected.validate_arguments(decoded.arguments)) { return error::invalid; }
                    status=completed.revalidate();if(status!=error::success) { return status; }
                    if(live(*row)!=error::success) { return error::cancelled; }
                    return convert(row->provider.publish_completed(completed,tool.id,decoded.arguments,{held->inputs.data(),held->count},expiry,output,[&]
                    {
                        if(cancelled&&cancelled())return error::cancelled;
                        group_lease rechecked;
                        auto status=source_group(completed.origin(),*row,{observations.data(),held->count},row->policy.tools[index].observations,rechecked);
                        if(status!=error::success)return status;
                        target_evidence current;
                        status=target_.resolve(target_.context,row->policy.tools[index],decoded.target,current);
                        if(status!=error::success)return status;
                        {
                            std::lock_guard lock{mutex_};
                            if(retired_||row->retired)return error::cancelled;
                            if(!same_target(current,row->targets[tool.target_index].current)||current.validate_arguments!=tool.selected.validate_arguments)return error::conflict;
                        }
                        group_lease final_sources;
                        status=source_group(completed.origin(),*row,{observations.data(),held->count},row->policy.tools[index].observations,final_sources);
                        if(status!=error::success)return status;
                        // The trusted final admission runs outside provider
                        // storage locks. It may recheck bounded source evidence
                        // and must fence its own lifetime and authority after
                        // reentry. The model token revalidates again afterwards.
                        return final_admission?final_admission():error::success;
                    }));
                });
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        error preflight(caller who,operation action,handle cap,std::size_t extent)
        {
            if(!same_process()||!permissions_) { return error::denied; }
            auto index=static_cast<std::size_t>(action);if(action!=operation::read&&action!=operation::accept) { return error::unsupported; }
            if(extent!=(action==operation::read?8u:56u)) { return error::invalid; }
            lease primary;auto status=permissions_->preflight(who,cap,proposal_contract::names[index],extent,primary);if(status!=error::success) { return status; }
            std::shared_ptr<binding> row;arbiter_binding* arbiter{};return resolve(primary.specification(),action,row,arbiter);
        }
        /** @brief Reserve owned carrier storage only after its exact operation authority.
         * @details Used by the guest facade before a copied request allocation.
         * This borrows the existing explicit per-alias workspace budget; it
         * neither opens a source/capability nor issues a proposal/capture handle.
         */
        error reserve_workspace(caller who,operation action,handle cap,std::size_t extent,workspace_lease& output)
        {
            if(!same_process()||!permissions_)return error::denied;
            auto index=static_cast<std::size_t>(action);if(index>=proposal_contract::names.size())return error::unsupported;
            if(extent>proposal_contract::maximum_request_bytes)return error::too_large;
            lease held;auto status=permissions_->preflight(who,cap,proposal_contract::names[index],extent,held);if(status!=error::success)return status;
            std::shared_ptr<binding> row;arbiter_binding* arbiter{};
            status=resolve(held.specification(),action==operation::accept?operation::accept:operation::read,row,arbiter);
            return status==error::success?row->workspace.reserve(extent,output):status;
        }
        error invoke(caller who,operation action,handle cap,bytes input,std::size_t capacity,std::size_t& required,sink deliver,cancellation cancelled={})
        {
            required=0;auto status=preflight(who,action,cap,input.size());if(status!=error::success) { return status; }
            if(!deliver||capacity>proposals::maximum_envelope_bytes) { return error::invalid; }
            proposals::admission::accept_request request;if(!proposal_contract::decode(action,input,request)||!request.proposal) { return error::invalid; }
            auto name=proposal_contract::names[static_cast<std::size_t>(action)];lease primary;status=permissions_->admit(who,cap,name,0,primary,false);if(status!=error::success) { return status; }
            std::shared_ptr<binding> row;arbiter_binding* arbitration{};status=resolve(primary.specification(),action,row,arbitration);if(status!=error::success) { return status; }
            try
            {
                if(cancelled&&cancelled()) { return error::cancelled; }
                workspace_lease workspace;status=row->workspace.reserve(0,workspace);if(status!=error::success) { return status; }
                if(action==operation::accept)
                {required=receipt_bytes();if(capacity<required) { return error::too_large; }if(request.key.session!=cap||!request.key.sequence||!request.proposal_capability||!request.target_capability||!request.target) { return error::invalid; }}
                reader_state* reader_state{};status=reader(*row,who,cap,arbitration,reader_state);if(status!=error::success) { return status; }
                auto emit=[&](bytes bytes,auto&& validate_context)
                {
                    try
                    {
                        if(cancelled&&cancelled()) { return false; }
                        // Cancellation may synchronously retire a typed source or
                        // revoke an independent buffer/source capability. Validate
                        // the whole disclosure context after that last callback.
                        if(!validate_context()) { return false; }
                        lease current;if(permissions_->admit(who,cap,name,0,current,false)!=error::success||live(*row)!=error::success) { return false; }
                        if(!deliver(bytes)||(cancelled&&cancelled())) { return false; }
                        if(!validate_context()) { return false; }
                        return permissions_->admit(who,cap,name,0,current,false)==error::success&&live(*row)==error::success;
                    }
                    catch(...){return false;}
                };
                if(action==operation::read)
                {
                    error failure=error::success;
                    auto result=row->provider.read(row->alias,reader_state->actor,cap,request.proposal,[&](auto const& record,auto const& tool,auto snapshots)
                    {
                        auto index=tool_index(*row,record);if(index==row->policy.tool_count){failure=error::denied;return false;}
                        group_lease sources;failure=source_group(who,*row,{record.observations.data(),record.observation_count},proposals::admission::observation_policy::bounded_age,sources);
                        if(failure!=error::success) { return false; }
                        auto measure=wire::writer::measuring(proposals::maximum_envelope_bytes);if(!proposal_contract::encode(measure,record,tool,snapshots)){failure=error::too_large;return false;}
                        required=measure.size();if(capacity<required){failure=error::too_large;return false;}
                        failure=workspace.grow(required);if(failure!=error::success) { return false; }byte_buffer output;output.resize_for_overwrite(required);wire::writer writer{output.writable()};
                        if(!proposal_contract::encode(writer,record,tool,snapshots)||writer.size()!=required){failure=error::io;return false;}
                        lease charged;failure=permissions_->admit(who,cap,name,input.size()+required,charged);if(failure!=error::success) { return false; }
                        auto current_sources=[&]
                        {
                            group_lease current;failure=source_group(who,*row,{record.observations.data(),record.observation_count},proposals::admission::observation_policy::bounded_age,current);
                            if(failure!=error::success) { return false; }
                            // Independently retiring a buffer provider must not
                            // disclose the already encoded source provenance.
                            for(auto const& snapshot:snapshots)
                            {
                                if(!snapshot){failure=error::bad_handle;return false;}
                                failure=snapshot->bytes.visit([&](auto,auto const&,auto const& digest)noexcept{return digest==snapshot->evidence.sha256;});
                                if(failure!=error::success) { return false; }
                            }
                            return true;
                        };
                        if(!emit(writer.written(),current_sources)){if(failure==error::success)failure=error::io;return false;}
                        return true;
                    });
                    return failure!=error::success?failure:convert(result);
                }
                std::array origin_claims{admission_claim{cap,"proposal.accept",0,false,true},admission_claim{request.proposal_capability,"proposal.read",0,false,true}};
                group_lease origin;status=permissions_->admit_group(who,origin_claims,origin,false);if(status!=error::success) { return status; }
                auto* proposal_grant=origin.specification(1);if(!proposal_grant||proposal_grant->kind!=purpose::proposal||proposal_grant->binding!="model-proposals"||proposal_grant->alias!=row->alias) { return error::denied; }
                proposals::admission::authorization_context context{row->scope,arbitration->target_scope};proposals::admission::receipt receipt;bool replay{};
                status=convert(row->provider.probe_retry(row->alias,reader_state->actor,request,context,receipt,replay));if(status!=error::success) { return status; }
                if(replay)
                {lease charged;status=permissions_->admit(who,cap,name,input.size()+required,charged);if(status!=error::success) { return status; }}
                else
                {
                    error failure=error::success;
                    auto admitted=row->provider.accept(row->alias,reader_state->actor,request,context,[&](auto const& record,auto const& selected)
                    {
                        auto index=tool_index(*row,record);
                        if(index==row->policy.tool_count||!(arbitration->policy.tool_mask&(std::uint64_t{1}<<index))) { return proposals::result::denied; }
                        auto const& rule=row->policy.tools[index];target_evidence expected;
                        {std::lock_guard lock{mutex_};if(arbitration->retired||retired_||row->retired) { return proposals::result::cancelled; }expected=row->targets[row->tools[index].target_index].current;}
                        std::array claims{admission_claim{cap,"proposal.accept",0,false,true},admission_claim{request.proposal_capability,"proposal.read",0,false,true},
                            admission_claim{request.target_capability,proposal_contract::target_names[static_cast<std::size_t>(rule.action)],0,false,true}};
                        group_lease all;failure=permissions_->admit_group(who,claims,all,false);if(failure!=error::success) { return convert(failure); }
                        auto* target_grant=all.specification(2);if(!target_grant||target_grant->alias!=rule.target_alias.view()||target_grant->kind!=target_kind(rule.action)) { return proposals::result::denied; }
                        target_evidence actual;failure=target_.resolve(target_.context,rule,request.target,actual);if(failure!=error::success) { return convert(failure); }
                        if(!target_valid(rule,request.target,actual)||!same_target(actual,expected)||selected.semantics!=record.selected_tool) { return proposals::result::conflict; }
                        group_lease sources;failure=source_group(who,*row,{record.observations.data(),record.observation_count},rule.observations,sources);if(failure!=error::success) { return convert(failure); }
                        if(!actual.validate_arguments({record.arguments.data(),record.argument_size})) { return proposals::result::invalid; }
                        failure=target_.validate(target_.context,who,request.target_capability,rule,actual,{record.arguments.data(),record.argument_size},{record.observations.data(),record.observation_count});
                        if(failure!=error::success) { return convert(failure); }
                        if((cancelled&&cancelled())||live(*row)!=error::success) { return proposals::result::cancelled; }
                        group_lease after;failure=source_group(who,*row,{record.observations.data(),record.observation_count},rule.observations,after);if(failure!=error::success) { return convert(failure); }
                        target_evidence checked;failure=target_.resolve(target_.context,rule,request.target,checked);if(failure!=error::success) { return convert(failure); }
                        if(!same_target(checked,actual)) { return proposals::result::conflict; }
                        group_lease rechecked;failure=permissions_->admit_group(who,claims,rechecked,false);if(failure!=error::success) { return convert(failure); }
                        lease charged;failure=permissions_->admit(who,cap,name,input.size()+required,charged);return convert(failure);
                    },receipt);
                    if(admitted!=proposals::result::success) { return failure!=error::success?failure:convert(admitted); }
                    {std::lock_guard lock{mutex_};reader_state->last=receipt;reader_state->released=false;}
                }
                std::array<std::uint8_t,512> output{};wire::writer writer{output};if(!proposal_contract::encode_receipt(writer,receipt)||writer.size()!=required) { return error::io; }
                return emit(writer.written(),[]noexcept{return true;})?error::success:error::io;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        /** @brief Describe/release the last inert receipt owned by this arbiter capability.
         * @details Release drops ordinary lookup, but preserves the bounded exact
         * acceptance replay tombstone. Neither operation rechecks old target or
         * source rights: historical admission is not fresh execution authority.
         * A new successful sequence acknowledges/replaces the previous receipt.
         */
        error request(caller who,handle cap,handle request_id,bool release,std::size_t capacity,std::size_t& required,
                      sink deliver,cancellation cancelled={})
        {
            required=release?8:receipt_bytes();
            if(!same_process()||!permissions_)return error::denied;
            if(!request_id||!deliver||capacity>proposals::maximum_envelope_bytes)return error::invalid;
            auto name=release?std::string_view{"request.release"}:std::string_view{"request.describe"};
            try
            {
                lease primary;auto status=permissions_->preflight(who,cap,name,8,primary);if(status!=error::success)return status;
                std::shared_ptr<binding> row;arbiter_binding* arbitration{};
                status=resolve(primary.specification(),operation::accept,row,arbitration);if(status!=error::success)return status;
                if(capacity<required)return error::too_large;
                if(cancelled&&cancelled())return error::cancelled;
                proposals::admission::receipt receipt;reader_state* owner{};
                {
                    std::lock_guard lock{mutex_};
                    for(auto& candidate:row->readers)
                        if(candidate.capability==cap&&same(candidate.who,who)&&candidate.arbiter&&candidate.last.request==request_id)
                            owner=&candidate;
                    if(!owner||(!release&&owner->released))return error::bad_handle;
                    receipt=owner->last;
                }
                lease charged;status=permissions_->admit(who,cap,name,8+required,charged);if(status!=error::success)return status;
                std::array<std::uint8_t,512> output{};wire::writer writer{output};
                {
                    std::lock_guard lock{mutex_};
                    if(retired_||row->retired||arbitration->retired)return error::cancelled;
                    if(owner->last.request!=request_id)return error::bad_handle;
                    if(release){bool changed=!owner->released;owner->released=true;if(!writer.boolean(changed))return error::io;}
                    else if(owner->released)return error::bad_handle;
                    else if(!proposal_contract::encode_receipt(writer,receipt))return error::io;
                }
                if(cancelled&&cancelled())return error::cancelled;
                // Cancellation callbacks may synchronously revoke authority while
                // returning false. Check again before any historical disclosure.
                lease current;status=permissions_->admit(who,cap,name,0,current,false);if(status!=error::success)return status;
                {std::lock_guard lock{mutex_};if(retired_||row->retired||arbitration->retired)return error::cancelled;}
                if(!deliver(writer.written())||(cancelled&&cancelled()))return error::io;
                lease checked;status=permissions_->admit(who,cap,name,0,checked,false);if(status!=error::success)return status;
                return live(*row);
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        error close(caller who,handle cap,handle proposal)
        {
            if(!same_process()||!permissions_) { return error::denied; }
            lease allowed;auto status=permissions_->admit(who,cap,"resource.close",16,allowed);if(status!=error::success) { return status; }
            std::shared_ptr<binding> row;arbiter_binding* unused{};status=resolve(allowed.specification(),operation::read,row,unused);
            return status==error::success?convert(row->provider.release(row->alias,proposal)):status;
        }
        error inspect(std::string_view alias,usage& output) const noexcept
        {
            if(!same_process()) { return error::denied; }
            std::unique_lock lock{mutex_,std::try_to_lock};if(!lock.owns_lock()) { return error::again; }auto row=named(alias);if(!row) { return error::denied; }
            proposals::usage sampled;auto status=convert(row->provider.try_inspect(alias,sampled));if(status!=error::success) { return status; }
            usage value{sampled.proposals,sampled.reader_slots,sampled.source_slots,sampled.tool_slots,row->captures.load(),0,0,row->retired};
            row->workspace.inspect(value.workspace_bytes,value.workspace_calls);output=value;return error::success;
        }
        void revoke(std::string_view alias) noexcept
        {
            if(!same_process())return;
            std::shared_ptr<binding> row;
            {std::lock_guard lock{mutex_};if(auto* selected=arbiter(alias)){selected->retired=true;row=selected->source;
                for(auto const& reader:row->readers)
                {if(reader.capability&&reader.arbiter&&reader.arbiter_alias.view()==alias)row->provider.revoke_reader(reader.capability);}
                return;}
                row=named(alias);if(!row)return;row->retired=true;}
            row->workspace.retire();row->provider.retire();
        }
        void retire() noexcept
        {
            if(!same_process())return;
            {std::lock_guard lock{mutex_};if(retired_)return;retired_=true;for(auto& arbiter:arbiters_)arbiter.retired=true;}
            for(auto const& row:bindings_){std::lock_guard lock{mutex_};row->retired=true;row->workspace.retire();row->provider.retire();}
        }
    };
}
