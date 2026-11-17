/** @file @brief Inert worker configuration, exact file opening and transactional startup. */
#define UWVM_MODEL_WORKER_NO_MAIN
#include "model_worker.cpp"
#include "support/legacy_process/model_worker_manifest.h"
namespace mw=s::model_worker;
static mw::configuration settings(x::manifest const& manifest,char** paths,std::string_view launcher={})
{
    mw::configuration result;
    for(auto field:mw::limit_fields)assert(result.limit(field.name,manifest.bound.*field.member)==error::success);
    assert(result.launcher(launcher.empty()?std::string_view{paths[8]}:launcher,manifest.launcher.bytes,manifest.launcher.sha256)==error::success);
    for(std::size_t i{};i<manifest.files.size();++i)
    {
        auto const& file=manifest.files[i];
        assert(result.image(file.role,file.destination.view(),paths[i],file.bytes,file.sha256)==error::success);
    }
    return result;
}
static error prepare(mw::configuration const& selected,std::shared_ptr<s::models::provider_lifetime>& worker,s::model_dispatch& dispatcher)
{return selected.prepare(dispatcher,"brain",1,worker);}
int main(int argc,char** argv)
{
    assert(argc==10);uwvm2::utils::container::std_vector<x::descriptor> ownership;auto manifest=worker_manifest(argv+1,ownership);
    auto authority=uwvm2::utils::container::make_shared<s::authority>();s::buffers::registry buffers;s::model_dispatch dispatcher{authority,buffers};
    assert(dispatcher.bind("brain",model_plan())==error::success);std::shared_ptr<s::models::provider_lifetime> worker;
    {
        mw::configuration empty;assert(empty.freeze(model_plan())==error::invalid&&!empty.frozen());
        assert(prepare(empty,worker,dispatcher)==error::invalid&&!worker);
        assert(empty.limit("unknown",1)==error::invalid&&empty.limit("stderr-bytes",0)==error::success&&empty.limit("stderr-bytes",1)==error::conflict);
        assert(empty.launcher("relative",1,{})==error::invalid&&empty.launcher("/",1,{})==error::invalid);
        assert(empty.image(x::image_role::runtime,"/worker/../bad","/missing",1,{})==error::invalid);
        assert(empty.placement("1,0","inherit","none")==error::invalid);
        assert(empty.placement("inherit","bind","none")==error::invalid);
        assert(empty.placement("inherit","inherit","none")==error::success);
        assert(empty.placement("0","local","none")==error::conflict);
    }
    {
        auto missing=settings(manifest,argv+1,"/definitely-missing-uwvm-approved-model-launcher");
        assert(missing.freeze(model_plan())==error::success&&missing.frozen()); // no path access at freeze
        assert(prepare(missing,worker,dispatcher)==error::io&&!worker);
        assert(missing.limit("poll-ns",1)==error::denied&&missing.launcher(argv[9],1,{})==error::denied);
        assert(missing.image(x::image_role::weights,"/model/new",argv[3],1,{})==error::denied);
        assert(missing.placement("inherit","inherit","none")==error::denied);
    }
    auto base=uwvm2::utils::container::std_string(argv[1]);base.resize(base.rfind('/'));
    auto link=base+"/manifest-rejected-symlink",fifo=base+"/manifest-rejected-fifo";
    assert(!::symlink(argv[9],link.c_str())&&!::mkfifo(fifo.c_str(),0600));
    for(auto const& path:{link,fifo,base})
    {
        auto denied=settings(manifest,argv+1,path);assert(denied.freeze(model_plan())==error::success);
        assert(prepare(denied,worker,dispatcher)==error::invalid&&!worker);
    }
    assert(!::unlink(link.c_str())&&!::unlink(fifo.c_str()));
    {
        auto wrong=manifest;wrong.files[2].sha256[0]^=1;
        auto selected=settings(wrong,argv+1);assert(selected.freeze(model_plan())==error::success);
        assert(prepare(selected,worker,dispatcher)==error::conflict&&!worker);
    }
    {
        auto selected=settings(manifest,argv+1);auto bad=model_plan();bad.task.sha256[0]^=1;
        assert(selected.freeze(bad)==error::success&&prepare(selected,worker,dispatcher)==error::denied&&!worker);
    }
    {
        auto unavailable=settings(manifest,argv+1);
        assert(unavailable.placement("8388607","inherit","none")==error::success);
        assert(unavailable.freeze(model_plan())==error::success); // Topology is not probed by static validation.
        assert(prepare(unavailable,worker,dispatcher)==error::denied&&!worker);
    }
    {
        auto selected=settings(manifest,argv+1);assert(selected.freeze(model_plan())==error::success);
        assert(prepare(selected,worker,dispatcher)==error::success&&worker);
        assert(prepare(selected,worker,dispatcher)==error::invalid);worker->retire();worker.reset();
    }
    std::puts("passed 7 worker configuration and actual factory groups");
}
