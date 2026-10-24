/* Genuine SDK2.8.0 TransportClientTcp/RouterClient/BaseClient loopback fixture.
 * No robot, ROS, authentication or external network endpoint is selected.
 * Socket server messages are explicit contract fixtures, not official peers.
 */
#include "kortex_notifications.hpp"
#include <TransportClientTcp.h>
#include <KinovaTcpUtilities.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <atomic>
#include <cassert>
#include <cerrno>
#include <condition_variable>
#include <cstring>
#include <mutex>
#include <thread>

namespace api = Kinova::Api;
using Channel = uwvm_kinova::KortexBoundedNotifications;
using Clock = std::chrono::steady_clock;

class OwnedFd {
    int value_;
  public:
    explicit OwnedFd(int value) : value_(value) { assert(value >= 0); }
    ~OwnedFd() { if (value_ >= 0) ::close(value_); }
    int get() const { return value_; }
    OwnedFd(OwnedFd const &) = delete;
    OwnedFd &operator=(OwnedFd const &) = delete;
};

static void transfer(int fd, unsigned char *bytes, std::size_t size, bool writing) {
    auto deadline = Clock::now() + std::chrono::seconds(3);
    std::size_t offset = 0;
    while (offset < size) {
        auto remaining = std::chrono::duration_cast<std::chrono::milliseconds>(deadline - Clock::now()).count();
        assert(remaining > 0 && remaining <= 3000);
        pollfd observed = {fd, static_cast<short>(writing ? POLLOUT : POLLIN), 0};
        int status = ::poll(&observed, 1, static_cast<int>(remaining));
        if (status < 0 && errno == EINTR) continue;
        assert(status == 1 && (observed.revents & observed.events));
        ssize_t count = writing
            ? ::send(fd, bytes + offset, size - offset, MSG_NOSIGNAL)
            : ::recv(fd, bytes + offset, size - offset, 0);
        if (count < 0 && (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)) continue;
        assert(count > 0 && static_cast<std::size_t>(count) <= size - offset);
        offset += static_cast<std::size_t>(count);
    }
}

static api::Frame receive_frame(int fd, KinovaTcpUtilities &framing) {
    std::array<unsigned char, 32> header{};
    assert(framing.KINOVA_HEADER_SIZE <= header.size());
    transfer(fd, header.data(), framing.KINOVA_HEADER_SIZE, false);
    assert(!std::memcmp(header.data(), framing.KINOVA_MAGIC_STRING.data(), framing.KINOVA_MAGIC_STRING.size()));
    std::uint32_t length = framing.ParseBufferHeader(header.data());
    assert(length > 0 && length <= 4096);
    std::array<unsigned char, 4096> bytes{};
    transfer(fd, bytes.data(), length, false);
    api::Frame frame;
    assert(frame.ParseFromArray(bytes.data(), static_cast<int>(length)) && frame.IsInitialized());
    return frame;
}

static void send_frame(int fd, KinovaTcpUtilities &framing, api::Frame const &frame) {
    std::string body = frame.SerializeAsString();
    std::array<unsigned char, 4128> bytes{};
    assert(!body.empty() && body.size() <= 4096 && framing.KINOVA_HEADER_SIZE <= 32);
    std::memcpy(bytes.data() + framing.KINOVA_HEADER_SIZE, body.data(), body.size());
    framing.PrependHeader(bytes.data(), static_cast<std::uint32_t>(body.size()));
    assert(framing.ParseBufferHeader(bytes.data()) == body.size());
    transfer(fd, bytes.data(), framing.KINOVA_HEADER_SIZE + body.size(), true);
}

/* The real original TCP connect/send/receive implementation is inherited.
 * Observation wraps its actual complete-frame callback, without replacing wire
 * IO, Router dispatch, protobuf stubs or the candidate's lifetime closure.
 */
class ObservedOriginalTcp final : public api::TransportClientTcp {
  public:
    std::atomic<unsigned> completed_callbacks{0};
    ObservedOriginalTcp() : api::TransportClientTcp(true) {}
    ~ObservedOriginalTcp() override { disconnect(); }
    void onMessage(std::function<void(char const *, std::uint32_t)> callback) override {
        api::TransportClientTcp::onMessage([this, callback](char const *bytes, std::uint32_t size) {
            callback(bytes, size);
            ++completed_callbacks;
        });
    }
};

int main() {
    OwnedFd listener(::socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0));
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = 0;
    assert(::bind(listener.get(), reinterpret_cast<sockaddr *>(&address), sizeof address) == 0);
    socklen_t address_size = sizeof address;
    assert(::getsockname(listener.get(), reinterpret_cast<sockaddr *>(&address), &address_size) == 0);
    assert(address_size == sizeof address && ntohl(address.sin_addr.s_addr) == INADDR_LOOPBACK && address.sin_port != 0);
    assert(::listen(listener.get(), 1) == 0);
    api::Common::NotificationOptions options;
    options.set_type(api::Common::NOTIFICATION_TYPE_EVENT);
    options.set_rate_m_sec(97);
    options.set_threshold_value(0.25f);
    options.GetReflection()->MutableUnknownFields(&options)->AddVarint(500, 0xfedcba9876543210ULL);
    api::Base::ActionNotification original;
    original.set_action_event(api::Base::ACTION_END);
    original.mutable_handle()->set_identifier(0xfedcba98u);
    original.mutable_timestamp()->set_sec(0xf1234567u);
    original.mutable_timestamp()->set_usec(654321);
    original.mutable_connection()->set_connection_information("actual-original-TCP-loopback-fixture");
    original.GetReflection()->MutableUnknownFields(&original)->AddVarint(501, 0xfedcba9876543210ULL);
    std::mutex mutex;
    std::condition_variable changed;
    unsigned stage = 0, requests = 0;
    std::string expected_notification;
    auto set_stage = [&](unsigned value) {
        std::lock_guard<std::mutex> lock(mutex);
        assert(value > stage);
        stage = value;
        changed.notify_all();
    };
    auto wait_stage = [&](unsigned value) {
        std::unique_lock<std::mutex> lock(mutex);
        assert(changed.wait_until(lock, Clock::now() + std::chrono::seconds(3), [&] { return stage >= value; }));
    };
    std::thread server([&] {
        pollfd incoming = {listener.get(), POLLIN, 0};
        assert(::poll(&incoming, 1, 3000) == 1 && (incoming.revents & POLLIN));
        OwnedFd peer(::accept4(listener.get(), nullptr, nullptr, SOCK_CLOEXEC | SOCK_NONBLOCK));
        KinovaTcpUtilities framing;
        api::Frame request = receive_frame(peer.get(), framing);
        ++requests;
        api::HeaderInfo header(request.header());
        assert(header.m_frameInfo.frameType == api::MSG_FRAME_REQUEST && header.m_serviceInfo.serviceId == api::eIdBase && header.m_serviceInfo.functionUid == api::Base::eUidActionTopic);
        assert(request.payload() == options.SerializeAsString());
        api::Common::NotificationHandle handle;
        handle.set_identifier(19);
        header.m_frameInfo.frameType = api::MSG_FRAME_RESPONSE;
        api::Frame reply;
        reply.set_payload(handle.SerializeAsString());
        header.m_payloadInfo.payloadLength = static_cast<std::uint32_t>(reply.payload().size());
        header.fillHeader(reply.mutable_header());
        api::Frame notification;
        header.m_frameInfo.frameType = api::MSG_FRAME_NOTIFICATION;
        header.m_messageInfo.messageId = 19;
        notification.set_payload(original.SerializeAsString());
        header.m_payloadInfo.payloadLength = static_cast<std::uint32_t>(notification.payload().size());
        header.fillHeader(notification.mutable_header());
        notification.GetReflection()->MutableUnknownFields(&notification)->AddVarint(502, 0xfedcba9876543210ULL);
        {
            std::lock_guard<std::mutex> lock(mutex);
            expected_notification = notification.SerializeAsString();
        }
        send_frame(peer.get(), framing, reply);
        wait_stage(1);
        send_frame(peer.get(), framing, notification);
        request = receive_frame(peer.get(), framing);
        ++requests;
        api::HeaderInfo unsubscribe_header(request.header());
        assert(unsubscribe_header.m_frameInfo.frameType == api::MSG_FRAME_REQUEST && unsubscribe_header.m_serviceInfo.serviceId == api::eIdBase && unsubscribe_header.m_serviceInfo.functionUid == api::Base::eUidUnsubscribe);
        api::Common::NotificationHandle selected;
        assert(selected.ParseFromString(request.payload()) && selected.identifier() == 19);
        unsubscribe_header.m_frameInfo.frameType = api::MSG_FRAME_RESPONSE;
        unsubscribe_header.m_payloadInfo.payloadLength = 0;
        reply.Clear();
        unsubscribe_header.fillHeader(reply.mutable_header());
        reply.set_payload("");
        send_frame(peer.get(), framing, reply);
        wait_stage(2);
        send_frame(peer.get(), framing, notification);
        wait_stage(3);
    });
    ObservedOriginalTcp transport;
    assert(transport.connect("127.0.0.1", ntohs(address.sin_port)));
    std::atomic<unsigned> errors{0};
    api::RouterClient router(&transport, [&](api::KError) { ++errors; });
    int identity = 0;
    uwvm_kinova::KortexNotificationPolicy policy;
    policy.authenticated_owner = &identity; // Fixture trust only; no native authentication claim.
    policy.exclusive_base_callback_slot = policy.callbacks_quiescent_at_construction = true;
    policy.subscribe = policy.poll = policy.unsubscribe = true;
    policy.active_slots = 2;
    policy.maximum_frame_bytes = 4096;
    policy.maximum_rpc_ms = 5000;
    policy.subscribe_expires_at = policy.poll_expires_at = Clock::now() + std::chrono::seconds(8);
    policy.unsubscribe_expires_at = Clock::now() + std::chrono::seconds(12);
    {
        Channel channel(router, policy);
        api::Common::NotificationHandle handle;
        channel.subscribe_actions(&identity, 1, Clock::now() + std::chrono::seconds(2), options, handle);
        assert(handle.identifier() == 19);
        set_stage(1);
        uwvm_kinova::KortexNotificationRead read;
        auto deadline = Clock::now() + std::chrono::seconds(2);
        std::uint32_t code;
        do {
            code = channel.poll(&identity, read);
            if (code == 11) std::this_thread::sleep_for(std::chrono::milliseconds(1));
        } while (code == 11 && Clock::now() < deadline);
        assert(code == 0 && read.sequence == 1 && read.stream_failure == 0);
        {
            std::lock_guard<std::mutex> lock(mutex);
            assert(read.frame.SerializeAsString() == expected_notification);
        }
        api::Base::ActionNotification decoded;
        assert(uwvm_kinova::decode_kortex_action_notification(read, decoded) == 0 && decoded.SerializeAsString() == original.SerializeAsString());
        channel.unsubscribe_actions(&identity, 2, Clock::now() + std::chrono::seconds(2));
    }
    set_stage(2); // Deliver through the actual socket after candidate/BaseClient destruction.
    auto deadline = Clock::now() + std::chrono::seconds(2);
    while (transport.completed_callbacks.load() < 4 && Clock::now() < deadline)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    assert(transport.completed_callbacks.load() == 4 && errors.load() == 0);
    set_stage(3);
    server.join();
    assert(requests == 2);
    transport.disconnect();
    return 0;
}
