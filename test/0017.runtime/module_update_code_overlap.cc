// Link against the product runtime and pass main.wasm from run_module_update_code_limits.py.
// Two distinct private candidates are prepared against the same 18-byte module.
#include <uwvm2/uwvm/run/owned_source.h>
#include <uwvm2/uwvm/cmdline/callback/impl.h>
#include <uwvm2/utils/container/string_concat.h>
#include <array>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <memory>

namespace lib = ::uwvm2::runtime::lib;
namespace threads = ::uwvm2::utils::thread;
using status = lib::llvm_jit_debug_replace_status;

static void require(bool condition, char const* message)
{
    if(!condition) { ::std::fprintf(stderr, "%s\n", message); ::std::exit(1); }
}

int main(int argc, char** argv)
{
    if(argc != 2) { return 64; }
    namespace mode = ::uwvm2::uwvm::runtime::runtime_mode;
    namespace wasm = ::uwvm2::uwvm::wasm;
    namespace permissions = ::uwvm2::utils::control::debug_permissions;
    namespace crash = ::uwvm2::runtime::diagnostic::crash;
    require(crash::process_policy.seal(crash::action::exit), "seal fault policy");
    require(permissions::configure_trusted_launch(permissions::permission_level::full), "enable DEV");
    mode::global_runtime_mode = mode::runtime_mode_t::full_compile;
    mode::global_runtime_compiler = mode::runtime_compiler_t::llvm_jit_only;
    mode::global_runtime_compile_threads = 0u;
    mode::runtime_compile_threads_existed = true;
    mode::global_runtime_llvm_jit_cache_path_mode = mode::runtime_llvm_jit_cache_path_mode_t::disabled;
    auto& parser{wasm::feature::wasm_binfmt_ver1_wasm1_parameter(wasm::storage::wasm_parameter.binfmt1_para)};
    parser.parser_limit.max_module_code_bytes = 20;
    wasm::storage::execute_wasm.module_name = u8"main";
    auto const path{::uwvm2::utils::container::u8concat_uwvm(::fast_io::mnp::code_cvt(::fast_io::mnp::os_c_str(argv[1])))};
    namespace cli = ::uwvm2::uwvm::cmdline;
    using argument = ::uwvm2::utils::cmdline::parameter_parsing_results;
    using kind = ::uwvm2::utils::cmdline::parameter_parsing_results_type;
    cli::parsing_result.emplace_back(argument{u8"main", nullptr, kind::dir});
    cli::parsing_result.emplace_back(argument{::uwvm2::utils::container::u8cstring_view{::fast_io::mnp::os_c_str(path.c_str())}, nullptr, kind::occupied_arg});
    cli::wasm_file_ppos = ::std::addressof(cli::parsing_result.back());
    require(::uwvm2::uwvm::run::prepare_owned_full_cli_source(true) == static_cast<int>(::uwvm2::uwvm::run::retval::ok), "initialize source");
    auto control{::std::make_shared<threads::cooperative_pause_domain>()};
    require(lib::llvm_jit_configure_debug_safe_points_host_api(control) == lib::llvm_jit_debug_configure_result::ok &&
            lib::llvm_jit_prepare_debug_host_api(), "prepare debugger");
    auto ticket{control->request_pause()};
    require(ticket && control->wait_until_paused(ticket, ::std::chrono::steady_clock::now() + ::std::chrono::seconds(5)) ==
            threads::cooperative_pause_result::paused, "acquire empty cohort pause");

    constexpr unsigned char local[]{1, 1, 0x7f, 0x41, 9, 0x0b};
    constexpr unsigned char local_nop[]{1, 1, 0x7f, 1, 0x41, 10, 0x0b};
    constexpr unsigned char two_nops[]{0, 1, 1, 0x41, 10, 0x0b};
    constexpr unsigned char original[]{0, 0x41, 7, 0x0b};
    auto prepare = [](unsigned function, unsigned generation, auto const& body)
    {
        return lib::llvm_jit_debug_prepare_function_replacement_host_api(0u, function, generation,
            reinterpret_cast<::std::byte const*>(body), ::std::size(body));
    };
    auto commit = [&](auto pending, status expected)
    {
        require(pending.status == status::replaced && pending.transaction, "candidate must pass preparation");
        lib::llvm_jit_debug_replace_result result{};
        require(control->while_stopped(ticket, [&] { result = lib::llvm_jit_debug_commit_function_replacement_host_api(pending.transaction); }) &&
                result.status == expected, "unexpected publication result");
        lib::llvm_jit_debug_discard_function_replacement_host_api(pending.transaction);
    };
    auto first{prepare(1, 1, local)};      // 20 bytes with the old function 2
    auto second{prepare(2, 1, local_nop)}; // 19 bytes with the old function 1
    namespace resource = ::uwvm2::utils::allocator::resource;
    auto& parser_live{resource::live_bytes[static_cast<unsigned>(resource::pool::parser)]};
    auto const before{parser_live.load()};
    auto same_size{prepare(2, 1, two_nops)}; // no parsed locals to own storage
    require(parser_live.load() >= before + sizeof(two_nops) + 2u, "prepared section backing is charged");
    commit(first, status::replaced);
    commit(second, status::invalid_body); // both would require 21
    commit(same_size, status::stale_generation); // its diagnostic maps still describe the old module
    auto retry{prepare(2, 1, local_nop)};
    require(retry.status == status::invalid_body && !retry.transaction, "retry observes published function 1");
    commit(prepare(1, 2, original), status::replaced);
    commit(prepare(2, 1, local_nop), status::replaced);
    require(control->resume(ticket), "release pause");
    lib::reset_runtime_state_host_api();
}
