#include <uwvm2/uwvm/wasm/feature/feature.h>
#include <uwvm2/extension/memtag/usage.h>
#include <array>
#include <uwvm2/validation/standard/wasm3/impl.h>
#include <uwvm2/validation/standard/wasm1p1/impl.h>
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
static void append_u32(std::vector<std::byte>& bytes, std::uint32_t value, bool padded=false)
{
    for(unsigned index{};;++index)
    {
        auto byte{value&127u};value>>=7u;bool const more{value!=0u || (padded && index!=4u)};
        bytes.push_back(static_cast<std::byte>(byte|(more?128u:0u)));if(!more)return;
    }
}
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
    auto validate_wasm1p1{[&](std::vector<std::byte> const& bytes)
    {
        namespace legacy=uwvm2::validation::standard::wasm1p1;
        uwvm2::validation::error::code_validation_error_impl error{};
        try {legacy::validate_code(legacy::wasm1p1_code_version{},module,0,
            bytes.data(),bytes.data()+bytes.size(),error,parameters);}
        catch(fast_io::error const&) {REQUIRE(error.err_code!=error_code::ok);}
        return error;
    }};
    unsigned cases{};
    for(auto const instruction:opcodes)
    {
        if(instruction.opcode==0x33u) { continue; } // Reserved, rejected below.
        for(unsigned memory{};memory!=2u;++memory)
        for(bool padded:{false,true})
        {
            std::vector<std::byte> prefix{std::byte{0xff}};
            append_u32(prefix,0x01010000u|instruction.opcode,padded);
            std::vector<std::byte> bytes{};
            for(unsigned arg{};arg!=instruction.args;++arg)
            {bytes.push_back(std::byte(memory==0u?0x41u:0x42u));bytes.push_back(std::byte{});}
            bytes.insert(bytes.end(),prefix.begin(),prefix.end());bytes.push_back(std::byte(memory));
            if(instruction.results) {bytes.push_back(std::byte{0x1a});}
            bytes.push_back(std::byte{0x0b});
            policy.disable_memtag_extension=false;
            REQUIRE(validate(bytes).err_code==error_code::ok);++cases;
            if(memory==0u) {REQUIRE(validate_wasm1p1(bytes).err_code==error_code::ok);++cases;}
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
            if(memory==0u) {REQUIRE(validate_wasm1p1(bytes).err_code==error_code::wasm1p1_feature_required);++cases;}
            policy.disable_memtag_extension=false;
            auto const index_offset{instruction.args*2u+prefix.size()};
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
            std::vector<std::byte> truncated{std::byte{0x00}};
            truncated.insert(truncated.end(),prefix.begin(),prefix.end());truncated.push_back(std::byte{0x80});
            REQUIRE(validate(truncated).err_code!=error_code::ok);++cases;
            auto overlong{truncated};overlong.insert(overlong.end(),
                {std::byte{0x80},std::byte{0x80},std::byte{0x80},std::byte{0x10},std::byte{0x0b}});
            REQUIRE(validate(overlong).err_code!=error_code::ok);++cases;
        }
    }
    for(auto const instruction:opcodes)
    for(unsigned obsolete{};obsolete!=2u;++obsolete)
    {
        std::vector<std::byte> bytes{std::byte{0x00},std::byte(obsolete?0xffu:0xfcu)};
        if(obsolete)for(auto value:{0x554c5445u,1u,1u})append_u32(bytes,value);
        append_u32(bytes,instruction.opcode);bytes.push_back(std::byte{0});bytes.push_back(std::byte{0x0b});
        REQUIRE(validate(bytes).err_code!=error_code::ok);++cases;
        REQUIRE(validate_wasm1p1(bytes).err_code!=error_code::ok);++cases;
    }
    for(auto word:{0x01010010u,0x01010030u,0x01010031u,0x01010032u,0x01010033u,0x01010034u,
                   0x01010035u,0x01010039u,0x01020020u,0x02010020u,0x00010020u,0x01000020u})
    {
        std::vector<std::byte> bytes{std::byte{0x00},std::byte{0xff}};append_u32(bytes,word);
        bytes.push_back(std::byte{0});bytes.push_back(std::byte{0x0b});
        REQUIRE(validate(bytes).err_code!=error_code::ok);++cases;
        REQUIRE(validate_wasm1p1(bytes).err_code!=error_code::ok);++cases;
    }
    std::printf("memtag validator: %u cases passed\n",cases);
}
