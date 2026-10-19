#pragma once
#include "api.h"
#include <rclcpp/any_service_callback.hpp>
#include <rclcpp/service.hpp>
#include <rcl/service.h>

namespace uwvm2::ros_gateway::ros2_compat
{
    /* Foxy's public Service<T>::handle_request is virtual and normally sends
     * an automatic response after a synchronous callback. Override dispatch,
     * not serialization or rcl ownership: no response is sent until the SDK's
     * checked service.respond operation calls the retained typed send_response.
     * Source: ros2/rclcpp, foxy, rclcpp/include/rclcpp/service.hpp:158,310-335.
     * The existing bounded service_state owns admission/deadlines and payloads;
     * this class never waits, creates a thread or reenters a Wasm callback. */
    template<class Service,class Callback> class legacy_deferred_service final : public ::rclcpp::Service<Service>
    {
        Callback callback_;
        std::weak_ptr<::rclcpp::Service<Service>> self_;
    public:
        legacy_deferred_service(std::shared_ptr<rcl_node_t> node,std::string const& name,
            rcl_service_options_t& options,Callback callback):
            ::rclcpp::Service<Service>{std::move(node),name,::rclcpp::AnyServiceCallback<Service>{},options},callback_{std::move(callback)} {}
        void bind(std::shared_ptr<::rclcpp::Service<Service>> const& self) { self_=self; }
        void handle_request(std::shared_ptr<rmw_request_id_t> header,std::shared_ptr<void> request) override
        {
            auto self{self_.lock()};
            if(self&&header&&request) { callback_(std::move(self),std::move(header),std::static_pointer_cast<typename Service::Request>(request)); }
        }
    };
    template<class Service,class Node,class Callback>
    std::shared_ptr<::rclcpp::Service<Service>> create_deferred_service(Node const& node,std::string const& name,Callback callback,qos_type const& qos)
    {
#if UWVM_ROS2_HAVE_DEFERRED_SERVICE
        return create_service<Service>(node,name,std::move(callback),qos);
#elif UWVM_ROS2_HAVE_LEGACY_DEFERRED_SERVICE
        auto options{rcl_service_get_default_options()};options.qos=profile(qos);
        auto entity{std::make_shared<legacy_deferred_service<Service,Callback>>(
            node->get_node_base_interface()->get_shared_rcl_node_handle(),name,options,std::move(callback))};
        entity->bind(entity);
        node->get_node_services_interface()->add_service(entity,nullptr);
        return entity;
#else
        (void)node;(void)name;(void)callback;(void)qos;
        throw std::runtime_error("selected native SDK has no safe deferred-service contract");
#endif
    }
}
