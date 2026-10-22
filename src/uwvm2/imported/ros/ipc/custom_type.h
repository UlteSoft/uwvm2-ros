/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <limits>
#include <initializer_list>
#include <string_view>
#include "custom_wire.h"
#include "complete_vendor_domain.h"
#include "custom_registry_limits.h"

namespace uwvm2::imported::ros::ipc
{
    // This first custom profile supports fixed Topic records only. These tags
    // describe a trusted generated layout; they are never sent by a Guest.
    // A registry establishes supported types and creates no capability grant.
    enum class custom_scalar_kind : ::std::uint8_t
    { boolean, u8, i8, u16, i16, u32, i32, u64, i64, f32, f64 };

    struct custom_scalar_layout
    {
        custom_scalar_kind kind{};
        ::std::uint32_t count{};
    };
    struct custom_topic_descriptor
    {
        unsigned ros_version{};
        ::std::string_view type{}, native_identity{}, codec_identity{};
        ::std::uint32_t wire_bytes{};
        custom_scalar_layout const* layout{};
        ::std::size_t layout_count{};
        // Bounded-v2 uses a separate complete generated grammar. The legacy
        // fixed-v1 fields stay source-compatible; the two forms cannot mix.
        custom_wire_node const* bounded_layout{};
        ::std::uint32_t minimum_wire_bytes{};
        // Trusted generated-source marker, never a Guest or wire field. Only
        // explicitly supported complete standard Topics may opt in; existing builtin IDs
        // remain reserved. Header/Time/Duration are dependency schemas only.
        bool official_standard{};
        // Trusted source-only domain selection for the exact original ROS2
        // control family below. This is never a Guest flag, Native proof or
        // permission. Native adapters still prove complete actual metadata.
        bool original_control{};
        // Explicit complete vendor Source domain. Set only by an independently
        // selected generated manifest; never a wire or Guest permission flag.
        bool complete_vendor{};
    };
    struct custom_topic_registry
    {
        custom_topic_descriptor const* data{};
        ::std::size_t size{};
    };
    /** Official interface identity and independently fingerprinted canonical
     * component schemas. These descriptors support types and grant no rights.
     * Request/goal IDs remain Runtime-owned envelopes outside these schemas. */
    struct custom_service_descriptor
    {
        unsigned ros_version{};
        ::std::string_view type{},native_identity{},codec_identity{};
        custom_topic_descriptor const* request{};
        custom_topic_descriptor const* response{};
    };
    struct custom_action_descriptor
    {
        unsigned ros_version{};
        ::std::string_view type{},native_identity{},codec_identity{};
        custom_topic_descriptor const* goal{};
        custom_topic_descriptor const* result{};
        custom_topic_descriptor const* feedback{};
    };
    struct custom_service_registry {custom_service_descriptor const* data{};::std::size_t size{};};
    struct custom_action_registry {custom_action_descriptor const* data{};::std::size_t size{};};

    // Native providers independently compare these declaration-level records
    // against official ROS1 Definition or ROS2 TypeDescription metadata. A
    // genuine native identity alone cannot bind a differently supplied codec
    // schema. Scalar type strings use the explicit .msg spellings; nested type
    // strings are complete version-specific names. No dynamic type is accepted.
    struct custom_native_field
    {
        ::std::string_view name{}, type{};
        ::std::uint32_t array_size{};
        bool is_array{};
        // Official ROS2 capacities, distinct from stricter canonical limits.
        // Unbounded native fields use zero here and still require an explicit
        // bounded canonical wire node. Both shapes are checked independently.
        ::std::uint32_t string_bound{};
        bool is_sequence{};
        ::std::uint32_t sequence_bound{};
        // Exact normalized official TypeDescription default literal. Empty
        // preserves every previous no-default aggregate initializer. This is
        // declaration metadata, never an omitted-wire fallback or authority.
        // The first supported nonempty family is ROS2 Quaternion float64s.
        ::std::string_view native_default{};
        // Complete-domain declaration presence is Source metadata. Native
        // TypeDescription alone cannot distinguish absent from explicit empty
        // string defaults; such declarations require additional actual
        // original-IDL proof and are refused by the current complete provider.
        bool has_declared_default{};
    };
    struct custom_native_record
    {
        ::std::string_view type{};
        custom_native_field const* fields{};
        ::std::size_t field_count{};
    };
    struct custom_native_schema
    {
        custom_native_record const* records{};
        ::std::size_t record_count{};
    };

    /** Trusted generated Service/Action declaration, including the official
     * protocol messages, service events and transitive standard dependencies.
     * A native provider specializes this trait without depending on Gateway
     * headers. Older providers retain their native baked-hash path; an empty
     * trait cannot attest an interface on a pre-RIHS ROS2 SDK. Registration
     * describes a compiled adapter and never grants Guest authority. */
    template<class Native> struct custom_native_interface_schema
    {
        static constexpr custom_native_schema schema() noexcept { return {}; }
    };

    inline constexpr unsigned custom_scalar_width(custom_scalar_kind kind) noexcept
    {
        switch(kind)
        {
            case custom_scalar_kind::boolean: case custom_scalar_kind::u8: case custom_scalar_kind::i8: return 1;
            case custom_scalar_kind::u16: case custom_scalar_kind::i16: return 2;
            case custom_scalar_kind::u32: case custom_scalar_kind::i32: case custom_scalar_kind::f32: return 4;
            case custom_scalar_kind::u64: case custom_scalar_kind::i64: case custom_scalar_kind::f64: return 8;
        }
        return 0;
    }
    inline constexpr bool custom_hexadecimal(::std::string_view text) noexcept
    {
        for(char c : text) { if(!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) { return false; } }
        return !text.empty();
    }
    inline constexpr bool custom_codec_identity(::std::string_view text) noexcept
    { return text.size() == 73 && text.substr(0, 9) == "UWVMROS1_" && custom_hexadecimal(text.substr(9)); }

    inline constexpr bool custom_topic_name(unsigned version, ::std::string_view name) noexcept
    {
        if((version != 1 && version != 2) || name.size() > 255) { return false; }
        auto slash{name.find('/')};
        if(slash == ::std::string_view::npos || !slash || slash > 63) { return false; }
        auto package{name.substr(0, slash)};
        if(package.front() < 'a' || package.front() > 'z') { return false; }
        for(char c : package) { if(!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_')) { return false; } }
        // Existing official profiles cannot be shadowed by a generated record.
        for(auto reserved : {"builtin", "std_msgs", "std_srvs", "geometry_msgs", "sensor_msgs", "rcl_interfaces",
                             "lifecycle_msgs", "actionlib_tutorials", "action_tutorials_interfaces"})
        { if(package == reserved) { return false; } }
        auto record{name.substr(slash + 1)};
        if(version == 2)
        {
            if(record.size() < 5 || record.substr(0, 4) != "msg/") { return false; }
            record.remove_prefix(4);
        }
        if(record.empty() || record.front() < 'A' || record.front() > 'Z') { return false; }
        for(char c : record)
        { if(!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')) { return false; } }
        return true;
    }

    /** Exact complete supported standard declarations; this establishes no authority. */
    inline constexpr bool official_standard_dependency_name(unsigned version, ::std::string_view name) noexcept
    {
        if(version == 1)
        {
            for(auto candidate : {::std::string_view{"std_msgs/Header"},
                                  ::std::string_view{"sensor_msgs/JointState"},
                                  ::std::string_view{"geometry_msgs/Accel"},
                                  ::std::string_view{"geometry_msgs/AccelStamped"},
                                  ::std::string_view{"geometry_msgs/AccelWithCovariance"},
                                  ::std::string_view{"geometry_msgs/AccelWithCovarianceStamped"},
                                  ::std::string_view{"geometry_msgs/Point"},
                                  ::std::string_view{"geometry_msgs/Point32"},
                                  ::std::string_view{"geometry_msgs/PointStamped"},
                                  ::std::string_view{"geometry_msgs/Polygon"},
                                  ::std::string_view{"geometry_msgs/PolygonStamped"},
                                  ::std::string_view{"geometry_msgs/Pose"},
                                  ::std::string_view{"geometry_msgs/PoseArray"},
                                  ::std::string_view{"geometry_msgs/PoseStamped"},
                                  ::std::string_view{"geometry_msgs/PoseWithCovariance"},
                                  ::std::string_view{"geometry_msgs/PoseWithCovarianceStamped"},
                                  ::std::string_view{"geometry_msgs/Quaternion"},
                                  ::std::string_view{"geometry_msgs/QuaternionStamped"},
                                  ::std::string_view{"geometry_msgs/Transform"},
                                  ::std::string_view{"geometry_msgs/TransformStamped"},
                                  ::std::string_view{"geometry_msgs/Twist"},
                                  ::std::string_view{"geometry_msgs/TwistStamped"},
                                  ::std::string_view{"geometry_msgs/TwistWithCovariance"},
                                  ::std::string_view{"geometry_msgs/TwistWithCovarianceStamped"},
                                  ::std::string_view{"geometry_msgs/Vector3"},
                                  ::std::string_view{"geometry_msgs/Vector3Stamped"},
                                  ::std::string_view{"geometry_msgs/Wrench"},
                                  ::std::string_view{"geometry_msgs/WrenchStamped"},
                                  ::std::string_view{"nav_msgs/Odometry"},
                                  ::std::string_view{"nav_msgs/Path"}})
            { if(name == candidate) { return true; } }
        }
        if(version == 2)
        {
            for(auto candidate : {::std::string_view{"std_msgs/msg/Header"},
                                  ::std::string_view{"sensor_msgs/msg/JointState"},
                                  ::std::string_view{"builtin_interfaces/msg/Time"},
                                  ::std::string_view{"builtin_interfaces/msg/Duration"},
                                  ::std::string_view{"geometry_msgs/msg/Accel"},
                                  ::std::string_view{"geometry_msgs/msg/AccelStamped"},
                                  ::std::string_view{"geometry_msgs/msg/AccelWithCovariance"},
                                  ::std::string_view{"geometry_msgs/msg/AccelWithCovarianceStamped"},
                                  ::std::string_view{"geometry_msgs/msg/Point"},
                                  ::std::string_view{"geometry_msgs/msg/Point32"},
                                  ::std::string_view{"geometry_msgs/msg/PointStamped"},
                                  ::std::string_view{"geometry_msgs/msg/Polygon"},
                                  ::std::string_view{"geometry_msgs/msg/PolygonStamped"},
                                  ::std::string_view{"geometry_msgs/msg/Pose"},
                                  ::std::string_view{"geometry_msgs/msg/PoseArray"},
                                  ::std::string_view{"geometry_msgs/msg/PoseStamped"},
                                  ::std::string_view{"geometry_msgs/msg/PoseWithCovariance"},
                                  ::std::string_view{"geometry_msgs/msg/PoseWithCovarianceStamped"},
                                  ::std::string_view{"geometry_msgs/msg/Quaternion"},
                                  ::std::string_view{"geometry_msgs/msg/QuaternionStamped"},
                                  ::std::string_view{"geometry_msgs/msg/Transform"},
                                  ::std::string_view{"geometry_msgs/msg/TransformStamped"},
                                  ::std::string_view{"geometry_msgs/msg/Twist"},
                                  ::std::string_view{"geometry_msgs/msg/TwistStamped"},
                                  ::std::string_view{"geometry_msgs/msg/TwistWithCovariance"},
                                  ::std::string_view{"geometry_msgs/msg/TwistWithCovarianceStamped"},
                                  ::std::string_view{"geometry_msgs/msg/Vector3"},
                                  ::std::string_view{"geometry_msgs/msg/Vector3Stamped"},
                                  ::std::string_view{"geometry_msgs/msg/Wrench"},
                                  ::std::string_view{"geometry_msgs/msg/WrenchStamped"},
                                  ::std::string_view{"nav_msgs/msg/Odometry"},
                                  ::std::string_view{"nav_msgs/msg/Path"}})
            { if(name == candidate) { return true; } }
        }
        return false;
    }
    inline constexpr bool official_standard_topic_name(unsigned version, ::std::string_view name) noexcept
    {
        if(!official_standard_dependency_name(version, name)) { return false; }
        if(version == 1) { return name != "std_msgs/Header" && name != "geometry_msgs/Twist"; }
        return name != "std_msgs/msg/Header" && name != "builtin_interfaces/msg/Time" &&
               name != "builtin_interfaces/msg/Duration" && name != "geometry_msgs/msg/Twist";
    }

    /** Closed original control names. Complete schemas can be dependencies;
     * only the two explicit Topics and seven Services may become endpoints.
     * No reserved package is opened generally, and a compiled row grants no
     * right to create, discover, publish, call, update or debug anything.
     */
    inline constexpr bool original_control_topic_name(unsigned version,::std::string_view name) noexcept
    {
        return version==2&&(name=="rcl_interfaces/msg/Log"||name=="rcl_interfaces/msg/ParameterEvent");
    }
    inline constexpr bool original_control_service_name(unsigned version,::std::string_view name) noexcept
    {
        if(version!=2)return false;
        for(auto candidate : {::std::string_view{"rcl_interfaces/srv/DescribeParameters"},::std::string_view{"rcl_interfaces/srv/GetParameterTypes"},::std::string_view{"rcl_interfaces/srv/GetParameters"},::std::string_view{"rcl_interfaces/srv/ListParameters"},::std::string_view{"rcl_interfaces/srv/SetParameters"},::std::string_view{"rcl_interfaces/srv/SetParametersAtomically"},::std::string_view{"type_description_interfaces/srv/GetTypeDescription"}})
        {if(name==candidate)return true;}
        return false;
    }
    inline constexpr bool original_control_record_name(unsigned version,::std::string_view name) noexcept
    {
        if(version!=2)return false;
        for(auto candidate : {::std::string_view{"builtin_interfaces/msg/Time"},
                              ::std::string_view{"rcl_interfaces/msg/FloatingPointRange"},
                              ::std::string_view{"rcl_interfaces/msg/IntegerRange"},
                              ::std::string_view{"rcl_interfaces/msg/ListParametersResult"},
                              ::std::string_view{"rcl_interfaces/msg/Log"},
                              ::std::string_view{"rcl_interfaces/msg/Parameter"},
                              ::std::string_view{"rcl_interfaces/msg/ParameterDescriptor"},
                              ::std::string_view{"rcl_interfaces/msg/ParameterEvent"},
                              ::std::string_view{"rcl_interfaces/msg/ParameterType"},
                              ::std::string_view{"rcl_interfaces/msg/ParameterValue"},
                              ::std::string_view{"rcl_interfaces/msg/SetParametersResult"},
                              ::std::string_view{"rcl_interfaces/srv/DescribeParameters_Request"},
                              ::std::string_view{"rcl_interfaces/srv/DescribeParameters_Response"},
                              ::std::string_view{"rcl_interfaces/srv/GetParameterTypes_Request"},
                              ::std::string_view{"rcl_interfaces/srv/GetParameterTypes_Response"},
                              ::std::string_view{"rcl_interfaces/srv/GetParameters_Request"},
                              ::std::string_view{"rcl_interfaces/srv/GetParameters_Response"},
                              ::std::string_view{"rcl_interfaces/srv/ListParameters_Request"},
                              ::std::string_view{"rcl_interfaces/srv/ListParameters_Response"},
                              ::std::string_view{"rcl_interfaces/srv/SetParameters_Request"},
                              ::std::string_view{"rcl_interfaces/srv/SetParameters_Response"},
                              ::std::string_view{"rcl_interfaces/srv/SetParametersAtomically_Request"},
                              ::std::string_view{"rcl_interfaces/srv/SetParametersAtomically_Response"},
                              ::std::string_view{"service_msgs/msg/ServiceEventInfo"},
                              ::std::string_view{"type_description_interfaces/msg/Field"},
                              ::std::string_view{"type_description_interfaces/msg/FieldType"},
                              ::std::string_view{"type_description_interfaces/msg/IndividualTypeDescription"},
                              ::std::string_view{"type_description_interfaces/msg/KeyValue"},
                              ::std::string_view{"type_description_interfaces/msg/TypeDescription"},
                              ::std::string_view{"type_description_interfaces/msg/TypeSource"},
                              ::std::string_view{"type_description_interfaces/srv/GetTypeDescription_Request"},
                              ::std::string_view{"type_description_interfaces/srv/GetTypeDescription_Response"}})
        {if(name==candidate)return true;}
        return false;
    }

    inline constexpr bool custom_topic_layout_valid(custom_topic_descriptor const& descriptor) noexcept
    {
        if(descriptor.bounded_layout)
        {
            custom_wire_extent extent{};
            return !descriptor.layout&&!descriptor.layout_count&&
                custom_wire_extent_of(*descriptor.bounded_layout,extent)&&
                extent.maximum==descriptor.wire_bytes&&extent.minimum==descriptor.minimum_wire_bytes;
        }
        if(descriptor.minimum_wire_bytes) { return false; }
        if(!descriptor.wire_bytes || !descriptor.layout_count || descriptor.wire_bytes > 32768 || descriptor.layout_count > 4096 ||
           (descriptor.layout_count && !descriptor.layout)) { return false; }
        ::std::uint64_t bytes{}, cells{};
        for(::std::size_t i{}; i != descriptor.layout_count; ++i)
        {
            auto const& field{descriptor.layout[i]};auto width{custom_scalar_width(field.kind)};
            if(!width || !field.count || field.count > 4096) { return false; }
            cells += field.count;bytes += ::std::uint64_t{field.count} * width;
            if(cells > 4096 || bytes > 32768) { return false; }
        }
        return bytes == descriptor.wire_bytes;
    }
    inline constexpr bool custom_topic_registry_well_formed(custom_topic_registry registry) noexcept
    {
        if(!custom_registry_counts_fit(registry.size,0,0) || (registry.size && !registry.data)) { return false; }
        ::std::uint64_t total_cells{};
        for(::std::size_t i{}; i != registry.size; ++i)
        {
            auto const& descriptor{registry.data[i]};
            if(!descriptor.wire_bytes ||
               (static_cast<unsigned>(descriptor.original_control)+static_cast<unsigned>(descriptor.official_standard)+
                static_cast<unsigned>(descriptor.complete_vendor)>1) ||
               !(descriptor.complete_vendor ? (descriptor.bounded_layout&&complete_vendor_endpoint_name(descriptor.ros_version,descriptor.type,"msg")) :
                 descriptor.original_control ? (descriptor.bounded_layout&&original_control_topic_name(descriptor.ros_version,descriptor.type)) :
                 descriptor.official_standard ? (descriptor.bounded_layout&&official_standard_topic_name(descriptor.ros_version,descriptor.type)) : custom_topic_name(descriptor.ros_version,descriptor.type)) || !custom_topic_layout_valid(descriptor) ||
               !custom_codec_identity(descriptor.codec_identity)) { return false; }
            if(descriptor.bounded_layout)
            {
                custom_wire_extent extent{};
                if(!custom_wire_extent_of(*descriptor.bounded_layout,extent)) { return false; }
                if(!custom_registry_cells_fit(total_cells,extent.cells)) { return false; }
                total_cells+=extent.cells;
            }
            for(::std::size_t field{}; field != descriptor.layout_count; ++field)
            {
                if(!custom_registry_cells_fit(total_cells,descriptor.layout[field].count)) { return false; }
                total_cells += descriptor.layout[field].count;
            }
            auto identity{descriptor.native_identity};
            if(descriptor.ros_version == 1)
            { if(identity.size() != 32 || !custom_hexadecimal(identity)) { return false; } }
            else if(identity.size() != 71 || identity.substr(0, 7) != "RIHS01_" || !custom_hexadecimal(identity.substr(7)))
            { return false; }
            for(::std::size_t previous{}; previous != i; ++previous)
            {
                auto const& other{registry.data[previous]};
                if(other.ros_version == descriptor.ros_version && other.type == descriptor.type) { return false; }
            }
        }
        return true;
    }
}

// The build supplies one reviewed generated header, shared by Worker and
// Gateways. It defines constexpr custom_topic_entries() in the namespace above.
// There is no runtime registration, Guest type upload, dlopen or default grant.
#if defined(UWVM_ROS_CUSTOM_TOPIC_MANIFEST)
#include UWVM_ROS_CUSTOM_TOPIC_MANIFEST
#else
namespace uwvm2::imported::ros::ipc
{
    inline constexpr custom_topic_registry custom_topic_entries() noexcept { return {nullptr, 0}; }
}
#endif

// The original control family has its own source-selected namespace/provider.
// It never defines the legacy bounded-interface macro or replaces vendor tables.
#if defined(UWVM_ROS_ORIGINAL_CONTROL_MANIFEST)
#include UWVM_ROS_ORIGINAL_CONTROL_MANIFEST
#else
namespace uwvm2::imported::ros::ipc::original_control_generated
{
    inline constexpr custom_topic_registry custom_topic_entries() noexcept {return {nullptr,0};}
    inline constexpr custom_service_registry custom_service_entries() noexcept {return {nullptr,0};}
    inline constexpr custom_action_registry custom_action_entries() noexcept {return {nullptr,0};}
}
#endif

namespace uwvm2::imported::ros::ipc
{
    namespace custom_registry_detail
    {
        // Check bounds before instantiating storage or dereferencing either
        // trusted table. Invalid input remains a whole-registry poison; it is
        // never truncated into an apparently valid prefix. Counts sum only
        // after the explicit compiled ceiling check, so arithmetic cannot wrap.
        template<class Registry>inline constexpr bool mergeable(Registry a,Registry b) noexcept
        {
            return custom_registry_counts_fit(a.size,b.size,0)&&
                (!a.size||a.data)&&(!b.size||b.data);
        }
        template<class Descriptor,::std::size_t N,class Registry>
        inline constexpr ::std::array<Descriptor,N> concatenate(Registry a,Registry b) noexcept
        {
            ::std::array<Descriptor,N> result{};
            // N==0 also covers malformed source tables: do not inspect them.
            if constexpr(N!=0)
            {
                ::std::size_t at{};
                for(::std::size_t i{};i<a.size;++i)result[at++]=a.data[i];
                for(::std::size_t i{};i<b.size;++i)result[at++]=b.data[i];
            }
            return result;
        }
#if defined(UWVM_ROS_ORIGINAL_CONTROL_MANIFEST)
        inline constexpr auto legacy_topics=custom_topic_entries();
        inline constexpr auto control_topics=original_control_generated::custom_topic_entries();
        inline constexpr bool topic_sources_valid=mergeable(legacy_topics,control_topics);
        inline constexpr ::std::size_t topic_count=topic_sources_valid?legacy_topics.size+control_topics.size:0;
        inline constexpr auto topics=concatenate<custom_topic_descriptor,topic_count>(legacy_topics,control_topics);
#endif
    }
    /** The two trusted source catalogs describe supported codecs, not rights.
     * With no original-control macro, return the exact old view/address. With
     * both catalogs, copied descriptor values point to their original immutable
     * child schemas. All consumers must source-recompile this registry seam.
     */
    inline constexpr custom_topic_registry effective_custom_topic_entries() noexcept
    {
#if defined(UWVM_ROS_ORIGINAL_CONTROL_MANIFEST)
        if(!custom_registry_detail::topic_sources_valid)return {nullptr,custom_registry_max_interfaces+1};
        return {custom_registry_detail::topics.data(),custom_registry_detail::topics.size()};
#else
        return custom_topic_entries();
#endif
    }
    inline constexpr bool custom_topic_registry_valid{custom_topic_registry_well_formed(effective_custom_topic_entries())};
    inline constexpr bool custom_interface_registries_valid() noexcept;
    inline constexpr bool custom_interface_registry_available() noexcept;

    inline constexpr custom_topic_descriptor const* find_custom_topic(unsigned version, ::std::string_view type) noexcept
    {
        // Validate the complete immutable table once at constant evaluation.
        // A malformed entry disables the entire table rather than admitting a
        // valid-looking prefix of an inconsistent provider.
        if(!custom_topic_registry_valid||!custom_interface_registry_available()) { return nullptr; }
        auto registry{effective_custom_topic_entries()};
        for(::std::size_t i{}; i != registry.size; ++i)
        { if(registry.data[i].ros_version == version && registry.data[i].type == type) { return registry.data + i; } }
        return nullptr;
    }

    /** Operation-aware canonical domain. A full original ParameterEvent Topic
     * must not shadow the existing param.subscribe-events projection, its
     * builtin identity, required fields or wire grammar merely by being linked.
     * Existing generic Topic rows retain their original selection semantics.
     */
    inline constexpr custom_topic_descriptor const* find_custom_topic_for_operation(unsigned version,
        ::std::string_view type,::std::string_view operation) noexcept
    {
        auto descriptor=find_custom_topic(version,type);
        if(descriptor&&descriptor->original_control&&operation!="topic.publish"&&operation!="topic.subscribe")return nullptr;
        return descriptor;
    }

    inline ::std::uint64_t custom_read_bits(::std::uint8_t const* bytes, unsigned width) noexcept
    {
        ::std::uint64_t bits{};
        for(unsigned i{}; i != width; ++i) { bits |= ::std::uint64_t{bytes[i]} << (8u * i); }
        return bits;
    }
    inline void custom_write_bits(::std::uint8_t* bytes, ::std::uint64_t bits, unsigned width) noexcept
    { for(unsigned i{}; i != width; ++i) { bytes[i] = static_cast<::std::uint8_t>(bits >> (8u * i)); } }

    inline bool validate_custom_topic(custom_topic_descriptor const& descriptor, ::std::uint8_t const* bytes,
                                      ::std::size_t size) noexcept
    {
        if(descriptor.bounded_layout)
        { return custom_topic_layout_valid(descriptor)&&validate_custom_wire(*descriptor.bounded_layout,bytes,size); }
        static_assert(sizeof(float) == 4 && sizeof(double) == 8 && ::std::numeric_limits<float>::is_iec559 &&
                      ::std::numeric_limits<double>::is_iec559, "custom fixed profile requires IEEE binary32/binary64");
        if(!custom_topic_layout_valid(descriptor) || size != descriptor.wire_bytes || (size && !bytes)) { return false; }
        ::std::size_t offset{};
        for(::std::size_t i{}; i != descriptor.layout_count; ++i)
        {
            auto const& field{descriptor.layout[i]};auto width{custom_scalar_width(field.kind)};
            for(::std::uint32_t cell{}; cell != field.count; ++cell)
            {
                if(field.kind == custom_scalar_kind::boolean && bytes[offset] > 1) { return false; }
                if(field.kind == custom_scalar_kind::f32)
                {
                    // Inspect exponent bits directly, so even signaling NaNs
                    // are refused without a floating operation or FP trap.
                    auto bits{static_cast<::std::uint32_t>(custom_read_bits(bytes + offset, 4))};
                    if((bits & 0x7f800000U) == 0x7f800000U) { return false; }
                }
                else if(field.kind == custom_scalar_kind::f64)
                {
                    auto bits{custom_read_bits(bytes + offset, 8)};
                    if((bits & 0x7ff0000000000000ULL) == 0x7ff0000000000000ULL) { return false; }
                }
                offset += width;
            }
        }
        return offset == size;
    }
}

#include "custom_interface.h"
