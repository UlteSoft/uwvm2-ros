/**
 * @file ancestry_reentry.cc
 * @brief Verify actual LLVM ROS/native callback reentry and generation retirement.
 * @details
 * Link with the SAME newly built ROS Runtime object/host API/full LLVM closure.
 * Only actual builtin ROS imports issue call identity. The native callback is
 * intentionally not a ROS principal and cannot mint an identity via this test.
 * No Gateway, policy grant, endpoint or ROS graph operation is requested.
 */
#define UWVM2TEST_RUNNER_USE_LLVM_JIT 1
#define UWVM2TEST_STRICT_NO_INTERPRETER 1
#include <uwvm2/uwvm/runtime/runtime_mode/impl.h>
#include "../../../../0013.uwvm_int/strict/uwvm_int_translate_strict_common.h"
#include <uwvm2/runtime/lib/uwvm_runtime.h>
#include <uwvm2/uwvm/imported/ros/host.h>
#include <cstdint>
#include <array>
#include <cstring>
#include <memory>
#include <vector>
#if !defined(UWVM_IMPORT_ROS1) && !defined(UWVM_IMPORT_ROS2)
# error actual ROS Runtime backend required
#endif
namespace
{
    /** @brief Terminate the witness immediately when a real runtime invariant fails. */
    void require(bool condition) noexcept
    {
        if(!condition)
        {
            ::fast_io::io::perrln("FAIL_ROS_LLVM_ANCESTRY_REENTRY");
            ::fast_io::fast_terminate();
        }
    }

    namespace strict = uwvm2test::uwvm_int_strict;
    namespace lib = uwvm2::runtime::lib;
    namespace mode = uwvm2::uwvm::runtime::runtime_mode;
    namespace types = uwvm2::uwvm::wasm::type;
    namespace container = uwvm2::utils::container;
    namespace ros = uwvm2::uwvm::imported::ros;
    using wasm1 = uwvm2::parser::wasm::standard::wasm1::features::wasm1;
    using value = uwvm2::parser::wasm::standard::wasm1::type::value_type;
    using features = types::feature_list<wasm1>;

    struct reenter
    {
        inline static constexpr container::u8string_view function_name{u8"reenter"};
        using result_tuple = types::import_function_result_tuple_t<features, value::i32>;
        using parameter_tuple = types::import_function_parameter_tuple_t<features>;
        using local_imported_function_type = types::local_imported_function_type_t<result_tuple, parameter_tuple>;
        inline static void const* module{};
        inline static unsigned calls{};

        static void call(local_imported_function_type& invocation) noexcept
        {
            lib::ros_caller_identity outside{};
            require(module != nullptr && !lib::ros_caller_identity_host_api(outside));
            std::uint32_t output{0xffffffffu};
            // Real registry-owned module storage, held by the outer full-entry
            // lease. This is synchronous exact-size host ABI, not Guest memory.
            lib::llvm_jit_call_raw_host_api(module, 3u, std::addressof(output), sizeof(output), nullptr, 0uz);
            require(!lib::ros_caller_identity_host_api(outside));
            ++calls;
            container::get<0>(invocation.res) = output;
        }
    };

    struct host_module
    {
        container::u8string_view module_name{u8"ancestry-host"};
        using local_function_tuple = container::tuple<reenter>;
    };

    strict::byte_vec read_fixture(char const* path)
    {
        ::fast_io::native_file input{::fast_io::mnp::os_c_str(path), ::fast_io::open_mode::in};
        ::std::array<char, 65537uz> bytes{};
        auto current{bytes.data()};
        auto const end{bytes.data() + bytes.size()};
        // [bytes.begin,current) initialized input; [current,end) bounded capacity.
        // An extra byte distinguishes a valid64KiB fixture from a truncated input.
        while(current != end)
        {
            auto const next{::fast_io::operations::read_some(input, current, end)};
            if(next == current) { break; }
            current = next;
        }
        auto const size{static_cast<::std::size_t>(current - bytes.data())};
        require(size != 0uz && size <= 65536uz);
        strict::byte_vec result(size);
        // [source initialized size bytes][destination exact size writable bytes].
        // No cross-Instance address or range change occurs in this local copy.
        std::memcpy(result.data(), bytes.data(), size);
        return result;
    }
}  // namespace

int main(int argc, char** argv)
{
    if(argc != 4 || (std::strcmp(argv[1], "ros1") != 0 && std::strcmp(argv[1], "ros2") != 0))
    {
        ::fast_io::io::perrln("usage: ancestry-reentry ros1|ros2 reentry-root.wasm reentry-library.wasm");
        return 2;
    }
    auto root = read_fixture(argv[2]);
    auto library = read_fixture(argv[3]);
    auto parameter = strict::make_wasm1p1_feature_parameter();
    auto& core3 = uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1p1_parameter(parameter);
    core3.disable_tail_call = false;
    types::local_imported_t host{host_module{}};
    auto previous = lib::observe_compiler_runtime_generation_host_api();
    for(unsigned pass{}; pass != 2u; ++pass)
    {
        // Every pass constructs/initializes actual modules and recompiles a new
        // nonwrapping Runtime generation. No old entry/object/principal is reused.
        auto prepare = [&]()
        {
#if defined(UWVM_IMPORT_ROS1)
            if(std::strcmp(argv[1], "ros1") == 0)
            {
                types::local_imported_t actual{ros::ros1_local_imported_module};
                return strict::prepare_runtime_from_wasm(root,
                                                         u8"reentry-root",
                                                         {
                                                             {&library, u8"reentry-library", &parameter}
                },
                                                         parameter,
                                                         {actual, host});
            }
#endif
#if defined(UWVM_IMPORT_ROS2)
            if(std::strcmp(argv[1], "ros2") == 0)
            {
                types::local_imported_t actual{ros::ros2_local_imported_module};
                return strict::prepare_runtime_from_wasm(root,
                                                         u8"reentry-root",
                                                         {
                                                             {&library, u8"reentry-library", &parameter}
                },
                                                         parameter,
                                                         {actual, host});
            }
#endif
            ::fast_io::fast_terminate();
        };
        auto prepared = prepare();
        reenter::module = prepared.mod;
        mode::global_runtime_llvm_jit_call_stack = mode::runtime_llvm_jit_call_stack_t::unwind;
        mode::global_runtime_compile_threads_resolved = 0;
        require(lib::ros_execution_backend_supported_host_api());
        auto epoch = lib::observe_compiler_runtime_generation_host_api();
        require(epoch > previous);
        lib::full_compile_run_config run{};
        run.entry_function_index = 4u;
        lib::full_compile_and_run_main_module(u8"reentry-root", run);
        require(reenter::calls == 2u * (pass + 1u));
        {
            // Actual successful identity resolution registered only the root.
            // The denied library->Host->root callback must not register another owner.
            std::lock_guard lock{ros::host::mutex};
            auto found = ros::host::instances.find(prepared.mod);
            require(ros::host::instances.size() == 1uz && found != ros::host::instances.end() && found->second->generation == epoch);
        }
        lib::ros_caller_identity outside{};
        require(!lib::ros_caller_identity_host_api(outside));
        require(!outside.runtime_instance && !outside.generation);
        reenter::module = nullptr;
        prepared.reset();
        auto retired = lib::observe_compiler_runtime_generation_host_api();
        require(retired > epoch);
        previous = retired;
        ros::host::shutdown();
    }
    ::fast_io::io::println("PASS_ROS_LLVM_ANCESTRY_REENTRY generations=2 callbacks=4 same_module=22 cross_module=13 outside_denied=1");
}
