/** @file @brief Real bounded 2x2 CPU model and in-sandbox syscall probes. */
#include <uwvm2/imported/ros/scene/model_worker_protocol.h>
#include <array>
#include <cerrno>
#include <cstdint>
#include <fcntl.h>
#include <linux/mempolicy.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>
namespace p = uwvm2::imported::ros::scene::model_worker;
namespace w = uwvm2::imported::ros::scene::wire;

extern "C" bool uwvm_model_compute(p::invocation const& request, p::emit_function emit, void* context) noexcept
{
    w::reader params{request.parameters};
    std::uint64_t mode{};
    if(!params.u64(mode) || !params.done()) { return false; }
    auto weight_fd = [&]() noexcept { for(auto const& value:request.weights) { if(value.identity()=="/model/weights") { return value.descriptor; } } return -1; };
    if(mode == 1)
    {
        auto fd = weight_fd();
        if(fd < 0) { return false; }
        std::array<std::uint8_t, 32> encoded{};
        auto count = ::pread(fd, encoded.data(), encoded.size(),0);
        if(count != 32 || request.input_count != 1 || request.inputs[0].data.size() != 16) { return false; }
        std::array<double, 4> weights{};
        w::reader values{encoded};
        for(auto& value: weights)
        {
            if(!values.f64(value)) { return false; }
        }
        std::array<double, 2> input{};
        w::reader input_reader{request.inputs[0].data};
        for(auto& value: input)
        {
            if(!input_reader.f64(value)) { return false; }
        }
        std::array<std::uint8_t, 16> output{};
        w::writer writer{output};
        if(!writer.f64(weights[0] * input[0] + weights[1] * input[1]) || !writer.f64(weights[2] * input[0] + weights[3] * input[1])) { return false; }
        return emit(context, p::response::succeeded, output);
    }
    if(mode == 2)
    {
        std::uint64_t denied{};
        auto socket = ::socket(AF_INET, SOCK_STREAM, 0);
        if(socket < 0) { denied |= 1; }
        else
        {
            ::close(socket);
        }
        auto host = ::open("/etc/passwd", O_RDONLY);
        if(host < 0) { denied |= 2; }
        else
        {
            ::close(host);
        }
        auto created = ::open("/scratch/created", O_CREAT | O_RDWR, 0600);
        if(created < 0) { denied |= 4; }
        else
        {
            ::close(created);
        }
        auto weights = weight_fd();
        if(weights < 0) { return false; }
        if(::fchmod(weights, 0700) < 0) { denied |= 8; }
        auto mapped = ::mmap(nullptr, 4096, PROT_READ | PROT_EXEC, MAP_PRIVATE, weights, 0);
        if(mapped == MAP_FAILED) { denied |= 16; }
        else
        {
            ::munmap(mapped, 4096);
        }

        auto writable = ::mmap(nullptr, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if(writable == MAP_FAILED) { return false; }
        if(::mprotect(writable, 4096, PROT_READ | PROT_EXEC) < 0) { denied |= 32; }
        ::munmap(writable, 4096);
        char* const arguments[]{const_cast<char*>("/worker/bootstrap"), nullptr};
        char* const environment[]{nullptr};
        if(::execve(arguments[0], arguments, environment) < 0) { denied |= 64; }
        if(::syscall(SYS_memfd_create, "unapproved", 0) < 0) { denied |= 128; }
        auto path = ::open("/worker/backend.so", O_WRONLY);
        if(path < 0) { denied |= 256; }
        else
        {
            ::close(path);
        }
        auto scratch = ::open("/scratch/slot-0", O_RDWR);
        if(scratch < 0) { return false; }
        std::uint8_t byte{42};
        if(::write(scratch, &byte, 1) == 1) { denied |= 512; }
        ::close(scratch);
        std::array<std::uint8_t, 8> output{};
        w::writer writer{output};
        if(!writer.u64(denied)) { return false; }
        return emit(context, p::response::succeeded, output);
    }
    if(mode == 3)
    {
        for(;;) {};
    }  // actual CPU/deadline termination fixture
    if(mode == 4)
    {
        std::array<std::uint8_t, 4096> bytes{};
        for(;;)
        {
            if(::write(2, bytes.data(), bytes.size()) < 0) { return false; }
        }
    }
    if(mode == 5)
    {
        std::array<std::uint8_t, 8> malformed{};
        malformed.fill(255);
        return ::write(1, malformed.data(), malformed.size()) == 8;
    }
    if(mode == 6)
    {
        std::array<unsigned long,256> cpus{};
        if(::syscall(SYS_sched_getaffinity,0,sizeof(cpus),cpus.data())<0)return false;
        std::uint64_t count{},first=UINT64_MAX,denied{};
        for(std::size_t i{};i<sizeof(cpus)*8;++i)if(cpus[i/(sizeof(unsigned long)*8)]&(1ul<<(i%(sizeof(unsigned long)*8))))
        {++count;if(first==UINT64_MAX)first=i;}
        // Even an unchanged placement request must be unavailable to provider code.
        if(::syscall(SYS_sched_setaffinity,0,sizeof(cpus),cpus.data())<0&&errno==ENOSYS)denied|=1;
        if(::syscall(SYS_set_mempolicy,MPOL_DEFAULT,nullptr,0)<0&&errno==ENOSYS)denied|=2;
        std::array<std::uint8_t,24> output{};w::writer writer{output};
        return writer.u64(count)&&writer.u64(first)&&writer.u64(denied)&&emit(context,p::response::succeeded,output);
    }
    return false;
}
