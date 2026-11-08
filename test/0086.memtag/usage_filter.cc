#include <uwvm2/extension/memtag/usage.h>
#include <array>
#include <cstdio>
#include <cstdlib>
namespace mt=uwvm2::extension::memtag;
static unsigned count{};
static void check(bool v){++count;if(!v)std::abort();}
int main()
{
    for(auto const& instruction:mt::instructions)
    for(unsigned length=1;length<=5;++length)
    {
        std::array<std::byte,8> bytes{};bytes[1]=std::byte{0xfc};
        for(unsigned i=0;i<length;++i)
        { bytes[2+i]=std::byte{static_cast<unsigned char>((i?0u:static_cast<unsigned>(instruction.operation))|(i+1<length?0x80u:0u))}; }
        check(mt::may_contain_memtag(bytes));
        check(mt::may_contain_memtag(std::span{bytes}.subspan(1,length+1)));
        if(length>1)check(!mt::may_contain_memtag(std::span{bytes}.subspan(1,length)));
    }
    std::array<std::byte,4> absent{std::byte{0xfc},std::byte{0x10},std::byte{0x20},std::byte{0xfc}};
    check(!mt::may_contain_memtag(absent));check(!mt::may_contain_memtag({}));
    // A matching byte sequence inside an immediate is intentionally a false
    // positive; this filter never grants a memory-enable permit.
    std::array<std::byte,4> possible{std::byte{0x41},std::byte{0xfc},std::byte{0x20},std::byte{0x0b}};
    check(mt::may_contain_memtag(possible));
    std::printf("PASS %u conservative opcode-filter assertions\n",count);
}
