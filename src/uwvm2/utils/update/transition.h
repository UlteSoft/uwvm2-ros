/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)           *
 * Licensed under the APL-2.0 License (see LICENSE file).      *
 *************************************************************/
#pragma once
#include "protocol.h"
namespace uwvm2::utils::update
{
    inline constexpr ::std::size_t maximum_policy_bytes = 65536u;
    inline constexpr ::std::size_t maximum_migration_bytes = 3600u;
    inline constexpr ::std::size_t maximum_manifest_bytes = 72u + maximum_functions * 8u + maximum_policy_bytes + maximum_migration_bytes;
    // Complete signed change declaration. The permit's functions field hashes
    // these exact bytes; a v1 list cannot be reinterpreted as a policy grant.
    struct transition_manifest
    {
        bool changes_policy{}, module_reset{}, module_preserve{}, module_migrate{};
        digest approved_contract_reference{};
        ::std::uint64_t target_policy_generation{}, target_revocation_generation{};
        ::std::vector<::std::uint64_t> scopes{};
        ::std::vector<unsigned char> policy{}, migration{};
        static bool decode(::std::span<unsigned char const> bytes, transition_manifest& out)
        {
            if(bytes.size() < 64u || bytes.size() > maximum_manifest_bytes) { return false; }
            bool const version3{::std::memcmp(bytes.data(), "UUT3", 4u) == 0};
            if(!version3 && ::std::memcmp(bytes.data(), "UUT2", 4u) != 0) { return false; }
            ::std::size_t const header{version3 ? 72u : 64u};
            if(bytes.size() < header) { return false; }
            auto read32{[&](::std::size_t pos) noexcept { ::std::uint32_t value{}; for(unsigned i{}; i<4u; ++i) { value |= static_cast<::std::uint32_t>(bytes[pos+i])<<(i*8u); } return value; }};
            auto read64{[&](::std::size_t pos) noexcept { ::std::uint64_t value{}; for(unsigned i{}; i<8u; ++i) { value |= static_cast<::std::uint64_t>(bytes[pos+i])<<(i*8u); } return value; }};
            auto const flags{read32(4)}, count{read32(8)}, length{read32(12)};
            auto const migration_length{version3 ? read32(64u) : 0u};
            if((flags & ~(version3 ? 15u : 7u)) != 0u || (flags & 6u) == 6u ||
               (version3 && ((flags & 14u) != 8u || read32(68u) != 0u || migration_length == 0u)) ||
               count > maximum_functions || length > maximum_policy_bytes || migration_length > maximum_migration_bytes ||
               bytes.size() != header + count*8u + length + migration_length) { return false; }
            transition_manifest candidate{};
            candidate.changes_policy = (flags & 1u) != 0u;
            candidate.module_reset = (flags & 2u) != 0u; candidate.module_preserve = (flags & 4u) != 0u; candidate.module_migrate = (flags & 8u) != 0u;
            if((candidate.module_reset || candidate.module_preserve || candidate.module_migrate) && count == 0u) { return false; }
            ::std::copy_n(bytes.data()+16u, 32u, candidate.approved_contract_reference.data());
            candidate.target_policy_generation = read64(48u); candidate.target_revocation_generation = read64(56u);
            if(candidate.changes_policy)
            {
                if(length == 0u || zero(candidate.approved_contract_reference) || candidate.target_policy_generation == 0u) { return false; }
            }
            else if(length != 0u || !zero(candidate.approved_contract_reference) || candidate.target_policy_generation != 0u || candidate.target_revocation_generation != 0u || count == 0u) { return false; }
            candidate.scopes.reserve(count);
            for(::std::uint32_t i{}; i<count; ++i)
            {
                auto const index{read64(header+i*8u)};
                if(!candidate.scopes.empty() && candidate.scopes.back() >= index) { return false; }
                candidate.scopes.push_back(index);
            }
            candidate.policy.assign(bytes.begin()+header+count*8u, bytes.begin()+header+count*8u+length);
            candidate.migration.assign(bytes.begin()+header+count*8u+length, bytes.end());
            out = ::std::move(candidate); return true;
        }
    };
    struct transition_facts
    {
        digest pending_contract{};
        ::std::uint64_t policy_generation{}, revocation_generation{}, pending_policy_generation{}, pending_revocation_generation{};
    };
}
