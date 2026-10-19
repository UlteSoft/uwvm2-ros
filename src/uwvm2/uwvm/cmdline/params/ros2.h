/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#if defined(UWVM_IMPORT_ROS2)
#ifndef UWVM_MODULE
#include <uwvm2/uwvm/cmdline/callback/ros.h>
#endif
#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wbraced-scalar-init"
#endif
namespace uwvm2::uwvm::cmdline::params
{
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_enable{
        .name{u8"--ros2-global-enable"},
        .describe{u8"Explicit ROS2 global enable configuration; default deny."},
        .usage{u8""},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::enable, 0>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_disable{
        .name{u8"--ros2-global-disable"},
        .describe{u8"Explicit ROS2 global disable configuration; default deny."},
        .usage{u8""},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::disable, 0>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_gateway_socket{
        .name{u8"--ros2-global-gateway-socket"},
        .describe{u8"Explicit ROS2 global gateway-socket configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_gateway_uid{
        .name{u8"--ros2-global-gateway-uid"},
        .describe{u8"Explicit ROS2 global gateway-uid configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_audit_deny_max_rate_hz{
        .name{u8"--ros2-global-audit-deny-max-rate-hz"},
        .describe{u8"Explicit ROS2 global audit-deny-max-rate-hz configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_audit_deny_burst{
        .name{u8"--ros2-global-audit-deny-burst"},
        .describe{u8"Explicit ROS2 global audit-deny-burst configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_audit_deny_max_records{
        .name{u8"--ros2-global-audit-deny-max-records"},
        .describe{u8"Explicit ROS2 global audit-deny-max-records configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_audit_deny_max_bytes{
        .name{u8"--ros2-global-audit-deny-max-bytes"},
        .describe{u8"Explicit ROS2 global audit-deny-max-bytes configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};

    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_node_name{
        .name{u8"--ros2-global-node-name"},
        .describe{u8"Explicit ROS2 global node-name configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_namespace{
        .name{u8"--ros2-global-namespace"},
        .describe{u8"Explicit ROS2 global namespace configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_domain_id{
        .name{u8"--ros2-global-domain-id"},
        .describe{u8"Explicit ROS2 global domain-id configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_rmw_implementation{
        .name{u8"--ros2-global-rmw-implementation"},
        .describe{u8"Explicit ROS2 global rmw-implementation configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_security_mode{
        .name{u8"--ros2-global-security-mode"},
        .describe{u8"Explicit ROS2 global security-mode configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_keystore{
        .name{u8"--ros2-global-keystore"},
        .describe{u8"Explicit ROS2 global keystore configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_enclave{
        .name{u8"--ros2-global-enclave"},
        .describe{u8"Explicit ROS2 global enclave configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_connect_timeout_ms{
        .name{u8"--ros2-global-connect-timeout-ms"},
        .describe{u8"Explicit ROS2 global connect-timeout-ms configuration; default deny."},
        .usage{u8"<value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::connection, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_grant{
        .name{u8"--ros2-global-grant"},
        .describe{u8"Explicit ROS2 global grant configuration; default deny."},
        .usage{u8"<capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::grant, 5>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_global_set{
        .name{u8"--ros2-global-set"},
        .describe{u8"Explicit ROS2 global set configuration; default deny."},
        .usage{u8"<capability:str> <key:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::set, 3>)},
        .pretreatment{::std::addressof(details::ros_details::set_pretreatment<details::ros_details::model::scope::global>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_create{
        .name{u8"--ros2-single-create"},
        .describe{u8"Explicit ROS2 single create configuration; default deny."},
        .usage{u8"<module:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::create, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_enable{
        .name{u8"--ros2-single-enable"},
        .describe{u8"Explicit ROS2 single enable configuration; default deny."},
        .usage{u8"<module:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::enable, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_disable{
        .name{u8"--ros2-single-disable"},
        .describe{u8"Explicit ROS2 single disable configuration; default deny."},
        .usage{u8"<module:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::disable, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_gateway_socket{
        .name{u8"--ros2-single-gateway-socket"},
        .describe{u8"Explicit ROS2 single gateway-socket configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_gateway_uid{
        .name{u8"--ros2-single-gateway-uid"},
        .describe{u8"Explicit ROS2 single gateway-uid configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_audit_deny_max_rate_hz{
        .name{u8"--ros2-single-audit-deny-max-rate-hz"},
        .describe{u8"Explicit ROS2 single audit-deny-max-rate-hz configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_audit_deny_burst{
        .name{u8"--ros2-single-audit-deny-burst"},
        .describe{u8"Explicit ROS2 single audit-deny-burst configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_audit_deny_max_records{
        .name{u8"--ros2-single-audit-deny-max-records"},
        .describe{u8"Explicit ROS2 single audit-deny-max-records configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_audit_deny_max_bytes{
        .name{u8"--ros2-single-audit-deny-max-bytes"},
        .describe{u8"Explicit ROS2 single audit-deny-max-bytes configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};

    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_node_name{
        .name{u8"--ros2-single-node-name"},
        .describe{u8"Explicit ROS2 single node-name configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_namespace{
        .name{u8"--ros2-single-namespace"},
        .describe{u8"Explicit ROS2 single namespace configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_domain_id{
        .name{u8"--ros2-single-domain-id"},
        .describe{u8"Explicit ROS2 single domain-id configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_rmw_implementation{
        .name{u8"--ros2-single-rmw-implementation"},
        .describe{u8"Explicit ROS2 single rmw-implementation configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_security_mode{
        .name{u8"--ros2-single-security-mode"},
        .describe{u8"Explicit ROS2 single security-mode configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_keystore{
        .name{u8"--ros2-single-keystore"},
        .describe{u8"Explicit ROS2 single keystore configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_enclave{
        .name{u8"--ros2-single-enclave"},
        .describe{u8"Explicit ROS2 single enclave configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_connect_timeout_ms{
        .name{u8"--ros2-single-connect-timeout-ms"},
        .describe{u8"Explicit ROS2 single connect-timeout-ms configuration; default deny."},
        .usage{u8"<module:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_grant{
        .name{u8"--ros2-single-grant"},
        .describe{u8"Explicit ROS2 single grant configuration; default deny."},
        .usage{u8"<module:str> <capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::grant, 6>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_single_set{
        .name{u8"--ros2-single-set"},
        .describe{u8"Explicit ROS2 single set configuration; default deny."},
        .usage{u8"<module:str> <capability:str> <key:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::single, details::ros_details::action::set, 4>)},
        .pretreatment{::std::addressof(details::ros_details::set_pretreatment<details::ros_details::model::scope::single>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_create{
        .name{u8"--ros2-group-create"},
        .describe{u8"Explicit ROS2 group create configuration; default deny."},
        .usage{u8"<group:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::create, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_add_module{
        .name{u8"--ros2-group-add-module"},
        .describe{u8"Explicit ROS2 group add-module configuration; default deny."},
        .usage{u8"<group:str> <module:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::add_module, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_enable{
        .name{u8"--ros2-group-enable"},
        .describe{u8"Explicit ROS2 group enable configuration; default deny."},
        .usage{u8"<group:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::enable, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_disable{
        .name{u8"--ros2-group-disable"},
        .describe{u8"Explicit ROS2 group disable configuration; default deny."},
        .usage{u8"<group:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::disable, 1>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_gateway_socket{
        .name{u8"--ros2-group-gateway-socket"},
        .describe{u8"Explicit ROS2 group gateway-socket configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_gateway_uid{
        .name{u8"--ros2-group-gateway-uid"},
        .describe{u8"Explicit ROS2 group gateway-uid configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_audit_deny_max_rate_hz{
        .name{u8"--ros2-group-audit-deny-max-rate-hz"},
        .describe{u8"Explicit ROS2 group audit-deny-max-rate-hz configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_audit_deny_burst{
        .name{u8"--ros2-group-audit-deny-burst"},
        .describe{u8"Explicit ROS2 group audit-deny-burst configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_audit_deny_max_records{
        .name{u8"--ros2-group-audit-deny-max-records"},
        .describe{u8"Explicit ROS2 group audit-deny-max-records configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_audit_deny_max_bytes{
        .name{u8"--ros2-group-audit-deny-max-bytes"},
        .describe{u8"Explicit ROS2 group audit-deny-max-bytes configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};

    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_node_name{
        .name{u8"--ros2-group-node-name"},
        .describe{u8"Explicit ROS2 group node-name configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_namespace{
        .name{u8"--ros2-group-namespace"},
        .describe{u8"Explicit ROS2 group namespace configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_domain_id{
        .name{u8"--ros2-group-domain-id"},
        .describe{u8"Explicit ROS2 group domain-id configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_rmw_implementation{
        .name{u8"--ros2-group-rmw-implementation"},
        .describe{u8"Explicit ROS2 group rmw-implementation configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_security_mode{
        .name{u8"--ros2-group-security-mode"},
        .describe{u8"Explicit ROS2 group security-mode configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_keystore{
        .name{u8"--ros2-group-keystore"},
        .describe{u8"Explicit ROS2 group keystore configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_enclave{
        .name{u8"--ros2-group-enclave"},
        .describe{u8"Explicit ROS2 group enclave configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_connect_timeout_ms{
        .name{u8"--ros2-group-connect-timeout-ms"},
        .describe{u8"Explicit ROS2 group connect-timeout-ms configuration; default deny."},
        .usage{u8"<group:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::connection, 2>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_grant{
        .name{u8"--ros2-group-grant"},
        .describe{u8"Explicit ROS2 group grant configuration; default deny."},
        .usage{u8"<group:str> <capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::grant, 6>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_group_set{
        .name{u8"--ros2-group-set"},
        .describe{u8"Explicit ROS2 group set configuration; default deny."},
        .usage{u8"<group:str> <capability:str> <key:str> <value:str>"},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::group, details::ros_details::action::set, 4>)},
        .pretreatment{::std::addressof(details::ros_details::set_pretreatment<details::ros_details::model::scope::group>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_check_policy{
        .name{u8"--ros2-check-policy"}, .describe{u8"Validate explicit ROS policy; no Guest execution."},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::check, 0>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
    inline constexpr ::uwvm2::utils::cmdline::parameter ros2_dump_effective_policy{
        .name{u8"--ros2-dump-effective-policy"}, .describe{u8"Validate explicit ROS policy; no Guest execution."},
        .handle{::std::addressof(details::ros_details::callback<details::ros_details::model::version::ros2, details::ros_details::model::scope::global, details::ros_details::action::dump, 0>)},
        .cate{::uwvm2::utils::cmdline::categorization::ros2}};
}
#if defined(__clang__)
# pragma clang diagnostic pop
#endif
#endif
