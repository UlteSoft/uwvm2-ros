/** @file @brief Real guest capture/model-take/publication and recoverable authority tests. */
#define UWVM_PROPOSAL_LIFECYCLE_FIXTURE_ONLY
#include "proposal_lifecycle_target.cpp"
#include <uwvm2/imported/ros/scene/proposal_guest.h>

struct guest_fixture:actual_fixture
{
    s::proposal_guest_dispatch guest{permissions,model,buffers,71,81,{1,1},target_clock};
    explicit guest_fixture(bool enabled=true,bool zero_budget=false,std::uint64_t sessions=2,std::uint64_t captures=4,unsigned actions=7)
        :actual_fixture(enabled,zero_budget,actions)
    {
        p::guest_limits limits{sessions,captures,0};assert(guest.required_storage(limits,limits.retained_bytes));
        assert(guest.configure(limits)==error::success&&guest.bind("suggestions",policy)==error::success);
        p::arbiter_plan arbiter;assert(arbiter.proposal_alias.assign("suggestions")&&arbiter.target_alias.assign("node"));arbiter.tool_mask=1;
        assert(guest.bind_arbiter("judge",arbiter)==error::success&&guest.prepare(sensor.access(),target.access())==error::success);
    }
    std::vector<std::uint8_t> capture_wire(std::uint64_t key=1,std::uint64_t expiry=0,s::handle source_cap=0)
    {
        std::array<std::uint8_t,1024> data;s::wire::writer out{data};
        assert(out.u64(read_cap)&&out.u64(key)&&c::write_profile(out,policy.selected)&&out.u64(1)&&out.u64(0)&&
            out.u64(source_cap?source_cap:sensor_cap)&&out.u64(sensor_handle)&&out.u64(buffer_cap)&&out.u64(expiry?expiry:observed_now+UINT64_C(3000000000)));
        return {out.written().begin(),out.written().end()};
    }
    std::vector<std::uint8_t> publish_wire(s::handle capture,s::handle request,s::wire::bytes final,std::uint64_t key=1)
    {
        std::array<std::uint8_t,16384> data;s::wire::writer out{data};assert(out.u64(read_cap)&&out.u64(key)&&out.u64(capture)&&out.u64(request)&&
            out.blob(final)&&out.u64(observed_now+UINT64_C(2000000000)));return {out.written().begin(),out.written().end()};
    }
    error call(pc::operation action,s::wire::bytes input,std::vector<std::uint8_t>& output,std::size_t capacity=8192,
               s::proposal_guest_dispatch::sink custom={},s::proposal_guest_dispatch::cancellation cancelled={})
    {
        std::size_t required{};auto copy=[&](auto data){output.assign(data.begin(),data.end());return true;};
        return guest.invoke(who,action,action==pc::operation::accept?accept_cap:read_cap,input,capacity,required,
                            custom?custom:s::proposal_guest_dispatch::sink(copy),cancelled);
    }
    pc::capture_receipt capture()
    {
        std::vector<std::uint8_t> out;assert(call(pc::operation::capture,capture_wire(),out)==error::success);
        return capture_receipt(out);
    }
    static pc::capture_receipt capture_receipt(s::wire::bytes data)
    {
        pc::capture_receipt result;s::wire::reader reader{data};std::uint64_t count{};assert(reader.u64(result.capture)&&result.capture&&reader.u64(count)&&count<=8);
        result.count=count;for(std::size_t i{};i<count;++i)assert(reader.u64(result.inputs[i].capability)&&reader.u64(result.inputs[i].buffer));assert(reader.done());return result;
    }
    /** Return the actual complete envelope obtained through model.take. */
    s::handle complete(pc::capture_receipt const& capture,std::vector<std::uint8_t>& final,bool lose_take=false)
    {
        auto generated=envelope();std::array<std::uint8_t,1024> data;s::wire::writer out{data};constexpr std::array<std::uint8_t,1> params{1};
        assert(out.u64(session)&&out.u64(1)&&out.u64(session)&&c::write_profile(out,profile("tensor.increment"))&&out.text("increment")&&out.text("bytes")&&out.blob(params));
        assert(out.u64(capture.count));for(std::size_t i{};i<capture.count;++i)assert(out.u64(capture.inputs[i].capability)&&out.u64(capture.inputs[i].buffer)&&
            out.u64(0)&&out.u64(0)&&out.boolean(false)&&out.boolean(false));
        assert(out.u64(8192)&&out.u64(1)&&out.u64(8)&&out.u64(s::models::now()+UINT64_C(1000000000)));
        std::vector<std::uint8_t> response;assert(model_call(c::operation::submit,out.written(),response)==error::success);s::wire::reader receipt{response};s::handle request{};assert(receipt.u64(request));
        s::model_dispatch::work work;assert(model.claim("brain",8,work)==error::success&&work.identity()==request);
        assert(work.with_input(0,[](auto bytes,auto const&,auto const&){return bytes.size()==4&&bytes[0]==1;})==error::success);
        assert(work.finish(s::models::state::succeeded,generated)==error::success);
        auto query=words({session,request});std::size_t required{};
        if(lose_take)assert(model.invoke(who,c::operation::take,model_cap,query,16384,required,[](auto){return false;})==error::io);
        assert(model_call(c::operation::take,query,response)==error::success);
        s::wire::reader taken{response};std::uint64_t id{},provider{},source{},sequence{},related{},reserved{};std::string_view state,effect,reason,kind,type,encoding;
        bool present{},publisher{},stamp{},transport{};s::wire::bytes body;
        assert(taken.u64(id)&&id==request&&taken.text(state)&&state=="succeeded"&&taken.text(effect)&&effect=="committed"&&taken.u64(provider)&&taken.text(reason)&&taken.boolean(present)&&present);
        assert(taken.u64(source)&&source==session&&taken.u64(provider)&&taken.u64(sequence)&&sequence&&taken.boolean(publisher)&&!publisher&&
            taken.boolean(stamp)&&!stamp&&taken.boolean(transport)&&!transport&&taken.u64(reserved)&&!reserved&&taken.u64(related)&&related==request&&
            taken.text(kind)&&kind=="model.result"&&taken.text(type)&&type==p::model_output_type&&taken.text(encoding)&&encoding==p::model_output_encoding&&taken.blob(body)&&taken.done());
        final.assign(body.begin(),body.end());assert(final==generated);return request;
    }
    s::proposal_guest_dispatch::usage usage()
    {s::proposal_guest_dispatch::usage value;assert(guest.inspect_guests(value)==error::success);return value;}
    std::uint64_t admissions()
    {uwvm2::utils::container::std_vector<s::authority_snapshot> rows;assert(permissions->inspect(rows,32));for(auto const& row:rows)if(row.alias=="suggestions")return row.admissions;assert(false);return 0;}
};
static s::handle publication_handle(s::wire::bytes bytes)
{s::wire::reader reader{bytes};s::handle id{};assert(reader.u64(id)&&id&&reader.done());return id;}
static void guest_complete_chain_and_exact_recovery()
{
    auto owned=uwvm2::utils::container::make_unique<guest_fixture>();auto& f=*owned;auto before=f.observe();auto wire=f.capture_wire();std::vector<std::uint8_t> out;
    assert(f.call(pc::operation::capture,wire,out,8192,[](auto){return false;})==error::io&&out.empty());
    auto used=f.usage();assert(used.sessions==1&&used.captures==1&&!used.borrowed_captures);
    assert(f.call(pc::operation::capture,wire,out)==error::success);auto saved=out;auto captured=f.capture_receipt(out);
    assert(captured.count==1&&captured.inputs[0].capability==f.buffer_cap&&captured.inputs[0].buffer==f.input);
    assert(f.call(pc::operation::capture,wire,out)==error::success&&out==saved&&f.usage().captures==1);
    auto changed=wire;changed.back()^=1;assert(f.call(pc::operation::capture,changed,out)==error::conflict);
    std::vector<std::uint8_t> final;auto request=f.complete(captured,final,true);auto publish=f.publish_wire(captured.capture,request,final);
    auto admissions=f.admissions();assert(f.call(pc::operation::publish,publish,out,8192,[](auto){return false;})==error::io&&f.admissions()==admissions+1);
    assert(f.call(pc::operation::publish,publish,out)==error::success);saved=out;auto proposal=publication_handle(out);
    assert(f.call(pc::operation::publish,publish,out)==error::success&&out==saved);
    changed=publish;changed.back()^=1;assert(f.call(pc::operation::publish,changed,out)==error::conflict);
    assert(f.guest.close(f.who,f.read_cap,captured.capture)==error::success&&f.usage().captures==0);
    assert(f.call(pc::operation::capture,wire,out)==error::bad_handle); // No resurrection.
    assert(f.call(pc::operation::read,words({proposal}),out)==error::success);
    auto accept=words({f.accept_cap,1,f.read_cap,proposal,f.target_cap,f.client,observed_now+UINT64_C(1000000000)});
    assert(f.call(pc::operation::accept,accept,out)==error::success&&f.observe().current==before.current&&f.observe().next_key==1);
    assert(f.lifecycle.take("node",f.server,[](auto const&){return true;})==error::again);
    assert(f.permissions->revoke("sensor",[](auto){})==error::success);
    assert(f.call(pc::operation::publish,publish,out)==error::success&&out==saved); // Historical only.
}
static void bounded_preflight_and_failed_admission()
{
    for(int mode=0;mode<4;++mode)
    {
        auto owned=uwvm2::utils::container::make_unique<guest_fixture>(mode!=0,mode==1,mode==2?0u:2u,mode==3?0u:4u);auto& f=*owned;std::vector<std::uint8_t> out;auto wire=f.capture_wire();auto before=f.admissions();
        assert(f.call(pc::operation::capture,wire,out)!=error::success&&out.empty()&&f.admissions()==before&&f.usage().captures==0&&f.usage().sessions==0);
    }
    auto owned=uwvm2::utils::container::make_unique<guest_fixture>();auto& f=*owned;auto wire=f.capture_wire();std::vector<std::uint8_t> out;auto before=f.admissions();
    assert(f.call(pc::operation::capture,wire,out,31)==error::too_large&&f.admissions()==before&&f.usage().sessions==0);
    auto denied=f.capture_wire(1,0,f.target_cap);assert(f.call(pc::operation::capture,denied,out)==error::denied&&f.usage().captures==0&&f.usage().sessions==0&&f.admissions()==before);
    assert(f.call(pc::operation::capture,wire,out)==error::success); // Failed requests did not consume key 1.
    s::proposal_guest_dispatch::workspace_lease reserved;assert(f.guest.reserve_workspace(f.who,pc::operation::capture,f.read_cap,wire.size(),reserved)==error::success);
    s::proposal_guest_dispatch::workspace_lease refused;assert(f.guest.reserve_workspace(f.who,pc::operation::capture,f.target_cap,wire.size(),refused)==error::denied);
    p::guest_configuration config;p::guest_limits value;assert(config.freeze(value)==error::invalid);
    assert(config.limit("producer.sessions",1)==error::success&&config.limit("capture.handles",1)==error::success&&config.limit("retained.bytes",0)==error::success&&config.freeze(value)==error::success);
    assert(config.limit("capture.handles",1)==error::conflict&&config.freeze(value)==error::conflict);
}
static void malformed_and_unrelated_completions()
{
    auto owned=uwvm2::utils::container::make_unique<guest_fixture>();auto& f=*owned;auto wire=f.capture_wire();std::vector<std::uint8_t> out;
    for(std::size_t n{};n<wire.size();++n)assert(f.call(pc::operation::capture,{wire.data(),n},out)!=error::success);
    assert(f.call(static_cast<pc::operation>(255),wire,out)==error::unsupported);
    auto captured=f.capture();std::vector<std::uint8_t> final;auto request=f.complete(captured,final);
    auto publish=f.publish_wire(captured.capture,request,final);
    for(std::size_t n{};n<publish.size();++n)assert(f.call(pc::operation::publish,{publish.data(),n},out)!=error::success);
    auto changed=final;changed.back()^=1;assert(f.call(pc::operation::publish,f.publish_wire(captured.capture,request,changed),out)!=error::success);
    assert(f.call(pc::operation::publish,f.publish_wire(captured.capture,request+1,final),out)!=error::success);
    assert(f.call(pc::operation::publish,publish,out)==error::success);
}
static void cancellation_close_and_reentry()
{
    for(int mode=0;mode<4;++mode)
    {
        auto owned=uwvm2::utils::container::make_unique<guest_fixture>();auto& f=*owned;auto wire=f.capture_wire();std::vector<std::uint8_t> out;
        if(mode==0){assert(f.call(pc::operation::capture,wire,out,8192,{},[]{return true;})==error::cancelled&&f.usage().captures==0&&f.usage().sessions==0);continue;}
        error nested=error::success;s::handle id{};
        auto sink=[&](auto bytes)
        {
            id=f.capture_receipt(bytes).capture;
            if(mode==1)nested=f.call(pc::operation::capture,wire,out);
            if(mode==2)assert(f.guest.close(f.who,f.read_cap,id)==error::success);
            if(mode==3)f.guest.retire();
            return true;
        };
        auto status=f.call(pc::operation::capture,wire,out,8192,sink);
        if(mode==1)assert(status==error::success&&nested==error::again&&f.usage().captures==1);
        else assert(status!=error::success&&f.usage().captures==0);
    }
    auto owned=uwvm2::utils::container::make_unique<guest_fixture>();auto& f=*owned;auto captured=f.capture();std::vector<std::uint8_t> final,out;auto request=f.complete(captured,final);auto wire=f.publish_wire(captured.capture,request,final);
    assert(f.call(pc::operation::publish,wire,out,8192,{},[&]{assert(f.guest.close(f.who,f.read_cap,captured.capture)==error::success);return true;})==error::cancelled);
    assert(f.usage().captures==0);s::proposal_dispatch::usage usage;assert(f.guest.inspect("suggestions",usage)==error::success&&usage.proposals==0);
}
static void expiry_revoke_and_capability_lifetime()
{
    for(int mode=0;mode<5;++mode)
    {
        auto owned=uwvm2::utils::container::make_unique<guest_fixture>();auto& f=*owned;auto capture=f.capture();std::vector<std::uint8_t> final,out;auto request=f.complete(capture,final);auto wire=f.publish_wire(capture.capture,request,final);
        if(mode==0)observed_now+=UINT64_C(3000000001);
        if(mode==1)assert(f.permissions->revoke("suggestions",[](auto){})==error::success);
        if(mode==2)f.sensor.retire();
        if(mode==3)f.buffers.revoke("data");
        if(mode==4)f.target.retire();
        assert(f.call(pc::operation::publish,wire,out)!=error::success);
        s::proposal_dispatch::usage usage;assert(f.guest.inspect("suggestions",usage)==error::success&&usage.proposals==0);
    }
    auto owned=uwvm2::utils::container::make_unique<guest_fixture>();auto& f=*owned;auto capture=f.capture();assert(f.permissions->close(f.who,f.read_cap)==error::success);
    assert(f.usage().captures==1&&f.permissions->open(f.who,"suggestions",f.read_cap)==error::success);
    assert(f.guest.close(f.who,f.read_cap,capture.capture)==error::success&&f.usage().captures==0);
}
static void recoverable_allocation_and_fork()
{
#if defined(UWVM_MODEL_DISPATCH_FAULT_INJECTION)
    unsigned failures{};
    for(std::size_t attempt=1;attempt<=12;++attempt)
    {
        auto owned=uwvm2::utils::container::make_unique<guest_fixture>();auto& f=*owned;auto wire=f.capture_wire();std::vector<std::uint8_t> out;out.reserve(8192);
        fail_after=attempt;failed_allocation=false;auto status=f.call(pc::operation::capture,wire,out);auto failed=failed_allocation;fail_after=0;
        if(failed){++failures;assert(status==error::no_memory&&f.usage().captures==0&&f.usage().sessions==0);assert(f.call(pc::operation::capture,wire,out)==error::success);}
        else{assert(status==error::success);break;}
    }
    assert(failures>=2);
    failures=0;
    for(std::size_t attempt=1;attempt<=8;++attempt)
    {
        auto owned=uwvm2::utils::container::make_unique<guest_fixture>();auto& f=*owned;auto captured=f.capture();std::vector<std::uint8_t> final,out;auto request=f.complete(captured,final);
        auto wire=f.publish_wire(captured.capture,request,final);out.reserve(8192);
        fail_after=attempt;failed_allocation=false;auto status=f.call(pc::operation::publish,wire,out);auto failed=failed_allocation;fail_after=0;
        if(failed)
        {
            ++failures;assert(status==error::no_memory&&f.usage().captures==1&&f.usage().sessions==1);
            s::proposal_dispatch::usage usage;assert(f.guest.inspect("suggestions",usage)==error::success&&usage.proposals==0);
            assert(f.call(pc::operation::publish,wire,out)==error::success);
        }
        else{assert(status==error::success);break;}
    }
    assert(failures>=1);
#endif
    auto owned=uwvm2::utils::container::make_unique<guest_fixture>();auto& f=*owned;auto wire=f.capture_wire();auto child=::fork();assert(child>=0);
    if(!child){::alarm(3);std::vector<std::uint8_t> out;::_exit(f.call(pc::operation::capture,wire,out)==error::denied?0:7);}
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
}
static void independent_permissions_and_concurrent_retire()
{
    for(unsigned permitted:{1u,2u,4u})
    {
        auto owned=uwvm2::utils::container::make_unique<guest_fixture>(true,false,2,4,permitted);auto& f=*owned;auto capture=f.capture_wire();std::vector<std::uint8_t> out;
        if(permitted==2)
        {
            auto receipt=f.capture();std::vector<std::uint8_t> final;auto request=f.complete(receipt,final);
            assert(f.call(pc::operation::publish,f.publish_wire(receipt.capture,request,final),out)==error::denied);
        }
        else assert(f.call(pc::operation::capture,capture,out)==error::denied);
        if(permitted!=1)assert(f.call(pc::operation::read,words({1}),out)==error::denied);
        if(permitted!=4)assert(f.guest.preflight(f.who,pc::operation::publish,f.read_cap,8)==error::denied);
        else assert(f.guest.preflight(f.who,pc::operation::publish,f.read_cap,8)==error::success);
    }
    for(unsigned interrupt:{1u,2u,3u})
    {
        auto owned=uwvm2::utils::container::make_unique<guest_fixture>();auto& f=*owned;auto capture=f.capture();std::vector<std::uint8_t> final,out;auto request=f.complete(capture,final);auto wire=f.publish_wire(capture.capture,request,final);
        unsigned calls{};auto cancel=[&]{if(++calls==interrupt)assert(f.permissions->revoke("suggestions",[](auto){})==error::success);return false;};
        assert(f.call(pc::operation::publish,wire,out,8192,{},cancel)!=error::success);
        s::proposal_dispatch::usage usage;assert(f.guest.inspect("suggestions",usage)==error::success&&usage.proposals==0);
    }
    auto owned=uwvm2::utils::container::make_unique<guest_fixture>();auto& f=*owned;auto wire=f.capture_wire();std::vector<std::uint8_t> out;std::barrier gate{2};
    std::thread retiring{[&]{gate.arrive_and_wait();auto usage=f.usage();assert(usage.captures==1&&usage.borrowed_captures==1);f.guest.retire();gate.arrive_and_wait();}};
    auto sink=[&](auto){gate.arrive_and_wait();gate.arrive_and_wait();return true;};
    assert(f.call(pc::operation::capture,wire,out,8192,sink)!=error::success);retiring.join();assert(f.usage().captures==0);
}
static void recoverable_configuration_and_malformed_native_profiles()
{
    auto owned=uwvm2::utils::container::make_unique<guest_fixture>();auto& f=*owned;
    s::sensors::source source{f.permissions,f.buffers,target_clock};s::sensors::source::configuration invalid;invalid.alias.size=SIZE_MAX;
    assert(source.configure({71,91,81},invalid,{101,1,102,1,0,0})==error::invalid);
    s::proposal_lifecycle_target::configuration malformed;malformed.alias.size=SIZE_MAX;assert(f.target.bind(malformed)==error::invalid);
    auto rule=f.policy.tools[0];rule.argument_type.size=SIZE_MAX;s::proposal_dispatch::target_evidence out;assert(f.target.resolve(rule,f.client,out)==error::denied);
    auto reference=f.policy.sources[0];reference.frame.size=SIZE_MAX;s::proposal_dispatch::source_sample sample;
    assert(f.sensor.capture(f.who,f.sensor_cap,f.sensor_handle,reference,sample)==error::denied);
#if defined(UWVM_MODEL_DISPATCH_FAULT_INJECTION)
    for(std::size_t attempt=1;attempt<=2;++attempt)
    {
        auto guest=uwvm2::utils::container::make_unique<s::proposal_guest_dispatch>(f.permissions,f.model,f.buffers,71,81,s::proposal_dispatch::limits{1,1},target_clock);
        p::guest_limits limits{2,4,0};assert(guest->required_storage(limits,limits.retained_bytes));assert(guest->configure(limits)==error::success);
        uwvm2::utils::container::std_string alias(80,'a');fail_after=1;failed_allocation=false;
        assert(guest->bind(std::move(alias),f.policy)==error::no_memory&&failed_allocation);fail_after=0;
        assert(guest->bind("suggestions",f.policy)==error::success);
        fail_after=attempt;failed_allocation=false;auto status=guest->prepare(f.sensor.access(),f.target.access());fail_after=0;
        assert(failed_allocation&&status==error::no_memory&&guest->prepare(f.sensor.access(),f.target.access())==error::success);
    }
#endif
}
int main()
{
    guest_complete_chain_and_exact_recovery();bounded_preflight_and_failed_admission();malformed_and_unrelated_completions();
    cancellation_close_and_reentry();expiry_revoke_and_capability_lifetime();recoverable_allocation_and_fork();independent_permissions_and_concurrent_retire();
    recoverable_configuration_and_malformed_native_profiles();std::cout<<"passed 8 guest capture publication security groups\n";return 0;
}
