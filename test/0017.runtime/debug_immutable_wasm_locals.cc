// Display-only immutable-local classification on complete owned opcode images.
// This helper grants no memory, code, pause or native address authority.
#include <uwvm2/uwvm/debugger/source_dwarf_expression.h>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cstdint>
#include <limits>
namespace d = ::uwvm2::uwvm::debugger::source_dwarf;
static void require(bool condition, char const* why)
{
    if(!condition) { ::std::fprintf(stderr, "immutable locals: %s\n", why); ::std::abort(); }
}
int main()
{
    using status = d::immutable_local_status;
    // A body longer than the former display limit; every nop is a boundary.
    ::std::vector<::std::byte> body(131072u, ::std::byte{0x01});
    ::std::vector<::std::uint_least8_t> bitmap(body.size()/8u, 0xffu);
    auto append = [&](unsigned value, bool instruction)
    {
        auto const offset{body.size()};
        body.push_back(static_cast<::std::byte>(value));
        if(bitmap.size() <= offset/8u) { bitmap.push_back(0u); }
        if(instruction) { bitmap[offset/8u] |= static_cast<::std::uint_least8_t>(1u << (offset%8u)); }
    };
    // i32.const 33; drop. The immediate 0x21 must never be a local.set.
    append(0x41u,true); append(0x21u,false); append(0x1au,true);
    append(0x0bu,true);
    require(d::immutable_wasm_local(body, bitmap, 300u) == status::intact, "large body and local 300 remain inspectable");
    require(d::immutable_wasm_local(body, bitmap, 65536u) == status::intact, "large valid local index remains inspectable");
    body.pop_back(); bitmap.back() &= static_cast<::std::uint_least8_t>(~(1u << (body.size()%8u)));
    // i32.const 0; local.set 300; end, with the exact two-byte local-index LEB.
    append(0x41u,true); append(0u,false); append(0x21u,true);
    append(0xacu,false); append(0x02u,false); append(0x0bu,true);
    require(d::immutable_wasm_local(body, bitmap, 300u) == status::overwritten, "high-index local.set is detected past 64 KiB");
    require(d::immutable_wasm_local(body, bitmap, 301u) == status::intact, "other local remains intact");
    require(d::immutable_wasm_local(body, bitmap, ::std::uint64_t{UINT32_MAX}+1u) == status::unavailable,
        "outside real Wasm local-index domain rejected");
    auto incomplete{bitmap}; incomplete.pop_back();
    require(d::immutable_wasm_local(body, incomplete, 301u) == status::unavailable, "incomplete source bitmap rejected");
    auto forged{bitmap}; auto const immediate{body.size()-3u};
    forged[immediate/8u] |= static_cast<::std::uint_least8_t>(1u << (immediate%8u));
    require(d::immutable_wasm_local(body, forged, 301u) == status::unavailable, "instruction boundary inside local LEB rejected");
    auto tail{bitmap}; tail.back() |= 0x80u;
    require(d::immutable_wasm_local(body, tail, 301u) == status::unavailable, "nonzero unused bitmap bits rejected");
    ::std::puts("immutable_wasm_locals: PASS large bodies, u32 local indices and boundary rejection");
}
