/**
 * @file ros1_xmlrpc_reuse.cc
 * @brief Verify bounded XMLRPC reuse against independently counted wire effects.
 * @details A local peer supplies finite valid, closed, delayed and invalid
 * responses. Reuse must preserve exact authority, response bounds, deadlines
 * and no-replay behavior. A real ROS Master separately proves that transport
 * retention does not cache parameter values. Assertions remain active in release.
 */
#include <platform/ros1/native_gateway/resources.h>
#include <gateway/common/native_io.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <atomic>
#include <iostream>
#include <stdexcept>
#include <thread>

namespace
{
    namespace gateway = uwvm2::ros_gateway;
    namespace resource = gateway::ros1_resources;
    namespace ipc = uwvm2::imported::ros::ipc;
    using capability = uwvm2::imported::ros::capability_spec;

    void require(bool condition, char const* detail)
    {
        if(!condition) { throw std::runtime_error(detail); }
    }

    /** @brief Own the only peer listener and count completed request envelopes. */
    struct peer
    {
        gateway::native_io::owned_fd listener{::socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0)};
        std::atomic<unsigned> connections{}, requests{}, closed{};
        std::atomic<char> next{'v'};
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

        /** @brief Read the production client's bounded, length-delimited request. */
        bool request(int socket)
        {
            auto const until{ipc::after(2000)};
            std::string header;
            while(header.size() < 8192)
            {
                std::uint8_t byte{};
                if(!ipc::transfer(socket, &byte, 1, false, until)) { return false; }
                header.push_back(static_cast<char>(byte));
                if(header.size() >= 4 && header.compare(header.size() - 4, 4, "\r\n\r\n") == 0) { break; }
            }
            auto const begin{header.find("Content-length: ")};
            require(begin != std::string::npos, "request length header");
            auto const length{std::stoul(header.substr(begin + 16))};
            require(length <= 4096, "bounded request");
            std::vector<std::uint8_t> body(length);
            return ipc::transfer(socket, body.data(), body.size(), false, until);
        }

        void serve()
        {
            while(!stop)
            {
                pollfd ready{listener.get(), POLLIN, 0};
                if(::poll(&ready, 1, 20) <= 0) { continue; }
                gateway::native_io::owned_fd socket{::accept4(listener.get(), nullptr, nullptr, SOCK_CLOEXEC | SOCK_NONBLOCK)};
                require(socket.get() >= 0, "accept");
                int enabled{1};
                require(::setsockopt(socket.get(), IPPROTO_TCP, TCP_NODELAY, &enabled, sizeof enabled) == 0, "TCP_NODELAY");
                ++connections;
                while(!stop)
                {
                    pollfd input{socket.get(), POLLIN, 0};
                    if(::poll(&input, 1, 20) <= 0) { continue; }
                    if(!request(socket.get())) { break; }
                    auto const mode{next.exchange('v')};
                    auto const number{++requests};
                    if(mode == 'c') { break; }
                    if(mode == 's') { std::this_thread::sleep_for(std::chrono::milliseconds(150)); }
                    std::string body{"<?xml version=\"1.0\"?><methodResponse><params><param><value><int>"};
                    body += std::to_string(number) + "</int></value></param></params></methodResponse>";
                    if(mode == 'm') { body = "<methodResponse><params>"; }
                    if(mode == 'f') { body = "<methodResponse><fault><value><int>7</int></value></fault></methodResponse>"; }
                    std::string response{mode == 'h' ? "HTTP/1.0 200 OK\r\n" : "HTTP/1.1 200 OK\r\n"};
                    response += mode == 'k' ? "Connection: close\r\n" : "Connection: keep-alive\r\n";
                    response += "Content-Length: " + std::to_string(mode == 'l' ? 4097 : body.size()) + "\r\n\r\n";
                    if(mode != 'l') { response += body; }
                    if(!ipc::transfer(socket.get(), reinterpret_cast<std::uint8_t*>(response.data()), response.size(), true,
                                      ipc::after(1000))) { break; }
                    if(mode == 'h' || mode == 'k' || mode == 'l') { break; }
                }
                ++closed;
            }
        }
    };

    void await(std::atomic<unsigned> const& value, unsigned expected)
    {
        auto const until{ipc::after(2000)};
        while(value < expected && std::chrono::steady_clock::now() < until) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }
        require(value == expected, "exact peer counter");
    }

    /** @brief Observe an externally changed live Master value through one owner. */
    void live_parameter(ros::NodeHandle& node)
    {
        gateway::service_client_pool pool;
        capability cap;
        cap.alias = "live";
        cap.operation = "param.get";
        cap.resource = "/uwvm_xmlrpc_reuse/value";
        cap.type = "builtin/string-v1";
        cap.type_identity = "builtin:param-string-v1";
        cap.limits = {{"timeout-ms", "1000"}, {"max-parameter-bytes", "1024"}};
        for(std::string const text: {"first", "changed"})
        {
            node.setParam(cap.resource, text);
            auto const result{resource::call(node, cap, {}, 1000, &pool)};
            require(result.error == 0 && result.payload.size() == text.size() + 4 &&
                    ipc::get(result.payload.data(), 4) == text.size() &&
                    std::string(result.payload.begin() + 4, result.payload.end()) == text, "stale parameter value");
        }
        node.deleteParam(cap.resource);
        require(resource::call(node, cap, {}, 1000, &pool).error != 0, "deleted parameter retained");
    }
}

int main(int argc, char** argv)
try
{
    ros::init(argc, argv, "xmlrpc_transport_test", ros::init_options::NoRosout);
    ros::NodeHandle node;
    peer server;
    gateway::service_client_pool pool;
    capability cap;
    cap.alias = "bounded";
    cap.operation = "param.get";
    cap.resource = "/uwvm_xmlrpc_reuse/value";
    cap.type = "builtin/string-v1";
    cap.type_identity = "builtin:param-string-v1";
    cap.limits = {{"timeout-ms", "1000"}, {"max-parameter-bytes", "1024"}};
    auto call = [&](unsigned timeout, capability const& selected)
    {
        auto const deadline{ipc::after(timeout)};
        return resource::with_xmlrpc(selected, "127.0.0.1", server.port, 4096, deadline, &pool,
            [&](resource::bounded_xmlrpc& client) -> ipc::call_result
            {
                XmlRpc::XmlRpcValue args, result;
                args[0] = "/uwvm_xmlrpc_reuse/client";
                auto left{std::chrono::duration_cast<std::chrono::milliseconds>(deadline - std::chrono::steady_clock::now()).count()};
                if(left <= 0) { return {110, {}}; }
                auto const error{client.execute_bounded("getParam", args, result, static_cast<std::uint32_t>(left))};
                if(error) { return {error, {}}; }
                require(result.getType() == XmlRpc::XmlRpcValue::TypeInt &&
                        static_cast<unsigned>(static_cast<int>(result)) == server.requests, "fresh response");
                return {0, {}};
            }).error;
    };
    for(unsigned n{}; n != 20; ++n) { require(call(1000, cap) == 0, "retained success"); }
    require(server.connections == 1 && server.requests == 20, "connection not reused");
    auto changed{cap};
    changed.limits["max-parameter-bytes"] = "1023";
    require(call(1000, changed) == 13 && server.requests == 20, "changed authority admitted");
    pool.close(cap.alias);
    require(call(1000, cap) == 0 && server.connections == 2 && server.requests == 21, "alias retirement");
    for(auto test: {std::pair{'s', 110u}, std::pair{'m', 107u}, std::pair{'f', 107u},
                   std::pair{'l', 90u}, std::pair{'c', 90u}})
    {
        unsigned const before{server.requests};
        unsigned const connections{server.connections};
        server.next = test.first;
        auto const begin{std::chrono::steady_clock::now()};
        require(call(test.first == 's' ? 40 : 1000, cap) == test.second, "failure status");
        require(std::chrono::steady_clock::now() - begin < std::chrono::milliseconds(500), "deadline exceeded");
        require(server.requests == before + 1, "request replayed");
        require(call(1000, cap) == 0 && server.requests == before + 2 && server.connections == connections + 1,
                "failed transport not retired");
    }
    for(char mode: {'h', 'k'})
    {
        unsigned const before{server.connections};
        server.next = mode;
        require(call(1000, cap) == 0 && call(1000, cap) == 0 && server.connections == before + 1, "nonpersistent response retained");
    }
    unsigned const before{server.requests};
    unsigned const connections{server.connections};
    server.next = 's';
    std::atomic<unsigned> first_status{999};
    std::thread first{[&] { first_status = call(1000, cap); }};
    await(server.requests, before + 1);
    auto const blocked{call(20, cap)};
    first.join();
    require(blocked == 110 && first_status == 0 && server.requests == before + 1, "waiter deadline or replay");
    require(call(1000, cap) == 0 && server.requests == before + 2 && server.connections == connections, "waiter damaged active stream");
    pool.close(cap.alias);
    await(server.closed, server.connections.load());
    require(!server.failed, "peer invariant");
    live_parameter(node);
    ros::shutdown();
    std::cout << "PASS XMLRPC: exact contract, reuse, retirement, fresh values, deadline, malformed, bounds, EOF, no replay, concurrent waiter\n";
    return 0;
}
catch(std::exception const& error)
{
    std::cerr << "FAIL XMLRPC: " << error.what() << '\n';
    return 1;
}
