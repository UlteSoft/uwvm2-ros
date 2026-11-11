// Include INSIDE the actual uwvm2::runtime::lib TU, after the WASIp1 context
// resolver and ordinary execution-scope definitions, before full materialize.
// Required headers (memory/sysstat/sysmacros/unistd/runtime_origin) are TU-scope.
#if defined(__linux__) && defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_USE_THREAD_LOCAL) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_MODULE)
extern "C++"
{
    class runtime_cache_origin_bridge final
    {
    public:
        class batch;
    private:
        using source = ::uwvm2::uwvm::runtime::full::full_source_instance;
        using ticket = ::uwvm2::runtime::llvm_jit_cache::runtime_origin::ticket;
        using use = ::uwvm2::runtime::llvm_jit_cache::runtime_origin::use;
        using reference = ::uwvm2::runtime::llvm_jit_cache::runtime_origin::reference;
        struct state final : ticket
        {
            // Declaration order releases Source BEFORE the genuine generation
            // lease. Reset cannot destroy providers/resources while an I/O use
            // remains. Metadata and engine captures never own this strong state.
            ::uwvm2::utils::thread::execution_domain::lease generation;
            source::owner original;
            ::std::uint_least64_t epoch;
            ::std::atomic_bool secret_phase{true};
            state(::uwvm2::utils::thread::execution_domain::lease actual, source::owner input,
                ::std::uint_least64_t observed) noexcept
                : generation{::std::move(actual)}, original{::std::move(input)}, epoch{observed} {}
        private:
            [[nodiscard]] bool actual_secret_phase() const noexcept override
            { return secret_phase.load(::std::memory_order_acquire); }
            [[nodiscard]] bool actual_current() const noexcept override
            {
                // No Root publication lock or mutable provider/FD re-read here:
                // parallel workers run while their parent holds that very lock.
                // Only this real ticket's live lease + initializer-sealed provider
                // bindings survive the short cold constructor below.
                return generation && !generation.stop_requested() && epoch == current_runtime_generation() &&
                    ::uwvm2::runtime::llvm_jit_cache::kernel_origin::dumping_disabled() &&
                    source::has_canonical_owner(original) && original->initialized_from_actual_state();
            }
        };
        inline static thread_local batch* current_{};

        [[nodiscard]] static ::std::shared_ptr<state> mint(compiled_module_record const& record) noexcept
        {
            namespace mode = ::uwvm2::uwvm::runtime::runtime_mode;
            if(!::uwvm2::runtime::llvm_jit_cache::kernel_origin::dumping_disabled() ||
               get_runtime_state_publication_depth() != 1u || get_runtime_execution_entry_depth() != 1u ||
               get_runtime_execution_lease() == nullptr || get_runtime_execution_lease()->stop_requested() ||
               g_runtime.compiled_all.load(::std::memory_order_acquire) ||
               g_runtime.published_runtime_state.kind != details::runtime_state_kind::empty ||
               g_runtime.bridges_initialized.load(::std::memory_order_acquire) ||
               mode::global_runtime_mode != mode::runtime_mode_t::full_compile ||
               mode::global_runtime_compiler != mode::runtime_compiler_t::llvm_jit_only ||
               g_runtime.debug_pause_control || g_runtime.checkpoint_profile || !record.llvm_jit_full_publication ||
               record.llvm_jit_full_publication->checkpoint_profile || record.llvm_jit_full_publication->plan)
            {
                ::uwvm2::runtime::llvm_jit_cache::details::runtime_cache_log_line(
                    u8"origin-admission-refused stage=runtime-state publication-depth=", get_runtime_state_publication_depth(),
                    u8" execution-depth=", get_runtime_execution_entry_depth(),
                    u8" execution-lease=", get_runtime_execution_lease() != nullptr,
                    u8" compiled=", g_runtime.compiled_all.load(::std::memory_order_acquire),
                    u8" published=", g_runtime.published_runtime_state.kind != details::runtime_state_kind::empty,
                    u8" bridges=", g_runtime.bridges_initialized.load(::std::memory_order_acquire),
                    u8" debugger=", g_runtime.debug_pause_control != nullptr,
                    u8" checkpoint=", static_cast<bool>(g_runtime.checkpoint_profile), u8" modules=", g_runtime.modules.size());
                return {};
            }
            // Merely configuring WASI threads does not start a guest. The
            // caller's actual cold preparation exclusion prevents Group launch
            // throughout this batch, in addition to the generation/publication
            // checks above. Keep child-world compilation out of this issuer:
            // it cannot satisfy ordinary entry/publication and uncompiled state.

            auto original{::uwvm2::uwvm::runtime::full::selected_full_source_owner_pin()};
            auto const* module{record.runtime_module};
            auto const id{original ? original->bound_initialized_module_id(module) : SIZE_MAX};
            if(!source::has_canonical_owner(original) || !original->initialized_from_actual_state() ||
               module == nullptr || id >= g_runtime.modules.size() ||
               &g_runtime.modules.index_unchecked(id) != &record ||
               original->bound_initialized_file(id, module) == nullptr ||
               &original->registry() != &::uwvm2::uwvm::runtime::storage::active_runtime_registry() ||
               record.llvm_jit_full_publication->source.get() != original.get() ||
               record.llvm_jit_full_publication->source.owner_before(original) ||
               original.owner_before(record.llvm_jit_full_publication->source))
            {
                ::uwvm2::runtime::llvm_jit_cache::details::runtime_cache_log_line(
                    u8"origin-admission-refused stage=source-identity canonical=", source::has_canonical_owner(original),
                    u8" registry-size=", original ? original->registry().size() : 0u,
                    u8" preloads=", original ? original->preloaded_file_count() : 0u);
                return {};
            }
            if(!original->has_current_cache_provider_bindings())
            {
                ::uwvm2::runtime::llvm_jit_cache::details::runtime_cache_log_line(u8"origin-admission-refused stage=native-provider-bindings");
                return {};
            }
            // FDs, preopens, quotas, arguments, environment and driver behavior
            // are current runtime state. Generated imports resolve through the
            // current module and canonical bridges; no prior capability enters
            // an object. Their contents neither grant cache-origin authority nor
            // disqualify this genuine cold compilation. Generation-bound plans
            // and debugger pointers are excluded before this scope is created.
            // Admission is acquired while the actual outer entry is still
            // admitted. It may now fail during stop: that is a cache miss,
            // never a fabricated private generation or a compile fallback.
            auto generation{g_runtime.execution_domain.try_enter()};
            if(!generation || generation.stop_requested())
            {
                ::uwvm2::runtime::llvm_jit_cache::details::runtime_cache_log_line(u8"origin-admission-refused stage=generation-lease");
                return {};
            }
            try { return ::std::shared_ptr<state>{new state{::std::move(generation), ::std::move(original), current_runtime_generation()}}; }
            catch(...) { return {}; }
        }
    public:
        class batch
        {
            friend class runtime_cache_origin_bridge;
            batch* previous_{};
            ::std::shared_ptr<state> actual_{};
        public:
            explicit batch(compiled_module_record const& actual) noexcept
                : previous_{current_}, actual_{previous_ == nullptr ? mint(actual) : ::std::shared_ptr<state>{}}
            { current_ = this; } // even a rejected nested scope masks any outer ticket
            batch(batch const&) = delete;
            batch& operator=(batch const&) = delete;
            ~batch()
            {
                if(actual_) { actual_->secret_phase.store(false, ::std::memory_order_release); }
                current_ = previous_;
            } // actual_ released after TLS/secret-read phase are cleared
        };
        [[nodiscard]] static reference capture() noexcept
        { return current_ && current_->actual_ && current_->actual_->authorized() ? reference{current_->actual_} : reference{}; }
    };
    [[gnu::visibility("hidden")]]
    ::uwvm2::runtime::llvm_jit_cache::runtime_origin::reference runtime_cache_current_origin() noexcept
    { return runtime_cache_origin_bridge::capture(); }
}
#endif
