#include "../uwvm_int_translate_strict_common.h"
#include <uwvm2/utils/macro/push_macros.h>

#if defined(UWVM_SUPPORT_MMAP) && defined(UWVM_ENABLE_UWVM_INT_COMBINE_OPS) && !defined(UWVM2TEST_RUNNER_USE_LLVM_JIT)
namespace
{
    using namespace uwvm2test::uwvm_int_strict;
    namespace mt = uwvm2::extension::memtag;
    namespace mode = uwvm2::uwvm::runtime::runtime_mode;
    using policy = optable::memtag_details::scalar_policy;

    byte_vec module(std::uint8_t adm_bits, bool tagged, bool second_memory)
    {
        module_builder mb{};
        mb.has_memory = mb.memory_address64 = mb.memory_has_max = true;
        mb.memory_min = 1;
        mb.memory_max = 2;
        mb.memory_tagged = tagged;
        mb.memory_adm_bits = adm_bits;
        if(second_memory) { mb.extra_memories.push_back({.min=1, .max=2, .has_max=true, .address64=true}); }

        func_body body{};
        auto emit = [&](wasm_op op) { append_u8(body.code, u8(op)); };
        auto get = [&](unsigned index) { emit(wasm_op::local_get); append_u32_leb(body.code, index); };
        get(0); // Keep the complete address as a live operand across both loads.
        get(0); emit(wasm_op::i64_load); append_u32_leb(body.code, 3); append_u64_leb(body.code, 7);
        get(1); emit(wasm_op::i32_load8_u); append_u32_leb(body.code, second_memory ? 0x40u : 0u);
        if(second_memory) { append_u32_leb(body.code, 1); }
        append_u64_leb(body.code, 9);
        emit(wasm_op::i64_extend_i32_u);
        emit(wasm_op::i64_xor);
        emit(wasm_op::i64_xor);
        emit(wasm_op::end);
        mb.add_func({{k_val_i64, k_val_i64}, {k_val_i64}}, std::move(body));
        return mb.build();
    }

    template<optable::uwvm_interpreter_translate_option_t opt = k_test_byref_opt>
    int check(std::uint8_t adm_bits, bool tagged, bool checks, bool combine, bool second_memory = false)
    {
        auto wasm = module(adm_bits, tagged, second_memory);
        auto features = make_wasm1p1_feature_parameter();
        auto& f = uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1p1_parameter(features);
        f.disable_memory64 = f.disable_multi_memory = f.disable_adm_extension = f.disable_memtag_extension = false;
        f.cli_mode = uwvm2::parser::wasm::standard::wasm1p1::features::wasm_feature_cli_mode::scoped;
        auto prepared = prepare_runtime_from_wasm(wasm, u8"adm16-local-load", {}, features, {}, checks, mt::owner_backend::software);
        auto& memory = const_cast<optable::memtag_details::native_memory_t&>(prepared.mod->local_defined_memory_vec_storage.index_unchecked(0).memory);
        auto const previous_combine = std::exchange(mode::global_runtime_uwvm_int_opcode_conbination_level,
            combine ? mode::runtime_uwvm_int_opcode_conbination_level_t::soft : mode::runtime_uwvm_int_opcode_conbination_level_t::disable);

        uwvm2::validation::error::code_validation_error_impl error{};
        optable::compile_option options{};
        options.execute_memtag = checks;
        auto compiled = compiler::compile_all_from_uwvm_single_func<opt>(*prepared.mod, options, error, &features);
        mode::global_runtime_uwvm_int_opcode_conbination_level = previous_combine;
        auto const& fn = compiled.local_funcs.index_unchecked(0);
        constexpr auto tuple = compiler::details::make_interpreter_tuple<opt>(std::make_index_sequence<compiler::details::interpreter_tuple_size<opt>()>{});
        constexpr auto positions = make_initial_stacktop_currpos<opt>();
        auto const load64 = optable::translate::get_uwvmint_memtag_scalar_cached_from_tuple<std::uint64_t, std::int64_t, 8, false, false,
            policy::adm16_live_local, opt>(positions, memory, tuple);
        auto const load8 = optable::translate::get_uwvmint_memtag_scalar_cached_from_tuple<std::uint64_t, std::int32_t, 1, false, false,
            policy::adm16_live_local, opt>(positions, memory, tuple);
        bool const fused = adm_bits == 16 && !(tagged && checks) && combine;
        UWVM2TEST_REQUIRE(bytecode_contains_fptr(fn.op.operands, load64) == fused);
        UWVM2TEST_REQUIRE(bytecode_contains_fptr(fn.op.operands, load8) == (fused && !second_memory));
        for(std::uint64_t page : {0u, 65536u})
        {
            if(page) { UWVM2TEST_REQUIRE(memory.try_grow_silently(1)); }
            constexpr std::array<std::uint8_t, 8> bytes{0x98, 0x76, 0xd3, 0x10, 0x22, 0x54, 0xba, 0xfe};
            std::memcpy(memory.memory_begin + page + 10, bytes.data(), bytes.size());
            auto const raw = ((adm_bits && !checks) ? UINT64_C(0xad00000000000000) : 0u) | (page + 3);
            auto const byte_address = second_memory ? page + 3 : raw;
            std::uint64_t expected_byte = 0xd3;
            if(second_memory)
            {
                auto& other = const_cast<optable::memtag_details::native_memory_t&>(prepared.mod->local_defined_memory_vec_storage.index_unchecked(1).memory);
                if(page) { UWVM2TEST_REQUIRE(other.try_grow_silently(1)); }
                other.memory_begin[byte_address + 9] = std::byte{0xe1};
                expected_byte = 0xe1;
            }
            byte_vec args(sizeof(raw) + sizeof(byte_address));
            std::memcpy(args.data(), &raw, sizeof(raw));
            std::memcpy(args.data() + sizeof(raw), &byte_address, sizeof(byte_address));
            auto result = interpreter_runner<opt>::run(fn, prepared.mod->local_defined_function_vec_storage.index_unchecked(0), args, load64, load8);
            if constexpr(!opt.is_tail_call)
            { UWVM2TEST_REQUIRE(result.hit_expected0 == fused && result.hit_expected1 == (fused && !second_memory)); }
            UWVM2TEST_REQUIRE(result.results.size() == 8);
            UWVM2TEST_REQUIRE(static_cast<std::uint64_t>(load_i64(result.results)) == (raw ^ UINT64_C(0xfeba542210d37698) ^ expected_byte));
        }
        return 0;
    }
}
#endif

int main()
{
#if defined(UWVM_SUPPORT_MMAP) && defined(UWVM_ENABLE_UWVM_INT_COMBINE_OPS) && !defined(UWVM2TEST_RUNNER_USE_LLVM_JIT)
    install_unexpected_traps();
    UWVM2TEST_REQUIRE(check(16, false, false, true) == 0);
    UWVM2TEST_REQUIRE(check(16, true, false, true) == 0);
    UWVM2TEST_REQUIRE(check(16, true, true, true) == 0);
    UWVM2TEST_REQUIRE(check(8, false, false, true) == 0);
    UWVM2TEST_REQUIRE(check(0, false, false, true) == 0);
    UWVM2TEST_REQUIRE(check(16, false, false, false) == 0);
    UWVM2TEST_REQUIRE(check(16, false, false, true, true) == 0);
    // A single merged integer slot forces the translator to spill the older live values.
    constexpr auto one_slot = make_tailcall_hardfloat_abi_opt<1, 1, true>();
    UWVM2TEST_REQUIRE(check<one_slot>(16, false, false, true) == 0);
#endif
    return 0;
}

#include <uwvm2/utils/macro/pop_macros.h>
