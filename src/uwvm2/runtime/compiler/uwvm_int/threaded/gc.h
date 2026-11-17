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
        if(sub>=20u && sub<=25u)
        {
            auto const raw=values_[base];
            auto const heap=::std::bit_cast<::std::int64_t>(ins.immediate);
            bool matches{};
            if(heap==static_cast<::std::int64_t>(core::abstract_heap_type::exn)||
               heap==static_cast<::std::int64_t>(core::abstract_heap_type::noexn))
            {
                reference ref{};::std::memcpy(&ref,&raw,sizeof(ref));
                bool const null=ref.kind==::uwvm2::object::global::wasm_ref_kind::wasm_null||
                    (ref.kind==::uwvm2::object::global::wasm_ref_kind::wasm_exn&&!ref.storage.ptr);
                if(!null&&!exception_token(ref))return fail(status::invariant_failure);
                matches=null?ins.alternate!=0u:heap==static_cast<::std::int64_t>(core::abstract_heap_type::exn);
            }
            else
            {
                auto const* store=world_->module(frame.module)->gc_store.get();
                if(!store || !store->valid())return fail(status::invariant_failure);
                reference ref{};::std::memcpy(&ref,&raw,sizeof(ref));
                matches=store->reference_type_matches(ref,{core::value_kind::reference,{heap},ins.alternate!=0u});
            }
            if(sub<=21u){value_top_=base;return push(static_cast<::std::uint32_t>(matches));}
            if(sub<=23u)return matches||fail(status::trap_cast);
            // Both branch edges preserve the original full reference. The
            // common branch operation moves the complete validated label tuple.
            return (matches==(sub==24u))?branch(ins.target):true;
        }
        else if(sub==26u || sub==27u)
        {
            auto const* store=world_->module(frame.module)->gc_store.get();
            if(!store || !store->valid())return fail(status::invariant_failure);
            reference input{},output{};::std::memcpy(&input,&values_[base],sizeof(input));
            auto const outcome=sub==26u?store->any_convert_extern(input,output):store->extern_convert_any(input,output);
            if(outcome!=storage::gc_object_status::ok)
            {
                if(outcome==storage::gc_object_status::out_of_memory||outcome==storage::gc_object_status::size_overflow)
                    return fail(status::resource_limit);
                return fail(status::invariant_failure);
            }
            result=value::reference(output);
        }
        else if(sub>=28u)
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
