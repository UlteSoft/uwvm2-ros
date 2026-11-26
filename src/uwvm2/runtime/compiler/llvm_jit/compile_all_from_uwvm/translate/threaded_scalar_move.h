// In the native threaded compiler this operand already covers the complete
// authorized guest span. x86-64 MOV accepts byte alignment and accesses
// exactly the indicated bytes. A racing plain Wasm access may tear across
// bytes. Each byte still comes from a real write. Keep the effect opaque to
// LLVM's data-race rules and ordered with the surrounding checks and effects.
// Wasm atomic opcodes use their separate, naturally aligned SC instructions.
[[nodiscard]] inline ::llvm::InlineAsm* threaded_scalar_move(::llvm::Type* type, bool store) noexcept
{
#if defined(__linux__) && defined(__x86_64__) && __SIZEOF_POINTER__ == 8 && !LLVM_THREAD_SANITIZER_BUILD
    // The private child selects its native target after initial IR admission.
    // Instrumented builds retain byte atomics visible to the JIT TSan pass.
    if(type == nullptr || !type->isIntegerTy()) { return nullptr; }
    char const* instruction{};
    switch(type->getIntegerBitWidth())
    {
        case 16u: instruction = "movw $1, $0"; break;
        case 32u: instruction = "movl $1, $0"; break;
        case 64u: instruction = "movq $1, $0"; break;
        default: return nullptr;
    }
    auto& context{type->getContext()};
    auto* pointer{::llvm::PointerType::getUnqual(context)};
    auto* signature{store ? ::llvm::FunctionType::get(::llvm::Type::getVoidTy(context), {pointer, type}, false)
                          : ::llvm::FunctionType::get(type, {pointer}, false)};
    return ::llvm::InlineAsm::get(signature, instruction, store ? "=*m,r,~{memory}" : "=r,*m,~{memory}", true);
#else
    static_cast<void>(type);
    static_cast<void>(store);
    return nullptr;
#endif
}

[[nodiscard]] inline ::llvm::CallInst* emit_threaded_scalar_move(::llvm::IRBuilder<>& builder, ::llvm::Value* pointer,
    ::llvm::Type* type, ::llvm::Value* stored = nullptr) noexcept
{
    auto* move{threaded_scalar_move(type, stored != nullptr)};
    if(move == nullptr) { return nullptr; }
    auto* call{stored ? builder.CreateCall(move->getFunctionType(), move, {pointer, stored})
                      : builder.CreateCall(move->getFunctionType(), move, {pointer}, "threaded.plain.load")};
    call->addParamAttr(0u, ::llvm::Attribute::get(builder.getContext(), ::llvm::Attribute::ElementType, type));
    call->setDoesNotThrow();
    return call;
}

[[nodiscard]] inline bool is_threaded_scalar_move(::llvm::CallBase const& call) noexcept
{
    if(!call.isInlineAsm() || call.arg_empty()) { return false; }
    auto* move{threaded_scalar_move(call.getParamElementType(0u), call.getType()->isVoidTy())};
    return move != nullptr && call.getCalledOperand() == move && call.getFunctionType() == move->getFunctionType();
}
