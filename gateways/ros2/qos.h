#pragma once
#include "../common/config.h"
#include "compat/api.h"
#if UWVM_ROS2_HAVE_COMPLETE_RMW_QOS
#include <rmw/time.h>
#endif

namespace uwvm2::ros_gateway::ros2_qos
{
#if UWVM_ROS2_HAVE_COMPLETE_RMW_QOS
    inline rmw_time_t duration(std::string const& value)
    {
        if(value=="infinite") { return RMW_DURATION_INFINITE; }
        auto ns{integer(value,86400000000000ULL)};
        return {ns/1000000000ULL,ns%1000000000ULL};
    }
#endif
    inline ros2_compat::qos_type configured(ros::capability_spec const& cap)
    {
        rmw_qos_profile_t profile{};
        profile.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;
        profile.depth=limit(cap,"qos.depth");
        profile.reliability=field(cap,"qos.reliability")=="reliable"?RMW_QOS_POLICY_RELIABILITY_RELIABLE:RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT;
        profile.durability=field(cap,"qos.durability")=="volatile"?RMW_QOS_POLICY_DURABILITY_VOLATILE:RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL;
#if UWVM_ROS2_HAVE_COMPLETE_RMW_QOS
        profile.deadline=duration(field(cap,"qos.deadline"));
        profile.lifespan=duration(field(cap,"qos.lifespan"));
        profile.liveliness=field(cap,"qos.liveliness")=="automatic"?RMW_QOS_POLICY_LIVELINESS_AUTOMATIC:RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC;
        profile.liveliness_lease_duration=duration(field(cap,"qos.liveliness-lease-duration"));
        profile.avoid_ros_namespace_conventions=false;
        return ros2_compat::make_qos(profile);
#else
        throw std::runtime_error("native SDK cannot represent the complete explicit QoS contract");
#endif
    }
    // Older actual getters return a raw rmw profile; newer getters return QoS.
    // Normalize observed values without substituting requested settings.
    inline rmw_qos_profile_t const& actual_profile(rmw_qos_profile_t const& value) noexcept {return value;}
    template<class T> auto actual_profile(T const& value) -> decltype(value.get_rmw_qos_profile())
    {return value.get_rmw_qos_profile();}
    template<class Actual,class Expected> bool equal(Actual const& actual,Expected const& expected)
    {
        auto a{actual_profile(actual)},e{actual_profile(expected)};
#if UWVM_ROS2_HAVE_COMPLETE_RMW_QOS
        // The actual RMW utility defines nonnormalized and saturated-infinite
        // equality. Field equality rejects legitimate equivalent observations.
        auto same=[](rmw_time_t x,rmw_time_t y){return rmw_time_equal(x,y);};
        return a.history==e.history&&a.depth==e.depth&&a.reliability==e.reliability&&a.durability==e.durability&&same(a.deadline,e.deadline)&&same(a.lifespan,e.lifespan)&&a.liveliness==e.liveliness&&same(a.liveliness_lease_duration,e.liveliness_lease_duration)&&a.avoid_ros_namespace_conventions==e.avoid_ros_namespace_conventions;
#else
        (void)a;(void)e;return false;
#endif
    }
    template<class T,class=void> struct supports_entity_qos : std::false_type {};
    template<class T> struct supports_entity_qos<T,std::void_t<decltype(std::declval<T const&>().get_actual_qos())>> : std::true_type {};
    template<class T,class=void> struct supports_client_qos : std::false_type {};
    template<class T> struct supports_client_qos<T,std::void_t<decltype(std::declval<T const&>().get_request_publisher_actual_qos()),decltype(std::declval<T const&>().get_response_subscription_actual_qos())>> : std::true_type {};
    template<class T,class=void> struct supports_service_qos : std::false_type {};
    template<class T> struct supports_service_qos<T,std::void_t<decltype(std::declval<T const&>().get_request_subscription_actual_qos()),decltype(std::declval<T const&>().get_response_publisher_actual_qos())>> : std::true_type {};
    template<class Entity> auto entity_impl(Entity const& value,ros2_compat::qos_type const& expected,int)
        -> decltype(value.get_actual_qos(),bool())
    { try { return equal(value.get_actual_qos(),expected); }catch(std::exception const&) { return false; } }
    template<class Entity> bool entity_impl(Entity const&,ros2_compat::qos_type const&,long) noexcept { return false; }
    template<class Entity> bool entity(Entity const& value,ros2_compat::qos_type const& expected)
    { return entity_impl(value,expected,0); }
    template<class Client> auto client_impl(Client const& client,ros2_compat::qos_type const& expected,int)
        -> decltype(client.get_request_publisher_actual_qos(),client.get_response_subscription_actual_qos(),bool())
    {
        try { return equal(client.get_request_publisher_actual_qos(),expected)&&equal(client.get_response_subscription_actual_qos(),expected); }
        catch(std::exception const&) { return false; }
    }
    template<class Client> bool client_impl(Client const&,ros2_compat::qos_type const&,long) noexcept { return false; }
    template<class Client> bool client(Client const& value,ros2_compat::qos_type const& expected)
    { return client_impl(value,expected,0); }
    template<class Service> auto service_impl(Service const& service,ros2_compat::qos_type const& expected,int)
        -> decltype(service.get_request_subscription_actual_qos(),service.get_response_publisher_actual_qos(),bool())
    {
        try { return equal(service.get_request_subscription_actual_qos(),expected)&&equal(service.get_response_publisher_actual_qos(),expected); }
        catch(std::exception const&) { return false; }
    }
    // Missing actual-QoS APIs are an unavailable capability, never evidence
    // that the requested profile was actually applied by the selected RMW.
    template<class Service> bool service_impl(Service const&,ros2_compat::qos_type const&,long) noexcept { return false; }
    template<class Service> bool service(Service const& value,ros2_compat::qos_type const& expected)
    { return service_impl(value,expected,0); }
}
