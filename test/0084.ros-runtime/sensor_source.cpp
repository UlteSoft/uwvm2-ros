/** @file @brief Real private ingress bytes, sealed sensor provenance and authority failures. */
#define main prior_sensor_core_main
#include "sensor_core.cpp"
#undef main
#include <uwvm2/imported/ros/scene/sensor_source.h>
namespace pa=s::proposals::admission;
static s::grant source_grant()
{
    s::grant out;out.alias="sensor";out.resource="fixed.ingress";out.binding="bounded-sensor";out.kind=s::purpose::sensor;out.max_handles=4;out.max_resources=2;
    for(auto action:{"sensor.open","sensor.take","sensor.describe","resource.observe","resource.close"})out.actions.emplace(action);
    return out;
}
struct sensor_fixture
{
    std::shared_ptr<s::authority> permissions=uwvm2::utils::container::make_shared<s::authority>();
    s::buffers::registry buffers;n::source provider{permissions,buffers,clock_now};
    int instance{},other_instance{};s::caller who{&instance,1},other{&other_instance,1};s::handle cap{},second{},resource{};
    s::proposals::source_rule rule;
    explicit sensor_fixture(s::grant grant=source_grant(),std::uint64_t objects=3,
                            uwvm2::utils::container::function_ref<void(s::authority&)> configure={})
    {
        time_now=1000;
        for(auto action:{"capability.open","capability.close","sensor.open","sensor.take","sensor.describe","resource.close"})
            assert(permissions->expose(action,s::exposure::allow)==error::success);
        if(configure)configure(*permissions);
        assert(permissions->configure(std::move(grant))==error::success&&permissions->enroll(who)==error::success&&permissions->enroll(other)==error::success);
        assert(permissions->open(who,"sensor",cap)==error::success&&permissions->open(who,"sensor",second)==error::success);
        s::buffers::plan bp;bp.schema=profile("sensor.buffer");bp.objects=objects;bp.bytes=4*objects;bp.object_bytes=bp.transfer_bytes=4;
        bp.leases=16;bp.rank=1;bp.elements=4;bp.element_mask=bp.order_mask=bp.color_mask=1;bp.workspace_bytes=64;
        assert(buffers.bind("frames",bp)==error::success);
        n::source::configuration configuration;configuration.ingress=plan();configuration.ingress.full=n::overflow::drop_oldest;
        assert(configuration.alias.assign("sensor")&&configuration.buffer_alias.assign("frames"));configuration.buffer_profile=bp.schema;configuration.history=2;configuration.buffer_objects=3;
        assert(provider.configure(owner,configuration,source)==error::success);
        assert(rule.alias.assign("sensor")&&rule.buffer_alias.assign("frames")&&rule.frame.assign("base"));rule.profile=configuration.ingress.profile;rule.buffer_profile=bp.schema;
    }
    void open(){assert(provider.open(who,cap,plan().profile,[&](auto id){resource=id;return true;})==error::success);}
    error take(){return provider.take(who,cap,resource,[](auto const&){return true;});}
    s::proposal_dispatch::source_sample capture()
    {s::proposal_dispatch::source_sample out;assert(provider.capture(who,cap,resource,rule,out)==error::success);return out;}
    s::buffers::usage usage(){s::buffers::usage out;assert(buffers.inspect("frames",out)==error::success);return out;}
};
static void default_zero_open_and_caller_isolation()
{
    sensor_fixture f;assert(f.provider.ingest(sample(),payload)==error::denied);
    assert(f.provider.open(f.who,0,plan().profile,[](auto){assert(false);return true;})==error::bad_handle);
    assert(f.provider.open(f.who,f.cap,plan().profile,[](auto){return false;})==error::io);
    f.open();assert(f.provider.ingest(sample(),payload)==error::success);
    assert(f.provider.take(f.other,f.cap,f.resource,[](auto const&){assert(false);return true;})==error::bad_handle);
    assert(f.provider.take(f.who,f.second,f.resource,[](auto const&){assert(false);return true;})==error::bad_handle);
    assert(f.take()==error::success);
    auto grant=source_grant();grant.limits.uncertain=true;grant.limits.bytes=0;sensor_fixture zero{grant};zero.open();
    assert(zero.provider.ingest(sample(),payload)==error::success&&zero.take()==error::again&&!zero.usage().objects);
}
static void real_frame_sealed_provenance_and_failed_delivery()
{
    sensor_fixture f;f.open();assert(f.provider.ingest(sample(),payload)==error::success);
    s::proposal_dispatch::source_sample absent;assert(f.provider.capture(f.who,f.cap,f.resource,f.rule,absent)==error::again);
    s::handle failed{};
    assert(f.provider.take(f.who,f.cap,f.resource,[&](auto const& frame){failed=frame.buffer;return false;})==error::io&&!f.usage().objects);
    s::buffers::information info;assert(f.buffers.describe("frames",failed,info)==error::bad_handle);
    s::handle retained{};
    assert(f.provider.take(f.who,f.cap,f.resource,[&](auto const& frame)
    {
        assert(frame.frame.metadata.source.sequence==1&&frame.frame.frame=="base"&&frame.frame.units=="metres");
        assert(std::equal(frame.frame.body.begin(),frame.frame.body.end(),payload.begin(),payload.end()));
        assert(frame.digest==s::buffers::hash(payload));retained=frame.buffer;
        assert(f.provider.capture(f.who,f.cap,f.resource,f.rule,absent)==error::again);
        assert(f.provider.take(f.who,f.cap,f.resource,[](auto const&){assert(false);return true;})==error::again);
        return true;
    })==error::success);
    auto captured=f.capture();auto const& observation=captured.observation;
    assert(observation.buffer==retained&&observation.source==f.resource&&!observation.capability&&observation.clock==8&&observation.epoch==9&&observation.source_ns==1);
    assert(observation.sample_sequence==1&&observation.source_generation==5&&observation.captured_steady_ns==1000&&observation.source_authority_epoch);
    assert(captured.environment==1&&captured.owner==2&&captured.owner_generation==3&&captured.frame.view()=="base");
    assert(f.buffers.describe("frames",retained,info)==error::success&&info.sealed&&info.sha256==s::buffers::hash(payload));
    std::uint64_t bytes{};assert(f.buffers.write("frames",retained,0,payload,bytes)==error::denied);
    assert(f.provider.validate(f.who,f.cap,f.rule,captured,observation,pa::observation_policy::latest_required)==error::success);
    auto fabricated=observation;++fabricated.sample_sequence;
    assert(f.provider.validate(f.who,f.cap,f.rule,captured,fabricated,pa::observation_policy::latest_required)==error::conflict);
    fabricated=observation;++fabricated.source_authority_epoch;
    assert(f.provider.validate(f.who,f.cap,f.rule,captured,fabricated,pa::observation_policy::bounded_age)==error::conflict);
    auto wrong=f.rule;wrong.buffer_profile.sha256[0]^=1;
    assert(f.provider.capture(f.who,f.cap,f.resource,wrong,absent)==error::denied);
}
static void latest_history_epoch_and_pin_capacity()
{
    sensor_fixture f;f.open();assert(f.provider.ingest(sample(),payload)==error::success&&f.take()==error::success);auto first=f.capture();
    s::buffers::registry::sealed_lease model_pin;assert(f.buffers.pin("frames",first.observation.buffer,model_pin)==error::success);
    assert(f.provider.ingest(sample(2),payload)==error::success);
    s::proposal_dispatch::source_sample out;assert(f.provider.capture(f.who,f.cap,f.resource,f.rule,out)==error::again);
    assert(f.provider.validate(f.who,f.cap,f.rule,first,first.observation,pa::observation_policy::latest_required)==error::conflict);
    assert(f.provider.validate(f.who,f.cap,f.rule,first,first.observation,pa::observation_policy::bounded_age)==error::success);
    assert(f.take()==error::success&&f.provider.ingest(sample(3),payload)==error::success&&f.take()==error::success);
    assert(f.provider.validate(f.who,f.cap,f.rule,first,first.observation,pa::observation_policy::bounded_age)==error::conflict);
    assert(f.usage().objects==3&&model_pin.visit([](auto data,auto const&,auto const&){return data.size()==4&&data[0]==1;})==error::success);
    assert(f.provider.ingest(sample(4),payload)==error::success&&f.take()==error::again&&f.usage().objects==3);
    model_pin={};assert(f.usage().objects==2&&f.take()==error::success);auto before_epoch=f.capture();
    auto changed=sample(5);++changed.epoch;assert(f.provider.ingest(changed,payload)==error::success);
    assert(f.provider.validate(f.who,f.cap,f.rule,before_epoch,before_epoch.observation,pa::observation_policy::bounded_age)==error::conflict);
}
static void reentrant_retirement_revoke_close_and_throw()
{
    for(int kind=0;kind!=4;++kind)
    {
        sensor_fixture f;f.open();assert(f.provider.ingest(sample(),payload)==error::success);
        auto result=f.provider.take(f.who,f.cap,f.resource,[&](auto const&)
        {
            if(kind==0)f.provider.retire();
            if(kind==1)assert(f.permissions->revoke("sensor",[](auto){})==error::success);
            if(kind==2)f.buffers.revoke("frames");
            if(kind==3)assert(f.provider.close(f.who,f.cap,f.resource)==error::success);
            return true;
        });
        assert(result!=error::success);
        s::proposal_dispatch::source_sample out;assert(f.provider.capture(f.who,f.cap,f.resource,f.rule,out)!=error::success);
        assert(!f.usage().objects);
    }
    sensor_fixture f;f.open();assert(f.provider.ingest(sample(),payload)==error::success);
    assert(f.provider.take(f.who,f.cap,f.resource,[](auto const&)->bool{throw 7;})==error::io&&!f.usage().objects&&f.take()==error::success);
    sensor_fixture opening;assert(opening.provider.open(opening.who,opening.cap,plan().profile,[&](auto){opening.provider.retire();return true;})!=error::success);
}
static void real_fast_io_failure_and_retained_queue()
{
#if defined(UWVM_SENSOR_FAULT_INJECTION)
    unsigned failures{};
    for(std::size_t point=1;point<16;++point)
    {
        sensor_fixture f;f.open();assert(f.provider.ingest(sample(),payload)==error::success);fail_after=point;allocation_failed=false;
        auto result=f.take();fail_after=0;
        if(!allocation_failed){assert(result==error::success);break;}
        ++failures;assert(result==error::no_memory&&!f.usage().objects&&f.take()==error::success);
    }
    assert(failures>=3);std::cout<<"actual sensor source fast_io failures "<<failures<<'\n';
#endif
}
static void concurrent_local_retirement_and_fork()
{
    sensor_fixture f;f.open();assert(f.provider.ingest(sample(),payload)==error::success);
    std::barrier rendezvous{2};error status{};
    std::thread worker{[&]{status=f.provider.take(f.who,f.cap,f.resource,[&](auto const&){rendezvous.arrive_and_wait();rendezvous.arrive_and_wait();return true;});}};
    rendezvous.arrive_and_wait();f.provider.retire();rendezvous.arrive_and_wait();worker.join();assert(status!=error::success&&!f.usage().objects);
    sensor_fixture live;live.open();auto pid=::fork();assert(pid>=0);
    if(!pid){::alarm(3);::_exit(live.provider.ingest(sample(),payload)==error::denied&&live.take()==error::denied?0:4);}
    int result{};assert(::waitpid(pid,&result,0)==pid&&WIFEXITED(result)&&!WEXITSTATUS(result));
}
static void invalid_host_clock_never_becomes_source_stamp()
{
    sensor_fixture f;f.open();time_now=UINT64_MAX;assert(f.provider.ingest(sample(),payload)==error::io);time_now=1000;
    assert(f.provider.ingest(sample(),payload)==error::success&&f.take()==error::success);
}
#if defined(UWVM_SENSOR_SOURCE_FIXTURE_ONLY)
int prior_sensor_source_fixture_main()
#else
int main()
#endif
{
    prior_sensor_core_main();default_zero_open_and_caller_isolation();real_frame_sealed_provenance_and_failed_delivery();
    latest_history_epoch_and_pin_capacity();reentrant_retirement_revoke_close_and_throw();real_fast_io_failure_and_retained_queue();
    concurrent_local_retirement_and_fork();invalid_host_clock_never_becomes_source_stamp();
    std::cout<<"passed 7 sensor source safety groups\n";return 0;
}
