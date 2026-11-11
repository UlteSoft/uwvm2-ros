#pragma once
namespace uwvm2::runtime::compiler::uwvm_int::threaded
{
    inline bool engine::gc_operation(instruction const& ins) noexcept
    {
        using value=storage::gc_object_value;using reference=storage::gc_reference;
        auto const sub=ins.opcode-0x800u;
        auto const& frame=calls_[call_top_-1u];
        if(value_top_<frame.base || ins.parameters>value_top_-frame.base) { return fail(status::invariant_failure); }
        auto const base=value_top_-ins.parameters;
        if(ins.results>values_.size()-base || ins.parameters>gc_scratch_.size()) { return fail(status::resource_limit); }
        value result{};
        if(sub>=28u)
        {
            if(sub==28u) { result=value::reference(::uwvm2::object::global::make_wasm_i31_reference(static_cast<::std::int32_t>(values_[base].low))); }
            else
            {
                reference ref{};::std::memcpy(&ref,&values_[base],sizeof(ref));
                if(ref.kind!=::uwvm2::object::global::wasm_ref_kind::wasm_i31) { return fail(status::trap_null); }
                result=value::i32(sub==29u ? static_cast<::std::uint32_t>(ref.storage.wasm_i31.get_s()) : ref.storage.wasm_i31.get_u());
            }
        }
        else
        {
            auto const count=ins.parameters;
            auto* scratch=gc_scratch_.data();
            for(::std::size_t i{};i<count;++i) { scratch[i]=::std::bit_cast<value>(values_[base+i]); }
            auto const outcome=::uwvm2::runtime::compiler::shared::gc_aggregate(
                *world_->module(frame.module),sub,static_cast<::std::uint32_t>(ins.immediate),static_cast<::std::uint32_t>(ins.target),scratch,count,result);
            for(::std::size_t i{};i<count;++i) { scratch[i]=value{}; }
            if(outcome!=storage::gc_object_status::ok)
            {
                if(outcome==storage::gc_object_status::null_reference) { return fail(status::trap_null); }
                if(outcome==storage::gc_object_status::out_of_memory || outcome==storage::gc_object_status::size_overflow) { return fail(status::resource_limit); }
                return fail(status::trap_table);
            }
        }
        value_top_=base;
        return !ins.results || push(::std::bit_cast<stack_value>(result));
    }
}
