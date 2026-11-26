// Private fragment of the signed update manager. The ordinary runner executes
// core Start functions. The manager can consume this one declarative subset
// while its real owner is cold and public execution is fenced.
#pragma once
namespace cold_relocations
{
    namespace mt = ::uwvm2::extension::memtag;
    struct relocation { ::std::uint64_t slot{}, base{}, addend{}; };
    struct extent { ::std::uint64_t begin{}, end{}; };
    using relocations = ::uwvm2::utils::container::std_vector<relocation>;
    using extents = ::uwvm2::utils::container::std_vector<extent>;

    // Exact, same-width lld static data relocations in owned memory zero.
    // Each row reads two allocation tags and stores a pointer. An addend
    // operates on the tagged symbol base, which also covers one-past pointers.
    inline bool decode(::std::span<unsigned char const> body, unsigned bits, relocations& rows)
    {
        ::std::size_t pos{};
        auto take = [&](unsigned byte) noexcept { return pos < body.size() && body[pos++] == byte; };
        auto const constant{bits == 64u ? 0x42u : 0x41u};
        auto tagged_address = [&](::std::uint64_t& address) noexcept
        {
            ::std::uint32_t opcode{}, memory{};
            return take(constant) && update::scalar_detail::signed_integer(body, pos, bits, address) &&
                take(mt::opcode_prefix) && update::u32(body, pos, opcode) && opcode == mt::encode_opcode(mt::opcode::load) &&
                update::u32(body, pos, memory) && memory == 0u;
        };
        if(!take(0u)) { return false; } // No local declarations.
        while(pos < body.size() && body[pos] != 0x0bu)
        {
            relocation row{};
            if(!tagged_address(row.slot) || !tagged_address(row.base)) { return false; }
            if(pos < body.size() && body[pos] == constant)
            {
                ++pos;
                if(!update::scalar_detail::signed_integer(body, pos, bits, row.addend) ||
                   !take(bits == 64u ? 0x7cu : 0x6au)) { return false; }
            }
            // Natural pointer width; unaligned store, implicit memory zero, offset zero.
            if(!take(bits == 64u ? 0x37u : 0x36u) || !take(0u) || !take(0u)) { return false; }
            rows.push_back(row);
        }
        return !rows.empty() && take(0x0bu) && pos == body.size();
    }

    template<typename Address, typename View>
    inline bool materialize(relocations& rows, extents& data, extents& objects,
                            View const& view, ::std::span<::std::byte> memory)
    {
        auto by_begin = [](extent a, extent b) noexcept { return a.begin < b.begin; };
        ::std::sort(data.begin(), data.end(), by_begin);
        ::std::sort(objects.begin(), objects.end(), by_begin);
        ::std::sort(rows.begin(), rows.end(), [](auto a, auto b) noexcept { return a.slot < b.slot; });
        struct write { ::std::byte* destination; Address value; };
        ::uwvm2::utils::container::std_vector<write> prepared{};
        prepared.reserve(rows.size());
        auto segment{data.begin()};
        ::std::uint64_t data_end{}, previous_end{};
        for(auto const& row : rows)
        {
            while(segment != data.end() && segment->begin <= row.slot)
            { data_end = (::std::max)(data_end, segment->end); ++segment; }
            if(row.slot < previous_end || row.slot > data_end || sizeof(Address) > data_end - row.slot ||
               row.slot > memory.size() || sizeof(Address) > memory.size() - row.slot) { return false; }
            previous_end = row.slot + sizeof(Address);
            auto object{::std::upper_bound(objects.begin(), objects.end(), row.base,
                [](::std::uint64_t base, extent range) noexcept { return base < range.begin; })};
            if(object == objects.begin()) { return false; }
            --object;
            auto const base{static_cast<Address>(row.base)}, slot{static_cast<Address>(row.slot)};
            auto const raw{static_cast<Address>(base + static_cast<Address>(row.addend))};
            if(row.base >= object->end || raw < object->begin || raw > object->end ||
               view.canonical_address(raw) != raw) { return false; }
            auto const target_tag{view.load(base)}, slot_tag{view.load(slot)};
            if(!target_tag || !target_tag.allocation_tag || !slot_tag) { return false; }
            auto const destination{view.check(view.with_tag(slot, slot_tag.allocation_tag), 0u, sizeof(Address))};
            if(!destination || destination.address != row.slot) { return false; }
            auto* bytes{view.native_pointer(memory.data(), destination)};
            Address original{};
            ::std::memcpy(&original, bytes, sizeof(Address));
            if constexpr(::std::endian::native == ::std::endian::big) { original = ::std::byteswap(original); }
            if(original != raw) { return false; } // A relocation can only tint an existing data pointer.
            auto value{static_cast<Address>(view.with_tag(base, target_tag.allocation_tag) + static_cast<Address>(row.addend))};
            if(value != view.with_tag(raw, target_tag.allocation_tag)) { return false; }
            if constexpr(::std::endian::native == ::std::endian::big) { value = ::std::byteswap(value); }
            prepared.push_back({bytes, value});
        }
        // Every address, complete word, source tag and raw preimage is proved.
        // The private fence retains this exact owner and its tag geometry.
        for(auto const& word : prepared) { ::std::memcpy(word.destination, &word.value, sizeof(Address)); }
        return true;
    }

    inline update::status initialize(::uwvm2::uwvm::wasm::type::wasm_file_t const& file,
        ::uwvm2::uwvm::runtime::storage::wasm_module_storage_t const& module, ::std::optional<::std::size_t>& local_start)
    {
        namespace wasm = ::uwvm2::parser::wasm;
        namespace sections = wasm::standard::wasm1::features;
        namespace storage = ::uwvm2::uwvm::runtime::storage;
        namespace initializer = ::uwvm2::uwvm::runtime::initializer;
        auto const& parsed{file.wasm_module_storage.wasm_binfmt_ver1_storage.sections};
        auto const& start{wasm::concepts::operation::get_first_type_in_tuple<sections::start_section_storage_t>(parsed)};
        if(!start.sec_span.sec_begin) { return update::status::ok; }
        auto const imports{module.imported_function_vec_storage.size()};
        if(start.start_idx < imports || start.start_idx - imports >= module.local_defined_function_vec_storage.size() ||
           !module.imported_memory_vec_storage.empty() || module.local_defined_memory_vec_storage.size() != 1u)
        { return update::status::unsupported; }
        auto const local{static_cast<::std::size_t>(start.start_idx - imports)};
        auto const& function{module.local_defined_function_vec_storage.index_unchecked(local)};
        auto const* type{function.function_type_ptr};
        auto const& memory{module.local_defined_memory_vec_storage.front_unchecked().memory};
        auto const& tags{memory.memtag_shadow};
        if(!type || type->parameter.begin != type->parameter.end || type->result.begin != type->result.end ||
           !function.wasm_code_ptr || !tags.enabled() || tags.shared() || !memory.memory_length_p) { return update::status::unsupported; }
        auto const& body{function.wasm_code_ptr->body};
        relocations rows{};
        if(!decode({reinterpret_cast<unsigned char const*>(body.code_begin), static_cast<::std::size_t>(body.code_end - body.code_begin)},
            static_cast<unsigned>(tags.width()), rows)) { return update::status::unsupported; }

        // Compilation has validated the whole module and supported_world has
        // proved the native resources are private. Resolve active segments in
        // Wasm order before inspecting any slot, including overlapping segments.
        if(initializer::apply_runtime_active_segments(file.module_name) != initializer::details::active_segment_result::ok)
        { return update::status::validation_failed; }
        ::std::span<::std::byte> bytes{memory.memory_begin, memory.memory_length_p->load(::std::memory_order_acquire)};
        if(tags.hardware_active() && (tags.hardware_base() != bytes.data() || !mt::hardware_mte::enable_current_thread_sync()))
        { return update::status::invalid_state; }
        extents data{}, objects{};
        for(auto const& segment : module.local_defined_data_vec_storage)
        {
            auto const& item{segment.data};
            if(item.kind != storage::wasm_data_segment_kind::active || item.byte_begin == item.byte_end) { continue; }
            auto const length{static_cast<::std::size_t>(item.byte_end - item.byte_begin)};
            if(item.memory_idx != 0u || !storage::wasm_data_segment_is_dropped(item) ||
               item.offset > bytes.size() || length > bytes.size() - item.offset) { return update::status::validation_failed; }
            data.push_back({item.offset, item.offset + length});
        }
        auto const& custom{wasm::concepts::operation::get_first_type_in_tuple<sections::custom_section_storage_t>(parsed)};
        for(auto const& section : custom.customs)
        {
            if(section.custom_name != mt::linear_globals_section_name) { continue; }
            auto const* begin{reinterpret_cast<::std::byte const*>(section.custom_begin)};
            auto const* end{reinterpret_cast<::std::byte const*>(section.sec_span.sec_end)};
            if(!mt::visit_linear_globals({begin, static_cast<::std::size_t>(end - begin)},
                [&](::std::uint32_t index, ::std::uint64_t& length) noexcept { length = bytes.size(); return index == 0u; },
                [&](mt::linear_global_descriptor descriptor)
                { if(descriptor.byte_length) { objects.push_back({descriptor.address, descriptor.address + descriptor.byte_length}); } }))
            { return update::status::validation_failed; }
        }
        mt::scoped_access_checks check_all_tags{true};
        auto apply = [&]<typename Address>()
        { return materialize<Address>(rows, data, objects, tags.view<Address, false>(bytes.size()), bytes); };
        if(!(tags.width() == mt::address_width::i64 ? apply.template operator()<::std::uint64_t>() : apply.template operator()<::std::uint32_t>()))
        { return update::status::validation_failed; }
        local_start = local;
        return update::status::ok;
    }
}
