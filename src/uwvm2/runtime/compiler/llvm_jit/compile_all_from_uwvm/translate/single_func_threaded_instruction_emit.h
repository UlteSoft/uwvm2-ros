// Private Child instruction control, independent of ordinary debugger roots,
// global module registries and checkpoint activation credentials. Included in
// the shared emitter after runtime_local_func_llvm_jit_emit_state_t.
[[nodiscard]] inline bool emit_runtime_private_threaded_instruction_point(
    runtime_local_func_llvm_jit_emit_state_t& state,bool lexical_control_edge=false) noexcept
{
    auto const target=threaded_host_emission_scope::instruction_target();
    if(!target)return true;
    if(!state.valid||!state.ir_builder||!state.local_func_storage_ptr||state.control_stack.empty()||
       state.emit_debug_safe_points)return false;
    if(!lexical_control_edge&&!state.control_stack.back().is_reachable)return true;
    auto const offset=state.current_wasm_op_offset;
    if(offset==SIZE_MAX)return false;
    if(!lexical_control_edge&&state.last_debug_instruction_offset==offset)return true;
    auto& builder=*state.ir_builder;auto* module=builder.GetInsertBlock()->getModule();
    auto const function=state.local_func_storage_ptr->function_index;::std::size_t ordinal{};
    if(!threaded_host_emission_scope::instruction_site(function,offset,ordinal))return false;
    auto* word=builder.getIntNTy(sizeof(::std::size_t)*CHAR_BIT);
    auto* type=::llvm::FunctionType::get(builder.getVoidTy(),{word,word,word},false);
    constexpr char name[]{"uwvm.threaded.instruction.v1"};
    auto* declaration=module->getFunction(name);
    if(!declaration)
    {
        if(module->getNamedValue(name))return false;
        declaration=::llvm::Function::Create(type,::llvm::Function::ExternalLinkage,name,*module);
        declaration->addFnAttr(::llvm::Attribute::NoUnwind);
        declaration->addFnAttr(::llvm::Attribute::NoInline);
        declaration->addFnAttr(::llvm::Attribute::Convergent);
        declaration->addFnAttr(::llvm::Attribute::NoDuplicate);
        apply_llvm_jit_host_calling_conv(*declaration);
    }
    if(declaration->getFunctionType()!=type||!declaration->isDeclaration()||
       threaded_host_emission_scope::bind(module,*declaration,target)!=threaded_host_emission_scope::selection::owned)return false;
    auto* call=apply_llvm_jit_host_calling_conv(builder.CreateCall(type,declaration,
        {::llvm::ConstantInt::get(word,function),::llvm::ConstantInt::get(word,ordinal),::llvm::ConstantInt::get(word,offset)}));
    call->setTailCallKind(::llvm::CallInst::TCK_NoTail);
    call->setDoesNotThrow(); // Deliberately side-effecting: never readonly/nosync/speculatable.
    state.last_debug_instruction_offset=offset;return true;
}
