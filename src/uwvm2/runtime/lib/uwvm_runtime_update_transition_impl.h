// Private production_update_detail fragment: held native ownership and actual
// policy/state publication helpers. Never imported into guest address space.
#pragma once
// Only called after the signed coordinator has drained every old execution
// lease. Staging may fail; publication later has no allocation or entropy step.
[[nodiscard]] inline bool prepare_memory_tags(session& s)
{
    namespace mt=::uwvm2::extension::memtag;
    if(!s.module_plan || !s.module_plan->reset_state) { return true; }
    auto& plan{*s.module_plan};
    if(g_runtime.modules.size()!=1u || !g_runtime.modules.front_unchecked().runtime_module) { return false; }
    auto const& module{*g_runtime.modules.front_unchecked().runtime_module};
    if(plan.memories.size()!=module.local_defined_memory_vec_storage.size()) { return false; }
    for(auto& memory:plan.memories)
    {
        auto const& actual{module.local_defined_memory_vec_storage.index_unchecked(memory.index).memory};
        memory.tagged=actual.memtag_shadow.enabled();
        if(!memory.tagged) { continue; }
        if(actual.memtag_shadow.width()!=mt::address_width::i32 || actual.memtag_shadow.shared() ||
           memory.byte_size>actual.memtag_shadow.prepared_memory_bytes() || memory.byte_size%16u!=0u) { return false; }
        memory.allocation_tags.assign(memory.byte_size/16u,0u);
    }
    if(::uwvm2::uwvm::runtime::runtime_mode::runtime_disable_memtag) { return true; }
    // Module transition comparison requires identical custom sections. Read the
    // already validated original descriptors, but prepare fresh allocation tags
    // exactly as initial launch does; tagged pointers in active data stay bytes.
    auto const& file{::uwvm2::uwvm::wasm::storage::active_execute_wasm()};
    using custom_type=::uwvm2::parser::wasm::standard::wasm1::features::custom_section_storage_t;
    auto const& custom{::uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<custom_type>(
        file.wasm_module_storage.wasm_binfmt_ver1_storage.sections)};
    for(auto const& section:custom.customs)
    {
        if(section.custom_name!=mt::linear_globals_section_name) { continue; }
        auto const* begin{reinterpret_cast<::std::byte const*>(section.custom_begin)};
        auto const* end{reinterpret_cast<::std::byte const*>(section.sec_span.sec_end)};
        bool valid{true};
        auto checked{mt::visit_linear_globals({begin,static_cast<::std::size_t>(end-begin)},
            [&](::std::uint32_t index,::std::uint64_t& bytes) noexcept
            { if(index>=plan.memories.size()) { return false; } bytes=plan.memories[index].byte_size;return true; },
            [&](mt::linear_global_descriptor descriptor)
            {
                if(!valid || descriptor.byte_length==0u) { return; }
                auto& memory{plan.memories[descriptor.memory_index]};
                ::std::uint8_t random{};
                if(!memory.tagged || !::uwvm2::runtime::memtag::next_random_tag(random)) { valid=false;return; }
                auto const tag{static_cast<unsigned char>((random&15u)!=0u ? random&15u : 15u)};
                auto const first{static_cast<::std::size_t>(descriptor.address/16u)};
                auto const last{static_cast<::std::size_t>((descriptor.address+descriptor.byte_length-1u)/16u+1u)};
                for(auto i{first};i<last;++i) { memory.allocation_tags[i]=tag; }
            })};
        if(!checked || !valid) { return false; }
    }
    return true;
}
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
        if(plan.reset_state)
        {
            auto const& tags{actual.memtag_shadow};
            if(memory.tagged!=tags.enabled() || (!memory.tagged && !memory.allocation_tags.empty())) { return false; }
            if(memory.tagged)
            {
                if(tags.width()!=::uwvm2::extension::memtag::address_width::i32 || tags.shared() ||
                   memory.byte_size>tags.prepared_memory_bytes() || memory.allocation_tags.size()!=memory.byte_size/16u) { return false; }
                for(auto tag:memory.allocation_tags) { if(tag>15u) { return false; } }
                if(tags.hardware_active() && (tags.hardware_base()!=actual.memory_begin ||
                   !::uwvm2::extension::memtag::hardware_mte::enable_current_thread_sync())) { return false; }
            }
        }
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
    if(auto checked{update::validate_migration(migration,*s.module_plan,s.scalar_resets)};checked!=update::status::ok) { return checked; }
    // A byte-only partial granule cannot transfer ownership of its neighbours.
    // Admit tagged migrations only for whole granules between tagged owners.
    for(auto const& row:migration.memories)
    {
        auto const& source{s.module_plan->memories[row.source_memory]};
        auto const& destination{s.module_plan->memories[row.destination_memory]};
        if(source.tagged!=destination.tagged || ((source.tagged || destination.tagged) &&
           ((row.source_offset|row.destination_offset|row.length)&15u)!=0u)) { return update::status::unsupported; }
    }
    ::std::vector<::std::vector<unsigned char>> copies(s.module_plan->memories.size());
    ::std::vector<::std::span<unsigned char const>> source_memories; source_memories.reserve(copies.size());
    for(auto const& memory : s.module_plan->memories)
    {
        auto const& actual{module.local_defined_memory_vec_storage.index_unchecked(memory.index).memory};
        if(actual.memtag_shadow.hardware_active())
        {
            auto& copied{copies[memory.index]};copied.resize(memory.byte_size);
            if(!::uwvm2::extension::memtag::copy_debug_memory(actual.memtag_shadow,
                {actual.memory_begin,memory.byte_size},0u,copied.data(),copied.size())) { return update::status::invalid_state; }
            source_memories.emplace_back(copied.data(),copied.size());
        }
        else { source_memories.emplace_back(reinterpret_cast<unsigned char const*>(actual.memory_begin),memory.byte_size); }
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
    auto result{update::apply_migration(migration, *s.module_plan, s.scalar_resets, source_memories, read_global)};
    if(result!=update::status::ok) { return result; }
    for(auto const& row:migration.memories)
    {
        auto const& source{module.local_defined_memory_vec_storage.index_unchecked(row.source_memory).memory};
        auto& destination{s.module_plan->memories[row.destination_memory]};
        if(!destination.tagged) { continue; }
        auto const copied{::uwvm2::extension::memtag::copy_debug_tags(source.memtag_shadow,
            s.module_plan->memories[row.source_memory].byte_size,row.source_offset,row.length,
            {destination.allocation_tags.data()+static_cast<::std::size_t>(row.destination_offset/16u),static_cast<::std::size_t>(row.length/16u)})};
        if(copied.status!=::uwvm2::extension::memtag::debug_tag_status::available) { return update::status::invalid_state; }
    }
    return update::status::ok;
}
[[nodiscard]] inline bool publish_module_reset(session& s) noexcept
{
    if(!s.module_plan || !s.module_plan->reset_state) { return true; }
    auto& module{*const_cast<::uwvm2::uwvm::runtime::storage::wasm_module_storage_t*>(g_runtime.modules.front_unchecked().runtime_module)};
    for(auto const& memory : s.module_plan->memories)
    {
        auto& actual{module.local_defined_memory_vec_storage.index_unchecked(memory.index).memory};
        // Use the old allocation tags for the copy, then publish the prepared
        // software AND hardware tags under the same drained publication fence.
        if(!::uwvm2::extension::memtag::copy_debug_memory(actual.memtag_shadow,
            {actual.memory_begin,memory.byte_size},0u,const_cast<unsigned char*>(memory.bytes.data()),memory.byte_size,true)) { return false; }
        if(!memory.tagged) { continue; }
        auto const view{actual.memtag_shadow.template view<::std::uint32_t,false>(memory.byte_size)};
        for(::std::size_t first{};first<memory.allocation_tags.size();)
        {
            auto last{first+1u};while(last<memory.allocation_tags.size() && memory.allocation_tags[last]==memory.allocation_tags[first]) { ++last; }
            if(!view.store(view.insert(static_cast<::std::uint32_t>(first*16u),memory.allocation_tags[first]),(last-first)*16u)) { return false; }
            first=last;
        }
    }
    for(::std::size_t i{}; i < s.module_plan->data_segments.size(); ++i)
    {
        auto& actual{module.local_defined_data_vec_storage.index_unchecked(i).data}; auto const& proposed{s.module_plan->data_segments[i]};
        actual.byte_begin = reinterpret_cast<::std::byte const*>(s.pending_data_image->data() + proposed.payload_offset);
        actual.byte_end = actual.byte_begin + proposed.payload_size; actual.offset = proposed.offset;
    }
    s.current_data_image = s.pending_data_image;
    return true;
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
    append_execution_safety_policy(material);
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
