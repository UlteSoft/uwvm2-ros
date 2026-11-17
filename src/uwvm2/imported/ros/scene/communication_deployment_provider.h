/** @file
 * @brief Owned isolated communication deployment and one supervisor per process pair.
 * @details Preparation pins provisioned endpoints and binds exact dispatcher-minted
 * plans. It does not start ROS nodes. Each admitted node.open is advanced on its
 * pair's finite supervisor; native traffic is enabled only after bilateral
 * authority checks. No in-process plugin load or ambient guest socket exists.
 *
 * Pair supervisors have separately reserved guarded stacks and finite poll
 * periods. They inherit the preparing thread's CPU/NUMA placement; native peer
 * placement and sandbox verification belong to external deployment. Neither affinity
 * nor a poll period promises a real-time latency bound or exclusive core. A
 * stalled pair's cleanup does not occupy another pair's supervisor. Stop fences
 * are nonblocking; local joins run outside authority and registry locks.
 */
#pragma once
#include "communication_deployment_configuration.h"
#include "communication_worker_endpoint_open.h"
#include "communication_worker_action_open.h"
#include "communication_worker_graph_admission.h"
#include "communication_node_open_provider.h"
#include "communication_worker_activation.h"
#include "supervisor_stack.h" // Shared existing guarded fast_io supervisor stack.
namespace uwvm2::imported::ros::scene::communication_deployment_provider
{
#if defined(__linux__)
    namespace worker=communication_worker;
    using config=communication_deployment_configuration;
    using string=::uwvm2::utils::container::std_string;
    template<class T>using vector=::uwvm2::utils::container::std_vector<T>;
    /** @brief Immutable owner routing, with no matching by guest profile labels alone. */
    class routing_backend final:public communication::backend
    {
        struct row{std::array<communication::owner,4> owners;std::shared_ptr<worker::paired_factory> factory;};
        vector<row> routes;
        worker::paired_factory* find(communication::owner actual)const noexcept
        {for(auto const& route:routes)for(auto const& owner:route.owners)if(owner==actual)return route.factory.get();return nullptr;}
    public:
        void add(std::array<worker::peer_configuration,2> const& peers,std::shared_ptr<worker::paired_factory> factory)
        {routes.push_back({{peers[0].context.authority,peers[0].node.authority,peers[1].context.authority,peers[1].node.authority},std::move(factory)});}
        communication::error create_context(communication::context_plan const& plan,std::shared_ptr<communication::native_context>& output)noexcept override
        {auto target=find(plan.authority);return target?target->create_context(plan,output):communication::error::denied;}
        communication::error prepare_node(communication::node_plan const& plan,std::shared_ptr<communication::native_context> const& context,communication::node_stage_request const& request)noexcept override
        {auto target=find(plan.authority);return target?target->prepare_node(plan,context,request):communication::error::denied;}
        communication::error create_node(communication::node_plan const& plan,std::shared_ptr<communication::native_context> const& context,std::shared_ptr<communication::native_node>& output)noexcept override
        {auto target=find(plan.authority);return target?target->create_node(plan,context,output):communication::error::denied;}
        communication::error create_topic(communication::topic_plan const&,communication::type_plan const&,std::shared_ptr<communication::native_node> const&,
            communication::handle,std::shared_ptr<communication::receive_queue> const&,std::shared_ptr<communication::native_topic>&)noexcept override
        {return communication::error::unsupported;}
        void retire_owner(communication::owner owner)noexcept override{if(auto target=find(owner))target->retire_owner(owner);}
        void context_closed(communication::owner owner,handle context)noexcept override{if(auto target=find(owner))target->context_closed(owner,context);}
        void capability_closed(handle capability)noexcept override{for(auto const& row:routes)row.factory->capability_closed(capability);}
    };
    /** @brief A joinable pair supervisor retaining every node cleanup coordinator.
     * @details The broker must outlive this owner and retire it before destroying
     * its communication_host. The thread starts only after all bindings freeze.
     * Stop does not erase unknown outcomes or establish native cleanup. Pair
     * cleanup remains unknown after disclosure until trusted external evidence exists.
     */
    class supervisor final:public communication::provider_lifetime
    {
        communication_host* host{};
        std::shared_ptr<worker::paired_factory> factory;
        vector<string> aliases;
        vector<std::shared_ptr<communication_node_open_provider>> nodes;
        model_worker::supervisor_stack stack;
        pthread_t thread{};pid_t process{::getpid()};
        std::mutex retirement;
        std::atomic<bool> stopped{},running{};
        std::uint64_t poll_ns{};bool started{},joined{};
        bool same_process()const noexcept{return process==::getpid();}
        static void* run(void* opaque)noexcept
        {
            auto& self=*static_cast<supervisor*>(opaque);
            sigset_t blocked;::sigemptyset(&blocked);::sigaddset(&blocked,SIGPIPE);::pthread_sigmask(SIG_BLOCK,&blocked,nullptr);
            self.running.store(true,std::memory_order_release);
            while(!self.stopped.load(std::memory_order_acquire))
            {
                for(auto const& alias:self.aliases)
                {if(self.stopped.load())break;(void)self.host->pump(alias);}
                if(!self.stopped.load())(void)self.factory->pump_graphs();
                if(self.factory->inspect().stopping)break;
                timespec delay{static_cast<time_t>(self.poll_ns/1000000000),static_cast<long>(self.poll_ns%1000000000)};
                // The finite cadence is explicit deployment policy, not a rate
                // grant or evidence that a native result completed by a deadline.
                while(::nanosleep(&delay,&delay)&&errno==EINTR&&!self.stopped.load()){}
            }
            // A pair failure may end the loop before the other node receives
            // another pump. Fence every ledger before any potentially slow
            // retirement; otherwise its guest could poll a dead supervisor.
            self.request_stop();self.factory->retire();
            for(auto const& node:self.nodes)(void)node->reconcile_cleanup();
            self.running.store(false,std::memory_order_release);return nullptr;
        }
    public:
        supervisor(communication_host& target,std::shared_ptr<worker::paired_factory> actual,std::uint64_t interval)
            :host(&target),factory(std::move(actual)),poll_ns(interval){}
        ~supervisor()override{retire();}
        void add(string alias,std::shared_ptr<communication_node_open_provider> node={})
        {aliases.push_back(std::move(alias));if(node)nodes.push_back(std::move(node));}
        error start(std::uint64_t bytes)noexcept
        {
            if(!same_process()||started||stopped.load()||!factory||!poll_ns||poll_ns>100000000)return error::invalid;
            auto status=stack.prepare(bytes);if(status!=error::success)return status;
            pthread_attr_t attributes;if(::pthread_attr_init(&attributes))return error::io;
            auto prepared=::pthread_attr_setstack(&attributes,stack.data(),stack.size());
            auto result=prepared?prepared: ::pthread_create(&thread,&attributes,run,this);::pthread_attr_destroy(&attributes);
            if(result)return error::io;
            started=true;return error::success;
        }
        void request_stop()noexcept override
        {
            if(!same_process())return;
            stopped.store(true,std::memory_order_release);
            for(auto const& node:nodes)node->request_stop();
            factory->request_stop();
        }
        void retire()noexcept override
        {
            if(!same_process())return;
            request_stop();std::lock_guard lock{retirement};
            if(started&&!joined){if(::pthread_join(thread,nullptr))return;joined=true;}
            factory->retire();for(auto const& node:nodes)(void)node->reconcile_cleanup();
        }
        bool cleanup_confirmed()const noexcept override
        {return same_process()&&!running.load(std::memory_order_acquire)&&factory->cleanup_confirmed();}
    };
    struct prepared
    {
        config::table<std::shared_ptr<supervisor>> supervisors;
        /** @brief Start all pair supervisors only after complete private preparation.
         * @details A partial start is stopped and joined before returning failure.
         * The caller publishes this result only after every start succeeds.
         */
        error start(std::uint64_t stack_bytes)noexcept
        {
            for(auto const& [name,value]:supervisors)
            {
                (void)name;auto status=value->start(stack_bytes);if(status==error::success)continue;
                for(auto const& [key,active]:supervisors){(void)key;active->request_stop();}
                for(auto const& [key,active]:supervisors){(void)key;active->retire();}
                return status;
            }
            return error::success;
        }
    };
    /** @brief Prepare all selected process pairs against the actual communication host.
     * @details The caller supplies a fresh private host with already bound plans.
     * Failure may fence that host; it must be discarded and never published.
     * Image descriptors close after factory sealing. No worker starts here and
     * no permission is inferred from an approved image. observation is an exact
     * trusted clock mapping, separately supplied by the enclosing broker.
     */
    inline error prepare(config const& configuration,communication_host& host,prepared& output,
                         communication_dispatch::observation_clock observation={})noexcept
    {
        if(!configuration.frozen()||!output.supervisors.empty())return error::invalid;
        try
        {
            auto routing=::uwvm2::utils::container::make_shared<routing_backend>();prepared next;
            auto limits=configuration.host_limits();
            for(auto const& [name,pair]:configuration.pairs())
            {
                auto factory=::uwvm2::utils::container::make_shared<worker::paired_factory>();
                auto owner=::uwvm2::utils::container::make_shared<supervisor>(host,factory,configuration.supervisor_poll_ns());
                std::array<worker::peer_configuration,2> peers;
                std::array<vector<communication_creation::plan>,2> topics,services,actions;
                std::array<std::string_view,2> names{pair.left,pair.right};
                std::array<std::shared_ptr<communication_dispatch>,2> dispatchers{host.dispatcher(names[0]),host.dispatcher(names[1])};
                if(!dispatchers[0]||!dispatchers[1])return error::denied;
                for(std::size_t i{};i<2;++i)
                {
                    auto dispatcher=dispatchers[i];auto admission=::uwvm2::utils::container::make_shared<worker::dispatch_admission>(dispatcher);
                    auto action_dispatcher=host.action_dispatcher(names[i]);
                    auto const& source=configuration.workers().find(names[i])->second;auto& peer=peers[i];auto const& bound=source.limits;
                    peer.attachment=&source.attachment;peer.definition=&source.definition;error status{};
                    peer.rpc={source.definition.request_bytes,source.definition.response_bytes,bound[0],bound[1],bound[2],bound[3]};
                    for(auto const& [alias,binding]:configuration.bindings())if(binding.worker_name==names[i])
                    {
                        auto const& definition=*configuration.definition(alias);
                        using family=communication_configuration::family;using kind=communication_open_contract::endpoint_kind;
                        if(definition.kind==family::context){status=dispatcher->describe_plan(alias,peer.context);if(status!=error::success)return status;continue;}
                        if(definition.kind==family::type||definition.kind==family::graph||definition.kind==family::event)continue;
                        communication_creation::plan creation;creation.worker_binding=binding.definition+1;
                        creation.selected=communication_worker_configuration::profile_of(definition);creation.provider_generation=source.definition.network.generation;
                        creation.carrier_bytes=limits.dispatch.carrier_bytes;
                        if(definition.kind==family::node)
                        {
                            status=dispatcher->describe_plan(alias,peer.node);if(status!=error::success)return status;
                            creation.authority=peer.node.authority;creation.kind=kind::node;creation.max_deadline_ns=limits.dispatch.node_deadline_ns;
                            auto node=::uwvm2::utils::container::make_shared<communication_node_open_provider>();
                            status=communication_dispatch::convert(node->configure(alias,creation,dispatcher,factory));if(status!=error::success)return status;
                            status=host.bind(alias,kind::node,node);if(status!=error::success)return status;owner->add(alias,std::move(node));continue;
                        }
                        if(definition.kind==family::action)
                        {
                            if(!action_dispatcher)return error::denied;
                            communication_action_dispatch::plan plan;status=action_dispatcher->describe_plan(alias,plan);if(status!=error::success)return status;
                            creation.authority=plan.policy.authority;creation.kind=kind::action;creation.max_deadline_ns=plan.policy.max_deadline_ns;actions[i].push_back(creation);
                            auto endpoint=::uwvm2::utils::container::make_shared<worker::action_open_provider>();
                            status=communication_dispatch::convert(endpoint->configure(alias,action_dispatcher,factory));if(status!=error::success)return status;
                            status=host.bind(alias,kind::action,endpoint);if(status!=error::success)return status;owner->add(alias);continue;
                        }
                        if(definition.kind==family::topic)
                        {communication::topic_plan plan;status=dispatcher->describe_plan(alias,plan);if(status!=error::success)return status;
                            creation.authority=plan.authority;creation.kind=kind::topic;creation.max_deadline_ns=plan.max_deadline_ns;topics[i].push_back(creation);}
                        else if(definition.kind==family::service)
                        {communication::service_plan plan;status=dispatcher->describe_plan(alias,plan);if(status!=error::success)return status;
                            creation.authority=plan.authority;creation.kind=kind::service;creation.max_deadline_ns=plan.max_deadline_ns;services[i].push_back(creation);}
                        else return error::unsupported;
                        auto endpoint=::uwvm2::utils::container::make_shared<worker::endpoint_open_provider>();
                        status=communication_dispatch::convert(endpoint->configure(alias,creation.kind,dispatcher,factory));if(status!=error::success)return status;
                        status=host.bind(alias,creation.kind,endpoint);if(status!=error::success)return status;owner->add(alias);
                    }
                    peer.topic_openings=topics[i];peer.topic_carrier_bytes=bound[4];peer.topic_bytes=bound[5];
                    if(!topics[i].empty()){peer.admit_topic=worker::dispatch_admission::admit_open;peer.charge_topic=worker::dispatch_admission::charge_open;peer.topic_authority=admission.get();peer.topic_authority_owner=admission;}
                    peer.service_openings=services[i];peer.service_carrier_bytes=bound[6];peer.service_bytes=bound[7];
                    if(!services[i].empty()){peer.admit_service=worker::dispatch_admission::admit_open;peer.charge_service=worker::dispatch_admission::charge_open;peer.service_authority=admission.get();peer.service_authority_owner=admission;}
                    peer.action_openings=actions[i];peer.action_carrier_bytes=bound[8];peer.action_bytes=bound[9];
                    if(!actions[i].empty())
                    {auto authorized=::uwvm2::utils::container::make_shared<worker::action_admission>(action_dispatcher);
                        peer.admit_action=worker::action_admission::admit_open;peer.charge_action=worker::action_admission::charge_open;
                        peer.action_authority=authorized.get();peer.action_authority_owner=std::move(authorized);}
                    peer.graph_query_bytes=bound[10];
                    if(auto graph=host.graph_dispatcher(names[i]))
                    {
                        auto bridge=::uwvm2::utils::container::make_shared<worker::graph_factory>();bridge->provider=factory;
                        bridge->admission=::uwvm2::utils::container::make_shared<worker::graph_admission>(graph);
                        status=graph->prepare({bridge.get(),worker::graph_factory::create,true,bridge});if(status!=error::success)return status;
                    }
                }
                auto activation=::uwvm2::utils::container::make_shared<worker::activation_admission>(dispatchers);
                auto status=communication_dispatch::convert(factory->configure(peers,worker::activation_admission::admit,activation.get(),pair.retirement_ns,activation));
                if(status!=error::success)return status;
                routing->add(peers,factory);next.supervisors.emplace(name,std::move(owner));
            }
            auto status=host.prepare(routing,observation);if(status!=error::success)return status;
            output=std::move(next);return error::success;
        }
        catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
    }
#endif
}
