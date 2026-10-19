/* SPDX-License-Identifier: Apache-2.0 */
/** @file Explicit Host selection of the complete original ROS2 control profile.
 * The independent actual Native export is compiled into this selected build;
 * a trusted runtime file selects its exact export digest and finite storage.
 * Neither the file nor the compiled schema grants any Guest ROS authority.
 * Every original capability/identity/codec/QoS/quota/session check still applies.
 */
#pragma once
#include "../common/native_io.h"
#include "owned-service-profile.h"
#include "native_rmw_publisher_registry.h"
#include "native_rmw_subscription_registry.h"
#include "native_rmw_subscription_factory.h"
#include <default_control_gateway.h>
#include <default_control_gateway_identity_input.hpp>
#include <array>
#include <set>
#include <memory>
#include <string>
#include <string_view>
#include <stdexcept>
#include <utility>
namespace uwvm2::ros_gateway::default_control_profile
{
    namespace actual=default_control_gateway;
    struct selection
    {
        native_rcl_service_registry::limits storage{};
        std::shared_ptr<actual::profile const> proof;
    };
    inline bool handles(ros::capability_spec const& cap)noexcept
    {
        if(auto row=ipc::find_custom_topic_for_operation(2,cap.type,cap.operation))return row->original_control;
        if(auto row=ipc::find_custom_service(2,cap.type))
            return row->request&&row->response&&row->request->original_control&&row->response->original_control;
        return false;
    }
    inline selection load(std::string const& path)
    {
#if !defined(__linux__)
        (void)path;throw std::runtime_error("Original control profile requires actual trusted Linux file admission");
#else
        native_io::config_lines input{path};std::string_view line;selection result;
        std::set<std::string> seen;unsigned lines{};std::size_t bytes{};
        while(input.next(line))
        {
            if(++lines>32||line.size()>256||bytes>4096-line.size())throw std::runtime_error("Original control profile text exceeds finite limit");
            bytes+=line.size();if(line.empty()||line.front()=='#')continue;
            // Use the real common parser's seven-slot contract; exactly two
            // fields remain mandatory, including when extra tokens are present.
            std::array<std::string_view,7> fields{};
            if(native_io::split(line,fields)!=2||!seen.emplace(fields[0]).second)throw std::runtime_error("Original control profile duplicate/malformed field");
            if(fields[0]=="maximum-entries")result.storage.maximum_entries=static_cast<std::size_t>(integer(fields[1],ros::rmw_service_bindings::maximum_entries));
            else if(fields[0]=="history-per-endpoint")result.storage.history_per_endpoint=static_cast<std::size_t>(integer(fields[1],owned_service_storage::maximum_history));
            else if(fields[0]=="export-packet-sha256")
            {
                // The literal is emitted only from an explicit independent
                // Native export packet with its external caller-supplied SHA.
                // This equality associates DATA; actual SDK proof follows.
                static_assert(sizeof(uwvm_default_control_gateway_input::export_packet_sha256)==65);
                if(fields[1]!=uwvm_default_control_gateway_input::export_packet_sha256)
                    throw std::runtime_error("Original control export packet differs from selected build");
            }
            else throw std::runtime_error("Unknown original control storage profile field");
        }
        if(seen.size()!=3||!result.storage.valid())throw std::runtime_error("All three original control profile fields are mandatory");
        return result;
#endif
    }
    inline void warm(selection& selected)
    {
        // Preparation output contains blank identities and is deliberately an
        // unavailable catalog. Linking such a file cannot activate a profile.
        if(!ipc::custom_topic_registry_valid||!ipc::custom_interface_registry_valid)
            throw std::runtime_error("Complete frozen control canonical tables required");
        std::size_t topics{},services{};
        auto ts=ipc::effective_custom_topic_entries();auto ss=ipc::effective_custom_service_entries();
        for(std::size_t i{};i<ts.size;++i)if(ts.data[i].original_control)++topics;
        for(std::size_t i{};i<ss.size;++i)if(ss.data[i].request&&ss.data[i].request->original_control)++services;
        if(topics!=2||services!=7)throw std::runtime_error("Complete original two-Topic/seven-Service catalog required");
        auto const& ids=uwvm_default_control_gateway_input::identities;
        auto error=actual::prepare(ids.data(),ids.size(),selected.proof);
        if(error||!selected.proof)throw std::runtime_error("Actual original CPP control graph/hash/source/default/layout proof refused");
    }
    inline native_rmw_publisher::factory publisher_factory(std::shared_ptr<selection const> selected)
    {
        return [selected=std::move(selected)](auto const& node,auto const& cap,auto const& owner,
            std::uint64_t incarnation,auto& endpoint,auto& observed)->std::uint32_t
        {
            if(handles(cap))return selected?actual::create_publisher(selected->proof,node,cap,owner,incarnation,endpoint,observed):95u;
            return native_rmw_publisher::create_selected(node,cap,owner,incarnation,endpoint,observed);
        };
    }
    inline native_rmw_subscription::factory subscription_factory(std::shared_ptr<selection const> selected)
    {
        return [selected=std::move(selected)](auto const& node,auto const& cap,auto const& owner,
            std::uint64_t incarnation,auto& endpoint,auto& observed)->std::uint32_t
        {
            if(handles(cap))return selected?actual::create_subscription(selected->proof,node,cap,owner,incarnation,endpoint,observed):95u;
            return native_rmw_subscription::create_selected(node,cap,owner,incarnation,endpoint,observed);
        };
    }
    inline native_rcl_service_registry::factory service_factory(std::shared_ptr<selection const> selected,bool legacy_selected)
    {
        return [selected=std::move(selected),legacy_selected](auto const& node,auto const& old_registry,auto const& cap,
            auto const& owner,std::uint64_t incarnation,std::size_t history,auto const& report,auto& endpoint,auto& observed)->std::uint32_t
        {
            if(handles(cap))return selected?actual::create_service(selected->proof,node,old_registry,cap,owner,incarnation,history,report,endpoint,observed):95u;
            return legacy_selected?owned_service_profile::create_selected(node,old_registry,cap,owner,incarnation,history,report,endpoint,observed):95u;
        };
    }
    inline native_rcl_service_registry::bounds_selector bounds_factory(std::shared_ptr<selection const> selected,bool legacy_selected)
    {
        return [selected=std::move(selected),legacy_selected](auto const& old_registry,auto const& cap,
            native_rcl_service_registry::canonical_sizes& output)->std::uint32_t
        {
            if(handles(cap))
            {
                if(!selected)return 95;actual::canonical_limits actual_bounds{};
                auto error=actual::canonical_bounds(selected->proof,cap,actual_bounds);if(error)return error;
                native_rcl_service_registry::canonical_sizes next{actual_bounds.request,actual_bounds.response,actual_bounds.separately_owned_native_depth_limit};
                output=next;return 0;
            }
            return legacy_selected?owned_service_profile::canonical_bounds(old_registry,cap,output):95u;
        };
    }
}
