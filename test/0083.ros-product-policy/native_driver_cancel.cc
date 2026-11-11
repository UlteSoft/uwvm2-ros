// Actual VM execution lease -> actual DSO -> stable WASI table -> quota wait.
// The custom native import only invokes the DSO. It provides no wait/cancel
// substitute: runtime_request_execution_stop_host_api must wake the real scope.
#include "../0017.runtime/mandatory_native_fixture.h"
#include "../0013.uwvm_int/strict/uwvm_int_translate_strict_common.h"
#include <uwvm2/runtime/lib/uwvm_runtime.h>
#include <uwvm2/uwvm/wasm/type/wasip1_api.h>
#include <atomic>
#include <chrono>
#include <thread>

#if !defined(UWVM_ROS_ZERO_HOST_DEFAULTS) || !defined(UWVM2TEST_RUNNER_USE_LLVM_JIT) || !defined(__linux__)
#error "This runtime fixture requires the actual Linux zero-default AOT product objects"
#endif

namespace
{
    namespace strict = uwvm2test::uwvm_int_strict;
    namespace lib = uwvm2::runtime::lib;
    namespace api = uwvm2::uwvm::wasm::type;
    namespace st = uwvm2::uwvm::imported::wasi::wasip1::storage;
    namespace quota = uwvm2::imported::wasi::wasip1::environment;
    using wasm1 = uwvm2::parser::wasm::standard::wasm1::features::wasm1;
    using features = api::feature_list<wasm1>;
    using results = api::import_function_result_tuple_t<features>;
    using parameters = api::import_function_parameter_tuple_t<features>;
    using host_call = api::local_imported_function_type_t<results, parameters>;
    api::capi_wasm_function driver{};
    std::atomic<std::uint32_t> driver_result{UINT32_MAX};
    std::atomic<bool> entry_returned{}, observed_cancelled{};

    struct native_driver
    {
        inline static constexpr uwvm2::utils::container::u8string_view function_name{u8"native_driver"};
        using result_tuple = results;
        using parameter_tuple = parameters;
        using local_imported_function_type = host_call;
        static void call(host_call&) noexcept
        {
            if(lib::runtime_execution_stop_requested_host_api()) std::abort();
            std::byte result[4]{}, params[1]{};
            driver(result, params);
            std::uint32_t code{}; std::memcpy(&code, result, sizeof(code));
            driver_result.store(code, std::memory_order_release);
            observed_cancelled.store(lib::runtime_execution_stop_requested_host_api(), std::memory_order_release);
        }
    };
    struct host_module
    {
        uwvm2::utils::container::u8string_view module_name{u8"cancel-host"};
        using local_function_tuple = uwvm2::utils::container::tuple<native_driver>;
    };
    strict::byte_vec make_module()
    {
        strict::module_builder module;
        module.has_memory = true; module.memory_min = 1;
        module.types.push_back({{}, {}});
        module.add_import_func("cancel-host", "native_driver", 0);
        strict::func_body body;
        auto integer = [&](std::int32_t value)
        { strict::append_u8(body.code, 0x41); strict::append_i32_leb(body.code, value); };
        auto store = [&](std::int32_t address, std::int32_t value)
        { integer(address); integer(value); for(auto byte : {0x36, 2, 0}) strict::append_u8(body.code, byte); };
        store(64, 160); store(68, 4); store(160, 0x65666173); store(80, 123456789); store(84, 123456789);
        strict::append_u8(body.code, 0x10); strict::append_u8(body.code, 0);
        auto check = [&](std::int32_t address, std::int32_t value)
        {
            integer(address); for(auto byte : {0x28, 2, 0}) strict::append_u8(body.code, byte);
            integer(value); for(auto byte : {0x47, 4, 0x40, 0, 0x0b}) strict::append_u8(body.code, byte);
        };
        check(80, 4); check(84, 123456789);
        integer(42); strict::append_u8(body.code, 0x0b);
        module.add_func({{}, {strict::k_val_i32}}, std::move(body));
        return module.build();
    }
}

int main(int argc, char** argv)
{
    if(!uwvm2test::mandatory_native::arguments(argc, argv) || argc != 2) return 64;
    auto wasm = make_module();
    api::local_imported_t host{host_module{}};
    auto prepared = strict::prepare_runtime_from_wasm(wasm, u8"driver-cancel", {}, strict::make_wasm1p1_feature_parameter(), {host});
    auto& env = st::default_wasip1_env;
    env.interface_enabled = true;
    namespace interfaces = uwvm2::utils::control::host_interfaces;
    if(!env.interfaces.set(interfaces::find(interfaces::family::wasip1, uwvm2::utils::container::u8string_view{u8"fd_write"}), interfaces::access::allow)) return 1;
    env.non_fd_permissions = quota::permission_t::stdout_write;
    auto& limit = env.limits.operations[static_cast<unsigned>(quota::budget_kind::stdout_write)];
    limit.window_calls = 1;
    limit.actions[2] = quota::limit_action::wait;
    if(!st::init_wasip1_environment(env)) return 2;
    uwvm2::uwvm::wasm::storage::preload_expose_wasip1_host_api = true;
    auto const* table = api::uwvm_get_wasip1_host_api_v1();
    if(table == nullptr) return 3;
    fast_io::native_dll_file library{fast_io::mnp::os_c_str(argv[1]), fast_io::dll_mode::posix_rtld_now};
    auto setter = reinterpret_cast<api::uwvm_set_wasip1_host_api_v1_t>(fast_io::dll_load_symbol(library, u8"uwvm_set_wasip1_host_api_v1"));
    auto getter = reinterpret_cast<api::capi_get_function_vec_t>(fast_io::dll_load_symbol(library, u8"uwvm_function"));
    if(setter == nullptr || getter == nullptr) return 4;
    setter(table);
    auto const functions = getter();
    for(std::size_t i{}; i != functions.function_size; ++i)
    {
        auto const& f = functions.function_begin[i];
        if(f.func_name_length == 10 && std::memcmp(f.func_name_ptr, "write_pair", 10) == 0) driver = f.func_ptr;
    }
    if(driver == nullptr) return 5;
    std::uint32_t answer{};
    std::thread worker([&]
    {
        lib::full_compile_run_config config;
        config.entry_function_index = 1;
        config.entry_abi_buffers.result_buffer = reinterpret_cast<std::byte*>(&answer);
        config.entry_abi_buffers.result_bytes = sizeof(answer);
        lib::full_compile_and_run_main_module(u8"driver-cancel", config);
        entry_returned.store(true, std::memory_order_release);
    });
    auto const deadline = std::chrono::steady_clock::now() + std::chrono::seconds(15);
    for(;;)
    {
        bool pending{};
        {
            std::scoped_lock lock{quota::process_budget_ledger.mutex,env.limits.accounting->mutex};
            auto const& count = env.limits.accounting->counters[static_cast<unsigned>(quota::budget_kind::stdout_write)];
            pending = quota::process_budget_ledger.waiters != nullptr && count.total_calls == 1 && count.active == 0;
        }
        if(pending) break;
        if(entry_returned.load(std::memory_order_acquire) || std::chrono::steady_clock::now() > deadline) std::abort();
        std::this_thread::yield();
    }
    auto const stopped_at = std::chrono::steady_clock::now();
    lib::runtime_request_execution_stop_host_api();
    worker.join();
    auto const cancel_duration = std::chrono::steady_clock::now() - stopped_at;
    auto const& counter = env.limits.accounting->counters[static_cast<unsigned>(quota::budget_kind::stdout_write)];
    if(driver_result.load() != 11u || !observed_cancelled.load() || answer != 42 || counter.total_calls != 1 ||
       counter.total_bytes != 4 || counter.active != 0 || cancel_duration >= std::chrono::milliseconds(500)) return 6;
    std::fprintf(stderr, "PASS real DSO stable-WASI quota cancellation: errno 11, first effect once, pending call untouched, actual lease stopped, wake_ms=%lld\n",
        static_cast<long long>(std::chrono::duration_cast<std::chrono::milliseconds>(cancel_duration).count()));
}
