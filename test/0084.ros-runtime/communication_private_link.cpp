/** @file @brief Real sealed-peer identity and bounded directional private-link failure tests.
 * @details The test-only idle backend permits the trusted fixture to inject
 * exact pipe frames. This qualifies relay identity/framing, not native DDS.
 */
#include "communication_attachment_fixture.h"
#include <sys/wait.h>
#include <uwvm2/imported/ros/scene/communication_private_link.h>
#include <dirent.h>
namespace t=s::communication_transport;
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
#if defined(UWVM_PRIVATE_LINK_ALLOCATION)
static std::atomic<std::size_t> fail_after{};static std::atomic<bool> injected{};
static bool reject()noexcept
{auto n=fail_after.load();while(n){if(fail_after.compare_exchange_weak(n,n-1)){if(n==1){injected=true;return true;}break;}}return false;}
extern "C" void* __real_malloc(std::size_t);extern "C" void* __real_calloc(std::size_t,std::size_t);extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return reject()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t a,std::size_t b){return reject()?nullptr:__real_calloc(a,b);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return reject()?nullptr:__real_realloc(p,n);}
static unsigned descriptor_count()
{auto directory=::opendir("/proc/self/fd");assert(directory);unsigned count{};while(::readdir(directory))++count;::closedir(directory);return count;}
#endif
static std::array<t::plan,2> policies()
{
    t::plan a;a.generation=734;a.domain=193;a.local_participant=0;a.peer_participant=1;
    a.local_discovery=55660;a.local_user=55661;a.peer_discovery=55662;a.peer_user=55663;
    a.local_discovery_source=60110;a.local_user_source=60111;a.peer_source=60112;
    a.local_prefix[0]=1;a.local_prefix[11]=7;a.peer_prefix[0]=2;a.peer_prefix[11]=9;
    a.allowed.fill(true);a.user_data_receive=a.user_data_transmit=true;a.frame_deadline_ns=20000000;
    a.max_datagram_bytes=1024;a.max_sample_bytes=4096;a.max_submessages=32;a.socket_buffer_bytes=16384;
    for(auto& q:a.queues){q.packets=8;q.bytes=8192;}assert(a.valid());
    auto b=a;b.generation=735;std::swap(b.local_participant,b.peer_participant);std::swap(b.local_discovery,b.peer_discovery);
    std::swap(b.local_user,b.peer_user);std::swap(b.local_prefix,b.peer_prefix);assert(b.valid());return {a,b};
}
static bytes packet(t::plan const& p,bool discovery)
{
    bytes b(48);b[0]='R';b[1]='T';b[2]='P';b[3]='S';b[4]=2;b[5]=3;b[6]=1;b[7]=15;
    std::copy(p.local_prefix.begin(),p.local_prefix.end(),b.begin()+8);
    b[20]=0x15;b[21]=5;b[22]=24;b[26]=16;b[32]=1;b[35]=discovery?0xc2:3;b[40]=1;
    assert(t::packet_allowed(p,discovery?t::role::discovery_tx:t::role::user_tx,b));return b;
}
static bool ready(int fd,short events,int timeout)
{pollfd p{fd,events,0};auto n=::poll(&p,1,timeout);assert(n>=0);return n&&(p.revents&events);}
static void write_frame(int fd,t::plan const& plan,std::uint64_t sequence,bytes const& body)
{
    bytes data(t::header_bytes+body.size());assert(t::encode_header({data.data(),t::header_bytes},{plan.generation,sequence,body.size()}));
    std::copy(body.begin(),body.end(),data.begin()+t::header_bytes);assert(::write(fd,data.data(),data.size())==static_cast<ssize_t>(data.size()));
}
static bytes read_frame(int fd,t::plan const& plan,std::uint64_t sequence)
{
    bytes data(t::header_bytes);std::size_t at{};
    auto read=[&]{while(at<data.size()){assert(ready(fd,POLLIN,1000));auto n=::read(fd,data.data()+at,data.size()-at);assert(n>0);at+=static_cast<std::size_t>(n);}};
    read();t::frame_header frame;assert(t::decode_header(data,plan,sequence,frame));data.resize(frame.length);at=0;read();return data;
}
struct pair_fixture
{
    std::array<t::plan,2> plans;
    std::array<fixture,2> provisioners;
    std::array<std::shared_ptr<a::connection>,2> workers;
    std::array<s::buffers::profile,2> profiles;
    std::array<std::array<a::descriptor,4>,2> endpoints;
    t::private_link link;
    pair_fixture(char**,std::array<t::plan,2> selected):plans(selected)
    {
        for(std::size_t i{};i<2;++i)
        {
            auto& provisioner=provisioners[i];auto configuration=provisioner.configuration();profiles[i]=configuration.profile();
            workers[i]=uwvm2::utils::container::make_shared<a::connection>();
            assert(workers[i]->prepare(configuration)==error::success&&workers[i]->begin()==error::again);
            provisioner.response(a::ready);assert(workers[i]->begin()==error::success);
            for(std::size_t direction{};direction<4;++direction)
                endpoints[i][direction].reset(::fcntl(provisioner.pipes[direction+3][direction%2],F_DUPFD_CLOEXEC,3));
        }
    }
    void attach()
    {
        assert(link.prepare(plans,workers,profiles)&&!link.activate());
        for(std::size_t side{};side<2;++side)
        {
            assert(workers[side]->activate_channel()==error::again);provisioners[side].response(a::attached);
            assert(workers[side]->activate_channel()==error::success);
        }
    }
    void fenced()
    {
        for(unsigned n{};n<100&&!link.inspect().fenced;++n)::poll(nullptr,0,2);
        assert(link.inspect().fenced&&link.inspect().uncertain);
        // Failure must fence both shared RPC sessions, even without a supervisor.
        assert(!workers[0]->session_live()&&!workers[1]->session_live());
    }
    ~pair_fixture(){link.retire();}
};
int main(int argc,char** argv)
{
    assert(argc==1||argc==9);
#if defined(UWVM_PRIVATE_LINK_ALLOCATION)
    {
        pair_fixture f{argv+1,policies()};std::size_t failures{};bool exhausted{};auto descriptors=descriptor_count();
        for(std::size_t point=1;point<64;++point)
        {
            {
                t::private_link value;fail_after=point;injected=false;auto success=value.prepare(f.plans,f.workers,f.profiles);auto failed=injected.load();fail_after=0;
                if(failed){++failures;assert(!success&&value.inspect().fenced);}else{assert(success);exhausted=true;}
            }
            assert(descriptor_count()==descriptors);if(exhausted)break;
        }
        assert(exhausted&&failures>=8);std::printf("recoverable private-link fast_io allocation points=%zu; no descriptor leak\n",failures);
    }
#endif
    {
        pair_fixture f{argv+1,policies()};auto bad=f.profiles;bad[0].sha256[0]^=1;t::private_link denied;
        assert(!denied.prepare(f.plans,f.workers,bad));auto other=f.plans;other[1].peer_prefix[11]^=1;assert(!denied.prepare(other,f.workers,f.profiles));
        f.attach();auto body=packet(f.plans[0],true);write_frame(f.endpoints[0][1].get(),f.plans[0],1,body);
        assert(!ready(f.endpoints[1][0].get(),POLLIN,30));assert(f.link.activate());assert(read_frame(f.endpoints[1][0].get(),f.plans[1],1)==body);
        for(std::size_t side{};side<2;++side)for(std::size_t channel{};channel<2;++channel)
        {
            if(!side&&!channel)continue;
            auto payload=packet(f.plans[side],!channel);
            write_frame(f.endpoints[side][channel*2+1].get(),f.plans[side],1,payload);assert(read_frame(f.endpoints[1-side][channel*2].get(),f.plans[1-side],1)==payload);
        }
        auto child=::fork();assert(child>=0);if(!child){assert(!f.link.activate()&&f.link.inspect().fenced);::_exit(0);}
        int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
        write_frame(f.endpoints[0][1].get(),f.plans[0],1,body);f.fenced(); // replay
    }
    {
        auto plans=policies();plans[1].allow_uncertain_attenuation=true;plans[1].queues[2].attenuated=true;
        pair_fixture f{argv+1,plans};f.attach();assert(f.link.activate());write_frame(f.endpoints[0][3].get(),plans[0],1,packet(plans[0],false));
        assert(!ready(f.endpoints[1][2].get(),POLLIN,60));auto status=f.link.inspect();assert(!status.fenced&&status.uncertain&&status.attenuated[1]==1);
        write_frame(f.endpoints[0][1].get(),plans[0],1,packet(plans[0],true));assert(read_frame(f.endpoints[1][0].get(),plans[1],1)==packet(plans[0],true));
        f.workers[1]->request_stop();f.fenced();
    }
    {
        pair_fixture f{argv+1,policies()};f.attach();assert(f.link.activate());char byte='x';assert(::write(f.endpoints[0][1].get(),&byte,1)==1);f.fenced();
    }
    std::puts("PASS private external-provider link: exact identities, no pre-activation delivery, four directions, generation translation, replay/death/timeout fencing, zero attenuation and fork");
}
