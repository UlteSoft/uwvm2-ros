#include "../0013.uwvm_int/strict/uwvm_int_translate_strict_common.h"
#include "host_snapshot_copy.h"
namespace {
using namespace uwvm2test::uwvm_int_strict;
namespace mt=uwvm2::extension::memtag;
namespace atom=uwvm2::validation::standard::wasm3;
byte_vec make(bool wide) {
 module_builder b{};b.has_memory=b.memory_has_max=b.memory_shared=true;
 b.memory_min=1;b.memory_max=2;b.memory_address64=wide;
 for(unsigned op=0;op!=0x4f;++op) {
  auto d=atom::describe_atomic_instruction(op);if(d.kind==atom::atomic_instruction_kind::invalid)continue;
  func_body body{};
  auto number=[&](std::int64_t n,bool value_wide){append_u8(body.code,value_wide?0x42:0x41);if(value_wide)append_i64_leb(body.code,n);else append_i32_leb(body.code,static_cast<std::int32_t>(n));};
  auto tagged=[&](unsigned pointer){number(pointer,wide);number(3,wide);append_u8(body.code,mt::opcode_prefix);append_u32_leb(body.code,mt::encode_opcode(mt::opcode::insert));append_u8(body.code,0);};
  tagged(0);number(256,wide);append_u8(body.code,mt::opcode_prefix);append_u32_leb(body.code,mt::encode_opcode(mt::opcode::store));append_u8(body.code,0);
  if(op!=3) {
   tagged(1);
   if(op<=2) {
    number(op==0?1:8,d.value_i64);
    if(op!=0)number(0,true);
   } else if(d.kind!=atom::atomic_instruction_kind::load) {
    if(d.kind==atom::atomic_instruction_kind::compare_exchange)number(9,d.value_i64);
    number(5,d.value_i64);
   }
  }
  append_u8(body.code,0xfe);append_u32_leb(body.code,op);
  if(op==3)append_u8(body.code,0);
  else {append_u8(body.code,d.natural_alignment);append_u8(body.code,7);}
  if(!d.has_result){number(0,false);}
  append_u8(body.code,0xb);b.add_func({{},{d.result_i64?k_val_i64:k_val_i32}},std::move(body));
 }
 return b.build();
}
template<optable::uwvm_interpreter_translate_option_t Option>
int check(bool wide,bool enabled) {
 auto wasm=make(wide);auto features=make_wasm1p1_feature_parameter();
 auto& p=uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1p1_parameter(features);
 p.disable_memtag_extension=false;p.disable_memory64=false;p.disable_threads=false;
 p.cli_mode=uwvm2::parser::wasm::standard::wasm1p1::features::wasm_feature_cli_mode::scoped;
 uwvm2::uwvm::runtime::runtime_mode::runtime_disable_memtag=!enabled;
 auto prepared=prepare_runtime_from_wasm(wasm,u8"memtag-interpreter-atomic",{},features);
 auto& memory=const_cast<uwvm2::object::memory::linear::native_memory_t&>(prepared.mod->local_defined_memory_vec_storage.index_unchecked(0).memory);
 if(enabled && !memory.memtag_shadow.enabled())UWVM2TEST_REQUIRE(memory.enable_memtag(wide?mt::address_width::i64:mt::address_width::i32,true)==mt::storage_error::none);
 uwvm2::validation::error::code_validation_error_impl error{};optable::compile_option options{};options.execute_memtag=enabled;
 auto compiled=compiler::compile_all_from_uwvm_single_func<Option>(*prepared.mod,options,error,&features);
 byte_vec args{};
 uwvm2::runtime::wasm_threads::wait_domain domain;
 uwvm2::runtime::wasm_threads::execution_scope scope{domain,{}};
 std::size_t index{};
 for(unsigned op=0;op!=0x4f;++op) {
  auto d=atom::describe_atomic_instruction(op);if(d.kind==atom::atomic_instruction_kind::invalid)continue;
  std::array<std::uint8_t,8> input{9};
  UWVM2TEST_REQUIRE(memtag_test_host_copy(memory,8,input.data(),input.size(),true));
  auto r=interpreter_runner<Option>::run(compiled.local_funcs.index_unchecked(index),
    prepared.mod->local_defined_function_vec_storage.index_unchecked(index),args,nullptr,nullptr);
  ++index;
  std::uint64_t result{};std::memcpy(&result,r.results.data(),r.results.size());
  std::uint64_t want=(op==1 || op==2)?1:(op<=3 || !d.has_result)?0:9;
  UWVM2TEST_REQUIRE(result==want);
  unsigned value=9;
  switch(d.kind) {
   case atom::atomic_instruction_kind::store:case atom::atomic_instruction_kind::exchange:case atom::atomic_instruction_kind::compare_exchange:value=5;break;
   case atom::atomic_instruction_kind::add:value=14;break;case atom::atomic_instruction_kind::sub:value=4;break;
   case atom::atomic_instruction_kind::and_:value=1;break;case atom::atomic_instruction_kind::or_:value=13;break;
   case atom::atomic_instruction_kind::xor_:value=12;break;default:break;
  }
  std::uint8_t observed{};
  UWVM2TEST_REQUIRE(memtag_test_host_copy(memory,8,&observed,1));
  UWVM2TEST_REQUIRE(observed==value);
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
 std::puts("PASS all 67 atomic opcodes: memory32/memory64, loop/tail, MTE enabled/disabled, load/store/RMW/wait/notify/fence");
 return 0;
}
