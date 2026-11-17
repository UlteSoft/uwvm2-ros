// Include after actual import-cache/effect classification. Observation keeps
// genuine defined-Wasm forwarding inside its logical caller chain. Foreign
// providers retain the host island; neither branch grants replay authority.
#pragma once
#if defined(UWVM_RUNTIME_LLVM_JIT)
extern "C++" void details::llvm_jit_checkpoint_observe_call_raw_from_generated_wasm_abi_bridge(
    ::std::uintptr_t module_address, ::std::uintptr_t function_index,
    ::std::uintptr_t result, ::std::size_t result_bytes,
    ::std::uintptr_t parameters, ::std::size_t parameter_bytes) UWVM_THROWS
{
    // Keep observation-only debug code on its existing classified dispatch.
    // A selected recording authenticates and journals the native leaf there;
    // ordinary execution never selects this debug bridge.
    details::llvm_jit_debug_classified_import_abi_bridge(
        module_address,function_index,result,result_bytes,parameters,parameter_bytes);
}
#endif
