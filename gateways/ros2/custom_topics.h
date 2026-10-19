/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include "compat/node.h"

#include "../common/server.h"
#include "qos.h"
#include "compat/api.h"
#include "compat/identity.h"
#include "compat/installed_source.h"
#include "compat/installed_protocol.h"
#include <uwvm2/imported/ros/ipc/custom_type.h>
#include <uwvm2/imported/ros/ipc/rmw_endpoint.h>
#include <rclcpp/rclcpp.hpp>
#include <rosidl_typesupport_cpp/message_type_support.hpp>
#if UWVM_ROS2_HAVE_TYPE_DESCRIPTION
#include <rosidl_runtime_c/type_description/type_description__struct.h>
#endif
#if __has_include(<rosidl_runtime_cpp/traits.hpp>)
#include <rosidl_runtime_cpp/traits.hpp>
#else
#include <rosidl_generator_cpp/traits.hpp>
#endif
#include <atomic>
#include <algorithm>
#include <map>
#include <memory>
#include <mutex>
#include <string_view>
#include <type_traits>
#include <tuple>

// This header is selected by the trusted build, never by an IPC request, ROS
// graph discovery, or a Wasm import. It includes official generated ROS2 message
// headers and defines UWVM_ROS_CUSTOM_ROS2_TYPES(X). Each row supplies:
//   X(NativeType, official_name, native_RIHS_identity, codec_identity,
//     canonical_fixed_bytes, GeneratedCodec)
// An omitted provider compiles a registry with no native custom bindings.
#if defined(UWVM_ROS_CUSTOM_ROS2_HEADER) != defined(UWVM_ROS_CUSTOM_TOPIC_MANIFEST)
#error "ROS2 custom topics require the paired common manifest and native provider headers"
#endif
#if defined(UWVM_ROS_CUSTOM_ROS2_TYPES)
#error "The ROS2 custom type table must be defined by its selected native provider header"
#endif
#if defined(UWVM_ROS_CUSTOM_ROS2_HEADER)
#include UWVM_ROS_CUSTOM_ROS2_HEADER
#ifndef UWVM_ROS_CUSTOM_ROS2_TYPES
#error "The selected ROS2 custom provider did not define its native type table"
#endif
#else
#define UWVM_ROS_CUSTOM_ROS2_TYPES(X)
#endif

// All custom translation units use the same trusted build-time partition.
// Empty/builtin consumers need no out-of-line custom objects. This default
// also preserves existing custom build selections without a new required input.
#ifndef UWVM_ROS2_CUSTOM_TOPIC_SHARDS
#define UWVM_ROS2_CUSTOM_TOPIC_SHARDS 32
#endif

// An incomplete shared owner is sufficient for the untyped factory API.
// The actual typed endpoint implementation is visible only in shard TUs.
namespace uwvm2::ros_gateway::native_rmw_publisher { class endpoint; }
namespace uwvm2::ros_gateway::native_rmw_subscription { class endpoint; }

namespace uwvm2::ros_gateway::ros2_custom_topics
{
    #if UWVM_ROS2_HAVE_TYPE_DESCRIPTION
    /** Map the closed canonical scalar vocabulary to official RIHS01 type IDs. */
    inline unsigned native_scalar_type(std::string_view type,bool complete=false) noexcept
    {
        if(complete&&type=="byte") { return rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BYTE; }
        if(complete&&type=="char") { return rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_CHAR; }
        if(type=="bool") { return rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN; }
        if(type=="int8") { return rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT8; }
        if(type=="uint8") { return rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8; }
        if(type=="int16") { return rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT16; }
        if(type=="uint16") { return rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT16; }
        if(type=="int32") { return rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32; }
        if(type=="uint32") { return rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT32; }
        if(type=="int64") { return rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT64; }
        if(type=="uint64") { return rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT64; }
        if(type=="float32") { return rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT; }
        if(type=="float64") { return rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE; }
        if(type=="string") { return rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING; }
        return 0;
    }

    /** Compare a borrowed official counted string without unbounded strlen. */
    inline bool native_text(rosidl_runtime_c__String const& actual,std::string_view expected) noexcept
    {
        if(actual.size!=expected.size()||actual.size>actual.capacity) { return false; }
        if(expected.empty()) { return true; }
        return actual.data&&std::string_view{actual.data,actual.size}==expected;
    }

    inline std::size_t native_schema_index(ipc::custom_native_schema schema,std::string_view type) noexcept
    {
        for(std::size_t i{};i!=schema.record_count;++i) { if(schema.records[i].type==type) { return i; } }
        return schema.record_count;
    }

    /** Reject cycles, excessive depth and disconnected generated definitions. */
    inline bool native_schema_visit(ipc::custom_native_schema schema,std::size_t record,
        std::uint8_t* colors,std::uint8_t* heights,unsigned depth,bool complete=false) noexcept
    {
        if(depth>16||colors[record]==1) { return false; }
        if(colors[record]==2) { return depth+heights[record]-1<=16; }
        colors[record]=1;
        unsigned height{1};
        auto const& current{schema.records[record]};
        for(std::size_t i{};i!=current.field_count;++i)
        {
            auto const& field{current.fields[i]};if(native_scalar_type(field.type,complete)) { continue; }
            auto next{native_schema_index(schema,field.type)};
            if(next==schema.record_count||!native_schema_visit(schema,next,colors,heights,depth+1,complete)) { return false; }
            height=std::max(height,static_cast<unsigned>(heights[next])+1);
        }
        if(depth+height-1>16) { return false; }
        heights[record]=static_cast<std::uint8_t>(height);colors[record]=2;return true;
    }

    /** Declared scalar defaults require complete actual modern metadata.
     * Defaults are not part of RIHS01. This admission only describes supported
     * scalar shapes; native_record_matches still compares the exact counted
     * original normalized text for EVERY declared field. No legacy null-default
     * introspection pointer proves absence/value, and arrays/string/nested
     * defaults remain refused until their complete source grammar is supplied.
     */
    inline bool native_default_variant(ipc::custom_native_record const& expected,bool complete=false) noexcept
    {
        if(expected.field_count&&!expected.fields) { return false; }
        for(std::size_t i{};i!=expected.field_count;++i)
        {
            auto const& field{expected.fields[i]};
            if(complete&&field.has_declared_default&&field.native_default.empty())return false;
            if(field.native_default.empty()) { continue; }
            if(complete&&field.type=="string")
            {
                if(field.native_default.size()>32768||field.is_array||field.array_size||field.is_sequence||field.sequence_bound)return false;
                continue; // Full counted native text is independently exact below.
            }
            if(field.native_default.size()>128||field.is_array||field.array_size||field.is_sequence||
               field.sequence_bound||field.string_bound) { return false; }
            if(field.type=="bool")
            { if(field.native_default!="True"&&field.native_default!="False") { return false; }continue; }
            bool number{complete&&(field.type=="byte"||field.type=="char")};
            for(auto type:{"int8","uint8","int16","uint16","int32","uint32","int64","uint64","float32","float64"})
            { number=number||field.type==type; }
            if(!number) { return false; }
            // This lexical shape is only defensive DATA admission. The real
            // generated typed layout and full native default text are checked
            // independently; no number is parsed or used as permission here.
            for(char c:field.native_default)
            { if(!((c>='0'&&c<='9')||c=='-'||c=='+'||c=='.'||c=='e'||c=='E')) { return false; } }
        }
        return true;
    }

    /**
     * Compare every declared field, including order, against official metadata.
     * Supported declared scalar defaults must match actual full native default
     * text; unknown/default-erased literals,
     * alternate native capacities and partial projections fail. Dynamic fields are admitted only
     * through an exact finite bounded-profile schema; fixed-v1 keeps its original
     * fixed-only contract.
     */
    inline bool native_record_matches(rosidl_runtime_c__type_description__IndividualTypeDescription const& actual,
        ipc::custom_native_record const& expected,bool complete=false) noexcept
    {
        if(!native_text(actual.type_name,expected.type)||actual.fields.size!=expected.field_count||
           actual.fields.size>actual.fields.capacity||(actual.fields.size&&(!actual.fields.data||!expected.fields))) { return false; }
        bool defaults{};
        for(std::size_t i{};i!=expected.field_count;++i) { defaults=defaults||!expected.fields[i].native_default.empty()||(complete&&expected.fields[i].has_declared_default); }
        if(complete)
        {
            for(std::size_t i{};i!=expected.field_count;++i)
                if(expected.fields[i].has_declared_default==expected.fields[i].native_default.empty())return false;
        }
        if(defaults&&!native_default_variant(expected,complete)) { return false; }
        for(std::size_t i{};i!=expected.field_count;++i)
        {
            auto const& field{expected.fields[i]};auto const& native{actual.fields.data[i]};
            // Preserve the original strict empty representation for every
            // undeclared default. Nonempty known literals are compared as
            // actual bounded counted text; no textual/numeric coercion.
            bool default_matches{field.native_default.empty()?
                !native.default_value.data&&!native.default_value.size&&!native.default_value.capacity:
                native_text(native.default_value,field.native_default)};
            if(field.name.empty()||!native_text(native.name,field.name)||!default_matches||native.type.string_capacity!=field.string_bound||
               native.type.capacity!=(field.is_array?field.array_size:field.is_sequence?field.sequence_bound:0)||
               (field.is_array&&field.is_sequence)||(field.type!="string"&&field.string_bound)) { return false; }
            auto scalar{native_scalar_type(field.type,complete)};
            bool nested{!scalar};
            if(nested&&!(complete?ipc::complete_vendor_native_name(2,field.type):ipc::custom_native_record_name(2,field.type))) { return false; }
            unsigned type{nested?rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE:scalar};
            if(field.type=="string"&&field.string_bound) {type=rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOUNDED_STRING;}
            if(field.is_array)
            {
                if(!field.array_size||field.array_size>4096) { return false; }
                type+=rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_ARRAY-
                    rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE;
            }
            else if(field.is_sequence)
            {
                if(field.array_size||field.sequence_bound>4096) {return false;}
                type+=(field.sequence_bound?rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE:
                    rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_UNBOUNDED_SEQUENCE)-
                    rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE;
            }
            else if(field.array_size) { return false; }
            if(native.type.type_id!=type||!native_text(native.type.nested_type_name,nested?field.type:std::string_view{})) { return false; }
        }
        return true;
    }

    /**
     * @brief Verify the complete official native root and dependency closure.
     *
     * A genuine native hash supplied beside an unrelated catalog is insufficient:
     * every field in every referenced record must match the generated schema.
     * Table sizes and recursion are bounded; this comparison allocates nothing.
     * Reference ordering is immaterial, while member declaration order is exact.
     */
    template<std::size_t MaximumRecords>
    inline bool matches_native_schema_bounded(rosidl_runtime_c__type_description__TypeDescription const& actual,
        ipc::custom_native_schema expected,std::string_view root,bool complete=false) noexcept
    {
        static_assert(MaximumRecords==128||MaximumRecords==256,"actual Message/protocol graph bound required");
        if(!expected.record_count||expected.record_count>MaximumRecords||!expected.records||
           actual.referenced_type_descriptions.size!=expected.record_count-1||
           actual.referenced_type_descriptions.size>actual.referenced_type_descriptions.capacity||
           (actual.referenced_type_descriptions.size&&!actual.referenced_type_descriptions.data)) { return false; }
        for(std::size_t i{};i!=expected.record_count;++i)
        {
            auto const& record{expected.records[i]};
            if(!(complete?ipc::complete_vendor_native_name(2,record.type):ipc::custom_native_record_name(2,record.type))||record.field_count>4096||(record.field_count&&!record.fields)) { return false; }
            for(std::size_t previous{};previous!=i;++previous)
            { if(record.type==expected.records[previous].type) { return false; } }
        }
        auto root_index{native_schema_index(expected,root)};if(root_index==expected.record_count) { return false; }
        std::uint8_t colors[MaximumRecords]{},heights[MaximumRecords]{};
        if(!native_schema_visit(expected,root_index,colors,heights,1,complete)) { return false; }
        for(std::size_t i{};i!=expected.record_count;++i) { if(colors[i]!=2) { return false; } }
        if(!native_record_matches(actual.type_description,expected.records[root_index],complete)) { return false; }
        for(std::size_t i{};i!=expected.record_count;++i)
        {
            if(i==root_index) { continue; }
            auto const& record{expected.records[i]};
            std::size_t matched{};
            for(std::size_t j{};j!=actual.referenced_type_descriptions.size;++j)
            {
                auto const& reference{actual.referenced_type_descriptions.data[j]};
                if(native_text(reference.type_name,record.type))
                { ++matched;if(!native_record_matches(reference,record,complete)) { return false; } }
            }
            if(matched!=1) { return false; }
        }
        return true;
    }
    /** Keep the existing Message128 budget; complete protocol roots have an
     * independent finite256-record bound because they include real Events,
     * ServiceEventInfo/Time and Action-generated protocol messages/services. */
    inline bool matches_native_schema(rosidl_runtime_c__type_description__TypeDescription const& actual,
        ipc::custom_native_schema expected,std::string_view root,bool complete=false) noexcept
    { return matches_native_schema_bounded<128>(actual,expected,root,complete); }
    inline bool matches_native_interface_schema(rosidl_runtime_c__type_description__TypeDescription const& actual,
        ipc::custom_native_schema expected,std::string_view root,bool complete=false) noexcept
    { return matches_native_schema_bounded<256>(actual,expected,root,complete); }

    #endif

    /** Attest the root after independently checking every actual native
     * component. A pre-RIHS SDK has no baked hash: its complete trusted official
     * protocol schema must be present and its RIHS01 must be recomputed. A
     * malformed or absent hash on a hash-capable SDK never enables fallback. */
    template<class Native,bool Complete=false,class Support>
    bool native_interface_contract(Support const* support,std::string_view root,std::string_view expected)
    {
        if(!support) { return false; }
        constexpr bool complete=Complete;
        if constexpr(complete&&!ipc::complete_vendor_native_traits<Native>::enabled)return false;
        if constexpr(complete) {if(!ipc::complete_vendor_native_traits<Native>::constants())return false;}
        if constexpr(ros2_compat::has_baked_hash<Support>::value)
        {
            if(ros2_compat::baked_hash(support)!=expected) { return false; }
#if UWVM_ROS2_HAVE_TYPE_DESCRIPTION
            if(!support->get_type_description_func) { return false; }
            auto description{support->get_type_description_func(support)};
            // RIHS01 omits defaults. A matching root hash/name cannot prove
            // the complete actual Service/Action protocol/default closure.
            return description&&matches_native_interface_schema(*description,
                ipc::custom_native_interface_schema<Native>::schema(),root,complete);
#else
            // A modern hash-capable SDK without full descriptions cannot
            // attest defaults; never substitute the legacy-derived path.
            return false;
#endif
        }
        else
        {
            // Complete legacy interfaces require independently captured IDL,
            // typed payload proofs and real Service/Action associations.
            // Without an explicitly compiled proof this remains refused.
            if constexpr(complete)return ros2_compat::installed_source::qualify_protocol<Native>(support,root,expected);
            return ros2_compat::matches_rihs(ipc::custom_native_interface_schema<Native>::schema(),root,expected);
        }
    }

    /**
     * @brief Qualify one statically generated, fixed-layout ROS2 topic binding.
     *
     * The common descriptor and native provider must agree independently about
     * the official type, native hash, codec fingerprint, and fixed wire length.
     * A trusted policy must explicitly select both identities. Registration
     * establishes implemented support; it creates no entity or capability.
     */
    template<typename Codec,typename=void> struct bounded_generated_codec : std::false_type {};
    template<typename Codec> struct bounded_generated_codec<Codec,std::void_t<decltype(Codec::bounded_profile)>> : std::bool_constant<Codec::bounded_profile> {};

    template<typename Native,typename Codec>
    struct binding
    {
        static_assert(rosidl_generator_traits::is_message<Native>::value,
            "a custom ROS2 topic binding requires an official generated message");
        static_assert(rosidl_generator_traits::has_fixed_size<Native>::value||bounded_generated_codec<Codec>::value,
            "dynamic ROS2 fields require a complete bounded generated codec");

        static bool native_contract()
        {
            // Native support and generated schemas are immutable build inputs.
            // Qualify once per typed binding, during startup before entities or
            // executor threads exist; each call still verifies its full policy
            // and descriptor identities. A refused binding stays refused.
            static bool const qualified{[] {
                constexpr bool complete=ipc::complete_vendor_codec<Codec>::value;
                if constexpr(complete)
                {if(!ipc::complete_vendor_native_traits<Native>::enabled||!ipc::complete_vendor_native_traits<Native>::constants())return false;}
                std::string_view name{Codec::type};
                auto generated_name{rosidl_generator_traits::name<Native>()};
                if(!generated_name) { return false; }
                auto support{rosidl_typesupport_cpp::get_message_type_support_handle<Native>()};
                if(!support) { return false; }
#if UWVM_ROS2_HAVE_TYPE_DESCRIPTION
                if(std::string_view{generated_name}!=name||!support->get_type_hash_func||!support->get_type_description_func) { return false; }
                auto hash{support->get_type_hash_func(support)};
                if(!hash||hash->version!=1||
                   !native_io::matches_hexadecimal(hash->value,sizeof hash->value,Codec::native_identity,"RIHS01_")) { return false; }
                auto description{support->get_type_description_func(support)};
                return description&&matches_native_schema(*description,Codec::native_schema(),name,complete);
#else
                if constexpr(complete)
                    return ros2_compat::installed_source::qualify<Native>(Codec::native_schema(),name,Codec::native_identity);
                // Introspection checks the complete canonical name, ordered
                // field types/capacities and dependency closure. Old pre-IDL
                // SDKs may spell their C++ trait name without /msg/ or /srv/.
                if constexpr(ros2_compat::has_baked_hash<std::remove_pointer_t<decltype(support)>>::value)
                { if(ros2_compat::baked_hash(support)!=Codec::native_identity) { return false; } }
                return ros2_compat::matches_schema(ros2_compat::message_members_for<Native>(),Codec::native_schema(),name)&&
                    ros2_compat::matches_rihs(Codec::native_schema(),name,Codec::native_identity);
#endif
            }()};
            return qualified;
        }

        static bool known(ros::capability_spec const& cap,std::string_view name,
            std::string_view native_identity,std::string_view codec_identity,
            std::size_t wire_bytes)
        {
            if((cap.operation!="topic.publish"&&cap.operation!="topic.subscribe")||std::string_view{cap.type}!=name) { return false; }
            auto const* descriptor{ipc::find_custom_topic(2,name)};
            if(!rosidl_generator_traits::has_fixed_size<Native>::value&&(!descriptor||!descriptor->bounded_layout)) {return false;}
            auto selected{cap.limits.find("adapter-codec-identity")};
            if(name!=Codec::type||native_identity!=Codec::native_identity||codec_identity!=Codec::codec_identity||
               wire_bytes!=Codec::wire_bytes||!descriptor||descriptor->complete_vendor!=ipc::complete_vendor_codec<Codec>::value||descriptor->ros_version!=2||descriptor->type!=name||
               descriptor->native_identity!=native_identity||descriptor->codec_identity!=codec_identity||
               descriptor->wire_bytes!=wire_bytes||std::string_view{cap.type_identity}!=native_identity||
               selected==cap.limits.end()||std::string_view{selected->second}!=codec_identity) { return false; }

            // Official generated traits attest the C++ specialization's ROS
            // name. The Jazzy TypeSupport hash and description name attest the
            // loaded generated implementation, before any DDS entity exists.
            return native_contract();
        }
    };

    /** Return true only for exactly one compiled native provider row. */
    inline bool known(ros::capability_spec const& cap)
    {
        unsigned matches{};bool qualified{};
        (void)cap;
#define UWVM_ROS2_CUSTOM_KNOWN(Native,Name,NativeId,CodecId,Bytes,Codec) \
        if(cap.type==Name) { ++matches;qualified=binding<Native,Codec>::known(cap,Name,NativeId,CodecId,Bytes); }
        UWVM_ROS_CUSTOM_ROS2_TYPES(UWVM_ROS2_CUSTOM_KNOWN)
#undef UWVM_ROS2_CUSTOM_KNOWN
        return matches==1&&qualified;
    }

    class adapter;
    namespace detail
    {
        template<typename Native,typename Codec> struct topic_row
        {
            using native_type=Native;
            using codec_type=Codec;
        };
        struct topic_end {};
        // The final sentinel permits an empty table without a trailing-comma
        // extension. Ordinals come from the exact provider macro, never a
        // separately maintained list of names or a projected CMake row count.
#define UWVM_ROS2_CUSTOM_ROW(Native,Name,NativeId,CodecId,Bytes,Codec) topic_row<Native,Codec>,
        using topic_rows=std::tuple<UWVM_ROS_CUSTOM_ROS2_TYPES(UWVM_ROS2_CUSTOM_ROW) topic_end>;
#undef UWVM_ROS2_CUSTOM_ROW
        inline constexpr std::size_t topic_count=std::tuple_size<topic_rows>::value-1;
        // Untyped row facts come from the SAME complete provider macro. Reading
        // names/identities here does not instantiate a native rclcpp endpoint.
        // A shard still qualifies every field against its exact Native/Codec.
        struct owned_publisher_row
        {
            std::string_view name,native_identity,codec_identity;
            std::size_t wire_bytes;
        };
#define UWVM_ROS2_CUSTOM_OWNED_PUBLISHER_ROW(Native,Name,NativeId,CodecId,Bytes,Codec) \
        owned_publisher_row{Name,NativeId,CodecId,Bytes},
        inline constexpr owned_publisher_row owned_publisher_rows[]{
            UWVM_ROS_CUSTOM_ROS2_TYPES(UWVM_ROS2_CUSTOM_OWNED_PUBLISHER_ROW) {}};
#undef UWVM_ROS2_CUSTOM_OWNED_PUBLISHER_ROW
        static_assert(sizeof(owned_publisher_rows)/sizeof(owned_publisher_rows[0])==topic_count+1,
            "owned publisher metadata must cover the complete original provider table");
        inline constexpr std::size_t topic_shards=UWVM_ROS2_CUSTOM_TOPIC_SHARDS;
        static_assert(topic_shards>=1&&topic_shards<=256,"custom topic shard count must be1..256");

        template<std::size_t Shard,std::size_t Shards> struct topic_shard
        {
            static ipc::call_result publish(adapter&,std::size_t,ros::capability_spec const&,
                std::vector<std::uint8_t> const&);
            static ipc::call_result subscribe(adapter&,std::size_t,ros::capability_spec const&,
                std::shared_ptr<event_queue>);
            static std::uint32_t create_owned_rmw_publisher(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&,std::size_t,
                ros::capability_spec const&,ipc::rmw_endpoint::binding const&,std::uint64_t,
                std::shared_ptr<native_rmw_publisher::endpoint>&,ipc::rmw_endpoint::packet&) noexcept;
            static std::uint32_t create_owned_rmw_subscription(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&,std::size_t,
                ros::capability_spec const&,ipc::rmw_endpoint::binding const&,std::uint64_t,
                std::shared_ptr<native_rmw_subscription::endpoint>&,ipc::rmw_endpoint::packet&) noexcept;
        private:
            template<std::size_t Index> static std::uint32_t create_owned_rmw_publisher_at(
                ::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&,std::size_t,ros::capability_spec const&,
                ipc::rmw_endpoint::binding const&,std::uint64_t,
                std::shared_ptr<native_rmw_publisher::endpoint>&,ipc::rmw_endpoint::packet&) noexcept;
            template<std::size_t Index> static std::uint32_t create_owned_rmw_subscription_at(
                ::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&,std::size_t,ros::capability_spec const&,
                ipc::rmw_endpoint::binding const&,std::uint64_t,
                std::shared_ptr<native_rmw_subscription::endpoint>&,ipc::rmw_endpoint::packet&) noexcept;
            template<std::size_t Index> static ipc::call_result publish_owned(adapter&,std::size_t,
                ros::capability_spec const&,std::vector<std::uint8_t> const&);
            template<std::size_t Index> static ipc::call_result subscribe_owned(adapter&,std::size_t,
                ros::capability_spec const&,std::shared_ptr<event_queue>);
        };

        // Only declarations are visible to main and the light dispatcher.
        // The rclcpp template definitions live in custom_topics_impl.h, which
        // is included exclusively by the generated per-shard translation units.
        ipc::call_result dispatch_publish(adapter&,std::size_t,ros::capability_spec const&,
            std::vector<std::uint8_t> const&);
        ipc::call_result dispatch_subscribe(adapter&,std::size_t,ros::capability_spec const&,
            std::shared_ptr<event_queue>);
        std::uint32_t dispatch_create_owned_rmw_publisher(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&,std::size_t,
            ros::capability_spec const&,ipc::rmw_endpoint::binding const&,std::uint64_t,
            std::shared_ptr<native_rmw_publisher::endpoint>&,ipc::rmw_endpoint::packet&) noexcept;
        std::uint32_t dispatch_create_owned_rmw_subscription(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&,std::size_t,
            ros::capability_spec const&,ipc::rmw_endpoint::binding const&,std::uint64_t,
            std::shared_ptr<native_rmw_subscription::endpoint>&,ipc::rmw_endpoint::packet&) noexcept;
    }

    /** Create a separately owned actual custom publisher, without the legacy
     * adapter's lazy alias map. The authenticated caller must reserve its owner,
     * binding, native incarnation and rollback before this call; table membership
     * grants nothing. Output and receipt remain unchanged on every refusal.
     *
     * Only untyped row facts are inspected here. In particular, calling the old
     * all-table known(cap) would instantiate every binding in main, defeating
     * sharding. Full Native/schema/identity/codec qualification runs in exactly
     * the selected shard before its actual typed endpoint creates a publisher.
     * An empty provider returns95 without requiring any custom shard objects.
     */
    inline std::uint32_t create_owned_rmw_publisher(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,
        ros::capability_spec const& cap,ipc::rmw_endpoint::binding const& owner,std::uint64_t incarnation,
        std::shared_ptr<native_rmw_publisher::endpoint>& output,ipc::rmw_endpoint::packet& observed) noexcept
    {
        if(!node||!ipc::rmw_endpoint::valid(owner)||!incarnation||cap.operation!="topic.publish"){return 22;}
        if constexpr(detail::topic_count==0){return 95;}
        else
        {
            std::size_t matched{},type_index{detail::topic_count};
            for(std::size_t i{};i!=detail::topic_count;++i)
            {if(std::string_view{cap.type}==detail::owned_publisher_rows[i].name){++matched;type_index=i;}}
            if(matched!=1){return 95;}
            return detail::dispatch_create_owned_rmw_publisher(node,type_index,cap,owner,incarnation,output,observed);
        }
    }

    /** Create a separately owned actual custom subscription, without the legacy
     * adapter's lazy alias map. The authenticated caller must reserve its owner,
     * binding, native incarnation and rollback before this call; table membership
     * grants nothing. Output and receipt remain unchanged on every refusal.
     *
     * Reuse the same complete macro-derived untyped row facts as the publisher.
     * No second table can project or reorder the provider.
     * Only untyped row facts are inspected here. In particular, calling the old
     * all-table known(cap) would instantiate every binding in main, defeating
     * sharding. Full Native/schema/identity/codec qualification runs in exactly
     * the selected shard before its actual typed endpoint creates a subscription.
     * An empty provider returns95 without requiring any custom shard objects.
     */
    inline std::uint32_t create_owned_rmw_subscription(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,
        ros::capability_spec const& cap,ipc::rmw_endpoint::binding const& owner,std::uint64_t incarnation,
        std::shared_ptr<native_rmw_subscription::endpoint>& output,ipc::rmw_endpoint::packet& observed) noexcept
    {
        if(!node||!ipc::rmw_endpoint::valid(owner)||!incarnation||cap.operation!="topic.subscribe"){return 22;}
        if constexpr(detail::topic_count==0){return 95;}
        else
        {
            std::size_t matched{},type_index{detail::topic_count};
            for(std::size_t i{};i!=detail::topic_count;++i)
            {if(std::string_view{cap.type}==detail::owned_publisher_rows[i].name){++matched;type_index=i;}}
            if(matched!=1){return 95;}
            return detail::dispatch_create_owned_rmw_subscription(node,type_index,cap,owner,incarnation,output,observed);
        }
    }

    /**
     * @brief Session-owned typed custom topic entities.
     *
     * Native messages are converted field by field; canonical bytes are never
     * passed to a generic CDR deserializer. Publishers and subscriptions remain
     * owned by this authenticated Gateway session. Closing an alias disables
     * ingress before releasing its native entity and wakes the executor.
     */
    class adapter
    {
        template<std::size_t,std::size_t> friend struct detail::topic_shard;
        struct publisher_entry
        {
            ros::capability_spec capability;
            std::shared_ptr<rclcpp::PublisherBase> entity;
        };
        struct subscriber_entry
        {
            std::shared_ptr<std::atomic<bool>> verified;
            std::weak_ptr<event_queue> queue;
            std::shared_ptr<rclcpp::SubscriptionBase> entity;
        };
        std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> node_;
        std::map<std::string,publisher_entry> publishers_;
        std::map<std::string,subscriber_entry> subscribers_;

        void notify() noexcept
        {
            try { ros2_compat::notify(node_); }
            catch(...) {} // Native entity retirement also runs during shutdown.
        }

        template<typename Native,typename Codec>
        ipc::call_result publish_bound(ros::capability_spec const& cap,
            std::vector<std::uint8_t> const& data);
        template<typename Native,typename Codec>
        ipc::call_result subscribe_bound(ros::capability_spec const& cap,std::shared_ptr<event_queue> queue);
    public:
        explicit adapter(std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> node):node_{std::move(node)} {}
        adapter(adapter const&)=delete;
        adapter& operator=(adapter const&)=delete;
        ~adapter()
        {
            for(auto const& item:subscribers_)
            { item.second.verified->store(false,std::memory_order_release);if(auto queue=item.second.queue.lock()) { queue->deactivate(); } }
            subscribers_.clear();publishers_.clear();notify();
        }

        ipc::call_result publish(ros::capability_spec const& cap,std::vector<std::uint8_t> const& data)
        {
            (void)data;
            if(cap.operation!="topic.publish"||!known(cap)) { return {95,{}}; }
            std::size_t type_index{};(void)type_index;
#define UWVM_ROS2_CUSTOM_PUBLISH(Native,Name,NativeId,CodecId,Bytes,Codec) \
            if(cap.type==Name) { return detail::dispatch_publish(*this,type_index,cap,data); } ++type_index;
            UWVM_ROS_CUSTOM_ROS2_TYPES(UWVM_ROS2_CUSTOM_PUBLISH)
#undef UWVM_ROS2_CUSTOM_PUBLISH
            return {95,{}};
        }
        ipc::call_result subscribe(ros::capability_spec const& cap,std::shared_ptr<event_queue> queue)
        {
            (void)queue;
            if(cap.operation!="topic.subscribe"||!known(cap)) { return {95,{}}; }
            std::size_t type_index{};(void)type_index;
#define UWVM_ROS2_CUSTOM_SUBSCRIBE(Native,Name,NativeId,CodecId,Bytes,Codec) \
            if(cap.type==Name) { return detail::dispatch_subscribe(*this,type_index,cap,std::move(queue)); } ++type_index;
            UWVM_ROS_CUSTOM_ROS2_TYPES(UWVM_ROS2_CUSTOM_SUBSCRIBE)
#undef UWVM_ROS2_CUSTOM_SUBSCRIBE
            return {95,{}};
        }
        void close(std::string const& alias) noexcept
        {
            auto found{subscribers_.find(alias)};
            if(found!=subscribers_.end())
            { found->second.verified->store(false,std::memory_order_release);if(auto queue=found->second.queue.lock()) { queue->deactivate(); }subscribers_.erase(found); }
            publishers_.erase(alias);notify();
        }
    };
}
