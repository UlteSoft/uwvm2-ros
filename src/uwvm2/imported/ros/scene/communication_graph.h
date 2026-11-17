/** @file
 * @brief Filtered actual graph snapshots with bounded revision/cursor recovery.
 * @details Graph data describes untrusted native discovery announcements. Names,
 * domain numbers and type strings are not credentials or executable capability.
 * The trusted source owns a counted native node attachment. Guest requests cannot
 * choose an observer node, peer address, ROS domain or unapproved discovery filter.
 */
#pragma once
#include "communication.h"
#include "wire.h"

namespace uwvm2::imported::ros::scene::communication::graph
{
    enum class kind : std::uint64_t
    {
        node = 1,
        publisher = 2,
        subscription = 4,
        service = 8,
        client = 16
    };

    struct entity
    {
        kind type{};
        ::uwvm2::utils::container::std_string node, endpoint, message_type;
        friend bool operator== (entity const&, entity const&) = default;
    };

    struct plan
    {
        owner authority;
        profile selected;
        handle context{};
        ::uwvm2::utils::container::std_vector<::uwvm2::utils::container::std_string> nodes;
        std::uint64_t fields{};  // kind bits 1..16; type strings require additional bit 32
        std::size_t max_rows{}, max_bytes{}, max_page_items{};
        std::uint64_t cursor_lifetime_ns{}, capture_deadline_ns{};
        friend bool operator==(plan const&,plan const&)=default;
    };

    struct source
    {
        virtual ~source() = default;
        virtual bool live() const noexcept = 0;
        virtual handle context() const noexcept = 0;
        virtual owner authority() const noexcept = 0;
        /// @brief Immutable provider accounting mode; queued captures charge on reserved submission.
        virtual bool queued_captures()const noexcept{return false;}
        virtual error capture(plan const&, ::uwvm2::utils::container::std_vector<entity>&) noexcept = 0;
        /** @brief Capture under this call's complete graph origins.
         * @details Trusted synchronous providers may use the fallback. Isolated
         * queued providers must override it, retain this token in the owned job,
         * and reject tokenless capture. Never retain the first opener's authority
         * as authority for a later observation or hold a lease while waiting.
         */
        virtual error capture(plan const& selected, ::uwvm2::utils::container::std_vector<entity>& out,operation_token const&) noexcept
        {return capture(selected,out);}
    };

    struct page
    {
        handle revision{};
        std::optional<handle> cursor;
        std::span<entity const> entities;
    };

    using sink = bool (*)(void*, page const&);

    class snapshot
    {
        std::mutex mutex;
        plan configuration;
        std::shared_ptr<source> native;
        ::uwvm2::utils::container::std_vector<entity> rows, current_rows;
        handle revision{}, page_revision{}, acknowledged_revision{}, next_cursor{}, pending_cursor{};
        std::size_t next_offset{}, pending_offset{}, pending_count{}, pending_limit{};
        std::optional<handle> pending_request;
        std::uint64_t expires{};
        bool configured{}, retired{}, busy{}, pending{};
        inline static std::atomic<handle> cursors{1};
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
            auto next = cursors.load();
            while(next && next != UINT64_MAX)
            {
                if(cursors.compare_exchange_weak(next, next + 1)) { return next; }
            }
            return 0;
        }

        error refresh(std::unique_lock<std::mutex>& lock,operation_token const* operation=nullptr)
        {
            busy = true;
            auto provider = native;
            auto started = now();
            lock.unlock();
            ::uwvm2::utils::container::std_vector<entity> captured;
            error status{};
            try
            {
                captured.reserve(configuration.max_rows);
                status = operation?provider->capture(configuration,captured,*operation):provider->capture(configuration, captured);
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
            if(retired || !provider->live()) { return error::cancelled; }
            if(status != error::ok) { return status; }
            auto time = now();
            if(time < started || time - started > configuration.capture_deadline_ns) { return error::timeout; }
            if(captured.size() > configuration.max_rows) { return error::too_large; }
            std::size_t extent{};
            for(auto const& row: captured)
            {
                auto bit = static_cast<std::uint64_t>(row.type);
                if(!bit || (bit & (bit - 1)) || (bit & ~std::uint64_t{31}) || !(configuration.fields & bit) ||
                   std::find(configuration.nodes.begin(), configuration.nodes.end(), row.node) == configuration.nodes.end() ||
                   ((configuration.fields & 32) == 0 && !row.message_type.empty()) || row.node.size() > 255 || row.endpoint.size() > 255 ||
                   row.message_type.size() > 255)
                {
                    return error::invalid;
                }
                for(auto const* text: {&row.node, &row.endpoint, &row.message_type})
                {
                    if(!wire::utf8({reinterpret_cast<std::uint8_t const*>(text->data()), text->size()}) ||
                       text->find('\0') != ::uwvm2::utils::container::std_string::npos)
                    {
                        return error::invalid;
                    }
                }
                auto bytes = std::size_t{32} + row.node.size() + row.endpoint.size() + row.message_type.size();
                if(bytes > configuration.max_bytes - extent) { return error::too_large; }
                extent += bytes;
            }
            if(!revision || current_rows != captured)
            {
                if(revision == UINT64_MAX) { return error::too_large; }
                ++revision;
                current_rows = std::move(captured);
            }
            return error::ok;
        }

    public:
        error configure(plan selected, std::shared_ptr<source> provider)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(configured || retired) { return error::denied; }
            if(!selected.authority || !valid(selected.selected) || !selected.context || !provider || !provider->live() ||
               provider->context() != selected.context || provider->authority().environment != selected.authority.environment || selected.nodes.empty() ||
               !selected.fields || (selected.fields & ~std::uint64_t{63}) || !(selected.fields & 31) || !selected.max_rows || !selected.max_bytes ||
               !selected.max_page_items || selected.max_page_items > selected.max_rows || !selected.cursor_lifetime_ns ||
               selected.cursor_lifetime_ns > INT64_MAX || !selected.capture_deadline_ns || selected.capture_deadline_ns > INT64_MAX)
            {
                return error::invalid;
            }
            for(std::size_t i{}; i < selected.nodes.size(); ++i)
            {
                if(!absolute_name(selected.nodes[i])) { return error::invalid; }
                for(std::size_t j{}; j < i; ++j)
                {
                    if(selected.nodes[j] == selected.nodes[i]) { return error::invalid; }
                }
            }
            rows.reserve(selected.max_rows);
            current_rows.reserve(selected.max_rows);
            configuration = std::move(selected);
            native = std::move(provider);
            configured = true;
            return error::ok;
        }

        /** @brief Complete a bounded graph page or replay its failed output.
         * @details The initial request captures actual native state once. Its
         * cursor binds this owner/filter/revision and expires at a steady deadline.
         * Failed sinks retain the exact page and cursor; a different request while
         * that output is pending cannot replace the recovery record. Native graph
         * scans are observations, not a guarantee that a peer still exists later.
         */
        error read(owner caller, profile const& selected, handle context, std::optional<handle> cursor, std::size_t limit, void* argument, sink deliver,
                   operation_token const* operation=nullptr)
        {
            if(!same_process()) { return error::denied; }
            std::unique_lock lock{mutex};
            if(!configured || retired || !native || !native->live()) { return error::cancelled; }
            if(caller != configuration.authority || selected != configuration.selected || context != configuration.context || !deliver)
            {
                return error::denied;
            }
            if(operation&&(!operation->valid(permission::graph_snapshot,operation_family::graph,configuration.authority,operation->resource,false)||
                operation->origins[2].resource!=configuration.context))return error::denied;
            if(busy) { return error::again; }
            if(!limit || limit > configuration.max_page_items || (cursor && !*cursor)) { return error::invalid; }
            auto time = now();
            if(pending && time >= expires)
            {
                pending = false;
                next_cursor = 0;
                next_offset = 0;
                if(cursor) { return error::timeout; }
            }
            if(pending)
            {
                if(cursor != pending_request || limit != pending_limit) { return error::busy; }
                if(time >= expires) { return error::timeout; }
            }
            else
            {
                if(cursor)
                {
                    if(*cursor != next_cursor) { return error::stale; }
                    if(time >= expires) { return error::timeout; }
                    pending_offset = next_offset;
                }
                else
                {
                    auto status = refresh(lock,operation);
                    if(status != error::ok) { return status; }
                    try
                    {
                        auto complete = current_rows;
                        rows.swap(complete);
                    }
                    catch(std::bad_alloc const&)
                    {
                        return error::no_memory;
                    }
                    page_revision = revision;
                    time = now();
                    pending_offset = 0;
                    next_offset = 0;
                    next_cursor = 0;
                    if(time > UINT64_MAX - configuration.cursor_lifetime_ns) { return error::too_large; }
                    expires = time + configuration.cursor_lifetime_ns;
                }
                pending_count = std::min(limit, rows.size() - pending_offset);
                pending_cursor = 0;
                if(pending_offset + pending_count < rows.size())
                {
                    pending_cursor = allocate();
                    if(!pending_cursor) { return error::too_large; }
                }
                pending_request = cursor;
                pending_limit = limit;
                pending = true;
            }
            page value{page_revision,
                       pending_cursor ? std::optional<handle>{pending_cursor} : std::nullopt,
                       std::span<entity const>{rows}.subspan(pending_offset, pending_count)};
            busy = true;
            lock.unlock();
            bool copied{};
            try
            {
                copied = deliver(argument, value);
            }
            catch(...)
            {
            }
            lock.lock();
            busy = false;
            if(retired || !native->live()) { return error::cancelled; }
            if(!copied) { return error::io; }
            next_cursor = pending_cursor;
            next_offset = pending_offset + pending_count;
            pending = false;
            if(!next_cursor) { acknowledged_revision = page_revision; }
            return error::ok;
        }

        /** @brief Refresh graph-watch readiness without consuming a page.
         * @details New native revisions are independent from retained pagination;
         * completing the final page acknowledges only that page's pinned revision.
         * At most three bounded result buffers overlap during refresh/page staging.
         */
        error observe(owner caller, handle& version, bool& ready,operation_token const* operation=nullptr)
        {
            if(!same_process()) { return error::denied; }
            std::unique_lock lock{mutex};
            if(!configured || caller != configuration.authority || retired || !native->live()) { return error::denied; }
            if(operation&&(!operation->valid(permission::resource_observe,operation_family::graph,configuration.authority,operation->resource,false)||
                operation->origins[2].resource!=configuration.context))return error::denied;
            if(busy) { return error::again; }
            auto status = refresh(lock,operation);
            if(status != error::ok) { return status; }
            version = revision;
            ready = revision != acknowledged_revision;
            return error::ok;
        }

        /** @brief Read only cached readiness, without starting a native capture.
         * @details Debugging and tokenless watch inspection cannot initiate IPC,
         * spend an observation budget, or impersonate the opener. An admitted
         * broker observation supplies a fresh token to observe instead.
         */
        error cached_observation(owner caller,handle& version,bool& ready) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            if(!configured||caller!=configuration.authority||retired||!native||!native->live())return error::denied;
            version=revision;ready=revision!=acknowledged_revision;return error::ok;
        }

        error revision_state(owner caller, handle& output) noexcept
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(!configured || caller != configuration.authority || retired || !native->live()) { return error::denied; }
            output = revision;
            return error::ok;
        }

        void retire() noexcept
        {
            if(!same_process()) { return; }
            std::lock_guard lock{mutex};
            retired = true;
        }
    };
}  // namespace uwvm2::imported::ros::scene::communication::graph
