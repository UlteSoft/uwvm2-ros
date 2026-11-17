/** @file
 * @brief Fixed-capacity, payload-free diagnostic snapshots with expiring cursors.
 * @details Trusted bindings select one exact source and a closed counter mask.
 * Guest scope numbers never select an arbitrary source. The broker holds the
 * diagnostic.read and source resource.observe authority for each read, including
 * cursor retries; this component is not a capability issuer. Capturing only
 * copies cached counters: it must not poll hardware, expire requests, execute a
 * control action, or expose names, paths, model data or other private payloads.
 *
 * Storage is allocated once with recoverable fast_io allocation. Capture and
 * output callbacks run outside the mutex. Failed output preserves the exact
 * page and cursor; subsequent valid cursors acknowledge earlier pages. Expiry
 * uses host steady time and never extends on retry. Capacity is independent of
 * optional broker rate attenuation. No native diagnostic publisher is implicit.
 */
#pragma once
#include "log_store.h"
#include <optional>

namespace uwvm2::imported::ros::scene::diagnostics
{
    using error=resources::error;
    using handle=resources::handle;
    using owner=logs::owner;
    /** @brief A closed numeric counter, with explicit availability/saturation.
     * @details code is 1..64 in the deployed source schema. Unavailable counters
     * must carry value zero and saturated false; zero then cannot be mistaken for
     * an observed value. Schemas define units; unknown codes are not accepted.
     */
    struct counter
    {std::uint64_t code{},value{};bool available{},saturated{};friend bool operator==(counter const&,counter const&)=default;};
    struct plan
    {handle scope{};std::uint64_t fields{},rows{},page_items{},storage_bytes{},lifetime_ns{},capture_ns{};};
    struct page
    {handle revision{};std::optional<handle> cursor;std::span<counter const> items;};
    struct usage
    {std::uint64_t reserved_bytes{},revision{},rows{},expires{};bool pending{},retired{},busy{};};
    /** @brief Trusted bounded source callback, never a guest-supplied function.
     * @details capture must fill at most output.size complete cached counters and
     * report count only on success. The caller owns all memory. native/context
     * must remain alive until this snapshot and its active reads are destroyed.
     */
    struct source
    {void* context{};error(*capture)(void*,std::span<counter>,std::size_t&) noexcept{};};

    class snapshot
    {
        mutable std::mutex mutex;
        owner authority{};plan policy{};source provider{};
        ::uwvm2::utils::container::std_unique_ptr<counter[]> rows;
        std::size_t count{},offset{},pending_count{},pending_limit{};
        handle revision{},expires{},next_cursor{},pending_next{};
        std::optional<handle> pending_request;
        bool configured{},retired{},busy{},active{},pending{};
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
        static std::uint64_t now() noexcept
        {auto value=resources::steady_now();return value>0?static_cast<std::uint64_t>(value):0;}
        void discard() noexcept
        {active=false;pending=false;next_cursor=0;pending_next=0;count=offset=pending_count=pending_limit=0;pending_request.reset();expires=0;}
    public:
        /** @brief Validate the exact fixed array reservation without allocation. */
        static bool required_storage(plan const& value,std::uint64_t& output) noexcept
        {
            if(!value.scope||!value.fields||!value.rows||value.rows>64||!value.page_items||value.page_items>value.rows||
               !value.lifetime_ns||value.lifetime_ns>INT64_MAX||!value.capture_ns||value.capture_ns>value.lifetime_ns)return false;
            output=value.rows*sizeof(counter);return true;
        }
        /** @brief Install a non-owning trusted source and preallocate all page storage.
         * @details Allocation failure leaves the object unconfigured. The source
         * is not invoked here. Default fields/capacity/source deny configuration.
         */
        error configure(owner caller,plan value,source selected)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(configured||retired)return error::denied;
            std::uint64_t needed{};if(!caller||!selected.capture||!required_storage(value,needed))return error::invalid;
            if(needed>value.storage_bytes)return error::too_large;
            auto next=::uwvm2::utils::container::make_unique_for_overwrite<counter[]>(static_cast<std::size_t>(value.rows));
            rows=std::move(next);authority=caller;policy=value;provider=selected;configured=true;return error::success;
        }
        /** @brief Read immutable, finite pages under the same exact source authority.
         * @details A new absent cursor starts a capture only when the previous
         * snapshot is finished or expired. A pending failed page accepts only its
         * exact cursor and limit. Successful intermediate pages require the
         * returned opaque cursor; it is not an offset or authority token. Cursor
         * identity is process-unique and cannot be reused across aliases/snapshots.
         * Expired cursors return timeout and never trigger a new capture. No sink
         * executes under the lock. A false or throwing sink preserves the page.
         * Revocation/expiry during a copy prevents advancement but cannot undo
         * bytes already observed by that sink; broker output checks remain required.
         */
        template<typename Sink> error read(owner caller,handle scope,std::optional<handle> cursor,std::size_t limit,Sink&& deliver)
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex};if(!configured||caller!=authority||scope!=policy.scope)return error::denied;
            if(retired)return error::cancelled;
            if(busy)return error::again;
            if(!limit||limit>policy.page_items||(cursor&&!*cursor))return error::invalid;
            auto time=now();if(!time)return error::io;
            if(active&&time>=expires){discard();if(cursor)return error::timeout;}
            if(pending)
            {if(cursor!=pending_request||limit!=pending_limit)return error::conflict;}
            else
            {
                if(!active)
                {
                    if(cursor)return error::bad_handle;
                    if(policy.lifetime_ns>INT64_MAX-time||revision==UINT64_MAX)return error::too_large;
                    auto started=time;std::size_t captured{};busy=true;lock.unlock();
                    auto status=provider.capture(provider.context,{rows.get(),static_cast<std::size_t>(policy.rows)},captured);
                    lock.lock();busy=false;
                    if(retired)return error::cancelled;
                    if(status!=error::success)return status;
                    time=now();if(time<started||time-started>=policy.capture_ns)return error::timeout;
                    if(captured>policy.rows)return error::too_large;
                    std::uint64_t seen{};
                    for(std::size_t i{};i<captured;++i)
                    {
                        auto const& row=rows[i];if(!row.code||row.code>64)return error::invalid;
                        auto bit=std::uint64_t{1}<<(row.code-1);
                        if(!(policy.fields&bit)||(seen&bit)||(!row.available&&(row.value||row.saturated)))return error::invalid;
                        seen|=bit;
                    }
                    std::sort(rows.get(),rows.get()+captured,[](auto const& a,auto const& b){return a.code<b.code;});
                    count=captured;offset=0;expires=started+policy.lifetime_ns;++revision;active=true;next_cursor=0;
                }
                else if(!cursor||*cursor!=next_cursor)return error::bad_handle;
                pending_count=std::min(limit,count-offset);pending_next=0;
                if(pending_count<count-offset)
                {pending_next=resources::allocate_identity();if(!pending_next)return error::too_large;}
                pending_request=cursor;pending_limit=limit;pending=true;
            }
            page value{revision,{}, {rows.get()+offset,pending_count}};
            if(pending_next)value.cursor.emplace(pending_next);
            busy=true;lock.unlock();bool copied{};
            try{copied=deliver(value);}catch(...){lock.lock();busy=false;throw;}
            lock.lock();busy=false;
            if(retired)return error::cancelled;
            if(now()>=expires){discard();return error::timeout;}
            if(!copied)return error::io;
            offset+=pending_count;next_cursor=pending_next;pending=false;pending_request.reset();
            if(offset==count)discard();
            return error::success;
        }
        /** @brief Fence read/capture and preserve cached metadata without side effects. */
        error retire(owner caller) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(!configured||caller!=authority)return error::denied;
            retired=true;return error::success;
        }
        /** @brief Observe cached counts; elapsed cursor deadlines are not enforced by inspection. */
        error inspect(owner caller,usage& output) const noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(!configured||caller!=authority)return error::denied;
            output={policy.rows*sizeof(counter),revision,count,expires,pending,retired,busy};return error::success;
        }
    };
}
