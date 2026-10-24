/* Original SDK2.8.0 BaseClient, RouterClient and protobuf dispatch contract.
 * The ITransportClient implementation below is an explicit finite memory fixture.
 * It is not a native socket, ROS/Gateway peer, robot or authentication proof.
 */
#include "kortex_notifications.hpp"
#include <RouterClient.h>
#include <cassert>

namespace api = Kinova::Api;
using Channel = uwvm_kinova::KortexBoundedNotifications;

class FiniteMemoryTransport final : public api::ITransportClient {
    std::array<char, 65536> transmit_{};
    std::function<void(char const *, std::uint32_t)> receiver_;

  public:
    unsigned subscribes = 0, unsubscribes = 0, requests = 0;
    api::Frame last_request;
    std::string expected_options;

    FiniteMemoryTransport() { readyState = api::OPEN; }
    bool connect(std::string, std::uint32_t) override {
        assert(false);
        return false;
    }
    void disconnect() override {
        readyState = api::CLOSED;
        receiver_ = {};
    }
    void onMessage(std::function<void(char const *, std::uint32_t)> callback) override {
        receiver_ = std::move(callback);
    }
    char *getTxBuffer(std::uint32_t const &size) override {
        assert(size <= transmit_.size());
        return transmit_.data();
    }
    std::size_t getMaxTxBufferSize() override { return transmit_.size(); }
    void getHostAddress(std::string &host, std::uint32_t &port) override {
        host = "explicit-memory-fixture";
        port = 0;
    }
    void inject(api::Frame const &frame) {
        assert(receiver_ && frame.IsInitialized() && frame.ByteSizeLong() <= 65536);
        auto bytes = frame.SerializeAsString();
        receiver_(bytes.data(), static_cast<std::uint32_t>(bytes.size()));
    }
    void send(char const *bytes, std::uint32_t size) override {
        assert(size <= transmit_.size() && last_request.ParseFromArray(bytes, size));
        ++requests;
        api::HeaderInfo header(last_request.header());
        assert(header.m_frameInfo.frameType == api::MSG_FRAME_REQUEST);
        assert(header.m_serviceInfo.serviceId == api::eIdBase);
        std::string response;
        if (header.m_serviceInfo.functionUid == api::Base::eUidActionTopic) {
            ++subscribes;
            api::Common::NotificationOptions options;
            assert(options.ParseFromString(last_request.payload()));
            assert(options.SerializeAsString() == expected_options);
            api::Common::NotificationHandle handle;
            handle.set_identifier(19);
            assert(handle.SerializeToString(&response));
        } else {
            assert(header.m_serviceInfo.functionUid == api::Base::eUidUnsubscribe);
            ++unsubscribes;
            api::Common::NotificationHandle handle;
            assert(handle.ParseFromString(last_request.payload()) &&
                   handle.identifier() == 19);
        }
        header.m_frameInfo.frameType = api::MSG_FRAME_RESPONSE;
        header.m_payloadInfo.payloadLength =
            static_cast<std::uint32_t>(response.size());
        api::Frame reply;
        header.fillHeader(reply.mutable_header());
        reply.set_payload(response);
        inject(reply);
    }
};

int main() {
    int identity = 0;
    unsigned errors = 0;
    FiniteMemoryTransport transport;
    api::RouterClient router(&transport, [&](api::KError) { ++errors; });
    uwvm_kinova::KortexNotificationPolicy policy;
    policy.authenticated_owner = &identity;
    policy.exclusive_base_callback_slot = true;
    policy.callbacks_quiescent_at_construction = true;
    policy.subscribe = policy.poll = policy.unsubscribe = true;
    policy.active_slots = 2;
    policy.maximum_frame_bytes = 4096;
    policy.maximum_rpc_ms = 5000;
    policy.subscribe_expires_at = policy.poll_expires_at =
        Channel::Clock::now() + std::chrono::seconds(5);
    policy.unsubscribe_expires_at = Channel::Clock::now() + std::chrono::seconds(8);
    api::Common::NotificationOptions options;
    options.set_type(api::Common::NOTIFICATION_TYPE_EVENT);
    options.set_rate_m_sec(97);
    options.set_threshold_value(0.25f);
    options.GetReflection()->MutableUnknownFields(&options)->AddVarint(
        500, 0xfedcba9876543210ULL);
    transport.expected_options = options.SerializeAsString();
    api::Frame notification;
    {
        Channel channel(router, policy);
        api::Common::NotificationHandle handle;
        channel.subscribe_actions(&identity, 1,
                                  Channel::Clock::now() + std::chrono::seconds(1),
                                  options, handle);
        assert(handle.identifier() == 19 && transport.subscribes == 1 && errors == 0);
        api::Base::ActionNotification original;
        original.set_action_event(api::Base::ACTION_END);
        original.mutable_handle()->set_identifier(0xfedcba98u);
        original.mutable_timestamp()->set_sec(0xf1234567u);
        original.mutable_timestamp()->set_usec(654321);
        original.mutable_connection()->set_connection_information(
            "complete-original-Router-fixture");
        original.GetReflection()->MutableUnknownFields(&original)->AddVarint(
            501, 0xfedcba9876543210ULL);
        auto payload = original.SerializeAsString();
        api::HeaderInfo header(transport.last_request.header());
        header.m_frameInfo.frameType = api::MSG_FRAME_NOTIFICATION;
        header.m_messageInfo.messageId = handle.identifier();
        header.m_payloadInfo.payloadLength = static_cast<std::uint32_t>(payload.size());
        header.fillHeader(notification.mutable_header());
        notification.set_payload(payload);
        notification.GetReflection()
            ->MutableUnknownFields(&notification)
            ->AddVarint(502, 0xfedcba9876543210ULL);
        transport.inject(notification);
        uwvm_kinova::KortexNotificationRead read;
        assert(channel.poll(&identity, read) == 0 && read.sequence == 1 &&
               read.stream_failure == 0);
        assert(read.frame.SerializeAsString() == notification.SerializeAsString());
        api::Base::ActionNotification decoded;
        assert(uwvm_kinova::decode_kortex_action_notification(read, decoded) == 0);
        assert(decoded.SerializeAsString() == original.SerializeAsString());
        assert(channel.poll(&identity, read) == 11 && read.stream_failure == 0);
        channel.unsubscribe_actions(&identity, 2,
                                    Channel::Clock::now() + std::chrono::seconds(1));
        assert(transport.unsubscribes == 1 && errors == 0);
    }
    // Actual Router still owns the installed closure. It must see only closed
    // shared state after the BaseClient/proxy/owner have been destroyed.
    transport.inject(notification);
    assert(errors == 0 && transport.requests == 2);
    return 0;
}
