/** @file @brief Actual sealed buffers, independent authority, caller ownership and recording recovery. */
#include <uwvm2/imported/ros/scene/recording_dispatch.h>
#include <cassert>
#include <iostream>
#include <thread>
#include <barrier>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;
namespace r=s::recording;
namespace c=uwvm2::imported::ros::recording_contract;
using s::error;using op=c::operation;
static thread_local std::size_t allocations{},fail_after{};
static bool fail() noexcept{++allocations;return fail_after&&!--fail_after;}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return fail()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t z){return fail()?nullptr:__real_calloc(n,z);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return fail()?nullptr:__real_realloc(p,n);}
struct packet
{
    std::array<std::uint8_t,8192> data{};std::size_t size{};
    s::wire::bytes view()const noexcept{return {data.data(),size};}
    bool copy(s::wire::bytes bytes){assert(bytes.size()<=data.size());std::copy(bytes.begin(),bytes.end(),data.begin());size=bytes.size();return true;}
};
template<typename F>static packet encode(F&& f){packet p;s::wire::writer w{p.data};f(w);p.size=w.size();return p;}
static s::buffers::profile profile(std::string_view name)
{s::buffers::profile p;assert(p.identity.assign(name));p.sha256[0]=3;return p;}
static bool valid(s::wire::bytes input)noexcept{return input.size()==1&&input[0]<=1;}
static auto channels()
{return std::array{r::mcap::channel{1,1,"/state","std_msgs/Bool","ros1msg","ros1","",r::mcap::raw("bool data\n"),valid,1}};}
static r::plan recording_plan()
{r::plan p{2,1024,4,2,512,4,0};std::uint64_t defs{};assert(r::store::required_storage(p,"ros1",channels(),p.reserved_bytes,defs));return p;}
static auto sources()
{s::recording_dispatch::channel_source source{1,{},profile("bool.bytes")};assert(source.alias.assign("input"));return std::array{source};}
static s::buffers::plan buffer_plan()
{
    s::buffers::plan p;p.schema=profile("bool.bytes");p.objects=8;p.bytes=16;p.object_bytes=4;p.transfer_bytes=4;
    p.workspace_bytes=32;p.leases=128;p.elements=4;p.element_mask=p.order_mask=p.color_mask=1;p.rank=1;return p;
}
struct fixture
{
    std::shared_ptr<s::authority> authority=uwvm2::utils::container::make_shared<s::authority>();
    s::buffers::registry buffers;
    s::recording_dispatch dispatch{authority,buffers,77,{2,2,1024*1024,32768,8}};
    int instance{},other_instance{};s::caller who{&instance,1},other{&other_instance,1};
    s::handle cap{},replay_cap{},buffer_cap{},wrong_cap{},buffer{},writer{};packet output;
    std::size_t required{};
    explicit fixture(bool export_input=true,bool observe=true,bool export_recording=true,bool zero=false,bool exposed=true,std::uint64_t calls=UINT64_MAX,std::uint64_t resources=UINT64_MAX)
    {
        assert(authority->expose("capability.open",s::exposure::allow)==error::success&&authority->expose("capability.close",s::exposure::allow)==error::success);
        assert(authority->expose("resource.close",s::exposure::allow)==error::success&&authority->expose("buffer.read",s::exposure::allow)==error::success);
        if(exposed)for(auto descriptor:c::operations)assert(authority->expose(uwvm2::utils::container::std_string{descriptor.name},s::exposure::allow)==error::success);
        s::grant recording;recording.alias="recording";recording.resource="private";recording.binding="private-recording";recording.kind=s::purpose::recording;
        recording.actions={"recording.open","recording.append","recording.checkpoint","recording.stop","resource.close"};
        if(observe)recording.actions.emplace("resource.observe");
        if(export_recording)recording.actions.emplace("data.export");
        recording.max_resources=resources;
        if(calls!=UINT64_MAX){recording.limits.calls=calls;recording.limits.uncertain=true;}
        assert(authority->configure(recording)==error::success);
        auto wrong=recording;wrong.alias="wrong";wrong.limits.calls.reset();wrong.limits.uncertain=false;assert(authority->configure(wrong)==error::success);
        s::grant replay;replay.alias="replay";replay.resource="private";replay.binding="private-replay";replay.kind=s::purpose::replay;
        replay.actions={"replay.open","replay.read","replay.seek","resource.close"};assert(authority->configure(replay)==error::success);
        s::grant input;input.alias="input";input.resource="bytes";input.binding="sealed-buffer";input.kind=s::purpose::buffer;input.actions={"buffer.read"};
        if(export_input)input.actions.emplace("data.export");
        if(zero){input.limits.calls=0;input.limits.uncertain=true;}
        assert(authority->configure(input)==error::success);
        assert(authority->enroll(who)==error::success&&authority->enroll(other)==error::success);
        assert(authority->open(who,"recording",cap)==error::success&&authority->open(who,"replay",replay_cap)==error::success&&
               authority->open(who,"input",buffer_cap)==error::success&&authority->open(who,"wrong",wrong_cap)==error::success);
        assert(buffers.bind("input",buffer_plan())==error::success);
        assert(dispatch.bind_recording("recording",profile("recording"),recording_plan(),"ros1",channels(),sources())==error::success);
        assert(dispatch.bind_recording("wrong",profile("recording"),recording_plan(),"ros1",channels(),sources())==error::success);
        assert(dispatch.bind_replay("replay","recording",{profile("replay"),2})==error::success&&dispatch.prepare()==error::success);
        buffer=make_buffer(1);
    }
    s::handle make_buffer(std::uint8_t value,bool sealed=true)
    {
        s::buffers::layout layout;layout.schema=profile("bool.bytes");layout.rank=1;layout.shape[0]=layout.strides[0]=layout.bytes=1;s::handle id{};
        assert(buffers.create("input",layout,8,[&](auto created){id=created;return true;})==error::success);
        std::uint64_t accepted{};assert(buffers.write("input",id,0,{&value,1},accepted)==error::success&&accepted==1);
        if(sealed){auto sha=s::buffers::hash({&value,1});assert(buffers.seal("input",id,sha)==error::success);}return id;
    }
    error call(op action,packet const& request,std::size_t capacity=8192,s::recording_dispatch::sink sink={},s::recording_dispatch::cancellation cancel={})
    {auto copy=[&](s::wire::bytes p){return output.copy(p);};return dispatch.invoke(who,action,action<=op::recording_stop?cap:replay_cap,request.view(),capacity,required,sink?sink:s::recording_dispatch::sink{copy},cancel);}
    packet open_request(bool replay=false)
    {return encode([&](auto& w){auto p=profile(replay?"replay":"recording");assert(w.text(p.identity.view())&&w.blob(p.sha256));});}
    void open(){assert(call(op::recording_open,open_request())==error::success);s::wire::reader in{output.view()};assert(in.u64(writer)&&in.done());}
    packet append(std::uint64_t key=1,s::handle source=0,s::handle id=0,std::uint64_t channel=1)
    {
        auto selected=id?id:buffer;s::buffers::information info;assert(buffers.describe("input",selected,info)==error::success);
        return encode([&](auto& w){assert(w.u64(writer)&&w.u64(key)&&w.u64(1)&&w.u64(source?source:buffer_cap)&&w.u64(selected)&&w.u64(channel)&&
            w.u64(9)&&w.u64(100)&&w.u64(200)&&w.u64(1)&&w.blob(info.sha256));});
    }
    packet mutation(std::uint64_t key,bool stop=false,bool drain=true)
    {return encode([&](auto& w){assert(w.u64(writer)&&w.u64(key)&&(!stop||w.boolean(drain)));});}
    r::object_info checkpoint(std::uint64_t key=2)
    {
        assert(call(op::recording_checkpoint,mutation(key))==error::success);s::wire::reader in{output.view()};std::uint64_t ignored{};
        for(unsigned i{};i<4;++i){assert(in.u64(ignored));}
        r::object_info object;s::wire::bytes sha;
        assert(in.u64(object.id)&&in.u64(object.bytes)&&in.u64(object.messages)&&in.blob(sha)&&sha.size()==32);std::copy(sha.begin(),sha.end(),object.sha256.begin());return object;
    }
    packet replay_request(r::object_info object,s::handle source=0)
    {return encode([&](auto& w){auto p=profile("replay");assert(w.u64(source?source:cap)&&w.text(p.identity.view())&&w.blob(p.sha256)&&w.u64(1)&&
        w.u64(object.id)&&w.u64(object.bytes)&&w.u64(object.messages)&&w.blob(object.sha256));});}
    s::handle replay(r::object_info object)
    {assert(call(op::replay_open,replay_request(object))==error::success);s::wire::reader in{output.view()};s::handle id{};assert(in.u64(id)&&in.done());return id;}
    static packet read(s::handle id,std::uint64_t count=1){return encode([&](auto& w){assert(w.u64(id)&&w.u64(count));});}
    r::usage usage(){r::usage value;assert(dispatch.inspect("recording",value)==error::success);return value;}
    std::uint64_t admitted(std::string_view alias)
    {uwvm2::utils::container::std_vector<s::authority_snapshot> rows;assert(authority->inspect(rows,8));for(auto const& row:rows)if(row.alias==alias)return row.admissions;assert(false);return 0;}
};
static void actual_record_replay_and_close()
{
    fixture f;f.open();assert(f.call(op::recording_append,f.append())==error::success&&f.usage().active_messages==1);
    auto object=f.checkpoint();auto id=f.replay(object);assert(f.usage().pins==1&&f.usage().replays==1);
    s::recording_dispatch::inspection recording_view,replay_view;
    auto admissions=f.admitted("recording"),replay_admissions=f.admitted("replay");
    assert(f.dispatch.inspect("recording",recording_view)==error::success&&!recording_view.replay&&recording_view.open_handles==2&&recording_view.store.pins==1);
    assert(f.dispatch.inspect("replay",replay_view)==error::success&&replay_view.replay&&replay_view.open_handles==1&&replay_view.reserved_bytes>0);
    assert(f.admitted("recording")==admissions&&f.admitted("replay")==replay_admissions);
    auto unchanged=replay_view;assert(f.dispatch.inspect("missing",replay_view)==error::bad_handle&&replay_view.open_handles==unchanged.open_handles);
    assert(f.dispatch.close(f.who,f.cap,object.id)==error::again);
    assert(f.call(op::replay_read,f.read(id))==error::success);s::wire::reader in{f.output.view()};std::uint64_t ordinal{},count{},channel{},seq{},log{},publish{};s::wire::bytes body;
    assert(in.u64(ordinal)&&!ordinal&&in.u64(count)&&count==1&&in.u64(channel)&&channel==1&&in.u64(seq)&&seq==9&&in.u64(log)&&log==100&&in.u64(publish)&&publish==200&&in.blob(body)&&body.size()==1&&body[0]==1&&in.done());
    assert(f.call(op::replay_read,f.read(id))==error::success&&f.output.size==16);
    assert(f.call(op::replay_seek,f.read(id,0))==error::success&&f.call(op::replay_read,f.read(id))==error::success&&f.output.size==57);
    assert(f.dispatch.close(f.who,f.replay_cap,id)==error::success&&!f.usage().pins);
    assert(f.dispatch.inspect("replay",replay_view)==error::success&&!replay_view.open_handles);
    assert(f.dispatch.close(f.who,f.cap,object.id)==error::success&&!f.usage().sealed_segments);
    assert(f.call(op::replay_open,f.replay_request(object))==error::bad_handle);
    assert(f.dispatch.close(f.who,f.cap,f.writer)==error::success&&!f.usage().recording);
    for(bool drain:{false,true})
    {
        f.open();assert(f.call(op::recording_append,f.append())==error::success);
        auto stop=f.mutation(2,true,drain);assert(f.call(op::recording_stop,stop)==error::success&&!f.usage().recording);
        auto charges=f.admitted("recording");assert(f.call(op::recording_stop,stop)==error::success&&f.admitted("recording")==charges);
        assert(f.call(op::recording_stop,f.mutation(2,true,!drain))==error::conflict);
        assert(f.usage().sealed_segments==(drain?1u:0u)&&!f.usage().active_messages);
        assert(f.dispatch.close(f.who,f.cap,f.writer)==error::success);
    }
}
static void exact_retry_and_capacity()
{
    fixture f{true,true,true,false,true,2};f.open();auto append=f.append();
    assert(f.call(op::recording_append,append,8192,[](auto){return false;})==error::io&&f.usage().active_messages==1);
    assert(f.admitted("recording")==2&&f.admitted("input")==2);
    assert(f.buffers.close("input",f.buffer)==error::success);
    assert(f.call(op::recording_append,append)==error::success&&f.admitted("recording")==2&&f.admitted("input")==2);
    auto changed=append;changed.data[48]^=1;assert(f.call(op::recording_append,changed)==error::conflict);
    assert(f.call(op::recording_checkpoint,f.mutation(2))==error::again&&!f.usage().sealed_segments);
    fixture capped{true,true,true,false,true,UINT64_MAX,1};capped.open();assert(capped.call(op::recording_append,capped.append())==error::success);
    assert(capped.call(op::recording_checkpoint,capped.mutation(2))==error::again&&!capped.usage().sealed_segments);
    fixture output;assert(output.call(op::recording_open,output.open_request(),8192,[](auto){return false;})==error::io&&!output.usage().recording);output.open();
    assert(output.call(op::recording_append,output.append())==error::success);
    assert(output.call(op::recording_checkpoint,output.mutation(2),8192,[](auto){return false;})==error::io&&output.usage().sealed_segments==1);
    auto before=output.admitted("recording");auto object=output.checkpoint();assert(output.admitted("recording")==before&&output.usage().sealed_segments==1);
    assert(output.call(op::replay_open,output.replay_request(object),8192,[](auto){return false;})==error::io&&!output.usage().pins&&!output.usage().replays);
}
static void rights_and_short_outputs()
{
    fixture hidden{true,true,true,false,false};assert(hidden.call(op::recording_open,hidden.open_request())==error::denied);
    for(unsigned mode{};mode<4;++mode)
    {
        fixture f{mode!=0,mode!=1,mode!=2,mode==3};f.open();auto before=f.admitted("recording");
        if(mode==0||mode==3){assert(f.call(op::recording_append,f.append())==(mode==0?error::denied:error::again)&&!f.usage().active_messages&&f.admitted("recording")==before);continue;}
        assert(f.call(op::recording_append,f.append())==error::success);auto object=f.checkpoint();before=f.admitted("replay");
        assert(f.call(op::replay_open,f.replay_request(object))==error::denied&&f.admitted("replay")==before&&!f.usage().pins);
    }
    fixture f;assert(f.call(op::recording_open,f.open_request(),7)==error::too_large&&!f.admitted("recording")&&!f.usage().recording);f.open();
    auto before=f.admitted("recording");assert(f.call(op::recording_append,f.append(),103)==error::too_large&&f.admitted("recording")==before&&!f.admitted("input"));
    assert(f.call(op::recording_append,f.append(1,f.wrong_cap))==error::denied&&f.admitted("recording")==before);
    assert(f.call(op::recording_append,f.append(1,0,0,2))==error::denied&&!f.usage().active_messages);
    auto unsealed=f.make_buffer(1,false);assert(f.call(op::recording_append,f.append(1,0,unsealed))==error::denied);
    assert(f.call(op::recording_append,f.append())==error::success);auto object=f.checkpoint();
    assert(f.call(op::replay_open,f.replay_request(object,f.wrong_cap))==error::denied);
    auto wrong=object;wrong.sha256[0]^=1;assert(f.call(op::replay_open,f.replay_request(wrong))==error::bad_handle);
    auto id=f.replay(object);before=f.admitted("replay");assert(f.call(op::replay_read,f.read(id),56)==error::too_large&&f.required==57&&f.admitted("replay")==before);
    assert(f.call(op::replay_read,f.read(id))==error::success&&f.output.size==57);
}
static void caller_and_capability_lifetimes()
{
    fixture f;f.open();auto append=f.append();auto original=f.who;f.who=f.other;
    assert(f.call(op::recording_append,append)==error::bad_handle&&f.dispatch.close(f.who,f.cap,f.writer)==error::bad_handle);
    f.who=original;assert(f.call(op::recording_append,append)==error::success);auto object=f.checkpoint();f.who=f.other;
    assert(f.call(op::replay_open,f.replay_request(object))==error::bad_handle&&f.dispatch.close(f.who,f.cap,object.id)==error::bad_handle);
    f.who=original;auto id=f.replay(object);f.who=f.other;assert(f.call(op::replay_read,f.read(id))==error::bad_handle);
    f.who=original;assert(f.authority->close(f.who,f.cap)==error::success);
    assert(f.call(op::replay_read,f.read(id))==error::bad_handle&&f.usage().pins==1);
    assert(f.dispatch.close(f.who,f.replay_cap,id)==error::success&&!f.usage().pins);
    assert(f.authority->open(f.who,"recording",f.cap)==error::success&&f.dispatch.close(f.who,f.cap,object.id)==error::success);
}
static void delivery_failure_and_local_fence()
{
    for(unsigned stage=1;stage<=4;++stage)
    {
        fixture f;f.open();auto append=f.append();unsigned calls{};bool copied{};
        auto cancel=[&]{if(++calls==stage)assert(f.authority->revoke("input",[](auto){})==error::success);return false;};
        auto sink=[&](auto){copied=true;return true;};
        assert(f.call(op::recording_append,append,8192,sink,cancel)==error::denied);
        assert(f.usage().active_messages==(stage>=3?1u:0u)&&copied==(stage==4));
    }
    fixture f;f.open();assert(f.call(op::recording_append,f.append())==error::success);auto object=f.checkpoint();auto id=f.replay(object);
    assert(f.call(op::replay_read,f.read(id),8192,[](auto)->bool{throw 4;})==error::io);
    assert(f.call(op::replay_read,f.read(id),8192,[](auto){return false;})==error::io);
    assert(f.call(op::replay_read,f.read(id))==error::success&&f.output.size==57);
    assert(f.call(op::replay_seek,f.read(id,0))==error::success);
    unsigned calls{};bool copied{};auto cancel=[&]{if(++calls==3)f.dispatch.revoke("replay");return false;};
    auto sink=[&](auto){copied=true;return true;};
    assert(f.call(op::replay_read,f.read(id),8192,sink,cancel)==error::cancelled&&!copied&&!f.usage().pins);
}
static void concurrent_retire_and_reentry()
{
    fixture f;f.open();assert(f.call(op::recording_append,f.append())==error::success);auto id=f.replay(f.checkpoint());
    auto sink=[&](auto){packet out;std::size_t needed{};auto request=f.read(id);auto copy=[&](auto bytes){return out.copy(bytes);};
        assert(f.dispatch.invoke(f.who,op::replay_read,f.replay_cap,request.view(),8192,needed,copy)==error::again);
        assert(f.dispatch.invoke(f.who,op::replay_seek,f.replay_cap,f.read(id,0).view(),8192,needed,copy)==error::again);return false;};
    assert(f.call(op::replay_read,f.read(id),8192,sink)==error::io);
    std::barrier rendezvous{2};error result{};
    std::thread reader{[&]{auto block=[&](auto){rendezvous.arrive_and_wait();rendezvous.arrive_and_wait();return true;};result=f.call(op::replay_read,f.read(id),8192,block);}};
    rendezvous.arrive_and_wait();f.dispatch.retire();rendezvous.arrive_and_wait();reader.join();assert(result==error::cancelled&&f.usage().retired);
}
static void allocation_and_fork()
{
    std::size_t failures{};
    for(std::size_t n=1;n<24;++n)
    {
        auto authority=uwvm2::utils::container::make_shared<s::authority>();s::buffers::registry buffers;
        s::recording_dispatch dispatch{authority,buffers,1,{1,1,1024*1024,8192,2}};auto policy=recording_plan();auto source=sources();auto channel=channels();auto p=profile("recording");
        allocations=0;fail_after=n;auto result=dispatch.bind_recording("recording",p,policy,"ros1",channel,source);fail_after=0;
        if(result==error::success)break;
        assert(result==error::no_memory);++failures;
        assert(dispatch.bind_recording("recording",p,policy,"ros1",channel,source)==error::success);
    }
    assert(failures==12);
    fixture f;f.open();auto append=f.append(),checkpoint=f.mutation(2);allocations=0;fail_after=1;
    assert(f.call(op::recording_append,append)==error::success&&f.call(op::recording_checkpoint,checkpoint)==error::success);fail_after=0;assert(!allocations);
    s::wire::reader in{f.output.view()};std::uint64_t ignored{};for(unsigned i{};i<4;++i){assert(in.u64(ignored));}
        r::object_info object;s::wire::bytes sha;
    assert(in.u64(object.id)&&in.u64(object.bytes)&&in.u64(object.messages)&&in.blob(sha));std::copy(sha.begin(),sha.end(),object.sha256.begin());
    auto id=f.replay(object);auto request=f.read(id);allocations=0;fail_after=1;assert(f.call(op::replay_read,request)==error::no_memory);fail_after=0;assert(allocations==1);
    assert(f.call(op::replay_read,request)==error::success&&f.output.size==57);
    auto pid=::fork();assert(pid>=0);if(!pid){bool denied=f.call(op::replay_read,request)==error::denied&&f.dispatch.close(f.who,f.replay_cap,id)==error::denied;::_exit(denied?0:1);}
    int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&!WEXITSTATUS(status));
}
static void complete_sources_and_hostile_configuration()
{
    fixture f;f.open();auto first=f.append();auto combined=first;
    combined.data[16]=2;std::copy(first.data.begin()+24,first.data.begin()+first.size,combined.data.begin()+first.size);combined.size+=first.size-24;
    // The final source digest is wrong: the earlier valid source must not be appended or charged.
    combined.data[combined.size-1]^=1;auto before=f.admitted("recording");
    assert(f.call(op::recording_append,combined)==error::conflict&&!f.usage().active_messages&&f.admitted("recording")==before&&!f.admitted("input"));
    combined.data[combined.size-1]^=1;assert(f.call(op::recording_append,combined)==error::success&&f.usage().active_messages==2&&f.admitted("input")==4);
    fixture retired;retired.open();auto request=retired.append();unsigned visits{};
    auto cancellation=[&]{if(++visits==2)retired.buffers.revoke("input");return false;};
    assert(retired.call(op::recording_append,request,8192,{},cancellation)==error::denied&&!retired.usage().active_messages&&!retired.admitted("input"));
    auto authority=uwvm2::utils::container::make_shared<s::authority>();s::buffers::registry buffers;
    s::recording_dispatch disabled{authority,buffers,1,{}};
    assert(disabled.bind_recording("empty",profile("recording"),recording_plan(),"ros1",channels(),sources())==error::too_large);
    s::recording_dispatch dispatch{authority,buffers,1,{1,1,1024*1024,8192,2}};
    auto malformed=profile("recording");malformed.identity.size=SIZE_MAX;
    assert(dispatch.bind_recording("recording",malformed,recording_plan(),"ros1",channels(),sources())==error::invalid);
    auto source=sources();source[0].alias.size=SIZE_MAX;
    assert(dispatch.bind_recording("recording",profile("recording"),recording_plan(),"ros1",channels(),source)==error::invalid);
    assert(dispatch.bind_recording("recording",profile("recording"),recording_plan(),"ros1",channels(),sources())==error::success);
    assert(dispatch.bind_replay("replay","recording",{malformed,1})==error::invalid);
}
int main()
{
    actual_record_replay_and_close();exact_retry_and_capacity();rights_and_short_outputs();caller_and_capability_lifetimes();
    delivery_failure_and_local_fence();concurrent_retire_and_reentry();allocation_and_fork();complete_sources_and_hostile_configuration();
    std::cout<<"passed 8 recording dispatch security groups\n";
}
