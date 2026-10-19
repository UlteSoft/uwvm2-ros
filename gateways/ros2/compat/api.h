#pragma once
#include "features.h"
#include <rclcpp/rclcpp.hpp>
#include <rmw/types.h>
#include <chrono>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <rcl/guard_condition.h>
#if __has_include(<rosidl_buffer/buffer.hpp>)
#include <rosidl_buffer/buffer.hpp>
#include <rosidl_buffer/cpu_buffer_impl.hpp>
#endif

namespace uwvm2::ros_gateway::ros2_compat
{
#if UWVM_ROS2_HAVE_QOS
    using qos_type = ::rclcpp::QoS;
    inline qos_type make_qos(rmw_qos_profile_t const& value)
    { return qos_type{::rclcpp::QoSInitialization::from_rmw(value),value}; }
    inline rmw_qos_profile_t profile(qos_type const& value) { return value.get_rmw_qos_profile(); }
#else
    using qos_type = rmw_qos_profile_t;
    inline qos_type make_qos(rmw_qos_profile_t const& value) { return value; }
    inline rmw_qos_profile_t profile(qos_type const& value) { return value; }
#endif
    /* Humble accepts the complete rmw profile; Jazzy adds the QoS overload.
     * SFINAE chooses the installed signature and never substitutes a default.
     * The public actual-QoS check remains separate and mandatory. */
    template<class S,class Node> auto create_client_impl(Node const& node,std::string const& name,qos_type const& qos,int)
        -> decltype(node->template create_client<S>(name,qos))
    { return node->template create_client<S>(name,qos); }
    template<class S,class Node> auto create_client_impl(Node const& node,std::string const& name,qos_type const& qos,long)
        -> decltype(node->template create_client<S>(name,profile(qos)))
    { return node->template create_client<S>(name,profile(qos)); }
    template<class S,class Node> auto create_client(Node const& node,std::string const& name,qos_type const& qos)
    { return create_client_impl<S>(node,name,qos,0); }
    template<class S,class Node,class Callback> auto create_service_impl(Node const& node,std::string const& name,Callback&& callback,qos_type const& qos,int)
        -> decltype(node->template create_service<S>(name,std::forward<Callback>(callback),qos))
    { return node->template create_service<S>(name,std::forward<Callback>(callback),qos); }
    template<class S,class Node,class Callback> auto create_service_impl(Node const& node,std::string const& name,Callback&& callback,qos_type const& qos,long)
        -> decltype(node->template create_service<S>(name,std::forward<Callback>(callback),profile(qos)))
    { return node->template create_service<S>(name,std::forward<Callback>(callback),profile(qos)); }
    template<class S,class Node,class Callback> auto create_service(Node const& node,std::string const& name,Callback&& callback,qos_type const& qos)
    { return create_service_impl<S>(node,name,std::forward<Callback>(callback),qos,0); }
    template<class M,class Node> auto create_publisher_impl(Node const& node,std::string const& name,qos_type const& qos,int)
        -> decltype(node->template create_publisher<M>(name,qos))
    { return node->template create_publisher<M>(name,qos); }
    template<class M,class Node> auto create_publisher_impl(Node const& node,std::string const& name,qos_type const& qos,long)
        -> decltype(node->template create_publisher<M>(name,profile(qos)))
    { return node->template create_publisher<M>(name,profile(qos)); }
    template<class M,class Node> auto create_publisher(Node const& node,std::string const& name,qos_type const& qos)
    { return create_publisher_impl<M>(node,name,qos,0); }
    template<class M,class Node,class Callback> auto create_subscription_impl(Node const& node,std::string const& name,qos_type const& qos,Callback&& callback,int)
        -> decltype(node->template create_subscription<M>(name,qos,std::forward<Callback>(callback)))
    { return node->template create_subscription<M>(name,qos,std::forward<Callback>(callback)); }
    template<class M,class Node,class Callback> auto create_subscription_impl(Node const& node,std::string const& name,qos_type const& qos,Callback&& callback,long)
        -> decltype(node->template create_subscription<M>(name,std::forward<Callback>(callback),profile(qos)))
    { return node->template create_subscription<M>(name,std::forward<Callback>(callback),profile(qos)); }
    template<class M,class Node,class Callback> auto create_subscription(Node const& node,std::string const& name,qos_type const& qos,Callback&& callback)
    { return create_subscription_impl<M>(node,name,qos,std::forward<Callback>(callback),0); }

    template<class Client,class Pending> auto remove_pending_impl(Client& client,Pending const& pending,int)
        -> decltype(client.remove_pending_request(pending),void())
    { (void)client.remove_pending_request(pending); }
    template<class Client,class Pending> void remove_pending_impl(Client&,Pending const&,long) noexcept {}
    /* The fallback is only for the adapter's fresh single-request client. Its
     * owning scope destroys the complete Client and its pending map on timeout;
     * this is not permission to retain unbounded legacy Client requests. */
    template<class Client,class Pending> void remove_pending(Client& client,Pending const& pending)
    { remove_pending_impl(client,pending,0); }
    /* Native rclcpp releases expose four different notification interfaces.
     * Humble returns GuardCondition by reference; other releases return a
     * pointer, provide a direct notification method, or expose the locked raw
     * rcl guard. Select by the actual public expression, in this order, so
     * overlapping APIs cannot make an integral-conversion overload ambiguous.
     * Each selected path performs a real notification; there is no no-op
     * fallback when an SDK has none of these supported interfaces.
     */
    template<unsigned Level> struct notify_priority : notify_priority<Level-1> {};
    template<> struct notify_priority<0> {};
    template<class Node> auto notify_impl(Node const& node,notify_priority<3>)
        -> decltype(node->get_node_base_interface()->trigger_notify_guard_condition(),void())
    { node->get_node_base_interface()->trigger_notify_guard_condition(); }
    template<class Node> auto notify_impl(Node const& node,notify_priority<2>)
        -> decltype(node->get_node_base_interface()->get_notify_guard_condition()->trigger(),void())
    { node->get_node_base_interface()->get_notify_guard_condition()->trigger(); }
    template<class Node> auto notify_impl(Node const& node,notify_priority<1>)
        -> decltype(node->get_node_base_interface()->get_notify_guard_condition().trigger(),void())
    { node->get_node_base_interface()->get_notify_guard_condition().trigger(); }
    template<class Node> auto notify_impl(Node const& node,notify_priority<0>)
        -> decltype(node->get_node_base_interface()->acquire_notify_guard_condition_lock(),void())
    {
        // Foxy exposes the raw guard plus its public lock, not GuardCondition::trigger.
        auto base{node->get_node_base_interface()};auto lock{base->acquire_notify_guard_condition_lock()};
        auto guard{base->get_notify_guard_condition()};
        if(guard&&rcl_trigger_guard_condition(guard)!=RCL_RET_OK)
        { throw std::runtime_error("native executor notification failed"); }
    }
    template<class Node> void notify(Node const& node) { notify_impl(node,notify_priority<3>{}); }

    /* Foxy passes shared_future<GoalHandle::SharedPtr>; Galactic and later pass
     * the shared pointer itself. No implicit get/wait is performed for a pointer.
     * The old callback runs only after its future is ready in native rclcpp. */
    template<class Handle,class Value> typename Handle::SharedPtr goal_response(Value const& value)
    {
        if constexpr(std::is_convertible_v<Value,typename Handle::SharedPtr>) { return value; }
        else { return value.get(); }
    }
    template<class Container> bool native_bytes_accessible(Container const&) noexcept { return true; }
#if __has_include(<rosidl_buffer/buffer.hpp>)
    template<class T,class Allocator> bool native_bytes_accessible(::rosidl::Buffer<T,Allocator> const& value) noexcept
    {
        // Buffer::size alone does not imply accessible CPU storage. Avoid its
        // throwing data()/iterator API on GPU/opaque backends; never implicitly
        // to_vector() an attacker-controlled native allocation before quotas.
        return dynamic_cast<::rosidl::CpuBufferImpl<T,Allocator> const*>(value.get_impl())!=nullptr;
    }
#endif
    template<class Buffer,class Iterator> auto assign_bytes_impl(Buffer& output,Iterator begin,Iterator end,int)
        -> decltype(output.assign(begin,end),void()) { output.assign(begin,end); }
    template<class Buffer,class Iterator> auto assign_bytes_impl(Buffer& output,Iterator begin,Iterator end,long)
        -> decltype(output.resize(static_cast<std::size_t>(end-begin)),output.data(),void())
    {
        auto count{static_cast<std::size_t>(end-begin)};output.resize(count);
        for(std::size_t i{};i!=count;++i) { output.data()[i]=begin[i]; }
    }
    /* Lyrical uint8[] may be rosidl::Buffer rather than std::vector. The caller
     * validates the complete wire and explicit byte quota before this copy. */
    template<class Buffer,class Iterator> void assign_bytes(Buffer& output,Iterator begin,Iterator end)
    { assign_bytes_impl(output,begin,end,0); }
    template<class Descriptor> auto is_dynamic_typing_impl(Descriptor const& value,int)
        -> decltype(value.dynamic_typing,bool()) { return value.dynamic_typing; }
    // Pre-Galactic parameters could change type. Absence of the descriptor
    // member is not evidence of a statically constrained remote parameter.
    template<class Descriptor> bool is_dynamic_typing_impl(Descriptor const&,long) noexcept { return true; }
    template<class Descriptor> bool is_dynamic_typing(Descriptor const& value)
    { return is_dynamic_typing_impl(value,0); }
    template<class Node> auto parameter_events_enabled_impl(Node const& node,int)
        -> decltype(node->get_node_options().start_parameter_event_publisher(),bool())
    { return node->get_node_options().start_parameter_event_publisher(); }
    template<class Node> bool parameter_events_enabled_impl(Node const&,long) noexcept { return true; }
    template<class Node> bool parameter_events_enabled(Node const& node) { return parameter_events_enabled_impl(node,0); }
    template<class Node> auto full_name_impl(Node const& node,int) -> decltype(std::string{node->get_fully_qualified_name()})
    { return std::string{node->get_fully_qualified_name()}; }
    template<class Node> std::string full_name_impl(Node const& node,long)
    { std::string result{node->get_namespace()};if(result.empty()||result.back()!='/') { result+='/'; }return result+node->get_name(); }
    template<class Node> std::string full_name(Node const& node) { return full_name_impl(node,0); }
    template<class Descriptor> auto dynamic_typing_impl(Descriptor& value,int)
        -> decltype(value.dynamic_typing=true,void()) { value.dynamic_typing=true; }
    template<class Descriptor> void dynamic_typing_impl(Descriptor&,long) noexcept {}
    template<class Descriptor> void dynamic_typing(Descriptor& value) { dynamic_typing_impl(value,0); }
}
