#include "../0013.uwvm_int/strict/uwvm_int_translate_strict_common.h"
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/runtime/compiler/uwvm_int/threaded/engine.h>
namespace {
using namespace uwvm2test::uwvm_int_strict;
namespace sh=uwvm2::runtime::compiler::shared;
namespace ti=uwvm2::runtime::compiler::uwvm_int::threaded;
namespace mt=uwvm2::extension::memtag;
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
  if(descriptor.operation==mt::opcode::add)continue;
  func_body f{};for(unsigned i{};i!=descriptor.operand_count;++i){append_u8(f.code,0x41);append_u8(f.code,0);}
  append_u8(f.code,0xfc);append_u8(f.code,static_cast<unsigned>(descriptor.operation));append_u8(f.code,0);
  if(descriptor.result_count)append_u8(f.code,0x1a);
  append_u8(f.code,0xb);builder.add_func({{},{}},std::move(f));++count;
 }
 return builder.build();
}
}
int main() {
 install_unexpected_traps();
 auto features=make_wasm1p1_feature_parameter();
 auto& p=uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1p1_parameter(features);
 p.disable_memtag_extension=false;p.disable_relaxed_simd=false;
 auto wasm=make();auto prepared=prepare_runtime_from_wasm(wasm,u8"threaded-decode",{},features);
 std::size_t charge{};ti::limits budget{};budget.instruction_bytes=128u<<20;
 for(auto const& function:prepared.mod->local_defined_function_vec_storage){
  ti::function_program out{};
  auto const status=ti::detail::lower(*prepared.mod,function,out,budget,charge,
   reinterpret_cast<std::uintptr_t>(function.wasm_code_ptr->body.expr_begin));
  UWVM2TEST_REQUIRE(status==ti::status::ok);
  UWVM2TEST_REQUIRE(!out.instructions.empty());
 }
 UWVM2TEST_REQUIRE(count>=255);
 std::printf("PASS shared decoder: %zu validated SIMD/marker functions, complete engine template instantiation\n",count);
}
