// Included inside initializer::details, after native alias-resolution helpers.
#pragma once
namespace mt_init
{
    namespace mt=::uwvm2::extension::memtag;
    using owner=::uwvm2::uwvm::runtime::storage::local_defined_memory_storage_t;
    struct memory_plan { owner* memory{}; mt::address_width width{}; bool shared{}; };
    struct global_plan { owner* memory{}; ::std::uint64_t address{},bytes{}; ::std::uint8_t tag{}; };
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
}

// Configure the actual resolved owners before any guest body is compiled.
// Imported aliases therefore share one shadow; unused ordinary memories keep
// their full address domain and never allocate shadow storage.
template<initialization_purpose Purpose>
[[nodiscard]] inline bool prepare_memtag_after_linking_in_context(initialization_context<Purpose>& world) noexcept
{
    namespace mt=::uwvm2::extension::memtag;
    namespace rt_mt=::uwvm2::runtime::memtag;
    ::std::vector<mt_init::memory_plan> memories{};
    ::std::vector<mt_init::global_plan> globals{};
    bool const execute{!::uwvm2::uwvm::runtime::runtime_mode::runtime_disable_memtag};
    auto const delivery{mt::selected_fault_delivery.load(::std::memory_order_relaxed)};
    if(!execute && (mt::is_asynchronous(delivery) || mt::is_hardware(delivery)))
    { return mt_init::error(u8"<launch>",u8"asynchronous or hardware MTE cannot be combined with --runtime-disable-memtag"); }
    if(mt::is_hardware(delivery) && !mt::hardware_mte::supported())
    { return mt_init::error(u8"<launch>",u8"hardware MTE requires OS-enabled checked mappings; no silent software fallback on unsupported platforms"); }
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
        auto const count{runtime.imported_memory_vec_storage.size()+runtime.local_defined_memory_vec_storage.size()};
        ::std::vector<mt::memory_usage> usage(count);
        mt::usage_sink sink{.memories=usage};
        if(!::uwvm2::uwvm::runtime::validator::validate_all_wasm_code_for_module(
            module,features,file->file_name,name,::std::addressof(sink)) || !sink.valid) { return false; }

        using custom_t=::uwvm2::parser::wasm::standard::wasm1::features::custom_section_storage_t;
        auto const& custom{::uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<custom_t>(module.sections)};
        bool seen_globals{};
        for(auto const& section:custom.customs)
        {
            if(section.custom_name==mt::obsolete_globals_section_name)
            { return mt_init::error(name,u8"obsolete memtag.globals section; regenerate with uwvm.memtag.linear.globals version 2"); }
            if(section.custom_name!=mt::linear_globals_section_name) { continue; }
            if(policy.disable_memtag_extension)
            { return mt_init::error(name,u8"memory-tag globals require the nonstandard extension; remove --wasm-feature-disable-memtag-extension"); }
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
                if(memory==nullptr || memory->memory_type_ptr==nullptr) { return false; }
                return ::uwvm2::object::memory::linear::with_memory_access_snapshot(
                    memory->memory,[&](::std::byte*,::std::size_t length) noexcept {bytes=length;return true;});
            }};
            auto plan_global{[&](::std::uint32_t index,::std::uint64_t address,::std::uint64_t bytes)
            {
                // The complete immutable section was validated against these
                // same resolved owners before any visitor can reach this point.
                auto* memory{mt_init::resolve(world,runtime,index)};
                auto const width{wasm_memory_is_address64(*memory->memory_type_ptr)?mt::address_width::i64:mt::address_width::i32};
                usage[index].used=true;usage[index].width=width;
                if(bytes)
                {
                    globals.push_back({memory,address,bytes,0u});
                }
            }};
            auto const checked{mt::visit_linear_globals(payload,memory_bytes,[&](mt::linear_global_descriptor descriptor)
            { plan_global(descriptor.memory_index,descriptor.address,descriptor.byte_length); })};
            if(!checked) { return mt_init::error(name,u8"invalid version, encoding, memory index or range in uwvm.memtag.linear.globals"); }

        }
        for(::std::size_t index{};index!=usage.size();++index)
        {
            auto const& item{usage[index]};
            if(!item.used) { continue; }
            if(item.has_undefined_placeholder)
            { return mt_init::error(name,u8"memtag.add is an author-reserved placeholder without published execution semantics"); }
            if(!execute) { continue; }
            auto* memory{mt_init::resolve(world,runtime,index)};
            if(memory==nullptr || memory->memory_type_ptr==nullptr)
            { return mt_init::error(name,u8"tagged accesses require an owned Wasm memory, including imported aliases"); }
            auto const width{wasm_memory_is_address64(*memory->memory_type_ptr)?mt::address_width::i64:mt::address_width::i32};
            auto const shared{wasm_memory_is_shared(*memory->memory_type_ptr)};
            if(width!=item.width) { return mt_init::error(name,u8"memory address width differs from its validated use"); }
            bool existing{};
            for(auto const& planned:memories)
            {
                if(planned.memory!=memory) { continue; }
                if(planned.width!=width || planned.shared!=shared)
                { return mt_init::error(name,u8"imported aliases disagree about memory geometry"); }
                existing=true;break;
            }
            if(!existing) { memories.push_back({memory,width,shared}); }
        }
    }
    if(!globals.empty())
    {
        ::std::vector<mt::global_tag_range> global_ranges{};
        global_ranges.reserve(globals.size());
        for(auto const& global:globals)
        {
            // Byte bounds were proved above, and only nonempty descriptors
            // enter the plan. Compare granules after canonical alias routing.
            global_ranges.push_back({reinterpret_cast<::std::uintptr_t>(global.memory),
                global.address >> mt::granule_size_log2,
                (global.address+global.bytes-1u) >> mt::granule_size_log2});
        }
        if(!mt::validate_global_tag_ranges(global_ranges))
        { return mt_init::error({},u8"linear globals descriptors overlap an allocation-tag granule on the same memory owner"); }
    }
    // Disabled execution still validates declarations and cross-owner plans,
    // but never allocates shadow storage, consumes entropy or mutates tags.
    if(!execute) { return true; }
    // All source declarations and globals descriptors have been checked before
    // the first owner changes. Failure still leaves the registry unpublished.
    for(auto const& plan:memories)
    {
        auto& memory{plan.memory->memory};
        if(mt::is_hardware(delivery) && !mt::logging_requested() && !::std::remove_reference_t<decltype(memory)>::can_mmap)
        { return mt_init::error({},u8"hardware MTE requires a reserved mmap owner; allocator-backed memories cannot silently use software delivery"); }
        if(memory.memtag_shadow.enabled())
        {
            if(memory.memtag_shadow.width()!=plan.width || memory.memtag_shadow.shared()!=plan.shared)
            { return mt_init::error({},u8"existing memory tag geometry differs"); }
            if(mt::is_hardware(delivery) && !mt::logging_requested() && !memory.memtag_shadow.hardware_selected())
            { return mt_init::error({},u8"hardware MTE requires every used owner to select checked hardware mappings"); }
            continue;
        }
        auto const result{memory.enable_memtag(plan.width,plan.shared)};
        if(result!=mt::storage_error::none)
        { return mt_init::error({},u8"cannot allocate tag storage or represent the memory in the extension address domain"); }
        if(mt::is_hardware(delivery) && !mt::logging_requested() && !memory.memtag_shadow.hardware_selected())
        { return mt_init::error({},u8"hardware MTE owner selection failed"); }
    }
    // Stage entropy before mutating any allocation tag; failures never leave a
    // partially tagged global set available to an executing guest.
    for(auto& global:globals)
    {
        ::std::uint8_t random{};
        if(!rt_mt::next_random_tag(random)) { return mt_init::error({},u8"host entropy unavailable for global allocation tags"); }
        auto const mask{global.memory->memory.memtag_shadow.width()==mt::address_width::i32?15u:255u};
        global.tag=static_cast<::std::uint8_t>(random&mask);
        if(!global.tag) { global.tag=static_cast<::std::uint8_t>(mask); }
    }
    for(auto const& global:globals)
    {
        auto const& memory{global.memory->memory};
        auto apply{[&]<typename Address,bool Shared>() noexcept
        {
            using view=mt::shadow_view<Address,Shared>;
            ::std::array<Address,2u> operands{
                view::insert(static_cast<Address>(global.address),global.tag),static_cast<Address>(global.bytes)};
            return static_cast<bool>(rt_mt::execute<true,Address,Shared>(memory,mt::opcode::store,operands));
        }};
        bool okay{};
        if(memory.memtag_shadow.width()==mt::address_width::i32)
        { okay=memory.memtag_shadow.shared()?apply.template operator()<::std::uint32_t,true>():apply.template operator()<::std::uint32_t,false>(); }
        else
        { okay=memory.memtag_shadow.shared()?apply.template operator()<::std::uint64_t,true>():apply.template operator()<::std::uint64_t,false>(); }
        if(!okay) { return mt_init::error({},u8"global tag initialization failed"); }
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
            if(!result && (result.error!=hw::setup_error::unsupported || memory.memtag_shadow.hardware_selected() ||
                ::uwvm2::extension::memtag::hardware_checked_access_requested()))
            { return mt_init::error(name,u8"cannot activate the selected hardware MTE mode for an owned memory"); }
        }
    }
    return true;
}
