// Included in initializer::details after the parser memory-type accessors.
#pragma once

namespace reservation = ::uwvm2::object::memory::linear::mmap_reservation;
namespace reservation_config = ::uwvm2::uwvm::wasm::storage;

struct mmap_reservation_plan
{
    ::std::size_t os_page{};
    reservation::bytes_type share{};
    bool defer_owner_mapping{};
    bool reserve_restore_peer{};
    ::uwvm2::uwvm::runtime::storage::runtime_registry_type const* live_predecessor{};
};

// F bytes are free and L bytes belong to the pinned graph that this restore
// will retire. Give the candidate min(F, floor((F + L) / 2)). On first launch
// L is zero. Other pinned graphs are already excluded from F. Subtraction
// avoids summing mapping lengths or overflowing at the inclusive 2^64 limit.
[[nodiscard]] inline constexpr reservation::byte_budget restore_generation_budget(reservation::byte_budget free,
    ::uwvm2::uwvm::runtime::storage::runtime_registry_type const* predecessor) noexcept
{
    auto unmatched{free};
    if(predecessor)
    {
        for(auto const& entry : *predecessor)
        {
            for(auto const& owner : entry.second.local_defined_memory_vec_storage)
            {
                if(!unmatched.take(owner.memory.get_acquire_reserved_space_ceil())) { return free; }
            }
        }
    }
    auto const keep{unmatched.full_address64 ? (reservation::bytes_type{1} << 63u) : unmatched.bytes / 2u + unmatched.bytes % 2u};
    if(!free.take(keep)) { return {}; } // ceil(unmatched / 2) <= unmatched <= free.
    return free;
}

[[nodiscard]] inline constexpr reservation::bytes_type const* local_mmap_data_cap(
    configured_runtime_memory_limits_t const* config, ::std::size_t index) noexcept
{
    if(config)
    {
        auto const it{config->local_defined_mmap_reservations.find(index)};
        if(it != config->local_defined_mmap_reservations.end()) { return ::std::addressof(it->second); }
    }
    return nullptr;
}

[[nodiscard]] inline constexpr reservation::owner_range mmap_owner_range(
    auto const& memory, runtime_memory_limits_t const* override_limits, reservation::bytes_type const* manual,
    mmap_reservation_plan const& plan) noexcept
{
    auto const& declaration{memory.limits};
    auto const shift{::uwvm2::extension::adm::page_shift(memory)};
    auto const page{reservation::bytes_type{1} << shift};
    constexpr auto unlimited{(::std::numeric_limits<reservation::bytes_type>::max)()};
    auto const declared_max{declaration.present_max ? declaration.max : unlimited};
    auto const wide{wasm_memory_is_address64(memory)};
    auto minimum{static_cast<reservation::bytes_type>(declaration.min)};
    auto maximum{static_cast<reservation::bytes_type>(declared_max)};
    if(override_limits)
    {
        minimum = (::std::max)(minimum, static_cast<reservation::bytes_type>(override_limits->min));
        if(override_limits->present_max)
        { maximum = (::std::min)(maximum, static_cast<reservation::bytes_type>(override_limits->max)); }
    }
    bool const tagged{::uwvm2::extension::memtag::is_tagged_memory(memory)};
    auto const k{::uwvm2::extension::adm::ignored_bits(memory)};
    if(k) { maximum = (::std::min)(maximum, ::uwvm2::extension::adm::maximum_pages(wide, k, tagged, shift)); }
    return reservation::bounds(reservation::host_bits, wide, minimum, maximum, page, plan.os_page, manual, tagged, k);
}

template <::uwvm2::parser::wasm::concepts::wasm_feature... Fs>
[[nodiscard]] inline constexpr auto const& local_mmap_declarations(
    ::uwvm2::parser::wasm::binfmt::ver1::wasm_binfmt_ver1_module_extensible_storage_t<Fs...> const& module) noexcept
{
    using section = ::uwvm2::parser::wasm::standard::wasm1::features::memory_section_storage_t<Fs...>;
    return ::uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<section>(module.sections).memories;
}

template <typename Visit>
[[nodiscard]] inline constexpr bool plan_mmap_reservations(Visit&& visit, mmap_reservation_plan& result) noexcept
{
    using namespace reservation_config;
    auto const pool{mmap_reservation_pool_bytes};
    bool const full{mmap_reservation_pool_full_address_space};
    if((full && reservation::host_bits < 64u) || (!full && (!pool || pool > mmap_reservation_hard_limit_bytes)))
    {
        ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output, u8"uwvm: [error] initializer: invalid mmap reservation pool (pool_bytes=",
            pool, u8", hard_limit_bytes=", mmap_reservation_hard_limit_bytes, u8").");
        return false;
    }
    constexpr bool mmap{::uwvm2::object::memory::linear::native_memory_t::can_mmap};
    if constexpr(!mmap)
    {
        if(mmap_reservation_pool_specified || full || pool != default_mmap_reservation_pool_bytes || mmap_reservation_participants)
        {
            ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output, u8"uwvm: [error] initializer: mmap reservation settings require the mmap backend.");
            return false;
        }
    }
    else
    {
        auto const [page, success]{::uwvm2::object::memory::platform_page::get_platform_page_size()};
        if(!success || !page)
        {
            ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output, u8"uwvm: [error] initializer: cannot determine the host mmap page size.");
            return false;
        }
        result.os_page = page;
    }
    // Mappings pinned by a live generation or a private child are unavailable to the next graph.
    auto const state{reservation::available_pool()};
    if(!state.valid)
    {
        ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output, u8"uwvm: [error] initializer: current mmap owners already exceed the configured process pool.");
        return false;
    }
    auto const budget{!mmap_reservation_participants && (result.reserve_restore_peer || result.live_predecessor) ?
        restore_generation_budget(state.remaining, result.live_predecessor) : state.remaining};
    reservation::census census{budget};
    auto const collect = [&](auto const& name, auto const& memories, configured_runtime_memory_limits_t const* config,
                             auto const& owner_pages)
    {
        if(config)
        {
            for(auto const& [index, value]: config->local_defined_mmap_reservations)
            {
                if(!mmap || index >= memories.size())
                {
                    ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                        u8"uwvm: [error] initializer: mmap reservation requires a local-defined mmap memory (module=", name,
                        u8", local_index=", index, u8", local_count=", memories.size(), u8", data_bytes=", value, u8").");
                    return false;
                }
            }
        }
        if constexpr(mmap)
        {
            for(::std::size_t i{}; i != memories.size(); ++i)
            {
                auto const& type{memories.index_unchecked(i)};
                auto const* manual{local_mmap_data_cap(config, i)};
                auto const range{mmap_owner_range(type, owner_pages(i), manual, result)};
                if(!range)
                {
                    auto const reason{[&]() -> ::uwvm2::utils::container::u8string_view
                    {
                        if(manual && reservation::host_bits >= 64u && !wasm_memory_is_address64(type) && !::uwvm2::extension::adm::ignored_bits(type))
                        { return u8"wasm32 has a fixed reservation on this host"; }
                        if(manual && (!*manual || (wasm_memory_is_address64(type) && !::std::has_single_bit(*manual))))
                        { return u8"a manual quota must be positive and memory64 requires a power of two"; }
                        return u8"data and guard must fit the host and memory bounds";
                    }()};
                    ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                        u8"uwvm: [error] initializer: invalid local mmap reservation (module=", name, u8", local_index=", i,
                        u8", address_bits=", wasm_memory_is_address64(type) ? 64u : 32u, u8", manual_data_bytes=", manual ? *manual : 0u,
                        u8", declared_min_pages=", type.limits.min, u8", declared_max_present=", type.limits.present_max,
                        u8", declared_max_pages=", type.limits.max, u8"): ", reason, u8".");
                    return false;
                }
                census.add(range, reservation::host_bits < 64u || wasm_memory_is_address64(type) || ::uwvm2::extension::adm::ignored_bits(type));
            }
        }
        return true;
    };
    if(!visit(collect)) { return false; }
    if constexpr(!mmap) { return true; }
    if(mmap_reservation_participants)
    {
        if(state.participants > mmap_reservation_participants || census.participants > mmap_reservation_participants - state.participants)
        {
            ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output, u8"uwvm: [error] initializer: mmap participant count is below the number of local owners (actual=",
                census.participants, u8", already_live=", state.participants, u8", configured=", mmap_reservation_participants, u8").");
            return false;
        }
        // Hold a share for each future owner even when the current generation has fewer memories.
        auto const future{reservation::bounds(reservation::host_bits, true, 0u, UINT64_MAX, default_linear_memory_page_size_bytes(), result.os_page)};
        auto const vacant{mmap_reservation_participants - state.participants - census.participants};
        if(vacant && !future) { return false; }
        census.add(future, false, vacant);
    }
    if(!census.fits_minimum())
    {
        ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
            u8"uwvm: [error] initializer: mmap reservation pool cannot cover all minimum data and guard windows (pool_bytes=", pool,
            u8", full_address64_pool=", full, u8", available_bytes=", state.remaining.bytes,
            u8", available_full_address64=", state.remaining.full_address64, u8", already_live_participants=", state.participants,
            u8", planning_bytes=", budget.bytes, u8", planning_full_address64=", budget.full_address64,
            u8", participants=", census.participants, u8", default_page_outer_guard_bytes=",
            reservation::outer_guard(default_linear_memory_page_size_bytes(), result.os_page), u8").");
        return false;
    }
    result.share = census.common_share();
    return true;
}

template <initialization_purpose Purpose>
[[nodiscard]] inline constexpr bool plan_world_mmap_reservations(initialization_context<Purpose>& world, mmap_reservation_plan& result) noexcept
{
    using kind = ::uwvm2::uwvm::wasm::type::module_type_t;
    auto const& declarations{world.declarations()};
    for(auto const& [name, config]: world.memory_limit_policy())
    {
        if(config.local_defined_mmap_reservations.empty()) { continue; }
        auto const it{declarations.find(::uwvm2::utils::container::u8string_view{name.data(), name.size()})};
        if(it == declarations.end() || (it->second.type != kind::exec_wasm && it->second.type != kind::preloaded_wasm))
        {
            ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output, u8"uwvm: [error] initializer: unknown local mmap reservation module: ", name);
            return false;
        }
    }
    return plan_mmap_reservations([&](auto const& collect)
    {
        for(auto const& [name, definition]: declarations)
        {
            if(definition.type != kind::exec_wasm && definition.type != kind::preloaded_wasm) { continue; }
            auto const* file{definition.module_storage_ptr.wf};
            if(!file || file->binfmt_ver != 1u) { return false; }
            auto const& memories{local_mmap_declarations(file->wasm_module_storage.wasm_binfmt_ver1_storage)};
            auto const& modules{world.modules()};
            auto const it{modules.find(name)};
            if(it == modules.end() || it->second.local_defined_memory_vec_storage.size() != memories.size()) { return false; }
            if(!collect(name, memories, world.find_memory_limit(name), [&](::std::size_t i) noexcept
                { return ::std::addressof(it->second.local_defined_memory_vec_storage.index_unchecked(i).effective_limits); })) { return false; }
        }
        return true;
    }, result);
}

inline constexpr void configure_mmap_owner(auto& owner, configured_runtime_memory_limits_t const* config,
                                           ::std::size_t index, mmap_reservation_plan const& plan) noexcept
{
    auto& memory{owner.memory};
    if constexpr(::std::remove_cvref_t<decltype(memory)>::can_mmap)
    {
        auto const& type{*owner.memory_type_ptr};
        auto const* manual{local_mmap_data_cap(config, index)};
        auto const range{mmap_owner_range(type, ::std::addressof(owner.effective_limits), manual, plan)};
        if(manual && *manual > range.maximum_data)
        {
            ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                u8"uwvm: [warning] initializer: manual mmap data reservation exceeds the memory maximum and was clipped (local_index=",
                index, u8", requested_data_bytes=", *manual, u8", capped_data_bytes=", range.maximum_data,
                u8", declared_max_pages=", type.limits.max, u8").");
        }
        memory.reservation_budget_bytes = range.span(plan.share);
        memory.use_planned_reservation = true;
        memory.reservation_adm_bits = static_cast<::std::uint_least8_t>(::uwvm2::extension::adm::ignored_bits(type));
        memory.reservation_tagged = ::uwvm2::extension::memtag::is_tagged_memory(type);
        memory.reservation_manual = manual != nullptr;
        memory.reservation_limit_bytes = static_cast<::std::size_t>(range.maximum_data);
        memory.allow_reservation_fallback = !manual || memory.reservation_budget_bytes > range.minimum_span;
        memory.status = wasm_memory_is_address64(type) ? decltype(memory.status)::wasm64 : decltype(memory.status)::wasm32;
    }
}

template <typename Memory>
inline constexpr void report_mmap_owner(Memory const& memory, auto const& declaration, auto name, ::std::size_t index) noexcept
{
    if constexpr(Memory::can_mmap)
    {
        auto const data{memory.reservation_limit_bytes};
        auto const reserved{memory.get_acquire_reserved_space_ceil()};
        if(declaration.present_max && (data >> memory.custom_page_size_log2) < declaration.max)
        {
            ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                u8"uwvm: [warning] initializer: memory reservation limits growth below the declared maximum (module=", name,
                u8", local_index=", index, u8", data_bytes=", data, u8", declared_max_pages=", declaration.max, u8").");
        }
        if(::uwvm2::uwvm::io::show_verbose) [[unlikely]]
        {
            auto const [system_page,known]{::uwvm2::object::memory::platform_page::get_platform_page_size()};
            verbose_info(u8"initializer: mmap reservation (module=", name, u8", local_index=", index,
                u8", address_bits=", memory.status == decltype(memory.status)::wasm64 ? 64u : 32u,
                u8", ignored_bits=", static_cast<unsigned>(memory.reservation_adm_bits),
                u8", protection=", memory.is_full_page_protection() ? ::uwvm2::utils::container::u8string_view{u8"full"} : ::uwvm2::utils::container::u8string_view{u8"partial"},
                u8", allocation=", memory.reservation_manual ? ::uwvm2::utils::container::u8string_view{u8"manual"} : ::uwvm2::utils::container::u8string_view{u8"automatic"},
                u8", aliases=", memory.reservation_alias_count, u8", wasm_page_bytes=", ::std::size_t{1} << memory.custom_page_size_log2,
                u8", system_page_bytes=", known ? system_page : 0u,
                u8", declared_min_pages=", declaration.min, u8", declared_max_present=", declaration.present_max,
                u8", declared_max_pages=", declaration.max, u8", data_bytes=", data, u8", guard_bytes=", reserved - data,
                u8", reserved_bytes=", reserved, u8"). ");
        }
    }
}
