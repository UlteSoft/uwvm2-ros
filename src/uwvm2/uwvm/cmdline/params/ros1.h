/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#if defined(UWVM_IMPORT_ROS1)
#ifndef UWVM_MODULE
#include <uwvm2/uwvm/cmdline/callback/ros.h>
#endif
#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wbraced-scalar-init"
#endif
namespace uwvm2::uwvm::cmdline::params
{
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_global_enable{
        .name{u8"--ros1-global-enable"},
        .describe{u8"Explicit ROS1 global enable configuration; default deny."},
        .usage{u8""},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::enable, 0>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_global_disable{
        .name{u8"--ros1-global-disable"},
        .describe{u8"Explicit ROS1 global disable configuration; default deny."},
        .usage{u8""},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::disable, 0>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_global_master_uri{
        .name{u8"--ros1-global-master-uri"},
        .describe{u8"Explicit ROS1 global master-uri configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_global_gateway_socket{
        .name{u8"--ros1-global-gateway-socket"},
        .describe{u8"Explicit ROS1 global gateway-socket configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_global_gateway_uid{
        .name{u8"--ros1-global-gateway-uid"},
        .describe{u8"Explicit ROS1 global gateway-uid configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_global_audit_deny_max_rate_hz{
        .name{u8"--ros1-global-audit-deny-max-rate-hz"},
        .describe{u8"Explicit ROS1 global audit-deny-max-rate-hz configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_global_audit_deny_burst{
        .name{u8"--ros1-global-audit-deny-burst"},
        .describe{u8"Explicit ROS1 global audit-deny-burst configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_global_audit_deny_max_records{
        .name{u8"--ros1-global-audit-deny-max-records"},
        .describe{u8"Explicit ROS1 global audit-deny-max-records configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_global_audit_deny_max_bytes{
        .name{u8"--ros1-global-audit-deny-max-bytes"},
        .describe{u8"Explicit ROS1 global audit-deny-max-bytes configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};

    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_global_node_name{
        .name{u8"--ros1-global-node-name"},
        .describe{u8"Explicit ROS1 global node-name configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_global_namespace{
        .name{u8"--ros1-global-namespace"},
        .describe{u8"Explicit ROS1 global namespace configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_global_advertise_host{
        .name{u8"--ros1-global-advertise-host"},
        .describe{u8"Explicit ROS1 global advertise-host configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_global_transport{
        .name{u8"--ros1-global-transport"},
        .describe{u8"Explicit ROS1 global transport configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_global_connect_timeout_ms{
        .name{u8"--ros1-global-connect-timeout-ms"},
        .describe{u8"Explicit ROS1 global connect-timeout-ms configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_global_grant{
        .name{u8"--ros1-global-grant"},
        .describe{u8"Explicit ROS1 global grant configuration; default deny."},
        .usage{u8"<capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::grant, 5>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_global_set{
        .name{u8"--ros1-global-set"},
        .describe{u8"Explicit ROS1 global set configuration; default deny."},
        .usage{u8"<capability:str> <key:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::set, 3>)},
        .pretreatment{::std::addressof(details::ros_details::set_pretreatment<details::ros_details::model::scope::global>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_single_create{
        .name{u8"--ros1-single-create"},
        .describe{u8"Explicit ROS1 single create configuration; default deny."},
        .usage{u8"<module:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::single, details::ros_details::action::create, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_single_enable{
        .name{u8"--ros1-single-enable"},
        .describe{u8"Explicit ROS1 single enable configuration; default deny."},
        .usage{u8"<module:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::single, details::ros_details::action::enable, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_single_disable{
        .name{u8"--ros1-single-disable"},
        .describe{u8"Explicit ROS1 single disable configuration; default deny."},
        .usage{u8"<module:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::single, details::ros_details::action::disable, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_single_master_uri{
        .name{u8"--ros1-single-master-uri"},
        .describe{u8"Explicit ROS1 single master-uri configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_single_gateway_socket{
        .name{u8"--ros1-single-gateway-socket"},
        .describe{u8"Explicit ROS1 single gateway-socket configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_single_gateway_uid{
        .name{u8"--ros1-single-gateway-uid"},
        .describe{u8"Explicit ROS1 single gateway-uid configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_single_audit_deny_max_rate_hz{
        .name{u8"--ros1-single-audit-deny-max-rate-hz"},
        .describe{u8"Explicit ROS1 single audit-deny-max-rate-hz configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_single_audit_deny_burst{
        .name{u8"--ros1-single-audit-deny-burst"},
        .describe{u8"Explicit ROS1 single audit-deny-burst configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_single_audit_deny_max_records{
        .name{u8"--ros1-single-audit-deny-max-records"},
        .describe{u8"Explicit ROS1 single audit-deny-max-records configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_single_audit_deny_max_bytes{
        .name{u8"--ros1-single-audit-deny-max-bytes"},
        .describe{u8"Explicit ROS1 single audit-deny-max-bytes configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};

    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_single_node_name{
        .name{u8"--ros1-single-node-name"},
        .describe{u8"Explicit ROS1 single node-name configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_single_namespace{
        .name{u8"--ros1-single-namespace"},
        .describe{u8"Explicit ROS1 single namespace configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_single_advertise_host{
        .name{u8"--ros1-single-advertise-host"},
        .describe{u8"Explicit ROS1 single advertise-host configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_single_transport{
        .name{u8"--ros1-single-transport"},
        .describe{u8"Explicit ROS1 single transport configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_single_connect_timeout_ms{
        .name{u8"--ros1-single-connect-timeout-ms"},
        .describe{u8"Explicit ROS1 single connect-timeout-ms configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_single_grant{
        .name{u8"--ros1-single-grant"},
        .describe{u8"Explicit ROS1 single grant configuration; default deny."},
        .usage{u8"<module:str> <capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::single, details::ros_details::action::grant, 6>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_single_set{
        .name{u8"--ros1-single-set"},
        .describe{u8"Explicit ROS1 single set configuration; default deny."},
        .usage{u8"<module:str> <capability:str> <key:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::single, details::ros_details::action::set, 4>)},
        .pretreatment{::std::addressof(details::ros_details::set_pretreatment<details::ros_details::model::scope::single>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_create{
        .name{u8"--ros1-group-create"},
        .describe{u8"Explicit ROS1 group create configuration; default deny."},
        .usage{u8"<group:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::create, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_add_module{
        .name{u8"--ros1-group-add-module"},
        .describe{u8"Explicit ROS1 group add-module configuration; default deny."},
        .usage{u8"<group:str> <module:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::add_module, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_enable{
        .name{u8"--ros1-group-enable"},
        .describe{u8"Explicit ROS1 group enable configuration; default deny."},
        .usage{u8"<group:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::enable, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_disable{
        .name{u8"--ros1-group-disable"},
        .describe{u8"Explicit ROS1 group disable configuration; default deny."},
        .usage{u8"<group:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::disable, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_master_uri{
        .name{u8"--ros1-group-master-uri"},
        .describe{u8"Explicit ROS1 group master-uri configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_gateway_socket{
        .name{u8"--ros1-group-gateway-socket"},
        .describe{u8"Explicit ROS1 group gateway-socket configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_gateway_uid{
        .name{u8"--ros1-group-gateway-uid"},
        .describe{u8"Explicit ROS1 group gateway-uid configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_audit_deny_max_rate_hz{
        .name{u8"--ros1-group-audit-deny-max-rate-hz"},
        .describe{u8"Explicit ROS1 group audit-deny-max-rate-hz configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_audit_deny_burst{
        .name{u8"--ros1-group-audit-deny-burst"},
        .describe{u8"Explicit ROS1 group audit-deny-burst configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_audit_deny_max_records{
        .name{u8"--ros1-group-audit-deny-max-records"},
        .describe{u8"Explicit ROS1 group audit-deny-max-records configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_audit_deny_max_bytes{
        .name{u8"--ros1-group-audit-deny-max-bytes"},
        .describe{u8"Explicit ROS1 group audit-deny-max-bytes configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};

    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_node_name{
        .name{u8"--ros1-group-node-name"},
        .describe{u8"Explicit ROS1 group node-name configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_namespace{
        .name{u8"--ros1-group-namespace"},
        .describe{u8"Explicit ROS1 group namespace configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_advertise_host{
        .name{u8"--ros1-group-advertise-host"},
        .describe{u8"Explicit ROS1 group advertise-host configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_transport{
        .name{u8"--ros1-group-transport"},
        .describe{u8"Explicit ROS1 group transport configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_connect_timeout_ms{
        .name{u8"--ros1-group-connect-timeout-ms"},
        .describe{u8"Explicit ROS1 group connect-timeout-ms configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_grant{
        .name{u8"--ros1-group-grant"},
        .describe{u8"Explicit ROS1 group grant configuration; default deny."},
        .usage{u8"<group:str> <capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::grant, 6>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_group_set{
        .name{u8"--ros1-group-set"},
        .describe{u8"Explicit ROS1 group set configuration; default deny."},
        .usage{u8"<group:str> <capability:str> <key:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::group, details::ros_details::action::set, 4>)},
        .pretreatment{::std::addressof(details::ros_details::set_pretreatment<details::ros_details::model::scope::group>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_check_policy{
        .name{u8"--ros1-check-policy"}, .describe{u8"Validate explicit ROS policy; no Guest execution."},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::check, 0>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros1_dump_effective_policy{
        .name{u8"--ros1-dump-effective-policy"}, .describe{u8"Validate explicit ROS policy; no Guest execution."},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros1, details::ros_details::model::scope::global, details::ros_details::action::dump, 0>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros1}};
}
#if defined(__clang__)
# pragma clang diagnostic pop
#endif
#endif
