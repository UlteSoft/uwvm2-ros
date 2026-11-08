#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/memtag/impl.h>
#include <atomic>
#include <thread>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <sys/wait.h>
#include <unistd.h>
namespace mt=uwvm2::extension::memtag;
namespace rt=uwvm2::runtime::memtag;
namespace hw=mt::hardware_mte;
namespace linear=uwvm2::object::memory::linear;
#define CHECK(...) do {if(!(__VA_ARGS__)) {std::fprintf(stderr,"FAIL %u\n",__LINE__);std::abort();}}while(false)
#if defined(__aarch64__)
static std::byte* base;
static void fault(int signal,siginfo_t* info,void*) {
 auto* correct=hw::tagged_pointer(base,7);unsigned value;
 asm volatile("ldrb %w0, [%1]":"=r"(value):"r"(correct):"memory");
 _exit(signal==SIGSEGV&&info->si_code==9&&value==42?0:90);
}
#endif
void run(bool hardware) {
 linear::mmap_memory_t owner;owner.init_by_page_count(1);
 CHECK(owner.enable_memtag(mt::address_width::i32,true)==mt::storage_error::none);
 owner.memory_begin[0]=std::byte{42};
 using view=mt::shadow_view<std::uint32_t,true>;
 CHECK(owner.memtag_shadow.view<std::uint32_t,true>(65536).store(view::insert(0,6),16));
 if(hardware) {
  CHECK(rt::activate_owner_hardware(owner));
#if defined(__aarch64__)
  base=owner.memory_begin;struct sigaction action{};action.sa_sigaction=fault;action.sa_flags=SA_SIGINFO;
  sigemptyset(&action.sa_mask);CHECK(sigaction(SIGSEGV,&action,nullptr)==0);
#endif
 }
 std::atomic<unsigned> phase{};
 std::thread retagger([&]{
  if(hardware)CHECK(rt::prepare_hardware_thread());
  while(phase.load(std::memory_order_acquire)!=1)std::this_thread::yield();
  CHECK(owner.memtag_shadow.view<std::uint32_t,true>(65536).store(view::insert(0,7),16));
  phase.store(2,std::memory_order_release);
 });
 auto result=rt::with_checked_access<std::uint32_t,true>(owner,view::insert(0,6),0,1,[&](std::span<std::byte> data) noexcept {
  phase.store(1,std::memory_order_release);
  while(phase.load(std::memory_order_acquire)!=2)std::this_thread::yield();
  // The old precheck succeeded; the allocation tag is now different.
  __atomic_store_n(reinterpret_cast<unsigned char*>(data.data()),99,__ATOMIC_RELAXED);
 });
 retagger.join();
 CHECK(!hardware&&result&&owner.memory_begin[0]==std::byte{99});
 CHECK(!rt::check_access<std::uint32_t,true>(owner,view::insert(0,6),0,1));
 bool called{};
 CHECK(!rt::with_checked_access<std::uint32_t,true>(owner,view::insert(65535,7),0,2,[&](std::span<std::byte>) noexcept {called=true;}));
 CHECK(!called);
}
int main(){
 run(false);
 std::puts("KNOWN_LOGICAL_PROTECTION_FAILURE checked_tag=6 retired_tag=7 before=42 after=99 next_access=rejected");
 std::puts("An exit status of zero means the logical failure was reproduced, not that memory protection passed.");
}
#include <uwvm2/utils/macro/pop_macros.h>
