/** @file @brief Exercise actual recoverable fast_io allocation failures in placement preparation. */
#define main placement_fixture_main
#include "execution_placement.cpp"
#undef main
static std::size_t fail_after{};static bool failed{};
static bool reject()noexcept{if(fail_after&&!--fail_after){failed=true;return true;}return false;}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return reject()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t bytes){return reject()?nullptr:__real_calloc(n,bytes);}
extern "C" void* __wrap_realloc(void* old,std::size_t n){return reject()?nullptr:__real_realloc(old,n);}
int main(int argc,char** argv)
{
    if(argc!=1)return placement_fixture_main(argc,argv);
    p::policy before;assert(p::parse("0","inherit","none",before)==error::success);
    fail_after=1;failed=false;auto parsed=p::parse("0-1025","bind","0-1",before);auto injected=failed;fail_after=0;
    assert(injected&&parsed==error::no_memory&&before.cpus.size()==1&&before.cpus[0]==1&&before.nodes.empty());
    std::size_t failures{};bool complete{};
    for(std::size_t point{1};point<=64;++point)
    {
        p::resolved selected;fail_after=point;failed=false;auto status=selected.prepare({});injected=failed;fail_after=0;
        if(injected)
        {++failures;assert(status==error::no_memory&&!selected.apply()&&selected.cpus().empty());assert(selected.prepare({})==error::success);}
        else{assert(status==error::success);complete=true;break;}
    }
    assert(complete&&failures>=2);assert(placement_fixture_main(argc,argv)==0);
    std::printf("PASS placement allocation rollback: %zu recoverable failure points\n",failures);
}
