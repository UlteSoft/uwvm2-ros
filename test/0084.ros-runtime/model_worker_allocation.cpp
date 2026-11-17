/** @file @brief Actual recoverable allocator failures before worker ownership publication. */
#define UWVM_MODEL_WORKER_NO_MAIN
#include "model_worker.cpp"
#include <dirent.h>
static std::atomic<std::size_t> fail_after{};
static std::atomic<bool> failed{};
static bool reject() noexcept
{
    auto current=fail_after.load(std::memory_order_relaxed);
    while(current)
    {
        if(fail_after.compare_exchange_weak(current,current-1,std::memory_order_relaxed))
        {if(current==1){failed.store(true,std::memory_order_relaxed);return true;}break;}
    }
    return false;
}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t bytes){return reject()?nullptr:__real_malloc(bytes);}
extern "C" void* __wrap_calloc(std::size_t count,std::size_t bytes){return reject()?nullptr:__real_calloc(count,bytes);}
extern "C" void* __wrap_realloc(void* value,std::size_t bytes){return reject()?nullptr:__real_realloc(value,bytes);}
static std::size_t descriptors()
{
    auto directory=::opendir("/proc/self/fd");assert(directory);std::size_t count{};
    while(auto entry=::readdir(directory)){if(entry->d_name[0]!='.')++count;}
    assert(!::closedir(directory));return count;
}
int main(int argc,char** argv)
{
    assert(argc==10);uwvm2::utils::container::std_vector<x::descriptor> ownership;auto manifest=worker_manifest(argv+1,ownership);
    manifest.model=model_plan().model;manifest.task=model_plan().task;
    std::size_t failures{};bool complete{};
    for(std::size_t point=1;point<=256;++point)
    {
        auto authority=uwvm2::utils::container::make_shared<s::authority>();s::buffers::registry buffers;s::model_dispatch dispatcher{authority,buffers};
        assert(dispatcher.bind("brain",model_plan())==error::success);std::shared_ptr<s::models::provider_lifetime> worker;
        auto before=descriptors();fail_after=point;failed=false;
        auto status=s::model_worker::service::prepare(dispatcher,"brain",1,manifest,worker);
        auto injected=failed.load();fail_after=0;
        if(injected){++failures;assert(status!=error::success&&!worker);}
        else {assert(status==error::success&&worker);worker->retire();worker.reset();complete=true;}
        assert(descriptors()==before);
        if(complete)break;
    }
    assert(complete&&failures>=8);
    std::printf("passed worker allocation rollback at %zu real failure points\n",failures);
}
