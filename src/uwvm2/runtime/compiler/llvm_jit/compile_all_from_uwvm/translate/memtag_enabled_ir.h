/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <span>
# include <cstdint>
# include <llvm/IR/IRBuilder.h>
# include <uwvm2/extension/memtag/instruction.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details
{
    struct enabled_memtag_ir_result { bool valid{};::llvm::Value* value{}; };
    // Scalar tag arithmetic is inline IR. Memory/entropy callbacks carry only
    // their genuine effects; recomposing the result from the original low bits
    // preserves alias/offset reasoning across tag load, random and store calls.
    template<typename EmitOperation>
    [[nodiscard]] inline enabled_memtag_ir_result emit_enabled_memtag_ir(
        ::llvm::IRBuilder<>& builder,::uwvm2::extension::memtag::opcode operation,
        ::llvm::IntegerType* address_type,::std::span<::llvm::Value* const> arguments,
        EmitOperation&& emit_operation) noexcept
    {
        namespace mt=::uwvm2::extension::memtag;
        auto const* descriptor{mt::find_instruction(static_cast<unsigned>(operation))};
        if(descriptor==nullptr || arguments.size()!=descriptor->operand_count ||
           address_type==nullptr || (address_type->getBitWidth()!=32u && address_type->getBitWidth()!=64u))
        { return {}; }
        for(auto const argument:arguments)
        { if(argument==nullptr || argument->getType()!=address_type) { return {}; } }
        auto const geometry{address_type->getBitWidth()==32u ? mt::memory32_geometry : mt::memory64_software_geometry};
        auto constant{[&](::std::uint64_t value) { return ::llvm::ConstantInt::get(address_type,value); }};
        auto untag{[&](::llvm::Value* value) { return builder.CreateAnd(value,constant(geometry.address_mask),"memtag.address"); }};
        auto extract{[&](::llvm::Value* value) { return builder.CreateLShr(value,constant(geometry.start_bit),"memtag.tag"); }};
        auto insert{[&](::llvm::Value* pointer,::llvm::Value* tag)
        {
            return builder.CreateOr(untag(pointer),
                builder.CreateShl(builder.CreateAnd(tag,constant(geometry.tag_value_mask)),constant(geometry.start_bit)),"memtag.pointer");
        }};
        switch(operation)
        {
            case mt::opcode::status:return {true,constant(static_cast<unsigned>(mt::runtime_status::software_full))};
            case mt::opcode::tagbits:return {true,constant(geometry.tag_bits)};
            case mt::opcode::startbit:return {true,constant(geometry.start_bit)};
            case mt::opcode::extract:return {true,extract(arguments[0])};
            case mt::opcode::insert:return {true,insert(arguments[0],arguments[1])};
            case mt::opcode::copy:return {true,insert(arguments[0],extract(arguments[1]))};
            case mt::opcode::untag:return {true,untag(arguments[0])};
            case mt::opcode::sub:return {true,builder.CreateSub(untag(arguments[0]),untag(arguments[1]),"memtag.distance")};
            case mt::opcode::hint:return {true,insert(arguments[0],builder.CreateAdd(extract(arguments[1]),arguments[2]))};
            case mt::opcode::add:return {}; // Published undefined placeholder.
            default:break;
        }
        auto const effect{emit_operation(operation,arguments)};
        if(!effect.valid) { return {}; }
        if(descriptor->result_count==0u) { return {true,nullptr}; }
        if(operation==mt::opcode::untagstore || operation==mt::opcode::untagstorez)
        { return {true,untag(arguments[0])}; }
        if(operation==mt::opcode::hintstore || operation==mt::opcode::hintstorez)
        { return {true,insert(arguments[0],builder.CreateAdd(extract(arguments[2]),arguments[3]))}; }
        if(effect.value==nullptr || effect.value->getType()!=address_type) { return {}; }
        return {true,insert(arguments[0],extract(effect.value))};
    }
}
