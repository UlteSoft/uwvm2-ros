#define UWVM_TEST
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/memtag/impl.h>
#include <uwvm2/extension/memtag/debug_view.h>
#include <array>
#include <cstdio>
#include <cstdlib>
namespace mt=uwvm2::extension::memtag;
namespace rt=uwvm2::runtime::memtag;
namespace linear=uwvm2::object::memory::linear;
static unsigned checks;
static void check(bool value,unsigned line) { ++checks;if(!value) { std::fprintf(stderr,"debug hardware copy failure %u\n",line);std::abort(); } }
#define CHECK(...) check(static_cast<bool>((__VA_ARGS__)),__LINE__)
template<typename Address,bool Shared> void exercise() {
 constexpr auto width=sizeof(Address)==4?mt::address_width::i32:mt::address_width::i64;
#if defined(UWVM_SUPPORT_MMAP)
 constexpr auto status=sizeof(Address)==4?linear::mmap_memory_status_t::wasm32:linear::mmap_memory_status_t::wasm64;
 linear::mmap_memory_t owner(65536,status);owner.init_by_page_count(1,4);
 CHECK(owner.enable_memtag(width,Shared)==mt::storage_error::none);
 auto& storage=owner.memtag_shadow;auto* base=owner.memory_begin;
#else
 std::array<std::byte,65536> data{};auto* base=data.data();mt::shadow_storage storage;
 CHECK(storage.enable(width,Shared,mt::storage_backend::growable,65536,65536)==mt::storage_error::none);
#endif
 auto view=storage.template view<Address,Shared>(65536);
 for(unsigned n{};n!=16;++n) {
  CHECK(view.store(decltype(view)::insert(n*16u,static_cast<std::uint8_t>(sizeof(Address)==4?n:n+0xa0u)),16));
  for(unsigned b{};b!=16;++b)base[n*16u+b]=std::byte((n*16u+b)&255u);
 }
#if defined(__linux__) && defined(__aarch64__) && defined(UWVM_SUPPORT_MMAP)
 CHECK(mt::hardware_mte::enable_current_thread_sync());CHECK(rt::activate_owner_hardware(owner));
 CHECK(storage.hardware_active());
#endif
 std::array<std::byte,256> out{};std::array<std::uint8_t,16> tags{};
 CHECK(mt::copy_debug_memory(storage,{base,65536},1,out.data(),254));
 for(unsigned n{};n!=254;++n)CHECK(out[n]==std::byte((n+1)&255u));
 CHECK(mt::copy_debug_tags(storage,65536,1,254,tags).count==16);
 for(unsigned n{};n!=16;++n)CHECK(tags[n]==(sizeof(Address)==4?n:n+0xa0u));
 std::array<std::byte,254> replacement{};replacement.fill(std::byte{0x72});
 CHECK(mt::copy_debug_memory(storage,{base,65536},1,replacement.data(),replacement.size(),true));
 CHECK(mt::copy_debug_memory(storage,{base,65536},0,out.data(),out.size()));
 CHECK(out.front()==std::byte{} && out.back()==std::byte{255});
 for(unsigned n=1;n!=255;++n)CHECK(out[n]==std::byte{0x72});
 CHECK(!mt::copy_debug_memory(storage,{base,65536},65535,replacement.data(),2,true));
 CHECK(!mt::copy_debug_memory(storage,{base,128},127,replacement.data(),2,true));
 CHECK(!mt::copy_debug_memory(storage,{base,65536},UINT64_MAX,replacement.data(),1,true));
 CHECK(mt::copy_debug_memory(storage,{base,65536},65536,nullptr,0));
 CHECK(!mt::copy_debug_memory(storage,{base,65536},0,nullptr,1));
 std::uint64_t canonical{};auto raw=decltype(view)::insert(17,9);
 CHECK(mt::debug_canonical_address(storage,raw,canonical) && canonical==17);
 if constexpr(sizeof(Address)==4)CHECK(!mt::debug_canonical_address(storage,UINT64_C(0x100000000),canonical));
 CHECK(mt::copy_debug_tags(storage,65536,0,256,tags).count==16);
 for(unsigned n{};n!=16;++n)CHECK(tags[n]==(sizeof(Address)==4?n:n+0xa0u));
}
int main() {
 mt::select_backend(true);

 exercise<std::uint32_t,false>();exercise<std::uint32_t,true>();exercise<std::uint64_t,false>();exercise<std::uint64_t,true>();
 mt::shadow_storage disabled;std::array<std::byte,4> memory{std::byte{7}},out{};
 CHECK(mt::copy_debug_memory(disabled,memory,0,out.data(),out.size()) && out[0]==std::byte{7});
 std::uint64_t raw=UINT64_C(0xf0000010),canonical{};CHECK(mt::debug_canonical_address(disabled,raw,canonical) && canonical==raw);
 std::printf("debug mixed-tag copy: %u checks passed\n",checks);
}
