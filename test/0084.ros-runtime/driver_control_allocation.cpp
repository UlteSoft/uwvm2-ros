/** @file @brief Recoverable receiver registry allocation; no partially published principal. */
#include <uwvm2/imported/ros/scene/driver_control.h>
#include <cassert>
#include <cstdio>
namespace d=uwvm2::imported::ros::scene::drivers;namespace c=uwvm2::imported::ros::scene::control;
static bool fail{},injected{};
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
static bool reject(){if(fail){fail=false;injected=true;return true;}return false;}
extern "C" void* __wrap_malloc(std::size_t n){return reject()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t s){return reject()?nullptr:__real_calloc(n,s);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return reject()?nullptr:__real_realloc(p,n);}
int main()
{
 d::receiver_control gate;d::protocol::binding binding{{1,0},{2,0},{3,0},{4,0},{5,0},{1,2,3,4}};c::handle id=999;
 assert(gate.configure(binding.deployment,binding.driver,binding.contract,1,1,{1000,100,1,false})==d::error::success);
 fail=true;auto result=gate.bind(binding,{1,1000,1000},5000,1000,id);fail=false;
 assert(injected&&result==d::error::no_memory&&id==999);
 assert(gate.bind(binding,{1,1000,1000},5000,1000,id)==d::error::success&&id!=999);
 assert(gate.freeze()==d::error::success);c::lease owner;assert(gate.grant(id,1,500,1000,owner)==d::error::success);
 std::puts("PASS recoverable receiver allocation, no partial authority publication");
}
