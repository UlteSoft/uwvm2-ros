/** @file
 * @brief Bounded private log records with exact replay and explicit loss policy.
 * @details This store is not an authorization boundary or a console/file/network
 * appender. The broker supplies the owner and holds the exact log.emit lease;
 * trusted export additionally needs its own destination and content authority.
 * No format string is evaluated. Names, messages and optional source locations
 * are guest claims, never host paths to open or trusted safety observations.
 * Optional event times are also claims; security decisions use host steady time.
 *
 * All persistent and drain scratch allocation is reserved at configuration with
 * recoverable fast_io allocators. Emit, replay, inspection and drain allocate
 * nothing. Full capacity rejects by default. drop_oldest is explicit attenuation
 * with an exact loss counter, and cannot evict a record reserved for delivery.
 * Exact replay of the last accepted request never appends another record, even
 * after export or eviction. A successful next sequence acknowledges that receipt.
 * This is process-local volatile acceptance, not durable or remote exactly-once.
 */
#pragma once
#include "buffers.h"
#include <mutex>

namespace uwvm2::imported::ros::scene::logs
{
    using error=resources::error;
    using handle=resources::handle;
    struct owner
    {
        handle environment{},grant{},generation{};
        explicit operator bool() const noexcept{return environment&&grant&&generation;}
        friend bool operator==(owner const&,owner const&)=default;
    };
    enum class severity:std::uint8_t{debug,info,warn,error,fatal};
    inline constexpr std::array severity_names{std::string_view{"debug"},std::string_view{"info"},std::string_view{"warn"},std::string_view{"error"},std::string_view{"fatal"}};
    enum class overflow:std::uint8_t{reject,drop_oldest};
    inline constexpr std::size_t maximum_loggers=64,maximum_name=255;
    using name=buffers::text<maximum_name>;
    /** @brief Immutable capacity and exact logger allowlist; zero grants nothing.
     * @details storage_bytes accounts for ring metadata, complete canonical
     * records, one retained replay request and one detached drain copy. It is
     * not allocator metadata or total process RSS. record_bytes bounds the
     * entire encoded record, not just its message. Severity masks use bits 0..4.
     */
    struct plan
    {
        std::array<name,maximum_loggers> loggers{};std::size_t logger_count{};
        std::uint64_t records{},record_bytes{},storage_bytes{},severity_mask{};
        overflow full{};
    };
    struct stamp{std::uint64_t clock{},epoch{};std::int64_t nanoseconds{};};
    /** @brief Borrowed, completely decoded canonical request; no guest pointer is retained. */
    struct request
    {
        std::uint64_t session{},sequence{};severity level{};std::string_view logger,message,file,function;
        stamp time{};std::uint64_t line{};bool has_time{},has_location{};
    };
    /** @brief Local acceptance receipt, unchanged by later drain or loss.
     * @details record is a monotonic store-local record number. accepted_bytes
     * includes the complete canonical request. lost_before records explicit
     * eviction that happened before this acceptance; no loss is hidden in success.
     */
    struct receipt
    {
        std::uint64_t session{},sequence{},record{},accepted_bytes{},lost_before{};
        friend bool operator==(receipt const&,receipt const&)=default;
    };
    /** @brief Payload-free counters, with saturating failed-attempt counters.
     * @details accepted, exported and lost are exact and never wrap. Counters
     * of rejected attempts saturate and set counters_saturated. Inspect does not
     * expire, export or consume anything and is suitable for a separate adapter.
     */
    struct usage
    {
        std::uint64_t accepted{},exported{},lost{},retained{},retained_bytes{},reserved_bytes{};
        std::uint64_t full_refusals{},copy_failures{};bool counters_saturated{},retired{},draining{};
    };
    inline bool valid_text(std::string_view value,std::size_t maximum,bool empty=true) noexcept
    {
        return (empty||!value.empty())&&value.size()<=maximum&&value.find('\0')==value.npos&&
            wire::utf8({reinterpret_cast<std::uint8_t const*>(value.data()),value.size()});
    }
    /** @brief Decode exact UTF-8 fields; unknown severities/trailing bytes fail closed. */
    inline bool decode(wire::bytes bytes,request& output) noexcept
    {
        wire::reader in{bytes};request value;std::string_view level;
        if(!in.u64(value.session)||!value.session||!in.u64(value.sequence)||!value.sequence||!in.text(level))return false;
        auto found=std::find(severity_names.begin(),severity_names.end(),level);if(found==severity_names.end())return false;
        value.level=static_cast<severity>(found-severity_names.begin());
        if(!in.text(value.logger)||!valid_text(value.logger,maximum_name,false)||!in.boolean(value.has_time)||
           (value.has_time&&(!in.u64(value.time.clock)||!value.time.clock||!in.u64(value.time.epoch)||!in.i64(value.time.nanoseconds)))||
           !in.text(value.message)||value.message.find('\0')!=value.message.npos||!in.boolean(value.has_location))return false;
        if(value.has_location&&(!in.text(value.file)||!valid_text(value.file,maximum_name)||!in.text(value.function)||
           !valid_text(value.function,maximum_name)||!in.u64(value.line)))return false;
        if(!in.done())return false;
        output=value;return true;
    }
    /** @brief Canonical encoding shared by trusted adapters and portability fixtures. */
    inline bool encode(wire::writer& out,request const& value) noexcept
    {
        auto level=static_cast<std::size_t>(value.level);
        return value.session&&value.sequence&&level<severity_names.size()&&valid_text(value.logger,maximum_name,false)&&
            valid_text(value.message,SIZE_MAX)&&(!value.has_location||(valid_text(value.file,maximum_name)&&valid_text(value.function,maximum_name)))&&
            out.u64(value.session)&&out.u64(value.sequence)&&out.text(severity_names[level])&&out.text(value.logger)&&out.boolean(value.has_time)&&
            (!value.has_time||(value.time.clock&&out.u64(value.time.clock)&&out.u64(value.time.epoch)&&out.i64(value.time.nanoseconds)))&&
            out.text(value.message)&&out.boolean(value.has_location)&&
            (!value.has_location||(out.text(value.file)&&out.text(value.function)&&out.u64(value.line)));
    }

    /** @brief One alias-private, fixed-allocation log stream.
     * @details The owner and this object must outlive active calls. No sink runs
     * under the mutex. Retire fences new calls and final consumption; a sink
     * already executing may have observed bytes and cannot be retroactively
     * undone. An exporter must report such uncertainty instead of claiming a
     * physical rollback. Destruction is trusted teardown after active calls join.
     */
    class store
    {
        struct entry{std::uint64_t record{},bytes{};};
        mutable std::mutex mutex;
        owner authority{};plan policy{};
        ::uwvm2::utils::container::std_unique_ptr<entry[]> entries;
        ::uwvm2::utils::container::std_unique_ptr<std::uint8_t[]> storage;
        std::size_t first{},count{},last_bytes{};receipt last{};usage counters{};bool configured{};
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
        std::uint8_t* slot(std::size_t index) noexcept{return storage.get()+index*static_cast<std::size_t>(policy.record_bytes);}
        void increment(std::uint64_t& value) noexcept
        {if(value==UINT64_MAX)counters.counters_saturated=true;else ++value;}
        bool admitted(owner caller) const noexcept{return configured&&caller==authority;}
    public:
        /** @brief Check multiplication and the complete logical storage reservation before allocation. */
        static bool required_storage(plan const& selected,std::uint64_t& output) noexcept
        {
            if(!selected.records||selected.records>65536||!selected.record_bytes||selected.record_bytes>buffers::maximum_transfer||
               !selected.logger_count||selected.logger_count>maximum_loggers||!selected.severity_mask||(selected.severity_mask&~std::uint64_t{31})||
               static_cast<unsigned>(selected.full)>1)return false;
            for(std::size_t i{};i<selected.logger_count;++i)
            {
                if(selected.loggers[i].size>maximum_name||!valid_text(selected.loggers[i].view(),maximum_name,false))return false;
                for(std::size_t j{};j<i;++j)if(selected.loggers[i]==selected.loggers[j])return false;
            }
            constexpr auto maximum=std::min<std::uint64_t>(SIZE_MAX,INT64_MAX);
            if(selected.records>maximum/sizeof(entry)||selected.records+2>maximum/selected.record_bytes)return false;
            auto metadata=selected.records*sizeof(entry),data=(selected.records+2)*selected.record_bytes;
            if(data>maximum-metadata)return false;
            output=data+metadata;return true;
        }
        /** @brief Atomically install an explicit owner and complete fixed buffers.
         * @details Recoverable allocation failure propagates to the host boundary;
         * this store remains unconfigured and may be retried. No partial capacity
         * or native appender is published, and default policy cannot configure.
         */
        error configure(owner caller,plan const& selected)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(configured||counters.retired)return error::denied;
            std::uint64_t needed{};
            if(!caller||!required_storage(selected,needed))return error::invalid;
            if(needed>selected.storage_bytes)return error::too_large;
            auto next_entries=::uwvm2::utils::container::make_unique_for_overwrite<entry[]>(static_cast<std::size_t>(selected.records));
            auto next_data=::uwvm2::utils::container::make_unique_for_overwrite<std::uint8_t[]>(static_cast<std::size_t>((selected.records+2)*selected.record_bytes));
            entries=std::move(next_entries);storage=std::move(next_data);policy=selected;authority=caller;
            counters.reserved_bytes=needed;configured=true;return error::success;
        }
        /** @brief Append once or replay the exact last accepted bytes without allocation.
         * @details Rejected requests do not consume a sequence. A new successful
         * request must use sequence 1 initially or last+1, and the original session.
         * Changed bytes at the last key conflict even after drain/eviction. Caller
         * must own the immutable request for this synchronous call and validate the
         * output carrier before entering; a failed receipt copy is recovered by replay.
         */
        error emit(owner caller,wire::bytes bytes,receipt& output) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(!admitted(caller))return error::denied;
            if(counters.retired)return error::cancelled;
            if(bytes.size()>policy.record_bytes)return error::too_large;
            request value;if(!decode(bytes,value))return error::invalid;
            auto level=static_cast<unsigned>(value.level);
            if(!(policy.severity_mask&(std::uint64_t{1}<<level)))return error::denied;
            bool logger{};for(std::size_t i{};i<policy.logger_count;++i)logger|=policy.loggers[i].view()==value.logger;
            if(!logger)return error::denied;
            if(last.sequence&&value.session==last.session&&value.sequence==last.sequence)
            {
                if(last_bytes!=bytes.size()||!std::equal(bytes.begin(),bytes.end(),slot(policy.records)))return error::conflict;
                output=last;return error::success;
            }
            if(value.sequence!=(last.sequence+1)||last.sequence==UINT64_MAX||(last.sequence&&value.session!=last.session))return error::conflict;
            if(counters.accepted==UINT64_MAX)return error::too_large;
            if(count==policy.records)
            {
                if(policy.full==overflow::reject||counters.draining){increment(counters.full_refusals);return error::again;}
                if(counters.lost==UINT64_MAX)return error::too_large;
                counters.retained_bytes-=entries[first].bytes;first=(first+1)%static_cast<std::size_t>(policy.records);--count;++counters.lost;
            }
            auto index=(first+count)%static_cast<std::size_t>(policy.records);
            std::memcpy(slot(index),bytes.data(),bytes.size());std::memcpy(slot(policy.records),bytes.data(),bytes.size());
            entries[index]={++counters.accepted,bytes.size()};++count;counters.retained=count;counters.retained_bytes+=bytes.size();
            last={value.session,value.sequence,counters.accepted,bytes.size(),counters.lost};last_bytes=bytes.size();output=last;
            return error::success;
        }
        /** @brief Deliver one complete front record through a separately authorized trusted sink.
         * @details Concurrent/reentrant drains return again. The detached scratch
         * remains reserved through the callback, and the front cannot be evicted.
         * False or throwing sinks preserve it. Retire during delivery prevents
         * final consumption; callers must not retry a non-idempotent remote effect
         * solely because this method reports cancellation or copy failure.
         */
        template<typename Sink> error drain(owner caller,Sink&& deliver)
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex};if(!admitted(caller))return error::denied;
            if(counters.retired)return error::cancelled;
            if(counters.draining||!count)return error::again;
            auto current=entries[first];auto* copy=slot(policy.records+1);
            std::memcpy(copy,slot(first),static_cast<std::size_t>(current.bytes));counters.draining=true;lock.unlock();
            bool copied{};
            try{copied=deliver(current.record,wire::bytes{copy,static_cast<std::size_t>(current.bytes)});}
            catch(...){lock.lock();counters.draining=false;increment(counters.copy_failures);throw;}
            lock.lock();counters.draining=false;
            if(counters.retired)return error::cancelled;
            if(!copied){increment(counters.copy_failures);return error::io;}
            counters.retained_bytes-=current.bytes;first=(first+1)%static_cast<std::size_t>(policy.records);--count;
            counters.retained=count;++counters.exported;return error::success;
        }
        /** @brief Fence future emission/export without performing I/O or hiding retained capacity. */
        error retire(owner caller) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(!admitted(caller))return error::denied;
            counters.retired=true;return error::success;
        }
        /** @brief Read cached metadata only under a separately checked inspection grant. */
        error inspect(owner caller,usage& output) const noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(!admitted(caller))return error::denied;
            output=counters;return error::success;
        }
    };
}
