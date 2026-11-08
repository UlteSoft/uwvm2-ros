#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/memtag/bridge.h>
#include <cstdio>
#include <cstdlib>
#include <thread>
#if defined(__linux__) && defined(__aarch64__)
#include <sys/wait.h>
#include <sys/prctl.h>
#include <unistd.h>
namespace mt=uwvm2::extension::memtag;
namespace rt=uwvm2::runtime::memtag;
namespace sig=uwvm2::object::memory::signal;
namespace linear=uwvm2::object::memory::linear;
static void require(bool ok,unsigned line) {if(!ok){std::fprintf(stderr,"hardware signal line %u\n",line);std::abort();}}
#define CHECK(...) require(static_cast<bool>((__VA_ARGS__)),__LINE__)
static int report_fd;
static volatile unsigned char* good_pointer;
struct report {sig::hardware_memtag_fault fault;unsigned char preserved;};
static void delivered(sig::hardware_memtag_fault const& fault) noexcept {
 report value{fault,*good_pointer};
 static_cast<void>(::write(report_fd,&value,sizeof(value)));::_Exit(126);
}
static void wrong_bounds(uwvm2::object::memory::error::mmap_memory_error_t const&) noexcept {::_Exit(31);}
static void previous(int,::siginfo_t*,void*) noexcept {::_Exit(91);}
template<class Address,bool Shared> void test() {
 linear::mmap_memory_t memory(65536,sizeof(Address)==4?linear::mmap_memory_status_t::wasm32:linear::mmap_memory_status_t::wasm64);memory.init_by_page_count(1,2,99);
 CHECK(memory.enable_memtag(sizeof(Address)==4?mt::address_width::i32:mt::address_width::i64,Shared)==mt::storage_error::none);
 CHECK(rt::activate_owner_hardware(memory));
 auto view=memory.memtag_shadow.view<Address,Shared>(65536);
 constexpr unsigned tag=sizeof(Address)==4?11:171;
 CHECK(view.store(view.insert(0,tag),16));
 auto* right=mt::hardware_mte::tagged_pointer(memory.memory_begin,tag);*right=std::byte{42};
 for(bool write:{false,true}) {
  int fds[2];CHECK(::pipe(fds)==0);auto pid=::fork();CHECK(pid>=0);
  if(!pid) {
   ::close(fds[0]);report_fd=fds[1];good_pointer=reinterpret_cast<volatile unsigned char*>(right);
   sig::detail::hardware_memtag_fault_func=delivered;sig::set_mmap_memory_out_of_bounds_handler(wrong_bounds);
   auto* bad=reinterpret_cast<volatile unsigned char*>(mt::hardware_mte::tagged_pointer(memory.memory_begin,1));
   if(write)*bad=77;else {volatile auto value=*bad;static_cast<void>(value);}::_Exit(0);
  }
  ::close(fds[1]);report got{};auto n=::read(fds[0],&got,sizeof(got));::close(fds[0]);int status{};CHECK(::waitpid(pid,&status,0)==pid);
  CHECK(WIFEXITED(status)&&WEXITSTATUS(status)==126);CHECK(n==sizeof(got));CHECK(got.fault.memory_index==99&&got.fault.offset==0&&got.fault.length==65536);
  CHECK(got.fault.allocation_tag==tag&&got.fault.hardware_allocation_tag==(tag&15)&&got.fault.instruction_address!=0&&got.preserved==42);
  if(got.fault.pointer_tag_available)CHECK(((got.fault.raw_address>>56)&15)==1);
 }
 bool synced{};std::thread worker{[&]{
   CHECK(::prctl(55,1UL|4UL|(0xfffeUL<<3),0UL,0UL,0UL)==0);
   CHECK(rt::prepare_hardware_thread());synced=(::prctl(56,0UL,0UL,0UL,0UL)&6)==2;
 }};worker.join();CHECK(synced);
}
int main() {
 CHECK(mt::hardware_mte::supported());struct ::sigaction action{};action.sa_flags=SA_SIGINFO;action.sa_sigaction=previous;::sigemptyset(&action.sa_mask);CHECK(::sigaction(SIGSEGV,&action,nullptr)==0);
 test<std::uint32_t,false>();test<std::uint32_t,true>();test<std::uint64_t,false>();test<std::uint64_t,true>();
 // A separate native allocation is never labelled a Wasm failure.
 auto* native=static_cast<std::byte*>(::mmap(nullptr,4096,PROT_READ|PROT_WRITE|0x20,MAP_PRIVATE|MAP_ANONYMOUS,-1,0));CHECK(native!=MAP_FAILED);
 mt::hardware_mte::store_tag(native,4);mt::hardware_mte::finish_tag_writes();
 auto pid=::fork();CHECK(pid>=0);if(!pid){volatile auto value=*reinterpret_cast<volatile unsigned char*>(native);static_cast<void>(value);::_Exit(0);}
 int status{};CHECK(::waitpid(pid,&status,0)==pid);CHECK(WIFEXITED(status)&&WEXITSTATUS(status)==91);CHECK(::munmap(native,4096)==0);
 std::puts("hardware signal: exact owner/full tag/SYNC/no partial write/unowned forwarding PASS");
}
#else
int main(){std::puts("hardware signal: unsupported host; software path unchanged");}
#endif
