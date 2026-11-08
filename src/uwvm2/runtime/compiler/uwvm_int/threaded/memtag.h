#pragma once
namespace uwvm2::runtime::compiler::uwvm_int::threaded
{
    inline bool engine::memtag_operation(instruction const& ins) noexcept
    {
        namespace mt=::uwvm2::extension::memtag;namespace rt=::uwvm2::runtime::memtag;
        auto const* descriptor{mt::find_instruction(ins.opcode-0x400u)};
        if(!descriptor || descriptor->operation==mt::opcode::add || ins.target!=0u) { return fail(status::invariant_failure); }
        ::std::array<::std::uint32_t,4> arguments{};
        for(::std::size_t i{descriptor->operand_count};i!=0u;--i)
        { ::std::uint64_t value{};if(!pop(value)) { return false; }arguments[i-1u]=static_cast<::std::uint32_t>(value); }
        auto& memory{*modules_[calls_[call_top_-1u].module].memory};
        ::std::span<::std::uint32_t const> values{arguments.data(),descriptor->operand_count};
        auto const result{execute_memtag_?rt::execute<true,::std::uint32_t,true>(memory,descriptor->operation,values):
            rt::execute<false,::std::uint32_t,true>(memory,descriptor->operation,values)};
        if(!result)
        {
            if(result.operation.error==mt::operation_error::memory_access)
            { return access_failure(result.operation.access,arguments[0]); }
            return fail(result.error==rt::bridge_error::entropy_unavailable?status::host_failure:status::invariant_failure);
        }
        return descriptor->result_count==0u || push(result.operation.value);
    }
}
