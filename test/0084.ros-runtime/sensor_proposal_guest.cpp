/** @file @brief A native process -> guest sensor -> guest capture/model-take/publish -> inert target chain. */
#define UWVM_PROPOSAL_LIFECYCLE_FIXTURE_ONLY
#include "proposal_lifecycle_target.cpp"
#include <uwvm2/imported/ros/scene/proposal_guest.h>
#include <uwvm2/imported/ros/scene/sensor_dispatch.h>
#include <poll.h>
namespace sc=uwvm2::imported::ros::sensor_contract;
struct complete_fixture:actual_fixture
{
    s::sensor_dispatch::configuration configured;
    uwvm2::utils::container::std_unique_ptr<s::sensor_dispatch> ingress;
    s::proposal_guest_dispatch guest{permissions,model,buffers,71,81,{1,1},target_clock};
    int channel[2]{-1,-1};s::handle live_sensor{},captured{},proposal{};std::vector<std::uint8_t> final;
    complete_fixture():actual_fixture(true,false,7,true)
    {
        assert(sensor.ingress_description(configured.source)==error::success);configured.actual={101,1,102,1,0,0};
        configured.transport={4,13,0};assert(s::sensors::ingress::required_storage(configured.transport,configured.transport.retained_bytes));
        configured.workspace_bytes=4096;configured.workspace_calls=8;std::uint64_t required{};assert(s::sensor_dispatch::required_storage(configured,required));
        ingress=uwvm2::utils::container::make_unique<s::sensor_dispatch>(permissions,buffers,71,81,s::sensor_dispatch::limits{1,required},target_clock);
        assert(::pipe2(channel,O_CLOEXEC|O_NONBLOCK)==0&&ingress->bind(configured,channel[0])==error::success&&ingress->prepare()==error::success);
        p::guest_limits limits{2,4,0};assert(guest.required_storage(limits,limits.retained_bytes)&&guest.configure(limits)==error::success&&guest.bind("suggestions",policy)==error::success);
        p::arbiter_plan arbiter;assert(arbiter.proposal_alias.assign("suggestions")&&arbiter.target_alias.assign("node"));arbiter.tool_mask=1;
        assert(guest.bind_arbiter("judge",arbiter)==error::success&&guest.prepare(ingress->access(),target.access())==error::success);
        std::array<std::uint8_t,144> bytes;s::wire::writer writer{bytes};assert(p::write_profile(writer,configured.source.ingress.profile));
        std::vector<std::uint8_t> output;assert(sensor_call(sc::operation::open,writer.written(),output)==error::success);s::wire::reader reader{output};assert(reader.u64(live_sensor)&&live_sensor&&reader.done());
        assert(live_sensor!=sensor_handle); // This resource came only from the guest carrier.
    }
    ~complete_fixture(){for(auto fd:channel)if(fd>=0)::close(fd);}
    error sensor_call(sc::operation action,s::wire::bytes input,std::vector<std::uint8_t>& output,s::sensor_dispatch::sink custom={})
    {std::size_t required{};auto sink=[&](auto bytes){output.assign(bytes.begin(),bytes.end());return true;};return ingress->invoke(who,action,sensor_cap,input,4096,required,custom?custom:s::sensor_dispatch::sink(sink));}
    error proposal_call(pc::operation action,s::wire::bytes input,std::vector<std::uint8_t>& output,s::proposal_guest_dispatch::sink custom={})
    {std::size_t required{};auto sink=[&](auto bytes){output.assign(bytes.begin(),bytes.end());return true;};return guest.invoke(who,action,action==pc::operation::accept?accept_cap:read_cap,input,8192,required,custom?custom:s::proposal_guest_dispatch::sink(sink));}
    void native_frame(std::uint64_t sequence=1)
    {
        constexpr std::array<std::uint8_t,4> actual{9,8,7,6};std::array<std::uint8_t,156> packet;std::size_t size{};
        assert(s::sensors::ingress_protocol::encode({101,1,sequence,0,0,0,false},configured.source.ingress.schema,actual,packet,size)==error::success);
        auto child=::fork();assert(child>=0);if(!child){::alarm(3);::close(channel[0]);auto written=::write(channel[1],packet.data(),size);::_exit(written==static_cast<ssize_t>(size)?0:1);}
        int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
    }
    s::handle take(bool lose=false)
    {
        std::vector<std::uint8_t> out;auto query=words({live_sensor});bool lost{};
        for(unsigned i{};;++i)
        {
            assert(i<1000);auto sink=[&](auto bytes){if(lose&&!lost){lost=true;return false;}out.assign(bytes.begin(),bytes.end());return true;};
            auto status=sensor_call(sc::operation::take,query,out,sink);if(status==error::io&&lost)continue;if(status==error::success)break;assert(status==error::again);
        }
        s::wire::reader in{out};std::uint64_t ignored{},buffer{};bool present{};std::int64_t timestamp{};std::string_view text;s::wire::bytes body;
        assert(in.u64(ignored)&&ignored==live_sensor&&in.u64(ignored)&&in.u64(ignored)&&in.boolean(present)&&!present);
        assert(in.boolean(present)&&present&&in.u64(ignored)&&in.u64(ignored)&&in.i64(timestamp)&&in.boolean(present)&&!present&&in.u64(ignored)&&in.u64(ignored)&&
            in.text(text)&&text=="sensor.frame"&&in.text(text)&&in.text(text)&&in.blob(body)&&body.size()==4&&body[0]==9&&in.u64(buffer)&&buffer);
        assert(buffer!=input);return buffer;
    }
    pc::capture_receipt capture(s::handle actual_buffer)
    {
        std::array<std::uint8_t,1024> bytes;s::wire::writer out{bytes};assert(out.u64(read_cap)&&out.u64(1)&&p::write_profile(out,policy.selected)&&
            out.u64(1)&&out.u64(0)&&out.u64(sensor_cap)&&out.u64(live_sensor)&&out.u64(buffer_cap)&&out.u64(observed_now+UINT64_C(3000000000)));
        std::vector<std::uint8_t> output;assert(proposal_call(pc::operation::capture,out.written(),output)==error::success);
        pc::capture_receipt result;s::wire::reader reader{output};std::uint64_t count{};assert(reader.u64(result.capture)&&reader.u64(count)&&count==1&&
            reader.u64(result.inputs[0].capability)&&result.inputs[0].capability==buffer_cap&&reader.u64(result.inputs[0].buffer)&&result.inputs[0].buffer==actual_buffer&&reader.done());
        result.count=1;captured=result.capture;return result;
    }
    s::handle infer(pc::capture_receipt const& source)
    {
        std::array<std::uint8_t,1024> bytes;s::wire::writer out{bytes};constexpr std::array<std::uint8_t,1> params{1};
        assert(out.u64(session)&&out.u64(1)&&out.u64(session)&&c::write_profile(out,profile("tensor.increment"))&&out.text("increment")&&out.text("bytes")&&out.blob(params)&&
            out.u64(1)&&out.u64(source.inputs[0].capability)&&out.u64(source.inputs[0].buffer)&&out.u64(0)&&out.u64(0)&&out.boolean(false)&&out.boolean(false)&&
            out.u64(8192)&&out.u64(1)&&out.u64(8)&&out.u64(s::models::now()+UINT64_C(1000000000)));
        std::vector<std::uint8_t> result;assert(model_call(c::operation::submit,out.written(),result)==error::success);s::wire::reader admitted{result};s::handle request{};assert(admitted.u64(request));
        s::model_dispatch::work work;assert(model.claim("brain",8,work)==error::success&&work.identity()==request);
        assert(work.with_input(0,[](auto body,auto const&,auto const&){return body.size()==4&&body[0]==9&&body[3]==6;})==error::success);
        auto generated=envelope();assert(work.finish(s::models::state::succeeded,generated)==error::success&&model_call(c::operation::take,words({session,request}),result)==error::success);
        s::wire::reader taken{result};std::uint64_t id{};std::string_view text;bool present{};s::wire::bytes body;
        assert(taken.u64(id)&&id==request&&taken.text(text)&&text=="succeeded"&&taken.text(text)&&taken.u64(id)&&taken.text(text)&&taken.boolean(present)&&present&&
            taken.u64(id)&&taken.u64(id)&&taken.u64(id)&&taken.boolean(present)&&!present&&taken.boolean(present)&&!present&&taken.boolean(present)&&!present&&
            taken.u64(id)&&taken.u64(id)&&taken.text(text)&&text=="model.result"&&taken.text(text)&&taken.text(text)&&taken.blob(body)&&taken.done());
        final.assign(body.begin(),body.end());assert(final==generated);return request;
    }
    std::vector<std::uint8_t> publication(s::handle request)
    {std::array<std::uint8_t,16384> bytes;s::wire::writer out{bytes};assert(out.u64(read_cap)&&out.u64(1)&&out.u64(captured)&&out.u64(request)&&out.blob(final)&&out.u64(observed_now+UINT64_C(2000000000)));return {out.written().begin(),out.written().end()};}
};
static void complete_actual_guest_chain(bool lost_sensor)
{
    auto own=uwvm2::utils::container::make_unique<complete_fixture>();auto& f=*own;auto before=f.observe();f.native_frame();auto source=f.capture(f.take(lost_sensor));
    auto request=f.infer(source);auto publication=f.publication(request);std::vector<std::uint8_t> output;
    assert(f.proposal_call(pc::operation::publish,publication,output,[](auto){return false;})==error::io&&f.proposal_call(pc::operation::publish,publication,output)==error::success);
    s::wire::reader reader{output};s::handle proposal{};assert(reader.u64(proposal)&&proposal&&reader.done());
    assert(f.guest.close(f.who,f.read_cap,f.captured)==error::success&&f.proposal_call(pc::operation::read,words({proposal}),output)==error::success);
    auto accept=words({f.accept_cap,1,f.read_cap,proposal,f.target_cap,f.client,observed_now+UINT64_C(1000000000)});
    assert(f.proposal_call(pc::operation::accept,accept,output)==error::success&&f.observe().current==before.current&&f.observe().next_key==1);
    assert(f.lifecycle.take("node",f.server,[](auto const&){return true;})==error::again);
}
static void actual_newer_native_frame_rejects_old_model_result()
{
    auto own=uwvm2::utils::container::make_unique<complete_fixture>();auto& f=*own;f.native_frame();auto captured=f.capture(f.take());auto request=f.infer(captured);
    f.native_frame(2);assert(f.take()!=captured.inputs[0].buffer);std::vector<std::uint8_t> output;
    assert(f.proposal_call(pc::operation::publish,f.publication(request),output)!=error::success&&output.empty());
    s::proposal_dispatch::usage usage;assert(f.guest.inspect("suggestions",usage)==error::success&&!usage.proposals&&f.observe().current.state==1);
}
int main()
{complete_actual_guest_chain(false);complete_actual_guest_chain(true);actual_newer_native_frame_rejects_old_model_result();std::cout<<"passed 3 native sensor guest model proposal chains\n";return 0;}
