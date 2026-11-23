#pragma once
namespace uwvm2::runtime::compiler::uwvm_int::threaded
{
    inline bool engine::memtag_operation(instruction const& ins) noexcept
    {
        namespace mt=::uwvm2::extension::memtag;namespace rt=::uwvm2::runtime::memtag;
        auto const* descriptor{mt::find_instruction(ins.opcode-0x400u)};
        if(!descriptor || descriptor->operation==mt::opcode::add) { return fail(status::invariant_failure); }
        ::std::array<::std::uint64_t,4> arguments{};
        for(::std::size_t i{descriptor->operand_count};i!=0u;--i)
        {if(!pop(arguments[i-1u]))return false;}
        auto& memory=*modules_[calls_[call_top_-1u].module].memories[ins.target].memory;
        return memory_geometry(memory,[&]<typename Address,bool Shared>() noexcept
        {
            ::std::array<Address,4> converted{};
            for(::std::size_t i{};i<descriptor->operand_count;++i)converted[i]=static_cast<Address>(arguments[i]);
            ::std::span<Address const> values{converted.data(),descriptor->operand_count};
            auto const result=[&]() noexcept
            {
                if(execute_memtag_) { return rt::execute<true,Address,Shared>(memory,descriptor->operation,values); }
                if(has_tag_storage_) { return rt::execute<false,Address,Shared>(memory,descriptor->operation,values); }
                return rt::execute_untagged<Address,Shared>(memory,*descriptor,values);
            }();
            if(!result)
            {
                if(result.operation.error==mt::operation_error::memory_access)
                { return access_failure(result.operation.access,arguments[0]); }
                if(mt::is_allocation_rejection(result.operation.error)) { return fail(status::trap_allocation); }
                return fail(result.error==rt::bridge_error::entropy_unavailable?status::host_failure:status::invariant_failure);
            }
            return descriptor->result_count==0u || push(result.operation.value);
        });
    }
}
