#pragma once
#include <uwvm2/uwvm/imported/wasi/wasip1/storage/env.h>
#include <uwvm2/uwvm/io/host_diagnostics.h>
#ifndef UWVM_MODULE
#include <cstdint>
#endif
#ifndef UWVM_MODULE
#include <uwvm2/uwvm/wasm/type/impl.h>
#endif

namespace uwvm2::uwvm::imported::wasi::threads
{
    struct spawn_function
    {
        using features = ::uwvm2::uwvm::wasm::type::binfmt_ver1_feature_list_t;
        using value = ::uwvm2::uwvm::wasm::type::feature_list_final_value_type_t<features>;
        using result_tuple = ::uwvm2::uwvm::wasm::type::import_function_result_tuple_t<features, value::i32>;
        using parameter_tuple = ::uwvm2::uwvm::wasm::type::import_function_parameter_tuple_t<features, value::i32>;
        using local_imported_function_type = ::uwvm2::uwvm::wasm::type::local_imported_function_type_t<result_tuple, parameter_tuple>;
        inline static constexpr ::uwvm2::utils::container::u8string_view function_name{u8"thread-spawn"};
        static void call(local_imported_function_type& call) noexcept
        {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS) && defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
            auto& env=::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env();
            if(!env.allows_interface(function_name) || !env.permits(::uwvm2::imported::wasi::wasip1::environment::permission_t::thread_spawn))
                ::uwvm2::imported::wasi::wasip1::environment::diagnostic_denied(env,
                    ::uwvm2::utils::control::host_interfaces::find(::uwvm2::utils::control::host_interfaces::family::wasip1,function_name),
                    !env.allows_interface(function_name));
#endif
            // Ordinary execution has no genuine Group/ChildLease. Actual
            // threaded dispatch intercepts this concrete factory member and
            // invokes Runtime's real reservation/world/compiler/native launch.
            ::uwvm2::utils::container::get<0>(call.res) = static_cast<::std::int32_t>(-1);
        }
    };
    struct local_imported_module_t
    {
        inline static constexpr ::uwvm2::utils::container::u8string_view module_name{u8"wasi"};
        using local_function_tuple = ::uwvm2::utils::container::tuple<spawn_function>;
    };
    inline constexpr local_imported_module_t local_imported_module{};
    static_assert(::uwvm2::uwvm::wasm::type::is_local_imported_module<local_imported_module_t>);
}
