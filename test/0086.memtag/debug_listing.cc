#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/debugger/wasm_listing.h>
#include <array>
#include <cstdio>
#include <cstdlib>
namespace listing=uwvm2::uwvm::debugger::wasm_listing;
namespace mt=uwvm2::extension::memtag;
static unsigned count{};
static void check(bool value){++count;if(!value)std::abort();}
int main()
{
 for(auto const& descriptor:mt::instructions)
 {
  if(descriptor.operation==mt::opcode::add)continue;
  // 0x010100xx has exactly four canonical ULEB32 bytes. Memory index is 300.
  std::array<std::byte,7> bytes{std::byte{0xff},static_cast<std::byte>(static_cast<unsigned>(descriptor.operation)|0x80u),
    std::byte{0x80},std::byte{0x84},std::byte{0x08},std::byte{0xac},std::byte{2}};
  auto name{listing::mnemonic(0xff,static_cast<unsigned>(descriptor.operation))};
  check(listing::instruction(bytes)==fast_io::concat_fast_io("uwvm.linear.v1::",name," 300"));
  check(listing::instruction(std::span{bytes}.first(6))!=fast_io::concat_fast_io("uwvm.linear.v1::",name," 300"));
 }
 check(listing::mnemonic(0xfc,0x20)=="extended-opcode");
 std::array<std::byte,10> overflow{std::byte{0xff},std::byte{0xa7},std::byte{0x80},std::byte{0x84},std::byte{0x08},
   std::byte{0xff},std::byte{0xff},std::byte{0xff},std::byte{0xff},std::byte{0x10}};
 check(listing::instruction(overflow)!=fast_io::concat_fast_io("uwvm.linear.v1::memtag.load 0"));
 std::printf("PASS %u MTE listing checks\n",count);
}
