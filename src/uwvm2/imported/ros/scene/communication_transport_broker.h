/** @file
 * @brief Host-owned fixed-peer UDP relay; the worker never receives a socket.
 * @details open() is an effect and must follow a held context/deployment grant.
 * It validates the whole zero-default plan before allocating or opening FDs.
 * activate() starts forwarding only after the sandbox attached handshake. Four
 * independent bounded queues preserve discovery reservations. Worker frames
 * with invalid length/generation/sequence/direction fence the relay; malformed
 * external UDP packets are discarded before the native parser. Kernel connected
 * receive sockets reject every unapproved source address and source port.
 * Overflow/death/revocation is observable as fenced/uncertain, never reported as
 * successful delivery. This is scope isolation, not local-peer authentication.
 */
#pragma once
#include "communication_transport_protocol.h"
#include "erased_buffer.h"
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/unique.h>
#include <array>
#include <atomic>
#include <chrono>
#include <mutex>
#if defined(__linux__)
#include <arpa/inet.h>
#include <cerrno>
#include <fcntl.h>
#include <sys/poll.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace uwvm2::imported::ros::scene::communication_transport
{
#if defined(__linux__)
    struct descriptor
    {
        int value{-1};descriptor()=default;explicit descriptor(int fd):value(fd){}
        ~descriptor(){if(value>=0)::close(value);}
        descriptor(descriptor const&)=delete;descriptor& operator=(descriptor const&)=delete;
        descriptor(descriptor&& from) noexcept:value(std::exchange(from.value,-1)){}
        descriptor& operator=(descriptor&& from) noexcept
        {if(this!=&from){if(value>=0)::close(value);value=std::exchange(from.value,-1);}return *this;}
        int release() noexcept{return std::exchange(value,-1);}
    };
    /** @brief Fixed preallocated payload slots; no post-start backing allocation. */
    class packet_queue
    {
        ::uwvm2::utils::container::std_vector<std::uint8_t> bytes;
        ::uwvm2::utils::container::std_vector<std::size_t> lengths;
        std::size_t stride{},head{},count{},partial{};
    public:
        ~packet_queue(){erase();}
        void erase()noexcept{erase_bytes(bytes);head=count=partial=0;}
        bool initialize(std::size_t packets,std::size_t maximum) noexcept
        {
            if(!packets||maximum>SIZE_MAX-header_bytes||packets>SIZE_MAX/(maximum+header_bytes))return false;
            try{stride=maximum+header_bytes;bytes.resize(packets*stride);lengths.resize(packets);return true;}catch(...){return false;}
        }
        bool empty() const noexcept{return count==0;}
        bool full() const noexcept{return count==lengths.size();}
        bool push(std::uint64_t generation,std::uint64_t sequence,wire::bytes payload) noexcept
        {
            if(full()||payload.size()+header_bytes>stride)return false;
            auto slot=(head+count)%lengths.size();auto* p=bytes.data()+slot*stride;
            if(!encode_header({p,header_bytes},{generation,sequence,payload.size()}))return false;
            std::memcpy(p+header_bytes,payload.data(),payload.size());lengths[slot]=payload.size()+header_bytes;++count;return true;
        }
        wire::bytes remaining() const noexcept
        {return empty()?wire::bytes{}:wire::bytes{bytes.data()+head*stride+partial,lengths[head]-partial};}
        wire::bytes payload() const noexcept
        {return empty()?wire::bytes{}:wire::bytes{bytes.data()+head*stride+header_bytes,lengths[head]-header_bytes};}
        void advance(std::size_t amount) noexcept
        {if(!count||amount>lengths[head]-partial)return;erase_bytes({bytes.data()+head*stride+partial,amount});partial+=amount;if(partial==lengths[head]){partial=0;head=(head+1)%lengths.size();--count;}}
        void pop() noexcept{if(count){erase_bytes({bytes.data()+head*stride,lengths[head]});partial=0;head=(head+1)%lengths.size();--count;}}
    };
    struct relay_snapshot
    {
        bool active{},fenced{},uncertain{};
        std::array<std::uint64_t,4> forwarded{},rejected{},attenuated{};
    };
    class broker
    {
        plan approved{};::pid_t process{::getpid()};
        std::array<descriptor,4> sockets,pipes,worker_pipes;
        std::array<packet_queue,4> queued;
        std::array<::uwvm2::utils::container::std_vector<std::uint8_t>,2> input;
        std::array<std::size_t,2> have{},need{header_bytes,header_bytes};
        std::array<std::chrono::steady_clock::time_point,2> partial_started{};
        std::array<std::uint64_t,4> sequence{1,1,1,1},window{},packets{},bytes{};
        std::array<std::atomic<std::uint64_t>,4> forwarded{},rejected{},attenuated{};
        std::atomic<bool> active{},failed{},uncertain{},stopping{};
        pthread_t thread{};bool started{},opened{},pipes_released{};
        static sockaddr_in address(std::uint16_t port) noexcept
        {sockaddr_in a{};a.sin_family=AF_INET;a.sin_port=htons(port);a.sin_addr.s_addr=htonl(INADDR_LOOPBACK);return a;}
        bool create_socket(std::size_t i,std::uint16_t local,std::uint16_t peer) noexcept
        {
            descriptor value{::socket(AF_INET,SOCK_DGRAM|SOCK_CLOEXEC|SOCK_NONBLOCK,IPPROTO_UDP)};
            if(value.value<0)return false;
            int bound=static_cast<int>(approved.socket_buffer_bytes),effective{};socklen_t size=sizeof(effective);
            int option=receiving(static_cast<role>(i))?SO_RCVBUF:SO_SNDBUF;
            if(::setsockopt(value.value,SOL_SOCKET,option,&bound,sizeof(bound))||
               ::getsockopt(value.value,SOL_SOCKET,option,&effective,&size)||effective<bound||effective>2*bound)return false;
            auto local_address=address(local),peer_address=address(peer);
            if(::bind(value.value,reinterpret_cast<sockaddr*>(&local_address),sizeof(local_address))||
               ::connect(value.value,reinterpret_cast<sockaddr*>(&peer_address),sizeof(peer_address)))return false;
            sockets[i]=std::move(value);return true;
        }
        bool permit(std::size_t i,std::size_t amount) noexcept
        {
            auto const& quota=approved.queues[i];if(!quota.attenuated)return true;
            auto now=static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
            if(window[i]!=now){window[i]=now;packets[i]=bytes[i]=0;}
            if(packets[i]>=quota.packets_per_second||amount>quota.bytes_per_second-bytes[i])
            {attenuated[i].fetch_add(1,std::memory_order_relaxed);uncertain.store(true,std::memory_order_release);return false;}
            ++packets[i];bytes[i]+=amount;return true;
        }
        void fence() noexcept{failed.store(true,std::memory_order_release);uncertain.store(true,std::memory_order_release);stopping.store(true,std::memory_order_release);}
        bool next(std::size_t i) noexcept
        {if(sequence[i]==UINT64_MAX){fence();return false;}++sequence[i];return true;}
        void from_udp(std::size_t i) noexcept
        {
            std::array<std::uint8_t,65000> packet;
            for(unsigned batch{};batch<8;++batch)
            {
                auto size=::recv(sockets[i].value,packet.data(),approved.max_datagram_bytes,MSG_DONTWAIT|MSG_TRUNC);
                if(size<0){if(errno==EINTR)continue;if(errno==EAGAIN||errno==EWOULDBLOCK||errno==ECONNREFUSED)return;fence();return;}
                if(static_cast<std::uint64_t>(size)>approved.max_datagram_bytes||
                   !packet_allowed(approved,static_cast<role>(i),{packet.data(),static_cast<std::size_t>(size)}))
                {rejected[i].fetch_add(1,std::memory_order_relaxed);continue;}
                if(!permit(i,static_cast<std::size_t>(size)))continue;
                if(!queued[i].push(approved.generation,sequence[i],{packet.data(),static_cast<std::size_t>(size)})){fence();return;}
                if(!next(i))return;
            }
        }
        void from_pipe(std::size_t i) noexcept
        {
            auto slot=i/2;auto& buffer=input[slot];
            for(unsigned batch{};batch<8;++batch)
            {
                auto size=::read(pipes[i].value,buffer.data()+have[slot],need[slot]-have[slot]);
                if(size<0){if(errno==EINTR)continue;if(errno==EAGAIN||errno==EWOULDBLOCK)return;fence();return;}
                if(!size){fence();return;}
                if(!have[slot])partial_started[slot]=std::chrono::steady_clock::now();
                have[slot]+=static_cast<std::size_t>(size);
                if(have[slot]!=need[slot])return;
                if(need[slot]==header_bytes)
                {
                    frame_header header;
                    if(!decode_header({buffer.data(),header_bytes},approved,sequence[i],header)){fence();return;}
                    need[slot]=header_bytes+static_cast<std::size_t>(header.length);continue;
                }
                wire::bytes packet{buffer.data()+header_bytes,need[slot]-header_bytes};
                if(!packet_allowed(approved,static_cast<role>(i),packet)){rejected[i].fetch_add(1,std::memory_order_relaxed);fence();return;}
                if(permit(i,packet.size())&&!queued[i].push(approved.generation,sequence[i],packet)){fence();return;}
                have[slot]=0;need[slot]=header_bytes;if(!next(i))return;
            }
        }
        void flush(std::size_t i) noexcept
        {
            for(unsigned batch{};batch<8&&!queued[i].empty();++batch)
            {
                bool rx=receiving(static_cast<role>(i));auto data=rx?queued[i].remaining():queued[i].payload();
                auto size=rx?::write(pipes[i].value,data.data(),data.size()):
                    ::send(sockets[i].value,data.data(),data.size(),MSG_DONTWAIT|MSG_NOSIGNAL);
                if(size<0){if(errno==EINTR)continue;if(errno==EAGAIN||errno==EWOULDBLOCK)return;fence();return;}
                if(!size||(!rx&&static_cast<std::size_t>(size)!=data.size())){fence();return;}
                if(rx){bool complete=static_cast<std::size_t>(size)==data.size();queued[i].advance(static_cast<std::size_t>(size));if(complete)forwarded[i].fetch_add(1,std::memory_order_relaxed);}
                else{queued[i].pop();forwarded[i].fetch_add(1,std::memory_order_relaxed);}
            }
        }
        static void* entry(void* object) noexcept{static_cast<broker*>(object)->run();return nullptr;}
        void run() noexcept
        {
            sigset_t mask;::sigemptyset(&mask);::sigaddset(&mask,SIGPIPE);::pthread_sigmask(SIG_BLOCK,&mask,nullptr);
            while(!stopping.load(std::memory_order_acquire))
            {
                for(std::size_t i{};i<have.size();++i)
                    if(have[i]&&std::chrono::steady_clock::now()-partial_started[i]>=std::chrono::nanoseconds{approved.frame_deadline_ns}){fence();break;}
                if(stopping.load(std::memory_order_acquire))break;
                std::array<pollfd,8> wait;
                for(std::size_t i{};i<4;++i)
                {
                    bool rx=receiving(static_cast<role>(i));
                    wait[i*2]={approved.allowed[i]?sockets[i].value:-1,static_cast<short>(rx?POLLIN:(!queued[i].empty()?POLLOUT:0)),0};
                    wait[i*2+1]={approved.allowed[i]?pipes[i].value:-1,static_cast<short>(rx?(!queued[i].empty()?POLLOUT:0):POLLIN),0};
                }
                auto ready=::poll(wait.data(),wait.size(),20);if(ready<0){if(errno==EINTR)continue;fence();break;}
                for(std::size_t i{};i<4&&!stopping.load(std::memory_order_acquire);++i)
                {
                    if(!approved.allowed[i])continue;
                    if(wait[i*2+1].revents&(POLLERR|POLLNVAL|POLLHUP)){fence();break;}
                    if(wait[i*2].revents&POLLNVAL){fence();break;}
                    bool rx=receiving(static_cast<role>(i));
                    if(rx&&(wait[i*2].revents&POLLIN))from_udp(i);
                    if(!rx&&(wait[i*2+1].revents&POLLIN))from_pipe(i);
                    if(!stopping.load(std::memory_order_acquire))flush(i);
                }
            }
            active.store(false,std::memory_order_release);
        }
    public:
        broker()=default;broker(broker const&)=delete;broker& operator=(broker const&)=delete;
        ~broker(){revoke();}
        /** @brief Validate, reserve and open the exact route set after authorization.
         * @details Failed initialization is terminal for this object and releases
         * every descriptor. This method never sends or receives a datagram.
         */
        bool open(plan const& value) noexcept
        {
            if(process!=::getpid()||opened||failed.load()||!value.valid())return false;
            approved=value;
            try
            {
                for(std::size_t i{};i<4;++i)
                {
                    if(approved.allowed[i]&&!queued[i].initialize(static_cast<std::size_t>(approved.queues[i].packets),approved.max_datagram_bytes))throw 0;
                    int fds[2];if(::pipe2(fds,O_CLOEXEC|O_NONBLOCK))throw 0;
                    auto rx=receiving(static_cast<role>(i));pipes[i]=descriptor{fds[rx?1:0]};worker_pipes[i]=descriptor{fds[rx?0:1]};
                    // Two pages per pipe is a separate fixed kernel reservation.
                    if(::fcntl(pipes[i].value,F_SETPIPE_SZ,8192)<0)throw 0;
                }
                for(auto& b:input)b.resize(approved.max_datagram_bytes+header_bytes);
                if(!create_socket(0,approved.local_discovery,approved.peer_source)||
                   !create_socket(1,approved.local_discovery_source,approved.peer_discovery)||
                   (approved.allowed[2]&&!create_socket(2,approved.local_user,approved.peer_source))||
                   (approved.allowed[3]&&!create_socket(3,approved.local_user_source,approved.peer_user)))throw 0;
                opened=true;return true;
            }
            catch(...)
            {for(auto& fd:sockets)fd=descriptor{};for(auto& fd:pipes)fd=descriptor{};for(auto& fd:worker_pipes)fd=descriptor{};failed.store(true);return false;}
        }
        /** @brief Transfer only the four anonymous worker pipe ends once. */
        bool take_worker_pipes(std::array<communication_worker::role_fd,4>& output) noexcept
        {
            if(process!=::getpid()||!opened||pipes_released||started||failed.load())return false;
            for(std::size_t i{};i<4;++i)output[i]={static_cast<role>(i),worker_pipes[i].release()};
            pipes_released=true;return true;
        }
        /** @brief Forward only after sandbox attachment; no implicit authorization. */
        bool activate() noexcept
        {
            if(process!=::getpid()||!opened||!pipes_released||started||failed.load())return false;
            active.store(true);auto error=::pthread_create(&thread,nullptr,entry,this);
            if(error){active.store(false);failed.store(true);return false;}started=true;return true;
        }
        /** @brief Stop and join before releasing socket/queue storage. */
        void revoke() noexcept
        {stopping.store(true);uncertain.store(started||uncertain.load());if(started&&process==::getpid()){::pthread_join(thread,nullptr);started=false;}active.store(false);}
        relay_snapshot inspect() const noexcept
        {
            relay_snapshot result{active.load(),failed.load(),uncertain.load()};
            for(std::size_t i{};i<4;++i){result.forwarded[i]=forwarded[i].load();result.rejected[i]=rejected[i].load();result.attenuated[i]=attenuated[i].load();}return result;
        }
    };
#endif
}
