#pragma once
#include <uwvm2/utils/container/standard.h>
// Same-walk compiler state only. None of these LLVM handles escape into a
// checkpoint file or identify a live runtime publication.
struct runtime_local_func_llvm_jit_emit_state_t;
struct llvm_jit_checkpoint_resume_dispatch_emit_state
{
    runtime_local_func_llvm_jit_emit_state_t* actual_state{};
    ::uwvm2::runtime::checkpoint::function_plan* actual_plan{};
    ::llvm::SwitchInst* dispatch{};
    ::llvm::Value* payload{};
    ::llvm::Value* payload_bytes{};
    ::llvm::Value* original_flags{};
    ::llvm::Value* flag_count{};
    ::llvm::AllocaInst* executed_flags{};
    ::llvm::BasicBlock* reject{};
#if defined(__linux__) && defined(__arm__) && defined(__ARM_PCS_VFP)
    struct numeric_local_landing
    {
        ::llvm::BasicBlock* extent{};
        ::llvm::SwitchInst* sites{};
    };
    // Local declarations and allocas belong to this one function. Each
    // packet size needs its own extent check before reading its local prefix.
    ::uwvm2::utils::container::std_map<::std::size_t, numeric_local_landing> numeric_landings{};
#endif
    ::uwvm2::utils::container::std_vector<::std::uint64_t> installed_sites{};
    // Compiler-owned zero SSA inputs remain zero in the public normal body.
    // Only the exact-original-ABI private clone substitutes authenticated
    // owner/TLS getter results. Normal execution folds its selector away and
    // performs no TLS query; no extra parameter invalidates genuine musttail.
    ::std::array<::llvm::FreezeInst*, 5u> placeholders{};
};

// Compiler-owned lexical call edge, not serialized execution authority.
struct llvm_jit_checkpoint_call_emit_state
{
    ::std::uint64_t before_site{}, waiting_site{}, after_site{};
    ::std::size_t opcode_offset{}, return_offset{}, prefix_operands{};
    bool waiting_published{};
    // A genuine restore-only child invocation returns here while THIS parent
    // activation/root frame remains live. These are compiler-owned SSA handles,
    // never native addresses, wire labels or independent resume permission.
    ::llvm::BasicBlock* nested_return_predecessor{};
    ::uwvm2::utils::container::std_vector<::llvm::Value*> nested_after_values{};
};
