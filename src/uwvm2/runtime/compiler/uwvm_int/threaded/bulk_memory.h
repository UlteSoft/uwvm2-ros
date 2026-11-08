#pragma once
namespace uwvm2::runtime::compiler::uwvm_int::threaded
{
    inline bool engine::bulk_memory(threads::ThreadedGroup::ChildLease& lease,instruction const& ins) noexcept
    {
        auto const module{calls_[call_top_-1uz].module};auto& bound{modules_[module]};
        auto const* instance{bound.program.instance};auto const sub{ins.opcode-0x300u};
        if(sub==9u)
        {
            if(ins.immediate>=instance->local_defined_data_vec_storage.size()) { return fail(status::invariant_failure); }
            // This flag belongs to this actual World. Dropping never changes
            // immutable payload pointers or releases bytes borrowed by another
            // activation. Parent/child worlds have distinct drop flags.
            auto& data{const_cast<storage::wasm_data_storage_t&>(instance->local_defined_data_vec_storage.index_unchecked(ins.immediate).data)};
            storage::drop_wasm_data_segment_payload(data);return true;
        }
        if(sub!=8u && sub!=10u && sub!=11u) { return fail(status::invariant_failure); }
        ::std::uint64_t size{},source{},destination{};
        if(!pop(size) || !pop(source) || !pop(destination)) { return false; }
        auto const n{static_cast<::std::size_t>(static_cast<::std::uint32_t>(size))};
        auto const dst{static_cast<::std::uint32_t>(destination)};
        auto const src{static_cast<::std::uint32_t>(source)};
        ::std::byte const* payload{};
        if(sub==8u)
        {
            if(ins.immediate>=instance->local_defined_data_vec_storage.size()) { return fail(status::invariant_failure); }
            auto const data{storage::load_wasm_data_segment_payload(instance->local_defined_data_vec_storage.index_unchecked(ins.immediate).data)};
            auto const extent{data.byte_begin==data.byte_end?0uz:static_cast<::std::size_t>(data.byte_end-data.byte_begin)};
            if(src>extent || n>extent-src) { return fail(status::trap_memory); }
            if(n) { payload=data.byte_begin+src; }
        }
        using byte_type=typename ::uwvm2::runtime::compiler::shared::wasm_threads::atomic_access_traits<1>::type;
        bool cancelled{};
        auto copy{[&](::std::span<::std::byte> destination_bytes,::std::span<::std::byte> source_bytes) noexcept
        {
            using view=::uwvm2::extension::memtag::shadow_view<::std::uint32_t,true>;
            auto const destination_offset{bound.memory->memtag_shadow.enabled()?view::untag(dst):dst};
            auto const source_offset{bound.memory->memtag_shadow.enabled()?view::untag(src):src};
            bool const backwards{sub==10u && destination_offset>source_offset && destination_offset-source_offset<n};
            for(::std::size_t i{};i<n;++i)
            {
                // A cancelled bulk instruction may have an already-written
                // prefix/suffix. The whole Group stops and requires reset.
                if((i&1023uz)==0u && lease.cancellation().stop_requested()) { cancelled=true;return; }
                auto const at{backwards?n-1uz-i:i};
                byte_type value{};
                if(sub==8u) { value=static_cast<byte_type>(payload[at]); }
                else if(sub==10u) { value=__atomic_load_n(reinterpret_cast<byte_type*>(source_bytes.data()+at),__ATOMIC_RELAXED); }
                else { value=static_cast<byte_type>(source); }
                __atomic_store_n(reinterpret_cast<byte_type*>(destination_bytes.data()+at),value,__ATOMIC_RELAXED);
            }
        }};
        // Both full ranges are checked before any byte is copied, including
        // zero-length end-of-memory cases. No native pointer outlives the pin.
        if(sub==10u)
        {
            auto const checked{::uwvm2::runtime::memtag::with_checked_copy<::std::uint32_t,::std::uint32_t>(
                *bound.memory,*bound.memory,dst,src,n,copy)};
            if(!checked) { return access_failure(checked.access,checked.source_failed?src:dst); }
        }
        else if(!memory_span(*bound.memory,dst,0u,n,[&](::std::span<::std::byte> destination_bytes) noexcept
            { copy(destination_bytes,{}); })) { return false; }
        return (!cancelled && !lease.cancellation().stop_requested()) || fail(status::cancelled);
    }

    template<typename Memory>
    inline bool engine::grow_memory(threads::ThreadedGroup::ChildLease& lease,Memory& memory,::std::size_t maximum) noexcept
    {
        namespace linear=::uwvm2::object::memory::linear;using phase=linear::threaded_grow_status;
        ::std::uint64_t delta{};if(!pop(delta)) { return false; }
        auto record_failure{[&](phase why,bool tainted) noexcept
        {
            lease.report_memory_failure(static_cast<::std::uint32_t>(why),tainted,result_.module,result_.function,result_.wasm_offset);
            return fail(status::memory_failure);
        }};
        // A real launch permanently closes old-engine reuse. Do not use a
        // synthetic readiness flag to authorize native memory mutation.
        if(!lease.full_source_reset_required()) { return record_failure(phase::invalid,false); }
        if constexpr(requires(Memory& actual) { actual.grow_threaded_profile(::std::size_t{},::std::size_t{},::std::stop_token{}); })
        {
            last_grow_=memory.grow_threaded_profile(static_cast<::std::uint32_t>(delta),maximum,lease.cancellation());
            switch(last_grow_.status)
            {
                case phase::success:
                    if(lease.cancellation().stop_requested()) { return fail(status::cancelled); }
                    return push(static_cast<::std::uint32_t>(last_grow_.old_pages));
                case phase::limit:case phase::commit_failed:
                    // commit_failed is only produced after actual PROT_NONE
                    // rollback succeeded and logical length stayed unchanged.
                    if(lease.cancellation().stop_requested()) { return fail(status::cancelled); }
                    return push(UINT32_MAX);
                case phase::cancelled:
                    return lease.cancellation().stop_requested()?fail(status::cancelled):record_failure(phase::invalid,false);
                case phase::rollback_failed:return record_failure(phase::rollback_failed,true);
                case phase::invalid:return record_failure(phase::invalid,last_grow_.tainted);
            }
        }
        return record_failure(phase::invalid,false);
    }
}
