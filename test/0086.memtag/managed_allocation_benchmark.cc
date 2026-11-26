// Reference workload for managed_allocation_benchmark.py. Keep every operation
// checked and use the same process entropy and owner API as the guest callbacks.
#include <charconv>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <uwvm2/runtime/memtag/impl.h>
namespace mt=uwvm2::extension::memtag;
namespace rt=uwvm2::runtime::memtag;
using Addr=std::uint32_t;

int main(int argc,char** argv)
{
    if(argc!=2) { return 2; }
    unsigned iterations{};
    auto const end=argv[1]+std::strlen(argv[1]);
    auto const parsed=std::from_chars(argv[1],end,iterations);
    if(parsed.ec!=std::errc{} || parsed.ptr!=end || iterations==0 || iterations>(1u<<26)) { return 2; }
    uwvm2::object::memory::linear::native_memory_t memory;
    memory.init_by_page_count(1);
    if(memory.enable_memtag(mt::address_width::i32,false,4u,mt::owner_backend::software)!=mt::storage_error::none) { return 1; }
    auto run=[&](mt::opcode operation,Addr pointer)
    {
        const Addr args[]{pointer,16};
        return rt::execute<true,Addr,false>(memory,operation,args);
    };
    auto before=run(mt::opcode::claim,0), after=run(mt::opcode::claim,32);
    if(!before || !after) { return 1; }
    for(unsigned i{};i!=iterations;++i)
    {
        auto result=run(mt::opcode::claim,16);
        if(!result || !run(mt::opcode::release,result.operation.value)) { return 1; }
    }
    return run(mt::opcode::release,before.operation.value) && run(mt::opcode::release,after.operation.value) ? 0 : 1;
}
