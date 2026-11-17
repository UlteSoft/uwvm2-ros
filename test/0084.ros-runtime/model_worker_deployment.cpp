/** @file
 * @brief Exercise the production deployment factory with real sandbox processes.
 * @details Freeze accesses no image paths. Preparation must publish all aliases
 * together, or terminate and reap every worker already started. Successful
 * deployment executes the denied-syscall probe through the model job carrier.
 * This native broker test is separate from actual Wasm execution qualification.
 */
#define UWVM_MODEL_WORKER_NO_MAIN
#include "model_worker.cpp"
#include <uwvm2/imported/ros/scene/deployment.h>
static void configure(s::deployment& deployment,x::manifest const& manifest,char** paths,bool bad_second)
{
    assert(deployment.create("robot")==error::success&&deployment.bind("robot","main")==error::success);
    assert(deployment.expose("robot","capability.open",s::exposure::allow)==error::success);
    for(auto const& operation:c::operations)
        assert(deployment.expose("robot",uwvm2::utils::container::std_string(operation.name),s::exposure::allow)==error::success);
    auto plan=model_plan();
    assert(deployment.model_budget("robot",{4,4,64,128,8,65536})==error::success);
    for(auto alias:{"first","second"})
    {
        s::grant grant;grant.alias=alias;grant.resource="matrix";grant.binding="local-model";grant.kind=s::purpose::vision_model;
        grant.max_handles=grant.max_resources=2;
        for(auto const& operation:c::operations)grant.actions.emplace(operation.name);
        assert(deployment.allow("robot",std::move(grant))==error::success&&deployment.model_plan("robot",alias,plan)==error::success);
        assert(deployment.model_limits("robot",alias,{2,2,32,1,8,64,4,32768,10000000000ull})==error::success);
        for(auto field:s::model_worker::limit_fields)
            assert(deployment.model_worker_limit("robot",alias,field.name,manifest.bound.*field.member)==error::success);
        assert(deployment.model_worker_launcher("robot",alias,paths[8],manifest.launcher.bytes,manifest.launcher.sha256)==error::success);
        for(std::size_t i{};i<manifest.files.size();++i)
        {
            auto file=manifest.files[i];if(bad_second&&std::string_view{alias}=="second"&&i==2)file.sha256[0]^=1;
            assert(deployment.model_worker_image("robot",alias,file.role,file.destination.view(),paths[i],file.bytes,file.sha256)==error::success);
        }
    }
}
static unsigned descriptors()
{
    unsigned count{};for(int descriptor{};descriptor<1024;++descriptor)if(::fcntl(descriptor,F_GETFD)!=-1)++count;return count;
}
static void reaped()
{
    int status{};errno=0;assert(::waitpid(-1,&status,WNOHANG)==-1&&errno==ECHILD);
}
static void invoke_probe(s::broker& broker,s::caller who,std::string_view alias)
{
    s::handle cap{};assert(broker.permissions.open(who,alias,cap)==error::success);
    uwvm2::utils::container::std_vector<std::uint8_t> result;
    auto call=[&](c::operation action,s::wire::bytes input)
    {
        std::size_t required{};auto emit=[&](s::wire::bytes body){result.assign(body.begin(),body.end());return true;};
        assert(broker.model_store.invoke(who,action,cap,input,4096,required,emit)==error::success);
    };
    std::array<std::uint8_t,512> encoded{};s::wire::writer opening{encoded};assert(c::write_profile(opening,model_plan().model));
    call(c::operation::open,opening.written());s::wire::reader opened{result};s::handle session{};assert(opened.u64(session)&&opened.done());
    s::wire::writer submitting{encoded};auto parameters=words({2});
    assert(submitting.u64(session)&&submitting.u64(1)&&submitting.u64(session)&&c::write_profile(submitting,model_plan().task)&&
        submitting.text("mode")&&submitting.text("bytes")&&submitting.blob(parameters)&&submitting.u64(0)&&
        submitting.u64(32)&&submitting.u64(2)&&submitting.u64(8192)&&submitting.u64(s::models::now()+5000000000ull));
    call(c::operation::submit,submitting.written());s::wire::reader submitted{result};s::handle request{};assert(submitted.u64(request));
    auto deadline=s::models::now()+6000000000ull;bool completed{};
    do
    {
        call(c::operation::request_describe,words({request}));s::wire::reader receipt{result};std::uint64_t identity{};std::string_view phase;
        assert(receipt.u64(identity)&&identity==request&&receipt.text(phase));
        if(phase=="succeeded"){completed=true;break;}
        assert(phase=="queued"||phase=="running");::poll(nullptr,0,1);
    }while(s::models::now()<deadline);
    assert(completed);call(c::operation::take,words({session,request}));
    assert(result.size()>=16);s::wire::reader body{s::wire::bytes{result}.last(16)};s::wire::bytes output;
    assert(body.blob(output)&&body.done());s::wire::reader number{output};std::uint64_t denied{};
    assert(number.u64(denied)&&number.done()&&denied==1023);
    call(c::operation::request_release,words({request}));
}
int main(int argc,char** argv)
{
    assert(argc==10);uwvm2::utils::container::std_vector<x::descriptor> ownership;auto manifest=worker_manifest(argv+1,ownership);
    auto input=[](auto const&,auto const&,auto,auto&){assert(false);return error::denied;};
    auto before=descriptors();
    {
        s::deployment deployment;configure(deployment,manifest,argv+1,true);assert(deployment.validate()==error::success);
        assert(deployment.prepare_resources(input,s::deployment::isolated_model_provider{})==error::conflict);
        int instance{};std::shared_ptr<s::broker> selected;assert(deployment.select("main",{&instance,1},selected)==error::denied&&!selected);
        assert(descriptors()==before);reaped();
    }
    {
        s::deployment deployment;configure(deployment,manifest,argv+1,false);assert(deployment.validate()==error::success);
        int instance{};s::caller who{&instance,1};std::shared_ptr<s::broker> selected;
        assert(deployment.select("main",who,selected)==error::denied&&!selected);
        assert(deployment.prepare_resources(input,s::deployment::isolated_model_provider{})==error::success);
        assert(deployment.select("main",who,selected)==error::success&&selected);
        invoke_probe(*selected,who,"first");invoke_probe(*selected,who,"second");
    }
    assert(descriptors()==before);reaped();
    std::puts("passed 2 actual deployment factory and rollback groups");
}
