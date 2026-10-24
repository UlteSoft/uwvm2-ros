/**
 * Exercise the actual Unix client handshake without ROS or a mock client.
 *
 * Each case uses a fresh owner-only filesystem socket and a real accepting
 * thread. The peer emits explicitly selected wire frames. Only the valid hello
 * and empty policy acknowledgement may leave the client session live. These
 * are transport contract cases; they do not establish ROS network delivery.
 * Build with C++17 or newer, -UNDEBUG, -pthread and -I src. Run only under the
 * project's bounded Linux test owner, with TMPDIR set to its owned output.
 */
#include <uwvm2/imported/ros/ipc/unix.h>
#include <cassert>
#include <cstdlib>
#include <string>
#include <thread>

#if defined(__linux__)
namespace ipc = uwvm2::imported::ros::ipc;
namespace
{
    struct owned_socket
    {
        int descriptor{-1};
        explicit owned_socket(int value) noexcept:descriptor{value} {}
        owned_socket(owned_socket const&)=delete;
        ~owned_socket() { if(descriptor>=0) { ::close(descriptor); } }
    };
    struct endpoint
    {
        std::string directory, path;
        owned_socket listener{::socket(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC,0)};
        endpoint()
        {
            auto temporary{std::getenv("TMPDIR")};
            assert(temporary&&temporary[0]=='/');
            directory=std::string{temporary}+"/ros-client-XXXXXX";
            assert(::mkdtemp(directory.data()));path=directory+"/socket";
            sockaddr_un address{};address.sun_family=AF_UNIX;
            assert(listener.descriptor>=0&&path.size()<sizeof address.sun_path);
            std::memcpy(address.sun_path,path.c_str(),path.size()+1);
            // No peer thread exists yet. Create the socket as0600 so the live
            // owner never observes a wider mode between bind and chmod.
            auto previous_mask{::umask(0177)};
            auto bound{::bind(listener.descriptor,reinterpret_cast<sockaddr*>(&address),sizeof address)};
            ::umask(previous_mask);
            assert(bound==0);
            assert(!::chmod(path.c_str(),0600)&&!::listen(listener.descriptor,1));
        }
        ~endpoint() { ::unlink(path.c_str());::rmdir(directory.c_str()); }
    };
    void exchange(bool malformed_hello,bool binding_body)
    {
        endpoint selected;
        std::thread peer{[&] {
            pollfd ready{selected.listener.descriptor,POLLIN,0};
            assert(::poll(&ready,1,3000)==1);
            owned_socket connection{::accept4(selected.listener.descriptor,nullptr,nullptr,SOCK_CLOEXEC|SOCK_NONBLOCK)};
            assert(connection.descriptor>=0);
            ipc::frame hello;
            assert(ipc::read_frame(connection.descriptor,hello,ipc::after(3000)));
            assert(hello.type==ipc::kind::hello&&hello.request_id==1&&hello.session_id==0);
            ipc::frame reply;reply.type=ipc::kind::response;reply.backend=hello.backend;
            reply.response_id=hello.request_id;reply.request_id=malformed_hello?7:0;
            reply.session_id=42;reply.timeout_ms=3000;
            assert(ipc::write_frame(connection.descriptor,reply,ipc::after(3000)));
            if(malformed_hello) { return; }
            ipc::frame binding;
            assert(ipc::read_frame(connection.descriptor,binding,ipc::after(3000)));
            ipc::request request;
            assert(ipc::decode_request(binding.payload,request)&&request.operation=="session.bind");
            reply.response_id=binding.request_id;reply.payload=binding_body?std::vector<std::uint8_t>{1}:std::vector<std::uint8_t>{};
            assert(ipc::write_frame(connection.descriptor,reply,ipc::after(3000)));
        }};
        ipc::client client;
        auto connected{client.connect(selected.path,1,static_cast<std::uint32_t>(::getuid()),3000)};
        if(malformed_hello) { assert(connected==13&&client.session_id()==0); }
        else
        {
            assert(connected==0&&client.session_id()==42);
            auto result{client.call("session.bind","target",{},3000)};
            assert(result.payload.empty());
            assert(result.error==(binding_body?13u:0u));
            assert(client.session_id()==(binding_body?0u:42u));
            if(binding_body) { assert(client.call("topic.publish","out",{},3000).error==107); }
        }
        peer.join();
    }
}
#endif

int main()
{
#if defined(__linux__)
    exchange(true,false);
    exchange(false,true);
    exchange(false,false);
#else
    // The actual socket contract requires Linux SO_PEERCRED. A portable codec
    // build alone must never report these three transport cases as executed.
    return 77;
#endif
}
