/** @file @brief Recording descriptor hashing, closed validators, finite budgets and actual recoverable allocation. */
#include <uwvm2/imported/ros/scene/recording_configuration.h>
#include <cassert>
#include <iostream>
namespace s=uwvm2::imported::ros::scene;
namespace r=s::recording;
using s::error;
static thread_local std::size_t allocations{},fail_after{};
static thread_local bool injected{};
static bool fail()noexcept{++allocations;if(fail_after&&!--fail_after){injected=true;return true;}return false;}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return fail()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t z){return fail()?nullptr:__real_calloc(n,z);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return fail()?nullptr:__real_realloc(p,n);}
static bool boolean(s::wire::bytes value)noexcept{return value.size()==1&&value[0]<=1;}
struct packet
{
    std::array<std::uint8_t,8192> data{};std::size_t size{};
    s::wire::bytes view()const noexcept{return {data.data(),size};}
};
static s::buffers::profile profile(std::string_view name)
{s::buffers::profile value;assert(value.identity.assign(name));value.sha256[0]=1;return value;}
static auto registry()
{return std::array{r::codec{"ros-bool","ros1","std_msgs/Bool","ros1msg","ros1",s::buffers::hash(r::mcap::raw("bool data\n")),boolean}};}
struct request
{packet encoded;s::buffers::profile selected;};
static request descriptor(std::string_view codec="ros-bool",std::string_view schema="bool data\n",std::string_view source="input",std::uint64_t channel=1,std::uint64_t count=1)
{
    packet body;s::wire::writer out{body.data};assert(out.text("ros1")&&out.u64(count));
    for(std::size_t i{};i<count;++i)
    {
        auto buffer=profile("serialized.bool");
        assert(out.u64(channel)&&out.u64(1)&&out.text("/long_robot_name/state")&&out.text(codec)&&out.text("std_msgs/Bool")&&out.text("ros1msg")&&
            out.text("ros1")&&out.text("")&&out.blob(r::mcap::raw(schema))&&out.u64(1)&&out.text(source)&&out.text(buffer.identity.view())&&out.blob(buffer.sha256));
    }
    body.size=out.size();request result;result.selected=profile("robot.recording");result.selected.sha256=s::buffers::hash(body.view());
    s::wire::writer encoded{result.encoded.data};assert(encoded.text(r::definition_magic)&&encoded.text(result.selected.identity.view())&&encoded.blob(result.selected.sha256));
    assert(encoded.size()+body.size<=result.encoded.data.size());
    std::copy(body.data.begin(),body.data.begin()+body.size,result.encoded.data.begin()+encoded.size());
    result.encoded.size=encoded.size()+body.size;return result;
}
static r::configuration configure(request const& request,std::uint64_t storage=1024*1024)
{
    r::configuration c;assert(c.profile(request.selected)==error::success);
    std::array<std::uint64_t,9> limits{2,1024,4,2,512,4,storage,8192,4};
    for(std::size_t i{};i<limits.size();++i){assert(c.limit(r::configuration::limit_names[i],limits[i])==error::success);}
    return c;
}
static void phases_and_ownership()
{
    auto request=descriptor();auto codecs=registry();r::configuration empty;r::definition definition;
    std::uint64_t ceiling=91;assert(empty.definition_limit(ceiling)==error::denied&&ceiling==91);
    assert(empty.parse(request.encoded.view(),codecs)==error::denied&&empty.freeze(definition)==error::denied);
    auto c=configure(request);assert(c.profile(request.selected)==error::conflict&&c.limit("segments",2)==error::conflict);
    assert(c.definition_limit(ceiling)==error::success&&ceiling==8192);
    assert(c.parse(request.encoded.view(),codecs)==error::success&&c.parse(request.encoded.view(),codecs)==error::conflict);
    request.encoded.data.fill(0);assert(c.freeze(definition)==error::success&&definition.channels.size()==1&&definition.selected==request.selected);
    assert(c.definition_limit(ceiling)==error::conflict);
    auto copy=definition;definition.channels[0].topic="changed";definition.channels[0].definition[0]='x';
    assert(copy.channels[0].topic=="/long_robot_name/state"&&copy.channels[0].definition[0]=='b'&&copy.channels[0].validate==boolean);
    std::uint64_t bytes{},retained{};assert(copy.storage(bytes)&&bytes>0&&copy.retained(retained)&&retained>=sizeof(copy));
    auto damaged=copy;damaged.ros_profile.size=SIZE_MAX;assert(!damaged.storage(bytes));
    s::recording_dispatch::replay_plan replay{profile("replay"),2};assert(s::recording_dispatch::replay_storage(replay,bytes)&&bytes>0);
    replay.handles=UINT64_MAX;assert(!s::recording_dispatch::replay_storage(replay,bytes));
    auto authority=uwvm2::utils::container::make_shared<s::authority>();s::buffers::registry buffers;
    s::recording_dispatch dispatcher{authority,buffers,9,{1,1,1024*1024,8192,2}};
    assert(copy.bind(dispatcher,"recording")==error::success&&dispatcher.prepare()==error::success);
    assert(c.limit("segment.bytes",100)==error::conflict);
}
static void truncation_hash_and_schema()
{
    auto request=descriptor();auto initial=configure(request);auto codecs=registry();
    for(std::size_t n{};n<request.encoded.size;++n)
    {auto c=initial;assert(c.parse(request.encoded.view().first(n),codecs)!=error::success);assert(c.parse(request.encoded.view(),codecs)==error::success);}
    auto changed=request.encoded;changed.data[changed.size-1]^=1;auto c=initial;assert(c.parse(changed.view(),codecs)==error::conflict);
    changed=request.encoded;changed.data[changed.size++]=0;assert(c.parse(changed.view(),codecs)==error::conflict);
    auto unknown=descriptor("arbitrary.native.symbol");c=configure(unknown);assert(c.parse(unknown.encoded.view(),codecs)==error::denied);
    auto wrong_schema=descriptor("ros-bool","string data\n");c=configure(wrong_schema);assert(c.parse(wrong_schema.encoded.view(),codecs)==error::denied);
    auto path=descriptor("ros-bool","bool data\n","/tmp/driver");c=configure(path);assert(c.parse(path.encoded.view(),codecs)==error::invalid);
    auto zero=descriptor("ros-bool","bool data\n","input",0);c=configure(zero);assert(c.parse(zero.encoded.view(),codecs)==error::invalid);
    auto duplicate=descriptor("ros-bool","bool data\n","input",1,2);c=configure(duplicate);assert(c.parse(duplicate.encoded.view(),codecs)==error::invalid);
    c=initial;assert(c.parse(request.encoded.view(),{})==error::denied);
    std::array repeated{codecs[0],codecs[0]};assert(c.parse(request.encoded.view(),repeated)==error::conflict);
    codecs[0].validate=nullptr;assert(c.parse(request.encoded.view(),codecs)==error::invalid);
}
static void finite_limits()
{
    auto request=descriptor();auto c=configure(request,1);assert(c.parse(request.encoded.view(),registry())==error::too_large);
    for(auto field:r::configuration::limit_names)
    {r::configuration invalid;assert(invalid.limit(field,UINT64_MAX)==error::invalid);}
    r::configuration invalid;assert(invalid.limit("unknown",1)==error::invalid&&invalid.limit("segments",4097)==error::too_large);
    auto profile=request.selected;profile.identity.size=SIZE_MAX;assert(invalid.profile(profile)==error::invalid);
    s::recording_budget_configuration budget;s::recording_dispatch::limits output;
    assert(budget.freeze(output)==error::denied);
    for(auto field:s::recording_budget_configuration::names)assert(budget.limit(field,0)==error::success);
    assert(budget.freeze(output)==error::success&&!output.recordings&&!output.workspace_bytes);
    assert(budget.limit("recordings",1)==error::conflict);
    s::recording_budget_configuration missing;
    for(auto field:s::recording_budget_configuration::names)assert(missing.limit(field,field=="recordings"?1:0)==error::success);
    assert(missing.freeze(output)==error::invalid);
}
static void recoverable_allocation()
{
    auto request=descriptor();auto codecs=registry();auto baseline=configure(request);
    allocations=0;assert(baseline.parse(request.encoded.view(),codecs)==error::success);auto parse_allocations=allocations;
    assert(parse_allocations&&parse_allocations<64);
    // Small-string capacity differs across libc++/libstdc++ and pointer widths.
    // Inject every observed real allocation instead of assuming a fixed count.
    for(std::size_t n=1;n<=parse_allocations;++n)
    {
        auto c=configure(request);injected=false;fail_after=n;auto result=c.parse(request.encoded.view(),codecs);fail_after=0;
        assert(injected&&result==error::no_memory);assert(c.parse(request.encoded.view(),codecs)==error::success);
    }
    auto c=configure(request);assert(c.parse(request.encoded.view(),codecs)==error::success);
    r::definition measured;allocations=0;assert(c.freeze(measured)==error::success);auto copy_allocations=allocations;
    assert(copy_allocations&&copy_allocations<64);
    for(std::size_t n=1;n<=copy_allocations;++n)
    {
        r::definition out;out.selected=profile("unchanged");injected=false;fail_after=n;auto result=c.freeze(out);fail_after=0;
        assert(injected&&result==error::no_memory&&out.selected.identity.view()=="unchanged"&&out.channels.empty());
        assert(c.freeze(out)==error::success&&out.channels.size()==1);
    }
    std::cout<<"allocation failures: parse="<<parse_allocations<<" copy="<<copy_allocations<<'\n';
}
int main()
{phases_and_ownership();truncation_hash_and_schema();finite_limits();recoverable_allocation();std::cout<<"passed 4 recording configuration security groups\n";}
