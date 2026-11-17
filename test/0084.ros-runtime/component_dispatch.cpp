/** @file
 * @brief Actual isolated child, immutable configuration and typed role authority.
 * @details The native topic factory is controlled fault injection, not a ROS
 * middleware interoperability claim. Process isolation and IPC execute for real.
 */
#define main previous_bridge_main
#include "bridge_dispatch.cpp"
#undef main
#include "support/legacy_process/component_dispatch.h"
#include "model_worker_fixture.h"
using cop=robot::operation;
static s::model_worker::configuration component_images(char** paths)
{
    uwvm2::utils::container::std_vector<x::descriptor> descriptors;auto manifest=worker_manifest(paths,descriptors);
    manifest.bound.request_bytes=manifest.bound.response_bytes=8192;manifest.bound.cpu_seconds=10;manifest.bound.stderr_bytes=4096;
    s::model_worker::configuration config;
    for(auto field:s::model_worker::limit_fields)assert(config.limit(field.name,manifest.bound.*field.member)==error::success);
    assert(config.launcher(paths[8],manifest.launcher.bytes,manifest.launcher.sha256)==error::success);
    for(std::size_t i{};i<manifest.files.size();++i)
    {auto const& f=manifest.files[i];if(f.role!=x::image_role::weights)assert(config.image(f.role,f.destination.view(),paths[i],f.bytes,f.sha256)==error::success);}
    assert(config.freeze_component(bridge_profile("component.profile"),bridge_profile("component.kill"))==error::success);return config;
}
struct component_fixture:bridge_fixture
{
    enum class receive_fault {none,timeout_busy,timeout_debt,timeout_empty,again_empty,query_denied,query_throws};
    receive_fault fault{};std::atomic<unsigned> debt_queries{};
    s::artifacts::registry inputs;s::components::plan policy;s::model_worker::configuration images;
    uwvm2::utils::container::std_unique_ptr<s::component_dispatch> component;
    s::handle component_cap{},configuration_cap{},instance_id{};bytes response;
    static void configure(s::authority& permissions,std::string_view omitted,std::optional<std::uint64_t> calls)
    {
        for(auto const& op:robot::operations)if(op.id>=cop::component_open)assert(permissions.expose(text(op.name),s::exposure::allow)==error::success);
        for(auto action:{"artifact.open","artifact.read","artifact.describe"})assert(permissions.expose(action,s::exposure::allow)==error::success);
        for(bool config:{false,true})
        {
            s::grant grant;grant.alias=config?"configuration":"component";grant.resource=grant.alias;grant.kind=config?s::purpose::configuration:s::purpose::component;
            grant.binding=config?"sealed-input":"isolated-roles";grant.max_resources=grant.max_handles=1;
            if(config){for(auto action:{"artifact.open","artifact.read","artifact.describe"})if(action!=omitted)grant.actions.emplace(action);}
            else for(auto const& op:robot::operations)if(op.id>=cop::component_open&&op.name!=omitted)grant.actions.emplace(op.name);
            grant.actions.emplace("resource.close");
            if(!config&&calls){grant.limits.calls=calls;grant.limits.window=std::chrono::hours(1);grant.limits.uncertain=true;}
            assert(permissions.configure(grant)==error::success);
        }
    }
    component_fixture(char** paths,std::uint64_t mode,std::string_view omitted={},std::optional<std::uint64_t> calls={},receive_fault failure=receive_fault::none)
        :bridge_fixture(omitted,{},[&](auto& auth){configure(auth,omitted,calls);}),fault(failure),images(component_images(paths))
    {
        policy.alias.assign("component");policy.selected=bridge_profile("component.profile");policy.stop=bridge_profile("component.kill");
        policy.configuration_alias.assign("configuration");policy.configuration_identity.assign("configuration");policy.configuration_generation=7;policy.configuration_bytes=8;
        std::array<std::uint8_t,8> data;w::writer out{data};out.u64(mode);policy.configuration_digest=s::buffers::hash(data);
        policy.ipc_bytes=8192;policy.message_bytes=64;policy.deadline_ns=fault==receive_fault::none?1000000000:50000000;policy.lifetime_ns=10000000000;policy.shutdown_ns=2000000000;
        policy.role_count=2;for(unsigned i{};i<2;++i){auto& role=policy.roles[i];role.alias.assign(i?"source":"destination");role.selected=bridge_profile(i?"source":"destination");role.codec.assign("std-bool");role.publisher=!i;}
        assert(s::component_dispatch::required_storage(policy,policy.retained_bytes));
        int fd=static_cast<int>(::syscall(SYS_memfd_create,"component-configuration",MFD_CLOEXEC|MFD_ALLOW_SEALING));assert(fd>=0&&::write(fd,data.data(),data.size())==8);
        assert(::fcntl(fd,F_ADD_SEALS,F_SEAL_WRITE|F_SEAL_GROW|F_SEAL_SHRINK|F_SEAL_SEAL)==0);
        std::shared_ptr<s::storage::immutable_input> input;s::storage::principal owner{71,"configuration"};
        assert(s::storage::immutable_input::create(fd,owner,s::purpose::configuration,8,8,input)==s::storage::success);::close(fd);
        s::artifacts::input_plan plan{"/trusted/configuration",policy.configuration_digest,7,8,8};
        assert(inputs.bind(owner,s::purpose::configuration,"configuration","sealed-input",plan,input)==error::success);
        assert(permissions->open(who,"component",component_cap)==error::success&&permissions->open(who,"configuration",configuration_cap)==error::success);
        component=uwvm2::utils::container::make_unique<s::component_dispatch>(permissions,inputs,s::bridge_dispatch::transport::from(*this),71,s::component_dispatch::limits{1,policy.retained_bytes});
        assert(component->bind(policy,images)==error::success&&component->prepare()==error::success);
    }
    error invoke(s::caller who,contract::operation action,s::handle cap,w::bytes input,std::size_t capacity,std::size_t& required,s::communication_host::sink sink)
    {return topics.invoke(who,action,cap,input,capacity,required,sink);}
    error invoke_receive(s::caller who,uwvm2::imported::ros::communication_receive_contract::operation action,s::handle cap,w::bytes input,
                         std::size_t capacity,std::size_t& required,s::communication_host::sink sink)
    {
        if(fault!=receive_fault::none&&action==uwvm2::imported::ros::communication_receive_contract::operation::topic_reserve)
        {required=0;return fault==receive_fault::again_empty?error::again:error::timeout;}
        return topics.invoke_receive(who,action,cap,input,capacity,required,sink);
    }
    error topic_receive_quiescent(s::caller who,s::handle cap,s::handle id,bool& empty)
    {
        ++debt_queries;empty=false;
        switch(fault)
        {
        case receive_fault::none:return topics.topic_receive_quiescent(who,cap,id,empty);
        case receive_fault::timeout_busy:return error::again;
        case receive_fault::timeout_debt:return error::success;
        case receive_fault::timeout_empty:case receive_fault::again_empty:empty=true;return error::success;
        case receive_fault::query_denied:return error::denied;
        case receive_fault::query_throws:throw std::bad_alloc{};
        }
        return error::io;
    }
    ~component_fixture(){component->retire();}
    bytes opening(){return encode([&](auto& out){return s::proposals::write_profile(out,policy.selected)&&out.u64(configuration_cap)&&out.text("configuration")&&out.u64(7)&&out.blob(policy.configuration_digest)&&
        out.u64(2)&&out.u64(1)&&out.u64(destination_cap)&&out.u64(destination)&&out.u64(2)&&out.u64(source_cap)&&out.u64(source);});}
    bytes keyed(bool stop=false){return encode([&](auto& out){return out.u64(instance_id)&&out.u64(1)&&out.u64(instance_id)&&(!stop||s::proposals::write_profile(out,policy.stop))&&out.u64(c::now()+(stop?1900000000:4900000000));});}
    error call(cop action,bytes const& input,std::size_t capacity=2048,s::component_dispatch::sink custom={})
    {std::size_t required{};auto sink=[&](w::bytes bytes){response.assign(bytes.begin(),bytes.end());return true;};
     auto status=component->invoke(who,action,component_cap,input,capacity,required,custom?custom:s::component_dispatch::sink(sink));
     if(status==error::success&&!custom){robot::response decoded;assert(robot::decode_response(action,response,{2048,2048,255,16},decoded));}return status;}
    void open(){assert(call(cop::component_open,opening())==error::success);w::reader in{response};assert(in.u64(instance_id)&&in.done());}
    s::component_dispatch::inspection usage()
    {s::component_dispatch::inspection value;for(unsigned n{};n<1000;++n){auto status=component->inspect("component",value);if(status==error::success)return value;assert(status==error::again);::usleep(100);}assert(false);return {};}
    void wait_exit(){auto until=c::now()+8000000000;while(!usage().exit_observed&&c::now()<until)::usleep(1000);assert(usage().exit_observed&&!usage().processes);}
};
static void actual_roles_and_isolation(char** paths)
{
    for(auto mode:{1,2,3,8})
    {
        component_fixture f{paths,static_cast<std::uint64_t>(mode)};if(mode==2)f.push();
        assert(!f.usage().processes);f.open();assert(!f.usage().processes);assert(f.call(cop::component_start,f.keyed())==error::success);f.wait_exit();
        std::fprintf(stderr,"component mode=%d messages=%llu unknown=%llu publications=%u\n",mode,
            static_cast<unsigned long long>(f.usage().messages),static_cast<unsigned long long>(f.usage().unknown),f.native->publisher->publications.load());
        assert(!f.usage().unknown&&f.usage().messages==static_cast<unsigned>(mode==2||mode==8?2:1));
        assert(f.native->publisher->publications==static_cast<unsigned>(mode==2?0:mode==8?2:1));
        if(mode==2)assert(!f.native->queue->has_reservation());
        assert(f.call(cop::component_describe,resource(f.instance_id))==error::success);robot::response response;
        assert(robot::decode_response(cop::component_describe,f.response,{2048,2048,255,16},response)&&response.execution.exit_observed&&!response.execution.processes&&!response.execution.first_failure);
    }
}
static void exact_recovery_stop_and_revocation(char** paths)
{
    component_fixture f{paths,4,{},2};f.open();auto start=f.keyed();assert(f.call(cop::component_start,start,2048,[](auto){return false;})==error::io);
    assert(f.call(cop::component_start,start)==error::success);
    auto until=c::now()+7000000000;while(!f.usage().ready&&c::now()<until)::usleep(1000);assert(f.usage().ready);
    auto stop=f.keyed(true);assert(f.call(cop::component_stop,stop,2048,[](auto){return false;})==error::io);f.wait_exit();assert(f.call(cop::component_stop,stop)==error::success);
    robot::response result;assert(robot::decode_response(cop::component_stop,f.response,{2048,2048,255,16},result)&&result.outcome.state=="succeeded");
    assert(!f.usage().failed&&f.usage().first_failure==s::components::fault::none);
    component_fixture revoked{paths,4};revoked.open();assert(revoked.call(cop::component_start,revoked.keyed())==error::success);
    until=c::now()+7000000000;while(!revoked.usage().ready&&c::now()<until)::usleep(1000);assert(revoked.usage().ready);
    assert(revoked.permissions->revoke("configuration",[&](auto alias){revoked.component->revoke(alias);})==error::success);revoked.wait_exit();assert(revoked.usage().retired);
}
static void authority_failed_copies_and_native_unknown(char** paths)
{
    for(auto omitted:{"artifact.read","component.open","topic.publish","data.export"})
    {component_fixture denied{paths,1,omitted};assert(denied.call(cop::component_open,denied.opening())==error::denied&&!denied.usage().handles&&!denied.usage().processes);}
    component_fixture f{paths,1};assert(f.call(cop::component_open,f.opening(),7)==error::too_large);
    assert(f.call(cop::component_open,f.opening(),2048,[](auto){return false;})==error::io&&!f.usage().handles);f.open();f.native->publisher->unknown=true;
    auto start=f.keyed();assert(f.call(cop::component_start,start,255)==error::too_large&&!f.usage().processes);
    assert(f.call(cop::component_start,start)==error::success);f.wait_exit();assert(f.usage().unknown==1&&f.native->publisher->publications==1);
    assert(f.call(cop::component_start,start)==error::success&&f.native->publisher->publications==1);
    for(auto mode:{5,6}){component_fixture bad{paths,static_cast<std::uint64_t>(mode)};bad.open();assert(bad.call(cop::component_start,bad.keyed())==error::success);bad.wait_exit();assert(!bad.native->publisher->publications);}
}
static void unacknowledged_exit_and_inert_stop(char** paths)
{
    component_fixture reserved{paths,9};reserved.push();reserved.open();
    assert(reserved.call(cop::component_start,reserved.keyed())==error::success);reserved.wait_exit();
    auto usage=reserved.usage();assert(usage.unknown==1&&usage.pending>=1&&usage.messages==1&&reserved.native->queue->has_reservation());
    assert(reserved.call(cop::component_describe,resource(reserved.instance_id))==error::success);
    robot::response result;assert(robot::decode_response(cop::component_describe,reserved.response,{2048,2048,255,16},result));
    assert(result.execution.exit_observed&&result.execution.state=="outcome_unknown");
    component_fixture idle{paths,1};idle.open();auto stop=idle.keyed(true);
    assert(idle.call(cop::component_stop,stop)==error::success);
    auto until=c::now()+3000000000;while(!idle.usage().retired&&c::now()<until)::usleep(1000);
    assert(idle.usage().retired&&!idle.usage().processes&&!idle.usage().exit_observed&&!idle.native->publisher->publications);
    assert(idle.call(cop::component_stop,stop)==error::success);
    assert(robot::decode_response(cop::component_stop,idle.response,{2048,2048,255,16},result)&&result.outcome.state=="succeeded"&&result.outcome.effect=="committed");
    assert(idle.call(cop::component_start,idle.keyed())==error::denied);
}
static void receive_timeout_requires_explicit_quiescence(char** paths)
{
    using fault=component_fixture::receive_fault;
    for(auto injected:{fault::timeout_busy,fault::timeout_debt,fault::timeout_empty,fault::again_empty,fault::query_denied,fault::query_throws})
    {
        component_fixture f{paths,10,{},{},injected};f.open();
        assert(f.call(cop::component_start,f.keyed())==error::success);f.wait_exit();
        auto usage=f.usage();bool known=injected==fault::timeout_empty||injected==fault::again_empty;
        assert(f.debt_queries==1&&usage.exit_observed&&!usage.processes);
        assert(usage.messages==unsigned(known)&&usage.unknown==unsigned(!known));
        assert(f.call(cop::component_describe,resource(f.instance_id))==error::success);
        robot::response result;assert(robot::decode_response(cop::component_describe,f.response,{2048,2048,255,16},result));
        assert((result.execution.state=="outcome_unknown")==!known);
        if(!known)
        {
            assert(f.component->close(f.who,f.component_cap,f.instance_id)==error::success);
            auto retired=f.usage();assert(retired.retired&&retired.unknown==1&&retired.pending>=1&&!retired.handles);
            assert(f.call(cop::component_open,f.opening())==error::denied);
        }
    }
    component_fixture f{paths,10};auto incomplete=s::bridge_dispatch::transport::from(f.topics);incomplete.quiescent=nullptr;
    s::component_dispatch rejected{f.permissions,f.inputs,incomplete,72,{1,f.policy.retained_bytes}};
    assert(rejected.bind(f.policy,f.images)==error::invalid);
}
static void native_crash_remains_a_failure_after_reaping(char** paths)
{
    component_fixture f{paths,11};f.open();auto start=f.keyed();
    assert(f.call(cop::component_start,start)==error::success);f.wait_exit();
    auto usage=f.usage();assert(usage.failed&&usage.exit_observed&&!usage.processes&&!usage.messages&&!usage.unknown&&!f.native->publisher->publications);
    assert(f.call(cop::component_describe,resource(f.instance_id))==error::success);
    robot::response result;assert(robot::decode_response(cop::component_describe,f.response,{2048,2048,255,16},result));
    assert(result.execution.state=="failed"&&result.execution.exit_observed&&result.execution.first_failure==4);
    // Retry recovers the original start receipt; it cannot restart the image.
    assert(f.call(cop::component_start,start)==error::success);
    assert(f.usage().exit_observed&&!f.usage().processes&&!f.native->publisher->publications);
    auto changed=start;changed[8]=2;assert(f.call(cop::component_start,changed)==error::conflict);
    // A subsequent successful stop/inspection cannot replace the first fault.
    assert(f.call(cop::component_stop,f.keyed(true))==error::success);
    for(unsigned i{};i<3;++i)
    {
        assert(f.call(cop::component_describe,resource(f.instance_id))==error::success);
        assert(robot::decode_response(cop::component_describe,f.response,{2048,2048,255,16},result));
        assert(result.execution.state=="failed"&&result.execution.first_failure==4&&result.execution.exit_observed);
    }
    assert(f.component->close(f.who,f.component_cap,f.instance_id)==error::success);
    auto retired=f.usage();assert(retired.retired&&retired.failed&&retired.first_failure==s::components::fault::channel_failed);
}
static void queued_start_cancellation_does_not_invent_failure(char** paths)
{
    for(unsigned attempt{};attempt<12;++attempt)
    {
        component_fixture f{paths,4};f.open();auto start=f.keyed();
        assert(f.call(cop::component_start,start)==error::success);
        auto stop=f.keyed(true);assert(f.call(cop::component_stop,stop)==error::success);
        auto until=c::now()+8000000000;
        for(;;)
        {
            assert(f.call(cop::component_stop,stop)==error::success);robot::response result;
            assert(robot::decode_response(cop::component_stop,f.response,{2048,2048,255,16},result));
            if(result.outcome.state=="succeeded")break;
            assert(c::now()<until);::usleep(1000);
        }
        auto observed=f.usage();assert(!observed.failed&&observed.first_failure==s::components::fault::none&&!observed.processes);
        assert(f.call(cop::component_start,start)==error::success);robot::response result;
        assert(robot::decode_response(cop::component_start,f.response,{2048,2048,255,16},result));
        assert(result.outcome.state!="queued"&&result.outcome.state!="running");
        if(!observed.exit_observed)assert(result.outcome.state=="cancelled"&&result.outcome.effect=="none");
    }
}
#ifndef UWVM_COMPONENT_DISPATCH_TEST_MAIN
#define UWVM_COMPONENT_DISPATCH_TEST_MAIN main
#endif
int UWVM_COMPONENT_DISPATCH_TEST_MAIN(int argc,char** argv)
{assert(argc==10);actual_roles_and_isolation(argv+1);exact_recovery_stop_and_revocation(argv+1);authority_failed_copies_and_native_unknown(argv+1);unacknowledged_exit_and_inert_stop(argv+1);receive_timeout_requires_explicit_quiescence(argv+1);native_crash_remains_a_failure_after_reaping(argv+1);queued_start_cancellation_does_not_invent_failure(argv+1);std::puts("passed 7 real isolated component safety groups");return 0;}
