/** @file @brief Real nullable fast_io ROS C allocations, capacity and realloc rollback. */
#include <uwvm2/imported/ros/scene/native/communication_allocator.h>
#include <cassert>
#include <cstdio>
#include <atomic>
static std::atomic<bool> fail{};
extern "C" void* __real_malloc(std::size_t);extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return fail.exchange(false)?nullptr:__real_malloc(n);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return fail.exchange(false)?nullptr:__real_realloc(p,n);}
int main()
{
    using allocator=uwvm2::imported::ros::scene::communication::ros2::query_allocator;
    allocator zero{0};auto disabled=zero.native();assert(!disabled.allocate(1,disabled.state)&&zero.exhausted()&&!zero.used());
    allocator bounded{128};auto native=bounded.native();auto* value=static_cast<unsigned char*>(native.zero_allocate(2,16,native.state));assert(value&&bounded.used()>=32);
    assert(reinterpret_cast<std::uintptr_t>(value)%alignof(std::max_align_t)==0);for(unsigned i{};i<32;++i)assert(value[i]==0);
    value[0]=71;auto prior=bounded.used();assert(!native.reallocate(value,1024,native.state)&&bounded.used()==prior&&value[0]==71&&bounded.exhausted());
    value=static_cast<unsigned char*>(native.reallocate(value,8,native.state));assert(value&&value[0]==71&&bounded.used()<prior);
    fail=true;prior=bounded.used();assert(!native.reallocate(value,48,native.state)&&bounded.used()==prior&&value[0]==71&&!fail);
    native.deallocate(value,native.state);assert(!bounded.used());
    fail=true;assert(!native.allocate(16,native.state)&&!bounded.used()&&!fail);
    assert(!native.zero_allocate(SIZE_MAX,2,native.state)&&!bounded.used());
    value=static_cast<unsigned char*>(native.allocate(0,native.state));assert(value&&bounded.used());native.deallocate(value,native.state);assert(!bounded.used());
    native.deallocate(nullptr,native.state);assert(!bounded.used());
    std::puts("passed recoverable bounded ROS C allocator");
}
