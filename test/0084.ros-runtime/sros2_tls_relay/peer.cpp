#include <rclcpp/rclcpp.hpp>
#include <rmw/rmw.h>
#include <std_msgs/msg/string.hpp>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509v3.h>
#include <sys/socket.h>
#include <algorithm>
#include <chrono>
#include <cerrno>
#include <cstdio>
#include <csignal>
#include <string_view>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

using namespace std::chrono_literals;
using Message = std_msgs::msg::String;
using Clock = std::chrono::steady_clock;
constexpr char topic[] = "/uwvm_test/relay";
constexpr size_t max_frame = 65;

void require(bool ok, char const* reason)
{
    if(!ok) throw std::runtime_error(reason);
}

constexpr bool valid(std::string_view value, char kind)
{
    auto hex = [](char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); };
    return value.size() == (kind == 'Q' ? 33u : 65u) && value[0] == kind &&
           std::all_of(value.begin() + 1, value.end(), hex);
}

constexpr std::string_view sample_request = "Q0123456789abcdef0123456789abcdef";
static_assert(valid(sample_request, 'Q') && !valid(sample_request, 'A'));
static_assert(!valid("Q0123456789abcdef0123456789abcdef0", 'Q'));
static_assert(!valid("Q0123456789abcdef0123456789abcdeZ", 'Q'));

class Channel
{
    std::unique_ptr<SSL_CTX, decltype(&SSL_CTX_free)> ctx{nullptr, SSL_CTX_free};
    std::unique_ptr<SSL, decltype(&SSL_free)> tls{nullptr, SSL_free};
    void read_all(char* data, size_t length)
    {
        while(length)
        {
            size_t count{};
            require(SSL_read_ex(tls.get(), data, length, &count) == 1 && count > 0, "TLS read failed");
            data += count;
            length -= count;
        }
    }
public:
    explicit Channel(bool server)
    {
        timeval timeout{9,0};
        require(setsockopt(0, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == 0 &&
                setsockopt(0, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) == 0, "missing relay socket");
        ctx.reset(SSL_CTX_new(TLS_method()));
        require(ctx && SSL_CTX_set_min_proto_version(ctx.get(), TLS1_3_VERSION) &&
                SSL_CTX_set_max_proto_version(ctx.get(), TLS1_3_VERSION), "select TLS 1.3");
        SSL_CTX_set_verify(ctx.get(), SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, nullptr);
        require(SSL_CTX_load_verify_locations(ctx.get(), "/tls/ca.pem", nullptr) == 1 &&
                SSL_CTX_use_certificate_chain_file(ctx.get(), "/tls/cert.pem") == 1 &&
                SSL_CTX_use_PrivateKey_file(ctx.get(), "/tls/key.pem", SSL_FILETYPE_PEM) == 1 &&
                SSL_CTX_check_private_key(ctx.get()) == 1, "load relay identity");
        tls.reset(SSL_new(ctx.get()));
        char const* peer = server ? "cyclone-bridge.test" : "fast-bridge.test";
        require(tls && SSL_set_fd(tls.get(), 0) == 1, "attach relay socket");
        SSL_set_hostflags(tls.get(), X509_CHECK_FLAG_NEVER_CHECK_SUBJECT | X509_CHECK_FLAG_NO_WILDCARDS);
        require(SSL_set1_host(tls.get(),peer) == 1, "set exact peer SAN");
        if((server ? SSL_accept(tls.get()) : SSL_connect(tls.get())) != 1)
        {
            std::fprintf(stderr,"TLS handshake failed; verify=%ld\n", SSL_get_verify_result(tls.get()));
            ERR_print_errors_fp(stderr);
            throw std::runtime_error("TLS handshake refused");
        }
        require(SSL_get_verify_result(tls.get()) == X509_V_OK && SSL_get0_peer_certificate(tls.get()) != nullptr,
                "missing verified TLS peer");
        // A TLS 1.3 client can finish locally before the server rejects its certificate.
        if(server) { send("S"); require(receive() == "C", "client confirmation"); }
        else { require(receive() == "S", "server confirmation"); send("C"); }
        std::printf("verified %s %s peer=%s\n", SSL_get_version(tls.get()), SSL_get_cipher_name(tls.get()), peer);
    }
    void send(std::string const& value)
    {
        require(!value.empty() && value.size() <= max_frame, "invalid frame length");
        std::string packet(1, static_cast<char>(value.size()));
        packet += value;
        size_t sent{};
        require(SSL_write_ex(tls.get(),packet.data(),packet.size(),&sent) == 1 && sent == packet.size(),"TLS write failed");
    }
    std::string receive()
    {
        char length{};
        read_all(&length,1);
        size_t size = static_cast<unsigned char>(length);
        require(size > 0 && size <= max_frame, "invalid frame length");
        std::string value(size,'\0');
        read_all(value.data(),size);
        return value;
    }
};

int main(int argc, char** argv)
{
    if(argc < 6) return 1;
    std::signal(SIGPIPE, SIG_IGN);
    try
    {
        std::string role = argv[1], expected_rmw = argv[2];
        bool left = role == "request" || role == "fast-bridge";
        bool bridge = role == "fast-bridge" || role == "cyclone-bridge";
        require(left || role == "echo" || role == "cyclone-bridge", "unknown role");
        auto denied_read = [](char const* path) {
            if(auto f = std::fopen(path, "rb")) { std::fclose(f); return false; }
            return errno == EACCES;
        };
        require(denied_read(bridge ? "/authority/enclaves/app/key.pem" : "/authority/enclaves/bridge/key.pem"),
                "foreign enclave key readable");
        if(!bridge) require(denied_read("/tls/key.pem"),"application can read relay TLS key");
        std::unique_ptr<Channel> channel;
        if(bridge) channel = std::make_unique<Channel>(left);
        std::string request, reply;
        if(!bridge)
        {
            request = std::string("Q") + argv[3];
            reply = std::string("A") + argv[3] + argv[4];
            require(valid(request,'Q') && valid(reply,'A'), "invalid nonce");
        }
        rclcpp::init(argc,argv);
        require(rmw_get_implementation_identifier() == expected_rmw, "unexpected RMW");
        rclcpp::NodeOptions options;
        options.enable_rosout(false).start_parameter_services(false).start_parameter_event_publisher(false);
        options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
        auto node = std::make_shared<rclcpp::Node>(bridge ? "relay_bridge" : "relay_app","/uwvm_test",options);
        rclcpp::executors::SingleThreadedExecutor exec;
        exec.add_node(node);
        auto writer = node->create_publisher<Message>(topic, 4);
        std::string observed;
        const char receive_kind = (role == "echo" || role == "fast-bridge") ? 'Q' : 'A';
        auto reader = node->create_subscription<Message>(topic, 4, [&](Message const& msg) {
            if(observed.empty() && valid(msg.data, receive_kind))
            {
                if(!bridge && msg.data != (left ? reply : request)) return;
                if(bridge && !left && msg.data.substr(1,32) != request.substr(1)) return;
                observed = msg.data;
                std::printf("%s received %s\n", role.c_str(), observed.c_str());
            }
        });
        std::puts("DDS ready");
        std::fflush(stdout);
        if(bridge && !left)
        {
            request = channel->receive();
            require(valid(request,'Q'),"invalid request frame");
        }
        auto publish = [&](std::string const& value) { Message m; m.data=value; writer->publish(m); };
        auto deadline = Clock::now()+8s;
        while(observed.empty() && Clock::now()<deadline)
        {
            if((left && !bridge) || (!left && bridge)) publish(request);
            exec.spin_some();
            std::this_thread::sleep_for(25ms);
        }
        if(observed.empty()) { rclcpp::shutdown(); std::puts("no matching sample"); return 4; }
        if(bridge)
        {
            channel->send(observed);
            if(!left) { rclcpp::shutdown(); return 0; }
            reply = channel->receive();
            require(valid(reply,'A') && reply.substr(1,32) == observed.substr(1),"invalid correlated reply");
        }
        if((left && bridge) || (!left && !bridge))
        {
            auto flush_until = Clock::now()+450ms;
            do { publish(reply); std::this_thread::sleep_for(25ms); } while(Clock::now()<flush_until);
        }
        rclcpp::shutdown();
        return 0;
    }
    catch(std::exception const& e) { std::fprintf(stderr,"%s\n",e.what()); return 5; }
}
