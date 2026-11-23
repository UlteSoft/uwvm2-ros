/** @file
 * @brief Owned action dispatch with explicit node and ordered schema authority.
 * @details Deployment fixes role, endpoint name, node/schema profiles, five QoS
 * policies, queues, retention and transition mask. The factory receives owners
 * resolved from actual admitted bindings; no guest owner or native address is
 * accepted. Mutations hold all origins plus their transition-specific action.
 * Canonical request hashes preserve exact failed-output retries without another
 * effect, even after a source capability closes. Unknown effects and failed open
 * cleanup retain a fenced slot. Revocation is not successful remote cancellation.
 * Cancellation sets contain only explicitly selected owned goal handles; each
 * candidate retains separate native admission evidence and recovery state.
 */
#pragma once
#include <utility>
#include "communication_dispatch.h"
#include "communication_workspace.h"
#include "communication_action_contract.h"
#include "communication_creation.h"
namespace uwvm2::imported::ros::scene
{
    class communication_action_dispatch
    {
        struct origins;
        using op=action_contract::operation;
        using owner=communication::owner;
    public:
        struct plan
        {communication::actions::plan policy;communication::profile node_profile;};
        /** @brief Opaque private draft for one canonical asynchronous action open.
         * @details Only this dispatcher can populate a valid draft. It retains
         * actual node/context/schema owners and one counted node lease before
         * native admission. The creation ledger owns canonical retry bytes and
         * effect evidence. A draft resource is unusable until publish_open().
         * Destroying a caller's draft reference does not erase a pending slot or
         * native debt; the supervisor must supply a released creation receipt.
         */
        class opening
        {
            friend class communication_action_dispatch;
            ::uwvm2::utils::container::std_string alias;
            std::shared_ptr<origins> source;
            communication::actions::plan policy;
            communication_creation::identity original;
            buffers::digest canonical{};std::size_t carrier_bytes{};
            communication::node_lease parent;
            handle creation_request{},private_resource{};
            bool released{};
            std::mutex submission;
        public:
            opening()=default;opening(opening const&)=delete;opening& operator=(opening const&)=delete;
            communication::actions::plan const& specification()const noexcept{return policy;}
            communication_creation::identity const& identity()const noexcept{return original;}
            communication::node_lease const& node()const noexcept{return parent;}
            handle resource()const noexcept{return private_resource;}
            handle request()const noexcept{return creation_request;}
        };
        /** @brief Trusted nonblocking worker enqueue, with no commit on failure.
         * @details The callback owns complete canonical retry bytes and charges
         * through admit_open only after reserving all native queue capacity. On
         * success it returns the actual retained creation receipt. It never
         * waits, publishes a resource or performs guest output.
         */
        struct asynchronous_factory
        {
            void* context{};
            communication::error(*enqueue)(void*,opening const&,wire::bytes,communication_creation::view&)noexcept{};
            communication::error(*inspect)(void*,opening const&,communication_creation::view&)noexcept{};
        };
        struct limits
        {std::size_t bindings{},carrier_bytes{},output_bytes{};std::uint64_t workspace_bytes{},workspace_calls{};};
        /** @brief Return any possibly activated endpoint even on factory failure.
         * @details A failed startup may already have received a goal. Such an
         * endpoint must be returned for retirement/debt retention, never silently
         * destroyed. Native setup must configure bounded storage before exposure.
         */
        struct factory
        {void* context{};communication::error(*create)(void*,communication::actions::plan const&,communication::node_lease,handle,std::shared_ptr<communication::actions::endpoint>&) noexcept{};};
        struct usage
        {std::uint64_t configured{},handles{},fenced{},workspace_bytes{},workspace_calls{};bool ready{},retired{};};
        using sink=communication_dispatch::sink;
        using workspace_lease=communication_workspace::lease;
    private:
        /** @brief Immutable origin storage without a native endpoint ownership edge.
         * @details Queued operations may outlive the dispatch row. Retaining the
         * row itself would form a cycle through endpoint -> provider -> queue ->
         * token. This separate provenance keeps parent identities alive while
         * resource retirement and provider draining remain explicit obligations.
         */
        struct provenance
        {
            communication::operation_token node;
            std::array<handle,3> schema_caps;
            std::array<owner,3> schema_owners;
        };
        struct origins
        {
            handle id{},node_cap{},node{};
            owner node_owner;
            communication::operation_token node_origins;
            std::array<handle,3> schema_caps{};
            std::array<owner,3> schema_owners{};
            std::shared_ptr<provenance const> retained;
            std::shared_ptr<communication::actions::endpoint> endpoint;
            bool published{},fenced{},has_receipt{};
            op previous_action{};
            buffers::digest canonical{};
            communication::receipt receipt;
        };
        struct binding
        {::uwvm2::utils::container::std_string alias;plan policy;std::shared_ptr<origins> live;bool opening{},revoked{};std::shared_ptr<class opening> pending;std::uint64_t last_creation_key{};};
        struct readiness final:events::source
        {
            std::shared_ptr<communication::actions::endpoint> endpoint;
            owner identity;handle resource{};
            readiness(std::shared_ptr<communication::actions::endpoint> value,owner scope,handle id):endpoint(std::move(value)),identity(scope),resource(id){}
            communication::error inspect(owner scope,handle id,handle generation,events::snapshot& out) noexcept override
            {
                if(scope!=identity||id!=resource||generation!=identity.generation)return communication::error::denied;
                std::uint64_t revision{};bool ready{},terminal{};auto status=endpoint->observe(scope,id,generation,revision,ready,terminal);
                if(status==communication::error::ok)out={generation,revision,ready,terminal};
                return status;
            }
        };
        std::shared_ptr<authority> permissions_;
        communication_dispatch& communication_;
        std::uint64_t environment_{},generation_{};
        limits limits_;
        factory factory_;
        communication_workspace workspace_;
        mutable std::mutex mutex_;
        ::uwvm2::utils::container::std_vector<binding> bindings_;
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
        binding* named(std::string_view alias) noexcept
        {for(auto& row:bindings_)if(row.alias==alias)return &row;return nullptr;}
        error resolve(grant const& grant,binding*& out)
        {
            std::lock_guard lock{mutex_};auto* row=named(grant.alias);
            if(!ready_||retired_||!row||row->revoked||grant.kind!=purpose::action||grant.binding!="native-action")return error::denied;
            out=row;return error::success;
        }
        error active(binding& row,handle id,std::shared_ptr<origins>& out)
        {
            std::lock_guard lock{mutex_};if(retired_||row.revoked)return error::cancelled;
            if(!row.live||!row.live->published||row.live->fenced||row.live->id!=id)return error::bad_handle;
            out=row.live;return error::success;
        }
        error group(caller who,handle cap,std::string_view action,origins const* origin,std::string_view transition,std::uint64_t bytes,group_lease& lease,bool charge,bool secondary=false)
        {
            std::array<admission_claim,7> claims{};std::size_t count=1;claims[0]={cap,action,bytes,secondary,false};
            if(origin)
            {
                claims[count++]={origin->node_cap,"node.open",0,false,true};
                if(origin->node_origins.count==2)claims[count++]={origin->node_origins.origins[1].capability,"context.open",0,false,true};
                for(auto cap:origin->schema_caps)claims[count++]={cap,"type.describe",0,false,true};
            }
            if(!transition.empty())claims[count++]={cap,transition,0,true,false};
            return permissions_->admit_group(who,{claims.data(),count},lease,charge);
        }
        error validate(caller who,binding const& row,origins const& source)
        {
            {std::lock_guard lock{mutex_};if(retired_||row.revoked||source.fenced)return error::cancelled;}
            auto status=communication_.validate_node(who,source.node_cap,source.node,row.policy.node_profile,source.node_owner);if(status!=error::success)return status;
            if(source.node_origins.caller_instance!=who.instance||source.node_origins.caller_generation!=who.generation||
                source.node_origins.capability!=source.node_cap||source.node_origins.resource!=source.node||
                source.node_origins.authority!=source.node_owner)return error::denied;
            group_lease parent;status=communication_.revalidate(source.node_origins,parent);if(status!=error::success)return status;
            auto const& p=row.policy.policy;std::array expected{&p.goal,&p.feedback,&p.result};
            for(std::size_t i{};i<3;++i)
            {owner actual;status=communication_.validate_schema(who,source.schema_caps[i],*expected[i],actual);if(status!=error::success)return status;if(actual!=source.schema_owners[i])return error::denied;}
            return error::success;
        }
        /** @brief Mint complete per-call action provenance, never opener authority.
         * @details Node, context and three schema roles remain ordered even when
         * schemas share a capability. Responses additionally name their exact
         * independent transition permission. This retained token is not a lease.
         */
        error operation(caller who,handle cap,std::string_view action,binding const& row,
                        std::shared_ptr<origins> const& source,std::string_view transition,
                        communication::operation_token& output)
        {
            using permission=communication::permission;
            communication::operation_token value;
            value.caller_instance=who.instance;value.caller_generation=who.generation;
            value.authority=row.policy.policy.authority;value.capability=cap;value.resource=source->id;
            value.family=communication::operation_family::action;value.action=communication::parse_permission(action);
            value.secondary=transition.empty()?permission::unconfigured:communication::parse_permission(transition);
            value.retained_origins=source->retained;value.origins[0]={value.authority,cap,source->id,value.action};
            if(source->node_origins.count!=2)return error::denied;
            value.origins[1]=source->node_origins.origins[0];value.origins[2]=source->node_origins.origins[1];
            for(std::size_t i{};i<3;++i)value.origins[3+i]={source->schema_owners[i],source->schema_caps[i],0,permission::type_describe};
            value.count=6;
            if(!transition.empty())value.origins[value.count++]={value.authority,cap,source->id,value.secondary};
            if(!value.valid(value.action,value.family,value.authority,source->id,false))return error::denied;
            output=std::move(value);return error::success;
        }
        template<class Encode> error output(caller who,handle cap,std::string_view action,binding& row,origins const* origin,std::string_view transition,
            std::size_t input,std::size_t capacity,std::size_t& required,sink deliver,Encode&& encode,bool charge=true)
        {
            auto measure=wire::writer::measuring(limits_.output_bytes);if(!encode(measure))return error::too_large;required=measure.size();if(capacity<required)return error::too_large;
            workspace_lease reservation;auto status=workspace_.reserve(required,reservation);if(status!=error::success)return status;
            byte_buffer bytes;bytes.resize_for_overwrite(required);wire::writer writer{bytes.writable()};if(!encode(writer)||writer.size()!=required)return error::conflict;
            group_lease admitted;status=group(who,cap,action,origin,transition,charge?input+required:0,admitted,charge);if(status!=error::success)return status;
            if(origin){status=validate(who,row,*origin);if(status!=error::success)return status;}
            if(!deliver(writer.written()))return error::io;
            group_lease after;status=group(who,cap,action,origin,transition,0,after,false);if(status!=error::success)return status;
            if(origin)return validate(who,row,*origin);
            {std::lock_guard lock{mutex_};return retired_||row.revoked?error::cancelled:error::success;}
        }
        void rollback(binding& row,std::shared_ptr<origins> const& origin) noexcept
        {
            auto closed=origin->endpoint?origin->endpoint->close(row.policy.policy.authority):communication::error::ok;
            if(closed!=communication::error::ok&&origin->endpoint)origin->endpoint->retire();
            std::lock_guard lock{mutex_};origin->published=false;origin->fenced=closed!=communication::error::ok;
            if(origin->fenced)row.live=origin;else if(row.live==origin)row.live.reset();
        }
    public:
        communication_action_dispatch(std::shared_ptr<authority> permissions,communication_dispatch& origins,std::uint64_t environment,std::uint64_t generation,limits capacity):
            permissions_(std::move(permissions)),communication_(origins),environment_(environment),generation_(generation),limits_(capacity)
        {bindings_.reserve(capacity.bindings);}
        ~communication_action_dispatch(){if(!same_process()){std::construct_at(std::addressof(bindings_));return;}retire();}
        error bind(::uwvm2::utils::container::std_string alias,plan value)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(ready_||retired_||!permissions_||!environment_||!generation_)return error::denied;
            auto const& p=value.policy;
            if(!identifier(alias)||!communication::valid(p.selected)||!communication::valid(value.node_profile)||!communication::absolute_name(p.resolved_name)||p.action_type.empty()||
                !p.max_goals||!std::in_range<std::int64_t>(p.max_goals)||!p.max_goal_bytes||!p.max_result_bytes||
                !p.reserved_bytes||!std::in_range<std::int64_t>(p.reserved_bytes)||
                !p.max_deadline_ns||p.max_deadline_ns>INT64_MAX||!p.retention_ns||p.retention_ns>INT64_MAX||(p.transition_mask&~std::uint64_t{510})||
                (p.direction!=communication::actions::role::client&&p.direction!=communication::actions::role::server)||!communication::valid(p.goal_quality)||
                !communication::valid(p.result_quality)||!communication::valid(p.cancel_quality)||!communication::valid(p.feedback_quality)||!communication::valid(p.status_quality))return error::invalid;
            for(auto const* schema:{&p.goal,&p.feedback,&p.result})if(!communication::valid(schema->selected)||schema->type_identity.empty()||schema->encoding.empty()||!schema->validate||schema->revoked)return error::invalid;
            if(named(alias))return error::conflict;
            if(bindings_.size()>=limits_.bindings)return error::too_large;
            auto id=resources::allocate_identity();if(!id)return error::too_large;value.policy.authority={environment_,id,generation_};
            value.policy.goal.authority=value.policy.feedback.authority=value.policy.result.authority={};
            bindings_.push_back({std::move(alias),std::move(value),{},false,false,{}});return error::success;
        }
        error prepare(factory provider)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(ready_||retired_||!provider.create)return error::denied;
            if(!limits_.carrier_bytes||!limits_.output_bytes)return error::invalid;
            auto status=workspace_.configure(limits_.workspace_bytes,limits_.workspace_calls);if(status!=error::success)return status;
            factory_=provider;ready_=true;return error::success;
        }
        /** @brief Freeze the dispatcher for verified asynchronous adoption only.
         * @details The ordinary synchronous open path stays unavailable in this
         * mode. The supervisor uses stage_open, record_open, adopt_open and
         * publish_open with separate creation and endpoint identities.
         */
        error prepare_async()
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(ready_||retired_||!limits_.carrier_bytes||limits_.output_bytes<64)return error::denied;
            auto status=workspace_.configure(limits_.workspace_bytes,limits_.workspace_calls);if(status!=error::success)return status;
            ready_=true;return error::success;
        }
        /// @brief Copy trusted immutable deployment metadata without granting or opening a resource.
        error describe_plan(std::string_view alias,plan& out)const
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(retired_)return error::cancelled;
            for(auto const& row:bindings_)if(row.alias==alias){out=row.policy;return error::success;}
            return error::bad_handle;
        }
        /** @brief Validate a complete open carrier and retain its actual host provenance.
         * @details The mandatory creation key/deadline trailer is part of exact
         * retry identity. Short output, wrong role/schema, zero handle capacity
         * and invalid deadlines fail before reserving a draft. No call budget is
         * consumed here: the worker ledger charges once after reserving its full
         * owned carrier. Replays require the original caller and capability.
         */
        error stage_open(caller who,handle cap,wire::bytes input,std::size_t capacity,std::shared_ptr<class opening>& output)
        {
            if(!same_process()||!permissions_)return error::denied;
            if(input.size()>limits_.carrier_bytes)return error::too_large;
            if(capacity<communication_open_contract::receipt_bytes)return error::too_large;
            wire::bytes semantic;communication_open_contract::creation suffix;action_contract::request request;
            if(!communication_open_contract::split_creation(input,semantic,suffix)||!action_contract::decode(op::open,semantic,request))return error::invalid;
            lease allowed;auto status=permissions_->admit(who,cap,"action.open",0,allowed,false);if(status!=error::success)return status;
            binding* row{};status=resolve(allowed.specification(),row);if(status!=error::success)return status;
            if(factory_.create)return error::denied;
            auto const& p=row->policy.policy;
            if(!communication_contract::matches(request.endpoint.profile,p.selected)||request.endpoint.name!=p.resolved_name||request.endpoint.type!=p.action_type||
                request.endpoint.role!=(p.direction==communication::actions::role::client?"client":"server"))return error::denied;
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
                if(suffix.deadline-now>p.max_deadline_ns)return error::invalid;
                row->opening=true;
            }
            struct finish{communication_action_dispatch* self;binding* row;~finish(){std::lock_guard lock{self->mutex_};row->opening=false;}} done{this,row};
            try
            {
                auto pending=::uwvm2::utils::container::make_shared<class opening>();pending->alias=row->alias;pending->policy=p;
                pending->canonical=canonical;pending->carrier_bytes=input.size();pending->source=::uwvm2::utils::container::make_shared<origins>();
                auto& origin=*pending->source;origin.node_cap=request.endpoint.node_capability;origin.node=request.endpoint.node;origin.schema_caps=request.endpoint.schema_capabilities;
                status=communication_.attach_node(who,origin.node_cap,origin.node,row->policy.node_profile,pending->parent);if(status!=error::success)return status;
                origin.node_owner=pending->parent.authority();
                status=communication_.retain_node_origins(who,origin.node_cap,origin.node,row->policy.node_profile,origin.node_origins);if(status!=error::success)return status;
                if(origin.node_origins.count!=2)return error::denied;
                std::array schemas{&pending->policy.goal,&pending->policy.feedback,&pending->policy.result};
                for(std::size_t i{};i<3;++i)
                {status=communication_.validate_schema(who,origin.schema_caps[i],*schemas[i],origin.schema_owners[i]);if(status!=error::success)return status;schemas[i]->authority=origin.schema_owners[i];}
                origin.retained=::uwvm2::utils::container::make_shared<provenance>(provenance{origin.node_origins,origin.schema_caps,origin.schema_owners});
                origin.id=resources::allocate_identity();if(!origin.id)return error::too_large;pending->private_resource=origin.id;
                auto& identity=pending->original;identity.who=who;identity.authority=p.authority;identity.key=suffix.key;identity.deadline=suffix.deadline;identity.count=6;
                identity.origins[0]={communication_creation::dependency::operation,p.authority,cap,0};
                for(std::size_t i{};i<2;++i)
                {auto const& parent=origin.node_origins.origins[i];identity.origins[i+1]={i?communication_creation::dependency::context:communication_creation::dependency::node,parent.authority,parent.capability,parent.resource};}
                for(std::size_t i{};i<3;++i)identity.origins[i+3]={communication_creation::dependency::schema,origin.schema_owners[i],origin.schema_caps[i],0};
                group_lease held;status=group(who,cap,"action.open",&origin,{},0,held,false);if(status!=error::success)return status;
                status=validate(who,*row,origin);if(status!=error::success)return status;
                std::lock_guard lock{mutex_};if(retired_||row->revoked)return error::cancelled;
                row->live=pending->source;row->pending=pending;output=std::move(pending);return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /** @brief Admit one exact pending creation without holding a lease across IPC waits. */
        error admit_open(communication_creation::identity const& identity,std::uint64_t bytes,group_lease& held,bool charge=false)
        {
            if(!same_process()||held)return error::denied;
            std::shared_ptr<class opening> pending;binding* row{};
            {std::lock_guard lock{mutex_};if(retired_)return error::cancelled;
                for(auto& value:bindings_)if(value.policy.policy.authority==identity.authority){row=&value;pending=value.pending;break;}}
            if(!row||!pending||pending->original!=identity)return error::denied;
            if(charge?bytes!=pending->carrier_bytes+communication_open_contract::receipt_bytes:bytes!=0)return error::invalid;
            communication_creation::claim_set claims;group_lease validated;auto status=permissions_->admit_group(identity.who,
                communication_creation::admission_claims(identity,communication_open_contract::endpoint_kind::action,claims),validated,false);
            if(status!=error::success)return status;
            status=validate(identity.who,*row,*pending->source);if(status!=error::success)return status;
            if(!charge){held=std::move(validated);return error::success;}
            return permissions_->admit_group(identity.who,communication_creation::admission_claims(identity,communication_open_contract::endpoint_kind::action,claims,bytes),held,true);
        }
        /// @brief Retain the actual worker-ledger request identity; never reinterpret it as a resource.
        error retain_open(communication_creation::identity const& original,std::shared_ptr<class opening>& output)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(retired_)return error::cancelled;
            for(auto const& row:bindings_)if(row.pending&&row.pending->original==original)
            {if(row.revoked)return error::cancelled;output=row.pending;return error::success;}
            return error::denied;
        }
        /// @brief Record only the actual worker-ledger request identity under dispatcher serialization.
        error record_open(std::shared_ptr<class opening> const& pending,communication_creation::view const& result)
        {
            if(!same_process()||!pending)return error::denied;
            std::lock_guard lock{mutex_};auto* row=named(pending->alias);
            if(!row||row->pending!=pending||retired_||row->revoked||result.reserved||result.kind!=communication_open_contract::endpoint_kind::action||
                result.receipt.key!=pending->original.key||!result.receipt.request_id||!result.provider_generation)return error::denied;
            if(pending->creation_request&&pending->creation_request!=result.receipt.request_id)return error::conflict;
            pending->creation_request=result.receipt.request_id;return error::success;
        }
        /** @brief Complete the public asynchronous action.open carrier transaction.
         * @details A 48-byte creation receipt is preflighted before staging.
         * Per-binding submission excludes reentrancy while the worker ledger
         * reserves and admits the exact key. Failed enqueue discards only an
         * uncommitted draft; failed guest copy keeps the request for exact retry.
         * No native endpoint is accepted as the returned creation-request ID.
         */
        error invoke_open(caller who,handle cap,wire::bytes input,std::size_t capacity,std::size_t& required,sink deliver,
                          asynchronous_factory provider,::uwvm2::utils::container::function_ref<bool()> cancelled={})noexcept
        {
            required=communication_open_contract::receipt_bytes;
            try
            {
                if(!same_process()||!provider.enqueue||!provider.inspect||!deliver)return error::denied;
                if(cancelled&&cancelled())return error::cancelled;
                std::shared_ptr<class opening> pending;auto status=stage_open(who,cap,input,capacity,pending);if(status!=error::success)return status;
                std::unique_lock serial{pending->submission,std::try_to_lock};if(!serial.owns_lock())return error::again;
                auto abandon=[&]
                {
                    std::lock_guard lock{mutex_};auto* row=named(pending->alias);
                    if(row&&row->pending==pending&&!pending->creation_request&&!pending->source->endpoint)
                    {if(row->live==pending->source)row->live.reset();row->pending.reset();}
                };
                if(cancelled&&cancelled()){abandon();return error::cancelled;}
                communication_creation::view receipt;
                status=communication_dispatch::convert(pending->creation_request?provider.inspect(provider.context,*pending,receipt):
                    provider.enqueue(provider.context,*pending,input,receipt));
                if(status!=error::success)
                {
                    abandon();
                    return status;
                }
                status=record_open(pending,receipt);if(status!=error::success)return status;
                std::array<std::uint8_t,communication_open_contract::receipt_bytes> storage;wire::writer out{storage};
                if(!communication_open_contract::encode(out,receipt.receipt)||out.size()!=required)return error::io;
                lease allowed;status=permissions_->admit(who,cap,"action.open",0,allowed,false);if(status!=error::success)return status;
                if(cancelled&&cancelled())return error::cancelled;
                if(!deliver(out.written()))return error::io;
                if(cancelled&&cancelled())return error::cancelled;
                lease after;return permissions_->admit(who,cap,"action.open",0,after,false);
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /** @brief Transfer the counted actual node lease to verified typed adoption.
         * @details A failed construction may request a new counted lease. This
         * remains a bounded host operation, protected by complete fresh origins.
         */
        error adoption_node(std::shared_ptr<class opening> const& pending,communication::node_lease& node)
        {
            if(!pending||node)return error::invalid;
            group_lease held;auto status=admit_open(pending->original,0,held);if(status!=error::success)return status;
            binding* row{};
            {std::lock_guard lock{mutex_};row=named(pending->alias);if(!row||row->pending!=pending||!pending->creation_request)return error::denied;
                if(pending->parent){node=std::move(pending->parent);return error::success;}}
            communication::node_lease replacement;
            status=communication_.attach_node(pending->original.who,pending->source->node_cap,pending->source->node,row->policy.node_profile,replacement);
            if(status!=error::success)return status;
            std::lock_guard lock{mutex_};if(row->pending!=pending||row->revoked||retired_)return error::cancelled;
            node=std::move(replacement);return error::success;
        }
        /// @brief Install an exact prepared host core privately; this does not publish a resource.
        error adopt_open(std::shared_ptr<class opening> const& pending,std::shared_ptr<communication::actions::endpoint> core)
        {
            if(!same_process()||!pending||!core||!core->owns(pending->original.authority,pending->resource()))return error::denied;
            group_lease held;auto status=admit_open(pending->original,0,held);if(status!=error::success)return status;
            std::lock_guard lock{mutex_};auto* row=named(pending->alias);if(!row||row->pending!=pending||!pending->creation_request)return error::denied;
            if(pending->source->endpoint&&pending->source->endpoint!=core)return error::conflict;
            pending->source->endpoint=std::move(core);return error::success;
        }
        /** @brief Publish the already adopted resource under its exact creation lease.
         * @details The caller first obtains the factory's successful published
         * receipt. This allocation-free step admits no native effect. A lost
         * guest copy retains the same active resource and full origin cohort.
         */
        error publish_open(std::shared_ptr<class opening> const& pending,communication_open_contract::published const& receipt,group_lease const& held)
        {
            if(!same_process()||!pending)return error::denied;
            communication_creation::claim_set claims;
            if(!held.matches(pending->original.who,communication_creation::admission_claims(pending->original,communication_open_contract::endpoint_kind::action,claims))||
                receipt.kind!=communication_open_contract::endpoint_kind::action||receipt.handle!=pending->resource()||receipt.creation.request_id!=pending->creation_request||
                receipt.creation.key!=pending->original.key||receipt.creation.status!=communication_open_contract::state::published||receipt.creation.result!=communication::error::ok||
                receipt.creation.observed==communication::effect::unknown||receipt.creation.cleanup_debt)return error::denied;
            std::lock_guard lock{mutex_};auto* row=named(pending->alias);
            if(!row||row->pending!=pending||row->live!=pending->source||!pending->source->endpoint||pending->source->fenced||row->revoked||retired_)return error::denied;
            pending->source->published=true;return error::success;
        }
        /** @brief Forget only a positively released creation with no active typed resource.
         * @details The supervisor supplies a cached receipt from its owning
         * ledger after verified endpoint/native cleanup. Unknown outcomes and
         * live or fenced cores cannot be converted into reusable capacity.
         */
        error release_open(std::shared_ptr<class opening> const& pending,communication_creation::view const& result)
        {
            if(!same_process()||!pending)return error::denied;
            std::lock_guard lock{mutex_};auto* row=named(pending->alias);
            if(!row||result.kind!=communication_open_contract::endpoint_kind::action||result.receipt.request_id!=pending->creation_request||
                result.receipt.key!=pending->original.key||result.receipt.status!=communication_open_contract::state::released||result.endpoint_live||
                result.receipt.cleanup_debt||result.receipt.observed==communication::effect::unknown)return error::again;
            // A lost copy of request.release reuses the exact verified draft.
            if(pending->released)return error::success;
            if(row->pending!=pending||pending->source->fenced||(pending->source->endpoint&&row->live==pending->source))return error::again;
            if(row->live==pending->source)row->live.reset();
            row->last_creation_key=pending->original.key;row->pending.reset();pending->parent.reset();pending->released=true;return error::success;
        }
        /** @brief Drop verified retired endpoint backing on the supervisor, preserving retry identity.
         * @details request.release retains its immutable draft for lost-copy
         * replay. That draft must not keep the closed native facade's counted
         * parent node alive. Call outside authority and registry locks after a
         * positively released receipt; unknown or active resources are refused.
         * Destruction happens after releasing this dispatcher's mutex.
         */
        error drain_released(std::shared_ptr<class opening> const& pending)
        {
            if(!same_process()||!pending)return error::denied;
            std::shared_ptr<communication::actions::endpoint> dropped;
            {std::lock_guard lock{mutex_};auto* row=named(pending->alias);
                if(!row||!pending->released||row->pending==pending||row->live==pending->source||pending->source->fenced)return error::again;
                dropped=std::move(pending->source->endpoint);}
            return error::success;
        }
        error preflight(caller who,op action,handle cap,std::size_t extent)
        {
            if(!same_process()||!permissions_)return error::denied;
            auto index=static_cast<std::size_t>(action);if(index>=action_contract::operations.size())return error::unsupported;
            if(extent>limits_.carrier_bytes)return error::too_large;
            bool mutation=action==op::submit||action==op::respond||action==op::cancel||action==op::cancel_set;
            lease granted;auto status=mutation?permissions_->admit(who,cap,action_contract::operations[index].name,0,granted,false):
                permissions_->preflight(who,cap,action_contract::operations[index].name,extent,granted);if(status!=error::success)return status;
            binding* row{};return resolve(granted.specification(),row);
        }
        error reserve_workspace(caller who,op action,handle cap,std::size_t extent,workspace_lease& out)
        {auto status=preflight(who,action,cap,extent);return status==error::success?workspace_.reserve(extent,out):status;}
        /** @brief Revalidate a queued action against the current full origin cohort.
         * @details Call outside provider/queue locks. The returned group permits
         * one bounded nonblocking IPC write, without charging again. Release it
         * before any wait. Closing the actual capability, parent or schema and
         * retiring any resource invalidates the operation, including pending work.
         */
        error revalidate(communication::operation_token const& token,group_lease& admitted)
        {
            if(!same_process()||!permissions_||admitted||token.family!=communication::operation_family::action||
                !token.valid(token.action,token.family,token.authority,token.resource,false))return error::denied;
            auto name=communication::permission_name(token.action);
            bool cleanup=token.action==communication::permission::resource_close;
            if(!cleanup&&std::none_of(action_contract::operations.begin(),action_contract::operations.end(),[&](auto const& entry){return entry.name==name;}))return error::denied;
            caller who{token.caller_instance,token.caller_generation};lease allowed;
            auto status=permissions_->admit(who,token.capability,name,0,allowed,false);if(status!=error::success)return status;
            binding* row{};status=resolve(allowed.specification(),row);
            if(status!=error::success||row->policy.policy.authority!=token.authority)return error::denied;
            std::shared_ptr<origins> origin;status=active(*row,token.resource,origin);if(status!=error::success)return status;
            if(origin->retained!=token.retained_origins)return error::denied;
            auto transition=token.secondary==communication::permission::unconfigured?std::string_view{}:communication::permission_name(token.secondary);
            group_lease fresh;
            if(cleanup){admission_claim claim{token.capability,name,0,false,false,true};status=permissions_->admit_group(who,{&claim,1},fresh,false);}
            else{status=group(who,token.capability,name,origin.get(),transition,0,fresh,false);if(status==error::success)status=validate(who,*row,*origin);}
            if(status!=error::success)return status;
            communication::operation_token actual;status=operation(who,token.capability,name,*row,origin,transition,actual);
            if(status!=error::success||actual.count!=token.count||actual.origins!=token.origins)return error::denied;
            admitted=std::move(fresh);return error::success;
        }
        /** @brief Admit one owned queued action with independent transition charging.
         * @details Call only after finite queue reservation. Complete live
         * provenance is checked before any counter changes, with the validation
         * group held through charging. A response adds its exact secondary work
         * unit without duplicating bytes. The queue consumes this proof once;
         * its outer dispatcher must not also charge. No native I/O occurs here.
         */
        error charge_operation(communication::operation_token const& token,std::uint64_t bytes,group_lease& admitted)
        {
            if(admitted)return error::denied;
            group_lease validated;auto status=revalidate(token,validated);if(status!=error::success)return status;
            if(token.action==communication::permission::resource_close)
            {
                admission_claim claim{token.capability,"resource.close",0,false,false,true};
                return permissions_->admit_group({token.caller_instance,token.caller_generation},{&claim,1},admitted,true);
            }
            std::array<admission_claim,8> claims{};
            for(std::size_t i{};i<token.count;++i)
            {
                bool secondary=token.action==communication::permission::action_respond&&i==token.count-1;
                claims[i]={token.origins[i].capability,communication::permission_name(token.origins[i].action),i?0:bytes,secondary,i!=0&&!secondary};
            }
            return permissions_->admit_group({token.caller_instance,token.caller_generation},{claims.data(),token.count},admitted,true);
        }
        error invoke(caller who,op action,handle cap,wire::bytes input,std::size_t capacity,std::size_t& required,sink deliver,
            ::uwvm2::utils::container::function_ref<bool()> cancelled={})
        {
            required=0;auto status=preflight(who,action,cap,input.size());if(status!=error::success)return status;
            if(!deliver||capacity>limits_.output_bytes)return error::invalid;
            action_contract::request request;if(!action_contract::decode(action,input,request))return error::invalid;
            auto name=action_contract::operations[static_cast<std::size_t>(action)].name;
            lease allowed;status=permissions_->admit(who,cap,name,0,allowed,false);if(status!=error::success)return status;
            binding* row{};status=resolve(allowed.specification(),row);if(status!=error::success)return status;
            origins const* current_origin{};
            std::string_view current_transition=action==op::respond?action_contract::transition_actions[static_cast<std::size_t>(request.transition)-1]:std::string_view{};
            auto checkpoint=[&]() -> error
            {
                if(cancelled&&cancelled())return error::cancelled;
                group_lease fresh;auto check=group(who,cap,name,current_origin,current_transition,0,fresh,false);if(check!=error::success)return check;
                if(current_origin)return validate(who,*row,*current_origin);
                std::lock_guard lock{mutex_};return retired_||row->revoked?error::cancelled:error::success;
            };
            auto emit=[&](wire::bytes value){try{return checkpoint()==error::success&&deliver(value)&&(!cancelled||!cancelled());}catch(...){return false;}};
            try
            {
                status=checkpoint();if(status!=error::success)return status;
                if(action==op::open)
                {
                    if(!factory_.create)return error::unsupported;
                    required=8;if(capacity<required)return error::too_large;
                    auto const& p=row->policy.policy;
                    if(!communication_contract::matches(request.endpoint.profile,p.selected)||request.endpoint.name!=p.resolved_name||request.endpoint.type!=p.action_type||
                        request.endpoint.role!=(p.direction==communication::actions::role::client?"client":"server"))return error::denied;
                    {
                        std::lock_guard lock{mutex_};if(retired_||row->revoked)return error::cancelled;if(row->opening||row->live)return error::again;
                        if(!allowed.specification().max_resources)return error::too_large;
                        row->opening=true;
                    }
                    struct completion{communication_action_dispatch* self;binding* row;~completion(){std::lock_guard lock{self->mutex_};row->opening=false;}} done{this,row};
                    auto origin=::uwvm2::utils::container::make_shared<origins>();origin->node_cap=request.endpoint.node_capability;origin->node=request.endpoint.node;
                    origin->schema_caps=request.endpoint.schema_capabilities;
                    group_lease held;status=group(who,cap,name,origin.get(),{},0,held,false);if(status!=error::success)return status;
                    communication::node_lease node;status=communication_.attach_node(who,origin->node_cap,origin->node,row->policy.node_profile,node);if(status!=error::success)return status;
                    origin->node_owner=node.authority();
                    status=communication_.retain_node_origins(who,origin->node_cap,origin->node,row->policy.node_profile,origin->node_origins);if(status!=error::success)return status;
                    auto policy=row->policy.policy;std::array schemas{&policy.goal,&policy.feedback,&policy.result};
                    for(std::size_t i{};i<3;++i){status=communication_.validate_schema(who,origin->schema_caps[i],*schemas[i],origin->schema_owners[i]);if(status!=error::success)return status;schemas[i]->authority=origin->schema_owners[i];}
                    origin->retained=::uwvm2::utils::container::make_shared<provenance>(provenance{origin->node_origins,origin->schema_caps,origin->schema_owners});
                    auto id=resources::allocate_identity();if(!id)return error::too_large;origin->id=id;
                    group_lease charged;status=group(who,cap,name,origin.get(),{},input.size()+8,charged,true);if(status!=error::success)return status;
                    current_origin=origin.get();status=checkpoint();if(status!=error::success)return status;
                    auto created=factory_.create(factory_.context,policy,std::move(node),id,origin->endpoint);
                    if(created!=communication::error::ok||!origin->endpoint){if(origin->endpoint)rollback(*row,origin);return created==communication::error::ok?error::io:communication_dispatch::convert(created);}
                    {std::lock_guard lock{mutex_};row->live=origin;}
                    std::array<std::uint8_t,8> bytes{};wire::writer writer{bytes};writer.u64(id);bool copied=emit(writer.written());
                    group_lease after;status=group(who,cap,name,origin.get(),{},0,after,false);if(status==error::success)status=validate(who,*row,*origin);
                    if(!copied||status!=error::success){rollback(*row,origin);return status!=error::success?status:error::io;}
                    {std::lock_guard lock{mutex_};origin->published=true;}return error::success;
                }
                std::shared_ptr<origins> origin;status=active(*row,request.resource,origin);if(status!=error::success)return status;
                std::string_view transition=action==op::respond?action_contract::transition_actions[static_cast<std::size_t>(request.transition)-1]:std::string_view{};
                bool keyed=action==op::submit||action==op::respond||action==op::cancel||action==op::cancel_set;buffers::digest canonical{};
                if(keyed)
                {
                    canonical=buffers::hash(input);communication::receipt prior;bool replay{};
                    {std::lock_guard lock{mutex_};if(origin->has_receipt&&origin->receipt.key==request.key){if(origin->previous_action!=action||origin->canonical!=canonical)return error::conflict;prior=origin->receipt;replay=true;}}
                    if(replay)
                    {
                        if(action==op::cancel_set)
                        {
                            communication::actions::cancellation_selection selected;
                            status=communication_dispatch::convert(origin->endpoint->selection_for(row->policy.policy.authority,prior.request,selected));
                            if(status!=error::success)return status;
                            return output(who,cap,name,*row,nullptr,{},input.size(),capacity,required,emit,
                                [&](auto& out){return action_contract::encode_selection(out,selected);},false);
                        }
                        status=communication_dispatch::convert(origin->endpoint->receipt_for(row->policy.policy.authority,prior.request,prior));if(status!=error::success)return status;
                        return output(who,cap,name,*row,nullptr,transition,input.size(),capacity,required,emit,
                            [&](auto& out){return communication_contract::encode_receipt(out,prior);},!origin->endpoint->queued_submission());
                    }
                }
                current_origin=origin.get();
                group_lease held;status=group(who,cap,name,origin.get(),transition,0,held,false);if(status!=error::success)return status;
                status=validate(who,*row,*origin);if(status!=error::success)return status;
                communication::operation_token authorized;status=operation(who,cap,name,*row,origin,transition,authorized);if(status!=error::success)return status;
                if(action==op::cancel_set)
                {
                    // Reserve the complete worst-case batch reply before any
                    // attempt. The set itself is charged once; queued providers
                    // additionally charge each independently admitted candidate.
                    required=168+40*request.cancelled_count;
                    if(required>limits_.output_bytes||capacity<required)return error::too_large;
                    workspace_lease reservation;status=workspace_.reserve(required,reservation);if(status!=error::success)return status;
                    group_lease charged;status=group(who,cap,name,origin.get(),{},input.size()+required,charged,true);if(status!=error::success)return status;
                    status=checkpoint();if(status!=error::success)return status;
                    communication::actions::cancellation_selection selected;
                    auto result=origin->endpoint->cancel_set(row->policy.policy.authority,request.key,
                        {request.cancelled_goals.data(),request.cancelled_count},request.deadline,selected,&authorized);
                    if(result!=communication::error::ok)return communication_dispatch::convert(result);
                    {std::lock_guard lock{mutex_};origin->has_receipt=true;origin->previous_action=action;origin->canonical=canonical;origin->receipt=selected.batch;}
                    std::array<std::uint8_t,168+40*communication::actions::maximum_cancellation_selection> bytes;
                    wire::writer writer{bytes};if(!action_contract::encode_selection(writer,selected))return error::io;required=writer.size();
                    if(!emit(writer.written()))return error::io;
                    group_lease after;status=group(who,cap,name,origin.get(),{},0,after,false);return status==error::success?validate(who,*row,*origin):status;
                }
                if(action==op::describe)
                {
                    communication::actions::endpoint_info endpoint;std::optional<communication::actions::goal_info> goal;
                    status=communication_dispatch::convert(origin->endpoint->describe(row->policy.policy.authority,endpoint));if(status!=error::success)return status;
                    if(request.has_goal){goal.emplace();status=communication_dispatch::convert(origin->endpoint->describe(row->policy.policy.authority,request.goal,*goal));if(status!=error::success)return status;}
                    return output(who,cap,name,*row,origin.get(),{},input.size(),capacity,required,emit,[&](auto& out){return action_contract::encode_state(out,endpoint,goal);});
                }
                if(action==op::take)
                {
                    bool queued=origin->endpoint->queued_delivery();
                    if(queued)
                    {required=action_contract::maximum_delivery_bytes(row->policy.policy);if(!required)return error::invalid;if(capacity<required||required>limits_.output_bytes)return error::too_large;}
                    struct destination{communication_action_dispatch* self;caller who;handle cap;binding* row;origins* origin;std::size_t input,capacity;std::size_t* required;sink deliver;bool charge;error failure{error::success};};
                    destination target{this,who,cap,row,origin.get(),input.size(),capacity,&required,emit,!queued};
                    auto copy=[](void* argument,communication::actions::delivery const& value)
                    {auto& d=*static_cast<destination*>(argument);d.failure=d.self->output(d.who,d.cap,"action.take",*d.row,d.origin,{},d.input,d.capacity,*d.required,d.deliver,
                        [&](auto& out){return action_contract::encode_delivery(out,value,d.row->policy.policy);},d.charge);return d.failure==error::success;};
                    status=communication_dispatch::convert(origin->endpoint->take(row->policy.policy.authority,&target,copy,&authorized));return target.failure!=error::success?target.failure:status;
                }
                required=160;if(capacity<required)return error::too_large;
                communication::actions::response response;
                if(action==op::respond&&!action_contract::exact_response(request,row->policy.policy,response))return error::invalid;
                if(action==op::submit&&(request.message.identity!=row->policy.policy.goal.type_identity||request.message.encoding!=row->policy.policy.goal.encoding))return error::denied;
                bool queue_charged=origin->endpoint->queued_submission();
                group_lease charged;status=group(who,cap,name,origin.get(),transition,queue_charged?0:input.size()+160,charged,!queue_charged);if(status!=error::success)return status;
                status=checkpoint();if(status!=error::success)return status;
                communication::receipt receipt;communication::error result=communication::error::unsupported;
                if(action==op::submit){communication::actions::uuid uuid{};std::copy(request.uuid.begin(),request.uuid.end(),uuid.begin());result=origin->endpoint->submit(row->policy.policy.authority,request.key,uuid,request.message.body,request.deadline,receipt,&authorized);}
                else if(action==op::respond)result=origin->endpoint->respond(row->policy.policy.authority,request.key,response,request.deadline,receipt,&authorized);
                else if(action==op::cancel)result=origin->endpoint->cancel(row->policy.policy.authority,request.key,request.goal,request.deadline,receipt,&authorized);
                if(result!=communication::error::ok)return communication_dispatch::convert(result);
                {std::lock_guard lock{mutex_};origin->has_receipt=true;origin->previous_action=action;origin->canonical=canonical;origin->receipt=receipt;}
                std::array<std::uint8_t,160> bytes{};wire::writer writer{bytes};if(!communication_contract::encode_receipt(writer,receipt))return error::io;required=writer.size();
                if(!emit(writer.written()))return error::io;
                group_lease after;status=group(who,cap,name,origin.get(),transition,0,after,false);return status==error::success?validate(who,*row,*origin):status;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        error event_source(caller who,handle cap,handle id,events::binding& out)
        {
            if(!same_process())return error::denied;
            lease allowed;auto status=permissions_->dependency(who,cap,"resource.observe",allowed);if(status!=error::success)return status;
            binding* row{};status=resolve(allowed.specification(),row);if(status!=error::success)return status;
            std::shared_ptr<origins> origin;status=active(*row,id,origin);if(status!=error::success)return status;
            group_lease admitted;status=group(who,cap,"resource.observe",origin.get(),{},0,admitted,false,true);if(status!=error::success)return status;
            status=validate(who,*row,*origin);if(status!=error::success)return status;
            try{out={row->policy.policy.authority,id,row->policy.policy.authority.generation,::uwvm2::utils::container::make_shared<readiness>(origin->endpoint,row->policy.policy.authority,id)};return error::success;}
            catch(std::bad_alloc const&){return error::no_memory;}
        }
        error validate_source(caller who,handle cap,events::binding const& source)
        {
            if(!same_process())return error::denied;
            lease allowed;auto status=permissions_->dependency(who,cap,"resource.observe",allowed);if(status!=error::success)return status;
            binding* row{};status=resolve(allowed.specification(),row);if(status!=error::success)return status;
            if(source.origin!=row->policy.policy.authority||source.generation!=row->policy.policy.authority.generation)return error::denied;
            std::shared_ptr<origins> origin;status=active(*row,source.resource,origin);if(status!=error::success)return status;
            group_lease admitted;status=group(who,cap,"resource.observe",origin.get(),{},0,admitted,false,true);return status==error::success?validate(who,*row,*origin):status;
        }
        error close(caller who,handle cap,handle id)
        {
            if(!same_process())return error::denied;
            lease allowed;auto status=permissions_->admit(who,cap,"resource.close",0,allowed,false);if(status!=error::success)return status;
            binding* row{};status=resolve(allowed.specification(),row);if(status!=error::success)return status;
            std::shared_ptr<origins> origin;status=active(*row,id,origin);if(status!=error::success)return status;
            admission_claim claim{cap,"resource.close",0,false,false,true};group_lease cleanup;
            status=permissions_->admit_group(who,{&claim,1},cleanup,false);if(status!=error::success)return status;
            communication::operation_token token;status=operation(who,cap,"resource.close",*row,origin,{},token);if(status!=error::success)return status;
            status=communication_dispatch::convert(origin->endpoint->close(row->policy.policy.authority,&token));if(status!=error::success)return status;
            std::lock_guard lock{mutex_};if(row->live==origin)row->live.reset();return error::success;
        }
        error inspect(std::string_view alias,usage& out) const noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex_,std::try_to_lock};if(!lock.owns_lock())return error::again;
            usage result;result.ready=ready_;result.retired=retired_;
            for(auto const& row:bindings_)if(alias.empty()||row.alias==alias){++result.configured;if(row.live){if(row.live->published)++result.handles;if(row.live->fenced)++result.fenced;}}
            auto status=workspace_.inspect(result.workspace_bytes,result.workspace_calls);if(status==error::success)out=result;return status;
        }
        void revoke(std::string_view alias) noexcept
        {
            if(!same_process())return;
            std::shared_ptr<origins> origin;{std::lock_guard lock{mutex_};auto* row=named(alias);if(!row)return;row->revoked=true;origin=row->live;if(origin)origin->fenced=true;}
            if(origin&&origin->endpoint)origin->endpoint->retire();
        }
        void retire() noexcept
        {
            if(!same_process())return;
            {std::lock_guard lock{mutex_};retired_=true;}workspace_.retire();for(auto& row:bindings_)revoke(row.alias);
        }
    };
}
