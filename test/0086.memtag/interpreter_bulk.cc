#include "../0013.uwvm_int/strict/uwvm_int_translate_strict_common.h"
#include "host_snapshot_copy.h"
namespace {
using namespace uwvm2test::uwvm_int_strict;
namespace mt=uwvm2::extension::memtag;
byte_vec make(bool destination64,bool source64,bool shared,bool mixed) {
 module_builder b{};b.has_memory=b.memory_has_max=true;b.memory_shared=shared;
 b.memory_min=1;b.memory_max=2;b.memory_address64=destination64;
 b.extra_memories.push_back({1,2,true,shared,source64});
 passive_data_segment segment;for(unsigned i{};i!=64;++i)append_u8(segment.bytes,0x80+i);
 b.passive_datas.push_back(std::move(segment));
 func_body body{};
 auto number=[&](std::int64_t n,bool wide){append_u8(body.code,wide?0x42:0x41);if(wide)append_i64_leb(body.code,n);else append_i32_leb(body.code,static_cast<std::int32_t>(n));};
 auto tag=[&](unsigned memory,unsigned pointer){bool wide=memory?source64:destination64;number(pointer,wide);if(!mixed || !memory){number(memory?4:3,wide);append_u8(body.code,0xfc);append_u8(body.code,static_cast<unsigned>(mt::opcode::insert));append_u8(body.code,memory);}};
 auto numeric=[&](unsigned op){append_u8(body.code,0xfc);append_u32_leb(body.code,op);};
 for(unsigned m{};m!=2;++m)if(!mixed || !m){tag(m,0);number(256,m?source64:destination64);numeric(static_cast<unsigned>(mt::opcode::store));append_u8(body.code,m);}
 tag(0,0);number(0x55,false);number(64,destination64);numeric(11);append_u8(body.code,0);
 tag(0,24);tag(0,8);number(48,destination64);numeric(10);append_u8(body.code,0);append_u8(body.code,0);
 tag(0,129);number(1,false);number(31,false);numeric(8);append_u8(body.code,0);append_u8(body.code,0);
 tag(1,0);number(0x66,false);number(64,source64);numeric(11);append_u8(body.code,1);
 tag(0,33);tag(1,7);number(45,destination64&&source64);numeric(10);append_u8(body.code,0);append_u8(body.code,1);
 tag(1,80);tag(0,35);number(43,destination64&&source64);numeric(10);append_u8(body.code,1);append_u8(body.code,0);
 number(0,false);append_u8(body.code,0xb);b.add_func({{},{k_val_i32}},std::move(body));return b.build();
}
template<optable::uwvm_interpreter_translate_option_t Option>
int check(bool destination64,bool source64,bool enabled,bool shared,bool mixed) {
 auto wasm=make(destination64,source64,shared,mixed);auto features=make_wasm1p1_feature_parameter();
 auto& p=uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1p1_parameter(features);
 p.disable_memtag_extension=false;p.disable_memory64=false;p.disable_threads=false;p.disable_multi_memory=false;
 p.cli_mode=uwvm2::parser::wasm::standard::wasm1p1::features::wasm_feature_cli_mode::scoped;
 uwvm2::uwvm::runtime::runtime_mode::runtime_disable_memtag=!enabled;
 auto prepared=prepare_runtime_from_wasm(wasm,u8"memtag-interpreter-bulk",{},features);
 auto owner=[&](unsigned m)->uwvm2::object::memory::linear::native_memory_t& {
  return const_cast<uwvm2::object::memory::linear::native_memory_t&>(prepared.mod->local_defined_memory_vec_storage.index_unchecked(m).memory);
 };
 if(enabled)for(unsigned m{};m!=2;++m)if((!mixed||!m) && !owner(m).memtag_shadow.enabled())
  UWVM2TEST_REQUIRE(owner(m).enable_memtag((m?source64:destination64)?mt::address_width::i64:mt::address_width::i32,shared)==mt::storage_error::none);
 uwvm2::validation::error::code_validation_error_impl error{};optable::compile_option options{};options.execute_memtag=enabled;
 auto compiled=compiler::compile_all_from_uwvm_single_func<Option>(*prepared.mod,options,error,&features);
 byte_vec args{};
 auto r=interpreter_runner<Option>::run(compiled.local_funcs.index_unchecked(0),
    prepared.mod->local_defined_function_vec_storage.index_unchecked(0),args,nullptr,nullptr);
 std::array<std::uint8_t,256> want0{},want1{};
 std::fill_n(want0.begin(),64,0x55);
 std::memmove(want0.data()+24,want0.data()+8,48);
 for(unsigned i{};i!=31;++i)want0[129+i]=static_cast<std::uint8_t>(0x81+i);
 std::fill_n(want1.begin(),64,0x66);
 std::memcpy(want0.data()+33,want1.data()+7,45);
 std::memcpy(want1.data()+80,want0.data()+35,43);
 std::array<std::uint8_t,256> observed0{},observed1{};
 UWVM2TEST_REQUIRE(memtag_test_host_copy(owner(0),0,observed0.data(),observed0.size()));
 UWVM2TEST_REQUIRE(memtag_test_host_copy(owner(1),0,observed1.data(),observed1.size()));
 UWVM2TEST_REQUIRE(observed0==want0 && observed1==want1);
 return 0;
}
}
int main(){
 install_unexpected_traps();constexpr optable::uwvm_interpreter_translate_option_t loop{.is_tail_call=false};
 constexpr auto ring=make_tailcall_scalar4_merged_opt<2>();
 for(bool destination64:{false,true})for(bool source64:{false,true})for(bool enabled:{false,true})
 for(bool shared:{false,true})for(bool mixed:{false,true}) {
  UWVM2TEST_REQUIRE(check<loop>(destination64,source64,enabled,shared,mixed)==0);
  UWVM2TEST_REQUIRE(check<ring>(destination64,source64,enabled,shared,mixed)==0);
 }
 std::puts("PASS bulk memory MTE: copy/fill/init, mixed widths and tag modes, shared, overlap, enabled/disabled, loop/tail");
 return 0;
}
