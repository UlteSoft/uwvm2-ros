/** @file @brief Device/control canonical calls, real sensor witnesses, bounded PTY effects and stop recovery. */
#define UWVM_SENSOR_SOURCE_FIXTURE_ONLY
#include "sensor_source.cpp"
#include <uwvm2/imported/ros/scene/device_dispatch.h>
#include <uwvm2/imported/ros/scene/device_configuration.h>
#include <uwvm2/imported/ros/scene/device_supervisor.h>
#include <poll.h>
namespace d=s::devices;namespace protocol=d::protocol;namespace robot=s::robot_contract;
using op=robot::operation;using vector=uwvm2::utils::container::std_vector<std::uint8_t>;
static constexpr std::string_view child_module="uwvm_ros_v2.platform_extension.fixture.v1";
static s::platform_extensions::policy child_policy(int selected)
{
    constexpr std::array routes{s::platform_extensions::route{"session.open",op::device_open},
        s::platform_extensions::route{"joint.command",op::device_submit},s::platform_extensions::route{"cycle.submit",op::control_submit}};
    s::platform_extensions::policy policy;assert(policy.register_module({child_module,routes})==error::success);
    s::grant granted;granted.alias="drive";granted.kind=s::purpose::device;granted.binding="framed-scalar";
    assert(policy.bind(child_module,granted)==error::success);
    if(selected>=0)assert(policy.select(child_module,selected!=0)==error::success);
    return policy;
}
struct dispatch_terminal
{
    int host{-1},peer{-1};
    dispatch_terminal()
    {
        host=::posix_openpt(O_RDWR|O_NOCTTY|O_NONBLOCK|O_CLOEXEC);assert(host>=0&&::grantpt(host)==0&&::unlockpt(host)==0);
        std::array<char,256> name;assert(::ptsname_r(host,name.data(),name.size())==0);peer=::open(name.data(),O_RDWR|O_NOCTTY|O_NONBLOCK|O_CLOEXEC);assert(peer>=0);
        termios settings;assert(::tcgetattr(peer,&settings)==0);::cfmakeraw(&settings);assert(::tcsetattr(peer,TCSANOW,&settings)==0);
    }
    ~dispatch_terminal(){::close(host);::close(peer);}
};
static vector record(auto&& encode)
{std::array<std::uint8_t,16384> storage;s::wire::writer writer{storage};assert(encode(writer));return {writer.written().begin(),writer.written().end()};}
static bool put_profile(s::wire::writer& out,s::buffers::profile const& profile){return s::proposals::write_profile(out,profile);}
static s::device_dispatch::device_plan device_policy()
{
    s::device_dispatch::device_plan out;out.alias.assign("drive");out.mode=profile("scalar");out.watchdog=profile("receiver-lease");
    auto& p=out.ledger;p.profile=profile("drive.plan");p.stop_profile=profile("drive.stop");p.schema.profile=profile("drive.command");
    p.schema.frame.assign("base");p.schema.count=1;p.schema.fields[0].name.assign("joint");p.schema.fields[0].quantity=d::unit::radian;
    p.schema.fields[0].minimum=-1;p.schema.fields[0].maximum=1;p.handles=2;p.requests=2;p.body_bytes=256;p.observations=p.minimum_observations=1;
    p.maximum_age_ns=p.maximum_deadline_ns=100;assert(d::core::required_storage(p,p.storage_bytes));
    out.transport={4096,200};out.arbitration={1000,100,10,false};assert(s::device_dispatch::required_storage(out,out.retained_bytes));return out;
}
struct device_fixture:sensor_fixture
{
    dispatch_terminal commands,acks,stops,stop_acks;protocol::receiver receiver;
    s::device_dispatch::device_plan dp=device_policy();s::device_dispatch::control_plan cp;
    uwvm2::utils::container::std_unique_ptr<s::device_dispatch> host;
    s::handle device_cap{},control_cap{},buffer_cap{},device{},control{},epoch{};
    vector output;robot::response response;
    static void configure(s::authority& permissions,std::string_view omitted,std::optional<std::uint64_t> control_calls)
    {
        for(auto const& action:robot::operations)if(action.id<=op::control_stop)assert(permissions.expose(uwvm2::utils::container::std_string(action.name),s::exposure::allow)==error::success);
        assert(permissions.expose("buffer.read",s::exposure::allow)==error::success);
        for(auto [alias,kind,binding]:{std::tuple{"drive",s::purpose::device,"framed-scalar"},std::tuple{"controller",s::purpose::control,"framed-control"},std::tuple{"frames",s::purpose::buffer,"sealed-buffer"}})
        {
            s::grant grant;grant.alias=alias;grant.resource=alias;grant.kind=kind;grant.binding=binding;grant.max_resources=2;grant.max_handles=2;
            for(auto const& action:robot::operations)
                if(action.id<=op::control_stop&&s::permits(kind,action.name)&&action.name!=omitted)grant.actions.emplace(action.name);
            if(kind==s::purpose::buffer){if(omitted!="buffer.read")grant.actions.emplace("buffer.read");if(omitted!="data.export")grant.actions.emplace("data.export");}
            grant.actions.emplace("resource.close");
            if(kind==s::purpose::control&&control_calls){grant.limits.calls=control_calls;grant.limits.uncertain=true;grant.limits.window=std::chrono::hours(1);}
            assert(permissions.configure(grant)==error::success);
        }
    }
    device_fixture(std::string_view omitted={},std::optional<std::uint64_t> control_calls={},int platform_state=-2)
        :sensor_fixture(source_grant(),3,[&](auto& permissions){configure(permissions,omitted,control_calls);})
    {
        open();assert(provider.ingest(sample(),payload)==error::success&&take()==error::success);
        assert(permissions->open(who,"drive",device_cap)==error::success&&permissions->open(who,"controller",control_cap)==error::success&&permissions->open(who,"frames",buffer_cap)==error::success);
        cp.alias.assign("controller");cp.device_alias.assign("drive");cp.selected=profile("controller.plan");cp.stop=dp.ledger.stop_profile;
        cp.sources[0]=rule;cp.source_count=1;cp.lease_ns=100;cp.priority=1;cp.write_window_ns=50;
        assert(s::device_dispatch::required_storage(cp,cp.retained_bytes));
        host=uwvm2::utils::container::make_unique<s::device_dispatch>(permissions,buffers,1,3,s::device_dispatch::limits{1,1,dp.retained_bytes+cp.retained_bytes},clock_now);
        if(platform_state!=-2)assert(host->configure_extensions(child_policy(platform_state))==error::success);
        auto endpoints=platform_state==-1||platform_state==0?std::array<int,4>{-1,-1,-1,-1}:
            std::array<int,4>{commands.host,acks.host,stops.host,stop_acks.host};
        assert(host->bind(dp,endpoints)==error::success&&host->bind(cp)==error::success&&host->prepare(provider.access())==error::success);
        assert(receiver.configure(3,dp.ledger.schema,dp.ledger.stop_profile)==error::success&&receiver.rearm_after_stopped(3,1)==error::success);
    }
    error call(op action,vector const& input,std::size_t capacity=4096,s::device_dispatch::sink custom={})
    {
        auto copying=[&](s::wire::bytes bytes){output.assign(bytes.begin(),bytes.end());return true;};std::size_t required{};
        return host->invoke(who,action,action>=op::control_open?control_cap:device_cap,input,capacity,required,custom?custom:s::device_dispatch::sink(copying));
    }
    void decode(op action){assert(robot::decode_response(action,output,{4096,8192,255,32},response));}
    vector open_device(){return record([&](auto& out){return put_profile(out,dp.ledger.profile);});}
    vector open_control(){return record([&](auto& out){return put_profile(out,cp.selected)&&out.u64(device_cap)&&out.u64(device)&&out.u64(1)&&out.u64(1)&&out.u64(cap)&&out.u64(resource)&&out.u64(buffer_cap);});}
    vector query(s::handle id){return record([&](auto& out){return out.u64(id);});}
    void start()
    {assert(call(op::device_open,open_device())==error::success);decode(op::device_open);device=response.handle;
        assert(call(op::control_open,open_control())==error::success);decode(op::control_open);control=response.handle;epoch=response.epoch;
        assert(host->pump("drive")==error::again);acknowledge(protocol::kind::controller);
        for(unsigned i{};i<16;++i)(void)host->pump("drive");}
    void acknowledge(protocol::kind kind,bool wrong=false)
    {
        protocol::stream_decoder decoder{kind};auto fd=kind==protocol::kind::stop?stops.peer:commands.peer;
        while(decoder.frame().empty())
        {
            pollfd wait{fd,POLLIN,0};assert(::poll(&wait,1,3000)==1);std::array<std::uint8_t,256> bytes;
            auto count=::read(fd,bytes.data(),std::min(bytes.size(),decoder.remaining()));assert(count>0);std::size_t used{};
            auto status=decoder.feed({bytes.data(),static_cast<std::size_t>(count)},used);assert(status==error::success||status==error::again);
        }
        protocol::header original;d::scalar_command values;
        auto status=kind==protocol::kind::controller?receiver.controller(decoder.frame(),time_now,original):
            kind==protocol::kind::command?receiver.command(decoder.frame(),time_now,original,values):receiver.stop(decoder.frame(),original);
        assert(status==error::success);if(kind==protocol::kind::command)assert(values.values[0]==0.5);
        if(wrong)original.digest[0]^=1;
        protocol::packet packet;assert(protocol::acknowledge(original,kind==protocol::kind::controller?protocol::acknowledgement::controller_installed:
            kind==protocol::kind::command?protocol::acknowledgement::executed:protocol::acknowledgement::stop_accepted,original.sequence,packet)==error::success);
        auto reply=kind==protocol::kind::stop?stop_acks.peer:acks.peer;
        assert(::write(reply,packet.data.data(),packet.size)==static_cast<ssize_t>(packet.size));
    }
    vector command_body()
    {return record([&](auto& out){d::scalar_command value;value.count=1;value.values[0]=0.5;return d::encode(out,dp.ledger.schema,value);});}
    vector submit_cycle(std::uint64_t sequence=1,std::uint64_t key=1)
    {auto body=command_body();return record([&](auto& out){return out.u64(control)&&out.u64(key)&&out.u64(control)&&out.u64(sequence)&&out.u64(epoch)&&
        put_profile(out,dp.ledger.schema.profile)&&out.text(d::scalar_type)&&out.text(d::scalar_encoding)&&out.blob(body);});}
    vector submit_device(std::uint64_t key=1)
    {
        auto actual=capture();auto body=command_body();auto const& source=actual.observation;
        return record([&](auto& out){return out.u64(device)&&out.u64(key)&&out.u64(device)&&out.u64(control_cap)&&out.u64(control)&&out.u64(epoch)&&
            put_profile(out,dp.mode)&&put_profile(out,dp.ledger.schema.profile)&&out.text(d::scalar_type)&&out.text(d::scalar_encoding)&&out.blob(body)&&
            out.u64(1)&&out.u64(buffer_cap)&&out.u64(source.buffer)&&out.u64(source.source)&&out.u64(source.source_generation)&&out.boolean(true)&&
            out.u64(source.clock)&&out.u64(source.epoch)&&out.i64(source.source_ns)&&out.boolean(true)&&out.text(actual.frame.view())&&out.u64(0)&&out.u64(1050);});
    }
    vector stop(bool device_stop=false,std::uint64_t key=1)
    {auto id=device_stop?device:control;return record([&](auto& out){return out.u64(id)&&out.u64(key)&&out.u64(id)&&put_profile(out,dp.ledger.stop_profile);});}
    s::device_dispatch::inspection usage()
    {
        s::device_dispatch::inspection out;
        for(unsigned i{};i<1000;++i){auto status=host->inspect("drive",out);if(status==error::success)return out;assert(status==error::again);::usleep(100);}
        assert(false);return out;
    }
};
static void complete_device_and_cycle_effects()
{
    for(bool cycle:{false,true})
    {
        device_fixture f;f.start();assert(f.usage().ready);
        assert(f.call(op::device_describe,f.query(f.device))==error::success);f.decode(op::device_describe);assert(f.response.device.state=="ready"&&f.response.device.watchdog.armed);
        if(cycle){assert(f.call(op::control_sample,f.query(f.control))==error::success);f.decode(op::control_sample);assert(f.response.sample.sequence==1&&f.response.sample.observations.count==1);}
        auto wire=cycle?f.submit_cycle():f.submit_device();auto action=cycle?op::control_submit:op::device_submit;
        assert(f.call(action,wire)==error::success);f.decode(action);auto request=f.response.outcome.request;
        assert(f.response.outcome.state=="queued"&&f.usage().requests==1&&f.usage().pins==1);
        assert(f.call(action,wire)==error::success&&f.usage().requests==1);assert(f.host->pump("drive")==error::again);
        f.acknowledge(protocol::kind::command);for(unsigned i{};i<16;++i)(void)f.host->pump("drive");
        auto query=record([&](auto& out){return out.u64(cycle?f.control:f.device)&&out.u64(cycle?1:request);});
        auto result=cycle?op::control_result:op::device_result;assert(f.call(result,query)==error::success);f.decode(result);
        assert(f.response.outcome.state=="succeeded"&&f.response.outcome.effect=="committed"&&!f.response.has_evidence);
        assert(f.call(op::control_stop,f.stop())==error::success);(void)f.host->pump("drive");f.acknowledge(protocol::kind::stop);
        for(unsigned i{};i<16;++i){(void)f.host->pump("drive");}assert(f.usage().stop_acknowledged);
        assert(f.call(op::control_stop,f.stop())==error::success);f.decode(op::control_stop);assert(f.response.outcome.effect=="observed_remote");
        assert(f.call(op::device_describe,f.query(f.device))==error::success);f.decode(op::device_describe);assert(f.response.device.state=="stopping");
        assert(f.host->close(f.who,f.device_cap,request)==error::success);
    }
}
static void independent_grants_and_zero_rates()
{
    for(auto missing:{"device.open","control.open","device.submit","device.stop","buffer.read","data.export"})
    {
        device_fixture f{missing};auto status=f.call(op::device_open,f.open_device());
        if(std::string_view(missing)=="device.open"){assert(status==error::denied);continue;}
        assert(status==error::success);f.decode(op::device_open);f.device=f.response.handle;
        assert(f.call(op::control_open,f.open_control())==error::denied&&!f.usage().ready&&!f.usage().requests);
    }
    device_fixture zero{{},0};assert(zero.call(op::device_open,zero.open_device())==error::success);zero.decode(op::device_open);zero.device=zero.response.handle;
    assert(zero.call(op::control_open,zero.open_control())==error::again);
    device_fixture once{{},2};once.start();assert(once.call(op::control_sample,once.query(once.control))==error::success);
    assert(once.call(op::control_submit,once.submit_cycle())==error::success);assert(once.call(op::control_stop,once.stop())==error::success);
}
static void failed_copies_deadlines_and_unknown_receipts()
{
    device_fixture f;assert(f.call(op::device_open,f.open_device(),7)==error::too_large&&!f.usage().handles);
    assert(f.call(op::device_open,f.open_device(),4096,[](auto){return false;})==error::io&&!f.usage().handles);
    f.start();assert(f.call(op::control_sample,f.query(f.control),4096,[](auto){return false;})==error::io&&!f.usage().pins);
    assert(f.call(op::control_sample,f.query(f.control))==error::success);auto wire=f.submit_cycle();
    assert(f.call(op::control_submit,wire,255)==error::too_large&&!f.usage().requests);
    assert(f.call(op::control_submit,wire,4096,[](auto){return false;})==error::io&&f.usage().requests==1);
    assert(f.call(op::control_submit,wire)==error::success&&f.usage().requests==1);
    assert(f.host->pump("drive")==error::again);f.acknowledge(protocol::kind::command,true);
    for(unsigned i{};i<16&&!f.usage().unknown;++i){(void)f.host->pump("drive");}assert(f.usage().unknown==1&&f.usage().stop_pending);
    // A corrupt native acknowledgement permanently fences both accounts.
    // Retained debt is still visible to trusted diagnostics; no guest retry,
    // reopening or new stop admission may revive the retired incarnation.
    assert(f.usage().retired&&f.usage().fault.cause==d::execution::failure::protocol_violation);
    assert(f.call(op::control_stop,f.stop())==error::denied);time_now=5000;
    assert(f.call(op::control_submit,wire)==error::denied&&f.usage().requests==1);
}
static void source_invalidation_and_callback_revocation()
{
    device_fixture f;f.start();assert(f.call(op::device_submit,f.submit_device())==error::success);
    assert(f.provider.ingest(sample(2),payload)==error::success);
    assert(f.host->pump("drive")==error::conflict||f.usage().latched);assert(f.usage().stop_pending&&!f.usage().unknown);
    device_fixture revoked;revoked.start();
    assert(revoked.call(op::control_sample,revoked.query(revoked.control),4096,[&](auto)
        {assert(revoked.permissions->revoke("frames",[&](auto alias){revoked.host->revoke(alias);})==error::success);return true;})!=error::success);
    assert(revoked.usage().retired&&revoked.usage().stop_pending&&!revoked.usage().requests);
}
static void closed_handle_history_and_channel_separation()
{
    device_fixture f;
    for(unsigned i{};i<64;++i)
    {
        assert(f.call(op::device_open,f.open_device())==error::success);f.decode(op::device_open);
        assert(f.host->close(f.who,f.device_cap,f.response.handle)==error::success&&!f.usage().handles);
    }
    f.start();assert(f.call(op::device_submit,f.submit_device())==error::success);f.decode(op::device_submit);auto request=f.response.outcome.request;
    assert(f.host->close(f.who,f.device_cap,f.device)==error::success);
    auto history=record([&](auto& out){return out.u64(f.device)&&out.u64(request);});
    assert(f.call(op::device_result,history)==error::success);f.decode(op::device_result);assert(f.response.outcome.state=="cancelled");
    assert(f.host->close(f.who,f.device_cap,request)==error::success);
    assert(f.call(op::device_result,history)==error::bad_handle);
    device_fixture separated;
    s::device_dispatch rejected{separated.permissions,separated.buffers,1,3,{1,1,separated.dp.retained_bytes+separated.cp.retained_bytes},clock_now};
    assert(rejected.bind(separated.dp,{separated.commands.host,separated.commands.peer,separated.stops.host,separated.stop_acks.host})==error::denied);
}
static void complete_configuration_no_native_effect()
{
    auto dp=device_policy();auto definition=record([&](auto& out)
    {
        return out.text(s::device_configuration::magic)&&out.text(dp.ledger.profile.identity.view())&&put_profile(out,dp.mode)&&put_profile(out,dp.watchdog)&&
            put_profile(out,dp.ledger.stop_profile)&&put_profile(out,dp.ledger.schema.profile)&&out.text("base")&&out.u64(1)&&out.text("joint")&&out.text("rad")&&out.f64(-1)&&out.f64(1);
    });
    dp.ledger.profile.sha256=s::buffers::hash(definition);s::device_configuration config;
    assert(config.definition(definition)==error::denied&&config.profile("drive",dp.ledger.profile)==error::success);
    auto corrupt=definition;corrupt.back()^=1;assert(config.definition(corrupt)==error::invalid&&config.definition(definition)==error::success);
    assert(config.endpoints({100,101,102,102})==error::conflict&&config.endpoints({100,101,102,103})==error::success);
    s::device_dispatch::device_plan frozen;std::array<int,4> fds;
    assert(config.freeze(frozen,fds)==error::invalid);
    std::array<std::uint64_t,14> values{2,2,256,1,1,100,100,dp.ledger.storage_bytes,dp.retained_bytes,4096,200,1000,100,10};
    for(std::size_t i{};i<values.size();++i)assert(config.limit(s::device_configuration::names[i],values[i])==error::success);
    assert(config.limit("handles",2)==error::conflict&&config.freeze(frozen,fds)==error::success&&fds[3]==103);
    assert(frozen.ledger.schema.fields[0].minimum==-1&&frozen.ledger.schema.fields[0].maximum==1);
    device_fixture base;auto cp=base.cp;
    auto control=record([&](auto& out){return out.text(s::control_configuration::magic)&&out.text(cp.selected.identity.view())&&out.text("drive")&&
        put_profile(out,cp.stop)&&out.u64(1)&&out.text("sensor")&&put_profile(out,cp.sources[0].profile)&&out.text("frames")&&put_profile(out,cp.sources[0].buffer_profile)&&out.text("base");});
    cp.selected.sha256=s::buffers::hash(control);s::control_configuration configured;
    assert(configured.profile("controller",cp.selected)==error::success&&configured.definition(control)==error::success);
    std::array<std::uint64_t,4> limits{100,1,50,cp.retained_bytes};
    for(std::size_t i{};i<limits.size();++i)assert(configured.limit(s::control_configuration::names[i],limits[i])==error::success);
    assert(configured.freeze(cp)==error::success&&cp.sources[0].buffer_alias.view()=="frames");
}
static std::uint64_t supervisor_origin{};
static std::uint64_t independent_clock()noexcept
{return 1000+static_cast<std::uint64_t>(uwvm2::imported::ros::resources::steady_now())-supervisor_origin;}
static void autonomous_expiry_and_zero_authority()
{
    device_fixture f;f.host.reset();
    f.dp.ledger.maximum_age_ns=UINT64_C(10000000000);f.dp.ledger.maximum_deadline_ns=UINT64_C(1000000000);
    f.dp.arbitration.maximum_lease_ns=UINT64_C(1000000000);f.dp.arbitration.prepare_ns=UINT64_C(200000000);
    f.dp.transport.stop_ack_ns=UINT64_C(300000000);f.cp.lease_ns=UINT64_C(500000000);f.cp.write_window_ns=UINT64_C(100000000);
    supervisor_origin=static_cast<std::uint64_t>(uwvm2::imported::ros::resources::steady_now());
    f.host=uwvm2::utils::container::make_unique<s::device_dispatch>(f.permissions,f.buffers,1,3,s::device_dispatch::limits{1,1,f.dp.retained_bytes+f.cp.retained_bytes},independent_clock);
    assert(f.host->bind(f.dp,{f.commands.host,f.acks.host,f.stops.host,f.stop_acks.host})==error::success&&f.host->bind(f.cp)==error::success&&f.host->prepare(f.provider.access())==error::success);
    f.start();
    s::device_supervisor supervisor{*f.host,{1048576,1000000,500000000}};assert(supervisor.start()==error::success);
    // No more guest calls: the real host clock expires the receiver lease and
    // the independent supervisor transmits the separately reserved stop frame.
    f.acknowledge(protocol::kind::stop);
    for(unsigned i{};i<200&&!f.usage().stop_acknowledged;++i)::usleep(1000);
    assert(f.usage().stop_acknowledged&&f.usage().latched);supervisor.retire();
    device_fixture idle;idle.dp.transport.stop_ack_ns=200;
    s::device_supervisor unused{*idle.host,{1048576,1000000,5000000}};assert(unused.start()==error::success);unused.retire();
    std::uint8_t byte{};assert(::read(idle.commands.peer,&byte,1)<0&&errno==EAGAIN);assert(::read(idle.stops.peer,&byte,1)<0&&errno==EAGAIN);
}
static void platform_child_and_common_authority()
{
    auto extension=[](device_fixture& f,std::string_view module,std::string_view operation,s::handle cap,vector const& request,
                      s::device_dispatch::sink custom={})
    {
        auto copy=[&](s::wire::bytes bytes){f.output.assign(bytes.begin(),bytes.end());return true;};std::size_t required{42};
        return f.host->invoke_extension(f.who,module,operation,cap,request,4096,required,custom?custom:s::device_dispatch::sink(copy));
    };
    for(int selected:{-1,0})
    {
        // Invalid descriptors deliberately prove no OS activation happened.
        device_fixture f{{},{},selected};assert(f.call(op::device_open,f.open_device())==error::denied);
        assert(extension(f,child_module,"session.open",f.device_cap,f.open_device())==error::denied);
        assert(f.host->pump("drive")==error::denied&&!f.usage().handles&&!f.usage().ready);
        std::uint8_t byte;assert(::read(f.commands.peer,&byte,1)<0&&errno==EAGAIN);
        assert(::read(f.stops.peer,&byte,1)<0&&errno==EAGAIN);
    }
    device_fixture absent{"device.open",{},1};
    assert(extension(absent,child_module,"session.open",absent.device_cap,absent.open_device())==error::denied);
    device_fixture f{{},{},1};f.start();auto request=f.submit_device();
    assert(extension(f,"uwvm_ros_v2.platform_extension.other.v1","joint.command",f.device_cap,request)==error::denied);
    assert(extension(f,child_module,"arbitrary.invoke",f.device_cap,request)==error::denied);
    assert(extension(f,child_module,"joint.command",f.control_cap,request)==error::denied);
    assert(extension(f,child_module,"joint.command",f.device_cap,request)==error::success&&f.usage().requests==1);
    // Both entry points use the exact same replay receipt and native request.
    auto receipt=f.output;assert(f.call(op::device_submit,request)==error::success&&f.output==receipt&&f.usage().requests==1);
    assert(f.host->configure_extensions(child_policy(0))==error::denied);
    assert(f.host->pump("drive")==error::again);f.acknowledge(protocol::kind::command);
    for(unsigned i{};i<16;++i)(void)f.host->pump("drive");
    assert(f.permissions->revoke("drive",[&](auto alias){f.host->revoke(alias);})==error::success);
    assert(extension(f,child_module,"joint.command",f.device_cap,request)==error::denied);
    (void)f.host->pump("drive");f.acknowledge(protocol::kind::stop);
    for(unsigned i{};i<16;++i){(void)f.host->pump("drive");}assert(f.usage().stop_acknowledged);
    device_fixture callback{{},{},1};callback.start();
    assert(extension(callback,child_module,"joint.command",callback.device_cap,callback.submit_device(),[&](auto)
        {assert(callback.permissions->revoke("drive",[&](auto alias){callback.host->revoke(alias);})==error::success);return true;})!=error::success);
    assert(callback.usage().retired&&callback.usage().stop_pending);
    callback.host->retire();assert(extension(callback,child_module,"session.open",callback.device_cap,callback.open_device())==error::denied);
    device_fixture attenuated{{},{},1};attenuated.start();s::attenuation reduction;
    reduction.calls=0;reduction.bytes=0;reduction.uncertain=true;reduction.window=std::chrono::hours(1);
    assert(attenuated.permissions->attenuate("drive",reduction)==error::success);
    assert(extension(attenuated,child_module,"joint.command",attenuated.device_cap,attenuated.submit_device())==error::again);
    assert(attenuated.call(op::device_submit,attenuated.submit_device())==error::again&&!attenuated.usage().requests);
    assert(attenuated.permissions->revoke("drive",[&](auto alias){attenuated.host->revoke(alias);})==error::success);
    (void)attenuated.host->pump("drive");attenuated.acknowledge(protocol::kind::stop);
    for(unsigned i{};i<16;++i)(void)attenuated.host->pump("drive");
    assert(attenuated.usage().stop_acknowledged);
}
#if !defined(UWVM_DEVICE_DISPATCH_FIXTURE_ONLY)
int main()
{complete_device_and_cycle_effects();independent_grants_and_zero_rates();failed_copies_deadlines_and_unknown_receipts();source_invalidation_and_callback_revocation();
 closed_handle_history_and_channel_separation();complete_configuration_no_native_effect();autonomous_expiry_and_zero_authority();platform_child_and_common_authority();
 std::puts("passed 8 device/control dispatch safety groups");}
#endif
