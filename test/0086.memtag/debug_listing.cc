#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/debugger/wasm_listing.h>
#include <uwvm2/uwvm/debugger/wasm_events.h>
#include <array>
#include <cstdio>
#include <cstdlib>
int main()
{
 namespace mt=uwvm2::extension::memtag;namespace listing=uwvm2::uwvm::debugger::wasm_listing;
 namespace events=uwvm2::uwvm::debugger::wasm_events;
 unsigned checks{};auto check=[&](bool value) { ++checks;if(!value) { std::abort(); } };
 for(auto const& descriptor:mt::instructions)
 {
  std::array<std::byte,4> bytes{std::byte{0xfc},static_cast<std::byte>(descriptor.operation),std::byte{0xac},std::byte{2}};
  auto name{listing::mnemonic(0xfc,static_cast<unsigned>(descriptor.operation))};
  check(!name.empty() && name.subview(0,7)=="memtag.");
  check(listing::instruction(bytes)==fast_io::concat_fast_io(name," 300"));
  auto decoded=events::decode(bytes,0);check(decoded.available && decoded.prefixed && (decoded.categories&static_cast<std::uint32_t>(events::category::memory_instruction))!=0);
  auto truncated=std::span(bytes).first(2);check(listing::instruction(truncated).find("bytes:")!=fast_io::containers::npos);
 }
 check(listing::mnemonic(0xfc,0x30)=="extended-opcode");
 std::array<std::byte,7> overflow{std::byte{0xfc},std::byte{0x27},std::byte{0xff},std::byte{0xff},std::byte{0xff},std::byte{0xff},std::byte{0x10}};
 check(listing::instruction(overflow).find("bytes:")!=fast_io::containers::npos);
 std::printf("memtag Wasm listings: %u checks passed\n",checks);
}
