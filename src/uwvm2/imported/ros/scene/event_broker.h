/** @file
 * @brief Finite owned stream handles for central scene-broker integration.
 * @details This dispatcher does not decode guest authentication. The central
 * broker supplies the current stream owner and resolves origin source_ref pairs
 * under resource.observe leases before open/poll. All stream plans are immutable
 * before first open. Close affects only the stream, and failed host output for
 * open must close the just-created handle before releasing its admission charge.
 */
#pragma once
#include "event_streams.h"

namespace uwvm2::imported::ros::scene::events
{
    class registry
    {
        struct entry
        {
            handle id{};
            std::shared_ptr<stream> value;
        };

        struct scope
        {
            plan policy;
            ::uwvm2::utils::container::std_vector<entry> entries;
            bool revoked{};
        };

        std::mutex mutex;
        ::uwvm2::utils::container::std_vector<scope> scopes;
        std::size_t capacity{};
        bool frozen{}, retired{};
        inline static std::atomic<handle> identities{1};
#if defined(__linux__)
        ::pid_t process{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process == ::getpid();
#else
            return true;
#endif
        }

        static handle allocate() noexcept
        {
            auto next = identities.load();
            while(next && next != UINT64_MAX)
            {
                if(identities.compare_exchange_weak(next, next + 1)) { return next; }
            }
            return 0;
        }

        scope* find(owner authority)
        {
            for(auto& scope: scopes)
            {
                if(scope.policy.authority == authority) { return &scope; }
            }
            return nullptr;
        }

    public:
        explicit registry(std::size_t max_profiles = 0) : capacity(max_profiles) { scopes.reserve(capacity); }

        error configure(plan policy, std::size_t max_streams)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(frozen || retired) { return error::denied; }
            if(!policy.authority || !communication::valid(policy.selected) || !max_streams || !policy.max_sources || policy.max_sources > 64 ||
               !policy.max_events || policy.max_sources > policy.max_events || !policy.fields || (policy.fields & ~std::uint64_t{3}) ||
               !policy.max_deadline_ns || policy.max_deadline_ns > INT64_MAX)
            {
                return error::invalid;
            }
            if(scopes.size() == capacity) { return error::again; }
            if(find(policy.authority)) { return error::invalid; }
            try
            {
                scope row;
                row.policy = std::move(policy);
                row.entries.resize(max_streams);
                scopes.push_back(std::move(row));
                return error::ok;
            }
            catch(std::bad_alloc const&)
            {
                return error::no_memory;
            }
            catch(std::length_error const&)
            {
                return error::too_large;
            }
        }

        error open(owner caller, profile const& selected, std::span<binding const> sources, void* argument, check admit, handle& output)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            frozen = true;
            auto scope = find(caller);
            if(retired || !scope || scope->revoked || scope->policy.selected != selected) { return error::denied; }
            entry* free{};
            for(auto& row: scope->entries)
            {
                if(!row.value)
                {
                    free = &row;
                    break;
                }
            }
            if(!free) { return error::again; }
            try
            {
                auto id = allocate();
                if(!id) { return error::again; }
                auto value = ::uwvm2::utils::container::make_shared<stream>();
                auto status = value->configure(scope->policy, sources, argument, admit);
                if(status != error::ok) { return status; }
                free->id = id;
                free->value = std::move(value);
                output = id;
                return error::ok;
            }
            catch(std::bad_alloc const&)
            {
                return error::no_memory;
            }
            catch(std::length_error const&)
            {
                return error::too_large;
            }
        }

        error poll(owner caller, handle id, std::size_t limit, std::uint64_t deadline, void* argument, check admit, void* destination, sink deliver)
        {
            if(!same_process()) { return error::denied; }
            std::shared_ptr<stream> value;
            {
                std::lock_guard lock{mutex};
                auto scope = find(caller);
                if(retired || !scope || scope->revoked) { return error::denied; }
                for(auto const& row: scope->entries)
                {
                    if(row.id == id && id)
                    {
                        value = row.value;
                        break;
                    }
                }
            }
            if(!value) { return error::bad_handle; }
            return value->poll(caller, limit, deadline, argument, admit, destination, deliver);
        }

        error close(owner caller, handle id, bool& output)
        {
            if(!same_process()) { return error::denied; }
            std::shared_ptr<stream> value;
            {
                std::lock_guard lock{mutex};
                auto scope = find(caller);
                if(!scope) { return error::denied; }
                for(auto& row: scope->entries)
                {
                    if(row.id == id && id)
                    {
                        value = std::move(row.value);
                        output = true;
                        if(value) { value->retire(); }
                        return error::ok;
                    }
                }
            }
            return error::bad_handle;
        }

        void revoke(owner caller) noexcept
        {
            if(!same_process()) { return; }
            std::lock_guard lock{mutex};
            frozen = true;
            auto scope = find(caller);
            if(!scope) { return; }
            scope->revoked = true;
            for(auto& row: scope->entries)
            {
                if(row.value) { row.value->retire(); }
            }
        }

        void retire() noexcept
        {
            if(!same_process()) { return; }
            std::lock_guard lock{mutex};
            frozen = true;
            retired = true;
            for(auto& scope: scopes)
            {
                scope.revoked = true;
                for(auto& row: scope.entries)
                {
                    if(row.value) { row.value->retire(); }
                }
            }
        }

        ~registry() { retire(); }
    };
}  // namespace uwvm2::imported::ros::scene::events
