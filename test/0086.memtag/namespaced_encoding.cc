#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/extension/memtag/encoding.h>
#include <uwvm2/extension/memtag/usage.h>
#include <uwvm2/uwvm/debugger/wasm_listing.h>
#include <uwvm2/uwvm/debugger/wasm_events.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <vector>

namespace mt = uwvm2::extension::memtag;
namespace listing = uwvm2::uwvm::debugger::wasm_listing;
namespace events = uwvm2::uwvm::debugger::wasm_events;
using bytes = std::vector<std::byte>;
static unsigned checks{};
static void require(bool value)
{ ++checks; if(!value) { std::fprintf(stderr, "encoding check %u failed\n", checks); std::abort(); } }

static void leb(bytes& out, std::uint32_t value, bool padded = false)
{
    for(unsigned index{};; ++index)
    {
        auto byte{value & 127u}; value >>= 7u;
        bool const more{value != 0u || (padded && index != 4u)};
        out.push_back(static_cast<std::byte>(byte | (more ? 128u : 0u)));
        if(!more) { return; }
    }
}
static bytes header(std::uint32_t operation, bool padded = false,
    std::uint32_t domain = 1u, std::uint32_t version = 1u)
{
    bytes out{std::byte{0xff}};
    leb(out, (version << 24u) | (domain << 16u) | operation, padded);
    return out;
}
static void rejected(bytes const& input, mt::encoding_error error)
{
    auto const* first{input.data() + 1u}; auto const* cursor{first};
    auto const result{mt::scan_opcode(cursor, input.data() + input.size())};
    require(result.error == error && cursor == first);
    require(!events::decode(input, 0u).available);
    require(listing::instruction(input) == "invalid memtag namespace");
}
int main()
{
    for(auto const& instruction : mt::instructions)
    for(bool padded : {false, true})
    {
        auto wire{header(static_cast<std::uint32_t>(instruction.operation), padded)};
        auto const header_size{wire.size()};
        for(std::size_t length{1u}; length < header_size; ++length)
        {
            bytes truncated(wire.begin(), wire.begin() + length);
            rejected(truncated, mt::encoding_error::malformed_integer);
        }
        leb(wire, 300u, padded);
        if(instruction.operation == mt::opcode::add)
        { rejected(wire, mt::encoding_error::reserved_operation); continue; }
        auto const* cursor{wire.data() + 1u};
        auto const result{mt::scan_opcode(cursor, wire.data() + wire.size())};
        require(result.error == mt::encoding_error::none && result.subopcode == static_cast<unsigned>(instruction.operation));
        require(cursor == wire.data() + header_size); // memoryidx has NOT been consumed.
        require(mt::may_contain_memtag(wire));
        auto const event{events::decode(wire, 0u)};
        require(event.available && event.prefixed && event.primary == 0xffu && event.extended == result.subopcode);
        require(event.categories == static_cast<unsigned>(events::category::memory_instruction));
        auto const name{listing::mnemonic(0xff, result.subopcode)};
        require(listing::instruction(wire) == fast_io::concat_fast_io("uwvm.linear.v1::", name, " 300"));
    }
    // Every version and domain is independent: no low-byte masking accepts an
    // unknown namespace. Domain 2 is reserved, not a claimed GC implementation.
    for(unsigned value{}; value != 256u; ++value)
    {
        if(value != 1u)
        {
            rejected(header(0x20u, false, 1u, value), mt::encoding_error::unsupported_version);
            rejected(header(0x20u, false, value), mt::encoding_error::unsupported_domain);
        }
    }
    // Independent complete operation-space oracle, including reserved add.
    for(unsigned operation{}; operation != 65536u; ++operation)
    {
        auto wire{header(operation)};
        auto const* first{wire.data()+1u};auto const* cursor{first};
        auto const result{mt::scan_opcode(cursor,wire.data()+wire.size())};
        bool const defined{(operation >= 0x20u && operation <= 0x2fu) ||
                           (operation >= 0x36u && operation <= 0x38u)};
        require((result.error == mt::encoding_error::none) == defined);
        require(defined ? result.subopcode == operation && cursor == wire.data()+wire.size() : cursor == first);
        if(operation == 0x33u) { require(result.error == mt::encoding_error::reserved_operation); }
    }
    bytes overflow{std::byte{0xff},std::byte{0x80},std::byte{0x80},std::byte{0x80},std::byte{0x80},std::byte{0x10}};
    rejected(overflow, mt::encoding_error::malformed_integer);
    overflow.back()=std::byte{0x80};overflow.push_back(std::byte{0});
    rejected(overflow, mt::encoding_error::malformed_integer);
    bytes old_alias{std::byte{0xff}};
    for(auto value:{0x554c5445u,1u,1u,0x20u,0u}) { leb(old_alias,value); }
    rejected(old_alias, mt::encoding_error::unsupported_version);
    for(auto const& descriptor:mt::instructions)
    {
        bytes old_fc{std::byte{0xfc}};leb(old_fc,static_cast<unsigned>(descriptor.operation));leb(old_fc,0u);
        require(mt::may_contain_memtag(old_fc)); // Force rejection even in an unused function.
        require(!events::decode(old_fc,0u).available);
        require(listing::mnemonic(0xfc,static_cast<unsigned>(descriptor.operation)) == "extended-opcode");
    }
    bytes standard_fc{std::byte{0xfc},std::byte{0x10},std::byte{0}};
    require(listing::mnemonic(0xfc,0x10) == "table.size");
    require(events::decode(standard_fc,0u).categories == static_cast<unsigned>(events::category::table_instruction));
    std::printf("PASS FF packed-u32 MTE decoding, all versions/domains/operations, rollback and debug mappings: %u checks\n", checks);
}
