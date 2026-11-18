/** @file
 * @brief Exact per-module authority for already provisioned driver channels.
 * @details The trusted host registers one channel for one alias and module.
 * Registration grants nothing. Group capabilities cannot make another module
 * use that channel. First use seals runtime incarnation; replacement cannot
 * inherit it. Bounds cover all retained buffers and the finite binding count.
 * Lock order: authority, dispatcher, session. Every syscall is nonblocking.
 * No native callback, driver implementation or process-control API is accepted.
 */
#pragma once
#include "authority.h"
#include "driver_contract.h"
#include "driver_session.h"
#include "schema_contract.h"
#include "platform_extension_policy.h"
#include "attachment_registry.h"
namespace uwvm2::imported::ros::scene
{
    class driver_dispatch
    {
        struct entry
        {
            ::uwvm2::utils::container::std_string alias,module,resource;drivers::session connection;
            drivers::limits capacity;drivers::protocol::binding identity;caller owner{};erased_buffer reply;
            schema_contract::operation const* schema{};schema_contract::limits decode;bool platform_allowed{true};
        };
        mutable std::mutex mutex_;
        ::uwvm2::utils::container::std_vector<::uwvm2::utils::container::std_unique_ptr<entry>> entries_;
        std::size_t maximum_{},bytes_{},used_{};bool configured_{},frozen_{},retired_{};
        struct declaration {int descriptor;drivers::credentials peer;};
        // Review metadata has no per-binding allocation or effect on runtime route budgets.
        ::uwvm2::utils::container::std_vector<declaration> declarations_;
        bool const declarations_only_{};
    public:
        explicit driver_dispatch(bool declarations_only=false):declarations_only_{declarations_only}{}
        /** @brief Visit immutable registration fields only. The visitor never touches the live channel. */
        template<class Visitor> bool visit_registrations(Visitor visit)const
        {
            std::lock_guard lock{mutex_};
            if(!declarations_only_)return false;
            for(std::size_t i{};i<entries_.size();++i)
            {
                auto const& row=entries_[i];
                if(!row->schema || !visit(row->alias,row->module,declarations_[i].peer,row->identity,row->capacity,row->schema->name,row->decode))
                    return false;
            }
            return true;
        }
        /** @brief Claim all privately pinned endpoints during global static freeze.
         * @details No guest can call this method. All environment registries
         * share the same temporary ownership set before any broker is published.
         */
        error claim_endpoints(attachment_registry& registry)const noexcept
        {
            std::lock_guard lock{mutex_};
            for(std::size_t i{};i<entries_.size();++i)
            {
                auto const& row=entries_[i];
                auto status=declarations_only_?registry.descriptor(declarations_[i].descriptor):registry.endpoint(row->connection.endpoint_identity());
                if(status!=error::success)return status;
                status=registry.session(row->identity.session);if(status!=error::success)return status;
            }
            return error::success;
        }
        error budget(std::size_t entries,std::size_t bytes)noexcept
        {std::lock_guard lock{mutex_};if(configured_||frozen_||retired_)return error::conflict;maximum_=entries;bytes_=bytes;configured_=true;return error::success;}
        /** @brief Duplicate a trusted connected descriptor without consuming peer data. */
        error bind(std::string_view alias,std::string_view module,std::string_view resource,int fd,drivers::credentials peer,
            drivers::protocol::binding const& identity,drivers::limits capacity)
        {
            std::lock_guard lock{mutex_};if(frozen_||retired_||!configured_)return error::denied;
            if(!identifier(alias)||module.empty()||module.size()>255||resource.empty()||resource.size()>255||
               fd<0||!drivers::valid(peer)||!drivers::protocol::valid(identity)||!drivers::valid(capacity))return error::invalid;
            for(auto const& row:entries_)if(row->alias==alias||row->identity.session==identity.session)return error::conflict;
            auto needed=sizeof(entry)+drivers::protocol::header_bytes+capacity.request_bytes+2*capacity.response_bytes+16+alias.size()+module.size()+resource.size();
            if(capacity.tls_identity_fd>=0)needed+=attachment_tls::reservation(drivers::protocol::header_bytes+std::max(capacity.request_bytes,capacity.response_bytes));
            if(entries_.size()>=maximum_||used_>bytes_||needed>bytes_-used_)return error::too_large;
            auto row=::uwvm2::utils::container::make_unique<entry>();row->alias=alias;row->module=module;row->resource=resource;row->capacity=capacity;row->identity=identity;
            if(!declarations_only_)
            {
                row->reply.resize_for_overwrite(capacity.response_bytes+16);
                auto result=row->connection.prepare(fd,peer,identity,capacity);if(result!=error::success)return result;
                for(auto const& other:entries_)if(row->connection.same_endpoint(other->connection))return error::conflict;
            }
            entries_.emplace_back(std::move(row));
            if(declarations_only_)
            {
                try{declarations_.push_back({fd,peer});}
                catch(...){entries_.pop_back();throw;}
            }
            used_+=needed;return error::success;
        }
        bool contains(std::string_view alias)const noexcept
        {std::lock_guard lock{mutex_};for(auto const& row:entries_)if(row->alias==alias)return row->schema!=nullptr;return false;}
        /** @brief Select a compiled canonical operation shape before freezing a channel.
         * @details No callback, arbitrary FFI or dynamic parser is registered.
         * Shapes bound the complete tree. Receiver-side contract admission must
         * independently enforce physical limits, authority and exclusive leases;
         * shape validation alone is never an actuation safety guarantee.
         */
        error schema(std::string_view alias,std::string_view operation,schema_contract::limits bounds)noexcept
        {
            std::lock_guard lock{mutex_};if(frozen_||retired_)return error::denied;
            auto* shape=schema_contract::find(operation);
            if(!shape||operation.starts_with("driver.")||operation.starts_with("module.")||operation.starts_with("permission.")||
               operation.starts_with("vm.")||operation.starts_with("component.")||!bounds.nodes||bounds.nodes>65536||bounds.elements>65536||!bounds.depth||bounds.depth>64)return error::invalid;
            for(auto& row:entries_)if(row->alias==alias)
            {if(row->schema)return error::conflict;bounds.bytes=std::max(row->capacity.request_bytes,row->capacity.response_bytes);row->decode=bounds;row->schema=shape;return error::success;}
            return error::denied;
        }
        /** @brief Reject unauthorized callers before allocating or reading guest bytes. */
        error preflight(authority& permissions,caller who,std::string_view module,drivers::operation action,handle cap,std::size_t bytes)
        {
            if(static_cast<unsigned>(action)>=drivers::names.size())return error::unsupported;
            lease held;auto status=action==drivers::operation::disconnect?
                permissions.admit(who,cap,drivers::names[static_cast<unsigned>(action)],bytes,held,false):
                permissions.preflight(who,cap,drivers::names[static_cast<unsigned>(action)],bytes,held);
            if(status!=error::success)return status;
            auto const& grant=held.specification();
            if(grant.kind!=purpose::driver||grant.binding!="registered-driver")return error::denied;
            std::lock_guard lock{mutex_};if(!frozen_||retired_)return error::denied;
            for(auto const& row:entries_)if(row->alias==grant.alias)
                return row->schema&&row->platform_allowed&&row->module==module&&row->resource==grant.resource&&(!row->owner.instance||
                    (row->owner.instance==who.instance&&row->owner.generation==who.generation))?error::success:error::denied;
            return error::denied;
        }
        /** @brief Apply child restrictions before publication; all validation precedes mutation. */
        error validate_platforms(platform_extensions::policy const& policy)const noexcept
        {
            std::lock_guard lock{mutex_};for(auto const& row:entries_)
                if(!row->schema||!policy.contract_matches(row->alias,row->schema->name))return error::denied;
            return error::success;
        }
        void freeze(platform_extensions::policy const& policy)noexcept
        {std::lock_guard lock{mutex_};if(declarations_only_)return;for(auto& row:entries_)row->platform_allowed=policy.allows(row->alias);frozen_=true;}
        void freeze()noexcept{platform_extensions::policy policy;freeze(policy);}
        void retire()noexcept
        {std::lock_guard lock{mutex_};retired_=true;for(auto& row:entries_)row->connection.disconnect();}
        void revoke(std::string_view alias)noexcept
        {std::lock_guard lock{mutex_};for(auto& row:entries_)if(row->alias==alias)row->connection.disconnect();}
        void retire_member(caller owner)noexcept
        {std::lock_guard lock{mutex_};for(auto& row:entries_)if(row->owner.instance==owner.instance&&row->owner.generation==owner.generation)row->connection.disconnect();}
        /** @brief Detached state query; no packet pump, spawn, close or authority change. */
        bool inspect(std::string_view alias,drivers::snapshot& result)const noexcept
        {std::lock_guard lock{mutex_};for(auto const& row:entries_)if(row->alias==alias){result=row->connection.inspect();return true;}return false;}
        /** @brief Admit original caller, exact operation and route before any channel effect.
         * @details Connect/disconnect requests are empty. Submit contains u64
         * sequence, u64 deadline and one blob; result contains u64 sequence.
         * Describe returns state, sequence, status and effect_unknown as four
         * u64 words. Result returns u64 status and one blob. Submission receipts
         * retain sequences across failed output; result consumption is transactional.
         */
        template<typename Sink>error invoke(authority& permissions,caller who,std::string_view module,drivers::operation action,handle cap,
            wire::bytes input,std::size_t capacity,std::size_t& required,Sink&& deliver)
        {
            if(static_cast<unsigned>(action)>=drivers::names.size())return error::unsupported;
            bool cleanup=action==drivers::operation::disconnect;lease held;
            auto status=permissions.admit(who,cap,drivers::names[static_cast<unsigned>(action)],input.size()+capacity,held,!cleanup);
            if(status!=error::success)return status;
            auto const& grant=held.specification();
            if(grant.kind!=purpose::driver||grant.binding!="registered-driver")return error::denied;
            std::lock_guard lock{mutex_};if(!frozen_||retired_)return error::denied;entry* selected{};
            for(auto const& row:entries_)if(row->alias==grant.alias){selected=row.get();break;}
            if(!selected||!selected->schema||!selected->platform_allowed||selected->module!=module||selected->resource!=grant.resource)return error::denied;
            auto& row=*selected;
            if(row.owner.instance&&(row.owner.instance!=who.instance||row.owner.generation!=who.generation))return error::denied;
            wire::reader in{input};std::uint64_t sequence{},deadline{};wire::bytes payload;
            if(action==drivers::operation::submit)
            {if(!in.u64(sequence)||!in.u64(deadline)||!in.blob(payload)||!in.done()||payload.size()>row.capacity.request_bytes||!schema_contract::validate(payload,row.schema->request,row.decode))return error::invalid;}
            else if(action==drivers::operation::result){if(!in.u64(sequence)||!in.done())return error::invalid;}
            else if(!input.empty())return error::invalid;
            required=action==drivers::operation::describe?32:action==drivers::operation::result?16+row.capacity.response_bytes:8;
            if(capacity<required)return error::too_large;
            if(action!=drivers::operation::describe)row.owner=who;
            auto now=static_cast<std::uint64_t>(resources::steady_now());
            if(action==drivers::operation::connect)status=row.connection.connect(now);
            else if(action==drivers::operation::submit)status=row.connection.submit(sequence,deadline,payload,now);
            else if(action==drivers::operation::disconnect){row.connection.disconnect();status=error::success;}
            else if(action==drivers::operation::result)
                return row.connection.result(sequence,now,[&](std::uint64_t value,wire::bytes body)
                {
                    struct wipe{erased_buffer& value;~wipe(){value.erase();}} clear{row.reply};
                    wire::writer out{row.reply.writable()};out.u64(value);out.blob(body);required=out.size();return out.good()&&deliver(out.written());
                },[&](std::uint64_t,wire::bytes body)noexcept{return schema_contract::validate(body,row.schema->result,row.decode);});
            if(status!=error::success)return status;
            std::array<std::uint8_t,32> bytes{};wire::writer out{bytes};
            if(action==drivers::operation::describe){auto snap=row.connection.inspect();out.u64(static_cast<std::uint64_t>(snap.phase));out.u64(snap.sequence);out.u64(snap.status);out.boolean(snap.effect_unknown);}
            else out.u64(action==drivers::operation::submit?sequence:1);
            required=out.size();return deliver(out.written())?error::success:error::invalid;
        }
    };
}
