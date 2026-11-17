/** @file @brief Owned operation queue qualification with actual nonblocking pipes. */
#define main communication_async_fixture_entry
#include "communication_worker_async.cpp"
#undef main
#include <uwvm2/imported/ros/scene/communication_jobs.h>
namespace jobs=s::communication_jobs;
namespace c=s::communication;
struct queued_operation
{
    fixture pipes;jobs::queue queue;c::operation_token token;std::array<std::uint8_t,3> body{4,5,6};
    std::uint64_t deadline{};std::atomic<unsigned> admissions{};bool reentry{};unsigned admission_fault{};
    int peer{};s::caller peer_caller{&peer,1};s::handle alternate{};
    explicit queued_operation(std::uint64_t bytes=16384,std::uint64_t slots=2,std::optional<std::uint64_t> calls={}):pipes(bytes,calls)
    {
        token.caller_instance=pipes.rights.who.instance;token.caller_generation=pipes.rights.who.generation;
        token.authority={17,1,1};token.capability=pipes.rights.claims[0].capability;token.resource=44;
        token.request=uwvm2::imported::ros::resources::allocate_identity();token.session=44;token.sequence=1;
        token.family=c::operation_family::topic;token.action=c::permission::topic_publish;token.count=4;
        token.retained_origins=uwvm2::utils::container::make_shared<int>(1);
        for(std::size_t i{};i<4;++i)token.origins[i]={{17,i+1,1},pipes.rights.claims[i].capability,i==0?44u:i==1?55u:i==2?66u:0u,
            i==0?c::permission::topic_publish:i==1?c::permission::node_open:i==2?c::permission::context_open:c::permission::type_describe};
        assert(token.valid(c::permission::topic_publish,c::operation_family::topic,token.authority,44));
        assert(pipes.rights.authority.enroll(peer_caller)==s::error::success);
        assert(pipes.rights.authority.open(pipes.rights.who,"topic",alternate)==s::error::success);
        jobs::plan plan{token.authority,token.family,44,1,71,slots,bytes,256,1000000000,slots*(bytes+256)};
        assert(queue.configure(plan,admit,this)==error::ok);deadline=c::now()+900000000;
    }
    static error admit(void* opaque,c::operation_token const& token,s::group_lease& held)noexcept
    {
        auto& self=*static_cast<queued_operation*>(opaque);++self.admissions;
        if(!token.valid(c::permission::topic_publish,c::operation_family::topic,self.token.authority,44))return error::denied;
        if(self.reentry){jobs::receipt observed;assert(self.queue.inspect(token,token.request,observed)==error::ok);}
        jobs::claim_set claims;jobs::admission_claims(token,claims);s::caller who{token.caller_instance,token.caller_generation};
        if(self.admission_fault==1)claims[0].action="topic.open";
        if(self.admission_fault==2)claims[0].capability=self.alternate;
        if(self.admission_fault==3)who=self.peer_caller;
        if(self.admission_fault==4)std::swap(claims[1],claims[2]);
        if(self.admission_fault==5)claims[1].validation_only=false;
        auto status=self.pipes.rights.authority.admit_group(who,{claims.data(),token.count},held,false);
        if(status==s::error::success&&self.admission_fault==6)
            assert(self.pipes.rights.authority.close(who,token.origins[1].capability)==s::error::success);
        return static_cast<error>(status);
    }
    s::group_lease group()
    {s::group_lease result;assert(admit(this,token,result)==error::ok);return result;}
    jobs::receipt submit(s::wire::bytes bytes={})
    {
        if(bytes.empty())bytes=body;
        jobs::receipt result;
        assert(queue.reserve(token,rpc::operation::topic_publish,bytes,deadline,result)==error::ok&&result.phase==jobs::state::reserved);
        auto held=charged(bytes.size());assert(queue.commit(token,held,result)==error::ok&&result.phase==jobs::state::queued);return result;
    }
    s::group_lease charged(std::uint64_t size=3)
    {
        std::uint64_t bytes{};assert(jobs::submission_cost(size,256,bytes));jobs::claim_set claims;s::group_lease result;
        assert(pipes.rights.authority.admit_group(pipes.rights.who,jobs::admission_claims(token,claims,bytes),result)==s::error::success);return result;
    }
    jobs::receipt inspect()
    {jobs::receipt result;assert(queue.inspect(token,token.request,result)==error::ok);return result;}
    c::native_result complete()
    {c::native_result result;c::operation_token identity;assert(queue.completion(identity,result)==error::ok&&identity.request==token.request);return result;}
};
void exact_admission()
{
    std::uint64_t cost{};assert(jobs::submission_cost(3,256,cost)&&cost==355);
    assert(!jobs::submission_cost(UINT64_MAX,256,cost)&&!jobs::submission_cost(3,UINT64_MAX,cost)&&!jobs::submission_cost(0,0,cost));
    for(unsigned fault{};fault<9;++fault)
    {
        queued_operation f;jobs::receipt result;
        assert(f.queue.reserve(f.token,rpc::operation::topic_publish,f.body,f.deadline,result)==error::ok);
        jobs::claim_set claims;auto expected=jobs::admission_claims(f.token,claims,cost);auto who=f.pipes.rights.who;
        if(fault==2)claims[0].action="topic.open";
        if(fault==3)claims[0].capability=f.alternate;
        if(fault==4)who=f.peer_caller;
        if(fault==5)std::swap(claims[1],claims[2]);
        if(fault==6)claims[1].validation_only=false;
        if(fault==7)++claims[0].bytes;
        if(fault==8){++who.generation;assert(f.pipes.rights.authority.enroll(who)==s::error::success);}
        s::group_lease held;
        assert(f.pipes.rights.authority.admit_group(who,expected,held,fault>1,fault==1)==s::error::success);
        assert(held.size()==f.token.count); // Same count never establishes this operation's authority.
        assert(f.queue.commit(f.token,held,result)==error::denied&&f.pipes.drain()==0);
        assert(f.inspect().phase==jobs::state::reserved);
    }
    {
        queued_operation f;jobs::receipt result;
        assert(f.queue.reserve(f.token,rpc::operation::topic_publish,f.body,f.deadline,result)==error::ok);
        auto held=f.charged();assert(f.queue.commit(f.token,held,result)==error::ok);
        assert(f.queue.commit(f.token,held,result)==error::ok); // Exact receipt recovery does not consume twice.
        auto second=f.token;second.request=uwvm2::imported::ros::resources::allocate_identity();++second.sequence;
        assert(f.queue.reserve(second,rpc::operation::topic_publish,f.body,f.deadline,result)==error::ok);
        assert(f.queue.commit(second,held,result)==error::denied&&f.pipes.drain()==0);
    }
    {
        queued_operation f;jobs::claim_set claims;auto expected=jobs::admission_claims(f.token,claims,cost);
        auto borrowed=claims;uwvm2::utils::container::std_string action="topic.publish";borrowed[0].action=action;s::group_lease held;
        assert(f.pipes.rights.authority.admit_group(f.pipes.rights.who,{borrowed.data(),f.token.count},held)==s::error::success);
        action.assign(200,'x'); // Force destruction/reallocation of the caller's original text.
        assert(held.matches(f.pipes.rights.who,expected));
        auto moved=std::move(held);assert(!held&&moved.matches(f.pipes.rights.who,expected));
        struct task{s::group_lease* held;s::caller who;std::span<s::admission_claim const> claims;bool refused{};} work{&moved,f.pipes.rights.who,expected};pthread_t thread;
        assert(!::pthread_create(&thread,nullptr,[](void* p)->void*
        {auto& work=*static_cast<task*>(p);work.refused=!bool(*work.held)&&!work.held->matches(work.who,work.claims)&&!work.held->consume(work.who,work.claims);return nullptr;},&work));
        assert(!::pthread_join(thread,nullptr)&&work.refused&&moved.consume(f.pipes.rights.who,expected));
        assert(!moved.consume(f.pipes.rights.who,expected));moved.release();assert(!moved.matches(f.pipes.rights.who,expected));
    }
    for(unsigned invalidation{};invalidation<4;++invalidation)
    {
        queued_operation f;jobs::receipt result;
        assert(f.queue.reserve(f.token,rpc::operation::topic_publish,f.body,f.deadline,result)==error::ok);
        auto held=f.charged();
        if(invalidation==0)assert(f.pipes.rights.authority.close(f.pipes.rights.who,f.token.origins[1].capability)==s::error::success);
        if(invalidation==1)assert(f.pipes.rights.authority.revoke("type",[] (auto) {})==s::error::success);
        if(invalidation==2)f.pipes.rights.authority.retire_member(f.pipes.rights.who);
        if(invalidation==3)f.pipes.rights.authority.retire([]{});
        assert(!held&&f.queue.commit(f.token,held,result)==error::denied&&f.pipes.drain()==0);
    }
    for(unsigned fault{1};fault<=6;++fault)
    {
        queued_operation f;f.submit();f.admission_fault=fault;
        assert(f.queue.pump(f.pipes.transport)==error::ok&&f.pipes.drain()==0);
        auto result=f.inspect();assert(result.status==error::denied&&result.observed==c::effect::none&&!result.cleanup_debt);
    }
    {
        queued_operation f;f.submit();assert(f.queue.pump(f.pipes.transport)==error::ok);f.pipes.response(1,f.body);
        assert(f.queue.pump(f.pipes.transport)==error::ok);std::array<std::uint8_t,256> output;output.fill(0xa5);jobs::receipt result;
        for(unsigned fault{1};fault<=6;++fault)
        {
            f.admission_fault=fault;auto held=f.group();assert(f.queue.copy(f.token,held,output,result)==error::denied);
            for(auto byte:output)assert(byte==0xa5);
        }
    }
}
struct managed_submission
{
    queued_operation& source;std::atomic<unsigned> charges{};std::atomic<bool> entered{},release{true};bool reentry{},wrong{},close_origin{};
    explicit managed_submission(queued_operation& value):source(value){}
    static error charge(void* opaque,c::operation_token const& token,std::uint64_t bytes,s::group_lease& held)noexcept
    {
        auto& self=*static_cast<managed_submission*>(opaque);++self.charges;auto& f=self.source;
        jobs::receipt pending;assert(f.queue.inspect(token,token.request,pending)==error::ok&&pending.phase==jobs::state::reserved);
        assert(f.queue.reserve(token,rpc::operation::topic_publish,f.body,f.deadline,pending)==error::stale);
        assert(f.queue.abandon(token)==error::denied);
        if(self.reentry)assert(f.queue.enqueue(token,rpc::operation::topic_publish,f.body,f.deadline,charge,&self,pending)==error::again);
        self.entered=true;while(!self.release.load())std::this_thread::yield();
        jobs::claim_set claims;auto expected=jobs::admission_claims(token,claims,bytes);
        if(self.wrong)claims[0].capability=f.alternate;
        auto status=f.pipes.rights.authority.admit_group(f.pipes.rights.who,expected,held,true);
        if(status!=s::error::success)return static_cast<error>(status);
        assert(f.queue.commit(token,held,pending)==error::denied); // Managed reservation cannot be taken over.
        if(self.close_origin)assert(f.pipes.rights.authority.close(f.pipes.rights.who,token.origins[1].capability)==s::error::success);
        return error::ok;
    }
    error enqueue(jobs::receipt& result)
    {auto& f=source;return f.queue.enqueue(f.token,rpc::operation::topic_publish,f.body,f.deadline,charge,this,result);}
    std::uint64_t admitted()
    {
        uwvm2::utils::container::std_vector<s::authority_snapshot> rows;assert(source.pipes.rights.authority.inspect(rows,4));
        for(auto const& row:rows)if(row.alias=="topic")return row.admissions;
        assert(false);return 0;
    }
};
static void managed_reservation_and_exact_charge()
{
    {
        queued_operation f{16384,1,1};managed_submission submit{f};submit.reentry=true;jobs::receipt receipt;
        assert(submit.enqueue(receipt)==error::ok&&receipt.phase==jobs::state::queued&&submit.charges==1&&submit.admitted()==1&&f.pipes.drain()==0);
        assert(submit.enqueue(receipt)==error::ok&&submit.charges==1&&submit.admitted()==1);
        auto next=f.token;next.request=uwvm2::imported::ros::resources::allocate_identity();++next.sequence;
        assert(f.queue.enqueue(next,rpc::operation::topic_publish,f.body,f.deadline,managed_submission::charge,&submit,receipt)==error::again&&submit.charges==1);
        f.body[0]^=1;assert(submit.enqueue(receipt)==error::stale&&submit.charges==1);f.body[0]^=1;
        assert(f.queue.pump(f.pipes.transport)==error::ok&&f.pipes.drain()==51);f.pipes.response(1,f.body);
        assert(f.queue.pump(f.pipes.transport)==error::ok&&submit.enqueue(receipt)==error::ok&&receipt.phase==jobs::state::complete&&submit.charges==1);
        assert(f.complete().observed==c::effect::locally_accepted);
    }
    {
        queued_operation f{16384,1,0};managed_submission submit{f};jobs::receipt result;
        assert(submit.enqueue(result)==error::again&&submit.charges==1&&submit.admitted()==0&&f.pipes.drain()==0);
        assert(f.queue.inspect(f.token,f.token.request,result)==error::denied);
        assert(submit.enqueue(result)==error::again&&submit.charges==2&&submit.admitted()==0);
    }
    for(bool origin:{false,true})
    {
        queued_operation f;managed_submission submit{f};submit.wrong=!origin;submit.close_origin=origin;jobs::receipt result;
        assert(submit.enqueue(result)==error::denied&&f.pipes.drain()==0);
        assert(f.queue.inspect(f.token,f.token.request,result)==error::denied);
    }
    for(unsigned run{};run<8;++run)
    {
        queued_operation f{16384,1,1};managed_submission submit{f};submit.release=false;
        struct task{managed_submission* submission;error status{};jobs::receipt receipt{};} work{&submit};pthread_t thread;
        assert(!::pthread_create(&thread,nullptr,[](void* opaque)->void*
        {auto& work=*static_cast<task*>(opaque);work.status=work.submission->enqueue(work.receipt);return nullptr;},&work));
        while(!submit.entered.load())std::this_thread::yield();
        jobs::receipt duplicate;
        assert(submit.enqueue(duplicate)==error::again&&submit.charges==1);
        submit.release=true;assert(!::pthread_join(thread,nullptr)&&work.status==error::ok);
        assert(submit.enqueue(duplicate)==error::ok&&duplicate.request==work.receipt.request&&submit.charges==1&&submit.admitted()==1);
    }
}
int main()
{
    assert(communication_async_fixture_entry()==0);
    exact_admission();
    managed_reservation_and_exact_charge();
    {
        queued_operation f;jobs::receipt result;auto changed=f.token;changed.count=3;
        assert(f.queue.reserve(changed,rpc::operation::topic_publish,f.body,f.deadline,result)==error::denied);
        changed=f.token;changed.origins[3].resource=99;assert(!changed.valid(changed.action,changed.family,changed.authority,44));
        changed=f.token;changed.origins[7]=changed.origins[0];assert(!changed.valid(changed.action,changed.family,changed.authority,44));
        assert(f.queue.reserve(f.token,rpc::operation::service_submit,f.body,f.deadline,result)==error::denied);
        assert(f.queue.reserve(f.token,rpc::operation::topic_publish,f.body,f.deadline,result)==error::ok);
        s::group_lease empty;assert(f.queue.commit(f.token,empty,result)==error::denied);
        changed=f.token;++changed.caller_generation;assert(f.queue.reserve(changed,rpc::operation::topic_publish,f.body,f.deadline,result)==error::stale);
        assert(f.queue.abandon(f.token)==error::ok);f.submit();
        auto before=f.admissions.load();f.reentry=true;assert(f.queue.pump(f.pipes.transport)==error::ok&&f.admissions==before+1);
        assert(f.pipes.drain()==51);assert(f.queue.reserve(f.token,rpc::operation::topic_publish,f.body,f.deadline,result)==error::ok&&result.phase==jobs::state::waiting);
        assert(f.queue.pump(f.pipes.transport)==error::again&&f.admissions==before+1); // receive holds no admission group.
        f.pipes.response(1,f.body);assert(f.queue.pump(f.pipes.transport)==error::ok);
        auto finished=f.inspect();assert(finished.status==error::ok&&finished.bytes==3&&finished.provider_generation==71);
        auto held=f.group();std::array<std::uint8_t,2> short_copy;assert(f.queue.copy(f.token,held,short_copy,result)==error::too_large);
        std::array<std::uint8_t,256> copy;assert(f.queue.copy(f.token,held,copy,result)==error::ok&&copy[0]==4);held.release();
        assert(f.queue.release(f.token,f.token.request)==error::busy);auto settled=f.complete();assert(!settled.deferred&&settled.provider_generation==71);
        assert(f.queue.release(f.token,f.token.request)==error::ok);
        auto old=f.token;f.token.request=uwvm2::imported::ros::resources::allocate_identity();++f.token.sequence;f.submit();
        assert(f.queue.cancel(f.token,f.token.request,f.pipes.transport,result)==error::ok&&result.observed==c::effect::none);
        f.complete();assert(f.queue.release(f.token,f.token.request)==error::ok);
        f.token.request=uwvm2::imported::ros::resources::allocate_identity();++f.token.sequence;f.submit(); // reuse acknowledged finite slot.
        assert(f.queue.reserve(old,rpc::operation::topic_publish,f.body,f.deadline,result)==error::stale);
    }
    {
        queued_operation f;f.submit();assert(f.pipes.rights.authority.close(f.pipes.rights.who,f.token.capability)==s::error::success);
        assert(f.queue.pump(f.pipes.transport)==error::ok&&f.pipes.drain()==0);
        auto result=f.inspect();assert(result.status==error::bad_handle&&result.observed==c::effect::none&&!result.cleanup_debt);
    }
    {
        queued_operation f;f.deadline=c::now()+1000000;f.submit();::poll(nullptr,0,2);
        assert(f.queue.pump(f.pipes.transport)==error::ok&&f.pipes.drain()==0);
        auto result=f.inspect();assert(result.status==error::timeout&&result.observed==c::effect::none&&!result.cleanup_debt);
    }
    {
        queued_operation f;assert(::fcntl(f.pipes.input[1],F_SETPIPE_SZ,4096)==4096);std::array<std::uint8_t,8192> body{};f.submit(body);
        assert(f.queue.pump(f.pipes.transport)==error::again);assert(f.pipes.drain()>0);jobs::receipt result;
        assert(f.queue.cancel(f.token,f.token.request,f.pipes.transport,result)==error::ok&&result.observed==c::effect::unknown&&result.cleanup_debt);
        f.complete();assert(f.queue.release(f.token,f.token.request)==error::busy&&f.queue.cleanup(f.token.request,false)==error::busy);
        assert(f.queue.cleanup(f.token.request,true)==error::ok&&f.queue.release(f.token,f.token.request)==error::busy);
    }
    {
        queued_operation f;f.submit();assert(f.queue.pump(f.pipes.transport)==error::ok);f.pipes.response(1,f.body);
        // A fully parsed response is authoritative even if cancellation races later.
        auto wire=f.pipes.transport.inspect().current;assert(f.pipes.transport.receive(wire)==error::ok);jobs::receipt result;
        assert(f.queue.cancel(f.token,f.token.request,f.pipes.transport,result)==error::ok&&result.status==error::ok&&result.bytes==3&&!result.cleanup_debt);
    }
    for(unsigned run{};run<12;++run)
    {
        queued_operation f;f.submit();struct task{queued_operation* value;std::atomic<bool> entered{};error result{};} work{&f};pthread_t thread;
        assert(!::pthread_create(&thread,nullptr,[](void* opaque)->void*{auto& work=*static_cast<task*>(opaque);work.entered=true;work.result=work.value->queue.pump(work.value->pipes.transport);return nullptr;},&work));
        while(!work.entered.load()){}jobs::receipt result;
        assert(f.queue.cancel(f.token,f.token.request,f.pipes.transport,result)==error::ok);assert(!::pthread_join(thread,nullptr));
        auto sent=f.pipes.drain();assert(sent==0||sent==51);assert(result.observed==(sent?c::effect::unknown:c::effect::none)&&result.cleanup_debt==bool(sent));
    }
    std::puts("PASS owned communication jobs: full origin, reserve/charge/commit, reentrant admission, lease-free receive, exact replay, monotonic tombstones, revocation, expiry, atomic cancellation and unknown cleanup debt");
    return 0;
}
