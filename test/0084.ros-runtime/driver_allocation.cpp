/** @file @brief Inject real fast_io allocation failures before driver registration publication. */
#include <uwvm2/imported/ros/scene/driver_dispatch.h>
#include <cassert>
#include <cstdio>
#include <dirent.h>
namespace s=uwvm2::imported::ros::scene;
namespace d=s::drivers;
static std::size_t fail_after{};static bool failed{};
static bool reject()noexcept{if(fail_after&&!--fail_after){failed=true;return true;}return false;}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t bytes){return reject()?nullptr:__real_malloc(bytes);}
extern "C" void* __wrap_calloc(std::size_t count,std::size_t bytes){return reject()?nullptr:__real_calloc(count,bytes);}
extern "C" void* __wrap_realloc(void* value,std::size_t bytes){return reject()?nullptr:__real_realloc(value,bytes);}
static std::size_t descriptors()
{auto* dir=::opendir("/proc/self/fd");assert(dir);std::size_t count{};while(auto* entry=::readdir(dir))if(entry->d_name[0]!='.')++count;assert(!::closedir(dir));return count;}
int main()
{
 int fds[2];assert(!::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,fds));int one=1;
 assert(!::setsockopt(fds[0],SOL_SOCKET,SO_PASSCRED,&one,sizeof(one)));
 auto now=static_cast<std::uint64_t>(uwvm2::imported::ros::resources::steady_now());
 d::protocol::binding identity{{1,2},{3,4},{5,6},{7,8},{9,10},{11,12,13,14}};
 d::credentials peer{static_cast<std::uint64_t>(::getpid()),::getuid(),::getgid()};
 std::size_t failures{};bool complete{};
 for(std::size_t point=1;point<=32;++point)
 {
  auto before=descriptors();
  {
   s::driver_dispatch dispatch;assert(dispatch.budget(1,65536)==s::error::success);
   fail_after=point;failed=false;auto status=s::error::no_memory;
   try{status=dispatch.bind("driver-allocation-rollback","main","arm-allocation-rollback",fds[0],peer,identity,{4096,4096,1000000000,now+60000000000});}
   catch(std::bad_alloc const&){}
   bool injected=failed;fail_after=0;
   if(injected)
   {
    ++failures;assert(status==s::error::no_memory&&!dispatch.contains("driver-allocation-rollback")&&descriptors()==before);
    assert(dispatch.bind("driver-allocation-rollback","main","arm-allocation-rollback",fds[0],peer,identity,{4096,4096,1000000000,now+60000000000})==s::error::success);
   }
   else{assert(status==s::error::success);complete=true;}
   assert(dispatch.schema("driver-allocation-rollback","parameter.exists",{0,64,64,8})==s::error::success);
   dispatch.freeze();dispatch.retire();
  }
  assert(descriptors()==before);if(complete)break;
 }
 assert(complete&&failures>=5);std::printf("PASS driver registration rollback at %zu recoverable allocations, no descriptor leaks\n",failures);
 ::close(fds[0]);::close(fds[1]);
}
