/*************************************************************
 * UWVM2 original ROS2 custom-Service untyped dispatch.       *
 * APL-2.0. No native typed endpoint definitions in this TU.  *
 *************************************************************/
#include "custom_services.h"
#include <array>
#include <utility>

// Do not include custom_services_impl.h here or in main. These actual address
// tables require symbols from EVERY selected shard. An omitted object fails
// actual linking rather than silently projecting the trusted provider table.
namespace uwvm2::ros_gateway::ros2_custom_services::detail
{
    namespace
    {
        using known_function=bool(*)(std::size_t,ros::capability_spec const&);
        using call_function=ipc::call_result(*)(adapter&,std::size_t,ros::capability_spec const&,
            std::vector<std::uint8_t> const&,std::uint32_t);
        using advertise_function=ipc::call_result(*)(adapter&,std::size_t,ros::capability_spec const&,
            std::shared_ptr<event_queue>);
        template<std::size_t... Index>
        constexpr std::array<known_function,sizeof...(Index)> known_table(std::index_sequence<Index...>)
        {return {{&service_shard<Index,service_shards>::known...}};}
        template<std::size_t... Index>
        constexpr std::array<call_function,sizeof...(Index)> call_table(std::index_sequence<Index...>)
        {return {{&service_shard<Index,service_shards>::call...}};}
        template<std::size_t... Index>
        constexpr std::array<advertise_function,sizeof...(Index)> advertise_table(std::index_sequence<Index...>)
        {return {{&service_shard<Index,service_shards>::advertise...}};}
        auto const knowns=known_table(std::make_index_sequence<service_shards>{});
        auto const calls=call_table(std::make_index_sequence<service_shards>{});
        auto const advertisements=advertise_table(std::make_index_sequence<service_shards>{});
    }
    bool dispatch_known(std::size_t type_index,ros::capability_spec const& cap)
    {
        if(type_index>=service_count){return false;}
        return knowns[type_index%service_shards](type_index,cap);
    }
    ipc::call_result dispatch_call(adapter& selected,std::size_t type_index,ros::capability_spec const& cap,
        std::vector<std::uint8_t> const& bytes,std::uint32_t timeout)
    {
        if(type_index>=service_count){return {95,{}};}
        return calls[type_index%service_shards](selected,type_index,cap,bytes,timeout);
    }
    ipc::call_result dispatch_advertise(adapter& selected,std::size_t type_index,ros::capability_spec const& cap,
        std::shared_ptr<event_queue> events)
    {
        if(type_index>=service_count){return {95,{}};}
        return advertisements[type_index%service_shards](selected,type_index,cap,std::move(events));
    }
}
