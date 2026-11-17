/** @file
 * @brief Exercise real model dispatch, whole-input authority and bounded worker borrows.
 * @details The native arithmetic worker tests the provider protocol, not LLM
 * inference quality or an unconfigured remote backend.
 */
#include <uwvm2/imported/ros/scene/model_dispatch.h>
#include <cassert>
#include <iostream>
#include <thread>
#include <barrier>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
namespace s=uwvm2::imported::ros::scene;
namespace c=uwvm2::imported::ros::model_contract;
using s::error;
#if defined(UWVM_MODEL_DISPATCH_FAULT_INJECTION)
static thread_local std::size_t fail_after{};
static thread_local bool failed_allocation{};
static bool deny_allocation() noexcept
{if(fail_after&&!--fail_after){failed_allocation=true;return true;}return false;}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t size){return deny_allocation()?nullptr:__real_malloc(size);}
extern "C" void* __wrap_calloc(std::size_t count,std::size_t size){return deny_allocation()?nullptr:__real_calloc(count,size);}
extern "C" void* __wrap_realloc(void* pointer,std::size_t size){return deny_allocation()?nullptr:__real_realloc(pointer,size);}
#endif
static s::buffers::profile profile(std::string_view name)
{s::buffers::profile value;assert(value.identity.assign(name));value.sha256.fill(42);return value;}
static s::buffers::plan buffer_plan()
{
    s::buffers::plan value;value.schema=profile("tensor");value.objects=2;value.bytes=8;value.object_bytes=4;
    value.transfer_bytes=4;value.workspace_bytes=16;value.leases=1;value.elements=4;value.rank=1;
    value.element_mask=1;value.order_mask=1;value.color_mask=1;return value;
}
static s::models::plan model_plan()
{
    s::models::plan value;value.model=profile("local.identity");value.task=profile("tensor.increment");
    assert(value.parameter_type.assign("increment")&&value.output_type.assign("tensor")&&value.encoding.assign("bytes"));
    value.sessions=2;value.requests=2;value.input_bytes=8;value.input_count=2;value.parameter_bytes=8;
    value.output_bytes=16;value.output_items=4;value.workspace_bytes=4096;value.deadline_ns=60000000000;value.streaming=true;
    value.validate_parameters=[](s::wire::bytes bytes) noexcept{return bytes.size()==1&&bytes[0]==1;};
    value.validate_output=[](s::wire::bytes bytes,bool) noexcept{return bytes.size()==4;};return value;
}
static s::grant model_grant()
{
    s::grant value;value.alias="brain";value.resource="inference";value.kind=s::purpose::vision_model;value.binding="local-model";
    value.actions={"model.open","model.submit","model.take","model.cancel","model.describe","request.describe","request.release","resource.close"};
    value.max_resources=2;value.max_handles=2;return value;
}
static s::grant data_grant()
{s::grant value;value.alias="data";value.resource="tensor";value.kind=s::purpose::buffer;value.binding="sealed-buffer";value.actions={"buffer.read"};value.max_handles=2;return value;}
static std::vector<std::uint8_t> words(std::initializer_list<std::uint64_t> values)
{std::vector<std::uint8_t> bytes(values.size()*8);s::wire::writer writer{bytes};for(auto value:values)assert(writer.u64(value));return bytes;}
static std::vector<std::uint8_t> opening()
{std::array<std::uint8_t,160> bytes{};s::wire::writer out{bytes};assert(c::write_profile(out,profile("local.identity")));return {out.written().begin(),out.written().end()};}
struct fixture
{
    std::shared_ptr<s::authority> permissions=uwvm2::utils::container::make_shared<s::authority>();
    s::buffers::registry buffers;s::model_dispatch dispatch{permissions,buffers};int instance{};s::caller who{&instance,1};
    s::handle model_cap{},data_cap{},input{},session{};
    explicit fixture(s::grant data=data_grant(),s::grant model=model_grant(),bool attach=true)
    {
        assert(permissions->expose("capability.open",s::exposure::allow)==error::success&&permissions->expose("capability.close",s::exposure::allow)==error::success);
        for(auto const& operation:c::operations)assert(permissions->expose(uwvm2::utils::container::std_string(operation.name),s::exposure::allow)==error::success);
        assert(permissions->expose("buffer.read",s::exposure::allow)==error::success);
        assert(permissions->configure(std::move(model))==error::success&&permissions->configure(std::move(data))==error::success);
        assert(permissions->enroll(who)==error::success&&permissions->open(who,"brain",model_cap)==error::success&&permissions->open(who,"data",data_cap)==error::success);
        assert(buffers.bind("data",buffer_plan())==error::success&&dispatch.bind("brain",model_plan())==error::success);
        s::buffers::layout layout;layout.schema=profile("tensor");layout.rank=1;layout.shape[0]=layout.bytes=4;layout.strides[0]=1;
        assert(buffers.create("data",layout,2,[&](auto id){input=id;return true;})==error::success);
        constexpr std::array<std::uint8_t,4> bytes{1,2,3,4};std::uint64_t accepted{};
        assert(buffers.write("data",input,0,bytes,accepted)==error::success&&buffers.seal("data",input,s::buffers::hash(bytes))==error::success);
        if(attach){assert(dispatch.attach("brain",8)==error::success);std::vector<std::uint8_t> out;assert(call(c::operation::open,opening(),out)==error::success);s::wire::reader reader{out};assert(reader.u64(session)&&reader.done());}
    }
    error call(c::operation action,s::wire::bytes input,std::vector<std::uint8_t>& output,std::size_t capacity=512,s::model_dispatch::sink custom={})
    {
        auto copy=[&](s::wire::bytes bytes){output.assign(bytes.begin(),bytes.end());return true;};std::size_t required{};
        return dispatch.invoke(who,action,model_cap,input,capacity,required,custom?custom:s::model_dispatch::sink(copy));
    }
    std::vector<std::uint8_t> inference(std::uint64_t sequence=1,bool forged=false)
    {
        std::array<std::uint8_t,512> bytes{};s::wire::writer out{bytes};constexpr std::array<std::uint8_t,1> parameters{1};
        assert(out.u64(session)&&out.u64(sequence)&&out.u64(session)&&c::write_profile(out,profile("tensor.increment"))&&out.text("increment")&&out.text("bytes")&&out.blob(parameters));
        assert(out.u64(1)&&out.u64(data_cap)&&out.u64(input)&&out.u64(forged?1:0)&&out.u64(0)&&out.boolean(false)&&out.boolean(false));
        assert(out.u64(8)&&out.u64(2)&&out.u64(8)&&out.u64(s::models::now()+model_plan().deadline_ns/2));
        return {out.written().begin(),out.written().end()};
    }
    std::uint64_t admissions(std::string_view alias)
    {uwvm2::utils::container::std_vector<s::authority_snapshot> rows;assert(permissions->inspect(rows,8));for(auto const& row:rows)if(row.alias==alias)return row.admissions;assert(false);return 0;}
    s::handle request(std::vector<std::uint8_t> const& bytes)
    {std::vector<std::uint8_t> out;assert(call(c::operation::submit,bytes,out)==error::success);s::wire::reader reader{out};s::handle id{};assert(reader.u64(id));return id;}
};
static void explicit_activation_and_replay()
{
    fixture inert{data_grant(),model_grant(),false};std::vector<std::uint8_t> output;
    assert(inert.call(c::operation::open,opening(),output)==error::unsupported);
    fixture f;auto input=f.inference();auto before=f.admissions("brain");
    assert(f.call(c::operation::submit,input,output,159)==error::too_large&&f.admissions("brain")==before&&!f.admissions("data"));
    auto failed=[](s::wire::bytes){return false;};assert(f.call(c::operation::submit,input,output,160,failed)==error::io);
    s::buffers::usage usage;assert(f.buffers.inspect("data",usage)==error::success&&usage.leases==1);
    auto id=f.request(input);assert(id&&f.buffers.inspect("data",usage)==error::success&&usage.leases==1);
    auto changed=input;changed.back()^=1;assert(f.call(c::operation::submit,changed,output)==error::conflict);
    assert(f.call(c::operation::submit,f.inference(2,true),output)==error::denied);
}
static void atomic_scopes_and_retained_inputs()
{
    auto data=data_grant();data.limits.uncertain=true;data.limits.calls=0;fixture denied{data};std::vector<std::uint8_t> output;
    auto before=denied.admissions("brain");assert(denied.call(c::operation::submit,denied.inference(),output)==error::again&&denied.admissions("brain")==before&&!denied.admissions("data"));
    fixture f;auto wire=f.inference();auto id=f.request(wire);assert(f.buffers.close("data",f.input)==error::success);
    assert(f.request(wire)==id); // Exact replay needs no new handle lookup/pin.
    s::model_dispatch::work work;assert(f.dispatch.claim("brain",8,work)==error::success&&work.identity()==id);
    std::array<std::uint8_t,4> result{};
    assert(work.with_parameters([](auto bytes){return bytes.size()==1&&bytes[0]==1;})==error::success);
    assert(work.with_input(0,[&](auto bytes,auto const&,auto const&){for(std::size_t n{};n<4;++n)result[n]=bytes[n]+1;return true;})==error::success);
    assert(work.emit(result)==error::success&&work.finish(s::models::state::succeeded,result)==error::success);
    auto query=words({f.session,id});auto failed=[](s::wire::bytes){return false;};assert(f.call(c::operation::take,query,output,512,failed)==error::io);
    assert(f.call(c::operation::take,query,output)==error::success);
    auto first=output;assert(f.call(c::operation::take,query,output)==error::success&&output!=first); // Distinct terminal delivery.
    assert(f.call(c::operation::request_release,words({id}),output)==error::success);
    s::wire::reader released{output};bool yes{};assert(released.boolean(yes)&&yes&&released.done());
    assert(f.call(c::operation::request_release,words({id}),output)==error::success);
    s::wire::reader again{output};assert(again.boolean(yes)&&!yes&&again.done());
    assert(f.request(wire)==id); // The session receipt survives explicit request release.
}
static void replay_after_source_retirement()
{
    auto data=data_grant();data.limits.uncertain=true;data.limits.calls=1;data.limits.bytes=4;
    data.limits.window=std::chrono::hours(1);fixture f{data};auto wire=f.inference();auto id=f.request(wire);
    assert(f.admissions("data")==1&&f.request(wire)==id&&f.admissions("data")==1);
    assert(f.permissions->close(f.who,f.data_cap)==error::success);
    assert(f.permissions->revoke("data",[&](auto alias){f.buffers.revoke(alias);})==error::success);
    assert(f.request(wire)==id&&f.admissions("data")==1);
    s::model_dispatch::work held;assert(f.dispatch.claim("brain",8,held)==error::again&&!held);
    std::vector<std::uint8_t> output;
    assert(f.call(c::operation::request_describe,words({id}),output)==error::success);
    s::wire::reader state{output};std::uint64_t observed{};std::string_view phase;
    assert(state.u64(observed)&&observed==id&&state.text(phase)&&phase=="cancelled");
    assert(f.call(c::operation::request_release,words({id}),output)==error::success&&f.request(wire)==id);
    auto changed=wire;changed.back()^=1;assert(f.call(c::operation::submit,changed,output)==error::conflict);
    assert(f.permissions->revoke("brain",[&](auto alias){f.dispatch.revoke(alias);})==error::success);
    assert(f.call(c::operation::submit,wire,output)==error::denied);
}
static void revoke_between_borrow_and_publication()
{
    fixture f;auto id=f.request(f.inference());s::model_dispatch::work work;assert(f.dispatch.claim("brain",8,work)==error::success);
    std::barrier begin{2};std::atomic<bool> borrowed{},revoked{};
    std::thread native{[&]{assert(work.with_input(0,[&](auto data,auto const&,auto const&){borrowed=data.size()==4;return true;})==error::success);
        begin.arrive_and_wait();begin.arrive_and_wait();assert(revoked.load()&&work.cancellation_requested());
        constexpr std::array<std::uint8_t,4> result{2,3,4,5};assert(work.emit(result)==error::denied);assert(work.finish(s::models::state::succeeded,result)==error::success);}};
    begin.arrive_and_wait();assert(borrowed.load());
    assert(f.permissions->revoke("data",[&](auto alias){f.buffers.revoke(alias);})==error::success);revoked=true;begin.arrive_and_wait();native.join();
    std::vector<std::uint8_t> output;assert(f.call(c::operation::take,words({f.session,id}),output)==error::denied);
    assert(f.call(c::operation::request_describe,words({id}),output)==error::success);s::wire::reader input{output};std::uint64_t found{};std::string_view state;
    assert(input.u64(found)&&found==id&&input.text(state)&&state=="cancelled");
    assert(f.call(c::operation::request_release,words({id}),output)==error::success);
}
static void lifetime_and_membership_fencing()
{
    s::model_dispatch::work held;
    {fixture f;f.request(f.inference());assert(f.dispatch.claim("brain",8,held)==error::success);f.permissions->retire_member(f.who);
        assert(held.cancellation_requested()&&held.with_parameters([](auto){assert(false);return true;})==error::denied);}
    assert(held.cancellation_requested());assert(held.finish(s::models::state::cancelled)==error::success);
    fixture f;s::model_dispatch::work wrong;auto id=f.request(f.inference());assert(f.dispatch.claim("brain",9,wrong)==error::denied&&!wrong);
    std::vector<std::uint8_t> output;assert(f.call(c::operation::request_release,words({id}),output)==error::again);
    assert(f.call(c::operation::cancel,words({f.session,id}),output)==error::success);
    assert(f.call(c::operation::request_release,words({id}),output)==error::success);
}
static void fork_before_authority_lock()
{
    fixture f;f.request(f.inference());s::model_dispatch::work work;assert(f.dispatch.claim("brain",8,work)==error::success);
    std::barrier rendezvous{2};
    std::thread owner{[&]{s::lease held;assert(f.permissions->admit(f.who,f.model_cap,"model.describe",0,held,false)==error::success);
        rendezvous.arrive_and_wait();rendezvous.arrive_and_wait();}};
    rendezvous.arrive_and_wait();auto pid=::fork();assert(pid>=0);
    if(!pid)
    {
        ::alarm(3);s::model_dispatch::work child;s::models::registry::workspace_lease workspace;std::vector<std::uint8_t> output;
        bool denied=work.cancellation_requested()&&work.with_parameters([](auto){return false;})==error::denied&&
            f.dispatch.claim("brain",8,child)==error::denied&&
            f.dispatch.preflight(f.who,c::operation::describe,f.model_cap,8)==error::denied&&
            f.dispatch.reserve_workspace(f.who,c::operation::submit,f.model_cap,1024,workspace)==error::denied&&
            f.call(c::operation::request_describe,words({work.identity()}),output)==error::denied;
        ::_exit(denied?0:3);
    }
    int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&!WEXITSTATUS(status));
    rendezvous.arrive_and_wait();owner.join();assert(work.finish(s::models::state::cancelled)==error::success);
}
static void actual_allocator_failure_rollback()
{
#if defined(UWVM_MODEL_DISPATCH_FAULT_INJECTION)
    unsigned failures{};bool exhausted{};
    for(std::size_t index=1;index<=64;++index)
    {
        fixture f;auto input=f.inference();std::vector<std::uint8_t> output;output.reserve(512);
        fail_after=index;failed_allocation=false;error status=error::success;
        try{status=f.call(c::operation::submit,input,output);}catch(std::bad_alloc const&){status=error::no_memory;}
        fail_after=0;
        if(failed_allocation)
        {
            ++failures;assert(status==error::no_memory||status==error::io);
            s::models::usage used;assert(f.dispatch.try_inspect("brain",used)==error::success&&used.requests<=1);
            s::buffers::usage data;assert(f.buffers.inspect("data",data)==error::success&&data.leases==used.requests);
            if(!used.requests)assert(!used.inputs&&!used.output_bytes&&!used.output_items&&!used.workspace);
        }
        else{assert(status==error::success);exhausted=true;}
        auto id=f.request(input);s::model_dispatch::work work;assert(f.dispatch.claim("brain",8,work)==error::success&&work.identity()==id);
        assert(work.finish(s::models::state::cancelled)==error::success);
        assert(f.call(c::operation::request_release,words({id}),output)==error::success);
        if(exhausted)break;
    }
    assert(exhausted&&failures>=6);
#endif
}
int main(){explicit_activation_and_replay();atomic_scopes_and_retained_inputs();replay_after_source_retirement();revoke_between_borrow_and_publication();lifetime_and_membership_fencing();fork_before_authority_lock();actual_allocator_failure_rollback();std::cout<<"passed 7 model dispatch security groups\n";return 0;}
