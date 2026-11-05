/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)           *
 * Licensed under the APL-2.0 License (see LICENSE file).      *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <memory>
#include <span>
#include <vector>
#endif
#ifndef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details::immutable_globals
{
    struct value
    {
        ::std::uint64_t index{};
        unsigned char type{};
        ::std::array<unsigned char, 8> bits{};
    };
    class binding_owner;
    using owner = ::std::shared_ptr<binding_owner const>;
    // A new immutable binding owns a new native object. No old Wasm immutable
    // slot is modified. Each generated engine pins its precise binding owner.
    class binding_owner final
    {
    public:
        struct alignas(16) slot
        {
            value data{};
            union alignas(16) scalar
            {
                ::std::uint32_t i32; ::std::uint64_t i64; float f32; double f64;
                constexpr scalar() noexcept : i64{} {}
            } storage{};
            [[nodiscard]] void const* address() const noexcept
            {
                switch(data.type)
                {
                    case 0x7f: return ::std::addressof(storage.i32);
                    case 0x7e: return ::std::addressof(storage.i64);
                    case 0x7d: return ::std::addressof(storage.f32);
                    case 0x7c: return ::std::addressof(storage.f64);
                    default: return nullptr;
                }
            }
        };
    private:
        void const* module_{};
        ::std::vector<slot> slots_{};
        binding_owner() = default;
    public:
        binding_owner(binding_owner const&) = delete;
        binding_owner& operator=(binding_owner const&) = delete;
        [[nodiscard]] void const* module() const noexcept { return module_; }
        [[nodiscard]] ::std::span<slot const> slots() const noexcept { return slots_; }
        [[nodiscard]] slot const* find(::std::uint64_t index) const noexcept
        {
            auto const found{::std::lower_bound(slots_.begin(), slots_.end(), index,
                [](slot const& item, ::std::uint64_t n) { return item.data.index < n; })};
            return found != slots_.end() && found->data.index == index ? ::std::addressof(*found) : nullptr;
        }
        [[nodiscard]] ::std::vector<value> values() const
        {
            ::std::vector<value> out; out.reserve(slots_.size());
            for(auto const& item : slots_) { out.push_back(item.data); }
            return out;
        }
        [[nodiscard]] static owner create(void const* actual_module, ::std::span<value const> values)
        {
            static_assert(sizeof(float) == 4u && sizeof(double) == 8u);
            if(actual_module == nullptr || values.empty() || values.size() > 4096u) { return {}; }
            ::std::shared_ptr<binding_owner> result{new binding_owner}; result->module_ = actual_module;
            result->slots_.reserve(values.size());
            for(auto const& v : values)
            {
                if(!result->slots_.empty() && result->slots_.back().data.index >= v.index) { return {}; }
                ::std::uint64_t raw{};
                for(unsigned j{}; j < 8u; ++j) { raw |= static_cast<::std::uint64_t>(v.bits[j]) << (j * 8u); }
                slot next{}; next.data = v;
                switch(v.type)
                {
                    case 0x7f: if(raw > UINT32_MAX) { return {}; } next.storage.i32 = static_cast<::std::uint32_t>(raw); break;
                    case 0x7e: next.storage.i64 = raw; break;
                    case 0x7d:
                        if(raw > UINT32_MAX || (raw & 0x7f800000u) == 0x7f800000u) { return {}; }
                        next.storage.f32 = ::std::bit_cast<float>(static_cast<::std::uint32_t>(raw)); break;
                    case 0x7c:
                        if((raw & 0x7ff0000000000000ull) == 0x7ff0000000000000ull) { return {}; }
                        next.storage.f64 = ::std::bit_cast<double>(raw); break;
                    default: return {};
                }
                result->slots_.push_back(next);
            }
            return result;
        }
    };
    // Compiler-only TLS. Generated code contains no lookup and no TLS access.
    // A declaration emitted under this scope MUST receive an engine-local map
    // before materialization; it is never registered through AddSymbol.
    inline thread_local binding_owner const* current{};
    class compilation_scope final
    {
        binding_owner const* previous_{};
    public:
        explicit compilation_scope(binding_owner const* selected) noexcept : previous_{current} { current = selected; }
        compilation_scope(compilation_scope const&) = delete;
        compilation_scope& operator=(compilation_scope const&) = delete;
        ~compilation_scope() { current = previous_; }
    };
    [[nodiscard]] inline binding_owner::slot const* select(void const* module, ::std::uint64_t index) noexcept
    { return current && current->module() == module ? current->find(index) : nullptr; }
}
