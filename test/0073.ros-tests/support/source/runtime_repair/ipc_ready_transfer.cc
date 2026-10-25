/**
 * @file ipc_ready_transfer.cc
 * @brief Verify ready I/O, expired deadlines, partial frames and EOF on real sockets.
 * @details These checks exercise the production transport. No socket or clock
 * is substituted, and all local descriptors belong exclusively to this test.
 */
#include <uwvm2/imported/ros/ipc/unix.h>
#include <array>
#include <cassert>
#include <iostream>
#include <thread>

namespace ipc = uwvm2::imported::ros::ipc;

struct socket_pair
{
    int value[2]{-1, -1};

    socket_pair() { assert(::socketpair(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0, value) == 0); }

    socket_pair(socket_pair const&) = delete;
    socket_pair& operator= (socket_pair const&) = delete;

    ~socket_pair()
    {
        for(int fd: value)
        {
            if(fd >= 0) { ::close(fd); }
        }
    }
};

/// @brief A ready descriptor must never bypass an already expired budget.
static void expired_ready_io()
{
    socket_pair pair;
    std::uint8_t byte{7}, output{99};
    auto const expired{ipc::after(0)};
    assert(!ipc::transfer(pair.value[0], &byte, 1, true, expired) && errno == ETIMEDOUT);
    assert(::recv(pair.value[1], &output, 1, MSG_DONTWAIT) == -1 && errno == EAGAIN);
    assert(::send(pair.value[0], &byte, 1, MSG_DONTWAIT | MSG_NOSIGNAL) == 1);
    assert(!ipc::transfer(pair.value[1], &output, 1, false, expired) && errno == ETIMEDOUT && output == 99);
    assert(ipc::transfer(pair.value[1], &output, 1, false, ipc::after(1000)) && output == byte);
}

/// @brief A partial frame must survive backpressure and retain byte-for-byte identity.
static void bounded_frame_roundtrip()
{
    socket_pair pair;
    int capacity{1024};
    assert(::setsockopt(pair.value[0], SOL_SOCKET, SO_SNDBUF, &capacity, sizeof(capacity)) == 0);
    ipc::frame request;
    request.type = ipc::kind::request;
    request.backend = 2;
    request.session_id = 71;
    request.request_id = 19;
    request.timeout_ms = 2000;
    request.payload.resize(256 * 1024);
    for(std::size_t i{}; i != request.payload.size(); ++i) { request.payload[i] = static_cast<std::uint8_t>(i); }
    bool written{};
    std::thread writer{[&] { written = ipc::write_frame(pair.value[0], request, ipc::after(2000)); }};
    ipc::frame received;
    bool const read{ipc::read_frame(pair.value[1], received, ipc::after(2000))};
    writer.join();
    assert(written && read && received.type == request.type && received.backend == request.backend);
    assert(received.session_id == request.session_id && received.request_id == request.request_id);
    assert(received.payload == request.payload);
}

/// @brief Unready input times out under the original deadline; EOF remains distinct.
static void timeout_and_eof()
{
    socket_pair pair;
    std::uint8_t output{99};
    auto const began{std::chrono::steady_clock::now()};
    assert(!ipc::transfer(pair.value[1], &output, 1, false, ipc::after(20)) && errno == ETIMEDOUT);
    assert(output == 99 && std::chrono::steady_clock::now() - began < std::chrono::seconds(1));
    assert(::shutdown(pair.value[0], SHUT_WR) == 0);
    assert(!ipc::transfer(pair.value[1], &output, 1, false, ipc::after(1000)) && errno == ECONNRESET);
    assert(output == 99);
}

/// @brief Coalescing and segmented writes must emit the identical public wire bytes.
static void exact_wire_bytes()
{
    for(std::size_t size: {0u, 1u, 1024u, 1025u})
    {
        socket_pair pair;
        ipc::frame request;
        request.type = ipc::kind::request;
        request.backend = 2;
        request.session_id = 71;
        request.request_id = 19;
        request.timeout_ms = 2000;
        request.payload.resize(size);
        for(std::size_t i{}; i != size; ++i) { request.payload[i] = static_cast<std::uint8_t>(i * 17u); }
        auto const header{ipc::encode_header_fixed(request)};
        std::vector<std::uint8_t> expected(header.begin(), header.end());
        expected.insert(expected.end(), request.payload.begin(), request.payload.end());
        assert(!ipc::write_frame(pair.value[0], request, ipc::after(0)) && errno == ETIMEDOUT);
        std::uint8_t probe{};
        assert(::recv(pair.value[1], &probe, 1, MSG_DONTWAIT) == -1 && errno == EAGAIN);
        assert(ipc::write_frame(pair.value[0], request, ipc::after(2000)));
        std::vector<std::uint8_t> actual(expected.size());
        assert(ipc::transfer(pair.value[1], actual.data(), actual.size(), false, ipc::after(2000)));
        assert(actual == expected);
        assert(::recv(pair.value[1], &probe, 1, MSG_DONTWAIT) == -1 && errno == EAGAIN);
    }
}

int main()
{
    expired_ready_io();
    bounded_frame_roundtrip();
    timeout_and_eof();
    exact_wire_bytes();
    std::cout << "PASS_IPC_READY_TRANSFER groups=4\n";
}
