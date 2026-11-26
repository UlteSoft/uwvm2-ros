// Included inside initializer::details, after native alias-resolution helpers.
#pragma once
#include <uwvm2/utils/container/standard.h>
namespace mt_init
{
    namespace mt=::uwvm2::extension::memtag;
    using owner=::uwvm2::uwvm::runtime::storage::local_defined_memory_storage_t;
    struct global_plan { owner* memory{}; ::std::uint64_t address{},bytes{}; ::std::uint8_t tag{}; };
    struct linked_plan
    {
        ::uwvm2::utils::container::initializer_std_vector<global_plan> globals{};
        ::uwvm2::utils::container::initializer_std_vector<mt::global_tag_range> global_ranges{};
    };
    inline bool error(::uwvm2::utils::container::u8string_view module,char8_t const* message) noexcept
    {
        ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
            u8"uwvm: [error] WasmMTE admission failed (module=\"",module,u8"\"): ",::fast_io::mnp::os_c_str(message));
        return false;
    }
    template<initialization_purpose Purpose>
    [[nodiscard]] inline owner* resolve(initialization_context<Purpose>& world,
        ::uwvm2::uwvm::runtime::storage::wasm_module_storage_t& module,::std::size_t index) noexcept
    {
        auto const imports{module.imported_memory_vec_storage.size()};
        if(index<imports)
        {
            owner* resolved{};
            if(!maybe_resolve_wasm1_imported_memory_defined_in_context(
                world,::std::addressof(module.imported_memory_vec_storage.index_unchecked(index)),resolved))
            { return nullptr; }
            return resolved;
        }
        index-=imports;
        if(index>=module.local_defined_memory_vec_storage.size()) { return nullptr; }
        return ::std::addressof(module.local_defined_memory_vec_storage.index_unchecked(index));
    }

    inline bool allow_unchecked_caller(owner& memory, ::uwvm2::utils::container::u8string_view caller) noexcept
    {
        using backend = mt::owner_backend;
        if(memory.backend == backend::arm_mte || memory.backend == backend::x86_mtt)
        { return error(caller,u8"an unchecked caller reaches an owner requiring hardware tag checks"); }
        memory.backend = backend::software;
        return true;
    }
}

// The type of each local definition selects its shadow. Imported globals
// descriptors are bound to that same definition after type-checked linking.
template<initialization_purpose Purpose>
[[nodiscard]] inline bool plan_linked_memtag(initialization_context<Purpose>& world, mt_init::linked_plan& linked) noexcept
#ifdef UWVM_CPP_EXCEPTIONS
try
#endif
{
    namespace mt=::uwvm2::extension::memtag;
    auto& globals{linked.globals};
    using name_view = ::uwvm2::utils::container::u8string_view;
    struct owner_name { name_view name; ::std::size_t index; };
    ::uwvm2::utils::container::initializer_map<mt_init::owner const*, owner_name> owner_names{};
    if(::uwvm2::uwvm::io::show_verbose)
    {
        for(auto& [name,module] : world.modules())
        {
            ::std::size_t i{};
            for(auto& owner : module.local_defined_memory_vec_storage)
            {
                owner.memory.reservation_alias_count = 0u;
                owner_names.try_emplace(::std::addressof(owner), owner_name{name,i++});
            }
        }
    }
    for(auto const& [name,declaration]:world.declarations())
    {
        using kind=::uwvm2::uwvm::wasm::type::module_type_t;
        if(declaration.type!=kind::exec_wasm && declaration.type!=kind::preloaded_wasm) { continue; }
        auto const* file{declaration.module_storage_ptr.wf};
        if(file==nullptr || file->binfmt_ver!=1u) { return mt_init::error(name,u8"invalid immutable module owner"); }
        auto const found{world.modules().find(name)};
        if(found==world.modules().end()) { return mt_init::error(name,u8"runtime module is missing"); }
        auto& runtime{found->second};
        auto const& module{file->wasm_module_storage.wasm_binfmt_ver1_storage};
        auto const& features{file->wasm_parameter.binfmt1_para};
        auto const& policy{::uwvm2::parser::wasm::standard::wasm1p1::features::get_wasm1p1_parameter(features)};
        auto const imports{runtime.imported_memory_vec_storage.size()}, locals{runtime.local_defined_memory_vec_storage.size()};
        if(locals > SIZE_MAX - imports) { return mt_init::error(name,u8"memory owner count exceeds the native range"); }
        auto const count{imports + locals};
        for(::std::size_t i{}; i != count; ++i)
        {
            auto const& type{::uwvm2::uwvm::runtime::storage::runtime_memory_type(runtime,i)};
            if(!::uwvm2::extension::adm::ignored_bits(type) && !(i < imports && ::uwvm2::uwvm::io::show_verbose)) { continue; }
            auto* owner{mt_init::resolve(world,runtime,i)};
            if(!owner)
            {
                if(::uwvm2::extension::adm::ignored_bits(type)) { return mt_init::error(name,u8"WasmADM import has no matching Wasm memory owner"); }
                continue; // Host adapters can supply an ordinary memory.
            }
            if(mt::is_tagged_memory(type) && !runtime.memory_policy.check_tags && !mt_init::allow_unchecked_caller(*owner,name)) { return false; }
            if(i < imports && ::uwvm2::uwvm::io::show_verbose)
            {
                auto const found{owner_names.find(owner)};
                if(found == owner_names.end()) { return mt_init::error(name,u8"linked memory owner is missing"); }
                ++owner->memory.reservation_alias_count;
                verbose_info(u8"initializer: mmap alias (module=",name,u8", memory_index=",i,
                    u8", owner_module=",found->second.name,u8", owner_local_index=",found->second.index,u8"). ");
            }
        }
        mt::usage_sink sink{.classify_only=true};
        if(!::uwvm2::uwvm::runtime::validator::validate_all_wasm_code_for_module(
            module,features,file->file_name,name,::std::addressof(sink)) || !sink.valid) { return false; }
        if(sink.uses_undefined_placeholder)
        { return mt_init::error(name,u8"memtag.add is an author-reserved placeholder without published execution semantics"); }

        using custom_t=::uwvm2::parser::wasm::standard::wasm1::features::custom_section_storage_t;
        auto const& custom{::uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<custom_t>(module.sections)};
        bool seen_globals{};
        for(auto const& section:custom.customs)
        {
            if(section.custom_name==mt::obsolete_globals_section_name)
            { return mt_init::error(name,u8"obsolete memtag.globals section; regenerate with uwvm.memtag.linear.globals version 2"); }
            if(section.custom_name!=mt::linear_globals_section_name) { continue; }
            if(policy.disable_memtag_extension || policy.disable_adm_extension)
            { return mt_init::error(name,u8"memory-tag globals require WasmADM and WasmMTE"); }
            if(seen_globals) { return mt_init::error(name,u8"duplicate memory-tag globals sections"); }
            seen_globals=true;
            auto const begin{reinterpret_cast<::std::uintptr_t>(section.custom_begin)};
            auto const end{reinterpret_cast<::std::uintptr_t>(section.sec_span.sec_end)};
            auto const module_begin{reinterpret_cast<::std::uintptr_t>(module.module_span.module_begin)};
            auto const module_end{reinterpret_cast<::std::uintptr_t>(module.module_span.module_end)};
            if(begin<module_begin || end<begin || end>module_end || end-begin>SIZE_MAX)
            { return mt_init::error(name,u8"globals section is outside its immutable source"); }
            ::std::span<::std::byte const> payload{reinterpret_cast<::std::byte const*>(begin),static_cast<::std::size_t>(end-begin)};
            auto memory_bytes{[&](::std::uint32_t index,::std::uint64_t& bytes) noexcept
            {
                if(index>=count) { return false; }
                auto* memory{mt_init::resolve(world,runtime,index)};
                if(memory==nullptr || memory->memory_type_ptr==nullptr || !mt::is_tagged_memory(*memory->memory_type_ptr)) { return false; }
                auto const shift{memory->memory.custom_page_size_log2};
                auto const pages{memory->effective_limits.min};
                if(shift >= 64u || pages > (UINT64_MAX >> shift)) { return false; }
                bytes = static_cast<::std::uint64_t>(pages) << shift;
                return true;
            }};
            auto plan_global{[&](::std::uint32_t index,::std::uint64_t address,::std::uint64_t bytes)
            {
                // The complete immutable section was validated against these
                // same resolved owners before any visitor can reach this point.
                auto* memory{mt_init::resolve(world,runtime,index)};
                if(bytes)
                {
                    globals.push_back({memory,address,bytes,0u});
                }
            }};
            auto const checked{mt::visit_linear_globals(payload,memory_bytes,[&](mt::linear_global_descriptor descriptor)
            { plan_global(descriptor.memory_index,descriptor.address,descriptor.byte_length); })};
            if(!checked) { return mt_init::error(name,u8"invalid version, encoding, WasmMTE memory index or range in uwvm.memtag.linear.globals"); }

        }
    }
    auto& global_ranges{linked.global_ranges};
    if(!globals.empty())
    {
        global_ranges.reserve(globals.size());
        for(::std::size_t i{};i<globals.size();++i)
        {
            auto const& global{globals[i]};
            // Byte bounds were proved above, and only nonempty descriptors
            // enter the plan. Compare granules after canonical alias routing.
            global_ranges.push_back({reinterpret_cast<::std::uintptr_t>(global.memory),
                global.address >> mt::granule_size_log2,
                (global.address+global.bytes-1u) >> mt::granule_size_log2, i});
        }
        if(!mt::validate_global_tag_ranges(global_ranges))
        { return mt_init::error({},u8"linear globals descriptors overlap an allocation-tag granule on the same memory owner"); }
    }
    return true;
}

#ifdef UWVM_CPP_EXCEPTIONS
catch(::std::bad_alloc const&) { return mt_init::error({},u8"linked memory metadata exceeds the initializer byte budget"); }
#endif

[[nodiscard]] inline bool configure_owner_addressing(mt_init::owner& record, ::uwvm2::utils::container::u8string_view module_name,
    bool caller_checks, ::std::optional<unsigned> saved_tag_start) noexcept
{
    namespace mt = ::uwvm2::extension::memtag;
    auto& memory{record.memory};
    auto const& type{*record.memory_type_ptr};
    auto const width{wasm_memory_is_address64(type) ? mt::address_width::i64 : mt::address_width::i32};
    auto const shared{wasm_memory_is_shared(type)};
    auto const k{::uwvm2::extension::adm::ignored_bits(type)};
    if(mt::is_tagged_memory(type) && !caller_checks && !mt_init::allow_unchecked_caller(record,module_name)) { return false; }
    auto const result{mt::is_tagged_memory(type) ? memory.enable_memtag(width, shared, k, record.backend, saved_tag_start) :
        memory.memtag_shadow.configure_adm(width, shared, k)};
    if(result != mt::storage_error::none)
    { return mt_init::error(module_name,u8"cannot initialize the declared memory address domain or selected WasmMTE backend"); }
    return true;
}

[[nodiscard]] inline bool prepare_memtag_after_linking_in_context(initialization_context<initialization_purpose::ordinary>& world,
    mmap_reservation_plan& reservations) noexcept
{
    namespace mt=::uwvm2::extension::memtag;
    namespace rt_mt=::uwvm2::runtime::memtag;
    mt_init::linked_plan linked{};
    if(!plan_linked_memtag(world, linked) || !plan_world_mmap_reservations(world, reservations)) { return false; }
    for(auto& [name, module] : world.modules())
    {
        auto const* config{world.find_memory_limit(name)};
        auto const imports{module.imported_memory_vec_storage.size()};
        ::std::size_t index{};
        for(auto& record : module.local_defined_memory_vec_storage)
        {
            auto const& declaration{*record.memory_type_ptr};
            configure_mmap_owner(record, config, index, reservations);
            if(!initialize_native_memory<true>(record.memory, record.effective_limits, imports + index,
                                               wasm_memory_is_shared(declaration), wasm_memory_is_address64(declaration)) ||
               !configure_owner_addressing(record, name))
            { return false; }
            report_mmap_owner(record.memory, declaration.limits, name, index);
            ++index;
        }
    }
    auto& globals{linked.globals};
    auto const& global_ranges{linked.global_ranges};
    // The entire graph is still unpublished. Canonical owners hold reservations
    // across modules, including when two descriptors reach the same owner by import.
    ::std::size_t staged{}, published{};
    auto rollback = [&]() noexcept
    {
        while(published)
        {
            auto const& g{globals[global_ranges[--published].plan_index]};
            (void)rt_mt::write_static_allocation(g.memory->memory.memtag_shadow,g.address,g.bytes,0);
        }
        while(staged)
        {
            auto const& g{globals[global_ranges[--staged].plan_index]};
            (void)g.memory->memory.memtag_shadow.allocation_directory().remove_permanent(
                g.address/mt::granule_size,(g.address+g.bytes-1u)/mt::granule_size,g.tag);
        }
    };
    rt_mt::static_color_usage<> static_colors{};
    mt_init::owner const* counted_owner{};
    for(auto const& range:global_ranges)
    {
        auto& g{globals[range.plan_index]};
        auto& shadow{g.memory->memory.memtag_shadow};
        if(counted_owner!=g.memory)
        {
            counted_owner=g.memory;
            static_colors={shadow.width(),shadow.hardware_selected()};
            shadow.allocation_directory().visit(0u,UINT64_MAX,[&](auto const& item) noexcept
            {
                if(item.permanent) { static_colors.record(item.tag); }
            });
        }
        if(!rt_mt::stage_static_allocation(shadow,g.address,g.bytes,static_colors,g.tag))
        { rollback(); return mt_init::error({},u8"cannot reserve a disjoint neighboring color for a linear global"); }
        ++staged;
    }
    for(auto const& range:global_ranges)
    {
        auto const& g{globals[range.plan_index]};
        if(!rt_mt::write_static_allocation(g.memory->memory.memtag_shadow,g.address,g.bytes,g.tag))
        { rollback(); return mt_init::error({},u8"global tag publication failed"); }
        ++published;
    }
    return true;
}

// Native hardware mode is selected once before code generation/publication.
// Unsupported hosts keep the complete software checks; setup failure never
// publishes an owner whose physical pointer convention is only partly active.
template<initialization_purpose Purpose>
[[nodiscard]] inline bool activate_memtag_hardware_in_context(initialization_context<Purpose>& world) noexcept
{
    namespace hw=::uwvm2::extension::memtag::hardware_mte;
    for(auto& [name,module]:world.modules())
    {
        for(auto& record:module.local_defined_memory_vec_storage)
        {
            auto& memory{record.memory};
            if(!memory.memtag_shadow.enabled()) { continue; }
            auto const result{::uwvm2::runtime::memtag::activate_owner_hardware(memory)};
            if(!result && (result.error!=hw::setup_error::unsupported || memory.memtag_shadow.hardware_selected()))
            { return mt_init::error(name,u8"cannot activate the selected hardware MTE mode for an owned memory"); }
        }
    }
    return true;
}
