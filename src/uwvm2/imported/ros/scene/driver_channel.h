/** @file
 * @brief Consume a deployment-provisioned local driver channel without process control.
 * @details The owner supplies a dedicated connected AF_UNIX/SOCK_SEQPACKET
 * endpoint and expected credentials in this VM's observation namespace. This
 * class never opens a path, connects a socket, enters a namespace, changes a
 * cgroup, sends a signal, waits for a process or launches code. Each received
 * packet needs kernel-supplied credentials and a live pinned process; forwarded descriptors are closed
 * and rejected. All I/O is nonblocking per call, independent of file flags.
 * Closing the private duplicate does not shutdown the shared socket or driver.
 * The deployment must not retain other readers/writers of this endpoint.
 */
#pragma once
#include <cerrno>
#include "driver_protocol.h"
#include "../runtime/resources.h"
#if defined(__linux__)
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/nsfs.h>
#include <linux/sched.h>
#include <poll.h>
#include <charconv>
#include <unistd.h>
#endif
namespace uwvm2::imported::ros::scene::drivers
{
    using error=resources::error;
    struct credentials{std::uint64_t pid{},uid{},gid{};friend bool operator==(credentials const&,credentials const&)=default;};
    inline bool valid(credentials const& peer) noexcept
    {return peer.pid && peer.pid<=INT32_MAX && peer.uid<=UINT32_MAX && peer.gid<=UINT32_MAX;}
    class channel
    {
        int fd_{-1},peer_lifetime_{-1};credentials expected_;
        std::uint64_t socket_device_{},socket_inode_{};
#if defined(__linux__)
        ::pid_t owner_{::getpid()};
#endif
    public:
        channel()=default;channel(channel const&)=delete;channel&operator=(channel const&)=delete;
        ~channel(){close();}
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return owner_==::getpid();
#else
            return true;
#endif
        }
        /** @brief Bind once to an already connected endpoint with per-packet credentials enabled.
         * @details SO_PASSCRED must be provisioned before this call; binding
         * changes neither the source descriptor's flags nor socket options.
         * Unknown namespace-translated identity is denied, never guessed.
         * A privileged provisioner may supply a borrowed time-namespace FD
         * opened from the pinned peer before credential isolation. Its kernel
         * namespace type and identity must equal this process's time namespace.
         * This attests clock provenance only: PID lifetime, packet credentials
         * and TLS authentication remain mandatory. The provisioner must prevent
         * subsequent peer time-namespace changes. No descriptor is entered,
         * retained or exposed to Wasm. Without evidence, procfs checking remains
         * strict; EACCES is never treated as matching namespaces.
         */
        error bind(int descriptor,credentials expected,int peer_time_namespace_fd=-1)noexcept
        {
            if(!same_process())return error::denied;
            if(fd_>=0)return error::conflict;
#if defined(__linux__)
            if(descriptor<0||peer_time_namespace_fd<-1||!valid(expected))return error::invalid;
            int duplicate=::fcntl(descriptor,F_DUPFD_CLOEXEC,0);if(duplicate<0)return error::bad_handle;
            auto refuse=[&](error e){::close(duplicate);return e;};
            int type{},pass{};::socklen_t size=sizeof(type);
            if(::getsockopt(duplicate,SOL_SOCKET,SO_TYPE,&type,&size)||size!=sizeof(type)||type!=SOCK_SEQPACKET)return refuse(error::denied);
            size=sizeof(pass);if(::getsockopt(duplicate,SOL_SOCKET,SO_PASSCRED,&pass,&size)||size!=sizeof(pass)||pass!=1)return refuse(error::denied);
            ::sockaddr_storage address{};size=sizeof(address);
            if(::getsockname(duplicate,reinterpret_cast<sockaddr*>(&address),&size)||address.ss_family!=AF_UNIX)return refuse(error::denied);
            size=sizeof(address);if(::getpeername(duplicate,reinterpret_cast<sockaddr*>(&address),&size)||address.ss_family!=AF_UNIX)return refuse(error::denied);
            // Pin the process incarnation, not merely its recyclable numeric PID.
            int lifetime=static_cast<int>(::syscall(SYS_pidfd_open,static_cast<::pid_t>(expected.pid),0));
            if(lifetime<0)return refuse(error::denied);
            ::pollfd watch{lifetime,POLLIN,0};
            if(::poll(&watch,1,0)!=0){::close(lifetime);return refuse(error::denied);}
            // Absolute monotonic deadlines require the same time namespace.
            // Network/mount/PID namespace separation is permitted; identity is
            // expressed in this observer's namespace. No namespace is entered.
            std::array<char,96> path{};constexpr std::string_view prefix="/proc/";
            std::copy(prefix.begin(),prefix.end(),path.begin());
            auto converted=std::to_chars(path.data()+prefix.size(),path.data()+64,expected.pid);
            constexpr std::string_view suffix="/ns/time";std::copy(suffix.begin(),suffix.end(),converted.ptr);
            struct ::stat local{},remote{},socket{};
            bool remote_valid=peer_time_namespace_fd<0?::stat(path.data(),&remote)==0:
                ::ioctl(peer_time_namespace_fd,NS_GET_NSTYPE)==CLONE_NEWTIME&&::fstat(peer_time_namespace_fd,&remote)==0;
            if(::stat("/proc/self/ns/time",&local)||!remote_valid||local.st_dev!=remote.st_dev||local.st_ino!=remote.st_ino||::fstat(duplicate,&socket))
            {::close(lifetime);return refuse(error::denied);}
            socket_device_=static_cast<std::uint64_t>(socket.st_dev);socket_inode_=static_cast<std::uint64_t>(socket.st_ino);
            fd_=duplicate;peer_lifetime_=lifetime;expected_=expected;return error::success;
#else
            (void)descriptor;(void)expected;(void)peer_time_namespace_fd;return error::unsupported;
#endif
        }
        void close()noexcept
        {
#if defined(__linux__)
            if(fd_>=0){int old=std::exchange(fd_,-1);::close(old);}
            if(peer_lifetime_>=0){int old=std::exchange(peer_lifetime_,-1);::close(old);}
#endif
        }
        bool same_endpoint(channel const& other)const noexcept
        {return fd_>=0&&other.fd_>=0&&socket_device_==other.socket_device_&&socket_inode_==other.socket_inode_;}
        /** @brief Cached kernel identity for trusted cross-environment alias rejection. */
        std::pair<std::uint64_t,std::uint64_t> endpoint_identity()const noexcept
        {return fd_>=0&&same_process()?std::pair{socket_device_,socket_inode_}:std::pair<std::uint64_t,std::uint64_t>{};}
        explicit operator bool()const noexcept
        {
#if defined(__linux__)
            if(fd_<0||peer_lifetime_<0||!same_process())return false;
            ::pollfd watch{peer_lifetime_,POLLIN,0};return ::poll(&watch,1,0)==0;
#else
            return false;
#endif
        }
    private:
        error send_parts(wire::bytes header,wire::bytes body)noexcept
        {
            if(!*this)return error::denied;
#if defined(__linux__)
            ::iovec parts[2]{{const_cast<std::uint8_t*>(header.data()),header.size()},{const_cast<std::uint8_t*>(body.data()),body.size()}};
            ::msghdr message{};message.msg_iov=parts;message.msg_iovlen=body.empty()?1:2;
            auto n=::sendmsg(fd_,&message,MSG_DONTWAIT|MSG_NOSIGNAL);
            if(n<0)return errno==EAGAIN||errno==EWOULDBLOCK||errno==EINTR?error::again:error::io;
            return static_cast<std::size_t>(n)==header.size()+body.size()?error::success:error::io;
#else
            return error::unsupported;
#endif
        }
    public:
        error send(wire::bytes header,wire::bytes body)noexcept
        {
            if(header.size()!=protocol::header_bytes||body.size()>protocol::maximum_payload)return error::invalid;
            return send_parts(header,body);
        }
        /** @brief Send one bounded TLS flight on this exact credential-checked endpoint. */
        error send_tls_packet(wire::bytes packet)noexcept
        {
            if(packet.empty()||packet.size()>protocol::header_bytes+protocol::maximum_payload+2048)return error::invalid;
            return send_parts(packet,{});
        }
        /** @brief Consume at most one packet; refuse truncation, FD transfer or wrong origin. */
        error receive(std::span<std::uint8_t> output,std::size_t& count)noexcept
        {
            if(!*this)return error::denied;
#if defined(__linux__)
            alignas(::cmsghdr) std::array<unsigned char,CMSG_SPACE(sizeof(::ucred))+CMSG_SPACE(8*sizeof(int))> ancillary{};
            ::iovec vector{output.data(),output.size()};::msghdr message{};message.msg_iov=&vector;message.msg_iovlen=1;
            message.msg_control=ancillary.data();message.msg_controllen=ancillary.size();
            auto n=::recvmsg(fd_,&message,MSG_DONTWAIT|MSG_CMSG_CLOEXEC|MSG_TRUNC);
            if(n<0)return errno==EAGAIN||errno==EWOULDBLOCK||errno==EINTR?error::again:error::io;
            bool found{},bad{};
            for(auto* c= CMSG_FIRSTHDR(&message);c;c=CMSG_NXTHDR(&message,c))
            {
                if(c->cmsg_len<CMSG_LEN(0)){bad=true;break;}
                if(c->cmsg_level==SOL_SOCKET&&c->cmsg_type==SCM_RIGHTS)
                {
                    auto size=c->cmsg_len-CMSG_LEN(0);
                    for(std::size_t i{};i+sizeof(int)<=size;i+=sizeof(int))
                    {int fd{};std::memcpy(&fd,CMSG_DATA(c)+i,sizeof(fd));::close(fd);}
                    bad=true;
                }
                else if(c->cmsg_level==SOL_SOCKET&&c->cmsg_type==SCM_CREDENTIALS&&c->cmsg_len==CMSG_LEN(sizeof(::ucred)))
                {
                    ::ucred peer{};std::memcpy(&peer,CMSG_DATA(c),sizeof(peer));
                    if(found||peer.pid<=0||credentials{static_cast<std::uint64_t>(peer.pid),peer.uid,peer.gid}!=expected_)bad=true;
                    found=true;
                }
                else bad=true;
            }
            if(!n)return error::io;
            if(bad||!found||(message.msg_flags&(MSG_TRUNC|MSG_CTRUNC))||static_cast<std::size_t>(n)>output.size())return error::denied;
            count=static_cast<std::size_t>(n);return error::success;
#else
            (void)output;(void)count;return error::unsupported;
#endif
        }
    };
}
