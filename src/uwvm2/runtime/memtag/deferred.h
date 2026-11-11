// Deferred tag-fault delivery, never deferred bounds enforcement. A worker
// receives values only: it cannot retain or dereference a Guest/native owner.
#pragma once
#ifndef UWVM_MODULE
# include <array>
# include <atomic>
# include <condition_variable>
# include <cstddef>
# include <cstdint>
# include <cstdlib>
# include <memory>
# include <mutex>
# include <thread>
# include <utility>
# include <vector>
# include <uwvm2/extension/memtag/execution_policy.h>
# include <uwvm2/extension/memtag/shadow.h>
# include <uwvm2/extension/memtag/hardware_mte.h>
# include <uwvm2/runtime/diagnostic/crash_policy.h>
# include "report.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::memtag::deferred
{
    namespace mt=::uwvm2::extension::memtag;
    struct fault
    {
        mt::access_result access{};
        ::std::uint64_t raw_pointer{}, memory_index{UINT64_MAX};
        ::std::uint64_t module_id{UINT64_MAX}, function_index{UINT64_MAX}, wasm_offset{UINT64_MAX};
        ::std::uint64_t sequence{};
    };
    using fault_callback=void(*)(fault const&) noexcept;
    inline ::std::atomic<fault_callback> deliver_callback{};

    struct stream
    {
        ::std::mutex mutex{};
        ::std::condition_variable ready{};
        ::std::uint64_t pending{}, next_sequence{};
        bool has_fault{};
        fault first{};
        ::std::atomic<bool> observed{};
    };
    struct item { ::std::shared_ptr<stream> destination{}; fault evidence{}; };

    class pool
    {
        static constexpr ::std::size_t capacity{128u};
        // Starting may temporarily unlock the queue mutex to join a partially
        // constructed worker set. A second admitted entry must not mutate that
        // vector until failure cleanup has completed.
        ::std::mutex start_mutex_{};
        ::std::mutex mutex_{};
        ::std::condition_variable ready_{};
        ::std::array<item,capacity> queue_{};
        ::std::size_t read_{}, count_{};
        bool stopping_{}, started_{};
        ::std::vector<::std::thread> workers_{};
        static void publish(item work) noexcept
        {
            auto& target{*work.destination};
            bool const log{mt::logging_requested()};
            if(log) { report::violation(work.evidence.access,work.evidence.raw_pointer,
                work.evidence.memory_index,work.evidence.module_id,work.evidence.function_index,
                work.evidence.wasm_offset,true); }
            bool drained{};
            {
                ::std::lock_guard lock{target.mutex};
                if(!log && (!target.has_fault || work.evidence.sequence<target.first.sequence))
                { target.first=work.evidence;target.has_fault=true; }
                drained=--target.pending==0u;
                target.observed.store(!log || !drained,::std::memory_order_release);
            }
            // Every waiter has the same pending==0 predicate. Intermediate
            // completions cannot satisfy it; only the last one needs to wake it.
            if(drained) { target.ready.notify_all(); }
        }
        void worker() noexcept
        {
            for(;;)
            {
                item work{};
                {
                    ::std::unique_lock lock{mutex_};
                    ready_.wait(lock,[&]{return stopping_ || count_!=0u;});
                    if(count_==0u && stopping_) { return; }
                    work=::std::move(queue_[read_]);read_=(read_+1u)%capacity;--count_;
                }
                publish(::std::move(work));
            }
        }
    public:
        pool()=default;
        pool(pool const&)=delete;
        pool& operator=(pool const&)=delete;
        ~pool()
        {
            { ::std::lock_guard lock{mutex_};stopping_=true; }
            ready_.notify_all();
            for(auto& worker:workers_) { if(worker.joinable()) { worker.join(); } }
        }
        [[nodiscard]] bool start(unsigned count) noexcept
        {
            // Cold startup only; a repeated request must use the same count.
            if(count==0u || count>32u) { return false; }
            ::std::lock_guard start_lock{start_mutex_};
            ::std::unique_lock lock{mutex_};
            if(started_) { return workers_.size()==count; }
#if defined(__cpp_exceptions)
            try
            {
                workers_.reserve(count);
                for(unsigned index{};index!=count;++index) { workers_.emplace_back([this]{worker();}); }
                started_=true;return true;
            }
            catch(...)
            {
                stopping_=true;lock.unlock();ready_.notify_all();
                for(auto& worker:workers_) { if(worker.joinable()) { worker.join(); } }
                lock.lock();workers_.clear();stopping_=false;return false;
            }
#else
            return false;
#endif
        }
        void submit(::std::shared_ptr<stream> const& target,fault evidence) noexcept
        {
            {
                ::std::lock_guard lock{target->mutex};
                evidence.sequence=target->next_sequence++;++target->pending;
                // The next access must observe pending evidence before it
                // enters another owner/allocator lock; workers cannot hide it.
                target->observed.store(true,::std::memory_order_release);
            }
            item work{target,evidence};
            {
                ::std::lock_guard lock{mutex_};
                if(started_ && !stopping_ && count_!=capacity)
                {
                    queue_[(read_+count_)%capacity]=::std::move(work);++count_;
                    ready_.notify_one();return;
                }
            }
            // Backpressure never drops a fault or borrows Guest memory. Inline
            // publication is a stronger/eager fallback, not a queue-size limit
            // on valid Guest accesses and not an implicit request-rate quota.
            publish(::std::move(work));
        }
        [[nodiscard]] static bool drain(::std::shared_ptr<stream> const& target,fault& result) noexcept
        {
            ::std::unique_lock lock{target->mutex};
            target->ready.wait(lock,[&]{return target->pending==0u;});
            if(!target->has_fault) { return false; }
            result=target->first;return true;
        }
    };

    [[nodiscard]] inline pool& workers() noexcept { static pool value{};return value; }
    inline thread_local ::std::shared_ptr<stream> active_stream{};
    // Trivial TLS fast witness. The owning shared_ptr above pins this stream
    // from admitted entry until its final drain; no Guest can obtain the pointer.
    inline thread_local stream* active_stream_raw{};
    inline thread_local unsigned entry_depth{};
    inline thread_local bool delivery_in_progress{};
    inline thread_local ::std::uint64_t origin_module{UINT64_MAX}, origin_function{UINT64_MAX}, origin_offset{UINT64_MAX};

    [[nodiscard]] constexpr bool boundary_opcode(unsigned op) noexcept
    {
        return op==0x08u || op==0x0au || (op>=0x0bu && op<=0x15u) ||
               op==0x1fu || op==0xd5u || op==0xd6u || op==0xfbu || op==0xfeu;
    }
    [[nodiscard]] constexpr bool draining_opcode(unsigned op) noexcept
    { return boundary_opcode(op) || op==0x40u || op==0xffu; }
    [[nodiscard]] constexpr bool tracked_opcode(unsigned op) noexcept
    { return boundary_opcode(op) || (op>=0x28u && op<=0x40u) || op==0xfcu || op==0xfdu || op==0xffu; }

    inline void set_origin(::std::uint64_t module,::std::uint64_t function,::std::uint64_t offset) noexcept
    { origin_module=module;origin_function=function;origin_offset=offset; }

    [[gnu::noinline,gnu::cold]] inline void deliver_pending() noexcept
    {
        if(delivery_in_progress) { return; }
        fault evidence{};
        if(!pool::drain(active_stream,evidence)) { return; }
        delivery_in_progress=true;
        auto const callback{deliver_callback.load(::std::memory_order_acquire)};
        if(callback!=nullptr) { callback(evidence); }
        namespace crash=::uwvm2::runtime::diagnostic::crash;
        crash::memory_detail const detail{evidence.memory_index,evidence.raw_pointer,
            evidence.access.mismatch_granule,evidence.access.pointer_tag,evidence.access.allocation_tag};
        crash::report(crash::category::memtag,"asynchronous memory tag mismatch",0u,
            evidence.module_id,evidence.function_index,evidence.wasm_offset,&detail,true);
        crash::pause_process();
        crash::finish_unhandled_fault();
    }
    [[gnu::always_inline]] inline void poll(bool barrier=false) noexcept
    {
        // Physical ASYNC delivery still synchronizes even without a software
        // record. Software enqueue publishes observed before returning; a worker
        // clears it only when all log records are acknowledged. Crash evidence
        // keeps it set. Thus a false acquire also proves an empty barrier.
        if(barrier) { mt::hardware_mte::synchronize_asynchronous_faults(); }
        auto const* current{active_stream_raw};
        if(current && current->observed.load(::std::memory_order_acquire)) [[unlikely]]
        { deliver_pending(); }
    }
    [[nodiscard]] inline ::std::uintptr_t pending_flag_address() noexcept
    {
        static_assert(sizeof(::std::atomic<bool>)==1u);
        if(!active_stream_raw) { ::std::_Exit(127); }
        return reinterpret_cast<::std::uintptr_t>(::std::addressof(active_stream_raw->observed));
    }

    // Private compiler bridge: provenance comes from a validated source span,
    // never a Guest-supplied index. A call/return/wait boundary drains earlier
    // faults before an irreversible host effect or successful process exit.
    inline void boundary(::std::uint64_t module,::std::uint64_t function,
        ::std::uint64_t offset,unsigned flags) noexcept
    {
        if(mt::asynchronous_requested()) { poll((flags&1u)!=0u); }
        set_origin(module,function,offset);
    }

    inline void record_llvm(::std::size_t memory_index,::std::uint64_t raw,
        ::std::uint64_t granule,::std::uint32_t pointer_tag,::std::uint32_t allocation_tag) noexcept
    {
        if(!active_stream && !mt::logging_requested()) { ::std::_Exit(127); } // Fail closed outside an execution scope.
        // The bridge receives the exact first failing granule, not the width
        // or effective byte offset. A low tag does not imply memory32.
        mt::access_result const result{mt::access_error::tag_mismatch,
            UINT64_MAX,granule,static_cast<::std::uint8_t>(pointer_tag),static_cast<::std::uint8_t>(allocation_tag)};
        if(active_stream)
        { workers().submit(active_stream,fault{result,raw,memory_index,origin_module,origin_function,origin_offset,0u}); }
        else { report::violation(result,raw,memory_index,origin_module,origin_function,origin_offset); }
    }

    // Direct checks carry validated source coordinates only on their mismatch
    // edge. Successful private accesses need no TLS publication or host call.
    // Keep the caller's host provenance; boundaries drain any queued evidence.
    inline void record_llvm_at(::std::size_t memory_index,::std::uint64_t raw,
        ::std::uint64_t granule,::std::uint32_t pointer_tag,::std::uint32_t allocation_tag,
        ::std::uint64_t module,::std::uint64_t function,::std::uint64_t offset) noexcept
    {
        if(!active_stream_raw && !mt::logging_requested()) { ::std::_Exit(127); }
        mt::access_result const result{mt::access_error::tag_mismatch,
            UINT64_MAX,granule,static_cast<::std::uint8_t>(pointer_tag),static_cast<::std::uint8_t>(allocation_tag)};
        if(active_stream_raw)
        { workers().submit(active_stream,fault{result,raw,memory_index,module,function,offset,0u}); }
        else { report::violation(result,raw,memory_index,module,function,offset); }
    }

    // Only an admitted native execution entry creates this scope. The stream
    // stays alive until every metadata item is acknowledged, before generation
    // leases can release. A nested host reentry drains its suspended caller.
    class entry_scope
    {
        bool entered_{};
        ::std::uint64_t saved_module_{UINT64_MAX},saved_function_{UINT64_MAX},saved_offset_{UINT64_MAX};
    public:
        entry_scope()=default;
        entry_scope(entry_scope const&)=delete;
        entry_scope& operator=(entry_scope const&)=delete;
        [[nodiscard]] bool enter() noexcept
        {
            bool const asynchronous{mt::asynchronous_requested()};
            if(!asynchronous && !mt::logging_requested()) { return true; }
            saved_module_=origin_module;saved_function_=origin_function;saved_offset_=origin_offset;
            if(entry_depth!=0u) { poll(true);++entry_depth;entered_=true;return true; }
            if(asynchronous)
            {
                if(!workers().start(mt::asynchronous_worker_count.load(::std::memory_order_relaxed))) { return false; }
#if defined(__cpp_exceptions)
                try { active_stream=::std::make_shared<stream>(); }
                catch(...) { return false; }
#else
                return false;
#endif
            }
            active_stream_raw=active_stream.get();
            entry_depth=1u;entered_=true;set_origin(UINT64_MAX,UINT64_MAX,UINT64_MAX);
            if(asynchronous) { mt::hardware_mte::arm_asynchronous_execution(); }
            return true;
        }
        void finish() noexcept
        {
            if(!entered_) { return; }
            poll(true);
            entered_=false;
            if(--entry_depth==0u)
            { mt::hardware_mte::disarm_asynchronous_execution();active_stream_raw=nullptr;active_stream.reset(); }
            // An admitted host callback may reenter Wasm. Its return must not
            // relabel later outer host-buffer checks with the nested module.
            set_origin(saved_module_,saved_function_,saved_offset_);
        }
        ~entry_scope() { finish(); }
    };

    // The synchronous checker has already established the complete ordinary
    // bounds and captured the actual tag at the access point. This implements
    // delayed *fault delivery*, like hardware ASYNC; it is NOT a promise that a
    // mismatching access was suppressed or that worker threads accelerate tag
    // comparison. Host-buffer operations never call this function.
    [[nodiscard]] inline mt::access_result defer_if_requested(mt::access_result result,
        ::std::uint64_t raw,::std::uint64_t memory_index,bool guest=true) noexcept
    {
        if(result.error==mt::access_error::tag_mismatch && mt::logging_requested())
        {
            if(guest && active_stream)
            { workers().submit(active_stream,fault{result,raw,memory_index,origin_module,origin_function,origin_offset,0u}); }
            else { report::violation(result,raw,memory_index,origin_module,origin_function,origin_offset); }
            result.error=mt::access_error::none;return result;
        }
        if(!guest || result.error!=mt::access_error::tag_mismatch || !active_stream) { return result; }
        workers().submit(active_stream,fault{result,raw,memory_index,origin_module,origin_function,origin_offset,0u});
        result.error=mt::access_error::none;
        return result;
    }
}
