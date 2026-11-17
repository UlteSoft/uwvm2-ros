/** @file
 * @brief Alias-owned transform sessions whose reservations survive close and callbacks.
 * @details Trusted binding performs no native I/O. A session independently owns
 * its frame cache; identical resource labels never merge histories. All guest
 * operations still need the enclosing broker's exact transform/clock authority.
 * The registry checks alias, handle kind and process before returning a borrow.
 * Closing removes guest reachability and retires the session, but its retained
 * slot/data/query/scratch charges remain until the last active borrow is gone.
 * This prevents close/open loops from defeating bounded memory reservations.
 */
#pragma once
#include "transform_queries.h"
#include <uwvm2/utils/container/shared.h>
#include <atomic>

namespace uwvm2::imported::ros::scene::transforms
{
    /** @brief Maximum simultaneous retained sessions and carrier-copy capacity.
     * @details Each session reserves the complete configured history data bank,
     * scratch bank and query-slot allocation. Aggregate budgets cover those
     * reservations, not only currently initialized samples. Metadata has one
     * statically bounded session/object/control record per retained slot plus
     * one trusted configuration per alias. carrier_bytes separately bounds
     * concurrent owned canonical-request copies; it is not a rate quota.
     */
    struct plan
    {
        session_plan session;
        std::uint64_t sessions{},data_bytes{},workspace_bytes{},request_bytes{},carrier_bytes{};
    };
    struct usage
    {
        std::uint64_t retained_sessions{},open_handles{},samples{},queries{},pending{},data_bytes{},workspace_bytes{},request_bytes{},carrier_bytes{};
        bool retired{};
    };
    inline bool valid(plan const& value) noexcept
    {
        std::uint64_t bank{},requests{};
        if(value.sessions==UINT64_MAX||!value.session.profile.identity.size||value.session.profile.identity.size>buffers::maximum_identity||
            !wire::utf8({reinterpret_cast<std::uint8_t const*>(value.session.profile.identity.bytes.data()),value.session.profile.identity.size})||
            value.session.profile.identity.view().find('\0')!=std::string_view::npos||value.session.deadline_ns>INT64_MAX||
            value.session.request_bytes>INT64_MAX||!cache::valid(value.session.history)||!session::request_storage(value.session,requests)||
            requests>value.session.request_bytes||!cache::bank_bytes(value.session.history,bank)||
            value.data_bytes>INT64_MAX||value.workspace_bytes>INT64_MAX||value.request_bytes>INT64_MAX||value.carrier_bytes>INT64_MAX)return false;
        auto fits=[&](std::uint64_t item,std::uint64_t total){return !item||value.sessions<=total/item;};
        return fits(value.session.history.data_bytes,value.data_bytes)&&fits(value.session.history.workspace_bytes,value.workspace_bytes)&&
            fits(value.session.request_bytes,value.request_bytes);
    }
    class registry
    {
        struct account
        {
            plan limits;
            std::atomic<std::uint64_t> retained{},carriers{};
            std::atomic<bool> retired{};
#if defined(__linux__)
            ::pid_t process{::getpid()};
#endif
            explicit account(plan value):limits{std::move(value)}{}
        };
        struct reservation
        {
            std::shared_ptr<account> owner;
            reservation()=default;explicit reservation(std::shared_ptr<account> value):owner{std::move(value)}{}
            reservation(reservation const&)=delete;reservation& operator=(reservation const&)=delete;
            reservation(reservation&&)=default;reservation& operator=(reservation&&)=delete;
            ~reservation(){if(owner)owner->retained.fetch_sub(1);}
        };
        struct object
        {
            reservation capacity; // destroyed after the actual session allocation
            session value;
            resources::handle id{};
            bool published{};
            explicit object(reservation&& charge):capacity{std::move(charge)}{}
        };
        mutable std::mutex mutex_;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,std::shared_ptr<account>,std::less<>> accounts_;
        ::uwvm2::utils::container::std_map<resources::handle,std::shared_ptr<object>> objects_;
        bool retired_{};
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
        std::shared_ptr<account> find(std::string_view alias) const noexcept
        {auto it=accounts_.find(alias);return it==accounts_.end()?nullptr:it->second;}
    public:
        class carrier_lease
        {
            friend class registry;
            std::shared_ptr<account> owner_;std::uint64_t bytes_{};
        public:
            carrier_lease()=default;carrier_lease(carrier_lease const&)=delete;carrier_lease& operator=(carrier_lease const&)=delete;
            carrier_lease(carrier_lease&& other) noexcept:owner_{std::move(other.owner_)},bytes_{std::exchange(other.bytes_,0)}{}
            carrier_lease& operator=(carrier_lease&& other) noexcept
            {if(this!=&other){reset();owner_=std::move(other.owner_);bytes_=std::exchange(other.bytes_,0);}return *this;}
            ~carrier_lease(){reset();}
            void reset() noexcept{if(owner_)owner_->carriers.fetch_sub(bytes_);owner_.reset();bytes_=0;}
            bool live() const noexcept
            {
                if(!owner_)return false;
#if defined(__linux__)
                if(owner_->process!=::getpid())return false;
#endif
                return !owner_->retired.load();
            }
            explicit operator bool() const noexcept{return static_cast<bool>(owner_);}
        };
        using open_sink=::uwvm2::utils::container::function_ref<bool(resources::handle)>;
        registry()=default;registry(registry const&)=delete;registry& operator=(registry const&)=delete;
        ~registry(){retire();}
        /** @brief Register inert trusted limits; no resource or capability is opened. */
        error bind(::uwvm2::utils::container::std_string alias,plan selected) noexcept
        {
            if(!same_process())return error::denied;
            if(alias.empty()||alias.size()>255||!valid(selected))return error::invalid;
            std::lock_guard lock{mutex_};if(retired_)return error::cancelled;if(accounts_.contains(alias))return error::conflict;
            try{accounts_.emplace(std::move(alias),::uwvm2::utils::container::make_shared<account>(std::move(selected)));return error::success;}
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
        }
        /** @brief Copy only preflight bounds; no large plan copy or allocation is needed. */
        error limits(std::string_view alias,contract::operation action,std::size_t& bound,buffers::profile& profile) const noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto owner=find(alias);if(!owner)return error::bad_handle;
            if(retired_||owner->retired.load())return error::cancelled;
            auto size=contract::request_bound(action,owner->limits.session.history.max_batch);if(!size)return error::unsupported;
            bound=size;profile=owner->limits.session.profile;return error::success;
        }
        /** @brief Reserve before copying a large guest request; token outlives the actual bytes. */
        error reserve_carrier(std::string_view alias,std::uint64_t bytes,carrier_lease& out) noexcept
        {
            if(!same_process())return error::denied;
            if(out||!bytes)return error::invalid;
            std::lock_guard lock{mutex_};auto owner=find(alias);if(!owner)return error::bad_handle;
            if(retired_||owner->retired.load())return error::cancelled;
            auto used=owner->carriers.load();
            do{if(used>owner->limits.carrier_bytes||bytes>owner->limits.carrier_bytes-used)return error::too_large;}
            while(!owner->carriers.compare_exchange_weak(used,used+bytes));
            out.owner_=std::move(owner);out.bytes_=bytes;return error::success;
        }
        /** @brief Configure a private cache and publish its handle only after successful copy. */
        error open(std::string_view alias,buffers::profile const& profile,resources::handle clock_capability,clock_context clock,
                   std::uint64_t grant_limit,open_sink deliver) noexcept
        {
            if(!same_process())return error::denied;
            if(!deliver)return error::invalid;
            std::shared_ptr<account> owner;
            {
                std::lock_guard lock{mutex_};owner=find(alias);if(!owner)return error::bad_handle;
                if(retired_||owner->retired.load())return error::cancelled;
                if(profile!=owner->limits.session.profile)return error::denied;
                auto used=owner->retained.load(),limit=std::min(grant_limit,owner->limits.sessions);
                do{if(used>=limit)return error::too_large;}while(!owner->retained.compare_exchange_weak(used,used+1));
            }
            reservation reserved{owner};std::shared_ptr<object> value;
            try
            {
                value=::uwvm2::utils::container::make_shared<object>(std::move(reserved));
                auto status=value->value.configure(owner->limits.session,clock_capability,clock);if(status!=error::success)return status;
                session_usage state;status=value->value.inspect(state);if(status!=error::success)return status;value->id=state.id;
                {
                    std::lock_guard lock{mutex_};if(retired_||owner->retired.load())return error::cancelled;
                    if(!objects_.emplace(value->id,value).second)return error::conflict;
                }
            }
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
            bool copied{};try{copied=deliver(value->id);}catch(...){copied=false;}
            std::lock_guard lock{mutex_};
            if(retired_||owner->retired.load()||!copied)
            {objects_.erase(value->id);value->value.retire();return retired_||owner->retired.load()?error::cancelled:error::io;}
            value->published=true;return error::success;
        }
        /** @brief Borrow a live private session; its reservation survives registry removal. */
        error borrow(std::string_view alias,resources::handle id,std::shared_ptr<session>& out) const noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto owner=find(alias);if(!owner)return error::bad_handle;
            if(retired_||owner->retired.load())return error::cancelled;
            auto it=objects_.find(id);
            if(it==objects_.end()||!it->second->published||it->second->capacity.owner!=owner)return error::bad_handle;
            out=std::shared_ptr<session>{it->second,&it->second->value};return error::success;
        }
        /** @brief Resolve a request within this exact alias using bounded cached membership.
         * @details At most configured sessions times request slots are visited.
         * No clock, native query or foreign alias is inspected. This supports
         * generic request.describe/release without inventing a global handle
         * authority or retaining released request allocations in an index.
         */
        error borrow_request(std::string_view alias,resources::handle id,std::shared_ptr<session>& out) const noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto owner=find(alias);if(!owner)return error::bad_handle;
            if(retired_||owner->retired.load())return error::cancelled;
            for(auto const& [identity,item]:objects_)
            {
                (void)identity;if(!item->published||item->capacity.owner!=owner)continue;
                bool found{};auto status=item->value.owns_request(id,found);if(status!=error::success)return status;
                if(found){out=std::shared_ptr<session>{item,&item->value};return error::success;}
            }
            return error::bad_handle;
        }
        /** @brief Retire before removing reachability; active calls retain all storage charges. */
        error close(std::string_view alias,resources::handle id) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto owner=find(alias);if(!owner)return error::bad_handle;
            auto it=objects_.find(id);if(it==objects_.end()||!it->second->published||it->second->capacity.owner!=owner)return error::bad_handle;
            it->second->value.retire();objects_.erase(it);return error::success;
        }
        void revoke(std::string_view alias) noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex_};auto owner=find(alias);if(!owner)return;owner->retired=true;
            for(auto it=objects_.begin();it!=objects_.end();)
                if(it->second->capacity.owner==owner){it->second->value.retire();it=objects_.erase(it);}else ++it;
        }
        void retire() noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex_};retired_=true;
            for(auto const& [alias,owner]:accounts_){(void)alias;owner->retired=true;}
            for(auto const& [id,value]:objects_){(void)id;value->value.retire();}
            objects_.clear();
        }
        /** @brief Cache-only inspection, including closed-but-retained reservations. */
        error inspect(std::string_view alias,usage& out) const noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex_,std::try_to_lock};if(!lock.owns_lock())return error::again;
            auto owner=find(alias);if(!owner)return error::bad_handle;
            usage value;value.retained_sessions=owner->retained.load();value.carrier_bytes=owner->carriers.load();value.retired=owner->retired.load();
            std::uint64_t bank{},requests{};
            if(!cache::bank_bytes(owner->limits.session.history,bank)||!session::request_storage(owner->limits.session,requests))return error::invalid;
            value.data_bytes=value.retained_sessions*owner->limits.session.history.data_bytes;
            value.workspace_bytes=value.retained_sessions*owner->limits.session.history.workspace_bytes;
            value.request_bytes=value.retained_sessions*owner->limits.session.request_bytes;
            for(auto const& [id,item]:objects_)
            {
                (void)id;if(item->capacity.owner!=owner||!item->published)continue;
                session_usage state;auto status=item->value.inspect(state);if(status!=error::success)return status;
                ++value.open_handles;value.samples+=state.history.samples;value.queries+=state.requests;value.pending+=state.pending;
            }
            out=value;return error::success;
        }
    };
}
