/** @file
 * @brief Finite host routing for canonical robot communication and creation.
 * @details Deployment binds approved plans and exact retained creation providers
 * before preparation. The guest supplies only its current capability and owned
 * canonical bytes. It cannot choose a native provider, process, endpoint token,
 * host path or callback. Opening returns a creation receipt; independent take
 * publishes the typed resource. All default capacities and permissions are zero.
 *
 * Supervision is explicit: pump() performs one finite pass over configured
 * providers outside authority/registry locks. Invocations and cached inspection
 * never start a hidden thread or wait for native work. The enclosing deployment
 * owns worker stop/reap and CPU/NUMA placement, and drains workers before final
 * destruction. This class is the broker composition layer, not a native provider
 * or a claim that the VM import/CLI boundary has been qualified.
 */
#pragma once
#include "communication_dispatch.h"
#include "communication_open_dispatch.h"
#include "communication_action_dispatch.h"
#include "communication_graph_dispatch.h"
#include "communication_event_dispatch.h"
#include <uwvm2/utils/container/shared.h>
namespace uwvm2::imported::ros::scene
{
    class communication_host
    {
    public:
        struct limits
        {communication_dispatch::limits dispatch;communication_open_dispatch::limits creation;};
        using operation=communication_contract::operation;
        using creation_operation=communication_open_dispatch::operation;
        using sink=communication_dispatch::sink;
        using cancellation=communication_open_dispatch::cancellation;
    private:
        struct source
        {
            ::uwvm2::utils::container::std_string alias;
            communication_open_contract::endpoint_kind family{};
            std::shared_ptr<communication_open_dispatch::provider> retained;
            error(*invoke)(communication_open_dispatch::provider*,caller,handle,wire::bytes,std::size_t,std::size_t&,sink,cancellation)noexcept{};
            communication::error(*pump)(communication_open_dispatch::provider*)noexcept{};
        };
        std::shared_ptr<authority> permissions;
        std::shared_ptr<communication_dispatch> core;
        struct domain{::uwvm2::utils::container::std_string name;std::shared_ptr<communication_dispatch> dispatcher;
            std::shared_ptr<communication_action_dispatch> actions;std::shared_ptr<communication_graph_dispatch> graphs;std::shared_ptr<communication_event_dispatch> events;};
        struct route{::uwvm2::utils::container::std_string alias;std::shared_ptr<communication_dispatch> dispatcher;
            std::shared_ptr<communication_action_dispatch> actions;std::shared_ptr<communication_graph_dispatch> graphs;std::shared_ptr<communication_event_dispatch> events;};
        ::uwvm2::utils::container::std_vector<domain> domains;
        ::uwvm2::utils::container::std_vector<route> routes;
        std::uint64_t environment_identity{},generation_identity{};
        communication_open_dispatch creation;
        ::uwvm2::utils::container::std_vector<source> sources;
        std::size_t maximum{};
        mutable std::mutex configuration;
        std::atomic<bool> ready{},retired{};
#if defined(__linux__)
        pid_t process{::getpid()};
#endif
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return process==::getpid();
#else
            return true;
#endif
        }
        bool available()const noexcept{return same_process()&&ready.load(std::memory_order_acquire)&&!retired.load();}
        std::shared_ptr<communication_dispatch> target(std::string_view alias)const noexcept
        {if(domains.empty())return core;for(auto const& row:routes)if(row.alias==alias)return row.dispatcher;return {};}
        std::shared_ptr<communication_action_dispatch> action_target(std::string_view alias)const noexcept
        {for(auto const& row:routes)if(row.alias==alias)return row.actions;return {};}
        template<class Dispatcher> std::shared_ptr<Dispatcher> observation_target(std::string_view alias)const noexcept
        {
            for(auto const& row:routes)if(row.alias==alias)
            {if constexpr(std::is_same_v<Dispatcher,communication_graph_dispatch>)return row.graphs;else return row.events;}
            return {};
        }
        template<class Dispatcher> error select_observation(caller who,handle cap,std::string_view action,std::shared_ptr<Dispatcher>& result)const
        {
            if(!available())return error::denied;
            lease allowed;auto status=permissions->admit(who,cap,action,0,allowed,false);if(status!=error::success)return status;
            auto const& policy=allowed.specification();
            if constexpr(std::is_same_v<Dispatcher,communication_graph_dispatch>)
            {if(policy.kind!=purpose::discovery||policy.binding!="native-graph")return error::denied;}
            else if(policy.kind!=purpose::event||policy.binding!="event-stream")return error::denied;
            result=observation_target<Dispatcher>(policy.alias);return result?error::success:error::denied;
        }
        template<auto Action>using observation_dispatcher=std::conditional_t<std::is_same_v<decltype(Action),graph_contract::operation>,communication_graph_dispatch,communication_event_dispatch>;
        template<auto Action>static constexpr auto const& observation_operations()noexcept
        {if constexpr(std::is_same_v<decltype(Action),graph_contract::operation>)return graph_contract::operations;else return event_contract::operations;}
        error select_action(caller who,handle cap,std::string_view action,std::shared_ptr<communication_action_dispatch>& result)const noexcept
        {
            if(!available())return error::denied;
            lease allowed;auto status=permissions->admit(who,cap,action,0,allowed,false);if(status!=error::success)return status;
            if(allowed.specification().kind!=purpose::action||allowed.specification().binding!="native-action")return error::denied;
            result=action_target(allowed.specification().alias);return result?error::success:error::denied;
        }
        error select(caller who,handle capability,std::string_view action,std::shared_ptr<communication_dispatch>& result)
        {
            if(!available())return error::denied;
            lease allowed;auto status=permissions->admit(who,capability,action,0,allowed,false);if(status!=error::success)return status;
            auto const& policy=allowed.specification();
            result=target(service_observer(policy)?policy.resource:policy.alias);
            return result?error::success:error::denied;
        }
        static bool creates(operation action)noexcept
        {return action==operation::node_open||action==operation::topic_open||action==operation::service_open;}
        error resolve(caller who,operation action,handle capability,source const*& result)noexcept
        {
            if(!available())return error::denied;
            auto index=static_cast<std::size_t>(action);if(index>=communication_contract::operations.size())return error::unsupported;
            lease allowed;auto status=permissions->admit(who,capability,communication_contract::operations[index].name,0,allowed,false);
            if(status!=error::success)return status;
            using kind=communication_open_contract::endpoint_kind;
            auto expected=action==operation::node_open?kind::node:action==operation::topic_open?kind::topic:kind::service;
            for(auto const& row:sources)if(row.alias==allowed.specification().alias&&row.family==expected)
            {result=&row;return error::success;}
            return error::denied;
        }
    public:
        communication_host(std::shared_ptr<authority> actual,std::uint64_t environment,std::uint64_t generation,limits capacity)
            :permissions(std::move(actual)),core(::uwvm2::utils::container::make_shared<communication_dispatch>(permissions,environment,generation,capacity.dispatch)),
             environment_identity(environment),generation_identity(generation),creation(permissions,capacity.creation),maximum(capacity.creation.bindings){}
        communication_host(communication_host const&)=delete;
        communication_host& operator=(communication_host const&)=delete;
        ~communication_host(){retire();}
        /// @brief Host-only access for immutable plan binding and provider preparation.
        std::shared_ptr<communication_dispatch> dispatcher()const noexcept{return same_process()?core:std::shared_ptr<communication_dispatch>{};}
        /** @brief Mint a private registry for one sealed worker in the same authority environment.
         * @details Equal ROS type/profile names on distinct workers remain local
         * to their registries. The actual authority instance is shared for atomic
         * bilateral admission. Named domains cannot be added after preparation.
         */
        error add_domain(::uwvm2::utils::container::std_string name,communication_dispatch::limits capacity,
                         communication_action_dispatch::limits action_capacity={},communication_graph_dispatch::limits graph_capacity={},
                         communication_event_dispatch::limits event_capacity={})noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{configuration};if(ready.load()||retired.load()||!sources.empty())return error::denied;
            if(!identifier(name))return error::invalid;
            for(auto const& row:domains)if(row.name==name)return error::conflict;
            if(domains.size()>=128)return error::too_large;
            try{auto next=::uwvm2::utils::container::make_shared<communication_dispatch>(permissions,environment_identity,generation_identity,capacity);
                std::shared_ptr<communication_action_dispatch> actions;
                std::shared_ptr<communication_graph_dispatch> graphs;std::shared_ptr<communication_event_dispatch> events;
                if(action_capacity.bindings)actions=::uwvm2::utils::container::make_shared<communication_action_dispatch>(permissions,*next,environment_identity,generation_identity,action_capacity);
                if(graph_capacity.bindings)graphs=::uwvm2::utils::container::make_shared<communication_graph_dispatch>(permissions,*next,environment_identity,generation_identity,graph_capacity);
                if(event_capacity.bindings)events=::uwvm2::utils::container::make_shared<communication_event_dispatch>(permissions,environment_identity,generation_identity,event_capacity);
                domains.push_back({std::move(name),std::move(next),std::move(actions),std::move(graphs),std::move(events)});return error::success;}
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        std::shared_ptr<communication_dispatch> dispatcher(std::string_view name)const noexcept
        {if(!same_process())return {};for(auto const& row:domains)if(row.name==name)return row.dispatcher;return {};}
        /// @brief Exact worker-local action registry; absent action capacity supplies no dispatcher.
        std::shared_ptr<communication_action_dispatch> action_dispatcher(std::string_view name)const noexcept
        {if(!same_process())return {};for(auto const& row:domains)if(row.name==name)return row.actions;return {};}
        std::shared_ptr<communication_graph_dispatch> graph_dispatcher(std::string_view name)const noexcept
        {if(!same_process())return {};for(auto const& row:domains)if(row.name==name)return row.graphs;return {};}
        std::shared_ptr<communication_event_dispatch> event_dispatcher(std::string_view name)const noexcept
        {if(!same_process())return {};for(auto const& row:domains)if(row.name==name)return row.events;return {};}
        /// @brief Bind an existing logical grant alias to one exact private registry without granting it.
        error route_alias(::uwvm2::utils::container::std_string alias,std::string_view name)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{configuration};if(ready.load()||retired.load())return error::denied;
            if(!identifier(alias))return error::invalid;
            for(auto const& row:routes)if(row.alias==alias)return error::conflict;
            auto selected=dispatcher(name);if(!selected)return error::bad_handle;
            if(routes.size()>=1536)return error::too_large;
            try{routes.push_back({std::move(alias),std::move(selected),action_dispatcher(name),graph_dispatcher(name),event_dispatcher(name)});return error::success;}
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /** @brief Retain a typed source for one exact dispatcher-minted plan.
         * @details Provider preparation must already have matched this plan.
         * The source's own inspect/prepare/publication paths independently verify
         * its owner and family. No native work, authority debit or handle
         * publication occurs here. Failure leaves no callable partial row.
         */
        template<class Provider>
        error bind(::uwvm2::utils::container::std_string alias,communication_open_contract::endpoint_kind family,std::shared_ptr<Provider> actual)noexcept
        {
            static_assert(std::is_base_of_v<communication_open_dispatch::provider,Provider>);
            if(!same_process()||!actual)return error::denied;
            std::lock_guard lock{configuration};if(ready.load()||retired.load())return error::denied;
            if(!maximum||maximum>128||sources.size()>=maximum)return error::too_large;
            for(auto const& row:sources)if(row.alias==alias)return error::conflict;
            try
            {
                auto selected=target(alias);if(!selected)return error::denied;
                communication::owner identity;error status;using kind=communication_open_contract::endpoint_kind;
                if(family==kind::node){communication::node_plan plan;status=selected->describe_plan(alias,plan);identity=plan.authority;}
                else if(family==kind::topic){communication::topic_plan plan;status=selected->describe_plan(alias,plan);identity=plan.authority;}
                else if(family==kind::service){communication::service_plan plan;status=selected->describe_plan(alias,plan);identity=plan.authority;}
                else if(family==kind::action){auto action=action_target(alias);if(!action)return error::denied;
                    communication_action_dispatch::plan plan;status=action->describe_plan(alias,plan);identity=plan.policy.authority;}
                else return error::invalid;
                if(status!=error::success)return status;
                sources.reserve(maximum);source pending;pending.alias=alias;pending.family=family;pending.retained=actual;
                pending.invoke=[](auto* provider,caller who,handle cap,wire::bytes body,std::size_t capacity,std::size_t& required,sink deliver,cancellation stopped)noexcept
                {return static_cast<Provider*>(provider)->invoke_open(who,cap,body,capacity,required,deliver,stopped);};
                pending.pump=[](auto* provider)noexcept{return static_cast<Provider*>(provider)->pump();};
                status=creation.bind(std::move(alias),identity,family,std::move(actual));if(status!=error::success)return status;
                sources.push_back(std::move(pending));return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        /** @brief Freeze routing and reserve bounded workspaces without node startup. */
        error prepare(std::shared_ptr<communication::backend> actual,communication_dispatch::observation_clock clock={})noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{configuration};if(ready.load()||retired.load()||sources.empty())return error::denied;
            try
            {
                auto status=error::success;
                if(domains.empty())status=core->prepare_all_async(actual,clock);
                else for(auto const& row:domains){status=row.dispatcher->prepare_all_async(actual,clock);if(status!=error::success)break;}
                if(status!=error::success){retire();return status;}
                for(auto const& row:domains)if(row.actions)
                {status=row.actions->prepare_async();if(status!=error::success){retire();return status;}}
                for(auto const& row:domains)if(row.events)
                {status=row.events->prepare(communication_event_dispatch::source_access::from(*this));if(status!=error::success){retire();return status;}}
                status=creation.prepare();if(status!=error::success){retired=true;core->retire();return status;}
                ready.store(true,std::memory_order_release);return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        error preflight(caller who,operation action,handle cap,std::size_t extent)noexcept
        {
            if(!available())return error::denied;
            auto index=static_cast<std::size_t>(action);if(index>=communication_contract::operations.size())return error::unsupported;
            std::shared_ptr<communication_dispatch> selected;auto status=select(who,cap,communication_contract::operations[index].name,selected);
            if(status!=error::success)return status;
            status=selected->preflight(who,action,cap,extent);if(status!=error::success||!creates(action))return status;
            source const* row{};return resolve(who,action,cap,row);
        }
        error reserve_workspace(caller who,operation action,handle cap,std::size_t extent,communication_dispatch::workspace_lease& held)
        {
            auto status=preflight(who,action,cap,extent);if(status!=error::success)return status;
            std::shared_ptr<communication_dispatch> selected;status=select(who,cap,communication_contract::operations[static_cast<std::size_t>(action)].name,selected);
            return status==error::success?selected->reserve_workspace(who,action,cap,extent,held):status;
        }
        error invoke(caller who,operation action,handle cap,wire::bytes body,std::size_t capacity,std::size_t& required,sink deliver,cancellation stopped={})noexcept
        {
            required=0;if(!available())return error::denied;
            if(!creates(action))
            {
                auto index=static_cast<std::size_t>(action);if(index>=communication_contract::operations.size())return error::unsupported;
                std::shared_ptr<communication_dispatch> selected;auto status=select(who,cap,communication_contract::operations[index].name,selected);
                return status==error::success?selected->invoke(who,action,cap,body,capacity,required,deliver,stopped):status;
            }
            source const* row{};auto status=resolve(who,action,cap,row);if(status!=error::success)return status;
            return row->invoke(row->retained.get(),who,cap,body,capacity,required,deliver,stopped);
        }
        error preflight_creation(caller who,creation_operation action,handle cap,std::size_t extent)noexcept
        {return available()?creation.preflight(who,action,cap,extent):error::denied;}
        error invoke_creation(caller who,creation_operation action,handle cap,wire::bytes body,std::size_t capacity,std::size_t& required,sink deliver,cancellation stopped={})noexcept
        {required=0;return available()?creation.invoke(who,action,cap,body,capacity,required,deliver,stopped):error::denied;}
        error preflight_receive(caller who,communication_receive_contract::operation action,handle cap,std::size_t extent)noexcept
        {
            auto index=static_cast<std::size_t>(action);if(index>=communication_receive_contract::operations.size())return error::unsupported;
            std::shared_ptr<communication_dispatch> selected;auto status=select(who,cap,communication_receive_contract::operations[index].name,selected);
            return status==error::success?selected->preflight_receive(who,action,cap,extent):status;
        }
        error invoke_receive(caller who,communication_receive_contract::operation action,handle cap,wire::bytes body,std::size_t capacity,std::size_t& required,sink deliver,cancellation stopped={})noexcept
        {
            required=0;auto index=static_cast<std::size_t>(action);if(index>=communication_receive_contract::operations.size())return error::unsupported;
            std::shared_ptr<communication_dispatch> selected;auto status=select(who,cap,communication_receive_contract::operations[index].name,selected);
            return status==error::success?selected->invoke_receive(who,action,cap,body,capacity,required,deliver,stopped):status;
        }
        /** @brief Route a trusted no-I/O receive-debt query under exact cleanup authority. */
        error topic_receive_quiescent(caller who,handle cap,handle id,bool& output)
        {
            output=false;std::shared_ptr<communication_dispatch> selected;
            auto status=select(who,cap,"topic.release",selected);
            return status==error::success?selected->topic_receive_quiescent(who,cap,id,output):status;
        }
        /** @brief Route one action carrier by its current capability and exact worker domain. */
        error preflight_action(caller who,action_contract::operation action,handle cap,std::size_t extent)
        {
            auto index=static_cast<std::size_t>(action);if(index>=action_contract::operations.size())return error::unsupported;
            std::shared_ptr<communication_action_dispatch> selected;auto status=select_action(who,cap,action_contract::operations[index].name,selected);
            return status==error::success?selected->preflight(who,action,cap,extent):status;
        }
        error reserve_action_workspace(caller who,action_contract::operation action,handle cap,std::size_t extent,communication_workspace::lease& output)
        {
            auto index=static_cast<std::size_t>(action);if(index>=action_contract::operations.size())return error::unsupported;
            std::shared_ptr<communication_action_dispatch> selected;auto status=select_action(who,cap,action_contract::operations[index].name,selected);
            return status==error::success?selected->reserve_workspace(who,action,cap,extent,output):status;
        }
        error invoke_action(caller who,action_contract::operation action,handle cap,wire::bytes body,std::size_t capacity,std::size_t& required,sink deliver,cancellation stopped={})
        {
            required=0;auto index=static_cast<std::size_t>(action);if(index>=action_contract::operations.size())return error::unsupported;
            std::shared_ptr<communication_action_dispatch> selected;auto status=select_action(who,cap,action_contract::operations[index].name,selected);
            if(status!=error::success)return status;
            if(action!=action_contract::operation::open)return selected->invoke(who,action,cap,body,capacity,required,deliver,stopped);
            lease allowed;status=permissions->admit(who,cap,"action.open",0,allowed,false);if(status!=error::success)return status;
            for(auto const& row:sources)if(row.alias==allowed.specification().alias&&row.family==communication_open_contract::endpoint_kind::action)
            {allowed.release();return row.invoke(row.retained.get(),who,cap,body,capacity,required,deliver,stopped);}
            return error::denied;
        }
        error close(caller who,handle cap,handle resource)
        {
            if(!available())return error::denied;
            lease allowed;auto status=permissions->admit(who,cap,"resource.close",0,allowed,false);if(status!=error::success)return status;
            if(allowed.specification().kind==purpose::action)
            {auto selected=action_target(allowed.specification().alias);allowed.release();return selected?selected->close(who,cap,resource):error::denied;}
            if(allowed.specification().kind==purpose::discovery)
            {auto selected=observation_target<communication_graph_dispatch>(allowed.specification().alias);allowed.release();return selected?selected->close(who,cap,resource):error::denied;}
            if(allowed.specification().kind==purpose::event)
            {auto selected=observation_target<communication_event_dispatch>(allowed.specification().alias);allowed.release();return selected?selected->close(who,cap,resource):error::denied;}
            std::shared_ptr<communication_dispatch> selected;status=select(who,cap,"resource.close",selected);return status==error::success?selected->close(who,cap,resource):status;
        }
        /** @brief Attach an existing node through its actual capability's private registry.
         * @details This grants no node and starts no process. Dependent providers
         * must additionally enforce their configured node alias. The dispatcher
         * checks the original context, generation and counted observer capacity.
         */
        error attach_node(caller who,handle cap,handle node,communication::profile const& profile,communication::node_lease& output)
        {std::shared_ptr<communication_dispatch> selected;auto status=select(who,cap,"node.open",selected);
            return status==error::success?selected->attach_node(who,cap,node,profile,output):status;}
        /// @brief Revalidate an exact dependent node without consuming node-creation quota.
        error validate_node(caller who,handle cap,handle node,communication::profile const& profile,communication::owner owner)
        {std::shared_ptr<communication_dispatch> selected;auto status=select(who,cap,"node.open",selected);
            return status==error::success?selected->validate_node(who,cap,node,profile,owner):status;}
        /** @brief Perform one bounded supervisor pass without waiting or issuing authority.
         * @details Continue other already accepted work if one provider fails.
         * The first failure is returned for supervision; each provider retains
         * its own outcome/cleanup evidence. No rate limit is silently refunded.
         */
        error pump()noexcept
        {
            if(!available())return error::denied;
            error failure=error::success;bool progressed{};
            for(auto const& row:sources)
            {
                auto status=row.pump(row.retained.get());
                if(status==communication::error::ok)progressed=true;
                else if(status!=communication::error::again&&status!=communication::error::busy&&failure==error::success)failure=static_cast<error>(status);
            }
            return failure!=error::success?failure:progressed?error::success:error::again;
        }
        /** @brief Advance one exact deployment-owned source on its pair's supervisor.
         * @details The alias is trusted host routing, never guest authority. The
         * source rechecks original capability groups before every native effect.
         * Separate supervisors prevent one pair's bounded cleanup from blocking
         * progress in an unrelated pair. Configuration remains immutable.
         */
        error pump(std::string_view alias)noexcept
        {
            if(!available())return error::denied;
            for(auto const& row:sources)if(row.alias==alias)return communication_dispatch::convert(row.pump(row.retained.get()));
            return error::bad_handle;
        }
        error inspect(std::string_view alias,communication_dispatch::usage& usage)const noexcept
        {auto selected=same_process()?target(alias):nullptr;return selected?selected->inspect(alias,usage):error::denied;}
        error inspect_action(std::string_view alias,communication_action_dispatch::usage& usage)const noexcept
        {auto selected=same_process()?action_target(alias):nullptr;return selected?selected->inspect(alias,usage):error::denied;}
        /** @brief Bounded graph/event transport selected by current capability, never resource-number guessing. */
        template<auto Action>error preflight_observation(caller who,handle cap,std::size_t extent)
        {
            constexpr auto index=static_cast<std::size_t>(Action);if constexpr(index>=observation_operations<Action>().size())return error::unsupported;
            std::shared_ptr<observation_dispatcher<Action>> selected;auto status=select_observation(who,cap,observation_operations<Action>()[index].name,selected);
            return status==error::success?selected->preflight(who,Action,cap,extent):status;
        }
        template<auto Action>error reserve_observation_workspace(caller who,handle cap,std::size_t extent,communication_workspace::lease& held)
        {
            constexpr auto index=static_cast<std::size_t>(Action);if constexpr(index>=observation_operations<Action>().size())return error::unsupported;
            std::shared_ptr<observation_dispatcher<Action>> selected;auto status=select_observation(who,cap,observation_operations<Action>()[index].name,selected);
            return status==error::success?selected->reserve_workspace(who,Action,cap,extent,held):status;
        }
        template<auto Action>error invoke_observation(caller who,handle cap,wire::bytes body,std::size_t capacity,std::size_t& required,sink deliver,cancellation stopped={})
        {
            required=0;constexpr auto index=static_cast<std::size_t>(Action);if constexpr(index>=observation_operations<Action>().size())return error::unsupported;
            std::shared_ptr<observation_dispatcher<Action>> selected;auto status=select_observation(who,cap,observation_operations<Action>()[index].name,selected);
            return status==error::success?selected->invoke(who,Action,cap,body,capacity,required,deliver,stopped):status;
        }
        template<class Dispatcher>error inspect_observation(std::string_view alias,typename Dispatcher::usage& usage)const noexcept
        {auto selected=same_process()?observation_target<Dispatcher>(alias):nullptr;return selected?selected->inspect(alias,usage):error::denied;}
        /** @brief Resolve readiness by separately granted source purpose and complete origin chain.
         * @details Stream membership does not grant the source. A graph watch is
         * refreshed with each poll's caller. Other sources expose cached queue
         * readiness only. No payload, endpoint address or native handle is exposed.
         */
        error event_source(caller who,handle cap,handle resource,events::binding& output)
        {
            if(!available())return error::denied;
            lease granted;auto status=permissions->dependency(who,cap,"resource.observe",granted);if(status!=error::success)return status;
            auto const& policy=granted.specification();
            if(policy.kind==purpose::action&&policy.binding=="native-action")
            {auto selected=action_target(policy.alias);granted.release();return selected?selected->event_source(who,cap,resource,output):error::denied;}
            if(policy.kind==purpose::discovery&&policy.binding=="native-graph")
            {auto selected=observation_target<communication_graph_dispatch>(policy.alias);granted.release();return selected?selected->event_source(who,cap,resource,output):error::denied;}
            if(policy.binding!="native-communication")return error::denied;
            auto selected=target(policy.alias);granted.release();return selected?selected->event_source(who,cap,resource,output):error::denied;
        }
        error validate_source(caller who,handle cap,events::binding const& source)
        {
            if(!available())return error::denied;
            lease granted;auto status=permissions->dependency(who,cap,"resource.observe",granted);if(status!=error::success)return status;
            auto const& policy=granted.specification();
            if(policy.kind==purpose::action&&policy.binding=="native-action")
            {auto selected=action_target(policy.alias);granted.release();return selected?selected->validate_source(who,cap,source):error::denied;}
            if(policy.kind==purpose::discovery&&policy.binding=="native-graph")
            {auto selected=observation_target<communication_graph_dispatch>(policy.alias);granted.release();return selected?selected->validate_source(who,cap,source):error::denied;}
            if(policy.binding!="native-communication")return error::denied;
            auto selected=target(policy.alias);granted.release();return selected?selected->validate_source(who,cap,source):error::denied;
        }
        communication::error observe_source(caller who,handle cap,events::binding const& source,events::snapshot& output)
        {
            auto status=validate_source(who,cap,source);if(status!=error::success)return static_cast<communication::error>(status);
            lease granted;status=permissions->dependency(who,cap,"resource.observe",granted);if(status!=error::success)return static_cast<communication::error>(status);
            if(granted.specification().kind==purpose::discovery)
            {auto selected=observation_target<communication_graph_dispatch>(granted.specification().alias);granted.release();
                return selected?selected->observe_source(who,cap,source,output):communication::error::denied;}
            granted.release();auto observed=source.provider->inspect(source.origin,source.resource,source.generation,output);
            status=validate_source(who,cap,source);return status==error::success?observed:static_cast<communication::error>(status);
        }
        void capability_closed(handle cap)noexcept{if(same_process()){core->capability_closed(cap);for(auto const& row:domains)row.dispatcher->capability_closed(cap);}}
        void revoke(std::string_view alias)noexcept{if(same_process()){
            if(auto selected=observation_target<communication_event_dispatch>(alias))selected->revoke(alias);
            if(auto selected=observation_target<communication_graph_dispatch>(alias))selected->revoke(alias);
            if(auto selected=action_target(alias))selected->revoke(alias);
            if(auto selected=target(alias))selected->revoke(alias);}}
        /// @brief Fence routes without waiting; the deployment separately retires native workers.
        void retire()noexcept{if(same_process()){retired=true;creation.retire();for(auto const& row:domains){
            if(row.events)row.events->retire();
            if(row.graphs)row.graphs->retire();
            if(row.actions)row.actions->retire();}
            core->retire();for(auto const& row:domains)row.dispatcher->retire();}}
    };
}
