/** @file @brief Real guest sensor handles, independent native writer, exports and bounded parser faults. */
#define UWVM_SENSOR_SOURCE_FIXTURE_ONLY
#include "sensor_source.cpp"
#include <uwvm2/imported/ros/scene/sensor_configuration.h>
#include <vector>
#include <poll.h>
#include <sys/socket.h>
namespace sc=uwvm2::imported::ros::sensor_contract;namespace ip=n::ingress_protocol;
static void(*native_clock_hook)()noexcept{};
static std::uint64_t native_clock()noexcept{if(native_clock_hook)native_clock_hook();return time_now;}
static s::grant exporting_grant(bool enabled=true)
{auto value=source_grant();if(enabled)value.actions.emplace("data.export");return value;}
struct native_sensor_fixture:sensor_fixture
{
    s::sensor_dispatch::configuration configuration;
    uwvm2::utils::container::std_unique_ptr<s::sensor_dispatch> dispatch;
    int descriptors[2]{-1,-1};s::handle sensor{};
    explicit native_sensor_fixture(s::grant grant=exporting_grant(),std::uint64_t read_bytes=7,bool terminal=false):sensor_fixture(std::move(grant),8)
    {
        assert(provider.ingress_description(configuration.source)==error::success);configuration.actual=source;
        configuration.transport={4,read_bytes,0};assert(n::ingress::required_storage(configuration.transport,configuration.transport.retained_bytes));
        configuration.workspace_bytes=4096;configuration.workspace_calls=8;
        std::uint64_t required{};assert(s::sensor_dispatch::required_storage(configuration,required));
        dispatch=uwvm2::utils::container::make_unique<s::sensor_dispatch>(permissions,buffers,1,3,s::sensor_dispatch::limits{1,required},native_clock);
        if(terminal)
        {
            descriptors[0]=::posix_openpt(O_RDWR|O_NOCTTY|O_NONBLOCK|O_CLOEXEC);assert(descriptors[0]>=0&&::grantpt(descriptors[0])==0&&::unlockpt(descriptors[0])==0);
            descriptors[1]=::open(::ptsname(descriptors[0]),O_RDWR|O_NOCTTY|O_NONBLOCK|O_CLOEXEC);assert(descriptors[1]>=0);
            ::termios raw{};assert(::tcgetattr(descriptors[1],&raw)==0);::cfmakeraw(&raw);assert(::tcsetattr(descriptors[1],TCSANOW,&raw)==0);
        }
        else assert(::pipe2(descriptors,O_NONBLOCK|O_CLOEXEC)==0);
        assert(dispatch->bind(configuration,descriptors[0])==error::success&&dispatch->prepare()==error::success);
    }
    ~native_sensor_fixture(){for(auto fd:descriptors)if(fd>=0)::close(fd);}
    error call(sc::operation action,s::wire::bytes input,std::vector<std::uint8_t>& output,std::size_t capacity=4096,
               s::sensor_dispatch::sink custom={},s::sensor_dispatch::cancellation cancelled={})
    {
        auto copy=[&](auto bytes){output.assign(bytes.begin(),bytes.end());return true;};std::size_t required{};
        return dispatch->invoke(who,action,cap,input,capacity,required,custom?custom:s::sensor_dispatch::sink(copy),cancelled);
    }
    std::vector<std::uint8_t> open_wire()
    {std::array<std::uint8_t,144> bytes;s::wire::writer out{bytes};assert(s::proposals::write_profile(out,configuration.source.ingress.profile));return {out.written().begin(),out.written().end()};}
    std::vector<std::uint8_t> query(s::handle id=0)
    {std::vector<std::uint8_t> bytes(8);s::wire::writer out{bytes};out.u64(id?id:sensor);return bytes;}
    void open()
    {std::vector<std::uint8_t> out;assert(call(sc::operation::open,open_wire(),out)==error::success);s::wire::reader reader{out};assert(reader.u64(sensor)&&sensor&&reader.done());}
    std::vector<std::uint8_t> frame(std::uint64_t sequence=1)
    {std::vector<std::uint8_t> bytes(ip::header_bytes+payload.size());std::size_t extent{};assert(ip::encode(sample(sequence),configuration.source.ingress.schema,payload,bytes,extent)==error::success&&extent==bytes.size());return bytes;}
    void write(s::wire::bytes bytes)
    {assert(::write(descriptors[1],bytes.data(),bytes.size())==static_cast<ssize_t>(bytes.size()));}
    n::ingress::usage native()
    {n::ingress::usage value;assert(dispatch->inspect("sensor",value)==error::success);return value;}
    error take(std::vector<std::uint8_t>& output,s::sensor_dispatch::sink custom={})
    {
        for(unsigned n{};n<1000;++n){auto status=call(sc::operation::take,query(),output,4096,custom);if(status!=error::again)return status;::pollfd wait{descriptors[0],POLLIN,0};assert(::poll(&wait,1,1)>=0);}assert(false);return error::timeout;
    }
    s::handle decode(s::wire::bytes bytes)
    {
        s::wire::reader in{bytes};std::uint64_t resource{},generation{},sequence{},clock{},epoch{},loss{},correlation{},buffer{},native_source{},native_sequence{};
        bool stamped{},received{},publisher{},available{};std::int64_t time{};std::string_view kind,type,encoding,frame,units;s::wire::bytes body;
        s::buffers::profile schema,buffer_profile;
        assert(in.u64(resource)&&resource==sensor&&in.u64(generation)&&generation==5&&in.u64(sequence)&&sequence==1&&in.boolean(stamped)&&stamped&&
            in.u64(clock)&&clock==8&&in.u64(epoch)&&epoch==9&&in.i64(time)&&time==1&&in.boolean(received)&&received&&in.u64(clock)&&clock==6&&in.u64(epoch)&&epoch==7&&
            in.i64(time)&&time==1000&&in.boolean(publisher)&&!publisher&&in.u64(loss)&&!loss&&in.u64(correlation)&&!correlation&&in.text(kind)&&kind=="sensor.frame"&&
            in.text(type)&&type=="bounded.sample"&&in.text(encoding)&&encoding=="bytes"&&in.blob(body)&&std::equal(body.begin(),body.end(),payload.begin(),payload.end())&&
            in.u64(buffer)&&buffer&&s::proposals::read_profile(in,schema)&&schema==plan().schema&&s::proposals::read_profile(in,buffer_profile)&&buffer_profile==profile("sensor.buffer")&&
            in.text(frame)&&frame=="base"&&in.text(units)&&units=="metres"&&in.u64(native_source)&&native_source==4&&in.boolean(available)&&available&&in.u64(native_sequence)&&native_sequence==1&&in.done());
        return buffer;
    }
};
static void actual_process_guest_open_take_provenance()
{
    for(bool terminal:{false,true})
    {
        native_sensor_fixture f{exporting_grant(),7,terminal};auto frame=f.frame();auto child=::fork();assert(child>=0);
        if(!child){::alarm(3);::close(f.descriptors[0]);for(std::size_t i{};i<frame.size();i+=13){auto amount=std::min<std::size_t>(13,frame.size()-i);if(::write(f.descriptors[1],frame.data()+i,amount)!=static_cast<ssize_t>(amount))::_exit(2);}::_exit(0);}
        int result{};assert(::waitpid(child,&result,0)==child&&WIFEXITED(result)&&!WEXITSTATUS(result));
        assert(f.dispatch->pump("sensor")==error::denied&&!f.native().bytes);f.open();std::vector<std::uint8_t> out;
        assert(f.take(out)==error::success);auto buffer=f.decode(out);assert(f.native().frames==1&&f.native().bytes==frame.size());
        s::buffers::information info;assert(f.buffers.describe("frames",buffer,info)==error::success&&info.sealed&&info.sha256==s::buffers::hash(payload));
        s::proposal_dispatch::source_sample actual;assert(f.dispatch->capture(f.who,f.cap,f.sensor,f.rule,actual)==error::success&&actual.observation.buffer==buffer&&actual.observation.source==f.sensor);
        assert(f.dispatch->validate(f.who,f.cap,f.rule,actual,actual.observation,pa::observation_policy::latest_required)==error::success);
        auto before=f.native();s::sensor_dispatch::inspection cached;assert(f.dispatch->inspect("sensor",cached)==error::success&&cached.source.handles==1&&cached.native.frames==1&&
            !cached.workspace_bytes&&!cached.workspace_calls&&f.native().reads==before.reads&&f.native().bytes==before.bytes);
        assert(f.call(sc::operation::describe,f.query(),out)==error::success&&f.dispatch->close(f.who,f.cap,f.sensor)==error::success&&f.dispatch->pump("sensor")==error::denied);
    }
}
static void short_export_and_wrong_handle_precede_reads()
{
    for(int mode=0;mode<3;++mode)
    {
        native_sensor_fixture f{exporting_grant(mode!=0)};f.open();f.write(f.frame());std::vector<std::uint8_t> out;
        auto status=f.call(sc::operation::take,f.query(mode==2?f.sensor+1:0),out,mode==1?1:4096);
        assert(status==(mode==0?error::denied:mode==1?error::too_large:error::bad_handle)&&out.empty()&&!f.native().bytes&&!f.usage().objects);
        int queued{};assert(::ioctl(f.descriptors[0],FIONREAD,&queued)==0&&queued==static_cast<int>(f.frame().size()));
    }
    native_sensor_fixture f;std::vector<std::uint8_t> out;assert(f.call(sc::operation::open,f.open_wire(),out,7)==error::too_large);
    f.write(f.frame());assert(f.dispatch->pump("sensor")==error::denied);
}
static void lost_copy_retains_queue_and_no_hidden_open_charge()
{
    native_sensor_fixture f;f.open();f.write(f.frame());std::vector<std::uint8_t> out;
    assert(f.take(out,[](auto){return false;})==error::io&&f.native().frames==1&&!f.usage().objects);
    auto reads=f.native().reads;assert(f.take(out)==error::success&&f.native().reads==reads&&f.decode(out));
    auto grant=exporting_grant();grant.limits.uncertain=true;grant.limits.calls=1;grant.limits.window=std::chrono::hours(1);
    native_sensor_fixture once{grant};once.open();once.write(once.frame());
    for(unsigned i{};once.native().frames==0;++i){assert(i<100);auto status=once.dispatch->pump("sensor");assert(status==error::success||status==error::again);}
    assert(once.call(sc::operation::take,once.query(),out)==error::again&&once.native().frames==1); // Creating once never blocks ongoing native ingress.
}
static void malformed_closed_and_fragmented_frames_latch()
{
    for(int mode=0;mode<5;++mode)
    {
        native_sensor_fixture f{exporting_grant(),4096};f.open();auto frame=f.frame();
        if(mode==0)frame[0]^=1;
        if(mode==1)frame.back()^=1;
        if(mode==2)frame[8]=5; // Size exceeds the configured four-byte native frame cap.
        if(mode==3)frame[80]^=1; // Exact schema digest mismatch.
        if(mode==4){f.write({frame.data(),10});assert(f.dispatch->pump("sensor")==error::again);::close(f.descriptors[1]);f.descriptors[1]=-1;}
        else f.write(frame);
        error status=error::again;for(unsigned i{};status==error::again&&i<4;++i)status=f.dispatch->pump("sensor");
        assert(status!=error::success&&status!=error::again&&!f.native().active&&!f.native().frames&&!f.usage().objects);
        assert(f.dispatch->pump("sensor")!=error::success);
    }
}
static thread_local native_sensor_fixture* callback_fixture{};
static bool reentrant_schema(s::wire::bytes bytes)noexcept
{if(callback_fixture)assert(callback_fixture->permissions->revoke("sensor",[](auto){})==error::success);return bytes.size()==4;}
static void callback_revoke_and_closed_capability_refuse_ingress()
{
    for(int mode=0;mode<3;++mode)
    {
        native_sensor_fixture f;f.open();f.write(f.frame());std::vector<std::uint8_t> out;
        if(mode==0)assert(f.permissions->close(f.who,f.cap)==error::success);
        if(mode==1)assert(f.dispatch->close(f.who,f.cap,f.sensor)==error::success);
        if(mode==2)assert(f.permissions->revoke("sensor",[](auto){})==error::success);
        assert(f.dispatch->pump("sensor")==error::denied&&!f.native().bytes&&!f.native().frames);
    }
    native_sensor_fixture f;f.open();f.write(f.frame());std::vector<std::uint8_t> out;
    assert(f.call(sc::operation::take,f.query(),out,4096,{},[&]{assert(f.permissions->revoke("sensor",[](auto){})==error::success);return false;})==error::denied&&!f.native().bytes);
    // Core clock/schema callbacks run outside the core mutex and cannot bypass
    // the source's final authority validation.
    native_sensor_fixture native;native.open();n::core core{clock_now};auto config=plan();config.validate_body=reentrant_schema;
    assert(core.configure(owner,config,source)==error::success);callback_fixture=&native;
    assert(core.ingest(owner,sample(),payload,nullptr,[&]{s::lease held;return native.permissions->admit(native.who,native.cap,"sensor.open",0,held,false);})==error::denied);
    callback_fixture=nullptr;assert(!inspect(core).records);
}
static std::vector<std::uint8_t> definition(native_sensor_fixture& f)
{
    auto const& p=f.configuration.source;std::array<std::uint8_t,4096> bytes;s::wire::writer out{bytes};auto const& a=f.configuration.actual;
    assert(out.text(n::definition_magic)&&out.text(p.ingress.profile.identity.view())&&s::proposals::write_profile(out,p.ingress.schema)&&out.text(p.ingress.type.view())&&
        out.text(p.ingress.encoding.view())&&out.text(p.ingress.frame.view())&&out.text(p.ingress.units.view())&&out.text(p.buffer_alias.view())&&s::proposals::write_profile(out,p.buffer_profile)&&
        out.u64(a.resource)&&out.u64(a.generation)&&out.u64(a.received_clock)&&out.u64(a.received_epoch)&&out.u64(a.source_clock)&&out.u64(a.source_epoch)&&
        out.boolean(p.ingress.source_sequence)&&out.boolean(p.ingress.source_stamp)&&out.text("drop_oldest"));return {out.written().begin(),out.written().end()};
}
static void complete_configuration_and_descriptor_boundaries()
{
    native_sensor_fixture f;auto bytes=definition(f);auto selected=f.configuration.source.ingress.profile;selected.sha256=s::buffers::hash(bytes);
    auto const& p=f.configuration.source.ingress;n::schema_registration schema{p.schema,p.type,p.encoding,p.validate_body};
    n::configuration config;s::sensor_dispatch::configuration frozen;
    assert(config.begin("sensor",selected,bytes,schema)==error::success&&config.freeze(frozen)==error::invalid);
    std::array<std::uint64_t,11> values{p.handles,p.records,p.body_bytes,p.storage_bytes,2,3,4,7,f.configuration.transport.retained_bytes,4096,8};
    for(std::size_t i{};i<values.size();++i)assert(config.limit(n::configuration::names[i],values[i])==error::success);
    assert(config.limit("frame.bytes",4)==error::conflict&&config.freeze(frozen)==error::success&&config.freeze(frozen)==error::denied);
    assert(frozen.source.ingress.profile==selected&&frozen.actual.resource==4);
    auto forged=schema;forged.schema.sha256[0]^=1;n::configuration bad;assert(bad.begin("sensor",selected,bytes,forged)==error::invalid);
    for(std::size_t i{};i<bytes.size();++i){n::configuration truncated;auto digest=selected;digest.sha256=s::buffers::hash({bytes.data(),i});assert(truncated.begin("sensor",digest,{bytes.data(),i},schema)==error::invalid);}
    n::ingress channel{f.provider};assert(channel.bind(f.descriptors[1],f.configuration.transport)==error::denied);
    int sockets[2];assert(::socketpair(AF_UNIX,SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC,0,sockets)==0);assert(channel.bind(sockets[0],f.configuration.transport)==error::denied);::close(sockets[0]);::close(sockets[1]);
    auto zero=f.configuration.transport;zero.retained_bytes=0;assert(channel.bind(f.descriptors[0],zero)==error::too_large);
}
static void real_allocation_failures_and_inherited_process()
{
#if defined(UWVM_SENSOR_FAULT_INJECTION)
    unsigned failed{};
    for(std::size_t attempt=1;attempt<=12;++attempt)
    {
        native_sensor_fixture f;f.open();f.write(f.frame());while(!f.native().frames){auto status=f.dispatch->pump("sensor");assert(status==error::success||status==error::again);}
        std::vector<std::uint8_t> out;out.reserve(4096);fail_after=attempt;allocation_failed=false;auto status=f.call(sc::operation::take,f.query(),out);auto fault=allocation_failed;fail_after=0;
        if(fault){++failed;assert(status==error::no_memory&&!f.usage().objects&&f.native().frames==1);assert(f.take(out)==error::success);}
        else{assert(status==error::success);break;}
    }
    assert(failed>=3);
    native_sensor_fixture failed_bind;n::ingress channel{failed_bind.provider};fail_after=1;allocation_failed=false;
    assert(channel.bind(failed_bind.descriptors[0],failed_bind.configuration.transport)==error::no_memory&&allocation_failed);fail_after=0;
    assert(channel.bind(failed_bind.descriptors[0],failed_bind.configuration.transport)==error::success);
#endif
    native_sensor_fixture f;f.open();auto child=::fork();assert(child>=0);
    if(!child){::alarm(3);std::vector<std::uint8_t> out;::_exit(f.dispatch->pump("sensor")==error::denied&&f.call(sc::operation::take,f.query(),out)==error::denied?0:1);}
    int result{};assert(::waitpid(child,&result,0)==child&&WIFEXITED(result)&&!WEXITSTATUS(result));
}
static void real_source_clock_reentry_and_concurrent_delivery()
{
    for(int mode=0;mode<3;++mode)
    {
        native_sensor_fixture f;f.open();f.write(f.frame());callback_fixture=&f;
        native_clock_hook=mode==0?+[]()noexcept{assert(callback_fixture->permissions->revoke("sensor",[](auto){})==error::success);}:
            mode==1?+[]()noexcept{assert(callback_fixture->dispatch->close(callback_fixture->who,callback_fixture->cap,callback_fixture->sensor)==error::success);}:
            +[]()noexcept{callback_fixture->dispatch->revoke("sensor");};
        error status=error::again;for(unsigned n{};status==error::again&&n<100;++n)status=f.dispatch->pump("sensor");
        native_clock_hook=nullptr;callback_fixture=nullptr;assert(status!=error::success&&status!=error::again&&!f.native().frames&&!f.usage().objects);
    }
    native_sensor_fixture f;f.open();f.write(f.frame());std::vector<std::uint8_t> output;std::barrier gate{2};std::atomic<bool> ready{};
    std::thread retiring{[&]{gate.arrive_and_wait();f.dispatch->revoke("sensor");ready=true;gate.arrive_and_wait();}};
    auto sink=[&](auto){gate.arrive_and_wait();gate.arrive_and_wait();return true;};assert(f.take(output,sink)!=error::success);retiring.join();assert(ready&&!f.usage().objects);
}
int main()
{
    assert(prior_sensor_source_fixture_main()==0);actual_process_guest_open_take_provenance();short_export_and_wrong_handle_precede_reads();
    lost_copy_retains_queue_and_no_hidden_open_charge();malformed_closed_and_fragmented_frames_latch();callback_revoke_and_closed_capability_refuse_ingress();
    complete_configuration_and_descriptor_boundaries();real_allocation_failures_and_inherited_process();real_source_clock_reentry_and_concurrent_delivery();
    std::cout<<"passed 8 native sensor guest security groups\n";return 0;
}
