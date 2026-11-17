// Actual frozen runtime embedding. This is a qualification-only host, not a
// guest API: load a real immutable Wasm source, execute, drain/reset, then load
// a fresh generation. The external test collector reads bytes synchronously
// on LOAD and UNLOAD, proving notification precedes executable retirement.
#include <uwvm2/uwvm/crtmain/impl.h>
#include <uwvm2/uwvm/run/owned_source.h>
#include <fast_io.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
namespace mode = ::uwvm2::uwvm::runtime::runtime_mode;
namespace lib = ::uwvm2::runtime::lib;
namespace full = ::uwvm2::uwvm::runtime::full;
static bool prepare(char const* file)
{
    char8_t filename[4096]{};
    auto const size{::std::strlen(file)};
    if(size >= sizeof(filename)) { return false; }
    // The launcher supplies UTF-8 path bytes. Copy object representations into
    // an actual char8_t buffer; no incompatible char/char8_t typed borrow.
    ::std::memcpy(filename,file,size+1u);
    ::uwvm2::utils::cmdline::parameter_parsing_results path{};
    path.str = ::uwvm2::utils::container::u8cstring_view{::fast_io::containers::null_terminated_t{},filename,size};
    ::uwvm2::uwvm::cmdline::wasm_file_ppos = ::std::addressof(path);
    ::uwvm2::uwvm::wasm::storage::execute_wasm.module_name = u8"intel-owner";
    auto const result{::uwvm2::uwvm::run::prepare_owned_full_cli_source()};
    ::uwvm2::uwvm::cmdline::wasm_file_ppos = nullptr;
    return result == static_cast<int>(::uwvm2::uwvm::run::retval::ok);
}
static bool execute()
{
    ::std::uint32_t input{35u},output{};
    lib::full_compile_run_config config{};
    config.entry_function_index = 0u;
    config.entry_abi_buffers.param_buffer = reinterpret_cast<::std::byte const*>(::std::addressof(input));
    config.entry_abi_buffers.param_bytes = sizeof(input);
    config.entry_abi_buffers.result_buffer = reinterpret_cast<::std::byte*>(::std::addressof(output));
    config.entry_abi_buffers.result_bytes = sizeof(output);
    lib::full_compile_and_run_main_module(u8"intel-owner", config);
    return output == 42u;
}
int main(int argc,char** argv)
{
    if(argc != 3) { return 64; }
    mode::global_runtime_mode = mode::runtime_mode_t::full_compile;
    mode::global_runtime_compiler = mode::runtime_compiler_t::llvm_jit_only;
    mode::global_runtime_llvm_jit_policy = mode::runtime_llvm_jit_policy_t::max;
    mode::global_runtime_llvm_jit_cache_path_mode = mode::runtime_llvm_jit_cache_path_mode_t::disabled;
    mode::global_runtime_compile_threads = 0u;
    mode::runtime_compile_threads_existed = true;
    ::uwvm2::uwvm::io::u8log_output.reopen(::fast_io::io_dup, ::fast_io::u8err());
    if(!prepare(argv[1]) || !execute()) { return 1; }
    auto first{full::selected_full_source_owner_pin()};
    auto const module{first->bound_initialized_main_module_id()};
    if(!lib::llvm_jit_full_source_publication_host_api(module).ready) { return 2; }
    if(::std::strcmp(argv[2],"retain") == 0) { ::std::puts("PASS retained actual code"); return 0; }
    if(::std::strcmp(argv[2],"reset") != 0) { return 64; }
    lib::reset_runtime_state_host_api();
    if(lib::llvm_jit_full_source_publication_host_api(module).ready) { return 3; }
    if(!prepare(argv[1]) || !execute()) { return 4; }
    auto second{full::selected_full_source_owner_pin()};
    if(first.get() == second.get()) { return 5; }
    lib::reset_runtime_state_host_api();
    ::std::puts("PASS two actual generations drained and retired");
}
