#pragma once
#include "action_client_lease.h"
#include <rclcpp_action/rclcpp_action.hpp>

namespace uwvm2::ros_gateway::ros2_compat
{
    /** Prefer the installed four-argument factory, preserving all five QoS
     * channels. Foxy's three-argument factory cannot express these options;
     * use its public typed constructor and waitable registration instead.
     * There is deliberately no default-QoS or null-success fallback. CMake
     * instantiates and links the complete selected route before enabling it.
     */
    template<class Action,class Node>
    auto create_action_client_impl(Node const& node,std::string const& name,
                                  rcl_action_client_options_t const& options,int)
        -> decltype(::rclcpp_action::create_client<Action>(node,name,nullptr,options))
    { return ::rclcpp_action::create_client<Action>(node,name,nullptr,options); }
    template<class Action,class Node>
    auto create_action_client_impl(Node const& node,std::string const& name,
                                  rcl_action_client_options_t const& options,long)
        -> decltype(legacy_action_client<::rclcpp_action::Client<Action>>(node,name,options))
    { return legacy_action_client<::rclcpp_action::Client<Action>>(node,name,options); }
    template<class Action,class Node>
    auto create_action_client(Node const& node,std::string const& name,rcl_action_client_options_t const& options)
        -> decltype(create_action_client_impl<Action>(node,name,options,0))
    { return create_action_client_impl<Action>(node,name,options,0); }
}
