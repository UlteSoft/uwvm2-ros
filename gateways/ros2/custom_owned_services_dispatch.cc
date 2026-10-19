#include "compat/node.h"
/* SPDX-License-Identifier: Apache-2.0 */
#include "custom_owned_services.h"
#include "native_rcl_service_registry.h"
#include <utility>
// NO impl/endpoint/legacy custom_services.h include. Every actual shard address
// is required. Omitting a shard is a real undefined-symbol link failure even
// when that partition happens to have no selected rows.
namespace uwvm2::ros_gateway::custom_owned_services
{
    namespace
    {
        using bounds_function=std::uint32_t(*)(std::size_t,ros::capability_spec const&,canonical_limits&)noexcept;
        using validate_function=std::uint32_t(*)(std::size_t,ros::capability_spec const&,bool,std::uint8_t const*,std::size_t)noexcept;
        using create_function=std::uint32_t(*)(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&,std::shared_ptr<uwvm_rmw_cpp::Registry const> const&,
            std::size_t,ros::capability_spec const&,wire::binding const&,std::uint64_t,std::size_t,std::shared_ptr<native::report> const&,std::shared_ptr<endpoint>&,wire::packet&)noexcept;
        template<std::size_t... I>constexpr auto bound_table(std::index_sequence<I...>){return std::array<bounds_function,sizeof...(I)>{{&detail::service_shard<I,detail::shards>::bounds...}};}
        template<std::size_t... I>constexpr auto validate_table(std::index_sequence<I...>){return std::array<validate_function,sizeof...(I)>{{&detail::service_shard<I,detail::shards>::validate...}};}
        template<std::size_t... I>constexpr auto create_table(std::index_sequence<I...>){return std::array<create_function,sizeof...(I)>{{&detail::service_shard<I,detail::shards>::create...}};}
        constexpr auto bounds=bound_table(std::make_index_sequence<detail::shards>{});
        constexpr auto validators=validate_table(std::make_index_sequence<detail::shards>{});
        constexpr auto factories=create_table(std::make_index_sequence<detail::shards>{});
        std::size_t selected(ros::capability_spec const& cap)noexcept
        {std::size_t index=detail::count,matches{};for(std::size_t i{};i<detail::count;++i)if(cap.type==detail::identities[i].type){index=i;++matches;}return matches==1?index:detail::count;}
    }
    std::uint32_t canonical_bounds(ros::capability_spec const& cap,canonical_limits& output)noexcept
    {auto index=selected(cap);if(index==detail::count)return 95;return bounds[index%detail::shards](index,cap,output);}
    bool known(ros::capability_spec const& cap)noexcept{canonical_limits limits;return canonical_bounds(cap,limits)==0;}
    std::uint32_t validate_body(ros::capability_spec const& cap,bool response,std::uint8_t const* bytes,std::size_t size)noexcept
    {
        std::size_t index=detail::count,matches{};
        for(std::size_t i{};i<detail::count;++i)if(cap.type==detail::identities[i].type){index=i;++matches;}
        if(matches>1)return 13;if(!matches)return 95;
        return validators[index%detail::shards](index,cap,response,bytes,size);
    }
    std::uint32_t create_selected(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,
        std::shared_ptr<uwvm_rmw_cpp::Registry const> const& original,ros::capability_spec const& cap,
        wire::binding const& owner,std::uint64_t incarnation,std::size_t history,std::shared_ptr<native::report> const& report,
        std::shared_ptr<endpoint>& output,wire::packet& observed)noexcept
    {
        std::size_t index=detail::count,matches{};for(std::size_t i{};i<detail::count;++i)if(cap.type==detail::identities[i].type){index=i;++matches;}
        if(matches>1)return 13;
        if(matches==1)return factories[index%detail::shards](node,original,index,cap,owner,incarnation,history,report,output,observed);
        try{return native_rcl_service_registry::create_fixed(node,original,cap,owner,incarnation,history,report,output,observed);}
        catch(std::bad_alloc const&){return 12;}catch(...){return 5;}
    }
}
