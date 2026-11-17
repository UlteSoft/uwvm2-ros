#pragma once
namespace uwvm2::runtime::compiler::uwvm_int::threaded
{
    inline bool engine::arithmetic(unsigned op) noexcept
    {
        ::std::uint64_t a{},b{},result{};
        bool const wide{(op>=0x50u&&op<=0x5au)||(op>=0x79u&&op<=0x8au)};
        if(op==0x45u || op==0x50u)
        { if(!pop(a)) { return false; }return push(wide?a==0u:static_cast<::std::uint32_t>(a)==0u); }
        if((op>=0x67u&&op<=0x69u)||(op>=0x79u&&op<=0x7bu))
        {
            if(!pop(a)) { return false; }unsigned const which{op-(wide?0x79u:0x67u)};
            if(wide) { result=which==0u ? ::std::countl_zero(a) : which==1u ? ::std::countr_zero(a) : ::std::popcount(a); }
            else { auto const n{static_cast<::std::uint32_t>(a)};result=which==0u ? ::std::countl_zero(n) : which==1u ? ::std::countr_zero(n) : ::std::popcount(n); }
            return push(result);
        }
        if(!pop(b)||!pop(a)) { return false; }
        if((op>=0x46u&&op<=0x4fu)||(op>=0x51u&&op<=0x5au))
        {
            unsigned const which{op-(wide?0x51u:0x46u)};
            auto compare{[&]<typename U>(U x,U y) noexcept
            {
                using S=::std::make_signed_t<U>;auto const sx{::std::bit_cast<S>(x)},sy{::std::bit_cast<S>(y)};
                switch(which) { case 0:return x==y;case 1:return x!=y;case 2:return sx<sy;case 3:return x<y;
                    case 4:return sx>sy;case 5:return x>y;case 6:return sx<=sy;case 7:return x<=y;case 8:return sx>=sy;default:return x>=y; }
            }};
            return push(wide?compare(a,b):compare(static_cast<::std::uint32_t>(a),static_cast<::std::uint32_t>(b)));
        }
        unsigned const which{op-(wide?0x7cu:0x6au)};
        auto compute{[&]<typename U>(U x,U y) noexcept -> bool
        {
            using S=::std::make_signed_t<U>;constexpr unsigned bits{sizeof(U)*8u};
            auto const sx{::std::bit_cast<S>(x)},sy{::std::bit_cast<S>(y)};unsigned const shift{static_cast<unsigned>(y&(bits-1u))};U z{};
            switch(which)
            {
                case 0:z=static_cast<U>(x+y);break;case 1:z=static_cast<U>(x-y);break;case 2:z=static_cast<U>(x*y);break;
                case 3:case 4:case 5:case 6:
                    if(!y) { return fail(status::trap_divide_by_zero); }
                    if((which==3u||which==5u)&&sx==::std::numeric_limits<S>::min()&&sy==S{-1})
                    { if(which==3u) { return fail(status::trap_integer_overflow); }z=0;break; }
                    if(which==3u) { z=::std::bit_cast<U>(static_cast<S>(sx/sy)); }
                    if(which==4u) { z=x/y; }if(which==5u) { z=::std::bit_cast<U>(static_cast<S>(sx%sy)); }if(which==6u) { z=x%y; }break;
                case 7:z=x&y;break;case 8:z=x|y;break;case 9:z=x^y;break;
                case 10:z=static_cast<U>(x<<shift);break;case 11:z=::std::bit_cast<U>(static_cast<S>(sx>>shift));break;
                case 12:z=x>>shift;break;case 13:z=::std::rotl(x,static_cast<int>(shift));break;case 14:z=::std::rotr(x,static_cast<int>(shift));break;
                default:return fail(status::invariant_failure);
            }
            result=z;return true;
        }};
        if(!(wide?compute(a,b):compute(static_cast<::std::uint32_t>(a),static_cast<::std::uint32_t>(b)))) { return false; }
        return push(result);
    }

    inline bool engine::step(threads::ThreadedGroup::ChildLease& lease, instruction const& ins) noexcept
    {
        auto& frame{calls_[call_top_-1uz]};auto& bound{modules_[frame.module]};auto const* instance{bound.program.instance};
        auto const op{ins.opcode};::std::uint64_t a{},c{};
        if(op>=0x10000u) { return ins.simd_handler?ins.simd_handler(*this,ins):fail(status::invariant_failure); }
        if(op>=0x800u) { return gc_operation(ins); }
        if(op>=0x400u) { return memtag_operation(ins); }
        if((op>=0x30cu&&op<=0x311u)||op==0x25u||op==0x26u)
        {
            using operations=::uwvm2::uwvm::runtime::initializer::threaded_table_operations;
            operations table{*world_,frame.module};using result=operations::status;
            auto check=[&](result r){return r==result::ok||fail(r==result::bounds?status::trap_table:status::invariant_failure);};
            auto address=[&](::std::uint64_t bits,::std::size_t index)
            {return storage::runtime_table_is_address64(*instance,index)?bits:static_cast<::std::uint64_t>(static_cast<::std::uint32_t>(bits));};
            auto const index=static_cast<::std::size_t>(ins.immediate);stack_value v{};storage::gc_reference ref{};
            if(op==0x30du)return check(table.drop(index));
            if(op==0x310u)
            {auto const* t=world_->table(frame.module,index);return t?push(t->elems.size()):fail(status::invariant_failure);}
            if(op==0x25u)
            {if(!pop(a)||!check(table.get(index,address(a,index),ref)))return false;::std::memcpy(&v,&ref,sizeof(ref));return push(v);}
            if(op==0x26u)
            {if(!pop(v)||!pop(a))return false;::std::memcpy(&ref,&v,sizeof(ref));return check(table.fill(index,address(a,index),1u,ref));}
            if(op==0x30fu||op==0x311u)
            {
                if(!pop(c)||!pop(v))return false;::std::memcpy(&ref,&v,sizeof(ref));
                if(op==0x30fu)
                {
                    auto const* actual=world_->table(frame.module,index);if(!actual)return fail(status::invariant_failure);
                    auto const n=threads::journal_table_grow(static_cast<::std::uint32_t>(index),address(c,index),actual->elems.size(),
                        [&]() noexcept{return table.grow(index,address(c,index),ref);});return push(address(n,index));
                }
                if(!pop(a))return false;return check(table.fill(index,address(a,index),address(c,index),ref));
            }
            ::std::uint64_t source{};if(!pop(c)||!pop(source)||!pop(a))return false;
            if(op==0x30cu)return check(table.init(ins.target,index,address(a,ins.target),static_cast<::std::uint32_t>(source),static_cast<::std::uint32_t>(c)));
            auto const count=storage::runtime_table_is_address64(*instance,index)&&storage::runtime_table_is_address64(*instance,ins.target)?c:static_cast<::std::uint32_t>(c);
            return check(table.copy(index,ins.target,address(a,index),address(source,ins.target),count));
        }
        if(op>=0x300u) { return bulk_memory(lease,ins); }
        if(op>=0x200u) { return floating(op); }
        if(op==0x103u) { ::std::atomic_thread_fence(::std::memory_order_seq_cst);return true; }
        if(op>=0x100u) { return atomic(ins,*bound.memories[ins.target].memory); }
        if(op>=0x28u&&op<=0x3eu) { return memory_access(ins,*bound.memories[ins.target].memory); }
        if((op>=0x45u&&op<=0x5au)||(op>=0x67u&&op<=0x8au)) { return arithmetic(op); }
        if((op>=0x5bu&&op<=0x66u)||(op>=0x8bu&&op<=0xa6u)||(op>=0xa8u&&op<=0xabu)||(op>=0xaeu&&op<=0xbfu)) { return floating(op); }
        switch(op)
        {
            case 0x00:return fail(status::trap_unreachable);
            case 0x01:return true;
            case 0x02:case 0x03:case 0x04:case 0x1f:
            {
                if(op==0x04u&&!pop(a)) { return false; }
                if(control_top_==controls_.size() || ins.parameters>value_top_-frame.base) { return fail(status::resource_limit); }
                controls_[control_top_++]={value_top_-ins.parameters,frame.pc,ins.target,ins.parameters,ins.results,op==0x03u,op==0x1fu?::std::addressof(ins):nullptr};
                if(op==0x04u && !static_cast<::std::uint32_t>(a)) { frame.pc=ins.alternate; }
                return true;
            }
            case 0x05:frame.pc=ins.target;return true;
            case 0x08:return throw_tag(lease,static_cast<::std::size_t>(ins.immediate));
            case 0x0a:
            {
                stack_value value{};if(!pop(value))return false;
                storage::gc_reference ref{};::std::memcpy(&ref,&value,sizeof(ref));
                if(ref.kind==::uwvm2::object::global::wasm_ref_kind::wasm_null||
                   (ref.kind==::uwvm2::object::global::wasm_ref_kind::wasm_exn&&!ref.storage.ptr))return fail(status::trap_null);
                auto const token=exception_token(ref);return token?raise(lease,token):fail(status::invariant_failure);
            }
            case 0x0b:
            {
                if(control_top_<=frame.controls) { return fail(status::invariant_failure); }
                if(control_top_==frame.controls+1uz) { return leave(); }
                auto const label{controls_[--control_top_]};return compact(label.base,label.results);
            }
            case 0x0c:return branch(static_cast<::std::size_t>(ins.immediate));
            case 0x0d:if(!pop(a)) { return false; }return !static_cast<::std::uint32_t>(a)||branch(static_cast<::std::size_t>(ins.immediate));
            case 0x0e:
                if(!pop(a)||ins.labels.empty()) { return fail(status::invariant_failure); }
                return branch(ins.labels[::std::min(static_cast<::std::size_t>(static_cast<::std::uint32_t>(a)),ins.labels.size()-1uz)]);
            case 0x0f:return leave();
            case 0x10:case 0x12:
            {
                if(ins.immediate>=bound.targets.size()) { return fail(status::invariant_failure); }
                auto const& target{bound.targets[ins.immediate]};
                if(op==0x12u) { return tail_enter(lease,target); }
                return target.host?host_call(lease,target):enter(target);
            }
            case 0x11:case 0x13:
            {
                if(!pop(a)) { return false; }
                using operations=::uwvm2::uwvm::runtime::initializer::threaded_table_operations;
                operations table{*world_,frame.module};storage::gc_reference reference{};
                auto const index=storage::runtime_table_is_address64(*instance,ins.target)?a:static_cast<::std::uint64_t>(static_cast<::std::uint32_t>(a));
                auto const result=table.get(ins.target,index,reference);
                if(result!=operations::status::ok)return fail(result==operations::status::bounds?status::trap_table:status::invariant_failure);
                if(reference.kind==::uwvm2::object::global::wasm_ref_kind::wasm_null)return fail(status::trap_null);
                target callee{};
                if(ins.immediate>=instance->type_section_storage.type_section_count||!reference_target(reference,callee)||
                   !type_matches(frame.module,instance->type_section_storage.type_section_begin+ins.immediate,callee))return fail(status::trap_signature);
                return op==0x13u?tail_enter(lease,callee):callee.host?host_call(lease,callee):enter(callee);
            }
            case 0x14:case 0x15:
            {
                stack_value value{};if(!pop(value))return false;
                storage::gc_reference reference{};::std::memcpy(&reference,&value,sizeof(reference));
                if(reference.kind==::uwvm2::object::global::wasm_ref_kind::wasm_null)return fail(status::trap_null);
                target callee{};
                if(ins.immediate>=instance->type_section_storage.type_section_count||!reference_target(reference,callee)||
                   !type_matches(frame.module,instance->type_section_storage.type_section_begin+ins.immediate,callee))return fail(status::trap_signature);
                return op==0x15u?tail_enter(lease,callee):callee.host?host_call(lease,callee):enter(callee);
            }
            case 0x1a:return pop(a);
            case 0x1b:case 0x1c:
            { stack_value left{},right{};if(!pop(c)||!pop(right)||!pop(left)) { return false; }return push(static_cast<::std::uint32_t>(c)?left:right); }
            case 0x20:case 0x21:case 0x22:
            {
                if(ins.immediate>=frame.base-frame.locals) { return fail(status::invariant_failure); }
                auto& local{values_[frame.locals+ins.immediate]};
                if(op==0x20u) { return push(local); }stack_value value{};if(!pop(value)) { return false; }local=value;return op==0x21u||push(value);
            }
            case 0x23:case 0x24:
            {
                if(ins.immediate>=bound.globals.size()) { return fail(status::invariant_failure); }
                auto* global{bound.globals[ins.immediate]};
                using k=::uwvm2::object::global::global_type;
                if(global->kind==k::wasm_ref)
                {
                    stack_value value{};
                    if(op==0x23u)
                    {::std::memcpy(&value,&global->storage.ref,sizeof(storage::gc_reference));return push(value);}
                    if(!global->is_mutable||!pop(value))return fail(status::invariant_failure);
                    storage::gc_reference reference{};::std::memcpy(&reference,&value,sizeof(reference));
                    auto const* store=static_cast<storage::gc_object_store const*>(global->ref_lease_store);
                    bool owned{};
                    for(::std::size_t m{};m<world_->module_count();++m)
                        if(store && store==world_->module(m)->gc_store.get()){owned=true;break;}
                    if(!owned||
                       storage::uwvm2_gc_retain_reference(store,&reference)!=storage::gc_object_status::ok)
                        return fail(status::invariant_failure);
                    global->storage.ref=reference;return true;
                }
                bool const wide{global->kind==k::wasm_i64 || global->kind==k::wasm_f64};
                void* cell{};
                switch(global->kind)
                {
                    case k::wasm_i32:cell=::std::addressof(global->storage.i32);break;
                    case k::wasm_i64:cell=::std::addressof(global->storage.i64);break;
                    case k::wasm_f32:cell=::std::addressof(global->storage.f32);break;
                    case k::wasm_f64:cell=::std::addressof(global->storage.f64);break;
                    case k::wasm_v128:cell=::std::addressof(global->storage.v128);break;
                    default:return fail(status::invariant_failure);
                }
                auto const width{global->kind==k::wasm_v128?16uz:wide?8uz:4uz};stack_value value{};
                if(op==0x23u) { ::std::memcpy(::std::addressof(value),cell,width);return push(value); }
                if(!global->is_mutable || !pop(value)) { return fail(status::invariant_failure); }
                ::std::memcpy(cell,::std::addressof(value),width);return true;
            }
            case 0x3f:
            {
                ::std::size_t length{};if(!threads::details::access(*bound.memories[ins.immediate].memory,0,0,length,[](::std::byte*) noexcept{})) { return fail(status::trap_memory); }
                return push(length>>16u);
            }
            case 0x40:return grow_memory(lease,*bound.memories[ins.immediate].memory,bound.memories[ins.immediate].maximum);
            case 0x41:return push(static_cast<::std::uint32_t>(ins.immediate));
            case 0x42:case 0x44:return push(ins.immediate);
            case 0x43:return push(static_cast<::std::uint32_t>(ins.immediate));
            case 0xd0:return push(0u);
            case 0xd2:
            {
                storage::gc_reference reference{};using kind=::uwvm2::object::global::wasm_ref_kind;
                auto const imports=instance->imported_function_vec_storage.size();
                if(ins.immediate<imports)
                {reference.kind=kind::wasm_func_imported;reference.storage.ptr=const_cast<storage::imported_function_storage_t*>(&instance->imported_function_vec_storage.index_unchecked(ins.immediate));}
                else
                {
                    auto const local=ins.immediate-imports;if(local>=instance->local_defined_function_vec_storage.size())return fail(status::invariant_failure);
                    reference.kind=kind::wasm_func_defined;reference.storage.ptr=const_cast<storage::local_defined_function_storage_t*>(&instance->local_defined_function_vec_storage.index_unchecked(local));
                }
                stack_value value{};::std::memcpy(&value,&reference,sizeof(reference));return push(value);
            }
            case 0xd3:
            {
                stack_value right{},left{};if(!pop(right)||!pop(left))return false;
                storage::gc_reference lhs{},rhs{};
                ::std::memcpy(&lhs,&left,sizeof(lhs));::std::memcpy(&rhs,&right,sizeof(rhs));
                using kind=::uwvm2::object::global::wasm_ref_kind;
                bool equal=lhs.kind==rhs.kind;
                if(equal)switch(lhs.kind)
                {
                    case kind::wasm_null:break;
                    case kind::wasm_i31:equal=lhs.storage.wasm_i31.get_u()==rhs.storage.wasm_i31.get_u();break;
                    case kind::wasm_struct:case kind::wasm_array:equal=lhs.storage.ptr==rhs.storage.ptr;break;
                    default:return fail(status::invariant_failure); // The validated ref.eq operands must be eqref.
                }
                return push(static_cast<::std::uint32_t>(equal));
            }
            case 0xd1:case 0xd4:case 0xd5:case 0xd6:
            {
                stack_value reference{};if(!pop(reference)) { return false; }
                // GC references carry an out-of-band kind as well as their
                // payload. In particular i31(0) is non-null. Preserve both
                // halves through assertions and reference branches; exception
                // IDs still use their canonical zero-extended scalar carrier.
                bool const nonnull{(reference.low|reference.high)!=0u};
                if(ins.opcode==0xd1u) { return push(!nonnull); }
                if(ins.opcode==0xd4u) { return nonnull?push(reference):fail(status::trap_null); }
                if(ins.opcode==0xd5u) { return nonnull?push(reference):branch(static_cast<::std::size_t>(ins.immediate)); }
                return nonnull?(push(reference)&&branch(static_cast<::std::size_t>(ins.immediate))):true;
            }
            case 0xa7:if(!pop(a)) { return false; }return push(static_cast<::std::uint32_t>(a));
            case 0xac:if(!pop(a)) { return false; }return push(static_cast<::std::uint64_t>(static_cast<::std::int64_t>(::std::bit_cast<::std::int32_t>(static_cast<::std::uint32_t>(a)))));
            case 0xad:if(!pop(a)) { return false; }return push(static_cast<::std::uint32_t>(a));
            case 0xc0:case 0xc1:case 0xc2:case 0xc3:case 0xc4:
            {
                if(!pop(a)) { return false; }unsigned const bits{op==0xc0u||op==0xc2u?8u:op==0xc1u||op==0xc3u?16u:32u};
                a&=(::std::uint64_t{1}<<bits)-1u;if(a&(::std::uint64_t{1}<<(bits-1u))) { a|=(~::std::uint64_t{})<<bits; }
                return push(op<=0xc1u?static_cast<::std::uint32_t>(a):a);
            }
            default:return fail(status::invariant_failure);
        }
    }
}
