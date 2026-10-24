/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
// This semantic contract runs twice: without a provider to prove default deny,
// and with the reviewed official deployment manifest to check explicit grants.
// It creates no native ROS entity and cannot replace Guest/IPC/native tests.
#include <uwvm2/imported/ros/policy/policy.h>
#include "../../../../../src/ros/gateway/common/server.h"
#include <fast_io.h>
#include <cstdlib>

namespace ros = ::uwvm2::imported::ros;
namespace gateway = ::uwvm2::ros_gateway;
namespace
{
    unsigned checks{};
    void require(bool value)
    {
        ++checks;
        if(!value) { ::fast_io::io::perrln("FAIL custom policy contract check=", checks);::std::abort(); }
    }
    bool gateway_accepts(ros::version version, ros::capability_spec const& cap)
    {
        try { gateway::validate(version, cap);return true; }
        catch(::std::runtime_error const&) { return false; }
    }
    ros::capability_spec topic(ros::ipc::custom_topic_descriptor const& descriptor, bool subscribe)
    {
        auto version{static_cast<ros::version>(descriptor.ros_version)};
        ros::capability_spec cap{"custom", subscribe ? "topic.subscribe" : "topic.publish", "/custom/record",
                                ::std::string{descriptor.type}, ::std::string{descriptor.native_identity}, {}};
        // Supply every existing operation field explicitly; the generated
        // descriptor neither creates nor fills in a runtime capability.
        for(auto key : ros::operation_schema(version, cap.operation)) { cap.limits.emplace(key, "4"); }
        for(auto key : {"max-message-bytes", "max-queue-bytes", "max-bytes-per-second", "audit-max-bytes"})
        { cap.limits[key] = "65536"; }
        cap.limits["audit"] = "allow-and-deny";
        cap.limits["validator"] = "none";
        cap.limits["overflow-policy"] = "reject";
        cap.limits["adapter-codec-identity"] = descriptor.codec_identity;
        if(version == ros::version::ros1)
        { cap.limits["transport"] = "tcpros";if(!subscribe) { cap.limits["latch"] = "false"; } }
        else
        {
            cap.limits["qos.reliability"] = "reliable";cap.limits["qos.durability"] = "volatile";
            cap.limits["qos.history"] = "keep-last";cap.limits["qos.liveliness"] = "automatic";
            for(auto key : {"qos.deadline", "qos.lifespan", "qos.liveliness-lease-duration"})
            { cap.limits[key] = "infinite"; }
        }
        return cap;
    }
    ::std::span<::std::byte const> view(::std::vector<::std::uint8_t> const& data)
    { return {reinterpret_cast<::std::byte const*>(data.data()), data.size()}; }
}
int main()
{
    ros::policy_store empty;
    for(auto version : {ros::version::ros1, ros::version::ros2})
    {
        auto target{empty.find_target(version, ros::scope::global, "")};
        require(target && !target->enabled && target->connection.empty() && target->capabilities.empty());
    }
    auto registry{ros::ipc::custom_topic_entries()};
    require(ros::ipc::custom_topic_registry_valid);
#if defined(UWVM_CUSTOM_TEST_REGISTERED)
    require(registry.size == 2);
    for(::std::size_t i{}; i != registry.size; ++i)
    {
        auto const& descriptor{registry.data[i]};auto version{static_cast<ros::version>(descriptor.ros_version)};
        require(ros::trusted_type_identity(version, descriptor.type) == descriptor.native_identity);
        for(bool subscribe : {false, true})
        {
            auto cap{topic(descriptor, subscribe)};
            require(bool(ros::validate_capability(version, cap)));require(gateway_accepts(version, cap));
            ::std::vector<::std::uint8_t> body(descriptor.wire_bytes);
            require(bool(ros::validate_message(version, cap, view(body))));require(gateway::validate_payload(cap, body));
            auto missing{cap};missing.limits.erase("adapter-codec-identity");
            require(!ros::validate_capability(version, missing));require(!gateway_accepts(version, missing));
            require(!ros::validate_message(version, missing, view(body)));require(!gateway::validate_payload(missing, body));
            auto wrong{cap};wrong.limits["adapter-codec-identity"] = "UWVMROS1_wrong";
            require(!ros::validate_capability(version, wrong));require(!gateway_accepts(version, wrong));
            wrong = cap;wrong.type_identity = "wrong-native-identity";
            require(!ros::validate_capability(version, wrong));require(!gateway_accepts(version, wrong));
            auto other{version == ros::version::ros1 ? ros::version::ros2 : ros::version::ros1};
            require(!ros::validate_capability(other, cap));require(!gateway_accepts(other, cap));
            for(auto const& field : ros::operation_schema(version, cap.operation))
            {
                missing = cap;missing.limits.erase(::std::string{field});
                require(!ros::validate_capability(version, missing));require(!gateway_accepts(version, missing));
            }
            wrong = cap;wrong.limits["max-message-bytes"] = "1";
            require(ros::validate_message(version, wrong, view(body)).code == ros::errc::too_large);
            if(subscribe)
            {
                gateway::event_queue queue{wrong};require(!queue.enqueue(body));require(queue.poll().error == 11);
                gateway::event_queue valid_queue{cap};require(valid_queue.enqueue(body));require(valid_queue.poll().payload == body);
                valid_queue.deactivate();require(!valid_queue.enqueue(body));require(valid_queue.poll().error == 125);
                require(bool(ros::validate_response_payload(version, cap, view(body))));
            }
            else
            {
                require(bool(ros::validate_response_payload(version, cap, {})));
                require(!ros::validate_response_payload(version, cap, view(body)));
            }
            body.push_back(0);require(!ros::validate_message(version, cap, view(body)));require(!gateway::validate_payload(cap, body));body.pop_back();
            ::std::size_t offset{};
            for(::std::size_t field{}; field != descriptor.layout_count; ++field)
            {
                auto scalar{descriptor.layout[field]};auto width{ros::ipc::custom_scalar_width(scalar.kind)};
                if(scalar.kind == ros::ipc::custom_scalar_kind::boolean)
                {
                    body[offset] = 2;require(!ros::validate_message(version, cap, view(body)));require(!gateway::validate_payload(cap, body));body[offset] = 0;
                }
                if(scalar.kind == ros::ipc::custom_scalar_kind::f32 || scalar.kind == ros::ipc::custom_scalar_kind::f64)
                {
                    ros::ipc::custom_write_bits(body.data() + offset, width == 4 ? 0x7f800001ULL : 0x7ff0000000000001ULL, width);
                    require(!ros::validate_message(version, cap, view(body)));require(!gateway::validate_payload(cap, body));
                    ros::ipc::custom_write_bits(body.data() + offset, 0, width);
                }
                offset += width * scalar.count;
            }
            // Command-line construction accepts this custom-only field once;
            // duplicate input is rejected and no target is enabled implicitly.
            ros::policy_store store;require(bool(store.create_target(version, ros::scope::single, "guest")));
            wrong = cap;wrong.limits.clear();require(bool(store.grant(version, ros::scope::single, "guest", wrong)));
            require(bool(store.set_capability(version, ros::scope::single, "guest", "custom", "adapter-codec-identity", descriptor.codec_identity)));
            require(store.set_capability(version, ros::scope::single, "guest", "custom", "adapter-codec-identity", descriptor.codec_identity).code == ros::errc::conflict);
            require(!store.find_target(version, ros::scope::single, "guest")->enabled);
        }
        for(auto operation : {"service.call", "action.send-goal", "param.get", "tf.broadcast"})
        {
            auto cap{topic(descriptor, false)};cap.operation = operation;
            require(!ros::backend_supported(version, operation, cap.type));require(!gateway_accepts(version, cap));
            require(!ros::validate_message(version, cap, {}));
        }
    }
#else
    require(registry.size == 0);
    require(!ros::ipc::find_custom_topic(1, "uwvm_custom_ros1_fixture/Packet"));
    require(!ros::ipc::find_custom_topic(2, "uwvm_ros_custom_test_msgs/msg/FixedSample"));
    require(!ros::backend_supported(ros::version::ros1, "topic.publish", "uwvm_custom_ros1_fixture/Packet"));
    require(!ros::backend_supported(ros::version::ros2, "topic.publish", "uwvm_ros_custom_test_msgs/msg/FixedSample"));
#endif
    ::fast_io::io::println("PASS custom Topic policy explicit grants checks=", checks, " registered=", registry.size);
}
