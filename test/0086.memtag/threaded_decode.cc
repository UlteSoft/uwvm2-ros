#include "../0013.uwvm_int/strict/uwvm_int_translate_strict_common.h"
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/runtime/compiler/uwvm_int/threaded/engine.h>
namespace {
using namespace uwvm2test::uwvm_int_strict;
namespace sh=uwvm2::runtime::compiler::shared;
namespace ti=uwvm2::runtime::compiler::uwvm_int::threaded;
namespace mt=uwvm2::extension::memtag;
namespace md=uwvm2::runtime::memtag::deferred;
std::size_t count{};
byte_vec make() {
 module_builder builder{};builder.has_memory=builder.memory_has_max=true;builder.memory_min=1;builder.memory_max=2;
 for(unsigned opcode{};opcode!=0x114u;++opcode) {
  static_cast<void>(sh::visit_wasm1p1_simd_instruction(static_cast<sh::simd_code>(opcode),
   [&]<sh::simd_code Op,sh::wasm1p1_simd_instruction_kind Kind,
        sh::wasm1p1_simd_scalar_kind Scalar,std::size_t Lanes,unsigned Align>() {
    using kind=sh::wasm1p1_simd_instruction_kind;using scalar=sh::wasm1p1_simd_scalar_kind;
    func_body f{};
    auto vector=[&]{append_u8(f.code,0xfd);append_u8(f.code,12);for(unsigned i{};i!=16;++i)append_u8(f.code,i+1);};
    auto number=[&]{append_u8(f.code,0x41);append_u8(f.code,0);};
    auto scalar_value=[&]{
     if constexpr(Scalar==scalar::i32)number();
     else if constexpr(Scalar==scalar::i64){append_u8(f.code,0x42);append_u8(f.code,0);}
     else if constexpr(Scalar==scalar::f32){append_u8(f.code,0x43);for(unsigned i{};i!=4;++i)append_u8(f.code,0);}
     else if constexpr(Scalar==scalar::f64){append_u8(f.code,0x44);for(unsigned i{};i!=8;++i)append_u8(f.code,0);}
    };
    if constexpr(Kind==kind::splat)scalar_value();
    else if constexpr(Kind==kind::replace_lane){vector();scalar_value();}
    else if constexpr(Kind==kind::unary||Kind==kind::test||Kind==kind::extract_lane)vector();
    else if constexpr(Kind==kind::binary||Kind==kind::shuffle){vector();vector();}
    else if constexpr(Kind==kind::ternary){vector();vector();vector();}
    else if constexpr(Kind==kind::shift){vector();number();}
    else if constexpr(Kind==kind::memory_load||Kind==kind::memory_store){
     number();if constexpr(Kind==kind::memory_store||Lanes!=0)vector();
    }
    append_u8(f.code,0xfd);append_u32_leb(f.code,opcode);
    if constexpr(Kind==kind::constant||Kind==kind::shuffle)for(unsigned i{};i!=16;++i)append_u8(f.code,i);
    if constexpr(Kind==kind::memory_load||Kind==kind::memory_store){
     append_u8(f.code,Align);append_u8(f.code,0);
     if constexpr(Lanes!=0)append_u8(f.code,Lanes-1);
    }
    if constexpr(Kind==kind::extract_lane||Kind==kind::replace_lane)append_u8(f.code,Lanes-1);
    if constexpr(Kind!=kind::memory_store)append_u8(f.code,0x1a);
    append_u8(f.code,0xb);
    builder.add_func({{},{}},std::move(f));++count;return true;
   }));
 }
 for(auto const& descriptor:mt::instructions) {
 for(bool padded:{false,true}) {
  if(descriptor.operation==mt::opcode::add)continue;
  func_body f{};for(unsigned i{};i!=descriptor.operand_count;++i){append_u8(f.code,0x41);append_u8(f.code,0);}
  append_u8(f.code,mt::opcode_prefix);
  auto const word{mt::encode_opcode(descriptor.operation)};
  if(padded)for(unsigned i{};i!=5u;++i)append_u8(f.code,((word>>(7u*i))&127u)|(i!=4u?128u:0u));
  else append_u32_leb(f.code,word);
  append_u8(f.code,0);
  if(descriptor.result_count)append_u8(f.code,0x1a);
  append_u8(f.code,0xb);builder.add_func({{},{}},std::move(f));++count;
 }
 }
 // The padded constant makes instruction indices differ from byte offsets.
 // This last function also sits well after the start of the actual module.
 func_body offsets{};
 for(unsigned byte:{0x01u,0x41u,0x80u,0x80u,0x80u,0x80u,0x00u,
                    0x1au,0x3fu,0x00u,0x1au,0x0bu})append_u8(offsets.code,byte);
 builder.add_func({{},{}},std::move(offsets));++count;
 return builder.build();
}
int check_synthetic_prefixes() {
 // The finite threaded decoder gives atomics private dispatch ids, including
 // fence (03), loads (10..16), stores (17..1d), and RMW operations (1e..4e).
 // Hardware-async delivery must still see the original FE boundary for all.
 for(unsigned sub{};sub!=0x4fu;++sub){
  if(sub>3u&&sub<0x10u)continue;
  ti::instruction ins{};ins.opcode=0x100u+sub;
  UWVM2TEST_REQUIRE(ins.primary_opcode()==0xfeu);
  UWVM2TEST_REQUIRE(md::draining_opcode(ins.primary_opcode()));
 }
 // FC saturation and FC bulk occupy distinct internal ranges, but both must
 // retain the standard primary byte instead of turning into MTE markers.
 for(unsigned sub{};sub!=12u;++sub){
  ti::instruction ins{};ins.opcode=(sub<8u?0x200u:0x300u)+sub;
  UWVM2TEST_REQUIRE(ins.primary_opcode()==0xfcu);
  UWVM2TEST_REQUIRE(!md::draining_opcode(ins.primary_opcode()));
 }
 for(unsigned opcode:{0x01u,0x28u,0x36u,0x3fu,0x41u,0x6au}){
  ti::instruction ins{};ins.opcode=opcode;
  UWVM2TEST_REQUIRE(ins.primary_opcode()==opcode);
  UWVM2TEST_REQUIRE(!md::draining_opcode(ins.primary_opcode()));
 }
 ti::instruction grow{};grow.opcode=0x40u;
 UWVM2TEST_REQUIRE(grow.primary_opcode()==0x40u);
 UWVM2TEST_REQUIRE(md::draining_opcode(grow.primary_opcode()));
 return 0;
}
}
int main() {
#if !defined(UWVM2TEST_STRICT_NO_INTERPRETER)
 install_unexpected_traps();
#endif
 UWVM2TEST_REQUIRE(check_synthetic_prefixes()==0);
 auto features=make_wasm1p1_feature_parameter();
 auto& p=uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1p1_parameter(features);
 p.disable_memtag_extension=false;p.disable_relaxed_simd=false;
 auto wasm=make();auto prepared=prepare_runtime_from_wasm(wasm,u8"threaded-decode",{},features);
 UWVM2TEST_REQUIRE(prepared.mod->imported_function_vec_storage.size()==0u);
 std::size_t charge{};ti::limits budget{};budget.instruction_bytes=128u<<20;
 auto const file_base{reinterpret_cast<std::uintptr_t>(wasm.data())};
 std::size_t functions{},mte_instructions{},simd_instructions{},located_instructions{};
 for(auto const& function:prepared.mod->local_defined_function_vec_storage){
  ti::function_program out{};
  auto const status=ti::detail::lower(*prepared.mod,function,out,budget,charge,file_base);
  UWVM2TEST_REQUIRE(status==ti::status::ok);
  UWVM2TEST_REQUIRE(!out.instructions.empty());
  auto const& body{function.wasm_code_ptr->body};
  auto const expression_begin{reinterpret_cast<std::uintptr_t>(body.expr_begin)-file_base};
  auto const expression_end{reinterpret_cast<std::uintptr_t>(body.code_end)-file_base};
  UWVM2TEST_REQUIRE(expression_begin>0u&&expression_begin<expression_end&&expression_end<=wasm.size());
  UWVM2TEST_REQUIRE(out.expression_file_offset==expression_begin);
  UWVM2TEST_REQUIRE(out.instructions.front().offset==expression_begin);
  UWVM2TEST_REQUIRE(out.expression_offset(out.instructions.front().offset)==0u);
  UWVM2TEST_REQUIRE(out.expression_offset(expression_begin-1u)==UINT64_MAX);
  UWVM2TEST_REQUIRE(out.expression_offset(0u)==UINT64_MAX);
  UWVM2TEST_REQUIRE(out.instructions.back().offset==expression_end-1u);
  std::size_t previous{};
  for(auto const& ins:out.instructions){
   UWVM2TEST_REQUIRE(ins.offset>=expression_begin&&ins.offset<expression_end);
   UWVM2TEST_REQUIRE(ins.offset>previous);previous=ins.offset;
   UWVM2TEST_REQUIRE(out.expression_offset(ins.offset)==ins.offset-expression_begin);
   // Independently compare each private dispatch id with the original byte in
   // the immutable module. Neither diagnostic conversion nor prefix recovery
   // may alter the file coordinates consumed by debugger/lease maps.
   auto const raw{std::to_integer<unsigned>(wasm[ins.offset])};
   UWVM2TEST_REQUIRE(ins.primary_opcode()==raw);
   if(raw==mt::opcode_prefix){
    ++mte_instructions;UWVM2TEST_REQUIRE(md::draining_opcode(ins.primary_opcode()));
   }
   if(raw==0xfdu){
    ++simd_instructions;UWVM2TEST_REQUIRE(!md::draining_opcode(ins.primary_opcode()));
   }
   ++located_instructions;
  }
  if(++functions==count){
   constexpr std::array<std::uint64_t,6u> expected{0u,1u,7u,8u,10u,11u};
   UWVM2TEST_REQUIRE(out.instructions.size()==expected.size());
   for(std::size_t i{};i!=expected.size();++i)
    UWVM2TEST_REQUIRE(out.expression_offset(out.instructions[i].offset)==expected[i]);
  }
 }
 UWVM2TEST_REQUIRE(count>=255);
 UWVM2TEST_REQUIRE(functions==count&&mte_instructions==38u&&simd_instructions>200u);
 std::printf("PASS shared decoder: %zu validated functions, %zu byte locations, %zu MTE and %zu SIMD primary bytes; file/debug offsets retained, expression offsets checked, atomic/FC boundaries checked\n",
  count,located_instructions,mte_instructions,simd_instructions);
}
