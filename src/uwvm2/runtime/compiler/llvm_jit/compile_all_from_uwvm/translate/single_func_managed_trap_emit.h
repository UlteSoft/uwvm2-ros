// Included after the ordinary native exception cleanup emitter. Only compiler
// marked trap sites are changed; internal helper failures remain fatal.
[[nodiscard]] inline bool qualify_runtime_local_func_llvm_jit_managed_traps(runtime_local_func_llvm_jit_emit_state_t& state) noexcept
{
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS) && defined(UWVM_CPP_EXCEPTIONS) && defined(__linux__)
    if(!state.debug_activation_enabled) { return true; }
    if(!state.native_guest_exceptions || state.ir_builder == nullptr || state.llvm_function == nullptr) { return false; }
    ::llvm::SmallVector<::llvm::CallInst*, 16> sites;
    for(auto& block : *state.llvm_function) for(auto& instruction : block)
    {
        auto* call{::llvm::dyn_cast<::llvm::CallInst>(::std::addressof(instruction))};
        if(call && (call->getMetadata("uwvm.managed.trap.candidate") || call->getMetadata("uwvm.managed.helper.candidate") ||
            (call->getCalledFunction() && call->getCalledFunction()->hasFnAttribute("uwvm.managed.helper")))) { sites.push_back(call); }
    }
    auto& builder{*state.ir_builder};
    // The old insertion point may be a trap block split below. Nothing emits
    // further guest instructions after this final qualification pass.
    for(auto* call : sites)
    {
        auto const count{call->arg_size()};
        bool const helper{call->getMetadata("uwvm.managed.helper.candidate") != nullptr ||
            (call->getCalledFunction() && call->getCalledFunction()->hasFnAttribute("uwvm.managed.helper"))};
        if(!helper && count != 3u && count != 8u) { return false; }
        auto* original_target{call->getCalledOperand()};
        auto* block{call->getParent()};
        auto* continuation{block->splitBasicBlock(call->getNextNode(), "guest.trap.never")};
        block->getTerminator()->eraseFromParent();
        ::llvm::SmallVector<::llvm::Value*, 8> args;
        for(auto& arg : call->args()) { args.push_back(arg.get()); }
        auto* type{call->getFunctionType()};
        auto location{call->getDebugLoc()};
        call->removeFromParent();
        builder.SetInsertPoint(block); builder.SetCurrentDebugLocation(location);
        auto* bridge{helper ? original_target : count == 3u ? get_llvm_runtime_bridge_function_symbol_value_unwrapped<
            ::uwvm2::runtime::lib::llvm_jit_managed_runtime_trap>(builder, type) :
            get_llvm_runtime_bridge_function_symbol_value_unwrapped<
            ::uwvm2::runtime::lib::llvm_jit_managed_memory_trap>(builder, type)};
        auto* invoked{emit_runtime_local_func_llvm_jit_may_throw_call(state, type, bridge, args, false)};
        if(!::llvm::isa_and_nonnull<::llvm::InvokeInst>(invoked)) { return false; }
        call->replaceAllUsesWith(invoked); call->deleteValue();
        apply_llvm_jit_host_calling_conv(invoked);
        invoked->setMetadata("uwvm.managed.trap.cleanup", ::llvm::MDNode::get(builder.getContext(), {}));
        builder.CreateBr(continuation);
    }
    builder.ClearInsertionPoint();
#else
    static_cast<void>(state);
#endif
    return true;
}
