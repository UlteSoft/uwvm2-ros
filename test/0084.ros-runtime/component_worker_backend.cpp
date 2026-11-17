/** @file @brief Actual isolated component role calls and malicious-worker denial probes. */
#include <uwvm2/imported/ros/scene/component_protocol.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctime>
namespace p=uwvm2::imported::ros::scene::component_protocol;namespace w=uwvm2::imported::ros::scene::wire;
extern "C" bool uwvm_component_run(p::invocation const& invocation,p::call_function call,void* context)noexcept
{
    w::reader in{invocation.configuration};std::uint64_t mode{};if(!in.u64(mode)||!in.done()||invocation.count!=2)return false;
    std::array<std::uint8_t,4096> output{};std::size_t size{};
    if(mode==11)
    {
        // A real native memory fault after confinement and startup, with no
        // permitted role call. The host must retain failure after reaping.
        auto page=::mmap(nullptr,4096,PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
        if(page==MAP_FAILED)return false;
        *static_cast<volatile std::uint8_t*>(page)=1;return false;
    }
    if(mode==10)return call(context,2,p::action::reserve,{},output,size)!=0;
    if(mode==4){for(;;){timespec wait{0,1000000};::nanosleep(&wait,nullptr);}}
    if(mode==5)return call(context,3,p::action::publish,{output.data(),1},output,size)!=0;
    if(mode==6){std::array<std::uint8_t,8> huge;huge.fill(255);(void)::write(1,huge.data(),huge.size());return true;}
    if(mode==2||mode==9)
    {
        if(call(context,2,p::action::reserve,{},output,size))return false;
        if(mode==9)return true;
        w::reader source{{output.data(),size}};std::uint64_t endpoint{},generation{},sequence{},nonce{};
        if(!source.u64(endpoint)||!source.u64(generation)||!source.u64(sequence)||!source.u64(nonce))return false;
        std::array<std::uint8_t,24> token;w::writer out{token};out.u64(generation);out.u64(sequence);out.u64(nonce);
        return call(context,2,p::action::acknowledge,token,output,size)==0;
    }
    if(mode==3)
    {
        auto net=::socket(AF_INET,SOCK_STREAM,0);if(net>=0){::close(net);return false;}
        auto file=::open("/etc/passwd",O_RDONLY);if(file>=0){::close(file);return false;}
        auto created=::open("/scratch/unbounded",O_CREAT|O_RDWR,0600);if(created>=0){::close(created);return false;}
        auto code=::mmap(nullptr,4096,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
        if(code!=MAP_FAILED){::munmap(code,4096);return false;}
        if(::syscall(SYS_fork)>=0)return false;
    }
    std::array<std::uint8_t,1> value{1};
    if(call(context,1,p::action::publish,value,output,size))return false;
    return mode!=8||call(context,1,p::action::publish,value,output,size)==0;
}
