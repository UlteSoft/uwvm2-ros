/** @file @brief Real connected UDP scope, pipe framing and separate rate reservations. */
#include <uwvm2/imported/ros/scene/communication_transport_broker.h>
#include <cassert>
#include <cstdio>
namespace s=uwvm2::imported::ros::scene;
namespace t=s::communication_transport;
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
#if defined(UWVM_TRANSPORT_ALLOCATION)
static std::atomic<std::size_t> fail_after{};static std::atomic<bool> injected{};
static bool reject() noexcept
{auto n=fail_after.load();while(n){if(fail_after.compare_exchange_weak(n,n-1)){if(n==1){injected=true;return true;}break;}}return false;}
extern "C" void* __real_malloc(std::size_t);extern "C" void* __real_calloc(std::size_t,std::size_t);extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return reject()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t b){return reject()?nullptr:__real_calloc(n,b);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return reject()?nullptr:__real_realloc(p,n);}
#endif
static t::plan policy()
{
    t::plan p;p.generation=734;p.domain=193;p.local_participant=0;p.peer_participant=1;
    p.local_discovery=55660;p.local_user=55661;p.peer_discovery=55662;p.peer_user=55663;
    p.local_discovery_source=60110;p.local_user_source=60111;p.peer_source=60112;
    p.local_prefix[0]=1;p.local_prefix[11]=7;p.peer_prefix[0]=2;p.peer_prefix[11]=9;
    p.allowed.fill(true);p.user_data_receive=p.user_data_transmit=true;p.frame_deadline_ns=5000000000ull;
    p.max_datagram_bytes=1024;p.max_sample_bytes=4096;p.max_submessages=32;p.socket_buffer_bytes=16384;
    for(auto& q:p.queues){q.packets=8;q.bytes=8192;}assert(p.valid());return p;
}
static bytes packet(t::plan const& p,t::role role)
{
    bytes b(48);b[0]='R';b[1]='T';b[2]='P';b[3]='S';b[4]=2;b[5]=3;b[6]=1;b[7]=15;
    auto const& prefix=t::receiving(role)?p.peer_prefix:p.local_prefix;std::copy(prefix.begin(),prefix.end(),b.begin()+8);
    b[20]=0x15;b[21]=5;b[22]=24;b[26]=16;b[32]=1;b[35]=t::discovery(role)?0xc2:3;b[40]=1;b[44]=0;
    assert(t::packet_allowed(p,role,b));return b;
}
static sockaddr_in address(std::uint16_t port,std::uint32_t ip=INADDR_LOOPBACK)
{sockaddr_in a{};a.sin_family=AF_INET;a.sin_port=htons(port);a.sin_addr.s_addr=htonl(ip);return a;}
static t::descriptor udp(std::uint16_t port,std::uint32_t ip=INADDR_LOOPBACK)
{t::descriptor fd{::socket(AF_INET,SOCK_DGRAM|SOCK_NONBLOCK|SOCK_CLOEXEC,0)};assert(fd.value>=0);auto a=address(port,ip);assert(!::bind(fd.value,reinterpret_cast<sockaddr*>(&a),sizeof(a)));return fd;}
static void send(int fd,std::uint16_t port,s::wire::bytes b)
{auto a=address(port);assert(::sendto(fd,b.data(),b.size(),0,reinterpret_cast<sockaddr*>(&a),sizeof(a))==static_cast<ssize_t>(b.size()));}
static bool ready(int fd,short direction,int milliseconds)
{pollfd p{fd,direction,0};auto n=::poll(&p,1,milliseconds);assert(n>=0);return n&&(p.revents&direction);}
static bytes read_frame(int fd,t::plan const& p,std::uint64_t sequence)
{
    bytes b(t::header_bytes);std::size_t have{};
    while(have<b.size()){assert(ready(fd,POLLIN,1000));auto n=::read(fd,b.data()+have,b.size()-have);assert(n>0);have+=static_cast<std::size_t>(n);}
    t::frame_header header;assert(t::decode_header(b,p,sequence,header));b.resize(header.length);have=0;
    while(have<b.size()){assert(ready(fd,POLLIN,1000));auto n=::read(fd,b.data()+have,b.size()-have);assert(n>0);have+=static_cast<std::size_t>(n);}return b;
}
static void write_frame(int fd,t::plan const& p,std::uint64_t sequence,s::wire::bytes payload,bool partial=false)
{
    bytes frame(t::header_bytes+payload.size());assert(t::encode_header({frame.data(),t::header_bytes},{p.generation,sequence,payload.size()}));
    std::copy(payload.begin(),payload.end(),frame.begin()+t::header_bytes);
    for(std::size_t at{};at<frame.size();){assert(ready(fd,POLLOUT,1000));auto amount=partial?std::size_t{1}:frame.size()-at;auto n=::write(fd,frame.data()+at,amount);assert(n>0);at+=static_cast<std::size_t>(n);}
}
struct fixture
{
    t::plan p; t::descriptor peer,discovery,user;
    t::broker relay;std::array<t::descriptor,4> pipes;
    explicit fixture(t::plan value):p(value),peer(udp(p.peer_source)),discovery(udp(p.peer_discovery)),user(udp(p.peer_user))
    {
        assert(relay.open(p));std::array<s::communication_worker::role_fd,4> ends{};assert(relay.take_worker_pipes(ends));
        for(std::size_t i{};i<4;++i){assert(t::index(ends[i].role)==i);pipes[i]=t::descriptor{ends[i].descriptor};}
        assert(!relay.take_worker_pipes(ends));assert(relay.activate());
    }
    ~fixture(){relay.revoke();}
};
int main()
{
    auto p=policy();assert(!t::plan{}.valid());auto malformed=p;malformed.peer_source=malformed.local_user;assert(!malformed.valid());
#if defined(UWVM_TRANSPORT_ALLOCATION)
    std::size_t failures{};bool exhausted{};
    for(std::size_t point=1;point<64;++point)
    {
        t::broker relay;fail_after=point;injected=false;auto opened=relay.open(p);auto failed=injected.load();fail_after=0;
        if(failed){++failures;assert(!opened&&relay.inspect().fenced);}else{assert(opened);exhausted=true;break;}
    }
    assert(exhausted&&failures>=8);std::printf("recoverable transport allocation failures %zu\n",failures);
#endif
    malformed=p;malformed.queues[0].attenuated=true;assert(!malformed.valid());malformed.allow_uncertain_attenuation=true;assert(malformed.valid());
    for(auto role:s::communication_worker::roles)
    {
        auto b=packet(p,role);for(std::size_t n{};n<b.size();++n)assert(!t::packet_allowed(p,role,{b.data(),n}));
        auto wrong=b;wrong[8]^=64;assert(!t::packet_allowed(p,role,wrong));
        wrong=b;wrong[20]=0x0f;assert(!t::packet_allowed(p,role,wrong));
        wrong=b;wrong[22]=255;assert(!t::packet_allowed(p,role,wrong));
        wrong=b;wrong[35]^=0xc0;assert(!t::packet_allowed(p,role,wrong));
    }
    auto data=packet(p,t::role::user_rx);malformed=p;malformed.user_data_receive=false;assert(!t::packet_allowed(malformed,t::role::user_rx,data));
    {
        auto statistics=data;statistics.resize(data.size()+60);statistics[data.size()]=0x80;statistics[data.size()+1]=1;statistics[data.size()+2]=56;
        assert(t::packet_allowed(p,t::role::user_rx,statistics));statistics.back()=1;assert(t::packet_allowed(p,t::role::user_rx,statistics));
        statistics[data.size()+4]=1;assert(!t::packet_allowed(p,t::role::user_rx,statistics)); // No arbitrary destination locator.
        statistics[data.size()+4]=0;statistics[7]=9;assert(!t::packet_allowed(p,t::role::user_rx,statistics));
    }
    std::array<std::uint8_t,32> header{};assert(t::encode_header(header,{p.generation,1,48}));t::frame_header decoded{};
    assert(t::decode_header(header,p,1,decoded));assert(!t::decode_header(header,p,2,decoded));
    for(std::size_t n{};n<header.size();++n)assert(!t::decode_header({header.data(),n},p,1,decoded));
    {
        fixture f{p};auto rx=packet(p,t::role::discovery_rx);
        auto wrong_port=udp(0);auto wrong_address=udp(p.peer_source,0x7f000002u);
        send(wrong_port.value,p.local_discovery,rx);send(wrong_address.value,p.local_discovery,rx);
        assert(!ready(f.pipes[0].value,POLLIN,80));send(f.peer.value,p.local_discovery,rx);assert(read_frame(f.pipes[0].value,p,1)==rx);
        auto bad=rx;bad[8]^=1;send(f.peer.value,p.local_discovery,bad);assert(!ready(f.pipes[0].value,POLLIN,50));
        send(f.peer.value,p.local_user,packet(p,t::role::user_rx));assert(read_frame(f.pipes[2].value,p,1)==packet(p,t::role::user_rx));
        auto tx=packet(p,t::role::discovery_tx);write_frame(f.pipes[1].value,p,1,tx,true);assert(ready(f.discovery.value,POLLIN,1000));
        std::array<std::uint8_t,1024> out; sockaddr_in source{};socklen_t source_size=sizeof(source);
        auto n=::recvfrom(f.discovery.value,out.data(),out.size(),0,reinterpret_cast<sockaddr*>(&source),&source_size);
        assert(n==static_cast<ssize_t>(tx.size())&&std::equal(tx.begin(),tx.end(),out.begin())&&ntohs(source.sin_port)==p.local_discovery_source);
        write_frame(f.pipes[3].value,p,1,packet(p,t::role::user_tx));assert(ready(f.user.value,POLLIN,1000));assert(::recv(f.user.value,out.data(),out.size(),0)==48);
        // A replayed private-pipe sequence is a worker protocol breach, not a new effect.
        write_frame(f.pipes[1].value,p,1,tx);for(unsigned i{};i<100&&!f.relay.inspect().fenced;++i){pollfd unused{-1,0,0};::poll(&unused,0,2);}
        auto observed=f.relay.inspect();assert(observed.fenced&&observed.uncertain&&observed.forwarded[0]==1&&observed.forwarded[1]==1&&observed.rejected[0]==1);
    }
    {
        auto limited=p;limited.allow_uncertain_attenuation=true;limited.queues[2].attenuated=true; // Explicit zero, not unlimited.
        fixture f{limited};send(f.peer.value,p.local_user,packet(p,t::role::user_rx));assert(!ready(f.pipes[2].value,POLLIN,60));
        send(f.peer.value,p.local_discovery,packet(p,t::role::discovery_rx));assert(read_frame(f.pipes[0].value,p,1)==packet(p,t::role::discovery_rx));
        auto observed=f.relay.inspect();assert(!observed.fenced&&observed.uncertain&&observed.attenuated[2]==1&&observed.forwarded[2]==0&&observed.forwarded[0]==1);
    }
    {
        auto timed=p;timed.frame_deadline_ns=20000000;fixture f{timed};std::uint8_t partial='R';assert(::write(f.pipes[1].value,&partial,1)==1);
        for(unsigned i{};i<100&&!f.relay.inspect().fenced;++i){pollfd unused{-1,0,0};::poll(&unused,0,2);}
        assert(f.relay.inspect().fenced&&f.relay.inspect().uncertain);
    }
    std::puts("passed transport envelope, source/port rejection, four directions, replay fence and zero attenuation");
}
