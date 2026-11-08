#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/memtag/wait.h>
#include <array>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>
namespace mt=uwvm2::extension::memtag;
namespace rt=uwvm2::runtime::memtag;
namespace wt=uwvm2::runtime::wasm_threads;
namespace linear=uwvm2::object::memory::linear;
static void require(bool pass,unsigned line){if(!pass){std::fprintf(stderr,"copy/wait failed line %u\n",line);std::abort();}}
#define CHECK(...) require(static_cast<bool>((__VA_ARGS__)),__LINE__)
template<typename A>void exercise(){
 linear::allocator_memory_t destination,source,ordinary;
 destination.sequentially_consistent_size=source.sequentially_consistent_size=true;
 destination.init_by_page_count(1);source.init_by_page_count(1);ordinary.init_by_page_count(1);
 constexpr auto width=sizeof(A)==4?mt::address_width::i32:mt::address_width::i64;
 CHECK(destination.enable_memtag(width,true)==mt::storage_error::none);
 CHECK(source.enable_memtag(width,true)==mt::storage_error::none);
 using View=mt::shadow_view<A,true>;
 auto dst=View::insert(0,3),src=View::insert(0,7);
 CHECK(destination.memtag_shadow.template view<A,true>(65536).store(dst,256));
 CHECK(source.memtag_shadow.template view<A,true>(65536).store(src,256));
 bool called{};
 auto copy=[&](std::span<std::byte> d,std::span<std::byte> s)noexcept{
  CHECK(destination.active_ops_p->load()!=0);
  CHECK(source.active_ops_p->load()!=0);
  called=true;if(!d.empty())std::memmove(d.data(),s.data(),d.size());
 };
 for(unsigned i{};i!=256;++i){source.memory_begin[i]=std::byte{0x55};destination.memory_begin[i]=std::byte{0x77};}
 CHECK(source.memtag_shadow.template view<A,true>(65536).store(View::insert(128,9),16));
 auto bad=rt::with_checked_copy(destination,source,dst,src,256,copy);
 CHECK(!bad && bad.source_failed && bad.access.mismatch_granule==128 && !called);
 for(unsigned i{};i!=256;++i)CHECK(destination.memory_begin[i]==std::byte{0x77});
 CHECK(source.memtag_shadow.template view<A,true>(65536).store(src,256));
 CHECK(rt::with_checked_copy(destination,source,dst,src,256,copy) && called);
 CHECK(destination.active_ops_p->load()==0 && source.active_ops_p->load()==0);
 CHECK(rt::with_checked_copy(destination,destination,View::insert(1,3),dst,127,[&](std::span<std::byte> d,std::span<std::byte> s)noexcept{
  CHECK(destination.active_ops_p->load()==1);std::memmove(d.data(),s.data(),d.size());
 }));
 CHECK(rt::with_checked_copy(ordinary,destination,A{},dst,128,[](std::span<std::byte> d,std::span<std::byte> s)noexcept{std::memcpy(d.data(),s.data(),d.size());}));
 CHECK(!rt::with_checked_copy(ordinary,destination,View::insert(0,3),dst,128,[](std::span<std::byte>,std::span<std::byte>)noexcept{std::abort();}));
 std::memset(destination.memory_begin,0,8);destination.memory_begin[0]=std::byte{9};
 wt::wait_domain domain;wt::execution_scope scope{domain,{}};
 auto mismatch=rt::wait_notify<1,A,true>(destination,A{},0,9,0);
 CHECK(!mismatch.access && mismatch.access.error==mt::access_error::tag_mismatch);
 auto unequal=rt::wait_notify<1,A,true>(destination,dst,0,8,0);
 CHECK(unequal.access && unequal.wait.status==wt::wait_status::not_equal);
 auto timed=rt::wait_notify<2,A,true>(destination,dst,0,9,0);
 CHECK(timed.access && timed.wait.status==wt::wait_status::timed_out);
 CHECK(destination.active_ops_p->load()==0);
 std::atomic_bool started{},finished{};
 std::thread waiter([&]{
  wt::execution_scope thread_scope{domain,{}};
  started.store(true,std::memory_order_release);
  auto r=rt::wait_notify<1,A,true>(destination,dst,0,9,1000000000);
  CHECK(r.access && r.wait.status==wt::wait_status::notified);finished.store(true,std::memory_order_release);
 });
 while(!started.load(std::memory_order_acquire))std::this_thread::yield();
 // Growth must complete while the other thread is asleep, proving the tag
 // check did not retain an allocator pin during the wait.
 CHECK(destination.grow_strictly(1));
 bool notified{};
 for(unsigned n{};n!=1000000 && !notified;++n){
  auto r=rt::wait_notify<0,A,true>(destination,dst,0,1,0);
  CHECK(r.access && r.wait.status==wt::wait_status::notified);
  notified=r.wait.notified==1;if(!notified)std::this_thread::yield();
 }
 CHECK(notified);waiter.join();CHECK(finished.load());
}
int main(){exercise<std::uint32_t>();exercise<std::uint64_t>();std::puts("PASS MTE mixed copy/alias pin/preflight and wait/notify/growth");}
#include <uwvm2/utils/macro/pop_macros.h>
