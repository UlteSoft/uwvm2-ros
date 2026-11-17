/** @file
 * @brief Finite, owned deployment configuration for isolated communication workers.
 * @details This builder separates provisioned attachments, semantic worker definitions,
 * capability aliases and process pairing. A definition never creates a grant.
 * Bindings select an exact definition from an already authenticated worker file;
 * guest input cannot select a native symbol, host path or transport endpoint.
 * All capacities are named, mandatory and initially zero. Rate attenuation is
 * still controlled by the independent authority policy, never these reservations.
 *
 * load_worker is an explicitly requested trusted configuration read. Validation
 * and plan binding perform no file access or process startup. Preparation later
 * pins existing channel identities and verifies the profile echo. Configuration is copied and
 * validated before publication, so allocation failure or an incomplete plan
 * cannot leave a partially callable worker. Owning allocations use recoverable
 * fast_io containers. Unsupported provider families fail closed.
 */
#pragma once
#include "communication_host.h"
#include "communication_worker_configuration.h"
#include "communication_attachment.h"
#include "configuration_file.h"
namespace uwvm2::imported::ros::scene
{
    class communication_deployment_configuration
    {
    public:
        using string=::uwvm2::utils::container::std_string;
        template<class T>using table=::uwvm2::utils::container::std_map<string,T,std::less<>>;
        struct field{std::string_view name;std::uint64_t maximum;};
        inline static constexpr std::array budget_fields{
            field{"workers",128},field{"pairs",64},field{"contexts",128},field{"nodes",128},field{"types",1024},
            field{"topics",128},field{"services",128},field{"queue-bytes",1ull<<34},field{"receipt-bytes",1ull<<24},
            field{"bindings",1536},field{"carrier-bytes",1ull<<24},field{"output-bytes",1ull<<24},
            field{"workspace-bytes",1ull<<34},field{"workspace-calls",65536},field{"node-deadline-ns",INT64_MAX},
            field{"creation-bindings",128},field{"creation-workspace-bytes",1ull<<30},field{"creation-workspace-calls",65536},
            field{"definition-bytes",1ull<<30},field{"provider-bytes",INT64_MAX},
            field{"supervisor-stack-bytes",64u*1024u*1024u},field{"supervisor-poll-ns",100000000},
            field{"actions",128},field{"action-workspace-bytes",1ull<<34},field{"action-workspace-calls",65536},
            field{"graphs",128},field{"graph-workspace-bytes",1ull<<34},field{"graph-workspace-calls",65536},
            field{"event-bindings",128},field{"event-workspace-bytes",1ull<<34},field{"event-workspace-calls",65536}};
        inline static constexpr std::array peer_fields{
            field{"rpc-retained-bytes",64u*1024u*1024u},field{"rpc-stderr-bytes",1ull<<30},
            field{"rpc-deadline-ns",60000000000ull},field{"rpc-poll-ns",100000000},
            field{"topic-carrier-bytes",1ull<<30},field{"topic-bytes",1ull<<30},
            field{"service-carrier-bytes",1ull<<30},field{"service-bytes",1ull<<30},
            field{"action-carrier-bytes",1ull<<30},field{"action-bytes",1ull<<30},field{"graph-query-bytes",1ull<<30}};
        struct worker
        {
            communication_attachment::configuration attachment;
            communication_worker_configuration::specification definition;
            std::uint64_t definition_bytes{},assigned{};
            std::array<std::uint64_t,peer_fields.size()> limits{};
        };
        struct binding{string worker_name;std::size_t definition{};std::shared_ptr<communication_configuration::definition const> local{};};
        struct pair{string left,right;std::uint64_t retirement_ns{};};
    private:
        std::array<std::uint64_t,budget_fields.size()> budget_{};
        std::uint64_t assigned_{};
        table<worker> workers_;
        table<binding> bindings_;
        table<pair> pairs_;
        static constexpr auto all_budget=(std::uint64_t{1}<<budget_fields.size())-1;
        static constexpr auto all_peer=(std::uint64_t{1}<<peer_fields.size())-1;
        bool frozen_{};
        std::uint64_t budget(std::string_view name)const noexcept
        {for(std::size_t i{};i<budget_fields.size();++i)if(budget_fields[i].name==name)return budget_[i];return 0;}
        static bool add(std::uint64_t amount,std::uint64_t maximum,std::uint64_t& used)noexcept
        {if(used>maximum||amount>maximum-used)return false;used+=amount;return true;}
        static communication::profile const& selected(communication_configuration::definition const& value)noexcept
        {return communication_worker_configuration::profile_of(value);}
        static bool supported(communication_configuration::family kind)noexcept
        {return kind<=communication_configuration::family::event;}
        static purpose purpose_of(communication_configuration::family kind)noexcept
        {
            using family=communication_configuration::family;
            switch(kind){case family::context:return purpose::context;case family::node:return purpose::node;
                case family::type:return purpose::type;case family::topic:return purpose::topic;case family::service:return purpose::service;case family::action:return purpose::action;
                case family::graph:return purpose::discovery;case family::event:return purpose::event;
                default:return purpose::component;}
        }
        communication_configuration::definition const* definition(binding const& value)const noexcept
        {
            if(value.local)return value.local.get();
            auto found=workers_.find(value.worker_name);
            return found!=workers_.end()&&value.definition<found->second.definition.definitions.size()?
                &found->second.definition.definitions[value.definition]:nullptr;
        }
        binding const* origin(std::string_view worker_name,communication_configuration::family kind,communication::profile const& profile)const noexcept
        {
            for(auto const& [alias,value]:bindings_)
            { (void)alias;auto item=definition(value);if(value.worker_name==worker_name&&item&&item->kind==kind&&selected(*item)==profile)return &value; }
            return nullptr;
        }
    public:
        bool empty()const noexcept{return workers_.empty()&&bindings_.empty()&&pairs_.empty()&&!assigned_;}
        bool contains(std::string_view alias)const noexcept{return bindings_.contains(alias);}
        bool frozen()const noexcept{return frozen_;}
        table<worker> const& workers()const noexcept{return workers_;}
        table<binding> const& bindings()const noexcept{return bindings_;}
        table<pair> const& pairs()const noexcept{return pairs_;}
        std::uint64_t supervisor_stack_bytes()const noexcept{return budget("supervisor-stack-bytes");}
        std::uint64_t supervisor_poll_ns()const noexcept{return budget("supervisor-poll-ns");}
        communication_configuration::definition const* definition(std::string_view alias)const noexcept
        {auto found=bindings_.find(alias);return found==bindings_.end()?nullptr:definition(found->second);}
        /** @brief Assign one finite aggregate reservation; duplicate and unknown names reject. */
        error limit(std::string_view name,std::uint64_t value)noexcept
        {
            if(frozen_)return error::denied;
            for(std::size_t i{};i<budget_fields.size();++i)if(budget_fields[i].name==name)
            {auto bit=std::uint64_t{1}<<i;if(assigned_&bit)return error::conflict;
                if(value>budget_fields[i].maximum||value>SIZE_MAX)return error::too_large;
                budget_[i]=value;assigned_|=bit;return error::success;}
            return error::invalid;
        }
        /** @brief Load a bounded startup definition and bind its exact full-file digest.
         * @details Aggregate worker/definition reservations must already exist.
         * The supplied path is trusted deployment input. This reads an owned
         * configuration only, never code, a device or a provider. The exact profile digest is derived
         * from this authenticated file and cannot be replaced by later settings.
         * The aggregate definition reservation charges the serialized startup
         * bytes and every retained bounded-schema graph, including its owned
         * descriptor. A file-size allowance alone cannot admit expanded schemas.
         * Parsing failure or insufficient retained capacity publishes no worker.
         */
        error load_worker(std::string_view name,std::string_view profile,std::string_view prefix,
                          std::string_view path,std::uint64_t bytes,buffers::digest digest)noexcept
        {
            if(frozen_)return error::denied;
            if(assigned_!=all_budget||!identifier(name)||!identifier(profile))return error::invalid;
            if(workers_.contains(name))return error::conflict;
            if(workers_.size()>=budget("workers")||!bytes||bytes>16u*1024u*1024u)return error::too_large;
            std::uint64_t total{};for(auto const& [key,value]:workers_){(void)key;if(!add(value.definition_bytes,budget("definition-bytes"),total))return error::too_large;}
            if(!add(bytes,budget("definition-bytes"),total))return error::too_large;
            try
            {
                worker value;buffers::profile startup;startup.sha256=digest;if(!startup.identity.assign(profile))return error::invalid;
                auto status=value.attachment.profile(startup,prefix);if(status!=error::success)return status;
                byte_buffer owned;status=configuration_file::read(path,bytes,digest,16u*1024u*1024u,owned);if(status!=error::success)return status;
                status=communication_worker_configuration::decode(owned.view(),bytes,value.definition);if(status!=error::success)return status;
                std::uint64_t retained=bytes;
                for(auto const& item:value.definition.definitions)
                    if(auto type=std::get_if<communication::type_plan>(&item.policy);type&&type->validate.bounded())
                    {
                        auto extra=type->validate.bounded()->storage_bytes();
                        if(!add(extra,budget("definition-bytes"),total)||!add(extra,budget("definition-bytes"),retained))return error::too_large;
                    }
                value.definition_bytes=retained;workers_.emplace(string(name),std::move(value));return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        /** @brief Apply one worker setting transactionally; callbacks perform inert configuration only. */
        template<class Configure>error worker_setting(std::string_view name,Configure&& configure)noexcept
        {
            if(frozen_)return error::denied;
            auto found=workers_.find(name);if(found==workers_.end())return error::bad_handle;
            try{auto next=found->second.attachment;auto status=configure(next);if(status!=error::success)return status;found->second.attachment=std::move(next);return error::success;}
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        /** @brief Set an explicit local RPC/adapter reservation; external process limits belong to deployment. */
        error worker_limit(std::string_view name,std::string_view field,std::uint64_t value)noexcept
        {
            if(frozen_)return error::denied;
            auto found=workers_.find(name);if(found==workers_.end())return error::bad_handle;
            for(std::size_t i{};i<peer_fields.size();++i)if(peer_fields[i].name==field)
            {auto& actual=found->second;auto bit=std::uint64_t{1}<<i;if(actual.assigned&bit)return error::conflict;
                if(value>peer_fields[i].maximum)return error::too_large;
                actual.limits[i]=value;actual.assigned|=bit;return error::success;}
            return error::invalid;
        }
        /** @brief Select one exact profile for a pre-existing grant of the same purpose.
         * @details Profiles on different workers never imply shared authority.
         * A worker definition can have only one alias here; role and dependent
         * node/schema definitions remain part of the selected immutable plan.
         */
        error bind(grant const& authority,std::string_view name,std::string_view profile)noexcept
        {
            if(frozen_)return error::denied;
            if(authority.binding!=(authority.kind==purpose::action?"native-action":authority.kind==purpose::discovery?"native-graph":
                authority.kind==purpose::event?"event-stream":"native-communication")||!identifier(authority.alias))return error::denied;
            if(bindings_.contains(authority.alias))return error::conflict;
            if(bindings_.size()>=budget("bindings"))return error::too_large;
            auto found=workers_.find(name);if(found==workers_.end())return error::bad_handle;
            std::optional<std::size_t> index;
            for(std::size_t i{};i<found->second.definition.definitions.size();++i)
                if(selected(found->second.definition.definitions[i]).identity==profile){if(index)return error::conflict;index=i;}
            if(!index)return error::bad_handle;
            auto const& value=found->second.definition.definitions[*index];
            if(!supported(value.kind))return error::unsupported;
            if(purpose_of(value.kind)!=authority.kind)return error::denied;
            for(auto const& [alias,current]:bindings_){(void)alias;if(!current.local&&current.worker_name==name&&current.definition==*index)return error::conflict;}
            try{bindings_.emplace(authority.alias,binding{string(name),*index});return error::success;}
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /** @brief Bind a host-local readiness stream to an explicit workspace partition.
         * @details The named worker selects only a finite host resource domain;
         * no event definition is added to its sealed native manifest. Sources
         * remain separate runtime capabilities and may come from other domains.
         * A stream observes readiness, never payloads or implicit source rights.
         * Values are streams, sources, events, delivery bytes, retained workspace,
         * maximum steady deadline and the readiness field mask, in that order.
         */
        error event_plan(grant const& authority,std::string_view domain,communication::profile profile,std::array<std::uint64_t,7> const& values)noexcept
        {
            if(frozen_)return error::denied;
            if(authority.kind!=purpose::event||authority.binding!="event-stream"||!identifier(authority.alias))return error::denied;
            if(!workers_.contains(domain)||!communication_configuration::selected_profile(profile))return error::invalid;
            if(bindings_.contains(authority.alias))return error::conflict;
            if(bindings_.size()>=budget("bindings")||!values[0]||values[0]>65536||!values[1]||values[1]>64||
               !values[2]||values[2]>65536||values[1]>values[2]||!values[3]||values[3]>budget("output-bytes")||
               !values[4]||values[4]>(1ull<<30)||!values[5]||values[5]>INT64_MAX||!values[6]||(values[6]&~3ull))return error::invalid;
            std::uint64_t minimum{};if(!events::stream::required_workspace(values[1],values[2],minimum)||minimum>values[4])return error::too_large;
            try
            {
                communication_event_dispatch::plan plan;plan.policy.selected=std::move(profile);
                plan.max_streams=values[0];plan.policy.max_sources=values[1];plan.policy.max_events=values[2];
                plan.max_bytes=values[3];plan.max_workspace=values[4];plan.policy.max_deadline_ns=values[5];plan.policy.fields=values[6];
                auto local=::uwvm2::utils::container::make_shared<communication_configuration::definition>(communication_configuration::definition{
                    communication_configuration::family::event,communication_configuration::provider::local_event_stream,std::move(plan)});
                bindings_.emplace(authority.alias,binding{string{domain},0,std::move(local)});return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /** @brief Couple exactly two workers into one explicit transport/cleanup failure domain. */
        error pair_workers(std::string_view name,std::string_view left,std::string_view right,std::uint64_t retirement_ns)noexcept
        {
            if(frozen_)return error::denied;
            if(!identifier(name)||left==right||!retirement_ns||retirement_ns>60000000000ull)return error::invalid;
            if(pairs_.contains(name))return error::conflict;
            if(pairs_.size()>=budget("pairs"))return error::too_large;
            if(!workers_.contains(left)||!workers_.contains(right))return error::bad_handle;
            for(auto const& [key,value]:pairs_){(void)key;if(value.left==left||value.right==left||value.left==right||value.right==right)return error::conflict;}
            auto const& a=workers_.find(left)->second.definition.network;auto const& b=workers_.find(right)->second.definition.network;
            if(a.domain!=b.domain||a.local_participant!=b.peer_participant||b.local_participant!=a.peer_participant||
               a.local_prefix!=b.peer_prefix||b.local_prefix!=a.peer_prefix||a.local_discovery!=b.peer_discovery||
               b.local_discovery!=a.peer_discovery||a.local_user!=b.peer_user||b.local_user!=a.peer_user)return error::invalid;
            for(std::size_t i{};i<4;++i)if(a.allowed[i]!=b.allowed[i^1])return error::invalid;
            try{pairs_.emplace(string(name),pair{string(left),string(right),retirement_ns});return error::success;}
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        communication_host::limits host_limits()const noexcept
        {
            communication_host::limits value;auto& d=value.dispatch;auto& n=d.native;
            n.contexts=budget("contexts");n.nodes=budget("nodes");n.types=budget("types");n.topics=budget("topics");n.services=budget("services");
            n.total_queue_bytes=budget("queue-bytes");n.max_receipt_bytes=budget("receipt-bytes");
            d.bindings=budget("bindings");d.carrier_bytes=budget("carrier-bytes");d.output_bytes=budget("output-bytes");
            d.workspace_bytes=budget("workspace-bytes");d.workspace_calls=budget("workspace-calls");d.node_deadline_ns=budget("node-deadline-ns");
            value.creation={static_cast<std::size_t>(budget("creation-bindings")),budget("creation-workspace-bytes"),budget("creation-workspace-calls")};return value;
        }
        /** @brief Validate every selected origin, budget and worker image without native I/O.
         * @details Work on a private configuration copy when freezing multiple
         * environments. All workers must have one context, one node and one
         * explicit peer; unused endpoint definitions confer no authority. The
         * returned reservation is a conservative aggregate, not RSS prediction.
         */
        error freeze(std::uint64_t& reserved_bytes)noexcept
        {
            if(frozen_)return error::conflict;
            if(empty()){frozen_=true;reserved_bytes=0;return error::success;}
            if(assigned_!=all_budget||workers_.empty()||workers_.size()!=pairs_.size()*2)return error::invalid;
            try
            {
                using family=communication_configuration::family;
                std::array<std::uint64_t,8> counts{};std::uint64_t queues{},creation{},providers{},definitions{};
                for(auto const& [alias,value]:bindings_)
                {
                    (void)alias;auto item=definition(value);if(!item||!supported(item->kind))return error::invalid;
                    ++counts[static_cast<std::size_t>(item->kind)];if(item->kind==family::node||item->kind==family::topic||item->kind==family::service||item->kind==family::action)++creation;
                    auto const& node=workers_.at(value.worker_name);
                    if(item->kind==family::node)
                    {auto const& p=std::get<communication::node_plan>(item->policy);if(!origin(value.worker_name,family::context,p.context))return error::invalid;}
                    if(item->kind==family::topic)
                    {auto const& p=std::get<communication::topic_plan>(item->policy);if(!origin(value.worker_name,family::node,p.node)||!origin(value.worker_name,family::type,p.type)||
                        !add(p.queue_bytes,budget("queue-bytes"),queues))return error::invalid;}
                    if(item->kind==family::service)
                    {auto const& p=std::get<communication::service_plan>(item->policy);if(!origin(value.worker_name,family::node,p.node)||!origin(value.worker_name,family::type,p.request_type)||
                        !origin(value.worker_name,family::type,p.response_type)||!add(p.reserved_bytes,budget("queue-bytes"),queues))return error::invalid;}
                    if(item->kind==family::action)
                    {auto const& p=std::get<communication_action_dispatch::plan>(item->policy);if(!origin(value.worker_name,family::node,p.node_profile)||
                        !origin(value.worker_name,family::type,p.policy.goal.selected)||!origin(value.worker_name,family::type,p.policy.feedback.selected)||
                        !origin(value.worker_name,family::type,p.policy.result.selected)||!add(p.policy.reserved_bytes,budget("queue-bytes"),queues))return error::invalid;}
                    if(item->kind==family::graph)
                    {auto const& p=std::get<communication_graph_dispatch::plan>(item->policy);if(!origin(value.worker_name,family::node,p.node_profile)||
                        !origin(value.worker_name,family::context,p.context_profile)||!add(p.policy.max_bytes,budget("queue-bytes"),queues))return error::invalid;}
                    if(item->kind==family::event)
                    {auto const& p=std::get<communication_event_dispatch::plan>(item->policy);
                        if(!p.max_streams||p.max_workspace>budget("event-workspace-bytes")/workers_.size()/p.max_streams)return error::too_large;}
                    if(!node.definition_bytes)return error::invalid;
                }
                for(std::size_t i{};i<5;++i)if(counts[i]>budget(budget_fields[2+i].name))return error::too_large;
                if(counts[5]>budget("actions"))return error::too_large;
                if(counts[5]&&(budget("action-workspace-bytes")/workers_.size()<budget("carrier-bytes")+budget("output-bytes")||
                   budget("action-workspace-calls")/workers_.size()<2))return error::invalid;
                if(counts[6]>budget("graphs")||counts[7]>budget("event-bindings"))return error::too_large;
                if(counts[6]&&(budget("graph-workspace-bytes")/workers_.size()<budget("carrier-bytes")+budget("output-bytes")||
                    budget("graph-workspace-calls")/workers_.size()<2))return error::invalid;
                if(counts[7]&&(budget("event-workspace-bytes")/workers_.size()<budget("carrier-bytes")+budget("output-bytes")||
                    budget("event-workspace-calls")/workers_.size()<2))return error::invalid;
                if(counts[0]!=workers_.size()||counts[1]!=workers_.size()||creation>budget("creation-bindings"))return error::invalid;
                if(!budget("carrier-bytes")||budget("output-bytes")<communication_open_contract::published_bytes||!budget("workspace-calls")||
                   budget("workspace-bytes")/workers_.size()<budget("carrier-bytes")+budget("output-bytes")||
                   budget("workspace-calls")/workers_.size()<2||!budget("node-deadline-ns")||!budget("creation-workspace-calls")||
                   budget("creation-workspace-bytes")<520||budget("supervisor-stack-bytes")<65536||!budget("supervisor-poll-ns"))return error::invalid;
                table<communication_attachment::configuration> prepared;
                for(auto const& [name,value]:workers_)
                {
                    if(value.assigned!=all_peer)return error::invalid;
                    std::size_t topics{},services{},actions{},graphs{},nodes{},contexts{};
                    for(auto const& [alias,b]:bindings_)if(b.worker_name==name)
                    {auto item=definition(alias);topics+=item->kind==family::topic;services+=item->kind==family::service;actions+=item->kind==family::action;
                        graphs+=item->kind==family::graph;nodes+=item->kind==family::node;contexts+=item->kind==family::context;}
                    if(nodes!=1||contexts!=1)return error::invalid;
                    auto image=value.attachment;auto status=image.freeze();if(status!=error::success)return status;
                    auto const& bound=image.endpoints();auto const& l=value.limits;
                    if(l[0]<value.definition.response_bytes||l[1]>bound.stderr_bytes||!l[2]||!l[3]||
                       (topics?(!l[4]||!l[5]):(l[4]||l[5]))||(services?(!l[6]||!l[7]):(l[6]||l[7]))||
                       (actions?(!l[8]||!l[9]):(l[8]||l[9]))||(graphs?l[10]<graphs*2*(value.definition.request_bytes+value.definition.response_bytes):l[10]!=0))return error::invalid;
                    for(auto bytes:{value.definition.request_bytes,value.definition.response_bytes,
                                    l[0],l[1],l[4],l[5],l[6],l[7],l[8],l[9],l[10]})if(!add(bytes,budget("provider-bytes"),providers))return error::too_large;
                    if(!add(value.definition_bytes,budget("definition-bytes"),definitions))return error::too_large;
                    prepared.emplace(name,std::move(image));
                }
                std::uint64_t total{};for(auto bytes:{providers,definitions,queues,budget("receipt-bytes"),budget("workspace-bytes"),budget("creation-workspace-bytes"),budget("action-workspace-bytes"),
                    budget("graph-workspace-bytes"),budget("event-workspace-bytes"),
                    budget("supervisor-stack-bytes")*pairs_.size(),creation*2*budget("carrier-bytes")})
                    if(!add(bytes,INT64_MAX,total))return error::too_large;
                for(auto& [name,image]:prepared)workers_.at(name).attachment=std::move(image);
                reserved_bytes=total;frozen_=true;return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        /** @brief Bind owned plans to a new dispatcher, minting host-only owners. */
        error bind_plans(communication_dispatch& dispatcher)const noexcept
        {
            if(!frozen_)return error::denied;
            try
            {
                for(auto const& [alias,value]:bindings_)
                {
                    auto item=definition(value);if(!item)return error::invalid;
                    auto status=std::visit([&](auto const& plan)->error
                    {if constexpr(requires{dispatcher.bind(alias,plan);})return dispatcher.bind(alias,plan);else return error::unsupported;},item->policy);
                    if(status!=error::success)return status;
                }
                return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /** @brief Bind one private registry per worker, sharing only the authority environment.
         * @details Aggregate workspace is divided into explicit equal shares;
         * unused remainder grants no extra concurrent call. Native object and
         * queue capacities are the actual selected plans, not a copy of the
         * environment maximum for every worker. Repeated ROS schemas on separate
         * nodes cannot collide or provide each other's origin capabilities.
         */
        error bind_hosts(communication_host& host)const noexcept
        {
            if(!frozen_||workers_.empty())return error::denied;
            try
            {
                using family=communication_configuration::family;
                for(auto const& [name,worker]:workers_)
                {
                    (void)worker;auto limits=host_limits().dispatch;limits.native.contexts=limits.native.nodes=limits.native.types=limits.native.topics=limits.native.services=0;
                    limits.native.total_queue_bytes=0;limits.bindings=0;
                    communication_action_dispatch::limits actions{0,limits.carrier_bytes,limits.output_bytes,
                        budget("action-workspace-bytes")/workers_.size(),budget("action-workspace-calls")/workers_.size()};
                    communication_graph_dispatch::limits graphs{0,limits.carrier_bytes,limits.output_bytes,
                        budget("graph-workspace-bytes")/workers_.size(),budget("graph-workspace-calls")/workers_.size()};
                    communication_event_dispatch::limits events{0,limits.carrier_bytes,limits.output_bytes,
                        budget("event-workspace-bytes")/workers_.size(),budget("event-workspace-calls")/workers_.size()};
                    limits.workspace_bytes/=workers_.size();limits.workspace_calls/=workers_.size();
                    for(auto const& [alias,b]:bindings_)if(b.worker_name==name)
                    {
                        ++limits.bindings;auto item=definition(alias);
                        switch(item->kind)
                        {
                            case family::context:++limits.native.contexts;break;case family::node:++limits.native.nodes;break;case family::type:++limits.native.types;break;
                            case family::topic:++limits.native.topics;limits.native.total_queue_bytes+=std::get<communication::topic_plan>(item->policy).queue_bytes;break;
                            case family::service:++limits.native.services;limits.native.total_queue_bytes+=std::get<communication::service_plan>(item->policy).reserved_bytes;break;
                            case family::action:++actions.bindings;--limits.bindings;break;
                            case family::graph:++graphs.bindings;--limits.bindings;break;
                            case family::event:++events.bindings;--limits.bindings;break;
                            default:return error::unsupported;
                        }
                    }
                    auto status=host.add_domain(name,limits,actions,graphs,events);if(status!=error::success)return status;
                }
                for(auto const& [alias,b]:bindings_)
                {
                    auto dispatcher=host.dispatcher(b.worker_name);auto item=definition(b);if(!dispatcher||!item)return error::invalid;
                    auto status=std::visit([&](auto const& plan)->error
                    {if constexpr(std::is_same_v<std::remove_cvref_t<decltype(plan)>,communication_action_dispatch::plan>)
                        {auto actions=host.action_dispatcher(b.worker_name);return actions?actions->bind(alias,plan):error::denied;}
                     else if constexpr(std::is_same_v<std::remove_cvref_t<decltype(plan)>,communication_graph_dispatch::plan>)
                        {auto graphs=host.graph_dispatcher(b.worker_name);return graphs?graphs->bind(alias,plan):error::denied;}
                     else if constexpr(std::is_same_v<std::remove_cvref_t<decltype(plan)>,communication_event_dispatch::plan>)
                        {auto events=host.event_dispatcher(b.worker_name);return events?events->bind(alias,plan):error::denied;}
                     else if constexpr(requires{dispatcher->bind(alias,plan);})return dispatcher->bind(alias,plan);else return error::unsupported;},item->policy);
                    if(status!=error::success)return status;
                    status=host.route_alias(alias,b.worker_name);if(status!=error::success)return status;
                }
                return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
    };
}
