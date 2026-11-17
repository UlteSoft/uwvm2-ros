/** @file @brief Test-only provisioner for authenticated directional ROS channels. */
#pragma once
#include <uwvm2/imported/ros/scene/communication_attachment.h>
#include <uwvm2/imported/ros/scene/communication_transport_broker.h>
#include <cassert>
#include <cstdio>
#include <dirent.h>
#include <signal.h>
namespace s=uwvm2::imported::ros::scene;
namespace a=s::communication_attachment;
using s::error;
inline std::size_t descriptors()
{
    auto* directory=::opendir("/proc/self/fd");assert(directory);std::size_t count{};
    while(::readdir(directory))++count;
    assert(!::closedir(directory));return count;
}
struct fixture
{
    int control[2];std::array<std::array<int,2>,7> pipes;
    a::registration selected;s::drivers::channel peer;
    fixture()
    {
        assert(!::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,control));
        int one=1;for(auto fd:control)assert(!::setsockopt(fd,SOL_SOCKET,SO_PASSCRED,&one,sizeof(one)));
        selected.control=control[0];selected.peer={static_cast<std::uint64_t>(::getpid()),::getuid(),::getgid()};
        selected.identity={{1,2},{3,4},{5,6},{7,8},{9,10},{11,12,13,14}};
        selected.startup_ns=1000000000;selected.session_until=static_cast<std::uint64_t>(uwvm2::imported::ros::resources::steady_now())+10000000000ull;
        selected.stderr_bytes=4096;
        for(std::size_t i{};i<7;++i)
        {assert(!::pipe2(pipes[i].data(),O_NONBLOCK|O_CLOEXEC));selected.pipes[i]=pipes[i][i==0||i==3||i==5?1:0];}
        assert(peer.bind(control[1],selected.peer)==error::success);
    }
    ~fixture(){for(auto& pair:pipes)for(auto fd:pair)::close(fd);for(auto fd:control)::close(fd);}
    a::configuration configuration()const
    {
        a::configuration out;s::buffers::profile profile;assert(profile.identity.assign("fixture"));profile.sha256.fill(42);
        assert(out.profile(profile,"external-ros")==error::success&&out.channel(selected)==error::success&&out.freeze()==error::success);return out;
    }
    void response(std::uint64_t kind,std::size_t corrupt=SIZE_MAX,bool rights=false)
    {
        std::array<std::uint8_t,1200> record{};std::size_t count{};
        assert(peer.receive(record,count)==error::success&&count>176);
        s::wire::writer writer{std::span{record}.subspan(16,8)};assert(writer.u64(kind));
        if(corrupt!=SIZE_MAX){assert(corrupt<count);record[corrupt]^=1;}
        if(!rights){assert(peer.send(std::span{record}.first(176),std::span{record}.subspan(176,count-176))==error::success);return;}
        ::iovec vector{record.data(),count};alignas(cmsghdr) std::array<unsigned char,CMSG_SPACE(sizeof(int))> storage{};
        ::msghdr message{};message.msg_iov=&vector;message.msg_iovlen=1;message.msg_control=storage.data();message.msg_controllen=storage.size();
        auto* extra=CMSG_FIRSTHDR(&message);extra->cmsg_level=SOL_SOCKET;extra->cmsg_type=SCM_RIGHTS;extra->cmsg_len=CMSG_LEN(sizeof(int));
        std::memcpy(CMSG_DATA(extra),&control[1],sizeof(int));assert(::sendmsg(control[1],&message,MSG_NOSIGNAL)==static_cast<ssize_t>(count));
    }
    void handshake(a::connection& connection)
    {
        assert(connection.begin()==error::again);response(a::ready);assert(connection.begin()==error::success);
        assert(connection.activate_channel()==error::again);response(a::attached);assert(connection.activate_channel()==error::success);
        assert(connection.attachment_confirmed());
    }
};
