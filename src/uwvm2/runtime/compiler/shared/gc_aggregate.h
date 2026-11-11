#pragma once
#ifndef UWVM_MODULE
#include <uwvm2/uwvm/runtime/storage/wasm_module.h>
#include <uwvm2/uwvm/runtime/storage/gc_segment.h>
#endif
namespace uwvm2::runtime::compiler::shared
{
    [[nodiscard]] inline ::uwvm2::uwvm::runtime::storage::gc_object_status gc_aggregate(
        ::uwvm2::uwvm::runtime::storage::wasm_module_storage_t const& instance,
        unsigned opcode, ::std::uint32_t first, ::std::uint32_t second,
        ::uwvm2::uwvm::runtime::storage::gc_object_value const* inputs, ::std::size_t input_count,
        ::uwvm2::uwvm::runtime::storage::gc_object_value& result) noexcept
    {
        namespace storage=::uwvm2::uwvm::runtime::storage;
        using status=storage::gc_object_status;using value=storage::gc_object_value;using reference=storage::gc_reference;
        auto const* module=&instance;auto* store=instance.gc_store.get();
        if(!store || !store->valid() || (input_count && !inputs)) { return status::invalid_store; }
        reference ref{};
        status outcome{status::invalid_value};
        auto const as_ref{[&](::std::size_t index) noexcept { return inputs[index].template as<reference>(); }};
        auto const as_i32{[&](::std::size_t index) noexcept { return inputs[index].template as<::std::uint32_t>(); }};
        switch(opcode)
        {
            case 0u: outcome = store->struct_new(first, inputs, input_count, ref); break;
            case 1u: if(input_count == 0uz) { outcome = store->struct_new_default(first, ref); } break;
            case 2u: case 3u: case 4u:
                if(input_count == 1uz) { outcome = store->struct_get(as_ref(0uz), second, opcode == 3u, result); }
                break;
            case 5u:
                if(input_count == 2uz) { outcome = store->struct_set(as_ref(0uz), second, inputs[1]); }
                break;
            case 6u:
                if(input_count == 2uz) { outcome = store->array_new(first, inputs[0], as_i32(1uz), ref); }
                break;
            case 7u:
                if(input_count == 1uz) { outcome = store->array_new_default(first, as_i32(0uz), ref); }
                break;
            case 8u:
                if(input_count == second) { outcome = store->array_new_fixed(first, inputs, input_count, ref); }
                break;
            case 9u:
                if(input_count == 2uz)
                {
                    outcome = storage::uwvm2_gc_array_new_data(store, module, first, second,
                        as_i32(0uz), as_i32(1uz), ::std::addressof(ref));
                }
                break;
            case 10u:
                if(input_count == 2uz)
                {
                    outcome = storage::uwvm2_gc_array_new_elem(store, module, first, second,
                        as_i32(0uz), as_i32(1uz), ::std::addressof(ref));
                }
                break;
            case 11u: case 12u: case 13u:
                if(input_count == 2uz) { outcome = store->array_get(as_ref(0uz), as_i32(1uz), opcode == 12u, result); }
                break;
            case 14u:
                if(input_count == 3uz) { outcome = store->array_set(as_ref(0uz), as_i32(1uz), inputs[2]); }
                break;
            case 15u:
                if(input_count == 1uz)
                {
                    ::std::size_t length{};
                    outcome = store->array_length(as_ref(0uz), length);
                    if(outcome == status::ok)
                    {
                        if(length > (::std::numeric_limits<::std::uint32_t>::max)()) { outcome = status::size_overflow; }
                        else { result = value::i32(static_cast<::std::uint32_t>(length)); }
                    }
                }
                break;
            case 16u:
                if(input_count == 4uz) { outcome = store->array_fill(as_ref(0uz), as_i32(1uz), inputs[2], as_i32(3uz)); }
                break;
            case 17u:
                if(input_count == 5uz)
                { outcome = store->array_copy(as_ref(0uz), as_i32(1uz), as_ref(2uz), as_i32(3uz), as_i32(4uz)); }
                break;
            case 18u:
                if(input_count == 4uz)
                {
                    auto const array{as_ref(0uz)};
                    outcome = storage::uwvm2_gc_array_init_data(store, module, first, second,
                        ::std::addressof(array), as_i32(1uz), as_i32(2uz), as_i32(3uz));
                }
                break;
            case 19u:
                if(input_count == 4uz)
                {
                    auto const array{as_ref(0uz)};
                    outcome = storage::uwvm2_gc_array_init_elem(store, module, first, second,
                        ::std::addressof(array), as_i32(1uz), as_i32(2uz), as_i32(3uz));
                }
                break;
            default: break;
        }
        if(outcome == status::ok)
        {
            if(opcode == 0u || opcode == 1u || (opcode >= 6u && opcode <= 10u))
            { result = value::reference(ref); }
        }
        return outcome;
    }

}
