/* SPDX-License-Identifier: Apache-2.0 */
/** Opt-in original SDK identity/storage selection, NOT a capability policy.
 * No identities, grants, native client, default Node or fixture are installed
 * when this profile is absent. Root must separately qualify its selected SDK
 * and independently exported hashes; a trusted text file is not that proof.
 */
#pragma once
#include "compat/node.h"
#include "../common/native_io.h"
#include "native_rcl_service_registry.h"
#include <set>
#include <algorithm>
#include <array>
#ifndef UWVM_ROS2_OWNED_CUSTOM_RCL_SERVICE_PROFILE
#define UWVM_ROS2_OWNED_CUSTOM_RCL_SERVICE_PROFILE 0
#endif
#if UWVM_ROS2_OWNED_CUSTOM_RCL_SERVICE_PROFILE
#include "custom_owned_services.h"
#endif

namespace uwvm2::ros_gateway::owned_service_profile
{
    struct selection
    {
        uwvm_rmw_cpp::NativeIdentities identities{};
        native_rcl_service_registry::limits storage{};
        std::size_t maximum_string_wire{};
        std::shared_ptr<uwvm_rmw_cpp::Registry const> registry;
    };
    inline constexpr std::array<std::string_view,11> identity_names{
        "string","time","event_info","empty_request","empty_response","empty_event",
        "add_request","add_response","add_event","empty_service","add_service"};
    static_assert(identity_names.size()==static_cast<std::size_t>(uwvm_rmw_cpp::Identity::count));
    inline bool hash(std::string_view text,uwvm_rmw_cpp::Hash& out) noexcept
    {
        if(text.size()!=64)return false;uwvm_rmw_cpp::Hash next{};bool nonzero{};
        auto nibble=[](char c)->int{return c>='0'&&c<='9'?c-'0':c>='a'&&c<='f'?c-'a'+10:-1;};
        for(std::size_t i{};i<next.size();++i){auto hi=nibble(text[2*i]),lo=nibble(text[2*i+1]);if(hi<0||lo<0)return false;
            next[i]=static_cast<std::uint8_t>((hi<<4)|lo);nonzero|=next[i]!=0;}
        if(!nonzero)return false;out=next;return true;
    }
    inline selection load(std::string const& path)
    {
#if !defined(__linux__)
        (void)path;throw std::runtime_error("Owned RCL profile requires actual Linux no-follow trusted-file admission");
#else
        // config_lines opens NOFOLLOW/NONBLOCK before fstat, requires an owned
        // ordinary file with safe mode, and has fixed4KiB buffers. Bound even
        // comment/empty lines and total read text; no file-sized allocation.
        native_io::config_lines input{path};std::string_view line;selection result;
        std::set<std::string> seen;unsigned lines{};std::size_t bytes{};
        while(input.next(line))
        {
            if(++lines>64||line.size()>256||bytes>8192-line.size())throw std::runtime_error("Owned service profile exceeds finite text bound");
            bytes+=line.size();if(line.empty()||line.front()=='#')continue;
            std::array<std::string_view,7> fields{};
            if(native_io::split(line,fields)!=2||!seen.emplace(fields[0]).second)throw std::runtime_error("Owned service profile duplicate or malformed field");
            auto found=std::find(identity_names.begin(),identity_names.end(),fields[0]);
            if(found!=identity_names.end())
            {if(!hash(fields[1],result.identities.hashes[static_cast<std::size_t>(found-identity_names.begin())]))throw std::runtime_error("Actual independently exported lowercase native RIHS hash required");}
            else if(fields[0]=="maximum-entries")result.storage.maximum_entries=static_cast<std::size_t>(integer(fields[1],ros::rmw_service_bindings::maximum_entries));
            else if(fields[0]=="history-per-endpoint")result.storage.history_per_endpoint=static_cast<std::size_t>(integer(fields[1],256));
            else if(fields[0]=="maximum-string-wire-bytes")result.maximum_string_wire=static_cast<std::size_t>(integer(fields[1],ipc::max_payload_bytes-518));
            else throw std::runtime_error("Unknown owned service profile field");
        }
        if(seen.size()!=14||!result.storage.valid()||result.maximum_string_wire<4)throw std::runtime_error("All14 exact owned service profile fields are mandatory");
        return result;
#endif
    }
    inline void warm(selection& selected)
    {
#if !UWVM_NATIVE_RCL_SERVICE_API || !UWVM_NATIVE_RCL_AVAILABILITY_API || !UWVM_ROS2_HAVE_COMPLETE_RMW_QOS
        (void)selected;throw std::runtime_error("Actual selected SDK service/QoS/availability compile-link features unavailable");
#else
        // Real CPP10 warms all11 complete original identities and immutable
        // native graphs. No Node/entity is created. Borrowers retain shared
        // ownership of this SAME nonmoving Registry until their actual fini.
        auto real=std::make_shared<uwvm_rmw_cpp::Registry const>(selected.identities,selected.maximum_string_wire);
        if(real->error())throw std::runtime_error("Actual original SDK graph/hash/codec Registry proof refused");
        selected.registry=std::move(real);
#endif
    }
    inline std::uint32_t canonical_bounds(std::shared_ptr<uwvm_rmw_cpp::Registry const> const& registry,
        ros::capability_spec const& cap,native_rcl_service_registry::canonical_sizes& output)noexcept
    {
        if(cap.type=="std_srvs/srv/Empty"||cap.type=="example_interfaces/srv/AddTwoInts")
            return native_rcl_service_registry::fixed_bounds(registry,cap,output);
#if UWVM_ROS2_OWNED_CUSTOM_RCL_SERVICE_PROFILE
        custom_owned_services::canonical_limits actual{};
        auto error=custom_owned_services::canonical_bounds(cap,actual);if(error)return error;
        native_rcl_service_registry::canonical_sizes next{actual.request,actual.response};output=next;return 0;
#else
        (void)registry;(void)cap;(void)output;return 95;
#endif
    }
    inline std::uint32_t create_selected(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,
        std::shared_ptr<uwvm_rmw_cpp::Registry const> const& registry,ros::capability_spec const& cap,
        ipc::rmw_endpoint::binding const& owner,std::uint64_t incarnation,std::size_t history,
        std::shared_ptr<native_rcl_service::report> const& report,
        std::shared_ptr<native_rcl_service_registry::endpoint>& output,ipc::rmw_endpoint::packet& observed)
    {
#if UWVM_ROS2_OWNED_CUSTOM_RCL_SERVICE_PROFILE
        return custom_owned_services::create_selected(node,registry,cap,owner,incarnation,history,report,output,observed);
#else
        return native_rcl_service_registry::create_fixed(node,registry,cap,owner,incarnation,history,report,output,observed);
#endif
    }
    inline bool known(selection const& profile,ros::capability_spec const& cap)
    {
        if(!profile.registry||profile.registry->error())return false;
        if(cap.operation!="service.call"&&cap.operation!="service.advertise"&&cap.operation!="service.respond")return false;
        native_rcl_service_registry::canonical_sizes body{};return canonical_bounds(profile.registry,cap,body)==0;
    }
}
