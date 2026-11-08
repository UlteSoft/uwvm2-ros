#define UWVM_TEST
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/memtag/impl.h>
#include <array>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <thread>
#if defined(__linux__)
#include <sys/wait.h>
#include <unistd.h>
#endif
namespace mt=uwvm2::extension::memtag;
namespace rt=uwvm2::runtime::memtag;
namespace linear=uwvm2::object::memory::linear;
static void require(bool pass,unsigned line) {
 if(!pass){std::fprintf(stderr,"bridge failed line %u\n",line);std::abort();}
}
#define CHECK(...) require(static_cast<bool>((__VA_ARGS__)),__LINE__)
#if defined(__linux__)
static unsigned entropy_calls;
static unsigned existing_fork_calls;
static void existing_fork_handler() { ++existing_fork_calls; }
static unsigned entropy_mode;
static std::atomic_size_t* pinned;
static ssize_t entropy(void* output,std::size_t bytes,unsigned flags) {
 CHECK(flags==0);
 if(pinned)CHECK(pinned->load()==0); // OS entropy is outside relocation guards.
 ++entropy_calls;
 if(entropy_mode==1){errno=EIO;return -1;}
 if(entropy_mode==2 && entropy_calls==1){errno=EINTR;return -1;}
 auto const count=entropy_mode==2 && bytes>7?7:bytes;
 for(std::size_t i=0;i!=count;++i)static_cast<unsigned char*>(output)[i]=0xab;
 return static_cast<ssize_t>(count);
}
static void reset_entropy(unsigned mode=0) {
 entropy_calls=0;entropy_mode=mode;
 rt::details::entropy_buffer.position=rt::details::entropy_buffer.bytes.size();
 rt::details::entropy_read_test_hook=entropy;
}
#endif
template<class Address,bool Shared,class Owner>
void exercise(Owner& owner) {
 constexpr auto width=sizeof(Address)==4?mt::address_width::i32:mt::address_width::i64;
 CHECK(owner.enable_memtag(width,Shared)==mt::storage_error::none);
 using View=mt::shadow_view<Address,Shared>;
 auto tag=View::insert(0,6);
 std::array<Address,2> args{tag,128};
 auto stored=rt::execute<true,Address,Shared>(owner,mt::opcode::store,args);
 CHECK(stored && stored.operation.value==tag);
 CHECK(rt::check_access<Address,Shared>(owner,tag,0,128));
 CHECK(!rt::check_access<Address,Shared>(owner,Address{},0,128));
 CHECK(!rt::check_access<Address,Shared>(owner,tag,UINT64_MAX,1));
 CHECK(!rt::check_access<Address,Shared>(owner,tag,1,UINT64_MAX));
 bool called{};
 CHECK(rt::with_checked_access<Address,Shared>(owner,tag,31,1,[&](std::span<std::byte> data) noexcept {
   CHECK(data.size()==1 && data.data()==owner.memory_begin+31);
   if constexpr(Owner::support_multi_thread && !Owner::can_mmap)CHECK(owner.active_ops_p->load()!=0);
   called=true;
 }));
 CHECK(called);
 called=false;
 CHECK(!rt::with_checked_access<Address,Shared>(owner,Address{},31,1,[&](std::span<std::byte>) noexcept {called=true;}));
 CHECK(!called);
 std::array<Address,2> mismatch{View::insert(64,7),16};
 CHECK(rt::execute<true,Address,Shared>(owner,mt::opcode::store,mismatch));
 auto bad=rt::check_access<Address,Shared>(owner,tag,0,128);
 CHECK(!bad && bad.error==mt::access_error::tag_mismatch && bad.mismatch_granule==64);
 args[1]=128;
 CHECK(rt::execute<true,Address,Shared>(owner,mt::opcode::store,args));
 for(unsigned i=0;i!=128;++i)owner.memory_begin[i]=std::byte{0x55};
 auto zeroed=rt::execute<true,Address,Shared>(owner,mt::opcode::untagstorez,args);
 CHECK(zeroed && zeroed.operation.value==0);
 for(unsigned i=0;i!=128;++i)CHECK(owner.memory_begin[i]==std::byte{});
 CHECK(rt::check_access<Address,Shared>(owner,Address{},0,128));
 CHECK(!rt::check_access<Address,!Shared>(owner,Address{},0,0));
 std::array<Address,1> load{0};
 CHECK(rt::execute<true,Address,Shared>(owner,mt::opcode::load,load));
 load[0]=static_cast<Address>(65536);
 CHECK(!rt::execute<true,Address,Shared>(owner,mt::opcode::load,load));
#if defined(__linux__)
 if constexpr(Owner::support_multi_thread && !Owner::can_mmap)pinned=owner.active_ops_p;
 reset_entropy();
 CHECK(!rt::execute<true,Address,Shared>(owner,mt::opcode::random,args));
 CHECK(entropy_calls==0);
 std::array<Address,2> over{65535,2};
 CHECK(!rt::execute<true,Address,Shared>(owner,mt::opcode::randomstore,over));
 CHECK(entropy_calls==0);
 CHECK(rt::execute<true,Address,Shared>(owner,mt::opcode::store,args));
 CHECK(entropy_calls==0);
 reset_entropy(1);
 args={0,16};
 auto failed=rt::execute<true,Address,Shared>(owner,mt::opcode::randomstorez,args);
 CHECK(!failed && failed.error==rt::bridge_error::entropy_unavailable && entropy_calls==1);
 CHECK(rt::check_access<Address,Shared>(owner,View::insert(0,6),0,16));
 reset_entropy(2);
 auto random=rt::execute<true,Address,Shared>(owner,mt::opcode::randomstore,args);
 CHECK(random && entropy_calls>2 && View::extract(random.operation.value)==(0xab&View::maximum_tag));
 CHECK(rt::check_access<Address,Shared>(owner,random.operation.value,0,16));
 reset_entropy();
 args={65536,0};
 auto empty=rt::execute<true,Address,Shared>(owner,mt::opcode::randomstore,args);
 CHECK(empty && entropy_calls==1); // Reference zero-length store still selects a random tag.
 rt::details::entropy_read_test_hook=nullptr;
 rt::details::entropy_buffer.position=256;pinned=nullptr;
#endif
 if constexpr(Owner::support_multi_thread && !Owner::can_mmap)CHECK(owner.active_ops_p->load()==0);
 CHECK(owner.grow_strictly(1)); // Every success/error return released its pin.
}
template<class Owner>
void disabled(Owner& owner) {
 using A=std::uint32_t;
 std::array<A,2> args{0,16};
#if defined(__linux__)
 reset_entropy(1);
#endif
 auto r=rt::execute<false,A,false>(owner,mt::opcode::randomstore,args);
 CHECK(r && r.operation.value==0 && !owner.memtag_shadow.enabled());
 for(unsigned i=0;i!=16;++i)owner.memory_begin[i]=std::byte{0x55};
 CHECK(rt::execute<false,A,false>(owner,mt::opcode::randomstorez,args));
 for(unsigned i=0;i!=16;++i)CHECK(owner.memory_begin[i]==std::byte{});
 args={0xf0000000u,16}; // Disabled must not strip tag-shaped address bits.
 CHECK(!rt::execute<false,A,false>(owner,mt::opcode::storez,args));
#if defined(__linux__)
 CHECK(entropy_calls==0);rt::details::entropy_read_test_hook=nullptr;
#endif
}
template<class Owner>
void concurrent() {
 Owner owner;owner.init_by_page_count(1);
 CHECK(owner.enable_memtag(mt::address_width::i32,true)==mt::storage_error::none);
 std::atomic_bool start{},stop{};
 std::thread reader([&]{
   start.store(true,std::memory_order_release);
   do {CHECK(rt::check_access<std::uint32_t,true>(owner,0,0,65536));}
   while(!stop.load(std::memory_order_acquire));
 });
 while(!start.load(std::memory_order_acquire))std::this_thread::yield();
 for(unsigned i=0;i!=8;++i)CHECK(owner.grow_strictly(1));
 stop.store(true,std::memory_order_release);reader.join();
 CHECK(owner.get_page_size()==9);
}
int main() {
#if defined(__linux__)
 CHECK(pthread_atfork(nullptr,existing_fork_handler,existing_fork_handler)==0);
#endif
 {
 linear::single_thread_allocator_memory_t m;m.init_by_page_count(1);disabled(m);
 exercise<std::uint32_t,false>(m);
 }
 {
 linear::allocator_memory_t m;m.init_by_page_count(1);
 exercise<std::uint64_t,true>(m);
 }
 concurrent<linear::allocator_memory_t>();
#if defined(UWVM_SUPPORT_MMAP)
 {
 linear::mmap_memory_t m;m.init_by_page_count(1);disabled(m);
 exercise<std::uint32_t,true>(m);
 }
 {
 linear::mmap_memory_t m(65536,linear::mmap_memory_status_t::wasm64);m.init_by_page_count(1);
 exercise<std::uint64_t,false>(m);
 }
 concurrent<linear::mmap_memory_t>();
#endif
#if defined(__linux__)
 // Real OS entropy works independently of any WASI runtime or guest authority.
 rt::details::entropy_read_test_hook=nullptr;rt::details::entropy_buffer.position=256;
 std::uint8_t tag{};CHECK(rt::next_random_tag(tag));
 auto pid=fork();CHECK(pid>=0);
 if(pid==0) {
   if(rt::details::entropy_buffer.position!=256 || existing_fork_calls!=1)_exit(2);
   if(!rt::next_random_tag(tag))_exit(3);
   _exit(0);
 }
 CHECK(existing_fork_calls==1);
 int status{};CHECK(waitpid(pid,&status,0)==pid && WIFEXITED(status) && WEXITSTATUS(status)==0);
#endif
 std::puts("PASS runtime memtag bridge: snapshots/guards/widths/shared/entropy/denials/disabled/fork");
}
#include <uwvm2/utils/macro/pop_macros.h>
