// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <uwvm_rmw/codec.h>
#include "default_control_gateway.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
namespace uwvm_default_control_gateway_detail {
constexpr std::size_t maximum_nodes=256,maximum_identities=128,maximum_codecs=64;
enum class Language:std::uint8_t {c,cpp};
enum class Kind:std::uint8_t {message,service};
struct Field {
 std::string_view name,nested_type,native_default;
 std::uint8_t description_type{},introspection_type{};
 std::uint64_t native_capacity{},native_string_capacity{};
 bool array{},upper{},vector_bool{};
};
struct Layout {std::size_t size;std::span<std::size_t const> offsets;};
struct Node {
 Language language;Kind kind;std::size_t identity;
 std::string_view name,native_namespace,native_local,source_encoding,source_text,original_source_sha256;
 std::span<Field const> fields;
 std::span<std::size_t const> references;
 std::uint32_t(*default_contract)() noexcept;
 Layout(*layout)() noexcept;
 rosidl_message_type_support_t const*(*message_generic)();
 rosidl_message_type_support_t const*(*message_introspection)();
 rosidl_service_type_support_t const*(*service_generic)();
 rosidl_service_type_support_t const*(*service_introspection)();
 std::array<std::size_t,3> service_components;
};
struct CodecSpec {
 std::size_t node,maximum,request,response,minimum;
 std::string_view canonical_identity;
 std::uint32_t(*body)(void const*,std::size_t) noexcept;
 std::uint32_t(*encode)(void const*,void*,std::size_t,std::size_t*) noexcept;
 std::uint32_t(*decode)(void const*,std::size_t,void*) noexcept;
};
struct Graph {std::span<std::string_view const>identity_names;std::span<Node const>nodes;std::span<CodecSpec const>codecs;};
// Strong definition from actual generated source; no weak stub or fake SDK.
Graph const& generated_graph() noexcept;
}
