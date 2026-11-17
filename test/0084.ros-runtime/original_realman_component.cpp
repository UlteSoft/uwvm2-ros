/** @file
 * @brief Exercise an unchanged vendor library only inside the component sandbox.
 * @details The original RealMan binary and its headers are copied from a
 * digest-pinned upstream archive by the test runner. This fixture links the
 * vendor's documented metadata query; it neither initializes robot transport
 * nor requests motion. Deliberate faults belong to this external test adapter,
 * not to the unmodified vendor library. No vendor code is linked into the VM.
 */
#include <uwvm2/imported/ros/scene/component_protocol.h>
#include <rm_interface.h>
#include <cerrno>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <unistd.h>

namespace original_driver
{
    static bool metadata()noexcept
    {
        char const* version=::rm_api_version();
        if(!version)return false;
        auto length=::strnlen(version,64);
        if(!length||length==64)return false;
        for(std::size_t i{};i<length;++i)if(version[i]<0x20||version[i]>0x7e)return false;
        return true;
    }
    [[maybe_unused]] static void crash()noexcept
    {
        auto page=::mmap(nullptr,4096,PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
        if(page!=MAP_FAILED)*static_cast<volatile unsigned char*>(page)=1;
        ::_exit(137);
    }
    static bool confined()noexcept
    {
        errno=0;auto socket=::socket(AF_INET,SOCK_STREAM,0);
        if(socket!=-1||errno!=ENOSYS){if(socket>=0)::close(socket);return false;}
        errno=0;auto process=::syscall(SYS_fork);
        if(process!=-1||errno!=ENOSYS){if(process==0)::_exit(138);return false;}
        errno=0;auto mapping=::mmap(nullptr,4096,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
        if(mapping!=MAP_FAILED||errno!=ENOSYS){if(mapping!=MAP_FAILED)::munmap(mapping,4096);return false;}
        char* argv[]={const_cast<char*>("/worker/bootstrap"),nullptr};char* environment[]={nullptr};
        errno=0;if(::execve(argv[0],argv,environment)!=-1||errno!=ENOSYS)return false;
        auto descriptor=::open("/etc/passwd",O_RDONLY|O_CLOEXEC);
        if(descriptor!=-1){::close(descriptor);return false;}
        return true;
    }
}
#if defined(UWVM_COMPONENT_CONSTRUCTOR_FAULT_FIXTURE)
__attribute__((constructor)) static void constructor_fault()noexcept
{if(!original_driver::metadata())::_exit(139);original_driver::crash();}
#endif
namespace p=uwvm2::imported::ros::scene::component_protocol;
extern "C" bool uwvm_component_run(p::invocation const& invocation,p::call_function,void*)noexcept
{
    if(invocation.count||invocation.configuration.size()!=1||invocation.configuration[0]!=42||
       !original_driver::metadata()||!original_driver::confined())return false;
#if defined(UWVM_COMPONENT_FAULT_FIXTURE)
    original_driver::crash();return false;
#elif defined(UWVM_COMPONENT_COMPLETION_FIXTURE)
    return true;
#else
    for(;;){timespec wait{0,1000000};::nanosleep(&wait,nullptr);}
#endif
}
