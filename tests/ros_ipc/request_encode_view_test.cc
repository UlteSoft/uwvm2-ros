#include <uwvm2/imported/ros/ipc/protocol.h>
#include <uwvm2/imported/ros/ipc/unix.h>
#include <cstdlib>
#include <cstdio>
#include <new>
#include <string>
#include <vector>
#include <utility>
#include <sys/wait.h>

namespace allocation
{
    bool enabled{};
    std::size_t calls{},bytes{};
    struct scope
    {
        scope() { calls=bytes=0;enabled=true; }
        ~scope() { enabled=false; }
    };
    struct result { std::size_t calls{},bytes{}; };
}
void* operator new(std::size_t size)
{
    auto p=std::malloc(size?size:1);if(!p) { throw std::bad_alloc{}; }
    if(allocation::enabled) { ++allocation::calls;allocation::bytes+=size; }return p;
}
void* operator new[](std::size_t size) { return ::operator new(size); }
void operator delete(void* p) noexcept { std::free(p); }
void operator delete[](void* p) noexcept { std::free(p); }
void operator delete(void* p,std::size_t) noexcept { std::free(p); }
void operator delete[](void* p,std::size_t) noexcept { std::free(p); }

namespace ipc=uwvm2::imported::ros::ipc;
unsigned checks{};
void require(bool condition,char const* reason)
{
    ++checks;if(!condition) { std::fprintf(stderr,"FAIL request encode view: %s\n",reason);std::exit(1); }
}

// Independent frozen-format reference, including rejection-before-output-change.
bool reference(ipc::request const& input,std::vector<std::uint8_t>& bytes)
{
    auto valid=[](std::string const& text) {
        if(text.empty()||text.size()>255) { return false; }
        for(auto c:text)
        { if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_'||c=='-'||c=='.')) { return false; } }
        return true;
    };
    if(!valid(input.operation)||!valid(input.alias)||input.data.size()>ipc::max_payload_bytes-518) { return false; }
    bytes.clear();bytes.reserve(8+input.operation.size()+input.alias.size()+input.data.size());
    auto field=[&](std::uint64_t value,unsigned width) {
        for(unsigned i{};i<width;++i) { bytes.push_back(static_cast<std::uint8_t>(value>>(i*8u))); }
    };
    field(input.operation.size(),2);field(input.alias.size(),2);field(input.data.size(),4);
    for(char c:input.operation) { bytes.push_back(static_cast<std::uint8_t>(c)); }
    for(char c:input.alias) { bytes.push_back(static_cast<std::uint8_t>(c)); }
    for(auto byte:input.data) { bytes.push_back(byte); }return true;
}

void differential(std::string operation,std::string alias,std::vector<std::uint8_t> data,bool valid)
{
    ipc::request input{std::move(operation),std::move(alias),std::move(data)};
    std::vector<std::uint8_t> expected{0xa5,0x00,0xff},owning{expected},braced{expected};
    require(reference(input,expected)==valid,"reference validity");
    require(ipc::encode_request(input,owning)==valid&&owning==expected,"owning byte/false-output compatibility");
    require(ipc::encode_request({input.operation,input.alias,input.data},braced)==valid&&braced==expected,"braced source/byte compatibility");
#if !defined(UWVM_ROS_REQUEST_VIEW_LEGACY_BASELINE)
    std::vector<std::uint8_t> view{0xa5,0x00,0xff};
    require(ipc::encode_request_view({input.operation,input.alias,input.data},view)==valid&&view==expected,"borrowed byte/false-output equality");
    auto saved=view;input.operation.assign("changed");input.alias.assign("changed");input.data.assign(3,0x42);
    require(view==saved,"encoded bytes independent of input lifetime/mutation");
#endif
}

allocation::result direct_allocations(bool invalid,bool borrowed)
{
    std::string operation(80,'o'),alias(80,'a');if(invalid) { operation[17]='!'; }
    std::vector<std::uint8_t> data(64,0x5a),bytes;
    bool ok{};allocation::result counted;
    {
        allocation::scope scope;
#if !defined(UWVM_ROS_REQUEST_VIEW_LEGACY_BASELINE)
        if(borrowed) { ok=ipc::encode_request_view({operation,alias,data},bytes); }
        else
#else
        (void)borrowed;
#endif
        { ok=ipc::encode_request({operation,alias,std::move(data)},bytes); }
        counted={allocation::calls,allocation::bytes};
    }
    require(ok==!invalid,"scoped direct validity");return counted;
}

void peer(int listener,std::string const& operation,std::string const& alias,std::vector<std::uint8_t> const& data)
{
    int fd=::accept(listener,nullptr,nullptr);if(fd<0) { ::_exit(11); }
    ipc::frame hello;
    if(!ipc::read_frame(fd,hello,ipc::after(5000))||hello.type!=ipc::kind::hello||hello.backend!=2||hello.request_id!=1) { ::_exit(12); }
    ipc::frame response;response.type=ipc::kind::response;response.backend=2;response.response_id=1;response.session_id=42;response.timeout_ms=1000;
    if(!ipc::write_frame(fd,response,ipc::after(5000))) { ::_exit(13); }
    std::vector<std::uint8_t> expected;
    if(!reference({operation,alias,data},expected)) { ::_exit(14); }
    for(std::uint64_t id:{2ULL,4ULL})
    {
        ipc::frame request;
        if(!ipc::read_frame(fd,request,ipc::after(5000))||request.type!=ipc::kind::request||request.backend!=2||
           request.session_id!=42||request.request_id!=id||request.payload!=expected) { ::_exit(15); }
        response.response_id=id;
        if(!ipc::write_frame(fd,response,ipc::after(5000))) { ::_exit(16); }
    }
    ::close(fd);::_exit(0);
}

struct socket_stats { allocation::result first{},invalid{},next{}; };
socket_stats actual_client()
{
    std::string path="/tmp/uwvm-request-view-"+std::to_string(::getpid())+".sock";
    int listener=::socket(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC,0);require(listener>=0,"listener socket");
    sockaddr_un address{};address.sun_family=AF_UNIX;
    require(path.size()<sizeof address.sun_path,"socket path length");
    std::memcpy(address.sun_path,path.c_str(),path.size()+1);::unlink(path.c_str());
    require(::bind(listener,reinterpret_cast<sockaddr*>(&address),sizeof address)==0&&::listen(listener,1)==0,"listener bind");
    require(::chmod(path.c_str(),0600)==0,"private socket permissions");
    std::string operation(80,'o'),alias(80,'a');std::vector<std::uint8_t> data(64,0x5a);
    auto pid=::fork();require(pid>=0,"fork peer");if(!pid) { peer(listener,operation,alias,data); }
    ::close(listener);socket_stats stats;
    {
        ipc::client client;require(client.connect(path,2,static_cast<std::uint32_t>(::geteuid()),1000)==0,"authenticated session hello");
        auto perform=[&](std::string const& op,std::uint32_t expected) {
            auto input=data;ipc::call_result result;allocation::result counted;
            {
                allocation::scope scope;result=client.call(op,alias,std::move(input),1000);counted={allocation::calls,allocation::bytes};
            }
            require(result.error==expected&&result.payload.empty(),"actual IPC result");return counted;
        };
        stats.first=perform(operation,0);
        auto bad=operation;bad[17]='!';stats.invalid=perform(bad,22);
        stats.next=perform(operation,0); // Invalid metadata consumes id3 without writing; session id42 stays live.
    }
    int status{};require(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&WEXITSTATUS(status)==0,"wire peer ids/session/bytes");
    require(::unlink(path.c_str())==0,"socket cleanup");return stats;
}

int main()
{
    differential("event.poll","sub",{},true);
    differential("O_1.-","A_2.-",{0x00,0x7f,0x80,0xff},true);
    differential(std::string(255,'o'),std::string(255,'a'),std::vector<std::uint8_t>(ipc::max_payload_bytes-518,0x5a),true);
    differential("op","alias",std::vector<std::uint8_t>(ipc::max_payload_bytes-517,0x5a),false);
    for(auto text:{std::string{},std::string(256,'x'),std::string{"a b"},std::string{"a/b"},std::string{"a\0b",3},std::string{"a\xff",2}})
    { differential(text,"alias",{},false);differential("op",text,{},false); }
    auto old=direct_allocations(false,false),invalid_old=direct_allocations(true,false);
    require(old.calls==3&&invalid_old.calls==2,"actual owning metadata baseline allocations");
    auto socket=actual_client();
#if defined(UWVM_ROS_REQUEST_VIEW_LEGACY_BASELINE)
    require(socket.first.calls==3&&socket.invalid.calls==2&&socket.next.calls==3,"legacy client actual allocation baseline");
    std::printf("PASS legacy request baseline checks=%u direct_calls=%zu direct_bytes=%zu invalid_calls=%zu invalid_bytes=%zu socket_calls=%zu socket_invalid=%zu socket_next=%zu same_session1 wire_equal1\n",checks,old.calls,old.bytes,invalid_old.calls,invalid_old.bytes,socket.first.calls,socket.invalid.calls,socket.next.calls);
#else
    auto view=direct_allocations(false,true),invalid_view=direct_allocations(true,true);
    require(view.calls==1&&view.bytes==8+80+80+64&&invalid_view.calls==0&&invalid_view.bytes==0,"borrowed metadata allocation improvement");
    require(socket.first.calls==1&&socket.invalid.calls==0&&socket.next.calls==1,"borrowed client actual allocation improvement");
    std::printf("PASS request view checks=%u direct_owning_calls=%zu direct_owning_bytes=%zu direct_view_calls=%zu direct_view_bytes=%zu invalid_owning_calls=%zu invalid_view_calls=%zu socket_calls=%zu socket_invalid=%zu socket_next=%zu same_session1 wire_equal1\n",checks,old.calls,old.bytes,view.calls,view.bytes,invalid_old.calls,invalid_view.calls,socket.first.calls,socket.invalid.calls,socket.next.calls);
#endif
}
