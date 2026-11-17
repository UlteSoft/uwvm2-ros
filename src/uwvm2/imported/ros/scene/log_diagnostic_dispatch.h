/** @file
 * @brief Independent log admission and compound, payload-free diagnostic authority.
 * @details Each immutable log alias owns a private store. A diagnostic alias
 * selects exactly one such source; a read must independently present its source
 * resource.observe capability. These aliases do not activate files, consoles,
 * sockets or native appenders. Both primary and source attenuation are admitted
 * atomically, with no implicit throttle within granted scope. Stable storage and
 * transient carrier budgets remain separate. No default plan grants authority.
 */
#pragma once
#include "log_diagnostic_contract.h"
#include "communication_workspace.h"
#include <uwvm2/utils/container/function_ref.h>

namespace uwvm2::imported::ros::scene
{
    class log_diagnostic_dispatch
    {
        using op=log_diagnostic_contract::operation;
    public:
        struct limits{std::uint64_t logs{},diagnostics{},storage_bytes{},request_bytes{},workspace_bytes{},workspace_calls{};};
        using bytes=wire::bytes;
        using sink=::uwvm2::utils::container::function_ref<bool(bytes)>;
        using cancelled=::uwvm2::utils::container::function_ref<bool()>;
        using workspace_lease=communication_workspace::lease;
    private:
        struct log_binding{logs::owner owner;logs::plan policy;logs::store store;bool revoked{};};
        struct diagnostic_binding
        {
            logs::owner owner;diagnostics::plan policy;::uwvm2::utils::container::std_string source_alias;
            std::shared_ptr<log_binding> source;diagnostics::snapshot snapshot;bool revoked{};
            /** @brief Closed log counter schema; no text or private log content is read. */
            static error capture(void* context,std::span<diagnostics::counter> output,std::size_t& count) noexcept
            {
                auto& binding=*static_cast<diagnostic_binding*>(context);logs::usage value;
                auto status=binding.source->store.inspect(binding.source->owner,value);if(status!=error::success)return status;
                std::array<std::uint64_t,11> values{value.accepted,value.exported,value.lost,value.retained,value.retained_bytes,
                    value.reserved_bytes,value.full_refusals,value.copy_failures,value.retired,value.draining,value.counters_saturated};
                std::size_t used{};
                for(std::size_t i{};i<values.size();++i)if(binding.policy.fields&(std::uint64_t{1}<<i))
                {
                    if(used==output.size())return error::too_large;
                    output[used++]={i+1,values[i],true,(i==6||i==7)&&values[i]==UINT64_MAX&&value.counters_saturated};
                }
                count=used;return error::success;
            }
        };
        std::shared_ptr<authority> permissions;
        limits capacity;std::uint64_t environment{},reserved{};
        mutable std::mutex mutex;bool ready{},retired{};
        communication_workspace workspace;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,std::shared_ptr<log_binding>,std::less<>> logs_;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,std::shared_ptr<diagnostic_binding>,std::less<>> diagnostics_;
#if defined(__linux__)
        ::pid_t process{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process==::getpid();
#else
            return true;
#endif
        }
        bool valid_capacity() const noexcept
        {
            return capacity.logs<=4096&&capacity.diagnostics<=4096&&capacity.storage_bytes<=INT64_MAX&&
                capacity.request_bytes<=buffers::maximum_transfer&&capacity.workspace_bytes<=INT64_MAX&&capacity.workspace_calls<=INT64_MAX;
        }
        error log(grant const& value,std::shared_ptr<log_binding>& output)
        {
            std::lock_guard lock{mutex};if(!ready||retired||value.kind!=purpose::log||value.binding!="private-log")return error::denied;
            auto found=logs_.find(value.alias);if(found==logs_.end()||found->second->revoked)return error::denied;
            output=found->second;return error::success;
        }
        error diagnostic(grant const& value,std::shared_ptr<diagnostic_binding>& output)
        {
            std::lock_guard lock{mutex};if(!ready||retired||value.kind!=purpose::diagnostic||value.binding!="local-diagnostic")return error::denied;
            auto found=diagnostics_.find(value.alias);if(found==diagnostics_.end()||found->second->revoked)return error::denied;
            output=found->second;return error::success;
        }
        /** @brief Recheck local retirement after reentrant callbacks, before disclosure.
         * @details The authority lease fences ordinary concurrent revocation,
         * but a callback can also retire a provider directly without changing
         * the capability table. No callback may intervene between these local
         * checks, the final authority check and the bounded effect/copy. A
         * post-copy check prevents consumption; it cannot recall observed bytes.
         */
        bool live(log_binding const& value) const noexcept
        {std::lock_guard lock{mutex};return !retired&&!value.revoked;}
        bool live(diagnostic_binding const& value) const noexcept
        {std::lock_guard lock{mutex};return !retired&&!value.revoked&&value.source&&!value.source->revoked;}
    public:
        /** @brief Construct trusted configuration only; runtime callers still need exact grants. */
        log_diagnostic_dispatch(std::shared_ptr<authority> access,std::uint64_t identity,limits bound):
            permissions{std::move(access)},capacity{bound},environment{identity}{}
        ~log_diagnostic_dispatch()
        {
            if(!same_process())
            {std::construct_at(std::addressof(logs_));std::construct_at(std::addressof(diagnostics_));std::construct_at(std::addressof(permissions));return;}
            retire();
        }
        /** @brief Reserve one complete log store before publishing its immutable alias.
         * @details The trusted configuration caller keeps the input stable for
         * this call. The binding and store each retain their own policy copy;
         * no reference to caller storage escapes. Passing the fixed logger
         * allowlist by reference avoids a large outgoing stack argument and
         * keeps allocation-failure unwinding independent of that copy on ARM32.
         * Allocation failure publishes neither a binding nor a reservation.
         */
        error bind_log(::uwvm2::utils::container::std_string alias,logs::plan const& policy)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(ready||retired||!permissions||!environment)return error::denied;
            if(!valid_capacity())return error::invalid;
            std::uint64_t bytes{};if(!identifier(alias)||!logs::store::required_storage(policy,bytes))return error::invalid;
            if(logs_.contains(alias))return error::conflict;
            if(logs_.size()>=capacity.logs||reserved>capacity.storage_bytes||bytes>capacity.storage_bytes-reserved||policy.record_bytes>capacity.request_bytes)return error::too_large;
            auto id=resources::allocate_identity();if(!id)return error::too_large;
            auto value=::uwvm2::utils::container::make_shared<log_binding>();value->owner={environment,id,1};value->policy=policy;
            auto status=value->store.configure(value->owner,policy);if(status!=error::success)return status;
            logs_.emplace(std::move(alias),std::move(value));reserved+=bytes;return error::success;
        }
        /** @brief Bind a finite diagnostic snapshot to one already configured local log.
         * @details Counter codes 1..11 are accepted/exported/lost/retained,
         * retained bytes/reserved bytes/full refusals/copy failures/retired/
         * draining/counter saturation. Observing these requires the actual source
         * alias's resource.observe grant on every invocation, never log.emit alone.
         */
        error bind_diagnostic(::uwvm2::utils::container::std_string alias,std::string_view source_alias,diagnostics::plan const& policy)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(ready||retired||!permissions||!environment)return error::denied;
            if(!valid_capacity())return error::invalid;
            std::uint64_t bytes{};if(!identifier(alias)||!diagnostics::snapshot::required_storage(policy,bytes)||
                (policy.fields&~std::uint64_t{2047})||static_cast<unsigned>(std::popcount(policy.fields))>policy.rows)return error::invalid;
            if(diagnostics_.contains(alias))return error::conflict;
            auto origin=logs_.find(source_alias);if(origin==logs_.end())return error::bad_handle;
            if(diagnostics_.size()>=capacity.diagnostics||reserved>capacity.storage_bytes||bytes>capacity.storage_bytes-reserved)return error::too_large;
            auto id=resources::allocate_identity();if(!id)return error::too_large;
            auto value=::uwvm2::utils::container::make_shared<diagnostic_binding>();value->owner={environment,id,1};value->policy=policy;
            value->source_alias.assign(source_alias);value->source=origin->second;
            auto status=value->snapshot.configure(value->owner,policy,{value.get(),diagnostic_binding::capture});if(status!=error::success)return status;
            diagnostics_.emplace(std::move(alias),std::move(value));reserved+=bytes;return error::success;
        }
        /** @brief Freeze finite carrier limits after all bindings; no I/O occurs. */
        error prepare()
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(ready||retired||!permissions||!environment)return error::denied;
            if(!valid_capacity()||capacity.request_bytes<log_diagnostic_contract::maximum_read_request)return error::invalid;
            auto status=workspace.configure(capacity.workspace_bytes,capacity.workspace_calls);if(status!=error::success)return status;
            ready=true;return error::success;
        }
        /** @brief Check primary authority and request extent before copying any guest bytes. */
        error preflight(caller who,op action,handle cap,std::size_t size,bool output_fits=true)
        {
            if(!same_process()||!permissions)return error::denied;
            auto index=static_cast<std::size_t>(action);if(index>=log_diagnostic_contract::operations.size())return error::unsupported;
            if(size>capacity.request_bytes)return error::too_large;
            auto cost=size+(action==op::emit?log_diagnostic_contract::receipt_bytes:0u);
            lease allowed;auto status=output_fits?permissions->preflight(who,cap,log_diagnostic_contract::operations[index].name,cost,allowed):
                permissions->admit(who,cap,log_diagnostic_contract::operations[index].name,0,allowed,false);
            if(status!=error::success)return status;
            if(action==op::emit)
            {std::shared_ptr<log_binding> value;status=log(allowed.specification(),value);return status==error::success&&size>value->policy.record_bytes?error::too_large:status;}
            std::shared_ptr<diagnostic_binding> value;status=diagnostic(allowed.specification(),value);
            return status==error::success&&size>log_diagnostic_contract::maximum_read_request?error::too_large:status;
        }
        /** @brief Retain transient carrier capacity until the actual owned request is freed. */
        error reserve_workspace(caller who,op action,handle cap,std::size_t size,workspace_lease& output)
        {auto status=preflight(who,action,cap,size);return status==error::success?workspace.reserve(size,output):status;}
        /** @brief Deliver an admitted receipt/page with exact source checks and no native effect. */
        error invoke(caller who,op action,handle cap,bytes input,std::size_t output_capacity,std::size_t& required,sink deliver,cancelled cancellation={})
        {
            required=0;
            auto status=preflight(who,action,cap,input.size(),output_capacity>=(action==op::emit?48u:24u));if(status!=error::success)return status;
            if(!deliver||output_capacity>buffers::maximum_transfer)return error::invalid;
            auto stopped=[&]{return cancellation&&cancellation();};if(stopped())return error::cancelled;
            auto name=log_diagnostic_contract::operations[static_cast<std::size_t>(action)].name;
            if(action==op::emit)
            {
                logs::request request;if(!logs::decode(input,request))return error::invalid;
                required=log_diagnostic_contract::receipt_bytes;if(output_capacity<required)return error::too_large;
                lease admitted;status=permissions->admit(who,cap,name,input.size()+required,admitted);if(status!=error::success)return status;
                std::shared_ptr<log_binding> binding;status=log(admitted.specification(),binding);if(status!=error::success)return status;
                if(stopped())return error::cancelled;
                // Cancellation hooks are trusted but may reenter authority.
                // A false answer is not proof that the held grant stayed live.
                lease before_effect;status=permissions->admit(who,cap,name,0,before_effect,false);if(status!=error::success)return status;
                if(!live(*binding))return error::cancelled;
                logs::receipt receipt;status=binding->store.emit(binding->owner,input,receipt);if(status!=error::success)return status;
                std::array<std::uint8_t,log_diagnostic_contract::receipt_bytes> output;wire::writer writer{output};
                for(auto value:{binding->owner.grant,receipt.session,receipt.sequence,receipt.record,receipt.accepted_bytes,receipt.lost_before})
                    if(!writer.u64(value))return error::invalid;
                if(stopped())return error::cancelled;
                lease before_copy;status=permissions->admit(who,cap,name,0,before_copy,false);if(status!=error::success)return status;
                if(!live(*binding))return error::cancelled;
                if(!deliver(writer.written())||stopped())return error::io;
                lease current;status=permissions->admit(who,cap,name,0,current,false);
                return status==error::success&&!live(*binding)?error::cancelled:status;
            }
            log_diagnostic_contract::read_request request;if(!log_diagnostic_contract::decode(input,request))return error::invalid;
            required=32+32*request.limit;if(output_capacity<required)return error::too_large;
            std::array claims{admission_claim{cap,name,input.size()+required,false,false},
                admission_claim{request.source_capability,"resource.observe",required,true,false}};
            group_lease admitted;status=permissions->admit_group(who,claims,admitted,false);if(status!=error::success)return status;
            std::shared_ptr<diagnostic_binding> binding;status=diagnostic(*admitted.specification(0),binding);if(status!=error::success)return status;
            auto const& origin=*admitted.specification(1);
            if(origin.alias!=binding->source_alias||origin.kind!=purpose::log||origin.binding!="private-log"||request.scope!=binding->policy.scope)return error::denied;
            if(request.limit>binding->policy.page_items)return error::too_large;
            if(stopped())return error::cancelled;
            group_lease charged;status=permissions->admit_group(who,claims,charged);if(status!=error::success)return status;
            if(!live(*binding))return error::cancelled;
            auto emit_page=[&](diagnostics::page const& page)
            {
                std::array<std::uint8_t,log_diagnostic_contract::maximum_page_bytes> output;wire::writer writer{output};
                if(stopped()||!log_diagnostic_contract::encode(writer,page))return false;
                group_lease before_copy;if(permissions->admit_group(who,claims,before_copy,false)!=error::success||!live(*binding))return false;
                required=writer.size();if(!deliver(writer.written())||stopped())return false;
                group_lease current;return permissions->admit_group(who,claims,current,false)==error::success&&live(*binding);
            };
            return binding->snapshot.read(binding->owner,request.scope,request.cursor,static_cast<std::size_t>(request.limit),emit_page);
        }
        /** @brief Invalidate a source and every dependent diagnostic stream before teardown. */
        void revoke(std::string_view alias) noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex};auto found=logs_.find(alias);
            if(found!=logs_.end()){found->second->revoked=true;static_cast<void>(found->second->store.retire(found->second->owner));}
            for(auto const& [name,value]:diagnostics_)if(name==alias||value->source_alias==alias)
            {value->revoked=true;static_cast<void>(value->snapshot.retire(value->owner));}
        }
        void retire() noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex};retired=true;workspace.retire();
            for(auto const& [alias,value]:logs_){(void)alias;value->revoked=true;static_cast<void>(value->store.retire(value->owner));}
            for(auto const& [alias,value]:diagnostics_){(void)alias;value->revoked=true;static_cast<void>(value->snapshot.retire(value->owner));}
        }
        /** @brief Detached metadata only, without expiry, capture, drain or permission elevation. */
        error inspect_log(std::string_view alias,logs::usage& output) const noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            auto found=logs_.find(alias);if(found==logs_.end())return error::bad_handle;
            return found->second->store.inspect(found->second->owner,output);
        }
        error inspect_diagnostic(std::string_view alias,diagnostics::usage& output) const noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            auto found=diagnostics_.find(alias);if(found==diagnostics_.end())return error::bad_handle;
            return found->second->snapshot.inspect(found->second->owner,output);
        }
    };
}
