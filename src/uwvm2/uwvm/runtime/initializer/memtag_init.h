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
            if(section.custom_name!=u8"memtag.globals") { continue; }
            if(policy.disable_memtag_extension)
            { return mt_init::error(name,u8"memtag.globals requires the nonstandard extension; remove --wasm-feature-disable-memtag-extension"); }
            if(seen_globals || count==0u) { return mt_init::error(name,u8"duplicate globals section or absent memory zero"); }
            seen_globals=true;
            auto* memory{mt_init::resolve(world,runtime,0u)};
            if(memory==nullptr || memory->memory_type_ptr==nullptr)
            { return mt_init::error(name,u8"memtag.globals requires an owned Wasm memory"); }
            auto const width{wasm_memory_is_address64(*memory->memory_type_ptr)?mt::address_width::i64:mt::address_width::i32};
            usage[0].used=true;usage[0].width=width;
            auto const begin{reinterpret_cast<::std::uintptr_t>(section.custom_begin)};
            auto const end{reinterpret_cast<::std::uintptr_t>(section.sec_span.sec_end)};
            auto const module_begin{reinterpret_cast<::std::uintptr_t>(module.module_span.module_begin)};
            auto const module_end{reinterpret_cast<::std::uintptr_t>(module.module_span.module_end)};
            if(begin<module_begin || end<begin || end>module_end || end-begin>SIZE_MAX)
            { return mt_init::error(name,u8"globals section is outside its immutable source"); }
            ::std::span<::std::byte const> payload{reinterpret_cast<::std::byte const*>(begin),static_cast<::std::size_t>(end-begin)};
            ::std::uint64_t bytes{};
            bool const snapshot{::uwvm2::object::memory::linear::with_memory_access_snapshot(
                memory->memory,[&](::std::byte*,::std::size_t length) noexcept {bytes=length;return true;})};
            if(!snapshot) { return mt_init::error(name,u8"memory snapshot unavailable"); }
            auto const checked{mt::visit_globals(payload,bytes,[&](mt::global_descriptor descriptor)
            { if(execute && descriptor.byte_length) {globals.push_back({memory,descriptor.address,descriptor.byte_length,0u});} })};
            if(!checked) { return mt_init::error(name,u8"invalid version, encoding or memory range in memtag.globals"); }
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
    // All source declarations and globals descriptors have been checked before
    // the first owner changes. Failure still leaves the registry unpublished.
    for(auto const& plan:memories)
    {
        auto& memory{plan.memory->memory};
        if(memory.memtag_shadow.enabled())
        {
            if(memory.memtag_shadow.width()!=plan.width || memory.memtag_shadow.shared()!=plan.shared)
            { return mt_init::error({},u8"existing memory tag geometry differs"); }
            continue;
        }
        auto const result{memory.enable_memtag(plan.width,plan.shared)};
        if(result!=mt::storage_error::none)
        { return mt_init::error({},u8"cannot allocate tag storage or represent the memory in the extension address domain"); }
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
            if(!result && (result.error!=hw::setup_error::unsupported || memory.memtag_shadow.hardware_selected()))
            { return mt_init::error(name,u8"cannot activate synchronous hardware MTE for an owned memory"); }
        }
    }
    return true;
}
