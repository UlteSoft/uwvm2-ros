/** @file @brief Pinned TLS transport checks on real provisioned Linux sockets. */
#include <uwvm2/imported/ros/scene/attachment_tls.h>
#include <cassert>
#include <cstdio>
#include <linux/memfd.h>
#include <sys/wait.h>
namespace scene=uwvm2::imported::ros::scene;
namespace tls=scene::attachment_tls;
namespace driver=scene::drivers;
using error=tls::error;

struct key_pair {
    std::array<std::uint8_t,32> seed{},pub{};
    key_pair(){
        EVP_PKEY* key=EVP_PKEY_Q_keygen(nullptr,nullptr,"ED25519");assert(key);
        auto size=seed.size();assert(EVP_PKEY_get_raw_private_key(key,seed.data(),&size)==1&&size==seed.size());
        size=pub.size();assert(EVP_PKEY_get_raw_public_key(key,pub.data(),&size)==1&&size==pub.size());
        EVP_PKEY_free(key);
    }
    ~key_pair(){scene::erase_bytes(seed);}
};
static int identity(key_pair const& own,key_pair const& remote,bool seal=true){
    int fd=static_cast<int>(::syscall(SYS_memfd_create,"test-attachment-identity",MFD_ALLOW_SEALING|MFD_CLOEXEC));assert(fd>=0);
    assert(::write(fd,own.seed.data(),32)==32&&::write(fd,remote.pub.data(),32)==32);
    if(seal)assert(!::fcntl(fd,F_ADD_SEALS,F_SEAL_WRITE|F_SEAL_GROW|F_SEAL_SHRINK|F_SEAL_SEAL));
    return fd;
}
static void pair(int (&sockets)[2]){
    assert(!::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,sockets));
    int enabled=1;
    for(int fd:sockets)assert(!::setsockopt(fd,SOL_SOCKET,SO_PASSCRED,&enabled,sizeof(enabled)));
}
enum class scenario { success,wrong_client_pin,wrong_server_pin,wrong_contract,cross_service,corrupt,wrong_origin };
static void exercise(scenario mode,tls::purpose service,std::size_t size){
    key_pair client,server,other;
    int left=identity(client,mode==scenario::wrong_server_pin?other:server);
    int right=identity(server,mode==scenario::wrong_client_pin?other:client);
    int client_pipe[2],server_pipe[2];pair(client_pipe);pair(server_pipe);
    driver::credentials peer{static_cast<std::uint64_t>(::getpid()),::getuid(),::getgid()};
    constexpr tls::id deployment{1,11},vm{2,12},scope{3,13},provider{4,14},sid{5,15};
    tls::digest contract{6,7,8,9};
    tls::channel sender,receiver;
    auto maximum=driver::protocol::header_bytes+size;
    assert(sender.bind(client_pipe[0],peer,left,service,deployment,vm,scope,provider,sid,contract,maximum)==error::success);
    if(mode==scenario::wrong_contract)++contract.back();
    if(mode==scenario::wrong_origin)peer.pid=static_cast<std::uint64_t>(::getppid());
    assert(receiver.bind(server_pipe[0],peer,right,mode==scenario::cross_service?tls::purpose::deployment:service,
                         deployment,vm,scope,provider,sid,contract,maximum,true)==error::success);
    ::close(left);::close(right);
    std::array<std::uint8_t,driver::protocol::header_bytes> header{};
    constexpr std::string_view marker="secret attachment payload for local TLS checks";
    std::copy(marker.begin(),marker.end(),header.begin());
    std::array<std::uint8_t,driver::protocol::maximum_payload> payload{};
    for(std::size_t n=0;n<size;++n)payload[n]=static_cast<std::uint8_t>(n*67+13);
    std::array<std::uint8_t,tls::maximum_frame> output{};output.fill(0xdb);
    std::array<std::uint8_t,tls::maximum_frame+2048> encrypted{};
    auto relay=[&](int input,int target,bool alter){
        ssize_t count=::recv(input,encrypted.data(),encrypted.size(),MSG_DONTWAIT);
        if(count<0){assert(errno==EAGAIN||errno==EWOULDBLOCK);return;}
        assert(count>0);
        auto end=encrypted.begin()+count;
        assert(std::search(encrypted.begin(),end,marker.begin(),marker.end())==end);
        if(alter)encrypted[static_cast<std::size_t>(count)-1]^=1;
        assert(::send(target,encrypted.data(),static_cast<std::size_t>(count),MSG_NOSIGNAL)==count);
    };
    bool sent{};std::size_t delivered=SIZE_MAX;error status=error::again;
    for(unsigned n=0;n<128&&status==error::again;++n){
        if(!sent){
            status=sender.send(header,std::span{payload}.first(size));
            if(status!=error::success&&status!=error::again)break;
            sent=status==error::success;
        }
        relay(client_pipe[1],server_pipe[1],mode==scenario::corrupt&&sent);
        status=receiver.receive(output,delivered);
        relay(server_pipe[1],client_pipe[1],false);
    }
    if(mode==scenario::success){
        if(status!=error::success)ERR_print_errors_fp(stderr);
        assert(status==error::success&&sent&&delivered==maximum);
        assert(std::equal(header.begin(),header.end(),output.begin()));
        assert(std::equal(payload.begin(),payload.begin()+size,output.begin()+header.size()));
        assert(receiver.send(header,{})==error::success);
        relay(server_pipe[1],client_pipe[1],false);
        assert(sender.receive(output,delivered)==error::success&&delivered==header.size());
    }else{
        assert(status==error::denied&&delivered==SIZE_MAX);
        assert(std::all_of(output.begin(),output.end(),[](auto byte){return byte==0xdb;}));
    }
    sender.close();receiver.close();
    for(int fd:client_pipe)::close(fd);
    for(int fd:server_pipe)::close(fd);
}
int main(){
    key_pair mine,peer;
    for(bool seal:{false,true}){
        int key=identity(mine,peer,seal);assert(tls::check_identity(key)==(seal?error::success:error::denied));::close(key);
    }
    for(auto kind:{tls::purpose::driver,tls::purpose::model,tls::purpose::deployment})
        for(auto size:{0u,8u,16384u,65536u})exercise(scenario::success,kind,size);
    for(auto failure:{scenario::wrong_client_pin,scenario::wrong_server_pin,scenario::wrong_contract,
                      scenario::cross_service,scenario::corrupt,scenario::wrong_origin})exercise(failure,tls::purpose::driver,8);
    std::puts("pinned TLS success, credential, binding, integrity and maximum-frame checks passed");
}
