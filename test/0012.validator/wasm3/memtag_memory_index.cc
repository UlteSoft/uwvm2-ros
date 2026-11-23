// MTE memory indices follow the same feature policy in standalone validation
// and both full compilers. A padded zero is not an MVP reserved zero byte.
#include <uwvm2/parser/wasm/standard/wasm2/impl.h>
#include <uwvm2/validation/standard/wasm3/impl.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <vector>

namespace parser = uwvm2::parser::wasm;
namespace validation = uwvm2::validation::standard::wasm3;
using wasm1 = parser::standard::wasm1::features::wasm1;
using wasm1p1 = parser::standard::wasm1p1::features::wasm1p1;
using wasm2 = parser::standard::wasm2::features::wasm2;
using code = uwvm2::validation::error::code_validation_error_code;
using bytes = std::vector<std::byte>;
#define CHECK(x) do { if(!(x)) { std::fprintf(stderr, "FAIL %u: %s\n", __LINE__, #x); std::abort(); } } while(false)

int main()
{
    unsigned checks{};
    for(bool wide : {false, true})
    {
        // One real binary memory declaration and one empty local function.
        unsigned char module_bytes[]{0,97,115,109,1,0,0,0,
            1,4,1,96,0,0,3,2,1,0,5,4,1,1,1,1,10,4,1,2,0,11};
        module_bytes[21] = wide ? 5 : 1;
        parser::concepts::feature_parameter_t<wasm1,wasm1p1,wasm2> parameters{};
        auto& policy = parser::standard::wasm2::features::get_wasm2_parameter(parameters);
        policy.cli_mode = decltype(policy.cli_mode)::direct_wasm3;
        auto const begin = reinterpret_cast<std::byte const*>(module_bytes);
        parser::base::error_impl parse_error{};
        auto module = parser::binfmt::ver1::wasm_binfmt_ver1_handle_func<wasm1,wasm1p1,wasm2>(
            begin, begin + sizeof(module_bytes), parse_error, parameters);
        for(bool multi : {false, true}) for(bool disabled : {false, true}) for(bool dead : {false, true})
        {
            policy.disable_multi_memory = !multi;
            policy.disable_memtag_extension = disabled;
            for(unsigned padding{}; padding != 6; ++padding)
            {
                for(unsigned index : {0u, 1u})
                {
                    bytes expression{};
                    if(dead) { expression.push_back(std::byte{}); }
                    // Packed linear-v1 memtag.status; the selected memory determines its result width.
                    for(unsigned b : {0xffu,0xa0u,0x80u,0x84u,0x08u}) { expression.push_back(std::byte(b)); }
                    if(padding == 0) { expression.push_back(std::byte(index)); }
                    else
                    {
                        expression.push_back(std::byte(0x80u | index));
                        for(unsigned i = 1; i != padding; ++i) { expression.push_back(std::byte{0x80}); }
                        expression.push_back(std::byte{});
                    }
                    expression.push_back(std::byte{0x1a});
                    expression.push_back(std::byte{0x0b});
                    uwvm2::validation::error::code_validation_error_impl error{};
                    bool accepted{true};
                    try
                    {
                        validation::validate_code_with_runtime_policy(module, 0,
                            expression.data(), expression.data() + expression.size(), error, parameters);
                    }
                    catch(fast_io::error const&) { accepted = false; }
                    bool const expected = !disabled && index == 0 && padding < 5 && (multi || padding == 0);
                    CHECK(accepted == expected);
                    CHECK(accepted == (error.err_code == code::ok));
                    ++checks;
                }
            }
        }
    }
    std::printf("PASS standalone MTE index feature/width/padding/unreachable matrix: %u checks\n", checks);
}
