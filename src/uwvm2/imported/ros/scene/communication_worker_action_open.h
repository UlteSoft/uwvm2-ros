/** @file
 * @brief Owned asynchronous action creation, publication and completion routing.
 * @details A sealed worker receives only canonical messages for one exact
 * action plan. Its node, three ordered schemas, role and transition rights are
 * independently revalidated before every native effect. No guest native token,
 * library path or callback is accepted. Configuration and enqueue perform no
 * process startup; the pair supervisor advances finite work outside authority
 * locks. Failed guest delivery retains exact receipts. Unknown effects retain
 * capacity and cannot be retried as a new effect or released by timeout.
 *
 * Admission contexts weakly retain their dispatcher and are strongly retained
 * by the worker queue and facade. Expiration denies admission. All owning
 * allocation uses recoverable fast_io. Resource close and creation release
 * remain separate authorized operations; process reaping belongs to deployment.
 */
#pragma once
#include "communication_action_dispatch.h"
#include "communication_open_dispatch.h"
#include "communication_worker_provider.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
#if defined(__linux__)
    class action_admission
    {
        std::weak_ptr<communication_action_dispatch> target;
        pid_t process{::getpid()};
        std::shared_ptr<communication_action_dispatch> lock()const noexcept
        {return process==::getpid()?target.lock():std::shared_ptr<communication_action_dispatch>{};}
    public:
        explicit action_admission(std::shared_ptr<communication_action_dispatch> const& value)noexcept:target(value){}
        static communication::error admit_open(void* context,communication_creation::identity const& original,group_lease& held)noexcept
        {
            if(!context)return communication::error::denied;
            auto actual=static_cast<action_admission*>(context)->lock();
            return actual?static_cast<communication::error>(actual->admit_open(original,0,held)):communication::error::denied;
        }
        static communication::error charge_open(void* context,communication_creation::identity const& original,std::uint64_t bytes,group_lease& held)noexcept
        {
            if(!context)return communication::error::denied;
            auto actual=static_cast<action_admission*>(context)->lock();
            return actual?static_cast<communication::error>(actual->admit_open(original,bytes,held,true)):communication::error::denied;
        }
        static communication::error admit(void* context,communication::operation_token const& original,group_lease& held)noexcept
        {
            if(!context)return communication::error::denied;
            auto actual=static_cast<action_admission*>(context)->lock();
            return actual?static_cast<communication::error>(actual->revalidate(original,held)):communication::error::denied;
        }
        static communication::error charge(void* context,communication::operation_token const& original,std::uint64_t bytes,group_lease& held)noexcept
        {
            if(!context)return communication::error::denied;
            auto actual=static_cast<action_admission*>(context)->lock();
            return actual?static_cast<communication::error>(actual->charge_operation(original,bytes,held)):communication::error::denied;
        }
    };
    class action_open_provider final:public communication_open_dispatch::provider
    {
        using error=communication::error;
        using draft=communication_action_dispatch::opening;
        std::shared_ptr<communication_action_dispatch> dispatch;
        std::shared_ptr<paired_factory> factory;
        std::shared_ptr<action_admission> admission;
        ::uwvm2::utils::container::std_string alias;
        communication::owner owner;
        std::shared_ptr<draft> current;
        std::shared_ptr<action_endpoint> facade;
        std::shared_ptr<communication::actions::endpoint> core;
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
            return result.origin.authority==owner&&result.state.kind==communication_open_contract::endpoint_kind::action?error::ok:error::denied;
        }
        static error enqueue(void* context,draft const& value,wire::bytes body,communication_creation::view& result)noexcept
        {
            auto& self=*static_cast<action_open_provider*>(context);
            if(value.identity().authority!=self.owner)return error::denied;
            std::shared_ptr<draft> retained;auto status=self.dispatch->retain_open(value.identity(),retained);
            if(status!=scene::error::success)return static_cast<error>(status);
            self.current=std::move(retained);
            return self.factory->open_action_request(value.node(),value.identity(),body,result);
        }
        static error inspect_creation(void* context,draft const& value,communication_creation::view& result)noexcept
        {return static_cast<action_open_provider*>(context)->factory->inspect_action_open(value.identity().who,value.request(),result);}
        error settle()noexcept
        {
            if(!facade||!core)return error::again;
            communication::operation_token token;communication::native_result outcome;
            auto status=facade->completion(token,outcome);if(status!=error::ok)return status;
            status=core->settle_operation(token.request,outcome);if(status!=error::ok)return status;
            return outcome.observed==communication::effect::unknown?error::busy:facade->release_completion(token);
        }
        error reconcile()noexcept
        {
            if(!current||!current->request()||!facade||!facade->cleanup_confirmed())return error::again;
            communication_action_dispatch::usage usage;auto status=dispatch->inspect(alias,usage);
            if(status!=scene::error::success)return static_cast<error>(status);
            if(usage.handles||usage.fenced)return error::busy;
            return factory->reconcile_close(current->request(),current->resource());
        }
    public:
        /** @brief Bind one immutable action plan before factory preparation; no authority is created. */
        error configure(::uwvm2::utils::container::std_string name,std::shared_ptr<communication_action_dispatch> target,
                        std::shared_ptr<paired_factory> actual)noexcept
        {
            if(process!=::getpid()||!target||!actual)return error::denied;
            std::lock_guard lock{transaction};if(configured.load())return error::denied;
            try
            {
                communication_action_dispatch::plan selected;auto status=target->describe_plan(name,selected);
                if(status!=scene::error::success)return static_cast<error>(status);
                auto context=::uwvm2::utils::container::make_shared<action_admission>(target);
                alias=std::move(name);owner=selected.policy.authority;dispatch=std::move(target);factory=std::move(actual);admission=std::move(context);
                configured.store(true,std::memory_order_release);return error::ok;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /** @brief Admit one exact creation key and retain failed delivery without native I/O. */
        scene::error invoke_open(caller who,handle cap,wire::bytes body,std::size_t capacity,std::size_t& required,
                                 communication_dispatch::sink deliver,communication_open_dispatch::cancellation cancelled={})noexcept
        {
            required=communication_open_contract::receipt_bytes;if(!ready())return scene::error::denied;
            std::unique_lock lock{transaction,std::try_to_lock};if(!lock.owns_lock()||released)return scene::error::again;
            return dispatch->invoke_open(who,cap,body,capacity,required,deliver,{this,enqueue,inspect_creation},cancelled);
        }
        error inspect(caller who,handle request,communication_open_dispatch::observation& result)noexcept override{return own(who,request,result);}
        error validate(communication_creation::identity const& original)noexcept override
        {if(!ready()||original.authority!=owner)return error::denied;group_lease held;return action_admission::admit_open(admission.get(),original,held);}
        /** @brief Adopt only a verified typed endpoint into its reserved private action resource. */
        error prepare(caller who,handle request,handle& resource)noexcept override
        {
            if(!ready())return error::denied;
            std::unique_lock lock{transaction,std::try_to_lock};if(!lock.owns_lock())return error::again;
            communication_open_dispatch::observation result;auto status=own(who,request,result);if(status!=error::ok)return status;
            if(!current||current->request()!=request||current->identity()!=result.origin)return error::denied;
            if(result.state.receipt.status!=communication_open_contract::state::ready||result.state.receipt.observed==communication::effect::unknown)return error::busy;
            status=validate(result.origin);if(status!=error::ok)return status;
            if(!core)
            {
                communication::node_lease parent;status=static_cast<error>(dispatch->adoption_node(current,parent));if(status!=error::ok)return status;
                status=factory->create_verified_action(request,current->specification(),std::move(parent),current->resource(),
                    action_admission::admit,action_admission::charge,admission.get(),facade,core,admission);
                if(status!=error::ok)return status;
            }
            status=static_cast<error>(dispatch->adopt_open(current,core));if(status==error::ok)resource=current->resource();return status;
        }
        error publish(caller who,handle request,group_lease const& held,handle resource,communication_open_contract::published& result)noexcept override
        {
            if(!ready())return error::denied;
            std::unique_lock lock{transaction,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(!current||current->request()!=request||current->resource()!=resource)return error::denied;
            auto status=factory->publish_action(who,request,held,resource,result);
            return status==error::ok?static_cast<error>(dispatch->publish_open(current,result,held)):status;
        }
        error cancel(caller who,handle request,communication_creation::view& result)noexcept override
        {
            communication_open_dispatch::observation original;auto status=own(who,request,original);if(status!=error::ok)return status;
            if(original.state.receipt.status==communication_open_contract::state::published)return error::busy;
            return factory->cancel_open(who,request,result);
        }
        /** @brief Release a known terminal receipt; supervisor later drops drained facade ownership. */
        error release(caller who,handle request)noexcept override
        {
            if(!ready())return error::denied;
            std::unique_lock lock{transaction,std::try_to_lock};if(!lock.owns_lock())return error::again;
            communication_open_dispatch::observation result;auto status=own(who,request,result);if(status!=error::ok)return status;
            if(!current||current->request()!=request)return error::denied;
            if(facade){status=reconcile();if(status!=error::ok)return status;}
            status=factory->release_open(who,request);if(status!=error::ok)return status;
            status=own(who,request,result);if(status!=error::ok)return status;
            status=static_cast<error>(dispatch->release_open(current,result.state));if(status==error::ok)released=true;return status;
        }
        /** @brief Advance one finite supervisor step and settle only actual native results.
         * @details No polling wait, guest callback or new permission is hidden
         * here. Explicit process placement and poll cadence govern scheduling;
         * neither establishes a hard real-time bound. Unknown completions stay
         * occupied, even after worker cleanup has been confirmed.
         */
        error pump()noexcept
        {
            if(!ready())return error::denied;
            std::unique_lock lock{transaction,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(released){auto status=dispatch->drain_released(current);if(status!=scene::error::success)return static_cast<error>(status);
                core.reset();facade.reset();released=false;return error::ok;}
            auto status=factory->pump_actions(),settled=settle(),cleaned=reconcile();
            for(auto result:{status,settled,cleaned})if(result!=error::ok&&result!=error::again&&result!=error::busy)return result;
            return status==error::ok||settled==error::ok||cleaned==error::ok?error::ok:error::again;
        }
    };
#endif
}
