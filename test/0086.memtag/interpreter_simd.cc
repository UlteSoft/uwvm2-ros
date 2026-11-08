#include "../0013.uwvm_int/strict/uwvm_int_translate_strict_common.h"
#include "host_snapshot_copy.h"
namespace {
using namespace uwvm2test::uwvm_int_strict;
namespace mt=uwvm2::extension::memtag;
struct simd_case { unsigned opcode, width, lane; bool consumes_vector, store; };
constexpr simd_case cases[]{
 {0,16,0,false,false},{1,8,0,false,false},{2,8,0,false,false},
 {3,8,0,false,false},{4,8,0,false,false},{5,8,0,false,false},{6,8,0,false,false},
 {7,1,0,false,false},{8,2,0,false,false},{9,4,0,false,false},{10,8,0,false,false},
 {11,16,0,true,true},
 {84,1,15,true,false},{85,2,7,true,false},{86,4,3,true,false},{87,8,1,true,false},
 {88,1,15,true,true},{89,2,7,true,true},{90,4,3,true,true},{91,8,1,true,true},
 {92,4,0,false,false},{93,8,0,false,false}
};
std::array<std::uint8_t,16> expected(simd_case c) {
 std::array<std::uint8_t,16> r{};
 if(c.consumes_vector) for(unsigned i{};i!=16;++i)r[i]=static_cast<std::uint8_t>(0x40+i);
 auto input=[](unsigned i){return static_cast<std::uint8_t>(0x80+i);};
 if(c.store) {
  r.fill(0);for(unsigned i{};i!=c.width;++i)r[i]=static_cast<std::uint8_t>(0x40+c.lane*c.width+i);
 } else if(c.opcode==0 || c.opcode==92 || c.opcode==93) {
  for(unsigned i{};i!=c.width;++i)r[i]=input(i);
 } else if(c.opcode>=84 && c.opcode<=87) {
  for(unsigned i{};i!=c.width;++i)r[c.lane*c.width+i]=input(i);
 } else if(c.opcode>=7 && c.opcode<=10) {
  for(unsigned i{};i!=16;++i)r[i]=input(i%c.width);
 } else {
  auto const in_width=1u<<((c.opcode-1)/2);
  bool const sign=(c.opcode&1)!=0;
  for(unsigned lane{};lane!=8/in_width;++lane) {
   for(unsigned j{};j!=in_width;++j)r[lane*2*in_width+j]=input(lane*in_width+j);
   for(unsigned j=in_width;j!=2*in_width;++j)r[lane*2*in_width+j]=sign?255u:0u;
  }
 }
 return r;
}
byte_vec make(bool wide) {
 module_builder b{};b.has_memory=b.memory_has_max=true;b.memory_min=1;b.memory_max=2;b.memory_address64=wide;
 for(auto const c:cases) {
  func_body body{};
  auto number=[&](std::int64_t n){append_u8(body.code,wide?0x42:0x41);if(wide)append_i64_leb(body.code,n);else append_i32_leb(body.code,static_cast<std::int32_t>(n));};
  auto tagged=[&](unsigned p){number(p);number(3);append_u8(body.code,0xfc);append_u8(body.code,static_cast<unsigned>(mt::opcode::insert));append_u8(body.code,0);};
  auto simd=[&](unsigned op){append_u8(body.code,0xfd);append_u32_leb(body.code,op);};
  // The allocation and memory operand deliberately cross a tag granule.
  tagged(0);number(256);append_u8(body.code,0xfc);append_u8(body.code,static_cast<unsigned>(mt::opcode::store));append_u8(body.code,0);
  if(!c.store)tagged(128);
  tagged(c.store?128:15);
  if(c.consumes_vector) {simd(12);for(unsigned i{};i!=16;++i)append_u8(body.code,0x40+i);}
  simd(c.opcode);append_u8(body.code,0);append_u8(body.code,0);
  if(c.opcode>=84 && c.opcode<=91)append_u8(body.code,c.lane);
  if(!c.store){simd(11);append_u8(body.code,0);append_u8(body.code,0);}
  append_u8(body.code,0x41);append_u8(body.code,0);append_u8(body.code,0xb);
  b.add_func({{},{k_val_i32}},std::move(body));
 }
 return b.build();
}
template<optable::uwvm_interpreter_translate_option_t Option>
int check(bool wide,bool enabled) {
 auto wasm=make(wide);auto features=make_wasm1p1_feature_parameter();
 auto& p=uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1p1_parameter(features);
 p.disable_memtag_extension=false;p.disable_memory64=false;
 p.cli_mode=uwvm2::parser::wasm::standard::wasm1p1::features::wasm_feature_cli_mode::scoped;
 uwvm2::uwvm::runtime::runtime_mode::runtime_disable_memtag=!enabled;
 auto prepared=prepare_runtime_from_wasm(wasm,u8"memtag-interpreter-simd",{},features);
 auto& memory=const_cast<uwvm2::object::memory::linear::native_memory_t&>(prepared.mod->local_defined_memory_vec_storage.index_unchecked(0).memory);
 if(enabled && !memory.memtag_shadow.enabled())UWVM2TEST_REQUIRE(memory.enable_memtag(wide?mt::address_width::i64:mt::address_width::i32,false)==mt::storage_error::none);
 uwvm2::validation::error::code_validation_error_impl error{};optable::compile_option options{};options.execute_memtag=enabled;
 auto compiled=compiler::compile_all_from_uwvm_single_func<Option>(*prepared.mod,options,error,&features);
 byte_vec args{};
 for(std::size_t i{};i!=std::size(cases);++i) {
  std::array<std::uint8_t,16> input{},observed{};
  for(unsigned n{};n!=16;++n)input[n]=static_cast<std::uint8_t>(0x80+n);
  UWVM2TEST_REQUIRE(memtag_test_host_copy(memory,15,input.data(),input.size(),true));
  UWVM2TEST_REQUIRE(memtag_test_host_copy(memory,128,observed.data(),observed.size(),true));
  auto r=interpreter_runner<Option>::run(compiled.local_funcs.index_unchecked(i),
    prepared.mod->local_defined_function_vec_storage.index_unchecked(i),args,nullptr,nullptr);
  auto want=expected(cases[i]);
  UWVM2TEST_REQUIRE(memtag_test_host_copy(memory,128,observed.data(),observed.size()));
  if(observed!=want) {
   std::fprintf(stderr,"SIMD mismatch opcode %u wide %u enabled %u tail %u\n",cases[i].opcode,wide,enabled,Option.is_tail_call);
   return 1;
  }
 }
 return 0;
}
}
int main(){
 install_unexpected_traps();constexpr optable::uwvm_interpreter_translate_option_t loop{.is_tail_call=false};
 constexpr auto ring=make_tailcall_scalar4_merged_opt<2>();
 for(bool wide:{false,true})for(bool enabled:{false,true}) {
  UWVM2TEST_REQUIRE(check<loop>(wide,enabled)==0);UWVM2TEST_REQUIRE(check<ring>(wide,enabled)==0);
 }
 std::puts("PASS all 22 SIMD memory opcodes: memory32/memory64, loop/tail, MTE enabled/disabled, cross-granule operands");
 return 0;
}
