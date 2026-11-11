#pragma once
#ifndef UWVM_MODULE
#include "contract.h"
#include <algorithm>
#include <bit>
#include <cstring>
#include <limits>
#include <vector>
#include <array>
#include <type_traits>
#include <uwvm2/runtime/compiler/shared/wasm1p1_simd.h>
#include <uwvm2/extension/memtag/encoding.h>
#include <uwvm2/uwvm/runtime/storage/impl.h>
#include <uwvm2/validation/error/impl.h>
#include <uwvm2/validation/standard/wasm3/reference_policy.h>
#include <uwvm2/validation/standard/wasm3/exception_immediate.h>
#include <uwvm2/validation/standard/wasm3/gc_immediate.h>

#endif
#include "value.h"
#if defined(__linux__) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS)
namespace uwvm2::runtime::compiler::uwvm_int::threaded
{
    namespace storage = ::uwvm2::uwvm::runtime::storage;
    using signature = storage::wasm_binfmt1_final_function_type_t;
    using value_type = storage::wasm_binfmt1_final_value_type_t;
    namespace core = ::uwvm2::parser::wasm::standard::wasm3::type;
    class engine;
    struct catch_clause { unsigned kind{}; ::std::uint32_t tag{}, label{}; };
    struct instruction
    {
        unsigned opcode{};
        ::std::uint64_t immediate{};
        ::std::size_t offset{}, target{}, alternate{}, parameters{}, results{};
        ::std::vector<::std::uint32_t> labels{};
        ::std::vector<catch_clause> catches{};
        ::std::array<::std::uint8_t,16u> literal{};
        bool (*simd_handler)(engine&,instruction const&) noexcept{};

        // Lowering groups prefixed instructions for dispatch. Delivery policy
        // still uses the original primary opcode (not that private dispatch id).
        [[nodiscard]] constexpr unsigned primary_opcode() const noexcept
        {
            return opcode>=0x10000u ? 0xfdu : opcode>=0x800u ? 0xfbu : opcode>=0x400u ? 0xffu :
                opcode>=0x200u ? 0xfcu : opcode>=0x100u ? 0xfeu : opcode;
        }
    };
    struct function_program
    {
        signature const* type{};
        ::std::size_t locals{};
        // Instruction/debug maps retain immutable file coordinates. Public
        // Wasm fault events use an offset from this function's first opcode.
        ::std::size_t expression_file_offset{};
        ::std::vector<instruction> instructions{};

        [[nodiscard]] ::std::uint64_t expression_offset(::std::size_t file_offset) const noexcept
        { return file_offset>=expression_file_offset ? file_offset-expression_file_offset : UINT64_MAX; }
    };
    struct module_program
    {
        storage::wasm_module_storage_t const* instance{};
        ::std::vector<function_program> functions{};
    };
    namespace detail
    {
        [[nodiscard]] inline bool numeric_type(value_type t) noexcept
        { return t==value_type::i32 || t==value_type::i64 || t==value_type::f32 || t==value_type::f64 || t==value_type::v128; }
        [[nodiscard]] inline bool value_type_supported(core::core_value_type t) noexcept
        {
            return t.kind<=core::value_kind::v128 || t.kind==core::value_kind::reference;
        }
        [[nodiscard]] inline bool carrier_type(value_type t) noexcept
        { return numeric_type(t) || t==value_type::externref || t==value_type::funcref; }
        [[nodiscard]] inline ::std::size_t abi_bytes(value_type t) noexcept
        { return t==value_type::i32 || t==value_type::f32 ? 4uz : t==value_type::v128 ? 16uz : t==value_type::funcref ? sizeof(storage::gc_reference) : 8uz; }
        [[nodiscard]] inline bool abi_fits(value_type const* begin,value_type const* end,::std::size_t limit) noexcept
        {
            for(auto at{begin};at!=end;++at)
            { auto const bytes{abi_bytes(*at)};if(bytes>limit) { return false; }limit-=bytes; }
            return true;
        }
        [[nodiscard]] inline bool executable_signature(signature const* t) noexcept
        {
            if(!t) { return false; }
            for(auto i{t->parameter.begin}; i != t->parameter.end; ++i) { if(!carrier_type(*i)) { return false; } }
            for(auto i{t->result.begin}; i != t->result.end; ++i) { if(!carrier_type(*i)) { return false; } }
            return true;
        }
        [[nodiscard]] inline bool equal_signature(signature const* a, signature const* b) noexcept
        {
            if(!executable_signature(a) || !executable_signature(b)) { return false; }
            for(auto i{a->parameter.begin};i!=a->parameter.end;++i) { if(!numeric_type(*i)) { return false; } }
            for(auto i{a->result.begin};i!=a->result.end;++i) { if(!numeric_type(*i)) { return false; } }
            for(auto i{b->parameter.begin};i!=b->parameter.end;++i) { if(!numeric_type(*i)) { return false; } }
            for(auto i{b->result.begin};i!=b->result.end;++i) { if(!numeric_type(*i)) { return false; } }
            return (a->parameter.end-a->parameter.begin)==(b->parameter.end-b->parameter.begin) &&
                   (a->result.end-a->result.begin)==(b->result.end-b->result.begin) &&
                   ::std::equal(a->parameter.begin,a->parameter.end,b->parameter.begin) &&
                   ::std::equal(a->result.begin,a->result.end,b->result.begin);
        }
        struct source_cursor
        {
            ::std::byte const* begin{};
            ::std::byte const* current{};
            ::std::byte const* end{};
            bool valid{true};
            [[nodiscard]] unsigned byte() noexcept
            {
                if(current==end) { valid=false;return 0; }
                return ::std::to_integer<unsigned>(*current++);
            }
            [[nodiscard]] ::std::uint64_t leb(unsigned width, bool signed_value=false) noexcept
            {
                ::std::uint64_t out{};unsigned shift{}, last{};
                for(unsigned n{}; n != (width+6u)/7u; ++n)
                {
                    last=byte();if(!valid) { return 0; }
                    unsigned const available{width-shift};
                    unsigned const mask{available>=7u?127u:((1u<<available)-1u)};
                    unsigned const unused{(last&127u)&~mask};
                    if(available<7u && ((!signed_value && unused) ||
                        (signed_value && unused!=0u && unused!=(127u&~mask)))) { valid=false;return 0; }
                    out|=static_cast<::std::uint64_t>(last&mask)<<shift;
                    shift+=available<7u?available:7u;
                    if(!(last&128u))
                    {
                        if(signed_value && shift<64u && (last&64u)) { out|=(~::std::uint64_t{})<<shift; }
                        return out;
                    }
                }
                valid=false;return 0;
            }
        };
        [[nodiscard]] inline bool supported_plain(unsigned op) noexcept
        {
            return op<=0x05u || op==0x08u || op==0x0au || (op>=0x0bu&&op<=0x13u) || (op>=0x1au && op<=0x1cu) || op==0x1fu || (op>=0x20u && op<=0x24u) ||
                   (op>=0x28u && op<=0x40u) || (op>=0x41u && op<=0xc4u) ||
                   op==0xd0u || op==0xd1u || (op>=0xd4u && op<=0xd6u);
        }
        // The caller retains a nonforgeable World/source validation certificate
        // from the actual immutable validated file and unchanged feature object.
        // Checked lowering still rejects every unsupported source shape.
        [[nodiscard]] inline status lower(storage::wasm_module_storage_t const& module,
            storage::local_defined_function_storage_t const& source, function_program& out,
            limits const& budget, ::std::size_t& charged_bytes, ::std::uintptr_t source_begin)
        {
            out.type=source.function_type_ptr;
            if(!executable_signature(out.type) || !source.wasm_code_ptr) { return status::unsupported_profile; }
            out.locals=static_cast<::std::size_t>(out.type->parameter.end-out.type->parameter.begin);
            if(out.locals>budget.values) { return status::resource_limit; }
            for(auto const& run: source.wasm_code_ptr->locals)
            {
                if(!(run.has_core_type ? value_type_supported(run.core_type) : numeric_type(run.type)) || run.count>budget.values-out.locals) { return status::unsupported_profile; }
                out.locals+=run.count;
            }
            auto const& body{source.wasm_code_ptr->body};
            out.expression_file_offset=static_cast<::std::size_t>(
                reinterpret_cast<::std::uintptr_t>(body.expr_begin)-source_begin);
            source_cursor input{reinterpret_cast<::std::byte const*>(body.expr_begin),
                reinterpret_cast<::std::byte const*>(body.expr_begin),reinterpret_cast<::std::byte const*>(body.code_end)};
            auto const bytes{static_cast<::std::size_t>(input.end-input.begin)};
            // Conservative exact-capacity reserve. No vector growth can exceed
            // the charged instruction bytes while lowering this function.
            if(charged_bytes>budget.instruction_bytes || bytes>(budget.instruction_bytes-charged_bytes)/sizeof(instruction))
            { return status::resource_limit; }
            charged_bytes+=bytes*sizeof(instruction);out.instructions.reserve(bytes);
            ::std::vector<::std::size_t> nesting{};nesting.reserve(::std::min(bytes,budget.controls));
            while(input.current!=input.end && input.valid)
            {
                instruction ins{};ins.offset=static_cast<::std::size_t>(reinterpret_cast<::std::uintptr_t>(input.current)-source_begin);ins.opcode=input.byte();
                auto const position{out.instructions.size()};
                auto const op{ins.opcode};
                if(op==2u || op==3u || op==4u || op==0x1fu)
                {
                    auto const block{static_cast<::std::int64_t>(input.leb(33u,true))};
                    if(block==-64) { }
                    else if(block>=-5 && block<=-1) { ins.results=1; }
                    else if(block==-23 || block==-12) { ins.results=1; }
                    else if(block==-29 || block==-28)
                    {
                        auto const heap{static_cast<::std::int64_t>(input.leb(33u,true))};
                        if(heap!=-23 && heap!=-12) { return status::unsupported_profile; }
                        ins.results=1;
                    }
                    else if(block>=0 && static_cast<::std::size_t>(block)<module.type_section_storage.type_section_count)
                    {
                        auto const* t{module.type_section_storage.type_section_begin+block};
                        if(!executable_signature(t)) { return status::unsupported_profile; }
                        ins.parameters=static_cast<::std::size_t>(t->parameter.end-t->parameter.begin);
                        ins.results=static_cast<::std::size_t>(t->result.end-t->result.begin);
                    }
                    else { return status::unsupported_profile; }
                    if(op==0x1fu)
                    {
                        auto const count{input.leb(32u)};
                        if(count>budget.controls || count>(budget.instruction_bytes-charged_bytes)/sizeof(catch_clause)) { return status::resource_limit; }
                        charged_bytes+=count*sizeof(catch_clause);ins.catches.reserve(count);
                        for(::std::uint64_t n{};n<count;++n)
                        {
                            catch_clause clause{};clause.kind=input.byte();if(clause.kind>3u) { return status::validation_failed; }
                            if(clause.kind<2u) { clause.tag=static_cast<::std::uint32_t>(input.leb(32u)); }
                            clause.label=static_cast<::std::uint32_t>(input.leb(32u));ins.catches.push_back(clause);
                        }
                    }
                    if(nesting.size()==budget.controls) { return status::resource_limit; }
                    nesting.push_back(position);
                }
                else if(op==5u)
                {
                    if(nesting.empty() || out.instructions[nesting.back()].opcode!=4u) { return status::validation_failed; }
                    out.instructions[nesting.back()].alternate=position+1uz;
                    ins.target=nesting.back();
                }
                else if(op==0x0bu)
                {
                    if(!nesting.empty())
                    {
                        auto const block{nesting.back()};nesting.pop_back();out.instructions[block].target=position;
                        if(!out.instructions[block].alternate) { out.instructions[block].alternate=position; }
                        if(out.instructions[block].opcode==4u && out.instructions[block].alternate!=position)
                        { out.instructions[out.instructions[block].alternate-1uz].target=position; }
                    }
                    else if(input.current!=input.end) { return status::validation_failed; }
                }
                else if(op==0x0eu)
                {
                    auto const n{input.leb(32u)};
                    if(n>budget.controls || n+1u>(budget.instruction_bytes-charged_bytes)/sizeof(::std::uint32_t))
                    { return status::resource_limit; }
                    charged_bytes+=(n+1u)*sizeof(::std::uint32_t);ins.labels.reserve(n+1u);
                    for(::std::uint64_t i{};i<=n;++i) { ins.labels.push_back(static_cast<::std::uint32_t>(input.leb(32u))); }
                }
                else if(op==0x0cu || op==0x0du || op==0x10u || op==0x12u || op==0x08u || (op>=0x20u&&op<=0x24u) || (op==0x3fu||op==0x40u) || op==0xd5u || op==0xd6u)
                { ins.immediate=input.leb(32u); }
                else if(op==0x11u || op==0x13u) { ins.immediate=input.leb(32u);ins.target=input.leb(32u); }
                else if(op==0x41u || op==0x42u) { ins.immediate=input.leb(op==0x41u?32u:64u,true); }
                else if(op==0x43u || op==0x44u)
                { for(unsigned n{};n!=(op==0x43u?4u:8u);++n) { ins.immediate|=static_cast<::std::uint64_t>(input.byte())<<(8u*n); } }
                else if(op==0x1cu)
                {
                    if(input.leb(32u)!=1u) { return status::validation_failed; }
                    auto const t{static_cast<::std::int64_t>(input.leb(33u,true))};
                    if(t==-29 || t==-28)
                    { auto const heap{static_cast<::std::int64_t>(input.leb(33u,true))};if(heap!=-23&&heap!=-12) { return status::unsupported_profile; } }
                    else if((t<-5||t>-1)&&t!=-23&&t!=-12) { return status::unsupported_profile; }
                }
                else if(op==0xd0u)
                {
                    namespace v3=::uwvm2::validation::standard::wasm3;
                    v3::recursive_type_context const empty_context{};
                    auto const* context{module.type_section_storage.core3_context_ptr};
                    // The retained source certificate already checked feature
                    // admission. Decode the complete heap again; every valid
                    // null heap uses the same all-zero runtime carrier.
                    auto const decoded{v3::scan_core3_ref_null_heap(input.current,input.end,
                        context==nullptr ? empty_context : *context,true,true,true)};
                    if(decoded.error!=v3::core3_ref_null_error::ok)
                    {
                        // Legacy function-only sections do not populate the
                        // recursive context. Failed decoders leave the cursor
                        // unchanged; only this actual type-count witness may
                        // admit their concrete function indices.
                        if(context!=nullptr || decoded.error!=v3::core3_ref_null_error::unknown_type)
                        { return status::validation_failed; }
                        auto const legacy{v3::scan_function_ref_null_heap(input.current,input.end,true,
                            module.type_section_storage.type_section_count,true,true)};
                        if(legacy.error!=v3::function_heap_immediate_error::ok)
                        { return status::validation_failed; }
                    }
                }
                else if(op==0xfbu)
                {
                    namespace v3=::uwvm2::validation::standard::wasm3;
                    auto const decoded{v3::scan_gc_instruction(input.current,input.end)};
                    if(decoded.error!=v3::gc_immediate_error::ok) { return status::validation_failed; }
                    // Aggregate and i31 operations use owned, full-width native
                    // reference carriers; unsupported casts remain explicit.
                    if(decoded.opcode>19u && decoded.opcode<28u) { return status::unsupported_profile; }
                    ins.opcode=0x800u+decoded.opcode;ins.immediate=decoded.first;ins.target=decoded.second;
                    auto const sub=decoded.opcode;
                    if(sub==0u)
                    { if(!module.gc_store || !module.gc_store->field_count(decoded.first,ins.parameters)) { return status::validation_failed; } }
                    else if(sub==1u) { ins.parameters=0u; }
                    else if(sub==8u) { ins.parameters=decoded.second; }
                    else if(sub==5u || sub==6u || sub==9u || sub==10u || (sub>=11u&&sub<=13u)) { ins.parameters=2u; }
                    else if(sub==14u) { ins.parameters=3u; }
                    else if(sub==16u || sub==18u || sub==19u) { ins.parameters=4u; }
                    else if(sub==17u) { ins.parameters=5u; }
                    else { ins.parameters=1u; }
                    if(ins.parameters>budget.values) { return status::resource_limit; }
                    ins.results=(sub==5u || sub==14u || (sub>=16u&&sub<=19u)) ? 0u : 1u;
                }
                else if(op==0xfcu || op==::uwvm2::extension::memtag::opcode_prefix)
                {
                    ::std::uint64_t sub{};
                    if(op==::uwvm2::extension::memtag::opcode_prefix)
                    {
                        auto const decoded{::uwvm2::extension::memtag::scan_opcode(input.current,input.end)};
                        if(decoded.error!=::uwvm2::extension::memtag::encoding_error::none)
                        { return status::validation_failed; }
                        sub=decoded.subopcode;
                    }
                    else
                    {
                        sub=input.leb(32u);
                        if(::uwvm2::extension::memtag::find_instruction(static_cast<::std::uint32_t>(sub)) != nullptr)
                        { return status::validation_failed; }
                    }
                    if(sub<=7u) { ins.opcode=0x200u+static_cast<unsigned>(sub); }
                    else if(sub<=11u)
                    {
                        ins.opcode=0x300u+static_cast<unsigned>(sub);
                        ins.immediate=input.leb(32u);
                        if(sub==8u || sub==10u) { ins.target=input.leb(32u); }
                    }
                    else if(auto const* tag{::uwvm2::extension::memtag::find_instruction(static_cast<::std::uint32_t>(sub))};
                        tag!=nullptr && tag->operation!=::uwvm2::extension::memtag::opcode::add)
                    {
                        ins.opcode=0x400u+static_cast<unsigned>(sub);
                        ins.target=input.leb(32u);
                        if(ins.target!=0u) { return status::unsupported_profile; }
                    }
                    else { return status::unsupported_profile; }
                }
                else if(op==0xfdu)
                {
                    namespace shared=::uwvm2::runtime::compiler::shared;
                    auto const sub{input.leb(32u)};ins.opcode=0x10000u+static_cast<unsigned>(sub);
                    bool const supported{shared::visit_wasm1p1_simd_instruction(static_cast<shared::simd_code>(sub),
                        [&]<shared::simd_code Op,shared::wasm1p1_simd_instruction_kind Kind,
                            shared::wasm1p1_simd_scalar_kind Scalar,::std::size_t Lanes,unsigned Align>() noexcept
                        {
                            using kind=shared::wasm1p1_simd_instruction_kind;
                            if constexpr(Kind==kind::constant || Kind==kind::shuffle)
                            {
                                for(auto& byte:ins.literal)
                                {
                                    byte=static_cast<::std::uint8_t>(input.byte());
                                    if constexpr(Kind==kind::shuffle) { if(byte>=32u) { input.valid=false; } }
                                }
                            }
                            else if constexpr(Kind==kind::memory_load || Kind==kind::memory_store)
                            {
                                auto const flags{input.leb(32u)};
                                if((flags&~127u)!=0u || (flags&63u)>Align) { input.valid=false;return true; }
                                ins.target=(flags&64u)?input.leb(32u):0u;
                                if(ins.target!=0u) { return false; }
                                ins.immediate=input.leb(module.memory_declarations_require_memory64?64u:32u);
                                if constexpr(Lanes!=0u) { ins.alternate=input.byte();if(ins.alternate>=Lanes) { input.valid=false; } }
                            }
                            else if constexpr(Kind==kind::extract_lane || Kind==kind::replace_lane)
                            { ins.alternate=input.byte();if(ins.alternate>=Lanes) { input.valid=false; } }
                            return true;
                        })};
                    if(!supported) { return status::unsupported_profile; }
                }
                else if((op>=0x28u&&op<=0x3eu) || op==0xfeu)
                {
                    if(op==0xfeu)
                    {
                        auto const sub{input.leb(32u)};
                        if(sub>0x4eu || (sub>3u&&sub<0x10u)) { return status::unsupported_profile; }
                        ins.opcode=0x100u+static_cast<unsigned>(sub);
                        if(sub==3u) { if(input.byte()!=0u) { return status::validation_failed; } }
                    }
                    if(ins.opcode!=0x103u)
                    {
                        auto const alignment{input.leb(32u)};
                        // This finite profile is wasm32, one imported shared
                        // memory. Multi-memory/memory64 are not silently erased.
                        if(alignment&~63u) { return status::unsupported_profile; }
                        ins.immediate=input.leb(32u);
                    }
                }
                else if(!supported_plain(op)) { return status::unsupported_profile; }
                // GC/SIMD, call_ref/tail calls and bulk table remain cold
                // unsupported. Atomic subcodes and numeric opcodes are decoded
                // exactly; no guest feature defaults are rewritten.
                out.instructions.push_back(::std::move(ins));
            }
            return input.valid && nesting.empty() && !out.instructions.empty() &&
                out.instructions.back().opcode==0x0bu ? status::ok : status::validation_failed;
        }
    }
}

#endif // Linux native-thread, C++ exceptions, non-module threaded source profile
