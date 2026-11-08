/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)           *
 * Copyright (c) 2025-present UlteSoft. All rights reserved.   *
 * Licensed under the APL-2.0 License (see LICENSE file).      *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <span>
# include <llvm/IR/IRBuilder.h>
# include <uwvm2/extension/memtag/instruction.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details
{
    struct disabled_memtag_ir_result
    {
        bool valid{};
        ::llvm::Value* value{};
    };

    // No runtime flag test, shadow address, tag arithmetic or helper call is
    // emitted in ignore mode. Zeroing and ordinary subtraction are actual data
    // effects and must survive. The caller supplies its normal checked fill
    // path, including memory64, shared-memory and imported-memory handling.
    template<typename EmitZero>
    [[nodiscard]] inline disabled_memtag_ir_result emit_disabled_memtag_ir(
        ::llvm::IRBuilder<>& builder, ::uwvm2::extension::memtag::opcode operation,
        ::llvm::IntegerType* address_type, ::std::span<::llvm::Value* const> arguments,
        EmitZero&& emit_zero) noexcept
    {
        namespace mt = ::uwvm2::extension::memtag;
        auto const* descriptor{mt::find_instruction(static_cast<unsigned>(operation))};
        if(descriptor == nullptr || arguments.size() != descriptor->operand_count ||
           address_type == nullptr || (address_type->getBitWidth() != 32u && address_type->getBitWidth() != 64u))
        { return {}; }
        for(auto const argument : arguments)
        { if(argument == nullptr || argument->getType() != address_type) { return {}; } }
        if(operation == mt::opcode::add) { return {}; } // Published undefined placeholder.
        if(operation == mt::opcode::status || operation == mt::opcode::extract || operation == mt::opcode::tagbits)
        { return {true, ::llvm::ConstantInt::get(address_type, 0u)}; }
        if(operation == mt::opcode::startbit)
        { return {true, ::llvm::ConstantInt::get(address_type, address_type->getBitWidth())}; }
        if(operation == mt::opcode::sub)
        { return {true, builder.CreateSub(arguments[0], arguments[1])}; }
        if(descriptor->zeroes_memory && !emit_zero(arguments[0], arguments[1])) { return {}; }
        return {true, descriptor->result_count == 0u ? nullptr : arguments[0]};
    }
}
