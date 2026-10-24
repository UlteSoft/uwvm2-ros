/* Genuine SDK2.8.0 BaseClient/protobuf contract over an explicit test Router.
 * This is not original Router transport, native robot, ROS, Gateway or Guest
 * integration. The finite native owner and complete original SDK pins are required.
 */
#include "kortex_notifications.hpp"
#include <cassert>
#include <map>
#include <thread>

namespace api = Kinova::Api;
using Channel = uwvm_kinova::KortexBoundedNotifications;

static api::Frame original_frame(api::FrameTypes kind, std::uint32_t uid,
                                 std::string const &payload,
                                 std::uint16_t message = 17) {
    api::HeaderInfo header;
    header.m_frameInfo.frame_info = 0;
    header.m_frameInfo.frameType = kind;
    header.m_serviceInfo.service_info = 0;
    header.m_serviceInfo.functionUid = uid;
    header.m_serviceInfo.serviceVersion = 1;
    header.m_messageInfo.message_info = 0;
    header.m_messageInfo.messageId = message;
    header.m_messageInfo.sessionId = 9;
    header.m_payloadInfo.payload_info = 0;
    header.m_payloadInfo.payloadLength = static_cast<std::uint32_t>(payload.size());
    api::Frame frame;
    header.fillHeader(frame.mutable_header());
    frame.set_payload(payload);
    return frame;
}

class RecordingRouter final : public api::IRouterClient {
  public:
    std::map<std::uint32_t, std::function<api::Error(api::Frame &)>> notifications;
    unsigned registrations = 0, subscribes = 0, unsubscribes = 0, sends = 0;
    bool fail_subscribe = false, fail_unsubscribe = false, allow_cyclic = false;
    std::string complete_options;

    void reset() override { assert(false); }
    void registerBridgingCallback(std::function<void(api::Frame &)>) override {
        assert(false);
    }
    void registerNotificationCallback(
        std::uint32_t service,
        std::function<api::Error(api::Frame &)> callback) override {
        assert(service == api::eIdBase ||
               (allow_cyclic && service == api::eIdBaseCyclic));
        ++registrations;
        notifications[service] = std::move(callback);
    }
    void registerErrorCallback(std::function<void(api::KError)>) override {
        assert(false);
    }
    void registerHitCallback(std::function<void(api::FrameTypes)>) override {
        assert(false);
    }
    std::future<api::Frame> send(std::string const &payload, std::uint32_t version,
                                 std::uint32_t uid, std::uint32_t device,
                                 api::RouterClientSendOptions const &options) override {
        ++sends;
        assert(version == 1 && device == 0 && !options.andForget &&
               options.timeout_ms > 0 && options.timeout_ms <= 5000);
        std::string response;
        if (uid == api::Base::eUidActionTopic) {
            ++subscribes;
            api::Common::NotificationOptions selected;
            assert(selected.ParseFromString(payload));
            assert(selected.SerializeAsString() == complete_options);
            if (fail_subscribe)
                throw std::runtime_error("explicit unknown subscribe fixture");
            api::Common::NotificationHandle handle;
            handle.set_identifier(17);
            assert(handle.SerializeToString(&response));
        } else {
            assert(uid == api::Base::eUidUnsubscribe);
            ++unsubscribes;
            api::Common::NotificationHandle handle;
            assert(handle.ParseFromString(payload) && handle.identifier() == 17);
            if (fail_unsubscribe)
                throw std::runtime_error("explicit unknown unsubscribe fixture");
        }
        std::promise<api::Frame> reply;
        reply.set_value(original_frame(api::MSG_FRAME_RESPONSE, uid, response));
        return reply.get_future();
    }
    api::Error sendWithCallback(std::string const &, std::uint32_t, std::uint32_t,
                                std::uint32_t, api::MessageCallback) override {
        assert(false);
        return {};
    }
    api::Error sendMsgFrame(api::Frame const &) override {
        assert(false);
        return {};
    }
    std::uint16_t getConnectionId() override { return 9; }
    void SetActivationStatus(bool) override { assert(false); }
    api::ITransportClient *getTransport() override { return nullptr; }
};

template <class F> static void refuses(F const &operation) {
    bool failed = false;
    try {
        operation();
    } catch (std::exception const &) {
        failed = true;
    }
    assert(failed);
}

static uwvm_kinova::KortexNotificationPolicy grant(void const *owner) {
    uwvm_kinova::KortexNotificationPolicy policy;
    policy.authenticated_owner = owner;
    policy.exclusive_base_callback_slot = true;
    policy.callbacks_quiescent_at_construction = true;
    policy.subscribe = policy.poll = policy.unsubscribe = true;
    policy.active_slots = 2;
    policy.maximum_frame_bytes = 2048;
    policy.maximum_rpc_ms = 5000;
    policy.subscribe_expires_at = policy.poll_expires_at =
        Channel::Clock::now() + std::chrono::seconds(5);
    policy.unsubscribe_expires_at = Channel::Clock::now() + std::chrono::seconds(8);
    return policy;
}

static api::Common::NotificationOptions complete_options() {
    api::Common::NotificationOptions options;
    options.set_type(api::Common::NOTIFICATION_TYPE_EVENT);
    options.set_rate_m_sec(73);
    options.set_threshold_value(0.125f);
    // Preserve a real original protobuf unknown field as well as its complete
    // known/default fields. The adapter must never project/copy-rename the value.
    options.GetReflection()->MutableUnknownFields(&options)->AddVarint(
        500, 0xfedcba9876543210ULL);
    return options;
}

int main() {
    int identity = 0, foreign = 0;
    {
        // A zero message bound must deny even payload-free original methods
        // before the Router boundary or local correlation reservation.
        RecordingRouter admission;
        admission.allow_cyclic = true;
        api::Base::BaseClient base(&admission);
        api::BaseCyclic::BaseCyclicClient cyclic(&admission);
        uwvm_kinova::KortexNativePolicy policy;
        policy.authenticated_owner = &identity;
        policy.enabled.fill(true);
        policy.exclusive_native_action_control = true;
        policy.maximum_rpc_ms = 1000;
        policy.maximum_message_bytes = 0;
        policy.expires_at = Channel::Clock::now() + std::chrono::seconds(5);
        uwvm_kinova::KortexNative native(base, cyclic, policy);
        auto deadline = Channel::Clock::now() + std::chrono::milliseconds(500);
        refuses([&] { native.stop(&identity, 1, deadline); });
        refuses([&] { native.clear_faults(&identity, 2, deadline); });
        refuses([&] { native.pause_action(&identity, 3, deadline); });
        refuses([&] { native.resume_action(&identity, 4, deadline); });
        refuses([&] { native.stop_action(&identity, 5, deadline); });
        refuses([&] { native.actuator_count(&identity, 6, deadline); });
        assert(admission.sends == 0);
        for (auto const &receipt : native.receipts_native_only()) {
            assert(receipt.correlation == 0 &&
                   receipt.delivery == uwvm_kinova::Delivery::unused);
        }
    }
    RecordingRouter empty;
    refuses([&] { Channel denied(empty, {}); });
    assert(empty.registrations == 0 && empty.subscribes == 0);

    RecordingRouter router;
    auto policy = grant(&identity);
    auto options = complete_options();
    router.complete_options = options.SerializeAsString();
    std::function<api::Error(api::Frame &)> retained_after_owner;
    api::Base::ActionNotification original;
    original.set_action_event(api::Base::ACTION_END);
    original.set_abort_details(static_cast<api::SubErrorCodes>(1));
    original.mutable_handle()->set_identifier(0xfedcba98u);
    original.mutable_timestamp()->set_sec(0xf1234567u);
    original.mutable_timestamp()->set_usec(654321);
    original.mutable_user_handle()->set_identifier(0xf2345678u);
    original.mutable_connection()->set_connection_identifier(0xf3456789u);
    original.mutable_connection()->set_connection_information(
        "explicit original connection fixture");
    original.mutable_connection()->mutable_user_handle()->set_identifier(0xf456789au);
    original.add_trajectory_info()->set_waypoint_index(0xf56789abu);
    original.mutable_trajectory_info(0)->set_joint_index(6);
    original.GetReflection()->MutableUnknownFields(&original)->AddLengthDelimited(
        501, "complete-original-extra");
    auto frame = original_frame(api::MSG_FRAME_NOTIFICATION, api::Base::eUidActionTopic,
                                original.SerializeAsString());
    frame.GetReflection()->MutableUnknownFields(&frame)->AddVarint(
        502, 0xfedcba9876543210ULL);
    auto frame_bytes = frame.SerializeAsString();
    {
        Channel channel(router, policy);
        assert(router.registrations == 1 && router.subscribes == 0);
        retained_after_owner = router.notifications.at(api::eIdBase);
        uwvm_kinova::KortexNotificationRead read;
        refuses([&] { channel.poll(&foreign, read); });
        assert(channel.poll(&identity, read) == 11 && read.stream_failure == 0);
        api::Common::NotificationHandle handle;
        channel.subscribe_actions(&identity, 101,
                                  Channel::Clock::now() + std::chrono::seconds(1),
                                  options, handle);
        assert(handle.identifier() == 17 && router.subscribes == 1);
        auto receipts = channel.receipts_native_only();
        assert(receipts[0].correlation == 101 &&
               receipts[0].delivery == uwvm_kinova::Delivery::returned);
        refuses([&] {
            channel.subscribe_actions(&identity, 102,
                                      Channel::Clock::now() + std::chrono::seconds(1),
                                      options, handle);
        });
        assert(router.subscribes == 1);

        auto &callback = router.notifications.at(api::eIdBase);
        assert(callback(frame).error_code() == api::ERROR_NONE);
        assert(callback(frame).error_code() == api::ERROR_NONE);
        assert(callback(frame).error_code() ==
               api::ERROR_NONE); // finite overflow, no thread/retry
        assert(channel.poll(&identity, read) == 0 && read.stream_failure == 90 &&
               read.sequence == 1);
        assert(read.frame.SerializeAsString() == frame_bytes);
        api::Base::ActionNotification decoded;
        assert(uwvm_kinova::decode_kortex_action_notification(read, decoded) == 0);
        assert(decoded.SerializeAsString() == original.SerializeAsString());
        auto wrong = read;
        auto wrong_header = api::HeaderInfo(wrong.frame.header());
        wrong_header.m_frameInfo.errorCode = 1;
        wrong_header.fillHeader(wrong.frame.mutable_header());
        assert(uwvm_kinova::decode_kortex_action_notification(wrong, decoded) == 22);
        wrong = read;
        wrong.frame.set_payload("\xff", 1);
        assert(uwvm_kinova::decode_kortex_action_notification(wrong, decoded) == 22);
        assert(channel.poll(&identity, read) == 0 && read.stream_failure == 90 &&
               read.sequence == 2);
        assert(read.frame.SerializeAsString() == frame_bytes);
        assert(channel.poll(&identity, read) == 11 && read.stream_failure == 90);
        callback(frame); // permanent loss stops new capture
        assert(channel.poll(&identity, read) == 11);
        channel.retire_local();
        refuses([&] {
            channel.unsubscribe_actions(
                &identity, 101, Channel::Clock::now() + std::chrono::seconds(1));
        }); // no correlation reuse
        assert(router.unsubscribes == 0);
        channel.unsubscribe_actions(&identity, 103,
                                    Channel::Clock::now() + std::chrono::seconds(1));
        assert(router.unsubscribes == 1);
        refuses([&] {
            channel.unsubscribe_actions(
                &identity, 104, Channel::Clock::now() + std::chrono::seconds(1));
        });
        assert(router.unsubscribes == 1);
    }
    assert(retained_after_owner(frame).error_code() == api::ERROR_NONE);
    assert(router.subscribes == 1 &&
           router.unsubscribes == 1); // destructor performs no RPC

    RecordingRouter unknown;
    unknown.complete_options = options.SerializeAsString();
    unknown.fail_subscribe = true;
    {
        Channel channel(unknown, grant(&identity));
        api::Common::NotificationHandle handle;
        refuses([&] {
            channel.subscribe_actions(&identity, 201,
                                      Channel::Clock::now() + std::chrono::seconds(1),
                                      options, handle);
        });
        auto history = channel.receipts_native_only();
        assert(history[0].delivery == uwvm_kinova::Delivery::unknown &&
               history[0].error);
        refuses([&] {
            channel.subscribe_actions(&identity, 202,
                                      Channel::Clock::now() + std::chrono::seconds(1),
                                      options, handle);
        });
        refuses([&] {
            channel.unsubscribe_actions(
                &identity, 203, Channel::Clock::now() + std::chrono::seconds(1));
        });
        assert(unknown.subscribes == 1 && unknown.unsubscribes == 0);
    }
    assert(unknown.subscribes == 1 && unknown.unsubscribes == 0);

    RecordingRouter cleanup;
    cleanup.complete_options = options.SerializeAsString();
    cleanup.fail_unsubscribe = true;
    {
        auto policy = grant(&identity);
        policy.poll_expires_at = Channel::Clock::now() + std::chrono::seconds(1);
        Channel channel(cleanup, policy);
        api::Common::NotificationHandle handle;
        channel.subscribe_actions(
            &identity, 301, Channel::Clock::now() + std::chrono::milliseconds(500),
            options, handle);
        std::this_thread::sleep_until(policy.poll_expires_at +
                                      std::chrono::milliseconds(1));
        uwvm_kinova::KortexNotificationRead read;
        refuses([&] { channel.poll(&identity, read); });
        // Receive expiry cannot erase the independently live cleanup grant.
        // The explicit test Router throws: unknown cleanup is retained, never retried.
        refuses([&] {
            channel.unsubscribe_actions(
                &identity, 302, Channel::Clock::now() + std::chrono::milliseconds(500));
        });
        assert(cleanup.unsubscribes == 1);
        auto history = channel.receipts_native_only();
        assert(history[0].delivery == uwvm_kinova::Delivery::returned);
        assert(history[1].correlation == 302 &&
               history[1].delivery == uwvm_kinova::Delivery::unknown &&
               history[1].error);
        refuses([&] {
            channel.unsubscribe_actions(
                &identity, 303, Channel::Clock::now() + std::chrono::milliseconds(500));
        });
        assert(cleanup.unsubscribes == 1);
    }
    assert(cleanup.subscribes == 1 && cleanup.unsubscribes == 1);
    return 0;
}
