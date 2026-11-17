/** @file
 * @brief Safety and actual native-adapter call-count checks for an unregistered proposal.
 * @details Exercises exact frozen XML bytes, the existing broker's three-step
 * adapter, and the proposed single-publish core. It does not execute a new VM
 * import or claim a VM speedup. Faults are injected before publication and at
 * delivery; concurrency uses actual threads and finite scratch reservations.
 */
#include "prototype/complete_publication.h"
#include "publication_fixture_bytes.h"
#include <uwvm2/imported/ros/scene/broker.h>
#include <atomic>
#include <barrier>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <sys/wait.h>

namespace p=uwvm2::test::complete_publication;
namespace scene=p::scene;
using p::error;using p::receipt;
[[maybe_unused]] static std::atomic<int> allocation_failure{-1};
#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
#define PUBLICATION_THREAD_SANITIZED
#endif
#endif
#if !defined(PUBLICATION_THREAD_SANITIZED)
void* operator new(std::size_t size)
{
    if(allocation_failure.load()>=0&&allocation_failure.fetch_sub(1)==0)throw std::bad_alloc{};
    if(auto value=std::malloc(size?size:1))return value;
    throw std::bad_alloc{};
}
void* operator new[](std::size_t size){return ::operator new(size);}
[[gnu::noinline]] static void free_bytes(void* pointer) noexcept{std::free(pointer);}
void operator delete(void* pointer) noexcept{free_bytes(pointer);}
void operator delete[](void* pointer) noexcept{free_bytes(pointer);}
void operator delete(void* pointer,std::size_t) noexcept{free_bytes(pointer);}
void operator delete[](void* pointer,std::size_t) noexcept{free_bytes(pointer);}
#endif
static int owner_identity{},peer_identity{};
static constexpr scene::caller owner{&owner_identity,3};
static constexpr scene::handle capability=91;
static p::plan limits(){return {4096,2,4096,4096,2,8224,2};}
static p::policy allow(){p::policy result;result.exposed=result.granted=p::all_actions;result.window=std::chrono::hours{1};return result;}
static std::vector<std::uint8_t> words(std::initializer_list<std::uint64_t> values,p::bytes data={})
{
    std::vector<std::uint8_t> output(values.size()*8+data.size());std::size_t index{};
    for(auto value:values)scene::put(output,index++,value);
    std::copy(data.begin(),data.end(),output.begin()+static_cast<std::ptrdiff_t>(index*8));return output;
}
static std::vector<std::uint8_t> request(std::uint64_t key,std::string_view value="abc")
{return words({key,value.size()},{reinterpret_cast<std::uint8_t const*>(value.data()),value.size()});}
static error invoke(p::coordinator& endpoint,std::vector<std::uint8_t> const& bytes,receipt& value,std::size_t capacity=72)
{
    std::size_t required{};
    return endpoint.invoke(owner,capability,bytes.size(),capacity,required,
        [&](auto output){std::copy(bytes.begin(),bytes.end(),output.begin());return true;},
        [&](receipt const& output){value=output;return true;},std::stop_token{});
}
static void pristine(p::coordinator const& endpoint)
{auto s=endpoint.inspect();assert(!s.objects&&!s.retained_bytes&&!s.scratch_bytes&&!s.scratch_calls&&!s.last_key);}

static void authority_and_defaults()
{
    for(bool exposure:{false,true})
    {
        auto policy=allow();(exposure?policy.exposed:policy.granted)|=1u<<31;
        p::ordered_receipt stream{4120};bool rejected{};
        try{p::coordinator invalid{limits(),policy,owner,capability,stream};}
        catch(std::invalid_argument const&){rejected=true;}
        assert(rejected);
    }
    auto bytes=request(1);
    for(unsigned bit:{p::begin,p::append,p::commit,p::publish})for(bool exposure:{false,true})
    {
        auto policy=allow();(exposure?policy.exposed:policy.granted)&=~bit;
        p::ordered_receipt stream{4120};p::coordinator endpoint{limits(),policy,owner,capability,stream};
        std::size_t required{};bool copied{};
        assert(endpoint.invoke(owner,capability,bytes.size(),72,required,[&](auto){copied=true;return true;},[](auto const&){return true;},std::stop_token{})==error::denied);
        assert(!copied);pristine(endpoint);
        // Empty payload requires the identical four actions.
        receipt result;assert(invoke(endpoint,request(1,""),result)==error::denied);
    }
    p::ordered_receipt zero_stream{4120};p::coordinator zero{limits(),{},owner,capability,zero_stream};receipt result;
    assert(invoke(zero,bytes,result)==error::denied);pristine(zero);
    p::ordered_receipt stream{4120};auto policy=allow();policy.granted=policy.exposed=p::required_publish;
    p::coordinator endpoint{limits(),policy,owner,capability,stream};assert(invoke(endpoint,bytes,result)==error::success);
    std::array<std::uint8_t,8> copy{};std::size_t size{};
    assert(endpoint.read_owned(owner,capability,result.object,copy,size)==error::denied);
    assert(endpoint.remove_owned(owner,capability,words({2,result.object}),result)==error::denied);
    std::size_t required{};bool copied{};
    for(auto who:std::array{scene::caller{&peer_identity,3},scene::caller{&owner_identity,4}})
        assert(endpoint.invoke(who,capability,bytes.size(),72,required,[&](auto){copied=true;return true;},[](auto const&){return true;},std::stop_token{})==error::denied);
    assert(endpoint.invoke(owner,capability+1,bytes.size(),72,required,[&](auto){copied=true;return true;},[](auto const&){return true;},std::stop_token{})==error::denied);
    assert(!copied);
}
static void bounds_short_and_budget()
{
    for(unsigned which{};which<4;++which)
    {
        auto policy=allow();if(which==0)policy.logical_calls=0;if(which==1)policy.logical_calls=2;if(which==2)policy.bytes_per_window=282;if(which==3)policy.bytes_per_window=0;
        p::ordered_receipt stream{4120};p::coordinator endpoint{limits(),policy,owner,capability,stream};
        bool copied{};std::size_t required{};auto bytes=request(1);
        assert(endpoint.invoke(owner,capability,bytes.size(),72,required,[&](auto){copied=true;return true;},[](auto const&){return true;},std::stop_token{})==error::again);
        assert(!copied&&!endpoint.inspect().window_calls&&!endpoint.inspect().window_bytes);pristine(endpoint);
    }
    p::ordered_receipt stream{4120};auto policy=allow();policy.logical_calls=3;policy.bytes_per_window=283;
    p::coordinator endpoint{limits(),policy,owner,capability,stream};auto bytes=request(1);std::size_t required{};bool copied{};
    assert(endpoint.invoke(owner,capability,bytes.size(),71,required,[&](auto){copied=true;return true;},[](auto const&){return true;},std::stop_token{})==error::too_large&&required==72&&!copied);
    assert(endpoint.invoke(owner,capability,4113,72,required,[&](auto){copied=true;return true;},[](auto const&){return true;},std::stop_token{})==error::too_large&&!copied);
    receipt result;assert(invoke(endpoint,bytes,result)==error::success);auto count=endpoint.inspect();
    assert(count.window_calls==3&&count.window_bytes==283&&count.admissions==1&&count.logical_work==3);
    assert(invoke(endpoint,bytes,result)==error::again);count=endpoint.inspect();assert(count.window_calls==3&&count.window_bytes==283);
    auto empty_policy=allow();empty_policy.logical_calls=2;p::ordered_receipt empty_stream{4120};p::coordinator empty{limits(),empty_policy,owner,capability,empty_stream};
    assert(invoke(empty,request(1,""),result)==error::again);pristine(empty);
}
static void owned_and_partial_copy()
{
    for(std::size_t length:{0u,32u,512u,513u,4096u})
    {
        p::ordered_receipt stream{4120};p::coordinator endpoint{limits(),allow(),owner,capability,stream};
        auto bytes=request(1,std::string(length,'x'));std::size_t required{};receipt sentinel{444};
        assert(endpoint.invoke(owner,capability,bytes.size(),72,required,[&](auto output){std::copy_n(bytes.begin(),bytes.size()/2,output.begin());return false;},
            [&](auto const& value){sentinel=value;return true;},std::stop_token{})==error::invalid);
        assert(sentinel.key==444&&!required);pristine(endpoint);
    }
    p::ordered_receipt stream{4120};p::coordinator endpoint{limits(),allow(),owner,capability,stream};auto bytes=request(1);std::size_t required{};receipt published;
    assert(endpoint.invoke(owner,capability,bytes.size(),72,required,[&](auto output){std::copy(bytes.begin(),bytes.end(),output.begin());bytes.back()='z';return true;},
        [&](auto const& value){published=value;return true;},std::stop_token{})==error::success);
    std::array<std::uint8_t,3> data{};std::size_t size{};assert(endpoint.read_owned(owner,capability,published.object,data,size)==error::success&&size==3&&data[2]=='c');
    auto malformed=request(2);malformed[8]=4;receipt old{888};assert(invoke(endpoint,malformed,old)==error::invalid&&old.key==888&&endpoint.inspect().last_key==1);
}
static void retry_and_shared_stream()
{
    bool rejected{};
    try{p::ordered_receipt too_small{4112};p::coordinator invalid{limits(),allow(),owner,capability,too_small};}
    catch(std::invalid_argument const&){rejected=true;}
    assert(rejected); // Publish's smaller header cannot shrink old append replay.
    p::ordered_receipt maximum_stream{4120};std::vector<std::uint8_t> payload(4096,0x5a);
    auto maximum_append=words({1,19,0},payload);receipt append_receipt;append_receipt.key=1;append_receipt.operation=2;
    maximum_stream.remember(maximum_append,append_receipt);receipt recovered;bool replayed{};
    assert(maximum_stream.match(2,maximum_append,recovered,replayed)==error::success&&replayed&&recovered==append_receipt);
    maximum_append.back()^=1;assert(maximum_stream.match(2,maximum_append,recovered,replayed)==error::conflict);
    p::coordinator following{limits(),allow(),owner,capability,maximum_stream};
    assert(invoke(following,request(2),recovered)==error::success&&recovered.key==2);
    p::ordered_receipt stream{4120};auto legacy=words({1,3});receipt prior;prior.key=1;prior.operation=1;
    stream.remember(legacy,prior); // Trusted legacy owner, before concurrent calls.
    p::coordinator endpoint{limits(),allow(),owner,capability,stream};receipt first;
    assert(invoke(endpoint,request(1),first)==error::conflict);assert(invoke(endpoint,request(2),first)==error::success);
    receipt replay;assert(invoke(endpoint,request(2),replay)==error::success&&replay==first&&endpoint.inspect().objects==1);
    assert(invoke(endpoint,request(2,"abd"),replay)==error::conflict);
    assert(invoke(endpoint,request(4),replay)==error::conflict);
    assert(endpoint.remove_owned(owner,capability,words({3,first.object}),replay)==error::success);
    auto removed=replay;assert(endpoint.remove_owned(owner,capability,words({3,first.object}),replay)==error::success&&replay==removed);
    assert(invoke(endpoint,request(2),replay)==error::conflict);
    assert(invoke(endpoint,request(4,""),replay)==error::success&&replay.object&&replay.state==2&&replay.flags==1&&replay.total_bytes==0);
}
static void capacity_and_delivery()
{
    auto bounds=limits();bounds.object_slots=bounds.max_handles=1;bounds.payload_bytes=bounds.object_bytes=3;
    p::ordered_receipt stream{4120};p::coordinator endpoint{bounds,allow(),owner,capability,stream};auto bytes=request(1);std::size_t required{};receipt first;
    assert(endpoint.invoke(owner,capability,bytes.size(),72,required,[&](auto output){std::copy(bytes.begin(),bytes.end(),output.begin());return true;},
        [&](auto const& value){first=value;return false;},std::stop_token{})==error::invalid);
    assert(first.object&&endpoint.inspect().objects==1&&endpoint.inspect().last_key==1);
    receipt repeated;assert(invoke(endpoint,bytes,repeated)==error::success&&first==repeated);
    assert(invoke(endpoint,request(2,""),repeated)==error::success&&repeated.effect_status==static_cast<unsigned>(error::again)&&!repeated.object);
    auto refused=repeated;assert(invoke(endpoint,request(2,""),repeated)==error::success&&repeated==refused&&endpoint.inspect().objects==1);
    assert(endpoint.remove_owned(owner,capability,words({3,first.object}),repeated)==error::success);
    auto snapshot=endpoint.inspect();assert(!snapshot.objects&&!snapshot.retained_bytes);
    auto fourth=request(4);assert(endpoint.invoke(owner,capability,fourth.size(),72,required,[&](auto output){std::copy(fourth.begin(),fourth.end(),output.begin());return true;},
        [](receipt const&)->bool{throw std::runtime_error{"lost sink"};},std::stop_token{})==error::invalid);
    assert(invoke(endpoint,fourth,repeated)==error::success&&repeated.key==4&&repeated.object!=first.object);
}
static void allocation_and_callback_faults()
{
    p::ordered_receipt stream{4120};p::coordinator endpoint{limits(),allow(),owner,capability,stream};auto bytes=request(1);receipt result;
#if !defined(PUBLICATION_THREAD_SANITIZED)
    allocation_failure=0;auto status=invoke(endpoint,bytes,result);allocation_failure=-1;
    assert(status==error::no_memory);pristine(endpoint);assert(!endpoint.inspect().admissions);
#endif
    std::size_t required{};
    assert(endpoint.invoke(owner,capability,bytes.size(),72,required,[](auto)->bool{throw std::runtime_error{"copy"};},[](auto const&){return true;},std::stop_token{})==error::invalid);
    pristine(endpoint);
#if !defined(PUBLICATION_THREAD_SANITIZED)
    // Permit only the owned request allocation. Any allocation after copying
    // and admission would fail, so this also checks the commit's fixed storage.
    allocation_failure=1;auto committed=invoke(endpoint,bytes,result);allocation_failure=-1;
    assert(committed==error::success&&result.key==1);
    auto later=request(2,"def");auto first=result;
    allocation_failure=0;auto failed=invoke(endpoint,later,result);allocation_failure=-1;
    assert(failed==error::no_memory&&result==first);
    auto snapshot=endpoint.inspect();assert(snapshot.objects==1&&snapshot.retained_bytes==3&&snapshot.last_key==1&&!snapshot.scratch_calls&&!snapshot.scratch_bytes);
    assert(invoke(endpoint,bytes,result)==error::success&&result==first);
#else
    assert(invoke(endpoint,bytes,result)==error::success&&result.key==1);
#endif
}
static void cancellation_and_revocation()
{
    auto bytes=request(1);
    for(bool revoke:{false,true})
    {
        p::ordered_receipt stream{4120};p::coordinator endpoint{limits(),allow(),owner,capability,stream};std::size_t required{};std::stop_source stopping;
        assert(endpoint.invoke(owner,capability,bytes.size(),72,required,[&](auto output)
            {std::copy(bytes.begin(),bytes.end(),output.begin());if(revoke)endpoint.revoke();else stopping.request_stop();return true;},
            [](auto const&){return true;},stopping.get_token())==(revoke?error::denied:error::cancelled));
        pristine(endpoint);assert(endpoint.inspect().admissions==(revoke?0u:1u));
    }
    p::ordered_receipt waiting_stream{4120};p::coordinator waiting{limits(),allow(),owner,capability,waiting_stream};
    std::stop_source stopping;std::barrier copied{2},release_reader{2};std::atomic<bool> leaving_reader{};error returned=error::success;
    std::thread worker{[&]
    {
        std::size_t length{};returned=waiting.invoke(owner,capability,bytes.size(),72,length,[&](auto output)
            {std::copy(bytes.begin(),bytes.end(),output.begin());copied.arrive_and_wait();release_reader.arrive_and_wait();leaving_reader=true;return true;},
            [](auto const&){return true;},stopping.get_token());
    }};
    copied.arrive_and_wait();
    {
        std::lock_guard final_lock{waiting_stream.mutex};release_reader.arrive_and_wait();
        while(!leaving_reader.load())std::this_thread::yield();
        // The checked reader has finished and publication cannot take its final
        // mutex until this scope exits. Cancellation must be re-read there.
        stopping.request_stop();
    }
    worker.join();assert(returned==error::cancelled);pristine(waiting);assert(waiting.inspect().admissions==1);
    p::ordered_receipt stream{4120};p::coordinator endpoint{limits(),allow(),owner,capability,stream};std::size_t required{};receipt committed;
    assert(endpoint.invoke(owner,capability,bytes.size(),72,required,[&](auto output){std::copy(bytes.begin(),bytes.end(),output.begin());return true;},
        [&](auto const& value){committed=value;endpoint.revoke();return false;},std::stop_token{})==error::invalid);
    assert(committed.object&&endpoint.inspect().objects==1&&endpoint.inspect().last_key==1);
    assert(invoke(endpoint,bytes,committed)==error::denied);
}
static void scratch_and_atomic_budget_threads()
{
    auto bounds=limits();bounds.scratch_calls=1;p::ordered_receipt stream{4120};p::coordinator endpoint{bounds,allow(),owner,capability,stream};
    auto bytes=request(1);std::size_t required{};receipt committed;
    assert(endpoint.invoke(owner,capability,bytes.size(),72,required,[&](auto output)
        {receipt nested;assert(invoke(endpoint,bytes,nested)==error::again);std::copy(bytes.begin(),bytes.end(),output.begin());return true;},
        [&](auto const& value){committed=value;std::array<std::uint8_t,3> data{};std::size_t size{};
            assert(endpoint.read_owned(owner,capability,value.object,data,size)==error::success&&size==3);return true;},std::stop_token{})==error::success);
    assert(!endpoint.inspect().scratch_calls&&!endpoint.inspect().scratch_bytes);
    auto policy=allow();policy.logical_calls=3;policy.bytes_per_window=283;
    p::ordered_receipt concurrent_stream{4120};p::coordinator concurrent{limits(),policy,owner,capability,concurrent_stream};
    std::barrier copied{2};std::atomic<unsigned> successes{},denied{};
    auto worker=[&]
    {
        std::size_t size{};auto result=concurrent.invoke(owner,capability,bytes.size(),72,size,[&](auto output)
            {std::copy(bytes.begin(),bytes.end(),output.begin());copied.arrive_and_wait();return true;},[](auto const&){return true;},std::stop_token{});
        if(result==error::success)++successes;else{assert(result==error::again);++denied;}
    };
    std::thread first{worker},second{worker};first.join();second.join();auto count=concurrent.inspect();
    assert(successes==1&&denied==1&&count.objects==1&&count.window_calls==3&&count.window_bytes==283&&!count.scratch_calls&&!count.scratch_bytes);
}
static void fork_rejection()
{
    p::ordered_receipt stream{4120};p::coordinator endpoint{limits(),allow(),owner,capability,stream};auto bytes=request(1);
    auto child=fork();assert(child>=0);if(!child){receipt result;_exit(invoke(endpoint,bytes,result)==error::denied?0:1);}
    int status{};assert(waitpid(child,&status,0)==child&&WIFEXITED(status)&&WEXITSTATUS(status)==0);pristine(endpoint);
}
static void actual_xml_adapter_call_topology()
{
    scene::broker legacy;scene::grant grant;grant.alias="xmlout";grant.resource="serialized-xml";grant.binding="volatile-output";grant.kind=scene::purpose::cache;
    grant.max_handles=1;grant.max_resources=1;grant.actions={"artifact.begin","artifact.append","artifact.commit","artifact.remove","artifact.read"};
    assert(legacy.permissions.expose("capability.open",scene::exposure::allow)==error::success);
    for(auto const& name:grant.actions)assert(legacy.permissions.expose(name,scene::exposure::allow)==error::success);
    assert(legacy.permissions.configure(grant)==error::success&&legacy.output_store.bind("xmlout","serialized-xml",{65536,1,65536,4096})==error::success);
    assert(legacy.permissions.enroll(owner)==error::success);scene::handle cap{};assert(legacy.permissions.open(owner,"xmlout",cap)==error::success);
    auto bounds=limits();bounds.object_slots=bounds.max_handles=1;p::ordered_receipt stream{4120};p::coordinator proposed{bounds,allow(),owner,capability,stream};
    unsigned legacy_calls{},proposed_calls{},checks{};std::uint64_t legacy_key=1,proposed_key=1;
    auto mutate=[&](auto operation,std::vector<std::uint8_t> const& input)
    {
        ++legacy_calls;receipt value;std::size_t required{};
        assert(legacy.invoke_output(owner,operation,cap,input,72,required,[&](scene::bytes output)
            {value={scene::word(output,0),scene::word(output,1),scene::word(output,2),scene::word(output,3),scene::word(output,4),scene::word(output,5),scene::word(output,6),scene::word(output,7),scene::word(output,8)};return true;})==error::success);
        assert(!value.effect_status&&value.key==legacy_key++);return value;
    };
    using operation=uwvm2::imported::ros::output_contract::operation;
    for(auto data:publication_fixture_bytes)
    {
        auto initial=mutate(operation::begin,words({legacy_key,data.size()}));auto id=initial.object;
        if(!data.empty())mutate(operation::append,words({legacy_key,id,0},data));
        mutate(operation::commit,words({legacy_key,id,data.size()}));
        if(!data.empty())
        {
            ++legacy_calls;std::size_t size{};auto input=words({id,0,data.size()});
            assert(legacy.invoke_artifact(owner,uwvm2::imported::ros::artifact_contract::operation::read,cap,input,data.size(),size,
                [&](scene::bytes output){assert(std::equal(data.begin(),data.end(),output.begin(),output.end()));return true;})==error::success);
        }
        mutate(operation::remove,words({legacy_key,id}));
        auto input=words({proposed_key,data.size()},data);receipt published;++proposed_calls;
        assert(invoke(proposed,input,published)==error::success&&!published.effect_status&&published.key==proposed_key++);
        if(!data.empty())
        {
            ++proposed_calls;std::vector<std::uint8_t> output(data.size());std::size_t size{};
            assert(proposed.read_owned(owner,capability,published.object,output,size)==error::success&&size==data.size()&&std::equal(data.begin(),data.end(),output.begin()));
        }
        ++proposed_calls;receipt removed;assert(proposed.remove_owned(owner,capability,words({proposed_key++,published.object}),removed)==error::success&&!removed.effect_status);
        ++checks;
    }
    assert(checks==14&&legacy_calls==66&&proposed_calls==40&&!proposed.inspect().objects);
    std::cout<<"XML native adapter topology legacy="<<legacy_calls<<" proposed="<<proposed_calls<<" outputs="<<checks<<"; no new VM import executed\n";
}
int main()
{
    authority_and_defaults();bounds_short_and_budget();owned_and_partial_copy();retry_and_shared_stream();capacity_and_delivery();
    allocation_and_callback_faults();cancellation_and_revocation();scratch_and_atomic_budget_threads();fork_rejection();actual_xml_adapter_call_topology();
    std::cout<<"passed 10 complete-publication prototype groups\n";
}
