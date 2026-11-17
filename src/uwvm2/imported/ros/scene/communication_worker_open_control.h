/** @file
 * @brief Creation-control adapter for an exact sealed worker binding.
 * @details The shared factory owns native creation receipts and positive cleanup
 * evidence. The retained typed adoption context owns the host registry draft;
 * its callbacks validate actual node/schema origins and prepare a bounded typed
 * resource from verified factory evidence. No callback may start native I/O or
 * publish a handle. Publication below always passes through the factory's exact
 * owner/resource checks. There is no arbitrary native token or plugin pointer in
 * a guest carrier, and no compatibility path to preload a library in the VM.
 */
#pragma once
#include "communication_open_dispatch.h"
#include "communication_worker_provider.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
#if defined(__linux__)
    class open_control final:public communication_open_dispatch::provider
    {
    public:
        using error=communication::error;
        struct adoption
        {
            std::shared_ptr<void> context;
            error(*validate)(void*,communication_creation::identity const&)noexcept{};
            error(*prepare)(void*,caller,handle,handle&)noexcept{};
            error(*published)(void*,communication_open_contract::published const&,group_lease const&)noexcept{};
            error(*released)(void*,communication_creation::view const&)noexcept{};
        };
    private:
        std::shared_ptr<paired_factory> factory;
        communication::owner owner;
        communication_open_contract::endpoint_kind family{};
        adoption typed;
        pid_t process{::getpid()};
        bool same_process()const noexcept{return process==::getpid();}
        bool configured()const noexcept
        {
            return same_process()&&factory&&owner&&typed.context&&typed.validate&&typed.prepare&&
                family>=communication_open_contract::endpoint_kind::topic&&family<=communication_open_contract::endpoint_kind::action;
        }
        error own(caller who,handle request,communication_open_dispatch::observation& result)noexcept
        {
            if(!configured())return error::denied;
            auto status=factory->inspect_open_origin(who,request,result.state,result.origin);
            if(status!=error::ok)return status;
            return result.origin.authority==owner&&result.state.kind==family?error::ok:error::denied;
        }
    public:
        /** @brief Retain immutable factory and typed draft ownership; perform no native work. */
        open_control(std::shared_ptr<paired_factory> actual,communication::owner scope,
                     communication_open_contract::endpoint_kind kind,adoption selected)noexcept
            :factory(std::move(actual)),owner(scope),family(kind),typed(std::move(selected)){}
        error inspect(caller who,handle request,communication_open_dispatch::observation& result)noexcept override
        {return own(who,request,result);}
        error validate(communication_creation::identity const& origin)noexcept override
        {
            if(!configured()||origin.authority!=owner)return error::denied;
            return typed.validate(typed.context.get(),origin);
        }
        error prepare(caller who,handle request,handle& resource)noexcept override
        {
            communication_open_dispatch::observation retained;auto status=own(who,request,retained);if(status!=error::ok)return status;
            if(retained.state.receipt.status!=communication_open_contract::state::ready||retained.state.receipt.observed==communication::effect::unknown)return error::busy;
            status=typed.validate(typed.context.get(),retained.origin);if(status!=error::ok)return status;
            return typed.prepare(typed.context.get(),who,request,resource);
        }
        error publish(caller who,handle request,group_lease const& current,handle resource,communication_open_contract::published& out)noexcept override
        {
            communication_open_dispatch::observation retained;auto status=own(who,request,retained);if(status!=error::ok)return status;
            using K=communication_open_contract::endpoint_kind;
            if(family==K::topic)status=factory->publish_topic(who,request,current,resource,out);
            else if(family==K::service)status=factory->publish_service(who,request,current,resource,out);
            else if(family==K::action)status=factory->publish_action(who,request,current,resource,out);
            else return error::denied;
            return status==error::ok&&typed.published?typed.published(typed.context.get(),out,current):status;
        }
        error cancel(caller who,handle request,communication_creation::view& result)noexcept override
        {
            communication_open_dispatch::observation retained;auto status=own(who,request,retained);if(status!=error::ok)return status;
            if(retained.state.receipt.status==communication_open_contract::state::published)return error::busy;
            return factory->cancel_open(who,request,result);
        }
        error release(caller who,handle request)noexcept override
        {
            communication_open_dispatch::observation retained;auto status=own(who,request,retained);
            if(status!=error::ok)return status;
            status=factory->release_open(who,request);if(status!=error::ok||!typed.released)return status;
            status=own(who,request,retained);return status==error::ok?typed.released(typed.context.get(),retained.state):status;
        }
    };
#endif
}
