/** @file
 * @brief Finite actual receive-only sensor deployment and three guest carriers.
 * @details Each private alias owns its source core/history and a trusted bound
 * native receive channel. No unconnected placeholder can open a sensor. Guests
 * select an exact profile, never native descriptors/routes. take requires both
 * sensor.take and independent grant-only data.export before reading or copying.
 * Its capacity preflight reserves the binding's largest complete reply before
 * any native read; actual transfer charges happen once for request+actual reply.
 * Failed copy/allocation keeps the source queue item and destroys unpublished
 * buffer storage. Native reads use existing open authority without creation-rate
 * charges. Frame/retained/syscall/workspace limits are distinct finite capacities.
 */
#pragma once
#include "sensor_codec.h"
#include "byte_buffer.h"
namespace uwvm2::imported::ros::scene
{
    class sensor_dispatch
    {
    public:
        using operation=sensor_contract::operation;using bytes=wire::bytes;
        using sink=::uwvm2::utils::container::function_ref<bool(bytes)>;
        using cancellation=::uwvm2::utils::container::function_ref<bool()>;
        using workspace_lease=proposals::workspace::lease;
        struct limits{std::uint64_t bindings{},storage_bytes{};};
        struct configuration
        {sensors::source::configuration source;sensors::source_identity actual;sensors::ingress::plan transport;std::uint64_t workspace_bytes{},workspace_calls{};};
        struct inspection
        {sensors::usage source;sensors::ingress::usage native;std::uint64_t workspace_bytes{},workspace_calls{};};
    private:
        struct binding
        {
            configuration policy;sensors::source provider;sensors::ingress native;proposals::workspace workspace;
            binding(std::shared_ptr<authority> permissions,buffers::registry& buffers,std::uint64_t(*clock)()noexcept)
                :provider(std::move(permissions),buffers,clock),native(provider){}
        };
        std::shared_ptr<authority> permissions_;buffers::registry& buffers_;handle environment_{},generation_{};limits limits_;
        std::uint64_t(*clock_)()noexcept;mutable std::mutex mutex_;std::array<std::shared_ptr<binding>,32> bindings_{};std::size_t count_{};
        std::uint64_t retained_{};bool ready_{},retired_{};
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
        std::shared_ptr<binding> named(std::string_view alias)const noexcept
        {for(std::size_t i{};i<count_;++i)if(bindings_[i]->policy.source.alias.view()==alias)return bindings_[i];return {};}
        error select(caller who,operation action,handle cap,std::uint64_t amount,lease& held,std::shared_ptr<binding>& output,bool preflight=false)
        {
            if(!same_process()||!permissions_)return error::denied;
            auto index=static_cast<std::size_t>(action);if(index>=sensor_contract::operations.size())return error::unsupported;
            auto name=sensor_contract::operations[index].name;
            auto status=preflight?permissions_->preflight(who,cap,name,amount,held):permissions_->admit(who,cap,name,0,held,false);
            if(status!=error::success)return status;
            auto const& grant=held.specification();if(grant.kind!=purpose::sensor||grant.binding!="bounded-sensor")return error::denied;
            std::lock_guard lock{mutex_};if(!ready_||retired_||!(output=named(grant.alias)))return error::denied;
            return error::success;
        }
        error export_right(caller who,handle cap,group_lease& held)
        {
            std::array claims{admission_claim{cap,"sensor.take",0,false,true},admission_claim{cap,"data.export",0,true,true}};
            return permissions_->admit_group(who,claims,held,false);
        }
        error current(caller who,operation action,handle cap,binding& expected)
        {lease allowed;std::shared_ptr<binding> row;auto status=select(who,action,cap,0,allowed,row);return status==error::success&&row.get()!=&expected?error::denied:status;}
        static error capture_adapter(void* context,caller who,handle cap,handle id,proposals::source_rule const& rule,proposal_dispatch::source_sample& output)
        {return static_cast<sensor_dispatch*>(context)->capture(who,cap,id,rule,output);}
        static error validate_adapter(void* context,caller who,handle cap,proposals::source_rule const& rule,proposal_dispatch::source_sample const& actual,
                                      proposals::admission::observation const& observed,proposals::admission::observation_policy policy)
        {return static_cast<sensor_dispatch*>(context)->validate(who,cap,rule,actual,observed,policy);}
    public:
        sensor_dispatch(std::shared_ptr<authority> permissions,buffers::registry& buffers,handle environment,handle generation,limits limits,
                        std::uint64_t(*clock)()noexcept=nullptr)
            :permissions_(std::move(permissions)),buffers_(buffers),environment_(environment),generation_(generation),limits_(limits),clock_(clock){}
        ~sensor_dispatch(){retire();}
        static bool required_storage(configuration const& policy,std::uint64_t& output)noexcept
        {
            std::uint64_t core{},native{};if(!sensors::core::required_storage(policy.source.ingress,core)||!sensors::ingress::required_storage(policy.transport,native)||
                policy.workspace_bytes>INT64_MAX||policy.workspace_calls>1024||core>INT64_MAX-native-sizeof(binding))return false;
            output=sizeof(binding)+core+native;return true;
        }
        /** @brief Bind complete finite source semantics and one actually checked trusted FD. */
        error bind(configuration const& policy,int descriptor)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(ready_||retired_||!permissions_||!environment_||!generation_)return error::denied;
            std::uint64_t needed{};if(!proposals::policy_text(policy.source.alias,true)||!required_storage(policy,needed)||limits_.bindings>32||limits_.storage_bytes>INT64_MAX)return error::invalid;
            if(named(policy.source.alias.view()))return error::conflict;
            if(count_>=limits_.bindings||retained_>limits_.storage_bytes||needed>limits_.storage_bytes-retained_)return error::too_large;
            try
            {
                auto row=::uwvm2::utils::container::make_shared<binding>(permissions_,buffers_,clock_);row->policy=policy;
                auto owner=resources::allocate_identity();if(!owner)return error::too_large;
                auto status=row->provider.configure({environment_,owner,generation_},policy.source,policy.actual);if(status!=error::success)return status;
                status=row->native.bind(descriptor,policy.transport);if(status!=error::success)return status;
                status=row->workspace.configure(policy.workspace_bytes,policy.workspace_calls);if(status!=error::success)return status;
                bindings_[count_++]=std::move(row);retained_+=needed;return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}
        }
        error prepare()noexcept
        {if(!same_process())return error::denied;std::lock_guard lock{mutex_};if(ready_||retired_)return error::denied;ready_=true;return error::success;}
        error preflight(caller who,operation action,handle cap,std::size_t extent)
        {
            if(extent>sensor_contract::maximum_request_bytes)return error::too_large;
            if(action!=operation::open&&extent!=8)return error::invalid;
            lease allowed;std::shared_ptr<binding> row;auto status=select(who,action,cap,extent,allowed,row,true);if(status!=error::success)return status;
            if(action==operation::take){group_lease export_scope;return export_right(who,cap,export_scope);}return error::success;
        }
        error reserve_workspace(caller who,operation action,handle cap,std::size_t extent,workspace_lease& output)
        {auto status=preflight(who,action,cap,extent);if(status!=error::success)return status;lease allowed;std::shared_ptr<binding> row;status=select(who,action,cap,extent,allowed,row);return status==error::success?row->workspace.reserve(extent,output):status;}
        /** @brief Invoke one copied canonical request; no guest pointer is retained. */
        error invoke(caller who,operation action,handle cap,bytes input,std::size_t capacity,std::size_t& required,sink deliver,cancellation cancelled={})
        {
            required=0;auto status=preflight(who,action,cap,input.size());if(status!=error::success)return status;
            if(!deliver||capacity>sensor_contract::maximum_reply_bytes)return error::invalid;
            sensor_contract::request request;if(!sensor_contract::decode(action,input,request))return error::invalid;
            lease primary;std::shared_ptr<binding> row;status=select(who,action,cap,input.size(),primary,row);if(status!=error::success)return status;
            try
            {
                workspace_lease workspace;status=row->workspace.reserve(0,workspace);if(status!=error::success)return status;
                auto check=[&](){if(cancelled&&cancelled())return error::cancelled;return current(who,action,cap,*row);};
                if(action==operation::open)
                {
                    required=8;if(capacity<required)return error::too_large;
                    status=check();if(status!=error::success)return status;
                    sensors::ingress::usage active;row->native.inspect(active);if(!active.active||active.retired)return error::unsupported;
                    error refusal=error::success;
                    status=row->provider.open(who,cap,request.selected,[&](handle id)
                    {refusal=check();if(refusal!=error::success)return false;std::array<std::uint8_t,8> output;wire::writer out{output};out.u64(id);
                        if(!deliver(out.written())){refusal=error::io;return false;}refusal=check();return refusal==error::success;},input.size()+required);
                    return refusal!=error::success?refusal:status;
                }
                if(action==operation::describe)
                {
                    sensors::usage value;sensors::ingress::usage native;auto measure=wire::writer::measuring(sensor_contract::maximum_reply_bytes);
                    if(!sensor_contract::encode_description(measure,row->policy.source,value,native))return error::invalid;
                    required=measure.size();if(capacity<required)return error::too_large;
                    status=check();if(status!=error::success)return status;
                    status=row->provider.describe(who,cap,request.sensor,value,input.size()+required);if(status!=error::success)return status;
                    status=row->native.inspect(native);if(status!=error::success)return status;
                    status=workspace.grow(required);if(status!=error::success)return status;
                    byte_buffer output;output.resize_for_overwrite(required);wire::writer out{output.writable()};
                    if(!sensor_contract::encode_description(out,row->policy.source,value,native)||out.size()!=required)return error::io;
                    status=check();if(status!=error::success)return status;if(!deliver(out.written()))return error::io;return check();
                }
                required=sensor_contract::maximum_take_size(row->policy.source);if(!required)return error::invalid;if(capacity<required)return error::too_large;
                group_lease export_scope;status=export_right(who,cap,export_scope);if(status!=error::success)return status;
                status=check();if(status!=error::success)return status;status=export_right(who,cap,export_scope);if(status!=error::success)return status;
                status=row->provider.preflight_handle(who,cap,request.sensor,"sensor.take");if(status!=error::success)return status;
                // Recover an already queued delivery before touching the peer.
                // Empty queues permit at most one finite read per guest call.
                bool pending{};status=row->provider.pending(who,cap,request.sensor,pending);if(status!=error::success)return status;
                if(!pending){status=row->native.pump();if(status!=error::success&&status!=error::again)return status;}
                byte_buffer output;error refusal=error::success;
                auto admission=[&](sensors::delivery const& frame,std::uint64_t& framing)
                {
                    auto measured=wire::writer::measuring(sensor_contract::maximum_reply_bytes);
                    if(!sensor_contract::encode_take(measured,request.sensor,frame,1,row->policy.source.buffer_profile))return error::invalid;
                    required=measured.size();if(capacity<required)return error::too_large;framing=input.size()+required-frame.body.size();
                    auto allowed=check();if(allowed!=error::success)return allowed;
                    allowed=export_right(who,cap,export_scope);if(allowed!=error::success)return allowed;
                    lease quota;allowed=permissions_->preflight(who,cap,"sensor.take",input.size()+required,quota);if(allowed!=error::success)return allowed;
                    allowed=workspace.grow(required);if(allowed!=error::success)return allowed;output.resize_for_overwrite(required);return error::success;
                };
                status=row->provider.take(who,cap,request.sensor,[&](sensors::source::retained_delivery const& frame)
                {
                    refusal=check();if(refusal!=error::success)return false;refusal=export_right(who,cap,export_scope);if(refusal!=error::success)return false;
                    wire::writer out{output.writable()};if(!sensor_contract::encode_take(out,request.sensor,frame.frame,frame.buffer,row->policy.source.buffer_profile)||out.size()!=required){refusal=error::io;return false;}
                    if(!deliver(out.written())){refusal=error::io;return false;}refusal=check();if(refusal!=error::success)return false;
                    refusal=export_right(who,cap,export_scope);return refusal==error::success;
                },admission);
                return refusal!=error::success?refusal:status;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        error close(caller who,handle cap,handle resource)
        {
            if(!same_process()||!permissions_)return error::denied;
            lease allowed;auto status=permissions_->admit(who,cap,"resource.close",0,allowed,false);
            if(status!=error::success)return status;
            auto const& grant=allowed.specification();if(grant.kind!=purpose::sensor||grant.binding!="bounded-sensor")return error::denied;
            std::shared_ptr<binding> row;{std::lock_guard lock{mutex_};if(!ready_||retired_||!(row=named(grant.alias)))return error::denied;}return row->provider.close(who,cap,resource);
        }
        proposal_dispatch::source_access access()noexcept{return {this,capture_adapter,validate_adapter};}
        error capture(caller who,handle cap,handle id,proposals::source_rule const& rule,proposal_dispatch::source_sample& output)
        {
            if(!same_process()||!proposals::policy_text(rule.alias,true))return error::denied;
            std::shared_ptr<binding> row;{std::lock_guard lock{mutex_};if(!ready_||retired_||!(row=named(rule.alias.view())))return error::denied;}
            return row->provider.capture(who,cap,id,rule,output);
        }
        error validate(caller who,handle cap,proposals::source_rule const& rule,proposal_dispatch::source_sample const& actual,
                       proposals::admission::observation const& observed,proposals::admission::observation_policy policy)
        {
            if(!same_process()||!proposals::policy_text(rule.alias,true))return error::denied;
            std::shared_ptr<binding> row;{std::lock_guard lock{mutex_};if(!ready_||retired_||!(row=named(rule.alias.view())))return error::denied;}
            return row->provider.validate(who,cap,rule,actual,observed,policy);
        }
        error inspect(std::string_view alias,sensors::ingress::usage& output)const noexcept
        {if(!same_process())return error::denied;std::lock_guard lock{mutex_};auto row=named(alias);return row?row->native.inspect(output):error::denied;}
        /** @brief Try-lock cached provider/receiver/workspace counters; never inspect native descriptors. */
        error inspect(std::string_view alias,inspection& output)const noexcept
        {
            if(!same_process())return error::denied;
            std::shared_ptr<binding> row;{std::unique_lock lock{mutex_,std::try_to_lock};if(!lock.owns_lock())return error::again;if(!(row=named(alias)))return error::denied;}
            inspection value;auto status=row->provider.inspect_cached(value.source);if(status!=error::success)return status;
            status=row->native.inspect_cached(value.native);if(status!=error::success)return status;
            row->workspace.inspect(value.workspace_bytes,value.workspace_calls);output=value;return error::success;
        }
        /** @brief Optional trusted scheduler pump; guests cannot name or call a native endpoint. */
        error pump(std::string_view alias)
        {if(!same_process())return error::denied;std::shared_ptr<binding> row;{std::lock_guard lock{mutex_};if(!ready_||retired_||!(row=named(alias)))return error::denied;}return row->native.pump();}
        void revoke(std::string_view alias)noexcept
        {if(!same_process())return;std::shared_ptr<binding> row;{std::lock_guard lock{mutex_};row=named(alias);}if(row){row->workspace.retire();row->native.retire();}}
        void retire()noexcept
        {if(!same_process())return;std::lock_guard lock{mutex_};if(retired_)return;retired_=true;for(std::size_t i{};i<count_;++i){bindings_[i]->workspace.retire();bindings_[i]->native.retire();}}
    };
}
