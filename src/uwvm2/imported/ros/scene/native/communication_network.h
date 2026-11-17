/** @file
 * @brief Explicit loopback-only transport for a dedicated native ROS worker.
 * @details Installation must precede every ROS context. It fixes the middleware,
 * removes inherited discovery peers and configuration files, and installs one
 * compile-time immutable FastDDS profile through its checked parser with builtin transports disabled. Only UDPv4
 * on 127.0.0.1 is configured; no physical interface, IPv6, TCP or shared-memory
 * transport is enabled. Domain numbers separate names and are not authentication.
 * This profile expressly permits unauthenticated peers on the same host. Remote
 * peers and authenticated deployments need a separately qualified transport.
 * No guest controls these strings. RMW SYSTEM_DEFAULT is intentional: Jazzy
 * LOCALHOST injects extra transports outside the XML whitelist. The XML profile
 * is parsed directly because FastDDS realpath rejects Linux memfd paths. This process-wide
 * policy cannot be installed into a process already hosting initialized ROS.
 */
#pragma once
#include "external_process_only.h"
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
#include <rclcpp/context.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/rtps/transport/UDPv4TransportDescriptor.h>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <string>
#if defined(__linux__)
# include <fcntl.h>
# include <sys/mman.h>
# include <unistd.h>
#endif

namespace uwvm2::imported::ros::scene::communication::ros2
{
    /** @brief Closed trusted transport installation checked before context creation. */
    class network_policy
    {
    public:
        virtual ~network_policy()=default;
        virtual bool valid() const noexcept=0;
        virtual bool permits_domain(std::uint32_t) const noexcept{return valid();}
    };
    class local_network : public network_policy
    {
        bool installed_policy{};
#if defined(__linux__)
        ::pid_t process{::getpid()};
#endif
        inline static constexpr char xml[] = R"xml(<?xml version="1.0" encoding="UTF-8"?>
<profiles xmlns="http://www.eprosima.com/XMLSchemas/fastRTPS_Profiles">
 <transport_descriptors><transport_descriptor>
  <transport_id>scene_loopback</transport_id><type>UDPv4</type>
  <interfaceWhiteList><address>127.0.0.1</address></interfaceWhiteList>
 </transport_descriptor></transport_descriptors>
 <participant profile_name="scene_loopback" is_default_profile="true"><rtps>
  <userTransports><transport_id>scene_loopback</transport_id></userTransports>
  <useBuiltinTransports>false</useBuiltinTransports>
  <defaultUnicastLocatorList><locator><udpv4><address>127.0.0.1</address></udpv4></locator></defaultUnicastLocatorList>
  <builtin>
   <metatrafficUnicastLocatorList><locator><udpv4><address>127.0.0.1</address></udpv4></locator></metatrafficUnicastLocatorList>
   <initialPeersList><locator><udpv4><address>127.0.0.1</address></udpv4></locator></initialPeersList>
  </builtin>
 </rtps></participant>
</profiles>)xml";

        static bool equal(char const* key, char const* value) noexcept
        {
            auto actual = std::getenv(key);
            return actual && std::strcmp(actual, value) == 0;
        }

    public:
        local_network() = default;
        ~local_network() = default;
        local_network(local_network const&) = delete;

        bool valid() const noexcept override
        {
#if defined(__linux__)
            if(process != ::getpid()) { return false; }
            if(!installed_policy) { return false; }
            auto const& qos = eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->get_default_participant_qos();
            if(qos.transport().use_builtin_transports || qos.transport().user_transports.size() != 1) { return false; }
            auto udp = std::dynamic_pointer_cast<eprosima::fastdds::rtps::UDPv4TransportDescriptor>(qos.transport().user_transports[0]);
            if(!udp || udp->interfaceWhiteList.size() != 1 || udp->interfaceWhiteList[0] != "127.0.0.1") { return false; }
            return equal("RMW_IMPLEMENTATION", "rmw_fastrtps_cpp") && !std::getenv("FASTDDS_DEFAULT_PROFILES_FILE") &&
                   !std::getenv("FASTRTPS_DEFAULT_PROFILES_FILE") && equal("ROS_AUTOMATIC_DISCOVERY_RANGE", "SYSTEM_DEFAULT") &&
                   equal("SKIP_DEFAULT_XML", "1") && equal("RMW_FASTRTPS_PUBLICATION_MODE", "SYNCHRONOUS") && !std::getenv("ROS_STATIC_PEERS") &&
                   !std::getenv("ROS_DISCOVERY_SERVER") && !std::getenv("ROS_SUPER_CLIENT") && !std::getenv("FASTDDS_ENVIRONMENT_FILE") &&
                   !std::getenv("FASTDDS_BUILTIN_TRANSPORTS") && !std::getenv("RMW_FASTRTPS_USE_QOS_FROM_XML") && !std::getenv("ROS_SECURITY_ENABLE") &&
                   !std::getenv("ROS_SECURITY_STRATEGY") && !std::getenv("ROS_SECURITY_KEYSTORE") && !std::getenv("ROS_SECURITY_ENCLAVE_OVERRIDE");
#else
            return false;
#endif
        }

        /** @brief Explicit trusted startup; a false argument leaves all state alone. */
        static std::shared_ptr<local_network> install(bool permit_unauthenticated_loopback)
        {
            if(!permit_unauthenticated_loopback) { return {}; }
#if defined(__linux__)
            static std::mutex serial;
            static std::shared_ptr<local_network> installed;
            std::lock_guard lock{serial};
            if(installed) { return installed->valid() ? installed : nullptr; }
            for(auto const& context: rclcpp::get_contexts())
            {
                if(context && context->is_valid()) { return {}; }
            }
            if((std::getenv("RMW_IMPLEMENTATION") && !equal("RMW_IMPLEMENTATION", "rmw_fastrtps_cpp")) ||
               (std::getenv("ROS_AUTOMATIC_DISCOVERY_RANGE") && !equal("ROS_AUTOMATIC_DISCOVERY_RANGE", "SYSTEM_DEFAULT")) ||
               (std::getenv("SKIP_DEFAULT_XML") && !equal("SKIP_DEFAULT_XML", "1")) ||
               (std::getenv("RMW_FASTRTPS_PUBLICATION_MODE") && !equal("RMW_FASTRTPS_PUBLICATION_MODE", "SYNCHRONOUS")))
            {
                return {};
            }
            // Existing configuration is an explicit conflict, never silently
            // downgraded from authenticated/remote ROS to this local profile.
            for(auto key: {"FASTDDS_DEFAULT_PROFILES_FILE",
                           "FASTRTPS_DEFAULT_PROFILES_FILE",
                           "ROS_STATIC_PEERS",
                           "ROS_DISCOVERY_SERVER",
                           "ROS_SUPER_CLIENT",
                           "FASTDDS_ENVIRONMENT_FILE",
                           "FASTDDS_BUILTIN_TRANSPORTS",
                           "ROS_SECURITY_ENABLE",
                           "ROS_SECURITY_STRATEGY",
                           "ROS_SECURITY_KEYSTORE",
                           "ROS_SECURITY_ENCLAVE_OVERRIDE",
                           "ROS_LOCALHOST_ONLY",
                           "RMW_FASTRTPS_USE_QOS_FROM_XML"})
            {
                if(std::getenv(key)) { return {}; }
            }
            auto value = ::uwvm2::utils::container::make_shared<local_network>();
            auto factory = eprosima::fastdds::dds::DomainParticipantFactory::get_instance();
            for(unsigned domain{}; domain <= 232; ++domain)
            {
                if(!factory->lookup_participants(domain).empty()) { return {}; }
            }
            if(factory->load_XML_profiles_string(xml, sizeof(xml) - 1) != eprosima::fastrtps::types::ReturnCode_t::RETCODE_OK) { return {}; }
            for(auto key: {"ROS_STATIC_PEERS",
                           "ROS_DISCOVERY_SERVER",
                           "ROS_SUPER_CLIENT",
                           "FASTDDS_ENVIRONMENT_FILE",
                           "FASTDDS_BUILTIN_TRANSPORTS",
                           "ROS_SECURITY_ENABLE",
                           "ROS_SECURITY_STRATEGY",
                           "ROS_SECURITY_KEYSTORE",
                           "ROS_SECURITY_ENCLAVE_OVERRIDE",
                           "ROS_LOCALHOST_ONLY",
                           "RMW_FASTRTPS_USE_QOS_FROM_XML"})
            {
                if(::unsetenv(key)) { return {}; }
            }
            if(::setenv("RMW_IMPLEMENTATION", "rmw_fastrtps_cpp", 1) || ::setenv("ROS_AUTOMATIC_DISCOVERY_RANGE", "SYSTEM_DEFAULT", 1) ||
               ::setenv("SKIP_DEFAULT_XML", "1", 1) || ::setenv("RMW_FASTRTPS_PUBLICATION_MODE", "SYNCHRONOUS", 1))
            {
                return {};
            }
            factory->load_profiles();
            auto native_policy = factory->get_default_participant_qos();
            native_policy.wire_protocol().builtin.metatrafficMulticastLocatorList.clear();
            if(factory->set_default_participant_qos(native_policy) != eprosima::fastrtps::types::ReturnCode_t::RETCODE_OK) { return {}; }
            value->installed_policy = true;
            if(!value->valid()) { return {}; }
            installed = value;
            return value;
#else
            return {};
#endif
        }
    };
}  // namespace uwvm2::imported::ros::scene::communication::ros2
