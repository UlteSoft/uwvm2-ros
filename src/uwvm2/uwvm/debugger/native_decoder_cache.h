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
        ::std::unique_ptr<slot> next{};

        ~slot() noexcept
        {
            // Destroy an arbitrary nesting depth iteratively at thread exit.
            while(next)
            {
                auto retired{::std::move(next)};
                next = ::std::move(retired->next);
            }
        }
    };

    template<typename Decoder>
    [[nodiscard]] inline ::std::unique_ptr<slot<Decoder>>& local_slot() noexcept
    {
        // One idle chain per decoder type and host thread. Each active lease
        // removes its node; nested queries never share mutable LLVM state.
        // The chain grows only to the observed simultaneous lease depth.
        static thread_local ::std::unique_ptr<slot<Decoder>> value{};
        return value;
    }

    template<typename Decoder, typename Description>
    class lease final
    {
        ::std::unique_ptr<slot<Decoder>> owner_{};
        Decoder* decoder_{};

        void acquire(Description const& description)
        {
            if(!native_target_metadata::valid(description)) { return; }
            auto& idle{local_slot<Decoder>()};
            if(idle)
            {
                owner_ = ::std::move(idle);
                idle = ::std::move(owner_->next);
            }
            else { owner_.reset(new (::std::nothrow) slot<Decoder>{}); }
            if(!owner_) { return; }
            if(!owner_->decoder || !owner_->key.matches(description))
            {
                owner_->decoder.reset();
                owner_->key.assign(description);
                owner_->decoder.reset(new (::std::nothrow) Decoder{description});
            }
            if(owner_->decoder && *owner_->decoder) { decoder_ = owner_->decoder.get(); }
            else { owner_.reset(); }
        }
    public:
        explicit lease(Description const& description) noexcept
        {
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
            try { acquire(description); }
            catch(...) { owner_.reset(); decoder_ = nullptr; }
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
            if(owner_)
            {
                auto& idle{local_slot<Decoder>()};
                // Return without allocation, including in noexcept builds.
                // Keep inner leases instead of destroying their MC contexts
                // when an outer query finishes.
                owner_->next = ::std::move(idle);
                idle = ::std::move(owner_);
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
