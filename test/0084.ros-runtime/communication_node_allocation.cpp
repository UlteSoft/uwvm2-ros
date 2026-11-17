/** @file @brief Exhaust every recoverable node-open allocation before creation admission. */
#define UWVM_COMMUNICATION_NODE_OPEN_FIXTURE_ONLY
#include "communication_node_open.cpp"
static std::atomic<std::size_t> fail_after{};
static std::atomic<bool> failed{};
static bool reject()noexcept
{
    auto current=fail_after.load();
    while(current)if(fail_after.compare_exchange_weak(current,current-1))
    {if(current==1){failed=true;return true;}break;}
    return false;
}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t bytes){return reject()?nullptr:__real_malloc(bytes);}
extern "C" void* __wrap_calloc(std::size_t count,std::size_t bytes){return reject()?nullptr:__real_calloc(count,bytes);}
extern "C" void* __wrap_realloc(void* value,std::size_t bytes){return reject()?nullptr:__real_realloc(value,bytes);}
static std::uint64_t admissions(pipeline& f)
{
    uwvm2::utils::container::std_vector<s::authority_snapshot> rows;assert(f.authority->inspect(rows,8));
    for(auto const& row:rows)if(row.alias=="node")return row.admissions;
    assert(false);return 0;
}
int main()
{
    bool complete{};unsigned failures{};
    for(std::size_t point{1};point<128;++point)
    {
        pipeline f;fail_after=point;failed=false;auto status=f.open();bool injected=failed.load();fail_after=0;
        assert(!f.native->stage_calls&&!f.native->node_creations&&!f.worker->stopped);
        if(injected)
        {++failures;assert(status==s::error::no_memory&&!f.request&&admissions(f)==0);assert(f.open()==s::error::success);}
        else{assert(status==s::error::success);complete=true;}
        assert(f.request&&admissions(f)==1&&f.open()==s::error::success&&admissions(f)==1);
        if(complete)break;
    }
    assert(complete&&failures);std::printf("recoverable node creation allocations: %u\n",failures);
    std::puts("PASS node creation recoverable allocation: each allocation fails before debit/startup, leaves no blocked draft and permits exact retry");
}
