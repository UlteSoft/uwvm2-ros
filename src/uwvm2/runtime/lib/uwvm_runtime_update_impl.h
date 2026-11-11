// Included in the runtime aggregation TU, after private replacement compilation.
// The production publisher never calls a debugger mutation entry point.
#pragma once
namespace production_update_detail
{
    namespace update = ::uwvm2::utils::update;
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
    namespace immutable_globals = ::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details::immutable_globals;
    inline void append_execution_safety_policy(::std::vector<unsigned char>& material)
    {
    // The issuer authorizes this execution-safety contract, separately
    // from debugger permissions. Diagnostic continuation must never
    // masquerade as an enforcing crash policy in a signed update.
    namespace mt=::uwvm2::extension::memtag;
    update::append_u64(material,2u); // Explicit software/hardware and sync/async policy contract.
    update::append_u64(material,::uwvm2::uwvm::runtime::runtime_mode::runtime_disable_memtag);
    update::append_u64(material,static_cast<unsigned>(mt::selected_fault_delivery.load(::std::memory_order_relaxed)));
    update::append_u64(material,static_cast<unsigned>(mt::selected_report_action.load(::std::memory_order_relaxed)));
    }
    [[nodiscard]] inline bool effective_policy_fingerprint(update::digest& fingerprint) noexcept
    {
        try
        {
            ::std::vector<unsigned char> material;
            append_execution_safety_policy(material);
            auto const& file{::uwvm2::uwvm::wasm::storage::active_execute_wasm()};
            update::append_u64(material, file.module_name.size());
            for(auto c : file.module_name) { material.push_back(static_cast<unsigned char>(c)); }
#if !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1) && defined(UWVM_IMPORT_WASI_WASIP1)
            namespace wasi = ::uwvm2::uwvm::imported::wasi::wasip1::storage;
            auto const* configured{wasi::find_wasip1_module_override(wasip1_module_target_kind_t::main_wasm, file.module_name)};
            if(configured)
            {
                update::append_u64(material, reinterpret_cast<::std::uintptr_t>(configured));
                update::append_u64(material, configured->enabled);
                if(!::uwvm2::imported::wasi::wasip1::environment::serialize_policy(configured->env,
                    [&](unsigned char byte) { material.push_back(byte); })) { return false; }
            }
            else { update::append_u64(material, 0); } // No module binding means an empty effective host policy.
#endif
            return update::hash(material, fingerprint);
        }
        catch(...) { return false; }
    }
    [[nodiscard]] inline unsigned char* scalar_bytes(::uwvm2::object::global::wasm_global_storage_t& global,
        update::scalar_reset_record const& change) noexcept
    {
        using kind = ::uwvm2::object::global::global_type;
        if(global.is_mutable == change.immutable) { return nullptr; }
        switch(change.type)
        {
            case 0x7f: if(global.kind == kind::wasm_i32 && change.width == sizeof(global.storage.i32)) { return reinterpret_cast<unsigned char*>(::std::addressof(global.storage.i32)); } break;
            case 0x7e: if(global.kind == kind::wasm_i64 && change.width == sizeof(global.storage.i64)) { return reinterpret_cast<unsigned char*>(::std::addressof(global.storage.i64)); } break;
            case 0x7d: if(global.kind == kind::wasm_f32 && change.width == sizeof(global.storage.f32)) { return reinterpret_cast<unsigned char*>(::std::addressof(global.storage.f32)); } break;
            case 0x7c: if(global.kind == kind::wasm_f64 && change.width == sizeof(global.storage.f64)) { return reinterpret_cast<unsigned char*>(::std::addressof(global.storage.f64)); } break;
        }
        return nullptr;
    }
    [[nodiscard]] inline bool scalar_preflight(::std::span<update::scalar_reset_record const> changes, immutable_globals::owner const& binding, bool reset_mutable = false) noexcept
    {
        if(changes.empty()) { return true; }
        if(g_runtime.modules.size() != 1 || !g_runtime.modules.front_unchecked().runtime_module) { return false; }
        auto const& module{*g_runtime.modules.front_unchecked().runtime_module};
        if(!module.imported_function_vec_storage.empty() || !module.imported_global_vec_storage.empty()) { return false; }
        for(auto const& change : changes)
        {
            if(change.index >= module.local_defined_global_vec_storage.size() || change.width == 0 || change.width > 8) { return false; }
            auto const& slot{module.local_defined_global_vec_storage.index_unchecked(change.index)};
            if(!slot.global_type_ptr || slot.global_type_ptr->is_mutable == change.immutable) { return false; }
            auto const* bytes{scalar_bytes(const_cast<::uwvm2::object::global::wasm_global_storage_t&>(slot.global), change)};
            if(change.immutable && binding)
            {
                if(binding->module() != ::std::addressof(module)) { return false; }
                if(auto const* previous{binding->find(change.index)})
                {
                    if(previous->data.type != change.type) { return false; }
                    bytes = static_cast<unsigned char const*>(previous->address());
                }
            }
            if(!bytes) { return false; }
            if(reset_mutable && !change.immutable) { continue; }
            for(unsigned i{}; i < change.width; ++i)
            {
                auto const position{::std::endian::native == ::std::endian::little ? i : change.width - 1u - i};
                if(bytes[position] != change.old_bits[i]) { return false; }
            }
        }
        return true;
    }
    inline void publish_scalars(::std::span<update::scalar_reset_record const> changes) noexcept
    {
        auto const& module{*g_runtime.modules.front_unchecked().runtime_module};
        for(auto const& change : changes)
        {
            if(change.immutable) { continue; } // New binding, never a write to an old immutable object.
            auto& global{const_cast<::uwvm2::object::global::wasm_global_storage_t&>(module.local_defined_global_vec_storage.index_unchecked(change.index).global)};
            auto* bytes{scalar_bytes(global, change)}; // Exact type, range and old bits preflighted under the same fence.
            for(unsigned i{}; i < change.width; ++i)
            {
                auto const position{::std::endian::native == ::std::endian::little ? i : change.width - 1u - i};
                bytes[position] = change.new_bits[i];
            }
        }
    }
    struct session
    {
        update::protocol protocol{};
        update::digest policy_fingerprint{};
        ::std::thread::id management_thread{::std::this_thread::get_id()};
        ::std::vector<unsigned char> installed{}, target{};
        ::std::vector<::std::unique_ptr<llvm_jit_debug_replace_transaction>> functions{};
        ::std::vector<update::scalar_reset_record> scalar_resets{};
        immutable_globals::owner current_global_binding{}, pending_global_binding{};
        ::std::unique_ptr<update::module_transition_plan> module_plan{};
        ::std::shared_ptr<::std::vector<unsigned char> const> current_data_image{}, pending_data_image{};
#if !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1) && defined(UWVM_IMPORT_WASI_WASIP1) && defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        using policy_plan_type = update::prepared_wasi_policy<::uwvm2::object::memory::linear::native_memory_t>;
        ::std::unique_ptr<policy_plan_type> policy_plan{};
#endif
        update::digest pending_contract{}, pending_policy_fingerprint{}, pending_state_measurement{};
        ::std::uint64_t pending_policy_generation{}, pending_revocation_generation{};
#if UWVM2_HAS_THREADED_PROFILE_HOST_API
        ::std::unique_ptr<threaded_management_exclusion> threaded_exclusion{};
#endif
        ::uwvm2::utils::thread::execution_domain::maintenance_transition maintenance{};
        ::uwvm2::utils::thread::execution_domain::private_generation_gate fence{};
        ::std::uint_least64_t runtime_generation{};
        ::std::uint64_t serial{};
        [[nodiscard]] bool has_policy_change() const noexcept
        {
#if !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1) && defined(UWVM_IMPORT_WASI_WASIP1) && defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            return static_cast<bool>(policy_plan);
#else
            return false;
#endif
        }
        void discard_candidate() noexcept
        {
            functions.clear(); scalar_resets.clear(); target.clear(); pending_global_binding.reset();
            module_plan.reset(); pending_data_image.reset(); pending_contract = {}; pending_policy_fingerprint = {}; pending_state_measurement = {};
            pending_policy_generation = 0u; pending_revocation_generation = 0u;
#if !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1) && defined(UWVM_IMPORT_WASI_WASIP1) && defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            policy_plan.reset();
#endif
        }
        [[nodiscard]] bool policy_matches() const noexcept
        { update::digest actual{}; return effective_policy_fingerprint(actual) && actual == policy_fingerprint; }
        [[nodiscard]] bool same_thread() const noexcept { return management_thread == ::std::this_thread::get_id(); }
        [[nodiscard]] bool reopen(bool private_cancel = false) noexcept
        {
            if(::uwvm2::runtime::diagnostic::crash::first_fault.load(::std::memory_order_acquire) ||
               runtime_managed_fault::faulted(runtime_generation) ||
               ::uwvm2::utils::thread::process_exit::requested.load(::std::memory_order_acquire)) { return false; }
            if(!same_thread() || !maintenance || !fence ||
               !(private_cancel ? g_runtime.execution_domain.cancel_private_update_fence(maintenance, fence) :
                   g_runtime.execution_domain.reopen_public_admission(maintenance, fence))) { return false; }
            maintenance = {};
#if UWVM2_HAS_THREADED_PROFILE_HOST_API
            threaded_exclusion.reset();
#endif
            return true;
        }
    };
    struct diagnostic_update_scope
    {
        session& owner;
        ::std::unique_lock<::std::mutex> debugger_exclusion{update_debug_coordination::management};
        explicit diagnostic_update_scope(session& value) noexcept : owner{value}
        { diagnostic_detail::update_in_progress.store(true, ::std::memory_order_release); }
        ~diagnostic_update_scope()
        {
            update_debug_coordination::fenced.store(static_cast<bool>(owner.fence), ::std::memory_order_release);
            diagnostic_detail::update(owner.protocol.observe());
            diagnostic_detail::update_in_progress.store(false, ::std::memory_order_release);
        }
    };
    // Destroyed before g_runtime. Pending ownership closes admission on detach;
    // no destructor resumes old code or rolls back an authenticated COMMIT.
    inline ::std::unique_ptr<session> current{};
    [[nodiscard]] inline bool available() noexcept
    {
        if(!current || !current->same_thread() || current->runtime_generation != current_runtime_generation()) { return false; }
        // report() owns the first terminal fault before any debugger pause or
        // managed native unwind. The stopped old epoch must never reopen via
        // a private cancel, preserve/migrate transaction or debugger authority.
        if(::uwvm2::runtime::diagnostic::crash::first_fault.load(::std::memory_order_acquire) ||
           runtime_managed_fault::faulted(current->runtime_generation)) { current->protocol.guest_faulted(); }
        else if(::uwvm2::utils::thread::process_exit::requested.load(::std::memory_order_acquire))
        { current->protocol.process_exit_requested(); }
        return true;
    }
    [[nodiscard]] inline bool supported_world() noexcept
    {
        if(g_runtime.modules.size() != 1 || !g_runtime.compiled_all.load(::std::memory_order_acquire)) { return false; }
        auto const& rec{g_runtime.modules.front_unchecked()}; auto const* module{rec.runtime_module};
        if(!module || !rec.llvm_jit_ready || rec.llvm_jit_debug_full_typed_entry_targets.empty() ||
           (rec.llvm_jit_full_publication && (rec.llvm_jit_full_publication->plan ||
            (rec.llvm_jit_full_publication->checkpoint_profile &&
             rec.llvm_jit_full_publication->checkpoint_profile->purpose() != ::uwvm2::runtime::checkpoint::compilation_purpose::observe_values))) ||
           !module->imported_memory_vec_storage.empty() || !module->imported_table_vec_storage.empty() ||
           !module->imported_global_vec_storage.empty()) { return false; }
        for(auto const& memory : module->local_defined_memory_vec_storage)
        {
            if(!memory.memory_type_ptr) { return false; }
            if constexpr(requires { memory.memory_type_ptr->shared; })
            { if(memory.memory_type_ptr->shared) { return false; } }
        }
        auto source{::uwvm2::uwvm::runtime::full::selected_full_source_owner_pin()};
        if(!source) { return false; }
        for(::std::size_t i{}; i < module->imported_function_vec_storage.size(); ++i)
        {
            ::uwvm2::uwvm::runtime::full::builtin_wasip1_function_data function{};
            if(!source->actual_builtin_wasip1_function(0u, current_runtime_generation(), module, i, function)) { return false; }
        }
        return true;
    }
    // Observation metadata is code-generation state, not resumable execution
    // authority. Keep the same owner/version checks as debugger replacement.
    [[nodiscard]] inline bool observation_preflight(compiled_module_record const& rec,
        llvm_jit_debug_replace_transaction const& f) noexcept
    {
        auto const publication{rec.llvm_jit_full_publication.get()};
        if(!publication || !publication->checkpoint_profile) { return !f.checkpoint_plan; }
        auto const& profile{publication->checkpoint_profile}; auto const& plan{f.checkpoint_plan};
        auto const count{rec.runtime_module->local_defined_function_vec_storage.size()};
        return profile->purpose() == ::uwvm2::runtime::checkpoint::compilation_purpose::observe_values &&
            profile.get() == g_runtime.checkpoint_profile.get() &&
            !profile.owner_before(g_runtime.checkpoint_profile) && !g_runtime.checkpoint_profile.owner_before(profile) &&
            plan && plan->get().resume_abi_revision == 0u &&
            plan->get().module == f.module_id && plan->get().function == f.function_index &&
            plan->get().function_generation == f.expected_generation + 1u &&
            plan->get().profile.get() == profile.get() && !plan->get().profile.owner_before(profile) &&
            !profile.owner_before(plan->get().profile) &&
            rec.llvm_jit_checkpoint_generation_owners.size() == count &&
            publication->checkpoint_resume_entries.size() == count && rec.llvm_jit_compiled.local_funcs.size() == count;
    }
    [[nodiscard]] inline bool prepare_global_binding(session& s)
    {
        s.pending_global_binding = s.current_global_binding;
        bool changed{};
        auto values{s.current_global_binding ? s.current_global_binding->values() : ::std::vector<immutable_globals::value>{}};
        for(auto const& change : s.scalar_resets)
        {
            if(!change.immutable) { continue; }
            auto where{::std::lower_bound(values.begin(), values.end(), change.index,
                [](auto const& item, auto index) { return item.index < index; })};
            immutable_globals::value replacement{change.index, change.type, change.new_bits};
            if(where != values.end() && where->index == change.index) { *where = replacement; }
            else { values.insert(where, replacement); }
            changed = true;
        }
        if(!changed) { return true; }
        s.pending_global_binding = immutable_globals::binding_owner::create(g_runtime.modules.front_unchecked().runtime_module, values);
        return static_cast<bool>(s.pending_global_binding);
    }
#include "uwvm_runtime_update_transition_impl.h"
    template<typename V, typename Less, typename Equal>
    inline void canonicalize(V& v, Less less, Equal equal)
    {
        ::std::sort(v.begin(), v.end(), less); ::std::size_t count{};
        for(auto const& item : v)
        { if(count == 0 || !equal(v.index_unchecked(count - 1), item)) { v.index_unchecked(count++) = item; } }
        v.resize(count);
    }
    inline void merge_metadata(session& s)
    {
        auto& merged{*s.functions.front()};
        for(::std::size_t i{1}; i < s.functions.size(); ++i)
        {
            for(auto const& entry : s.functions[i]->prepared_unwind_entries) { merged.prepared_unwind_entries.push_back(entry); }
            for(auto const& entry : s.functions[i]->prepared_code_ranges) { merged.prepared_code_ranges.push_back(entry); }
            for(auto const& entry : s.functions[i]->prepared_native_function_ranges) { merged.prepared_native_function_ranges.push_back(entry); }
        }
        canonicalize(merged.prepared_unwind_entries, [](auto const& a, auto const& b) { return a.address < b.address; },
            [](auto const& a, auto const& b) { return a.address == b.address; });
        auto less{[](auto const& a, auto const& b) { return a.begin < b.begin || (a.begin == b.begin && a.end < b.end); }};
        auto equal{[](auto const& a, auto const& b) { return a.begin == b.begin && a.end == b.end; }};
        canonicalize(merged.prepared_code_ranges, less, equal);
        canonicalize(merged.prepared_native_function_ranges, less, equal);
    }
#endif
}

extern "C++" ::uwvm2::utils::update::status runtime_update_configure_host_api(
    ::uwvm2::utils::update::configuration const& config) noexcept
{
    namespace u = ::uwvm2::utils::update;
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
    namespace mode = ::uwvm2::uwvm::runtime::runtime_mode;
    if(production_update_detail::current ||
       ::uwvm2::utils::security::wsea::secure_enabled() ||
       mode::global_runtime_mode != mode::runtime_mode_t::full_compile || mode::global_runtime_compiler != mode::runtime_compiler_t::llvm_jit_only)
    { return u::status::unsupported; }
#if UWVM2_HAS_THREADED_PROFILE_HOST_API
    if(runtime_threaded_profile_builtin_requested_host_api()) { return u::status::unsupported; }
#endif
    auto const& file{::uwvm2::uwvm::wasm::storage::active_execute_wasm()};
    if(!file.has_owned_source_image() || file.source_size() > u::maximum_image_bytes || file.source_size() < 8 ||
       !::uwvm2::uwvm::wasm::storage::preloaded_wasm.empty()) { return u::status::unsupported; }
    try
    {
        auto state{::std::make_unique<production_update_detail::session>()};
        auto const* first{reinterpret_cast<unsigned char const*>(file.source_cbegin())};
        state->installed.assign(first, first + file.source_size()); u::digest base{};
        if(!u::hash(state->installed, base)) { return u::status::unsupported; }
        if(!production_update_detail::effective_policy_fingerprint(state->policy_fingerprint)) { return u::status::invalid_input; }
        auto measured_config{config}; ::std::array<unsigned char, 64> policy{};
        ::std::copy(config.contract.begin(), config.contract.end(), policy.begin());
        ::std::copy(state->policy_fingerprint.begin(), state->policy_fingerprint.end(), policy.begin()+32);
        if(!u::hash(policy, measured_config.contract)) { return u::status::unsupported; }
        if(auto result{state->protocol.configure(measured_config, base)}; result != u::status::ok) { return result; }
        auto domain{::std::make_shared<::uwvm2::utils::thread::cooperative_pause_domain>()};
        bool configured{};
        if(get_runtime_execution_entry_depth() != 0 || get_runtime_state_publication_depth() != 0 ||
           get_runtime_compilation_metadata_callback_depth() != 0) { return u::status::invalid_state; }
#if UWVM2_HAS_THREADED_PROFILE_HOST_API
        state->threaded_exclusion.reset(new threaded_management_exclusion{
            runtime_threaded_profile_try_management_host_api(threaded_management_kind::incremental_mutation)});
        if(!*state->threaded_exclusion) { return u::status::busy; }
#endif
        state->maintenance = g_runtime.execution_domain.try_maintenance_transition();
        if(!state->maintenance) { return u::status::busy; }
        state->fence = g_runtime.execution_domain.configure_and_suspend_public_admission(state->maintenance, [&]() noexcept
        {
            runtime_state_publication_guard publication{};
            if(g_runtime.compiled_all.load(::std::memory_order_acquire) || g_runtime.bridges_initialized.load(::std::memory_order_acquire)) { return; }
            // The same code-generation machinery is private to this production
            // profile; no observer/controller/DeveloperGrant is installed.
            if(g_runtime.debug_pause_control)
            {
                // A separate authenticated debugger was configured while cold.
                // Keep its exact observer/domain and emitted value policy.
                if(g_runtime.debug_pause_control->is_closed() || !g_runtime.debug_observer.context ||
                   !g_runtime.checkpoint_profile || g_runtime.checkpoint_profile->purpose() !=
                       ::uwvm2::runtime::checkpoint::compilation_purpose::observe_values) { return; }
            }
            else
            {
                g_runtime.debug_pause_control = ::std::move(domain);
                g_runtime.debug_granularity = llvm_jit_debug_safe_point_granularity::entry_loop;
            }
            state->runtime_generation = current_runtime_generation(); configured = true;
        });
        if(!state->fence || !configured) { return u::status::invalid_state; }
        // Reuse all ordinary preparation checks with a real private lease.
        // Public callers cannot obtain this internal token or compile helper.
        auto lease{state->fence.try_enter()};
        if(!lease || !full_compile_prepare_internal(::std::move(lease)
#if UWVM2_HAS_THREADED_PROFILE_HOST_API
            , state->threaded_exclusion.get()
#endif
            )) { return u::status::invalid_state; }
        if(!production_update_detail::supported_world() || !state->policy_matches()) { return u::status::unsupported; }
        runtime_managed_fault::enable(state->runtime_generation);
        production_update_detail::current = ::std::move(state);
        update_debug_coordination::configured.store(true, ::std::memory_order_release);
        update_debug_coordination::fenced.store(true, ::std::memory_order_release);
        diagnostic_detail::update(production_update_detail::current->protocol.observe());
        return u::status::ok;
    }
    catch(...) { return u::status::exhausted; }
#else
    (void)config; return u::status::unsupported;
#endif
}
extern "C++" ::uwvm2::utils::update::status runtime_update_launch_host_api(
    ::uwvm2::utils::update::permit const& permit) noexcept
{
    namespace u = ::uwvm2::utils::update;
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
    if(!production_update_detail::available()) { return u::status::invalid_state; }
    auto& s{*production_update_detail::current};
    production_update_detail::diagnostic_update_scope observed_update{s};
    // An authenticated launch retry is only a receipt, including while a later
    // update holds the fence or after opening this launch failed. Never reopen.
    if(s.protocol.launch_authorized()) { return s.protocol.authorize_launch(permit); }
    if(!s.maintenance || !s.fence || !s.policy_matches() || !production_update_detail::supported_world() ||
       !g_runtime.execution_domain.drain_update_until(s.maintenance, s.fence, ::std::chrono::steady_clock::now()))
    { return u::status::invalid_state; }
    if(auto result{s.protocol.authorize_launch(permit)}; result != u::status::ok) { return result; }
    if(!s.reopen()) { return u::status::committed_but_not_active; }
    if(!s.protocol.launch_activated()) { s.protocol.suspend_execution(); return u::status::committed_but_not_active; }
    return u::status::ok;
#else
    (void)permit; return u::status::unsupported;
#endif
}
extern "C++" ::uwvm2::utils::update::status runtime_update_observe_host_api(::uwvm2::utils::update::facts& out) noexcept
{
    namespace u = ::uwvm2::utils::update;
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
    if(!production_update_detail::available()) { return u::status::invalid_state; }
    out = production_update_detail::current->protocol.observe();
    diagnostic_detail::update(out); return u::status::ok;
#else
    (void)out; return u::status::unsupported;
#endif
}
static ::uwvm2::utils::update::status prepare_update_internal(
    ::uwvm2::utils::update::permit const& permit, ::std::span<unsigned char const> target,
    ::std::span<::std::uint64_t const> functions, ::std::chrono::steady_clock::time_point deadline,
    ::uwvm2::utils::update::transition_manifest const* transition = nullptr, ::std::span<unsigned char const> declaration = {}) noexcept
{
    namespace u = ::uwvm2::utils::update;
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
    if(!production_update_detail::available()) { return u::status::invalid_state; }
    if(target.size() > u::maximum_image_bytes || target.size() < 8 || (functions.empty() && !(transition && transition->changes_policy)) || functions.size() > u::maximum_functions) { return u::status::invalid_input; }
    auto& s{*production_update_detail::current};
    production_update_detail::diagnostic_update_scope observed_update{s};
    if(auto accepted{s.protocol.begin(permit)}; accepted != u::status::ok) { return accepted; }
    auto fail{[&](u::status error) noexcept
    {
        s.discard_candidate();
        (void)s.protocol.cancel_private();
        if(s.fence && !s.reopen(true)) { s.protocol.suspend_execution(); return u::status::invalid_state; }
        s.maintenance = {};
#if UWVM2_HAS_THREADED_PROFILE_HOST_API
        s.threaded_exclusion.reset();
#endif
        return error;
    }};
    try
    {
        // Copy once. Hash, scope validation, compile and activation use this owner.
        s.target.assign(target.begin(), target.end()); u::digest actual{}, scope{};
        ::std::vector<unsigned char> indices; indices.reserve(functions.size() * 8);
        for(auto index : functions) { u::append_u64(indices, index); }
        if(!u::hash(s.target, actual) || actual != permit.target || !u::hash(declaration.empty() ? ::std::span<unsigned char const>{indices} : declaration, scope) || scope != permit.functions) { return fail(u::status::unauthorized); }
        ::std::vector<u::function_body> bodies;
        if(transition && (transition->module_reset || transition->module_preserve || transition->module_migrate))
        {
            s.module_plan = ::std::make_unique<u::module_transition_plan>();
            if(auto checked{u::compare_module_transition(s.installed, s.target, functions, transition->module_reset || transition->module_migrate, *s.module_plan)}; checked != u::status::ok) { return fail(checked); }
            bodies = ::std::move(s.module_plan->bodies); s.scalar_resets = ::std::move(s.module_plan->scalars);
            if(s.module_plan->reset_state && !s.module_plan->data_segments.empty()) { s.pending_data_image = ::std::make_shared<::std::vector<unsigned char> const>(s.target); }
        }
        else if(functions.empty())
        { if(s.installed != s.target) { return fail(u::status::scope_mismatch); } }
        else if(auto checked{u::compare_scope(s.installed, s.target, functions, bodies, s.scalar_resets)}; checked != u::status::ok) { return fail(checked); }
#if UWVM2_HAS_THREADED_PROFILE_HOST_API
        s.threaded_exclusion.reset(new threaded_management_exclusion{runtime_threaded_profile_try_management_host_api(threaded_management_kind::incremental_mutation)});
        if(!*s.threaded_exclusion) { return fail(u::status::busy); }
#endif
        s.maintenance = g_runtime.execution_domain.try_maintenance_transition();
        if(!s.maintenance) { return fail(u::status::busy); }
        s.fence = g_runtime.execution_domain.fence_for_update(s.maintenance);
        if(s.fence) { s.protocol.suspend_execution(); diagnostic_detail::update(s.protocol.observe()); }
        if(!s.fence || !g_runtime.execution_domain.drain_update_until(s.maintenance, s.fence, deadline)) { return fail(u::status::busy); }
        if(!production_update_detail::supported_world()) { return fail(u::status::unsupported); }
        if(!s.policy_matches()) { return fail(u::status::unauthorized); }
        if(transition && transition->changes_policy)
        {
#if !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1) && defined(UWVM_IMPORT_WASI_WASIP1) && defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            auto* binding{production_update_detail::active_policy_binding()};
            if(!binding || !binding->enabled_is_set) { return fail(u::status::unsupported); }
            auto const& config{s.protocol.current_configuration()};
            auto prepared{u::prepare_wasi_policy(binding->env, transition->policy, config.policy_generation,
                transition->target_policy_generation, config.revocation_generation, transition->target_revocation_generation)};
            if(prepared.result != u::status::ok || !prepared.plan) { return fail(prepared.result); }
            s.policy_plan = ::std::move(prepared.plan); s.pending_policy_generation = transition->target_policy_generation;
            s.pending_revocation_generation = transition->target_revocation_generation;
            if(!production_update_detail::projected_policy_fingerprint(*s.policy_plan, s.pending_policy_fingerprint)) { return fail(u::status::invalid_input); }
            ::std::array<unsigned char, 64> contract{};
            ::std::copy(transition->approved_contract_reference.begin(), transition->approved_contract_reference.end(), contract.begin());
            ::std::copy(s.pending_policy_fingerprint.begin(), s.pending_policy_fingerprint.end(), contract.begin()+32u);
            if(!u::hash(contract, s.pending_contract)) { return fail(u::status::unsupported); }
#else
            return fail(u::status::unsupported);
#endif
        }
        if(!production_update_detail::prepare_memory_tags(s) || !production_update_detail::module_preflight(s) ||
           !production_update_detail::policy_change_preflight(s)) { return fail(u::status::scope_mismatch); }
        if(!production_update_detail::scalar_preflight(s.scalar_resets, s.current_global_binding, s.module_plan && s.module_plan->reset_state)) { return fail(u::status::scope_mismatch); }
        if(transition && transition->module_migrate)
        {
            u::migration_plan migration{};
            if(auto checked{u::decode_migration(transition->migration, migration)}; checked != u::status::ok) { return fail(checked); }
            if(auto migrated{production_update_detail::prepare_migration(s, migration)}; migrated != u::status::ok) { return fail(migrated); }
        }
        if(s.module_plan && s.module_plan->reset_state && !u::measure_migrated_state(*s.module_plan, s.scalar_resets, s.pending_state_measurement))
        { return fail(u::status::exhausted); }
        if(!production_update_detail::prepare_global_binding(s)) { return fail(u::status::invalid_input); }
        auto& rec{g_runtime.modules.front_unchecked()};
        if(rec.llvm_jit_debug_full_retained_generations.size() + functions.size() > u::maximum_transactions * u::maximum_functions) { return fail(u::status::exhausted); }
        s.functions.reserve(functions.size());
        for(auto index : functions)
        {
            if((index & (u::global_reset_tag | u::immutable_global_tag)) != 0) { continue; }
            if(index >= rec.llvm_jit_debug_full_entry_generations.size() || bodies[index].size > 65536) { return fail(u::status::unsupported); }
            auto result{prepare_function_replacement_internal(0u, index + rec.runtime_module->imported_function_vec_storage.size(),
                rec.llvm_jit_debug_full_entry_generations[index], reinterpret_cast<::std::byte const*>(s.target.data() + bodies[index].offset), bodies[index].size, true, s.pending_global_binding)};
            if(result.status != llvm_jit_debug_replace_status::replaced || !result.transaction)
            {
                using preparation=llvm_jit_debug_replace_status;
                switch(result.status)
                {
                    case preparation::exhausted:return fail(u::status::exhausted);
                    case preparation::invalid_body:return fail(u::status::validation_failed);
                    case preparation::unsupported_mode:return fail(u::status::unsupported);
                    case preparation::invalid_target:case preparation::abi_mismatch:return fail(u::status::scope_mismatch);
                    case preparation::stale_generation:return fail(u::status::stale_base);
                    case preparation::compile_failure:return fail(u::status::compile_failed);
                    default:return fail(u::status::invalid_state);
                }
            }
            s.functions.emplace_back(result.transaction);
            if(!production_update_detail::observation_preflight(rec, *s.functions.back())) { return fail(u::status::compile_failed); }
        }
        rec.llvm_jit_debug_full_retained_generations.reserve(rec.llvm_jit_debug_full_retained_generations.size() + s.functions.size());
        if(!s.functions.empty()) { production_update_detail::merge_metadata(s); }
        auto measured{permit.encoding(false)};
        measured.insert(measured.end(), actual.begin(), actual.end()); u::append_u64(measured, s.runtime_generation);
        u::append_u64(measured, ++s.serial);
        measured.insert(measured.end(), s.pending_contract.begin(), s.pending_contract.end());
        measured.insert(measured.end(), s.pending_state_measurement.begin(), s.pending_state_measurement.end());
        u::append_u64(measured, s.pending_policy_generation); u::append_u64(measured, s.pending_revocation_generation);
        u::digest measurement{};
        if(!u::hash(measured, measurement) || !s.protocol.prepared(measurement)) { return fail(u::status::invalid_state); }
        return u::status::ok;
    }
    catch(...) { return fail(u::status::exhausted); }
#else
    (void)permit; (void)target; (void)functions; (void)deadline; (void)transition; (void)declaration; return u::status::unsupported;
#endif
}
#include "uwvm_runtime_update_transition_api.h"
extern "C++" ::uwvm2::utils::update::status runtime_update_ready_host_api() noexcept
{
    namespace u = ::uwvm2::utils::update;
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
    if(!production_update_detail::available()) { return u::status::invalid_state; }
    auto& s{*production_update_detail::current};
    production_update_detail::diagnostic_update_scope observed_update{s};
    if(!s.maintenance || !s.fence || !s.policy_matches() || !production_update_detail::module_preflight(s) || !production_update_detail::policy_change_preflight(s) || (s.functions.empty() && s.scalar_resets.empty() && !s.has_policy_change() && !s.module_plan) || !production_update_detail::scalar_preflight(s.scalar_resets, s.current_global_binding, s.module_plan && s.module_plan->reset_state) || s.protocol.observe().state != u::phase::prepared ||
       !g_runtime.execution_domain.drain_update_until(s.maintenance, s.fence, ::std::chrono::steady_clock::now())) { return u::status::invalid_state; }
    auto const& facts{s.protocol.observe()};
    ::std::array<unsigned char, 104> material{};
    ::std::copy(facts.prepared.begin(), facts.prepared.end(), material.begin());
    ::std::copy(facts.nonce.begin(), facts.nonce.end(), material.begin() + 32);
    ::std::copy(facts.installed.begin(), facts.installed.end(), material.begin() + 64);
    for(unsigned i{}; i < 8; ++i) { material[96+i] = static_cast<unsigned char>(s.serial >> (i*8)); }
    // The serial identifies this retained, drained state view. It cannot be
    // mutated by guest entry, debugger, reset or a second prepared transaction.
    u::digest ready{};
    return u::hash(material, ready) && s.protocol.ready(ready) ? u::status::ok : u::status::invalid_state;
#else
    return u::status::unsupported;
#endif
}
extern "C++" ::uwvm2::utils::update::status runtime_update_cancel_private_host_api() noexcept
{
    namespace u = ::uwvm2::utils::update;
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
    if(!production_update_detail::available()) { return u::status::invalid_state; }
    auto& s{*production_update_detail::current};
    production_update_detail::diagnostic_update_scope observed_update{s};
    if(!s.protocol.cancel_private()) { return u::status::invalid_state; }
    s.discard_candidate();
    if(!s.reopen(true)) { s.protocol.suspend_execution(); return u::status::invalid_state; }
    return u::status::ok;
#else
    return u::status::unsupported;
#endif
}
extern "C++" ::uwvm2::utils::update::status runtime_update_decide_host_api(::uwvm2::utils::update::permit const& permit) noexcept
{
    namespace u = ::uwvm2::utils::update;
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
    if(!production_update_detail::available()) { return u::status::invalid_state; }
    auto& s{*production_update_detail::current};
    production_update_detail::diagnostic_update_scope observed_update{s};
    if(auto accepted{s.protocol.decide(permit)}; accepted != u::status::ok) { return accepted; }
    if(s.protocol.observe().state == u::phase::active) { return u::status::ok; } // Receipt lost: no second application.
    if(s.protocol.observe().state == u::phase::aborted)
    {
        s.discard_candidate();
        if(s.fence && !s.reopen()) { s.protocol.suspend_execution(); return u::status::invalid_state; }
        return u::status::ok;
    }
    auto failed{[&]() noexcept { s.protocol.activation_failed(); return u::status::committed_but_not_active; }};
    if(s.protocol.observe().state != u::phase::committed || !s.maintenance || !s.fence || !s.policy_matches() || !production_update_detail::module_preflight(s) || !production_update_detail::policy_change_preflight(s) || (s.functions.empty() && s.scalar_resets.empty() && !s.has_policy_change() && !s.module_plan) || !production_update_detail::scalar_preflight(s.scalar_resets, s.current_global_binding, s.module_plan && s.module_plan->reset_state) ||
       !g_runtime.execution_domain.drain_update_until(s.maintenance, s.fence, ::std::chrono::steady_clock::now())) { return failed(); }
    auto& rec{g_runtime.modules.front_unchecked()};
    // All checks precede a single bounded publication. The already durable
    // decision is never undone if local preflight fails.
    if(rec.llvm_jit_debug_full_retained_generations.capacity() - rec.llvm_jit_debug_full_retained_generations.size() < s.functions.size()) { return failed(); }
    for(auto const& f : s.functions)
    {
        if(!f || f->committed || f->expected_runtime_epoch != current_runtime_generation() ||
           f->local_index >= rec.llvm_jit_debug_full_entry_generations.size() ||
           rec.llvm_jit_debug_full_entry_generations[f->local_index] != f->expected_generation || !f->metadata_ready ||
           !production_update_detail::observation_preflight(rec, *f) || f->typed_entry == 0 || f->raw_entry == 0) { return failed(); }
    }
    auto const first_new_generation{rec.llvm_jit_debug_full_retained_generations.size()};
    {
        runtime_state_publication_guard publication{};
#if defined(UWVM_EXPERIMENTAL_NATIVE_OWNER_TABLE_V2) && UWVM_EXPERIMENTAL_NATIVE_OWNER_TABLE_V2 == 1
        // The real generated owner-table proof remains mandatory for EVERY
        // candidate before the one publication. No debugger grant is substituted.
        for(auto const& f : s.functions)
        { if(!details::native_owner_publication_bridge::qualifies_actual_replacement_for_commit(rec, *f)) { return failed(); } }
#endif
        if(!production_update_detail::publish_policy_change(s)) { return failed(); }
        if(!s.functions.empty())
        {
            auto& merged{*s.functions.front()};
            ::std::swap(g_runtime.llvm_jit_unwind_entries, merged.prepared_unwind_entries);
            ::std::swap(g_runtime.llvm_jit_code_ranges, merged.prepared_code_ranges);
            ::std::swap(g_runtime.llvm_jit_native_function_ranges, merged.prepared_native_function_ranges);
        }
        if(!production_update_detail::publish_module_reset(s)) { return failed(); }
        production_update_detail::publish_scalars(s.scalar_resets);
        for(auto& owner : s.functions)
        {
            auto* f{owner.get()}; auto const index{f->local_index};
            rec.llvm_jit_debug_full_retained_generations.emplace_back(::std::move(owner)); f->committed = true;
            if(f->checkpoint_plan)
            {
                rec.llvm_jit_compiled.local_funcs.index_unchecked(index).checkpoint_plan = f->checkpoint_plan;
                rec.llvm_jit_full_publication->checkpoint_resume_entries[index] = {f->checkpoint_plan, 0u};
                rec.llvm_jit_checkpoint_generation_owners[index] = f;
            }
#if defined(UWVM_EXPERIMENTAL_NATIVE_OWNER_TABLE_V2) && UWVM_EXPERIMENTAL_NATIVE_OWNER_TABLE_V2 == 1
            rec.actual_native_generation_positions[index] = rec.llvm_jit_debug_full_retained_generations.size() - 1u;
#endif
            if(f->debug_shutdown_abi_revision != 1u && rec.llvm_jit_full_publication)
            { rec.llvm_jit_full_publication->debug_shutdown_abi_revision.store(0u, ::std::memory_order_release); }
            ::std::atomic_ref<::std::uintptr_t>{rec.llvm_jit_local_raw_entry_addresses.index_unchecked(index)}.store(f->raw_entry, ::std::memory_order_release);
            ::std::atomic_ref<::std::uintptr_t>{rec.llvm_jit_local_entry_addresses.index_unchecked(index)}.store(f->typed_entry, ::std::memory_order_release);
            ::std::atomic_ref<::std::uintptr_t>{rec.llvm_jit_debug_full_typed_entry_targets.index_unchecked(index)}.store(f->typed_entry, ::std::memory_order_release);
            auto const generation{++rec.llvm_jit_debug_full_entry_generations[index]};
            rec.llvm_jit_debug_full_safe_points[index] = {f->debug_safe_point_bits.data(), f->debug_safe_point_bits.size(), f->debug_expression_size, generation};
        }
        s.current_global_binding = s.pending_global_binding;
        s.installed.swap(s.target); s.protocol.installed();
    }
    // No live execution lease remains. Invalidate old IDE source locations
    // before reopening actual admission, outside the publication mutex.
    if(update_debug_coordination::code_published && g_runtime.debug_observer.context)
    {
        for(auto i{first_new_generation}; i != rec.llvm_jit_debug_full_retained_generations.size(); ++i)
        {
            auto const& f{rec.llvm_jit_debug_full_retained_generations[i]};
            if(f && f->committed && f->expected_generation + 1u == rec.llvm_jit_debug_full_entry_generations[f->local_index])
            {
                auto const* begin{reinterpret_cast<::std::byte const*>(f->code.body.expr_begin)};
                auto const* end{reinterpret_cast<::std::byte const*>(f->code.body.code_end)};
                update_debug_coordination::code_published(g_runtime.debug_observer.context.get(),
                    f->module_id, f->function_index, {begin, static_cast<::std::size_t>(end-begin)}, f->expected_generation+1u);
            }
        }
    }
    s.discard_candidate();
    if(!s.reopen()) { return failed(); }
    s.protocol.activated(); return u::status::ok;
#else
    (void)permit; return u::status::unsupported;
#endif
}
extern "C++" void runtime_update_disconnected_host_api() noexcept
{
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
    if(production_update_detail::available()) { production_update_detail::current->protocol.disconnected(); diagnostic_detail::update(production_update_detail::current->protocol.observe()); }
#endif
}
