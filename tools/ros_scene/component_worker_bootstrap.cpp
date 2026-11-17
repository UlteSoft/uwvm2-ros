/** @file
 * @brief Fixed isolated-component bootstrap; the VM never loads backend code.
 * @details This executable and pre-filter constructors belong to the approved
 * image closure. The outer namespace/seccomp policy already applies when they
 * run. Final no-exec/no-executable-mapping restrictions precede configuration
 * delivery. The control attachment must be empty: components receive no native
 * file descriptors. All robot access uses the closed role message callback.
 */
#include "../../test/0084.ros-runtime/support/legacy_process/model_sandbox.h"
#include <uwvm2/imported/ros/scene/component_protocol.h>
#include <uwvm2/imported/ros/scene/byte_buffer.h>
#include <dlfcn.h>
namespace s=uwvm2::imported::ros::scene;namespace p=s::component_protocol;
static bool transfer(int fd,std::span<std::uint8_t> bytes,bool sending)noexcept
{
    std::size_t done{};while(done<bytes.size())
    {auto count=sending?::write(fd,bytes.data()+done,bytes.size()-done) : ::read(fd,bytes.data()+done,bytes.size()-done);
        if(count<0&&errno==EINTR)continue;if(count<=0)return false;done+=static_cast<std::size_t>(count);}
    return true;
}
static bool send(s::wire::bytes bytes)noexcept
{return transfer(1,{const_cast<std::uint8_t*>(bytes.data()),bytes.size()},true);}
static bool packet(s::wire::bytes bytes)noexcept
{std::array<std::uint8_t,8> size;s::wire::writer out{size};return out.u64(bytes.size())&&send(size)&&send(bytes);}
struct context {std::uint64_t sequence{},maximum{};bool failed{};s::byte_buffer scratch;};
static std::uint32_t call(void* opaque,std::uint64_t slot,p::action action,s::wire::bytes input,std::span<std::uint8_t> output,std::size_t& size)noexcept
{
    auto& state=*static_cast<context*>(opaque);size=0;
    if(state.failed||state.sequence==UINT64_MAX||!slot||static_cast<unsigned>(action)<1||static_cast<unsigned>(action)>4||input.size()>state.maximum-40)return 22;
    try
    {
        state.scratch.resize_for_overwrite(input.size()+40);s::wire::writer out{state.scratch.writable()};
        out.u64(static_cast<std::uint64_t>(p::frame::call));out.u64(++state.sequence);out.u64(slot);out.u64(static_cast<std::uint64_t>(action));out.blob(input);
        if(!packet(out.written())){state.failed=true;return 5;}
        std::array<std::uint8_t,8> header;if(!transfer(0,header,false)){state.failed=true;return 5;}
        s::wire::reader length{header};std::uint64_t extent{};if(!length.u64(extent)||extent>state.maximum||extent<24){state.failed=true;return 22;}
        state.scratch.resize_for_overwrite(extent);if(!transfer(0,state.scratch.writable(),false)){state.failed=true;return 5;}
        s::wire::reader response{state.scratch.view()};std::uint64_t sequence{},status{};s::wire::bytes body;
        if(!response.u64(sequence)||sequence!=state.sequence||!response.u64(status)||status>UINT32_MAX||!response.blob(body)||!response.done())
        {state.failed=true;return 22;}
        size=body.size();if(size>output.size())return 90;std::copy(body.begin(),body.end(),output.begin());return static_cast<std::uint32_t>(status);
    }
    catch(...){state.failed=true;return 12;}
}
int main(int argc,char** argv)
{
    if(argc!=4)return 120;
    std::array<std::uint64_t,3> limits{};
    for(unsigned i{};i<3;++i)
    {std::string_view text{argv[i+1]};auto parsed=std::from_chars(text.data(),text.data()+text.size(),limits[i]);if(parsed.ec!=std::errc{}||parsed.ptr!=text.data()+text.size())return 121;}
    if(limits[0]<128||limits[0]>1048576||limits[1]!=limits[0]||!limits[2]||limits[2]>INT64_MAX)return 121;
    void* library=::dlopen("/worker/backend.so",RTLD_NOW|RTLD_LOCAL);if(!library)return 122;
    auto backend=reinterpret_cast<p::backend_function>(::dlsym(library,"uwvm_component_run"));if(!backend)return 123;
    rlimit file_bound{limits[2],limits[2]};if(::prlimit(0,RLIMIT_FSIZE,&file_bound,nullptr))return 124;
    s::model_sandbox::syscall_filter filter{true};if(!filter.install())return 125;
    if(!send({reinterpret_cast<std::uint8_t const*>(p::ready.data()),p::ready.size()}))return 126;
    std::array<std::uint8_t,8> empty{};alignas(cmsghdr)std::array<std::uint8_t,CMSG_SPACE(sizeof(int))> control{};
    iovec vector{empty.data(),empty.size()};msghdr message{};message.msg_iov=&vector;message.msg_iovlen=1;message.msg_control=control.data();message.msg_controllen=control.size();
    if(::recvmsg(3,&message,MSG_CMSG_CLOEXEC)!=8||message.msg_flags&(MSG_TRUNC|MSG_CTRUNC)||CMSG_FIRSTHDR(&message)||
        !std::all_of(empty.begin(),empty.end(),[](auto value){return value==0;}))return 127;
    ::close(3);if(!send({reinterpret_cast<std::uint8_t const*>(p::attached.data()),p::attached.size()}))return 128;
    try
    {
        std::array<std::uint8_t,8> header;if(!transfer(0,header,false))return 129;
        s::wire::reader size{header};std::uint64_t extent{};if(!size.u64(extent)||extent>limits[0])return 130;
        s::byte_buffer configuration;configuration.resize_for_overwrite(extent);if(!transfer(0,configuration.writable(),false))return 131;
        p::invocation invocation;if(!p::decode(configuration.view(),invocation))return 132;
        std::array<std::uint8_t,24> started;s::wire::writer ready{started};ready.u64(static_cast<std::uint64_t>(p::frame::started));ready.u64(invocation.generation);ready.u64(invocation.instance);
        if(!packet(ready.written()))return 133;
        context state;state.maximum=limits[0];auto result=backend(invocation,call,&state);if(state.failed)return 134;
        std::array<std::uint8_t,24> finished;s::wire::writer out{finished};out.u64(static_cast<std::uint64_t>(p::frame::finished));out.u64(state.sequence);out.boolean(result);
        return packet(out.written())?(result?0:1):135;
    }
    catch(...){return 136;}
}
