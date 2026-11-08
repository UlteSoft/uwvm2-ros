/*************************************************************
 * Private source-only foundation candidate; actual project APIs.
 *************************************************************/

#pragma once
#ifndef UWVM_MODULE
# include <cstddef>
# include <memory>
# include <string>
# include <utility>
# include <vector>
# include <uwvm2/utils/macro/push_macros.h>
# include <uwvm2/uwvm/runtime/macro/push_macros.h>
# include <fast_io.h>
# include <uwvm2/utils/control/impl.h>
# include <uwvm2/uwvm/cmdline/impl.h>
# include <uwvm2/uwvm/wasm/impl.h>
# include <uwvm2/uwvm/runtime/storage/impl.h>
# include <uwvm2/uwvm/runtime/initializer/impl.h>
# include <uwvm2/uwvm/runtime/runtime_mode/impl.h>
# include <uwvm2/runtime/lib/uwvm_runtime.h>
# include "loader.h"
# include "retval.h"
# if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
#  include <uwvm2/uwvm/imported/ros/validation.h>
#  include <uwvm2/uwvm/imported/ros/host.h>
# endif
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::run
{
    // Native authenticated launcher input, never a guest authority token. The
    // original management profile and existing target contracts are rechecked.
    struct dev_rebuild_input
    {
        ::std::u8string path{};
        ::uwvm2::utils::control::owned_file_image::owner image{};
        ::std::size_t module_index{}; // 0 = main; 1..N = startup preload order, never a new identity.
    };
    [[nodiscard]] inline bool dev_rebuild_supported() noexcept
    {
#if defined(__linux__) && defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_CPP_EXCEPTIONS)
        if(::uwvm2::utils::security::wsea::secure_enabled() ||
           !::uwvm2::utils::control::debug_permissions::full_allowed() ||
           ::uwvm2::uwvm::runtime::runtime_mode::global_runtime_compiler !=
               ::uwvm2::uwvm::runtime::runtime_mode::runtime_compiler_t::llvm_jit_only ||
           ::uwvm2::uwvm::runtime::runtime_mode::global_runtime_mode !=
               ::uwvm2::uwvm::runtime::runtime_mode::runtime_mode_t::full_compile ||
           !::uwvm2::uwvm::wasm::storage::active_execute_wasm().has_owned_source_image()) { return false; }
# if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
        // ROS provider/physical authority continuity requires its own stable
        // runtime domain. A new debug source is not permission to recreate it.
        if(::uwvm2::uwvm::imported::ros::policy_configured) { return false; }
# endif
# if UWVM2_HAS_THREADED_PROFILE_HOST_API
        if(::uwvm2::runtime::lib::runtime_threaded_profile_builtin_requested_host_api()) { return false; }
# endif
# if defined(UWVM_SUPPORT_PRELOAD_DL)
        if(!::uwvm2::uwvm::wasm::storage::preloaded_dl.empty()) { return false; }
# endif
# if defined(UWVM_SUPPORT_WEAK_SYMBOL)
        if(!::uwvm2::uwvm::wasm::storage::weak_symbol.empty()) { return false; }
# endif
        for(auto const& provider : ::uwvm2::uwvm::wasm::storage::preload_local_imported)
        { if(provider.ptr == nullptr || provider.get_module_name() != u8"wasi_snapshot_preview1") { return false; } }
        for(auto const& preload : ::uwvm2::uwvm::wasm::storage::active_preloaded_wasm())
        { if(!preload.has_owned_source_image()) { return false; } }
        return true;
#else
        return false;
#endif
    }
    // Actual owning CLI preparation: real parse/init occurs once in the
    // existing drained maintenance callback. Numeric eligibility is decided
    // later by the actual full validator/publisher, with native fallback.
    // Native administration/flags/preload mutations are externally serialized.
    [[nodiscard]] inline int prepare_owned_full_cli_source(bool retain_immutable_image = false, bool restart_after_retirement = false, dev_rebuild_input* rebuild = nullptr) noexcept
    {
#if (defined(UWVM_RUNTIME_UWVM_INTERPRETER) || defined(UWVM_RUNTIME_LLVM_JIT)) && \
    defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS)
        namespace mode = ::uwvm2::uwvm::runtime::runtime_mode;
        bool supported_full_backend{};
# if defined(UWVM_RUNTIME_UWVM_INTERPRETER)
        supported_full_backend = supported_full_backend ||
            mode::global_runtime_compiler == mode::runtime_compiler_t::uwvm_interpreter_only;
# endif
# if defined(UWVM_RUNTIME_LLVM_JIT)
        supported_full_backend = supported_full_backend ||
            mode::global_runtime_compiler == mode::runtime_compiler_t::llvm_jit_only;
# endif
        if(!supported_full_backend ||
           mode::global_runtime_mode != mode::runtime_mode_t::full_compile ||
           !::uwvm2::uwvm::cmdline::wasm_file_ppos ||
           ::uwvm2::uwvm::wasm::storage::preloaded_wasm.size() > 4095u)
        { return static_cast<int>(retval::parameter_error); }
# if defined(UWVM_SUPPORT_PRELOAD_DL)
        if(!::uwvm2::uwvm::wasm::storage::preloaded_dl.empty())
        {
            // Explicitly loaded trusted drivers have process lifetime. They do
            // not prevent owning the genuine Wasm parser/registry/code graph.
            // They remain foreign providers in the sealed census: this does
            // not authorize cache reuse, arbitrary memory snapshots, native ASM
            // debugging or restoring a driver into a new execution world.
            if(restart_after_retirement || rebuild) { return static_cast<int>(retval::parameter_error); }
#  if UWVM2_HAS_THREADED_PROFILE_HOST_API
            if(::uwvm2::runtime::lib::runtime_threaded_profile_builtin_requested_host_api())
            { return static_cast<int>(retval::parameter_error); }
#  endif
        }
# endif
# if defined(UWVM_SUPPORT_WEAK_SYMBOL)
        if(!::uwvm2::uwvm::wasm::storage::weak_symbol.empty()) { return static_cast<int>(retval::parameter_error); }
# endif
        if(rebuild && (!retain_immutable_image || !restart_after_retirement || !rebuild->image || !dev_rebuild_supported() ||
            rebuild->module_index > ::uwvm2::uwvm::wasm::storage::active_preloaded_wasm().size()))
        { return static_cast<int>(retval::parameter_error); }
        struct context
        {
            // Real owned inputs captured before the old selection is retired.
            ::std::u8string path, rename;
            ::uwvm2::utils::control::owned_file_image::owner image{};
            ::std::vector<::uwvm2::uwvm::runtime::full::full_preload_input> preloads{};
# if defined(UWVM_RUNTIME_LLVM_JIT)
            mode::runtime_llvm_jit_exception_dispatch_t dispatch{mode::global_runtime_llvm_jit_exception_dispatch};
# endif
            bool restart{}, preserve_host_state{};
            bool ordinary_cache_input{}; // Derived below from actual CLI setup, never supplied as a certificate.
            int result{static_cast<int>(retval::parameter_error)};
        } state{};
        state.restart = restart_after_retirement;
        state.preserve_host_state = rebuild != nullptr;
# if defined(UWVM_RUNTIME_LLVM_JIT) && !defined(UWVM_MODULE)
        state.ordinary_cache_input = !retain_immutable_image && !restart_after_retirement &&
            ::uwvm2::uwvm::wasm::storage::execute_wasm_mode == ::uwvm2::uwvm::wasm::base::mode::run &&
            mode::global_runtime_mode == mode::runtime_mode_t::full_compile &&
            mode::global_runtime_compiler == mode::runtime_compiler_t::llvm_jit_only &&
            ::uwvm2::uwvm::wasm::storage::preloaded_wasm.empty();
# endif
        try
        {
            auto const path{::uwvm2::uwvm::cmdline::wasm_file_ppos->str};
            auto const rename{rebuild ? ::uwvm2::uwvm::wasm::storage::active_execute_wasm().module_name :
                ::uwvm2::uwvm::wasm::storage::execute_wasm.module_name};
            state.path = ::fast_io::u8concat_std(path);
            state.rename = ::fast_io::u8concat_std(rename);
            if(rebuild)
            {
                if(rebuild->module_index == 0u)
                { state.path = ::std::move(rebuild->path); state.image = ::std::move(rebuild->image); }
                else
                {
                    auto const& original{::uwvm2::uwvm::wasm::storage::active_execute_wasm()};
                    state.path = ::fast_io::u8concat_std(original.file_name);
                    auto input{::uwvm2::utils::control::owned_file_image::copy_owned_bytes(
                        {reinterpret_cast<::std::byte const*>(original.source_cbegin()), original.source_size()},
                        ::uwvm2::utils::control::owned_file_image::maximum_bytes)};
                    if(!input) { return static_cast<int>(retval::load_main_module_error); }
                    state.image = ::std::move(input.image);
                }
            }
            else if(retain_immutable_image)
            {
                // Cold host input DATA only. Read into private owned bytes BEFORE
                // any parser/initializer body/type/segment pointer can escape.
                // Never hash/copy an already parsed map and pretend its borrows
                // were rebound. This performs no code-body validation/translation.
                auto loaded{::uwvm2::utils::control::owned_file_image::read(state.path,
                    ::uwvm2::utils::control::owned_file_image::maximum_bytes)};
                if(!loaded)
                {
                    ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                        u8"uwvm: [error] Unable to retain immutable debugger Wasm input before parsing; owned-image status=",
                        ::fast_io::mnp::dec(static_cast<unsigned>(loaded.error)), u8".\n\n");
                    return static_cast<int>(retval::load_main_module_error);
                }
                state.image = ::std::move(loaded.image);
            }
            // Copy command-line preload INPUTS before old source selection is
            // retired. Never move/reuse a previously parsed map/Core3 context:
            // every final source-owned preload below is parsed from scratch.
            auto const& original_preloads{rebuild ? ::uwvm2::uwvm::wasm::storage::active_preloaded_wasm() :
                ::uwvm2::uwvm::wasm::storage::preloaded_wasm};
            state.preloads.reserve(original_preloads.size());
            auto remaining_bytes{::uwvm2::utils::control::owned_file_image::maximum_bytes};
            if(state.image)
            {
                if(state.image->size() > remaining_bytes) { return static_cast<int>(retval::load_main_module_error); }
                remaining_bytes -= state.image->size(); // BEFORE any next private-image allocation.
            }
            ::std::size_t preload_index{};
            for(auto const& original : original_preloads)
            {
                ++preload_index;
                ::uwvm2::uwvm::runtime::full::full_preload_input input{};
                input.file_name = ::fast_io::u8concat_std(original.file_name);
                input.module_name = ::fast_io::u8concat_std(original.module_name);
                input.parameters = original.wasm_parameter; // Exact original per-module feature policy.
                if(rebuild && rebuild->module_index == preload_index)
                {
                    if(!rebuild->image || rebuild->image->size() > remaining_bytes)
                    { return static_cast<int>(retval::load_main_module_error); }
                    remaining_bytes -= rebuild->image->size();
                    input.image = ::std::move(rebuild->image); input.file_name = ::std::move(rebuild->path);
                }
                else if(retain_immutable_image)
                {
                    auto loaded{rebuild ? ::uwvm2::utils::control::owned_file_image::copy_owned_bytes(
                        {reinterpret_cast<::std::byte const*>(original.source_cbegin()), original.source_size()}, remaining_bytes) :
                        ::uwvm2::utils::control::owned_file_image::read(input.file_name, remaining_bytes)};
                    if(!loaded || loaded.image->size() > remaining_bytes)
                    {
                        ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                            u8"uwvm: [error] Unable to retain immutable debugger preload before parsing; shared owned-image quota=1 GiB.\n\n");
                        return static_cast<int>(retval::load_main_module_error);
                    }
                    remaining_bytes -= loaded.image->size(); input.image = ::std::move(loaded.image);
                }
                state.preloads.push_back(::std::move(input));
            }
            auto const prepare{[](void* pointer) noexcept -> bool
            {
                auto& ctx{*static_cast<context*>(pointer)};
                try
                {
                    if(ctx.restart && !::uwvm2::uwvm::runtime::full::builtin_wasip1_loader_identity::retire_actual_builtin_after_source_drain())
                    { return false; }
                    auto candidate{::uwvm2::uwvm::runtime::full::full_source_instance::create_unparsed(
                        ::std::move(ctx.path), ::std::move(ctx.rename), ::std::move(ctx.preloads))};
                    if(!::uwvm2::uwvm::runtime::full::select_unparsed_full_source_after_drain(candidate)) { return false; }
                    auto const loaded{::uwvm2::uwvm::wasm::loader::load_wasm_file(
                        candidate->file_for_native_initialization(), candidate->owned_file_name(), candidate->owned_rename(),
                        ::uwvm2::uwvm::wasm::storage::wasm_parameter, ::std::move(ctx.image))};
                    if(loaded != ::uwvm2::uwvm::wasm::loader::load_wasm_file_rtl::ok)
                    { ctx.result = static_cast<int>(retval::load_main_module_error); return false; }
                    auto& preloads{candidate->preloaded_files_for_native_initialization()};
                    for(::std::size_t index{}; index != preloads.size(); ++index)
                    {
                        // [FINAL source-owned preload vector] end
                        // [safe] index<size BEFORE WF access; vector never grows
                        // after any name/type/body pointer is installed by parser.
                        auto& file{preloads.index_unchecked(index)};
                        auto const loaded_preload{::uwvm2::uwvm::wasm::loader::load_wasm_file(file,
                            file.file_name, file.module_name, file.wasm_parameter,
                            candidate->take_preloaded_image_for_native_initialization(index))};
                        if(loaded_preload != ::uwvm2::uwvm::wasm::loader::load_wasm_file_rtl::ok)
                        { ctx.result = static_cast<int>(retval::load_main_module_error); return false; }
                    }
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
                    // Owning the parsed source must not bypass the ordinary ROS
                    // admission sequence. Validate all loaded module bindings and
                    // attest configured gateways before any runtime initialization.
                    if(::uwvm2::uwvm::imported::ros::policy_configured)
                    {
                        if(auto result{::uwvm2::uwvm::imported::ros::validate_loaded_policy(true)}; !result)
                        {
                            ::uwvm2::uwvm::imported::ros::print_policy_error(result);
                            ctx.result = static_cast<int>(retval::parameter_error);
                            return false;
                        }
                        if(auto result{::uwvm2::uwvm::imported::ros::host::preflight()}; !result)
                        {
                            ::uwvm2::uwvm::imported::ros::print_policy_error(result);
                            ctx.result = static_cast<int>(retval::parameter_error);
                            return false;
                        }
                    }
#endif
                    // Native standard pipeline, with original provider behavior.
                    // No guest may execute inside this maintenance callback.
                    if(auto const code{load_local_modules(ctx.preserve_host_state)}; code != static_cast<int>(retval::ok))
                    { ctx.result = code; return false; }
                    if(auto const code{load_weak_symbol_modules()}; code != static_cast<int>(retval::ok))
                    { ctx.result = code; return false; }
                    if(::uwvm2::uwvm::wasm::loader::construct_all_module_and_check_duplicate_module() !=
                       ::uwvm2::uwvm::wasm::loader::load_and_check_modules_rtl::ok ||
                       ::uwvm2::uwvm::wasm::loader::check_import_exist_and_detect_cycles() !=
                       ::uwvm2::uwvm::wasm::loader::load_and_check_modules_rtl::ok)
                    { ctx.result = static_cast<int>(retval::check_module_error); return false; }
                    if(!::uwvm2::uwvm::runtime::initializer::initialize_runtime(true))
                    { ctx.result = static_cast<int>(retval::check_module_error); return false; }
                    if(!candidate->seal_actual_initializer()) { return false; }
                    auto const* main{candidate->initialized_main_module()};
                    if(main == nullptr) { return false; }
                    // Numeric admission is a later real full validation decision.
                    // Any unsupported body/import/schema keeps this sealed source
                    // and its normal native ABI; never throw away an initialized
                    // valid module just to rerun ordinary loader/initializer.
                    // This frozen cold request supplies no authority. The original
                    // real validator/factory still decides numeric eligibility.
# if defined(UWVM_RUNTIME_LLVM_JIT)
                    // INT uses its actual complete validator publication below;
                    // selecting INT never requests a private LLVM numeric ABI.
                    if(mode::global_runtime_compiler == mode::runtime_compiler_t::llvm_jit_only &&
                       ctx.dispatch != mode::runtime_llvm_jit_exception_dispatch_t::native_unwind)
                    { candidate->request_numeric_full_after_actual_initializer(); }
# endif
# if defined(UWVM_RUNTIME_LLVM_JIT) && !defined(UWVM_MODULE)
                    if(ctx.ordinary_cache_input)
                    { candidate->cache_cli_origin_ = ::uwvm2::uwvm::runtime::full::full_source_instance::cache_cli_origin::ordinary_full_cli; }
# endif
                    ctx.result = static_cast<int>(retval::ok);
                    return true;
                }
                catch(...) { return false; }
            }};
            if(!::uwvm2::runtime::lib::replace_full_source_after_drain_host_api(prepare, ::std::addressof(state)))
            { return state.result == static_cast<int>(retval::ok) ? static_cast<int>(retval::parameter_error) : state.result; }
            return state.result;
        }
        catch(...) { return state.result; }
#else
        static_cast<void>(retain_immutable_image);
        static_cast<void>(restart_after_retirement);
        static_cast<void>(rebuild);
        return static_cast<int>(retval::parameter_error);
#endif
    }
}
#ifndef UWVM_MODULE
# include <uwvm2/uwvm/runtime/macro/pop_macros.h>
# include <uwvm2/utils/macro/pop_macros.h>
#endif
