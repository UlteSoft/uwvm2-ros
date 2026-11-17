/** @file @brief Actual raw media transforms, atomic output publication and independent authority faults. */
#include <uwvm2/imported/ros/scene/media_dispatch.h>
#include <uwvm2/imported/ros/scene/media_configuration.h>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;
namespace b=s::buffers;namespace r=s::robot_contract;
using error=s::error;using op=r::operation;
static std::atomic<std::uint64_t> current{1000};
static std::atomic<int> fail_allocation{-1};
#if defined(UWVM_MEDIA_FAULT_INJECTION)
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __wrap_malloc(std::size_t size)
{
    int remaining=fail_allocation.load();
    if(remaining>=0&&fail_allocation.fetch_sub(1)==0)return nullptr;
    return __real_malloc(size);
}
#endif
static std::uint64_t clock_now()noexcept{return current.load();}
static b::profile profile(std::string_view name)
{b::profile result;assert(result.identity.assign(name));result.sha256.fill(9);return result;}
static s::media_dispatch::plan plan()
{
    s::media_dispatch::plan value;value.selected=profile("raw.rgb.bgr");value.input_profile=profile("rgb");value.output_profile=profile("bgr");
    assert(value.alias.assign("media")&&value.input_alias.assign("input")&&value.output_alias.assign("output"));
    value.transform=s::media_dispatch::codec::swap_red_blue;value.input_color=b::color_space::rgb;value.output_color=b::color_space::bgr;
    value.width=2;value.height=1;value.channels=3;value.streams=2;value.frames=2;value.workspace_bytes=8192;value.workspace_calls=8;value.deadline_ns=1000;
    assert(s::media_dispatch::required_storage(value,value.storage_bytes));return value;
}
static s::grant grant(std::string_view alias,s::purpose kind,std::string_view binding,std::initializer_list<char const*> actions)
{
    s::grant value;value.alias=alias;value.resource=alias;value.kind=kind;value.binding=binding;value.max_handles=4;value.max_resources=4;
    for(auto name:actions){value.actions.emplace(name);}return value;
}
struct fixture
{
    std::shared_ptr<s::authority> permissions=uwvm2::utils::container::make_shared<s::authority>();b::registry buffers;
    s::media_dispatch::plan policy; s::media_dispatch dispatch;
    int identity{};s::caller who{&identity,1};s::handle cap{},input_cap{},output_cap{},input{},stream{};
    std::array<std::uint8_t,6> bytes{1,2,3,4,5,6};
    fixture(std::string_view omitted={},std::string_view attenuate={},bool zero=false,s::media_dispatch::plan value=plan())
        :policy(value),dispatch(permissions,buffers,71,{1,policy.storage_bytes},clock_now)
    {
        current=1000;assert(permissions->expose("capability.open",s::exposure::allow)==error::success);
        auto media=grant("media",s::purpose::media,"immutable-buffer-media",{"media.open","media.submit","media.take","media.close","resource.close"});
        auto source=grant("input",s::purpose::buffer,"sealed-buffer",{"buffer.read","data.export"});
        auto target=grant("output",s::purpose::buffer,"sealed-buffer",{"buffer.create","buffer.write","buffer.seal","buffer.read"});
        uwvm2::utils::container::std_set<uwvm2::utils::container::std_string> exposed;
        for(auto value:{media,source,target})
        {
            if(!omitted.empty())value.actions.erase(uwvm2::utils::container::std_string(omitted));
            if(value.alias==attenuate){value.limits.calls=zero?0:100;value.limits.uncertain=true;}
            for(auto const& action:value.actions)if(action!="data.export"&&exposed.emplace(action).second)assert(permissions->expose(action,s::exposure::allow)==error::success);
            assert(permissions->configure(value)==error::success);
        }
        assert(permissions->enroll(who)==error::success);
        assert(permissions->open(who,"media",cap)==error::success&&permissions->open(who,"input",input_cap)==error::success&&permissions->open(who,"output",output_cap)==error::success);
        for(bool output:{false,true})
        {
            b::plan p;p.schema=output?policy.output_profile:policy.input_profile;p.objects=4;p.bytes=24;p.object_bytes=p.transfer_bytes=6;
            p.workspace_bytes=64;p.leases=8;p.elements=6;p.rank=3;p.element_mask=1;p.order_mask=1;p.color_mask=255;
            assert(buffers.bind(output?"output":"input",p)==error::success);
        }
        b::layout format;format.schema=policy.input_profile;format.rank=3;format.shape={1,2,3};format.strides={6,3,1};format.bytes=6;format.color=b::color_space::rgb;
        assert(buffers.create("input",format,4,[&](auto id){input=id;return true;})==error::success);std::uint64_t written{};
        assert(buffers.write("input",input,0,bytes,written)==error::success&&written==6&&buffers.seal("input",input,b::hash(bytes))==error::success);
        assert(dispatch.bind(policy)==error::success&&dispatch.prepare()==error::success);
    }
    template<typename Writer>std::vector<std::uint8_t> encode(Writer writer)
    {std::array<std::uint8_t,2048> bytes;s::wire::writer out{bytes};assert(writer(out));return {out.written().begin(),out.written().end()};}
    std::vector<std::uint8_t> open_wire()
    {return encode([&](auto& out){return s::proposals::write_profile(out,policy.selected)&&out.u64(input_cap)&&out.u64(input)&&out.u64(output_cap)&&out.boolean(false);});}
    std::vector<std::uint8_t> submit(std::uint64_t key=1)
    {return encode([&](auto& out){return out.u64(stream)&&out.u64(key)&&out.u64(stream)&&out.u64(input_cap)&&out.u64(input)&&
        s::proposals::write_profile(out,policy.input_profile)&&out.text("image")&&out.u64(key)&&out.u64(2)&&out.u64(1)&&out.u64(3)&&out.u64(0)&&
        out.u64(1)&&out.u64(0)&&out.u64(6)&&out.u64(6)&&out.u64(1)&&out.u64(1800);});}
    std::vector<std::uint8_t> query(){return encode([&](auto& out){return out.u64(stream);});}
    std::vector<std::uint8_t> close_wire(std::uint64_t key=1)
    {return encode([&](auto& out){return out.u64(stream)&&out.u64(key)&&out.u64(stream)&&out.u64(1800);});}
    error call(op action,s::wire::bytes input,std::vector<std::uint8_t>& out,std::size_t capacity=2048,s::media_dispatch::sink custom={},s::media_dispatch::cancellation cancel={})
    {std::size_t required{};auto copy=[&](auto bytes){out.assign(bytes.begin(),bytes.end());return true;};return dispatch.invoke(who,action,cap,input,capacity,required,custom?custom:s::media_dispatch::sink(copy),cancel);}
    void open()
    {std::vector<std::uint8_t> out;assert(call(op::media_open,open_wire(),out)==error::success);s::wire::reader read{out};assert(read.u64(stream)&&stream&&read.done());}
    s::media_dispatch::inspection usage()
    {s::media_dispatch::inspection out;assert(dispatch.inspect("media",out)==error::success);return out;}
    b::usage used(std::string_view alias="output")
    {b::usage out;assert(buffers.inspect(alias,out)==error::success);return out;}
    std::uint64_t admissions(std::string_view alias)
    {uwvm2::utils::container::std_vector<s::authority_snapshot> out;assert(permissions->inspect(out,3));for(auto const& row:out)if(row.alias==alias)return row.admissions;assert(false);return 0;}
    s::handle output(std::vector<std::uint8_t> const& out)
    {
        r::response response;assert(r::decode_response(op::media_take,out,{2048,2048,255,16},response));
        assert(response.outcome.state=="succeeded"&&response.outcome.effect=="committed"&&response.outcome.provider_generation==71);
        assert(response.origin.source==input&&!response.origin.has_source_stamp&&!response.origin.has_received_stamp&&!response.origin.has_publisher_identity&&
               response.buffer.capability==output_cap&&response.media.width==2&&response.media.height==1&&response.media.channels==3);
        return response.buffer.resource;
    }
};
static void transformed_delivery_and_exact_recovery()
{
    fixture f;f.open();std::vector<std::uint8_t> out;auto submitted=f.submit();
    assert(f.call(op::media_submit,submitted,out,2048,[](auto){return false;})==error::io&&f.usage().completed==1&&f.used().objects==1);
    auto charged=f.admissions("media");current=1900;
    assert(f.call(op::media_submit,submitted,out)==error::success&&f.admissions("media")==charged&&f.usage().completed==1);
    auto previous=out;assert(f.call(op::media_submit,submitted,out)==error::success&&out==previous);
    assert(f.call(op::media_take,f.query(),out,1)==error::too_large&&f.usage().pending==1);
    assert(f.call(op::media_take,f.query(),out,2048,[](auto){return false;})==error::io&&f.usage().pending==1);
    bool nested{};assert(f.call(op::media_take,f.query(),out,2048,[&](auto bytes){std::vector<std::uint8_t> other;
        assert(f.call(op::media_take,f.query(),other)==error::again);out.assign(bytes.begin(),bytes.end());nested=true;return true;})==error::success&&nested);
    auto id=f.output(out);assert(!f.usage().pending&&f.usage().delivered==1&&!f.used().leases&&!f.used("input").leases);
    assert(f.buffers.read("output",id,0,6,[](auto data){return std::equal(data.begin(),data.end(),std::array<std::uint8_t,6>{3,2,1,6,5,4}.begin());})==error::success);
    assert(f.call(op::media_take,f.query(),out)==error::again);assert(f.buffers.close("output",id)==error::success&&!f.used().objects);
}
static void independent_actions_and_zero_rates()
{
    for(auto action:{"media.open","buffer.read","data.export","buffer.create","buffer.write","buffer.seal"})
    {fixture f{action};std::vector<std::uint8_t> out;assert(f.call(op::media_open,f.open_wire(),out)==error::denied&&!f.usage().streams&&!f.used().objects);}
    for(auto action:{"media.submit","media.take","media.close"})
    {fixture f{action};f.open();std::vector<std::uint8_t> out;
        auto op=action==std::string_view("media.submit")?op::media_submit:action==std::string_view("media.take")?op::media_take:op::media_close;
        auto request=op==op::media_submit?f.submit():op==op::media_close?f.close_wire():f.query();
        assert(f.call(op,request,out)==error::denied&&!f.usage().completed);}
    for(auto alias:{"media","input","output"})
    {fixture f{{},alias,true};std::vector<std::uint8_t> out;
        if(alias==std::string_view("media"))assert(f.call(op::media_open,f.open_wire(),out)==error::again);
        else{f.open();assert(f.call(op::media_submit,f.submit(),out)==error::again);}
        assert(!f.used().objects&&!f.usage().completed);}
}
static void publication_rollback_and_scope_revocation()
{
    for(int mode=0;mode<4;++mode)
    {fixture f;std::vector<std::uint8_t> out;
        assert(f.call(op::media_open,f.open_wire(),out,2048,[&](auto){if(mode==0)return false;if(mode==1)throw 1;
            assert(f.permissions->revoke(mode==2?"input":"output",[](auto const&) {})==error::success);return true;})!=error::success);
        assert(!f.usage().streams);}
    for(auto alias:{"input","output","media"})
    {fixture f;f.open();std::vector<std::uint8_t> out;assert(f.call(op::media_submit,f.submit(),out)==error::success);
        assert(f.call(op::media_take,f.query(),out,2048,[&](auto){assert(f.permissions->revoke(alias,[](auto const&) {})==error::success);return true;})!=error::success);
        assert(f.usage().pending==1&&f.usage().delivered==0);
        f.dispatch.revoke("media");assert(!f.usage().pending&&!f.used().objects&&!f.used("input").leases);}
}
static void closure_deadline_bounds_and_cancellation()
{
    fixture f;f.open();std::vector<std::uint8_t> out;auto before=f.admissions("media");
    assert(f.call(op::media_submit,f.submit(),out,1)==error::too_large&&f.admissions("media")==before);
    current=1800;assert(f.call(op::media_submit,f.submit(),out)==error::timeout&&!f.used().objects);current=1000;
    assert(f.call(op::media_submit,f.submit(),out,2048,{},[]{return true;})==error::cancelled&&!f.used().objects&&!f.usage().pending&&!f.used("input").leases);
    assert(f.call(op::media_submit,f.submit(),out)==error::success);assert(f.call(op::media_submit,f.submit(3),out)==error::conflict&&f.usage().completed==1);
    auto close=f.close_wire();assert(f.call(op::media_close,close,out,2048,[](auto){return false;})==error::io&&!f.used().objects&&!f.usage().pending);
    current=1900;assert(f.call(op::media_close,close,out)==error::success);auto saved=out;
    assert(f.call(op::media_close,close,out)==error::success&&out==saved);
    assert(f.call(op::media_submit,f.submit(2),out)==error::timeout);
    assert(f.dispatch.close(f.who,f.cap,f.stream)==error::success&&!f.usage().streams&&f.used("input").objects==1);
    auto policy=plan();policy.frames=0;assert(s::media_dispatch::required_storage(policy,policy.storage_bytes));fixture empty{{},{},false,policy};empty.open();
    assert(empty.call(op::media_submit,empty.submit(),out)==error::again&&!empty.used().objects);
    policy=plan();policy.frames=UINT64_MAX;std::uint64_t ignored{};assert(!s::media_dispatch::required_storage(policy,ignored));
}
static void staged_buffer_transaction()
{
    fixture f;b::information source;assert(f.buffers.describe("input",f.input,source)==error::success);source.value.schema=f.policy.output_profile;source.value.color=b::color_space::bgr;
    {b::registry::sealed_creation transaction;assert(f.buffers.stage_sealed("output",source.value,4,f.bytes,transaction)==error::success);
        b::information result;assert(f.buffers.describe("output",transaction.identity(),result)==error::bad_handle&&f.used().objects==1&&f.used().active_handles==0&&f.used().leases==1);}
    assert(!f.used().objects&&!f.used().leases);
    b::registry::sealed_creation transaction;assert(f.buffers.stage_sealed("output",source.value,4,f.bytes,transaction)==error::success);
    b::registry::sealed_lease retained;assert(f.buffers.publish_sealed(transaction,retained)==error::success);auto id=retained.identity();
    assert(f.used().active_handles==1&&f.buffers.close("output",id)==error::success&&f.used().objects==1);
    retained={};assert(!f.used().objects);
}
static void concurrency_and_fork()
{
    fixture f;f.open();std::atomic<unsigned> done{};std::vector<std::thread> threads;
    for(int i=0;i<8;++i)threads.emplace_back([&]{std::vector<std::uint8_t> out;auto status=f.call(op::media_submit,f.submit(),out);
        assert(status==error::success||status==error::again||status==error::conflict);if(status==error::success)++done;});
    for(auto& thread:threads){thread.join();}assert(done&&f.usage().completed==1&&f.usage().pending==1&&f.used().objects==1);
    auto child=::fork();assert(child>=0);if(!child){std::vector<std::uint8_t> out;::_exit(f.call(op::media_take,f.query(),out)==error::denied?0:1);}
    int result{};assert(::waitpid(child,&result,0)==child&&WIFEXITED(result)&&!WEXITSTATUS(result));
}
static void recoverable_allocation_and_configuration()
{
#if defined(UWVM_MEDIA_FAULT_INJECTION)
    for(int allocation=0;allocation<3;++allocation)
    {
        fixture f;f.open();auto input=f.submit();std::vector<std::uint8_t> out;auto before=f.admissions("media");
        fail_allocation=allocation;auto status=f.call(op::media_submit,input,out);fail_allocation=-1;
        assert(status==error::no_memory&&!f.used().objects&&!f.used("input").leases&&!f.usage().pending&&!f.usage().completed);
        assert(f.admissions("media")==before&&f.call(op::media_submit,input,out)==error::success);
    }
#endif
    fixture f;s::media_configuration settings;assert(settings.limit("streams",1)==error::denied);
    auto definition=f.encode([&](auto& out){return out.text(s::media_configuration::magic)&&out.text(f.policy.selected.identity.view())&&
        out.text("input")&&s::proposals::write_profile(out,f.policy.input_profile)&&out.text("output")&&s::proposals::write_profile(out,f.policy.output_profile)&&
        out.text("swap_red_blue")&&out.text("rgb")&&out.text("bgr")&&out.u64(2)&&out.u64(1)&&out.u64(3);});
    auto selected=f.policy.selected;selected.sha256=b::hash(definition);assert(settings.profile("media",selected)==error::success);
    auto broken=definition;broken.back()^=1;assert(settings.definition(broken)==error::invalid&&settings.definition_pending());
    assert(settings.definition(definition)==error::success&&!settings.definition_pending());s::media_dispatch::plan result;
    assert(settings.freeze(result)==error::invalid);std::array<std::uint64_t,6> values{2,2,f.policy.storage_bytes,8192,8,1000};
    for(std::size_t i=0;i<values.size();++i)assert(settings.limit(s::media_configuration::names[i],values[i])==error::success);
    assert(settings.limit("streams",2)==error::conflict&&settings.freeze(result)==error::success&&result.selected==selected);
    assert(settings.limit("workspace.call",2)==error::invalid&&settings.profile("media",selected)==error::conflict);
}
int main()
{transformed_delivery_and_exact_recovery();independent_actions_and_zero_rates();publication_rollback_and_scope_revocation();
 closure_deadline_bounds_and_cancellation();staged_buffer_transaction();concurrency_and_fork();recoverable_allocation_and_configuration();
 std::cout<<"passed 7 media execution safety groups\n";}
