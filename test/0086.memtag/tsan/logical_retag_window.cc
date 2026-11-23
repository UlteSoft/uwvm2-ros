// Complete-operation regression: a concurrent retag must wait until the
// already checked byte effect finishes. This file formerly demonstrated the
// old raw owner-view write bypass; success now means that bypass is closed.
#ifndef UWVM_TEST
# define UWVM_TEST
#endif
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/memtag/impl.h>
#include <atomic>
#include <chrono>
#include <thread>
#include <cstdio>
#include <cstdlib>
namespace mt=uwvm2::extension::memtag;
namespace rt=uwvm2::runtime::memtag;
namespace linear=uwvm2::object::memory::linear;
#define CHECK(...) do {if(!(__VA_ARGS__)) {std::fprintf(stderr,"FAIL %u\n",__LINE__);std::abort();}}while(false)
template<typename Address> void run(bool through_instruction) {
 linear::mmap_memory_t owner;
 if constexpr(sizeof(Address)==8)owner.status=linear::mmap_memory_status_t::wasm64;
 owner.init_by_page_count(1,2);
 constexpr auto width=sizeof(Address)==4?mt::address_width::i32:mt::address_width::i64;
 CHECK(owner.enable_memtag(width,true)==mt::storage_error::none);
 owner.memory_begin[0]=std::byte{42};
 using view=mt::shadow_view<Address,true>;
 auto const borrowed=owner.memtag_shadow.view<Address,true>(65536);
 CHECK(borrowed.store(view::insert(0,6),16));
 std::atomic<unsigned> phase{};
 std::thread retagger([&]{
  while(phase.load(std::memory_order_acquire)!=1)std::this_thread::yield();
  if(through_instruction) {
   Address const args[]{view::insert(0,7),16};
   CHECK(rt::execute<true,Address,true>(owner,mt::opcode::store,args));
  } else { CHECK(borrowed.store(view::insert(0,7),16)); }
  phase.store(2,std::memory_order_release);
 });
 auto result=rt::with_checked_access<Address,true>(owner,view::insert(0,6),0,1,[&](std::span<std::byte> data) noexcept {
  phase.store(1,std::memory_order_release);
  auto const deadline=std::chrono::steady_clock::now()+std::chrono::seconds(10);
  auto const pending=uwvm2::utils::mutex::rwlock_pending_mask();
  // Observe a real pending writer, not a sleep-based scheduling assumption.
  while((owner.memtag_shadow.shared_tag_state_for_test()&pending)==0) {
   CHECK(phase.load(std::memory_order_acquire)==1);
   CHECK(std::chrono::steady_clock::now()<deadline);
   std::this_thread::yield();
  }
  CHECK(phase.load(std::memory_order_acquire)==1);
  __atomic_store_n(reinterpret_cast<unsigned char*>(data.data()),99,__ATOMIC_RELAXED);
 });
 retagger.join();
 CHECK(result&&phase.load(std::memory_order_acquire)==2&&owner.memory_begin[0]==std::byte{99});
 bool called{};
 CHECK(!rt::with_checked_access<Address,true>(owner,view::insert(0,6),0,1,[&](std::span<std::byte>) noexcept {called=true;}));
 CHECK(!called);
 CHECK(rt::with_checked_access<Address,true>(owner,view::insert(0,7),0,1,[&](std::span<std::byte> data) noexcept {
  CHECK(data[0]==std::byte{99});called=true;
 }));
 CHECK(called);called=false;
 CHECK(!rt::with_checked_access<Address,true>(owner,view::insert(65535,7),0,2,[&](std::span<std::byte>) noexcept {called=true;}));
 CHECK(!called);
 // Failed tag writes must not partly retag the existing prefix.
 CHECK(!borrowed.store(view::insert(65535,9),2));
 CHECK(borrowed.load(65535).allocation_tag==0);
 // The instruction's enclosing exclusive lease must also include zeroing,
 // without recursively acquiring the owner-view write gate.
 Address const zero[]{view::insert(0,8),16};
 CHECK(rt::execute<true,Address,true>(owner,mt::opcode::storez,zero));
 CHECK(borrowed.load(0).allocation_tag==8);
 CHECK(owner.memory_begin[0]==std::byte{});
}
int main(){
 run<std::uint32_t>(false);run<std::uint32_t>(true);
 run<std::uint64_t>(false);run<std::uint64_t>(true);
 std::puts("PASS owner-view and instruction retag serialized with complete access; stale accesses rejected; wasm32/64; no partial writes");
}
#include <uwvm2/utils/macro/pop_macros.h>
