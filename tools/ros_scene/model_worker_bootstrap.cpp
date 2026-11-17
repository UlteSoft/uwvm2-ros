/** @file
 * @brief Approved CPU bootstrap: load fixed runtime, remove exec, then accept data.
 * @details The executable and every pre-filter constructor are deployment TCB.
 * The backend is exactly /worker/backend.so from the sealed manifest. Model
 * descriptors are absent until the parent observes the post-filter ready marker
 * and transfers them over the fixed receive-only control socket. No model/IPC
 * data is read before final seccomp succeeds.
 */
#include "../../test/0084.ros-runtime/support/legacy_process/model_sandbox.h"
#include <uwvm2/imported/ros/scene/model_worker_protocol.h>
#include <uwvm2/imported/ros/scene/byte_buffer.h>
#include <charconv>
#include <dlfcn.h>
namespace s = uwvm2::imported::ros::scene;
namespace p = s::model_worker;

static bool read_all(int fd, std::span<std::uint8_t> output) noexcept
{
    std::size_t offset{};
    while(offset < output.size())
    {
        auto count = ::read(fd, output.data() + offset, output.size() - offset);
        if(count < 0 && errno == EINTR) { continue; }
        if(count <= 0) { return false; }
        offset += count;
    }
    return true;
}

static bool write_all(int fd, s::wire::bytes bytes) noexcept
{
    std::size_t offset{};
    while(offset < bytes.size())
    {
        auto count = ::write(fd, bytes.data() + offset, bytes.size() - offset);
        if(count < 0 && errno == EINTR) { continue; }
        if(count <= 0) { return false; }
        offset += count;
    }
    return true;
}

struct output_context
{
    std::uint64_t request{}, maximum{};
    bool terminal{}, failed{};
    p::response terminal_kind{};
    s::byte_buffer terminal_body;
};

static bool emit(void* opaque, p::response kind, s::wire::bytes body) noexcept
{
    auto& context = *static_cast<output_context*>(opaque);
    if(context.terminal || context.failed || body.size() > context.maximum || static_cast<unsigned>(kind) < 1 || static_cast<unsigned>(kind) > 3 ||
       (kind == p::response::failed && !body.empty()))
    {
        context.failed = true;
        return false;
    }
    if(kind!=p::response::partial)
    {
        try { context.terminal_body.resize_for_overwrite(body.size()); std::copy(body.begin(),body.end(),context.terminal_body.writable().begin()); }
        catch(...) { context.failed=true;return false; }
        context.terminal=true;context.terminal_kind=kind;return true;
    }
    std::array<std::uint8_t, 32> header{};
    s::wire::writer writer{header};
    if(!writer.u64(body.size() + 24) || !writer.u64(static_cast<std::uint64_t>(kind)) || !writer.u64(context.request) || !writer.u64(body.size()) ||
       !write_all(1, header) || !write_all(1, body))
    {
        context.failed = true;
        return false;
    }
    context.terminal = kind != p::response::partial;
    return true;
}

int main(int argc, char** argv)
{
    if(argc != 4) { return 120; }
    std::uint64_t request_max{}, response_max{}, file_max{};
    auto parse = [](char const* argument, std::uint64_t& output)
    {
        std::string_view text{argument};
        auto result = std::from_chars(text.data(), text.data() + text.size(), output);
        return result.ec == std::errc{} && result.ptr == text.data() + text.size();
    };
    if(!parse(argv[1], request_max) || !parse(argv[2], response_max) || !parse(argv[3], file_max) || !file_max || file_max > INT64_MAX || !request_max || request_max > 256ull * 1024 * 1024 || response_max < 24 ||
       response_max > 4ull * 1024 * 1024 + 64)
    {
        return 121;
    }
    auto library = ::dlopen("/worker/backend.so", RTLD_NOW | RTLD_LOCAL);
    if(!library) { return 122; }
    auto backend = reinterpret_cast<p::backend_function>(::dlsym(library, "uwvm_model_compute"));
    if(!backend) { return 123; }
    struct rlimit file_bound{file_max,file_max};
    if(::prlimit(0,RLIMIT_FSIZE,&file_bound,nullptr)) { return 132; }
    s::model_sandbox::syscall_filter final{true};
    if(!final.install()) { return 124; }
    if(!write_all(1, {reinterpret_cast<std::uint8_t const*>(p::ready.data()), p::ready.size()})) { return 125; }
    std::array<std::uint8_t,20000> weight_metadata{};
    alignas(cmsghdr) std::array<std::uint8_t,CMSG_SPACE(sizeof(int)*64)> ancillary{};
    iovec vector{weight_metadata.data(),weight_metadata.size()};
    msghdr message{}; message.msg_iov=&vector;message.msg_iovlen=1;message.msg_control=ancillary.data();message.msg_controllen=ancillary.size();
    auto received=::recvmsg(3,&message,MSG_CMSG_CLOEXEC);
    if(received<8 || (message.msg_flags&(MSG_TRUNC|MSG_CTRUNC))) { return 133; }
    s::wire::reader metadata{{weight_metadata.data(),static_cast<std::size_t>(received)}};
    std::uint64_t weight_count{};
    std::array<p::weight_view,64> weights{};
    if(!metadata.u64(weight_count) || weight_count>weights.size()) { return 134; }
    auto header=CMSG_FIRSTHDR(&message);
    if(weight_count && (!header || header->cmsg_level!=SOL_SOCKET || header->cmsg_type!=SCM_RIGHTS || header->cmsg_len!=CMSG_LEN(sizeof(int)*weight_count))) { return 135; }
    if((!weight_count && header) || (header && CMSG_NXTHDR(&message,header))) { return 136; }
    for(std::size_t i{};i<weight_count;++i)
    {
        auto& value=weights[i];std::string_view name;
        if(!metadata.text(name) || name.size()>value.name.size() || !name.starts_with("/model/") || !metadata.u64(value.bytes)) { return 137; }
        std::copy(name.begin(),name.end(),value.name.begin());value.name_size=name.size();
        std::memcpy(&value.descriptor,CMSG_DATA(header)+i*sizeof(int),sizeof(int));
        struct stat information{};
        constexpr int required=1|2|4|8|32;
        auto seals=::fcntl(value.descriptor,F_GET_SEALS);
        if(::fstat(value.descriptor,&information) || !S_ISREG(information.st_mode) || information.st_size<0 ||
           static_cast<std::uint64_t>(information.st_size)!=value.bytes || seals<0 || (seals&required)!=required || (information.st_mode&0111)) { return 138; }
    }
    if(!metadata.done()) { return 139; }
    ::close(3);
    if(!write_all(1,{reinterpret_cast<std::uint8_t const*>(p::attached.data()),p::attached.size()})) { return 140; }
    for(;;)
    {
        std::array<std::uint8_t, 8> header{};
        if(!read_all(0, header)) { return 0; }
        s::wire::reader size_reader{header};
        std::uint64_t extent{};
        if(!size_reader.u64(extent) || extent > request_max) { return 126; }
        try
        {
            output_context context;
            context.maximum=response_max-24;
            {
                s::byte_buffer carrier;
                carrier.resize_for_overwrite(static_cast<std::size_t>(extent));
                if(!read_all(0, carrier.writable())) { return 127; }
                p::invocation request;
                if(!p::decode(carrier.view(), request)) { return 128; }
                request.weights={weights.data(),static_cast<std::size_t>(weight_count)};
                context.request=request.request;
                auto succeeded = backend(request, emit, &context);
                if(context.failed) { return 129; }
                if(!context.terminal && !emit(&context, succeeded ? p::response::succeeded : p::response::failed, {})) { return 130; }
            } // all request bytes and synchronous backend use have ended
            std::array<std::uint8_t,32> result_header{};
            s::wire::writer result_writer{result_header};
            auto body=context.terminal_body.view();
            if(!result_writer.u64(body.size()+24) || !result_writer.u64(static_cast<std::uint64_t>(context.terminal_kind)) ||
               !result_writer.u64(context.request) || !result_writer.u64(body.size()) || !write_all(1,result_header) || !write_all(1,body)) { return 141; }
        }
        catch(...)
        {
            return 131;
        }
    }
}
