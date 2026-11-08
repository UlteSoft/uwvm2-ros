#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/compiler/llvm_jit/compile_all_from_uwvm/impl.h>
#include <cstdio>
#include <cstdlib>
namespace jt=uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
namespace mt=uwvm2::extension::memtag;
struct trap_record { unsigned kind;std::size_t owner;std::uint64_t raw,granule;std::uint32_t pointer,allocation; };
namespace uwvm2::runtime::lib {
void llvm_jit_managed_memtag_trap(std::size_t owner,std::uint64_t raw,std::uint64_t granule,std::uint32_t pointer,std::uint32_t allocation)
{ throw trap_record{1,owner,raw,granule,pointer,allocation}; }
void llvm_jit_managed_memory_trap(std::size_t owner,std::uint_least64_t,std::uint_least64_t raw,std::uint_least32_t,
 std::uint_least64_t,std::size_t,std::uintptr_t,std::uintptr_t) {throw trap_record{2,owner,raw,0,0,0};}
void llvm_jit_managed_runtime_trap(llvm_jit_trap_kind,std::uintptr_t,std::uintptr_t) {throw trap_record{3,0,0,0,0,0};}
namespace details {void llvm_jit_debug_cancelled_wait_abi_bridge(){throw trap_record{4,0,0,0,0,0};}}
}
static void require(bool ok,unsigned line) {if(!ok){std::fprintf(stderr,"JIT bridge assertion %u\n",line);std::abort();}}
#define CHECK(...) require(bool((__VA_ARGS__)),__LINE__)
template<typename Address,bool Shared>void check(){
 using Memory=jt::runtime_native_memory_t;using View=mt::shadow_view<Address,Shared>;
#if defined(UWVM_SUPPORT_MMAP)
 Memory memory(65536,sizeof(Address)==4?uwvm2::object::memory::linear::mmap_memory_status_t::wasm32:
 uwvm2::object::memory::linear::mmap_memory_status_t::wasm64);
#else
 Memory memory;
#endif
 memory.sequentially_consistent_size=Shared;memory.init_by_page_count(1);memory.diagnostic_owner_memory_index=42;
 CHECK(memory.enable_memtag(sizeof(Address)==4?mt::address_width::i32:mt::address_width::i64,Shared)==mt::storage_error::none);
 auto const owner=reinterpret_cast<std::uintptr_t>(&memory);auto const tag=View::insert(0,3);
 auto const view=memory.memtag_shadow.template view<Address,Shared>(65536);
 CHECK(view.store(tag,512));
 auto scalar=[&]<unsigned Bits,std::size_t Bytes,bool Signed>(){
  constexpr std::uint64_t input=0x887766554433eeffull;
  (void)jt::llvm_jit_memtag_scalar_bridge<Address,Shared,Bits,Bytes,false,true,true>(owner,15,tag,input);
  auto const output=jt::llvm_jit_memtag_scalar_bridge<Address,Shared,Bits,Bytes,Signed,false,true>(owner,15,tag,0);
  std::uint64_t expected=input;
  if constexpr(Bytes<8)expected&=(std::uint64_t{1}<<(Bytes*8))-1;
  if constexpr(Signed && Bytes*8<Bits){auto const sign=std::uint64_t{1}<<(Bytes*8-1);expected=(expected^sign)-sign;}
  if constexpr(Bits==32)expected=static_cast<std::uint32_t>(expected);
  CHECK(output==expected);
 };
 scalar.template operator()<32,1,false>();scalar.template operator()<32,1,true>();
 scalar.template operator()<32,2,false>();scalar.template operator()<32,2,true>();scalar.template operator()<32,4,false>();
 scalar.template operator()<64,1,false>();scalar.template operator()<64,1,true>();
 scalar.template operator()<64,2,false>();scalar.template operator()<64,2,true>();
 scalar.template operator()<64,4,false>();scalar.template operator()<64,4,true>();scalar.template operator()<64,8,false>();
 auto simd=[&]<unsigned Opcode>(){
  constexpr auto op=static_cast<jt::llvm_jit_simd_code>(Opcode);
  constexpr bool store=Opcode==11 || (Opcode>=88 && Opcode<=91);
  constexpr auto width=jt::llvm_jit_simd_details::simd_memory_access_size<op>();
  constexpr auto lane=Opcode>=84&&Opcode<=91 ? 16/width-1 : 0;
  jt::runtime_wasm_v128 input{},result{};
  std::array<std::byte,16> source{},expected{};
  for(unsigned i{};i!=16;++i){source[i]=std::byte(0x80+i);reinterpret_cast<std::byte*>(&input)[i]=std::byte(0x40+i);}
  std::memcpy(memory.memory_begin+15,source.data(),16);
  // Independent byte-level oracle, not the runtime SIMD evaluator.
  if constexpr(store){for(unsigned i{};i!=width;++i)expected[i]=std::byte(0x40+lane*width+i);}
  else if constexpr(Opcode==0 || Opcode==92 || Opcode==93){for(unsigned i{};i!=width;++i)expected[i]=source[i];}
  else if constexpr(Opcode>=84 && Opcode<=87){
   for(unsigned i{};i!=16;++i)expected[i]=std::byte(0x40+i);
   for(unsigned i{};i!=width;++i)expected[lane*width+i]=source[i];
  }else if constexpr(Opcode>=7 && Opcode<=10){for(unsigned i{};i!=16;++i)expected[i]=source[i%width];}
  else{
   constexpr unsigned input_width=1u<<((Opcode-1)/2);
   for(unsigned n{};n!=8/input_width;++n){
    for(unsigned i{};i!=input_width;++i)expected[n*2*input_width+i]=source[n*input_width+i];
    for(unsigned i=input_width;i!=2*input_width;++i)expected[n*2*input_width+i]=std::byte((Opcode&1)?255:0);
   }
  }
  jt::llvm_jit_memtag_simd_bridge<op,Address,Shared,true>(owner,15,tag,reinterpret_cast<std::uintptr_t>(&input),
   lane,reinterpret_cast<std::uintptr_t>(&result));
  if constexpr(store)CHECK(std::memcmp(memory.memory_begin+15,expected.data(),width)==0);
  else CHECK(std::memcmp(&result,expected.data(),16)==0);
 };
#define SIMD_CASE(N) simd.template operator()<N>()
 SIMD_CASE(0);SIMD_CASE(1);SIMD_CASE(2);SIMD_CASE(3);SIMD_CASE(4);SIMD_CASE(5);SIMD_CASE(6);
 SIMD_CASE(7);SIMD_CASE(8);SIMD_CASE(9);SIMD_CASE(10);SIMD_CASE(11);SIMD_CASE(84);SIMD_CASE(85);
 SIMD_CASE(86);SIMD_CASE(87);SIMD_CASE(88);SIMD_CASE(89);SIMD_CASE(90);SIMD_CASE(91);SIMD_CASE(92);SIMD_CASE(93);
#undef SIMD_CASE
 namespace atomic=uwvm2::validation::standard::wasm3;
 for(unsigned op=0x10;op!=0x4f;++op){
  std::memset(memory.memory_begin,0,8);memory.memory_begin[0]=std::byte{7};
  auto const descriptor=atomic::describe_atomic_instruction(op);
  auto const old=jt::llvm_jit_memtag_atomic_bridge<Address,Shared,true>(owner,0,tag,3,7,op);
  bool const store=descriptor.kind==atomic::atomic_instruction_kind::store;
  CHECK(old==(store?0:7));
  unsigned expected=7;
  switch(descriptor.kind){
   case atomic::atomic_instruction_kind::store:expected=3;break;
   case atomic::atomic_instruction_kind::add:expected=10;break;
   case atomic::atomic_instruction_kind::sub:expected=4;break;
   case atomic::atomic_instruction_kind::and_:expected=3;break;
   case atomic::atomic_instruction_kind::or_:expected=7;break;
   case atomic::atomic_instruction_kind::xor_:expected=4;break;
   case atomic::atomic_instruction_kind::exchange:case atomic::atomic_instruction_kind::compare_exchange:expected=3;break;
   default:break;
  }
  CHECK(memory.memory_begin[0]==std::byte(expected));
 }
 jt::llvm_jit_memtag_fill_bridge<Address,Shared,true>(owner,tag,0x55,256);
 jt::llvm_jit_memtag_copy_bridge<Address,Address,true>(owner,owner,View::insert(17,3),tag,128);
 for(unsigned i{};i!=256;++i)CHECK(memory.memory_begin[i]==std::byte{0x55});
 if constexpr(Shared){
  std::memset(memory.memory_begin,0,8);
  CHECK((jt::llvm_jit_memtag_wait_notify_bridge<0,Address,Shared,true>(owner,0,tag,1,0))==0);
  CHECK((jt::llvm_jit_memtag_wait_notify_bridge<1,Address,Shared,true>(owner,0,tag,1,0))==1);
  CHECK((jt::llvm_jit_memtag_wait_notify_bridge<2,Address,Shared,true>(owner,0,tag,0,0))==2);
 }
 for(unsigned test{};test!=8;++test){
  CHECK(view.store(tag,512));unsigned const bad=test<=2?16:test==3||test==4?64:test==5?192:0;
  CHECK(view.store(View::insert(bad,4),16));std::memset(memory.memory_begin,0x44,256);
  bool caught{};
  try{
   switch(test){
    case 0:(void)jt::llvm_jit_memtag_scalar_bridge<Address,Shared,64,8,false,true,true>(owner,15,tag,0);break;
    case 1:(void)jt::llvm_jit_memtag_scalar_bridge<Address,Shared,64,8,false,false,true>(owner,15,tag,0);break;
    case 2:{jt::runtime_wasm_v128 value{};jt::llvm_jit_memtag_simd_bridge<jt::llvm_jit_simd_code::v128_store,Address,Shared,true>(
      owner,15,tag,reinterpret_cast<std::uintptr_t>(&value),0,0);break;}
    case 3:jt::llvm_jit_memtag_fill_bridge<Address,Shared,true>(owner,tag,0x55,128);break;
    case 4:case 5:jt::llvm_jit_memtag_copy_bridge<Address,Address,true>(owner,owner,View::insert(128,3),tag,128);break;
    case 6:(void)jt::llvm_jit_memtag_atomic_bridge<Address,Shared,true>(owner,0,tag,0,0,0x18);break;
    case 7:(void)jt::llvm_jit_memtag_wait_notify_bridge<1,Address,Shared,true>(owner,0,tag,0,0);break;
   }
  }catch(trap_record const& error){
   caught=true;CHECK(error.kind==1 && error.owner==42 && error.granule==bad && error.pointer==3 && error.allocation==4);
   CHECK(error.raw==(test==5?View::insert(128,3):tag));
  }
  CHECK(caught);for(unsigned i{};i!=256;++i)CHECK(memory.memory_begin[i]==std::byte{0x44});
 }
 []<typename Owner>(Owner const& current){if constexpr(Owner::support_multi_thread && !Owner::can_mmap)CHECK(current.active_ops_p->load()==0);}(memory);
 CHECK(memory.grow_strictly(1));
}
int main(){
 uwvm2::runtime::wasm_threads::wait_domain domain;uwvm2::runtime::wasm_threads::execution_scope scope{domain,{}};
 check<std::uint32_t,false>();check<std::uint32_t,true>();check<std::uint64_t,false>();check<std::uint64_t,true>();
 std::puts("PASS JIT pinned MTE bridges: scalar/SIMD/atomic/bulk/wait, all widths, shared modes, no partial mutation and traps after pins");
}
