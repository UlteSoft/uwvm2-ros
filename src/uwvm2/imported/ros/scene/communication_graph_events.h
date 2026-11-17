/** @file
 * @brief Real graph-watch readiness tied to an owned revisioned snapshot source.
 * @details Graph payloads still require graph.snapshot admission. resource.observe
 * only reveals that a revision is available; names/types never enter the event.
 * Finishing a full page sequence acknowledges its pinned revision. New discovery
 * while pagination is retained remains ready and cannot rewrite old cursor pages.
 */
#pragma once
#include "communication_graph.h"
#include "event_streams.h"

namespace uwvm2::imported::ros::scene::communication::graph
{
    class watch final : public events::source
    {
        std::shared_ptr<snapshot> provider;
        owner scope;
        handle identity{}, version{1}, observed_revision{};
        bool observed_ready{}, busy{};
#if defined(__linux__)
        ::pid_t process{::getpid()};
#endif
        std::mutex mutex;

    public:
        watch(std::shared_ptr<snapshot> selected, owner authority, handle resource) : provider(std::move(selected)), scope(authority), identity(resource) {}

        error inspect(owner caller, handle resource, handle generation, events::snapshot& output) noexcept override
        {return inspect(caller,resource,generation,output,nullptr);}

        /** @brief Observe with current-call provenance; tokenless access is cached only. */
        error inspect(owner caller,handle resource,handle generation,events::snapshot& output,operation_token const* operation) noexcept
        {
#if defined(__linux__)
            if(process != ::getpid()) { return error::denied; }
#endif
            if(!provider || caller != scope || resource != identity || !identity || generation != scope.generation) { return error::denied; }
            if(operation&&(operation->watch!=identity||operation->action!=permission::resource_observe))return error::denied;
            std::unique_lock lock{mutex};
            if(busy) { return error::again; }
            busy = true;
            lock.unlock();
            handle revision{};
            bool ready{};
            error status{};
            try
            {
                status = operation?provider->observe(scope,revision,ready,operation):provider->cached_observation(scope,revision,ready);
            }
            catch(std::bad_alloc const&)
            {
                status = error::no_memory;
            }
            catch(...)
            {
                status = error::io;
            }
            lock.lock();
            busy = false;
            if(status != error::ok) { return status; }
            if(revision != observed_revision || ready != observed_ready)
            {
                if(version == UINT64_MAX) { return error::too_large; }
                ++version;
                observed_revision = revision;
                observed_ready = ready;
            }
            output = {generation, version, ready, false};
            return error::ok;
        }
    };
}  // namespace uwvm2::imported::ros::scene::communication::graph
