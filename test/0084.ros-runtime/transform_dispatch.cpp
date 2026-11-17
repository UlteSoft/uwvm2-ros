/** @file @brief Exercise canonical TF against actual authority and runtime clocks. */
#include <uwvm2/imported/ros/scene/transform_dispatch.h>
#include <cassert>
#include <iostream>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;
namespace t=s::transforms;
namespace c=uwvm2::imported::ros::transform_contract;
using s::error;
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
static c::name name(std::string_view text){c::name value;assert(value.assign(text));return value;}
template<typename Fn> static bytes encode(Fn fn)
{std::array<std::uint8_t,8192> value{};s::wire::writer out{value};fn(out);return {out.written().begin(),out.written().end()};}
static std::uint64_t identity(bytes const& value){s::wire::reader in{value};std::uint64_t id{};assert(in.u64(id));return id;}
static t::plan plan()
{
    t::plan result;auto& s=result.session;auto& p=s.history;assert(s.profile.identity.assign("frames"));s.profile.sha256.fill(1);
    p.frames={name("base"),name("camera"),name("map")};p.frame_count=3;
    p.edges={t::edge{2,0,true,false},t::edge{0,1,false,true}};p.edge_count=2;
    p.max_samples=16;p.samples_per_child=8;p.max_batch=8;p.history_ns=10000000000;p.geometry={1000,1e-12};p.velocity_window_ns=1000000000;
    p.linear_speed=p.angular_speed=10;assert(t::cache::bank_bytes(p,p.data_bytes));p.workspace_bytes=p.data_bytes;
    s.requests=4;s.deadline_ns=1000000000;assert(t::session::request_storage(s,s.request_bytes));
    result.sessions=2;result.data_bytes=p.data_bytes*2;result.workspace_bytes=p.workspace_bytes*2;result.request_bytes=s.request_bytes*2;result.carrier_bytes=8192;return result;
}
struct fixture
{
    std::shared_ptr<s::authority> authority=uwvm2::utils::container::make_shared<s::authority>();
    uwvm2::imported::ros::resources::registry clocks;s::transform_dispatch dispatch{authority,clocks};
    int instance{};s::caller who{&instance,1};s::handle cap{},clock_cap{},clock{},buffer{};std::uint64_t epoch{};
    fixture(bool replacement=true,bool exposed=true,bool zero=false,bool ros=false)
    {
        for(auto action:{"capability.open","capability.close","clock.create","clock.read","request.describe","request.release"})
            assert(authority->expose(action,s::exposure::allow)==error::success);
        if(exposed)for(auto const& action:c::operations)assert(authority->expose(uwvm2::utils::container::std_string(action.name),s::exposure::allow)==error::success);
        s::grant transform;transform.alias="frames";transform.resource="frames";transform.binding="local-transform";transform.kind=s::purpose::transform;transform.max_resources=2;
        for(auto const& action:c::operations)transform.actions.emplace(action.name);
        for(auto action:{"resource.close","request.describe","request.release"})transform.actions.emplace(action);
        if(replacement)transform.actions.emplace("transform.static.replace");
        if(zero){transform.limits.calls=0;transform.limits.uncertain=true;}
        s::grant timing;timing.alias="clock";timing.resource="clock";timing.binding="local";timing.kind=ros?s::purpose::ros_clock:s::purpose::steady_clock;
        timing.actions={"clock.create","clock.read","resource.close"};timing.limits.calls=1;timing.limits.window=std::chrono::hours{1};timing.limits.uncertain=true;
        assert(authority->configure(transform)==error::success&&authority->configure(timing)==error::success&&authority->enroll(who)==error::success);
        assert(authority->open(who,"frames",cap)==error::success&&authority->open(who,"clock",clock_cap)==error::success);
        assert(dispatch.bind("frames",plan())==error::success);s::lease lease;
        assert(authority->admit(who,clock_cap,"clock.create",8,lease)==error::success);
        auto kind=ros?uwvm2::imported::ros::resources::clock_kind::ros:uwvm2::imported::ros::resources::clock_kind::steady;
        assert(clocks.clock_create("clock",1,kind,clock)==error::success);
        if(ros){assert(clocks.clock_update("clock",clock,10000000000,true)==error::success);epoch=1;}
    }
    bytes opening(s::handle source=0)
    {
        auto profile=plan().session.profile;
        return encode([&](auto& out){assert(out.text(profile.identity.view())&&out.blob(profile.sha256)&&out.u64(source?source:clock_cap)&&out.u64(clock));});
    }
    error call(c::operation op,bytes const& input,bytes& output,std::size_t capacity=1024,s::transform_dispatch::sink sink={},s::transform_dispatch::cancelled cancel={})
    {
        auto copy=[&](s::wire::bytes data){output.assign(data.begin(),data.end());return true;};std::size_t required{};
        return dispatch.invoke(who,op,cap,input,capacity,required,sink?sink:s::transform_dispatch::sink{copy},cancel);
    }
    void open(){bytes output;assert(call(c::operation::open,opening(),output)==error::success);buffer=identity(output);}
    bytes publication(std::uint64_t key,std::int64_t time=10,double x=10,bool fixed=false,std::uint64_t generation=0)
    {
        c::transform value{name(fixed?"base":"map"),name(fixed?"camera":"base"),{clock,epoch,time},{{x,0,0},{}},fixed};
        return encode([&](auto& out){assert(out.u64(buffer)&&out.u64(key)&&out.u64(buffer)&&out.u64(generation)&&out.u64(1)&&c::write_transform(out,value));});
    }
    void write_query(s::wire::writer& out,std::int64_t time=10,std::string_view source="base",std::string_view target="map")
    {assert(out.text(target)&&out.boolean(true)&&c::write_stamp(out,{clock,epoch,time})&&out.text(source)&&out.boolean(false)&&out.boolean(false));}
    bytes query(std::int64_t time=10,std::string_view source="base")
    {return encode([&](auto& out){assert(out.u64(buffer));write_query(out,time,source);});}
    bytes submission(std::uint64_t key,std::int64_t time=10)
    {return encode([&](auto& out){assert(out.u64(buffer)&&out.u64(key)&&out.u64(buffer));write_query(out,time);assert(out.u64(uwvm2::imported::ros::resources::steady_now()+500000000));});}
    bytes take(std::uint64_t request)
    {return encode([&](auto& out){assert(out.u64(buffer)&&out.u64(request));});}
    std::uint64_t admitted(std::string_view alias)
    {uwvm2::utils::container::std_vector<s::authority_snapshot> rows;assert(authority->inspect(rows,2));for(auto const& row:rows)if(row.alias==alias)return row.admissions;assert(false);return 0;}
    t::usage usage(){t::usage value;assert(dispatch.inspect("frames",value)==error::success);return value;}
};
static void default_denial_short_output_and_clock_budget()
{
    bytes out;fixture hidden{true,false};assert(hidden.call(c::operation::open,hidden.opening(),out)==error::denied&&!hidden.usage().retained_sessions);
    fixture zero{true,true,true};assert(zero.call(c::operation::open,zero.opening(),out)==error::again&&!zero.usage().retained_sessions);
    fixture f;assert(f.call(c::operation::open,f.opening(),out,7)==error::too_large&&!f.admitted("frames")&&!f.usage().retained_sessions);
    assert(f.call(c::operation::open,f.opening(f.cap),out)==error::denied&&!f.admitted("frames"));
    assert(f.call(c::operation::open,f.opening(),out,8,[](auto){return false;})==error::io&&!f.usage().retained_sessions);
    f.open();assert(f.admitted("clock")==1); // Existing clock creation quota is exhausted, validation still works.
    auto input=f.publication(1);auto before=f.admitted("frames");
    assert(f.call(c::operation::publish,input,out,159)==error::too_large&&f.admitted("frames")==before&&!f.usage().samples);
    assert(f.call(c::operation::publish,input,out)==error::success&&f.usage().samples==1&&f.admitted("clock")==1);
    assert(f.call(c::operation::lookup,f.query(),out)==error::success);
    assert(f.call(c::operation::available,f.query(11),out)==error::success&&identity(out)==0);
    assert(f.call(c::operation::available,f.query(10,"private"),out)==error::denied);
}
static void static_generation_and_exact_receipt_recovery()
{
    fixture denied{false};denied.open();bytes out;
    assert(denied.call(c::operation::publish,denied.publication(1,0,1,true),out)==error::success);
    assert(denied.call(c::operation::publish,denied.publication(2,0,2,true,2),out)==error::denied&&denied.usage().samples==1);
    fixture f;f.open();assert(f.call(c::operation::publish,f.publication(1,0,1,true),out)==error::success);
    assert(f.call(c::operation::publish,f.publication(2,0,2,true),out)==error::conflict);
    assert(f.call(c::operation::publish,f.publication(2,0,2,true,1),out)==error::conflict);
    auto request=f.publication(2,0,2,true,2);
    assert(f.call(c::operation::publish,request,out,1024,[](auto){return false;})==error::io);
    assert(f.call(c::operation::publish,request,out)==error::success);auto first=out;
    assert(f.authority->revoke("clock",[&](auto alias){f.clocks.revoke(alias);})==error::success);
    assert(f.call(c::operation::publish,request,out)==error::success&&out==first); // no fresh clock observation
    assert(f.call(c::operation::lookup,f.query(),out)==error::denied);
    assert(f.call(c::operation::publish,f.publication(3),out)==error::denied);
}
static void owned_wait_cancel_and_generic_requests()
{
    fixture f;f.open();bytes out;auto submitted=f.submission(1);
    assert(f.call(c::operation::submit,submitted,out)==error::success);auto request=identity(out);
    assert(f.call(c::operation::take,f.take(request),out)==error::again);
    assert(f.call(c::operation::publish,f.publication(2),out)==error::success);
    assert(f.call(c::operation::take,f.take(request),out,1024,[](auto){return false;})==error::io);
    bool stopped{};auto copy_then_stop=[&](auto){stopped=true;return true;};auto cancellation=[&]{return stopped;};
    assert(f.call(c::operation::take,f.take(request),out,1024,copy_then_stop,cancellation)==error::cancelled);
    assert(f.call(c::operation::take,f.take(request),out)==error::success);
    auto input=encode([&](auto& writer){assert(writer.u64(request));});std::size_t required{};
    auto copy=[&](s::wire::bytes data){out.assign(data.begin(),data.end());return true;};
    assert(f.dispatch.invoke_request(f.who,false,f.cap,input,160,required,copy)==error::success&&identity(out)==request);
    assert(f.dispatch.invoke_request(f.who,true,f.cap,input,7,required,copy)==error::too_large);
    assert(f.dispatch.invoke_request(f.who,true,f.cap,input,8,required,copy)==error::success&&identity(out)==1);
    assert(f.dispatch.invoke_request(f.who,true,f.cap,input,8,required,copy)==error::success&&identity(out)==0);
    submitted=f.submission(3,99);assert(f.call(c::operation::submit,submitted,out)==error::success);request=identity(out);
    assert(f.authority->revoke("clock",[&](auto alias){f.clocks.revoke(alias);})==error::success);
    assert(f.call(c::operation::cancel,f.take(request),out)==error::success);
    assert(f.call(c::operation::submit,submitted,out)==error::success&&identity(out)==request); // cancelled historical receipt
    input=encode([&](auto& writer){assert(writer.u64(request));});
    assert(f.dispatch.invoke_request(f.who,false,f.cap,input,160,required,copy)==error::success&&identity(out)==request);
    assert(f.dispatch.invoke_request(f.who,true,f.cap,input,8,required,copy)==error::success&&identity(out)==1);
}
static void live_clock_and_epoch_delivery_fences()
{
    fixture f{true,true,false,true};f.open();bytes out;
    assert(f.call(c::operation::publish,f.publication(1),out)==error::success);
    auto request=f.submission(2);assert(f.call(c::operation::submit,request,out)==error::success);auto id=identity(out);
    auto jump=[&](auto){assert(f.clocks.clock_update("clock",f.clock,0,true)==error::success);return true;};
    assert(f.call(c::operation::take,f.take(id),out,1024,jump)==error::conflict);
    assert(f.call(c::operation::lookup,f.query(),out)==error::conflict);
    auto clear=encode([&](auto& writer){assert(writer.u64(f.buffer)&&writer.u64(3)&&writer.u64(f.buffer)&&writer.u64(2));});
    assert(f.call(c::operation::clear,clear,out)==error::success);++f.epoch;
    assert(f.call(c::operation::take,f.take(id),out)==error::success); // cancellation receipt, no old-epoch transform
    assert(f.clocks.close("clock",f.clock)==error::success);
    assert(f.call(c::operation::lookup,f.query(),out)==error::bad_handle);
    assert(f.dispatch.close("frames",f.buffer)==error::success&&!f.usage().retained_sessions);
}
static void bounded_carrier_and_process_rejection()
{
    fixture f;s::transforms::registry::carrier_lease workspace;
    assert(f.dispatch.reserve_workspace(f.who,c::operation::publish,f.cap,8192,workspace)==error::too_large); // profile's batch bound first
    assert(f.dispatch.reserve_workspace(f.who,c::operation::publish,f.cap,4096,workspace)==error::success&&workspace.live());
    assert(f.usage().carrier_bytes==4096);workspace.reset();f.open();
    auto pid=::fork();assert(pid>=0);if(!pid)
    {
        assert(f.dispatch.preflight(f.who,c::operation::lookup,f.cap,8)==error::denied);
        assert(f.dispatch.preflight_request(f.who,false,f.cap,8)==error::denied);bytes out;
        assert(f.call(c::operation::lookup,f.query(),out)==error::denied);::_exit(0);
    }
    int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&WEXITSTATUS(status)==0);
}
static void cancellation_reentry_precedes_disclosure()
{
    for(auto alias:{"frames","clock"})
    {
        fixture f;f.open();bytes out;assert(f.call(c::operation::publish,f.publication(1),out)==error::success);
        unsigned checks{};bool copied{};
        auto cancellation=[&]{if(++checks==2)assert(f.authority->revoke(alias,[](auto){})==error::success);return false;};
        auto sink=[&](auto){copied=true;return true;};
        assert(f.call(c::operation::lookup,f.query(),out,1024,sink,cancellation)==error::denied&&!copied);
    }
    fixture clock_jump{true,true,false,true};clock_jump.open();bytes out;
    assert(clock_jump.call(c::operation::publish,clock_jump.publication(1),out)==error::success);
    unsigned checks{};bool copied{};
    auto cancellation=[&]{if(++checks==2)assert(clock_jump.clocks.clock_update("clock",clock_jump.clock,0,true)==error::success);return false;};
    auto sink=[&](auto){copied=true;return true;};
    assert(clock_jump.call(c::operation::lookup,clock_jump.query(),out,1024,sink,cancellation)==error::conflict&&!copied);
    fixture replay;replay.open();auto request=replay.publication(1);
    assert(replay.call(c::operation::publish,request,out)==error::success);checks=0;copied=false;
    auto revoke=[&]{if(++checks==2)assert(replay.authority->revoke("frames",[](auto){})==error::success);return false;};
    assert(replay.call(c::operation::publish,request,out,1024,sink,revoke)==error::io&&!copied);
    fixture generic;generic.open();assert(generic.call(c::operation::submit,generic.submission(1,99),out)==error::success);
    auto id=identity(out);auto input=encode([&](auto& writer){assert(writer.u64(id));});checks=0;copied=false;std::size_t required{};
    auto deny=[&]{if(++checks==2)assert(generic.authority->revoke("frames",[](auto){})==error::success);return false;};
    assert(generic.dispatch.invoke_request(generic.who,false,generic.cap,input,160,required,sink,deny)==error::denied&&!copied);
}
int main()
{
    default_denial_short_output_and_clock_budget();static_generation_and_exact_receipt_recovery();owned_wait_cancel_and_generic_requests();
    live_clock_and_epoch_delivery_fences();bounded_carrier_and_process_rejection();cancellation_reentry_precedes_disclosure();
    std::cout<<"passed 6 transform dispatch security groups\n";
}
