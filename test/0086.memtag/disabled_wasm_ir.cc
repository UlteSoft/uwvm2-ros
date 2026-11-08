// Production Wasm parser/initializer/typed translator -> LLVM IR. This test
// does not execute generated code; link with the matching production runtime.
#define UWVM2TEST_STRICT_NO_INTERPRETER 1
#include "../0013.uwvm_int/strict/uwvm_int_translate_strict_common.h"
#include <uwvm2/runtime/compiler/llvm_jit/compile_all_from_uwvm/impl.h>
#include <llvm/IR/Verifier.h>
#include <string>
namespace strict = uwvm2test::uwvm_int_strict;
namespace compiler = uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm;
namespace mt = uwvm2::extension::memtag;
static void require(bool value,unsigned line)
{ if(!value) { std::fprintf(stderr,"FAIL memtag Wasm IR line %u\n",line);std::abort(); } }
#define REQUIRE(x) require(static_cast<bool>(x),__LINE__)
strict::byte_vec fixture(bool address64,bool placeholder=false)
{
    strict::module_builder module; module.has_memory=true;module.memory_min=1;
    module.memory_address64=address64;
    for(unsigned raw{};raw!=256u;++raw)
    {
        auto const* descriptor{mt::find_instruction(raw)};
        if(descriptor==nullptr || (raw==0x33u)!=placeholder) {continue;}
        strict::func_type type{};
        auto const value_type{address64 ? strict::k_val_i64 : strict::k_val_i32};
        for(unsigned i{};i!=descriptor->operand_count;++i){type.params.push_back(value_type);}
        if(descriptor->result_count){type.results.push_back(value_type);}
        strict::func_body body;
        for(unsigned i{};i!=descriptor->operand_count;++i)
        {strict::append_u8(body.code,0x20u);strict::append_u8(body.code,static_cast<std::uint8_t>(i));}
        strict::append_u8(body.code,0xfcu);strict::append_u8(body.code,static_cast<std::uint8_t>(raw));
        strict::append_u8(body.code,0u);strict::append_u8(body.code,0x0bu);
        module.add_func(type,std::move(body));
    }
    return module.build();
}
int main()
{
    for(bool address64:{false,true})
    {
        auto bytes{fixture(address64)};
        auto features{strict::make_wasm1p1_feature_parameter()};
        auto& policy{uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1p1_parameter(features)};
        policy.disable_memory64=false;
        auto prepared{strict::prepare_runtime_from_wasm(bytes,u8"memtag-disabled-ir",{},features)};
        compiler::compile_option options{};
        options.validator_feature_parameter=&features;
        options.compilation_mode=compiler::llvm_jit_compilation_mode::full;
        options.emit_call_stack_frames=false;
        options.execute_memtag=false;
        uwvm2::validation::error::code_validation_error_impl error{};
        auto compiled{compiler::compile_all_from_uwvm(*prepared.mod,options,error,0)};
        REQUIRE(error.err_code==uwvm2::validation::error::code_validation_error_code::ok);
        REQUIRE(compiled.llvm_jit_module.emitted);
        REQUIRE(!llvm::verifyModule(*compiled.llvm_jit_module.llvm_module,&llvm::errs()));
        REQUIRE(compiled.local_funcs.size()==19u);
        std::string text;llvm::raw_string_ostream stream{text};
        compiled.llvm_jit_module.llvm_module->print(stream,nullptr);
        REQUIRE(text.find("memtag_shadow")==std::string::npos);
        std::printf("PASS production Wasm -> LLVM disabled MTE: 19 defined instructions, memory%u\n",address64?64u:32u);
    }
}
