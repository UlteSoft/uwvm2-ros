/*
 * Real common-registry, schema, wire and capability-manager source contracts.
 *
 * No ROS SDK object, native type hash, capability grant or execution result is
 * synthesized. The input is an actual reviewed control_manifest.hpp generated
 * separately. Preparation input has blank native identities. Frozen input must
 * come from independently exported real installed native metadata. Structural
 * admission of such a declaration still does not prove a Native adapter.
 *
 * This file is intentionally included a second time, ONLY from the production
 * custom_type.h ORIGINAL-CONTROL trusted-manifest seam. In that phase it includes the genuine
 * manifest with just its topic-entry function renamed, then publishes either
 * exactly that table or one deliberately damaged declaration. This lets the
 * poisoned executable exercise the actual constexpr whole-table refusal and
 * real lookup functions, without duplicating their implementation.
 */
#if defined(UWVM_CONTROL_CONTRACT_MANIFEST_PHASE)

#define custom_topic_entries uwvm_control_original_topic_entries
#include UWVM_CONTROL_PROVIDED_MANIFEST
#undef custom_topic_entries

namespace uwvm2::imported::ros::ipc::original_control_generated
{
#if UWVM_CONTROL_POISON_TABLE
    static_assert(uwvm_control_original_topic_entries().size == 2,
                  "Poison input must be the complete real two-Topic control manifest");
    inline constexpr auto uwvm_control_poisoned_topics = [] {
        auto const original = uwvm_control_original_topic_entries();
        ::std::array<custom_topic_descriptor, 2> rows{original.data[0], original.data[1]};
        // Preserve both real native identities. Only a required codec
        // declaration is missing, so the valid-looking first row must also be
        // unavailable through actual production lookups.
        rows[1].codec_identity = {};
        return rows;
    }();
    inline constexpr custom_topic_registry custom_topic_entries() noexcept
    { return {uwvm_control_poisoned_topics.data(), uwvm_control_poisoned_topics.size()}; }
#else
    inline constexpr custom_topic_registry custom_topic_entries() noexcept
    { return uwvm_control_original_topic_entries(); }
#endif
}

#else

#if !defined(UWVM_CONTROL_CONTRACT_SELF) || !defined(UWVM_CONTROL_PROVIDED_MANIFEST) || \
    !defined(UWVM_CONTROL_EXPECT_FROZEN) || !defined(UWVM_CONTROL_POISON_TABLE)
#error "Use the explicit preparation/frozen CMake targets with reviewed generated products"
#endif
#if !defined(UWVM_ROS_ORIGINAL_CONTROL_MANIFEST) || !defined(UWVM_CONTROL_HAS_LEGACY)
#error "Use the independent original-control manifest seam and explicit legacy-provider selection"
#endif

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#define UWVM_CONTROL_CONTRACT_MANIFEST_PHASE 1
#include <uwvm2/imported/ros/capability/manager.h>
#include <uwvm2/imported/ros/ipc/custom_interface.h>
#undef UWVM_CONTROL_CONTRACT_MANIFEST_PHASE

// Actual C records and codecs from the SAME generator product as the manifest.
// Their complete definitions are used; there are no stand-in native structs.
#include "bounded_records.h"

namespace control_contract
{
    namespace ros = ::uwvm2::imported::ros;
    namespace ipc = ::uwvm2::imported::ros::ipc;
    namespace control = ::uwvm2::imported::ros::ipc::original_control_generated;
    using ros::version;
    using ros::errc;
    using namespace ::std::literals;

    inline constexpr ::std::array records{
        "builtin_interfaces/msg/Time"sv,
        "rcl_interfaces/msg/FloatingPointRange"sv,
        "rcl_interfaces/msg/IntegerRange"sv,
        "rcl_interfaces/msg/ListParametersResult"sv,
        "rcl_interfaces/msg/Log"sv,
        "rcl_interfaces/msg/Parameter"sv,
        "rcl_interfaces/msg/ParameterDescriptor"sv,
        "rcl_interfaces/msg/ParameterEvent"sv,
        "rcl_interfaces/msg/ParameterType"sv,
        "rcl_interfaces/msg/ParameterValue"sv,
        "rcl_interfaces/msg/SetParametersResult"sv,
        "rcl_interfaces/srv/DescribeParameters_Request"sv,
        "rcl_interfaces/srv/DescribeParameters_Response"sv,
        "rcl_interfaces/srv/GetParameterTypes_Request"sv,
        "rcl_interfaces/srv/GetParameterTypes_Response"sv,
        "rcl_interfaces/srv/GetParameters_Request"sv,
        "rcl_interfaces/srv/GetParameters_Response"sv,
        "rcl_interfaces/srv/ListParameters_Request"sv,
        "rcl_interfaces/srv/ListParameters_Response"sv,
        "rcl_interfaces/srv/SetParameters_Request"sv,
        "rcl_interfaces/srv/SetParameters_Response"sv,
        "rcl_interfaces/srv/SetParametersAtomically_Request"sv,
        "rcl_interfaces/srv/SetParametersAtomically_Response"sv,
        "service_msgs/msg/ServiceEventInfo"sv,
        "type_description_interfaces/msg/Field"sv,
        "type_description_interfaces/msg/FieldType"sv,
        "type_description_interfaces/msg/IndividualTypeDescription"sv,
        "type_description_interfaces/msg/KeyValue"sv,
        "type_description_interfaces/msg/TypeDescription"sv,
        "type_description_interfaces/msg/TypeSource"sv,
        "type_description_interfaces/srv/GetTypeDescription_Request"sv,
        "type_description_interfaces/srv/GetTypeDescription_Response"sv};
    inline constexpr ::std::array topics{
        "rcl_interfaces/msg/Log"sv, "rcl_interfaces/msg/ParameterEvent"sv};
    inline constexpr ::std::array services{
        "rcl_interfaces/srv/DescribeParameters"sv,
        "rcl_interfaces/srv/GetParameterTypes"sv,
        "rcl_interfaces/srv/GetParameters"sv,
        "rcl_interfaces/srv/ListParameters"sv,
        "rcl_interfaces/srv/SetParameters"sv,
        "rcl_interfaces/srv/SetParametersAtomically"sv,
        "type_description_interfaces/srv/GetTypeDescription"sv};

    // Exact globals emitted from the actual complete control catalog. This is
    // an inventory association, not a second registry or a hand-written hash.
    inline constexpr ::std::array record_descriptors{
        &control::custom_record_Ros2Builtin_interfacesMsgTime,
        &control::custom_record_Ros2Rcl_interfacesMsgFloatingPointRange,
        &control::custom_record_Ros2Rcl_interfacesMsgIntegerRange,
        &control::custom_record_Ros2Rcl_interfacesMsgListParametersResult,
        &control::custom_record_Ros2Rcl_interfacesMsgLog,
        &control::custom_record_Ros2Rcl_interfacesMsgParameter,
        &control::custom_record_Ros2Rcl_interfacesMsgParameterDescriptor,
        &control::custom_record_Ros2Rcl_interfacesMsgParameterEvent,
        &control::custom_record_Ros2Rcl_interfacesMsgParameterType,
        &control::custom_record_Ros2Rcl_interfacesMsgParameterValue,
        &control::custom_record_Ros2Rcl_interfacesMsgSetParametersResult,
        &control::custom_record_Ros2Rcl_interfacesSrvDescribeParameters_Request,
        &control::custom_record_Ros2Rcl_interfacesSrvDescribeParameters_Response,
        &control::custom_record_Ros2Rcl_interfacesSrvGetParameterTypes_Request,
        &control::custom_record_Ros2Rcl_interfacesSrvGetParameterTypes_Response,
        &control::custom_record_Ros2Rcl_interfacesSrvGetParameters_Request,
        &control::custom_record_Ros2Rcl_interfacesSrvGetParameters_Response,
        &control::custom_record_Ros2Rcl_interfacesSrvListParameters_Request,
        &control::custom_record_Ros2Rcl_interfacesSrvListParameters_Response,
        &control::custom_record_Ros2Rcl_interfacesSrvSetParameters_Request,
        &control::custom_record_Ros2Rcl_interfacesSrvSetParameters_Response,
        &control::custom_record_Ros2Rcl_interfacesSrvSetParametersAtomically_Request,
        &control::custom_record_Ros2Rcl_interfacesSrvSetParametersAtomically_Response,
        &control::custom_record_Ros2Service_msgsMsgServiceEventInfo,
        &control::custom_record_Ros2Type_description_interfacesMsgField,
        &control::custom_record_Ros2Type_description_interfacesMsgFieldType,
        &control::custom_record_Ros2Type_description_interfacesMsgIndividualTypeDescription,
        &control::custom_record_Ros2Type_description_interfacesMsgKeyValue,
        &control::custom_record_Ros2Type_description_interfacesMsgTypeDescription,
        &control::custom_record_Ros2Type_description_interfacesMsgTypeSource,
        &control::custom_record_Ros2Type_description_interfacesSrvGetTypeDescription_Request,
        &control::custom_record_Ros2Type_description_interfacesSrvGetTypeDescription_Response};

    static_assert(records.size() == 32 && record_descriptors.size() == records.size() && topics.size() == 2 && services.size() == 7);
    static_assert(control::custom_topic_entries().size == 2);
    static_assert(control::custom_service_entries().size == 7);
    static_assert(control::custom_action_entries().size == 0);
    static_assert(ipc::effective_custom_topic_entries().size == ipc::custom_topic_entries().size + 2);
    static_assert(ipc::effective_custom_service_entries().size == ipc::custom_service_entries().size + 7);
    static_assert(ipc::effective_custom_action_entries().size == ipc::custom_action_entries().size);
    static_assert(ipc::custom_topic_descriptor{}.original_control == false);
    static_assert(ipc::custom_topic_descriptor{}.official_standard == false);
    static_assert(ipc::custom_topic_name(2, "vendor_fixture/msg/WholeValue"));
    static_assert(!ipc::custom_topic_name(2, "rcl_interfaces/msg/Log"));

#if UWVM_CONTROL_EXPECT_FROZEN && !UWVM_CONTROL_POISON_TABLE
    static_assert(ipc::custom_topic_registry_valid);
    static_assert(ipc::custom_interface_registry_available());
    static_assert(ipc::find_custom_topic_for_operation(2, topics[0], "topic.publish") != nullptr);
    static_assert(ipc::find_custom_service(2, services[6]) != nullptr);
#else
    static_assert(!ipc::custom_topic_registry_valid);
    static_assert(!ipc::custom_interface_registry_available());
    static_assert(ipc::find_custom_topic_for_operation(2, topics[0], "topic.publish") == nullptr);
    static_assert(ipc::find_custom_service(2, services[6]) == nullptr);
#endif

    void require(bool okay, char const* detail)
    { if(!okay) throw ::std::runtime_error(detail); }
    void require(ros::result const& result, char const* detail)
    { require(static_cast<bool>(result), detail); }
    void code(ros::result const& result, errc expected, char const* detail)
    { require(result.code == expected, detail); }
    ::std::span<::std::byte const> bytes(::std::vector<::std::uint8_t> const& value)
    { return {reinterpret_cast<::std::byte const*>(value.data()), value.size()}; }

    void closed_names()
    {
        for(::std::size_t i{}; i != records.size(); ++i)
        {
            auto const& descriptor = *record_descriptors[i];
            require(descriptor.type == records[i] && descriptor.ros_version == 2 &&
                    descriptor.original_control && !descriptor.official_standard && descriptor.bounded_layout,
                    "generated closure differs from selected exact source inventory");
#if UWVM_CONTROL_EXPECT_FROZEN
            require(ipc::custom_component_valid(2, &descriptor), "frozen closure has an unbound or invalid real component");
#else
            require(descriptor.native_identity.empty(), "preparation closure contains a native identity");
#endif
        }
        for(auto name : records)
        {
            require(ipc::original_control_record_name(2, name), "complete selected record missing");
            require(!ipc::original_control_record_name(1, name), "control record admitted in ROS1");
            require(!ipc::original_control_record_name(0, name), "control record admitted in unknown backend");
        }
        for(auto name : services)
            require(ipc::original_control_service_name(2, name), "selected Service root missing");
        for(auto name : topics)
            require(ipc::original_control_topic_name(2, name), "selected Topic missing");
        for(auto name : {"rcl_interfaces/msg/LoggerLevel", "rcl_interfaces/srv/GetLoggerLevels",
                         "rcl_interfaces/srv/GetParameters_Event", "vendor_fixture/msg/WholeValue",
                         "rcl_interfaces/msg/Log/", "Rcl_interfaces/msg/Log"})
        {
            require(!ipc::original_control_record_name(2, name), "closed record domain widened");
            require(!ipc::original_control_topic_name(2, name), "closed Topic domain widened");
            require(!ipc::original_control_service_name(2, name), "closed Service domain widened");
        }
        // These are control dependencies/components, never control Topics. An
        // independently supplied old registry remains independent: if it had a
        // legal real row for a name outside the old reserved namespaces, its
        // old explicit selection must not be erased by this new domain.
        for(auto name : records)
            if(::std::find(topics.begin(), topics.end(), name) == topics.end())
            {
                require(!ipc::original_control_topic_name(2, name), "dependency became a Topic");
                auto old = ipc::custom_topic_entries();
                ipc::custom_topic_descriptor const* independently_selected{};
                for(::std::size_t i{}; i != old.size; ++i)
                    if(old.data[i].ros_version == 2 && old.data[i].type == name) independently_selected = old.data + i;
                for(auto operation : {"topic.publish", "topic.subscribe"})
                {
                    auto const* selected = ipc::find_custom_topic_for_operation(2, name, operation);
#if UWVM_CONTROL_EXPECT_FROZEN && !UWVM_CONTROL_POISON_TABLE
                    if(independently_selected)
                        require(selected && !selected->original_control &&
                                selected->native_identity == independently_selected->native_identity &&
                                selected->codec_identity == independently_selected->codec_identity,
                                "new control dependency displaced an independent old explicit row");
                    else
#endif
                        require(selected == nullptr, "control dependency acquired an implicit Topic selection");
                }
            }
    }

    void legacy_tail_and_identity_absence()
    {
        // Old aggregate initializers may omit both appended markers. We test
        // the real descriptor and real validator, with EMPTY identities. This
        // is deliberately not a manufactured generic-native positive case.
        ipc::custom_topic_descriptor old{2, "vendor_fixture/msg/WholeValue", {}, {}, 0, nullptr, 0};
        require(!old.official_standard && !old.original_control, "old aggregate tail changed");
        require(!ipc::custom_topic_registry_well_formed({&old, 1}), "blank native IDs admitted");
        require(ipc::custom_native_record_name(2, "vendor_fixture/srv/WholeValue_Request"),
                "old generic component grammar changed");
        require(!ipc::custom_component_valid(2, &old), "old unbound descriptor gained authority");
    }

    void no_default_authority()
    {
        ros::capability_manager manager;
        int object{};
        require(manager.register_instance(&object, 1, "control_contract.wasm", nullptr, nullptr),
                "register real default-empty instance");
        ::std::uint64_t handle = UINT64_MAX;
        code(manager.open(&object, 1, version::ros2, "control-log", handle), errc::permission_denied,
             "linked type support granted a default alias");
        require(handle == 0, "denied open did not clear output handle");
        ros::authorization lease;
        code(manager.authorize(&object, 1, version::ros2, UINT64_MAX, "topic.publish", {}, lease),
             errc::bad_handle, "forged handle admitted in disabled default target");
        require(!lease, "denied authorization retained a lease");
        require(manager.unregister_instance(&object, 1), "retire default instance");
    }

    ros::capability_spec operation_cap(::std::string_view alias, ::std::string_view operation,
                                       ::std::string_view type, ::std::string_view resource)
    {
        ros::capability_spec cap;
        cap.alias = alias; cap.operation = operation; cap.type = type; cap.resource = resource;
        cap.type_identity = ros::trusted_type_identity(version::ros2, type, operation);
        for(auto field : ros::operation_schema(version::ros2, operation, type))
            cap.limits.emplace(field, "1");
        auto set = [&](char const* key, char const* value) {
            if(cap.limits.contains(key)) cap.limits[key] = value;
        };
        for(auto key : {"max-message-bytes", "max-request-bytes", "max-response-bytes", "max-parameter-bytes"})
            set(key, "32768");
        for(auto key : {"max-queue-bytes", "max-event-queue-bytes", "audit-max-bytes"}) set(key, "65536");
        set("max-bytes-per-second", "1048576");
        set("max-rate-hz", "1000000"); set("burst", "64"); set("qos.depth", "64");
        set("audit", "allow-and-deny"); set("audit-max-rate-hz", "1000");
        set("audit-burst", "64"); set("audit-max-records", "128");
        set("validator", "none"); set("overflow-policy", "reject");
        set("qos.reliability", "reliable"); set("qos.durability", "volatile");
        set("qos.history", "keep-last"); set("qos.liveliness", "automatic");
        for(auto key : {"qos.deadline", "qos.lifespan", "qos.liveliness-lease-duration"}) set(key, "infinite");
        set("parameter-node", "/control"); set("parameter-name", "data");
        if(auto descriptor = ipc::find_custom_topic_for_operation(2, type, operation))
            cap.limits["adapter-codec-identity"] = descriptor->codec_identity;
        return cap;
    }

    ros::target_policy target()
    {
        ros::target_policy value;
        value.ros_version = version::ros2; value.target_scope = ros::scope::global; value.enabled = true;
        value.connection = {{"gateway-socket", "/tmp/uwvm-control-contract.sock"}, {"gateway-uid", "1000"},
            {"node-name", "control_contract"}, {"namespace", "/"}, {"connect-timeout-ms", "1000"},
            {"audit-deny-max-rate-hz", "10"}, {"audit-deny-burst", "10"},
            {"audit-deny-max-records", "128"}, {"audit-deny-max-bytes", "65536"},
            {"domain-id", "1"}, {"rmw-implementation", "rmw_fastrtps_cpp"},
            {"security-mode", "disabled-test"}, {"keystore", "not-applicable"}, {"enclave", "not-applicable"}};
        // No operation below connects this configured socket or creates a ROS
        // entity. This target is consumed only by the actual policy/manager.
        return value;
    }

    ::std::vector<::std::uint8_t> legacy_event()
    {
        auto value = ipc::encode_parameter_event({123, 456, 2, "/control", "data", "hello"});
        constexpr ::std::array<::std::uint8_t, 38> expected{
            123,0,0,0,200,1,0,0,2,8,0,0,0,'/','c','o','n','t','r','o','l',
            4,0,0,0,'d','a','t','a',5,0,0,0,'h','e','l','l','o'};
        require(value.size() == expected.size() && ::std::equal(value.begin(), value.end(), expected.begin()),
                "legacy parameter projection bytes changed");
        return value;
    }

    void legacy_parameter_projection()
    {
        auto cap = operation_cap("parameter-events", "param.subscribe-events", topics[1], "/control/data");
        auto schema = ros::operation_schema(version::ros2, cap.operation, cap.type);
        require(::std::find(schema.begin(), schema.end(), "adapter-codec-identity") == schema.end(),
                "full control Topic added adapter requirement to old parameter schema");
        require(!cap.limits.contains("adapter-codec-identity"), "legacy cap accidentally has a new adapter field");
        require(cap.type_identity == ros::builtin_type_identity(version::ros2, topics[1]),
                "full Topic shadowed old parameter identity");
        require(ipc::find_custom_topic_for_operation(2, topics[1], cap.operation) == nullptr,
                "full Topic selected for parameter projection");
        require(ros::validate_capability(version::ros2, cap), "old complete parameter grant rejected");
        auto payload = legacy_event();
        require(ros::validate_message(version::ros2, cap, bytes(payload)), "old valid parameter projection rejected");
        auto wrong = cap; wrong.limits["parameter-name"] = "other";
        code(ros::validate_message(version::ros2, wrong, bytes(payload)), errc::permission_denied,
             "legacy node/name scope ceased being enforced");
    }

#if UWVM_CONTROL_EXPECT_FROZEN && !UWVM_CONTROL_POISON_TABLE
    template<class Text>
    void text(Text& out, ::std::string_view value)
    {
        require(value.size() <= sizeof out.data, "fixture text exceeds actual generated capacity");
        out.size = static_cast<::std::uint32_t>(value.size());
        ::std::memcpy(out.data, value.data(), value.size());
    }

    ::std::vector<::std::uint8_t> full_event()
    {
        auto value = ::std::make_unique<roswasmc_bounded_2_rcl_interfaces_msg_ParameterEvent>();
        value->stamp.sec = 123; value->stamp.nanosec = 456; text(value->node, "/control");
        // The three actual Parameter sequences are empty, not omitted fields.
        // Use the actual complete C codec; this is distinct from the selected
        // node/name/value projection above.
        auto const* descriptor = ipc::find_custom_topic_for_operation(2, topics[1], "topic.publish");
        require(descriptor != nullptr, "frozen full ParameterEvent descriptor absent");
        ::std::vector<::std::uint8_t> payload(descriptor->wire_bytes);
        ::std::size_t used = SIZE_MAX;
        require(roswasmc_bounded_2_rcl_interfaces_msg_ParameterEvent_encode(value.get(), payload.data(), payload.size(), &used) == 0,
                "actual full ParameterEvent codec rejected fixture");
        require(used <= payload.size(), "actual full ParameterEvent length outside capacity");
        payload.resize(used);
        return payload;
    }

    ::std::vector<::std::uint8_t> full_log()
    {
        auto value = ::std::make_unique<roswasmc_bounded_2_rcl_interfaces_msg_Log>();
        value->stamp.sec = 123; value->stamp.nanosec = 456; value->level = 20; value->line = 17;
        text(value->name, "control_contract"); text(value->msg, "complete log");
        text(value->file, "registry_contract.cc"); text(value->function, "full_log");
        auto const* descriptor = ipc::find_custom_topic_for_operation(2, topics[0], "topic.publish");
        require(descriptor != nullptr, "frozen Log descriptor absent");
        ::std::vector<::std::uint8_t> payload(descriptor->wire_bytes);
        ::std::size_t used = SIZE_MAX;
        require(roswasmc_bounded_2_rcl_interfaces_msg_Log_encode(value.get(), payload.data(), payload.size(), &used) == 0,
                "actual complete Log codec rejected fixture");
        require(used <= payload.size(), "actual Log length outside capacity");
        payload.resize(used);
        require(ipc::validate_custom_topic(*descriptor, payload.data(), payload.size()),
                "common grammar differs from actual C Log codec");
        return payload;
    }

    void frozen_selection()
    {
        for(auto name : topics)
        {
            auto const* pub = ipc::find_custom_topic_for_operation(2, name, "topic.publish");
            auto const* sub = ipc::find_custom_topic_for_operation(2, name, "topic.subscribe");
            require(pub && sub == pub && pub->original_control && !pub->official_standard,
                    "complete frozen control Topic selection failed");
            require(pub->native_identity == ros::trusted_type_identity(version::ros2, name, "topic.publish"),
                    "explicit Topic did not select its actual supplied native identity");
            for(auto operation : {"param.subscribe-events", "log.write", "service.call", "topic.unknown", ""})
                require(ipc::find_custom_topic_for_operation(2, name, operation) == nullptr,
                        "control marker widened its operation domain");
        }
        for(auto name : services)
        {
            auto const* item = ipc::find_custom_service(2, name);
            require(item && ipc::custom_service_shape_valid(*item), "selected real Service shape unavailable");
            require(item->request->original_control && item->response->original_control,
                    "control Service lost complete two-component marking");
            require(ipc::find_custom_service(1, name) == nullptr, "ROS1 acquired a ROS2 control Service");
            require(ipc::find_custom_action(2, name) == nullptr, "Service became an Action");
        }
    }

    void mixed_markers_and_owners()
    {
        auto const original = *ipc::find_custom_service(2, services[6]);
        auto request = *original.request, response = *original.response;
        auto item = original; item.request = &request; item.response = &response;
        response.original_control = false;
        require(!ipc::custom_service_shape_valid(item), "mixed control/generic pair admitted");
        response = *original.response; request.official_standard = true;
        require(!ipc::custom_service_shape_valid(item), "control/standard dual component admitted");
        request = *original.request; item.request = &response; item.response = &request;
        require(!ipc::custom_service_shape_valid(item), "request/response owner roles swapped");
        item = original; item.type = services[0];
        require(!ipc::custom_service_shape_valid(item), "components accepted under another exact Service owner");
        item = original; item.ros_version = 1;
        require(!ipc::custom_service_shape_valid(item), "component owner admitted in wrong backend");
        auto row = *ipc::find_custom_topic_for_operation(2, topics[0], "topic.publish");
        row.official_standard = true;
        require(!ipc::custom_topic_registry_well_formed({&row, 1}), "dual Topic marker admitted");
        row = *ipc::find_custom_topic_for_operation(2, topics[0], "topic.publish");
        row.type = records[0];
        require(!ipc::custom_topic_registry_well_formed({&row, 1}), "dependency admitted as full control Topic");
    }

    void topic_and_parameter_coexist()
    {
        auto topic = operation_cap("full-events", "topic.publish", topics[1], "/control/full_events");
        auto parameter = operation_cap("parameter-events", "param.subscribe-events", topics[1], "/control/data");
        require(topic.limits.contains("adapter-codec-identity"), "full Topic lacks explicit canonical identity");
        require(ros::validate_capability(version::ros2, topic), "new complete Topic capability invalid");
        require(ros::validate_capability(version::ros2, parameter), "old projection capability invalid");
        auto full = full_event(), old = legacy_event();
        require(ros::validate_message(version::ros2, topic, bytes(full)), "new full Topic payload rejected");
        require(ros::validate_message(version::ros2, parameter, bytes(old)), "legacy projection payload rejected");
        require(!ros::validate_message(version::ros2, topic, bytes(old)), "legacy projection accepted as full Topic");
        require(!ros::validate_message(version::ros2, parameter, bytes(full)), "full Topic accepted as legacy projection");
        auto missing = topic; missing.limits.erase("adapter-codec-identity");
        require(!ros::validate_capability(version::ros2, missing), "full Topic accepted without its explicit codec ID");
        auto wrong_identity = topic;
        wrong_identity.type_identity = ipc::find_custom_service(2, services[6])->request->native_identity;
        code(ros::validate_capability(version::ros2, wrong_identity), errc::permission_denied,
             "a different real component identity authorized this full Topic");
        auto too_small = topic; too_small.limits["max-message-bytes"] = "1";
        code(ros::validate_message(version::ros2, too_small, bytes(full)), errc::too_large,
             "canonical full Topic bypassed the actual granted byte quota");
        auto extra = parameter; extra.limits["adapter-codec-identity"] = topic.limits.at("adapter-codec-identity");
        code(ros::validate_capability(version::ros2, extra), errc::invalid_argument,
             "old projection silently accepted new full Topic codec field");
        auto both = target(); both.capabilities.emplace(topic.alias, topic); both.capabilities.emplace(parameter.alias, parameter);
        require(ros::validate_target(both), "full Topic and old projection cannot coexist in one real target");
    }

    void explicit_grant_lifecycle()
    {
        auto log = operation_cap("control-log", "topic.publish", topics[0], "/control/log");
        auto parameter = operation_cap("parameter-events", "param.subscribe-events", topics[1], "/control/data");
        auto policy = target(); policy.capabilities.emplace(log.alias, log); policy.capabilities.emplace(parameter.alias, parameter);
        require(ros::validate_target(policy), "real explicit policy invalid");
        auto payload = full_log(), projection = legacy_event();
        ros::capability_manager manager; int object{}, other{};
        require(manager.register_instance(&object, 7, "control_contract.wasm", nullptr, &policy), "register explicit target");
        require(manager.register_instance(&other, 7, "other_contract.wasm", nullptr, nullptr), "register isolated default target");
        ::std::uint64_t handle{}, parameter_handle{};
        require(manager.open(&object, 7, version::ros2, log.alias, handle) && handle, "open explicit Log grant");
        require(manager.open(&object, 7, version::ros2, parameter.alias, parameter_handle), "open legacy projection grant");
        ros::authorization lease;
        require(manager.authorize(&object, 7, version::ros2, handle, log.operation, bytes(payload), lease), "authorize actual full Log");
        require(lease && lease.capability() && lease.capability()->type_identity == log.type_identity,
                "actual lease lost capability identity");
        lease.release();
        code(manager.authorize(&other, 7, version::ros2, handle, log.operation, bytes(payload), lease), errc::bad_handle,
             "cross-instance numeric handle acquired authority");
        code(manager.authorize(&object, 7, version::ros2, handle, "topic.subscribe", bytes(payload), lease), errc::permission_denied,
             "publish handle widened operation");
        require(manager.authorize(&object, 7, version::ros2, parameter_handle, parameter.operation, bytes(projection), lease),
                "old parameter projection fails through actual manager");
        lease.release();
        bool last{}; auto const stale = handle;
        require(manager.close(&object, 7, version::ros2, handle, &last) && last, "close actual Log grant once");
        code(manager.authorize(&object, 7, version::ros2, stale, log.operation, bytes(payload), lease), errc::bad_handle,
             "closed handle still usable");
        require(manager.open(&object, 7, version::ros2, log.alias, handle) && handle != stale, "reopen reused a stale full handle");
        require(manager.revoke(&object, 7, version::ros2, log.alias), "revoke explicit alias");
        code(manager.authorize(&object, 7, version::ros2, handle, log.operation, bytes(payload), lease), errc::bad_handle,
             "revoked live handle still usable");
        ::std::uint64_t denied = UINT64_MAX;
        code(manager.open(&object, 7, version::ros2, log.alias, denied), errc::revoked, "revoked alias reopened");
        require(denied == 0, "revoked open changed failure output convention");
        require(manager.close(&object, 7, version::ros2, parameter_handle), "close independent old projection");
        require(manager.unregister_instance(&object, 7), "unregister explicit instance");
        require(manager.unregister_instance(&other, 7), "unregister isolated instance");
    }

#if UWVM_CONTROL_HAS_LEGACY
    void independent_legacy_provider_coexists()
    {
        auto old_topics = ipc::custom_topic_entries();
        auto old_services = ipc::custom_service_entries();
        auto old_actions = ipc::custom_action_entries();
        require(old_topics.size + old_services.size + old_actions.size != 0,
                "explicit legacy-provider contract received an empty table");
        for(::std::size_t i{}; i != old_topics.size; ++i)
        {
            auto const& old = old_topics.data[i];
            require(!old.original_control, "legacy manifest claimed the independent control marker");
            auto const* selected = ipc::find_custom_topic_for_operation(old.ros_version, old.type, "topic.publish");
            require(selected && selected->type == old.type && selected->native_identity == old.native_identity &&
                    selected->codec_identity == old.codec_identity,
                    "independent old Topic was lost or rebound during control composition");
            // Existing non-control rows retain the previous operation-neutral
            // lookup helper semantics. This does NOT grant that operation or
            // claim that a backend can use this Topic for parameter events.
            require(ipc::find_custom_topic_for_operation(old.ros_version, old.type, "param.subscribe-events") == selected,
                    "old generic lookup semantics changed");
        }
        for(::std::size_t i{}; i != old_services.size; ++i)
        {
            auto const& old = old_services.data[i];
            auto const* selected = ipc::find_custom_service(old.ros_version, old.type);
            require(selected && selected->native_identity == old.native_identity && selected->codec_identity == old.codec_identity,
                    "independent old Service was lost or rebound");
        }
        for(::std::size_t i{}; i != old_actions.size; ++i)
        {
            auto const& old = old_actions.data[i];
            auto const* selected = ipc::find_custom_action(old.ros_version, old.type);
            require(selected && selected->native_identity == old.native_identity && selected->codec_identity == old.codec_identity,
                    "independent old Action was lost or rebound");
        }
        frozen_selection();
    }
#endif
#endif

    void unavailable_whole_table()
    {
#if !UWVM_CONTROL_EXPECT_FROZEN || UWVM_CONTROL_POISON_TABLE
        for(auto name : topics)
            require(ipc::find_custom_topic_for_operation(2, name, "topic.publish") == nullptr,
                    "unavailable whole table leaked a valid-looking Topic prefix");
        for(auto name : services)
            require(ipc::find_custom_service(2, name) == nullptr,
                    "unavailable Topic table did not disable associated Service table");
        require(ipc::find_custom_action(2, "vendor_fixture/action/WholeAction") == nullptr,
                "unavailable whole table acquired an Action");
        auto old_topics = ipc::custom_topic_entries();
        auto old_services = ipc::custom_service_entries();
        auto old_actions = ipc::custom_action_entries();
        for(::std::size_t i{}; i != old_topics.size; ++i)
            require(ipc::find_custom_topic(old_topics.data[i].ros_version, old_topics.data[i].type) == nullptr,
                    "unavailable control table leaked an old Topic prefix");
        for(::std::size_t i{}; i != old_services.size; ++i)
            require(ipc::find_custom_service(old_services.data[i].ros_version, old_services.data[i].type) == nullptr,
                    "unavailable control table leaked an old Service prefix");
        for(::std::size_t i{}; i != old_actions.size; ++i)
            require(ipc::find_custom_action(old_actions.data[i].ros_version, old_actions.data[i].type) == nullptr,
                    "unavailable control table leaked an old Action prefix");
#if !UWVM_CONTROL_EXPECT_FROZEN
        auto original = control::uwvm_control_original_topic_entries();
        for(::std::size_t i{}; i != original.size; ++i)
            require(original.data[i].native_identity.empty(), "preparation input secretly freezes native Topic IDs");
        auto original_services = control::custom_service_entries();
        for(::std::size_t i{}; i != original_services.size; ++i)
        {
            auto const& item = original_services.data[i];
            require(item.native_identity.empty() && item.request->native_identity.empty() && item.response->native_identity.empty(),
                    "preparation input secretly freezes native Service/component IDs");
        }
#else
        auto original = control::uwvm_control_original_topic_entries();
        require(ipc::custom_topic_registry_well_formed(original), "poison input was not a real structurally valid frozen table");
        require(!control::custom_topic_entries().data[1].codec_identity.size(), "poison negative was not installed");
#endif
#endif
    }
}

int main()
{
    using namespace control_contract;
    try
    {
        unsigned groups{};
        auto run = [&](char const* name, auto function) {
            function(); ++groups;
            ::fast_io::io::println("PASS ", ::fast_io::mnp::os_c_str(name));
        };
        run("closed_names_and_dependency_non_topics", closed_names);
        run("legacy_aggregate_tail_and_blank_identity_refusal", legacy_tail_and_identity_absence);
        run("real_manager_default_empty_authority", no_default_authority);
        run("legacy_parameter_identity_schema_and_wire", legacy_parameter_projection);
#if UWVM_CONTROL_EXPECT_FROZEN && !UWVM_CONTROL_POISON_TABLE
        run("frozen_exact_topic_service_operation_selection", frozen_selection);
        run("mixed_markers_owner_roles_and_dependency_refusal", mixed_markers_and_owners);
        run("full_topic_and_old_parameter_projection_coexist", topic_and_parameter_coexist);
        run("real_manager_explicit_grant_close_revoke_isolation", explicit_grant_lifecycle);
#if UWVM_CONTROL_HAS_LEGACY
        run("actual_independent_legacy_and_control_providers_coexist", independent_legacy_provider_coexists);
#endif
#else
        run("unavailable_whole_table_lookup_refusal", unavailable_whole_table);
#endif
        ::fast_io::io::println("CONTROL_COMMON_CONTRACT_GROUPS ", groups);
        return 0;
    }
    catch(::std::exception const& error)
    {
        ::fast_io::io::perrln("CONTROL_COMMON_CONTRACT_FAILURE ", ::fast_io::mnp::os_c_str(error.what()));
        return 1;
    }
}

#endif
