// Production parser/initializer/typed translator regression: each MTE source
// boundary is published once. Link with the matching production runtime.
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
        strict::append_u8(body.code,mt::opcode_prefix);strict::append_u32_leb(body.code,mt::encode_opcode(descriptor->operation));
        strict::append_u8(body.code,0u);strict::append_u8(body.code,0x0bu);
        module.add_func(type,std::move(body));
    }
    return module.build();
}
int main()
{

    for(bool log:{false,true})
    for(bool address64:{false,true})
    {
        mt::selected_report_action.store(log?mt::report_action::silent_log:mt::report_action::crash);
        auto bytes{fixture(address64)};
        auto features{strict::make_wasm1p1_feature_parameter()};
        auto& policy{uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1p1_parameter(features)};
        policy.disable_memory64=false;
        auto prepared{strict::prepare_runtime_from_wasm(bytes,u8"memtag-origin-ir",{},features)};
        compiler::compile_option options{};
        options.validator_feature_parameter=&features;
        options.compilation_mode=compiler::llvm_jit_compilation_mode::full;
        options.emit_call_stack_frames=false;
        options.execute_memtag=true;
        uwvm2::validation::error::code_validation_error_impl error{};
        auto compiled{compiler::compile_all_from_uwvm(*prepared.mod,options,error,0)};
        REQUIRE(error.err_code==uwvm2::validation::error::code_validation_error_code::ok);
        REQUIRE(compiled.llvm_jit_module.emitted);
        REQUIRE(!llvm::verifyModule(*compiled.llvm_jit_module.llvm_module,&llvm::errs()));
        REQUIRE(compiled.local_funcs.size()==19u);

        unsigned boundaries{},duplicates{};
        for(auto const& function:*compiled.llvm_jit_module.llvm_module)
        for(auto const& block:function)
        {
            llvm::CallInst const* previous{};
            for(auto const& instruction:block)
            {
                auto const* call=llvm::dyn_cast<llvm::CallInst>(&instruction);
                if(call && call->arg_size()==4u && call->getArgOperand(0u)->getType()->isIntegerTy(64u) &&
                   call->getArgOperand(3u)->getType()->isIntegerTy(32u))
                {
                    ++boundaries;
                    if(previous && previous->getCalledOperand()==call->getCalledOperand() && previous->arg_size()==4u &&
                       previous->getArgOperand(0u)==call->getArgOperand(0u) && previous->getArgOperand(1u)==call->getArgOperand(1u) &&
                       previous->getArgOperand(2u)==call->getArgOperand(2u) && previous->getArgOperand(3u)==call->getArgOperand(3u))
                    { ++duplicates; }
                }
                previous=call;
            }
        }
        std::printf("origin boundaries=%u duplicates=%u log=%u width=%u\n",boundaries,duplicates,unsigned(log),address64?64u:32u);
        std::fflush(stdout);
        REQUIRE(duplicates==0u);
        if(log) { REQUIRE(boundaries>=19u); }
        std::printf("PASS production Wasm -> LLVM unique MTE origins: 19 defined instructions, memory%u\n",address64?64u:32u);
    }
}
