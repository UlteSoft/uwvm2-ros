// Private production_update_detail fragment: held native ownership and actual
// policy/state publication helpers. Never imported into guest address space.
#pragma once
[[nodiscard]] inline bool module_preflight(session const& s) noexcept
{
    if(!s.module_plan) { return true; }
    if(g_runtime.modules.size() != 1u || !g_runtime.modules.front_unchecked().runtime_module) { return false; }
    auto const& module{*g_runtime.modules.front_unchecked().runtime_module}; auto const& plan{*s.module_plan};
    if(module.local_defined_memory_vec_storage.size() != plan.memories.size() || module.local_defined_data_vec_storage.size() != plan.data_segments.size()) { return false; }
    for(auto const& memory : plan.memories)
    {
        if(memory.index >= module.local_defined_memory_vec_storage.size()) { return false; }
        auto const& actual{module.local_defined_memory_vec_storage.index_unchecked(memory.index).memory};
        if(actual.custom_page_size_log2 != 16u || actual.get_page_size() != memory.byte_size / 65536u ||
           (memory.byte_size != 0u && actual.memory_begin == nullptr) || (plan.reset_state && memory.bytes.size() != memory.byte_size)) { return false; }
    }
    for(::std::size_t i{}; i < plan.data_segments.size(); ++i)
    {
        auto const& actual{module.local_defined_data_vec_storage.index_unchecked(i).data}; auto const& proposed{plan.data_segments[i]};
        if(actual.kind != ::uwvm2::uwvm::runtime::storage::wasm_data_segment_kind::active ||
           !::uwvm2::uwvm::runtime::storage::wasm_data_segment_is_dropped(actual) || actual.memory_idx != proposed.memory_index) { return false; }
        if(plan.reset_state && (!s.pending_data_image || proposed.payload_offset > s.pending_data_image->size() ||
            proposed.payload_size > s.pending_data_image->size() - proposed.payload_offset)) { return false; }
    }
    return true;
}
[[nodiscard]] inline update::status prepare_migration(session& s, update::migration_plan const& migration)
{
    if(!s.module_plan || !s.module_plan->reset_state || !module_preflight(s)) { return update::status::invalid_state; }
    auto const& module{*g_runtime.modules.front_unchecked().runtime_module};
    ::std::vector<::std::span<unsigned char const>> source_memories; source_memories.reserve(s.module_plan->memories.size());
    for(auto const& memory : s.module_plan->memories)
    {
        auto const& actual{module.local_defined_memory_vec_storage.index_unchecked(memory.index).memory};
        source_memories.emplace_back(reinterpret_cast<unsigned char const*>(actual.memory_begin), memory.byte_size);
    }
    auto read_global{[&](update::scalar_reset_record const& source, ::std::array<unsigned char,8>& out) noexcept
    {
        if(source.immutable || source.index >= module.local_defined_global_vec_storage.size()) { return false; }
        auto const& record{module.local_defined_global_vec_storage.index_unchecked(source.index)};
        if(!record.global_type_ptr || !record.global_type_ptr->is_mutable) { return false; }
        auto const* bytes{scalar_bytes(const_cast<::uwvm2::object::global::wasm_global_storage_t&>(record.global), source)};
        if(!bytes) { return false; } out = {};
        for(unsigned i{}; i<source.width; ++i)
        { out[i] = bytes[::std::endian::native == ::std::endian::little ? i : source.width-1u-i]; }
        return true;
    }};
    return update::apply_migration(migration, *s.module_plan, s.scalar_resets, source_memories, read_global);
}
inline void publish_module_reset(session& s) noexcept
{
    if(!s.module_plan || !s.module_plan->reset_state) { return; }
    auto& module{*const_cast<::uwvm2::uwvm::runtime::storage::wasm_module_storage_t*>(g_runtime.modules.front_unchecked().runtime_module)};
    for(auto const& memory : s.module_plan->memories)
    { if(memory.byte_size) { ::std::memcpy(module.local_defined_memory_vec_storage.index_unchecked(memory.index).memory.memory_begin, memory.bytes.data(), memory.byte_size); } }
    for(::std::size_t i{}; i < s.module_plan->data_segments.size(); ++i)
    {
        auto& actual{module.local_defined_data_vec_storage.index_unchecked(i).data}; auto const& proposed{s.module_plan->data_segments[i]};
        actual.byte_begin = reinterpret_cast<::std::byte const*>(s.pending_data_image->data() + proposed.payload_offset);
        actual.byte_end = actual.byte_begin + proposed.payload_size; actual.offset = proposed.offset;
    }
    s.current_data_image = s.pending_data_image;
}
#if !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1) && defined(UWVM_IMPORT_WASI_WASIP1) && defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
[[nodiscard]] inline auto* active_policy_binding() noexcept
{
    auto const& file{::uwvm2::uwvm::wasm::storage::active_execute_wasm()};
    return ::uwvm2::uwvm::imported::wasi::wasip1::storage::find_wasip1_module_override(wasip1_module_target_kind_t::main_wasm, file.module_name);
}
[[nodiscard]] inline bool projected_policy_fingerprint(session::policy_plan_type const& plan, update::digest& fingerprint)
{
    auto const* configured{active_policy_binding()}; if(!configured) { return false; }
    ::std::vector<unsigned char> material;
    auto const& file{::uwvm2::uwvm::wasm::storage::active_execute_wasm()};
    update::append_u64(material, file.module_name.size()); for(auto c : file.module_name) { material.push_back(static_cast<unsigned char>(c)); }
    update::append_u64(material, reinterpret_cast<::std::uintptr_t>(configured)); update::append_u64(material, plan.interface_enabled());
    if(!update::serialize_projected(configured->env, plan, [&](unsigned char byte) { material.push_back(byte); })) { return false; }
    return update::hash(material, fingerprint);
}
#endif
[[nodiscard]] inline bool policy_change_preflight(session const& s) noexcept
{
    if(!s.has_policy_change()) { return true; }
#if !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1) && defined(UWVM_IMPORT_WASI_WASIP1) && defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
    auto const* binding{active_policy_binding()}; auto const& config{s.protocol.current_configuration()};
    return binding && update::preflight_wasi_policy(binding->env, *s.policy_plan, config.policy_generation, config.revocation_generation) == update::status::ok;
#else
    return false;
#endif
}
[[nodiscard]] inline bool publish_policy_change(session& s) noexcept
{
    if(!s.has_policy_change()) { return true; }
#if !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1) && defined(UWVM_IMPORT_WASI_WASIP1) && defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
    auto* binding{active_policy_binding()};
    if(!binding || !update::apply_wasi_policy(binding->env, *s.policy_plan)) { return false; }
    binding->enabled = binding->env.interface_enabled; binding->enabled_is_set = true;
    binding->non_fd_permissions = binding->env.non_fd_permissions;
    binding->permission_presence = static_cast<decltype(binding->permission_presence)>(UINT32_MAX);
    binding->fd_limit = binding->env.fd_storage.fd_limit; binding->fd_limit_is_set = true;
    if(!s.protocol.install_contract(s.pending_contract, s.pending_policy_generation, s.pending_revocation_generation)) { return false; }
    s.policy_fingerprint = s.pending_policy_fingerprint; return true;
#else
    return false;
#endif
}
