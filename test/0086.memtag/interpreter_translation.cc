#include "../0013.uwvm_int/strict/uwvm_int_translate_strict_common.h"
namespace {
using namespace uwvm2test::uwvm_int_strict;
namespace mt=uwvm2::extension::memtag;
byte_vec make(bool wide,bool placeholder=false,bool padded=false) {
 module_builder b{};b.has_memory=b.memory_has_max=true;b.memory_min=1;b.memory_max=2;b.memory_address64=wide;
 auto address_type=wide?k_val_i64:k_val_i32;
 func_body body{};
 auto number=[&](std::int64_t n){append_u8(body.code,wide?0x42:0x41);if(wide)append_i64_leb(body.code,n);else append_i32_leb(body.code,static_cast<std::int32_t>(n));};
 auto tagop=[&](mt::opcode op){
  append_u8(body.code,mt::opcode_prefix);
  auto const word{mt::encode_opcode(op)};
  if(padded)for(unsigned i{};i!=5u;++i)append_u8(body.code,((word>>(7u*i))&127u)|(i!=4u?128u:0u));
  else append_u32_leb(body.code,word);
  append_u8(body.code,0);
 };
 if(placeholder){
  number(0);number(0);number(0);tagop(mt::opcode::add);append_u8(body.code,0x1a);
 } else {
  number(0);number(3);tagop(mt::opcode::insert);number(32);tagop(mt::opcode::store);
  number(0);number(3);tagop(mt::opcode::insert);append_u8(body.code,0x41);append_i32_leb(body.code,123);
  append_u8(body.code,0x36);append_u8(body.code,2);append_u8(body.code,1);
  number(0);number(3);tagop(mt::opcode::insert);append_u8(body.code,0x28);append_u8(body.code,2);append_u8(body.code,1);
 }
 if(placeholder){append_u8(body.code,0x41);append_u8(body.code,0);}
 append_u8(body.code,0xb);b.add_func({{},{k_val_i32}},std::move(body));
 func_body status{};append_u8(status.code,mt::opcode_prefix);
 auto const word{mt::encode_opcode(mt::opcode::status)};
 if(padded)for(unsigned i{};i!=5u;++i)append_u8(status.code,((word>>(7u*i))&127u)|(i!=4u?128u:0u));
 else append_u32_leb(status.code,word);
 append_u8(status.code,0);append_u8(status.code,0xb);
 b.add_func({{},{address_type}},std::move(status));return b.build();
}
template<optable::uwvm_interpreter_translate_option_t Option>
int check(bool wide,bool enabled,bool placeholder=false,bool padded=false) {
 auto wasm=make(wide,placeholder,padded);auto features=make_wasm1p1_feature_parameter();
 auto& p=uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1p1_parameter(features);
 p.disable_memtag_extension=false;p.disable_memory64=false;
 p.cli_mode=uwvm2::parser::wasm::standard::wasm1p1::features::wasm_feature_cli_mode::scoped;
 uwvm2::uwvm::runtime::runtime_mode::runtime_disable_memtag=!enabled;
 auto prepared=prepare_runtime_from_wasm(wasm,u8"memtag-interpreter",{},features);
 auto& memory=const_cast<uwvm2::object::memory::linear::native_memory_t&>(prepared.mod->local_defined_memory_vec_storage.index_unchecked(0).memory);
 if(enabled && !memory.memtag_shadow.enabled())UWVM2TEST_REQUIRE(memory.enable_memtag(wide?mt::address_width::i64:mt::address_width::i32,false)==mt::storage_error::none);
 if constexpr(uwvm2::object::memory::linear::native_memory_t::can_mmap) {
  if(mt::hardware_mte::supported())UWVM2TEST_REQUIRE(!memory.memtag_shadow.hardware_active());
 }
 uwvm2::validation::error::code_validation_error_impl error{};optable::compile_option options{};options.execute_memtag=enabled;
 bool rejected{};
 try {
  auto compiled=compiler::compile_all_from_uwvm_single_func<Option>(*prepared.mod,options,error,&features);
  UWVM2TEST_REQUIRE(!placeholder);
  byte_vec args{};
  auto r=interpreter_runner<Option>::run(compiled.local_funcs.index_unchecked(0),
    prepared.mod->local_defined_function_vec_storage.index_unchecked(0),args,nullptr,nullptr);
  UWVM2TEST_REQUIRE(r.results.size()==4);std::int32_t value{};std::memcpy(&value,r.results.data(),4);UWVM2TEST_REQUIRE(value==123);
  auto status=interpreter_runner<Option>::run(compiled.local_funcs.index_unchecked(1),
    prepared.mod->local_defined_function_vec_storage.index_unchecked(1),args,nullptr,nullptr);
  std::uint64_t result{};std::memcpy(&result,status.results.data(),status.results.size());
  UWVM2TEST_REQUIRE(result==(enabled?static_cast<unsigned>(mt::runtime_status::software_full):0u));
 } catch(fast_io::error const&) {
  rejected=true;
  if(!placeholder)std::fprintf(stderr,"MTE translation error %u\n",unsigned(error.err_code));
 }
 UWVM2TEST_REQUIRE(rejected==placeholder);
 return 0;
}
}
int main() {
 install_unexpected_traps();
 constexpr optable::uwvm_interpreter_translate_option_t loop{.is_tail_call=false};
 constexpr auto ring=make_tailcall_scalar4_merged_opt<2>();
 for(bool wide:{false,true})for(bool enabled:{false,true})for(bool padded:{false,true}) {
  UWVM2TEST_REQUIRE(check<loop>(wide,enabled,false,padded)==0);UWVM2TEST_REQUIRE(check<ring>(wide,enabled,false,padded)==0);
 }
 // Undefined add is rejected by startup admission before this runner; the
 // validator and operation-contract tests cover that reserved instruction.
 std::puts("PASS interpreter MTE translation: memory32/memory64, loop/tail ring, enabled/disabled, scalar store/load");
 return 0;
}
