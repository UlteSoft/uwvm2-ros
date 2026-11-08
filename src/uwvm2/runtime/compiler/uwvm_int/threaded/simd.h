#pragma once
namespace uwvm2::runtime::compiler::uwvm_int::threaded
{
    template<::uwvm2::runtime::compiler::shared::simd_code Op,
        ::uwvm2::runtime::compiler::shared::wasm1p1_simd_instruction_kind Kind,
        ::uwvm2::runtime::compiler::shared::wasm1p1_simd_scalar_kind Scalar,
        ::std::size_t Lanes,unsigned Align>
    inline bool engine::simd_execute(engine& actual,instruction const& ins) noexcept
    {
        namespace shared=::uwvm2::runtime::compiler::shared;
        namespace simd=shared::wasm1p1_simd_details;
        using kind=shared::wasm1p1_simd_instruction_kind;using scalar=shared::wasm1p1_simd_scalar_kind;
        using vector=simd::wasm_v128;
        static_assert(sizeof(vector)==sizeof(stack_value));
        auto pop_vector{[&](vector& value) noexcept
        { stack_value bits{};if(!actual.pop(bits)) { return false; }value=::std::bit_cast<vector>(bits);return true; }};
        auto push_vector{[&](vector value) noexcept { return actual.push(::std::bit_cast<stack_value>(value)); }};
        vector a{},b{},c{};::std::uint64_t integer{};auto const lane{static_cast<simd::u8>(ins.alternate)};
        if constexpr(Kind==kind::constant) { return push_vector(::std::bit_cast<vector>(ins.literal)); }
        else if constexpr(Kind==kind::shuffle)
        {
            if(!pop_vector(b)||!pop_vector(a)) { return false; }
            simd::u8 lanes[16]{};::std::memcpy(lanes,ins.literal.data(),16u);
            return push_vector(simd::eval_shuffle(a,b,simd::make_shuffle_controls(lanes)));
        }
        else if constexpr(Kind==kind::splat || Kind==kind::replace_lane)
        {
            if(!actual.pop(integer)) { return false; }
            if constexpr(Kind==kind::replace_lane) { if(!pop_vector(a)) { return false; } }
            if constexpr(Scalar==scalar::i32)
            {
                auto const value{::std::bit_cast<simd::wasm_i32>(static_cast<::std::uint32_t>(integer))};
                if constexpr(Kind==kind::splat) { return push_vector(simd::eval_full_splat_i32<Op>(value)); }
                else { return push_vector(simd::eval_replace_lane_i32<Op>(a,value,lane)); }
            }
            else if constexpr(Scalar==scalar::i64)
            {
                auto const value{::std::bit_cast<simd::wasm_i64>(integer)};
                if constexpr(Kind==kind::splat) { return push_vector(simd::eval_full_splat_i64<Op>(value)); }
                else { return push_vector(simd::eval_replace_lane_i64<Op>(a,value,lane)); }
            }
            else if constexpr(Scalar==scalar::f32)
            {
                auto const value{::std::bit_cast<simd::wasm_f32>(static_cast<::std::uint32_t>(integer))};
                if constexpr(Kind==kind::splat) { return push_vector(simd::eval_full_splat_f32<Op>(value)); }
                else { return push_vector(simd::eval_replace_lane_f32<Op>(a,value,lane)); }
            }
            else
            {
                static_assert(Scalar==scalar::f64);
                auto const value{::std::bit_cast<simd::wasm_f64>(integer)};
                if constexpr(Kind==kind::splat) { return push_vector(simd::eval_full_splat_f64<Op>(value)); }
                else { return push_vector(simd::eval_replace_lane_f64<Op>(a,value,lane)); }
            }
        }
        else if constexpr(Kind==kind::extract_lane)
        {
            if(!pop_vector(a)) { return false; }
            if constexpr(Scalar==scalar::i32) { return actual.push(::std::bit_cast<::std::uint32_t>(simd::eval_extract_lane_i32<Op>(a,lane))); }
            else if constexpr(Scalar==scalar::i64) { return actual.push(::std::bit_cast<::std::uint64_t>(simd::eval_extract_lane_i64<Op>(a,lane))); }
            else if constexpr(Scalar==scalar::f32) { return actual.push(::std::bit_cast<::std::uint32_t>(simd::eval_extract_lane_f32<Op>(a,lane))); }
            else { static_assert(Scalar==scalar::f64);return actual.push(::std::bit_cast<::std::uint64_t>(simd::eval_extract_lane_f64<Op>(a,lane))); }
        }
        else if constexpr(Kind==kind::unary)
        { return pop_vector(a)&&push_vector(simd::eval_full_unop<Op>(a)); }
        else if constexpr(Kind==kind::binary)
        { return pop_vector(b)&&pop_vector(a)&&push_vector(simd::eval_full_binop<Op>(a,b)); }
        else if constexpr(Kind==kind::ternary)
        {
            if(!pop_vector(c)||!pop_vector(b)||!pop_vector(a)) { return false; }
            if constexpr(Op==shared::simd_code::v128_bitselect) { return push_vector(simd::eval_bitselect(a,b,c)); }
            else { return push_vector(simd::eval_full_ternop<Op>(a,b,c)); }
        }
        else if constexpr(Kind==kind::test)
        { return pop_vector(a)&&actual.push(static_cast<::std::uint32_t>(simd::eval_full_test<Op>(a))); }
        else if constexpr(Kind==kind::shift)
        {
            return actual.pop(integer)&&pop_vector(a)&&push_vector(
                simd::eval_full_shift<Op>(a,::std::bit_cast<simd::wasm_i32>(static_cast<::std::uint32_t>(integer))));
        }
        else
        {
            static_assert(Kind==kind::memory_load || Kind==kind::memory_store);
            if constexpr(Kind==kind::memory_store || Lanes!=0u) { if(!pop_vector(a)) { return false; } }
            if(!actual.pop(integer)) { return false; }
            auto& memory{*actual.modules_[actual.calls_[actual.call_top_-1u].module].memory};
            constexpr auto bytes{simd::simd_memory_access_size<Op>()};
            auto const okay{actual.memory_span(memory,static_cast<::std::uint32_t>(integer),ins.immediate,bytes,
                [&](::std::span<::std::byte> data) noexcept
                {
                    // Shared plain SIMD accesses may tear, but cannot create a
                    // C++ data race. Evaluate only on a private byte snapshot.
                    ::std::array<::std::byte,16> buffer{};
                    using byte_type=typename shared::wasm_threads::atomic_access_traits<1>::type;
                    if constexpr(Kind==kind::memory_load)
                    {
                        for(::std::size_t i{};i!=bytes;++i)
                        { buffer[i]=static_cast<::std::byte>(__atomic_load_n(reinterpret_cast<byte_type*>(data.data()+i),__ATOMIC_RELAXED)); }
                        b=simd::eval_memory_load<Op>(buffer.data(),a,lane);
                    }
                    else
                    {
                        simd::eval_memory_store<Op>(buffer.data(),a,lane);
                        for(::std::size_t i{};i!=bytes;++i)
                        { __atomic_store_n(reinterpret_cast<byte_type*>(data.data()+i),static_cast<byte_type>(buffer[i]),__ATOMIC_RELAXED); }
                    }
                })};
            if(!okay) { return false; }
            if constexpr(Kind==kind::memory_load) { return push_vector(b); }
            else { return true; }
        }
    }
}
