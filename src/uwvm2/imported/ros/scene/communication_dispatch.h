/** @file
 * @brief Canonical communication admission with explicit origin capabilities.
 * @details Trusted deployment supplies one immutable plan per logical alias.
 * The dispatcher replaces plan owner fields with its environment/grant/generation
 * identities; guest integers never establish ownership. Opening a node retains
 * its explicit context capability. Opening an endpoint retains its explicit node,
 * context and ordered schema capabilities. Every later operation revalidates
 * those complete origins under one authority lease. Existing node/type/context
 * dependencies are validation-only: their creation rates are not charged again.
 *
 * Native factories are injected only by prepare(). Configuration performs no
 * native I/O. Default zero limits and no factory cannot activate communication.
 * Host-owned carriers are bounded before decoding; detached output is measured
 * and reserved before allocation/copy. A short output never charges admission or
 * performs a mutation. Exact underlying request keys recover failed mutation
 * output without repeating the native effect. Failed open closes its unpublished
 * object; failed cleanup retains a fenced tombstone instead of permitting reopen.
 * This portable dispatcher is not a native-worker syscall sandbox.
 */
#pragma once
#include "authority.h"
#include "service_observation_audit.h"
#include "communication_contract.h"
#include "communication_receive_contract.h"
#include "byte_buffer.h"
#include "buffers.h"
#include "communication_events.h"
#include "communication_creation.h"
#include <uwvm2/utils/container/function_ref.h>
#include <uwvm2/utils/container/shared.h>
#include <variant>

namespace uwvm2::imported::ros::scene
{
    class communication_dispatch
    {
    public:
        class opening;
    private:
        using core=communication::registry;
        using op=communication_contract::operation;
        using profile=communication::profile;
        using owner=communication::owner;
        using plan=std::variant<communication::context_plan,communication::node_plan,communication::type_plan,
                                communication::topic_plan,communication::service_plan>;
        struct origins
        {
            handle id{};
            std::array<admission_claim,6> claims{};
            std::array<handle,6> resources{};
            std::array<owner,6> owners{};
            std::size_t count{};
            bool published{},orphan{};
            bool has_receipt{};
            op previous_action{};
            buffers::digest canonical{};
            communication::receipt receipt;
            communication::delivery_consumer receiver;
            communication::delivery_reference retained_receive;
            communication_receive_contract::reference public_receive,settled_receive;
            communication::receipt receive_receipt;
            std::uint64_t receive_deadline{};
            bool receive_charged{},receive_ack{},receive_unknown{};
        };
        struct binding
        {
            ::uwvm2::utils::container::std_string alias;
            owner identity;
            purpose kind{};
            plan policy;
            std::shared_ptr<origins> live;
            bool opening{},revoked{};
            std::shared_ptr<class opening> pending;
            std::uint64_t last_creation_key{};
        };
        struct workspace_account
        {
            std::mutex mutex;
            std::uint64_t maximum{},calls_max{},used{},calls{};
            bool retired{};
#if defined(__linux__)
            ::pid_t process{::getpid()};
#endif
        };
    public:
        /** @brief Private topic/service draft retaining exact admitted dependencies.
         * @details Only the dispatcher can populate a valid draft. Its reserved
         * host identity is unusable until verified native adoption and separate
         * publication. One counted node lease and the registry slot stay owned
         * across failed guest copies, cancellation and uncertain native effects.
         * Dropping an external reference cannot free the slot. A positively
         * released creation receipt is required to reuse the binding.
         */
        class opening
        {
            friend class communication_dispatch;
            ::uwvm2::utils::container::std_string alias;
            std::shared_ptr<origins> source;
            std::variant<communication::topic_plan,communication::service_plan,communication::node_plan> policy;
            std::array<communication::type_plan,2> schemas;
            communication_creation::identity original;
            communication_open_contract::endpoint_kind family{};
            buffers::digest canonical{};
            std::size_t carrier_bytes{};
            communication::node_lease parent;
            communication::endpoint_slot slot;
            communication::node_stage_request node_stage;
            std::atomic<handle> private_resource{};
            std::atomic<handle> creation_request{};
            handle provider_generation{};
            std::weak_ptr<communication::native_topic> topic;
            std::weak_ptr<communication::native_service> service;
            bool adopted{},released{};
            std::mutex submission;
        public:
            opening()=default;opening(opening const&)=delete;opening& operator=(opening const&)=delete;
            template<typename Plan> Plan const* specification()const noexcept{return std::get_if<Plan>(&policy);}
            communication::type_plan const* schema(std::size_t index)const noexcept
            {return index<(family==communication_open_contract::endpoint_kind::node?0u:family==communication_open_contract::endpoint_kind::topic?1u:2u)?&schemas[index]:nullptr;}
            communication_creation::identity const& identity()const noexcept{return original;}
            communication_open_contract::endpoint_kind kind()const noexcept{return family;}
            communication::node_lease const& node()const noexcept{return parent;}
            handle resource()const noexcept{return private_resource.load(std::memory_order_acquire);}
            handle request()const noexcept{return creation_request.load(std::memory_order_acquire);}
        };
        /** @brief Nonblocking trusted creation adapter; failure means no commit.
         * @details Enqueue reserves canonical bytes and native queue capacity,
         * then charges only through admit_open. Inspect recovers the same actual
         * request after adoption has moved the node lease. Neither callback
         * publishes a guest resource or performs a guest copy.
         */
        struct asynchronous_factory
        {
            void* context{};
            communication::error(*enqueue)(void*,opening const&,wire::bytes,communication_creation::view&)noexcept{};
            communication::error(*inspect)(void*,opening const&,communication_creation::view&)noexcept{};
        };
        struct limits
        {
            communication::limits native;
            std::size_t bindings{},carrier_bytes{},output_bytes{};
            std::uint64_t workspace_bytes{},workspace_calls{};
            /// @brief Explicit finite startup deadline; zero disables asynchronous node creation.
            std::uint64_t node_deadline_ns{};
        };
        struct usage
        {std::uint64_t configured{},handles{},orphaned{},workspace_bytes{},workspace_calls{};bool ready{},retired{};};
        using sink=::uwvm2::utils::container::function_ref<bool(wire::bytes)>;
        /** @brief Trusted observation clock for service.available, never fabricated RMW time. */
        struct observation_clock
        {
            void* context{};
            bool (*sample)(void*,communication_contract::clock_epoch&,std::int64_t&) noexcept{};
        };
        class workspace_lease
        {
            friend class communication_dispatch;
            std::shared_ptr<workspace_account> account;
            std::uint64_t bytes{};
        public:
            workspace_lease()=default;workspace_lease(workspace_lease const&)=delete;
            workspace_lease& operator=(workspace_lease const&)=delete;
            workspace_lease(workspace_lease&& other) noexcept:account(std::move(other.account)),bytes(std::exchange(other.bytes,0)){}
            workspace_lease& operator=(workspace_lease&& other) noexcept
            {if(this!=&other){reset();account=std::move(other.account);bytes=std::exchange(other.bytes,0);}return *this;}
            ~workspace_lease(){reset();}
            void reset() noexcept
            {
#if defined(__linux__)
                if(account&&account->process!=::getpid())
                {std::construct_at(std::addressof(account));bytes=0;return;}
#endif
                if(account){std::lock_guard lock{account->mutex};account->used-=bytes;--account->calls;}account.reset();bytes=0;
            }
            explicit operator bool() const noexcept{return bool(account);}
        };
    private:
        std::shared_ptr<authority> permissions_;
        std::shared_ptr<core> core_;
        std::shared_ptr<workspace_account> workspace_;
        std::uint64_t environment_{},generation_{};
        limits limits_;
        observation_clock observation_;
        mutable std::mutex mutex_;
        ::uwvm2::utils::container::std_vector<binding> bindings_;
        bool ready_{},retired_{},endpoints_async_{},nodes_async_{};
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
        static bool kind_matches(purpose kind,op action) noexcept
        {
            switch(action)
            {
            case op::context_open:case op::context_describe:case op::context_shutdown:return kind==purpose::context;
            case op::node_open:case op::node_describe:return kind==purpose::node;
            case op::type_describe:return kind==purpose::type;
            case op::topic_open:case op::topic_publish:case op::topic_take:case op::topic_describe:
            case op::qos_liveliness_assert:case op::qos_acknowledgements_wait:return kind==purpose::topic;
            case op::qos_describe:return kind==purpose::topic||kind==purpose::service;
            case op::service_open:case op::service_submit:case op::service_take:case op::service_respond:
            case op::service_available:case op::service_abandon:case op::service_describe:return kind==purpose::service;
            default:return false;
            }
        }
        static bool receive_matches(purpose kind,communication::permission action)noexcept
        {
            using P=communication::permission;
            return kind==purpose::topic?(action==P::topic_reserve||action==P::topic_acknowledge||action==P::topic_release):
                kind==purpose::service&&(action==P::service_reserve||action==P::service_acknowledge||action==P::service_release);
        }
        static bool cleanup_operation(communication::permission action)noexcept
        {return action==communication::permission::topic_release||action==communication::permission::service_release||action==communication::permission::resource_close;}
        binding* named(std::string_view alias) noexcept
        {for(auto& value:bindings_)if(value.alias==alias)return &value;return nullptr;}
        static profile const& selected(binding const& value) noexcept
        {return std::visit([](auto const& p)->profile const&{return p.selected;},value.policy);}
        template<typename P> error bind_impl(::uwvm2::utils::container::std_string alias,P value,purpose kind)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};
            if(ready_||retired_||!permissions_||!environment_||!generation_)return error::denied;
            if(!identifier(alias)||!communication::valid(value.selected))return error::invalid;
            if(named(alias))return error::conflict;
            if(bindings_.size()>=limits_.bindings)return error::too_large;
            auto identity=resources::allocate_identity();if(!identity)return error::too_large;
            value.authority={environment_,identity,generation_};
            bindings_.push_back({std::move(alias),value.authority,kind,plan{std::move(value)},{},false,false,{},0});
            return error::success;
        }
        error reserve(std::size_t bytes,workspace_lease& output)
        {
            if(output)return error::conflict;
            if(!workspace_)return error::denied;
            std::lock_guard lock{workspace_->mutex};
            if(workspace_->retired)return error::cancelled;
            if(bytes>workspace_->maximum-workspace_->used||workspace_->calls>=workspace_->calls_max)return error::again;
            workspace_->used+=bytes;++workspace_->calls;output.account=workspace_;output.bytes=bytes;return error::success;
        }
        error resolve(grant const& grant,binding*& output) noexcept
        {
            std::lock_guard lock{mutex_};
            auto* value=named(grant.alias);
            if(!ready_||retired_||!core_||!value||value->revoked||value->kind!=grant.kind||grant.binding!="native-communication")return error::denied;
            output=value;return error::success;
        }
        error resolve_observation(grant const& policy,std::string_view action,binding*& output) noexcept
        {
            if(!service_observer(policy))return resolve(policy,output);
            if(action!="service.available")return error::denied;
            std::lock_guard lock{mutex_};
            auto* actual=named(policy.resource);
            if(!ready_||retired_||!core_||!actual||actual->revoked||actual->kind!=purpose::service||
               std::get<communication::service_plan>(actual->policy).direction!=communication::service_role::client)return error::denied;
            output=actual;return error::success;
        }
        error active(binding& value,handle id,std::shared_ptr<origins>& output) noexcept
        {
            std::lock_guard lock{mutex_};
            if(retired_||value.revoked||!value.live||!value.live->published||value.live->orphan||value.live->id!=id)return error::bad_handle;
            output=value.live;return error::success;
        }
        error group(caller who,handle primary,std::string_view action,std::uint64_t bytes,origins const* source,group_lease& result,bool charge,bool check_budget=false)
        {
            std::array<admission_claim,7> claims{};claims[0]={primary,action,bytes};std::size_t count=1;
            if(source){for(std::size_t i{};i<source->count;++i)claims[count++]=source->claims[i];}
            auto status=permissions_->admit_group(who,{claims.data(),count},result,charge,check_budget);
            return status==error::success&&source?validate_origins(*source,result):status;
        }
        /** @brief Revalidate one registered readiness observer and the actual client's held origins.
         * @details The client grant never pays for an observation. Validate the pair and all
         * node/context/schema grants under a single lease before any quota debit.
         */
        error availability_group(caller who,handle cap,binding const& client,handle linked,origins const& source,
                                 std::uint64_t bytes,group_lease& output,bool charge=false)
        {
            if(!linked)return group(who,cap,"service.available",bytes,&source,output,charge);
            if(linked==cap||!source.id||source.count!=4)return error::denied;
            std::array<admission_claim,6> claims{};
            claims[0]={cap,"service.available",bytes};
            for(std::size_t i{};i<source.count;++i)claims[i+1]=source.claims[i];
            claims.back()={linked,"service.submit",0,false,true};
            group_lease validated;
            auto status=permissions_->admit_group(who,claims,validated,false);
            if(status!=error::success)return status;
            auto observer=validated.specification(0), target=validated.specification(5);
            if(!observer||!target||!service_observer(*observer)||observer->resource!=client.alias||target->alias!=client.alias||
               target->kind!=purpose::service||target->binding!="native-communication")return error::denied;
            status=validate_origins(source,validated);
            if(status!=error::success)return status;
            if(charge)return permissions_->admit_group(who,claims,output,true);
            output=std::move(validated);return error::success;
        }
        /** @brief Check retained exact resources as well as their capability grants.
         * @details Local provider retirement is independent of authority-table
         * revocation. Readiness and nested node users must reject either form;
         * an admitted integer capability cannot resurrect a retired dependency.
         */
        error validate_origins(origins const& source,group_lease const& admitted)
        {
            for(std::size_t i{};i<source.count;++i)
            {
                auto* grant=admitted.specification(i+1);if(!grant)return error::denied;
                binding* dependency{};auto status=resolve(*grant,dependency);if(status!=error::success)return status;
                auto resource=source.resources[i];if(!resource)continue;
                auto native=dependency->kind==purpose::context?core_->validate_context(dependency->identity,resource,communication_dispatch::selected(*dependency)):
                    dependency->kind==purpose::node?core_->validate_node(dependency->identity,resource,communication_dispatch::selected(*dependency)):communication::error::denied;
                status=convert(native);if(status!=error::success)return status;
            }
            return error::success;
        }
        error add_origin(caller who,handle capability,std::string_view action,purpose expected,origins& target,binding*& value)
        {
            lease admitted;auto result=permissions_->admit(who,capability,action,0,admitted,false);
            if(result!=error::success)return result;
            result=resolve(admitted.specification(),value);if(result!=error::success||value->kind!=expected)return error::denied;
            if(target.count==target.claims.size())return error::too_large;
            target.owners[target.count]=value->identity;
            target.claims[target.count++]={capability,action,0,false,true};return error::success;
        }
        error append_parent(binding& parent,handle id,origins& target)
        {
            std::shared_ptr<origins> value;auto status=active(parent,id,value);if(status!=error::success)return status;
            if(!target.count)return error::invalid;
            target.resources[target.count-1]=id;
            if(value->count>target.claims.size()-target.count)return error::too_large;
            for(std::size_t i{};i<value->count;++i)
            {target.owners[target.count]=value->owners[i];target.claims[target.count]=value->claims[i];target.resources[target.count++]=value->resources[i];}
            return error::success;
        }
        /** @brief Mint the actual call's complete retained origin token.
         * @details This is host-only state. The primary capability belongs to
         * this call, not to the endpoint's first opener. Every dependency owner
         * is resolved from the admitted immutable binding. No guest integer,
         * profile name or cached permission is sufficient to construct a token.
         */
        error operation(caller who,handle cap,std::string_view action,binding& selected,
                        std::shared_ptr<origins> const& origin,group_lease const& admitted,
                        communication::operation_token& output,handle linked=0)
        {
            using family=communication::operation_family;
            communication::operation_token value;
            value.caller_instance=who.instance;value.caller_generation=who.generation;
            value.authority=selected.identity;value.capability=cap;value.resource=origin->id;
            value.family=selected.kind==purpose::context?family::context:selected.kind==purpose::node?family::node:
                selected.kind==purpose::topic?family::topic:selected.kind==purpose::service?family::service:family::unconfigured;
            value.action=communication::parse_permission(action);value.retained_origins=origin;
            value.origins[0]={selected.identity,cap,origin->id,value.action};value.count=origin->count+1;
            for(std::size_t i{};i<origin->count;++i)
            {
                if(!cleanup_operation(value.action))
                {
                    auto grant=admitted.specification(i+1);if(!grant)return error::denied;
                    binding* dependency{};auto status=resolve(*grant,dependency);if(status!=error::success)return status;
                    if(dependency->identity!=origin->owners[i])return error::denied;
                }
                value.origins[i+1]={origin->owners[i],origin->claims[i].capability,origin->resources[i],
                    communication::parse_permission(origin->claims[i].action)};
            }
            if(linked)
            {
                if(value.family!=family::service||value.action!=communication::permission::service_available||value.count!=5)return error::denied;
                value.secondary=communication::permission::service_submit;
                value.origins[value.count++]={selected.identity,linked,origin->id,value.secondary};
            }
            if(!value.valid(value.action,value.family,selected.identity,origin->id,false))return error::denied;
            output=std::move(value);return error::success;
        }
        template<typename Encoder> error output(caller who,handle cap,std::string_view action,origins const* origins,
            std::size_t input_bytes,std::size_t capacity,std::size_t& required,sink deliver,Encoder&& encode,bool charge=true)
        {
            auto measure=wire::writer::measuring(limits_.output_bytes);
            if(!encode(measure))return error::too_large;
            required=measure.size();if(capacity<required)return error::too_large;
            if(required>UINT64_MAX-input_bytes)return error::too_large;
            workspace_lease reservation;auto status=reserve(required,reservation);if(status!=error::success)return status;
            byte_buffer bytes;bytes.resize_for_overwrite(required);wire::writer writer{bytes.writable()};
            if(!encode(writer)||writer.size()!=required)return error::conflict;
            group_lease admitted;status=group(who,cap,action,input_bytes+required,origins,admitted,charge);
            if(status!=error::success)return status;
            if(!deliver(bytes.view()))return error::io;
            group_lease after;return group(who,cap,action,0,origins,after,false);
        }
        error begin_open(binding& value)
        {
            std::lock_guard lock{mutex_};if(value.revoked||retired_)return error::cancelled;
            if(value.opening||value.live)return error::again;
            value.opening=true;return error::success;
        }
        void end_open(binding& value) noexcept{std::lock_guard lock{mutex_};value.opening=false;}
        void rollback_open(binding& value,std::shared_ptr<origins> const& opened) noexcept
        {
            auto status=core_->close(value.identity,opened->id);
            std::lock_guard lock{mutex_};
            if(status==communication::error::ok){if(value.live==opened)value.live.reset();}
            else{opened->orphan=true;opened->published=false;value.live=opened;}
        }
    public:
        static error convert(communication::error value) noexcept
        {
            switch(value)
            {
            case communication::error::ok:return error::success;case communication::error::io:return error::io;
            case communication::error::bad_handle:return error::bad_handle;case communication::error::again:case communication::error::busy:return error::again;
            case communication::error::no_memory:return error::no_memory;case communication::error::denied:return error::denied;
            case communication::error::invalid:return error::invalid;case communication::error::too_large:return error::too_large;
            case communication::error::unsupported:return error::unsupported;case communication::error::timeout:return error::timeout;
            case communication::error::stale:return error::conflict;case communication::error::cancelled:return error::cancelled;
            }
            return error::io;
        }
        communication_dispatch(std::shared_ptr<authority> permissions,std::uint64_t environment,std::uint64_t generation,limits capacity):
            permissions_(std::move(permissions)),environment_(environment),generation_(generation),limits_(capacity)
        {bindings_.reserve(capacity.bindings);}
        communication_dispatch(communication_dispatch const&)=delete;communication_dispatch& operator=(communication_dispatch const&)=delete;
        ~communication_dispatch(){retire();}
        error bind(::uwvm2::utils::container::std_string alias,communication::context_plan value){return bind_impl(std::move(alias),std::move(value),purpose::context);}
        error bind(::uwvm2::utils::container::std_string alias,communication::node_plan value){return bind_impl(std::move(alias),std::move(value),purpose::node);}
        error bind(::uwvm2::utils::container::std_string alias,communication::type_plan value){return bind_impl(std::move(alias),std::move(value),purpose::type);}
        error bind(::uwvm2::utils::container::std_string alias,communication::topic_plan value){return bind_impl(std::move(alias),std::move(value),purpose::topic);}
        error bind(::uwvm2::utils::container::std_string alias,communication::service_plan value){return bind_impl(std::move(alias),std::move(value),purpose::service);}
        /** @brief Copy one immutable typed plan for trusted provider preparation.
         * @details The generated owner is available before prepare() so a sealed
         * worker factory can bind its explicit creation plans to the exact host
         * authority. This host-only method does not issue a capability, open a
         * resource, activate native discovery or accept a guest-supplied owner.
         * A failed copy leaves the output unchanged.
         */
        template<typename Plan> error describe_plan(std::string_view alias,Plan& output)const noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(retired_)return error::cancelled;
            try
            {
                for(auto const& row:bindings_)if(row.alias==alias)
                {
                    auto const* value=std::get_if<Plan>(&row.policy);if(!value)return error::invalid;
                    Plan copy=*value;output=std::move(copy);return error::success;
                }
                return error::bad_handle;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /** @brief Freeze all plans atomically; no resource is opened by prepare. */
        error prepare(std::shared_ptr<communication::backend> provider)
        {return prepare(std::move(provider),observation_clock{});}
        error prepare(std::shared_ptr<communication::backend> provider,observation_clock observed)
        {return prepare_mode(std::move(provider),observed,false);}
        /** @brief Require asynchronous topic/service creation with explicit receipts.
         * @details Context and trusted node staging retain their local setup
         * paths. Endpoint opens cannot fall back to the synchronous host factory.
         * Deployment must separately configure a finite creation ledger and
         * exact native plans; preparation performs no process or middleware I/O.
         */
        error prepare_async(std::shared_ptr<communication::backend> provider)
        {return prepare_mode(std::move(provider),observation_clock{},true);}
        error prepare_async(std::shared_ptr<communication::backend> provider,observation_clock observed)
        {return prepare_mode(std::move(provider),observed,true);}
        /** @brief Require explicit creation receipts for nodes and all endpoints.
         * @details A positive node_deadline_ns is mandatory. Context creation is
         * still logical and inert. Only a trusted creation coordinator may advance
         * native startup, using an accepted receipt and the full current origins.
         */
        error prepare_all_async(std::shared_ptr<communication::backend> provider)
        {return prepare_mode(std::move(provider),observation_clock{},true,true);}
        error prepare_all_async(std::shared_ptr<communication::backend> provider,observation_clock observed)
        {return prepare_mode(std::move(provider),observed,true,true);}
    private:
        error prepare_mode(std::shared_ptr<communication::backend> provider,observation_clock observed,bool asynchronous,bool nodes=false)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(ready_||retired_||!provider)return error::denied;
            if(!environment_||!generation_||!limits_.carrier_bytes||!limits_.output_bytes||!limits_.workspace_bytes||!limits_.workspace_calls||
                limits_.workspace_bytes>INT64_MAX||limits_.workspace_calls>INT64_MAX||
                (nodes&&(!limits_.node_deadline_ns||limits_.node_deadline_ns>INT64_MAX))||
                (asynchronous&&limits_.output_bytes<communication_open_contract::published_bytes))return error::invalid;
            try
            {
                auto next=::uwvm2::utils::container::make_shared<core>(std::move(provider),limits_.native);
                // Types precede endpoint validation regardless of CLI order.
                for(auto& entry:bindings_)if(entry.kind==purpose::type)
                {auto status=next->configure(std::get<communication::type_plan>(entry.policy));if(status!=communication::error::ok)return convert(status);}
                for(auto& entry:bindings_)if(entry.kind!=purpose::type)
                {auto status=std::visit([&](auto const& plan){return next->configure(plan);},entry.policy);if(status!=communication::error::ok)return convert(status);}
                auto workspace=::uwvm2::utils::container::make_shared<workspace_account>();
                workspace->maximum=limits_.workspace_bytes;workspace->calls_max=limits_.workspace_calls;
                workspace_=std::move(workspace);core_=std::move(next);observation_=observed;endpoints_async_=asynchronous;nodes_async_=nodes;ready_=true;return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}
        }
    public:
        /** @brief Retain one exact asynchronous node draft without starting a process.
         * @details The context must be a live published object under its original
         * capability. The complete key/deadline carrier identifies every startup
         * retry. No native token or resource is minted here. The creation ledger
         * reserves canonical bytes and charges once before advance_node is legal.
         */
        error stage_node_open(caller who,handle cap,wire::bytes input,std::size_t capacity,std::shared_ptr<class opening>& output)
        {
            if(!same_process()||!permissions_)return error::denied;
            if(input.size()>limits_.carrier_bytes||capacity<communication_open_contract::receipt_bytes)return error::too_large;
            if(capacity>limits_.output_bytes)return error::invalid;
            wire::bytes semantic;communication_open_contract::creation suffix;communication_contract::request request;
            if(!communication_open_contract::split_creation(input,semantic,suffix)||!communication_contract::decode(op::node_open,semantic,request))return error::invalid;
            lease allowed;auto status=permissions_->admit(who,cap,"node.open",0,allowed,false);if(status!=error::success)return status;
            binding* row{};status=resolve(allowed.specification(),row);if(status!=error::success)return status;
            if(!nodes_async_||row->kind!=purpose::node)return error::denied;
            auto const& plan=std::get<communication::node_plan>(row->policy);
            if(!communication_contract::matches(request.profile,plan.selected))return error::denied;
            auto canonical=buffers::hash(input);
            {
                std::lock_guard lock{mutex_};if(retired_||row->revoked)return error::cancelled;
                if(row->pending)
                {
                    auto const& current=*row->pending;
                    if(!communication_creation::same_caller(current.original.who,who)||current.original.origins[0].capability!=cap||
                        current.original.key!=suffix.key||current.original.deadline!=suffix.deadline||current.carrier_bytes!=input.size()||current.canonical!=canonical)return error::conflict;
                    output=row->pending;return error::success;
                }
                if(row->live||row->opening)return error::again;
                if(row->last_creation_key==UINT64_MAX||suffix.key!=row->last_creation_key+1)return error::conflict;
                if(!allowed.specification().max_resources)return error::too_large;
                auto now=communication::now();if(suffix.deadline<=now)return error::timeout;
                if(suffix.deadline-now>limits_.node_deadline_ns)return error::invalid;
                row->opening=true;
            }
            struct finish{communication_dispatch* self;binding* row;~finish(){self->end_open(*row);}} done{this,row};
            try
            {
                auto pending=::uwvm2::utils::container::make_shared<class opening>();pending->alias=row->alias;pending->policy=plan;
                pending->family=communication_open_contract::endpoint_kind::node;pending->canonical=canonical;pending->carrier_bytes=input.size();
                pending->source=::uwvm2::utils::container::make_shared<origins>();auto& origin=*pending->source;binding* context{};
                status=add_origin(who,request.origin_capability,"context.open",purpose::context,origin,context);if(status!=error::success)return status;
                if(selected(*context)!=plan.context)return error::denied;
                status=append_parent(*context,request.resource,origin);if(status!=error::success)return status;
                if(origin.count!=1)return error::denied;
                group_lease held;status=group(who,cap,"node.open",0,&origin,held,false);if(status!=error::success)return status;
                auto& identity=pending->original;identity.who=who;identity.authority=row->identity;identity.key=suffix.key;identity.deadline=suffix.deadline;identity.count=2;
                identity.origins[0]={communication_creation::dependency::operation,row->identity,cap,0};
                identity.origins[1]={communication_creation::dependency::context,context->identity,request.origin_capability,request.resource};
                pending->node_stage={who.instance,who.generation,cap,request.origin_capability,request.resource,context->identity,canonical};
                std::lock_guard lock{mutex_};if(retired_||row->revoked)return error::cancelled;
                row->live=pending->source;row->pending=pending;output=std::move(pending);return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /** @brief Advance one already-admitted node startup without a guest copy.
         * @details The coordinator must first mark the accepted creation as
         * sending with cleanup debt. The exact original authority is reacquired
         * for each bounded prepare/create step. Waiting and process retirement
         * belong outside these leases. A successful native node remains private
         * and yields opaque actual-node evidence; publication is separate.
         */
        error advance_node(std::shared_ptr<class opening> const& pending,communication::node_attachment& output)
        {
            if(!same_process()||!pending||pending->family!=communication_open_contract::endpoint_kind::node||!pending->request())return error::denied;
            std::unique_lock serial{pending->submission,std::try_to_lock};if(!serial.owns_lock())return error::again;
            group_lease held;auto status=admit_open(pending->original,0,held);if(status!=error::success)return status;
            if(communication::now()>=pending->original.deadline)return error::timeout;
            auto const& policy=std::get<communication::node_plan>(pending->policy);
            if(pending->resource())return convert(core_->node_evidence(policy.authority,pending->resource(),output));
            auto const& stage=pending->node_stage;
            status=convert(core_->prepare_node(policy.authority,stage.context_authority,stage.context,policy.selected,stage));if(status!=error::success)return status;
            handle resource{};status=convert(core_->open_node(policy.authority,stage.context_authority,stage.context,policy.selected,resource));if(status!=error::success)return status;
            {
                std::lock_guard lock{mutex_};pending->private_resource=pending->source->id=resource;
                // Preserve a created object even if later evidence is absent.
                // It cannot be published, but still requires explicit cleanup.
                pending->adopted=true;
            }
            return convert(core_->node_evidence(policy.authority,resource,output));
        }
        /** @brief Retire only a private node after the coordinator accepts cancellation.
         * @details Closing the host object is not proof of namespace cleanup.
         * The creation ledger still retains any native or unknown debt, and the
         * supervisor must reap the exact worker before confirming cleanup.
         * Published nodes require separately authorized resource.close instead.
         */
        error discard_private_node(std::shared_ptr<class opening> const& pending)
        {
            if(!same_process()||!pending||pending->family!=communication_open_contract::endpoint_kind::node)return error::denied;
            std::unique_lock serial{pending->submission,std::try_to_lock};if(!serial.owns_lock())return error::again;
            {std::lock_guard lock{mutex_};auto* row=named(pending->alias);if(!row||row->pending!=pending||pending->source->published)return error::denied;}
            if(pending->resource()){auto status=convert(core_->close(pending->original.authority,pending->resource()));if(status!=error::success)return status;}
            std::lock_guard lock{mutex_};auto* row=named(pending->alias);if(row&&row->live==pending->source)row->live.reset();
            pending->source->id=0;pending->private_resource.store(0,std::memory_order_release);pending->adopted=false;return error::success;
        }
        /** @brief Stage a canonical endpoint open without charging or native effects.
         * @details Exact profile, role, resolved name, ordered schemas and actual
         * node/context origins are resolved under current authority. A short
         * output, zero resource capacity, invalid deadline or changed retry
         * cannot reserve anything. An accepted draft owns one registry slot;
         * the owning creation ledger subsequently stores the complete carrier.
         * Only the exact caller, capability, key and canonical bytes can replay.
         */
        error stage_open(caller who,op action,handle cap,wire::bytes input,std::size_t capacity,std::shared_ptr<class opening>& output)
        {
            if(action==op::node_open)return stage_node_open(who,cap,input,capacity,output);
            if(!same_process()||!permissions_)return error::denied;
            if(action!=op::topic_open&&action!=op::service_open)return error::unsupported;
            if(input.size()>limits_.carrier_bytes)return error::too_large;
            if(capacity<communication_open_contract::receipt_bytes)return error::too_large;
            if(capacity>limits_.output_bytes)return error::invalid;
            wire::bytes semantic;communication_open_contract::creation suffix;communication_contract::request request;
            if(!communication_open_contract::split_creation(input,semantic,suffix)||!communication_contract::decode(action,semantic,request))return error::invalid;
            auto name=action==op::topic_open?"topic.open":"service.open";
            lease allowed;auto status=permissions_->admit(who,cap,name,0,allowed,false);if(status!=error::success)return status;
            binding* row{};status=resolve(allowed.specification(),row);if(status!=error::success)return status;
            if(!endpoints_async_||!kind_matches(row->kind,action))return error::denied;
            auto topic=std::get_if<communication::topic_plan>(&row->policy);
            auto service=std::get_if<communication::service_plan>(&row->policy);
            if(!communication_contract::matches(request.endpoint.profile,selected(*row))||
                request.endpoint.schema_count!=(topic?1u:2u)||request.endpoint.name!=(topic?topic->resolved_name:service->resolved_name)||
                request.endpoint.role!=(topic?(topic->direction==communication::role::publisher?"publisher":"subscription"):
                    (service->direction==communication::service_role::client?"client":"server")))return error::denied;
            if(service&&request.endpoint.type!=service->service_type)return error::denied;
            auto canonical=buffers::hash(input);
            {
                std::lock_guard lock{mutex_};if(retired_||row->revoked)return error::cancelled;
                if(row->pending)
                {
                    auto const& current=*row->pending;
                    if(!communication_creation::same_caller(current.original.who,who)||current.original.origins[0].capability!=cap||
                        current.original.key!=suffix.key||current.original.deadline!=suffix.deadline||current.carrier_bytes!=input.size()||current.canonical!=canonical)return error::conflict;
                    output=row->pending;return error::success;
                }
                if(row->live||row->opening)return error::again;
                if(row->last_creation_key==UINT64_MAX||suffix.key!=row->last_creation_key+1)return error::conflict;
                if(!allowed.specification().max_resources)return error::too_large;
                auto now=communication::now();if(suffix.deadline<=now)return error::timeout;
                if(suffix.deadline-now>(topic?topic->max_deadline_ns:service->max_deadline_ns))return error::invalid;
                row->opening=true;
            }
            struct finish{communication_dispatch* self;binding* row;~finish(){self->end_open(*row);}} done{this,row};
            try
            {
                auto pending=::uwvm2::utils::container::make_shared<class opening>();pending->alias=row->alias;
                if(topic)pending->policy=*topic;else pending->policy=*service;
                pending->family=topic?communication_open_contract::endpoint_kind::topic:communication_open_contract::endpoint_kind::service;
                pending->canonical=canonical;pending->carrier_bytes=input.size();pending->source=::uwvm2::utils::container::make_shared<origins>();
                auto& origin=*pending->source;binding* parent{};
                status=add_origin(who,request.endpoint.node_capability,"node.open",purpose::node,origin,parent);if(status!=error::success)return status;
                status=append_parent(*parent,request.endpoint.node,origin);if(status!=error::success)return status;
                if(origin.count!=2)return error::denied;
                for(std::size_t i{};i<request.endpoint.schema_count;++i)
                {
                    binding* schema{};status=add_origin(who,request.endpoint.schema_capabilities[i],"type.describe",purpose::type,origin,schema);
                    if(status!=error::success)return status;
                    auto const& expected=topic?topic->type:i?service->response_type:service->request_type;
                    if(selected(*schema)!=expected)return error::denied;
                    pending->schemas[i]=std::get<communication::type_plan>(schema->policy);
                }
                if(topic&&request.endpoint.type!=pending->schemas[0].type_identity)return error::denied;
                group_lease held;status=group(who,cap,name,0,&origin,held,false);if(status!=error::success)return status;
                status=attach_node(who,request.endpoint.node_capability,request.endpoint.node,topic?topic->node:service->node,pending->parent);
                if(status!=error::success)return status;
                auto& identity=pending->original;identity.who=who;identity.authority=row->identity;identity.key=suffix.key;identity.deadline=suffix.deadline;identity.count=origin.count+1;
                identity.origins[0]={communication_creation::dependency::operation,row->identity,cap,0};
                for(std::size_t i{};i<origin.count;++i)identity.origins[i+1]={i==0?communication_creation::dependency::node:
                    i==1?communication_creation::dependency::context:communication_creation::dependency::schema,origin.owners[i],origin.claims[i].capability,origin.resources[i]};
                status=convert(core_->prepare_adoption(row->identity,pending->parent,selected(*row),topic?communication::operation_family::topic:
                    communication::operation_family::service,pending->slot));if(status!=error::success)return status;
                origin.id=pending->private_resource=pending->slot.resource();
                std::lock_guard lock{mutex_};
                if(retired_||row->revoked){core_->abandon_adoption(pending->slot,true);return error::cancelled;}
                row->live=pending->source;row->pending=pending;output=std::move(pending);return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /** @brief Validate actual retained resources before charging exact creation bytes.
         * @details Called only by the trusted ledger after its complete capacity
         * reservation. Existing dependency rates are validation-only. A forged
         * owner, resource, schema order, deadline or byte charge cannot match the
         * immutable private draft. The returned lease covers one nonblocking
         * send/publication and must never span a worker wait.
         */
        error admit_open(communication_creation::identity const& identity,std::uint64_t bytes,group_lease& held,bool charge=false)
        {
            if(!same_process()||held)return error::denied;
            std::shared_ptr<class opening> pending;binding* row{};
            {std::lock_guard lock{mutex_};if(retired_)return error::cancelled;
                for(auto& value:bindings_)if(value.identity==identity.authority){row=&value;pending=value.pending;break;}}
            if(!row||!pending||pending->original!=identity)return error::denied;
            if(charge?bytes!=pending->carrier_bytes+communication_open_contract::receipt_bytes:bytes!=0)return error::invalid;
            communication_creation::claim_set claims;group_lease validated;
            auto status=permissions_->admit_group(identity.who,communication_creation::admission_claims(identity,pending->family,claims),validated,false);
            if(status!=error::success)return status;
            status=validate_origins(*pending->source,validated);if(status!=error::success)return status;
            {std::lock_guard lock{mutex_};if(retired_||row->revoked||row->pending!=pending||row->live!=pending->source||pending->source->orphan)return error::cancelled;}
            if(!charge){held=std::move(validated);return error::success;}
            return permissions_->admit_group(identity.who,communication_creation::admission_claims(identity,pending->family,claims,bytes),held,true);
        }
        /** @brief Retain an exact host-minted draft for its trusted coordinator.
         * @details This cached lookup grants no authority and performs no I/O.
         * Only a matching immutable identity may recover the owning pending
         * object; guest input cannot construct or replace its private state.
         */
        error retain_open(communication_creation::identity const& original,std::shared_ptr<class opening>& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(retired_)return error::cancelled;
            for(auto const& row:bindings_)if(row.identity==original.authority&&row.pending&&row.pending->original==original&&!row.revoked)
            {output=row.pending;return error::success;}
            return error::denied;
        }
        /** @brief Compare the actual authority environment before retaining two activation leases.
         * @details Numeric environment equality alone does not prove a shared
         * authority lock. This host-only comparison prevents cross-environment
         * lock ordering and cannot issue a guest capability.
         */
        bool shares_authority(communication_dispatch const& other)const noexcept
        {return same_process()&&other.same_process()&&permissions_&&permissions_==other.permissions_&&environment_==other.environment_;}
        /** @brief Admit an exact accepted node startup before enabling its private transport.
         * @details Every plan field and the full canonical request digest must
         * match the dispatcher-owned pending draft. A foreign numeric context,
         * caller, capability, incarnation or profile cannot supply authority.
         * This performs no I/O and never charges a second creation unit. The
         * caller retains the returned lease only across bounded link activation.
         */
        error admit_node_activation(communication::node_plan const& node,communication::context_plan const& context,
                                   communication::node_stage_request const& request,group_lease& held)noexcept
        {
            if(!same_process()||held)return error::denied;
            try
            {
                std::shared_ptr<class opening> pending;
                {
                    std::lock_guard lock{mutex_};if(!ready_||retired_)return error::denied;
                    binding const* parent{};
                    for(auto const& row:bindings_)
                    {
                        if(row.identity==node.authority&&row.kind==purpose::node&&!row.revoked)pending=row.pending;
                        if(row.identity==context.authority&&row.kind==purpose::context&&!row.revoked)parent=&row;
                    }
                    if(!pending||!parent||pending->family!=communication_open_contract::endpoint_kind::node||!pending->request()||
                        request!=pending->node_stage||request.context_authority!=context.authority)return error::denied;
                    auto const& actual=std::get<communication::node_plan>(pending->policy);
                    auto const& original=std::get<communication::context_plan>(parent->policy);
                    if(actual.authority!=node.authority||actual.selected!=node.selected||actual.context!=node.context||actual.name!=node.name||
                        actual.name_space!=node.name_space||actual.max_observers!=node.max_observers||node.context!=context.selected||
                        original.selected!=context.selected||original.domain!=context.domain||original.enclave!=context.enclave||
                        original.network!=context.network)return error::denied;
                }
                if(communication::now()>=pending->original.deadline)return error::timeout;
                return admit_open(pending->original,0,held);
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /// @brief Record only the owning ledger's actual request and provider incarnation.
        error record_open(std::shared_ptr<class opening> const& pending,communication_creation::view const& result)
        {
            if(!same_process()||!pending)return error::denied;
            std::lock_guard lock{mutex_};auto* row=named(pending->alias);
            if(!row||row->pending!=pending||retired_||row->revoked||result.reserved||result.kind!=pending->family||
                result.receipt.key!=pending->original.key||!result.receipt.request_id||!result.provider_generation)return error::denied;
            if((pending->request()&&pending->request()!=result.receipt.request_id)||
                (pending->provider_generation&&pending->provider_generation!=result.provider_generation))return error::conflict;
            pending->provider_generation=result.provider_generation;pending->creation_request.store(result.receipt.request_id,std::memory_order_release);return error::success;
        }
        /** @brief Execute a canonical asynchronous topic/service.open transaction.
         * @details The 48-byte receipt is checked before staging. Failed enqueue
         * discards only a proven uncommitted registry reservation. Failed output
         * retains the accepted request for exact replay without another charge,
         * including after the parent capability closes. Replay still requires
         * the current caller's primary open capability. No resource is published.
         */
        error invoke_open(caller who,op action,handle cap,wire::bytes input,std::size_t capacity,std::size_t& required,sink deliver,
                          asynchronous_factory provider,::uwvm2::utils::container::function_ref<bool()> cancelled={})noexcept
        {
            required=communication_open_contract::receipt_bytes;
            try
            {
                if(!same_process()||!provider.enqueue||!provider.inspect||!deliver)return error::denied;
                if(cancelled&&cancelled())return error::cancelled;
                std::shared_ptr<class opening> pending;auto status=stage_open(who,action,cap,input,capacity,pending);if(status!=error::success)return status;
                std::unique_lock serial{pending->submission,std::try_to_lock};if(!serial.owns_lock())return error::again;
                bool accepted=bool(pending->request());
                auto abandon=[&]
                {
                    std::lock_guard lock{mutex_};auto* row=named(pending->alias);
                    if(!accepted&&row&&row->pending==pending&&!pending->request()&&!pending->adopted&&
                        (!pending->slot||core_->abandon_adoption(pending->slot,true)==communication::error::ok))
                    {if(row->live==pending->source)row->live.reset();row->pending.reset();pending->parent.reset();}
                };
                struct rollback{decltype(abandon)& discard;~rollback(){discard();}} guard{abandon};
                if(cancelled&&cancelled())return error::cancelled;
                communication_creation::view receipt;
                status=convert(pending->request()?provider.inspect(provider.context,*pending,receipt):provider.enqueue(provider.context,*pending,input,receipt));
                if(status!=error::success)return status;
                accepted=true;
                status=record_open(pending,receipt);if(status!=error::success)return status;
                std::array<std::uint8_t,communication_open_contract::receipt_bytes> storage;wire::writer out{storage};
                if(!communication_open_contract::encode(out,receipt.receipt)||out.size()!=required)return error::io;
                auto name=action==op::node_open?"node.open":action==op::topic_open?"topic.open":"service.open";
                lease allowed;status=permissions_->admit(who,cap,name,0,allowed,false);if(status!=error::success)return status;
                if(cancelled&&cancelled())return error::cancelled;
                if(!deliver(out.written()))return error::io;
                if(cancelled&&cancelled())return error::cancelled;
                lease after;return permissions_->admit(who,cap,name,0,after,false);
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /** @brief Transfer a counted actual node to the verified endpoint facade.
         * @details Failed facade construction can reacquire a counted lease.
         * Exact complete authority is required on every attempt; no native
         * resource ID supplied by the guest participates in this operation.
         */
        error adoption_node(std::shared_ptr<class opening> const& pending,communication::node_lease& node)
        {
            if(!pending||node||pending->family==communication_open_contract::endpoint_kind::node)return error::invalid;
            group_lease held;auto status=admit_open(pending->original,0,held);if(status!=error::success)return status;
            {std::lock_guard lock{mutex_};auto* row=named(pending->alias);
                if(!row||row->pending!=pending||!pending->request())return error::denied;
                if(pending->parent){node=std::move(pending->parent);return error::success;}}
            auto const& source=*pending->source;communication::node_lease replacement;
            status=attach_node(pending->original.who,source.claims[0].capability,source.resources[0],
                std::visit([](auto const& plan)->profile const&{if constexpr(requires{plan.node;})return plan.node;else return plan.selected;},pending->policy),replacement);
            if(status!=error::success)return status;
            std::lock_guard lock{mutex_};auto* row=named(pending->alias);
            if(!row||row->pending!=pending||row->revoked||retired_)return error::cancelled;
            node=std::move(replacement);return error::success;
        }
        /// @brief Adopt a verified topic with the ledger's actual worker generation.
        error adopt_open(std::shared_ptr<class opening> const& pending,std::shared_ptr<communication::native_topic> endpoint)
        {
            if(!pending||!endpoint||pending->family!=communication_open_contract::endpoint_kind::topic)return error::denied;
            group_lease held;auto status=admit_open(pending->original,0,held);if(status!=error::success)return status;
            std::lock_guard lock{mutex_};if(!pending->request())return error::denied;
            if(pending->adopted)return pending->topic.lock()==endpoint?error::success:error::conflict;
            status=convert(core_->adopt_topic(pending->slot,endpoint,pending->provider_generation));
            if(status==error::success){pending->topic=endpoint;pending->adopted=true;}return status;
        }
        /// @brief Adopt a verified service into its reserved exact typed registry slot.
        error adopt_open(std::shared_ptr<class opening> const& pending,std::shared_ptr<communication::native_service> endpoint)
        {
            if(!pending||!endpoint||pending->family!=communication_open_contract::endpoint_kind::service)return error::denied;
            group_lease held;auto status=admit_open(pending->original,0,held);if(status!=error::success)return status;
            std::lock_guard lock{mutex_};if(!pending->request())return error::denied;
            if(pending->adopted)return pending->service.lock()==endpoint?error::success:error::conflict;
            status=convert(core_->adopt_service(pending->slot,endpoint,pending->provider_generation));
            if(status==error::success){pending->service=endpoint;pending->adopted=true;}return status;
        }
        /** @brief Publish only an adopted resource from the exact creation receipt.
         * @details The trusted provider first publishes its verified receipt
         * under the full current origin lease. This allocation-free step makes
         * the private host resource usable. A failed guest copy never creates a
         * second object and may recover the same retained published receipt.
         */
        error publish_open(std::shared_ptr<class opening> const& pending,communication_open_contract::published const& receipt,group_lease const& held)
        {
            if(!same_process()||!pending)return error::denied;
            communication_creation::claim_set claims;
            if(!held.matches(pending->original.who,communication_creation::admission_claims(pending->original,pending->family,claims))||
                receipt.kind!=pending->family||receipt.handle!=pending->resource()||receipt.creation.request_id!=pending->request()||
                receipt.creation.key!=pending->original.key||receipt.creation.status!=communication_open_contract::state::published||receipt.creation.result!=communication::error::ok||
                receipt.creation.observed==communication::effect::unknown||receipt.creation.cleanup_debt)return error::denied;
            auto status=validate_origins(*pending->source,held);if(status!=error::success)return status;
            std::lock_guard lock{mutex_};auto* row=named(pending->alias);
            if(!row||row->pending!=pending||row->live!=pending->source||!pending->adopted||pending->source->orphan||row->revoked||retired_)return error::denied;
            pending->source->published=true;return error::success;
        }
        /** @brief Release a draft only after positively established native cleanup.
         * @details A live published resource must first close independently.
         * Unknown outcomes, unconfirmed debt and orphaned resources keep their
         * capacity. Unsent cancelled drafts may abandon their private registry
         * reservation. Release remains available after parent/grant revocation;
         * the outer control dispatcher supplies independent cleanup authority.
         */
        error release_open(std::shared_ptr<class opening> const& pending,communication_creation::view const& result)
        {
            if(!same_process()||!pending)return error::denied;
            std::lock_guard lock{mutex_};auto* row=named(pending->alias);
            if(!row||result.kind!=pending->family||result.receipt.request_id!=pending->request()||
                result.provider_generation!=pending->provider_generation||result.receipt.key!=pending->original.key||
                result.receipt.status!=communication_open_contract::state::released||result.endpoint_live||result.receipt.cleanup_debt||
                result.receipt.observed==communication::effect::unknown)return error::again;
            // A failed guest copy may replay release after the private slot was
            // removed. The exact owned draft remembers only verified release.
            if(pending->released)return error::success;
            if(row->pending!=pending||pending->source->orphan||(pending->adopted&&row->live==pending->source))return error::again;
            if(pending->slot){auto status=convert(core_->abandon_adoption(pending->slot,true));if(status!=error::success)return status;}
            if(row->live==pending->source)row->live.reset();
            row->last_creation_key=pending->original.key;row->pending.reset();pending->parent.reset();pending->released=true;return error::success;
        }
        error preflight(caller who,op action,handle cap,std::size_t extent)
        {
            if(!same_process())return error::denied;
            auto index=static_cast<std::size_t>(action);if(index>=communication_contract::operations.size())return error::unsupported;
            if(!permissions_)return error::denied;
            if(extent>limits_.carrier_bytes)return error::too_large;
            // Recovery may follow consumption of the final permitted mutation.
            // Its bounded input must be decoded before distinguishing an exact
            // retry from a new effect. New effects still charge at publication.
            bool mutation=action==op::topic_publish||action==op::service_submit||action==op::service_respond||action==op::context_shutdown||
                action==op::qos_liveliness_assert||action==op::qos_acknowledgements_wait||action==op::service_abandon||
                action==op::qos_describe||action==op::service_available||
                (nodes_async_&&action==op::node_open)||
                (endpoints_async_&&(action==op::topic_open||action==op::service_open));
            lease granted;auto status=mutation?permissions_->admit(who,cap,communication_contract::operations[index].name,0,granted,false):
                permissions_->preflight(who,cap,communication_contract::operations[index].name,extent,granted);
            if(status!=error::success)return status;
            binding* selected{};status=resolve_observation(granted.specification(),communication_contract::operations[index].name,selected);
            return status==error::success&&!kind_matches(selected->kind,action)?error::denied:status;
        }
        error reserve_workspace(caller who,op action,handle cap,std::size_t extent,workspace_lease& output)
        {auto status=preflight(who,action,cap,extent);return status==error::success?reserve(extent,output):status;}
        /** @brief Revalidate queued work against the current complete resource cohort.
         * @details A supervisor calls this without holding provider/queue locks.
         * The returned validation-only lease protects a bounded nonblocking
         * write, never a reply wait. Closure of the actual caller capability,
         * parent, schema or resource invalidates the token. A matching alias or
         * owner cannot revive a retired resource or substitute another origin.
         * The caller must release the lease before receiving or waiting on IPC.
         */
        error revalidate(communication::operation_token const& token,group_lease& admitted)
        {
            if(!same_process()||!permissions_||admitted)return error::denied;
            if(!token.valid(token.action,token.family,token.authority,token.resource,false))return error::denied;
            auto name=communication::permission_name(token.action);
            auto descriptor=std::find_if(communication_contract::operations.begin(),communication_contract::operations.end(),
                [&](auto const& entry){return entry.name==name;});
            bool receive=receive_matches(purpose::topic,token.action)||receive_matches(purpose::service,token.action);
            bool cleanup=cleanup_operation(token.action);
            bool closing=token.action==communication::permission::resource_close;
            if(!receive&&!closing&&descriptor==communication_contract::operations.end())return error::denied;
            caller who{token.caller_instance,token.caller_generation};
            lease allowed;auto status=permissions_->admit(who,token.capability,name,0,allowed,false);if(status!=error::success)return status;
            binding* selected{};status=resolve_observation(allowed.specification(),name,selected);
            bool linked=service_observer(allowed.specification());
            if(linked!=(token.secondary==communication::permission::service_submit))return error::denied;
            if(status!=error::success||selected->identity!=token.authority||
                (closing?(selected->kind!=purpose::topic&&selected->kind!=purpose::service):
                    receive?!receive_matches(selected->kind,token.action):!kind_matches(selected->kind,descriptor->id)))return error::denied;
            std::shared_ptr<origins> origin;status=active(*selected,token.resource,origin);if(status!=error::success)return status;
            if(origin!=token.retained_origins)return error::denied;
            {std::lock_guard lock{mutex_};if(origin->receive_unknown)return error::conflict;}
            group_lease fresh;
            if(cleanup)
            {admission_claim claim{token.capability,name,0,false,false,true};status=permissions_->admit_group(who,{&claim,1},fresh,false);}
            else if(linked)status=availability_group(who,token.capability,*selected,token.origins[5].capability,*origin,0,fresh);
            else status=group(who,token.capability,name,0,origin.get(),fresh,false);
            if(status!=error::success)return status;
            communication::operation_token actual;status=operation(who,token.capability,name,*selected,origin,fresh,actual,linked?token.origins[5].capability:0);
            if(status!=error::success||actual.family!=token.family||actual.count!=token.count||actual.origins!=token.origins)return error::denied;
            admitted=std::move(fresh);return error::success;
        }
        /** @brief Charge one pre-reserved queued effect with its complete actual origins.
         * @details Only a trusted queue adapter calls this after owning all RPC
         * bytes and reply capacity. The current zero-cost validation group stays
         * held while the charged group is acquired. Dependency grants remain
         * validation-only. No native I/O or reply wait is permitted here; the
         * queue must consume this group once and the outer dispatcher must not
         * independently debit the same effect.
         */
        error charge_operation(communication::operation_token const& token,std::uint64_t bytes,group_lease& admitted)
        {
            if(admitted)return error::denied;
            group_lease validated;auto status=revalidate(token,validated);if(status!=error::success)return status;
            if(cleanup_operation(token.action))
            {
                admission_claim claim{token.capability,communication::permission_name(token.action),0,false,false,true};
                return permissions_->admit_group({token.caller_instance,token.caller_generation},{&claim,1},admitted,true);
            }
            std::array<admission_claim,8> claims{};
            for(std::size_t i{};i<token.count;++i)claims[i]={token.origins[i].capability,communication::permission_name(token.origins[i].action),i?0:bytes,false,i!=0};
            return permissions_->admit_group({token.caller_instance,token.caller_generation},{claims.data(),token.count},admitted,true);
        }
        /** @brief Mint one exact receive operation, retaining original owners after parent closure.
         * @details RESERVE and ACK require all current node/context/schema grants.
         * RELEASE admits only the explicit current cleanup capability and checks
         * the retained immutable cohort. It cannot read payloads or mint another
         * resource. This host-only entry performs no charge, IPC or guest copy.
         */
        error retain_receive(caller who,handle cap,handle id,communication::permission action,communication::operation_token& output)
        {
            if(!same_process()||!permissions_)return error::denied;
            auto name=communication::permission_name(action);lease allowed;
            auto status=permissions_->admit(who,cap,name,0,allowed,false);if(status!=error::success)return status;
            binding* selected{};status=resolve(allowed.specification(),selected);
            if(status!=error::success||!receive_matches(selected->kind,action))return error::denied;
            std::shared_ptr<origins> origin;status=active(*selected,id,origin);if(status!=error::success)return status;
            {std::lock_guard lock{mutex_};if(origin->receive_unknown)return error::conflict;}
            group_lease admitted;
            if(cleanup_operation(action))
            {admission_claim claim{cap,name,0,false,false,true};status=permissions_->admit_group(who,{&claim,1},admitted,false);}
            else status=group(who,cap,name,0,origin.get(),admitted,false);
            return status==error::success?operation(who,cap,name,*selected,origin,admitted,output):status;
        }
        /** @brief Inspect an owned topic's cleanup debt without issuing another receive.
         * @details This trusted provider query uses independent topic.release
         * authority, preserves unknown outcomes and performs no traffic charge.
         * It is not a guest import and cannot disclose or consume a message.
         */
        error topic_receive_quiescent(caller who,handle cap,handle id,bool& output)
        {
            output=false;communication::operation_token token;
            auto status=retain_receive(who,cap,id,communication::permission::topic_release,token);
            if(status!=error::success)return status;
            group_lease current;status=revalidate(token,current);if(status!=error::success)return status;
            return convert(core_->topic_receive_quiescent(token,output));
        }
        /** @brief Validate a fixed receive carrier before copying any guest bytes.
         * @details Retrying an already admitted reservation or settlement must
         * not require another traffic unit. Scope and exact retry checks occur
         * after the complete bounded carrier has been copied by the VM wrapper.
         */
        error preflight_receive(caller who,communication_receive_contract::operation action,handle cap,std::size_t extent)
        {
            namespace contract=communication_receive_contract;
            if(!same_process()||!permissions_)return error::denied;
            auto index=static_cast<std::size_t>(action);if(index>=contract::operations.size())return error::unsupported;
            if(extent!=(contract::reserves(action)?16u:48u)||extent>limits_.carrier_bytes)return error::invalid;
            auto name=contract::operations[index].name;lease allowed;
            auto status=permissions_->admit(who,cap,name,0,allowed,false);if(status!=error::success)return status;
            binding* selected{};status=resolve(allowed.specification(),selected);
            return status==error::success&&!receive_matches(selected->kind,communication::parse_permission(name))?error::denied:status;
        }
        /** @brief Reserve, acknowledge or release exactly one original endpoint record.
         * @details Full bounded output/workspace is reserved before provider
         * polling. Failed copies keep the original nonce and consume at most one
         * reserve admission. ACK/RELEASE reserve their complete receipt first;
         * exact replay neither repeats the effect nor charges another unit.
         * The actual provider incarnation is mandatory and independent of the
         * local queue generation. The retained mapping never accepts a guest's
         * native identity. An absent isolated receive adapter remains unsupported.
         */
        error invoke_receive(caller who,communication_receive_contract::operation action,handle cap,wire::bytes input,
                             std::size_t capacity,std::size_t& required,sink deliver,
                             ::uwvm2::utils::container::function_ref<bool()> cancelled={})noexcept
        {
            namespace contract=communication_receive_contract;
            required=0;if(!same_process())return error::denied;
            try
            {
                if(!deliver)return error::invalid;
                auto status=preflight_receive(who,action,cap,input.size());if(status!=error::success)return status;
                contract::request request;if(!contract::decode(action,input,request))return error::invalid;
                auto name=contract::operations[static_cast<std::size_t>(action)].name;lease allowed;
                status=permissions_->admit(who,cap,name,0,allowed,false);if(status!=error::success)return status;
                binding* selected{};status=resolve(allowed.specification(),selected);if(status!=error::success)return status;
                std::shared_ptr<origins> origin;status=active(*selected,request.reservation.endpoint,origin);if(status!=error::success)return status;
                communication::delivery_consumer receiver{who.instance,who.generation,cap};
                auto checkpoint=[&](){return cancelled&&cancelled()?error::cancelled:error::success;};
                if(checkpoint()!=error::success)return error::cancelled;
                bool topic=selected->kind==purpose::topic,acknowledge=contract::acknowledges(action);
                bool queued{};status=convert(core_->queued_receive(selected->identity,origin->id,queued));
                if(status!=error::success)return status;
                if(!contract::reserves(action))
                {
                    required=160;if(capacity<required||limits_.output_bytes<required)return error::too_large;
                    communication::receipt receipt;communication::delivery_reference native;bool replay{};
                    {
                        std::lock_guard lock{mutex_};
                        if(origin->receiver!=receiver)return error::denied;
                        if(origin->receive_receipt.request&&origin->receive_receipt.key==request.key)
                        {
                            if(origin->settled_receive!=request.reservation||origin->receive_ack!=acknowledge)return error::conflict;
                            receipt=origin->receive_receipt;replay=true;
                        }
                        else
                        {
                            if(origin->receive_unknown)return error::conflict;
                            if(origin->public_receive!=request.reservation)return error::denied;
                            native=origin->retained_receive;
                        }
                    }
                    if(!replay)
                    {
                        communication::operation_token authorized;status=retain_receive(who,cap,origin->id,communication::parse_permission(name),authorized);
                        if(status!=error::success)return status;
                        group_lease charged;status=queued?revalidate(authorized,charged):charge_operation(authorized,acknowledge?input.size()+160:0,charged);
                        if(status!=error::success)return status;
                        if(checkpoint()!=error::success)return error::cancelled;
                        auto host_request=resources::allocate_identity();if(!host_request)return error::too_large;
                        communication::delivery_receipt result;
                        auto outcome=topic?core_->settle_topic(selected->identity,origin->id,receiver,native,request.key,acknowledge,result,&authorized):
                            core_->settle_service(selected->identity,origin->id,receiver,native,request.key,acknowledge,result,&authorized);
                        bool uncertain=outcome==communication::error::io||outcome==communication::error::cancelled||!communication::known_status(outcome);
                        if(outcome!=communication::error::ok&&!uncertain)return convert(outcome);
                        uncertain=uncertain||!result.request||result.source_generation!=native.generation||result.key!=request.key||
                            !communication::known_status(result.status)||result.observed>communication::effect::unknown||
                            (result.status==communication::error::ok?result.observed!=communication::effect::locally_accepted:result.observed!=communication::effect::none);
                        receipt={host_request,request.reservation.provider_generation,request.key,
                            uncertain?communication::error::io:result.status,uncertain?communication::effect::unknown:result.observed};
                        std::lock_guard lock{mutex_};origin->receive_receipt=receipt;origin->settled_receive=request.reservation;
                        origin->receive_ack=acknowledge;
                        if(uncertain||result.status==communication::error::ok)origin->receive_charged=false;
                        origin->receive_unknown=uncertain;
                    }
                    std::array<std::uint8_t,160> body;wire::writer out{body};
                    if(!communication_contract::encode_receipt(out,receipt))return error::io;
                    required=out.size();lease current;status=permissions_->admit(who,cap,name,0,current,false);
                    if(status!=error::success||checkpoint()!=error::success)return status==error::success?error::cancelled:status;
                    if(!deliver(out.written()))return error::io;
                    status=permissions_->admit(who,cap,name,0,current,false);
                    return status==error::success?checkpoint():status;
                }
                {std::lock_guard lock{mutex_};if(origin->receive_unknown)return error::conflict;}
                communication::operation_token authorized;status=retain_receive(who,cap,origin->id,communication::parse_permission(name),authorized);
                if(status!=error::success)return status;
                communication::profile type;std::size_t bound{};bool response{};communication::endpoint_identity identity;
                if(topic)
                {
                    auto const& plan=std::get<communication::topic_plan>(selected->policy);
                    if(plan.direction!=communication::role::subscription)return error::denied;
                    type=plan.type;bound=plan.max_message_bytes;communication::topic_info info;
                    status=convert(core_->describe(selected->identity,origin->id,info));identity=info.native;
                }
                else
                {
                    auto const& plan=std::get<communication::service_plan>(selected->policy);response=plan.direction==communication::service_role::client;
                    type=response?plan.response_type:plan.request_type;bound=response?plan.max_response_bytes:plan.max_request_bytes;
                    communication::service_info info;status=convert(core_->describe(selected->identity,origin->id,info));identity=info.native;
                }
                if(status!=error::success)return status;
                if(!communication::valid(identity)||!identity.provider_generation)return error::unsupported;
                communication::type_plan const* schema{};
                for(auto const& entry:bindings_)if(entry.kind==purpose::type&&communication_dispatch::selected(entry)==type)schema=&std::get<communication::type_plan>(entry.policy);
                if(!schema||!schema->validate||bound>limits_.output_bytes)return error::denied;
                required=bound+schema->type_identity.size()+schema->encoding.size()+256;
                if(required>limits_.output_bytes||capacity<required)return error::too_large;
                bool recovering{};
                {std::lock_guard lock{mutex_};recovering=origin->receive_charged&&origin->receiver==receiver&&origin->receive_deadline==request.deadline;}
                if(!recovering&&!queued)
                {
                    lease budget;status=permissions_->preflight(who,cap,name,input.size()+required,budget);
                    if(status!=error::success)return status;
                }
                workspace_lease workspace;status=reserve(required,workspace);if(status!=error::success)return status;
                byte_buffer owned;owned.resize_for_overwrite(required);error failure=error::success;
                auto copy=[&](communication::delivery_reference native,std::uint64_t until,communication::metadata const& metadata,
                              handle correlation,bool actual_response,wire::bytes payload)
                {
                    if(until!=request.deadline||native.source!=origin->id||metadata.source!=native.source||metadata.generation!=native.generation||
                        metadata.sequence!=native.sequence||!native.sequence||!native.nonce||payload.size()>bound||!schema->validate(payload)||
                        (topic?correlation!=0:(!correlation||actual_response!=response)))
                    {failure=error::io;return false;}
                    contract::reference public_reference{origin->id,*identity.provider_generation,native.sequence,native.nonce};
                    wire::writer writer{owned.writable()};
                    if(!contract::encode(writer,public_reference,until,metadata,correlation,topic?"topic.message":response?"service.response":"service.request",
                        {schema->type_identity,schema->encoding,payload})){failure=error::too_large;return false;}
                    required=writer.size();bool charged{};
                    {
                        std::lock_guard lock{mutex_};charged=origin->receive_charged&&origin->public_receive==public_reference;
                        if(charged&&(origin->receiver!=receiver||origin->receive_deadline!=until)){failure=error::conflict;return false;}
                    }
                    group_lease held;failure=(charged||queued)?revalidate(authorized,held):charge_operation(authorized,input.size()+required,held);
                    if(failure!=error::success)return false;
                    {
                        std::lock_guard lock{mutex_};origin->receiver=receiver;origin->retained_receive=native;origin->public_receive=public_reference;
                        origin->receive_deadline=until;origin->receive_charged=true;
                    }
                    if(checkpoint()!=error::success){failure=error::cancelled;return false;}
                    if(!deliver(writer.written())){failure=error::io;return false;}
                    group_lease after;failure=revalidate(authorized,after);
                    if(failure==error::success)failure=checkpoint();
                    return failure==error::success;
                };
                communication::error result;
                if(topic)result=core_->reserve_topic(selected->identity,origin->id,receiver,request.deadline,
                    [&](auto ref,auto until,communication::delivery const& value){return copy(ref,until,value.information,0,false,value.body);},&authorized);
                else
                {
                    using callback=decltype(copy);auto invoke=[](void* context,communication::delivery_reference ref,std::uint64_t until,communication::service_delivery const& value)
                    {return (*static_cast<callback*>(context))(ref,until,value.information,value.correlation,value.response,value.body);};
                    result=core_->reserve_service(selected->identity,origin->id,receiver,request.deadline,&copy,invoke,&authorized);
                }
                return failure==error::success?convert(result):failure;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        /** @brief Apply trusted worker completion to the same retained origin.
         * @details No new permission or effect is granted. The supervisor calls
         * outside authority/queue locks and retains an again result for retry.
         * Revoked resources may retain their true final receipt for recovery.
         */
        error settle_operation(communication::operation_token const& token,communication::native_result result)noexcept
        {
            if(!same_process()||!core_)return error::denied;
            {
                std::lock_guard lock{mutex_};binding* selected{};
                for(auto& entry:bindings_)if(entry.identity==token.authority){selected=&entry;break;}
                if(!selected||!selected->live||selected->live->id!=token.resource||selected->live!=token.retained_origins)return error::denied;
            }
            return convert(core_->settle_operation(token,result));
        }
        /** @brief Retain a node's exact current capability and original context cohort.
         * @details Composite providers use this host-only token to preserve the
         * real context owner, resource and capability. It grants nothing and
         * holds no authority lease or observer slot after return. Revalidate it
         * before each effect; profile or owner equality alone is insufficient.
         */
        error retain_node_origins(caller who,handle cap,handle id,profile const& expected,
                                  communication::operation_token& output)
        {
            if(!same_process()||!permissions_)return error::denied;
            lease allowed;auto status=permissions_->admit(who,cap,"node.open",0,allowed,false);if(status!=error::success)return status;
            binding* node{};status=resolve(allowed.specification(),node);
            if(status!=error::success||node->kind!=purpose::node||selected(*node)!=expected)return error::denied;
            std::shared_ptr<origins> origin;status=active(*node,id,origin);if(status!=error::success)return status;
            group_lease all;status=group(who,cap,"node.open",0,origin.get(),all,false);if(status!=error::success)return status;
            status=convert(core_->validate_node(node->identity,id,expected));if(status!=error::success)return status;
            return operation(who,cap,"node.open",*node,origin,all,output);
        }
        /** @brief Attach a counted actual node under its original context authority. */
        error attach_node(caller who,handle cap,handle id,profile const& profile,communication::node_lease& output)
        {
            if(!same_process())return error::denied;
            lease allowed;auto status=permissions_->admit(who,cap,"node.open",0,allowed,false);if(status!=error::success)return status;
            binding* node{};status=resolve(allowed.specification(),node);if(status!=error::success||node->kind!=purpose::node)return error::denied;
            std::shared_ptr<origins> origin;status=active(*node,id,origin);if(status!=error::success)return status;
            group_lease all;status=group(who,cap,"node.open",0,origin.get(),all,false);if(status!=error::success)return status;
            return convert(core_->attach_node(node->identity,id,profile,output));
        }
        error validate_node(caller who,handle cap,handle id,profile const& profile,owner expected)
        {
            if(!same_process())return error::denied;
            lease allowed;auto status=permissions_->admit(who,cap,"node.open",0,allowed,false);if(status!=error::success)return status;
            binding* node{};status=resolve(allowed.specification(),node);if(status!=error::success||node->kind!=purpose::node||node->identity!=expected)return error::denied;
            std::shared_ptr<origins> origin;status=active(*node,id,origin);if(status!=error::success)return status;
            group_lease all;status=group(who,cap,"node.open",0,origin.get(),all,false);if(status!=error::success)return status;
            return convert(core_->validate_node(node->identity,id,profile));
        }
        /** @brief Resolve actual schema provenance without copying/allocating its descriptor.
         * @details All schema data, including the validator, must match the
         * immutable registered profile. The returned owner is minted by this
         * dispatcher. It cannot be supplied by guest input or a sibling plan.
         */
        error validate_schema(caller who,handle cap,communication::type_plan const& expected,owner& actual)
        {
            if(!same_process())return error::denied;
            lease allowed;auto status=permissions_->admit(who,cap,"type.describe",0,allowed,false);if(status!=error::success)return status;
            binding* selected{};status=resolve(allowed.specification(),selected);if(status!=error::success||selected->kind!=purpose::type)return error::denied;
            auto const& value=std::get<communication::type_plan>(selected->policy);
            if(value.selected!=expected.selected||value.type_identity!=expected.type_identity||value.encoding!=expected.encoding||
                value.descriptor!=expected.descriptor||value.validate!=expected.validate)return error::denied;
            actual=selected->identity;return error::success;
        }
        error validate_context(caller who,handle cap,handle id,profile const& profile)
        {
            if(!same_process())return error::denied;
            lease allowed;auto status=permissions_->admit(who,cap,"context.describe",0,allowed,false);if(status!=error::success)return status;
            binding* selected{};status=resolve(allowed.specification(),selected);if(status!=error::success||selected->kind!=purpose::context)return error::denied;
            std::shared_ptr<origins> origin;status=active(*selected,id,origin);if(status!=error::success)return status;
            return convert(core_->validate_context(selected->identity,id,profile));
        }
        /** @brief Resolve one subscription readiness source under all retained origins.
         * @details The event provider must additionally retain and revalidate
         * the source capability on every observation/delivery. This source
         * exposes no message payload or fabricated publisher credentials.
         */
        error event_source(caller who,handle cap,handle id,events::binding& output)
        {
            if(!same_process())return error::denied;
            lease allowed;auto status=permissions_->dependency(who,cap,"resource.observe",allowed);if(status!=error::success)return status;
            binding* selected{};status=resolve(allowed.specification(),selected);if(status!=error::success||selected->kind!=purpose::topic)return error::denied;
            auto const& plan=std::get<communication::topic_plan>(selected->policy);if(plan.direction!=communication::role::subscription)return error::denied;
            std::shared_ptr<origins> origin;status=active(*selected,id,origin);if(status!=error::success)return status;
            std::array<admission_claim,7> claims{};claims[0]={cap,"resource.observe",0,true,true};
            for(std::size_t i{};i<origin->count;++i){claims[i+1]=origin->claims[i];}
            group_lease admitted;
            status=permissions_->admit_group(who,{claims.data(),origin->count+1},admitted,false);if(status!=error::success)return status;
            status=validate_origins(*origin,admitted);if(status!=error::success)return status;
            std::uint64_t revision{};bool ready{},terminal{};
            status=convert(core_->observe(selected->identity,id,selected->identity.generation,revision,ready,terminal));if(status!=error::success)return status;
            try
            {
                auto provider=::uwvm2::utils::container::make_shared<events::communication_source>(core_);
                output={selected->identity,id,selected->identity.generation,std::move(provider)};return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}
        }
        /** @brief Revalidate an already retained subscription source without another allocation. */
        error validate_source(caller who,handle cap,events::binding const& source)
        {
            if(!same_process())return error::denied;
            lease allowed;auto status=permissions_->dependency(who,cap,"resource.observe",allowed);if(status!=error::success)return status;
            binding* selected{};status=resolve(allowed.specification(),selected);
            if(status!=error::success||selected->kind!=purpose::topic||selected->identity!=source.origin||source.generation!=selected->identity.generation)return error::denied;
            std::shared_ptr<origins> origin;status=active(*selected,source.resource,origin);if(status!=error::success)return status;
            std::array<admission_claim,7> claims{};claims[0]={cap,"resource.observe",0,true,true};
            for(std::size_t i{};i<origin->count;++i){claims[i+1]=origin->claims[i];}
            group_lease admitted;
            status=permissions_->admit_group(who,{claims.data(),origin->count+1},admitted,false);
            return status==error::success?validate_origins(*origin,admitted):status;
        }
    private:
        /** @brief Observe only the registered live native client, with an optional independent observer grant.
         * @details The trusted worker obtains readiness from that exact client's native getter.
         * A queued job charges once when reserved, then rechecks both grants at transmission
         * and result delivery. No query performs a service invocation. Synchronous adapters
         * must provide a bounded nonblocking getter. A short output performs no native work.
         */
        error service_readiness(caller who,handle cap,binding& client,communication_contract::request const& request,
                                std::size_t bytes,std::size_t capacity,std::size_t& required,sink deliver,
                                ::uwvm2::utils::container::function_ref<bool()> cancelled,lease& initial)
        {
            if(!observation_.sample)return error::unsupported;
            if(cancelled&&cancelled())return error::cancelled;
            std::shared_ptr<origins> origin;
            auto status=active(client,request.resource,origin);if(status!=error::success)return status;
            group_lease authorized;
            status=availability_group(who,cap,client,request.origin_capability,*origin,0,authorized);
            if(status!=error::success)return status;
            if(request.origin_capability&&service_observation_audit::current)
                service_observation_audit::associate(authorized.specification(0)->alias,client.alias);
            // Record a re-entrant withdrawal only after a verified association and
            // before this call returns. An unknown source never supplies a peer label.
            struct withdrawal_check
            {
                authority& permissions;caller who;handle observer,source;
                ~withdrawal_check()
                {
                    if(!source||!service_observation_audit::current||!service_observation_audit::current->source_length)return;
                    lease first,second;
                    bool lost_observer=permissions.admit(who,observer,"service.available",0,first,false)!=error::success;
                    bool lost_source=permissions.admit(who,source,"service.submit",0,second,false)!=error::success;
                    using loss=service_observation_audit::withdrawal;
                    service_observation_audit::current->lost=lost_observer?(lost_source?loss::both:loss::observer):(lost_source?loss::source:loss::none);
                }
            } audit{*permissions_,who,cap,request.origin_capability};
            required=32;if(capacity<required)return error::too_large;
            workspace_lease workspace;status=reserve(required,workspace);if(status!=error::success)return status;
            communication::operation_token context;
            status=operation(who,cap,"service.available",client,origin,authorized,context,request.origin_capability);
            if(status!=error::success)return status;
            bool queued{};
            status=convert(core_->queued_observation(client.identity,request.resource,queued));if(status!=error::success)return status;
            group_lease charged;
            if(!queued)
            {
                status=availability_group(who,cap,client,request.origin_capability,*origin,bytes+required,charged,true);
                if(status!=error::success)return status;
            }
            initial.release();authorized.release();
            bool available{};
            status=convert(core_->service_available(client.identity,request.resource,available,&context));
            charged.release();
            if(status!=error::success)return status;
            communication_contract::clock_epoch epoch;std::int64_t now{};
            if(!observation_.sample(observation_.context,epoch,now)||!epoch)return error::io;
            std::array<std::uint8_t,32> body{};wire::writer output{body};
            if(!output.boolean(available)||!communication_contract::encode_stamp(output,epoch,now)||output.size()!=required)return error::io;
            if(cancelled&&cancelled())return error::cancelled;
            group_lease delivery;status=revalidate(context,delivery);if(status!=error::success)return status;
            if(!deliver(output.written()))return error::io;
            if(cancelled&&cancelled())return error::cancelled;
            group_lease after;return revalidate(context,after);
        }
    public:
        error invoke(caller who,op action,handle cap,wire::bytes input,std::size_t capacity,std::size_t& required,sink deliver,
                     ::uwvm2::utils::container::function_ref<bool()> cancelled={})
        {
            required=0;auto status=preflight(who,action,cap,input.size());if(status!=error::success)return status;
            if(!deliver||capacity>limits_.output_bytes)return error::invalid;
            communication_contract::request request;if(!communication_contract::decode(action,input,request))return error::invalid;
            auto action_name=communication_contract::operations[static_cast<std::size_t>(action)].name;
            lease allowed;status=permissions_->admit(who,cap,action_name,0,allowed,false);if(status!=error::success)return status;
            binding* selected{};status=resolve_observation(allowed.specification(),action_name,selected);if(status!=error::success)return status;
            if(action==op::service_available)
            {
                if(service_observer(allowed.specification())!=(request.origin_capability!=0))return error::denied;
                try{return service_readiness(who,cap,*selected,request,input.size(),capacity,required,deliver,cancelled,allowed);}
                catch(std::bad_alloc const&){return error::no_memory;}
                catch(...){return error::io;}
            }
            origins const* current_origin{};
            // A cancellation callback may revoke recursively and still return
            // false. Recheck the complete cohort after that callback and before
            // either native effects or the first byte of public delivery.
            auto checkpoint=[&]() -> error
            {
                if(cancelled&&cancelled())return error::cancelled;
                group_lease fresh;auto check=group(who,cap,action_name,0,current_origin,fresh,false);if(check!=error::success)return check;
                std::lock_guard lock{mutex_};return retired_||selected->revoked?error::cancelled:error::success;
            };
            auto emit=[&](wire::bytes bytes)
            {try{return checkpoint()==error::success&&deliver(bytes)&&(!cancelled||!cancelled());}catch(...){return false;}};
            try
            {
                status=checkpoint();if(status!=error::success)return status;
                bool opening=action==op::context_open||action==op::node_open||action==op::topic_open||action==op::service_open;
                if(opening)
                {
                    if((nodes_async_&&action==op::node_open)||(endpoints_async_&&(action==op::topic_open||action==op::service_open)))return error::unsupported;
                    required=8;if(capacity<required)return error::too_large;
                    if(!allowed.specification().max_resources)return error::too_large;
                    auto const& requested=action==op::context_open||action==op::node_open?request.profile:request.endpoint.profile;
                    if(!communication_contract::matches(requested,communication_dispatch::selected(*selected)))return error::denied;
                    auto origin=::uwvm2::utils::container::make_shared<origins>();binding* parent{};
                    if(action==op::node_open)
                    {
                        status=add_origin(who,request.origin_capability,"context.open",purpose::context,*origin,parent);if(status!=error::success)return status;
                        status=append_parent(*parent,request.resource,*origin);if(status!=error::success)return status;
                    }
                    if(action==op::topic_open||action==op::service_open)
                    {
                        status=add_origin(who,request.endpoint.node_capability,"node.open",purpose::node,*origin,parent);if(status!=error::success)return status;
                        status=append_parent(*parent,request.endpoint.node,*origin);if(status!=error::success)return status;
                        for(std::size_t i{};i<request.endpoint.schema_count;++i)
                        {
                            binding* schema{};status=add_origin(who,request.endpoint.schema_capabilities[i],"type.describe",purpose::type,*origin,schema);
                            if(status!=error::success)return status;
                            auto const& expected=action==op::topic_open?std::get<communication::topic_plan>(selected->policy).type:
                                i==0?std::get<communication::service_plan>(selected->policy).request_type:std::get<communication::service_plan>(selected->policy).response_type;
                            if(communication_dispatch::selected(*schema)!=expected)return error::denied;
                        }
                    }
                    status=begin_open(*selected);if(status!=error::success)return status;
                    struct finish{communication_dispatch* self;binding* value;~finish(){self->end_open(*value);}} finished{this,selected};
                    current_origin=origin.get();
                    if(action==op::node_open)
                    {
                        // An exact zero/insufficient creation budget must reject
                        // before even a private process can be staged. Polling
                        // does not debit the eventual successful creation unit.
                        group_lease budget;status=group(who,cap,action_name,input.size()+8,origin.get(),budget,false,true);if(status!=error::success)return status;
                        status=checkpoint();if(status!=error::success)return status;
                        communication::node_stage_request stage{who.instance,who.generation,cap,request.origin_capability,request.resource,parent->identity,buffers::hash(input)};
                        status=convert(core_->prepare_node(selected->identity,parent->identity,request.resource,communication_dispatch::selected(*selected),stage));
                        if(status!=error::success)return status;
                    }
                    group_lease admitted;status=group(who,cap,action_name,input.size()+8,origin.get(),admitted,true);if(status!=error::success)return status;
                    status=checkpoint();if(status!=error::success)return status;
                    communication::error opened{};
                    if(action==op::context_open)opened=core_->open_context(selected->identity,communication_dispatch::selected(*selected),origin->id);
                    else if(action==op::node_open)opened=core_->open_node(selected->identity,parent->identity,request.resource,communication_dispatch::selected(*selected),origin->id);
                    else if(action==op::topic_open)opened=core_->open_topic(selected->identity,parent->identity,request.endpoint.node,communication_dispatch::selected(*selected),
                        request.endpoint.name,request.endpoint.type,request.endpoint.role=="publisher"?communication::role::publisher:communication::role::subscription,origin->id);
                    else opened=core_->open_service(selected->identity,parent->identity,request.endpoint.node,communication_dispatch::selected(*selected),
                        request.endpoint.name,request.endpoint.type,request.endpoint.role=="client"?communication::service_role::client:communication::service_role::server,origin->id);
                    if(opened!=communication::error::ok)return convert(opened);
                    {std::lock_guard lock{mutex_};selected->live=origin;}
                    std::array<std::uint8_t,8> bytes{};wire::writer writer{bytes};writer.u64(origin->id);
                    bool copied=emit(writer.written());group_lease after;status=group(who,cap,action_name,0,origin.get(),after,false);
                    if(!copied||status!=error::success){rollback_open(*selected,origin);return status==error::success?error::io:status;}
                    {std::lock_guard lock{mutex_};origin->published=true;}return error::success;
                }
                if(action==op::type_describe)
                {
                    auto const& schema=std::get<communication::type_plan>(selected->policy);
                    if(request.type!=schema.type_identity)return error::denied;
                    if(request.cursor)return error::conflict;
                    auto encode=[&](wire::writer& out)
                    {
                        // One complete bounded approved descriptor; there is no
                        // invented continuation cursor or remote introspection.
                        return out.text("uwvm/type/Descriptor")&&out.text("bytes")&&out.blob(schema.descriptor)&&
                            out.u64(selected->identity.generation)&&out.boolean(false)&&out.u64(0);
                    };
                    return output(who,cap,action_name,nullptr,input.size(),capacity,required,emit,encode);
                }
                std::shared_ptr<origins> origin;status=active(*selected,request.resource,origin);if(status!=error::success)return status;
                bool queue_charged{};
                if(selected->kind==purpose::topic||selected->kind==purpose::service||selected->kind==purpose::context)
                {status=convert(core_->queued_submission(selected->identity,request.resource,queue_charged));if(status!=error::success)return status;}
                // Recover history before re-reading original sources. The
                // complete canonical hash is generated here from owned bytes,
                // never accepted as an untrusted caller assertion.
                bool keyed=action==op::topic_publish||action==op::service_submit||action==op::service_respond||
                    action==op::context_shutdown||action==op::qos_liveliness_assert;
                buffers::digest canonical{};
                if(keyed)
                {
                    canonical=buffers::hash(input);communication::receipt prior;bool replay{};
                    {
                        std::lock_guard lock{mutex_};
                        if(origin->has_receipt&&origin->receipt.key==request.key)
                        {
                            if(origin->previous_action!=action||origin->canonical!=canonical)return error::conflict;
                            prior=origin->receipt;replay=true;
                        }
                    }
                    if(replay)
                    {
                        status=convert(core_->receipt_for(selected->identity,request.resource,prior.request,prior));if(status!=error::success)return status;
                        return output(who,cap,action_name,nullptr,input.size(),capacity,required,emit,
                            [&](auto& out){return communication_contract::encode_receipt(out,prior);},!queue_charged);
                    }
                }
                current_origin=origin.get();
                {
                    std::lock_guard lock{mutex_};
                    if(origin->receive_unknown&&action!=op::topic_describe&&action!=op::service_describe&&
                        action!=op::qos_describe&&action!=op::service_available)return error::conflict;
                }
                group_lease checked;status=group(who,cap,action_name,0,origin.get(),checked,false);if(status!=error::success)return status;
                communication::operation_token authorized;
                status=operation(who,cap,action_name,*selected,origin,checked,authorized);if(status!=error::success)return status;
                if(action==op::context_describe)
                {communication::context_info info;status=convert(core_->describe(selected->identity,request.resource,info));if(status!=error::success)return status;
                 return output(who,cap,action_name,origin.get(),input.size(),capacity,required,emit,[&](auto& out){return communication_contract::encode_context(out,info);});}
                if(action==op::node_describe)
                {communication::node_info info;status=convert(core_->describe(selected->identity,request.resource,info));if(status!=error::success)return status;
                 return output(who,cap,action_name,origin.get(),input.size(),capacity,required,emit,[&](auto& out){return communication_contract::encode_node(out,info);});}
                if(action==op::topic_describe)
                {communication::topic_info info;status=convert(core_->describe(selected->identity,request.resource,info));if(status!=error::success)return status;
                 return output(who,cap,action_name,origin.get(),input.size(),capacity,required,emit,[&](auto& out){return communication_contract::encode_topic(out,info);});}
                if(action==op::service_describe)
                {communication::service_info info;status=convert(core_->describe(selected->identity,request.resource,info));if(status!=error::success)return status;
                 return output(who,cap,action_name,origin.get(),input.size(),capacity,required,emit,[&](auto& out){return communication_contract::encode_service(out,info);});}
                if(action==op::qos_describe)
                {
                    bool queued{};status=convert(core_->queued_observation(selected->identity,request.resource,queued));if(status!=error::success)return status;
                    auto const& requested=selected->kind==purpose::topic?std::get<communication::topic_plan>(selected->policy).quality:
                        std::get<communication::service_plan>(selected->policy).quality;
                    auto measure=wire::writer::measuring(limits_.output_bytes);
                    if(!communication_contract::encode_qos(measure,requested,requested))return error::too_large;
                    required=measure.size();if(capacity<required)return error::too_large;
                    group_lease budget;
                    if(!queued){status=group(who,cap,action_name,input.size()+required,origin.get(),budget,false,true);if(status!=error::success)return status;}
                    communication::qos actual;std::array<std::uint8_t,32> gid{};std::size_t size{};
                    status=convert(core_->describe_qos(selected->identity,request.resource,actual,gid,size,&authorized));if(status!=error::success)return status;
                    return output(who,cap,action_name,origin.get(),input.size(),capacity,required,emit,[&](auto& out){return communication_contract::encode_qos(out,requested,actual);},!queued);
                }
                if(action==op::topic_take)
                {
                    error failure=error::success;auto const& p=std::get<communication::topic_plan>(selected->policy);
                    auto* schema=static_cast<communication::type_plan const*>(nullptr);
                    for(auto const& entry:bindings_)if(entry.kind==purpose::type&&communication_dispatch::selected(entry)==p.type)schema=&std::get<communication::type_plan>(entry.policy);
                    if(!schema)return error::denied;
                    auto take=[&](communication::delivery const& value)
                    {failure=output(who,cap,action_name,origin.get(),input.size(),capacity,required,emit,[&](auto& out)
                      {return communication_contract::encode_delivery(out,value.information,0,"topic.message",{schema->type_identity,schema->encoding,value.body});});return failure==error::success;};
                    status=convert(core_->take(selected->identity,request.resource,take,&authorized));return failure!=error::success?failure:status;
                }
                if(action==op::service_take)
                {
                    struct destination
                    {
                        communication_dispatch* self;caller who;handle cap;std::string_view action;origins const* origin;
                        std::size_t input,capacity;std::size_t* required;sink deliver;communication::service_plan const* plan;
                        error failure{error::success};
                    } target{this,who,cap,action_name,origin.get(),input.size(),capacity,&required,emit,&std::get<communication::service_plan>(selected->policy)};
                    auto copy=[](void* pointer,communication::service_delivery const& value)
                    {
                        auto& d=*static_cast<destination*>(pointer);auto const& profile=value.response?d.plan->response_type:d.plan->request_type;
                        communication::type_plan const* schema{};
                        for(auto const& entry:d.self->bindings_)if(entry.kind==purpose::type&&communication_dispatch::selected(entry)==profile)schema=&std::get<communication::type_plan>(entry.policy);
                        if(!schema){d.failure=error::denied;return false;}
                        d.failure=d.self->output(d.who,d.cap,d.action,d.origin,d.input,d.capacity,*d.required,d.deliver,[&](auto& out)
                        {return communication_contract::encode_delivery(out,value.information,value.correlation,value.response?"service.response":"service.request",{schema->type_identity,schema->encoding,value.body});});
                        return d.failure==error::success;
                    };
                    status=convert(core_->service_take(selected->identity,request.resource,&target,copy,&authorized));return target.failure!=error::success?target.failure:status;
                }
                // Variable receipt reserves a conservative upper bound before
                // any native mutation. Successful output reports exact bytes.
                required=160;if(capacity<required)return error::too_large;
                group_lease admitted;status=group(who,cap,action_name,queue_charged?0:input.size()+160,origin.get(),admitted,!queue_charged);if(status!=error::success)return status;
                status=checkpoint();if(status!=error::success)return status;
                communication::receipt receipt;communication::error result=communication::error::unsupported;
                if(action==op::topic_publish)result=core_->publish(selected->identity,request.resource,request.key,request.message.identity,request.message.encoding,request.message.body,request.deadline,receipt,&authorized);
                else if(action==op::service_submit||action==op::service_respond)result=core_->service_mutate(selected->identity,request.resource,request.key,request.correlation,
                    request.message.identity,request.message.encoding,request.message.body,request.deadline,action==op::service_respond,receipt,&authorized);
                else if(action==op::context_shutdown)result=core_->shutdown(selected->identity,request.resource,request.key,receipt,&authorized);
                else if(action==op::qos_liveliness_assert)result=core_->assert_liveliness(selected->identity,request.resource,request.key,receipt,&authorized);
                else if(action==op::service_abandon)result=core_->service_abandon(selected->identity,request.resource,request.correlation,receipt,&authorized);
                else if(action==op::qos_acknowledgements_wait)
                {
                    admitted.release();checked.release();allowed.release();
                    result=core_->wait_acknowledged(selected->identity,request.resource,request.deadline,receipt,&authorized);
                }
                if(result!=communication::error::ok)return convert(result);
                if(keyed)
                {
                    std::lock_guard lock{mutex_};origin->has_receipt=true;origin->previous_action=action;
                    origin->canonical=canonical;origin->receipt=receipt;
                }
                std::array<std::uint8_t,160> bytes{};wire::writer writer{bytes};if(!communication_contract::encode_receipt(writer,receipt))return error::io;
                required=writer.size();group_lease final;status=group(who,cap,action_name,0,origin.get(),final,false);if(status!=error::success)return status;
                if(!emit(writer.written()))return error::io;
                group_lease after;return group(who,cap,action_name,0,origin.get(),after,false);
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        error close(caller who,handle cap,handle id)
        {
            if(!same_process())return error::denied;
            lease allowed;auto status=permissions_->admit(who,cap,"resource.close",0,allowed,false);if(status!=error::success)return status;
            binding* selected{};status=resolve(allowed.specification(),selected);if(status!=error::success)return status;
            std::shared_ptr<origins> origin;status=active(*selected,id,origin);if(status!=error::success)return status;
            {std::lock_guard lock{mutex_};if(origin->receive_unknown)return error::conflict;}
            admission_claim claim{cap,"resource.close",0,false,false,true};group_lease cleanup;
            status=permissions_->admit_group(who,{&claim,1},cleanup,false);if(status!=error::success)return status;
            if(selected->kind==purpose::topic||selected->kind==purpose::service)
            {
                communication::operation_token authorized;status=operation(who,cap,"resource.close",*selected,origin,cleanup,authorized);
                if(status!=error::success)return status;
                status=convert(core_->close_owned(selected->identity,id,authorized));
            }
            else status=convert(core_->close(selected->identity,id));
            if(status!=error::success)return status;
            std::lock_guard lock{mutex_};if(selected->live==origin)selected->live.reset();return error::success;
        }
        error inspect(std::string_view alias,usage& out) const noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex_,std::try_to_lock};if(!lock.owns_lock())return error::again;
            usage value;value.ready=ready_;value.retired=retired_;
            for(auto const& entry:bindings_)if(alias.empty()||entry.alias==alias)
            {++value.configured;if(entry.live){if(entry.live->published)++value.handles;if(entry.live->orphan)++value.orphaned;}}
            if(workspace_){std::unique_lock account{workspace_->mutex,std::try_to_lock};if(!account.owns_lock())return error::again;value.workspace_bytes=workspace_->used;value.workspace_calls=workspace_->calls;}
            out=value;return error::success;
        }
        /** @brief Notify pending startup after the central capability-close succeeds.
         * @details Persistent published resources keep their explicit close
         * semantics. Only unpublished stages tied to this exact capability are
         * fenced; cleanup capacity remains held until actual worker reap.
         */
        void capability_closed(handle cap)noexcept
        {if(same_process()&&core_)core_->capability_closed(cap);}
        void revoke(std::string_view alias) noexcept
        {
            if(!same_process())return;
            owner identity;
            {std::lock_guard lock{mutex_};auto* value=named(alias);if(!value)return;value->revoked=true;identity=value->identity;}
            if(core_)core_->revoke(identity);
        }
        void retire() noexcept
        {
            if(!same_process())return;
            {std::lock_guard lock{mutex_};retired_=true;for(auto& value:bindings_)value.revoked=true;}
            if(workspace_){std::lock_guard lock{workspace_->mutex};workspace_->retired=true;}
            if(core_)core_->retire();
        }
    };
}
