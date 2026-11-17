/** @file @brief Real sealed startup with caller-bound nonblocking stage/cleanup evidence. */
#include "communication_attachment_fixture.h"
#include <sys/wait.h>
#include <uwvm2/imported/ros/scene/communication_worker_stage.h>
namespace c=s::communication;
namespace w=s::communication_worker;
namespace resources=uwvm2::imported::ros::resources;
using stage=w::node_stage;
#if defined(UWVM_WORKER_STAGE_ALLOCATION)
static std::atomic<std::size_t> fail_after{};static std::atomic<bool> injected{};
static bool reject()noexcept
{auto n=fail_after.load();while(n){if(fail_after.compare_exchange_weak(n,n-1)){if(n==1){injected=true;return true;}break;}}return false;}
extern "C" void* __real_malloc(std::size_t);extern "C" void* __real_calloc(std::size_t,std::size_t);extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return reject()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t a,std::size_t b){return reject()?nullptr:__real_calloc(a,b);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return reject()?nullptr:__real_realloc(p,n);}
#endif
struct retained_context final:c::native_context
{
    std::atomic<bool> active{true};bool live()const noexcept override{return active.load();}
    c::error shutdown()noexcept override{active=false;return c::error::ok;}
};
static c::node_plan plan()
{c::node_plan p;p.authority={1,5,2};p.selected.identity="node";p.selected.digest[0]=1;p.context.identity="context";p.context.digest[0]=2;p.name="robot";p.name_space="/scene";p.max_observers=1;return p;}
static void cleanup(stage& value,bool disclosed)
{
    value.request_stop();assert(value.cleanup_progress()==(disclosed?c::error::busy:c::error::ok));
    auto final=value.inspect();assert(final.phase==(disclosed?w::stage_phase::cleanup_unknown:w::stage_phase::retired));
    assert(final.cleanup_debt==disclosed&&!final.identity_confirmed&&!final.launches);
}
int main(int argc,char**)
{
    assert(argc==1||argc==9);fixture provisioner;auto manifest=provisioner.configuration();
    auto selected=plan();auto context=uwvm2::utils::container::make_shared<retained_context>();int caller{};
    c::node_stage_request request{&caller,3,51,52,101,{1,6,7},{}};request.request_digest[0]=1;
#if defined(UWVM_WORKER_STAGE_ALLOCATION)
    {
        std::size_t failures{};bool exhausted{};auto before=descriptors();
        for(std::size_t point=1;point<32;++point)
        {
            {
                stage value;fail_after=point;injected=false;
                auto status=value.configure(manifest,selected);auto failed=injected.load();fail_after=0;
                if(failed){++failures;assert(status==c::error::no_memory);}
                else{assert(status==c::error::ok);exhausted=true;}
                cleanup(value,false);
            }
            assert(descriptors()==before);if(exhausted)break;
        }
        assert(exhausted&&failures>=1);std::printf("recoverable node-stage attachment fast_io allocation points=%zu\n",failures);
    }
#endif
    {
        stage value;assert(value.configure(manifest,selected)==c::error::ok);assert(!value.inspect().launches&&!value.inspect().cleanup_debt);
        auto bad=request;bad.caller_instance=nullptr;assert(value.prepare_node(selected,context,bad)==c::error::denied);
        assert(value.prepare_node(selected,context,request)==c::error::again&&!value.inspect().launches);
        bad=request;++bad.caller_generation;assert(value.prepare_node(selected,context,bad)==c::error::denied);
        auto other=uwvm2::utils::container::make_shared<retained_context>();assert(value.prepare_node(selected,other,request)==c::error::denied);
        provisioner.response(a::ready);assert(value.prepare_node(selected,context,request)==c::error::ok);
        assert(value.inspect().identity_confirmed&&value.references_capability(51)&&value.references_capability(52)&&!value.references_capability(53));
        assert(value.commit(selected,context,request)==c::error::denied);
        assert(value.attach()==c::error::again);provisioner.response(a::attached);
        assert(value.attachment_progress()==c::error::ok&&value.inspect().phase==w::stage_phase::attached);
        assert(value.commit(selected,context,bad)==c::error::denied&&value.commit(selected,context,request)==c::error::ok);
        auto forked=::fork();assert(forked>=0);if(!forked){assert(value.prepare_node(selected,context,request)==c::error::denied);::_exit(0);}
        int exit{};assert(::waitpid(forked,&exit,0)==forked&&WIFEXITED(exit)&&!WEXITSTATUS(exit));cleanup(value,true);
    }
    {
        stage value;assert(value.configure(manifest,selected)==c::error::ok);context->active=false;
        assert(value.prepare_node(selected,context,request)==c::error::cancelled&&!value.inspect().launches);
        cleanup(value,false);context->active=true;
    }
    {
        stage value;assert(value.configure(manifest,selected)==c::error::ok&&value.prepare_node(selected,context,request)==c::error::again);
        cleanup(value,true);
    }
    std::puts("PASS caller-bound external node stage: inert registration, exact incarnation/context, private attachment, commit, cancellation, fork and retained cleanup debt");
}
