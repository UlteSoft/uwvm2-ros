/** @file @brief Isolated lifecycle fixture: immutable configuration, no roles, explicit host stop. */
#include <uwvm2/imported/ros/scene/component_protocol.h>
#include <ctime>
#if defined(UWVM_COMPONENT_FAULT_FIXTURE) || defined(UWVM_COMPONENT_CONSTRUCTOR_FAULT_FIXTURE)
#include <sys/mman.h>
static void fault_page()noexcept
{
    auto page=::mmap(nullptr,4096,PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
    if(page!=MAP_FAILED)*static_cast<volatile unsigned char*>(page)=1;
}
#endif
#if defined(UWVM_COMPONENT_CONSTRUCTOR_FAULT_FIXTURE)
// Exercise code running inside dlopen, before the protocol-ready handshake.
__attribute__((constructor)) static void constructor_fault()noexcept{fault_page();}
#endif
#if defined(UWVM_COMPONENT_UNANNOUNCED_EXIT_FIXTURE) || defined(UWVM_COMPONENT_MALFORMED_FRAME_FIXTURE)
#include <unistd.h>
#endif
namespace p=uwvm2::imported::ros::scene::component_protocol;
extern "C" bool uwvm_component_run(p::invocation const& invocation,p::call_function,void*)noexcept
{
    if(invocation.count||invocation.configuration.size()!=1||invocation.configuration[0]!=42)return false;
#if defined(UWVM_COMPONENT_FAULT_FIXTURE)
    fault_page();return false;
#elif defined(UWVM_COMPONENT_UNANNOUNCED_EXIT_FIXTURE)
    // No finished frame: even an OS exit code of zero must retain failure.
    ::_exit(0);
#elif defined(UWVM_COMPONENT_REPORTED_FAILURE_FIXTURE)
    return false;
#elif defined(UWVM_COMPONENT_COMPLETION_FIXTURE)
    return true;
#elif defined(UWVM_COMPONENT_MALFORMED_FRAME_FIXTURE)
    // Unknown frame kind after a valid start, not an inferred native crash.
    std::array<std::uint8_t,24> bytes{};bytes[0]=16;bytes[8]=255;
    if(::write(1,bytes.data(),bytes.size())!=static_cast<ssize_t>(bytes.size()))::_exit(1);
    for(;;){timespec wait{0,1000000};::nanosleep(&wait,nullptr);}
#else
    for(;;){timespec wait{0,1000000};::nanosleep(&wait,nullptr);}
#endif
}
