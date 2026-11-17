/** @file @brief Recoverable allocations, concurrent read delivery and receipt tombstones. */
#define main proposal_configuration_fixture_main
#include "proposal_configuration.cpp"
#undef main
static void actual_allocation_failures()
{
#if defined(UWVM_MODEL_DISPATCH_FAULT_INJECTION)
    unsigned capture_failures{},publish_failures{};
    for(std::size_t index=1;index<=16;++index)
    {
        proposal_fixture value;s::proposal_dispatch::captured_inputs result;
        fail_after=index;failed_allocation=false;auto status=value.capture(result);fail_after=0;
        if(!failed_allocation){assert(status==error::success);break;}
        ++capture_failures;assert(status==error::no_memory&&!result);
        s::proposal_dispatch::usage usage;assert(value.dispatch.inspect("suggestions",usage)==error::success&&!usage.captures&&!usage.workspace_bytes&&!usage.workspace_calls);
        assert(value.capture(result)==error::success);result.reset();assert(value.dispatch.inspect("suggestions",usage)==error::success&&!usage.captures&&!usage.workspace_bytes&&!usage.workspace_calls);
    }
    for(std::size_t index=1;index<=16;++index)
    {
        proposal_fixture value;s::proposal_dispatch::captured_inputs captured;assert(value.capture(captured)==error::success);auto body=value.envelope();auto request=value.finish(body);s::handle id{};
        fail_after=index;failed_allocation=false;auto status=value.dispatch.publish(captured,request,body,1400,id);fail_after=0;
        if(!failed_allocation){assert(status==error::success&&id);break;}
        ++publish_failures;assert(status==error::no_memory&&!id);s::proposal_dispatch::usage usage;assert(value.dispatch.inspect("suggestions",usage)==error::success&&!usage.proposals&&usage.captures==1&&usage.workspace_calls==1);
        assert(value.dispatch.publish(captured,request,body,1400,id)==error::success&&id);
    }
    assert(capture_failures>=2&&publish_failures>=1);std::printf("actual fast_io failure points capture=%u publish=%u\n",capture_failures,publish_failures);
#endif
}
static void cancelled_failed_and_reentrant_delivery()
{
    proposal_fixture value;auto id=value.published();auto accept=value.acceptance(id);std::vector<std::uint8_t> output;
    bool cancelled=true;auto check=[&]{return cancelled;};
    assert(value.call(pc::operation::accept,accept,output,8192,{},0,check)==error::cancelled&&!value.validations);
    cancelled=false;auto flip=[&](auto){cancelled=true;return true;};
    assert(value.call(pc::operation::accept,accept,output,8192,flip,0,check)==error::io&&value.validations==1);
    cancelled=false;assert(value.call(pc::operation::accept,accept,output)==error::success&&value.validations==1);
    proposal_fixture throwing;id=throwing.published();throwing.target_callback=[]{throw std::runtime_error("bounded fixture refusal");};
    assert(throwing.call(pc::operation::accept,throwing.acceptance(id),output)==error::denied&&!throwing.executions);throwing.target_callback={};
    assert(throwing.call(pc::operation::accept,throwing.acceptance(id),output)==error::success);
    proposal_fixture closing;id=closing.published();auto close=[&](auto){return closing.dispatch.close(closing.who,closing.read_cap,id)==error::again;};
    assert(closing.call(pc::operation::read,words({id}),output,8192,close)==error::success);
    assert(closing.dispatch.close(closing.who,closing.read_cap,id)==error::success);
    assert(closing.call(pc::operation::read,words({id}),output)==error::bad_handle);
}
static void receipt_describe_release_and_scope()
{
    proposal_fixture value;auto id=value.published();auto accept=value.acceptance(id);std::vector<std::uint8_t> output;
    assert(value.call(pc::operation::accept,accept,output)==error::success);auto accepted=output;s::wire::reader wire{output};s::handle request{};assert(wire.u64(request));
    std::size_t required{};auto copy=[&](auto bytes){output.assign(bytes.begin(),bytes.end());return true;};
    assert(value.dispatch.request(value.who,value.accept_cap,request,false,8192,required,copy)==error::success&&output==accepted);
    auto before=value.admissions("judge");assert(value.dispatch.request(value.who,value.accept_cap,request,true,0,required,copy)==error::too_large&&before==value.admissions("judge"));
    assert(value.dispatch.request(value.who,value.read_cap,request,false,8192,required,copy)==error::denied);
    assert(value.dispatch.request(value.who,value.accept_cap,request,true,8192,required,copy)==error::success&&output==words({1}));
    assert(value.dispatch.request(value.who,value.accept_cap,request,true,8192,required,copy)==error::success&&output==words({0}));
    assert(value.dispatch.request(value.who,value.accept_cap,request,false,8192,required,copy)==error::bad_handle);
    assert(value.call(pc::operation::accept,accept,output)==error::success&&output==accepted&&value.validations==1);
    assert(value.dispatch.request(value.who,value.accept_cap,request,false,8192,required,copy)==error::bad_handle);
    proposal_fixture revoked;id=revoked.published();assert(revoked.call(pc::operation::accept,revoked.acceptance(id),output)==error::success);
    s::wire::reader encoded{output};assert(encoded.u64(request));unsigned checks{};bool copied{};
    auto cancel=[&]{if(++checks==2)assert(revoked.permissions->revoke("judge",[](auto){})==error::success);return false;};
    auto disclose=[&](auto){copied=true;return true;};
    assert(revoked.dispatch.request(revoked.who,revoked.accept_cap,request,false,8192,required,disclose,cancel)==error::denied&&!copied);
}
static void actual_concurrency_and_retained_capture()
{
    proposal_fixture value;s::proposal_dispatch::captured_inputs retained;auto id=value.published(&retained);
    std::atomic<unsigned> good{};std::array<std::thread,4> threads;
    for(auto& thread:threads)thread=std::thread{[&]{for(unsigned round{};round<16;++round){std::vector<std::uint8_t> output;if(value.call(pc::operation::read,words({id}),output)==error::success)++good;}}};
    for(auto& thread:threads){thread.join();}
    assert(good==64);
    value.dispatch.retire();s::proposal_dispatch::usage usage;assert(value.dispatch.inspect("suggestions",usage)==error::success&&usage.retired&&usage.captures==1&&usage.workspace_calls==1);
    retained.reset();assert(value.dispatch.inspect("suggestions",usage)==error::success&&!usage.captures&&!usage.workspace_calls&&!usage.workspace_bytes);
}
static void read_cancellation_revalidates_actual_sources_before_disclosure()
{
    for(unsigned mode{};mode!=7;++mode)
    {
        proposal_fixture value;auto id=value.published();std::vector<std::uint8_t> output;unsigned checks{};bool disclosed{};
        auto cancel=[&]
        {
            if(++checks==2)
            {
                if(mode==0)assert(value.permissions->revoke("sensor",[](auto){})==error::success);
                if(mode==1)assert(value.permissions->revoke("data",[](auto){})==error::success);
                if(mode==2)++value.sample.observation.source_generation;
                if(mode==3)value.sample.owner=0; // Actual typed source retired; authority alone remains valid.
                if(mode==4)value.buffers.revoke("data");
                if(mode==5)++value.sample.observation.source_authority_epoch;
                if(mode==6)value.source_callback=[&]{assert(value.permissions->revoke("suggestions",[](auto){})==error::success);};
            }
            return false;
        };
        auto deliver=[&](auto){disclosed=true;return true;};
        assert(value.call(pc::operation::read,words({id}),output,8192,deliver,0,cancel)!=error::success&&!disclosed&&checks>=2);
    }
}
static void capture_explicit_buffer_authority_and_exact_provenance()
{
    for(unsigned mode{};mode!=5;++mode)
    {
        proposal_fixture value;s::proposal_dispatch::captured_inputs captured;
        s::proposal_dispatch::source_reference reference{0,value.source_cap,value.sample.observation.source,value.data_cap};
        auto who=value.who;
        if(mode==0)reference.buffer_capability=0;
        if(mode==1)reference.buffer_capability=value.source_cap;
        if(mode==2)++who.generation;
        if(mode==3)assert(value.permissions->revoke("data",[](auto){})==error::success);
        if(mode==4)assert(value.permissions->close(value.who,value.data_cap)==error::success);
        assert(value.dispatch.capture(who,"suggestions",{&reference,1},captured)!=error::success&&!captured);
    }
    proposal_fixture explicit_right;explicit_right.sample.observation.capability=0; // Native source has no caller-specific authority token.
    s::handle another_cap{};assert(explicit_right.permissions->open(explicit_right.who,"data",another_cap)==error::success);
    s::proposal_dispatch::source_reference reference{0,explicit_right.source_cap,explicit_right.sample.observation.source,another_cap};s::proposal_dispatch::captured_inputs captured;
    assert(explicit_right.dispatch.capture(explicit_right.who,"suggestions",{&reference,1},captured)==error::success&&captured.capability(0)==another_cap);
    proposal_fixture changed;s::proposal_dispatch::captured_inputs original;assert(changed.capture(original)==error::success);auto original_buffer=changed.input;
    s::buffers::layout format;format.schema=profile("tensor");format.rank=1;format.shape[0]=format.bytes=4;format.strides[0]=1;
    assert(changed.buffers.create("data",format,2,[&](auto id){changed.input=id;return true;})==error::success&&changed.input!=original_buffer);
    constexpr std::array<std::uint8_t,4> identical{1,2,3,4};std::uint64_t accepted{};
    assert(changed.buffers.write("data",changed.input,0,identical,accepted)==error::success&&changed.buffers.seal("data",changed.input,s::buffers::hash(identical))==error::success);
    auto final=changed.envelope();auto request=changed.finish(final);s::handle proposal{};
    assert(changed.dispatch.publish(original,request,final,1400,proposal)==error::conflict&&!proposal); // Same profile and digest, different actual allocation.
    for(unsigned mode{};mode!=3;++mode)
    {
        proposal_fixture retired;s::proposal_dispatch::captured_inputs token;unsigned validations{};
        retired.source_callback=[&]
        {
            if(++validations==2)
            {
                if(mode==0)retired.sample.owner=0;
                if(mode==1)++retired.sample.observation.source_authority_epoch;
                if(mode==2)retired.buffers.revoke("data");
            }
        };
        assert(retired.capture(token)!=error::success&&!token);
    }
}
int main()
{
    assert(proposal_configuration_fixture_main()==0);actual_allocation_failures();cancelled_failed_and_reentrant_delivery();receipt_describe_release_and_scope();actual_concurrency_and_retained_capture();
    read_cancellation_revalidates_actual_sources_before_disclosure();capture_explicit_buffer_authority_and_exact_provenance();std::puts("passed 6 proposal recovery security groups");return 0;
}
