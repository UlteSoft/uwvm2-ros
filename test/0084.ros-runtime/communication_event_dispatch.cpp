/** @file
 * @brief Stream authority, finite reservations, failed-copy recovery and concurrent revoke.
 */
#define UWVM_COMMUNICATION_DISPATCH_FIXTURE_ONLY
#include "communication_dispatch.cpp"
#include <uwvm2/imported/ros/scene/communication_event_dispatch.h>
namespace ec=uwvm2::imported::ros::event_contract;
static void event_permissions(s::authority& permissions)
{
    for(auto const& item:ec::operations)assert(permissions.expose(text(item.name),s::exposure::allow)==error::success);
    s::grant grant;grant.alias="events";grant.resource="events";grant.kind=s::purpose::event;grant.binding="event-stream";grant.max_resources=1;
    for(auto action:{"event.stream.open","event.stream.poll","event.stream.close","resource.close"})grant.actions.emplace(action);
    assert(permissions.configure(std::move(grant))==error::success);
}
struct event_fixture
{
    struct probe final:s::events::source
    {
        std::shared_ptr<s::events::source> provider;s::communication_event_dispatch* dispatcher;bool* observed;
        probe(std::shared_ptr<s::events::source> source,s::communication_event_dispatch* owner,bool* seen):provider(std::move(source)),dispatcher(owner),observed(seen){}
        ~probe() override{s::communication_event_dispatch::usage usage;assert(dispatcher->inspect({},usage)==error::success&&usage.workspace_bytes>=4096);*observed=true;}
        c::error inspect(c::owner owner,c::handle resource,c::handle generation,s::events::snapshot& out) noexcept override{return provider->inspect(owner,resource,generation,out);}
    };
    fixture origin{true,true,event_permissions};
    s::communication_event_dispatch dispatch{origin.authority,111,3,{1,4096,4096,16384,4}};
    s::handle cap{},stream{};bool backing_released_before_refund{};
    explicit event_fixture(bool prepared=true,bool decorated=false)
    {
        origin.sub_id=origin.opened(contract::operation::topic_open,origin.sub_cap,origin.endpoint("sub","subscription"));
        assert(dispatch.bind("events",{{{},profile("events"),1,2,3,10000000000ULL},1,4096,4096})==error::success);
        if(prepared)
        {
            auto access=s::communication_event_dispatch::source_access::from(origin.dispatch);
            if(decorated)access={this,[](void* pointer,s::caller who,s::handle cap,s::handle resource,s::events::binding& out)
                {auto& self=*static_cast<event_fixture*>(pointer);auto status=self.origin.dispatch.event_source(who,cap,resource,out);if(status!=error::success)return status;
                 out.provider=uwvm2::utils::container::make_shared<probe>(std::move(out.provider),&self.dispatch,&self.backing_released_before_refund);return error::success;},
                [](void* pointer,s::caller who,s::handle cap,s::events::binding const& source){return static_cast<event_fixture*>(pointer)->origin.dispatch.validate_source(who,cap,source);}};
            assert(dispatch.prepare(access)==error::success);
        }
        assert(origin.authority->open(origin.who,"events",cap)==error::success);
    }
    bytes open(s::handle source_cap=0,s::handle source=0,std::uint64_t space=4096,std::uint64_t output=4096)
    {return encode([&](auto& out){return out.u64(1)&&out.u64(source_cap?source_cap:origin.sub_cap)&&out.u64(source?source:origin.sub_id)&&
        contract::encode_profile(out,profile("events"))&&out.u64(output)&&out.u64(2)&&out.u64(space);});}
    bytes poll(){return encode([&](auto& out){return out.u64(stream)&&out.u64(2)&&out.u64(c::now()+1000000000ULL);});}
    error call(ec::operation action,w::bytes input,bytes& out,std::size_t capacity=4096,s::communication_event_dispatch::sink sink={},
        uwvm2::utils::container::function_ref<bool()> cancelled={})
    {
        std::size_t required{};auto copy=[&](w::bytes data){out.assign(data.begin(),data.end());return true;};
        auto status=dispatch.invoke(origin.who,action,cap,input,capacity,required,sink?sink:s::communication_event_dispatch::sink{copy},cancelled);
        if(status==error::success){auto* definition=schema::find(ec::operations[static_cast<std::size_t>(action)].name);assert(definition&&schema::validate(out,definition->result,{4096,256,64,32}));}
        return status;
    }
    void opened(){bytes out;assert(call(ec::operation::open,open(),out)==error::success);w::reader reader{out};assert(reader.u64(stream)&&reader.done());}
    std::uint64_t reservations(){s::communication_event_dispatch::usage usage;assert(dispatch.inspect({},usage)==error::success);return usage.workspace_bytes;}
};
#ifndef UWVM_COMMUNICATION_EVENT_FIXTURE_ONLY
static void event_origins_and_capacity()
{
    { event_fixture cancelled;auto input=cancelled.open();auto action=ec::operation::open;std::size_t required{};auto stop=[]{return true;};auto copy=[](w::bytes){return true;};
       assert(cancelled.dispatch.invoke(cancelled.origin.who,action,cancelled.cap,input,4096,required,copy,stop)==error::cancelled&&!cancelled.reservations()); }
    event_fixture inert(false);bytes out;assert(inert.call(ec::operation::open,inert.open(),out)==error::denied);
    event_fixture f;auto calls=f.origin.admissions("events");assert(f.call(ec::operation::open,f.open(),out,7)==error::too_large&&!f.reservations()&&f.origin.admissions("events")==calls);
    assert(f.call(ec::operation::open,f.open(0,0,1),out)==error::too_large&&!f.reservations());
    assert(f.call(ec::operation::open,f.open(0,0,4096,1),out)==error::too_large&&!f.reservations());
    assert(f.call(ec::operation::open,f.open(f.origin.pub_cap,f.origin.sub_id),out)==error::denied&&!f.reservations());
    assert(f.call(ec::operation::open,f.open(f.origin.sub_cap,f.origin.node_id),out)==error::bad_handle&&!f.reservations());
    f.opened();assert(f.reservations()==4096&&f.origin.admissions("context")==1&&f.origin.admissions("node")==1);
    assert(f.call(ec::operation::open,f.open(),out)==error::too_large&&f.reservations()==4096);
    auto old=f.stream;auto input=resource(old);auto throwing=[](w::bytes)->bool{throw 7;};
    assert(f.call(ec::operation::close,input,out,4096,throwing)==error::io&&!f.reservations());
    assert(f.call(ec::operation::close,input,out)==error::success&&!f.reservations());
    f.opened();assert(f.stream!=old&&f.reservations()==4096);
    assert(f.call(ec::operation::close,input,out)==error::bad_handle);
    assert(f.call(ec::operation::close,resource(f.stream),out)==error::success&&!f.reservations());
    // Closing only the stream leaves its actual subscription usable.
    assert(f.origin.call(contract::operation::topic_describe,f.origin.sub_cap,resource(f.origin.sub_id),out)==error::success);
}
static void independent_resource_close()
{
    event_fixture f;f.opened();auto first=f.stream;
    assert(f.dispatch.close(f.origin.who,f.origin.sub_cap,first)==error::denied);
    assert(f.dispatch.close(f.origin.who,f.cap,first+1)==error::bad_handle&&f.reservations());
    assert(f.origin.authority->close(f.origin.who,f.origin.context_cap)==error::success);
    assert(f.dispatch.close(f.origin.who,f.cap,first)==error::success&&!f.reservations());
    assert(f.dispatch.close(f.origin.who,f.cap,first)==error::success);
    event_fixture active;active.opened();bytes output;
    auto close_during_copy=[&](w::bytes){assert(active.dispatch.close(active.origin.who,active.cap,active.stream)==error::success);
        assert(active.reservations()>=4096);return true;};
    assert(active.call(ec::operation::poll,active.poll(),output,4096,close_during_copy)==error::cancelled&&!active.reservations());
}
static void event_failed_delivery()
{
    event_fixture f;bytes out,failed;auto reject=[](w::bytes){return false;};
    assert(f.call(ec::operation::open,f.open(),out,4096,reject)==error::io&&!f.reservations());f.opened();
    auto calls=f.origin.admissions("events");assert(f.call(ec::operation::poll,f.poll(),out,8)==error::too_large&&f.origin.admissions("events")==calls);
    auto fail=[&](w::bytes value){failed.assign(value.begin(),value.end());return false;};
    assert(f.call(ec::operation::poll,f.poll(),out,4096,fail)==error::io);
    assert(f.call(ec::operation::poll,f.poll(),out)==error::success&&out==failed);
    assert(f.call(ec::operation::poll,f.poll(),out)==error::again);
    c::metadata meta;meta.source=f.origin.sub_id;meta.generation=f.origin.native->generation;
    std::array<std::uint8_t,1> payload{1};assert(f.origin.native->queue->push(meta,payload)==c::error::ok);
    bool cancelled{};auto interrupted=[&](w::bytes value){failed.assign(value.begin(),value.end());cancelled=true;return true;};auto check=[&]{return cancelled;};
    assert(f.call(ec::operation::poll,f.poll(),out,4096,interrupted,check)==error::io);cancelled=false;
    assert(f.call(ec::operation::poll,f.poll(),out)==error::success&&out==failed);
}
static void event_revocation()
{
    event_fixture f;f.opened();bytes out;auto revoke=[&](w::bytes value)
    {out.assign(value.begin(),value.end());assert(f.origin.authority->close(f.origin.who,f.origin.context_cap)==error::success);return true;};
    assert(f.call(ec::operation::poll,f.poll(),out,4096,revoke)==error::bad_handle);
    assert(f.call(ec::operation::poll,f.poll(),out)==error::bad_handle);
    assert(f.call(ec::operation::close,resource(f.stream),out)==error::success&&!f.reservations());
    event_fixture reentrant;auto retire=[&](w::bytes){reentrant.dispatch.revoke("events");return true;};
    assert(reentrant.call(ec::operation::open,reentrant.open(),out,4096,retire)==error::cancelled&&!reentrant.reservations());
}
static void event_retained_close_capacity()
{
    event_fixture f(true,true);f.opened();bytes out;
    auto during_close=[&](w::bytes value)
    {
        out.assign(value.begin(),value.end());bytes nested;
        assert(f.reservations()>=4096);
        assert(f.call(ec::operation::open,f.open(),nested)==error::too_large);
        return true;
    };
    assert(f.call(ec::operation::close,resource(f.stream),out,4096,during_close)==error::success&&!f.reservations());
    assert(f.backing_released_before_refund);
    f.opened();auto during_poll=[&](w::bytes)
    {
        bytes nested;assert(f.call(ec::operation::close,resource(f.stream),nested)==error::success);
        assert(f.reservations()>=4096);
        assert(f.call(ec::operation::open,f.open(),nested)==error::too_large);
        return true;
    };
    assert(f.call(ec::operation::poll,f.poll(),out,4096,during_poll)==error::cancelled&&!f.reservations());
    f.opened();assert(f.call(ec::operation::close,resource(f.stream),out)==error::success&&!f.reservations());
}
static void event_concurrent_revoke_and_fork()
{
    event_fixture f;f.opened();bytes out;std::atomic<bool> entered{},attempted{},completed{};
    std::thread revoker([&]{while(!entered.load())std::this_thread::yield();attempted=true;
        assert(f.origin.authority->revoke("subscriber",[&](auto alias){f.origin.dispatch.revoke(alias);})==error::success);completed=true;});
    auto copy=[&](w::bytes value){out.assign(value.begin(),value.end());entered=true;while(!attempted.load())std::this_thread::yield();assert(!completed.load());return true;};
    auto result=f.call(ec::operation::poll,f.poll(),out,4096,copy);
    // The copying lease blocks revoke during the sink. After the sink releases
    // it, the stream performs another origin check: either linearization order
    // is legal, but a successful post-revocation poll never is.
    assert(result==error::success||result==error::denied);revoker.join();assert(completed.load());
    assert(f.call(ec::operation::poll,f.poll(),out)==error::denied);
    s::communication_event_dispatch::workspace_lease token;assert(f.dispatch.reserve_workspace(f.origin.who,ec::operation::poll,f.cap,100,token)==error::success);
    auto pid=::fork();assert(pid>=0);if(!pid){token.reset();assert(f.dispatch.preflight(f.origin.who,ec::operation::poll,f.cap,1)==error::denied);f.dispatch.retire();::_exit(0);}
    int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&!WEXITSTATUS(status));token.reset();
    assert(f.call(ec::operation::close,resource(f.stream),out)==error::success&&!f.reservations());
}
static void event_cancellation_reentrancy()
{
    for(bool local:{false,true})for(auto alias:{"context","node","request","subscriber","events"})for(unsigned phase:{1u,2u,3u})
    {
        event_fixture f;unsigned callbacks{},copies{};std::size_t required{};
        auto retire=[&](auto name){if(name==std::string_view{"events"})f.dispatch.revoke(name);else f.origin.dispatch.revoke(name);};
        auto cancel=[&]{if(++callbacks==phase){if(local)retire(alias);else assert(f.origin.authority->revoke(alias,retire)==error::success);}return false;};
        auto copy=[&](w::bytes){++copies;return true;};auto input=f.open();
        assert(f.dispatch.invoke(f.origin.who,ec::operation::open,f.cap,input,4096,required,copy,cancel)!=error::success&&callbacks>=phase&&!copies&&!f.reservations());
    }
    for(auto alias:{"context","node","request","subscriber","events"})for(unsigned phase:{2u,3u})
    {
        event_fixture f;f.opened();unsigned callbacks{},copies{};std::size_t required{};
        auto cancel=[&]{if(++callbacks==phase)assert(f.origin.authority->revoke(alias,[&](auto name){if(name==std::string_view{"events"})f.dispatch.revoke(name);else f.origin.dispatch.revoke(name);})==error::success);return false;};
        auto copy=[&](w::bytes){++copies;return true;};auto input=f.poll();
        assert(f.dispatch.invoke(f.origin.who,ec::operation::poll,f.cap,input,4096,required,copy,cancel)!=error::success&&!copies);
    }
}
int main(){independent_resource_close();event_cancellation_reentrancy();event_origins_and_capacity();event_failed_delivery();event_revocation();event_retained_close_capacity();event_concurrent_revoke_and_fork();std::cout<<"passed 6 event dispatch security groups\n";}

#endif
