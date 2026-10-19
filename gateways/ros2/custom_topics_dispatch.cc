#include "compat/node.h"
/*************************************************************
 * UWVM2 ROS2 custom-topic shard dispatch. APL-2.0.          *
 *************************************************************/
#include "custom_topics.h"
#include <array>
#include <utility>

// This translation unit intentionally never includes custom_topics_impl.h.
// Its tables refer to out-of-line symbols from every generated shard; omitting
// a shard object is a link error, rather than silently losing provider rows.
namespace uwvm2::ros_gateway::ros2_custom_topics::detail
{
    namespace
    {
        using publish_function=ipc::call_result(*)(adapter&,std::size_t,
            ros::capability_spec const&,std::vector<std::uint8_t> const&);
        using subscribe_function=ipc::call_result(*)(adapter&,std::size_t,
            ros::capability_spec const&,std::shared_ptr<event_queue>);
        using owned_publisher_function=std::uint32_t(*)(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&,std::size_t,
            ros::capability_spec const&,ipc::rmw_endpoint::binding const&,std::uint64_t,
            std::shared_ptr<native_rmw_publisher::endpoint>&,ipc::rmw_endpoint::packet&) noexcept;
        using owned_subscription_function=std::uint32_t(*)(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&,std::size_t,
            ros::capability_spec const&,ipc::rmw_endpoint::binding const&,std::uint64_t,
            std::shared_ptr<native_rmw_subscription::endpoint>&,ipc::rmw_endpoint::packet&) noexcept;

        template<std::size_t... Index>
        constexpr std::array<publish_function,sizeof...(Index)> publish_table(std::index_sequence<Index...>)
        { return {{&topic_shard<Index,topic_shards>::publish...}}; }

        template<std::size_t... Index>
        constexpr std::array<subscribe_function,sizeof...(Index)> subscribe_table(std::index_sequence<Index...>)
        { return {{&topic_shard<Index,topic_shards>::subscribe...}}; }

        template<std::size_t... Index>
        constexpr std::array<owned_publisher_function,sizeof...(Index)> owned_publisher_table(std::index_sequence<Index...>)
        {return {{&topic_shard<Index,topic_shards>::create_owned_rmw_publisher...}};}
        template<std::size_t... Index>
        constexpr std::array<owned_subscription_function,sizeof...(Index)> owned_subscription_table(std::index_sequence<Index...>)
        {return {{&topic_shard<Index,topic_shards>::create_owned_rmw_subscription...}};}

        constexpr auto owned_publishers=owned_publisher_table(std::make_index_sequence<topic_shards>{});
        constexpr auto owned_subscriptions=owned_subscription_table(std::make_index_sequence<topic_shards>{});
        constexpr auto publishers=publish_table(std::make_index_sequence<topic_shards>{});
        constexpr auto subscribers=subscribe_table(std::make_index_sequence<topic_shards>{});
    }

    std::uint32_t dispatch_create_owned_rmw_publisher(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,std::size_t type_index,
        ros::capability_spec const& cap,ipc::rmw_endpoint::binding const& owner,std::uint64_t incarnation,
        std::shared_ptr<native_rmw_publisher::endpoint>& output,ipc::rmw_endpoint::packet& observed) noexcept
    {
        if(type_index>=topic_count){return 95;}
        return owned_publishers[type_index%topic_shards](node,type_index,cap,owner,incarnation,output,observed);
    }

    std::uint32_t dispatch_create_owned_rmw_subscription(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,std::size_t type_index,
        ros::capability_spec const& cap,ipc::rmw_endpoint::binding const& owner,std::uint64_t incarnation,
        std::shared_ptr<native_rmw_subscription::endpoint>& output,ipc::rmw_endpoint::packet& observed) noexcept
    {
        if(type_index>=topic_count){return 95;}
        return owned_subscriptions[type_index%topic_shards](node,type_index,cap,owner,incarnation,output,observed);
    }

    ipc::call_result dispatch_publish(adapter& selected,std::size_t type_index,
        ros::capability_spec const& cap,std::vector<std::uint8_t> const& data)
    {
        if(type_index>=topic_count) { return {95,{}}; }
        return publishers[type_index%topic_shards](selected,type_index,cap,data);
    }

    ipc::call_result dispatch_subscribe(adapter& selected,std::size_t type_index,
        ros::capability_spec const& cap,std::shared_ptr<event_queue> queue)
    {
        if(type_index>=topic_count) { return {95,{}}; }
        return subscribers[type_index%topic_shards](selected,type_index,cap,std::move(queue));
    }
}
