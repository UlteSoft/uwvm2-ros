/** @file @brief Static no-I/O policy, actual receive-only activation and broker routing security. */
#include "sensor_proposal_deployment_fixture.h"
#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
static thread_local std::size_t allocations{},fail_after{};
static bool fail()noexcept{++allocations;return fail_after&&!--fail_after;}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return fail()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t z){return fail()?nullptr:__real_calloc(n,z);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return fail()?nullptr:__real_realloc(p,n);}
namespace f=sensor_proposal_fixture;namespace s=f::s;using s::error;
namespace sensor=uwvm2::imported::ros::sensor_contract;namespace proposal=uwvm2::imported::ros::proposal_contract;
static void static_zero_exact_and_complete()
{
    s::deployment empty;f::environment(empty);assert(empty.validate()==error::success);
    assert(empty.sensor_profile("robot","sensor",f::sensor_profile(),"octets")==error::invalid);
    s::deployment denied;f::environment(denied);
    assert(denied.sensor_profile("robot","sensor",f::sensor_profile(),"octets")==error::denied&&denied.sensor_definition_file("robot","sensor","/must-not-open",1,{})==error::denied);
    for(std::size_t missing{};missing<11;++missing)
    {
        s::deployment d;f::environment(d);f::sensor(d,INT_MAX,missing);assert(d.validate()==error::invalid);
        assert(d.sensor_limit("robot","sensor",s::sensors::configuration::names[missing],f::sensor_limits()[missing])==error::success&&d.validate()==error::success);
        assert(d.sensor_descriptor("robot","sensor",0)==error::invalid&&d.prepare_resources(f::no_input)!=error::success);
    }
    s::deployment no_budget;f::environment(no_budget);f::sensor(no_budget,INT_MAX,11,true,false);assert(no_budget.validate()==error::invalid);
    assert(no_budget.sensor_budget("robot",{0,0})==error::success&&no_budget.validate()==error::too_large);
    s::deployment exact;f::environment(exact);f::sensor(exact,INT_MAX);
    assert(exact.sensor_descriptor("robot","sensor",0)==error::conflict&&exact.sensor_definition_file("robot","sensor","/must-not-open",1,{})==error::conflict);
    assert(exact.validate()==error::success);int identity{};std::shared_ptr<s::broker> broker;
    assert(exact.select("main",{&identity,1},broker)==error::denied&&!broker);
}
static void proposal_static_dependencies()
{
    for(std::size_t missing{};missing<11;++missing)
    {
        s::deployment d;f::environment(d);f::sensor(d,INT_MAX);f::proposal(d,missing);assert(d.validate()==error::invalid);
        assert(d.proposal_limit("robot","suggestions",s::proposals::configuration::limit_names[missing],f::proposal_limits[missing])==error::success&&d.validate()==error::success);
        assert(d.proposal_definition_file("robot","suggestions","/must-not-open",1,{})==error::invalid);
    }
    for(int wrong{};wrong<2;++wrong)
    {s::deployment d;f::environment(d);f::sensor(d,INT_MAX);f::proposal(d,11,wrong==0,wrong==1);assert(d.validate()==error::invalid);}
    s::deployment no_source;f::environment(no_source);f::proposal(no_source);assert(no_source.validate()==error::invalid);
    s::deployment no_grant;f::environment(no_grant);f::model(no_grant);
    assert(no_grant.proposal_profile("robot","suggestions",f::proposal_profile(),"brain")==error::denied&&no_grant.proposal_arbiter("robot","judge","suggestions","node",1)==error::denied);
}
struct live_sensor
{
    s::deployment deployment;int descriptors[2]{-1,-1},identity{};s::caller who{&identity,1};std::shared_ptr<s::broker> broker;
    s::handle cap{},handle{};f::bytes output;std::size_t required{};
    explicit live_sensor(bool export_data=true,bool throttled=false)
    {
        assert(::pipe2(descriptors,O_NONBLOCK|O_CLOEXEC)==0);f::environment(deployment);f::sensor(deployment,descriptors[0],11,export_data);
        if(throttled)assert(deployment.limit("robot","sensor","delivery","may-reject")==error::success&&deployment.limit("robot","sensor","calls-per-window","0")==error::success);
        assert(deployment.validate()==error::success&&deployment.prepare_resources(f::no_input)==error::success&&deployment.select("main",who,broker)==error::success);
        assert(broker->permissions.open(who,"sensor",cap)==error::success);
    }
    ~live_sensor(){for(auto fd:descriptors)if(fd>=0)::close(fd);}
    error call(sensor::operation action,f::bytes const& bytes,std::size_t capacity=4096,s::sensor_dispatch::sink sink={})
    {auto copy=[&](s::bytes data){output.assign(data.begin(),data.end());return true;};return broker->invoke_sensor(who,action,cap,bytes,capacity,required,sink?sink:s::sensor_dispatch::sink(copy));}
    auto open_request(){return f::encode([](auto& out){assert(s::proposals::write_profile(out,f::sensor_profile()));});}
    auto query(){return f::encode([&](auto& out){assert(out.u64(handle));});}
    void open(){assert(call(sensor::operation::open,open_request())==error::success);handle=s::word(output,0);}
    auto packet()
    {std::array<std::uint8_t,4> body{9,8,7,6};f::bytes data(s::sensors::ingress_protocol::header_bytes+body.size());std::size_t size{};assert(s::sensors::ingress_protocol::encode({101,1,1,0,0,0,false},s::sensor_octets_registration().schema,body,data,size)==error::success&&size==data.size());return data;}
    void write(){auto data=packet();assert(::write(descriptors[1],data.data(),data.size())==static_cast<ssize_t>(data.size()));}
    auto queued(){int bytes{};assert(::ioctl(descriptors[0],FIONREAD,&bytes)==0);return bytes;}
    auto admissions()
    {uwvm2::utils::container::std_vector<s::authority_snapshot> rows;assert(broker->permissions.inspect(rows,8));for(auto const& row:rows)if(row.alias=="sensor")return row.admissions;assert(false);return std::uint64_t{};}
};
static void activation_read_disclosure_and_close()
{
    live_sensor f;f.write();assert(f.queued()==static_cast<int>(f.packet().size()));auto before=f.admissions();
    assert(f.call(sensor::operation::open,f.open_request(),7)==error::too_large&&f.admissions()==before&&f.queued()>0);f.open();
    assert(f.call(sensor::operation::take,f.query(),1)==error::too_large&&f.queued()>0);
    // One admitted guest call performs at most one bounded native read: the
    // first consumes the header, then reports again until the body is present.
    assert(f.call(sensor::operation::take,f.query())==error::again&&f.queued()==4);
    assert(f.call(sensor::operation::take,f.query(),4096,[](auto){return false;})==error::io&&f.queued()==0);
    assert(f.call(sensor::operation::take,f.query())==error::success&&f.call(sensor::operation::describe,f.query())==error::success);
    s::debug::request query;query.selected=s::debug::selection::resources;s::debug::view view;before=f.admissions();f.deployment.inspect(query,view);
    assert(view.result==s::debug::status::ok&&f.admissions()==before);bool found{};for(std::size_t i{};i<view.size;++i)if(view.rows[i].type.view()=="sensor")
    {found=true;assert(view.rows[i].open_handles==1&&view.rows[i].output_items==1&&s::debug::valid(view.rows[i]));}assert(found);
    std::size_t required{};using op=uwvm2::imported::ros::runtime_contract::operation;
    assert(f.broker->invoke(f.who,op::resource_close,f.cap,f.query(),0,required,[](auto){return false;})==error::invalid&&f.admissions()==before);
    assert(f.broker->invoke(f.who,op::resource_close,f.cap,f.query(),0,required,[](auto){return true;})==error::success&&f.admissions()==before+1);
    assert(f.call(sensor::operation::take,f.query())==error::bad_handle);
    live_sensor denied{false};denied.open();denied.write();assert(denied.call(sensor::operation::take,denied.query())==error::denied&&denied.queued()>0);
    live_sensor zero{true,true};assert(zero.call(sensor::operation::open,zero.open_request())==error::again);
}
static void activation_failure_and_revoke()
{
    int descriptors[2];assert(::pipe2(descriptors,O_NONBLOCK|O_CLOEXEC)==0);
    s::deployment d;f::environment(d);f::sensor(d,descriptors[1]);assert(d.validate()==error::success&&d.prepare_resources(f::no_input)==error::denied);
    int identity{};std::shared_ptr<s::broker> state;assert(d.select("main",{&identity,1},state)==error::denied&&!state);::close(descriptors[0]);::close(descriptors[1]);
    live_sensor f;f.open();f.write();assert(f.broker->revoke("sensor")==error::success&&f.call(sensor::operation::take,f.query())==error::denied&&f.queued()>0);
}
static void configuration_hex_failures()
{
    s::byte_buffer previous;previous.resize_for_overwrite(1);previous.writable()[0]=42;
    uwvm2::utils::container::std_string large(2048,'0');auto before=allocations;
    for(auto invalid:{"", "0", "GG", "AA"})
        assert(s::configuration_file::decode_hex(invalid,4096,previous)==error::invalid);
    assert(s::configuration_file::decode_hex(large,512,previous)==error::too_large&&allocations==before);
    large.back()='G';assert(s::configuration_file::decode_hex(large,4096,previous)==error::invalid&&allocations==before);
    large.back()='0';fail_after=1;auto result=s::configuration_file::decode_hex(large,4096,previous);fail_after=0;
    assert(result==error::no_memory&&previous.view().size()==1&&previous.view()[0]==42);
    s::deployment d;f::environment(d);before=allocations;
    assert(d.sensor_definition_hex("robot","sensor",large)==error::denied&&d.proposal_definition_hex("robot","suggestions",large)==error::denied&&allocations==before);
    assert(d.allow("robot",f::grant("sensor",s::purpose::sensor,"bounded-sensor",{"sensor.open"}))==error::success&&
           d.sensor_profile("robot","sensor",f::sensor_profile(),"octets")==error::success);
    auto canonical=f::hex(f::sensor_definition());
    assert(d.sensor_definition_hex("robot","sensor","GG")==error::invalid&&d.sensor_definition_hex("robot","sensor",canonical)==error::success);
    before=allocations;assert(d.sensor_definition_hex("robot","sensor",large)==error::conflict&&allocations==before);
    auto proposal=f::hex(f::proposal_definition());std::size_t failures{};
    for(std::size_t attempt=1;attempt<16;++attempt)
    {
        s::deployment next;f::environment(next);f::model(next);
        assert(next.allow("robot",f::grant("suggestions",s::purpose::proposal,"model-proposals",{"proposal.capture"}))==error::success&&
               next.proposal_profile("robot","suggestions",f::proposal_profile(),"brain")==error::success);
        before=allocations;assert(next.proposal_definition_hex("robot","suggestions","GG")==error::invalid&&allocations==before);
        fail_after=attempt;result=next.proposal_definition_hex("robot","suggestions",proposal);fail_after=0;
        if(result==error::success)break;
        assert(result==error::no_memory);++failures;
        assert(next.proposal_definition_hex("robot","suggestions",proposal)==error::success);
        before=allocations;assert(next.proposal_definition_hex("robot","suggestions",large)==error::conflict&&allocations==before);
    }
    assert(failures>=2);
}
int main(){static_zero_exact_and_complete();proposal_static_dependencies();activation_read_disclosure_and_close();activation_failure_and_revoke();configuration_hex_failures();std::cout<<"passed 5 sensor proposal deployment groups\n";}
