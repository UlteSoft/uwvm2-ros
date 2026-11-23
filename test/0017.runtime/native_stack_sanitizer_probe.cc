// ASan fake-stack storage must not be mistaken for an actual pthread stack.
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/runtime/lib/uwvm_runtime_debug_native_stack.h>
#include <cstdio>
#include <pthread.h>

namespace ns=::uwvm2::runtime::lib::native_stack;
namespace ds=::uwvm2::runtime::lib::details::debug_native_stack;

[[gnu::noinline]] static bool check()
{
    ds::owner retired;
    auto const frame=reinterpret_cast<::std::uintptr_t>(__builtin_frame_address(0));
    auto const tid=static_cast<::std::uint_least64_t>(::fast_io::system_call<__NR_gettid,long>());
    {
        ns::scope actual;
        if(!actual.ready())return false;
        auto const* bounds=ns::active_bounds.load();
        if(!bounds||frame<bounds->low||frame>=bounds->high)return false;
        ns::scope nested;
        if(!nested.ready()||ns::active_bounds.load()!=bounds)return false;
        ds::scope registration;
        retired=registration.pin();
        if(!retired||!retired->contains_frame(tid,frame,frame)||retired->contains_frame(tid+1u,frame,frame))return false;
        // In ASan builds this automatic scope may live outside the real stack;
        // it still cannot authorize reading that separate allocation as a frame.
        auto const candidate=reinterpret_cast<::std::uintptr_t>(&actual);
        if((candidate<bounds->low||candidate>=bounds->high)&&retired->contains_frame(tid,candidate,candidate))return false;
    }
    return !retired->contains_frame(tid,frame,frame)&&ns::active_bounds.load()==nullptr;
}

int main()
{
    if(!check())return 1;
    auto const page=static_cast<::std::size_t>(::sysconf(_SC_PAGESIZE));
    auto const bytes=2u*1024u*1024u+2u*page;
    auto* mapping=static_cast<::std::byte*>(::mmap(nullptr,bytes,PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0));
    if(mapping==MAP_FAILED)return 2;
    if(::mprotect(mapping+page,bytes-2u*page,PROT_READ|PROT_WRITE))return 3;
    ::pthread_attr_t attributes{};::pthread_t worker{};bool passed{};
    if(::pthread_attr_init(&attributes)||::pthread_attr_setstack(&attributes,mapping+page,bytes-2u*page))return 4;
    if(::pthread_create(&worker,&attributes,+[](void* result)->void*
    { *static_cast<bool*>(result)=check();return nullptr; },&passed))return 5;
    if(::pthread_attr_destroy(&attributes)||::pthread_join(worker,nullptr))return 6;
    if(::munmap(mapping,bytes)||!passed)return 7;
    ::std::puts("native-stack sanitizer probe: main, guarded worker, nested scope and retired owner passed");
}
