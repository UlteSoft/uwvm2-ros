/* SPDX-License-Identifier: Apache-2.0 */
/** @file Finite canonical service storage admission, without ROS effects.
 * The trusted provider must independently prove both complete native payload
 * graphs and canonical codecs before supplying these sizes. This arithmetic
 * neither selects a codec nor authorizes a service. A history slot reserves
 * one full canonical payload and one original UWRE header; a separate control
 * slot must remain available. URMI/native objects are additional owner-budgeted
 * storage and must never be charged as invented Guest traffic.
 */
#pragma once
#include <cstddef>
#include <cstdint>
#include <limits>
namespace uwvm2::ros_gateway::owned_service_storage
{
    inline constexpr std::uint64_t maximum_canonical_body=32768;
    inline constexpr std::uint64_t maximum_history=256;
    inline constexpr std::uint64_t maximum_separately_owned_native_depth=1000;
    struct bodies
    {
        std::size_t request{},response{};
        // Trusted selected-bounds metadata, never decoded from a capability or
        // Guest packet. Zero preserves the original history-coupled admission.
        // A proved provider may set a finite independent native DDS queue limit
        // only when its real native owner separately budgets that queue. This
        // is not a native receipt, authorization flag or allocation guarantee.
        // All callback providers/consumers must source-recompile this tail.
        std::uint64_t separately_owned_native_depth_limit{};
    };
    struct grant
    {
        std::uint64_t history{},queue_depth{},queue_bytes{},max_inflight{},qos_depth{};
        std::uint64_t max_request_bytes{},max_response_bytes{},observer_max_event_bytes{};
        bool client{};
    };
    /** The actual caller supplies ipc::event_header_bytes. Returning0 proves
     * only this finite storage calculation, never the upstream type contract.
     * All inputs are bounded/checked before multiplication or addition. */
    inline std::uint32_t admit(bodies const& body,grant const& policy,std::uint64_t header)noexcept
    {
        if(!header||!policy.history||policy.history>maximum_history||
            body.request>maximum_canonical_body||body.response>maximum_canonical_body)return 90;
        if(policy.max_request_bytes<body.request||policy.max_response_bytes<body.response||
            policy.queue_depth<=policy.history||policy.history<policy.max_inflight)return 90;
        if(body.separately_owned_native_depth_limit)
        {
            if(body.separately_owned_native_depth_limit>maximum_separately_owned_native_depth||
                !policy.qos_depth||policy.qos_depth>body.separately_owned_native_depth_limit)return 90;
        }
        else if(policy.history<policy.qos_depth)return 90;
        auto incoming=static_cast<std::uint64_t>(policy.client?body.response:body.request);
        constexpr auto maximum=std::numeric_limits<std::uint64_t>::max();
        if(incoming>maximum-header)return 90;
        auto item=header+incoming;
        if(policy.history>(maximum-header)/item)return 90;
        auto complete=policy.history*item+header;
        if(policy.queue_bytes<complete||(policy.client&&policy.observer_max_event_bytes<item))return 90;
        return 0;
    }
}
