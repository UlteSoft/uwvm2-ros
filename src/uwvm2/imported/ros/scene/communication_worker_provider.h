/** @file
 * @brief One frozen pair of externally provisioned ROS peers with bilateral admission.
 * @details configure() verifies the exact startup profile from each approved
 * definition, duplicates the registered channels and reserves objects without spawning.
 * create_context() opens only a logical private context. prepare_node() advances
 * the caller-bound nonblocking node stages after complete dispatcher preflight;
 * neither pipe attachment nor readiness publishes native discovery.
 *
 * create_node() commits one admitted node. Only when BOTH nodes committed does
 * the trusted admission callback acquire both current node/context groups in
 * the SAME authority environment. Those leases remain held through activation.
 * Missing/failed admission fences the pair. The node grant enables only node
 * discovery metadata. Explicit topic creation plans independently admit each
 * exact asynchronous open and construct a typed facade only from a verified
 * creation receipt. Services reserve independent request/response correlations
 * and local-interest cancellation capacity. Action integration is separate.
 * Graph attachment requires a separate explicit query reservation and an exact
 * sealed filter. Its supervisor pumps bounded IPC with fresh call authority;
 * the node grant alone cannot send or disclose a graph observation.
 *
 * The pair is one deployment incarnation: immutable plans, profiles, capacities,
 * callback and process identities cannot be replaced, even after retirement.
 * Revocation/capability closure fences both dependent routes without waiting.
 * retire() runs outside authority/registry locks, joins the relay and drains RPC
 * users before releasing borrowed descriptors. Unknown namespace cleanup stays
 * explicit debt; it never authorizes another worker or releases capacity.
 */
#pragma once
#include "communication_provider.h"
#include "communication_worker_stage.h"
#include "communication_worker_channel.h"
#include "communication_private_link.h"
#include "communication_worker_configuration.h"
#include "communication_worker_graph.h"
#include "communication_worker_openings.h"
#include "communication_worker_topic.h"
#include "communication_worker_service.h"
#include "communication_worker_action.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
#if defined(__linux__)
    namespace channel=communication_worker_channel;
    namespace configuration=communication_worker_configuration;
    /** @brief Trusted finite input, borrowed only during configure().
     * @details Inherited endpoint descriptors may be closed after successful configuration.
     * RPC capacities are explicit and cannot exceed the sealed profile. A finite
     * retirement wait merely bounds teardown waiting; expiry retains cleanup
     * debt. It is never evidence that a namespace or native effect disappeared.
     */
    struct peer_configuration
    {
        communication_attachment::configuration const* attachment{};
        configuration::specification const* definition{};
        communication::context_plan context;
        communication::node_plan node;
        channel::limits rpc;
        /// @brief Explicit aggregate graph query payload reservation; zero disables graph attachment.
        std::uint64_t graph_query_bytes{};
        /// @brief Exact topic creation reservations; empty disables native topic creation.
        std::span<communication_creation::plan const> topic_openings{};
        std::uint64_t topic_carrier_bytes{},topic_bytes{};
        openings::admit admit_topic{};
        communication_creation::charge charge_topic{};
        void* topic_authority{};
        /// @brief Exact service creation and adapter reservations; empty/zero disables services.
        std::span<communication_creation::plan const> service_openings{};
        std::uint64_t service_carrier_bytes{},service_bytes{};
        openings::admit admit_service{};
        communication_creation::charge charge_service{};
        void* service_authority{};
        /// @brief Exact action creation, three-schema and correlation reservations; empty disables actions.
        std::span<communication_creation::plan const> action_openings{};
        std::uint64_t action_carrier_bytes{},action_bytes{};
        openings::admit admit_action{};
        communication_creation::charge charge_action{};
        void* action_authority{};
        /** @brief Retain exact callback contexts through creation-queue retirement.
         * @details When present, get() must equal the corresponding borrowed
         * authority pointer. Contexts should refer weakly to their dispatcher,
         * so factory ownership cannot create an authority/lifetime cycle.
         */
        std::shared_ptr<void> topic_authority_owner,service_authority_owner,action_authority_owner;
    };
    struct provider_snapshot
    {
        bool configured{},stopping{},drained{},cleanup_confirmed{};
        std::array<stage_snapshot,2> stages;
        communication_transport::private_link_snapshot link;
    };
    class paired_factory final:public communication::backend,public communication::provider_lifetime
    {
        using error=communication::error;
        struct logical_context final:communication::native_context
        {
            std::atomic<bool> active{true};
            std::shared_ptr<communication_attachment::connection> worker;
            explicit logical_context(std::shared_ptr<communication_attachment::connection> value)noexcept:worker(std::move(value)){}
            bool live()const noexcept override{return active.load(std::memory_order_acquire);}
            /** @brief Fence local context and signal its exact retained worker without waiting.
             * @details The supervisor still owns confirmed namespace cleanup;
             * this signal does not establish middleware or physical completion.
             */
            error shutdown()noexcept override{active.store(false,std::memory_order_release);if(worker)worker->request_stop();return error::ok;}
        };
        struct logical_node final:communication::native_node
        {
            std::shared_ptr<communication_attachment::connection> worker;
            handle generation{};
            bool committed{};
            logical_node(std::shared_ptr<communication_attachment::connection> value,handle incarnation)noexcept:worker(std::move(value)),generation(incarnation){}
            handle provider_generation()const noexcept override{return generation;}
            /** @brief Only a committed node owns the worker's lifetime effect.
             * @details Storage is reserved before stage.commit(), which may
             * return again when the liveness poll is interrupted. Destroying
             * that provisional allocation must leave the existing stage and
             * process intact for retry. After commit, final destruction fences
             * the worker even when bilateral activation later fails.
             */
            ~logical_node()override{if(committed)worker->request_stop();}
        };
        /** @brief One frozen binding's capacity, held before native creation.
         * @details Successful facade construction moves preparation into its
         * final-destroyed member. Registry/provider/observer references therefore
         * retain capacity until all actual buffers are destroyed. A close reply
         * alone cannot authorize a second allocation. Each immutable binding has
         * one slot; the sum of all slots is checked against the deployment budget.
         */
        struct endpoint_account
        {
            communication_workspace capacity;
            communication_workspace::lease preparation;
            std::uint64_t bytes{};
            handle request{},resource{};
        };
        struct participant
        {
            // Destroyed after every queue that can invoke these contexts.
            std::shared_ptr<void> topic_authority_owner,service_authority_owner,action_authority_owner;
            node_stage stage;
            configuration::specification definition;
            buffers::profile profile;
            communication::context_plan context_plan;
            communication::node_plan node_plan;
            communication::node_stage_request attempt;
            std::shared_ptr<logical_context> context;
            std::weak_ptr<logical_node> node;
            communication_worker_async::transport rpc;
            channel::limits capacity;
            ::uwvm2::utils::container::std_vector<std::shared_ptr<graph_source>> graphs;
            openings topic_creation;
            ::uwvm2::utils::container::std_vector<std::shared_ptr<topic_endpoint>> topics;
            ::uwvm2::utils::container::std_vector<communication::owner> topic_owners;
            ::uwvm2::utils::container::std_vector<endpoint_account> topic_accounts;
            std::mutex topic_submission;
            bool topics_enabled{};
            openings service_creation;
            ::uwvm2::utils::container::std_vector<std::shared_ptr<service_endpoint>> services;
            ::uwvm2::utils::container::std_vector<communication::owner> service_owners;
            ::uwvm2::utils::container::std_vector<endpoint_account> service_accounts;
            std::mutex service_submission;
            bool services_enabled{};
            openings action_creation;
            ::uwvm2::utils::container::std_vector<std::shared_ptr<action_endpoint>> actions;
            ::uwvm2::utils::container::std_vector<std::shared_ptr<communication::actions::endpoint>> action_cores;
            ::uwvm2::utils::container::std_vector<communication::owner> action_owners;
            ::uwvm2::utils::container::std_vector<endpoint_account> action_accounts;
            std::mutex action_submission;
            bool actions_enabled{};
            std::uint64_t graph_limit{},graph_reserved{};
            std::atomic<communication::handle> node_capability{},context_capability{};
            bool attempted{},channel_ready{},committed{};
        };
        // Retained activation callback outlives all participant startup/drain work.
        std::shared_ptr<void> activation_authority_owner;
        std::array<::uwvm2::utils::container::std_unique_ptr<participant>,2> peers;
        communication_transport::private_link link;
        communication::admit_pair admission{};void* admission_context{};
        std::uint64_t retirement_ns{};
        std::atomic<bool> configured{},stopping{},drained{},confirmed{};
        mutable std::mutex mutex;std::mutex retirement_mutex;
        pid_t owner{::getpid()};bool link_prepared{},link_active{};
        bool same_process()const noexcept{return owner==::getpid();}
        template<typename Endpoint>
        error enqueue_endpoint(openings& creation,std::mutex& submission,endpoint_account& account,
                               std::shared_ptr<Endpoint> const& retained,communication_creation::identity const& origin,
                               wire::bytes canonical,communication_creation::view& output)noexcept
        {
            // Reentrant admission returns again. No factory lock precedes an
            // authority callback, and a failed enqueue only releases its own slot.
            std::unique_lock serialized{submission,std::try_to_lock};if(!serialized.owns_lock())return error::again;
            bool acquired{};
            {
                std::lock_guard lock{mutex};if(stopping.load())return error::cancelled;
                communication_creation::view previous;
                bool replay=account.request&&creation.inspect(origin.who,account.request,previous)==error::ok&&previous.receipt.key==origin.key;
                if(!replay&&!retained&&!account.preparation)
                {
                    auto status=account.capacity.reserve(account.bytes,account.preparation);
                    if(status!=scene::error::success)return static_cast<error>(status);
                    acquired=true;
                }
            }
            auto status=creation.enqueue(origin,canonical,output);
            {
                std::lock_guard lock{mutex};
                if(status==error::ok&&account.preparation){account.request=output.receipt.request_id;account.resource=0;}
                else if(acquired)account.preparation.reset();
            }
            return status;
        }
        template<typename Endpoint>
        error reconcile_endpoint(openings& creation,::uwvm2::utils::container::std_vector<endpoint_account>& accounts,
                                 ::uwvm2::utils::container::std_vector<std::shared_ptr<Endpoint>>& endpoints,handle request,handle resource)noexcept
        {
            std::shared_ptr<Endpoint> dropped;
            {
                std::lock_guard lock{mutex};
                for(std::size_t i{};i<accounts.size();++i)if(accounts[i].request==request)
                {
                    if(!resource||accounts[i].resource!=resource)return error::denied;
                    if(!endpoints[i])return error::ok;
                    if(endpoints[i]->resource()!=resource||!endpoints[i]->cleanup_confirmed())return error::busy;
                    auto status=creation.cleanup(request,true);if(status!=error::ok)return status;
                    dropped=std::move(endpoints[i]);break;
                }
            }
            // Destruction may drain queues. It must not run under the factory
            // lock; external references continue to hold the slot's actual bytes.
            return dropped?error::ok:error::bad_handle;
        }
        template<typename Endpoint>
        error release_endpoint(openings& creation,std::mutex& submission,::uwvm2::utils::container::std_vector<endpoint_account>& accounts,
                               ::uwvm2::utils::container::std_vector<std::shared_ptr<Endpoint>> const& endpoints,caller who,handle request)noexcept
        {
            std::unique_lock serialized{submission,std::try_to_lock};if(!serialized.owns_lock())return error::again;
            std::lock_guard lock{mutex};auto status=creation.release(who,request);if(status!=error::ok)return status;
            for(std::size_t i{};i<accounts.size();++i)if(accounts[i].request==request)
            {if(!endpoints[i])accounts[i].preparation.reset();break;}
            return error::ok;
        }
        static bool same(communication::context_plan const& a,communication::context_plan const& b)noexcept
        {return a.authority==b.authority&&a.selected==b.selected&&a.domain==b.domain&&a.enclave==b.enclave&&a.network==b.network;}
        static bool same(communication::node_plan const& a,communication::node_plan const& b)noexcept
        {return a.authority==b.authority&&a.selected==b.selected&&a.context==b.context&&a.name==b.name&&a.name_space==b.name_space&&a.max_observers==b.max_observers;}
        static bool same(communication::topic_plan const& a,communication::topic_plan const& b)noexcept
        {return a.authority==b.authority&&a.selected==b.selected&&a.node==b.node&&a.type==b.type&&a.resolved_name==b.resolved_name&&a.direction==b.direction&&
            a.quality==b.quality&&a.max_message_bytes==b.max_message_bytes&&a.queue_depth==b.queue_depth&&a.queue_bytes==b.queue_bytes&&
            a.overflow_policy==b.overflow_policy&&a.max_deadline_ns==b.max_deadline_ns;}
        static bool same(communication::service_plan const& a,communication::service_plan const& b)noexcept
        {return a.authority==b.authority&&a.selected==b.selected&&a.node==b.node&&a.request_type==b.request_type&&a.response_type==b.response_type&&
            a.resolved_name==b.resolved_name&&a.service_type==b.service_type&&a.direction==b.direction&&a.quality==b.quality&&a.pending==b.pending&&
            a.max_request_bytes==b.max_request_bytes&&a.max_response_bytes==b.max_response_bytes&&a.reserved_bytes==b.reserved_bytes&&a.max_deadline_ns==b.max_deadline_ns;}
        static bool same(communication::type_plan const& a,communication::type_plan const& b)noexcept
        {return a.authority==b.authority&&a.selected==b.selected&&a.type_identity==b.type_identity&&a.encoding==b.encoding&&a.descriptor==b.descriptor&&
            a.validate==b.validate&&a.revoked==b.revoked;}
        static bool same(communication::actions::plan const& a,communication::actions::plan const& b)noexcept
        {return a.authority==b.authority&&a.selected==b.selected&&a.resolved_name==b.resolved_name&&a.action_type==b.action_type&&a.direction==b.direction&&
            a.goal_quality==b.goal_quality&&a.result_quality==b.result_quality&&a.cancel_quality==b.cancel_quality&&a.feedback_quality==b.feedback_quality&&a.status_quality==b.status_quality&&
            same(a.goal,b.goal)&&same(a.feedback,b.feedback)&&same(a.result,b.result)&&a.max_goals==b.max_goals&&a.max_cancels_per_goal==b.max_cancels_per_goal&&
            a.feedback_slots==b.feedback_slots&&a.max_goal_bytes==b.max_goal_bytes&&a.max_feedback_bytes==b.max_feedback_bytes&&a.max_result_bytes==b.max_result_bytes&&
            a.reserved_bytes==b.reserved_bytes&&a.transition_mask==b.transition_mask&&a.max_deadline_ns==b.max_deadline_ns&&a.retention_ns==b.retention_ns;}
        error failed(error status)noexcept{request_stop();return status;}
        error connect()noexcept
        {
            if(stopping.load())return error::cancelled;
            if(!link_prepared)
            {
                if(peers[0]->stage.inspect().phase!=stage_phase::ready||peers[1]->stage.inspect().phase!=stage_phase::ready)return error::again;
                if(!link.prepare({peers[0]->definition.network,peers[1]->definition.network},
                    {peers[0]->stage.retained_process(),peers[1]->stage.retained_process()},{peers[0]->profile,peers[1]->profile}))return failed(error::io);
                link_prepared=true;
                for(std::size_t i{};i<2;++i)
                {
                    auto status=peers[i]->stage.attach();if(status!=error::ok&&status!=error::again)return failed(status);
                }
            }
            bool ready=true;
            for(auto& peer:peers)
            {
                auto phase=peer->stage.inspect().phase;
                if(phase==stage_phase::waiting_attachment)
                {auto status=peer->stage.attachment_progress();if(status==error::again){ready=false;continue;}if(status!=error::ok)return failed(status);}
                else if(phase!=stage_phase::attached&&phase!=stage_phase::committed)return failed(error::cancelled);
                if(!peer->channel_ready)
                {
                    auto child=peer->stage.retained_process();auto capacity=peer->capacity;auto used=peer->stage.inspect().diagnostic_bytes;
                    if(used>capacity.stderr_bytes)return failed(error::too_large);
                    capacity.stderr_bytes-=used;
                    auto status=peer->rpc.prepare({child->input_fd(),child->output_fd(),child->stderr_fd(),child->process_fd(),child->session_until(),child->stop_flag()},capacity,peer->definition.network.generation);
                    if(status!=error::ok)return failed(status);
                    peer->channel_ready=true;
                }
            }
            return ready?error::ok:error::again;
        }
        error activate(std::unique_lock<std::mutex>& serialized)noexcept
        {
            if(!peers[0]->committed||!peers[1]->committed)return error::ok;
            if(link_active)return error::ok;
            if(!admission||stopping.load()||!peers[0]->context->live()||!peers[1]->context->live()||
                peers[0]->node.expired()||peers[1]->node.expired())return failed(error::denied);
            std::array<communication::activation_origin,2> origins;
            for(std::size_t i{};i<2;++i)origins[i]={&peers[i]->node_plan,&peers[i]->context_plan,&peers[i]->attempt};
            // Frozen participants outlive this call. Do not retain a node here:
            // closing its final registry reference must still cancel activation.
            // No factory lock may precede acquisition of an authority lease.
            serialized.unlock();
            communication::activation_guard held;
            auto status=admission(admission_context,origins,held);
            serialized.lock(); // Fixed order: bilateral authority -> factory.
            if(status!=error::ok||!held)return failed(status==error::ok?error::denied:status);
            if(stopping.load()||!peers[0]->context->live()||!peers[1]->context->live()||peers[0]->node.expired()||peers[1]->node.expired()||
                peers[0]->stage.inspect().phase!=stage_phase::committed||peers[1]->stage.inspect().phase!=stage_phase::committed||!link.activate())return failed(error::cancelled);
            link_active=true;return error::ok;
        }
    public:
        paired_factory()=default;~paired_factory()override{retire();}
        /** @brief Freeze two exact same-environment manifests and a mandatory authority callback.
         * @details No worker is started. Profiles decoded from hash-verified
         * manifest bytes must exactly match both trusted node/context plans.
         * All VM allocations use recoverable fast_io. Failure leaves no usable
         * partial provider, and callers may not concurrently configure/retire.
         */
        error configure(std::array<peer_configuration,2> const& selected,communication::admit_pair admit,void* user,
                        std::uint64_t retirement_wait_ns,std::shared_ptr<void> authority_owner={})noexcept
        {
            if(!same_process()||configured.load()||stopping.load())return error::denied;
            if(!admit||!retirement_wait_ns||retirement_wait_ns>60000000000ull)return error::invalid;
            if(authority_owner&&authority_owner.get()!=user)return error::invalid;
            try
            {
                std::array<::uwvm2::utils::container::std_unique_ptr<participant>,2> pending;
                auto environment=selected[0].node.authority.environment;if(!environment)return error::denied;
                for(std::size_t i{};i<2;++i)
                {
                    auto const& input=selected[i];
                    if((input.topic_authority_owner&&input.topic_authority_owner.get()!=input.topic_authority)||
                        (input.service_authority_owner&&input.service_authority_owner.get()!=input.service_authority)||
                        (input.action_authority_owner&&input.action_authority_owner.get()!=input.action_authority))return error::invalid;
                    if(!input.attachment||!input.definition||!input.node.authority||!input.context.authority||input.node.authority.environment!=environment||input.context.authority.environment!=environment)return error::denied;
                    auto value=::uwvm2::utils::container::make_unique<participant>();value->definition=*input.definition;error status{};
                    if(!input.attachment->frozen()||!configuration::origins(value->definition))return error::invalid;
                    value->topic_authority_owner=input.topic_authority_owner;value->service_authority_owner=input.service_authority_owner;
                    value->action_authority_owner=input.action_authority_owner;
                    auto context=configuration::find(value->definition,communication_configuration::family::context,input.context.selected);
                    auto node=configuration::find(value->definition,communication_configuration::family::node,input.node.selected);
                    if(!context||!node)return error::denied;
                    auto cp=std::get<communication::context_plan>(context->policy);auto np=std::get<communication::node_plan>(node->policy);
                    cp.authority=input.context.authority;np.authority=input.node.authority;
                    if(!same(cp,input.context)||!same(np,input.node)||np.context!=cp.selected)return error::denied;
                    auto const& bounds=input.rpc;
                    if(bounds.request_bytes!=value->definition.request_bytes||bounds.response_bytes!=value->definition.response_bytes||
                        bounds.retained_bytes<bounds.response_bytes||bounds.retained_bytes>64u*1024u*1024u||bounds.stderr_bytes>input.attachment->endpoints().stderr_bytes||
                        !bounds.deadline_ns||bounds.deadline_ns>60000000000ull||!bounds.poll_ns||bounds.poll_ns>100000000)return error::invalid;
                    if(input.graph_query_bytes>(1ull<<30))return error::invalid;
                    value->context_plan=input.context;value->node_plan=input.node;value->capacity=bounds;value->profile=input.attachment->profile();
                    value->graph_limit=input.graph_query_bytes;value->graphs.resize(value->definition.definitions.size());
                    if(input.topic_bytes>(1ull<<30))return error::invalid;
                    if(!input.topic_openings.empty())
                    {
                        if(!input.topic_bytes||!input.admit_topic||!input.charge_topic)return error::invalid;
                        value->topic_owners.resize(value->definition.definitions.size());
                        value->topic_accounts.resize(value->definition.definitions.size());
                        std::uint64_t required_topics{};
                        for(auto const& plan:input.topic_openings)
                        {
                            if(plan.kind!=communication_open_contract::endpoint_kind::topic||plan.authority.environment!=environment||
                                plan.provider_generation!=value->definition.network.generation||!plan.worker_binding||plan.worker_binding>value->definition.definitions.size())return error::denied;
                            auto const& definition=value->definition.definitions[plan.worker_binding-1];
                            if(definition.kind!=communication_configuration::family::topic||configuration::profile_of(definition)!=plan.selected||
                                std::get<communication::topic_plan>(definition.policy).node!=input.node.selected)return error::denied;
                            auto const& topic=std::get<communication::topic_plan>(definition.policy);
                            auto mutation=2*bounds.request_bytes+bounds.response_bytes;
                            auto needed=mutation+2*(bounds.request_bytes+bounds.response_bytes)+(topic.direction==communication::role::publisher?mutation:
                                bounds.request_bytes+3*bounds.response_bytes+topic.max_message_bytes)+endpoint_closure::reserved_bytes;
                            if(required_topics>input.topic_bytes||needed>input.topic_bytes-required_topics)return error::too_large;
                            required_topics+=needed;
                            value->topic_owners[plan.worker_binding-1]=plan.authority;
                            auto& account=value->topic_accounts[plan.worker_binding-1];account.bytes=needed;
                            status=static_cast<error>(account.capacity.configure(needed,1));if(status!=error::ok)return status;
                        }
                        status=value->topic_creation.configure(input.topic_openings,input.topic_carrier_bytes,input.admit_topic,input.charge_topic,input.topic_authority);
                        if(status!=error::ok)return status;
                        value->topics_enabled=true;value->topics.resize(value->definition.definitions.size());
                    }
                    else if(input.topic_bytes||input.topic_carrier_bytes||input.admit_topic||input.charge_topic||input.topic_authority||input.topic_authority_owner)return error::invalid;
                    if(input.service_bytes>(1ull<<30))return error::invalid;
                    if(!input.service_openings.empty())
                    {
                        if(!input.service_bytes||!input.admit_service||!input.charge_service)return error::invalid;
                        value->service_owners.resize(value->definition.definitions.size());std::uint64_t required_services{};
                        value->service_accounts.resize(value->definition.definitions.size());
                        for(auto const& plan:input.service_openings)
                        {
                            if(plan.kind!=communication_open_contract::endpoint_kind::service||plan.authority.environment!=environment||
                                plan.provider_generation!=value->definition.network.generation||!plan.worker_binding||plan.worker_binding>value->definition.definitions.size())return error::denied;
                            auto const& definition=value->definition.definitions[plan.worker_binding-1];
                            if(definition.kind!=communication_configuration::family::service||configuration::profile_of(definition)!=plan.selected||
                                std::get<communication::service_plan>(definition.policy).node!=input.node.selected)return error::denied;
                            auto needed=service_endpoint::reservation_bytes(std::get<communication::service_plan>(definition.policy),bounds.request_bytes,bounds.response_bytes);
                            if(!needed||required_services>input.service_bytes||needed>input.service_bytes-required_services)return error::too_large;
                            required_services+=needed;value->service_owners[plan.worker_binding-1]=plan.authority;
                            auto& account=value->service_accounts[plan.worker_binding-1];account.bytes=needed;
                            status=static_cast<error>(account.capacity.configure(needed,1));if(status!=error::ok)return status;
                        }
                        status=value->service_creation.configure(input.service_openings,input.service_carrier_bytes,input.admit_service,input.charge_service,input.service_authority);
                        if(status!=error::ok)return status;
                        value->services_enabled=true;value->services.resize(value->definition.definitions.size());
                    }
                    else if(input.service_bytes||input.service_carrier_bytes||input.admit_service||input.charge_service||input.service_authority||input.service_authority_owner)return error::invalid;
                    if(input.action_bytes>(1ull<<30))return error::invalid;
                    if(!input.action_openings.empty())
                    {
                        if(!input.action_bytes||!input.admit_action||!input.charge_action)return error::invalid;
                        value->action_owners.resize(value->definition.definitions.size());value->action_accounts.resize(value->definition.definitions.size());
                        std::uint64_t required_actions{};
                        for(auto const& plan:input.action_openings)
                        {
                            if(plan.kind!=communication_open_contract::endpoint_kind::action||plan.authority.environment!=environment||
                                plan.provider_generation!=value->definition.network.generation||!plan.worker_binding||plan.worker_binding>value->definition.definitions.size())return error::denied;
                            auto const& definition=value->definition.definitions[plan.worker_binding-1];
                            if(definition.kind!=communication_configuration::family::action||configuration::profile_of(definition)!=plan.selected)return error::denied;
                            auto const& wrapper=std::get<communication_action_dispatch::plan>(definition.policy);
                            if(wrapper.node_profile!=input.node.selected)return error::denied;
                            auto needed=action_endpoint::reservation_bytes(wrapper.policy,bounds.request_bytes,bounds.response_bytes);
                            if(!needed||required_actions>input.action_bytes||needed>input.action_bytes-required_actions)return error::too_large;
                            required_actions+=needed;value->action_owners[plan.worker_binding-1]=plan.authority;
                            auto& account=value->action_accounts[plan.worker_binding-1];account.bytes=needed;
                            status=static_cast<error>(account.capacity.configure(needed,1));if(status!=error::ok)return status;
                        }
                        status=value->action_creation.configure(input.action_openings,input.action_carrier_bytes,input.admit_action,input.charge_action,input.action_authority);
                        if(status!=error::ok)return status;
                        value->actions_enabled=true;value->actions.resize(value->definition.definitions.size());value->action_cores.resize(value->definition.definitions.size());
                    }
                    else if(input.action_bytes||input.action_carrier_bytes||input.admit_action||input.charge_action||input.action_authority||input.action_authority_owner)return error::invalid;
                    status=value->stage.configure(*input.attachment,input.node);if(status!=error::ok)return status;
                    pending[i]=std::move(value);
                }
                if(same(pending[0]->node_plan,pending[1]->node_plan)||same(pending[0]->context_plan,pending[1]->context_plan)||
                    pending[0]->stage.retained_process()->overlaps(*pending[1]->stage.retained_process()))return error::invalid;
                peers=std::move(pending);admission=admit;admission_context=user;retirement_ns=retirement_wait_ns;
                activation_authority_owner=std::move(authority_owner);
                configured.store(true,std::memory_order_release);return error::ok;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        error create_context(communication::context_plan const& plan,std::shared_ptr<communication::native_context>& output)noexcept override
        {
            if(!same_process()||!configured.load()||stopping.load())return error::denied;
            std::lock_guard lock{mutex};if(stopping.load())return error::cancelled;
            try{for(auto& peer:peers)if(same(peer->context_plan,plan))
            {if(peer->context)return error::busy;peer->context=::uwvm2::utils::container::make_shared<logical_context>(peer->stage.retained_process());output=peer->context;return error::ok;}}
            catch(...){return error::no_memory;}
            return error::denied;
        }
        /** @brief Advance at most bounded startup/attachment work, with no waits or publication. */
        error prepare_node(communication::node_plan const& plan,std::shared_ptr<communication::native_context> const& context,
                           communication::node_stage_request const& request)noexcept override
        {
            if(!same_process()||!configured.load()||stopping.load())return error::denied;
            std::lock_guard lock{mutex};if(stopping.load())return error::cancelled;
            for(auto& peer:peers)if(same(peer->node_plan,plan))
            {
                if(peer->context!=context||request.context_authority!=peer->context_plan.authority||!context||!request.caller_instance||
                    !request.caller_generation||!request.node_capability||!request.context_capability||!request.context)return error::denied;
                if(peer->attempted&&peer->attempt!=request)return error::denied;
                if(!peer->attempted){peer->attempt=request;peer->attempted=true;peer->node_capability.store(request.node_capability);peer->context_capability.store(request.context_capability);}
                auto phase=peer->stage.inspect().phase;
                if(phase!=stage_phase::waiting_attachment&&phase!=stage_phase::attached)
                {auto status=peer->stage.prepare_node(plan,context,request);if(status!=error::ok)return status==error::again?status:failed(status);}
                return connect();
            }
            return error::denied;
        }
        /** @brief Commit one node; bilateral leases gate the first discovery packet. */
        error create_node(communication::node_plan const& plan,std::shared_ptr<communication::native_context> const& context,
                          std::shared_ptr<communication::native_node>& output)noexcept override
        {
            if(!same_process()||!configured.load()||stopping.load())return error::denied;
            std::unique_lock lock{mutex};if(stopping.load())return error::cancelled;
            try{for(auto& peer:peers)if(same(peer->node_plan,plan))
            {
                if(peer->context!=context||!peer->attempted||!peer->channel_ready)return error::denied;
                if(peer->committed)return error::busy;
                auto value=::uwvm2::utils::container::make_shared<logical_node>(peer->stage.retained_process(),peer->definition.network.generation);
                auto status=peer->stage.commit(plan,context,peer->attempt);if(status!=error::ok)return status;
                value->committed=true;
                peer->node=value;peer->committed=true;
                status=activate(lock);if(status!=error::ok)return status;
                output=std::move(value);return error::ok;
            }}catch(...){return error::no_memory;}
            return error::denied;
        }
        /** @brief Endpoints require a separately qualified RPC adapter and admission. */
        error create_topic(communication::topic_plan const&,communication::type_plan const&,std::shared_ptr<communication::native_node> const&,
                           communication::handle,std::shared_ptr<communication::receive_queue> const&,std::shared_ptr<communication::native_topic>&)noexcept override
        {return error::unsupported;}
        /** @brief Queue an explicitly admitted topic open on the exact actual native node.
         * @details The factory independently matches the complete carrier's
         * node/schema capabilities, profile, name, type and direction to its
         * sealed manifest and creation owner. The ledger charges once. No
         * factory lock is held across authority admission, and no bytes are sent
         * until pump_topics obtains fresh complete authority. The reserved host
         * endpoint remains unusable until typed adoption and publication.
         */
        error open_topic_request(communication::node_lease const& node,communication_creation::identity const& origin,
                                 wire::bytes canonical,communication_creation::view& output)noexcept
        {
            if(!same_process()||!configured.load()||stopping.load()||!node.live())return error::denied;
            wire::bytes semantic;communication_open_contract::creation suffix;communication_contract::request decoded;
            if(!communication_open_contract::split_creation(canonical,semantic,suffix)||suffix.key!=origin.key||suffix.deadline!=origin.deadline||
                !communication_contract::decode(communication_contract::operation::topic_open,semantic,decoded))return error::invalid;
            participant* selected{};std::size_t selected_index{};
            {
                std::lock_guard lock{mutex};if(stopping.load()||!link_active)return error::cancelled;
                for(auto& peer:peers)
                {
                    auto parent=peer->node.lock();if(!parent||parent.get()!=node.get())continue;
                    if(!peer->topics_enabled||node.authority()!=peer->node_plan.authority||node.context()!=peer->attempt.context||
                        origin.count!=4||origin.authority.environment!=peer->node_plan.authority.environment||
                        origin.origins[1].authority!=node.authority()||origin.origins[1].resource!=node.identity()||
                        origin.origins[2].authority!=peer->context_plan.authority||origin.origins[2].resource!=node.context())return error::denied;
                    if(decoded.endpoint.node!=node.identity()||decoded.endpoint.node_capability!=origin.origins[1].capability||
                        decoded.endpoint.schema_count!=1||decoded.endpoint.schema_capabilities[0]!=origin.origins[3].capability)return error::denied;
                    bool matched{};
                    for(std::size_t i{};i<peer->topic_owners.size();++i)if(peer->topic_owners[i]==origin.authority)
                    {
                        auto const& policy=std::get<communication::topic_plan>(peer->definition.definitions[i].policy);
                        auto type=configuration::find(peer->definition,communication_configuration::family::type,policy.type);
                        if(!type||!communication_contract::matches(decoded.endpoint.profile,policy.selected)||decoded.endpoint.name!=policy.resolved_name||
                            decoded.endpoint.type!=std::get<communication::type_plan>(type->policy).type_identity||
                            decoded.endpoint.role!=(policy.direction==communication::role::publisher?"publisher":"subscription"))return error::denied;
                        matched=true;selected_index=i;break;
                    }
                    if(!matched)return error::denied;
                    selected=peer.get();break;
                }
            }
            return selected?enqueue_endpoint(selected->topic_creation,selected->topic_submission,selected->topic_accounts[selected_index],
                selected->topics[selected_index],origin,canonical,output):error::denied;
        }
        /** @brief Construct a typed facade only from this factory's verified creation receipt.
         * @details Schema, topic, direction, capacities and actual node must
         * match the sealed manifest. The native endpoint is read from the owned
         * ledger, never supplied by the guest or guessed from a request. Factory
         * retention survives failed host publication and prevents forgotten
         * native resources. The explicit aggregate byte reservation is charged
         * before exposing the facade; retries reuse the same host resource.
         */
        error create_verified_topic(handle request,communication::topic_plan const& policy,communication::type_plan const& schema,
                                    communication::node_lease node,handle host_resource,communication_jobs::admit revalidate,
                                    communication_jobs::charge charge,void* context,std::shared_ptr<topic_endpoint>& output,
                                    std::shared_ptr<void> authority_owner={})noexcept
        {
            if(authority_owner&&authority_owner.get()!=context)return error::invalid;
            if(!same_process()||!configured.load()||stopping.load()||!node.live()||!host_resource||!revalidate||!charge)return error::denied;
            std::lock_guard lock{mutex};if(stopping.load()||!link_active)return error::cancelled;
            try
            {
                for(auto& peer:peers)
                {
                    auto actual=peer->node.lock();if(!actual||actual.get()!=node.get())continue;
                    if(!peer->topics_enabled||node.authority()!=peer->node_plan.authority||node.context()!=peer->attempt.context)return error::denied;
                    communication_creation::work work;handle native{};
                    auto status=peer->topic_creation.provider_result(request,work,native);if(status!=error::ok)return status;
                    if(!work.binding||work.binding->kind!=communication_open_contract::endpoint_kind::topic||work.original.authority!=policy.authority||
                        work.binding->selected!=policy.selected||work.original.count!=4||work.original.origins[1].resource!=node.identity()||
                        work.original.origins[1].authority!=node.authority()||work.original.origins[2].resource!=node.context()||
                        work.original.origins[2].authority!=peer->context_plan.authority||work.original.origins[3].authority!=schema.authority)return error::denied;
                    auto index=static_cast<std::size_t>(work.binding->worker_binding-1);
                    auto expected=std::get<communication::topic_plan>(peer->definition.definitions[index].policy);expected.authority=policy.authority;
                    if(!same(expected,policy))return error::denied;
                    auto definition=configuration::find(peer->definition,communication_configuration::family::type,policy.type);
                    if(!definition)return error::denied;
                    auto const& approved=std::get<communication::type_plan>(definition->policy);
                    if(schema.selected!=approved.selected||schema.type_identity!=approved.type_identity||schema.encoding!=approved.encoding||
                        schema.descriptor!=approved.descriptor||schema.validate!=approved.validate||schema.revoked)return error::denied;
                    if(peer->topics[index])
                    {
                        if(!peer->topics[index]->matches(policy,host_resource,actual.get()))return error::denied;
                        output=peer->topics[index];return error::ok;
                    }
                    auto request_bytes=peer->capacity.request_bytes,reply_bytes=peer->capacity.response_bytes;
                    auto mutation=2*request_bytes+reply_bytes;
                    auto needed=mutation+2*(request_bytes+reply_bytes)+(policy.direction==communication::role::publisher?mutation:
                        request_bytes+3*reply_bytes+policy.max_message_bytes)+endpoint_closure::reserved_bytes;
                    auto& account=peer->topic_accounts[index];
                    if(account.bytes!=needed||account.request!=request||!account.preparation)return error::again;
                    auto endpoint=::uwvm2::utils::container::make_shared<topic_endpoint>();
                    communication_jobs::plan queue{policy.authority,communication::operation_family::topic,host_resource,work.binding->worker_binding,
                        work.binding->provider_generation,1,request_bytes,reply_bytes,policy.max_deadline_ns,needed};
                    status=endpoint->configure(policy,schema,std::move(node),queue,native,{request_bytes,reply_bytes,needed},revalidate,charge,context,&account.preparation,std::move(authority_owner));
                    if(status!=error::ok)return status;
                    if(stopping.load())return error::cancelled;
                    peer->topics[index]=endpoint;account.resource=host_resource;output=std::move(endpoint);return error::ok;
                }
                return error::denied;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /** @brief Publish the exact verified open under the caller's current origin group.
         * @details This is trusted bookkeeping after successful registry adoption.
         * The host resource must be the retained facade's exact identity. Lost
         * guest output can repeat publication without another native open.
         */
        error publish_topic(caller who,handle request,group_lease const& current,handle resource,communication_open_contract::published& output)noexcept
        {
            if(!same_process()||!configured.load()||stopping.load())return error::denied;
            for(auto& peer:peers)if(peer->topics_enabled)
            {
                communication_creation::work work;handle native{};
                if(peer->topic_creation.provider_result(request,work,native)!=error::ok)continue;
                std::shared_ptr<topic_endpoint> endpoint;
                {std::lock_guard lock{mutex};endpoint=peer->topics[work.binding->worker_binding-1];}
                if(!endpoint||endpoint->resource()!=resource)return error::denied;
                return peer->topic_creation.publish(who,request,current,resource,output);
            }
            return error::bad_handle;
        }
        /** @brief Inspect an exact creation receipt without native I/O or authority inference. */
        error inspect_topic_open(caller who,handle request,communication_creation::view& output)noexcept
        {
            if(!same_process()||!configured.load())return error::denied;
            for(auto& peer:peers)if(peer->topics_enabled)
            {auto status=peer->topic_creation.inspect(who,request,output);if(status==error::ok)return status;}
            return error::denied;
        }
        /** @brief Advance creation and typed topic RPC work on existing bounded transports.
         * @details Called outside authority and registry locks. The supervising
         * dispatcher separately reconciles endpoint completions with its retained
         * host receipts before releasing them. No hidden thread or wait is added.
         */
        error pump_topics()noexcept
        {
            if(!same_process()||!configured.load()||stopping.load())return error::cancelled;
            bool progress{};
            for(auto& peer:peers)if(peer->topics_enabled)
            {
                {std::lock_guard lock{mutex};if(stopping.load()||!link_active)return error::cancelled;}
                auto status=peer->topic_creation.pump(peer->rpc);
                if(status==error::ok)progress=true;else if(status!=error::again&&status!=error::busy)return status;
                for(std::size_t i{};i<peer->topics.size();++i)
                {
                    std::shared_ptr<topic_endpoint> endpoint;
                    {std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return progress?error::ok:error::again;endpoint=peer->topics[i];}
                    if(!endpoint)continue;
                    status=endpoint->pump(peer->rpc);
                    if(status==error::ok)progress=true;else if(status!=error::again&&status!=error::busy)return status;
                }
            }
            return progress?error::ok:error::again;
        }
        /** @brief Enqueue exact service creation without waiting for native work.
         * @details Both schema capabilities, node/context origins, role, service
         * type, immutable profile and retry trailer are checked independently of
         * the caller's audit carrier. No topic/action authority can substitute.
         */
        error open_service_request(communication::node_lease const& node,communication_creation::identity const& origin,
                                   wire::bytes canonical,communication_creation::view& output)noexcept
        {
            if(!same_process()||!configured.load()||stopping.load()||!node.live())return error::denied;
            wire::bytes semantic;communication_open_contract::creation suffix;communication_contract::request decoded;
            if(!communication_open_contract::split_creation(canonical,semantic,suffix)||suffix.key!=origin.key||suffix.deadline!=origin.deadline||
                !communication_contract::decode(communication_contract::operation::service_open,semantic,decoded))return error::invalid;
            participant* selected{};std::size_t selected_index{};
            {
                std::lock_guard lock{mutex};if(stopping.load()||!link_active)return error::cancelled;
                for(auto& peer:peers)
                {
                    auto parent=peer->node.lock();if(!parent||parent.get()!=node.get())continue;
                    if(!peer->services_enabled||node.authority()!=peer->node_plan.authority||node.context()!=peer->attempt.context||
                        origin.count!=5||origin.authority.environment!=peer->node_plan.authority.environment||
                        origin.origins[1].authority!=node.authority()||origin.origins[1].resource!=node.identity()||
                        origin.origins[2].authority!=peer->context_plan.authority||origin.origins[2].resource!=node.context())return error::denied;
                    if(decoded.endpoint.node!=node.identity()||decoded.endpoint.node_capability!=origin.origins[1].capability||decoded.endpoint.schema_count!=2||
                        decoded.endpoint.schema_capabilities[0]!=origin.origins[3].capability||decoded.endpoint.schema_capabilities[1]!=origin.origins[4].capability)return error::denied;
                    bool matched{};
                    for(std::size_t i{};i<peer->service_owners.size();++i)if(peer->service_owners[i]==origin.authority)
                    {
                        auto const& policy=std::get<communication::service_plan>(peer->definition.definitions[i].policy);
                        if(!communication_contract::matches(decoded.endpoint.profile,policy.selected)||decoded.endpoint.name!=policy.resolved_name||
                            decoded.endpoint.type!=policy.service_type||decoded.endpoint.role!=(policy.direction==communication::service_role::client?"client":"server"))return error::denied;
                        matched=true;selected_index=i;break;
                    }
                    if(!matched)return error::denied;
                    selected=peer.get();break;
                }
            }
            return selected?enqueue_endpoint(selected->service_creation,selected->service_submission,selected->service_accounts[selected_index],
                selected->services[selected_index],origin,canonical,output):error::denied;
        }
        /** @brief Adopt a service only from the owning ledger's successful native receipt.
         * @details The sealed request/response schemas and all message/correlation
         * capacities must match. A recoverable allocation failure retains native
         * cleanup debt in the creation ledger. Exact retries reuse the retained
         * facade and host resource; no worker identity is accepted from a guest.
         */
        error create_verified_service(handle request,communication::service_plan const& policy,communication::type_plan const& request_schema,
                                      communication::type_plan const& response_schema,communication::node_lease node,handle host_resource,
                                      communication_jobs::admit revalidate,communication_jobs::charge charge,void* context,std::shared_ptr<service_endpoint>& output,
                                      std::shared_ptr<void> authority_owner={})noexcept
        {
            if(authority_owner&&authority_owner.get()!=context)return error::invalid;
            if(!same_process()||!configured.load()||stopping.load()||!node.live()||!host_resource||!revalidate||!charge)return error::denied;
            std::lock_guard lock{mutex};if(stopping.load()||!link_active)return error::cancelled;
            try
            {
                for(auto& peer:peers)
                {
                    auto actual=peer->node.lock();if(!actual||actual.get()!=node.get())continue;
                    if(!peer->services_enabled||node.authority()!=peer->node_plan.authority||node.context()!=peer->attempt.context)return error::denied;
                    communication_creation::work work;handle native{};auto status=peer->service_creation.provider_result(request,work,native);if(status!=error::ok)return status;
                    if(!work.binding||work.binding->kind!=communication_open_contract::endpoint_kind::service||work.original.authority!=policy.authority||
                        work.binding->selected!=policy.selected||work.original.count!=5||work.original.origins[1].resource!=node.identity()||
                        work.original.origins[1].authority!=node.authority()||work.original.origins[2].resource!=node.context()||
                        work.original.origins[2].authority!=peer->context_plan.authority||work.original.origins[3].authority!=request_schema.authority||
                        work.original.origins[4].authority!=response_schema.authority)return error::denied;
                    auto index=static_cast<std::size_t>(work.binding->worker_binding-1);
                    auto expected=std::get<communication::service_plan>(peer->definition.definitions[index].policy);expected.authority=policy.authority;
                    if(!same(expected,policy))return error::denied;
                    auto matches_schema=[&](communication::type_plan const& schema,communication::profile const& selected)
                    {
                        auto definition=configuration::find(peer->definition,communication_configuration::family::type,selected);if(!definition)return false;
                        auto const& approved=std::get<communication::type_plan>(definition->policy);
                        return schema.selected==approved.selected&&schema.type_identity==approved.type_identity&&schema.encoding==approved.encoding&&
                            schema.descriptor==approved.descriptor&&schema.validate==approved.validate&&!schema.revoked;
                    };
                    if(!matches_schema(request_schema,policy.request_type)||!matches_schema(response_schema,policy.response_type))return error::denied;
                    if(peer->services[index])
                    {
                        if(!peer->services[index]->matches(policy,host_resource,actual.get()))return error::denied;
                        output=peer->services[index];return error::ok;
                    }
                    auto request_bytes=peer->capacity.request_bytes,reply_bytes=peer->capacity.response_bytes;
                    auto needed=service_endpoint::reservation_bytes(policy,request_bytes,reply_bytes);
                    auto& account=peer->service_accounts[index];
                    if(!needed||account.bytes!=needed||account.request!=request||!account.preparation)return error::again;
                    auto endpoint=::uwvm2::utils::container::make_shared<service_endpoint>();
                    communication_jobs::plan queue{policy.authority,communication::operation_family::service,host_resource,work.binding->worker_binding,
                        work.binding->provider_generation,1,request_bytes,reply_bytes,policy.max_deadline_ns,needed};
                    status=endpoint->configure(policy,request_schema,response_schema,std::move(node),queue,native,{request_bytes,reply_bytes,needed},revalidate,charge,context,&account.preparation,std::move(authority_owner));
                    if(status!=error::ok)return status;
                    if(stopping.load())return error::cancelled;
                    peer->services[index]=endpoint;account.resource=host_resource;output=std::move(endpoint);return error::ok;
                }
                return error::denied;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /// @brief Publish an actually adopted service under fresh complete origin authority.
        error publish_service(caller who,handle request,group_lease const& current,handle resource,communication_open_contract::published& output)noexcept
        {
            if(!same_process()||!configured.load()||stopping.load())return error::denied;
            for(auto& peer:peers)if(peer->services_enabled)
            {
                communication_creation::work work;handle native{};if(peer->service_creation.provider_result(request,work,native)!=error::ok)continue;
                std::shared_ptr<service_endpoint> endpoint;
                {std::lock_guard lock{mutex};endpoint=peer->services[work.binding->worker_binding-1];}
                if(!endpoint||endpoint->resource()!=resource)return error::denied;
                return peer->service_creation.publish(who,request,current,resource,output);
            }
            return error::denied;
        }
        error inspect_service_open(caller who,handle request,communication_creation::view& output)noexcept
        {
            if(!same_process()||!configured.load())return error::denied;
            for(auto& peer:peers)if(peer->services_enabled)
            {auto status=peer->service_creation.inspect(who,request,output);if(status==error::ok)return status;}
            return error::denied;
        }
        /** @brief Advance service creation and all isolated service queues without new threads.
         * @details The owning dispatcher settles each cached completion into the
         * registry before releasing it. The same bounded transport is shared
         * with topic and graph work; no hidden worker or scheduler is created.
         */
        error pump_services()noexcept
        {
            if(!same_process()||!configured.load()||stopping.load())return error::cancelled;
            bool progress{};
            for(auto& peer:peers)if(peer->services_enabled)
            {
                {std::lock_guard lock{mutex};if(stopping.load()||!link_active)return error::cancelled;}
                auto status=peer->service_creation.pump(peer->rpc);
                if(status==error::ok)progress=true;else if(status!=error::again&&status!=error::busy)return status;
                for(std::size_t i{};i<peer->services.size();++i)
                {
                    std::shared_ptr<service_endpoint> endpoint;
                    {std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return progress?error::ok:error::again;endpoint=peer->services[i];}
                    if(!endpoint)continue;
                    status=endpoint->pump(peer->rpc);
                    if(status==error::ok)progress=true;else if(status!=error::again&&status!=error::busy)return status;
                }
            }
            return progress?error::ok:error::again;
        }
        /** @brief Admit an asynchronous action open with the complete sealed three-schema cohort.
         * @details The creation key/deadline trailer, actual node, role, action
         * type and immutable profile must agree. The binding's finite capacity
         * is reserved before enqueue admission. No factory lock spans authority
         * callbacks and no guest handle is treated as a native action identity.
         */
        error open_action_request(communication::node_lease const& node,communication_creation::identity const& origin,
                                  wire::bytes canonical,communication_creation::view& output)noexcept
        {
            if(!same_process()||!configured.load()||stopping.load()||!node.live())return error::denied;
            wire::bytes semantic;communication_open_contract::creation suffix;action_contract::request decoded;
            if(!communication_open_contract::split_creation(canonical,semantic,suffix)||suffix.key!=origin.key||suffix.deadline!=origin.deadline||
                !action_contract::decode(action_contract::operation::open,semantic,decoded))return error::invalid;
            participant* selected{};std::size_t index{};
            {
                std::lock_guard lock{mutex};if(stopping.load()||!link_active)return error::cancelled;
                for(auto& peer:peers)
                {
                    auto parent=peer->node.lock();if(!parent||parent.get()!=node.get())continue;
                    if(!peer->actions_enabled||node.authority()!=peer->node_plan.authority||node.context()!=peer->attempt.context||origin.count!=6||
                        origin.authority.environment!=peer->node_plan.authority.environment||origin.origins[1].authority!=node.authority()||
                        origin.origins[1].resource!=node.identity()||origin.origins[2].authority!=peer->context_plan.authority||origin.origins[2].resource!=node.context())return error::denied;
                    if(decoded.endpoint.node!=node.identity()||decoded.endpoint.node_capability!=origin.origins[1].capability||decoded.endpoint.schema_count!=3)return error::denied;
                    for(std::size_t i{};i<3;++i)if(decoded.endpoint.schema_capabilities[i]!=origin.origins[i+3].capability)return error::denied;
                    bool matched{};
                    for(std::size_t i{};i<peer->action_owners.size();++i)if(peer->action_owners[i]==origin.authority)
                    {
                        auto const& policy=std::get<communication_action_dispatch::plan>(peer->definition.definitions[i].policy).policy;
                        if(!communication_contract::matches(decoded.endpoint.profile,policy.selected)||decoded.endpoint.name!=policy.resolved_name||
                            decoded.endpoint.type!=policy.action_type||decoded.endpoint.role!=(policy.direction==communication::actions::role::client?"client":"server"))return error::denied;
                        index=i;matched=true;break;
                    }
                    if(!matched)return error::denied;
                    selected=peer.get();break;
                }
            }
            return selected?enqueue_endpoint(selected->action_creation,selected->action_submission,selected->action_accounts[index],selected->actions[index],origin,canonical,output):error::denied;
        }
        /** @brief Construct the host action ledger and typed facade from verified native creation.
         * @details All plan fields and schema descriptors/codecs must match the
         * sealed configuration. Both objects are retained across failed guest
         * publication. The preparation capacity moves only after both objects
         * and their weak ingress connection have been initialized successfully.
         */
        error create_verified_action(handle request,communication::actions::plan const& policy,communication::node_lease node,handle resource,
            communication_jobs::admit revalidate,communication_jobs::charge charge,void* context,std::shared_ptr<action_endpoint>& provider,
            std::shared_ptr<communication::actions::endpoint>& output,std::shared_ptr<void> authority_owner={})noexcept
        {
            if(!same_process()||!configured.load()||stopping.load()||!node.live()||!resource||!revalidate||!charge)return error::denied;
            std::lock_guard lock{mutex};if(stopping.load()||!link_active)return error::cancelled;
            try
            {
                for(auto& peer:peers)
                {
                    auto parent=peer->node.lock();if(!parent||parent.get()!=node.get())continue;
                    if(!peer->actions_enabled||node.authority()!=peer->node_plan.authority||node.context()!=peer->attempt.context)return error::denied;
                    communication_creation::work work;handle native{};auto status=peer->action_creation.provider_result(request,work,native);if(status!=error::ok)return status;
                    if(!work.binding||work.binding->kind!=communication_open_contract::endpoint_kind::action||work.original.authority!=policy.authority||
                        work.binding->selected!=policy.selected||work.original.count!=6||work.original.origins[1].authority!=node.authority()||
                        work.original.origins[1].resource!=node.identity()||work.original.origins[2].authority!=peer->context_plan.authority||
                        work.original.origins[2].resource!=node.context())return error::denied;
                    auto index=static_cast<std::size_t>(work.binding->worker_binding-1);
                    auto expected=std::get<communication_action_dispatch::plan>(peer->definition.definitions[index].policy).policy;expected.authority=policy.authority;
                    std::array actual_schemas{&policy.goal,&policy.feedback,&policy.result};std::array expected_schemas{&expected.goal,&expected.feedback,&expected.result};
                    for(std::size_t i{};i<3;++i)
                    {
                        if(actual_schemas[i]->authority!=work.original.origins[i+3].authority||actual_schemas[i]->revoked)return error::denied;
                        expected_schemas[i]->authority=actual_schemas[i]->authority;
                    }
                    if(!same(expected,policy))return error::denied;
                    if(peer->actions[index])
                    {
                        auto const& core=peer->action_cores[index];
                        if(peer->actions[index]->resource()!=resource||!core||!core->owns(policy.authority,resource))return error::denied;
                        provider=peer->actions[index];output=core;return error::ok;
                    }
                    auto& account=peer->action_accounts[index];
                    if(account.request!=request||!account.preparation)return error::again;
                    auto facade=::uwvm2::utils::container::make_shared<action_endpoint>();
                    auto core=::uwvm2::utils::container::make_shared<communication::actions::endpoint>();
                    communication_jobs::plan queue{policy.authority,communication::operation_family::action,resource,work.binding->worker_binding,
                        work.binding->provider_generation,1,peer->capacity.request_bytes,peer->capacity.response_bytes,policy.max_deadline_ns,account.bytes};
                    status=facade->configure(policy,std::move(node),queue,native,revalidate,charge,context,nullptr,std::move(authority_owner));if(status!=error::ok)return status;
                    status=core->configure(policy,resource,facade);if(status!=error::ok)return status;
                    status=facade->attach(core);if(status!=error::ok)return status;
                    if(stopping.load())return error::cancelled;
                    status=facade->retain_capacity(account.preparation);if(status!=error::ok)return status;
                    peer->actions[index]=facade;peer->action_cores[index]=core;account.resource=resource;provider=std::move(facade);output=std::move(core);return error::ok;
                }
                return error::denied;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /// @brief Publish only the exact initialized host action under fresh complete origin authority.
        error publish_action(caller who,handle request,group_lease const& current,handle resource,communication_open_contract::published& output)noexcept
        {
            if(!same_process()||!configured.load()||stopping.load())return error::denied;
            for(auto& peer:peers)if(peer->actions_enabled)
            {
                communication_creation::work work;handle native{};if(peer->action_creation.provider_result(request,work,native)!=error::ok)continue;
                std::shared_ptr<action_endpoint> endpoint;std::shared_ptr<communication::actions::endpoint> core;
                {std::lock_guard lock{mutex};endpoint=peer->actions[work.binding->worker_binding-1];core=peer->action_cores[work.binding->worker_binding-1];}
                if(!endpoint||endpoint->resource()!=resource||!core||!core->owns(work.original.authority,resource))return error::denied;
                return peer->action_creation.publish(who,request,current,resource,output);
            }
            return error::bad_handle;
        }
        error inspect_action_open(caller who,handle request,communication_creation::view& output)noexcept
        {
            if(!same_process()||!configured.load())return error::denied;
            for(auto& peer:peers)if(peer->actions_enabled)
            {auto status=peer->action_creation.inspect(who,request,output);if(status==error::ok)return status;}
            return error::bad_handle;
        }
        /** @brief Atomically inspect a caller-owned creation and its exact original cohort.
         * @details This cached host-only observation neither pumps the worker
         * nor authorizes a public request operation. The public dispatcher must
         * match the returned owner/family against its admitted immutable binding.
         */
        error inspect_open_origin(caller who,handle request,communication_creation::view& output,communication_creation::identity& origin)noexcept
        {
            if(!same_process()||!configured.load())return error::denied;
            for(auto& peer:peers)
            {
                if(peer->topics_enabled&&peer->topic_creation.inspect_origin(who,request,output,origin)==error::ok)return error::ok;
                if(peer->services_enabled&&peer->service_creation.inspect_origin(who,request,output,origin)==error::ok)return error::ok;
                if(peer->actions_enabled&&peer->action_creation.inspect_origin(who,request,output,origin)==error::ok)return error::ok;
            }
            return error::bad_handle;
        }
        /// @brief Pump action RPCs on the existing shared transport; settle host receipts separately.
        error pump_actions()noexcept
        {
            if(!same_process()||!configured.load()||stopping.load())return error::cancelled;
            bool progress{};
            for(auto& peer:peers)if(peer->actions_enabled)
            {
                {std::lock_guard lock{mutex};if(stopping.load()||!link_active)return error::cancelled;}
                auto status=peer->action_creation.pump(peer->rpc);
                if(status==error::ok)progress=true;else if(status!=error::again&&status!=error::busy)return status;
                for(std::size_t i{};i<peer->actions.size();++i)
                {
                    std::shared_ptr<action_endpoint> endpoint;
                    {std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return progress?error::ok:error::again;endpoint=peer->actions[i];}
                    if(!endpoint)continue;
                    status=endpoint->pump(peer->rpc);if(status==error::ok)progress=true;else if(status!=error::again&&status!=error::busy)return status;
                }
            }
            return progress?error::ok:error::again;
        }
        /** @brief Reconcile a verified close with its exact creation and host identities.
         * @details Trusted supervisor bookkeeping only, with no I/O or new
         * authority. The facade itself must hold a successful exact native-close
         * receipt. A guest request/resource number, timeout or signal cannot
         * substitute. Dropping factory retention does not release actual buffer
         * capacity while the registry or another observer still retains it.
         */
        error reconcile_close(handle request,handle resource)noexcept
        {
            if(!same_process()||!configured.load()||!request||!resource)return error::denied;
            for(auto& peer:peers)
            {
                if(peer->topics_enabled)
                {auto status=reconcile_endpoint(peer->topic_creation,peer->topic_accounts,peer->topics,request,resource);if(status!=error::bad_handle)return status;}
                if(peer->services_enabled)
                {auto status=reconcile_endpoint(peer->service_creation,peer->service_accounts,peer->services,request,resource);if(status!=error::bad_handle)return status;}
                if(peer->actions_enabled)
                {
                    auto status=reconcile_endpoint(peer->action_creation,peer->action_accounts,peer->actions,request,resource);
                    if(status!=error::bad_handle)
                    {
                        std::shared_ptr<communication::actions::endpoint> dropped;
                        if(status==error::ok){std::lock_guard lock{mutex};for(std::size_t i{};i<peer->action_accounts.size();++i)
                            if(peer->action_accounts[i].request==request&&peer->action_accounts[i].resource==resource){dropped=std::move(peer->action_cores[i]);break;}}
                        return status;
                    }
                }
            }
            return error::bad_handle;
        }
        /** @brief Cancel the exact owned creation receipt without erasing cleanup debt.
         * @details The public dispatcher must independently admit
         * communication.open.cancel on the original binding. This bookkeeping
         * method never uses a cancelled open as permission to close an already
         * published endpoint. That endpoint needs its own resource.close flow.
         */
        error cancel_open(caller who,handle request,communication_creation::view& result)noexcept
        {
            if(!same_process()||!configured.load())return error::denied;
            for(auto& peer:peers)
            {
                if(peer->topics_enabled&&peer->topic_creation.inspect(who,request,result)==error::ok)return peer->topic_creation.cancel(who,request,result);
                if(peer->services_enabled&&peer->service_creation.inspect(who,request,result)==error::ok)return peer->service_creation.cancel(who,request,result);
                if(peer->actions_enabled&&peer->action_creation.inspect(who,request,result)==error::ok)return peer->action_creation.cancel(who,request,result);
            }
            return error::bad_handle;
        }
        /** @brief Release a known terminal creation under caller-bound ownership.
         * @details The public dispatcher separately admits request.release.
         * Active endpoints, unknown outcomes and unconfirmed native debt remain
         * busy. Unsent cancelled preparation returns its capacity; an adopted
         * facade returns capacity only when its actual queues are destroyed.
         */
        error release_open(caller who,handle request)noexcept
        {
            if(!same_process()||!configured.load())return error::denied;
            communication_creation::view current;
            for(auto& peer:peers)
            {
                if(peer->topics_enabled&&peer->topic_creation.inspect(who,request,current)==error::ok)
                    return release_endpoint(peer->topic_creation,peer->topic_submission,peer->topic_accounts,peer->topics,who,request);
                if(peer->services_enabled&&peer->service_creation.inspect(who,request,current)==error::ok)
                    return release_endpoint(peer->service_creation,peer->service_submission,peer->service_accounts,peer->services,who,request);
                if(peer->actions_enabled&&peer->action_creation.inspect(who,request,current)==error::ok)
                    return release_endpoint(peer->action_creation,peer->action_submission,peer->action_accounts,peer->actions,who,request);
            }
            return error::bad_handle;
        }
        /** @brief Attach an exact graph plan to the already admitted sealed native node.
         * @details This method is for the graph dispatcher's trusted factory,
         * never guest bytes. The complete filter and capacities must match the
         * sealed manifest; owners/context handles are supplied by the host.
         * The explicit aggregate graph reservation is checked before allocation.
         * No query is sent and no observation is charged here. The factory owns
         * each query through stop/drain, including failed caller publication or
         * unknown outcomes. Retrying the same unpublished plan reuses it safely.
         */
        error create_graph(communication::graph::plan const& plan,communication::node_lease node,
                           communication_jobs::admit revalidate,communication_jobs::charge charge,void* context,
                           std::shared_ptr<communication::graph::source>& output,std::shared_ptr<void> authority_owner={})noexcept
        {
            if(!same_process()||!configured.load()||stopping.load()||!node.live()||!revalidate||!charge)return error::denied;
            std::lock_guard lock{mutex};if(stopping.load()||!link_active)return error::cancelled;
            try
            {
                for(auto& peer:peers)
                {
                    auto parent=peer->node.lock();if(!parent||parent.get()!=node.get())continue;
                    if(node.authority()!=peer->node_plan.authority||node.context()!=peer->attempt.context||
                        plan.authority.environment!=peer->node_plan.authority.environment||plan.context!=node.context())return error::denied;
                    auto const& definitions=peer->definition.definitions;
                    for(std::size_t i{};i<definitions.size();++i)
                    {
                        auto const& definition=definitions[i];
                        if(definition.kind!=communication_configuration::family::graph||configuration::profile_of(definition)!=plan.selected)continue;
                        auto expected=std::get<communication_graph_dispatch::plan>(definition.policy).policy;
                        expected.authority=plan.authority;expected.context=plan.context;if(expected!=plan)return error::denied;
                        if(peer->graphs[i])
                        {if(!peer->graphs[i]->matches(plan,parent.get()))return error::denied;output=peer->graphs[i];return error::ok;}
                        auto needed=2*(peer->capacity.request_bytes+peer->capacity.response_bytes);
                        if(peer->graph_reserved>peer->graph_limit||needed>peer->graph_limit-peer->graph_reserved)return error::too_large;
                        auto source=::uwvm2::utils::container::make_shared<graph_source>();
                        communication_jobs::plan queue{plan.authority,communication::operation_family::graph,plan.authority.grant,i+1,
                            peer->definition.network.generation,1,peer->capacity.request_bytes,peer->capacity.response_bytes,peer->capacity.deadline_ns,needed};
                        auto status=source->configure(plan,std::move(node),queue,revalidate,charge,context,std::move(authority_owner));if(status!=error::ok)return status;
                        if(stopping.load())return error::cancelled;
                        peer->graphs[i]=source;peer->graph_reserved+=needed;output=std::move(source);return error::ok;
                    }
                    return error::denied;
                }
                return error::denied;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /** @brief Advance finite graph RPC work without holding the factory lock.
         * @details Called by a trusted supervisor outside authority/registry
         * locks. Each queue independently revalidates its actual call before a
         * nonblocking send. A peer has one shared wire sequence; concurrent pumps
         * serialize through that transport instead of creating another channel.
         * This method creates no thread and performs no blocking reply wait.
         */
        error pump_graphs()noexcept
        {
            if(!same_process()||!configured.load()||stopping.load())return error::cancelled;
            // Relay failure applies even when no graph capability exists. The
            // relay has already fenced both shared RPC sessions; now retire
            // the pair's ledgers through the ordinary supervisor path.
            if(link.inspect().fenced)return failed(error::cancelled);
            bool progressed{};
            for(auto& peer:peers)for(std::size_t i{};i<peer->graphs.size();++i)
            {
                std::shared_ptr<graph_source> source;
                {std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return progressed?error::ok:error::again;
                    if(stopping.load()||!link_active)return error::cancelled;
                    source=peer->graphs[i];}
                if(!source)continue;
                auto status=source->pump(peer->rpc);if(status==error::ok)progressed=true;
                else if(status!=error::again&&status!=error::busy)return status;
            }
            return progressed?error::ok:error::again;
        }
        void retire_owner(communication::owner revoked)noexcept override
        {if(!same_process()||!configured.load())return;for(auto const& peer:peers)if(peer->node_plan.authority==revoked||peer->context_plan.authority==revoked){request_stop();return;}}
        void capability_closed(communication::handle cap)noexcept override
        {if(!same_process()||!configured.load()||!cap)return;for(auto const& peer:peers)if(peer->node_capability.load()==cap||peer->context_capability.load()==cap){request_stop();return;}}
        void context_closed(communication::owner selected,communication::handle)noexcept override
        {if(!same_process()||!configured.load())return;for(auto const& peer:peers)if(peer->context_plan.authority==selected){request_stop();return;}}
        /** @brief Atomically fence the pair without signalling either external peer. */
        void request_stop()noexcept override
        {
            if(!same_process())return;
            stopping.store(true,std::memory_order_release);link.request_stop();
            if(configured.load(std::memory_order_acquire))for(auto const& peer:peers)
            {peer->topic_creation.request_stop();peer->service_creation.request_stop();peer->action_creation.request_stop();peer->rpc.request_stop();peer->stage.request_stop();}
        }
        /** @brief Stop/drain outside authority locks; unresolved cleanup stays reserved. */
        void retire()noexcept override
        {
            if(!same_process())return;
            request_stop();std::lock_guard retirement{retirement_mutex};
            // Publication checks stopping while holding mutex; after this
            // barrier the owned graph table is immutable for retirement.
            {std::lock_guard lock{mutex};}
            link.retire();if(!configured.load()){drained=true;confirmed=true;return;}
            for(auto& peer:peers)
            {
                peer->topic_creation.retire();
                for(auto const& endpoint:peer->topics)if(endpoint)endpoint->drain();
                peer->service_creation.retire();
                for(auto const& endpoint:peer->services)if(endpoint)endpoint->drain();
                peer->action_creation.retire();
                for(auto const& endpoint:peer->actions)if(endpoint)endpoint->retire();
                for(auto const& source:peer->graphs)if(source)source->retire();
                peer->rpc.retire();
            }
            drained.store(true,std::memory_order_release);
            auto deadline=communication::now()+retirement_ns;bool complete{};
            do
            {
                complete=true;bool pending{};
                for(auto& peer:peers){auto status=peer->stage.cleanup_progress();complete&=status==error::ok;pending|=status==error::again;}
                if(complete||!pending||communication::now()>=deadline)break;
                ::poll(nullptr,0,1);
            }while(true);
            confirmed.store(complete,std::memory_order_release);
        }
        bool cleanup_confirmed()const noexcept override{return same_process()&&confirmed.load(std::memory_order_acquire);}
        provider_snapshot inspect()const noexcept
        {
            if(!same_process())return {};
            std::lock_guard lock{mutex};provider_snapshot result{configured.load(),stopping.load(),drained.load(),confirmed.load(),{},link.inspect()};
            if(result.configured)for(std::size_t i{};i<2;++i)result.stages[i]=peers[i]->stage.inspect();
            return result;
        }
    };
#endif
}
