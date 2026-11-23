/** @file
 * @brief Explicit-source readiness streams with retained capacity reservations.
 * @details An event grant never grants its sources. Every open/poll holds the
 * primary and each resource.observe grant as one group, then the trusted resolver
 * validates each provider's complete ancestor chain. Guest source numbers remain
 * immutable one-based positions, never native addresses or peer identities.
 * Requested bytes limit encoded deliveries, items bound retained records, and
 * workspace reserves collection capacity until close. Fixed stream/control-block
 * metadata is bounded separately by max_streams; process RSS remains deployment
 * controlled. Closing a stream never closes its source. Failed copies preserve
 * sequence positions; revocation cannot be bypassed by replaying a staged batch.
 */
#pragma once
#include <utility>
#include "communication_dispatch.h"
#include "communication_workspace.h"
#include "event_contract.h"
namespace uwvm2::imported::ros::scene
{
    class communication_event_dispatch
    {
        using op=event_contract::operation;
        using owner=communication::owner;
    public:
        struct plan
        {events::plan policy;std::size_t max_streams{};std::uint64_t max_bytes{},max_workspace{};};
        struct limits
        {std::size_t bindings{},carrier_bytes{},output_bytes{};std::uint64_t workspace_bytes{},workspace_calls{};};
        /** @brief Owned function pointers into a trusted resolver with dispatcher lifetime.
         * @details No borrowed function_ref is stored. Resolver implementations
         * select providers by trusted grant kind, never numeric resource guesses.
         */
        struct source_access
        {
            void* context{};
            error(*resolve)(void*,caller,handle,handle,events::binding&){};
            error(*validate)(void*,caller,handle,events::binding const&){};
            communication::error(*observe)(void*,caller,handle,events::binding const&,events::snapshot&){};
            template<class T> static source_access from(T& value) noexcept
            {return {&value,[](void* p,caller who,handle cap,handle resource,events::binding& out){return static_cast<T*>(p)->event_source(who,cap,resource,out);},
                [](void* p,caller who,handle cap,events::binding const& source){return static_cast<T*>(p)->validate_source(who,cap,source);},
                [](void* p,caller who,handle cap,events::binding const& source,events::snapshot& out)->communication::error
                {
                    auto& resolver=*static_cast<T*>(p);
                    if constexpr(requires{resolver.observe_source(who,cap,source,out);})return resolver.observe_source(who,cap,source,out);
                    else
                    {
                        auto status=resolver.validate_source(who,cap,source);
                        return status==error::success?source.provider->inspect(source.origin,source.resource,source.generation,out):static_cast<communication::error>(status);
                    }
                }};}
        };
        struct usage
        {std::uint64_t configured{},streams{},workspace_bytes{},workspace_calls{};bool ready{},retired{};};
        using sink=communication_dispatch::sink;
        using workspace_lease=communication_workspace::lease;
    private:
        struct retained_account
        {
            std::atomic<std::uint64_t> count{};
#if defined(__linux__)
            ::pid_t process{::getpid()};
#endif
        };
        /** @brief Keep a stream metadata slot until its final borrower retires.
         * @details Public-handle removal is not storage retirement. A close or
         * poll callback can still own this record, so creation counts this lease
         * even after close detaches the public slot. It is declared before all
         * owned backing and therefore releases after that backing is destroyed.
         */
        struct retained_lease
        {
            std::shared_ptr<retained_account> account;
            retained_lease()=default;retained_lease(retained_lease const&)=delete;retained_lease& operator=(retained_lease const&)=delete;
            retained_lease(retained_lease&& other) noexcept:account(std::move(other.account)){}
            retained_lease& operator=(retained_lease&& other) noexcept
            {if(this!=&other){reset();account=std::move(other.account);}return *this;}
            ~retained_lease(){reset();}
            void reset() noexcept
            {
#if defined(__linux__)
                if(account&&account->process!=::getpid()){std::construct_at(std::addressof(account));return;}
#endif
                if(account)account->count.fetch_sub(1,std::memory_order_release);
                account.reset();
            }
            error acquire(std::shared_ptr<retained_account> const& selected,std::uint64_t maximum) noexcept
            {
                if(account||!selected)return error::conflict;
                auto count=selected->count.load(std::memory_order_acquire);
                do{if(count>=maximum)return error::too_large;}while(!selected->count.compare_exchange_weak(count,count+1,std::memory_order_acq_rel));
                account=selected;return error::success;
            }
        };
        struct record
        {
            retained_lease retained;
            workspace_lease reservation;
            handle id{};
            std::array<handle,64> capabilities{};
            std::array<events::binding,64> sources{};
            std::size_t count{},items{};
            std::uint64_t bytes{};
            std::shared_ptr<events::stream> stream;
            bool published{},closed{};
        };
        struct binding
        {::uwvm2::utils::container::std_string alias;plan policy;::uwvm2::utils::container::std_vector<std::shared_ptr<record>> streams;::uwvm2::utils::container::std_vector<handle> closed_receipts;std::shared_ptr<retained_account> retained;bool opening{},revoked{};};
        std::shared_ptr<authority> permissions_;
        std::uint64_t environment_{},generation_{};
        limits limits_;
        source_access sources_;
        communication_workspace workspace_;
        mutable std::mutex mutex_;
        ::uwvm2::utils::container::std_vector<binding> bindings_;
        bool ready_{},retired_{};
#if defined(__linux__)
        ::pid_t process_{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process_==::getpid();
#else
            return true;
#endif
        }
        binding* named(std::string_view alias) noexcept
        {for(auto& row:bindings_)if(row.alias==alias)return &row;return nullptr;}
        error resolve(grant const& grant,binding*& out)
        {
            std::lock_guard lock{mutex_};auto* row=named(grant.alias);
            if(!ready_||retired_||!row||row->revoked||grant.kind!=purpose::event||grant.binding!="event-stream")return error::denied;
            out=row;return error::success;
        }
        error active(binding& row,handle id,std::shared_ptr<record>& out)
        {
            std::lock_guard lock{mutex_};if(retired_||row.revoked)return error::cancelled;
            for(auto const& entry:row.streams)if(entry&&entry->id==id&&entry->published&&!entry->closed){out=entry;return error::success;}
            return error::bad_handle;
        }
        error group(caller who,handle cap,std::string_view action,record const* origin,std::uint64_t bytes,group_lease& lease,bool charge)
        {
            std::array<admission_claim,65> claims{};claims[0]={cap,action,bytes,false,!charge};std::size_t count=1;
            if(origin)for(std::size_t i{};i<origin->count;++i)claims[count++]={origin->capabilities[i],"resource.observe",0,true,true};
            return permissions_->admit_group(who,{claims.data(),count},lease,charge);
        }
        struct verification
        {communication_event_dispatch* self;caller who;binding* selected;record* origin;error failure{error::success};};
        static bool check(void* argument,owner scope,handle resource) noexcept
        {
            auto& v=*static_cast<verification*>(argument);
            try
            {
                {std::lock_guard lock{v.self->mutex_};if(v.self->retired_||v.selected->revoked||v.origin->closed){v.failure=error::cancelled;return false;}}
                for(std::size_t i{};i<v.origin->count;++i)
                {
                    auto const& source=v.origin->sources[i];if(source.origin!=scope||source.resource!=resource)continue;
                    lease admitted;v.failure=v.self->permissions_->dependency(v.who,v.origin->capabilities[i],"resource.observe",admitted);
                    if(v.failure==error::success)v.failure=v.self->sources_.validate(v.self->sources_.context,v.who,v.origin->capabilities[i],source);
                    return v.failure==error::success;
                }
                v.failure=error::denied;return false;
            }
            catch(std::bad_alloc const&){v.failure=error::no_memory;return false;}catch(...){v.failure=error::io;return false;}
        }
        error validate(verification& value)
        {for(std::size_t i{};i<value.origin->count;++i){auto const& source=value.origin->sources[i];if(!check(&value,source.origin,source.resource))return value.failure;}return error::success;}
        /** @brief Carry the current poll caller through the exact origin resolver.
         * @details The stream never stores this callback argument. Graph sources
         * mint fresh complete tokens here; local topic/action queues read cached
         * state. No opener capability is reused as a later caller's authority.
         */
        static communication::error observe(void* argument,events::binding const& selected,events::snapshot& out) noexcept
        {
            auto& v=*static_cast<verification*>(argument);
            try
            {
                if(!check(argument,selected.origin,selected.resource))return static_cast<communication::error>(v.failure);
                for(std::size_t i{};i<v.origin->count;++i)
                {
                    auto const& source=v.origin->sources[i];
                    if(source.origin!=selected.origin||source.resource!=selected.resource||source.generation!=selected.generation||source.provider!=selected.provider)continue;
                    auto& access=v.self->sources_;
                    auto status=access.observe?access.observe(access.context,v.who,v.origin->capabilities[i],source,out):
                        source.provider->inspect(source.origin,source.resource,source.generation,out);
                    v.failure=communication_dispatch::convert(status);return status;
                }
                v.failure=error::denied;return communication::error::denied;
            }
            catch(std::bad_alloc const&){v.failure=error::no_memory;return communication::error::no_memory;}
            catch(...){v.failure=error::io;return communication::error::io;}
        }
        void discard(binding& row,std::shared_ptr<record> const& origin) noexcept
        {
            {std::lock_guard lock{mutex_};origin->closed=true;origin->published=false;for(auto& entry:row.streams)if(entry==origin){entry.reset();break;}}
            if(origin->stream)origin->stream->retire();
        }
    public:
        communication_event_dispatch(std::shared_ptr<authority> permissions,std::uint64_t environment,std::uint64_t generation,limits capacity):
            permissions_(std::move(permissions)),environment_(environment),generation_(generation),limits_(capacity)
        {bindings_.reserve(capacity.bindings);}
        ~communication_event_dispatch(){if(!same_process()){std::construct_at(std::addressof(bindings_));return;}retire();}
        error bind(::uwvm2::utils::container::std_string alias,plan value)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(ready_||retired_||!permissions_||!environment_||!generation_)return error::denied;
            auto const& p=value.policy;
            if(!identifier(alias)||!communication::valid(p.selected)||!value.max_streams||!std::in_range<std::int64_t>(value.max_streams)||!p.max_sources||p.max_sources>64||
                !p.max_events||!std::in_range<std::int64_t>(p.max_events)||p.max_sources>p.max_events||!p.fields||(p.fields&~std::uint64_t{3})||!p.max_deadline_ns||p.max_deadline_ns>INT64_MAX||
                !value.max_bytes||value.max_bytes>limits_.output_bytes||!value.max_workspace||value.max_workspace>limits_.workspace_bytes)return error::invalid;
            if(named(alias))return error::conflict;
            if(bindings_.size()>=limits_.bindings)return error::too_large;
            auto id=resources::allocate_identity();if(!id)return error::too_large;value.policy.authority={environment_,id,generation_};
            binding row;row.alias=std::move(alias);row.policy=std::move(value);row.streams.resize(row.policy.max_streams);row.closed_receipts.resize(row.policy.max_streams);row.retained=::uwvm2::utils::container::make_shared<retained_account>();bindings_.push_back(std::move(row));return error::success;
        }
        error prepare(source_access provider)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(ready_||retired_||!provider.resolve||!provider.validate)return error::denied;
            if(!limits_.carrier_bytes||!limits_.output_bytes)return error::invalid;
            auto status=workspace_.configure(limits_.workspace_bytes,limits_.workspace_calls);if(status!=error::success)return status;
            sources_=provider;ready_=true;return error::success;
        }
        error preflight(caller who,op action,handle cap,std::size_t extent)
        {
            if(!same_process()||!permissions_)return error::denied;
            auto index=static_cast<std::size_t>(action);if(index>=event_contract::operations.size())return error::unsupported;
            if(extent>limits_.carrier_bytes)return error::too_large;
            lease granted;auto status=permissions_->preflight(who,cap,event_contract::operations[index].name,extent,granted);if(status!=error::success)return status;
            binding* row{};return resolve(granted.specification(),row);
        }
        error reserve_workspace(caller who,op action,handle cap,std::size_t extent,workspace_lease& out)
        {auto status=preflight(who,action,cap,extent);return status==error::success?workspace_.reserve(extent,out):status;}
        error invoke(caller who,op action,handle cap,wire::bytes input,std::size_t capacity,std::size_t& required,sink deliver,
            ::uwvm2::utils::container::function_ref<bool()> cancelled={})
        {
            required=0;auto status=preflight(who,action,cap,input.size());if(status!=error::success)return status;
            if(!deliver||capacity>limits_.output_bytes)return error::invalid;
            event_contract::request request;if(!event_contract::decode(action,input,request))return error::invalid;
            auto name=event_contract::operations[static_cast<std::size_t>(action)].name;
            lease allowed;status=permissions_->admit(who,cap,name,0,allowed,false);if(status!=error::success)return status;
            binding* row{};status=resolve(allowed.specification(),row);if(status!=error::success)return status;
            record* current_origin{};
            auto checkpoint=[&]() -> error
            {
                if(cancelled&&cancelled())return error::cancelled;
                group_lease fresh;auto check=group(who,cap,name,current_origin,0,fresh,false);if(check!=error::success)return check;
                if(current_origin){verification verified{this,who,row,current_origin};return validate(verified);}
                std::lock_guard lock{mutex_};return retired_||row->revoked?error::cancelled:error::success;
            };
            auto emit=[&](wire::bytes value){try{return checkpoint()==error::success&&deliver(value)&&(!cancelled||!cancelled());}catch(...){return false;}};
            try
            {
                status=checkpoint();if(status!=error::success)return status;
                if(action==op::open)
                {
                    required=8;if(capacity<required)return error::too_large;
                    if(!event_contract::matches(request,row->policy.policy.selected))return error::denied;
                    if(!request.bytes||request.bytes>row->policy.max_bytes||request.items<request.source_count||request.items>row->policy.policy.max_events||
                        !request.workspace||request.workspace>row->policy.max_workspace||request.source_count>row->policy.policy.max_sources)return error::too_large;
                    events::event sample{1,1,1,0,row->policy.policy.fields,false,false};auto minimum_output=wire::writer::measuring(limits_.output_bytes);
                    if(!event_contract::encode(minimum_output,{&sample,1})||request.bytes<minimum_output.size())return error::too_large;
                    std::uint64_t minimum{};if(!events::stream::required_workspace(request.source_count,request.items,minimum)||request.workspace<minimum)return error::too_large;
                    std::size_t slot{};
                    {
                        std::lock_guard lock{mutex_};if(retired_||row->revoked)return error::cancelled;if(row->opening)return error::again;
                        std::size_t occupied{};slot=row->streams.size();for(std::size_t i{};i<row->streams.size();++i){if(row->streams[i])++occupied;else if(slot==row->streams.size())slot=i;}
                        if(occupied>=allowed.specification().max_resources||slot==row->streams.size())return error::too_large;
                        row->opening=true;
                    }
                    struct completion{communication_event_dispatch* self;binding* row;~completion(){std::lock_guard lock{self->mutex_};row->opening=false;}} done{this,row};
                    retained_lease retained;status=retained.acquire(row->retained,std::min<std::uint64_t>(row->policy.max_streams,allowed.specification().max_resources));if(status!=error::success)return status;
                    workspace_lease reservation;status=workspace_.reserve(request.workspace,reservation);if(status!=error::success)return status;
                    auto origin=::uwvm2::utils::container::make_shared<record>();origin->count=request.source_count;origin->items=request.items;origin->bytes=request.bytes;origin->reservation=std::move(reservation);origin->retained=std::move(retained);
                    for(std::size_t i{};i<origin->count;++i)origin->capabilities[i]=request.sources[i].capability;
                    group_lease admitted;status=group(who,cap,name,origin.get(),0,admitted,false);if(status!=error::success)return status;
                    for(std::size_t i{};i<origin->count;++i)
                    {
                        status=sources_.resolve(sources_.context,who,request.sources[i].capability,request.sources[i].resource,origin->sources[i]);if(status!=error::success)return status;
                        if(origin->sources[i].resource!=request.sources[i].resource||origin->sources[i].origin.environment!=environment_||!origin->sources[i].origin||
                            !origin->sources[i].generation||!origin->sources[i].provider)return error::denied;
                    }
                    verification verified{this,who,row,origin.get()};status=validate(verified);if(status!=error::success)return status;
                    auto id=resources::allocate_identity();if(!id)return error::too_large;origin->id=id;
                    auto core=::uwvm2::utils::container::make_shared<events::stream>();auto policy=row->policy.policy;policy.max_events=request.items;
                    admitted.release();allowed.release();
                    status=communication_dispatch::convert(core->configure(std::move(policy),{origin->sources.data(),origin->count},&verified,check,observe));if(status!=error::success)return verified.failure!=error::success?verified.failure:status;
                    origin->stream=std::move(core);group_lease charged;status=group(who,cap,name,origin.get(),input.size()+8,charged,true);if(status!=error::success)return status;
                    current_origin=origin.get();status=checkpoint();if(status!=error::success)return status;
                    {std::lock_guard lock{mutex_};if(retired_||row->revoked)return error::cancelled;row->streams[slot]=origin;}
                    std::array<std::uint8_t,8> bytes{};wire::writer writer{bytes};writer.u64(id);bool copied=emit(writer.written());
                    group_lease after;status=group(who,cap,name,origin.get(),0,after,false);if(status==error::success)status=validate(verified);
                    if(!copied||status!=error::success){discard(*row,origin);return status!=error::success?status:error::io;}
                    {std::lock_guard lock{mutex_};origin->published=true;row->closed_receipts[slot]=0;}return error::success;
                }
                std::shared_ptr<record> origin;
                if(action==op::close)
                {
                    required=8;if(capacity<required)return error::too_large;
                    bool replay{};
                    {
                        std::lock_guard lock{mutex_};if(retired_||row->revoked)return error::cancelled;
                        for(auto id:row->closed_receipts)if(id&&id==request.stream)replay=true;
                        if(!replay)for(auto const& entry:row->streams)if(entry&&entry->id==request.stream&&entry->published&&!entry->closed)origin=entry;
                    }
                    if(!replay&&!origin)return error::bad_handle;
                    group_lease charged;status=group(who,cap,name,nullptr,input.size()+8,charged,true);if(status!=error::success)return status;
                    status=checkpoint();if(status!=error::success)return status;
                    if(!replay)
                    {
                        status=communication_dispatch::convert(origin->stream->close(row->policy.policy.authority));if(status!=error::success)return status;
                        std::lock_guard lock{mutex_};origin->closed=true;origin->published=false;
                        for(std::size_t i{};i<row->streams.size();++i)if(row->streams[i]==origin){row->closed_receipts[i]=origin->id;row->streams[i].reset();break;}
                    }
                    // Each slot retains one closed id until the next successful
                    // open acknowledges it; lost close output is safe to retry.
                    std::array<std::uint8_t,8> bytes{};wire::writer writer{bytes};writer.boolean(true);bool copied=emit(writer.written());
                    group_lease after;status=group(who,cap,name,nullptr,0,after,false);return status!=error::success?status:copied?error::success:error::io;
                }
                status=active(*row,request.stream,origin);if(status!=error::success)return status;
                current_origin=origin.get();status=checkpoint();if(status!=error::success)return status;
                if(request.limit>origin->items)return error::too_large;
                group_lease held;status=group(who,cap,name,origin.get(),0,held,false);if(status!=error::success)return status;
                verification verified{this,who,row,origin.get()};status=validate(verified);if(status!=error::success)return status;
                struct destination{communication_event_dispatch* self;caller who;handle cap;std::string_view action;binding* row;record* origin;verification* verified;std::size_t input,capacity;std::size_t* required;sink deliver;error failure{error::success};};
                destination target{this,who,cap,name,row,origin.get(),&verified,input.size(),capacity,&required,emit};
                auto copy=[](void* argument,std::span<events::event const> values)
                {
                    auto& d=*static_cast<destination*>(argument);auto measure=wire::writer::measuring(d.origin->bytes);
                    if(!event_contract::encode(measure,values)){d.failure=error::too_large;return false;}
                    *d.required=measure.size();if(d.capacity<*d.required){d.failure=error::too_large;return false;}
                    workspace_lease reservation;d.failure=d.self->workspace_.reserve(*d.required,reservation);if(d.failure!=error::success)return false;
                    byte_buffer bytes;bytes.resize_for_overwrite(*d.required);wire::writer writer{bytes.writable()};if(!event_contract::encode(writer,values)){d.failure=error::io;return false;}
                    group_lease admitted;d.failure=d.self->group(d.who,d.cap,d.action,d.origin,d.input+*d.required,admitted,true);if(d.failure!=error::success)return false;
                    if(!d.deliver(writer.written())){d.failure=error::io;return false;}
                    group_lease after;d.failure=d.self->group(d.who,d.cap,d.action,d.origin,0,after,false);
                    if(d.failure==error::success)d.failure=d.self->validate(*d.verified);
                    return d.failure==error::success;
                };
                held.release();allowed.release();
                status=communication_dispatch::convert(origin->stream->poll(row->policy.policy.authority,request.limit,request.deadline,&verified,check,&target,copy,observe));
                return target.failure!=error::success?target.failure:verified.failure!=error::success?verified.failure:status;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        /** @brief Close only the named stream under independent resource.close authority.
         * @details Source revocation never prevents cleanup. This preserves the
         * same bounded closed-id receipt as event.stream.close, so lost output
         * may be retried, and releases no source resource or source permission.
         */
        error close(caller who,handle cap,handle id)
        {
            if(!same_process()||!id)return error::denied;
            lease admitted;auto status=permissions_->admit(who,cap,"resource.close",0,admitted,false);if(status!=error::success)return status;
            binding* row{};status=resolve(admitted.specification(),row);if(status!=error::success)return status;
            std::shared_ptr<record> origin;
            {std::lock_guard lock{mutex_};if(retired_||row->revoked)return error::cancelled;
                for(auto closed:row->closed_receipts)if(closed==id)return error::success;
                for(auto const& entry:row->streams)if(entry&&entry->id==id&&entry->published&&!entry->closed)origin=entry;}
            if(!origin)return error::bad_handle;
            group_lease charged;status=group(who,cap,"resource.close",nullptr,16,charged,true);if(status!=error::success)return status;
            status=communication_dispatch::convert(origin->stream->close(row->policy.policy.authority));if(status!=error::success)return status;
            std::lock_guard lock{mutex_};origin->closed=true;origin->published=false;
            for(std::size_t i{};i<row->streams.size();++i)if(row->streams[i]==origin){row->closed_receipts[i]=origin->id;row->streams[i].reset();break;}
            return error::success;
        }
        error inspect(std::string_view alias,usage& out) const noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex_,std::try_to_lock};if(!lock.owns_lock())return error::again;
            usage result;result.ready=ready_;result.retired=retired_;
            for(auto const& row:bindings_)if(alias.empty()||row.alias==alias){++result.configured;for(auto const& entry:row.streams)if(entry&&entry->published&&!entry->closed)++result.streams;}
            auto status=workspace_.inspect(result.workspace_bytes,result.workspace_calls);if(status==error::success)out=result;return status;
        }
        void revoke(std::string_view alias) noexcept
        {
            if(!same_process())return;
            binding* row{};{std::lock_guard lock{mutex_};row=named(alias);if(!row)return;row->revoked=true;}
            for(std::size_t i{};i<row->streams.size();++i)
            {std::shared_ptr<record> origin;{std::lock_guard lock{mutex_};origin=row->streams[i];}if(origin)discard(*row,origin);}
        }
        void retire() noexcept
        {
            if(!same_process())return;
            {std::lock_guard lock{mutex_};retired_=true;}workspace_.retire();for(auto& row:bindings_)revoke(row.alias);
        }
    };
}
