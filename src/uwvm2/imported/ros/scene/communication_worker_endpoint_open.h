/** @file
 * @brief Owned asynchronous Topic/Service creation and settlement coordinator.
 * @details This host-only adapter binds an exact canonical dispatcher plan to
 * an isolated worker factory. Configuration and creation submission perform no
 * native I/O. The deployment supervisor explicitly pumps finite work; there is
 * no private thread, polling loop, implicit process launch or borrowed guest
 * pointer. Creation, publication, ordinary traffic and cleanup each use their
 * own current authority. A creation receipt never serves as an endpoint handle.
 *
 * Callback contexts are retained by both creation queues and endpoint facades.
 * They refer weakly to the dispatcher: expiration denies admission instead of
 * leaving a dangling callback or an ownership cycle. Every owning allocation
 * uses the recoverable fast_io allocator. Absent capacities and grants disable
 * the corresponding operation. Worker retirement, CPU/NUMA placement and the
 * paired process failure domain remain explicit deployment responsibilities.
 */
#pragma once
#include "communication_dispatch.h"
#include "communication_open_dispatch.h"
#include "communication_worker_provider.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
#if defined(__linux__)
    /** @brief Retained, non-owning access to the exact canonical admission path.
     * @details The factory retains this context through creation-queue drain;
     * each facade separately retains it through its own destruction. No cached
     * lease or authority is inherited from creation. An expired dispatcher
     * denies every callback without touching a released object.
     */
    class dispatch_admission
    {
        std::weak_ptr<communication_dispatch> target;
        pid_t process{::getpid()};
        std::shared_ptr<communication_dispatch> lock()const noexcept
        {return process==::getpid()?target.lock():std::shared_ptr<communication_dispatch>{};}
    public:
        explicit dispatch_admission(std::shared_ptr<communication_dispatch> const& value)noexcept:target(value){}
        static communication::error admit_open(void* context,communication_creation::identity const& original,group_lease& held)noexcept
        {
            if(!context)return communication::error::denied;
            auto actual=static_cast<dispatch_admission*>(context)->lock();
            return actual?static_cast<communication::error>(actual->admit_open(original,0,held)):communication::error::denied;
        }
        static communication::error charge_open(void* context,communication_creation::identity const& original,std::uint64_t bytes,group_lease& held)noexcept
        {
            if(!context)return communication::error::denied;
            auto actual=static_cast<dispatch_admission*>(context)->lock();
            return actual?static_cast<communication::error>(actual->admit_open(original,bytes,held,true)):communication::error::denied;
        }
        static communication::error admit(void* context,communication::operation_token const& original,group_lease& held)noexcept
        {
            if(!context)return communication::error::denied;
            auto actual=static_cast<dispatch_admission*>(context)->lock();
            return actual?static_cast<communication::error>(actual->revalidate(original,held)):communication::error::denied;
        }
        static communication::error charge(void* context,communication::operation_token const& original,std::uint64_t bytes,group_lease& held)noexcept
        {
            if(!context)return communication::error::denied;
            auto actual=static_cast<dispatch_admission*>(context)->lock();
            return actual?static_cast<communication::error>(actual->charge_operation(original,bytes,held)):communication::error::denied;
        }
    };

    class endpoint_open_provider final:public communication_open_dispatch::provider
    {
        using error=communication::error;
        using kind=communication_open_contract::endpoint_kind;
        using draft=communication_dispatch::opening;
        std::shared_ptr<communication_dispatch> dispatch;
        std::shared_ptr<paired_factory> factory;
        std::shared_ptr<dispatch_admission> admission;
        ::uwvm2::utils::container::std_string alias;
        communication::owner owner;
        kind family{};
        std::shared_ptr<draft> current;
        std::shared_ptr<topic_endpoint> topic;
        std::shared_ptr<service_endpoint> service;
        mutable std::mutex transaction;
        std::atomic<bool> configured{};
        bool released{};
        pid_t process{::getpid()};
        bool ready()const noexcept{return process==::getpid()&&configured.load(std::memory_order_acquire);}
        error own(caller who,handle request,communication_open_dispatch::observation& result)noexcept
        {
            if(!ready())return error::denied;
            auto status=factory->inspect_open_origin(who,request,result.state,result.origin);
            if(status!=error::ok)return status;
            return result.origin.authority==owner&&result.state.kind==family?error::ok:error::denied;
        }
        static error enqueue(void* context,draft const& value,wire::bytes body,communication_creation::view& result)noexcept
        {
            auto& self=*static_cast<endpoint_open_provider*>(context);
            if(value.kind()!=self.family||value.identity().authority!=self.owner)return error::denied;
            std::shared_ptr<draft> retained;auto status=self.dispatch->retain_open(value.identity(),retained);
            if(status!=scene::error::success)return static_cast<error>(status);
            self.current=std::move(retained);
            return self.family==kind::topic?self.factory->open_topic_request(value.node(),value.identity(),body,result):
                self.factory->open_service_request(value.node(),value.identity(),body,result);
        }
        static error inspect_creation(void* context,draft const& value,communication_creation::view& result)noexcept
        {
            auto& self=*static_cast<endpoint_open_provider*>(context);
            return self.family==kind::topic?self.factory->inspect_topic_open(value.identity().who,value.request(),result):
                self.factory->inspect_service_open(value.identity().who,value.request(),result);
        }
        error settle(handle* completed)noexcept
        {
            if(!topic&&!service)return error::again;
            communication::operation_token token;communication::native_result outcome;
            auto status=topic?topic->completion(token,outcome):service->completion(token,outcome);
            if(status!=error::ok)return status;
            status=static_cast<error>(dispatch->settle_operation(token,outcome));if(status!=error::ok)return status;
            // Unknown effects stay in both ledgers. No repeated send, implicit
            // refund or capacity reuse follows an uncertain completion.
            if(outcome.observed==communication::effect::unknown)return error::busy;
            status=topic?topic->release_completion(token):service->release_completion(token);
            if(status==error::ok&&completed)*completed=token.request;
            return status;
        }
        error reconcile()noexcept
        {
            if(!current||!current->request()||(!topic&&!service))return error::again;
            if(!(topic?topic->cleanup_confirmed():service->cleanup_confirmed()))return error::again;
            communication_dispatch::usage usage;auto status=dispatch->inspect(alias,usage);
            if(status!=scene::error::success)return static_cast<error>(status);
            if(usage.handles||usage.orphaned)return error::busy;
            return factory->reconcile_close(current->request(),current->resource());
        }
    public:
        /** @brief Freeze one exact plan before the factory or dispatcher starts.
         * @details The factory is retained but may still be unconfigured.
         * Configure its matching peer with admission_context(), admit_open and
         * charge_open, retaining that context in the peer configuration. This
         * step does not issue authority, reserve native resources or launch ROS.
         */
        error configure(::uwvm2::utils::container::std_string name,kind selected,
                        std::shared_ptr<communication_dispatch> target,std::shared_ptr<paired_factory> actual)noexcept
        {
            if(process!=::getpid()||!target||!actual)return error::denied;
            std::lock_guard lock{transaction};if(configured.load())return error::denied;
            if(selected!=kind::topic&&selected!=kind::service)return error::invalid;
            try
            {
                communication::owner identity;scene::error status;
                if(selected==kind::topic){communication::topic_plan plan;status=target->describe_plan(name,plan);identity=plan.authority;}
                else{communication::service_plan plan;status=target->describe_plan(name,plan);identity=plan.authority;}
                if(status!=scene::error::success)return static_cast<error>(status);
                auto context=::uwvm2::utils::container::make_shared<dispatch_admission>(target);
                alias=std::move(name);owner=identity;family=selected;dispatch=std::move(target);factory=std::move(actual);admission=std::move(context);
                configured.store(true,std::memory_order_release);return error::ok;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /// @brief Immutable callback ownership for trusted creation-queue configuration.
        std::shared_ptr<dispatch_admission> admission_context()const noexcept{return ready()?admission:std::shared_ptr<dispatch_admission>{};}
        /** @brief Submit/recover an exact 48-byte creation receipt without native work.
         * @details Failed output retains accepted work. Short output, denial and
         * allocation failure cannot launch an endpoint. Concurrent/reentrant
         * mutations return again rather than blocking inside a guest callback.
         */
        scene::error invoke_open(caller who,handle cap,wire::bytes body,std::size_t capacity,std::size_t& required,
                                 communication_dispatch::sink deliver,communication_open_dispatch::cancellation cancelled={})noexcept
        {
            required=communication_open_contract::receipt_bytes;if(!ready())return scene::error::denied;
            std::unique_lock lock{transaction,std::try_to_lock};if(!lock.owns_lock()||released)return scene::error::again;
            auto action=family==kind::topic?communication_contract::operation::topic_open:communication_contract::operation::service_open;
            return dispatch->invoke_open(who,action,cap,body,capacity,required,deliver,{this,enqueue,inspect_creation},cancelled);
        }
        error inspect(caller who,handle request,communication_open_dispatch::observation& result)noexcept override{return own(who,request,result);}
        error validate(communication_creation::identity const& origin)noexcept override
        {if(!ready()||origin.authority!=owner)return error::denied;group_lease held;return dispatch_admission::admit_open(admission.get(),origin,held);}
        /** @brief Privately adopt only the exact verified native creation result. */
        error prepare(caller who,handle request,handle& resource)noexcept override
        {
            if(!ready())return error::denied;
            std::unique_lock lock{transaction,std::try_to_lock};if(!lock.owns_lock())return error::again;
            communication_open_dispatch::observation result;auto status=own(who,request,result);if(status!=error::ok)return status;
            if(!current||current->request()!=request||current->identity()!=result.origin)return error::denied;
            if(result.state.receipt.status!=communication_open_contract::state::ready||result.state.receipt.observed==communication::effect::unknown)return error::busy;
            status=validate(result.origin);if(status!=error::ok)return status;
            if(!topic&&!service)
            {
                communication::node_lease parent;status=static_cast<error>(dispatch->adoption_node(current,parent));if(status!=error::ok)return status;
                if(family==kind::topic)status=factory->create_verified_topic(request,*current->specification<communication::topic_plan>(),*current->schema(0),
                    std::move(parent),current->resource(),dispatch_admission::admit,dispatch_admission::charge,admission.get(),topic,admission);
                else status=factory->create_verified_service(request,*current->specification<communication::service_plan>(),*current->schema(0),*current->schema(1),
                    std::move(parent),current->resource(),dispatch_admission::admit,dispatch_admission::charge,admission.get(),service,admission);
                if(status!=error::ok)return status;
            }
            status=static_cast<error>(topic?dispatch->adopt_open(current,topic):dispatch->adopt_open(current,service));
            if(status==error::ok)resource=current->resource();
            return status;
        }
        error publish(caller who,handle request,group_lease const& held,handle resource,communication_open_contract::published& result)noexcept override
        {
            if(!ready())return error::denied;
            std::unique_lock lock{transaction,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(!current||current->request()!=request||current->resource()!=resource)return error::denied;
            auto status=family==kind::topic?factory->publish_topic(who,request,held,resource,result):factory->publish_service(who,request,held,resource,result);
            return status==error::ok?static_cast<error>(dispatch->publish_open(current,result,held)):status;
        }
        error cancel(caller who,handle request,communication_creation::view& result)noexcept override
        {
            communication_open_dispatch::observation original;auto status=own(who,request,original);if(status!=error::ok)return status;
            if(original.state.receipt.status==communication_open_contract::state::published)return error::busy;
            return factory->cancel_open(who,request,result);
        }
        /** @brief Release only a known terminal creation; retain facades until pump().
         * @details Native close and host-registry close must both be positively
         * observed. No wait/drain runs under the request.release authority lease.
         * The supervisor later drops the final facade references and capacity.
         */
        error release(caller who,handle request)noexcept override
        {
            if(!ready())return error::denied;
            std::unique_lock lock{transaction,std::try_to_lock};if(!lock.owns_lock())return error::again;
            communication_open_dispatch::observation result;auto status=own(who,request,result);if(status!=error::ok)return status;
            if(!current||current->request()!=request)return error::denied;
            if(topic||service){status=reconcile();if(status!=error::ok)return status;}
            status=factory->release_open(who,request);if(status!=error::ok)return status;
            status=own(who,request,result);if(status!=error::ok)return status;
            status=static_cast<error>(dispatch->release_open(current,result.state));if(status==error::ok)released=true;
            return status;
        }
        /** @brief Advance finite worker work and settle exact host mutation receipts.
         * @details Call outside authority/registry locks. This also drains and
         * releases already-closed facades after request.release; it never closes
         * an endpoint implicitly. Unknown results remain occupied. Return again
         * means no completion in this step, never a scheduler latency guarantee.
         * An optional host-only output reports a settled host request, never a
         * native endpoint identity; it is zero when no mutation was settled.
         */
        error pump(handle* completed=nullptr)noexcept
        {
            if(completed)*completed=0;
            if(!ready())return error::denied;
            std::unique_lock lock{transaction,std::try_to_lock};if(!lock.owns_lock())return error::again;
            // Keep the small released draft for failed-copy replay. A later
            // accepted creation replaces it; its counted parent is already free.
            if(released){topic.reset();service.reset();released=false;return error::ok;}
            auto status=family==kind::topic?factory->pump_topics():factory->pump_services();
            auto settled=settle(completed);
            auto cleaned=reconcile();
            if(settled!=error::ok&&settled!=error::again&&settled!=error::busy)return settled;
            if(cleaned!=error::ok&&cleaned!=error::again&&cleaned!=error::busy)return cleaned;
            if(status!=error::ok&&status!=error::again&&status!=error::busy)return status;
            return status==error::ok||settled==error::ok||cleaned==error::ok?error::ok:error::again;
        }
    };
#endif
}
