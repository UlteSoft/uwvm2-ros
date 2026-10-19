#include <uwvm2/imported/ros/ipc/unix.h>
#include <cassert>
#include <thread>
#include <iostream>
using namespace uwvm2::imported::ros::ipc;
int main()
{
    frame f; f.backend=1; f.timeout_ms=1000; f.request_id=2; f.session_id=42; f.payload={1,2,3};
    auto bytes=encode_header(f); frame decoded; std::uint32_t length{};
    assert(bytes.size()==48&&decode_header(bytes.data(),bytes.size(),decoded,length)&&length==3&&decoded.session_id==42);
    for(auto pos:{0,4,6,8,9}) { auto bad=bytes; bad[pos]=255; assert(!decode_header(bad.data(),bad.size(),decoded,length)); }
    auto oversized=bytes; for(unsigned i=36;i<40;++i) { oversized[i]=255; } assert(!decode_header(oversized.data(),oversized.size(),decoded,length));
    request r{"topic.publish","out",{0,1,2}},r2; std::vector<std::uint8_t> data;
    assert(encode_request(r,data)&&decode_request(data,r2)&&r2.operation==r.operation&&r2.data==r.data);
    data.push_back(0); assert(!decode_request(data,r2));
    assert(!encode_request({"topic.publish","../attack",{}},data));
#if defined(__linux__)
    int fds[2]; assert(!::socketpair(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC,0,fds));
    std::uint32_t uid{},pid{}; assert(peer_credentials(fds[0],uid,pid)&&uid==::getuid()&&pid==static_cast<std::uint32_t>(::getpid()));
    std::thread sender{[&] { assert(write_frame(fds[0],f,after(1000))); }};
    assert(read_frame(fds[1],decoded,after(1000))&&decoded.payload==f.payload); sender.join();
    auto start=std::chrono::steady_clock::now(); assert(!read_frame(fds[1],decoded,after(20))&&errno==ETIMEDOUT);
    assert(std::chrono::steady_clock::now()-start<std::chrono::seconds(1));
    int cancellation[2];assert(!::pipe(cancellation));std::uint8_t wake{1};assert(::write(cancellation[1],&wake,1)==1);
    start=std::chrono::steady_clock::now();assert(!read_frame(fds[1],decoded,after(1000),cancellation[0])&&errno==ECANCELED);
    assert(std::chrono::steady_clock::now()-start<std::chrono::milliseconds(100));::close(cancellation[0]);::close(cancellation[1]);::close(fds[0]); ::close(fds[1]);
    assert(random_session()!=0&&random_session()!=0);
    std::cout<<"PASS Linux SO_PEERCRED, framing, oversized/truncated payload, timeout, worker-lifetime cancellation\n";
#else
    client c; assert(c.connect("/tmp/unused",1,0,1000)==95);
    std::cout<<"PASS portable codec; BLOCKED Linux transport (non-Linux host)\n";
#endif
}
