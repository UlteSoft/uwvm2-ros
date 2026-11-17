/** @file @brief Recoverable fast_io allocation rollback before process publication. */
#define UWVM_COMMUNICATION_SANDBOX_NO_MAIN
#include "communication_worker_sandbox.cpp"
#include <dirent.h>
static std::atomic<std::size_t> fail_after{};static std::atomic<bool> failed{};
static bool reject()noexcept
{
    auto current=fail_after.load();while(current){if(fail_after.compare_exchange_weak(current,current-1)){if(current==1){failed=true;return true;}break;}}return false;
}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return reject()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t size){return reject()?nullptr:__real_calloc(n,size);}
extern "C" void* __wrap_realloc(void* p,std::size_t size){return reject()?nullptr:__real_realloc(p,size);}
static std::size_t descriptors()
{
    auto directory=::opendir("/proc/self/fd");assert(directory);std::size_t count{};
    while(auto entry=::readdir(directory)){if(entry->d_name[0]!='.')++count;}assert(!::closedir(directory));return count;
}
int main(int argc,char** argv)
{
    assert(argc==9);::signal(SIGPIPE,SIG_IGN);uwvm2::utils::container::std_vector<x::descriptor> ownership;auto manifest=make_manifest(argv+1,ownership);
    std::size_t preparation_failures{},launch_failures{};bool complete{};
    for(std::size_t point=1;point<=512;++point)
    {
        auto before=descriptors();bool injected{};
        {
            x::process child;fail_after=point;failed=false;error status=error::io;
            try{status=child.prepare(manifest);}catch(std::bad_alloc const&){}
            injected=failed.load();fail_after=0;assert(child.process_fd()<0);
            if(injected){++preparation_failures;assert(status!=error::success);}else{assert(status==error::success);complete=true;}
        }
        assert(descriptors()==before);if(complete)break;
    }
    assert(complete&&preparation_failures>=8);complete=false;
    for(std::size_t point=1;point<=32;++point)
    {
        auto before=descriptors();
        {
            x::process child;assert(child.prepare(manifest)==error::success);auto prepared_count=descriptors();fail_after=point;failed=false;error status=error::io;
            try{status=child.launch();}catch(std::bad_alloc const&){}
            auto injected=failed.load();fail_after=0;
            if(injected){++launch_failures;assert(status!=error::success&&child.process_fd()<0&&descriptors()==prepared_count);}
            else{assert(status==error::success);reaped(child);complete=true;}
        }
        assert(descriptors()==before);if(complete)break;
    }
    assert(complete&&launch_failures>=3);
    std::printf("PASS recoverable communication allocation rollback: prepare=%zu launch=%zu real failure points, no FD leaks\n",preparation_failures,launch_failures);
}
