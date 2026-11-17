/** @file @brief Real isolated inference, authority revocation and confirmed process cleanup. */
#include "support/legacy_process/model_worker.h"
#include "model_worker_fixture.h"
#include <cassert>
#include <cstdio>
#include <vector>
#include <thread>
namespace s=uwvm2::imported::ros::scene;
namespace c=uwvm2::imported::ros::model_contract;
using s::error;
static s::buffers::profile profile(std::string_view name)
{s::buffers::profile value;assert(value.identity.assign(name));value.sha256.fill(42);return value;}
static s::models::plan model_plan()
{
    s::models::plan p;p.model=profile("matrix.cpu");p.task=profile("matrix.multiply");
    assert(p.parameter_type.assign("mode")&&p.output_type.assign("matrix.result")&&p.encoding.assign("bytes"));
    p.sessions=2;p.requests=2;p.input_bytes=32;p.input_count=1;p.parameter_bytes=8;p.output_bytes=64;p.output_items=4;p.workspace_bytes=32768;p.deadline_ns=10000000000ull;
    p.validate_parameters=[](s::wire::bytes data) noexcept {s::wire::reader reader{data};std::uint64_t mode{};return reader.u64(mode)&&mode>=1&&mode<=5&&reader.done();};
    p.validate_output=[](s::wire::bytes data,bool) noexcept {return data.size()==16||data.size()==8;};return p;
}
static std::vector<std::uint8_t> words(std::initializer_list<std::uint64_t> values)
{std::vector<std::uint8_t> result(values.size()*8);s::wire::writer writer{result};for(auto value:values)assert(writer.u64(value));return result;}
struct fixture
{
    std::shared_ptr<s::authority> permissions=uwvm2::utils::container::make_shared<s::authority>();
    s::buffers::registry buffers;s::model_dispatch dispatcher{permissions,buffers};
    std::shared_ptr<s::models::provider_lifetime> worker;
    int instance{};s::caller who{&instance,1};s::handle model_cap{},data_cap{},input{},session{};
    fixture(x::manifest const& manifest)
    {
        assert(permissions->expose("capability.open",s::exposure::allow)==error::success);
        for(auto const& operation:c::operations)assert(permissions->expose(uwvm2::utils::container::std_string(operation.name),s::exposure::allow)==error::success);
        assert(permissions->expose("buffer.read",s::exposure::allow)==error::success);
        s::grant model;model.alias="brain";model.resource="matrix";model.kind=s::purpose::vision_model;model.binding="local-model";model.max_resources=model.max_handles=2;
        for(auto const& operation:c::operations)model.actions.emplace(operation.name);
        s::grant data;data.alias="data";data.resource="tensor";data.kind=s::purpose::buffer;data.binding="sealed-buffer";data.actions={"buffer.read"};data.max_handles=2;
        assert(permissions->configure(std::move(model))==error::success&&permissions->configure(std::move(data))==error::success);
        assert(permissions->enroll(who)==error::success&&permissions->open(who,"brain",model_cap)==error::success&&permissions->open(who,"data",data_cap)==error::success);
        s::buffers::plan b;b.schema=profile("tensor");b.objects=2;b.bytes=32;b.object_bytes=b.transfer_bytes=16;b.workspace_bytes=32;b.leases=2;b.elements=16;b.rank=1;b.element_mask=1;b.order_mask=1;b.color_mask=1;
        assert(buffers.bind("data",b)==error::success&&dispatcher.bind("brain",model_plan())==error::success);
        auto prepared=s::model_worker::service::prepare(dispatcher,"brain",1,manifest,worker);
        if(prepared!=error::success)std::fprintf(stderr,"service prepare=%u\n",static_cast<unsigned>(prepared));
        assert(prepared==error::success);
        s::buffers::layout layout;layout.schema=b.schema;layout.rank=1;layout.shape[0]=layout.bytes=16;layout.strides[0]=1;
        assert(buffers.create("data",layout,2,[&](auto id){input=id;return true;})==error::success);
        auto values=words({std::bit_cast<std::uint64_t>(5.0),std::bit_cast<std::uint64_t>(6.0)});std::uint64_t accepted{};
        assert(buffers.write("data",input,0,values,accepted)==error::success&&buffers.seal("data",input,s::buffers::hash(values))==error::success);
        std::array<std::uint8_t,160> encoded{};s::wire::writer writer{encoded};assert(c::write_profile(writer,model_plan().model));
        std::vector<std::uint8_t> result;assert(call(c::operation::open,writer.written(),result)==error::success);s::wire::reader reader{result};assert(reader.u64(session)&&reader.done());
    }
    ~fixture(){if(worker)worker->retire();}
    error call(c::operation action,s::wire::bytes input,std::vector<std::uint8_t>& output)
    {
        auto copy=[&](s::wire::bytes bytes){output.assign(bytes.begin(),bytes.end());return true;};std::size_t required{};
        return dispatcher.invoke(who,action,model_cap,input,4096,required,copy);
    }
    s::handle submit(std::uint64_t mode,std::uint64_t lifetime=5000000000ull)
    {
        std::array<std::uint8_t,512> encoded{};s::wire::writer writer{encoded};auto parameters=words({mode});
        assert(writer.u64(session)&&writer.u64(1)&&writer.u64(session)&&c::write_profile(writer,model_plan().task)&&writer.text("mode")&&writer.text("bytes")&&writer.blob(parameters));
        assert(writer.u64(1)&&writer.u64(data_cap)&&writer.u64(input)&&writer.u64(0)&&writer.u64(0)&&writer.boolean(false)&&writer.boolean(false));
        assert(writer.u64(32)&&writer.u64(2)&&writer.u64(8192)&&writer.u64(s::models::now()+lifetime));
        std::vector<std::uint8_t> result;assert(call(c::operation::submit,writer.written(),result)==error::success);s::wire::reader reader{result};s::handle request{};assert(reader.u64(request));return request;
    }
    std::string phase(s::handle request)
    {
        std::vector<std::uint8_t> result;assert(call(c::operation::request_describe,words({request}),result)==error::success);
        s::wire::reader reader{result};std::uint64_t id{};std::string_view phase;assert(reader.u64(id)&&id==request&&reader.text(phase));return std::string{phase};
    }
    std::string wait(s::handle request,bool running=false)
    {
        auto deadline=s::models::now()+6000000000ull;std::string observed;
        do {observed=phase(request);if(running?observed=="running":observed!="queued"&&observed!="running")return observed;::poll(nullptr,0,1);}while(s::models::now()<deadline);
        std::fprintf(stderr,"timed out phase=%s\n",observed.c_str());assert(false);return {};
    }
    void release(s::handle request)
    {
        std::vector<std::uint8_t> result;error status;auto deadline=s::models::now()+5000000000ull;
        do {status=call(c::operation::request_release,words({request}),result);if(status==error::again)::poll(nullptr,0,1);}while(status==error::again&&s::models::now()<deadline);
        assert(status==error::success);s::buffers::usage use;assert(buffers.inspect("data",use)==error::success&&use.leases==0);
    }
};
#ifndef UWVM_MODEL_WORKER_NO_MAIN
int main(int argc,char** argv)
{
    assert(argc==10);uwvm2::utils::container::std_vector<x::descriptor> ownership;auto manifest=worker_manifest(argv+1,ownership);
    manifest.model=model_plan().model;manifest.task=model_plan().task;
    {
        fixture f{manifest};auto request=f.submit(1);assert(f.wait(request)=="succeeded");
        std::vector<std::uint8_t> result;assert(f.call(c::operation::take,words({f.session,request}),result)==error::success);
        // The canonical take ends with the immutable 16-byte result body.
        assert(result.size()>=24);s::wire::reader body{std::span<std::uint8_t const>{result}.last(24)};s::wire::bytes numbers;assert(body.blob(numbers)&&body.done());
        s::wire::reader values{numbers};double first{},second{};assert(values.f64(first)&&values.f64(second)&&values.done()&&first==17&&second==39);f.release(request);
        std::thread a([&]{f.worker->retire();}),b([&]{f.worker->retire();});a.join();b.join();
    }
    {
        fixture f{manifest};auto request=f.submit(3,50000000);assert(f.wait(request)=="cancelled");f.release(request);
    }
    {
        fixture f{manifest};auto request=f.submit(3);assert(f.wait(request,true)=="running");
        assert(f.permissions->revoke("data",[&](auto alias){f.buffers.revoke(alias);})==error::success);
        assert(f.wait(request)=="cancelled");std::vector<std::uint8_t> result;assert(f.call(c::operation::take,words({f.session,request}),result)==error::denied);f.release(request);
    }
    for(auto mode:{4u,5u})
    {
        fixture f{manifest};auto request=f.submit(mode);assert(f.wait(request)=="failed");f.release(request);
    }
    std::puts("passed 5 real isolated model worker groups");
    return 0;
}
#endif
