#include <uwvm2/uwvm/wasm/feature/feature.h>
#include <uwvm2/extension/memtag/usage.h>
#include <array>
#include <uwvm2/validation/standard/wasm3/impl.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
namespace feature=uwvm2::uwvm::wasm::feature;
namespace parser=uwvm2::parser::wasm;
namespace validation=uwvm2::validation::standard::wasm3;
using error_code=uwvm2::validation::error::code_validation_error_code;
static void require(bool value, unsigned line)
{ if(!value) {std::fprintf(stderr,"memtag validator failure at %u\n",line);std::abort();} }
#define REQUIRE(x) require(static_cast<bool>(x),__LINE__)
int main()
{
    // Two real declarations: memory zero is memory32, memory one is memory64.
    constexpr unsigned char module_bytes[]{0,97,115,109,1,0,0,0,
        1,4,1,96,0,0,3,2,1,0,5,5,2,0,1,4,1,10,4,1,2,0,11};
    feature::wasm_binfmt_ver1_feature_parameter_storage_t parameters{};
    auto& policy{feature::wasm_binfmt_ver1_wasm1p1_parameter(parameters)};
    policy.disable_memory64=false;policy.disable_multi_memory=false;
    parser::base::error_impl parse_error{};
    auto const* first{reinterpret_cast<std::byte const*>(module_bytes)};
    auto module{feature::binfmt_ver1_handler(first,first+sizeof(module_bytes),parse_error,parameters)};
    REQUIRE(parse_error.err_code==parser::base::wasm_parse_error_code::ok);
    struct signature {unsigned opcode, args, results;};
    constexpr signature opcodes[]{
        {0x20,0,1},{0x21,1,1},{0x22,2,1},{0x23,0,1},{0x24,0,1},
        {0x25,2,1},{0x26,2,1},{0x27,1,1},{0x28,1,1},{0x29,2,1},
        {0x2a,2,1},{0x2b,2,0},{0x2c,2,0},{0x2d,1,1},{0x2e,2,1},
        {0x2f,2,1},{0x33,3,1},{0x36,3,1},{0x37,4,1},{0x38,4,1}};
    auto validate{[&](std::vector<std::byte> const& bytes)
    {
        uwvm2::validation::error::code_validation_error_impl error{};
        try {validation::validate_code(validation::wasm3_code_version{},module,0,
            bytes.data(),bytes.data()+bytes.size(),error,parameters);}
        catch(fast_io::error const&) {REQUIRE(error.err_code!=error_code::ok);}
        return error;
    }};
    unsigned cases{};
    for(auto const instruction:opcodes)
    {
        for(unsigned memory{};memory!=2u;++memory)
        {
            std::vector<std::byte> bytes{};
            for(unsigned arg{};arg!=instruction.args;++arg)
            {bytes.push_back(std::byte(memory==0u?0x41u:0x42u));bytes.push_back(std::byte{});}
            bytes.push_back(std::byte{0xfc});bytes.push_back(std::byte(instruction.opcode));bytes.push_back(std::byte(memory));
            if(instruction.results) {bytes.push_back(std::byte{0x1a});}
            bytes.push_back(std::byte{0x0b});
            policy.disable_memtag_extension=false;
            REQUIRE(validate(bytes).err_code==error_code::ok);++cases;
            {
                std::array<uwvm2::extension::memtag::memory_usage,2> uses{};
                uwvm2::extension::memtag::usage_sink sink{uses};
                uwvm2::validation::error::code_validation_error_impl error{};
                validation::validate_code_to_sink(validation::wasm3_code_version{},module,0,
                    bytes.data(),bytes.data()+bytes.size(),error,parameters,sink);
                REQUIRE(error.err_code==error_code::ok && sink.valid && uses[memory].used && !uses[1u-memory].used);
                REQUIRE(uses[memory].width == (memory==0u ? uwvm2::extension::memtag::address_width::i32 :
                    uwvm2::extension::memtag::address_width::i64));
                REQUIRE(uses[memory].has_undefined_placeholder == (instruction.opcode==0x33u));++cases;
            }

            policy.disable_memtag_extension=true;
            auto const disabled{validate(bytes)};
            REQUIRE(disabled.err_code==error_code::wasm1p1_feature_required);
            REQUIRE(disabled.err_selectable.wasm1p1_feature_required.feature==parser::base::wasm1p1_feature_kind::memtag_extension);++cases;
            policy.disable_memtag_extension=false;
            auto const index_offset{instruction.args*2u+2u};
            bytes[index_offset]=std::byte{3};
            REQUIRE(validate(bytes).err_code==error_code::illegal_memory_index);++cases;
            bytes[index_offset]=std::byte(memory);
            if(instruction.args!=0u)
            {
                bytes[0]=std::byte(memory==0u?0x42u:0x41u);
                REQUIRE(validate(bytes).err_code==error_code::numeric_operand_type_mismatch);++cases;
                bytes.erase(bytes.begin(),bytes.begin()+2);
                REQUIRE(validate(bytes).err_code!=error_code::ok);++cases;
            }
            std::vector<std::byte> truncated{std::byte{0x00},std::byte{0xfc},std::byte(instruction.opcode),std::byte{0x80}};
            REQUIRE(validate(truncated).err_code!=error_code::ok);++cases;
            std::vector<std::byte> overlong{std::byte{0x00},std::byte{0xfc},std::byte(instruction.opcode),
                std::byte{0x80},std::byte{0x80},std::byte{0x80},std::byte{0x80},std::byte{0x10},std::byte{0x0b}};
            REQUIRE(validate(overlong).err_code!=error_code::ok);++cases;
        }
    }
    for(unsigned gap:{0x30u,0x31u,0x32u,0x34u,0x35u,0x39u})
    {REQUIRE(validate({std::byte{0xfc},std::byte(gap),std::byte{0},std::byte{0x0b}}).err_code!=error_code::ok);++cases;}
    std::printf("memtag validator: %u cases passed\n",cases);
}
