// Test-only historical process owner. Never include or link into the VM product.
/** @file
 * @brief Isolated component lifecycle with exact original role authority and bounded IPC.
 * @details A prepared binding owns sealed code images and one reserved supervisor
 * stack. Opening copies one independently granted immutable configuration and
 * retains explicit topic handles. Starting is the only guest transition that
 * can launch code. The VM never dlopens a component. The Linux child inherits
 * the existing cgroup and the explicitly resolved CPU/NUMA policy.
 *
 * Role messages cannot select capabilities, endpoints, native symbols or host
 * functions. Every proxy effect uses the original capability cohort. Native
 * publication and receive settlement use retained exact keys, with no retry
 * under a fresh identity after uncertainty. Stop fences new role messages,
 * signals only pinned process identities, and reports exit only after the
 * entire namespace process tree is observed gone. Downstream uncertainty can
 * survive that exit. Restart requires a new deployment incarnation.
 */
#pragma once
#include "component_policy.h"
#include <uwvm2/imported/ros/scene/component_fault.h>
#include <uwvm2/imported/ros/scene/component_protocol.h>
#include <uwvm2/imported/ros/scene/bridge_dispatch.h>
#include <uwvm2/imported/ros/scene/artifacts.h>

namespace uwvm2::imported::ros::scene
{
    class component_dispatch
    {
    public:
        using operation=robot_contract::operation;using bytes=wire::bytes;using sink=communication_host::sink;using cancellation=communication_host::cancellation;
        struct limits {std::uint64_t bindings{},retained_bytes{};};
        struct inspection {std::uint64_t handles{},processes{},pending{},unknown{},retained_bytes{},messages{};bool ready{},retired{},exit_observed{},failed{};components::fault first_failure{};};
    private:
        struct role_state
        {robot_contract::reference target;handle publication_key{},settlement_key{};communication_receive_contract::reference reservation;};
        struct binding
        {
            components::plan policy;model_worker::configuration images;mutable std::mutex mutex;
            caller who{};handle cap{},id{},configuration_cap{},start_key{},stop_key{};
            buffers::digest start_digest{},stop_digest{};robot_contract::receipt started,stopped;
            std::array<role_state,16> roles{};byte_buffer configuration,incoming,outgoing,native;
            std::uint64_t until{},run_until{},stop_until{},messages{},stderr_seen{},unknown{};
            bool published{},opening{},queued{},launching{},launched{},ready{},exited{},retired{},failed{},finished{},cleanup_unknown{};
            components::fault first_failure{};
            std::atomic<bool> stop{},closing{};
#if defined(__linux__)
            model_sandbox::process child;model_worker::supervisor_stack stack;pthread_t thread{};bool threaded{},joined{};
#endif
            component_dispatch* host{};
        };
        std::shared_ptr<authority> permissions_;artifacts::registry& artifacts_;bridge_dispatch::transport transport_;
        handle generation_{};limits capacity_;std::uint64_t retained_{};
        mutable std::mutex configuration_,join_;
        ::uwvm2::utils::container::std_vector<std::shared_ptr<binding>> bindings_;
        bool prepared_{};std::atomic<bool> retired_{};
#if defined(__linux__)
        pid_t process_{::getpid()};
#endif
        static std::uint64_t now()noexcept{return static_cast<std::uint64_t>(resources::steady_now());}
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return process_==::getpid();
#else
            return true;
#endif
        }
        static bool same(caller a,caller b)noexcept{return a.instance==b.instance&&a.generation==b.generation;}
        static std::string_view name(operation action)noexcept{return robot_contract::operations[static_cast<std::size_t>(action)].name;}
        static bool match(robot_contract::profile const& a,buffers::profile const& b)noexcept
        {return a.identity==b.identity.view()&&a.digest.size()==32&&std::equal(a.digest.begin(),a.digest.end(),b.sha256.begin());}
        static bool profile(wire::writer& out,buffers::profile const& p)noexcept{return out.text(p.identity.view())&&out.blob(p.sha256);}
        static bool receipt(wire::writer& out,robot_contract::receipt const& r)noexcept
        {return out.u64(r.request)&&out.text(r.state)&&out.text(r.effect)&&out.u64(r.provider_generation)&&out.text(r.reason);}
        /** @brief Latch the first detected failure; caller owns the state lock.
         * @details Failure remains independent of cleanup, retirement and
         * unresolved effects. A later stop cannot rewrite the recorded cause.
         */
        static void fail_locked(binding& row,components::fault reason)noexcept
        {row.failed=true;if(row.first_failure==components::fault::none)row.first_failure=reason;}
        static bool fail(binding& row,components::fault reason)noexcept
        {std::lock_guard lock{row.mutex};fail_locked(row,reason);return false;}
        std::shared_ptr<binding> find(std::string_view alias)const noexcept
        {for(auto const& row:bindings_)if(row->policy.alias.view()==alias)return row;return {};}
        error select(caller who,operation action,handle cap,std::shared_ptr<binding>& result)
        {
            if(!same_process()||!permissions_||action<operation::component_open||action>operation::component_describe)return error::denied;
            lease held;auto status=permissions_->admit(who,cap,name(action),0,held,false);if(status!=error::success)return status;
            auto const& grant=held.specification();if(grant.kind!=purpose::component||grant.binding!="isolated-roles")return error::denied;
            std::lock_guard lock{configuration_};if(!prepared_)return error::denied;result=find(grant.alias);return result?error::success:error::denied;
        }
        /** @brief Revalidate every original role and configuration scope as one coherent authority group. */
        error group(binding const& row,operation action,group_lease& held,std::uint64_t bytes=0,bool charge=false)
        {
            std::array<admission_claim,100> claims{};std::size_t count{};
            claims[count++]={row.cap,name(action),bytes,false,false,action==operation::component_stop};
            if(action==operation::component_stop)
            {auto status=permissions_->admit_group(row.who,{claims.data(),count},held,charge);return status!=error::success?status:!charge||held.consume(row.who,{claims.data(),count})?error::success:error::denied;}
            for(auto action:{"artifact.open","artifact.read","artifact.describe"})claims[count++]={row.configuration_cap,action,0,false,true};
            std::array<std::size_t,16> indices{};
            for(std::size_t i{};i<row.policy.role_count;++i)
            {
                auto cap=row.roles[i].target.capability;indices[i]=count;claims[count++]={cap,"topic.describe",0,false,true};
                if(row.policy.roles[i].publisher)claims[count++]={cap,"topic.publish",0,false,true};
                else for(auto action:{"topic.reserve","topic.acknowledge","topic.release","data.export"})claims[count++]={cap,action,0,action==std::string_view("data.export"),true};
            }
            auto status=permissions_->admit_group(row.who,{claims.data(),count},held,charge);if(status!=error::success)return status;
            auto configuration=held.specification(1);
            if(!configuration||configuration->kind!=purpose::configuration||configuration->binding!="sealed-input"||
                configuration->alias!=row.policy.configuration_alias.view()||configuration->resource!=row.policy.configuration_identity.view())return error::denied;
            for(std::size_t i{};i<row.policy.role_count;++i)
            {auto grant=held.specification(indices[i]);if(!grant||grant->kind!=purpose::topic||grant->binding!="native-communication"||grant->alias!=row.policy.roles[i].alias.view())return error::denied;}
            return !charge||held.consume(row.who,{claims.data(),count})?error::success:error::denied;
        }
        error validate_targets(binding const& row)
        {
            for(std::size_t i{};i<row.policy.role_count;++i)
            {
                if(!transport_.context||!transport_.invoke||!transport_.receive)return error::denied;
                auto const& policy=row.policy.roles[i];auto codec=communication::codecs::find(policy.codec.view());
                std::array<std::uint8_t,8> request;wire::writer writer{request};writer.u64(row.roles[i].target.resource);std::size_t required{};bool correct{};
                auto status=transport_.invoke(transport_.context,row.who,communication_contract::operation::topic_describe,row.roles[i].target.capability,request,2048,required,[&](bytes response)
                {
                    wire::reader in{response};std::uint64_t node{},generation{};communication_contract::profile_view actual;
                    std::string_view name,type,direction;bool present{};bytes gid;
                    correct=in.u64(node)&&node&&communication_contract::read_profile(in,actual)&&actual.identity==policy.selected.identity.view()&&
                        actual.digest.size()==32&&std::equal(actual.digest.begin(),actual.digest.end(),policy.selected.sha256.begin())&&in.text(name)&&communication::absolute_name(name)&&
                        in.text(type)&&type==codec->identity&&in.text(direction)&&direction==(policy.publisher?"publisher":"subscription")&&
                        in.boolean(present)&&(!present||(in.u64(generation)&&generation))&&in.boolean(present)&&(!present||(in.blob(gid)&&!gid.empty()))&&in.done();return correct;
                });if(status!=error::success||!correct)return status==error::success?error::denied:status;
            }
            return error::success;
        }
        /** @brief Reaping confirms cleanup; it never clears a native/protocol failure.
         * @details Failure and kernel-confirmed exit are independent facts.
         * Unknown role effects retain their stronger uncertainty state. A
         * successful historical start receipt never authorizes another launch.
         */
        static std::string_view state(binding const& row)noexcept
        {return row.retired?"retired":row.unknown?"outcome_unknown":row.failed?"failed":row.exited?"exited":row.stop.load()?"stopping":row.ready?"ready":row.launching||row.queued?"starting":"prepared";}
        bool describe(binding const& row,wire::writer& out)const noexcept
        {
            auto const& last=row.stopped.request>row.started.request?row.stopped:row.started;
            std::uint64_t pending=row.unknown;
            for(std::size_t i{};i<row.policy.role_count;++i)pending+=bool(row.roles[i].reservation.nonce);
            return profile(out,row.policy.selected)&&out.text(state(row))&&out.u64(generation_)&&out.u64(row.launched&&!row.exited?1:0)&&
                out.u64(row.policy.retained_bytes)&&out.u64(pending)&&out.u64(row.unknown)&&out.boolean(row.exited)&&out.u64(static_cast<std::uint64_t>(row.first_failure))&&
                out.boolean(bool(last.request))&&(!last.request||receipt(out,last));
        }
#if defined(__linux__)
        static void pause(binding const& row)noexcept
        {auto interval=row.images.limits().poll_ns;timespec duration{static_cast<time_t>(interval/1000000000),static_cast<long>(interval%1000000000)};(void)::nanosleep(&duration,nullptr);}
        bool current(binding& row,std::uint64_t deadline)noexcept
        {
            if(row.stop.load()||row.closing.load()||retired_.load())return false;
            if(now()>=deadline)return fail(row,components::fault::deadline_elapsed);
            try{group_lease held;if(group(row,operation::component_start,held)==error::success)return true;}
            catch(...){}
            return fail(row,components::fault::authority_lost);
        }
        bool diagnostics(binding& row)noexcept
        {
            std::array<std::uint8_t,512> bytes;auto count=::read(row.child.stderr_fd(),bytes.data(),bytes.size());
            if(count<=0)return count==0||errno==EAGAIN||errno==EINTR||fail(row,components::fault::channel_failed);
            auto maximum=row.images.limits().stderr_bytes;
            if(row.stderr_seen>maximum||static_cast<std::uint64_t>(count)>maximum-row.stderr_seen)return fail(row,components::fault::diagnostic_limit);
            row.stderr_seen+=static_cast<std::uint64_t>(count);return true;
        }
        /** @brief Nonblocking bounded IPC; permission locks never span a wait. */
        bool transfer(binding& row,int fd,std::span<std::uint8_t> data,bool writing,std::uint64_t deadline)noexcept
        {
            std::size_t offset{};
            while(offset<data.size())
            {
                if(!current(row,deadline)||!diagnostics(row))return false;
                ssize_t count{};
                if(writing)
                {
                    try
                    {
                        group_lease held;if(group(row,operation::component_start,held)!=error::success)return fail(row,components::fault::authority_lost);
                        std::lock_guard lock{row.mutex};if(row.stop.load()||row.closing.load()||retired_.load())return false;
                        if(now()>=deadline){fail_locked(row,components::fault::deadline_elapsed);return false;}
                        count=::write(fd,data.data()+offset,std::min<std::size_t>(4096,data.size()-offset));
                    }
                    catch(...){return fail(row,components::fault::channel_failed);}
                }
                else count=::read(fd,data.data()+offset,std::min<std::size_t>(4096,data.size()-offset));
                if(count>0){offset+=static_cast<std::size_t>(count);continue;}
                if(!count||(errno!=EINTR&&errno!=EAGAIN))
                {if(row.stop.load()||row.closing.load()||retired_.load())return false;return fail(row,components::fault::channel_failed);}pause(row);
            }
            return true;
        }
        bool send(binding& row,bytes data,std::uint64_t deadline)noexcept
        {
            std::array<std::uint8_t,8> header;wire::writer out{header};out.u64(data.size());
            return transfer(row,row.child.input_fd(),header,true,deadline)&&transfer(row,row.child.input_fd(),{const_cast<std::uint8_t*>(data.data()),data.size()},true,deadline);
        }
        bool receive(binding& row,bytes& output,std::uint64_t deadline)noexcept
        {
            std::array<std::uint8_t,8> header;if(!transfer(row,row.child.output_fd(),header,false,deadline))return false;
            wire::reader in{header};std::uint64_t size{};if(!in.u64(size)||size>row.policy.ipc_bytes||size<16)return fail(row,components::fault::protocol_violation);
            auto data=row.incoming.writable().first(static_cast<std::size_t>(size));if(!transfer(row,row.child.output_fd(),data,false,deadline))return false;
            output=data;return true;
        }
        bool marker(binding& row,std::string_view marker,std::uint64_t deadline)noexcept
        {
            std::array<std::uint8_t,64> bytes;if(marker.size()>bytes.size()||!transfer(row,row.child.output_fd(),{bytes.data(),marker.size()},false,deadline))return false;
            return std::equal(marker.begin(),marker.end(),bytes.begin())||fail(row,components::fault::startup_protocol);
        }
        /** @brief Route one role-only child message with a retained native transaction identity.
         * @details A receive deadline is not proof of an empty queue. Returning
         * a known timeout requires an independently authorized topic.release
         * inspection that confirms no outstanding RPC or retained reservation.
         * Denial, contention and unresolved receive debt latch uncertainty and
         * remain observable after the isolated process exits.
         */
        bool proxy(binding& row,std::uint64_t sequence,std::uint64_t slot,component_protocol::action action,bytes body)noexcept
        {
            if(!slot||slot>row.policy.role_count||body.size()>row.policy.message_bytes)return false;
            auto& role=row.roles[slot-1];auto const& policy=row.policy.roles[slot-1];auto codec=communication::codecs::find(policy.codec.view());
            if((action==component_protocol::action::publish)!=policy.publisher)return false;
            std::uint64_t until=std::min(row.run_until,now()+row.policy.deadline_ns);wire::writer request{row.native.writable()};
            if(action==component_protocol::action::publish)
            {
                if(role.publication_key==UINT64_MAX||!codec->validate(body))return false;
                if(!request.u64(role.target.resource)||!request.u64(role.publication_key+1)||!request.u64(role.target.resource)||
                    !communication_contract::encode_message(request,{codec->identity,codec->encoding,body})||!request.u64(until))return false;
            }
            else if(action==component_protocol::action::reserve)
            {if(!body.empty()||role.reservation.nonce)return false;if(!request.u64(role.target.resource)||!request.u64(until))return false;}
            else
            {
                if(action!=component_protocol::action::acknowledge&&action!=component_protocol::action::release)return false;
                wire::reader input{body};std::uint64_t generation{},number{},nonce{};
                if(!input.u64(generation)||!input.u64(number)||!input.u64(nonce)||!input.done()||!nonce||generation!=role.reservation.provider_generation||
                    number!=role.reservation.sequence||nonce!=role.reservation.nonce||role.settlement_key==UINT64_MAX)return false;
                if(!request.u64(role.target.resource)||!request.u64(role.settlement_key+1)||!request.u64(role.target.resource)||!request.u64(generation)||!request.u64(number)||!request.u64(nonce))return false;
            }
            error status=error::again;std::size_t copied{};bool attempted{};
            while(current(row,until))
            {
                try
                {
                    group_lease held;status=group(row,operation::component_start,held);if(status!=error::success)break;
                    std::lock_guard lock{row.mutex};if(row.stop.load()||row.closing.load()||retired_.load()||now()>=until)break;
                    std::size_t required{};auto reply_bound=action==component_protocol::action::reserve?row.policy.message_bytes+2048:256;
                    auto output=row.outgoing.writable().subspan(24,static_cast<std::size_t>(reply_bound));copied=0;
                    auto sink=[&](bytes bytes){if(bytes.size()>output.size())return false;std::copy(bytes.begin(),bytes.end(),output.begin());copied=bytes.size();return true;};
                    attempted=true;
                    status=action==component_protocol::action::publish?
                        transport_.invoke(transport_.context,row.who,communication_contract::operation::topic_publish,role.target.capability,request.written(),output.size(),required,sink):
                        transport_.receive(transport_.context,row.who,action==component_protocol::action::reserve?communication_receive_contract::operation::topic_reserve:
                            action==component_protocol::action::acknowledge?communication_receive_contract::operation::topic_acknowledge:communication_receive_contract::operation::topic_release,
                            role.target.capability,request.written(),output.size(),required,sink);
                }
                catch(...){status=error::io;}
                if(status!=error::again){break;}pause(row);
            }
            bool resolved=status==error::success;
            if(resolved)
            {
                auto response=row.outgoing.view().subspan(24,copied);
                if(action==component_protocol::action::reserve)
                {
                    robot_contract::response_decoder in{response,{response.size(),std::max<std::uint64_t>(row.policy.message_bytes,256),255,0}};
                    communication_receive_contract::reference reserved;robot_contract::metadata origin;robot_contract::message message;
                    std::uint64_t deadline{},correlation{};std::string_view kind;
                    resolved=in.identity(reserved.endpoint)&&reserved.endpoint==role.target.resource&&in.identity(reserved.provider_generation)&&
                        in.identity(reserved.sequence)&&in.identity(reserved.nonce)&&in.deadline(deadline)&&deadline==until&&in.origin(origin)&&
                        origin.source==role.target.resource&&origin.sequence==reserved.sequence&&in.number(correlation)&&!correlation&&in.text(kind)&&kind=="topic.message"&&
                        in.body(message)&&message.type==codec->identity&&message.encoding==codec->encoding&&message.body.size()<=row.policy.message_bytes&&codec->validate(message.body)&&in.done();
                    if(resolved){std::lock_guard lock{row.mutex};role.reservation=reserved;}
                }
                else
                {
                    robot_contract::receipt receipt;robot_contract::response_decoder in{response,{response.size(),0,255,0}};
                    resolved=in.outcome(receipt)&&in.done()&&receipt.state!="outcome_unknown"&&(receipt.effect=="none"||receipt.effect=="locally_accepted"||receipt.effect=="observed_remote");
                    if(resolved){if(action==component_protocol::action::publish)++role.publication_key;else{std::lock_guard lock{row.mutex};++role.settlement_key;role.reservation={};}}
                }
            }
            if(attempted&&action==component_protocol::action::reserve&&(status==error::timeout||status==error::again))
            {
                bool empty{};error inspected=error::io;
                try{inspected=transport_.quiescent(transport_.context,row.who,role.target.capability,role.target.resource,empty);}
                catch(...){empty=false;}
                if(inspected==error::success&&empty){status=error::timeout;copied=0;resolved=true;}
            }
            if(!resolved&&attempted)
            {std::lock_guard lock{row.mutex};row.unknown=1;return false;}
            if(!resolved)return false;
            wire::writer result{row.outgoing.writable().first(24)};result.u64(sequence);result.u64(static_cast<std::uint64_t>(status));result.u64(copied);
            {std::lock_guard lock{row.mutex};if(row.messages==UINT64_MAX)return false;++row.messages;}
            return send(row,row.outgoing.view().first(copied+24),row.run_until);
        }
        bool execute(binding& row)noexcept
        {
            auto deadline=row.until;error result=error::again;
            while(current(row,deadline))
            {result=row.child.identify();if(result!=error::again)break;pause(row);}
            if(result!=error::success)
            {if(row.stop.load()||row.closing.load()||retired_.load())return false;return fail(row,components::fault::startup_protocol);}
            if(!marker(row,component_protocol::ready,deadline))return false;
            do{result=row.child.enable_weights();if(result==error::again)pause(row);}while(result==error::again&&current(row,deadline));
            if(result!=error::success)
            {if(row.stop.load()||row.closing.load()||retired_.load())return false;return fail(row,components::fault::startup_protocol);}
            if(!marker(row,component_protocol::attached,deadline))return false;
            wire::writer startup{row.outgoing.writable()};startup.u64(generation_);startup.u64(row.id);profile(startup,row.policy.selected);
            if(!startup.blob(row.configuration.view())||!startup.u64(row.policy.role_count))return false;
            for(std::size_t i{};i<row.policy.role_count;++i)
                if(!startup.u64(i+1)||!startup.boolean(row.policy.roles[i].publisher)||!profile(startup,row.policy.roles[i].selected))return false;
            if(!send(row,startup.written(),deadline)){return false;}bytes received;if(!receive(row,received,deadline))return false;
            wire::reader response{received};std::uint64_t kind{},generation{},instance{};
            if(!response.u64(kind)||kind!=static_cast<std::uint64_t>(component_protocol::frame::started)||!response.u64(generation)||generation!=generation_||
                !response.u64(instance)||instance!=row.id||!response.done())return fail(row,components::fault::startup_protocol);
            {std::lock_guard lock{row.mutex};if(row.stop.load())return false;row.ready=true;row.launching=false;row.started.state="succeeded";row.started.effect="observed_remote";}
            std::uint64_t sequence{};
            while(receive(row,received,row.run_until))
            {
                wire::reader in{received};std::uint64_t kind{},number{},slot{},action{};bytes body;
                if(!in.u64(kind)||!in.u64(number))return fail(row,components::fault::protocol_violation);
                if(kind==static_cast<std::uint64_t>(component_protocol::frame::finished))
                {bool succeeded{};if(number!=sequence||!in.boolean(succeeded)||!in.done())return fail(row,components::fault::protocol_violation);
                    std::lock_guard lock{row.mutex};row.finished=true;if(!succeeded)fail_locked(row,components::fault::backend_failed);return succeeded;}
                if(kind!=static_cast<std::uint64_t>(component_protocol::frame::call)||sequence==UINT64_MAX||number!=sequence+1||!in.u64(slot)||!in.u64(action)||
                    action<1||action>4||!in.blob(body)||!in.done())return fail(row,components::fault::protocol_violation);
                if(!proxy(row,number,slot,static_cast<component_protocol::action>(action),body))return false;
                ++sequence;
            }
            return false;
        }
        /** @brief Reap without confusing launcher exit with whole-namespace cleanup.
         * @details Failed namespace identity leaves conservative process debt,
         * retained reservations and an explicit unknown stop receipt. The
         * supervisor terminates after reaping its child, but neither stop nor
         * historical start retries can clear that debt or authorize a restart.
         * A diagnostic deadline alone never confirms exit.
         */
        void cleanup(binding& row)noexcept
        {
            row.child.request_stop();auto until=now()+row.policy.shutdown_ns;
            {std::lock_guard lock{row.mutex};if(row.stop_until)until=std::min(until,row.stop_until);}
            for(;;)
            {
                {std::lock_guard lock{row.mutex};if(row.stop_until)until=std::min(until,row.stop_until);}
                int status{};auto result=row.child.reap(status);
                if(result==error::success)
                {
                    std::lock_guard lock{row.mutex};row.ready=false;row.launching=false;
                    if(!row.child.cleanup_confirmed())
                    {
                        row.cleanup_unknown=true;row.unknown=1;
                        if(row.started.state=="queued"||row.started.state=="running")
                            {row.started.state="outcome_unknown";row.started.effect="unknown";row.started.reason="startup.unconfirmed";}
                        if(row.stopped.request){row.stopped.state="outcome_unknown";row.stopped.effect="unknown";row.stopped.reason="exit.unconfirmed";}
                        return;
                    }
                    row.exited=true;
                    for(std::size_t i{};i<row.policy.role_count;++i)if(row.roles[i].reservation.nonce)row.unknown=1;
                    if(row.started.state=="queued"||row.started.state=="running"){row.started.state="failed";row.started.effect="committed";row.started.reason="startup.failed";}
                    if(row.stopped.request){row.stopped.state="succeeded";row.stopped.effect="observed_remote";row.stopped.reason="";}
                    return;
                }
                if(now()>=until)
                {std::lock_guard lock{row.mutex};if(row.stopped.request){row.stopped.state="outcome_unknown";row.stopped.effect="unknown";row.stopped.reason="exit.unconfirmed";}}
                pause(row);
            }
        }
        static void* run(void* opaque)noexcept
        {
            auto& row=*static_cast<binding*>(opaque);auto& self=*row.host;
            sigset_t blocked;::sigemptyset(&blocked);::sigaddset(&blocked,SIGPIPE);::pthread_sigmask(SIG_BLOCK,&blocked,nullptr);
            for(;;)
            {
                bool queued{};{std::lock_guard lock{row.mutex};queued=row.queued;}
                if(row.stop.load()||row.closing.load()||self.retired_.load())break;
                if(!queued){pause(row);continue;}
                bool launch_failed{};
                try
                {
                    group_lease held;auto status=self.group(row,operation::component_start,held);
                    std::lock_guard lock{row.mutex};row.queued=false;
                    // Stop can win after the unlocked queue observation. It is
                    // cancellation without a native effect, not a launch fault.
                    // Closing/retirement must be rechecked under this same lock.
                    if(row.stop.load()||row.closing.load()||self.retired_.load())break;
                    if(status!=error::success)
                    {fail_locked(row,components::fault::admission_refused);row.started.state="failed";row.started.effect="none";row.started.reason="startup.denied";break;}
                    if(now()>=row.until)
                    {fail_locked(row,components::fault::deadline_elapsed);row.started.state="failed";row.started.effect="none";row.started.reason="startup.expired";break;}
                    row.launching=true;status=row.child.launch();row.launched=row.child.was_launched();
                    if(status!=error::success)
                    {fail_locked(row,components::fault::launch_failed);row.launching=false;row.started.state="failed";row.started.effect="none";row.started.reason="launch.failed";launch_failed=true;}
                    else {row.started.state="running";row.started.effect="locally_accepted";}
                }
                catch(...)
                {std::lock_guard lock{row.mutex};fail_locked(row,components::fault::launch_failed);row.launched=row.child.was_launched();row.launching=false;
                    row.started.state="failed";row.started.effect="none";row.started.reason="launch.failed";launch_failed=true;}
                // Fork may succeed before the launch gate fails. Retain and
                // reap that process outside both authority and state locks.
                if(launch_failed){if(row.launched){self.cleanup(row);return nullptr;}break;}
                if(!self.execute(row))
                {std::lock_guard lock{row.mutex};if(!row.stop.load()&&!row.closing.load()&&!self.retired_.load())fail_locked(row,components::fault::channel_failed);}
                self.cleanup(row);return nullptr;
            }
            std::lock_guard lock{row.mutex};row.queued=false;row.retired|=row.stop.load()||row.closing.load()||self.retired_.load();
            if(!row.launched&&row.started.request&&row.started.state=="queued")
            {row.started.state="cancelled";row.started.effect="none";row.started.reason="startup.cancelled";}
            if(row.stopped.request){row.stopped.state="succeeded";row.stopped.effect="committed";row.stopped.reason="";}return nullptr;
        }
#endif
    public:
        component_dispatch(std::shared_ptr<authority> authority,artifacts::registry& artifacts,bridge_dispatch::transport transport,handle generation,limits capacity)
            :permissions_(std::move(authority)),artifacts_(artifacts),transport_(transport),generation_(generation),capacity_(capacity){}
        ~component_dispatch(){retire();}
        static bool required_storage(components::plan const& p,std::uint64_t& bytes)noexcept
        {if(!components::valid(p))return false;bytes=sizeof(binding)+p.configuration_bytes+3*p.ipc_bytes+4*::uwvm2::utils::allocator::fast_io_strict::array_prefix<std::uint8_t>;return true;}
        /** @brief Prepare exact sealed code and finite buffers without executing the component. */
        error bind(components::plan const& plan,model_worker::configuration const& images)
        {
            if(!same_process()||!generation_)return error::denied;
            if(plan.role_count&&(!transport_.context||!transport_.invoke||!transport_.receive||!transport_.quiescent))return error::invalid;
            std::uint64_t needed{};if(!required_storage(plan,needed)||needed>plan.retained_bytes||!images.frozen())return error::invalid;
            std::lock_guard lock{configuration_};if(prepared_||retired_.load())return error::denied;if(find(plan.alias.view()))return error::conflict;
            if(bindings_.size()>=capacity_.bindings||retained_>capacity_.retained_bytes||plan.retained_bytes>capacity_.retained_bytes-retained_)return error::too_large;
#if defined(__linux__)
            auto row=::uwvm2::utils::container::make_shared<binding>();row->policy=plan;row->images=images;row->host=this;
            row->configuration.resize_for_overwrite(plan.configuration_bytes);row->incoming.resize_for_overwrite(plan.ipc_bytes);
            row->outgoing.resize_for_overwrite(plan.ipc_bytes);row->native.resize_for_overwrite(plan.ipc_bytes);
            auto status=images.prepare_component(row->child);if(status!=error::success)return status;
            bindings_.push_back(std::move(row));retained_+=plan.retained_bytes;return error::success;
#else
            return error::unsupported;
#endif
        }
        /** @brief Atomically prepare finite idle supervisors without launching component code.
         * @details A stack, attribute or thread-creation failure permanently
         * fences this dispatcher and joins every previously started supervisor
         * before returning. No binding or retry can reuse a partial preparation.
         * Joining runs outside configuration, binding and authority locks; the
         * caller must also hold no authority lease. Prepared image/buffer
         * reservations remain owned until dispatcher destruction. Deployment
         * publishes the dispatcher only after this operation succeeds.
         */
        error prepare()noexcept
        {
            if(!same_process()){return error::denied;}std::unique_lock lock{configuration_};if(prepared_||retired_.load())return error::denied;
#if defined(__linux__)
            auto rollback=[&](error status)noexcept
            {
                retired_.store(true);
                for(auto const& row:bindings_)
                {row->closing.store(true);std::lock_guard state{row->mutex};row->retired=true;}
                lock.unlock();retire();return status;
            };
            for(auto const& row:bindings_)
            {
                if(retired_.load())return rollback(error::denied);
                auto status=row->stack.prepare(row->images.limits().supervisor_bytes);if(status!=error::success)return rollback(status);
                pthread_attr_t attr;if(::pthread_attr_init(&attr))return rollback(error::io);
                auto result=::pthread_attr_setstack(&attr,row->stack.data(),row->stack.size());if(!result)result=::pthread_create(&row->thread,&attr,run,row.get());
                ::pthread_attr_destroy(&attr);if(result)return rollback(error::io);row->threaded=true;
            }
            if(retired_.load())return rollback(error::denied);
            prepared_=true;return error::success;
#else
            return error::unsupported;
#endif
        }
        error preflight(caller who,operation action,handle cap,std::size_t extent)
        {if(extent>16384)return error::too_large;std::shared_ptr<binding> row;return select(who,action,cap,row);}
        error invoke(caller who,operation action,handle cap,bytes input,std::size_t capacity,std::size_t& required,sink deliver,cancellation cancelled={})noexcept;
        error close(caller who,handle cap,handle id)noexcept;
        error inspect(std::string_view alias,inspection& output)const noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock config{configuration_,std::try_to_lock};if(!config.owns_lock())return error::again;auto row=find(alias);if(!row)return error::denied;
            std::unique_lock lock{row->mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            std::uint64_t pending=row->unknown;for(std::size_t i{};i<row->policy.role_count;++i)pending+=bool(row->roles[i].reservation.nonce);
            output={row->published?1u:0u,row->launched&&!row->exited?1u:0u,pending,row->unknown,row->policy.retained_bytes,row->messages,row->ready,row->retired,row->exited,row->failed,row->first_failure};return error::success;
        }
        /** @brief Fence affected roles and signal reserved process cleanup without joining under authority. */
        void revoke(std::string_view alias)noexcept
        {
            if(!same_process()){return;}std::lock_guard config{configuration_};
            for(auto const& row:bindings_)
            {
                bool dependent=alias==row->policy.alias.view()||alias==row->policy.configuration_alias.view();
                for(std::size_t i{};i<row->policy.role_count;++i)dependent|=alias==row->policy.roles[i].alias.view();
                if(!dependent){continue;}std::lock_guard lock{row->mutex};row->retired=true;row->stop.store(true);
#if defined(__linux__)
                row->child.request_stop();
#endif
            }
        }
        void request_stop()noexcept
        {
            if(!same_process()){return;}retired_.store(true);std::lock_guard lock{configuration_};
            for(auto const& row:bindings_){row->closing.store(true);
#if defined(__linux__)
                row->child.request_stop();
#endif
            }
        }
        /** @brief Join all supervisors before native provider or authority destruction. */
        void retire()noexcept
        {
            if(!same_process()){return;}request_stop();std::lock_guard lock{join_};
#if defined(__linux__)
            for(auto const& row:bindings_)if(row->threaded&&!row->joined){if(!::pthread_join(row->thread,nullptr))row->joined=true;}
#endif
        }
    };

    inline error component_dispatch::invoke(caller who,operation action,handle cap,bytes input,std::size_t capacity,std::size_t& required,sink deliver,cancellation cancelled)noexcept
    {
        required=0;
        try
        {
            auto status=preflight(who,action,cap,input.size());if(status!=error::success)return status;
            robot_contract::request request;if(!deliver||!robot_contract::decode(action,input,{16384,8192,255,16},request))return error::invalid;
            std::shared_ptr<binding> row;status=select(who,action,cap,row);if(status!=error::success)return status;
            auto check=[&](){if(cancelled&&cancelled())return error::cancelled;std::shared_ptr<binding> selected;return select(who,action,cap,selected);};
            status=check();if(status!=error::success)return status;std::array<std::uint8_t,2048> output;wire::writer out{output};
            auto copy=[&](){required=out.size();if(capacity<required)return error::too_large;auto checked=check();if(checked!=error::success)return checked;
                if(!deliver(out.written())){return error::io;}return check();};
            if(action==operation::component_open)
            {
                required=8;if(capacity<required)return error::too_large;
                auto const& p=row->policy;
                if(!match(request.selected,p.selected)||request.configuration_identity!=p.configuration_identity.view()||request.configuration_generation!=p.configuration_generation||
                    request.configuration_digest.size()!=32||!std::equal(request.configuration_digest.begin(),request.configuration_digest.end(),p.configuration_digest.begin())||request.bindings.count!=p.role_count)return error::denied;
                lease primary;status=permissions_->preflight(who,cap,name(action),input.size()+8+p.configuration_bytes,primary);if(status!=error::success)return status;
                if(!primary.specification().max_resources)return error::again;
                {std::lock_guard lock{row->mutex};if(row->retired)return error::denied;if(row->published||row->opening||row->id)return error::again;
                    robot_contract::decoder in{request.bindings.encoded,{request.bindings.encoded.size(),0,0,0}};
                    for(std::size_t i{};i<p.role_count;++i){std::uint64_t slot{};if(!in.identity(slot)||slot!=i+1||!in.resource(row->roles[i].target))return error::invalid;}
                    if(!in.done()){return error::invalid;}row->opening=true;row->who=who;row->cap=cap;row->configuration_cap=request.configuration_capability;}
                struct rollback{binding& row;bool committed{};~rollback(){if(!committed){std::lock_guard lock{row.mutex};row.opening=false;row.cap=row.id=row.configuration_cap=0;row.roles={};row.who={};}}} pending{*row};
                group_lease held;status=group(*row,action,held);if(status!=error::success)return status;
                status=validate_targets(*row);if(status!=error::success)return status;
                std::array claims{admission_claim{cap,name(action),input.size()+8+p.configuration_bytes,false,false},
                    admission_claim{request.configuration_capability,"artifact.open",40,false,false},admission_claim{request.configuration_capability,"artifact.read",p.configuration_bytes,false,false}};
                group_lease charged;status=permissions_->admit_group(who,claims,charged,true);if(status!=error::success||!charged.consume(who,claims))return status==error::success?error::denied:status;
                handle artifact{};status=artifacts_.open(p.configuration_alias.view(),p.configuration_generation,p.configuration_digest,held.specification(1)->max_resources,artifact);if(status!=error::success)return status;
                struct release{artifacts::registry& store;std::string_view alias;handle id;~release(){store.abandon(alias,id);}} opened{artifacts_,p.configuration_alias.view(),artifact};
                artifacts::information info;status=artifacts_.describe(p.configuration_alias.view(),artifact,info);if(status!=error::success)return status;
                if(info.size!=p.configuration_bytes||info.transfer<p.configuration_bytes||info.purpose_code!=2)return error::denied;
                std::size_t received{};status=artifacts_.read(p.configuration_alias.view(),artifact,0,row->configuration.writable(),received);
                if(status!=error::success||received!=p.configuration_bytes)return status==error::success?error::io:status;
                auto id=resources::allocate_identity();if(!id)return error::too_large;out.u64(id);held.release();charged.release();primary.release();
                status=copy();if(status!=error::success)return status;
                status=group(*row,action,held);if(status!=error::success)return status;
                {std::lock_guard lock{row->mutex};if(row->retired)return error::cancelled;row->id=id;row->published=true;row->opening=false;pending.committed=true;}
                return error::success;
            }
            {std::lock_guard lock{row->mutex};if(!row->published||!same(who,row->who)||row->cap!=cap||row->id!=request.resource)return error::bad_handle;}
            if(action==operation::component_describe)
            {
                {std::lock_guard lock{row->mutex};if(!describe(*row,out))return error::io;}
                required=out.size();if(capacity<required)return error::too_large;
                lease charged;status=permissions_->admit(who,cap,name(action),input.size()+required,charged,true);if(status!=error::success)return status;
                charged.release();return copy();
            }
            required=256;if(capacity<required)return error::too_large;auto digest=buffers::hash(input);
            {std::lock_guard lock{row->mutex};auto key=action==operation::component_start?row->start_key:row->stop_key;
                if(request.key.sequence==key){if(digest!=(action==operation::component_start?row->start_digest:row->stop_digest))return error::conflict;
                    receipt(out,action==operation::component_start?row->started:row->stopped);}}
            if(out.size())return copy();
            auto time=now();if(request.deadline<=time)return error::timeout;
            auto maximum=action==operation::component_start?row->images.limits().startup_ns:row->policy.shutdown_ns;
            if(request.deadline-time>maximum)return error::invalid;
            if(action==operation::component_stop&&!match(request.selected,row->policy.stop))return error::denied;
            group_lease held;status=group(*row,action,held,action==operation::component_stop?0:input.size()+required,true);if(status!=error::success)return status;
            {std::lock_guard lock{row->mutex};auto& key=action==operation::component_start?row->start_key:row->stop_key;
                if(row->retired&&action==operation::component_start)return error::denied;
                if(key||request.key.sequence!=1)return error::conflict;
                auto id=resources::allocate_identity();if(!id)return error::too_large;
                if(action==operation::component_start)
                {
                    if(row->stop.load()||row->failed||row->exited||time>INT64_MAX-row->policy.lifetime_ns)return error::denied;
                    row->start_key=1;row->start_digest=digest;row->until=request.deadline;row->run_until=time+row->policy.lifetime_ns;
                    row->started={id,generation_,"queued","locally_accepted",""};row->queued=true;receipt(out,row->started);
                }
                else
                {
                    row->stop_key=1;row->stop_digest=digest;row->stop_until=request.deadline;row->stop.store(true);
                    row->stopped={id,generation_,row->cleanup_unknown?"outcome_unknown":row->exited||(!row->launched&&row->failed)?"succeeded":"queued",
                        row->cleanup_unknown?"unknown":row->exited?"observed_remote":!row->launched&&row->failed?"committed":"locally_accepted",
                        row->cleanup_unknown?"exit.unconfirmed":""};
#if defined(__linux__)
                    row->child.request_stop();
#endif
                    receipt(out,row->stopped);
                }}
            held.release();return copy();
        }
        catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
    }
    inline error component_dispatch::close(caller who,handle cap,handle id)noexcept
    {
        if(!same_process())return error::denied;
        try
        {
            lease held;auto status=permissions_->admit(who,cap,"resource.close",0,held,false);if(status!=error::success)return status;
            std::shared_ptr<binding> row;{std::lock_guard lock{configuration_};row=find(held.specification().alias);}if(!row)return error::denied;
            std::lock_guard lock{row->mutex};if(!row->published||row->id!=id||row->cap!=cap||!same(row->who,who))return error::bad_handle;
            row->published=false;row->retired=true;row->closing.store(true);
#if defined(__linux__)
            row->child.request_stop();
#endif
            return error::success;
        }
        catch(...){return error::io;}
    }
}
