/** @file @brief Actual sealed-image bubblewrap CPU inference and denial probes. */
#include "support/legacy_process/model_sandbox.h"
#include <uwvm2/imported/ros/scene/model_worker_protocol.h>
#include <cassert>
#include <cstdio>
#include <cstring>
namespace x = uwvm2::imported::ros::scene::model_sandbox;
namespace w = uwvm2::imported::ros::scene::wire;
namespace p = uwvm2::imported::ros::scene::model_worker;
using x::error;

static x::image_spec image(char const* path, std::string_view destination, x::image_role role, uwvm2::utils::container::std_vector<x::descriptor>& ownership)
{
    x::descriptor file{::open(path, O_RDONLY | O_CLOEXEC)};
    assert(file);
    struct stat info{};
    assert(!::fstat(file.get(), &info) && info.st_size >= 0);
    ::fast_io::sha256_context hashing{};
    std::array<std::byte, 65536> bytes{};
    std::uint64_t offset{};
    while(offset < static_cast<std::uint64_t>(info.st_size))
    {
        auto count = ::pread(file.get(), bytes.data(), bytes.size(), offset);
        assert(count > 0);
        hashing.update(bytes.data(), bytes.data() + count);
        offset += count;
    }
    x::image_spec value;
    value.source = file.get();
    value.bytes = offset;
    value.role = role;
    assert(value.destination.assign(destination));
    hashing.do_final();
    hashing.digest_to_byte_ptr(reinterpret_cast<std::byte*>(value.sha256.data()));
    ownership.emplace_back(std::move(file));
    return value;
}

static bool transfer(x::process& child, int fd, std::span<std::uint8_t> bytes, bool writing)
{
    std::size_t offset{};
    auto deadline = uwvm2::imported::ros::resources::steady_now() + 5000000000ll;
    while(offset < bytes.size() && uwvm2::imported::ros::resources::steady_now() < deadline)
    {
        auto count = writing ? ::write(fd, bytes.data() + offset, bytes.size() - offset) : ::read(fd, bytes.data() + offset, bytes.size() - offset);
        if(count > 0)
        {
            offset += count;
            continue;
        }
        if(count == 0)
        {
            std::array<char,4096> errors{};auto size=::read(child.stderr_fd(),errors.data(),errors.size());if(size>0)assert(::write(2,errors.data(),size)==size);
            int status{};auto reaped=child.reap(status);std::fprintf(stderr,"early worker EOF: reap=%u status=%d\n",static_cast<unsigned>(reaped),status);return false;
        }
        if(errno != EINTR && errno != EAGAIN) { return false; }
        std::array<char, 4096> errors{};
        auto stderr_count = ::read(child.stderr_fd(), errors.data(), errors.size());
        if(stderr_count > 0) { assert(::write(2, errors.data(), stderr_count)==stderr_count); }
        struct pollfd wait{fd, static_cast<short>(writing ? POLLOUT : POLLIN), 0};
        ::poll(&wait, 1, 10);
    }
    return offset == bytes.size();
}

static p::response invoke(x::process& child, std::uint64_t mode, std::span<std::uint8_t> output, std::size_t& size)
{
    std::array<std::uint8_t, 256> body{};
    w::writer writer{body};
    std::array<std::uint8_t, 8> parameters{};
    w::writer encoded_params{parameters};
    assert(encoded_params.u64(mode));
    std::array<std::uint8_t, 16> input{};
    w::writer encoded_input{input};
    assert(encoded_input.f64(5) && encoded_input.f64(6));
    assert(writer.u64(mode) && writer.blob(parameters) && writer.u64(1) && writer.blob({}) && writer.blob(input));
    std::array<std::uint8_t, 8> header{};
    w::writer prefix{header};
    assert(prefix.u64(writer.size()));
    assert(transfer(child, child.input_fd(), header, true) && transfer(child, child.input_fd(), {body.data(), writer.size()}, true));
    assert(transfer(child, child.output_fd(), header, false));
    w::reader response_size{header};
    std::uint64_t count{};
    assert(response_size.u64(count) && count <= output.size());
    size = count;
    assert(transfer(child, child.output_fd(), output.first(size), false));
    p::response kind{};
    std::uint64_t request{};
    w::bytes bytes;
    assert(p::decode_response(output.first(size), kind, request, bytes) && request == mode);
    return kind;
}

int main(int argc, char** argv)
{
    assert(argc == 10);
    ::signal(SIGPIPE, SIG_IGN);
    uwvm2::utils::container::std_vector<x::descriptor> descriptors;
    x::manifest manifest;
    manifest.launcher = image(argv[9], "/worker/launcher", x::image_role::runtime, descriptors);
    manifest.files.push_back(image(argv[1], "/worker/bootstrap", x::image_role::bootstrap, descriptors));
    manifest.files.push_back(image(argv[2], "/worker/backend.so", x::image_role::runtime, descriptors));
    manifest.files.push_back(image(argv[3], "/model/weights", x::image_role::weights, descriptors));
    manifest.files.push_back(image(argv[4], "/lib64/ld-linux-x86-64.so.2", x::image_role::runtime, descriptors));
    manifest.files.push_back(image(argv[5], "/lib/x86_64-linux-gnu/libc.so.6", x::image_role::runtime, descriptors));
    manifest.files.push_back(image(argv[6], "/lib/x86_64-linux-gnu/libstdc++.so.6", x::image_role::runtime, descriptors));
    manifest.files.push_back(image(argv[7], "/lib/x86_64-linux-gnu/libgcc_s.so.1", x::image_role::runtime, descriptors));
    manifest.files.push_back(image(argv[8], "/lib/x86_64-linux-gnu/libm.so.6", x::image_role::runtime, descriptors));
    manifest.bound = {64 * 1024 * 1024, 256 * 1024 * 1024, 5, 4096, 128, 65536, 1, 4096, 4096, 4096, 5000000000ull, 10000000,1024*1024};
    namespace placement=uwvm2::imported::ros::scene::execution_placement;
    placement::resolved parent;assert(parent.prepare({})==error::success);std::size_t first_cpu=SIZE_MAX;
    for(std::size_t i{};i<parent.cpus().size()*placement::word_bits;++i)
        if(parent.cpus()[i/placement::word_bits]&(placement::word{1}<<(i%placement::word_bits))){first_cpu=i;break;}
    assert(first_cpu!=SIZE_MAX);
    assert(placement::parse(uwvm2::utils::container::to_string(first_cpu),"inherit","none",manifest.placement)==error::success);
    assert(x::valid(manifest));
    x::process child;
    auto prepared = child.prepare(manifest);
    std::fprintf(stderr, "prepare=%u\n", static_cast<unsigned>(prepared));
    assert(prepared == error::success);
    assert(child.launch() == error::success);
    auto identity=error::again;
    for(unsigned attempts{};attempts<500 && identity==error::again;++attempts) { identity=child.identify(); if(identity==error::again) { ::poll(nullptr,0,10); } }
    if(identity!=error::success) { std::array<char,4096> errors{};auto n=::read(child.stderr_fd(),errors.data(),errors.size());if(n>0)assert(::write(2,errors.data(),n)==n);std::fprintf(stderr,"identity=%u\n",static_cast<unsigned>(identity)); }
    assert(identity==error::success);
    std::array<std::uint8_t, 64> ready{};
    assert(transfer(child, child.output_fd(), {ready.data(), p::ready.size()}, false));
    assert((std::string_view{reinterpret_cast<char const*>(ready.data()), p::ready.size()} == p::ready));
    std::array<std::uint8_t, 4096> response{};
    std::size_t length{};
    pollfd waiting{child.output_fd(),POLLIN,0};
    assert(::poll(&waiting,1,20)==0);  // bootstrap cannot continue without actual descriptor transfer
    assert(child.enable_weights() == error::success);
    assert(transfer(child,child.output_fd(),{ready.data(),p::attached.size()},false));
    assert((std::string_view{reinterpret_cast<char const*>(ready.data()),p::attached.size()}==p::attached));
    assert(invoke(child, 1, response, length) == p::response::succeeded);
    p::response kind;
    std::uint64_t id;
    w::bytes result;
    assert(p::decode_response({response.data(), length}, kind, id, result));
    w::reader numbers{result};
    double first{}, second{};
    assert(numbers.f64(first) && numbers.f64(second) && numbers.done() && first == 17 && second == 39);
    assert(invoke(child, 2, response, length) == p::response::succeeded && p::decode_response({response.data(), length}, kind, id, result));
    w::reader probes{result};
    std::uint64_t mask{};
    assert(probes.u64(mask) && mask == 1023);
    assert(invoke(child,6,response,length)==p::response::succeeded&&p::decode_response({response.data(),length},kind,id,result));
    w::reader topology{result};std::uint64_t cpus{},cpu{},blocked{};
    assert(topology.u64(cpus)&&topology.u64(cpu)&&topology.u64(blocked)&&topology.done());
    std::printf("worker CPU count=%llu selected=%llu forbidden-placement-syscalls=%llu\n",static_cast<unsigned long long>(cpus),static_cast<unsigned long long>(cpu),static_cast<unsigned long long>(blocked));
    assert(cpus==1&&cpu==first_cpu&&blocked==3);
    child.request_stop();
    int status{};
    while(child.reap(status) == error::again)
    {
        struct pollfd stopped{child.process_fd(), POLLIN, 0};
        ::poll(&stopped, 1, 100);
    }
    assert(WIFSIGNALED(status) && WTERMSIG(status) == SIGKILL);
    std::puts("passed actual isolated model inference and 12 syscall/file/placement probes");
}
