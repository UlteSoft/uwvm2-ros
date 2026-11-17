/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)            *
 * Licensed under the APL-2.0 License (see LICENSE file).      *
 *************************************************************/
#pragma once
#include <uwvm2/utils/container/standard.h>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <map>
#include <span>
#include <string>
#include <string_view>
#include <vector>
#if defined(UWVM_RUNTIME_INTEL_JIT_PROFILE) && UWVM_RUNTIME_INTEL_JIT_PROFILE == 1
# include <atomic>
# include <mutex>
# include <jitprofiling.h>
# if defined(__linux__)
#  include <unistd.h>
# endif
#endif

namespace uwvm2::runtime::lib::details::intel_jit_profile
{
    /** @brief End-exclusive native range boundary and its original source line.
     * @details Intel's API describes the END of each range, not its beginning.
     * A complete table ends at method size. Line zero explicitly means unknown;
     * it must not be filled from a neighbouring Wasm operation or source file.
     */
    struct line_boundary { unsigned end_offset{}, source_line{}; };

    /** @brief Borrowed, bounded metadata from a trusted actual code owner.
     * @details Address/size must describe readable, finalized executable bytes
     * retained until the corresponding retirement notification returns. This
     * internal interface is NOT a guest hostcall or a debugger read permission.
     * Strings and lines are copied synchronously. Source is a display label;
     * this component never opens it, resolves it, or downloads external DWARF.
     */
    struct method_view
    {
        ::std::uintptr_t address{};
        ::std::size_t size{};
        ::std::string_view name{}, module{}, source{};
        ::std::span<line_boundary const> lines{};
    };

    enum class activation : unsigned char { disabled, unavailable, ready };
    enum class registration_status : unsigned char
    { disabled, unavailable, invalid_metadata, limit, exhausted_ids, collector_rejected, reported };

    inline constexpr ::std::size_t maximum_methods{65536u};
    inline constexpr ::std::size_t maximum_lines{65536u};
    inline constexpr ::std::size_t maximum_label_bytes{4096u};

    [[nodiscard]] inline bool valid_label(::std::string_view label, bool required) noexcept
    {
        if((required && label.empty()) || label.size() > maximum_label_bytes) { return false; }
        for(unsigned char c : label) { if(c < 0x20u || c == 0x7fu) { return false; } }
        return true;
    }

    [[nodiscard]] inline bool valid_method(method_view const& method) noexcept
    {
        if(method.address == 0u || method.size == 0u || method.size > (::std::numeric_limits<unsigned>::max)() ||
           method.size > UINTPTR_MAX - method.address || !valid_label(method.name, true) ||
           !valid_label(method.module, false) || !valid_label(method.source, false) ||
           method.lines.size() > maximum_lines || (!method.lines.empty() && method.source.empty())) { return false; }
        unsigned previous{};
        for(auto row : method.lines)
        {
            if(row.end_offset <= previous || row.end_offset > method.size) { return false; }
            previous = row.end_offset;
        }
        return method.lines.empty() || previous == method.size;
    }

#if defined(UWVM_RUNTIME_INTEL_JIT_PROFILE) && UWVM_RUNTIME_INTEL_JIT_PROFILE == 1
    namespace sdk
    {
        // The official SDK's initialization and method counter are not atomic.
        // ALL component calls, including construction/destruction, share this
        // mutex. Embedded clients must use this component rather than call the
        // same SDK directly in parallel. No SDK call occurs in static init.
        inline ::std::mutex mutex{};
        inline bool identifiers_exhausted{};
        // The first trusted pre-fork activation fixes this SDK instance to its
        // process. A fork child must not wait on inherited SDK locks or notify
        // against the parent's method identity space. Exec creates fresh state.
        inline ::std::atomic<::std::uint64_t> process_id{};
        [[nodiscard]] inline ::std::uint64_t current_process() noexcept
        {
# if defined(__linux__)
            return static_cast<::std::uint64_t>(::getpid());
# else
            return 0u; // Other process APIs require independent qualification.
# endif
        }
        [[nodiscard]] inline bool bind_process() noexcept
        {
            auto const current{current_process()};
            if(current == 0u) { return false; }
            ::std::uint64_t expected{};
            return process_id.compare_exchange_strong(expected, current, ::std::memory_order_acq_rel) || expected == current;
        }

        inline void unload(unsigned id, ::std::uintptr_t address, unsigned size) noexcept
        {
            iJIT_Method_Load event{};
            event.method_id = id;
            event.method_load_address = reinterpret_cast<void*>(address);
            event.method_size = size;
            static_cast<void>(iJIT_NotifyEvent(iJVM_EVENT_TYPE_METHOD_UNLOAD_START, &event));
        }
    }
#endif

    /** @brief Optional profiling registrations owned by the actual code arena.
     * @details Default construction performs no SDK call, environment lookup,
     * library load, allocation, or code read. A trusted host launcher must opt
     * in BEFORE materialization. Opt-in allows the official static SDK to load
     * the collector selected by the host INTEL_JIT_PROFILER64 environment; it is
     * not a sandboxed guest capability and must never derive from guest env.
     *
     * Register before execution; retire only after executions have drained and
     * BEFORE executable storage is released/reused. Loader detachment alone
     * does not retire code. No process-global shutdown event is sent, because
     * another independent registry may still own live executable regions.
     *
     * Each registry is confined to its materializer/retirement owner. Separate
     * registries may run concurrently; SDK calls and ID exhaustion are globally
     * serialized. Failed registration cannot change runtime authority or code.
     */
    class registry final
    {
        struct record { ::std::uint64_t object{}; ::std::uintptr_t end{}; unsigned id{}, size{}; };
        activation state_{activation::disabled};
#if defined(UWVM_RUNTIME_INTEL_JIT_PROFILE) && UWVM_RUNTIME_INTEL_JIT_PROFILE == 1
        ::std::uint64_t process_{};
#endif
        ::uwvm2::utils::container::std_map<::std::uintptr_t, record> records_{};
    public:
        explicit registry(bool trusted_enable = false) noexcept
        {
            if(!trusted_enable) { return; }
            state_ = activation::unavailable;
#if defined(UWVM_RUNTIME_INTEL_JIT_PROFILE) && UWVM_RUNTIME_INTEL_JIT_PROFILE == 1
            if(!sdk::bind_process()) { return; }
            process_ = sdk::process_id.load(::std::memory_order_acquire);
            ::std::lock_guard lock{sdk::mutex};
            if(iJIT_IsProfilingActive() == iJIT_SAMPLING_ON) { state_ = activation::ready; }
#endif
        }
        registry(registry const&) = delete;
        registry& operator=(registry const&) = delete;
        registry(registry&&) = delete;
        registry& operator=(registry&&) = delete;
        ~registry() noexcept { retire_all(); }

        /** @brief Reject inherited fork state before any SDK mutex or event.
         * @details The trusted launcher must activate in its parent process
         * before starting guest/worker forks. Fork children use exec or _exit;
         * they cannot reactivate this inherited SDK instance. No reset API can
         * grant a child the parent's profiling identity space.
         */
        [[nodiscard]] bool owner_process() const noexcept
        {
#if defined(UWVM_RUNTIME_INTEL_JIT_PROFILE) && UWVM_RUNTIME_INTEL_JIT_PROFILE == 1
            return process_ != 0u && process_ == sdk::current_process();
#else
            return false;
#endif
        }
        [[nodiscard]] activation state() const noexcept
        { return state_ == activation::disabled ? state_ : owner_process() ? state_ : activation::unavailable; }
        [[nodiscard]] ::std::size_t size() const noexcept { return records_.size(); }

        /** @brief Publish one bounded method, without retaining caller metadata.
         * @details No metadata is inspected while disabled/unavailable. All
         * potentially throwing allocations complete before the load event. A
         * rejected load gets a matching cleanup event before its record drops.
         * Overlapping ranges are rejected, including aliases of an existing
         * entry, so Intel cannot silently invalidate a still-live method.
         */
        [[nodiscard]] registration_status report(::std::uint64_t object, method_view const& method)
        {
            if(state_ == activation::disabled) { return registration_status::disabled; }
            if(state_ != activation::ready || !owner_process()) { return registration_status::unavailable; }
            if(!valid_method(method)) { return registration_status::invalid_metadata; }
            if(records_.size() == maximum_methods) { return registration_status::limit; }
            auto next{records_.lower_bound(method.address)};
            if(next != records_.end() && next->first < method.address + method.size) { return registration_status::invalid_metadata; }
            if(next != records_.begin())
            {
                auto previous{next}; --previous;
                if(previous->second.end > method.address) { return registration_status::invalid_metadata; }
            }
#if defined(UWVM_RUNTIME_INTEL_JIT_PROFILE) && UWVM_RUNTIME_INTEL_JIT_PROFILE == 1
            ::uwvm2::utils::container::std_string name{method.name}, module{method.module}, source{method.source};
            ::uwvm2::utils::container::std_vector<LineNumberInfo> lines;
            lines.reserve(method.lines.size());
            for(auto row : method.lines) { lines.push_back({row.end_offset, row.source_line}); }
            auto inserted{records_.emplace(method.address, record{object, method.address + method.size, 0u, static_cast<unsigned>(method.size)}).first};
            ::std::lock_guard lock{sdk::mutex};
            if(sdk::identifiers_exhausted) { records_.erase(inserted); return registration_status::exhausted_ids; }
            auto const id{iJIT_GetNewMethodID()};
            if(id == 0u)
            {
                sdk::identifiers_exhausted = true;
                records_.erase(inserted);
                return registration_status::exhausted_ids;
            }
            // The final nonzero SDK ID remains valid. Prevent another call from
            // wrapping a different SDK implementation back into earlier IDs.
            if(id == (::std::numeric_limits<unsigned>::max)()) { sdk::identifiers_exhausted = true; }
            iJIT_Method_Load_V2 event{}; // Required official SDK ABI spelling.
            event.method_id = id;
            event.method_name = name.data();
            event.method_load_address = reinterpret_cast<void*>(method.address);
            event.method_size = static_cast<unsigned>(method.size);
            event.line_number_size = static_cast<unsigned>(lines.size());
            event.line_number_table = lines.empty() ? nullptr : lines.data();
            event.source_file_name = source.empty() ? nullptr : source.data();
            event.module_name = module.empty() ? nullptr : module.data();
            if(iJIT_NotifyEvent(iJVM_EVENT_TYPE_METHOD_LOAD_FINISHED_V2, &event) != 1)
            {
                sdk::unload(id, method.address, event.method_size);
                records_.erase(inserted);
                return registration_status::collector_rejected;
            }
            inserted->second.id = id;
            return registration_status::reported;
#else
            static_cast<void>(object);
            return registration_status::unavailable;
#endif
        }

        /** @brief Retire all ranges of one loaded object before freeing its code. */
        void retire(::std::uint64_t object) noexcept
        {
            if(!owner_process()) { return; }
            for(auto it{records_.begin()}; it != records_.end();)
            {
                if(it->second.object != object) { ++it; continue; }
#if defined(UWVM_RUNTIME_INTEL_JIT_PROFILE) && UWVM_RUNTIME_INTEL_JIT_PROFILE == 1
                { ::std::lock_guard lock{sdk::mutex}; sdk::unload(it->second.id, it->first, it->second.size); }
#endif
                it = records_.erase(it);
            }
        }
        /** @brief Drain profiling registrations; does not release executable storage. */
        void retire_all() noexcept
        {
            if(!owner_process()) { return; }
#if defined(UWVM_RUNTIME_INTEL_JIT_PROFILE) && UWVM_RUNTIME_INTEL_JIT_PROFILE == 1
            if(!records_.empty())
            {
                ::std::lock_guard lock{sdk::mutex};
                for(auto const& [address, entry] : records_) { sdk::unload(entry.id, address, entry.size); }
            }
#endif
            records_.clear();
        }
    };
}
