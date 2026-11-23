// Real controller export, whole-world restoration, unwind and renewed stop IDs.
// All capture, retirement and continuation callbacks are production providers.
#include "mandatory_native_fixture.h"
#include <uwvm2/uwvm/run/run.h>
#include <uwvm2/uwvm/debugger/console.h>
#include <uwvm2/uwvm/run/owned_source.h>
#include <uwvm2/uwvm/cmdline/callback/impl.h>
#include <uwvm2/utils/control/debug_permissions.h>
#include <uwvm2/utils/control/owned_file_image.h>
#include <uwvm2/runtime/checkpoint/materialization.h>
#include <uwvm2/runtime/gc/entry_admission.h>
#include <uwvm2/uwvm/debugger/checkpoint_codec.h>
#include <uwvm2/runtime/diagnostic/snapshot_state.h>
#include <fast_io.h>
#include <fast_io_unit/string.h>
#include <atomic>
#include <array>
#include <barrier>
#include <cstddef>
#include <cstdint>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
// Derive actual target/backend capabilities in this fixture's own balanced scope.
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#if !defined(UWVM_RUNTIME_LLVM_JIT) || !defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) || !defined(UWVM_CPP_EXCEPTIONS)
# error Actual checkpoint census fixture requires LLVM full/native threads/C++ EH
#endif
namespace cp = ::uwvm2::uwvm::debugger::checkpoint;
namespace lib = ::uwvm2::runtime::lib;
namespace mode = ::uwvm2::uwvm::runtime::runtime_mode;
namespace threads = ::uwvm2::utils::thread;
namespace checkpoint = ::uwvm2::runtime::checkpoint;
namespace image = ::uwvm2::utils::control;
namespace full = ::uwvm2::uwvm::runtime::full;
using domain = threads::cooperative_pause_domain;
static void require(bool valid, unsigned line)
{
    if(valid) { return; }
    ::fast_io::print(::fast_io::err(), "debug_snapshot_controller_restore FAIL line=", ::fast_io::mnp::dec(line), "\n");
    ::fast_io::fast_terminate();
}
#define REQUIRE(x) require(bool(x), __LINE__)
static auto deadline() { return ::std::chrono::steady_clock::now() + ::std::chrono::seconds{180}; }
struct source_setup
{
    ::std::u8string path{};
    image::owned_file_image::owner immutable{};
    full::full_source_instance::mutable_owner source{};
    bool ready{};
    static bool prepare(void* pointer) noexcept
    {
        // [actual main-owned synchronous drained setup context] owner_end
        // [safe] retained until replace_full_source_after_drain callback returns.
        auto& state{*static_cast<source_setup*>(pointer)};
        try
        {
            auto candidate{full::full_source_instance::create_unparsed(state.path, u8"checkpoint-instance")};
            if(!full::select_unparsed_full_source_after_drain(candidate)) { return false; }
            // SAME exclusive source image is adopted BEFORE magic/section parse,
            // not copied from a mutable file mapping after compiler validation.
            auto const loaded{::uwvm2::uwvm::wasm::loader::load_wasm_file(candidate->file_for_native_initialization(),
                candidate->owned_file_name(), candidate->owned_rename(), ::uwvm2::uwvm::wasm::storage::wasm_parameter,
                ::std::move(state.immutable))};
            if(loaded != ::uwvm2::uwvm::wasm::loader::load_wasm_file_rtl::ok ||
               ::uwvm2::uwvm::run::load_local_modules(false)!=static_cast<int>(::uwvm2::uwvm::run::retval::ok) ||
               ::uwvm2::uwvm::wasm::loader::construct_all_module_and_check_duplicate_module() !=
                   ::uwvm2::uwvm::wasm::loader::load_and_check_modules_rtl::ok ||
               ::uwvm2::uwvm::wasm::loader::check_import_exist_and_detect_cycles() !=
                   ::uwvm2::uwvm::wasm::loader::load_and_check_modules_rtl::ok) { return false; }
            // These modules have no start functions. Complete ordinary segment
            // instantiation, including passive expression payloads, BEFORE any
            // setup guest entry or source seal; true would leave it deferred.
            if(!::uwvm2::uwvm::runtime::initializer::initialize_runtime(false)) { return false; }
            if(!candidate->seal_actual_initializer()) { return false; }
            state.source = ::std::move(candidate); state.ready = true; return true;
        }
        catch(...) { return false; }
    }
};
int main(int argc,char** argv)
{

    if(!::uwvm2test::mandatory_native::arguments(argc, argv)) { return 64; }
    if(argc != 3) { return 64; }

    REQUIRE(::uwvm2::utils::control::debug_permissions::configure_trusted_launch(
        ::uwvm2::utils::control::debug_permissions::permission_level::full));
    mode::global_runtime_mode = mode::runtime_mode_t::full_compile;
    mode::global_runtime_compiler = mode::runtime_compiler_t::llvm_jit_only;

    mode::global_runtime_compile_threads = 0u; mode::runtime_compile_threads_existed = true;
    mode::global_runtime_llvm_jit_cache_path_mode = mode::runtime_llvm_jit_cache_path_mode_t::disabled;
    auto& features{::uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1p1_parameter(::uwvm2::uwvm::wasm::storage::wasm_parameter.binfmt1_para)};
    features.disable_gc = false; features.explicit_enable_gc = true;
    features.disable_function_references = false; features.explicit_enable_function_references = true;
    features.disable_reference_types = false; features.explicit_enable_reference_types = true;
    features.disable_memory64 = false; features.explicit_enable_memory64 = true;
    features.disable_table64 = false; features.explicit_enable_table64 = true;
    features.disable_table_instructions = false; features.explicit_enable_table_instructions = true;
    features.disable_multiple_tables = false; features.explicit_enable_multiple_tables = true;
    features.disable_table_initializer = false; features.explicit_enable_table_initializer = true;
    features.disable_exceptions = false; features.explicit_enable_exceptions = true;
    features.disable_simd = false; features.explicit_enable_simd = true;
    features.disable_bulk_memory = false; features.explicit_enable_bulk_memory = true;
    source_setup setup{};
    setup.path = ::fast_io::u8concat_std(::fast_io::mnp::code_cvt(::fast_io::mnp::os_c_str(argv[1])));
    auto& arguments{::uwvm2::uwvm::cmdline::parsing_result};
    ::uwvm2::uwvm::cmdline::wasm_file_ppos = nullptr; arguments.clear();
    arguments.emplace_back(::uwvm2::utils::cmdline::parameter_parsing_results{
        u8"debug-checkpoint-complete-instance", nullptr, ::uwvm2::utils::cmdline::parameter_parsing_results_type::dir});
    arguments.emplace_back(::uwvm2::utils::cmdline::parameter_parsing_results{
        ::uwvm2::utils::container::u8cstring_view{::fast_io::mnp::os_c_str(setup.path.c_str())}, nullptr,
        ::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg});
    // [actual finalized global CLI argument allocation] end
    // [safe] cursor installed AFTER all vector growth, retained through reset.
    ::uwvm2::uwvm::cmdline::wasm_file_ppos = ::std::addressof(arguments.back());
    namespace ws=::uwvm2::uwvm::imported::wasi::wasip1::storage;
    namespace api=::uwvm2::utils::control::host_interfaces;
    using permission=::uwvm2::imported::wasi::wasip1::environment::permission_t;
    ::uwvm2::uwvm::wasm::storage::local_preload_wasip1=false;
    auto* policy=ws::try_create_targetless_wasip1_module_override(u8"checkpoint-instance");REQUIRE(policy);
    policy->enabled=true;policy->enabled_is_set=true;
    policy->non_fd_permissions=permission::args|permission::environ|permission::stdio_metadata|permission::stdout_write;
    policy->permission_presence=policy->non_fd_permissions;
    for(auto name:{"args_sizes_get","environ_sizes_get","fd_fdstat_get"})
    { REQUIRE(policy->interfaces.set(api::find(api::family::wasip1,::std::string_view{name}),api::access::allow)); }
    policy->noinherit_system_environment=true;policy->noinherit_system_environment_is_set=true;
    policy->force_args_is_set=true;policy->force_argument_storage.emplace_back(u8"alpha");policy->force_argument_storage.emplace_back(u8"beta");
    policy->add_or_replace_environment.emplace(u8"MODE",u8"restore");
    auto bytes{image::owned_file_image::read(setup.path, 1048576u)}; REQUIRE(bytes);
    auto const original{bytes.image->bytes()}; REQUIRE(original.size() >= 8u && original.size() <= PTRDIFF_MAX);
    setup.immutable = ::std::move(bytes.image);
    REQUIRE(lib::replace_full_source_after_drain_host_api(source_setup::prepare, ::std::addressof(setup)) &&
        setup.ready && setup.source && setup.source->file().has_owned_source_image());

    namespace dbg=::uwvm2::uwvm::debugger;
    namespace ctl=::uwvm2::utils::control;
    ctl::launch_config config{};config.debug_enabled=true;config.compiler=ctl::backend::llvm;
    config.profile=ctl::debug_permissions::permission_level::full;config.mode=ctl::compile_mode::full;
    config.origin=ctl::launch_origin::console;config.instance[0]=1;config.vm_process=dbg::posix_abi::getpid_noexcept();
    auto owner=dbg::controller::create(config,1u,lib::llvm_jit_capture_debug_stack_host_api,lib::llvm_jit_debug_read_memory_host_api);REQUIRE(owner && owner->status()==ctl::error::none);
    auto profile=checkpoint::compilation_profile::create_for_trusted_observer();REQUIRE(profile);
    REQUIRE(lib::llvm_jit_configure_debug_session_host_api(owner->domain(),owner->observer(),lib::llvm_jit_debug_safe_point_granularity::instruction)==lib::llvm_jit_debug_configure_result::ok);
    REQUIRE(lib::llvm_jit_configure_debug_value_observation_host_api(profile->limits())==lib::llvm_jit_debug_configure_result::ok);
    REQUIRE(lib::llvm_jit_prepare_debug_host_api());
    REQUIRE(::uwvm2::uwvm::runtime::initializer::apply_runtime_active_segments(u8"checkpoint-instance")==::uwvm2::uwvm::runtime::initializer::details::active_segment_result::ok);
    REQUIRE(::uwvm2::uwvm::run::install_debug_source_maps(*owner));
    lib::full_compile_run_config initialize{};initialize.entry_function_index=3u;
    lib::full_compile_and_run_main_module(u8"checkpoint-instance",initialize);
    auto command=[&](::std::string const& text){
        ::fast_io::io::perrln("COMMAND ",text);
        auto parsed=dbg::parse_console_command(::fast_io::string_view{text.data(),text.size()});
        auto result=owner->execute(parsed,::std::chrono::seconds{30});
        ::fast_io::io::perrln("REPLY ",dbg::details::format_reply(result,parsed));
        return result;
    };
    REQUIRE(owner->arm_initial_pause());REQUIRE(command("breakwasm 0 4 0").status==ctl::error::none);
    REQUIRE(command("continue").status==ctl::error::none);
    struct launch {
        ::std::shared_ptr<dbg::controller> owner;::std::uint32_t value{};
        static void body(void* p)noexcept {
            auto& self=*static_cast<launch*>(p);lib::full_compile_run_config run{};run.entry_function_index=4u;
            run.entry_abi_buffers.result_buffer=reinterpret_cast<::std::byte*>(&self.value);run.entry_abi_buffers.result_bytes=sizeof(self.value);
            lib::full_compile_and_run_main_module(u8"checkpoint-instance",run);
            self.owner->notify_guest_exit(lib::runtime_llvm_jit_debug_registered_worker_exit_code_host_api());
        }
    };
    auto started=::std::make_shared<launch>();started->owner=owner;
    auto worker=lib::runtime_launch_llvm_jit_debug_guest_worker_host_api(owner->domain(),started,launch::body);
    REQUIRE(worker.status==lib::llvm_jit_debug_guest_worker_launch_status::started);
    auto until=deadline();dbg::controller_reply stopped;
    do {stopped=owner->inspect();REQUIRE(::std::chrono::steady_clock::now()<until);::std::this_thread::yield();}
    while(stopped.execution!=dbg::execution_status::stopped || stopped.threads.empty());
    ::std::string base{argv[2]};
    auto saved=command(::fast_io::concat_std("dump wasm full ",stopped.stop_identifier," ",base,".first.uwsnap"));
    REQUIRE(saved.status==ctl::error::none && saved.dump_published);
    auto previous=stopped.stop_identifier;
    for(unsigned iteration{};iteration!=2u;++iteration) {
        auto restored=command(::fast_io::concat_std("dump restore current ",previous," ",base,".first.uwsnap"));
        REQUIRE(restored.status==ctl::error::none && restored.restore_applied && !restored.restore_pending);
        REQUIRE(restored.execution==dbg::execution_status::stopped && restored.threads.size()==1u && restored.stop_identifier>previous);
        previous=restored.stop_identifier;
        auto trace=command(::fast_io::concat_std("bt ",restored.threads[0].identifier));
        REQUIRE(trace.status==ctl::error::none && trace.threads.size()==1u && trace.threads[0].trace && trace.threads[0].trace->frame_count()>0u);
        REQUIRE(command(::fast_io::concat_std("locals wasm ",restored.threads[0].identifier)).status==ctl::error::none);
    }
    auto saved_again=command(::fast_io::concat_std("dump wasm full ",previous," ",base,".second.uwsnap"));
    REQUIRE(saved_again.status==ctl::error::none && saved_again.dump_published);
    REQUIRE(command("continue").status==ctl::error::none);
    until=deadline();
    do {stopped=owner->inspect();REQUIRE(::std::chrono::steady_clock::now()<until);::std::this_thread::yield();}
    while(stopped.execution!=dbg::execution_status::exited);
    REQUIRE(stopped.guest_exit_code==0u);
    auto joined=lib::llvm_jit_checkpoint_join_restored_instance_host_api(lib::observe_compiler_runtime_generation_host_api(),deadline());
    REQUIRE(joined.status==lib::llvm_jit_checkpoint_restored_join_status::joined);
    REQUIRE(lib::runtime_join_llvm_jit_debug_guest_worker_until_host_api(worker.worker,deadline()).actual.status==::fast_io::thread_join_status::joined);
    lib::reset_runtime_state_host_api();::uwvm2::uwvm::cmdline::wasm_file_ppos=nullptr;
    ::fast_io::io::println("actual controller dump, two restores, renewed pauses and completion PASS");
}
#include <uwvm2/uwvm/runtime/macro/pop_macros.h>
#include <uwvm2/utils/macro/pop_macros.h>
