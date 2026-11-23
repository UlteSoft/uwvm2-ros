/** @file
 * @brief Capability-scoped graph observations with explicit actual node origins.
 * @details The immutable graph plan selects context/node profiles and an exact
 * discovery filter. Each request names existing context and observer capabilities;
 * no node is opened implicitly and validation never spends node creation rates.
 * The first admitted observation pins these origins until revocation. Native
 * factory injection is trusted and portable: this header has no ROS dependency.
 * A graph watch reveals readiness only; payloads require graph.snapshot.
 * Native discovery is untrusted observation, never peer authentication. Deadlines
 * detect overruns after capture; this dispatcher is not a preemptive native worker.
 */
#pragma once
#include <utility>
#include "communication_dispatch.h"
#include "communication_workspace.h"
#include "communication_graph_contract.h"
#include "communication_graph_events.h"
namespace uwvm2::imported::ros::scene
{
    class communication_graph_dispatch
    {
        using op=graph_contract::operation;
        using owner=communication::owner;
        using profile=communication::profile;
        using snapshot=communication::graph::snapshot;
    public:
        struct plan
        {communication::graph::plan policy;profile context_profile,node_profile;};
        struct limits
        {std::size_t bindings{},carrier_bytes{},output_bytes{};std::uint64_t workspace_bytes{},workspace_calls{};};
        struct factory
        {void* context{};communication::error(*create)(void*,communication::graph::plan const&,communication::node_lease,std::shared_ptr<communication::graph::source>&) noexcept{};
            bool queued_captures{};std::shared_ptr<void> owner{};};
        struct usage
        {std::uint64_t configured{},snapshots{},watches{},workspace_bytes{},workspace_calls{};bool ready{},retired{};};
        using sink=communication_dispatch::sink;
        using workspace_lease=communication_workspace::lease;
    private:
        /** @brief Acyclic provenance retained by pending native captures.
         * @details This owns parent identities only, never the graph/source/queue.
         * Keeping the live row here would form a provider -> job -> row cycle.
         */
        struct provenance
        {communication::operation_token node;handle context_cap{},context{};};
        struct origins
        {
            handle context_cap{},context{},node_cap{},node{};
            owner node_owner;
            communication::operation_token node_origins;
            std::shared_ptr<provenance const> retained;
            std::shared_ptr<snapshot> graph;
            std::shared_ptr<communication::graph::watch> watch;
            handle watch_id{};
            bool watch_published{};
        };
        struct binding
        {::uwvm2::utils::container::std_string alias;plan policy;std::shared_ptr<origins> live;bool busy{},revoked{};};
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
            if(!ready_||retired_||!row||row->revoked||grant.kind!=purpose::discovery||grant.binding!="native-graph")return error::denied;
            out=row;return error::success;
        }
        error group(caller who,handle cap,std::string_view action,origins const& origin,std::uint64_t bytes,group_lease& lease,bool charge,bool secondary=false)
        {
            std::array<admission_claim,4> claims{};std::size_t count{};
            claims[count++]={cap,action,bytes,secondary,false};
            claims[count++]={origin.node_cap,"node.open",0,false,true};
            if(origin.node_origins.count==2)claims[count++]={origin.node_origins.origins[1].capability,"context.open",0,false,true};
            claims[count++]={origin.context_cap,"context.describe",0,false,true};
            return permissions_->admit_group(who,{claims.data(),count},lease,charge);
        }
        error validate(caller who,binding const& row,origins const& origin)
        {
            {std::lock_guard lock{mutex_};if(retired_||row.revoked)return error::cancelled;}
            auto result=communication_.validate_context(who,origin.context_cap,origin.context,row.policy.context_profile);
            if(result!=error::success)return result;
            result=communication_.validate_node(who,origin.node_cap,origin.node,row.policy.node_profile,origin.node_owner);if(result!=error::success)return result;
            if(origin.node_origins.caller_instance!=who.instance||origin.node_origins.caller_generation!=who.generation||
                origin.node_origins.capability!=origin.node_cap||origin.node_origins.resource!=origin.node||origin.node_origins.authority!=origin.node_owner||
                origin.node_origins.count!=2||origin.node_origins.origins[1].resource!=origin.context||!origin.retained)return error::denied;
            group_lease parent;return communication_.revalidate(origin.node_origins,parent);
        }
        error operation(caller who,handle cap,std::string_view action,binding const& row,std::shared_ptr<origins> const& origin,
                        communication::operation_token& output)
        {
            using permission=communication::permission;
            communication::operation_token value;value.caller_instance=who.instance;value.caller_generation=who.generation;
            value.authority=row.policy.policy.authority;value.capability=cap;value.action=communication::parse_permission(action);
            {std::lock_guard lock{mutex_};if(retired_||row.revoked||row.live!=origin)return error::cancelled;
                value.resource=value.authority.grant;if(value.action==permission::resource_observe)value.watch=origin->watch_id;}
            value.family=communication::operation_family::graph;value.retained_origins=origin->retained;
            if(origin->node_origins.count!=2)return error::denied;
            value.origins[0]={value.authority,cap,value.resource,value.action};value.origins[1]=origin->node_origins.origins[0];
            value.origins[2]=origin->node_origins.origins[1];
            value.origins[3]={value.origins[2].authority,origin->context_cap,origin->context,permission::context_describe};value.count=4;
            if(!value.resource||!value.valid(value.action,value.family,value.authority,value.resource,false))return error::denied;
            output=std::move(value);return error::success;
        }
        error enter(binding& row)
        {std::lock_guard lock{mutex_};if(retired_||row.revoked)return error::cancelled;if(row.busy)return error::again;row.busy=true;return error::success;}
        void leave(binding& row) noexcept{std::lock_guard lock{mutex_};row.busy=false;}
        error prepare_origins(caller who,binding& row,graph_contract::request const& request,std::shared_ptr<origins>& result)
        {
            {
                std::lock_guard lock{mutex_};result=row.live;
            }
            if(result)
            {
                if(result->context_cap!=request.context_capability||result->context!=request.context||result->node_cap!=request.observer_node_capability||result->node!=request.observer_node)
                    return error::conflict;
                return validate(who,row,*result);
            }
            auto next=::uwvm2::utils::container::make_shared<origins>();next->context_cap=request.context_capability;next->context=request.context;
            next->node_cap=request.observer_node_capability;next->node=request.observer_node;
            auto status=communication_.validate_context(who,next->context_cap,next->context,row.policy.context_profile);if(status!=error::success)return status;
            communication::node_lease node;status=communication_.attach_node(who,next->node_cap,next->node,row.policy.node_profile,node);if(status!=error::success)return status;
            if(node.context()!=next->context)return error::denied;
            next->node_owner=node.authority();
            status=communication_.retain_node_origins(who,next->node_cap,next->node,row.policy.node_profile,next->node_origins);if(status!=error::success)return status;
            if(next->node_origins.count!=2||next->node_origins.origins[1].resource!=next->context)return error::denied;
            next->retained=::uwvm2::utils::container::make_shared<provenance>(provenance{next->node_origins,next->context_cap,next->context});
            std::shared_ptr<communication::graph::source> source;
            auto policy=row.policy.policy;policy.context=next->context;
            auto opened=factory_.create(factory_.context,policy,std::move(node),source);if(opened!=communication::error::ok)return communication_dispatch::convert(opened);
            if(!source||source->authority()!=next->node_owner||source->context()!=next->context||source->queued_captures()!=factory_.queued_captures)return error::denied;
            auto graph=::uwvm2::utils::container::make_shared<snapshot>();
            status=communication_dispatch::convert(graph->configure(std::move(policy),std::move(source)));if(status!=error::success)return status;
            next->graph=std::move(graph);status=validate(who,row,*next);if(status!=error::success)return status;
            {std::lock_guard lock{mutex_};if(retired_||row.revoked)return error::cancelled;row.live=next;}
            result=std::move(next);return error::success;
        }
    public:
        communication_graph_dispatch(std::shared_ptr<authority> authority,communication_dispatch& origins,std::uint64_t environment,std::uint64_t generation,limits capacity):
            permissions_(std::move(authority)),communication_(origins),environment_(environment),generation_(generation),limits_(capacity)
        {bindings_.reserve(capacity.bindings);}
        ~communication_graph_dispatch(){if(!same_process()){std::construct_at(std::addressof(bindings_));return;}retire();}
        error bind(::uwvm2::utils::container::std_string alias,plan value)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(ready_||retired_||!permissions_||!environment_||!generation_)return error::denied;
            auto const& p=value.policy;
            if(!identifier(alias)||!communication::valid(p.selected)||!communication::valid(value.context_profile)||!communication::valid(value.node_profile)||
                p.nodes.empty()||!p.fields||(p.fields&~std::uint64_t{63})||!(p.fields&31)||!p.max_rows||!p.max_bytes||!p.max_page_items||
                p.max_page_items>p.max_rows||!std::in_range<std::int64_t>(p.max_rows)||!std::in_range<std::int64_t>(p.max_bytes)||
                !p.cursor_lifetime_ns||p.cursor_lifetime_ns>INT64_MAX||!p.capture_deadline_ns||p.capture_deadline_ns>INT64_MAX)return error::invalid;
            for(std::size_t i{};i<p.nodes.size();++i)
            {if(!communication::absolute_name(p.nodes[i]))return error::invalid;for(std::size_t j{};j<i;++j)if(p.nodes[i]==p.nodes[j])return error::invalid;}
            if(named(alias))return error::conflict;
            if(bindings_.size()>=limits_.bindings)return error::too_large;
            auto id=resources::allocate_identity();if(!id)return error::too_large;
            value.policy.authority={environment_,id,generation_};value.policy.context=0;
            bindings_.push_back({std::move(alias),std::move(value),{},false,false});return error::success;
        }
        error prepare(factory provider)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(ready_||retired_||!provider.create||(provider.owner&&provider.owner.get()!=provider.context))return error::denied;
            if(!limits_.carrier_bytes||!limits_.output_bytes)return error::invalid;
            auto status=workspace_.configure(limits_.workspace_bytes,limits_.workspace_calls);if(status!=error::success)return status;
            factory_=std::move(provider);ready_=true;return error::success;
        }
        error preflight(caller who,op action,handle cap,std::size_t extent)
        {
            if(!same_process()||!permissions_)return error::denied;
            auto index=static_cast<std::size_t>(action);if(index>=graph_contract::operations.size())return error::unsupported;
            if(extent>limits_.carrier_bytes)return error::too_large;
            // The owned queue charges before submission. Pending/replayed pages
            // must remain decodable after the last authorized capture unit.
            lease granted;auto status=permissions_->admit(who,cap,graph_contract::operations[index].name,0,granted,false);if(status!=error::success)return status;
            binding* row{};status=resolve(granted.specification(),row);if(status!=error::success)return status;
            return factory_.queued_captures?error::success:permissions_->preflight(who,cap,graph_contract::operations[index].name,extent,granted);
        }
        error reserve_workspace(caller who,op action,handle cap,std::size_t extent,workspace_lease& output)
        {auto status=preflight(who,action,cap,extent);return status==error::success?workspace_.reserve(extent,output):status;}
        /** @brief Revalidate the exact current caller and all four graph origins.
         * @details A retained source does not preserve authority after capability
         * close, watch close/reopen, caller retirement or origin revocation. The
         * returned group may cover only a nonblocking send, never a reply wait.
         */
        error revalidate(communication::operation_token const& token,group_lease& admitted)
        {
            using permission=communication::permission;
            if(!same_process()||!permissions_||admitted||token.family!=communication::operation_family::graph||
                !token.valid(token.action,token.family,token.authority,token.resource,false)||
                (token.action!=permission::graph_snapshot&&token.action!=permission::graph_watch&&token.action!=permission::resource_observe))return error::denied;
            caller who{token.caller_instance,token.caller_generation};auto name=communication::permission_name(token.action);
            bool observation=token.action==permission::resource_observe;lease allowed;
            auto status=observation?permissions_->dependency(who,token.capability,name,allowed):permissions_->admit(who,token.capability,name,0,allowed,false);
            if(status!=error::success)return status;
            binding* row{};status=resolve(allowed.specification(),row);if(status!=error::success||row->policy.policy.authority!=token.authority)return error::denied;
            std::shared_ptr<origins> origin;
            {std::lock_guard lock{mutex_};origin=row->live;if(!origin||origin->retained!=token.retained_origins||
                token.resource!=token.authority.grant||(observation&&(!origin->watch_published||origin->watch_id!=token.watch)))return error::bad_handle;}
            group_lease fresh;status=group(who,token.capability,name,*origin,0,fresh,false,observation);if(status!=error::success)return status;
            status=validate(who,*row,*origin);if(status!=error::success)return status;
            communication::operation_token actual;status=operation(who,token.capability,name,*row,origin,actual);
            if(status!=error::success||actual.resource!=token.resource||actual.watch!=token.watch||actual.count!=token.count||actual.origins!=token.origins)return error::denied;
            admitted=std::move(fresh);return error::success;
        }
        /** @brief Charge a fully reserved isolated graph capture exactly once. */
        error charge_operation(communication::operation_token const& token,std::uint64_t bytes,group_lease& admitted)
        {
            if(admitted)return error::denied;
            group_lease validated;auto status=revalidate(token,validated);if(status!=error::success)return status;
            std::array<admission_claim,4> claims{};
            for(std::size_t i{};i<token.count;++i)claims[i]={token.origins[i].capability,communication::permission_name(token.origins[i].action),i?0:bytes,
                i==0&&token.action==communication::permission::resource_observe,i!=0};
            return permissions_->admit_group({token.caller_instance,token.caller_generation},claims,admitted,true);
        }
        error invoke(caller who,op action,handle cap,wire::bytes input,std::size_t capacity,std::size_t& required,sink deliver,
            ::uwvm2::utils::container::function_ref<bool()> cancelled={})
        {
            required=0;auto status=preflight(who,action,cap,input.size());if(status!=error::success)return status;
            if(!deliver||capacity>limits_.output_bytes)return error::invalid;
            graph_contract::request request;if(!graph_contract::decode(action,input,request))return error::invalid;
            auto name=graph_contract::operations[static_cast<std::size_t>(action)].name;
            lease allowed;status=permissions_->admit(who,cap,name,0,allowed,false);if(status!=error::success)return status;
            binding* row{};status=resolve(allowed.specification(),row);if(status!=error::success)return status;
            if(!graph_contract::matches(request,row->policy.policy.selected))return error::denied;
            if(!allowed.specification().max_resources)return error::too_large;
            required=action==op::watch?8:24;if(capacity<required)return error::too_large;
            if(action==op::snapshot&&request.limit>row->policy.policy.max_page_items)return error::too_large;
            status=enter(*row);if(status!=error::success)return status;
            struct completion{communication_graph_dispatch* self;binding* row;~completion(){self->leave(*row);}} done{this,row};
            try
            {
                origins proposed;proposed.context_cap=request.context_capability;proposed.context=request.context;proposed.node_cap=request.observer_node_capability;proposed.node=request.observer_node;
                origins const* current_origin{};
                auto checkpoint=[&]() -> error
                {
                    if(cancelled&&cancelled())return error::cancelled;
                    group_lease fresh;auto check=group(who,cap,name,current_origin?*current_origin:proposed,0,fresh,false);if(check!=error::success)return check;
                    if(current_origin)return validate(who,*row,*current_origin);
                    std::lock_guard lock{mutex_};return retired_||row->revoked?error::cancelled:error::success;
                };
                status=checkpoint();if(status!=error::success)return status;
                auto emit=[&](wire::bytes value){try{return checkpoint()==error::success&&deliver(value)&&(!cancelled||!cancelled());}catch(...){return false;}};
                group_lease held;status=group(who,cap,name,proposed,0,held,false);if(status!=error::success)return status;
                std::shared_ptr<origins> origin;status=prepare_origins(who,*row,request,origin);if(status!=error::success)return status;
                current_origin=origin.get();status=checkpoint();if(status!=error::success)return status;
                if(action==op::watch)
                {
                    {std::lock_guard lock{mutex_};if(origin->watch_id)return error::again;}
                    auto id=resources::allocate_identity();if(!id)return error::too_large;
                    auto watch=::uwvm2::utils::container::make_shared<communication::graph::watch>(origin->graph,row->policy.policy.authority,id);
                    group_lease charged;status=group(who,cap,name,*origin,input.size()+8,charged,true);if(status!=error::success)return status;
                    {std::lock_guard lock{mutex_};origin->watch_id=id;origin->watch=watch;}
                    std::array<std::uint8_t,8> bytes{};wire::writer out{bytes};out.u64(id);
                    bool copied=emit(out.written());group_lease after;status=group(who,cap,name,*origin,0,after,false);
                    if(status==error::success)status=validate(who,*row,*origin);
                    {std::lock_guard lock{mutex_};if(copied&&status==error::success)origin->watch_published=true;else{origin->watch_id=0;origin->watch.reset();}}
                    return status!=error::success?status:copied?error::success:error::io;
                }
                struct target{communication_graph_dispatch* self;caller who;handle cap;std::string_view action;binding* row;origins* origin;std::size_t input,capacity;std::size_t* required;sink deliver;error failure{error::success};};
                target destination{this,who,cap,name,row,origin.get(),input.size(),capacity,&required,emit};
                auto copy=[](void* argument,communication::graph::page const& page)
                {
                    auto& d=*static_cast<target*>(argument);auto measure=wire::writer::measuring(d.self->limits_.output_bytes);
                    if(!graph_contract::encode(measure,page)){d.failure=error::too_large;return false;}
                    *d.required=measure.size();if(d.capacity<*d.required){d.failure=error::too_large;return false;}
                    workspace_lease reservation;d.failure=d.self->workspace_.reserve(*d.required,reservation);if(d.failure!=error::success)return false;
                    byte_buffer bytes;bytes.resize_for_overwrite(*d.required);wire::writer writer{bytes.writable()};
                    if(!graph_contract::encode(writer,page)){d.failure=error::io;return false;}
                    bool charge=!d.self->factory_.queued_captures;
                    group_lease admitted;d.failure=d.self->group(d.who,d.cap,d.action,*d.origin,charge?d.input+*d.required:0,admitted,charge);if(d.failure!=error::success)return false;
                    if(!d.deliver(writer.written())){d.failure=error::io;return false;}
                    group_lease after;d.failure=d.self->group(d.who,d.cap,d.action,*d.origin,0,after,false);
                    if(d.failure==error::success)d.failure=d.self->validate(d.who,*d.row,*d.origin);
                    return d.failure==error::success;
                };
                communication::operation_token authorized;status=operation(who,cap,name,*row,origin,authorized);if(status!=error::success)return status;
                held.release();allowed.release();
                status=communication_dispatch::convert(origin->graph->read(row->policy.policy.authority,row->policy.policy.selected,request.context,request.cursor,request.limit,&destination,copy,&authorized));
                return destination.failure==error::success?status:destination.failure;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        error event_source(caller who,handle cap,handle id,events::binding& out)
        {
            if(!same_process())return error::denied;
            lease allowed;auto status=permissions_->dependency(who,cap,"resource.observe",allowed);if(status!=error::success)return status;
            binding* row{};status=resolve(allowed.specification(),row);if(status!=error::success)return status;
            std::shared_ptr<origins> origin;{std::lock_guard lock{mutex_};origin=row->live;if(!origin||!origin->watch_published||origin->watch_id!=id)return error::bad_handle;}
            group_lease admitted;status=group(who,cap,"resource.observe",*origin,0,admitted,false,true);if(status!=error::success)return status;
            status=validate(who,*row,*origin);if(status!=error::success)return status;
            out={row->policy.policy.authority,id,row->policy.policy.authority.generation,origin->watch};return error::success;
        }
        error validate_source(caller who,handle cap,events::binding const& source)
        {
            events::binding actual;auto status=event_source(who,cap,source.resource,actual);if(status!=error::success)return status;
            return actual.origin==source.origin&&actual.generation==source.generation&&actual.provider==source.provider?error::success:error::denied;
        }
        /** @brief Refresh readiness with this poll's caller, never its opener.
         * @details The result contains readiness only. Payloads still require
         * graph.snapshot. The native adapter receives all original node/context
         * authorities and the explicit context.describe capability on every call.
         */
        communication::error observe_source(caller who,handle cap,events::binding const& source,events::snapshot& out)
        {
            auto status=validate_source(who,cap,source);if(status!=error::success)return static_cast<communication::error>(status);
            lease allowed;status=permissions_->dependency(who,cap,"resource.observe",allowed);if(status!=error::success)return static_cast<communication::error>(status);
            binding* row{};status=resolve(allowed.specification(),row);if(status!=error::success)return static_cast<communication::error>(status);
            std::shared_ptr<origins> origin;std::shared_ptr<communication::graph::watch> watch;
            {std::lock_guard lock{mutex_};origin=row->live;if(!origin||!origin->watch_published||origin->watch_id!=source.resource)return communication::error::bad_handle;watch=origin->watch;}
            communication::operation_token token;status=operation(who,cap,"resource.observe",*row,origin,token);if(status!=error::success)return static_cast<communication::error>(status);
            if(token.watch!=source.resource)return communication::error::bad_handle;
            // Do not retain an authority lease across provider calls or IPC waits.
            allowed.release();
            auto result=watch->inspect(source.origin,source.resource,source.generation,out,&token);if(result!=communication::error::ok)return result;
            status=validate_source(who,cap,source);return status==error::success?result:static_cast<communication::error>(status);
        }
        error close(caller who,handle cap,handle id)
        {
            if(!same_process())return error::denied;
            lease admitted;auto status=permissions_->admit(who,cap,"resource.close",0,admitted,false);if(status!=error::success)return status;
            binding* row{};status=resolve(admitted.specification(),row);if(status!=error::success)return status;
            status=enter(*row);if(status!=error::success)return status;
            struct completion{communication_graph_dispatch* self;binding* row;~completion(){self->leave(*row);}} done{this,row};
            {std::lock_guard lock{mutex_};if(!row->live||!row->live->watch_published||row->live->watch_id!=id)return error::bad_handle;}
            lease charged;status=permissions_->admit(who,cap,"resource.close",16,charged);if(status!=error::success)return status;
            std::lock_guard lock{mutex_};row->live->watch_published=false;row->live->watch_id=0;row->live->watch.reset();return error::success;
        }
        error inspect(std::string_view alias,usage& out) const noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex_,std::try_to_lock};if(!lock.owns_lock())return error::again;
            usage result;result.ready=ready_;result.retired=retired_;
            for(auto const& row:bindings_)if(alias.empty()||row.alias==alias){++result.configured;if(row.live){++result.snapshots;if(row.live->watch_published)++result.watches;}}
            auto status=workspace_.inspect(result.workspace_bytes,result.workspace_calls);if(status==error::success)out=result;return status;
        }
        void revoke(std::string_view alias) noexcept
        {
            if(!same_process())return;
            std::shared_ptr<origins> origin;{std::lock_guard lock{mutex_};auto* row=named(alias);if(!row)return;row->revoked=true;origin=row->live;}
            if(origin)origin->graph->retire();
        }
        void retire() noexcept
        {
            if(!same_process())return;
            {std::lock_guard lock{mutex_};retired_=true;}
            workspace_.retire();for(auto& row:bindings_)revoke(row.alias);
        }
    };
}
