/* SPDX-License-Identifier: Apache-2.0 */
#pragma once
#include "compat/node.h"
#include "custom_topics.h"
#include "custom_owned_service_exports.h"
#include <uwvm_rmw/cpp_codecs.hpp>
#include <array>
#include <tuple>
#include <memory>
#include <string_view>
namespace uwvm2::ros_gateway::native_rcl_service {class report;}
namespace uwvm2::ros_gateway::native_rcl_service_registry {class endpoint;}
#ifndef UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES
#define UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(M)
#endif
#ifndef UWVM_ROS2_CUSTOM_SERVICE_SHARDS
#define UWVM_ROS2_CUSTOM_SERVICE_SHARDS 32
#endif
namespace uwvm2::ros_gateway::custom_owned_services
{
    namespace wire=::uwvm2::imported::ros::ipc::rmw_endpoint;
    namespace native=::uwvm2::ros_gateway::native_rcl_service;
    using endpoint=::uwvm2::ros_gateway::native_rcl_service_registry::endpoint;
    struct canonical_limits
    {std::size_t request{},response{};std::string_view type,native_identity,codec_identity;};
    /** Metadata proof only; no Node/entity/permission. A future registry MUST
     * explicitly use this exact companion instead of its fixed16 queue limits.
     * This source does not silently register or alter that integration seam. */
    std::uint32_t canonical_bounds(ros::capability_spec const&,canonical_limits&)noexcept;
    bool known(ros::capability_spec const&)noexcept;
    /** Complete canonical bytes only, against the SAME qualified row/root and
     * selected component descriptor. No Native object construction, endpoint,
     * owner, quota or output. Call during actual warmed profile lifetime.
     * 0 valid;22 malformed;95 unsupported;13 identity/selection;12 allocation. */
    std::uint32_t validate_body(ros::capability_spec const&,bool response,
        std::uint8_t const*,std::size_t)noexcept;
    /** Exact original registry factory signature. A source-compatible explicit
     * builtin fallback is allowed only when no compiled custom row matches.
     * The endpoint also implements the actual integrated positive-wait virtual
     * method. It BORROWS the same authenticated profile graph domain/lifetime;
     * this factory creates neither a second client nor a graph domain. */
    std::uint32_t create_selected(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&,
        std::shared_ptr<uwvm_rmw_cpp::Registry const> const&,ros::capability_spec const&,
        wire::binding const&,std::uint64_t,std::size_t,std::shared_ptr<native::report> const&,
        std::shared_ptr<endpoint>&,wire::packet&)noexcept;
    namespace detail
    {
        template<class N,class Q,class S>struct service_row{using Native=N;using Request=Q;using Response=S;};
        struct end_row{};
#define UWVM_OWNED_SERVICE_ROW(Native,Name,NativeId,CodecId,Request,Response) service_row<Native,Request,Response>,
        using rows=std::tuple<UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(UWVM_OWNED_SERVICE_ROW) end_row>;
#undef UWVM_OWNED_SERVICE_ROW
        inline constexpr std::size_t count=std::tuple_size<rows>::value-1;
        struct row_identity{std::string_view type,native,canonical;};
#define UWVM_OWNED_SERVICE_FACTS(Native,Name,NativeId,CodecId,Request,Response) row_identity{Name,NativeId,CodecId},
        inline constexpr row_identity identities[]{UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(UWVM_OWNED_SERVICE_FACTS) {}};
#undef UWVM_OWNED_SERVICE_FACTS
        static_assert(sizeof(identities)/sizeof(identities[0])==count+1,"complete original service macro facts");
        inline constexpr std::size_t shards=UWVM_ROS2_CUSTOM_SERVICE_SHARDS;
        static_assert(shards>=1&&shards<=256,"actual service partitions1..256");
        template<std::size_t Shard,std::size_t Shards>struct service_shard
        {
            static std::uint32_t bounds(std::size_t,ros::capability_spec const&,canonical_limits&)noexcept;
            static std::uint32_t validate(std::size_t,ros::capability_spec const&,bool,
                std::uint8_t const*,std::size_t)noexcept;
            static std::uint32_t create(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&,
                std::shared_ptr<uwvm_rmw_cpp::Registry const> const&,std::size_t,ros::capability_spec const&,
                wire::binding const&,std::uint64_t,std::size_t,std::shared_ptr<native::report> const&,
                std::shared_ptr<endpoint>&,wire::packet&)noexcept;
        };
    }
}
