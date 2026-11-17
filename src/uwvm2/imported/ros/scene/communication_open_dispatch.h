/** @file
 * @brief Scoped asynchronous creation observation, publication and cleanup.
 * @details Creation requests are caller-owned records, never usable endpoints.
 * Deployment binds an exact authority and endpoint family to a retained trusted
 * provider. A take checks the complete original node/context/schema cohort and
 * the independent communication.open.take grant. It reserves the complete
 * result before preparing a typed resource, then publishes under fresh authority.
 * No method waits for IPC or starts a thread. Failed guest copies preserve the
 * same published handle. Cancellation and receipt release have independently
 * granted, traffic-independent cleanup admission; they cannot close a published
 * endpoint, erase unknown outcomes or refund creation charges.
 *
 * Providers prepare private typed resources without native I/O. Publication is
 * allocation-free and must accept only exact verified creation evidence. Their
 * owning lifetime is retained until all calls finish. All owned dispatcher
 * storage uses recoverable fast_io allocation, with finite configured slots.
 */
#pragma once
#include "communication_creation.h"
#include "communication_workspace.h"
#include <uwvm2/utils/container/function_ref.h>

namespace uwvm2::imported::ros::scene
{
    class communication_open_dispatch
    {
    public:
        enum class operation:std::uint8_t{take,cancel,describe,release};
        inline static constexpr std::array<std::string_view,4> names{
            "communication.open.take","communication.open.cancel","request.describe","request.release"};
        struct observation
        {communication_creation::identity origin;communication_creation::view state;};
        /** @brief Trusted publication adapter; guest integers never select a native object.
         * @details inspect and validate are side-effect-free. prepare may allocate
         * bounded host storage but performs no native I/O and publishes nothing.
         * publish is allocation-free under the supplied complete origin lease.
         * cancel/release preserve native and unknown cleanup debt. A published
         * endpoint requires resource.close, independently from this protocol.
         */
        struct provider
        {
            virtual ~provider()=default;
            virtual communication::error inspect(caller,handle,observation&)noexcept=0;
            virtual communication::error validate(communication_creation::identity const&)noexcept=0;
            virtual communication::error prepare(caller,handle,handle&)noexcept=0;
            virtual communication::error publish(caller,handle,group_lease const&,handle,communication_open_contract::published&)noexcept=0;
            virtual communication::error cancel(caller,handle,communication_creation::view&)noexcept=0;
            virtual communication::error release(caller,handle)noexcept=0;
        };
        struct limits{std::size_t bindings{};std::uint64_t workspace_bytes{},workspace_calls{};};
        using sink=::uwvm2::utils::container::function_ref<bool(wire::bytes)>;
        using cancellation=::uwvm2::utils::container::function_ref<bool()>;
    private:
        struct binding
        {
            ::uwvm2::utils::container::std_string alias;
            communication::owner identity;communication_open_contract::endpoint_kind kind{};
            std::shared_ptr<provider> actual;std::mutex transaction;
        };
        std::shared_ptr<authority> permissions;
        ::uwvm2::utils::container::std_vector<::uwvm2::utils::container::std_unique_ptr<binding>> bindings;
        communication_workspace workspace;limits bound;
        std::mutex configuration;std::atomic<bool> ready{},retired{};
#if defined(__linux__)
        pid_t process{::getpid()};
#endif
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return process==::getpid();
#else
            return true;
#endif
        }
        static error convert(communication::error value)noexcept
        {
            if(value==communication::error::busy)return error::again;
            if(value==communication::error::stale)return error::conflict;
            return communication::known_status(value)?static_cast<error>(value):error::io;
        }
        static purpose kind(communication_open_contract::endpoint_kind value)noexcept
        {
            using K=communication_open_contract::endpoint_kind;
            return value==K::node?purpose::node:value==K::topic?purpose::topic:value==K::service?purpose::service:purpose::action;
        }
        error resolve(grant const& policy,binding*& output)noexcept
        {
            if(!ready.load(std::memory_order_acquire)||retired.load())return error::denied;
            for(auto const& row:bindings)if(row->alias==policy.alias)
            {
                if(policy.kind!=kind(row->kind)||policy.binding!=(row->kind==communication_open_contract::endpoint_kind::action?"native-action":"native-communication"))return error::denied;
                output=row.get();return error::success;
            }
            return error::denied;
        }
        static bool owned(binding const& row,caller who,handle request,observation const& result)noexcept
        {
            namespace creation=communication_creation;using K=communication_open_contract::endpoint_kind;
            auto const& origin=result.origin;auto const& state=result.state;
            auto count=row.kind==K::node?2u:static_cast<unsigned>(row.kind)+2u;
            if(!creation::same_caller(who,origin.who)||origin.authority!=row.identity||origin.count!=count||
                state.kind!=row.kind||state.receipt.request_id!=request||state.receipt.key!=origin.key||!origin.key||!origin.deadline||
                !state.provider_generation||state.reserved||!communication::known_status(state.receipt.result)||
                state.receipt.status<communication_open_contract::state::queued||state.receipt.status>communication_open_contract::state::released||
                state.receipt.observed>communication::effect::unknown)return false;
            for(std::size_t i{};i<origin.count;++i)
            {
                auto const& item=origin.origins[i];auto expected=i==0?creation::dependency::operation:row.kind==K::node?creation::dependency::context:
                    i==1?creation::dependency::node:i==2?creation::dependency::context:creation::dependency::schema;
                if(item.kind!=expected||!item.authority||item.authority.environment!=row.identity.environment||!item.capability||
                    ((expected==creation::dependency::context||expected==creation::dependency::node)?!item.resource:bool(item.resource))||
                    (!i&&item.authority!=row.identity))return false;
            }
            for(std::size_t i{origin.count};i<origin.origins.size();++i)if(origin.origins[i]!=creation::origin{})return false;
            return true;
        }
        error scope(binding& row,observation const& value,group_lease& held)noexcept
        {
            communication_creation::claim_set claims;
            auto status=permissions->admit_group(value.origin.who,communication_creation::admission_claims(value.origin,row.kind,claims),held,false);
            if(status!=error::success)return status;
            status=convert(row.actual->validate(value.origin));if(status!=error::success)held.release();return status;
        }
    public:
        communication_open_dispatch(std::shared_ptr<authority> permission,limits capacity):permissions(std::move(permission)),bound(capacity){}
        communication_open_dispatch(communication_open_dispatch const&)=delete;
        communication_open_dispatch& operator=(communication_open_dispatch const&)=delete;
        ~communication_open_dispatch(){retire();}
        /** @brief Bind one immutable creation authority without issuing authority or performing I/O. */
        error bind(::uwvm2::utils::container::std_string alias,communication::owner identity,
                   communication_open_contract::endpoint_kind family,std::shared_ptr<provider> actual)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{configuration};if(ready.load()||retired.load())return error::denied;
            if(!permissions||!identifier(alias)||!identity||!actual||!communication_open_contract::valid(family)||!bound.bindings||bound.bindings>128)return error::invalid;
            if(bindings.size()>=bound.bindings)return error::too_large;
            for(auto const& row:bindings)if(row->alias==alias||row->identity==identity)return error::conflict;
            try
            {
                auto row=::uwvm2::utils::container::make_unique<binding>();row->alias=std::move(alias);row->identity=identity;row->kind=family;row->actual=std::move(actual);
                bindings.push_back(std::move(row));return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /// @brief Reserve finite concurrent carrier/result workspace before serving any call.
        error prepare()noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{configuration};if(ready.load()||retired.load()||bindings.empty())return error::denied;
            if(bound.workspace_bytes<520)return error::too_large;
            auto status=workspace.configure(bound.workspace_bytes,bound.workspace_calls);
            if(status==error::success)ready.store(true,std::memory_order_release);
            return status;
        }
        /** @brief Validate the exact fixed carrier before reading guest memory. */
        error preflight(caller who,operation action,handle capability,std::size_t extent)noexcept
        {
            if(!same_process()||!permissions||!ready.load()||retired.load())return error::denied;
            auto index=static_cast<std::size_t>(action);if(index>=names.size())return error::unsupported;
            if(extent!=8)return error::invalid;
            lease allowed;auto status=permissions->admit(who,capability,names[index],0,allowed,false);if(status!=error::success)return status;
            binding* row{};return resolve(allowed.specification(),row);
        }
        /** @brief Inspect, publish, cancel or release one exact caller-owned creation.
         * @details Every short-output check precedes provider preparation,
         * cancellation, rate debit and release. A failed publication copy keeps
         * the same endpoint; replay never repeats native effects or another take
         * debit. Request descriptions are bounded cached observations. Explicit
         * zero take/describe rates reject new admissions. Independent cancel and
         * release permissions still require current exposure and membership.
         */
        error invoke(caller who,operation action,handle capability,wire::bytes input,std::size_t capacity,std::size_t& required,
                     sink deliver,cancellation cancelled={})noexcept
        {
            namespace open=communication_open_contract;required=0;
            try
            {
                auto status=preflight(who,action,capability,input.size());if(status!=error::success)return status;
                if(!deliver)return error::invalid;
                wire::reader in{input};std::uint64_t request{};if(!in.u64(request)||!request||!in.done())return error::invalid;
                auto name=names[static_cast<std::size_t>(action)];lease allowed;
                status=permissions->admit(who,capability,name,0,allowed,false);if(status!=error::success)return status;
                binding* row{};status=resolve(allowed.specification(),row);if(status!=error::success)return status;
                allowed.release();std::unique_lock serial{row->transaction,std::try_to_lock};if(!serial.owns_lock())return error::again;
                communication_workspace::lease reservation;status=workspace.reserve(520,reservation);if(status!=error::success)return status;
                observation current;status=convert(row->actual->inspect(who,request,current));if(status!=error::success)return status;
                if(!owned(*row,who,request,current))return error::denied;
                auto checkpoint=[&]{return retired.load()||(cancelled&&cancelled());};if(checkpoint())return error::cancelled;
                auto phase=current.state.receipt.status;bool cleanup=action==operation::cancel||action==operation::release;
                auto encode_description=[&](wire::writer& out){return open::encode_description(out,current.state.receipt,current.state.provider_generation);};
                if(action==operation::take)required=open::published_bytes;
                else if(action==operation::cancel)required=open::receipt_bytes;
                else if(action==operation::release)required=8;
                else {auto out=wire::writer::measuring(512);if(!encode_description(out))return error::io;required=out.size();}
                if(capacity<required)return error::too_large;
                bool published=phase==open::state::published;
                if(action==operation::take&&phase!=open::state::ready&&!published)
                    return phase==open::state::cancelled||phase==open::state::failed?
                        current.state.receipt.result==communication::error::ok?error::io:convert(current.state.receipt.result):error::again;
                if(action==operation::cancel&&published)return error::again;
                if(action==operation::release&&(current.state.endpoint_live||current.state.receipt.cleanup_debt||current.state.receipt.observed==communication::effect::unknown||
                    (phase!=open::state::cancelled&&phase!=open::state::failed&&phase!=open::state::released)))return error::again;
                admission_claim claim{capability,name,cleanup?0:input.size()+required,false,false,cleanup};
                bool debit=!cleanup&&!(action==operation::take&&published);
                group_lease admission;status=permissions->admit_group(who,{&claim,1},admission,false,debit);if(status!=error::success)return status;
                group_lease original;handle prepared{};
                if(action==operation::take)
                {
                    status=scope(*row,current,original);if(status!=error::success)return status;
                    if(published){prepared=current.state.published;if(!prepared||!current.state.endpoint_live||current.state.receipt.cleanup_debt)return error::io;}
                    else {status=convert(row->actual->prepare(who,request,prepared));if(status!=error::success)return status;if(!prepared)return error::io;}
                }
                if(checkpoint())return error::cancelled;
                group_lease charged;status=permissions->admit_group(who,{&claim,1},charged,debit||cleanup);if(status!=error::success)return status;
                std::array<std::uint8_t,512> storage;wire::writer out{storage};
                if(action==operation::take)
                {
                    open::published result;status=convert(row->actual->publish(who,request,original,prepared,result));if(status!=error::success)return status;
                    if(result.kind!=row->kind||result.handle!=prepared||result.creation.request_id!=request||result.creation.key!=current.origin.key||!open::encode(out,result))return error::io;
                }
                else if(action==operation::cancel)
                {
                    communication_creation::view result;status=convert(row->actual->cancel(who,request,result));if(status!=error::success)return status;
                    if(result.kind!=row->kind||result.receipt.request_id!=request||result.receipt.key!=current.origin.key||
                        result.receipt.status!=open::state::cancelled||!open::encode(out,result.receipt))return error::io;
                }
                else if(action==operation::release)
                {status=convert(row->actual->release(who,request));if(status!=error::success)return status;if(!open::encode_release(out,true))return error::io;}
                else if(!encode_description(out))return error::io;
                if(out.size()!=required)return error::io;
                if(checkpoint())return error::cancelled;
                if(!deliver(out.written()))return error::io;
                if(checkpoint())return error::cancelled;
                group_lease after;status=permissions->admit_group(who,{&claim,1},after,false);if(status!=error::success)return status;
                return action==operation::take?scope(*row,current,original):error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /// @brief Fence new calls without waiting, discarding receipts or touching providers.
        void retire()noexcept{if(same_process()){retired=true;workspace.retire();}}
    };
}
