/** @file
 * Trusted compile-selected builtin/custom owned-subscription selector.
 * Include this only in the native owning consumer. No factory pointer comes
 * from a Guest, graph name or IPC packet; the caller authenticates and retains
 * its source lease, registry slot and rollback before calling. This header
 * grants no operation and never invokes the legacy alias-based adapter.
 */
#pragma once
#include "compat/node.h"
#include "native_rmw_subscription.h"

namespace uwvm2::ros_gateway::native_rmw_subscription
{
    /** Compose the exact four real builtin contracts with the complete custom
     * provider. A selected builtin failure is final, not a reason to try a
     * different codec. Custom binding::known and real native type templates
     * are instantiated only in the selected generated shard TU. The inherited
     * queue owns the same-callback URMI/body record and its independent borrow
     * nonce; this selector never copies, consumes or manufactures such data.
     */
    inline std::uint32_t create_selected(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,
        ros::capability_spec const& cap,wire::binding const& owner,std::uint64_t incarnation,
        std::shared_ptr<endpoint>& output,wire::packet& observed) noexcept
    {
        if(!node||!wire::valid(owner)||!incarnation||cap.operation!="topic.subscribe"){return 22;}
        if(cap.type==native_rmw_builtin::string_codec::type)
        {return builtin<native_rmw_builtin::string_codec>::create(node,cap,owner,incarnation,output,observed);}
        if(cap.type==native_rmw_builtin::bool_codec::type)
        {return builtin<native_rmw_builtin::bool_codec>::create(node,cap,owner,incarnation,output,observed);}
        if(cap.type==native_rmw_builtin::twist_codec::type)
        {return builtin<native_rmw_builtin::twist_codec>::create(node,cap,owner,incarnation,output,observed);}
        if(cap.type==native_rmw_builtin::image_codec::type)
        {return builtin<native_rmw_builtin::image_codec>::create(node,cap,owner,incarnation,output,observed);}
        return ros2_custom_topics::create_owned_rmw_subscription(node,cap,owner,incarnation,output,observed);
    }
}
