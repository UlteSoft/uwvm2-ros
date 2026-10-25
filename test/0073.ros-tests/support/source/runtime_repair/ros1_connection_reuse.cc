/**
 * @file ros1_connection_reuse.cc
 * @brief Verify retained TCPROS streams without weakening bounded-call contracts.
 * @details An owned numeric ROS Master supplies discovery. A local TCPROS peer
 * counts handshakes and requests and supplies deterministic response failures.
 * Assertions remain active in release builds. No request may be replayed, and
 * an expired waiter must not interrupt the request already owning the stream.
 */
#include <platform/ros1/native_gateway/bounded_service.h>
#include <std_srvs/SetBool.h>
#include <atomic>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

namespace
{
    namespace gateway = uwvm2::ros_gateway;
    namespace ipc = uwvm2::imported::ros::ipc;
    using clock_type = std::chrono::steady_clock;
    using service_type = std_srvs::SetBool;

    void require(bool condition, char const* detail)
    {
        if(!condition) { throw std::runtime_error(detail); }
    }

    bool body_valid(std::vector<std::uint8_t> const& body)
    {
        return body.size() >= 5 && body[0] <= 1 && ipc::get(body.data() + 1, 4) == body.size() - 5 &&
               gateway::valid_utf8(body.data() + 5, body.size() - 5);
    }

    /** @brief Own the fault peer and count actual wire effects independently. */
    struct peer
    {
        gateway::native_io::owned_fd listener{::socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0)};
        std::atomic<unsigned> connections{}, requests{}, closed{};
        std::atomic<char> next{'v'};
        std::atomic<char> next_header{'v'};
        std::atomic<bool> stop{}, failed{};
        std::thread worker;
        unsigned port{};

        peer()
        {
            require(listener.get() >= 0, "listener");
            sockaddr_in address{};
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            require(::bind(listener.get(), reinterpret_cast<sockaddr*>(&address), sizeof address) == 0 &&
                    ::listen(listener.get(), 8) == 0, "bind/listen");
            socklen_t size{sizeof address};
            require(::getsockname(listener.get(), reinterpret_cast<sockaddr*>(&address), &size) == 0, "listener port");
            port = ntohs(address.sin_port);
            worker = std::thread{[this]
            {
                try { serve(); }
                catch(...) { failed = true; }
            }};
        }

        ~peer()
        {
            stop = true;
            if(worker.joinable()) { worker.join(); }
        }

        void serve()
        {
            while(!stop)
            {
                pollfd ready{listener.get(), POLLIN, 0};
                if(::poll(&ready, 1, 20) <= 0) { continue; }
                gateway::native_io::owned_fd socket{::accept4(listener.get(), nullptr, nullptr, SOCK_CLOEXEC | SOCK_NONBLOCK)};
                require(socket.get() >= 0, "accept");
                ++connections;
                auto const until{ipc::after(3000)};
                std::array<std::uint8_t, 4> length{};
                require(ipc::transfer(socket.get(), length.data(), 4, false, until), "header length");
                auto const count{ipc::get(length.data(), 4)};
                require(count <= 8192, "bounded header");
                std::vector<std::uint8_t> header(static_cast<std::size_t>(count));
                require(ipc::transfer(socket.get(), header.data(), header.size(), false, until), "header body");
                std::string_view fields{reinterpret_cast<char const*>(header.data()), header.size()};
                require(fields.find("persistent=1") != std::string_view::npos, "persistent handshake");
                header.clear();
                auto const header_mode{next_header.exchange('v')};
                gateway::ros1_service::header_field(header, "md5sum", header_mode == 'd' ? "incorrect" : ros::service_traits::MD5Sum<service_type>::value());
                gateway::ros1_service::header_field(header, "type", header_mode == 'h' ? "foreign/Service" : ros::service_traits::DataType<service_type>::value());
                std::vector<std::uint8_t> record;
                ipc::put(record, header.size(), 4);
                record.insert(record.end(), header.begin(), header.end());
                require(ipc::transfer(socket.get(), record.data(), record.size(), true, until), "peer header");
                while(!stop)
                {
                    pollfd input{socket.get(), POLLIN, 0};
                    if(::poll(&input, 1, 20) <= 0) { continue; }
                    auto const deadline{ipc::after(1000)};
                    if(!ipc::transfer(socket.get(), length.data(), 4, false, deadline)) { break; }
                    require(ipc::get(length.data(), 4) == 1, "request length");
                    std::uint8_t value{};
                    require(ipc::transfer(socket.get(), &value, 1, false, deadline) && value == 1, "request value");
                    auto const mode{next.exchange('v')};
                    ++requests;
                    if(mode == 'c') { break; }
                    if(mode == 's') { std::this_thread::sleep_for(std::chrono::milliseconds(150)); }
                    std::vector<std::uint8_t> reply{1, 7, 0, 0, 0, 1, 2, 0, 0, 0, 'o', 'k'};
                    if(mode == 'm') { reply[5] = 2; }
                    if(mode == 'l') { reply[1] = 100; reply.resize(5); }
                    if(!ipc::transfer(socket.get(), reply.data(), reply.size(), true, deadline)) { break; }
                }
                ++closed;
            }
        }
    };

    /** @brief Wait for a measured peer effect, never infer reuse from elapsed time. */
    void await(std::atomic<unsigned> const& value, unsigned expected)
    {
        auto const until{ipc::after(2000)};
        while(value < expected && clock_type::now() < until) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }
        require(value == expected, "exact peer counter");
    }
}

int main(int argc, char** argv)
try
{
    ros::init(argc, argv, "retained_transport_test", ros::init_options::NoRosout);
    ros::NodeHandle node;
    peer server;
    gateway::service_client_pool pool;
    uwvm2::imported::ros::capability_spec cap;
    cap.alias = "bounded";
    cap.operation = "service.call";
    cap.resource = "/uwvm_reuse/boolean";
    cap.type = ros::service_traits::DataType<service_type>::value();
    cap.type_identity = ros::service_traits::MD5Sum<service_type>::value();
    cap.limits = {{"timeout-ms", "1000"}, {"max-request-bytes", "1"}, {"max-response-bytes", "64"}};
    XmlRpc::XmlRpcValue args, result;
    // A distinct peer name must not replace this client's registered node URI.
    args[0] = "/uwvm_reuse/fault_peer"; args[1] = cap.resource;
    args[2] = "rosrpc://127.0.0.1:" + std::to_string(server.port);
    args[3] = "http://127.0.0.1:1";
    gateway::ros1_resources::bounded_xmlrpc master{ros::master::getHost(), static_cast<int>(ros::master::getPort()), 4096};
    require(master.execute_bounded("registerService", args, result, 2000) == 0 && static_cast<int>(result[0]) == 1, "register");
    auto call = [&](unsigned timeout, auto const& selected)
    {
        service_type service;
        service.request.data = true;
        service.response.message = "untouched";
        auto const status{gateway::ros1_service::call(selected, service, timeout, body_valid, &pool)};
        if(status == 0) { require(service.response.success && service.response.message == "ok", "response"); }
        else { require(service.response.message == "untouched", "failed response changed output"); }
        return status;
    };
    for(unsigned i{}; i != 4; ++i) { require(call(1000, cap) == 0, "retained success"); }
    require(server.connections == 1 && server.requests == 4, "connection not reused");
    auto changed{cap}; changed.limits["max-response-bytes"] = "63";
    require(call(1000, changed) == 13 && server.requests == 4, "changed contract admitted");
    pool.close(cap.alias);
    require(call(1000, cap) == 0 && server.connections == 2 && server.requests == 5, "retirement");

    for(char mode: {'h', 'd'})
    {
        pool.close(cap.alias);
        unsigned const before{server.requests};
        server.next_header = mode;
        require(call(1000, cap) == 13 && server.requests == before, "wrong peer identity sent a request");
        require(call(1000, cap) == 0 && server.requests == before + 1, "fresh handshake after identity refusal");
    }

    for(auto test: {std::pair{'s', 110u}, std::pair{'m', 22u}, std::pair{'l', 90u}, std::pair{'c', 107u}})
    {
        unsigned const before{server.requests};
        server.next = test.first;
        auto const begin{clock_type::now()};
        require(call(test.first == 's' ? 40 : 1000, cap) == test.second, "failure status");
        require(clock_type::now() - begin < std::chrono::milliseconds(500), "deadline exceeded");
        require(server.requests == before + 1, "request replayed");
        require(call(1000, cap) == 0 && server.requests == before + 2, "fresh call after refusal");
    }
    unsigned const before{server.requests};
    server.next = 's';
    std::atomic<unsigned> first_status{999};
    std::thread first{[&] { first_status = call(1000, cap); }};
    await(server.requests, before + 1);
    auto const blocked{call(20, cap)};
    first.join();
    require(blocked == 110 && first_status == 0 && server.requests == before + 1, "waiter deadline or replay");
    require(call(1000, cap) == 0 && server.requests == before + 2, "waiter damaged active stream");
    pool.close(cap.alias);
    await(server.closed, server.connections.load());
    require(!server.failed, "peer invariant");
    ros::shutdown();
    std::cout << "PASS persistent TCPROS: reuse, full contract, retirement, peer identity, timeout, malformed, bound, EOF, no replay, concurrent deadline\n";
    return 0;
}
catch(std::exception const& error)
{
    std::cerr << "FAIL persistent TCPROS: " << error.what() << '\n';
    return 1;
}
