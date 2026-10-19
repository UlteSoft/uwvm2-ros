#pragma once
#include "node.h"
#include "action_client.h"
#include <type_traits>
#include <cstddef>

namespace uwvm2::ros_gateway::ros2_compat
{
    /** Compile/link probe of public core interfaces without a tutorial type.
     *
     * This does not qualify a concrete Action. Every selected custom Action
     * must separately instantiate the complete typed lifecycle below. CMake
     * only links these functions; no probe executable is run or ROS initialized.
     */
    inline void action_core_probe(std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> const& node)
    {
        auto options{::rcl_action_client_get_default_options()};
        options.goal_service_qos=rmw_qos_profile_services_default;
        options.result_service_qos=rmw_qos_profile_services_default;
        options.cancel_service_qos=rmw_qos_profile_services_default;
        options.feedback_topic_qos=rmw_qos_profile_default;
        options.status_topic_qos=rmw_qos_profile_default;
        auto server{::rcl_action_server_get_default_options()};
        server.goal_service_qos=options.goal_service_qos;
        server.result_service_qos=options.result_service_qos;
        server.cancel_service_qos=options.cancel_service_qos;
        server.feedback_topic_qos=options.feedback_topic_qos;
        server.status_topic_qos=options.status_topic_qos;
        server.result_timeout.nanoseconds=1000000;
        auto waitables{node->get_node_waitables_interface()};
        auto empty=std::shared_ptr<::rclcpp::Waitable>{};
        waitables->add_waitable(empty,nullptr);waitables->remove_waitable(empty,nullptr);
        // A volatile member-pointer reference retains a genuine native Action
        // library symbol, rather than a header-only sizeof capability claim.
        bool (::rclcpp_action::ClientBase::* volatile ready)() const=&::rclcpp_action::ClientBase::action_server_is_ready;
        (void)ready;(void)server;
    }

    /** Instantiate client/server construction, five exact QoS channels, public
     * registration and destruction for an actual selected official Action.
     * No invented Action message or typesupport implementation is supplied.
     */
    template<class Action> void action_lifecycle_probe(std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> const& node)
    {
        using Goal=typename Action::Goal;
        using ServerGoal=::rclcpp_action::ServerGoalHandle<Action>;
        auto options{::rcl_action_client_get_default_options()};
        options.goal_service_qos=rmw_qos_profile_services_default;
        options.result_service_qos=rmw_qos_profile_services_default;
        options.cancel_service_qos=rmw_qos_profile_services_default;
        options.feedback_topic_qos=rmw_qos_profile_default;
        options.status_topic_qos=rmw_qos_profile_default;
        auto client{create_action_client<Action>(node,"/compile_only",options)};
        auto server_options{::rcl_action_server_get_default_options()};
        server_options.goal_service_qos=options.goal_service_qos;
        server_options.result_service_qos=options.result_service_qos;
        server_options.cancel_service_qos=options.cancel_service_qos;
        server_options.feedback_topic_qos=options.feedback_topic_qos;
        server_options.status_topic_qos=options.status_topic_qos;
        server_options.result_timeout.nanoseconds=1000000;
        auto server=::rclcpp_action::create_server<Action>(node,"/compile_only",
            [](::rclcpp_action::GoalUUID const&,std::shared_ptr<Goal const>){return ::rclcpp_action::GoalResponse::REJECT;},
            [](std::shared_ptr<ServerGoal>){return ::rclcpp_action::CancelResponse::REJECT;},
            [](std::shared_ptr<ServerGoal>){},server_options);
        client.reset();server.reset();
    }

    /** Check the trusted generated type row against all three concrete native
     * component types before instantiating its complete public Action route.
     */
    template<class Action,class GoalCodec,class ResultCodec,class FeedbackCodec>
    void custom_action_lifecycle_probe(std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> const& node)
    {
        static_assert(std::is_same<typename Action::Goal,typename GoalCodec::Native>::value,"actual custom Goal codec type mismatch");
        static_assert(std::is_same<typename Action::Result,typename ResultCodec::Native>::value,"actual custom Result codec type mismatch");
        static_assert(std::is_same<typename Action::Feedback,typename FeedbackCodec::Native>::value,"actual custom Feedback codec type mismatch");
        action_lifecycle_probe<Action>(node);
    }
}
