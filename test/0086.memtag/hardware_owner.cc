#define UWVM_TEST
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/memtag/impl.h>
#include <uwvm2/runtime/memtag/native_pointer.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <stop_token>
#include <utility>
#if defined(__linux__) && defined(__aarch64__)
#include <csignal>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <unistd.h>
#endif
namespace mt=uwvm2::extension::memtag;
namespace hw=mt::hardware_mte;
namespace rt=uwvm2::runtime::memtag;
namespace linear=uwvm2::object::memory::linear;
static void require(bool okay,unsigned line) {
 if(!okay){std::fprintf(stderr,"hardware owner failed line %u\n",line);std::abort();}
}
#define CHECK(...) require(static_cast<bool>((__VA_ARGS__)),__LINE__)
#if defined(__linux__) && defined(__aarch64__) && defined(UWVM_SUPPORT_MMAP)
static std::byte* correct;
static void fault(int signal,siginfo_t* info,void*) {
 unsigned value{};
 asm volatile("ldrb %w0, [%1]":"=r"(value):"r"(correct):"memory");
 _exit(signal==SIGSEGV && info->si_code==9 && value==42?0:66);
}
static void mismatch_rejected(std::byte* pointer,bool store) {
 auto const child=fork();CHECK(child>=0);
 if(child==0) {
  correct=pointer;
  struct sigaction action{};action.sa_sigaction=fault;action.sa_flags=SA_SIGINFO;sigemptyset(&action.sa_mask);
  if(sigaction(SIGSEGV,&action,nullptr))_exit(67);
  auto* wrong=hw::tagged_pointer(pointer,3);
  if(store)asm volatile("strb %w0, [%1]"::"r"(99u),"r"(wrong):"memory");
  else{unsigned value;asm volatile("ldrb %w0, [%1]":"=r"(value):"r"(wrong):"memory");}
  _exit(68);
 }
 int result{};CHECK(waitpid(child,&result,0)==child);CHECK(WIFEXITED(result)&&WEXITSTATUS(result)==0);
}
template<typename Address,bool Shared>
void exercise() {
 constexpr auto width=sizeof(Address)==4?mt::address_width::i32:mt::address_width::i64;
 constexpr auto status=sizeof(Address)==4?linear::mmap_memory_status_t::wasm32:linear::mmap_memory_status_t::wasm64;
 linear::mmap_memory_t owner(65536,status);owner.init_by_page_count(1,4);
 CHECK(owner.enable_memtag(width,Shared,8,mt::owner_backend::arm_mte)==mt::storage_error::none);
 CHECK(!owner.memtag_shadow.hardware_active());
 CHECK(owner.memtag_shadow.hardware_selected());
 owner.memory_begin[0]=std::byte{42};
 auto view=owner.memtag_shadow.template view<Address,Shared>(65536);
 CHECK(view.tag_start_bit()==sizeof(Address)*8u-8u);
 auto tag=view.with_tag(Address{0xb} << (sizeof(Address)*8u-4u),9);
 CHECK(view.store(tag,32));
 CHECK(rt::activate_owner_hardware(owner));CHECK(owner.memtag_shadow.hardware_active());
 CHECK(rt::activate_owner_hardware(owner));
 CHECK(hw::load_tag(owner.memory_begin)==9 && hw::load_tag(owner.memory_begin+16)==9);
 CHECK(hw::load_tag(owner.memory_begin+32)==0);
 // The directory and physical granules agree on each four-bit color.
 auto managed=[&](mt::opcode operation,Address pointer) {
  Address operands[]{pointer,16};
  return rt::execute<true,Address,Shared>(owner,operation,operands);
 };
 auto reserved=managed(mt::opcode::reserve,128);CHECK(reserved);
 CHECK(hw::load_tag(owner.memory_begin+128)==0);
 auto active=managed(mt::opcode::claimz,208);CHECK(active); // exactly 64 bytes between extents
 auto a=view.pointer_tag(active.operation.value), r=view.pointer_tag(reserved.operation.value);
 CHECK(a && r && a!=r && hw::load_tag(owner.memory_begin+208)==a);
 CHECK(managed(mt::opcode::activate,reserved.operation.value));
 CHECK(hw::load_tag(owner.memory_begin+128)==r);
 CHECK(rt::with_checked_access<Address,Shared>(owner,active.operation.value,0,16,
  [](std::span<std::byte> bytes) noexcept { for(auto value:bytes) CHECK(value==std::byte{}); }));
 CHECK(managed(mt::opcode::release,active.operation.value));CHECK(managed(mt::opcode::release,reserved.operation.value));
 CHECK(hw::load_tag(owner.memory_begin+128)==0 && hw::load_tag(owner.memory_begin+208)==0);
 std::byte* native{};
 CHECK(rt::with_checked_access<Address,Shared>(owner,tag,0,32,[&](std::span<std::byte> bytes) noexcept {
  CHECK(bytes[0]==std::byte{42});CHECK((reinterpret_cast<std::uintptr_t>(bytes.data())>>56u)==9);
  native=bytes.data(); // Stable mmap fixture retains it only while the owner lives.
 }));
 mismatch_rejected(native,false);mismatch_rejected(native,true);
 std::array<Address,2> args{view.with_tag(16,6),16};
 CHECK(rt::execute<true,Address,Shared>(owner,mt::opcode::storez,args));
 CHECK(hw::load_tag(owner.memory_begin)==9 && hw::load_tag(owner.memory_begin+16)==6);
 CHECK(rt::with_checked_access<Address,Shared>(owner,args[0],0,16,[](std::span<std::byte> b) noexcept {
  for(auto byte:b)CHECK(byte==std::byte{});
 }));
 bool called{};
 CHECK(!rt::with_checked_access<Address,Shared>(owner,tag,0,32,[&](std::span<std::byte>) noexcept{called=true;}));
 CHECK(!called);
 CHECK(owner.grow_strictly(1));CHECK(owner.get_page_size()==2);
 CHECK(hw::load_tag(owner.memory_begin)==9 && hw::load_tag(owner.memory_begin+16)==6);
 CHECK(hw::load_tag(owner.memory_begin+65536)==0);
 args={view.with_tag(65536,5),32};
 CHECK(rt::execute<true,Address,Shared>(owner,mt::opcode::storez,args));
 CHECK(hw::load_tag(owner.memory_begin+65536)==5);
 CHECK(rt::with_host_access(owner,args[0],32,[](std::span<std::byte> b) noexcept {
  std::memset(b.data(),0x71,b.size());
 }));
 CHECK(rt::with_checked_copy<Address,Address>(owner,owner,args[0],args[0],32,
  [](std::span<std::byte> dst,std::span<std::byte> src) noexcept { std::memmove(dst.data(),src.data(),src.size()); }));
 linear::mmap_memory_t moved(std::move(owner));CHECK(moved.memtag_shadow.hardware_active());
 CHECK(!owner.memtag_shadow.hardware_active() && !owner.memtag_shadow.hardware_selected());
 CHECK(rt::check_access<Address,Shared>(moved,args[0],0,32));
 moved.clear();CHECK(!moved.memtag_shadow.hardware_active());
}
void threaded_growth() {
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
 linear::mmap_memory_t owner(65536,linear::mmap_memory_status_t::wasm32);owner.init_by_page_count(1,4);
 owner.sequentially_consistent_size=true;
 CHECK(owner.enable_memtag(mt::address_width::i32,true,8,mt::owner_backend::arm_mte)==mt::storage_error::none);
 CHECK(rt::activate_owner_hardware(owner));
 auto view=owner.memtag_shadow.view<std::uint32_t,true>(65536);
 CHECK(view.store(view.with_tag(0,8),16));
 uwvm2::utils::thread::stop_source stop;
 mt::details::storage_failure_test_hook=[](mt::storage_operation op,std::size_t){return op==mt::storage_operation::commit;};
 auto failed=owner.grow_threaded_profile(1,262144,stop.get_token());
 mt::details::storage_failure_test_hook=nullptr;
 CHECK(failed.status==linear::threaded_grow_status::commit_failed && owner.get_page_size()==1);
 CHECK(hw::load_tag(owner.memory_begin)==8);
 auto grown=owner.grow_threaded_profile(1,262144,stop.get_token());
 CHECK(grown.status==linear::threaded_grow_status::success && owner.get_page_size()==2);
 CHECK(owner.memtag_shadow.prepared_memory_bytes()>=131072);
 CHECK(hw::load_tag(owner.memory_begin+65536)==0);
 auto fresh=owner.memtag_shadow.view<std::uint32_t,true>(131072);
 CHECK(fresh.store(fresh.with_tag(65536,6),16));CHECK(hw::load_tag(owner.memory_begin+65536)==6);
 auto limited=owner.grow_threaded_profile(4096,1u<<29,stop.get_token());
 CHECK(limited.status==linear::threaded_grow_status::limit && owner.get_page_size()==2);
 stop.request_stop();
 CHECK(owner.grow_threaded_profile(1,262144,stop.get_token()).status==linear::threaded_grow_status::cancelled);
#endif
}
void partial_granule() {
 linear::mmap_memory_t owner(1,linear::mmap_memory_status_t::wasm32);owner.init_by_page_count(17,10000);
 CHECK(owner.enable_memtag(mt::address_width::i32,false,8,mt::owner_backend::arm_mte)==mt::storage_error::none);
 auto v=owner.memtag_shadow.view<std::uint32_t>(17);CHECK(v.store(v.with_tag(0,7),17));
 CHECK(rt::activate_owner_hardware(owner));
 CHECK(owner.grow_strictly(14));CHECK(hw::load_tag(owner.memory_begin+16)==7);
 CHECK(rt::check_access<std::uint32_t,false>(owner,v.with_tag(0,7),0,31));
 CHECK(owner.grow_strictly(20));CHECK(hw::load_tag(owner.memory_begin+32)==0);
 CHECK(rt::check_access<std::uint32_t,false>(owner,32,0,19));
}
#endif
int main() {
#if defined(__linux__) && defined(__aarch64__) && defined(UWVM_SUPPORT_MMAP)
 mt::selected_fault_delivery.store(mt::fault_delivery::hardware_sync);
 CHECK(hw::supported());CHECK(hw::enable_current_thread_sync());
 CHECK((prctl(56,0ul,0ul,0ul,0ul)&6)==2);
 std::thread worker([]{CHECK(hw::enable_current_thread_sync());CHECK((prctl(56,0ul,0ul,0ul,0ul)&6)==2);});worker.join();
 exercise<std::uint32_t,false>();exercise<std::uint32_t,true>();
 exercise<std::uint64_t,false>();exercise<std::uint64_t,true>();partial_granule();threaded_growth();
 std::puts("hardware mmap owner/checked spans/tagstorez/grow/move/synchronous fault: PASS");
#else
 CHECK(!hw::supported());CHECK(!hw::enable_current_thread_sync());
 mt::shadow_storage storage;CHECK(!storage.hardware_active());
 CHECK(storage.activate_hardware(nullptr,0).error==hw::setup_error::unsupported);
 std::puts("non-MTE host remains software-only: PASS");
#endif
}
