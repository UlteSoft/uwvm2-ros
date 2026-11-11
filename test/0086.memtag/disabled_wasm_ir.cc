// Production Wasm parser/initializer/typed translator -> LLVM IR. This test
// does not execute generated code; link with the matching production runtime.
#define UWVM2TEST_STRICT_NO_INTERPRETER 1
#include "../0013.uwvm_int/strict/uwvm_int_translate_strict_common.h"
#include <uwvm2/runtime/compiler/llvm_jit/compile_all_from_uwvm/impl.h>
#include <llvm/IR/Verifier.h>
#include <string>
#include <array>
#include <vector>
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
        strict::append_u8(body.code,mt::opcode_prefix);strict::append_u32_leb(body.code,mt::encode_opcode(descriptor->operation));
        strict::append_u8(body.code,0u);strict::append_u8(body.code,0x0bu);
        module.add_func(type,std::move(body));
    }
    return module.build();
}
static void disabled_effects()
{
    // All defined opcodes lower to identities/constants/subtraction or their
    // required ordinary zero fill. Tag helpers and hidden flag tests are not
    // allowed even before the LLVM optimizer runs.
    for(unsigned bits:{32u,64u})
    for(unsigned raw{};raw!=256u;++raw)
    {
        auto const* descriptor{mt::find_instruction(raw)};
        if(!descriptor || descriptor->operation==mt::opcode::add) {continue;}
        llvm::LLVMContext context;llvm::Module module("disabled-effect",context);
        llvm::IRBuilder<> b(context);auto* type=b.getIntNTy(bits);
        std::vector<llvm::Type*> parameters(descriptor->operand_count,type);
        auto* function=llvm::Function::Create(llvm::FunctionType::get(b.getVoidTy(),parameters,false),
            llvm::Function::ExternalLinkage,"test",module);
        b.SetInsertPoint(llvm::BasicBlock::Create(context,"entry",function));
        std::vector<llvm::Value*> values;for(auto& arg:function->args()){values.push_back(&arg);}
        unsigned zeroes{};
        auto result=compiler::details::emit_disabled_memtag_ir(b,descriptor->operation,type,values,
            [&](llvm::Value* address,llvm::Value* length) noexcept
            { REQUIRE(address==values[0] && length==values[1]);++zeroes;return true; });
        REQUIRE(result.valid);REQUIRE(zeroes==unsigned(descriptor->zeroes_memory));
        auto operation=descriptor->operation;
        if(operation==mt::opcode::sub)
        {
            auto const* subtraction{llvm::dyn_cast<llvm::BinaryOperator>(result.value)};
            REQUIRE(subtraction && subtraction->getOpcode()==llvm::Instruction::Sub);
            REQUIRE(subtraction->getOperand(0u)==values[0u] && subtraction->getOperand(1u)==values[1u]);
            REQUIRE(b.GetInsertBlock()->size()==1u);
        }
        else
        {
            REQUIRE(b.GetInsertBlock()->empty());
            if(operation==mt::opcode::status || operation==mt::opcode::extract || operation==mt::opcode::tagbits)
            { REQUIRE(llvm::cast<llvm::ConstantInt>(result.value)->isZero()); }
            else if(operation==mt::opcode::startbit)
            { REQUIRE(llvm::cast<llvm::ConstantInt>(result.value)->getZExtValue()==bits); }
            else { REQUIRE(result.value==(descriptor->result_count?values[0]:nullptr)); }
        }
    }
}
int main()
{
    disabled_effects();
    ::std::array<::std::array<::std::size_t,128u>,2u> baseline{};
    for(bool log:{false,true})
    for(bool address64:{false,true})
    {
        mt::selected_report_action.store(log?mt::report_action::silent_log:mt::report_action::crash);
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
        ::std::array<::std::size_t,128u> histogram{};
        for(auto const& function:*compiled.llvm_jit_module.llvm_module)
        for(auto const& block:function)
        for(auto const& instruction:block)
        {
            REQUIRE(instruction.getOpcode()<histogram.size());
            ++histogram[instruction.getOpcode()];
        }
        if(!log) { baseline[address64]=histogram; }
        else { REQUIRE(histogram==baseline[address64]); }
        std::printf("PASS production Wasm -> LLVM disabled MTE: 19 defined instructions, memory%u\n",address64?64u:32u);
    }
}
