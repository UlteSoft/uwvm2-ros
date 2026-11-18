/** @file
 * @brief Receive one authenticated descriptor handoff in an external adapter.
 * @details This host-only helper is not included in the VM import surface. The
 * supervisor passes a private connected courier and its exact expected identity.
 * The adapter sends READY, accepts exactly four CLOEXEC descriptors from that
 * live process, then validates the immutable bootstrap before publishing any
 * output. All failures close every received descriptor. No pathname, listener,
 * process creation, signal, device grant or fallback is available. The courier
 * contains no key bytes: it transfers sealed objects through the kernel. The
 * subsequent application channel still requires bound mutual TLS and independent
 * operation admission. Success does not authorize a hardware effect.
 */
#pragma once
#include "provider_bootstrap.h"
#include <cerrno>
#include <cstring>
#include <string_view>
#include <utility>
#if defined(__linux__)
#include <sys/socket.h>
#include <sys/syscall.h>
#include <poll.h>
#endif
namespace uwvm2::imported::ros::scene::provider_bootstrap
{
    enum class receive_status {success,invalid,unavailable,denied,expired};
    /** @brief Exclusive ownership of delivered objects, never process authority.
     * @details Accessors borrow the descriptors. Close the originals with reset
     * once the TLS channel has consumed/duplicated them. Closing a sealed identity
     * is not a physical-erasure guarantee. Copy and move are deliberately absent.
     */
    class descriptors
    {
        std::array<int,4> values_{-1,-1,-1,-1};
        friend receive_status receive(int,credentials,std::uint64_t,descriptors&,record&) noexcept;
    public:
        descriptors()=default;
        descriptors(descriptors const&)=delete;
        descriptors&operator=(descriptors const&)=delete;
        ~descriptors(){reset();}
        void reset() noexcept
        {
            for(auto& value:values_)
            {
#if defined(__linux__)
                if(value>=0)::close(value);
#endif
                value=-1;
            }
        }
        bool empty()const noexcept{return std::all_of(values_.begin(),values_.end(),[](int value){return value<0;});}
        int carrier()const noexcept{return values_[0];}
        int identity()const noexcept{return values_[1];}
        int clock()const noexcept{return values_[2];}
        int manifest()const noexcept{return values_[3];}
    };
    /** @brief Receive transactionally with a local monotonic timeout of at most five seconds.
     * @details The expected provisioner identity comes from trusted launch state,
     * never from a courier message. The caller owns the courier and closes it on
     * completion. The function duplicates it without changing flags or options.
     * It pins the provisioner's process incarnation with a pidfd. Unknown control
     * data, a wrong sender, truncation, extra/missing objects or invalid bootstrap
     * fail closed; no delivered object is published until all checks pass. The
     * output descriptor bundle must be empty. Existing record output is preserved
     * on failure. There are no heap allocations or unbounded retry loops.
     */
    inline receive_status receive(int courier,credentials provisioner,std::uint64_t timeout_ns,
                                  descriptors& output,record& configuration) noexcept
    {
#if defined(__linux__)
        if(courier<0||!output.empty()||!timeout_ns||timeout_ns>UINT64_C(5000000000)||
           !provisioner.pid||provisioner.pid>INT32_MAX||provisioner.pid==static_cast<std::uint64_t>(::getpid())||
           provisioner.uid>=UINT32_MAX||provisioner.gid>=UINT32_MAX)return receive_status::invalid;
        auto monotonic=[]() noexcept -> std::uint64_t
        {
            ::timespec t{};
            if(::clock_gettime(CLOCK_MONOTONIC,&t)||t.tv_sec<0||t.tv_nsec<0||t.tv_nsec>=1000000000||
               static_cast<std::uint64_t>(t.tv_sec)>INT64_MAX/1000000000)return 0;
            return static_cast<std::uint64_t>(t.tv_sec)*1000000000+static_cast<std::uint64_t>(t.tv_nsec);
        };
        auto start=monotonic();if(!start||start>INT64_MAX-timeout_ns)return receive_status::unavailable;
        auto until=start+timeout_ns;
        struct retained
        {
            int fd;
            ~retained(){if(fd>=0)::close(fd);}
        } endpoint{::fcntl(courier,F_DUPFD_CLOEXEC,3)};
        if(endpoint.fd<0)return receive_status::unavailable;
        int type{},pass{};::socklen_t length=sizeof(type);
        if(::getsockopt(endpoint.fd,SOL_SOCKET,SO_TYPE,&type,&length)||length!=sizeof(type)||type!=SOCK_SEQPACKET)return receive_status::invalid;
        length=sizeof(pass);
        if(::getsockopt(endpoint.fd,SOL_SOCKET,SO_PASSCRED,&pass,&length)||length!=sizeof(pass)||pass!=1)return receive_status::invalid;
        ::sockaddr_storage address{};length=sizeof(address);
        if(::getsockname(endpoint.fd,reinterpret_cast<sockaddr*>(&address),&length)||address.ss_family!=AF_UNIX)return receive_status::invalid;
        length=sizeof(address);
        if(::getpeername(endpoint.fd,reinterpret_cast<sockaddr*>(&address),&length)||address.ss_family!=AF_UNIX)return receive_status::invalid;
        retained lifetime{static_cast<int>(::syscall(SYS_pidfd_open,static_cast<::pid_t>(provisioner.pid),0))};
        if(lifetime.fd<0)return receive_status::denied;
        auto live=[&]() noexcept{::pollfd p{lifetime.fd,POLLIN,0};return ::poll(&p,1,0)==0;};
        constexpr std::string_view ready="WROS-READY",delivered="WROS-FDS";
        bool sent{};
        for(;;)
        {
            auto now=monotonic();if(!now)return receive_status::unavailable;
            if(now>=until)return receive_status::expired;
            if(!live())return receive_status::denied;
            if(!sent)
            {
                auto n=::send(endpoint.fd,ready.data(),ready.size(),MSG_DONTWAIT|MSG_NOSIGNAL);
                if(n==static_cast<ssize_t>(ready.size()))sent=true;
                else if(n>=0||(errno!=EAGAIN&&errno!=EINTR))return receive_status::denied;
            }
            if(sent)
            {
                descriptors candidate;
                std::array<char,16> bytes{};
                // Linux admits at most 253 SCM_RIGHTS descriptors per message.
                // Receive the whole bounded set before rejecting extras, rather
                // than depending on translated ancillary truncation to close
                // handles the adapter cannot see (notably in user-mode emulators).
                alignas(::cmsghdr) std::array<unsigned char,CMSG_SPACE(253*sizeof(int))+CMSG_SPACE(sizeof(::ucred))> control{};
                ::iovec vector{bytes.data(),bytes.size()};::msghdr message{};
                message.msg_iov=&vector;message.msg_iovlen=1;
                message.msg_control=control.data();message.msg_controllen=control.size();
                auto n=::recvmsg(endpoint.fd,&message,MSG_DONTWAIT|MSG_CMSG_CLOEXEC|MSG_TRUNC);
                if(n>=0)
                {
                    bool bad=message.msg_flags&(MSG_TRUNC|MSG_CTRUNC);unsigned identities{},rights{},count{};
                    for(auto* c=CMSG_FIRSTHDR(&message);c;c=CMSG_NXTHDR(&message,c))
                    {
                        if(c->cmsg_len<CMSG_LEN(0)){bad=true;break;}
                        if(c->cmsg_level==SOL_SOCKET&&c->cmsg_type==SCM_RIGHTS)
                        {
                            ++rights;auto size=c->cmsg_len-CMSG_LEN(0);if(size%sizeof(int))bad=true;
                            for(std::size_t at{};at+sizeof(int)<=size;at+=sizeof(int))
                            {
                                int fd;std::memcpy(&fd,CMSG_DATA(c)+at,sizeof(fd));
                                if(count<4)candidate.values_[count++]=fd;
                                else{::close(fd);bad=true;}
                            }
                        }
                        else if(c->cmsg_level==SOL_SOCKET&&c->cmsg_type==SCM_CREDENTIALS&&c->cmsg_len==CMSG_LEN(sizeof(::ucred)))
                        {
                            ::ucred identity{};std::memcpy(&identity,CMSG_DATA(c),sizeof(identity));++identities;
                            bad|=credentials{static_cast<std::uint64_t>(identity.pid),identity.uid,identity.gid}!=provisioner;
                        }
                        else bad=true;
                    }
                    record value;
                    if(bad||identities!=1||rights!=1||count!=4||n!=static_cast<ssize_t>(delivered.size())||
                       !std::equal(delivered.begin(),delivered.end(),bytes.begin())||!live()||
                       !load(candidate.manifest(),candidate.carrier(),candidate.identity(),candidate.clock(),value))return receive_status::denied;
                    now=monotonic();if(!now)return receive_status::unavailable;
                    if(now>=until)return receive_status::expired;
                    output.values_.swap(candidate.values_);configuration=value;return receive_status::success;
                }
                if(errno!=EAGAIN&&errno!=EINTR)return receive_status::denied;
            }
            ::pollfd waiters[2]{{lifetime.fd,POLLIN,0},{endpoint.fd,static_cast<short>(sent?POLLIN:POLLOUT),0}};
            auto result=::poll(waiters,2,static_cast<int>(std::min<std::uint64_t>((until-now+999999)/1000000,5)));
            if(result<0&&errno!=EINTR)return receive_status::unavailable;
        }
#else
        (void)courier;(void)provisioner;(void)timeout_ns;(void)output;(void)configuration;return receive_status::unavailable;
#endif
    }
}
