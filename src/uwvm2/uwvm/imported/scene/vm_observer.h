/** @file
 * @brief Bounded read-only VM metadata behind an independent vm/runtime grant.
 * @details The host authenticates the actual caller and holds the observation
 * lease before this projection. Startup compiler settings and the registered
 * declaration world remain stable under the caller's execution lease. Counts
 * describe registered declarations, not executing threads or live instances.
 * No module names, host paths, process IDs, addresses, source bytes, credentials
 * or command-line arguments are exported. Compiled support is explicitly
 * distinct from per-module permission; this reader cannot enable a feature.
 */
#pragma once
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/scene/permission_report.h>
#include <uwvm2/uwvm/wasm/storage/all_module.h>
#include <uwvm2/uwvm/runtime/runtime_mode/impl.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#endif
namespace uwvm2::uwvm::imported::scene::vm_observer
{
    namespace report=::uwvm2::imported::ros::scene::permission_report;
    using error=::uwvm2::imported::ros::scene::error;
    /** @brief Immutable canonical labels, validated during compilation rather than per query. */
    namespace labels
    {
        inline constexpr report::literal_row query_only{"vm","query-only"};
        inline constexpr report::literal_row address_bits{"vm","host-address-bits"};
        inline constexpr report::literal_row selected_interpreter{"execution","compiler","interpreter"};
        inline constexpr report::literal_row selected_llvm{"execution","compiler","llvm"};
        inline constexpr report::literal_row selected_unavailable{"execution","compiler","unavailable"};
        inline constexpr report::literal_row execution_mode{"execution","mode","full-compile"};
        inline constexpr report::literal_row thread_ceiling{"execution","compile-thread-ceiling"};
        inline constexpr report::literal_row interpreter{"compiled","interpreter"};
        inline constexpr report::literal_row llvm{"compiled","llvm"};
        inline constexpr report::literal_row preloadwasm{"compiled","preloadwasm"};
        inline constexpr report::literal_row rosv2{"compiled","rosv2"};
        inline constexpr report::literal_row preloaddl{"compiled","preloaddl"};
        inline constexpr report::literal_row wasip1{"compiled","wasip1"};
        inline constexpr report::literal_row ros1{"compiled","ros1v1"};
        inline constexpr report::literal_row ros2{"compiled","ros2v1"};
        inline constexpr report::literal_row main_wasm{"registered-declarations","main-wasm"};
        inline constexpr report::literal_row preload_wasm{"registered-declarations","preload-wasm"};
        inline constexpr report::literal_row preload_dl{"registered-declarations","preload-dl"};
        inline constexpr report::literal_row builtin{"registered-declarations","builtin"};
    }
    /** @brief Project only closed, scalar facts; no resource or policy mutation. */
    inline error read(report::page& output)
    {
        namespace mode=::uwvm2::uwvm::runtime::runtime_mode;
        using kind=::uwvm2::uwvm::wasm::type::module_type_t;
        constexpr bool interpreter=
#if defined(UWVM_RUNTIME_UWVM_INTERPRETER)
            true;
#else
            false;
#endif
        constexpr bool compiler=
#if defined(UWVM_RUNTIME_LLVM_JIT)
            true;
#else
            false;
#endif
        enum class engine{unavailable,interpreter,llvm};auto selected=engine::unavailable;
#if defined(UWVM_RUNTIME_UWVM_INTERPRETER)
        if(mode::global_runtime_compiler==mode::runtime_compiler_t::uwvm_interpreter_only)selected=engine::interpreter;
#endif
#if defined(UWVM_RUNTIME_LLVM_JIT)
        if(mode::global_runtime_compiler==mode::runtime_compiler_t::llvm_jit_only)selected=engine::llvm;
#endif
        if(!output.add(labels::query_only,1)||!output.add(labels::address_bits,sizeof(void*)*8))return error::too_large;
        bool selected_written=selected==engine::interpreter?output.add(labels::selected_interpreter,0):
            selected==engine::llvm?output.add(labels::selected_llvm,0):output.add(labels::selected_unavailable,0);
        if(!selected_written||!output.add(labels::execution_mode,0)||
           !output.add(labels::thread_ceiling,mode::global_runtime_compile_threads_resolved)||
           !output.add(labels::interpreter,interpreter)||!output.add(labels::llvm,compiler)||
           !output.add(labels::preloadwasm,1)||!output.add(labels::rosv2,1))return error::too_large;
        constexpr bool preloaddl=
#if defined(UWVM_SUPPORT_PRELOAD_DL)
            true;
#else
            false;
#endif
        constexpr bool wasip1=
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
            true;
#else
            false;
#endif
        constexpr bool ros1=
#if defined(UWVM_IMPORT_ROS1)
            true;
#else
            false;
#endif
        constexpr bool ros2=
#if defined(UWVM_IMPORT_ROS2)
            true;
#else
            false;
#endif
        if(!output.add(labels::preloaddl,preloaddl)||!output.add(labels::wasip1,wasip1)||
           !output.add(labels::ros1,ros1)||!output.add(labels::ros2,ros2))return error::too_large;
        std::array<std::uint64_t,4> counts{};std::size_t visited{};
        for(auto const& entry : ::uwvm2::uwvm::wasm::storage::all_module)
        {
            if(visited==report::maximum_scan)return error::too_large;
            ++visited;
            switch(entry.second.type)
            {
            case kind::exec_wasm:++counts[0];break;
            case kind::preloaded_wasm:++counts[1];break;
#if defined(UWVM_SUPPORT_PRELOAD_DL)
            case kind::preloaded_dl:++counts[2];break;
#endif
            case kind::local_import:++counts[3];break;
            default:return error::unsupported;
            }
        }
        if(!output.add(labels::main_wasm,counts[0])||!output.add(labels::preload_wasm,counts[1])||
           !output.add(labels::preload_dl,counts[2])||!output.add(labels::builtin,counts[3]))return error::too_large;
        return output.finish(true,true)?error::success:error::too_large;
    }
}
#ifndef UWVM_MODULE
#include <uwvm2/uwvm/runtime/macro/pop_macros.h>
#endif
