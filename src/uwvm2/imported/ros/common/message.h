/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include "../ipc/service_availability.h"

#include <bit>
#include <charconv>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <span>
#ifndef UWVM_MODULE
#include <fast_io.h>
#endif
#include "types.h"
#include "parameter_search_contract.h"
#include "../ipc/image.h"
#include "../ipc/transform.h"
#include "../ipc/parameter_event.h"
#include "../ipc/parameter.h"
#include "../ipc/action.h"
#include "../ipc/custom_type.h"
#include "../ipc/event.h"

namespace uwvm2::imported::ros
{
    inline bool parse_u64(::std::string_view text, ::std::uint64_t& value) noexcept
    {
        if(text.empty() || text.front() < '0' || text.front() > '9') { return false; }
        ::std::uint64_t parsed{};
        auto const [end, ec]{::fast_io::parse_by_scan(text.data(), text.data() + text.size(), parsed)};
        if(ec != ::fast_io::parse_code::ok || end != text.data() + text.size()) { return false; }
        value = parsed; return true;
    }

    inline bool parse_positive_double(::std::string_view text, double& value) noexcept
    {
        if(text.empty() || !((text.front() >= '0' && text.front() <= '9') || text.front() == '.' || text.front() == '-')) { return false; }
        double parsed{};
        auto const [end, ec]{::fast_io::parse_by_scan(text.data(), text.data() + text.size(), parsed)};
        if(ec != ::fast_io::parse_code::ok || end != text.data() + text.size() || !::std::isfinite(parsed) || parsed <= 0.) { return false; }
        value = parsed; return true;
    }

    inline bool valid_utf8(::std::span<::std::byte const> bytes) noexcept
    {
        ::std::size_t pos{};
        while(pos < bytes.size())
        {
            auto lead{::std::to_integer<::std::uint8_t>(bytes[pos++])};
            if(lead <= 0x7f) { continue; }
            unsigned count{};
            ::std::uint32_t cp{};
            ::std::uint32_t minimum{};
            if(lead >= 0xc2 && lead <= 0xdf) { count = 1; cp = lead & 0x1f; minimum = 0x80; }
            else if(lead >= 0xe0 && lead <= 0xef) { count = 2; cp = lead & 0x0f; minimum = 0x800; }
            else if(lead >= 0xf0 && lead <= 0xf4) { count = 3; cp = lead & 7; minimum = 0x10000; }
            else { return false; }
            if(count > bytes.size() - pos) { return false; }
            for(unsigned i{}; i != count; ++i)
            {
                auto next{::std::to_integer<::std::uint8_t>(bytes[pos++])};
                if((next & 0xc0) != 0x80) { return false; }
                cp = (cp << 6) | (next & 0x3f);
            }
            if(cp < minimum || cp > 0x10ffff || (cp >= 0xd800 && cp <= 0xdfff)) { return false; }
        }
        return true;
    }

    inline constexpr ::std::string_view builtin_type_identity(version v, ::std::string_view type) noexcept
    {
        if(valid_version(v))
        {
            auto parameter{ipc::parameter_identity(v,type)};
            if(!parameter.empty()) { return parameter; }
            auto event{ipc::parameter_event_identity(v,type)};
            if(!event.empty()) { return event; }
            if(type == "builtin/time-v1") { return "builtin:time-nanoseconds-v1"; }
            if(type == ipc::event_type) { return ipc::event_type_identity; }
            if(type == "builtin/graph-list-v1") { return "builtin:graph-list-v1"; }
            if(type == "builtin/string-v1") { return "builtin:param-string-v1"; }
            if(type == "builtin/log-v1") { return "builtin:log-utf8-v1"; }
        }
        if(v == version::ros1)
        {
            if(type == "builtin/string-v1") { return "builtin:param-string-v1"; }

            if(type == "std_msgs/String") { return "992ce8a1687cec8c8bd883ec73ca41d1"; }
            if(type == "std_msgs/Bool") { return "8b94c1b53db61fb6aed406028ad6332a"; }
            if(type == "geometry_msgs/TransformStamped") { return "b5764a33bfeb3588febc2682852579b0"; }
            if(type == "sensor_msgs/Image") { return "060021388200f6f0f447d0fcd9c64743"; }
            if(type == "geometry_msgs/Twist") { return "9f195f881246fdfa2798d1d3eebca84a"; }
            if(type == "std_srvs/Trigger") { return "937c9679a518e3a18d831e57125ea522"; }
            if(type == "std_srvs/SetBool") { return "09fb03525b03e7ea1fd3992bafd87e16"; }
            if(type == "actionlib_tutorials/Fibonacci") { return "f59df5767bf7634684781c92598b2406"; }
        }
        else if(v == version::ros2)
        {
            // RIHS01 hashes from Jazzy generated type-description JSON; gateway verifies rosidl type support.
            if(type == "std_msgs/msg/String") { return "RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18"; }
            if(type == "std_msgs/msg/Bool") { return "RIHS01_feb91e995ff9ebd09c0cb3d2aed18b11077585839fb5db80193b62d74528f6c9"; }
            if(type == "geometry_msgs/msg/TransformStamped") { return "RIHS01_0a241f87d04668d94099cbb5ba11691d5ad32c2f29682e4eb5653424bd275206"; }
            if(type == "sensor_msgs/msg/Image") { return "RIHS01_d31d41a9a4c4bc8eae9be757b0beed306564f7526c88ea6a4588fb9582527d47"; }
            if(type == "geometry_msgs/msg/Twist") { return "RIHS01_9c45bf16fe0983d80e3cfe750d6835843d265a9a6c46bd2e609fcddde6fb8d2a"; }
            // Exact ROS2 native spellings and published complete service hashes.
            // These identities do not qualify an arbitrary SDK: the native
            // registry still proves its selected full generated type graph.
            if(type == "std_srvs/srv/Empty") { return "RIHS01_5888399dedec5ccc85ea6451949fd2c9f97bfdf963f9a588821639fcd31b5d19"; }
            if(type == "example_interfaces/srv/AddTwoInts") { return "RIHS01_e118de6bf5eeb66a2491b5bda11202e7b68f198d6f67922cf30364858239c81a"; }
            if(type == "std_srvs/srv/Trigger") { return "RIHS01_eeff2cd6fa5ad9d27cdf4dec64818317839b62f212a91e6b5304b634b2062c5f"; }
            if(type == "std_srvs/srv/SetBool") { return "RIHS01_abe9e4bb6b41b40e6789712c00ec8871923e089af3f667a79992a428cff2da0a"; }
            if(type == "lifecycle_msgs/srv/GetState") { return "RIHS01_800a0a5aae599782b02932de0caf563f6dc4e7e94b794eadde075ba2cbef9795"; }
            if(type == "lifecycle_msgs/srv/ChangeState") { return "RIHS01_356fe34f0475a43acf54542013af4167b0e729f77ea22ffb045c6ad8e20668e5"; }
            if(type == "rcl_interfaces/msg/ParameterEvent") { return "RIHS01_043e627780fcad87a22d225bc2a037361dba713fca6a6b9f4b869a5aa0393204"; }
            if(type == "action_tutorials_interfaces/action/Fibonacci") { return "RIHS01_525d2c3c8a3979003645ffa01186e360347194550426993cad2fb950058ab5a0"; }
        }
        return {};
    }

    inline constexpr ::std::string_view trusted_type_identity(version v, ::std::string_view type) noexcept
    {
        auto builtin{builtin_type_identity(v, type)};
        if(!builtin.empty()) { return builtin; }
        if(auto descriptor{ipc::find_custom_topic(static_cast<unsigned>(v), type)}) { return descriptor->native_identity; }
        if(auto descriptor{ipc::find_custom_service(static_cast<unsigned>(v), type)}) { return descriptor->native_identity; }
        if(auto descriptor{ipc::find_custom_action(static_cast<unsigned>(v), type)}) { return descriptor->native_identity; }
        return {};
    }
    /** Keep the complete original Topic identity separate from older
     * operation-specific projections of the same native name. The two-argument
     * legacy identity API remains unchanged for existing source callers.
     */
    inline constexpr ::std::string_view trusted_type_identity(version v,::std::string_view type,
        ::std::string_view operation) noexcept
    {
        if(auto descriptor=ipc::find_custom_topic(static_cast<unsigned>(v),type);descriptor&&descriptor->original_control)
        {
            if(ipc::find_custom_topic_for_operation(static_cast<unsigned>(v),type,operation))return descriptor->native_identity;
            return builtin_type_identity(v,type);
        }
        return trusted_type_identity(v,type);
    }
    inline bool custom_topic_codec_matches(version v, capability_spec const& cap) noexcept
    {
        auto descriptor{ipc::find_custom_topic_for_operation(static_cast<unsigned>(v), cap.type, cap.operation)};
        auto identity{cap.limits.find("adapter-codec-identity")};
        return descriptor && descriptor->native_identity == cap.type_identity && identity != cap.limits.end() &&
               identity->second == descriptor->codec_identity;
    }

    /** Bind an explicitly granted custom interface to the immutable compiler
     * catalog. A native identity alone never selects an unreviewed codec. */
    inline bool custom_interface_codec_matches(version v, capability_spec const& cap) noexcept
    {
        auto identity{cap.limits.find("adapter-codec-identity")};
        if(identity == cap.limits.end()) { return false; }
        if(auto descriptor{ipc::find_custom_service(static_cast<unsigned>(v), cap.type)})
        { return cap.type_identity == descriptor->native_identity && identity->second == descriptor->codec_identity; }
        if(auto descriptor{ipc::find_custom_action(static_cast<unsigned>(v), cap.type)})
        { return cap.type_identity == descriptor->native_identity && identity->second == descriptor->codec_identity; }
        return custom_topic_codec_matches(v, cap);
    }

    /** Compare a serialized component against an explicit quota without copying
     * guest bytes. Prefix bytes are protocol-owned tickets, never authority. */
    inline result custom_payload_quota(capability_spec const& cap, ::std::string_view key,
                                      ::std::size_t size, ::std::size_t prefix = 0)
    {
        auto limit{cap.limits.find(key)}; ::std::uint64_t maximum{};
        if(limit == cap.limits.end() || !parse_u64(limit->second, maximum) || !maximum)
        { return {errc::invalid_argument, "custom interface requires an explicit byte quota"}; }
        if(size < prefix) { return {errc::invalid_argument, "custom interface envelope is truncated"}; }
        return size - prefix <= maximum ? result{} : result{errc::too_large, "custom interface exceeds its explicit byte quota"};
    }

    inline result validate_canonical_string(::std::span<::std::byte const> bytes, ::std::uint64_t maximum)
    {
        if(bytes.size() > maximum) { return {errc::too_large, "canonical string exceeds explicit byte quota"}; }
        if(bytes.size() < 4) { return {errc::invalid_argument, "canonical string length truncated"}; }
        ::std::uint32_t length{};
        for(unsigned i{}; i != 4; ++i) { length |= ::std::uint32_t(::std::to_integer<unsigned char>(bytes[i])) << (8 * i); }
        if(length != bytes.size() - 4 || !valid_utf8(bytes.subspan(4))) { return {errc::invalid_argument, "canonical string length or UTF-8 invalid"}; }
        return {};
    }

    inline result validate_service_response(version, capability_spec const&, ::std::span<::std::byte const>);

    inline result validate_transform_payload(version v, capability_spec const& cap, ::std::span<::std::byte const> bytes)
    {
        auto maximum_it{cap.limits.find("max-message-bytes")};
        ::std::uint64_t maximum{};
        if(maximum_it == cap.limits.end() || !parse_u64(maximum_it->second, maximum))
        { return {errc::invalid_argument, "explicit transform byte quota required"}; }
        if(bytes.size() > maximum) { return {errc::too_large, "transform exceeds explicit byte quota"}; }
        // Preserve quota/error precedence while rejecting impossible canonical
        // extents before a potentially large allocation/copy. Do not inspect or
        // borrow payload fields here: the owned copy is still fully decoded.
        if(!ipc::valid_transform_extent(bytes.size()))
        { return {errc::invalid_argument, "invalid canonical TransformStamped"}; }
        ::std::vector<::std::uint8_t> canonical{};
        canonical.reserve(bytes.size());
        for(auto byte : bytes) { canonical.push_back(::std::to_integer<::std::uint8_t>(byte)); }
        ipc::transform_data transform{};
        if(!ipc::decode_transform(canonical, transform) || (v == version::ros2 && transform.sequence))
        { return {errc::invalid_argument, "invalid canonical TransformStamped"}; }
        auto source{cap.limits.find("source-frame")}, target{cap.limits.find("target-frame")};
        if(source == cap.limits.end() || target == cap.limits.end() || transform.child != source->second || transform.parent != target->second)
        { return {errc::permission_denied, "transform frame pair differs from explicit capability"}; }
        auto translation{cap.limits.find("tf.max-translation-abs")}, tolerance{cap.limits.find("tf.quaternion-norm-tolerance")};
        double bound{}, epsilon{};
        if(translation == cap.limits.end() || tolerance == cap.limits.end() || !parse_positive_double(translation->second, bound) || !parse_positive_double(tolerance->second, epsilon))
        { return {errc::invalid_argument, "explicit transform field bounds required"}; }
        for(unsigned i{}; i != 3; ++i)
        { if(::std::abs(transform.values[i]) > bound) { return {errc::permission_denied, "transform translation exceeds explicit bound"}; } }
        double norm{}; for(unsigned i{3}; i != 7; ++i) { norm += transform.values[i] * transform.values[i]; }
        if(::std::abs(norm - 1.) > epsilon) { return {errc::permission_denied, "transform quaternion exceeds explicit tolerance"}; }
        return {};
    }

    inline result validate_message(version v, capability_spec const& cap, ::std::span<::std::byte const> bytes)
    {
        auto const identity{trusted_type_identity(v, cap.type, cap.operation)};
        if(identity.empty()) { return {errc::unsupported, "type has no trusted generated adapter"}; }
        if(identity != cap.type_identity) { return {errc::permission_denied, "trusted type identity mismatch"}; }
        auto const* data{reinterpret_cast<::std::uint8_t const*>(bytes.data())};
        // This NEW metadata envelope is never a Service request body. Type and
        // compiled custom codec identity remain independently mandatory.
        if(cap.operation == "service.available")
        {
            if(ipc::find_custom_service(static_cast<unsigned>(v),cap.type)&&!custom_interface_codec_matches(v,cap))
            { return {errc::permission_denied,"availability custom codec identity mismatch"}; }
            ipc::client_availability::packet request{};::std::uint64_t maximum{};
            if(!ipc::client_availability::decode_request(data,bytes.size(),request)||request.binding.backend!=static_cast<unsigned>(v)||
               request.operation==ipc::client_availability::command::retire||!parse_u64(cap.limits.at("max-wait-ns"),maximum)||request.value>maximum)
            { return {errc::invalid_argument,"invalid bounded availability envelope"}; }
            return {};
        }
        if(auto descriptor{ipc::find_custom_service(static_cast<unsigned>(v), cap.type)})
        {
            if(!custom_interface_codec_matches(v, cap)) { return {errc::permission_denied, "custom service codec identity mismatch"}; }
            if(cap.operation == "service.advertise" && bytes.empty()) { return {}; }
            auto key{cap.operation == "service.respond" ? "max-response-bytes" : "max-request-bytes"};
            auto prefix{cap.operation == "service.call" ? ::std::size_t{} : ::std::size_t{8}};
            if(auto quota{custom_payload_quota(cap, key, bytes.size(), prefix)}; !quota) { return quota; }
            bool valid{cap.operation == "service.advertise" ? ipc::validate_custom_service_event(*descriptor, data, bytes.size()) :
                ipc::validate_custom_service_request(*descriptor, cap.operation, data, bytes.size())};
            return valid ? result{} : result{errc::invalid_argument, "invalid custom service request or ticket envelope"};
        }
        if(auto descriptor{ipc::find_custom_action(static_cast<unsigned>(v), cap.type)})
        {
            if(!custom_interface_codec_matches(v, cap)) { return {errc::permission_denied, "custom action codec identity mismatch"}; }
            if(cap.operation == "action.advertise" && bytes.empty()) { return {}; }
            if(cap.operation == "action.send-goal" || cap.operation == "action.advertise")
            { if(auto quota{custom_payload_quota(cap, "max-goal-bytes", bytes.size(), cap.operation == "action.advertise" ? 8 : 0)}; !quota) { return quota; } }
            else if(cap.operation == "action.respond")
            {
                if(bytes.size() < 9) { return {errc::invalid_argument, "custom action response is truncated"}; }
                auto key{data[8] == 1 ? "max-feedback-bytes" : "max-result-bytes"};
                if(auto quota{custom_payload_quota(cap, key, bytes.size())}; !quota) { return quota; }
            }
            bool valid{cap.operation == "action.advertise" ? ipc::validate_custom_action_event(*descriptor, data, bytes.size()) :
                ipc::validate_custom_action_request(*descriptor, cap.operation, data, bytes.size())};
            return valid ? result{} : result{errc::invalid_argument, "invalid custom action request or goal envelope"};
        }
        if(auto descriptor{ipc::find_custom_topic_for_operation(static_cast<unsigned>(v), cap.type, cap.operation)})
        {
            if(cap.operation != "topic.publish" && cap.operation != "topic.subscribe")
            { return {errc::unsupported, "custom fixed profile supports Topic messages only"}; }
            if(!custom_topic_codec_matches(v, cap)) { return {errc::permission_denied, "custom codec identity differs from trusted registry"}; }
            auto limit{cap.limits.find("max-message-bytes")};::std::uint64_t maximum{};
            if(limit == cap.limits.end() || !parse_u64(limit->second, maximum) || !maximum)
            { return {errc::invalid_argument, "custom Topic requires explicit message byte quota"}; }
            if(bytes.size() > maximum) { return {errc::too_large, "custom Topic exceeds explicit message byte quota"}; }
            // Mandatory shape/Bool/finite checks apply even with validator=none.
            return ipc::validate_custom_topic(*descriptor, reinterpret_cast<::std::uint8_t const*>(bytes.data()), bytes.size()) ?
                   result{} : result{errc::invalid_argument, "invalid custom fixed Topic payload"};
        }
        if(cap.operation.starts_with("action."))
        {
            auto data{reinterpret_cast<::std::uint8_t const*>(bytes.data())};
            auto maximum=[&](char const* key) { ::std::uint64_t n{};auto it{cap.limits.find(key)};if(it!=cap.limits.end()) { parse_u64(it->second,n); }return n; };
            if(cap.operation=="action.send-goal"||cap.operation=="action.advertise")
            {
                if(cap.operation=="action.advertise"&&bytes.empty()) { return {}; }
                auto offset{cap.operation=="action.advertise"?8U:0U};
                if(bytes.size()!=offset+4||(offset&&!ipc::get(data,8))) { return {errc::invalid_argument,"invalid canonical action goal"}; }
                if(bytes.size()-offset>maximum("max-goal-bytes")) { return {errc::too_large,"action goal exceeds explicit quota"}; }
                ::std::int32_t order{};
                if(!ipc::fibonacci_order(data+offset,bytes.size()-offset,order)) { return {errc::invalid_argument,"Fibonacci order outside safe generated adapter range"}; }
                if(static_cast<::std::uint64_t>(order)>maximum("fibonacci.max-order")) { return {errc::permission_denied,"Fibonacci order exceeds explicit bound"}; }
                return {};
            }
            if(cap.operation=="action.respond")
            {
                if(!ipc::validate_action_sequence(data,bytes.size(),true)) { return {errc::invalid_argument,"invalid canonical action server response"}; }
                auto limit{maximum(data[8]==1?"max-feedback-bytes":"max-result-bytes")};
                return bytes.size()<=limit?result{}:result{errc::too_large,"action server response exceeds explicit quota"};
            }
            if(bytes.size()!=8||(cap.operation!="action.cancel-any-goal"&&!ipc::get(data,8))) { return {errc::invalid_argument,"action operation requires explicit nonzero goal ID"}; }
            return {};
        }
        if(cap.operation == "param.subscribe-events")
        {
            bool typed{ipc::parameter_event_type(v,cap.type)!=ipc::parameter_kind::unsupported};
            if(bytes.empty()) { return typed?result{static_cast<errc>(ipc::validate_parameter_contract(v,cap)),"typed event contract"}:result{}; }
            ::std::uint64_t maximum{}; auto limit{cap.limits.find("max-parameter-bytes")};
            if(limit == cap.limits.end() || !parse_u64(limit->second, maximum)) { return {errc::invalid_argument, "explicit parameter-event byte quota required"}; }
            if(bytes.size() > maximum) { return {errc::too_large, "parameter event exceeds explicit byte quota"}; }
            ::std::vector<::std::uint8_t> canonical{}; canonical.reserve(bytes.size());
            for(auto byte : bytes) { canonical.push_back(::std::to_integer<::std::uint8_t>(byte)); }
            if(typed) { return {static_cast<errc>(ipc::validate_typed_parameter_event(v,cap,canonical)),"selected typed parameter event validation"}; }
            ipc::parameter_event event{};
            if(!ipc::decode_parameter_event(canonical, event)) { return {errc::invalid_argument, "invalid selected parameter event"}; }
            auto node{cap.limits.find("parameter-node")}, name{cap.limits.find("parameter-name")};
            if(node == cap.limits.end() || name == cap.limits.end() || event.node != node->second || event.name != name->second)
            { return {errc::permission_denied, "parameter event differs from explicit node/name"}; }
            return {};
        }
        if(cap.operation.starts_with("lifecycle."))
        { return bytes.empty() ? result{} : result{errc::invalid_argument, "lifecycle request must be empty; transition belongs to explicit policy"}; }
        if(cap.operation == "tf.lookup")
        { return bytes.size() == 8 ? result{} : result{errc::invalid_argument, "TF lookup requires u64 nanoseconds"}; }
        if(cap.operation == "tf.broadcast") { return validate_transform_payload(v, cap, bytes); }
        if(cap.operation.starts_with("time.") || cap.operation.starts_with("graph."))
        { return bytes.empty() ? result{} : result{errc::invalid_argument, "time request must be empty"}; }
        if(cap.operation == "param.set" || cap.operation == "param.declare")
        { return {static_cast<errc>(ipc::validate_parameter_value(v,cap,reinterpret_cast<::std::uint8_t const*>(bytes.data()),bytes.size())), "typed parameter request validation"}; }
        if(cap.operation == "log.write" || cap.operation.starts_with("param."))
        {
            bool const needs_string{cap.operation == "log.write" || cap.operation == "param.set" || cap.operation == "param.declare"};
            if(!needs_string) { return bytes.empty() ? result{} : result{errc::invalid_argument, "parameter request must be empty"}; }
            auto max_it{cap.limits.find(cap.operation == "log.write" ? "max-message-bytes" : "max-parameter-bytes")};
            ::std::uint64_t maximum{};
            if(max_it == cap.limits.end() || !parse_u64(max_it->second, maximum)) { return {errc::invalid_argument, "explicit string byte quota required"}; }
            return validate_canonical_string(bytes, maximum);
        }
        if(cap.operation == "service.advertise")
        {
            if(bytes.empty()) { return {}; }
            if(bytes.size() < 8) { return {errc::invalid_argument, "service event request ID truncated"}; }
            ::std::uint64_t id{}; for(unsigned i{}; i != 8; ++i) { id |= ::std::uint64_t(::std::to_integer<unsigned char>(bytes[i])) << (8 * i); }
            if(!id) { return {errc::invalid_argument, "service event request ID must be nonzero"}; }
            auto request_cap{cap}; request_cap.operation = "service.call";
            return validate_message(v, request_cap, bytes.subspan(8));
        }
        if(cap.operation == "service.respond")
        {
            if(bytes.size() < 8) { return {errc::invalid_argument, "service response request ID truncated"}; }
            ::std::uint64_t id{}; for(unsigned i{}; i != 8; ++i) { id |= ::std::uint64_t(::std::to_integer<unsigned char>(bytes[i])) << (8 * i); }
            if(!id) { return {errc::invalid_argument, "service response request ID must be nonzero"}; }
            auto response_cap{cap}; response_cap.operation = "service.call";
            return validate_service_response(v, response_cap, bytes.subspan(8));
        }
        bool const service{cap.operation == "service.call"};
        auto max_it{cap.limits.find(service ? "max-request-bytes" : "max-message-bytes")};
        ::std::uint64_t maximum{};
        if(max_it == cap.limits.end() || !parse_u64(max_it->second, maximum) || !maximum)
        { return {errc::invalid_argument, "explicit message/request byte limit is required"}; }
        if(bytes.size() > maximum) { return {errc::too_large, "message exceeds explicit byte limit"}; }
        if(cap.type == "sensor_msgs/Image" || cap.type == "sensor_msgs/msg/Image")
        {
            ipc::image_view image{};
            if(!ipc::decode_image_view(reinterpret_cast<::std::uint8_t const*>(bytes.data()),bytes.size(),static_cast<::std::uint8_t>(v),image))
            { return {errc::invalid_argument, "Image structure, encoding, stride, timestamp or length invalid"}; }
            return {};
        }
        if(service)
        {
            // Empty has no canonical bytes. Its generated native placeholder
            // belongs to the selected native codec, never this wire format.
            if(v == version::ros2 && cap.type == "std_srvs/srv/Empty")
            { return bytes.empty() ? result{} : result{errc::invalid_argument, "Empty request must have zero canonical bytes"}; }
            // All i64 bit patterns are valid. Validate extent without signed
            // arithmetic, alignment assumptions or a copy of guest bytes.
            if(v == version::ros2 && cap.type == "example_interfaces/srv/AddTwoInts")
            { return bytes.size() == 16 ? result{} : result{errc::invalid_argument, "AddTwoInts request requires two canonical i64 fields"}; }
            if(cap.type == "std_srvs/Trigger" || cap.type == "std_srvs/srv/Trigger")
            { return bytes.empty() ? result{} : result{errc::invalid_argument, "Trigger request must be empty"}; }
            if(cap.type == "std_srvs/SetBool" || cap.type == "std_srvs/srv/SetBool")
            { return bytes.size() == 1 && ::std::to_integer<unsigned char>(bytes[0]) <= 1 ? result{} : result{errc::invalid_argument, "SetBool request must be canonical boolean"}; }
            return {errc::unsupported, "service request has no trusted adapter"};
        }
        bool const is_string{cap.type == "std_msgs/String" || cap.type == "std_msgs/msg/String"};
        bool const is_bool{cap.type == "std_msgs/Bool" || cap.type == "std_msgs/msg/Bool"};
        if(is_string)
        {
            if(bytes.size() < 4) { return {errc::invalid_argument, "truncated string length"}; }
            ::std::uint32_t length{};
            for(unsigned i{}; i != 4; ++i) { length |= ::std::uint32_t(::std::to_integer<unsigned char>(bytes[i])) << (8 * i); }
            if(length != bytes.size() - 4 || !valid_utf8(bytes.subspan(4)))
            { return {errc::invalid_argument, "string length or UTF-8 encoding invalid"}; }
        }
        else if(is_bool)
        {
            if(bytes.size() != 1 || ::std::to_integer<unsigned char>(bytes[0]) > 1)
            { return {errc::invalid_argument, "boolean must have canonical value 0 or 1"}; }
        }
        else
        {
            if(bytes.size() != 48) { return {errc::invalid_argument, "Twist requires six f64 fields"}; }
            for(unsigned i{}; i != 6; ++i)
            {
                ::std::uint64_t bits{};
                for(unsigned j{}; j != 8; ++j) { bits |= ::std::uint64_t(::std::to_integer<unsigned char>(bytes[8 * i + j])) << (8 * j); }
                auto value{::std::bit_cast<double>(bits)};
                if(!::std::isfinite(value)) { return {errc::invalid_argument, "nonfinite Twist field"}; }
                auto validator{cap.limits.find("validator")};
                if(validator != cap.limits.end() && validator->second == "twist-bounds")
                {
                    auto bound{cap.limits.find(i < 3 ? "twist.linear-max-abs" : "twist.angular-max-abs")};
                    double limit{};
                    if(bound == cap.limits.end() || !parse_positive_double(bound->second, limit) || ::std::abs(value) > limit)
                    { return {errc::permission_denied, "Twist field exceeds explicit bound"}; }
                }
            }
        }
        return {};
    }
    inline result validate_service_response(version v, capability_spec const& cap, ::std::span<::std::byte const> bytes)
    {
        if(auto descriptor{ipc::find_custom_service(static_cast<unsigned>(v), cap.type)})
        {
            if(cap.operation != "service.call" || !custom_interface_codec_matches(v, cap))
            { return {errc::permission_denied, "custom service response capability/codec mismatch"}; }
            if(auto quota{custom_payload_quota(cap, "max-response-bytes", bytes.size())}; !quota) { return quota; }
            return ipc::validate_custom_service_output(*descriptor, cap.operation, reinterpret_cast<::std::uint8_t const*>(bytes.data()), bytes.size()) ?
                result{} : result{errc::invalid_argument, "invalid custom service response"};
        }
        if(cap.operation != "service.call" || builtin_type_identity(v, cap.type) != cap.type_identity)
        { return {errc::permission_denied, "response capability/type mismatch"}; }
        bool const empty{v == version::ros2 && cap.type == "std_srvs/srv/Empty"};
        bool const add{v == version::ros2 && cap.type == "example_interfaces/srv/AddTwoInts"};
        if(!empty && !add && cap.type != "std_srvs/Trigger" && cap.type != "std_srvs/srv/Trigger" && cap.type != "std_srvs/SetBool" && cap.type != "std_srvs/srv/SetBool")
        { return {errc::unsupported, "service response has no trusted adapter"}; }
        auto max_it{cap.limits.find("max-response-bytes")};
        ::std::uint64_t maximum{};
        if(max_it == cap.limits.end() || !parse_u64(max_it->second, maximum) || !maximum) { return {errc::invalid_argument, "explicit response byte limit required"}; }
        if(bytes.size() > maximum) { return {errc::too_large, "response exceeds explicit limit"}; }
        if(empty) { return bytes.empty() ? result{} : result{errc::invalid_argument, "Empty response must have zero canonical bytes"}; }
        if(add) { return bytes.size() == 8 ? result{} : result{errc::invalid_argument, "AddTwoInts response requires one canonical i64 field"}; }
        if(bytes.size() < 5 || ::std::to_integer<unsigned char>(bytes[0]) > 1) { return {errc::invalid_argument, "truncated or noncanonical service response"}; }
        ::std::uint32_t length{};
        for(unsigned i{}; i != 4; ++i) { length |= ::std::uint32_t(::std::to_integer<unsigned char>(bytes[1 + i])) << (8 * i); }
        if(length != bytes.size() - 5 || !valid_utf8(bytes.subspan(5))) { return {errc::invalid_argument, "invalid service response string"}; }
        return {};
    }
    inline result validate_request_payload(version v, capability_spec const& cap, ::std::span<::std::byte const> bytes)
    { return validate_message(v, cap, bytes); }

    inline result validate_response_payload(version v, capability_spec const& cap, ::std::span<::std::byte const> bytes)
    {
        if(cap.operation == "service.available")
        {
            ipc::client_availability::packet response{};
            if(trusted_type_identity(v,cap.type,cap.operation)!=cap.type_identity||
               (ipc::find_custom_service(static_cast<unsigned>(v),cap.type)&&!custom_interface_codec_matches(v,cap)))
            { return {errc::permission_denied,"availability response native/codec identity mismatch"}; }
            return ipc::client_availability::decode(reinterpret_cast<::std::uint8_t const*>(bytes.data()),bytes.size(),response,true)&&
                   response.binding.backend==static_cast<unsigned>(v)&&response.operation!=ipc::client_availability::command::retire?
                   result{}:result{errc::invalid_argument,"invalid availability response envelope"};
        }
        if(auto descriptor{ipc::find_custom_service(static_cast<unsigned>(v), cap.type)})
        {
            if(!custom_interface_codec_matches(v, cap)) { return {errc::permission_denied, "custom service response identity mismatch"}; }
            if(cap.operation == "service.call") { return validate_service_response(v, cap, bytes); }
            return ipc::validate_custom_service_output(*descriptor, cap.operation, reinterpret_cast<::std::uint8_t const*>(bytes.data()), bytes.size()) ?
                result{} : result{errc::invalid_argument, "custom service operation response must be empty"};
        }
        if(auto descriptor{ipc::find_custom_action(static_cast<unsigned>(v), cap.type)})
        {
            if(!custom_interface_codec_matches(v, cap)) { return {errc::permission_denied, "custom action response identity mismatch"}; }
            if(cap.operation == "action.receive-feedback" || cap.operation == "action.receive-result")
            { if(auto quota{custom_payload_quota(cap, cap.operation == "action.receive-feedback" ? "max-feedback-bytes" : "max-result-bytes", bytes.size())}; !quota) { return quota; } }
            return ipc::validate_custom_action_output(*descriptor, cap.operation, reinterpret_cast<::std::uint8_t const*>(bytes.data()), bytes.size()) ?
                result{} : result{errc::invalid_argument, "invalid custom action response"};
        }
        if(ipc::find_custom_topic_for_operation(static_cast<unsigned>(v), cap.type, cap.operation))
        {
            if(!custom_topic_codec_matches(v, cap)) { return {errc::permission_denied, "custom response native/codec identity mismatch"}; }
            if(cap.operation == "topic.publish")
            { return bytes.empty() ? result{} : result{errc::invalid_argument, "custom publish response must be empty"}; }
            if(cap.operation == "topic.subscribe") { return validate_message(v, cap, bytes); }
            return {errc::unsupported, "custom response operation is not implemented"};
        }
        if(builtin_type_identity(v, cap.type).empty() || builtin_type_identity(v, cap.type) != cap.type_identity)
        { return {errc::permission_denied, "response trusted type identity mismatch"}; }
        if(cap.operation == "service.call") { return validate_service_response(v, cap, bytes); }
        if(cap.operation.starts_with("action."))
        {
            if(cap.operation=="action.advertise"||cap.operation=="action.respond") { return bytes.empty()?result{}:result{errc::invalid_argument,"action server operation response must be empty"}; }
            if(cap.operation=="action.send-goal")
            {
                if(bytes.size()!=8) { return {errc::invalid_argument,"send-goal response requires u64 goal ID"}; }
                ::std::uint64_t goal{};for(unsigned i{};i!=8;++i) { goal|=::std::uint64_t(::std::to_integer<unsigned char>(bytes[i]))<<(8*i); }
                return goal?result{}:result{errc::invalid_argument,"generated goal ID must be nonzero"};
            }
            if(cap.operation=="action.cancel-own-goal"||cap.operation=="action.cancel-any-goal") { return bytes.size()==1&&::std::to_integer<unsigned char>(bytes[0])<=1?result{}:result{errc::invalid_argument,"cancel response requires canonical boolean"}; }
            auto key{cap.operation=="action.receive-feedback"?"max-feedback-bytes":"max-result-bytes"};::std::uint64_t maximum{};auto it{cap.limits.find(key)};
            if(it==cap.limits.end()||!parse_u64(it->second,maximum)) { return {errc::invalid_argument,"explicit action event quota required"}; }
            if(bytes.size()>maximum) { return {errc::too_large,"action event exceeds explicit quota"}; }
            auto data{reinterpret_cast<::std::uint8_t const*>(bytes.data())};bool result{cap.operation=="action.receive-result"};
            return ipc::validate_action_sequence(data,bytes.size(),result)&&(!result||data[8]>=2)?ros::result{}:ros::result{errc::invalid_argument,"invalid canonical action feedback/result"};
        }
        if(cap.operation == "tf.lookup") { return validate_transform_payload(v, cap, bytes); }
        if(cap.operation.starts_with("lifecycle."))
        {
            ::std::uint64_t maximum{}; auto limit{cap.limits.find("max-response-bytes")};
            if(limit == cap.limits.end() || !parse_u64(limit->second, maximum)) { return {errc::invalid_argument, "explicit lifecycle response byte quota required"}; }
            if(bytes.size() > maximum) { return {errc::too_large, "lifecycle response exceeds explicit byte quota"}; }
            if(cap.operation == "lifecycle.change-state")
            { return bytes.size() == 1 && ::std::to_integer<unsigned char>(bytes[0]) <= 1 ? result{} : result{errc::invalid_argument, "lifecycle change response requires canonical boolean"}; }
            if(cap.operation == "lifecycle.get-state")
            { return bytes.size() >= 5 ? validate_canonical_string(bytes.subspan(1), maximum - 1) : result{errc::invalid_argument, "lifecycle state response truncated"}; }
        }
        if(cap.operation.starts_with("time."))
        {
            ::std::uint64_t maximum{}; auto limit{cap.limits.find("max-response-bytes")};
            if(limit == cap.limits.end() || !parse_u64(limit->second, maximum) || maximum < 8) { return {errc::invalid_argument, "explicit time response byte quota must allow 8 bytes"}; }
            return bytes.size() == 8 ? result{} : result{errc::invalid_argument, "time response requires u64 nanoseconds"};
        }
        if(cap.operation == "param.subscribe-events" || cap.operation == "tf.broadcast" || cap.operation == "service.advertise" || cap.operation == "service.respond" || cap.operation == "log.write" || (cap.operation == "param.set" || cap.operation == "param.delete" || cap.operation == "param.declare" || cap.operation == "param.undeclare"))
        { return bytes.empty() ? result{} : result{errc::invalid_argument, "operation response must be empty"}; }
        if(v == version::ros1 && cap.operation == "param.exists")
        { return bytes.size() == 1 && ::std::to_integer<unsigned char>(bytes[0]) <= 1 ? result{} : result{errc::invalid_argument, "exists response must be canonical boolean"}; }
        if(cap.operation == "param.get")
        { return {static_cast<errc>(ipc::validate_parameter_value(v,cap,reinterpret_cast<::std::uint8_t const*>(bytes.data()),bytes.size())), "typed parameter response validation"}; }
        if(cap.operation == "param.get" || cap.operation == "param.describe" || cap.operation == "param.search" || cap.operation == "param.list" || cap.operation.starts_with("graph."))
        {
            ::std::uint64_t maximum{}; auto limit{cap.limits.find(cap.operation.starts_with("graph.") ? "max-graph-result-bytes" : "max-parameter-bytes")};
            if(limit == cap.limits.end() || !parse_u64(limit->second, maximum)) { return {errc::invalid_argument, "explicit parameter byte quota required"}; }
            auto encoded{validate_canonical_string(bytes, maximum)};
            if(!encoded || cap.operation != "param.search") { return encoded; }
            // Search returns a resource name, rather than arbitrary text.
            // Validate the same exact/explicit-parent candidates as the
            // Gateway before exposing any native parameter name to a guest.
            ::std::string_view name{reinterpret_cast<char const*>(bytes.data() + 4), bytes.size() - 4};
            return parameter_search_name_allowed(cap, name) ? result{} :
                result{errc::permission_denied, "parameter search response is outside its explicit candidate contract"};
        }
        return {errc::unsupported, "operation has no trusted response adapter"};
    }

}
