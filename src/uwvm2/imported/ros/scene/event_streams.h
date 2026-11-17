/** @file
 * @brief Bounded transactional observation of explicitly bound trusted sources.
 * @details Streams never inherit authority from a resource handle. The broker
 * resolves every origin grant and resource, holds each resource.observe lease,
 * and supplies a source object that rechecks ownership/generation internally.
 * A stream exposes readiness and terminal status only; payload observation is
 * deliberately absent from this schema. Topic/service take remains separately
 * authorized. Default plans, owners and field masks grant nothing. Poll is a
 * nonblocking observation with an absolute steady deadline, not a busy wait.
 */
#pragma once
#include "communication.h"
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>

namespace uwvm2::imported::ros::scene::events
{
    using communication::error;
    using communication::handle;
    using communication::owner;
    using communication::profile;
    enum class field : std::uint64_t
    {
        readiness = 1,
        terminal = 2
    };

    struct snapshot
    {
        handle generation{}, revision{};
        bool ready{}, terminal{};
    };

    /** @brief Trusted source adapter; input triples are not guest credentials.
     * @details A successful inspection is owned and generation-fenced. No source
     * may return underlying payload, arbitrary peer identities or native pointers.
     * Revision is a monotonic source observation counter and must never wrap.
     */
    struct source
    {
        virtual ~source() = default;
        virtual error inspect(owner, handle, handle, snapshot&) noexcept = 0;
    };

    struct binding
    {
        owner origin;
        handle resource{}, generation{};
        std::shared_ptr<source> provider;
    };

    /** @brief Per-call trusted observation callback, never retained by the stream.
     * @details The broker supplies the actual caller and exact bound capability
     * through its argument. A native observer must receive a newly validated
     * origin token on each configure/poll. The tokenless source fallback is for
     * local cached observations; it cannot start isolated native discovery.
     */
    using observer = error (*)(void*,binding const&,snapshot&) noexcept;

    struct plan
    {
        owner authority;
        profile selected;
        std::size_t max_sources{}, max_events{};
        std::uint64_t fields{}, max_deadline_ns{};
    };

    struct event
    {
        handle sequence{}, source_index{}, source_generation{}, lost_before{};
        std::uint64_t fields{};
        bool ready{}, terminal{};
    };

    using sink = bool (*)(void*, std::span<event const>);
    using check = bool (*)(void*, owner, handle) noexcept;

    /** @brief Fixed stream with per-origin observation permission rechecks.
     * @details Trusted setup allocates both observed-source and pending-event
     * storage. Snapshot changes are retained across failed guest output. Poll
     * never consumes any source data. It delivers a bounded prefix of complete
     * records without splitting a record; explicit close wakes no native resource
     * and leaves all observed sources untouched. A caller's failed/throwing copy
     * leaves sequence/revision delivery positions unchanged.
     */
    class stream
    {
        struct tracked
        {
            binding selected;
            snapshot previous;
            bool observed{};
        };

        std::mutex mutex;
        plan configuration;
        ::uwvm2::utils::container::std_vector<tracked> sources;
        ::uwvm2::utils::container::std_vector<event> pending;
        ::uwvm2::utils::container::std_vector<snapshot> candidates, staged;
        handle next_sequence{1};
        bool closed{}, busy{}, configured{};
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

    public:
        stream() = default;

        /** @brief Required logical collection storage for an immutable stream.
         * @details Fixed object/control-block metadata is bounded by stream slots
         * separately. This checks arithmetic before any vector reservation; it is
         * not an estimate of allocator pages, ROS memory or total process RSS.
         */
        static bool required_workspace(std::uint64_t sources,std::uint64_t events,std::uint64_t& bytes) noexcept
        {
            constexpr auto per_source=sizeof(tracked)+2*sizeof(snapshot);
            if(!sources||!events||sources>64||sources>events||sources>UINT64_MAX/per_source||events>UINT64_MAX/sizeof(event))return false;
            auto source_bytes=sources*per_source,event_bytes=events*sizeof(event);
            if(source_bytes>UINT64_MAX-event_bytes)return false;
            bytes=source_bytes+event_bytes;return true;
        }

        /** @brief Install immutable source capabilities before publication.
         * @details Duplicate origin/resource pairs are rejected. Every binding
         * must be inspected under its own already-admitted observation lease.
         * The callback supplied here and to poll is trusted broker code, never a
         * guest callback; it must check resource.observe, not merely handle shape.
         */
        error configure(plan selected, std::span<binding const> bindings, void* argument, check admit,observer observe=nullptr)
        {
            if(!same_process()) { return error::denied; }
            std::unique_lock lock{mutex};
            if(configured || closed) { return error::denied; }
            if(busy)return error::again;
            if(!selected.authority || !communication::valid(selected.selected) || !selected.max_sources || !selected.max_events ||
               selected.max_sources > selected.max_events || bindings.empty() || bindings.size() > selected.max_sources || !selected.fields ||
               (selected.fields & ~std::uint64_t{3}) || !selected.max_deadline_ns || selected.max_deadline_ns > INT64_MAX || !admit)
            {
                return error::invalid;
            }
            busy=true;
            struct completion
            {stream* self;std::unique_lock<std::mutex>& held;~completion(){if(!held.owns_lock())held.lock();self->busy=false;}} done{this,lock};
            lock.unlock();
            ::uwvm2::utils::container::std_vector<tracked> checked;
            checked.reserve(bindings.size());
            for(auto const& binding: bindings)
            {
                if(!binding.origin || binding.origin.environment != selected.authority.environment || !binding.resource || !binding.generation ||
                   !binding.provider || !admit(argument, binding.origin, binding.resource))
                {
                    return error::denied;
                }
                for(auto const& previous: checked)
                {
                    if(previous.selected.origin == binding.origin && previous.selected.resource == binding.resource) { return error::invalid; }
                }
                snapshot observed;
                auto result = observe?observe(argument,binding,observed):binding.provider->inspect(binding.origin, binding.resource, binding.generation, observed);
                if(result != error::ok) { return result; }
                if(observed.generation != binding.generation || !observed.revision) { return error::stale; }
                // The first poll publishes an actual current observation, not
                // the setup snapshot. Configuration consumes no source data.
                checked.push_back({binding, {}, false});
            }
            lock.lock();
            if(closed)return error::cancelled;
            pending.reserve(selected.max_events);
            candidates.resize(bindings.size());
            staged.resize(bindings.size());
            sources = std::move(checked);
            configuration = std::move(selected);
            configured = true;
            return error::ok;
        }

        error poll(owner caller, std::size_t limit, std::uint64_t deadline, void* authority_argument, check admit, void* sink_argument, sink deliver,
                   observer observe=nullptr)
        {
            if(!same_process()) { return error::denied; }
            std::unique_lock lock{mutex};
            if(!configured || closed) { return error::cancelled; }
            if(caller != configuration.authority || !admit || !deliver) { return error::denied; }
            if(busy) { return error::again; }
            if(!limit || limit > configuration.max_events) { return error::invalid; }
            auto time = communication::now();
            if(deadline <= time || deadline > INT64_MAX || deadline - time > configuration.max_deadline_ns) { return error::timeout; }
            busy = true;
            lock.unlock();
            error status = error::ok;
            // Revalidate all origin authorities even when replaying a pending
            // output transaction. Revoked payload visibility cannot survive in a
            // previously staged batch merely because the stream grant survives.
            for(std::size_t i{}; i < sources.size(); ++i)
            {
                auto const& selected = sources[i].selected;
                if(!admit(authority_argument, selected.origin, selected.resource))
                {
                    status = error::denied;
                    break;
                }
                auto result = observe?observe(authority_argument,selected,candidates[i]):selected.provider->inspect(selected.origin, selected.resource, selected.generation, candidates[i]);
                if(result != error::ok)
                {
                    status = result;
                    break;
                }
                if(candidates[i].generation != selected.generation || !candidates[i].revision ||
                   (sources[i].observed && candidates[i].revision < sources[i].previous.revision))
                {
                    status = error::stale;
                    break;
                }
            }
            lock.lock();
            if(closed) { status = error::cancelled; }
            if(status != error::ok)
            {
                busy = false;
                return status;
            }
            if(pending.empty())
            {
                std::copy(candidates.begin(), candidates.end(), staged.begin());
                if(next_sequence > UINT64_MAX - sources.size())
                {
                    busy = false;
                    return error::too_large;
                }
                for(std::size_t i{}; i < sources.size(); ++i)
                {
                    auto const& row = sources[i];
                    auto const& value = candidates[i];
                    if(row.observed && value.revision == row.previous.revision && !((configuration.fields & 1) && value.ready) &&
                       !((configuration.fields & 2) && value.terminal))
                    {
                        continue;
                    }
                    bool ready = (configuration.fields & 1) && value.ready, terminal = (configuration.fields & 2) && value.terminal;
                    // No unselected field enters the record, including when a
                    // source revision changed only because of hidden state.
                    if(row.observed && ready == ((configuration.fields & 1) && row.previous.ready) &&
                       terminal == ((configuration.fields & 2) && row.previous.terminal) && !ready && !terminal)
                    {
                        continue;
                    }
                    pending.push_back({next_sequence + pending.size(), i + 1, value.generation, 0, configuration.fields, ready, terminal});
                }
            }
            if(pending.empty())
            {
                for(std::size_t i{}; i < sources.size(); ++i)
                {
                    sources[i].previous = candidates[i];
                    sources[i].observed = true;
                }
                busy = false;
                return error::again;
            }
            auto delivered_count = std::min(limit, pending.size());
            if(communication::now() >= deadline)
            {
                busy = false;
                return error::timeout;
            }
            lock.unlock();
            bool copied{};
            try
            {
                copied = deliver(sink_argument, std::span<event const>{pending}.first(delivered_count));
            }
            catch(...)
            {
            }
            error checked = error::ok;
            for(auto const& row: sources)
            {
                if(!admit(authority_argument, row.selected.origin, row.selected.resource))
                {
                    checked = error::denied;
                    break;
                }
                snapshot current;
                auto status = row.selected.provider->inspect(row.selected.origin, row.selected.resource, row.selected.generation, current);
                if(status != error::ok || current.generation != row.selected.generation)
                {
                    checked = status == error::ok ? error::stale : status;
                    break;
                }
            }
            lock.lock();
            busy = false;
            if(closed) { return error::cancelled; }
            if(checked != error::ok) { return checked; }
            if(communication::now() >= deadline) { return error::timeout; }
            if(!copied) { return error::io; }
            next_sequence += delivered_count;
            // Level readiness remains observable until the source is drained.
            // A successful prefix advances only those source positions; other
            // complete records remain staged with their original sequence.
            for(std::size_t i{}; i < delivered_count; ++i)
            {
                auto index = pending[i].source_index - 1;
                sources[index].previous = staged[index];
                sources[index].observed = true;
            }
            pending.erase(pending.begin(), pending.begin() + static_cast<std::ptrdiff_t>(delivered_count));
            return error::ok;
        }

        error close(owner caller) noexcept
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(!configured || caller != configuration.authority) { return error::denied; }
            closed = true;
            return error::ok;
        }

        void retire() noexcept
        {
            if(!same_process()) { return; }
            std::lock_guard lock{mutex};
            closed = true;
        }
    };
}  // namespace uwvm2::imported::ros::scene::events
