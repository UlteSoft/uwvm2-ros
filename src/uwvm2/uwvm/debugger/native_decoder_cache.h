/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <cstddef>
# include <cstdint>
# include <memory>
# include <new>
# include <span>
# include <string>
# include <string_view>
# include <utility>
# include "native_target_metadata.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::uwvm::debugger::native_decoder_cache
{
    // Only target-description DATA and MC machinery survive a request.
    // Never retain code bytes, addresses, decoded instructions, captures,
    // stop identifiers, generations, registers or execution authority.
    struct target_key
    {
        ::std::string triple{}, cpu{}, features{};
        unsigned version{}, pointer_bits{}, maximum{}, alignment{};
        bool little_endian{};

        template<typename Description>
        [[nodiscard]] bool matches(Description const& value) const noexcept
        {
            return version == value.description_version && pointer_bits == value.pointer_bits &&
                maximum == value.maximum_instruction_bytes && alignment == value.minimum_instruction_alignment &&
                little_endian == value.little_endian &&
                triple == ::std::string_view{value.triple.data(), value.triple_size} &&
                cpu == ::std::string_view{value.cpu.data(), value.cpu_size} &&
                features == ::std::string_view{value.features.data(), value.features_size};
        }
        template<typename Description>
        void assign(Description const& value)
        {
            triple.assign(value.triple.data(), value.triple_size);
            cpu.assign(value.cpu.data(), value.cpu_size);
            features.assign(value.features.data(), value.features_size);
            version = value.description_version; pointer_bits = value.pointer_bits;
            maximum = value.maximum_instruction_bytes; alignment = value.minimum_instruction_alignment;
            little_endian = value.little_endian;
        }
    };

    template<typename Decoder>
    struct slot
    {
        target_key key{};
        ::std::unique_ptr<Decoder> decoder{};
    };

    template<typename Decoder>
    [[nodiscard]] inline slot<Decoder>& local_slot() noexcept
    {
        // One idle owner per decoder type and host thread, never a global
        // mutable LLVM context. A lease removes it from this slot entirely.
        static thread_local slot<Decoder> value{};
        return value;
    }

    template<typename Decoder, typename Description>
    class lease final
    {
        target_key key_{};
        ::std::unique_ptr<Decoder> decoder_{};

        void acquire(Description const& description)
        {
            if(!native_target_metadata::valid(description)) { return; }
            auto& idle{local_slot<Decoder>()};
            if(idle.decoder && idle.key.matches(description))
            {
                key_ = ::std::move(idle.key);
                decoder_ = ::std::move(idle.decoder);
                return;
            }
            // A nested/reentrant query owns a separate context. It cannot
            // mutate or evict a context that the outer query is still using.
            idle.decoder.reset();
            key_.assign(description);
            decoder_.reset(new (::std::nothrow) Decoder{description});
            if(decoder_ && !*decoder_) { decoder_.reset(); }
        }
    public:
        explicit lease(Description const& description) noexcept
        {
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
            try { acquire(description); }
            catch(...) { decoder_.reset(); }
#else
            acquire(description);
#endif
        }
        lease(lease const&) = delete;
        lease& operator=(lease const&) = delete;
        lease(lease&&) = delete;
        lease& operator=(lease&&) = delete;
        ~lease() noexcept
        {
            if(decoder_)
            {
                auto& idle{local_slot<Decoder>()};
                idle.decoder = ::std::move(decoder_);
                idle.key = ::std::move(key_);
            }
        }
        [[nodiscard]] explicit operator bool() const noexcept { return decoder_ && static_cast<bool>(*decoder_); }
        [[nodiscard]] auto decode(::std::uintptr_t pc, ::std::span<::std::uint8_t const> bytes) noexcept
            -> decltype(::std::declval<Decoder&>().decode(pc, bytes))
        { return decoder_ ? decoder_->decode(pc, bytes) : decltype(decoder_->decode(pc, bytes)){}; }
        [[nodiscard]] ::std::size_t instruction_size(::std::uintptr_t pc, ::std::span<::std::uint8_t const> bytes) noexcept
            requires requires(Decoder& value) { value.instruction_size(pc, bytes); }
        { return decoder_ ? decoder_->instruction_size(pc, bytes) : 0u; }
        [[nodiscard]] ::std::size_t fixed_instruction_bytes() const noexcept
            requires requires(Decoder const& value) { value.fixed_instruction_bytes(); }
        { return decoder_ ? decoder_->fixed_instruction_bytes() : 0u; }
        [[nodiscard]] bool linear_run_safe(::std::uintptr_t pc, ::std::span<::std::uint8_t const> bytes) noexcept
            requires requires(Decoder& value) { value.linear_run_safe(pc, bytes); }
        { return decoder_ && decoder_->linear_run_safe(pc, bytes); }
        [[nodiscard]] bool relocation_neutral(::std::uintptr_t pc, ::std::span<::std::uint8_t const> bytes) noexcept
            requires requires(Decoder& value) { value.relocation_neutral(pc, bytes); }
        { return decoder_ && decoder_->relocation_neutral(pc, bytes); }
    };
}
