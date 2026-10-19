/*************************************************************
 * UWVM2 ROS2 exact-node Lifecycle adapter. APL-2.0.           *
 *************************************************************/
#pragma once
#include "compat/node.h"
#include "../common/config.h"
#include "qos.h"
#include "compat/identity.h"
#if UWVM_ROS2_HAVE_LIFECYCLE
#include <rclcpp/rclcpp.hpp>
#include <lifecycle_msgs/srv/get_state.hpp>
#include <lifecycle_msgs/srv/change_state.hpp>
#include <lifecycle_msgs/msg/transition.hpp>
#include <lifecycle_msgs/msg/state.hpp>
#include <rosidl_typesupport_cpp/service_type_support.hpp>
#include <iomanip>

namespace uwvm2::ros_gateway::ros2_lifecycle
{
    template<typename Service> inline std::string identity()
    {
        return ros2_compat::baked_hash(rosidl_typesupport_cpp::get_service_type_support_handle<Service>());
    }
    inline bool known(ros::capability_spec const& cap)
    {
        if(cap.operation=="lifecycle.get-state")
        { return cap.type=="lifecycle_msgs/srv/GetState"&&ros2_compat::builtin_service<lifecycle_msgs::srv::GetState>(cap.type,cap.type_identity); }
        if(cap.operation=="lifecycle.change-state")
        { return cap.type=="lifecycle_msgs/srv/ChangeState"&&ros2_compat::builtin_service<lifecycle_msgs::srv::ChangeState>(cap.type,cap.type_identity); }
        return false;
    }
    template<typename Service> inline std::shared_ptr<typename Service::Response> invoke(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,ros::capability_spec const& cap,std::string const& resource,
        std::shared_ptr<typename Service::Request> request,std::chrono::steady_clock::time_point deadline,std::uint32_t& error)
    {
        error=110;
        auto remaining{std::chrono::duration_cast<std::chrono::nanoseconds>(deadline-std::chrono::steady_clock::now())};
        if(remaining.count()<=0) { return {}; }
        if constexpr(!ros2_qos::supports_client_qos<::rclcpp::Client<Service>>::value) { error=95;return {}; }
        auto configured{ros2_qos::configured(cap)};
        auto client{ros2_compat::create_client<Service>(node,resource,configured)};
        if(!ros2_qos::client(*client,configured)) { error=95;return {}; }
        remaining=std::chrono::duration_cast<std::chrono::nanoseconds>(deadline-std::chrono::steady_clock::now());
        if(remaining.count()<=0||!client->wait_for_service(remaining)) { return {}; }
        if(std::chrono::steady_clock::now()>=deadline) { return {}; }
        auto pending{client->async_send_request(request)};
        if(pending.wait_until(deadline)!=std::future_status::ready) { ros2_compat::remove_pending(*client,pending); return {}; }
        if(std::chrono::steady_clock::now()>=deadline) { return {}; }
        error=0;return pending.get();
    }
    inline ipc::call_result call(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,ros::capability_spec const& cap,std::vector<std::uint8_t> const& request,std::uint32_t timeout)
    {
        if(!known(cap)) { return {13,{}}; }
        if(!request.empty()) { return {22,{}}; }
        auto deadline{std::chrono::steady_clock::now()+std::chrono::milliseconds(std::min(timeout,static_cast<std::uint32_t>(limit(cap,"timeout-ms"))))};
        std::uint32_t error{};
        if(cap.operation=="lifecycle.get-state")
        {
            auto state{invoke<lifecycle_msgs::srv::GetState>(node,cap,cap.resource+"/get_state",std::make_shared<lifecycle_msgs::srv::GetState::Request>(),deadline,error)};
            if(!state) { return {error,{}}; }
            if(!wire_size_fits(limit(cap,"max-response-bytes"),{5,state->current_state.label.size()})) { return {90,{}}; }
            if(!valid_utf8(reinterpret_cast<std::uint8_t const*>(state->current_state.label.data()),state->current_state.label.size())) { return {22,{}}; }
            std::vector<std::uint8_t> data;data.reserve(5+state->current_state.label.size());data.push_back(state->current_state.id);ipc::put(data,state->current_state.label.size(),4);
            data.insert(data.end(),state->current_state.label.begin(),state->current_state.label.end());
            if(std::chrono::steady_clock::now()>=deadline) { return {110,{}}; }
            return {0,std::move(data)};
        }
        auto transition{std::make_shared<lifecycle_msgs::srv::ChangeState::Request>()};
        auto allowed{field(cap,"allowed-transition")};
        using t=lifecycle_msgs::msg::Transition;using s=lifecycle_msgs::msg::State;
        if(allowed=="configure") { transition->transition.id=t::TRANSITION_CONFIGURE; }
        else if(allowed=="cleanup") { transition->transition.id=t::TRANSITION_CLEANUP; }
        else if(allowed=="activate") { transition->transition.id=t::TRANSITION_ACTIVATE; }
        else if(allowed=="deactivate") { transition->transition.id=t::TRANSITION_DEACTIVATE; }
        else if(allowed=="shutdown")
        {
            // Internal read selects the native transition; it is not delivered to Guest.
            auto state{invoke<lifecycle_msgs::srv::GetState>(node,cap,cap.resource+"/get_state",std::make_shared<lifecycle_msgs::srv::GetState::Request>(),deadline,error)};
            if(!state) { return {error,{}}; }
            if(state->current_state.id==s::PRIMARY_STATE_UNCONFIGURED) { transition->transition.id=t::TRANSITION_UNCONFIGURED_SHUTDOWN; }
            else if(state->current_state.id==s::PRIMARY_STATE_INACTIVE) { transition->transition.id=t::TRANSITION_INACTIVE_SHUTDOWN; }
            else if(state->current_state.id==s::PRIMARY_STATE_ACTIVE) { transition->transition.id=t::TRANSITION_ACTIVE_SHUTDOWN; }
            else { return {17,{}}; }
        }
        else { return {22,{}}; }
        auto result{invoke<lifecycle_msgs::srv::ChangeState>(node,cap,cap.resource+"/change_state",transition,deadline,error)};
        if(!result) { return {error,{}}; }
        if(limit(cap,"max-response-bytes")<1) { return {90,{}}; }
        std::vector<std::uint8_t> data{static_cast<std::uint8_t>(result->success?1:0)};
        if(std::chrono::steady_clock::now()>=deadline) { return {110,{}}; }
        return {0,std::move(data)};
    }
}
#else
namespace uwvm2::ros_gateway::ros2_lifecycle
{
    inline bool known(ros::capability_spec const&) { return false; }
    inline ipc::call_result call(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&,ros::capability_spec const&,std::vector<std::uint8_t> const&,std::uint32_t) { return {95,{}}; }
}
#endif
