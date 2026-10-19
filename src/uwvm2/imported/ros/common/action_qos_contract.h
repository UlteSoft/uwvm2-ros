/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once

#include <array>
#include "types.h"

namespace uwvm2::imported::ros
{
    inline constexpr ::std::array<::std::string_view, 40> action_server_qos_fields{
        "qos.goal-service.reliability", "qos.goal-service.durability",
        "qos.goal-service.history", "qos.goal-service.depth",
        "qos.goal-service.deadline", "qos.goal-service.lifespan",
        "qos.goal-service.liveliness", "qos.goal-service.liveliness-lease-duration",
        "qos.cancel-service.reliability", "qos.cancel-service.durability",
        "qos.cancel-service.history", "qos.cancel-service.depth",
        "qos.cancel-service.deadline", "qos.cancel-service.lifespan",
        "qos.cancel-service.liveliness", "qos.cancel-service.liveliness-lease-duration",
        "qos.result-service.reliability", "qos.result-service.durability",
        "qos.result-service.history", "qos.result-service.depth",
        "qos.result-service.deadline", "qos.result-service.lifespan",
        "qos.result-service.liveliness", "qos.result-service.liveliness-lease-duration",
        "qos.feedback.reliability", "qos.feedback.durability",
        "qos.feedback.history", "qos.feedback.depth",
        "qos.feedback.deadline", "qos.feedback.lifespan",
        "qos.feedback.liveliness", "qos.feedback.liveliness-lease-duration",
        "qos.status.reliability", "qos.status.durability",
        "qos.status.history", "qos.status.depth",
        "qos.status.deadline", "qos.status.lifespan",
        "qos.status.liveliness", "qos.status.liveliness-lease-duration"
    };

    inline bool equal_action_server_qos_contract(capability_spec const& server, capability_spec const& response)
    {
        // These are explicit trusted contracts, so use the same exact-text
        // semantics as service.advertise/respond, including decimal spelling.
        for(auto key : action_server_qos_fields)
        {
            auto server_field{server.limits.find(key)}, response_field{response.limits.find(key)};
            if(server_field == server.limits.end() || response_field == response.limits.end() || server_field->second != response_field->second)
            { return false; }
        }
        return true;
    }

    inline constexpr ::std::array<::std::string_view, 8> action_cancel_qos_fields{
        "qos.cancel-service.reliability", "qos.cancel-service.durability",
        "qos.cancel-service.history", "qos.cancel-service.depth",
        "qos.cancel-service.deadline", "qos.cancel-service.lifespan",
        "qos.cancel-service.liveliness", "qos.cancel-service.liveliness-lease-duration"
    };

    /**
     * Compare the endpoint contract reused by an outgoing Goal cancellation.
     *
     * The Goal's client was created by its send-goal capability. Cancellation
     * has independent authority, but cannot silently replace that client's
     * cancel-service endpoint. Compare all eight explicit fields before any
     * native cancellation. Other Action channels do not participate in this
     * operation. Missing fields fail closed, and decimal spelling retains the
     * exact-text semantics used by the server/respond contract above.
     *
     * Both capabilities must remain immutable for the Goal's lifetime.
     */
    inline bool equal_action_cancel_qos_contract(capability_spec const& goal, capability_spec const& cancellation)
    {
        for(auto key : action_cancel_qos_fields)
        {
            auto goal_field{goal.limits.find(key)}, cancellation_field{cancellation.limits.find(key)};
            if(goal_field == goal.limits.end() || cancellation_field == cancellation.limits.end() ||
               goal_field->second != cancellation_field->second)
            { return false; }
        }
        return true;
    }
}
