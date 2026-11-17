/** @file
 * @brief Retained authorization and factory bridge for isolated graph capture.
 * @details A graph dispatcher retains this bridge; native queues retain only its
 * admission object, which holds a weak dispatcher reference. This prevents an
 * ownership cycle and fails closed after dispatcher retirement. Every capture
 * revalidates the current caller, graph, observer node and context capabilities.
 * Creation reserves the sealed worker's finite query budget without starting a
 * node, adding a thread, granting a socket or spending a capture rate unit.
 */
#pragma once
#include "communication_worker_provider.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
#if defined(__linux__)
    class graph_admission
    {
        std::weak_ptr<communication_graph_dispatch> target;
        pid_t process{::getpid()};
        std::shared_ptr<communication_graph_dispatch> lock()const noexcept
        {return process==::getpid()?target.lock():std::shared_ptr<communication_graph_dispatch>{};}
    public:
        explicit graph_admission(std::shared_ptr<communication_graph_dispatch> const& value)noexcept:target(value){}
        static communication::error admit(void* context,communication::operation_token const& token,group_lease& held)noexcept
        {
            if(!context)return communication::error::denied;
            auto actual=static_cast<graph_admission*>(context)->lock();
            return actual?static_cast<communication::error>(actual->revalidate(token,held)):communication::error::denied;
        }
        static communication::error charge(void* context,communication::operation_token const& token,std::uint64_t bytes,group_lease& held)noexcept
        {
            if(!context)return communication::error::denied;
            auto actual=static_cast<graph_admission*>(context)->lock();
            return actual?static_cast<communication::error>(actual->charge_operation(token,bytes,held)):communication::error::denied;
        }
    };
    struct graph_factory
    {
        std::shared_ptr<paired_factory> provider;
        std::shared_ptr<graph_admission> admission;
        static communication::error create(void* context,communication::graph::plan const& plan,
            communication::node_lease node,std::shared_ptr<communication::graph::source>& output)noexcept
        {
            if(!context)return communication::error::denied;
            auto& self=*static_cast<graph_factory*>(context);
            if(!self.provider||!self.admission)return communication::error::denied;
            return self.provider->create_graph(plan,std::move(node),graph_admission::admit,graph_admission::charge,
                self.admission.get(),output,self.admission);
        }
    };
#endif
}
