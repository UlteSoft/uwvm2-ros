#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/compiler/uwvm_int/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/runtime/compiler/uwvm_int/optable/memtag_bulk.h>
#include <uwvm2/runtime/compiler/uwvm_int/optable/memtag_simd.h>
#include <uwvm2/runtime/compiler/uwvm_int/optable/memtag_threads.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <sys/wait.h>
#include <unistd.h>
namespace op=uwvm2::runtime::compiler::uwvm_int::optable;
namespace md=op::memtag_details;
namespace mt=uwvm2::extension::memtag;
static md::native_memory_t* active;
static std::uint64_t expected_raw,expected_granule;
static void UWVM_INTERPRETER_OPFUNC_TYPE_MACRO mismatch(std::size_t owner,std::uint64_t raw,std::uint64_t granule,std::uint32_t pointer,std::uint32_t allocation)noexcept{
 if(owner!=42 || raw!=expected_raw || granule!=expected_granule || pointer!=3 || allocation!=4)_exit(90);
 for(unsigned i{};i!=256;++i)if(active->memory_begin[i]!=std::byte{0x44})_exit(91);
 _exit(0);
}
template<class Address,bool Shared>void child(unsigned test){
 using Memory=md::native_memory_t;
#if defined(UWVM_SUPPORT_MMAP)
 Memory memory(65536,sizeof(Address)==4?uwvm2::object::memory::linear::mmap_memory_status_t::wasm32:
 uwvm2::object::memory::linear::mmap_memory_status_t::wasm64);
#else
 Memory memory;
#endif
 memory.sequentially_consistent_size=Shared;memory.init_by_page_count(1);memory.diagnostic_owner_memory_index=42;active=&memory;
 if(memory.enable_memtag(sizeof(Address)==4?mt::address_width::i32:mt::address_width::i64,Shared)!=mt::storage_error::none)_exit(92);
 using View=mt::shadow_view<Address,Shared>;auto const tag=View::insert(0,3);
 auto const view=memory.memtag_shadow.template view<Address,Shared>(65536);
 if(!view.store(tag,256))_exit(93);
 unsigned const bad=test<=2?16:test==3||test==4?64:test==5?192:0;
 if(!view.store(View::insert(bad,4),16))_exit(94);
 for(unsigned i{};i!=256;++i)memory.memory_begin[i]=std::byte{0x44};
 expected_raw=test==5?View::insert(128,3):tag;expected_granule=bad;
 op::trap_memtag_func=mismatch;
 std::array<std::byte,128> storage{};auto* stack=storage.data();
 switch(test){
  case 0:md::push(stack,tag);md::push(stack,std::uint64_t{123});md::scalar<Address,Shared,std::int64_t,8,false,true>(memory,stack,15);break;
  case 1:md::push(stack,tag);md::scalar<Address,Shared,std::int64_t,8,false,false>(memory,stack,15);break;
  case 2:md::push(stack,tag);md::push(stack,md::simd::wasm_v128{});md::simd_memory<Address,Shared,md::simd::simd_code::v128_store>(memory,stack,15,0);break;
  case 3:md::push(stack,tag);md::push(stack,std::uint32_t{0x55});md::push(stack,Address{128});md::fill<Address,Shared>(memory,nullptr,stack);break;
  case 4:case 5:
   md::push(stack,View::insert(128,3));md::push(stack,tag);md::push(stack,Address{128});md::copy<Address,Address>(memory,&memory,stack);break;
  case 6:md::push(stack,tag);md::push(stack,std::uint64_t{123});md::atomic_memory<Address,Shared,0x18>(memory,stack,8);break;
  case 7:md::push(stack,tag);md::push(stack,std::uint32_t{0});md::push(stack,std::int64_t{0});md::atomic_memory<Address,Shared,1>(memory,stack,0);break;
 }
 _exit(95);
}
template<class Address,bool Shared>bool run(){
 for(unsigned test{};test!=8;++test){
  auto pid=fork();if(pid<0)return false;if(pid==0)child<Address,Shared>(test);
  int status{};if(waitpid(pid,&status,0)!=pid || !WIFEXITED(status) || WEXITSTATUS(status)!=0){
   std::fprintf(stderr,"negative test %u width %zu shared %u status %d\n",test,sizeof(Address),Shared,status);return false;
  }
 }
 return true;
}
int main(){
 if(!run<std::uint32_t,false>()||!run<std::uint32_t,true>()||!run<std::uint64_t,false>()||!run<std::uint64_t,true>())return 1;
 std::puts("PASS MTE traps: scalar/SIMD/atomic/bulk/wait, exact granule/tags/owner, no partial stores");return 0;
}
