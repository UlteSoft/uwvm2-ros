/** @file
 * @brief Bounded RTPS routing over externally provisioned directional pipes.
 * @details Deployment owns both peers, all pipe creation and sandbox policy.
 * The VM duplicates authenticated attachments and forwards only after fresh
 * bilateral context/node admission. Fixed RTPS profiles, bounded queues and
 * optional attenuation remain enforced. No process, socket path, cgroup or
 * namespace is created. Relay retirement never proves remote cleanup.
 */
#pragma once
#include "communication_attachment.h"
#include "communication_transport_broker.h"

namespace uwvm2::imported::ros::scene::communication_transport
{
#if defined(__linux__)
    struct private_link_snapshot
    {
        bool prepared{},active{},fenced{},uncertain{};
        std::array<std::uint64_t,4> forwarded{},attenuated{};
    };
    class private_link
    {
        struct route
        {
            packet_queue queued;
            ::uwvm2::utils::container::std_vector<std::uint8_t> input;
            std::size_t have{},need{header_bytes};
            std::uint64_t incoming{1},outgoing{1},partial_started{};
        };
        std::array<plan,2> approved;
        std::array<std::shared_ptr<communication_attachment::connection>,2> workers;
        std::array<descriptor,2> identities;
        std::array<std::array<descriptor,4>,2> host;
        std::array<route,4> routes;

        std::array<std::uint64_t,8> window{},packets{},bytes{};
        std::array<std::atomic<std::uint64_t>,4> forwarded{},attenuated{};
        std::atomic<bool> active{},failed{},uncertain{},stopping{};
        ::pid_t owner{::getpid()};pthread_t thread{};bool prepared{},started{};
        static std::uint64_t now() noexcept
        {return static_cast<std::uint64_t>(resources::steady_now());}
        bool same_process() const noexcept{return owner==::getpid();}
        bool live_identities() const noexcept
        {
            std::array<pollfd,2> live{{{identities[0].value,POLLIN,0},{identities[1].value,POLLIN,0}}};
            auto result=::poll(live.data(),live.size(),0);
            return result==0 || (result<0&&errno==EINTR);
        }
        void fail() noexcept
        {
            // Fence the shared session before publishing relay failure. Every
            // RPC send/receive consults this same flag; no supervisor cadence
            // may leave a failed DDS route with an independently usable RPC.
            for(auto const& worker:workers)if(worker)worker->request_stop();
            failed.store(true,std::memory_order_release);uncertain.store(true,std::memory_order_release);stopping.store(true,std::memory_order_release);
        }
        bool permit(std::size_t side,std::size_t direction,std::size_t amount) noexcept
        {
            auto const& limit=approved[side].queues[direction];if(!limit.attenuated)return true;
            auto i=side*4+direction;auto tick=now()/1000000000ull;
            if(window[i]!=tick){window[i]=tick;packets[i]=bytes[i]=0;}
            if(packets[i]>=limit.packets_per_second||amount>limit.bytes_per_second-bytes[i])return false;
            ++packets[i];bytes[i]+=amount;return true;
        }
        bool sessions_live()const noexcept
        {return workers[0]&&workers[1]&&workers[0]->session_live()&&workers[1]->session_live();}
        void receive(std::size_t i) noexcept
        {
            auto side=i/2,channel=i%2,tx=channel*2+1,rx=channel*2;auto& route=routes[i];
            for(unsigned batch{};batch<8;++batch)
            {
                if(!sessions_live()){fail();return;}
                auto got=::read(host[side][tx].value,route.input.data()+route.have,route.need-route.have);
                if(got<0){if(errno==EINTR)continue;if(errno==EAGAIN||errno==EWOULDBLOCK)return;fail();return;}
                if(!got){fail();return;}
                if(!route.have)route.partial_started=now();
                route.have+=static_cast<std::size_t>(got);
                if(route.have!=route.need)return;
                if(route.need==header_bytes)
                {
                    frame_header header;
                    if(!decode_header({route.input.data(),header_bytes},approved[side],route.incoming,header)||
                        header.length>approved[1-side].max_datagram_bytes){fail();return;}
                    route.need=header_bytes+static_cast<std::size_t>(header.length);continue;
                }
                wire::bytes payload{route.input.data()+header_bytes,route.need-header_bytes};
                if(route.incoming==UINT64_MAX||!packet_allowed(approved[side],static_cast<role>(tx),payload)||
                    !packet_allowed(approved[1-side],static_cast<role>(rx),payload)){fail();return;}
                ++route.incoming;
                if(!permit(side,tx,payload.size())||!permit(1-side,rx,payload.size()))
                {attenuated[i].fetch_add(1,std::memory_order_relaxed);uncertain.store(true,std::memory_order_release);}
                else
                {
                    if(route.outgoing==UINT64_MAX||!route.queued.push(approved[1-side].generation,route.outgoing,payload)){fail();return;}
                    ++route.outgoing;
                }
                erase_bytes({route.input.data(),route.have});
                route.have=0;route.need=header_bytes;
            }
        }
        void flush(std::size_t i) noexcept
        {
            auto& route=routes[i];auto destination=1-i/2,rx=(i%2)*2;
            for(unsigned batch{};batch<8&&!route.queued.empty();++batch)
            {
                if(!sessions_live()){fail();return;}
                auto payload=route.queued.remaining();auto count=::write(host[destination][rx].value,payload.data(),payload.size());
                if(count<0){if(errno==EINTR)continue;if(errno==EAGAIN||errno==EWOULDBLOCK)return;fail();return;}
                if(!count){fail();return;}
                auto length=static_cast<std::size_t>(count);bool complete=length==payload.size();route.queued.advance(length);
                if(complete)forwarded[i].fetch_add(1,std::memory_order_relaxed);
            }
        }
        static void* entry(void* pointer) noexcept{static_cast<private_link*>(pointer)->run();return nullptr;}
        void run() noexcept
        {
            sigset_t mask;::sigemptyset(&mask);::sigaddset(&mask,SIGPIPE);::pthread_sigmask(SIG_BLOCK,&mask,nullptr);
            while(!stopping.load(std::memory_order_acquire))
            {
                if(!sessions_live()||!live_identities()){fail();break;}
                for(std::size_t i{};i<routes.size();++i)
                    if(routes[i].have&&now()-routes[i].partial_started>=approved[i/2].frame_deadline_ns){fail();break;}
                if(stopping.load(std::memory_order_acquire))break;
                std::array<pollfd,10> wait{};
                for(std::size_t i{};i<routes.size();++i)
                {
                    auto side=i/2,channel=i%2;bool allowed=approved[side].allowed[channel*2+1]&&approved[1-side].allowed[channel*2];
                    wait[i*2]={allowed?host[side][channel*2+1].value:-1,POLLIN,0};
                    wait[i*2+1]={allowed?host[1-side][channel*2].value:-1,static_cast<short>(routes[i].queued.empty()?0:POLLOUT),0};
                }
                for(std::size_t i{};i<2;++i)wait[8+i]={identities[i].value,POLLIN,0};
                auto result=::poll(wait.data(),wait.size(),20);if(result<0){if(errno==EINTR)continue;fail();break;}
                if(wait[8].revents||wait[9].revents){fail();break;}
                for(std::size_t i{};i<routes.size()&&!stopping.load(std::memory_order_acquire);++i)
                {
                    if((wait[i*2].revents|wait[i*2+1].revents)&(POLLERR|POLLNVAL|POLLHUP)){fail();break;}
                    if(wait[i*2].revents&POLLIN)receive(i);
                    if(!stopping.load(std::memory_order_acquire))flush(i);
                }
            }
            for(auto& route:routes){erase_bytes(route.input);route.queued.erase();}
            active.store(false,std::memory_order_release);
        }
    public:
        private_link()=default;private_link(private_link const&)=delete;private_link& operator=(private_link const&)=delete;
        ~private_link(){retire();}
        /** @brief Verify reciprocal fixed routes and both authenticated identities before allocation.
         * @details Arguments are trusted deployment objects. Configurations are
         * exact hashes of approved worker profiles, not guest-supplied assertions.
         * Preparation and activation are serialized by the owning supervisor.
         */
        bool prepare(std::array<plan,2> const& policies,
                     std::array<std::shared_ptr<communication_attachment::connection>,2> const& processes,
                     std::array<buffers::profile,2> const& configurations) noexcept
        {
            if(!same_process()||prepared||failed.load()||stopping.load()||!policies[0].valid()||!policies[1].valid()||!processes[0]||!processes[1]||processes[0]==processes[1])return false;
            auto const& a=policies[0];auto const& b=policies[1];
            if(a.domain!=b.domain||a.local_participant!=b.peer_participant||a.peer_participant!=b.local_participant||
                a.local_prefix!=b.peer_prefix||a.peer_prefix!=b.local_prefix||a.local_discovery!=b.peer_discovery||
                a.local_user!=b.peer_user||a.peer_discovery!=b.local_discovery||a.peer_user!=b.local_user)return false;
            for(std::size_t i{};i<2;++i)
            {
                if(!processes[i]->privately_ready()||processes[i]->attachment_confirmed()||
                    processes[i]->configuration_profile()!=configurations[i]||!processes[i]->identity_confirmed())return false;
                for(std::size_t direction{};direction<4;++direction)
                    if(policies[i].allowed[direction]!=policies[1-i].allowed[direction^1])return false;
                identities[i]=descriptor{::fcntl(processes[i]->process_fd(),F_DUPFD_CLOEXEC,4)};
                if(identities[i].value<0){failed.store(true);return false;}
            }
            if(!live_identities()){failed.store(true);return false;}
            try
            {
                approved=policies;workers=processes;
                for(std::size_t i{};i<routes.size();++i)
                {
                    auto side=i/2,channel=i%2,tx=channel*2+1,rx=channel*2;
                    auto count=std::min(approved[side].queues[tx].packets,approved[1-side].queues[rx].packets);
                    auto maximum=std::min(approved[side].max_datagram_bytes,approved[1-side].max_datagram_bytes);
                    routes[i].input.resize(header_bytes+maximum);
                    if(approved[side].allowed[tx]&&!routes[i].queued.initialize(count,maximum))throw 0;
                }
                for(std::size_t side{};side<2;++side)for(std::size_t direction{};direction<4;++direction)
                {
                    int duplicate=::fcntl(processes[side]->transport_fd(direction),F_DUPFD_CLOEXEC,3);
                    if(duplicate<0)throw 0;
                    host[side][direction]=descriptor{duplicate};
                }
                prepared=true;return true;
            }
            catch(...)
            {for(auto& side:host)for(auto& fd:side)fd=descriptor{};workers={};failed.store(true);return false;}
        }
        /** @brief Start only after both exact workers confirm attachment and fresh authorization. */
        bool activate() noexcept
        {
            if(!same_process()||!prepared||started||failed.load()||stopping.load()||
                !workers[0]->attachment_confirmed()||!workers[1]->attachment_confirmed()||!live_identities())return false;
            active.store(true);if(::pthread_create(&thread,nullptr,entry,this)){active.store(false);failed.store(true);return false;}started=true;return true;
        }
        /** @brief Signal fencing without waiting on a relay or acquiring a native lock. */
        void request_stop() noexcept
        {if(same_process()){stopping.store(true,std::memory_order_release);uncertain.store(true,std::memory_order_release);}}
        /** @brief Fence and join the relay outside every authority/provider lock.
         * @details The owning supervisor serializes this with activate(). No
         * remote cleanup claim follows from a relay join. Deployment owns
         * peer lifecycle and any durable unknown-effect reconciliation.
         */
        void retire() noexcept
        {if(!same_process())return;request_stop();if(started){::pthread_join(thread,nullptr);started=false;}for(auto& route:routes){erase_bytes(route.input);route.queued.erase();}}
        private_link_snapshot inspect() const noexcept
        {
            if(!same_process())return {false,false,true,true,{},{}};
            private_link_snapshot result{prepared,active.load(),failed.load()||stopping.load(),uncertain.load()};
            for(std::size_t i{};i<4;++i){result.forwarded[i]=forwarded[i].load();result.attenuated[i]=attenuated[i].load();}return result;
        }
    };
#endif
}
