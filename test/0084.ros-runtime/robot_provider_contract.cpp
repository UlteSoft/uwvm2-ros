/** @file
 * @brief Independent canonical fixtures and adversarial bounds for all robot provider requests.
 * @details No provider, ROS peer, device or guest import is fabricated by these
 * tests. They qualify the request boundary and its agreement with the separately
 * generated catalogue shapes. All fixtures use fixed storage; the real allocator
 * wrappers additionally prove that decoding performs no heap allocation.
 */
#include <uwvm2/imported/ros/scene/robot_provider_contract.h>
#include <uwvm2/imported/ros/scene/robot_provider_response.h>
#include <uwvm2/imported/ros/scene/operation_design.h>
#include <uwvm2/imported/ros/scene/schema_contract.h>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <pthread.h>
namespace r=uwvm2::imported::ros::scene::robot_contract;
namespace w=uwvm2::imported::ros::scene::wire;
namespace s=uwvm2::imported::ros::scene::schema_contract;
static constexpr r::limits capacity{16384,4096,255,256};
static constexpr s::limits structure{16384,8192,256,16};
static thread_local bool deny_allocations{},allocation_attempted{};
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){if(deny_allocations){allocation_attempted=true;return nullptr;}return __real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t size){if(deny_allocations){allocation_attempted=true;return nullptr;}return __real_calloc(n,size);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){if(deny_allocations){allocation_attempted=true;return nullptr;}return __real_realloc(p,n);}
struct record
{
    std::array<std::uint8_t,16384> data{};std::size_t size{};
    w::bytes view()const noexcept{return {data.data(),size};}
};
static void number(w::writer& out,std::uint64_t value){assert(out.u64(value));}
static void text(w::writer& out,std::string_view value){assert(out.text(value));}
static void profile(w::writer& out,std::string_view name="approved")
{text(out,name);std::array<std::uint8_t,32> sha{};sha.fill(5);assert(out.blob(sha));}
static void ref(w::writer& out,std::uint64_t capability=10,std::uint64_t resource=11)
{number(out,capability);number(out,resource);}
static void key(w::writer& out,std::uint64_t session=7){number(out,session);number(out,1);}
static void message(w::writer& out)
{text(out,"robot.command");text(out,"le64");std::array<std::uint8_t,3> body{1,2,3};assert(out.blob(body));}
static void observation(w::writer& out,std::uint64_t capability=10)
{ref(out,capability);number(out,12);number(out,13);number(out,1);number(out,14);number(out,15);assert(out.i64(-3));number(out,1);text(out,"base");}
static void dependency(w::writer& out,std::uint64_t capability=10)
{ref(out,capability);number(out,17);profile(out);}
static void frame(w::writer& out,std::string_view kind="image",std::uint64_t offset=0,std::uint64_t row_bytes=9,
                  std::uint64_t stride=12,std::uint64_t rows=2,std::uint64_t planes=1)
{
    profile(out);text(out,kind);number(out,1);
    number(out,kind=="image"?3:0);number(out,kind=="image"?2:0);number(out,3);number(out,kind=="audio"?2:0);
    number(out,planes);
    for(std::uint64_t n{};n<planes;++n){number(out,offset);number(out,row_bytes);number(out,stride);number(out,rows);}
}
static record fixture(r::operation action)
{
    record value;w::writer out{value.data};using enum r::operation;
    switch(action)
    {
    case device_open:profile(out);break;
    case device_submit:
        key(out);number(out,7);ref(out);number(out,2);profile(out,"mode");profile(out,"command");message(out);
        number(out,1);observation(out);number(out,1);dependency(out);number(out,1000);break;
    case device_result:case control_result:number(out,7);number(out,19);break;
    case device_stop:case control_stop:key(out);number(out,7);profile(out,"stop");break;
    case device_describe:case control_sample:case media_take:case bridge_describe:case component_describe:number(out,7);break;
    case control_open:profile(out);ref(out);number(out,1);number(out,1);ref(out,12,13);number(out,14);break;
    case control_submit:key(out);number(out,7);number(out,19);number(out,2);profile(out,"command");message(out);break;
    case media_open:profile(out);ref(out);number(out,20);number(out,1);ref(out,21,22);break;
    case media_submit:key(out);number(out,7);ref(out);frame(out);number(out,1000);break;
    case media_close:case bridge_start:case component_start:key(out);number(out,7);number(out,1000);break;
    case bridge_create:profile(out);number(out,1);number(out,1);ref(out);ref(out,12,13);break;
    case bridge_stop:case component_stop:key(out);number(out,7);profile(out,"stop");number(out,1000);break;
    case component_open:
        profile(out,"manifest");number(out,10);text(out,"configuration");number(out,3);
        {std::array<std::uint8_t,32> sha{};assert(out.blob(sha));}
        number(out,1);number(out,1);ref(out);break;
    }
    value.size=out.size();return value;
}
static bool decode(r::operation action,w::bytes input,r::request& output,r::limits bounds=capacity)
{
    allocation_attempted=false;deny_allocations=true;auto valid=r::decode(action,input,bounds,output);deny_allocations=false;
    assert(!allocation_attempted);return valid;
}
static void unchanged_failure(r::operation action,w::bytes data,r::limits bounds=capacity)
{
    r::request output;output.resource=9173;output.deadline=9241;output.selected.identity="canary";
    assert(!decode(action,data,output,bounds));
    assert(output.resource==9173&&output.deadline==9241&&output.selected.identity=="canary");
}
static void shapes_truncation_and_no_allocation()
{
    namespace design=uwvm2::imported::ros::scene::operation_design;
    static_assert(design::operations.size()==162);
    for(auto const& descriptor:r::operations)
    {
        auto documented=std::find_if(design::operations.begin(),design::operations.end(),
            [&](auto const& row){return row.name==descriptor.name;});
        assert(documented!=design::operations.end()&&documented->registered);
        auto input=fixture(descriptor.id);r::request decoded;
        assert(decode(descriptor.id,input.view(),decoded)&&decoded.action==descriptor.id);
        auto shape=s::find(descriptor.name);assert(shape&&s::validate(input.view(),shape->request,structure));
        for(std::size_t n{};n<input.size;++n)unchanged_failure(descriptor.id,input.view().first(n));
        input.data[input.size++]=0;unchanged_failure(descriptor.id,input.view());--input.size;
        unchanged_failure(descriptor.id,input.view(),{});
        auto short_capacity=capacity;short_capacity.request_bytes=input.size-1;unchanged_failure(descriptor.id,input.view(),short_capacity);
    }
    r::request value;assert(!r::decode(static_cast<r::operation>(255),{},capacity,value));
}
static void identities_profiles_and_replay_syntax()
{
    auto sample=fixture(r::operation::device_describe);std::fill_n(sample.data.begin(),8,0);
    unchanged_failure(r::operation::device_describe,sample.view());
    sample=fixture(r::operation::bridge_start);sample.data[0]=8;unchanged_failure(r::operation::bridge_start,sample.view());
    sample=fixture(r::operation::bridge_start);std::fill_n(sample.data.begin()+8,8,0);unchanged_failure(r::operation::bridge_start,sample.view());
    sample=fixture(r::operation::bridge_start);std::fill_n(sample.data.begin()+24,8,255);unchanged_failure(r::operation::bridge_start,sample.view());
    sample={};w::writer out{sample.data};text(out,std::string_view{"bad\0name",8});std::array<std::uint8_t,32> digest{};
    assert(out.blob(digest));sample.size=out.size();unchanged_failure(r::operation::device_open,sample.view());
    out=w::writer{sample.data};text(out,"approved");assert(out.blob({digest.data(),31}));sample.size=out.size();unchanged_failure(r::operation::device_open,sample.view());
    out=w::writer{sample.data};number(out,2);std::array<std::uint8_t,2> bad{0xc0,0x80};assert(out.encoded(bad)&&out.blob(digest));
    sample.size=out.size();unchanged_failure(r::operation::device_open,sample.view());
    sample=fixture(r::operation::device_open);auto small=capacity;small.text_bytes=3;unchanged_failure(r::operation::device_open,sample.view(),small);
    sample=fixture(r::operation::control_submit);small=capacity;small.blob_bytes=2;unchanged_failure(r::operation::control_submit,sample.view(),small);
}
static void global_budgets_and_dynamic_bindings()
{
    auto input=fixture(r::operation::device_submit);auto limited=capacity;limited.elements=1;
    unchanged_failure(r::operation::device_submit,input.view(),limited);limited.elements=2;r::request result;
    assert(decode(r::operation::device_submit,input.view(),result,limited));
    // More than 64 independently declared bindings are valid when explicitly
    // reserved. This is not a fixed-size global source registry.
    input={};w::writer out{input.data};profile(out);number(out,128);
    for(unsigned n=1;n<=128;++n){number(out,n);ref(out,10+n,100+n);ref(out,210+n,300+n);}
    input.size=out.size();limited=capacity;limited.elements=128;
    assert(decode(r::operation::bridge_create,input.view(),result,limited)&&result.bindings.count==128);
    limited.elements=127;unchanged_failure(r::operation::bridge_create,input.view(),limited);
    for(auto slot:{std::uint64_t{0},std::uint64_t{1}})
    {
        out=w::writer{input.data};profile(out);number(out,2);number(out,1);ref(out);ref(out);number(out,slot);ref(out);ref(out);
        input.size=out.size();unchanged_failure(r::operation::bridge_create,input.view());
    }
    out=w::writer{input.data};profile(out);number(out,UINT64_MAX);input.size=out.size();unchanged_failure(r::operation::bridge_create,input.view());
    out=w::writer{input.data};profile(out);number(out,0);input.size=out.size();unchanged_failure(r::operation::bridge_create,input.view());
}
static record media(std::uint64_t offset,std::uint64_t row_bytes,std::uint64_t stride,std::uint64_t rows,std::uint64_t planes=1,std::string_view kind="image")
{record value;w::writer out{value.data};key(out);number(out,7);ref(out);frame(out,kind,offset,row_bytes,stride,rows,planes);number(out,1000);value.size=out.size();return value;}
static void media_expansion_and_overlap()
{
    auto input=media(0,9,12,2);r::request result;
    assert(decode(r::operation::media_submit,input.view(),result));assert(r::raw_extent(result.media,21)&&!r::raw_extent(result.media,20));
    for(auto bad:std::array{media(UINT64_MAX,9,12,2),media(0,0,12,2),media(0,9,8,2),media(0,9,12,0),
                            media(0,9,UINT64_MAX,2),media(0,9,12,2,2),media(0,9,12,2,0),media(0,9,12,2,1,"unknown")})
        unchanged_failure(r::operation::media_submit,bad.view());
    input=media(0,9,12,2,0,"encoded_packet");assert(decode(r::operation::media_submit,input.view(),result));
    assert(!r::raw_extent(result.media,100));
    input=media(0,9,12,2,1,"audio");assert(decode(r::operation::media_submit,input.view(),result)&&r::raw_extent(result.media,21));
    result.media.kind=static_cast<r::frame_kind>(255);assert(!r::raw_extent(result.media,21));
}
static void canonical_source_order()
{
    record input;
    for(auto duplicate:{true,false})
    {
        w::writer out{input.data};key(out);number(out,7);ref(out);number(out,2);profile(out);profile(out);message(out);
        number(out,2);observation(out,12);observation(out,duplicate?12:11);number(out,0);number(out,1000);input.size=out.size();
        unchanged_failure(r::operation::device_submit,input.view());
        out=w::writer{input.data};key(out);number(out,7);ref(out);number(out,2);profile(out);profile(out);message(out);
        number(out,0);number(out,2);dependency(out,12);dependency(out,duplicate?12:11);number(out,1000);input.size=out.size();
        unchanged_failure(r::operation::device_submit,input.view());
    }
}
static void hostile_bytes_and_concurrent_borrowing()
{
    std::array<std::uint8_t,512> data{};std::uint64_t state=0x1234abcddef;
    for(unsigned n{};n<24000;++n)
    {
        for(auto& byte:data){state^=state<<13;state^=state>>7;state^=state<<17;byte=static_cast<std::uint8_t>(state);}
        r::request value;(void)decode(r::operations[n%22].id,{data.data(),n%data.size()},value);
        r::response returned;(void)r::decode_response(r::operations[n%22].id,{data.data(),n%data.size()},capacity,returned);
    }
    auto input=fixture(r::operation::device_submit);
    std::array<pthread_t,4> threads;
    for(auto& thread:threads)assert(pthread_create(&thread,nullptr,[](void* context)->void*
    {auto const& input=*static_cast<record const*>(context);for(unsigned n{};n<2000;++n){r::request value;assert(decode(r::operation::device_submit,input.view(),value));}return nullptr;},&input)==0);
    for(auto& thread:threads)assert(pthread_join(thread,nullptr)==0);
}
static void timestamp(w::writer& out)
{number(out,3);number(out,4);assert(out.i64(-17));}
static void receipt(w::writer& out,std::string_view state="succeeded",std::string_view effect="observed_remote",std::uint64_t generation=13)
{number(out,19);text(out,state);text(out,effect);number(out,generation);text(out,"");}
static record reply(r::operation action)
{
    record value;w::writer out{value.data};using enum r::operation;
    switch(action)
    {
    case device_open:case media_open:case bridge_create:case component_open:number(out,7);break;
    case device_submit:case device_stop:case control_submit:case control_result:case control_stop:case media_submit:
    case media_close:case bridge_start:case bridge_stop:case component_start:case component_stop:receipt(out);break;
    case device_result:
        receipt(out);number(out,1);text(out,"stopped_observed");number(out,13);number(out,2);timestamp(out);number(out,1);observation(out);break;
    case device_describe:
        profile(out);profile(out);profile(out);text(out,"ready");number(out,13);
        profile(out,"watchdog");text(out,"independent_controller");number(out,11);number(out,1);number(out,1);number(out,1000);number(out,15);
        number(out,8192);number(out,3);number(out,4);number(out,5);number(out,1000);number(out,2000);number(out,1);break;
    case control_open:number(out,7);number(out,2);number(out,13);number(out,1000);break;
    case control_sample:
        number(out,7);number(out,19);number(out,2);timestamp(out);number(out,1000);number(out,1);observation(out);
        number(out,0);number(out,1);dependency(out);break;
    case media_take:
        receipt(out);number(out,12);number(out,13);number(out,19);number(out,1);timestamp(out);number(out,1);timestamp(out);
        number(out,1);{std::array<std::uint8_t,2> identity{3,7};assert(out.blob(identity));}number(out,4);
        frame(out);ref(out);number(out,2);break;
    case bridge_describe:
        profile(out);text(out,"running");number(out,13);number(out,2);number(out,1);number(out,3);number(out,1);number(out,1);receipt(out);break;
    case component_describe:
        profile(out);text(out,"exited");number(out,13);number(out,0);number(out,512);number(out,1);number(out,1);number(out,1);number(out,0);number(out,1);receipt(out);break;
    }
    value.size=out.size();return value;
}
static bool decode_reply(r::operation action,w::bytes input,r::response& output,r::limits bounds=capacity)
{
    allocation_attempted=false;deny_allocations=true;auto valid=r::decode_response(action,input,bounds,output);deny_allocations=false;
    assert(!allocation_attempted);return valid;
}
static void reply_failure(r::operation action,w::bytes bytes,r::limits bounds=capacity)
{
    r::response output;output.handle=71;output.execution.state="canary";
    assert(!decode_reply(action,bytes,output,bounds)&&output.handle==71&&output.execution.state=="canary");
}
static void result_shapes_and_evidence()
{
    for(auto const& descriptor:r::operations)
    {
        auto value=reply(descriptor.id);r::response result;
        assert(decode_reply(descriptor.id,value.view(),result)&&result.action==descriptor.id);
        auto shape=s::find(descriptor.name);assert(shape&&s::validate(value.view(),shape->result,structure));
        for(std::size_t n{};n<value.size;++n)reply_failure(descriptor.id,value.view().first(n));
        value.data[value.size++]=0;reply_failure(descriptor.id,value.view());--value.size;
        reply_failure(descriptor.id,value.view(),{});
    }
    // Exhaust the independent closed state/effect matrix, including known
    // partial effects after cancellation: cancellation does not undo motion.
    constexpr std::array states{"admitted","queued","running","succeeded","failed","cancelled","outcome_unknown","bogus"};
    constexpr std::array effects{"none","locally_accepted","committed","observed_remote","partial","unknown","bogus"};
    constexpr std::array<unsigned,8> allowed{3,2,2,12,31,31,48,0};
    for(std::size_t state{};state<states.size();++state)for(std::size_t effect{};effect<effects.size();++effect)
    {
        record value;w::writer out{value.data};receipt(out,states[state],effects[effect]);value.size=out.size();r::response result;
        assert(decode_reply(r::operation::device_submit,value.view(),result)==((allowed[state]&(1u<<effect))!=0));
    }
    for(auto generation:{0u,12u})
    {
        record value;w::writer out{value.data};receipt(out);number(out,1);text(out,"stopped_observed");number(out,generation);
        number(out,2);timestamp(out);number(out,1);observation(out);value.size=out.size();reply_failure(r::operation::device_result,value.view());
    }
    record value;w::writer out{value.data};receipt(out);number(out,1);text(out,"stopped_observed");number(out,13);
    number(out,2);timestamp(out);number(out,0);value.size=out.size();reply_failure(r::operation::device_result,value.view());
    value=reply(r::operation::control_sample);auto limited=capacity;limited.elements=1;reply_failure(r::operation::control_sample,value.view(),limited);
    value=reply(r::operation::media_take);limited=capacity;limited.blob_bytes=1;reply_failure(r::operation::media_take,value.view(),limited);
    // A described exited tree cannot still claim an observed live process.
    out=w::writer{value.data};profile(out);text(out,"exited");number(out,13);number(out,1);number(out,512);number(out,0);number(out,0);
    number(out,1);number(out,0);number(out,0);value.size=out.size();reply_failure(r::operation::component_describe,value.view());
    // First-failure evidence survives retirement and uncertainty, but cannot
    // accompany a fabricated healthy state or use an unknown numeric reason.
    for(auto state:{"failed","retired","outcome_unknown","ready","exited"})
        for(auto code:{0u,4u,9u,10u})
        {
            out=w::writer{value.data};profile(out);text(out,state);number(out,13);number(out,0);number(out,512);
            number(out,1);number(out,1);number(out,1);number(out,code);number(out,0);value.size=out.size();
            r::response parsed;
            bool allowed=code<=9&&(state!=std::string_view("failed")||code)&&
                (!code||state==std::string_view("failed")||state==std::string_view("retired")||state==std::string_view("outcome_unknown"));
            assert(decode_reply(r::operation::component_describe,value.view(),parsed)==allowed);
        }
    // Available, armed and ready are different claims: a worker cannot invent
    // a ready deployment with an unavailable/unarmed watchdog in its reply.
    for(auto flags:std::array{std::array<unsigned,2>{0,1},std::array<unsigned,2>{1,0}})
    {
        out=w::writer{value.data};profile(out);profile(out);profile(out);text(out,"ready");number(out,13);
        profile(out);text(out,"independent_controller");number(out,11);number(out,flags[0]);number(out,flags[1]);number(out,1000);number(out,15);
        number(out,8192);number(out,3);number(out,4);number(out,5);number(out,1000);number(out,2000);number(out,1);
        value.size=out.size();reply_failure(r::operation::device_describe,value.view());
    }
    // Empty sanitized reason is valid; oversized or embedded-NUL text is not.
    for(auto bad:std::array<std::string_view,2>{std::string_view{"bad\0reason",10},std::string_view{"bad\0",4}})
    {
        out=w::writer{value.data};number(out,19);text(out,"failed");text(out,"none");number(out,13);text(out,bad);
        value.size=out.size();reply_failure(r::operation::device_submit,value.view());
    }
}
int main()
{
    shapes_truncation_and_no_allocation();identities_profiles_and_replay_syntax();global_budgets_and_dynamic_bindings();
    media_expansion_and_overlap();canonical_source_order();hostile_bytes_and_concurrent_borrowing();
    result_shapes_and_evidence();
    std::puts("passed 44 robot provider request/result shapes and 7 safety groups");
}
