/** @file
 * @brief Auditable guest capture -> ordinary model job -> authenticated proposal publication.
 * @details capture and publish have independent actions, strict per-capability
 * keys and finite retained receipts. Capture returns actual buffer references;
 * the guest supplies these to the unchanged model.submit interface. After a
 * successful model.take, publish supplies the complete terminal envelope. The
 * inner dispatcher authenticates the original caller, input order/allocation
 * identities/digests, complete final bytes and confirmed model cleanup.
 *
 * No guest field creates source provenance. Neither operation opens a source,
 * model, target or capability. Capture close releases only its own references;
 * completed model jobs and published proposals retain their independent pins.
 * Failed output copies recover through exact keys. Changed/old keys conflict;
 * failed admission consumes no key. Optional admission budgets are separate from
 * fixed session/capture/workspace capacity, and every omitted capacity is zero.
 */
#pragma once
#include "proposal_dispatch.h"
namespace uwvm2::imported::ros::scene::proposals
{
    struct guest_limits{std::uint64_t sessions{},captures{},retained_bytes{};};
    class guest_configuration
    {
        guest_limits value_;unsigned assigned_{};bool frozen_{};
    public:
        inline static constexpr std::array<std::string_view,3> names{"producer.sessions","capture.handles","retained.bytes"};
        error limit(std::string_view name,std::uint64_t value)noexcept
        {
            if(frozen_)return error::conflict;
            auto found=std::find(names.begin(),names.end(),name);if(found==names.end())return error::invalid;
            auto index=static_cast<unsigned>(found-names.begin()),bit=1u<<index;if(assigned_&bit)return error::conflict;
            if(value>(index==0?32u:index==1?1024u:static_cast<std::uint64_t>(INT64_MAX)))return error::too_large;
            if(index==0)value_.sessions=value;else if(index==1)value_.captures=value;else value_.retained_bytes=value;
            assigned_|=bit;return error::success;
        }
        error freeze(guest_limits& output)noexcept
        {if(frozen_)return error::conflict;if(assigned_!=7)return error::invalid;output=value_;frozen_=true;return error::success;}
    };
}
namespace uwvm2::imported::ros::scene
{
    class proposal_guest_dispatch
    {
    public:
        using operation=proposal_contract::operation;
        using bytes=wire::bytes;using sink=proposal_dispatch::sink;using cancellation=proposal_dispatch::cancellation;
        using workspace_lease=proposal_dispatch::workspace_lease;
        struct usage{std::uint64_t sessions{},captures{},borrowed_captures{},retained_bytes{};bool retired{};};
    private:
        struct binding{buffers::text<255> alias;proposals::plan policy;bool retired{};};
        struct session
        {
            handle capability{};caller who;binding* scope{};bool busy{};
            handle capture_key{},publish_key{};buffers::digest capture_hash{},publish_hash{};
            proposal_contract::capture_receipt captured;handle published{};
        };
        struct captured
        {
            handle id{};caller who;binding* scope{};std::uint64_t expiry{};std::size_t borrowed{};
            bool published{},closed{};proposal_dispatch::captured_inputs inputs;
        };
        std::shared_ptr<authority> permissions_;
        proposal_dispatch inner_;
        mutable std::mutex mutex_;
        std::array<binding,32> bindings_{};std::size_t count_{};
        proposals::guest_limits limits_;
        ::uwvm2::utils::container::std_unique_ptr<session[]> sessions_;
        ::uwvm2::utils::container::std_unique_ptr<captured[]> captures_;
        bool configured_{},ready_{},retired_{};
        std::uint64_t(*clock_)()noexcept;
#if defined(__linux__)
        ::pid_t process_{::getpid()};
#endif
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return process_==::getpid();
#else
            return true;
#endif
        }
        static bool same(caller a,caller b)noexcept{return !(a<b)&&!(b<a);}
        binding* find(std::string_view alias)noexcept
        {for(std::size_t i{};i<count_;++i)if(bindings_[i].alias.view()==alias)return &bindings_[i];return nullptr;}
        bool live(binding const& value)const noexcept
        {std::lock_guard lock{mutex_};return ready_&&!retired_&&!value.retired;}
        error primary(caller who,handle cap,operation action,std::uint64_t extent,lease& held,binding*& scope,bool charge,bool preflight=false)
        {
            if(!permissions_||!same_process())return error::denied;
            if(action!=operation::capture&&action!=operation::publish)return error::unsupported;
            auto name=proposal_contract::names[static_cast<std::size_t>(action)];
            auto result=preflight?permissions_->preflight(who,cap,name,extent,held):permissions_->admit(who,cap,name,extent,held,charge);
            if(result!=error::success)return result;
            auto const& grant=held.specification();if(grant.kind!=purpose::proposal||grant.binding!="model-proposals")return error::denied;
            std::lock_guard lock{mutex_};scope=find(grant.alias);return !ready_||retired_||!scope||scope->retired?error::denied:error::success;
        }
        error time_bound(binding const& scope,std::uint64_t expiry,std::uint64_t now)const noexcept
        {return !now||now>INT64_MAX||expiry<=now||expiry>INT64_MAX||expiry-now>scope.policy.storage.max_lifetime_ns?error::timeout:error::success;}
        error enter(caller who,handle cap,binding& scope,handle key,operation action,buffers::digest const& hash,session*& output,bool& replay)
        {
            std::lock_guard lock{mutex_};if(retired_||scope.retired)return error::cancelled;
            session* vacant{};for(std::size_t i{};i<limits_.sessions;++i)
            {
                auto& value=sessions_[i];if(!value.capability){if(!vacant)vacant=&value;continue;}
                if(value.capability!=cap)continue;
                if(!same(value.who,who)||value.scope!=&scope)return error::denied;
                output=&value;break;
            }
            if(!output){if(!vacant)return error::again;if(key!=1)return error::conflict;*vacant={};vacant->capability=cap;vacant->who=who;vacant->scope=&scope;output=vacant;}
            if(output->busy)return error::again;
            auto prior=action==operation::capture?output->capture_key:output->publish_key;
            auto const& digest=action==operation::capture?output->capture_hash:output->publish_hash;
            if(key==prior&&key){if(hash!=digest)return error::conflict;replay=true;}
            else if(!key||prior==UINT64_MAX||key!=prior+1)return error::conflict;
            output->busy=true;return error::success;
        }
        struct leave
        {proposal_guest_dispatch& self;session& value;~leave(){std::lock_guard lock{self.mutex_};value.busy=false;if(!value.capture_key&&!value.publish_key)value={};}};
        struct borrowed_capture
        {
            proposal_guest_dispatch& self;captured* value{};proposal_dispatch::captured_inputs inputs;
            explicit borrowed_capture(proposal_guest_dispatch& owner)noexcept:self(owner){}
            ~borrowed_capture()
            {
                inputs.reset();if(!value)return;std::lock_guard lock{self.mutex_};--value->borrowed;
                if(!value->borrowed&&(value->closed||!value->published))*value={};
            }
        };
        error borrow(caller who,binding& scope,handle id,borrowed_capture& output)
        {
            std::lock_guard lock{mutex_};if(retired_||scope.retired)return error::cancelled;
            for(std::size_t index{};index<limits_.captures;++index)
            {
                auto& row=captures_[index];if(row.id!=id||!id)continue;
                if(row.closed||!row.published||row.scope!=&scope||!same(row.who,who))return error::bad_handle;
                ++row.borrowed;output.value=&row;output.inputs=row.inputs;return error::success;
            }
            return error::bad_handle;
        }
        bool capture_live(borrowed_capture const& value)const noexcept
        {std::lock_guard lock{mutex_};return value.value&&value.value->published&&!value.value->closed&&!retired_&&!value.value->scope->retired;}
        error reserve_capture(caller who,binding& scope,std::uint64_t expiry,std::uint64_t grant_capacity,borrowed_capture& output)
        {
            std::lock_guard lock{mutex_};if(retired_||scope.retired)return error::cancelled;
            captured* vacant{};std::uint64_t owned{};
            for(std::size_t index{};index<limits_.captures;++index)
            {auto& row=captures_[index];if(row.id&&row.scope==&scope)++owned;else if(!row.id&&!vacant)vacant=&row;}
            if(!vacant||owned>=scope.policy.captures||owned>=grant_capacity)return error::again;
            auto id=resources::allocate_identity();if(!id)return error::too_large;
            vacant->id=id;vacant->scope=&scope;vacant->who=who;vacant->expiry=expiry;vacant->borrowed=1;output.value=vacant;return error::success;
        }
        /** @brief Copy only after the last reentrant callback and complete retained proof check. */
        template<typename Validate>error emit(caller who,handle cap,operation action,binding& scope,bytes output,sink deliver,cancellation cancelled,Validate&& validate)
        {
            if(cancelled&&cancelled())return error::cancelled;
            auto status=validate();if(status!=error::success)return status;
            lease held;binding* current{};status=primary(who,cap,action,0,held,current,false);if(status!=error::success)return status;
            if(current!=&scope)return error::denied;
            if(!deliver(output)||(cancelled&&cancelled()))return error::io;
            status=validate();if(status!=error::success)return status;
            return primary(who,cap,action,0,held,current,false);
        }
    public:
        proposal_guest_dispatch(std::shared_ptr<authority> permissions,model_dispatch& models,buffers::registry& buffers,handle environment,handle generation,
                                proposal_dispatch::limits limits,std::uint64_t(*clock)()noexcept=proposals::admission::steady_now)
            :permissions_(permissions),inner_(std::move(permissions),models,buffers,environment,generation,limits,clock),clock_(clock?clock:proposals::admission::steady_now){}
        ~proposal_guest_dispatch(){retire();}
        static bool required_storage(proposals::guest_limits const& limits,std::uint64_t& bytes)noexcept
        {
            if(limits.sessions>32||limits.captures>1024)return false;
            bytes=(limits.sessions?limits.sessions*sizeof(session)+::uwvm2::utils::allocator::fast_io_strict::array_prefix<session>:0)+
                (limits.captures?limits.captures*sizeof(captured)+::uwvm2::utils::allocator::fast_io_strict::array_prefix<captured>:0);return true;
        }
        /** @brief Configure exact producer/capture tables; underlying proposal workspace is separate. */
        error configure(proposals::guest_limits limits)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(configured_||ready_||retired_)return error::denied;
            std::uint64_t required{};if(!required_storage(limits,required)||limits.retained_bytes>INT64_MAX)return error::invalid;
            if(required>limits.retained_bytes)return error::too_large;
            limits_=limits;configured_=true;return error::success;
        }
        error bind(::uwvm2::utils::container::std_string alias,proposals::plan const& policy)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(ready_||retired_)return error::denied;if(count_==bindings_.size())return error::too_large;
            if(find(alias))return error::conflict;
            buffers::text<255> retained;if(!retained.assign(alias))return error::invalid;
            // Move the already-owned argument into the recoverable inner bind;
            // copying a long alias here could throw before its error boundary.
            auto status=inner_.bind(std::move(alias),policy);if(status!=error::success)return status;
            auto& row=bindings_[count_++];row.alias=retained;row.policy=policy;return error::success;
        }
        error bind_arbiter(::uwvm2::utils::container::std_string alias,proposals::arbiter_plan const& policy)
        {return inner_.bind_arbiter(std::move(alias),policy);}
        error prepare(proposal_dispatch::source_access sources,proposal_dispatch::target_access targets)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(ready_||retired_)return error::denied;
            try
            {
                auto sessions=limits_.sessions?::uwvm2::utils::container::make_unique<session[]>(static_cast<std::size_t>(limits_.sessions)):
                    ::uwvm2::utils::container::std_unique_ptr<session[]>{};
                auto captures=limits_.captures?::uwvm2::utils::container::make_unique<captured[]>(static_cast<std::size_t>(limits_.captures)):
                    ::uwvm2::utils::container::std_unique_ptr<captured[]>{};
                auto status=inner_.prepare(sources,targets);if(status!=error::success)return status;
                sessions_=std::move(sessions);captures_=std::move(captures);ready_=true;return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}
        }
        error preflight(caller who,operation action,handle cap,std::size_t extent)
        {
            if(action==operation::read||action==operation::accept)return inner_.preflight(who,action,cap,extent);
            if(extent>proposal_contract::maximum_request_bytes)return error::too_large;
            lease held;binding* scope{};return primary(who,cap,action,extent,held,scope,false,true);
        }
        error reserve_workspace(caller who,operation action,handle cap,std::size_t extent,workspace_lease& output)
        {auto status=preflight(who,action,cap,extent);return status==error::success?inner_.reserve_workspace(who,action,cap,extent,output):status;}
        /** @brief Complete canonical guest request; caller has copied and bounded input already. */
        error invoke(caller who,operation action,handle cap,bytes input,std::size_t capacity,std::size_t& required,sink deliver,cancellation cancelled={})
        {
            if(action==operation::read||action==operation::accept)return inner_.invoke(who,action,cap,input,capacity,required,deliver,cancelled);
            required=0;auto status=preflight(who,action,cap,input.size());if(status!=error::success)return status;
            if(!deliver||capacity>proposals::maximum_envelope_bytes)return error::invalid;
            try
            {
                proposal_contract::capture_request capture;proposal_contract::publish_request publication;handle key{};
                if(action==operation::capture)
                {if(!proposal_contract::decode_capture(input,capture)||capture.key.session!=cap)return error::invalid;key=capture.key.sequence;required=16+capture.count*16;}
                else
                {if(!proposal_contract::decode_publish(input,publication)||publication.key.session!=cap)return error::invalid;key=publication.key.sequence;required=8;}
                if(capacity<required)return error::too_large;
                lease primary_held;binding* scope{};status=primary(who,cap,action,input.size()+required,primary_held,scope,false,true);if(status!=error::success)return status;
                if(action==operation::capture&&(capture.selected!=scope->policy.selected||capture.count>scope->policy.storage.observations))return error::denied;
                auto hash=buffers::hash(input);session* producer{};bool replay{};
                status=enter(who,cap,*scope,key,action,hash,producer,replay);if(status!=error::success)return status;leave done{*this,*producer};
                if(cancelled&&cancelled())return error::cancelled;
                if(action==operation::capture)
                {
                    borrowed_capture selected{*this};proposal_contract::capture_receipt receipt;
                    if(replay)
                    {receipt=producer->captured;status=borrow(who,*scope,receipt.capture,selected);if(status!=error::success)return status;}
                    else
                    {
                        auto now=clock_();status=time_bound(*scope,capture.expiry,now);if(status!=error::success)return status;
                        std::array<proposal_dispatch::source_reference,8> references{};
                        for(std::size_t index{};index<capture.count;++index)
                        {auto const& ref=capture.sources[index];references[index]={static_cast<std::size_t>(ref.index),ref.capability,ref.resource,ref.buffer_capability};}
                        status=inner_.preflight_capture(who,scope->alias.view(),{references.data(),capture.count});if(status!=error::success)return status;
                        lease checked;binding* again{};status=primary(who,cap,action,input.size()+required,checked,again,false,true);if(status!=error::success)return status;
                        status=reserve_capture(who,*scope,capture.expiry,checked.specification().max_resources,selected);if(status!=error::success)return status;
                        lease charged;status=primary(who,cap,action,input.size()+required,charged,again,true);if(status!=error::success)return status;
                        status=inner_.capture(who,scope->alias.view(),{references.data(),capture.count},selected.inputs);if(status!=error::success)return status;
                        if(cancelled&&cancelled())return error::cancelled;
                        now=clock_();status=time_bound(*scope,capture.expiry,now);if(status!=error::success)return status;
                        status=inner_.validate_capture(who,scope->alias.view(),selected.inputs,proposals::admission::observation_policy::latest_required);if(status!=error::success)return status;
                        status=primary(who,cap,action,0,checked,again,false);if(status!=error::success)return status;
                        receipt.capture=selected.value->id;receipt.count=selected.inputs.size();
                        for(std::size_t index{};index<receipt.count;++index)receipt.inputs[index]={selected.inputs.capability(index),selected.inputs.buffer(index)};
                        {
                            std::lock_guard lock{mutex_};if(retired_||scope->retired||selected.value->closed)return error::cancelled;
                            selected.value->inputs=selected.inputs;selected.value->published=true;producer->capture_key=key;producer->capture_hash=hash;producer->captured=receipt;
                        }
                    }
                    auto validate=[&]
                    {
                        auto now=clock_();if(!capture_live(selected))return error::cancelled;
                        if(!now||now>=selected.value->expiry)return error::timeout;
                        auto valid=inner_.validate_capture(who,scope->alias.view(),selected.inputs);
                        return valid==error::success&&!capture_live(selected)?error::cancelled:valid;
                    };
                    status=validate();if(status!=error::success)return status;
                    if(replay){lease charged;binding* again{};status=primary(who,cap,action,input.size()+required,charged,again,true);if(status!=error::success)return status;}
                    std::array<std::uint8_t,144> output;wire::writer writer{output};if(!proposal_contract::encode_capture(writer,receipt)||writer.size()!=required)return error::io;
                    return emit(who,cap,action,*scope,writer.written(),deliver,cancelled,validate);
                }
                handle proposal{};
                if(replay)
                {proposal=producer->published;lease charged;binding* again{};status=primary(who,cap,action,input.size()+required,charged,again,true);if(status!=error::success)return status;}
                else
                {
                    borrowed_capture selected{*this};status=borrow(who,*scope,publication.capture,selected);if(status!=error::success)return status;
                    auto now=clock_();if(!now||now>=selected.value->expiry)return error::timeout;
                    status=time_bound(*scope,publication.expiry,now);if(status!=error::success)return status;
                    status=inner_.validate_capture(who,scope->alias.view(),selected.inputs);if(status!=error::success)return status;
                    proposals::envelope envelope;if(!proposals::decode_envelope(publication.envelope,envelope))return error::invalid;
                    auto mode=proposals::admission::observation_policy::unconfigured;
                    for(std::size_t index{};index<scope->policy.tool_count;++index)if(proposals::matches(envelope,scope->policy.tools[index]))mode=scope->policy.tools[index].observations;
                    if(mode==proposals::admission::observation_policy::unconfigured)return error::denied;
                    bool charged{};lease charged_held;error refusal=error::success;
                    auto final_admission=[&]
                    {
                        auto current=clock_();if(!capture_live(selected))return error::cancelled;
                        if(!current||current>=selected.value->expiry)return error::timeout;
                        auto valid=time_bound(*scope,publication.expiry,current);if(valid!=error::success)return valid;
                        valid=inner_.validate_capture(who,scope->alias.view(),selected.inputs,mode);if(valid!=error::success)return valid;
                        if(!capture_live(selected))return error::cancelled;
                        binding* again{};valid=primary(who,cap,action,charged?0:input.size()+required,charged_held,again,!charged);
                        if(valid==error::success)charged=true;
                        return valid;
                    };
                    auto cancellation_check=[&]{if(cancelled&&cancelled()){refusal=error::cancelled;return true;}return false;};
                    status=inner_.publish(selected.inputs,publication.model_request,publication.envelope,publication.expiry,proposal,final_admission,cancellation_check);
                    if(status!=error::success)return refusal!=error::success?refusal:status;
                    // No callback occurs between inner publication and receipt
                    // attachment. The retained outer authority lock fences revoke.
                    std::lock_guard lock{mutex_};producer->publish_key=key;producer->publish_hash=hash;producer->published=proposal;
                }
                std::array<std::uint8_t,8> output;wire::writer writer{output};writer.u64(proposal);
                return emit(who,cap,action,*scope,writer.written(),deliver,cancelled,[&]{return live(*scope)?error::success:error::cancelled;});
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        /** @brief Close a capture independently; published proposals retain their own pins. */
        error close(caller who,handle cap,handle resource)
        {
            if(!same_process()||!permissions_)return error::denied;
            lease allowed;auto status=permissions_->admit(who,cap,"resource.close",0,allowed,false);if(status!=error::success)return status;
            auto const& grant=allowed.specification();if(grant.kind!=purpose::proposal||grant.binding!="model-proposals")return error::denied;
            {
                std::lock_guard lock{mutex_};auto scope=find(grant.alias);if(!ready_||!scope||retired_||scope->retired)return error::denied;
                for(std::size_t index{};index<limits_.captures;++index)
                {
                    auto& value=captures_[index];if(value.id!=resource||!resource)continue;
                    if(value.scope!=scope||!same(value.who,who)||value.closed||!value.published)return error::bad_handle;
                    lease charged;status=permissions_->admit(who,cap,"resource.close",16,charged);if(status!=error::success)return status;
                    value.closed=true;if(!value.borrowed)value={};return error::success;
                }
            }
            return inner_.close(who,cap,resource);
        }
        error request(caller who,handle cap,handle id,bool release,std::size_t capacity,std::size_t& required,sink deliver,cancellation cancelled={})
        {return inner_.request(who,cap,id,release,capacity,required,deliver,cancelled);}
        error inspect(std::string_view alias,proposal_dispatch::usage& output)const noexcept{return inner_.inspect(alias,output);}
        error inspect_guests(usage& output)const noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex_,std::try_to_lock};if(!lock.owns_lock())return error::again;
            usage value;required_storage(limits_,value.retained_bytes);value.retired=retired_;
            for(std::size_t index{};index<limits_.sessions;++index)if(sessions_&&sessions_[index].capability)++value.sessions;
            for(std::size_t index{};index<limits_.captures;++index)if(captures_&&captures_[index].id){++value.captures;value.borrowed_captures+=captures_[index].borrowed;}
            output=value;return error::success;
        }
        void revoke(std::string_view alias)noexcept
        {
            if(!same_process())return;
            {std::lock_guard lock{mutex_};if(auto scope=find(alias)){scope->retired=true;
                for(std::size_t index{};index<limits_.captures;++index)if(captures_&&captures_[index].scope==scope)
                {auto& value=captures_[index];value.closed=true;if(!value.borrowed)value={};}}}
            inner_.revoke(alias);
        }
        void retire()noexcept
        {
            if(!same_process())return;
            {std::lock_guard lock{mutex_};if(retired_)return;retired_=true;
                for(std::size_t index{};index<limits_.captures;++index)if(captures_){auto& value=captures_[index];value.closed=true;if(!value.borrowed)value={};}}
            inner_.retire();
        }
    };
}
