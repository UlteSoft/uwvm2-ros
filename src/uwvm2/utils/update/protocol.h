/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)           *
 * Licensed under the APL-2.0 License (see LICENSE file).      *
 *************************************************************/
#pragma once
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <string_view>
#include <vector>
#include <uwvm2/utils/security/wsea.h>

namespace uwvm2::utils::update
{
    using digest = ::std::array<unsigned char, 32>;
    inline constexpr ::std::size_t maximum_image_bytes = 64u * 1024u * 1024u;
    inline constexpr ::std::size_t maximum_functions = 64u;
    inline constexpr ::std::size_t maximum_transactions = 32u;
    enum class phase : unsigned char { disabled, active, preparing, prepared, waiting_for_decision, committed, committed_failed, aborted, awaiting_launch, guest_fault, process_exit_pending };
    enum class action : unsigned char { prepare = 1, commit = 2, abort = 3, launch = 4 };
    enum class status : unsigned char
    {
        ok, unsupported, invalid_state, invalid_input, unauthorized, expired, stale_base,
        scope_mismatch, busy, exhausted, validation_failed, compile_failed, committed_but_not_active
    };
    inline char const* name(status s) noexcept
    {
        switch(s)
        {
#define UWVM_UPDATE_STATUS(x) case status::x: return #x
            UWVM_UPDATE_STATUS(ok); UWVM_UPDATE_STATUS(unsupported); UWVM_UPDATE_STATUS(invalid_state);
            UWVM_UPDATE_STATUS(invalid_input); UWVM_UPDATE_STATUS(unauthorized); UWVM_UPDATE_STATUS(expired);
            UWVM_UPDATE_STATUS(stale_base); UWVM_UPDATE_STATUS(scope_mismatch); UWVM_UPDATE_STATUS(busy);
            UWVM_UPDATE_STATUS(exhausted); UWVM_UPDATE_STATUS(validation_failed); UWVM_UPDATE_STATUS(compile_failed);
            UWVM_UPDATE_STATUS(committed_but_not_active);
#undef UWVM_UPDATE_STATUS
        }
        return "invalid_state";
    }
    inline bool hash(::std::span<unsigned char const> bytes, digest& result) noexcept
    {
#if defined(UWVM_WSEA_SUPPORTED)
        return ::uwvm2::utils::security::wsea::detail::hash(bytes.data(), bytes.size(), result);
#else
        (void)bytes; (void)result; return false;
#endif
    }
    inline bool random_nonce(digest& result) noexcept
    {
#if defined(UWVM_WSEA_SUPPORTED)
        return RAND_bytes(result.data(), static_cast<int>(result.size())) == 1;
#else
        (void)result; return false;
#endif
    }
    inline void append_u64(::std::vector<unsigned char>& bytes, ::std::uint64_t n)
    { for(unsigned i{}; i != 8; ++i) { bytes.push_back(static_cast<unsigned char>(n >> (i * 8))); } }
    inline bool zero(digest const& d) noexcept { return d == digest{}; }
    // Fixed-width wire format: no optional/duplicate fields or nested object parser.
    // All integers are LE64. The domain is signed verbatim, including its trailing NUL.
    struct permit
    {
        action operation{};
        digest issuer{}, nonce{}, transaction{}, base{}, target{}, contract{}, prepared{}, ready{}, scope{}, functions{};
        ::std::uint64_t base_epoch{}, target_epoch{}, policy_generation{}, revocation_generation{}, expires_unix_seconds{}, decision_sequence{};
        ::std::array<unsigned char, 64> signature{};
        static constexpr ::std::string_view domain{"uwvm.local-update.activation.v1\0", 32};
        static constexpr ::std::size_t unsigned_size = domain.size() + 8 + 10 * 32 + 6 * 8;
        static constexpr ::std::size_t wire_size = unsigned_size + 64;
        [[nodiscard]] ::std::array<unsigned char, unsigned_size> unsigned_encoding() const noexcept
        {
            ::std::array<unsigned char, unsigned_size> out{}; ::std::size_t pos{};
            for(char c : domain) { out[pos++] = static_cast<unsigned char>(c); }
            out[pos] = static_cast<unsigned char>(operation); pos += 8;
            for(auto const* d : {&issuer, &nonce, &transaction, &base, &target, &contract, &prepared, &ready, &scope, &functions})
            { for(auto byte : *d) { out[pos++] = byte; } }
            for(auto n : {base_epoch, target_epoch, policy_generation, revocation_generation, expires_unix_seconds, decision_sequence})
            { for(unsigned i{}; i < 8; ++i) { out[pos++] = static_cast<unsigned char>(n >> (i*8)); } }
            return out;
        }
        [[nodiscard]] ::std::vector<unsigned char> encoding(bool with_signature = true) const
        {
            auto const fixed{unsigned_encoding()};
            ::std::vector<unsigned char> out; out.reserve(wire_size); out.insert(out.end(), fixed.begin(), fixed.end());
            if(with_signature) { out.insert(out.end(), signature.begin(), signature.end()); }
            return out;
        }
        static bool decode(::std::span<unsigned char const> in, permit& result) noexcept
        {
            if(in.size() != wire_size || ::std::memcmp(in.data(), domain.data(), domain.size()) != 0) { return false; }
            ::std::size_t pos{domain.size()}; auto operation{in[pos++]};
            if(operation < 1 || operation > 4) { return false; }
            for(unsigned i{}; i != 7; ++i) { if(in[pos++] != 0) { return false; } }
            permit p{}; p.operation = static_cast<action>(operation);
            for(auto* d : {&p.issuer, &p.nonce, &p.transaction, &p.base, &p.target, &p.contract, &p.prepared, &p.ready, &p.scope, &p.functions})
            { ::std::copy_n(in.data() + pos, 32, d->data()); pos += 32; }
            for(auto* n : {&p.base_epoch, &p.target_epoch, &p.policy_generation, &p.revocation_generation, &p.expires_unix_seconds, &p.decision_sequence})
            { for(unsigned i{}; i != 8; ++i) { *n |= static_cast<::std::uint64_t>(in[pos++]) << (i * 8); } }
            ::std::copy_n(in.data() + pos, 64, p.signature.data()); result = p; return true;
        }
    };
    struct configuration
    {
        digest public_key{}, contract{}, scope{};
        ::std::uint64_t epoch{1}, policy_generation{1}, revocation_generation{};
    };
    struct facts
    {
        phase state{phase::disabled}; digest nonce{}, installed{}, prepared{}, ready{}, transaction{}, contract{}, scope{}, issuer{};
        ::std::uint64_t installed_epoch{}, authenticated_commit_epoch{}, decision_sequence{};
        bool active{}, coordinator_connected{};
    };
    inline bool verify(permit const& p, configuration const& cfg) noexcept
    {
#if defined(UWVM_WSEA_SUPPORTED)
        namespace crypto = ::uwvm2::utils::security::wsea::detail;
        if(!crypto::admitted_ed25519_point(cfg.public_key.data()) ||
           !crypto::admitted_ed25519_signature(p.signature.data(), p.signature.size())) { return false; }
        crypto::owned<EVP_PKEY, EVP_PKEY_free> key{EVP_PKEY_new_raw_public_key(EVP_PKEY_ED25519, nullptr, cfg.public_key.data(), cfg.public_key.size())};
        crypto::owned<EVP_MD_CTX, EVP_MD_CTX_free> ctx{EVP_MD_CTX_new()};
        if(!key || !ctx || EVP_DigestVerifyInit(ctx.get(), nullptr, nullptr, nullptr, key.get()) != 1) { return false; }
        auto const bytes{p.unsigned_encoding()};
        return EVP_DigestVerify(ctx.get(), p.signature.data(), p.signature.size(), bytes.data(), bytes.size()) == 1;
#else
        (void)p; (void)cfg; return false;
#endif
    }
    // This state machine owns authentication facts only. Runtime readiness must
    // additionally retain its real execution fence and immutable compiler owners.
    class protocol
    {
        configuration config_{}, candidate_config_{}, launch_config_{}; facts facts_{}; permit candidate_{}, launch_{};
        bool launch_authorized_{};
        ::std::vector<digest> transactions_{};
        bool candidate_contract_installed_{};
        static ::std::uint64_t now() noexcept
        { return static_cast<::std::uint64_t>(::std::chrono::duration_cast<::std::chrono::seconds>(::std::chrono::system_clock::now().time_since_epoch()).count()); }
        status authenticate(permit const& p, configuration const& authority) const noexcept
        {
            if(p.issuer != facts_.issuer || p.nonce != facts_.nonce || p.scope != authority.scope ||
               p.contract != authority.contract || p.policy_generation != authority.policy_generation ||
               p.revocation_generation != authority.revocation_generation || zero(p.transaction) || !verify(p, authority)) { return status::unauthorized; }
            auto const time{now()};
            if(p.expires_unix_seconds < time || p.expires_unix_seconds - time > 3600) { return status::expired; }
            return status::ok;
        }
        bool same_candidate(permit const& p) const noexcept
        {
            return p.transaction == candidate_.transaction && p.base == candidate_.base && p.target == candidate_.target &&
                p.base_epoch == candidate_.base_epoch && p.target_epoch == candidate_.target_epoch &&
                p.functions == candidate_.functions && p.prepared == facts_.prepared && p.ready == facts_.ready;
        }
    public:
        [[nodiscard]] facts const& observe() const noexcept { return facts_; }
        [[nodiscard]] permit const& candidate() const noexcept { return candidate_; }
        [[nodiscard]] configuration const& current_configuration() const noexcept { return config_; }
        [[nodiscard]] status configure(configuration const& cfg, digest const& actual_base)
        {
            if(facts_.state != phase::disabled || cfg.policy_generation == 0 || zero(cfg.public_key) || zero(cfg.contract) || zero(cfg.scope) || zero(actual_base)) { return status::invalid_input; }
            if(!hash(cfg.public_key, facts_.issuer) || !random_nonce(facts_.nonce)) { return status::unsupported; }
            transactions_.reserve(maximum_transactions); config_ = cfg;
            facts_.installed = actual_base; facts_.installed_epoch = 0; facts_.contract = cfg.contract; facts_.scope = cfg.scope;
            facts_.state = phase::awaiting_launch; facts_.active = false; facts_.coordinator_connected = true; return status::ok;
        }
        // A fresh process cannot infer the supervisor's durable decision from its
        // local image. This nonce-bound assertion establishes that recovery floor.
        // Authentication and real Runtime admission publication are separate.
        [[nodiscard]] status authorize_launch(permit const& p) noexcept
        {
            if(launch_authorized_)
            {
                if(p.unsigned_encoding() != launch_.unsigned_encoding()) { return status::unauthorized; }
                return authenticate(p, launch_config_); // Receipt only, never reopen.
            }
            if(facts_.state != phase::awaiting_launch || !facts_.coordinator_connected) { return status::invalid_state; }
            if(p.operation != action::launch || p.base != facts_.installed || p.target != facts_.installed ||
               p.base_epoch != 0 || p.target_epoch == 0 || p.decision_sequence == 0 ||
               !zero(p.prepared) || !zero(p.ready) || !zero(p.functions) ||
               p.policy_generation < config_.policy_generation || p.revocation_generation < config_.revocation_generation)
            { return status::invalid_input; }
            auto authority{config_};
            authority.policy_generation = p.policy_generation; authority.revocation_generation = p.revocation_generation;
            if(auto s{authenticate(p, authority)}; s != status::ok) { return s; }
            launch_ = p; launch_config_ = authority; config_ = authority; launch_authorized_ = true;
            facts_.installed_epoch = p.target_epoch; facts_.authenticated_commit_epoch = p.target_epoch;
            facts_.decision_sequence = p.decision_sequence; facts_.transaction = p.transaction;
            return status::ok;
        }
        [[nodiscard]] bool launch_authorized() const noexcept { return launch_authorized_; }
        [[nodiscard]] bool launch_activated() noexcept
        {
            if(!launch_authorized_ || facts_.state != phase::awaiting_launch || !facts_.coordinator_connected) { return false; }
            facts_.state = phase::active; facts_.active = true; return true;
        }
        [[nodiscard]] status begin(permit const& p)
        {
            if(auto s{authenticate(p, config_)}; s != status::ok) { return s; }
            if(facts_.state != phase::active && facts_.state != phase::aborted) { return status::busy; }
            if(p.operation != action::prepare || !zero(p.prepared) || !zero(p.ready) || p.decision_sequence != 0 || zero(p.target)) { return status::invalid_input; }
            if(p.base != facts_.installed || p.base_epoch != facts_.installed_epoch || p.target_epoch <= facts_.installed_epoch ||
               p.target_epoch <= facts_.authenticated_commit_epoch) { return status::stale_base; }
            if(transactions_.size() >= maximum_transactions) { return status::exhausted; }
            if(p.transaction == launch_.transaction || ::std::find(transactions_.begin(), transactions_.end(), p.transaction) != transactions_.end()) { return status::unauthorized; }
            transactions_.push_back(p.transaction); candidate_config_ = config_; candidate_contract_installed_ = false; candidate_ = p; facts_.transaction = p.transaction;
            facts_.prepared = {}; facts_.ready = {}; facts_.state = phase::preparing; return status::ok;
        }
        [[nodiscard]] bool prepared(digest const& measurement) noexcept
        { if(facts_.state != phase::preparing || zero(measurement)) { return false; } facts_.prepared = measurement; facts_.state = phase::prepared; return true; }
        [[nodiscard]] bool ready(digest const& measurement) noexcept
        {
            if(facts_.state != phase::prepared || zero(measurement)) { return false; }
            facts_.ready = measurement; facts_.state = phase::waiting_for_decision; facts_.active = false; return true;
        }
        [[nodiscard]] bool cancel_private() noexcept
        {
            if(facts_.state != phase::preparing && facts_.state != phase::prepared) { return false; }
            facts_.state = phase::aborted; facts_.active = true; return true;
        }
        [[nodiscard]] status decide(permit const& p) noexcept
        {
            // Decisions and their retries bind the base authority captured at PREPARE,
            // even after the approved target contract has been installed.
            if(auto s{authenticate(p, candidate_config_)}; s != status::ok) { return s; }
            if(!same_candidate(p) || p.decision_sequence == 0) { return status::unauthorized; }
            if((facts_.state == phase::active || facts_.state == phase::committed || facts_.state == phase::committed_failed) &&
               facts_.authenticated_commit_epoch == p.target_epoch && facts_.decision_sequence == p.decision_sequence)
            { return p.operation == action::commit ? status::ok : status::invalid_state; }
            if(facts_.state == phase::aborted && p.operation == action::abort && facts_.decision_sequence == p.decision_sequence) { return status::ok; }
            if(facts_.state != phase::waiting_for_decision || p.decision_sequence <= facts_.decision_sequence) { return status::invalid_state; }
            if(p.operation != action::commit && p.operation != action::abort) { return status::invalid_input; }
            facts_.decision_sequence = p.decision_sequence;
            if(p.operation == action::abort) { facts_.state = phase::aborted; facts_.active = true; return status::ok; }
            facts_.authenticated_commit_epoch = p.target_epoch; facts_.state = phase::committed; facts_.active = false; return status::ok;
        }
        [[nodiscard]] bool install_contract(digest const& contract, ::std::uint64_t generation, ::std::uint64_t revocation) noexcept
        {
            if(facts_.state != phase::committed || candidate_contract_installed_ || zero(contract) || config_.policy_generation == UINT64_MAX ||
               generation != config_.policy_generation + 1u || revocation < config_.revocation_generation) { return false; }
            config_.contract = contract; config_.policy_generation = generation; config_.revocation_generation = revocation;
            facts_.contract = contract; candidate_contract_installed_ = true; return true;
        }
        void installed() noexcept
        {
            if(facts_.state != phase::committed) { return; }
            facts_.installed = candidate_.target; facts_.installed_epoch = candidate_.target_epoch;
        }
        void activated() noexcept
        {
            if(facts_.state != phase::committed) { return; }
            facts_.installed = candidate_.target; facts_.installed_epoch = candidate_.target_epoch;
            facts_.state = phase::active; facts_.active = true;
        }
        void activation_failed() noexcept
        { if(facts_.state == phase::committed) { facts_.state = phase::committed_failed; facts_.active = false; } }
        void suspend_execution() noexcept { facts_.active = false; }
        void guest_faulted() noexcept { facts_.active = false; facts_.state = phase::guest_fault; }
        void process_exit_requested() noexcept { facts_.active = false; facts_.state = phase::process_exit_pending; }
        void disconnected() noexcept { facts_.coordinator_connected = false; }
    };
    struct function_body { ::std::size_t offset{}, size{}; };
    struct section { unsigned char id{}; ::std::size_t offset{}, size{}; };
    inline bool u32(::std::span<unsigned char const> in, ::std::size_t& pos, ::std::uint32_t& value) noexcept
    {
        value = 0;
        for(unsigned shift{}; shift < 35; shift += 7)
        {
            if(pos >= in.size()) { return false; } auto b{in[pos++]};
            if(shift == 28 && (b & 0xf0) != 0) { return false; }
            value |= static_cast<::std::uint32_t>(b & 127) << shift;
            if((b & 128) == 0) { return true; }
        }
        return false;
    }
    // Structural scope check, not a replacement for the actual Wasm validator.
    // Non-code bytes, including custom sections, must be identical. Changed bodies
    // still pass the existing feature-aware parser/type validator/compiler.
    inline bool split(::std::span<unsigned char const> bytes, ::std::vector<section>& sections, ::std::vector<function_body>& bodies)
    {
        constexpr unsigned char magic[]{0,97,115,109,1,0,0,0};
        if(bytes.size() < 8 || bytes.size() > maximum_image_bytes || ::std::memcmp(bytes.data(), magic, 8) != 0) { return false; }
        ::std::size_t pos{8}; bool code{};
        while(pos < bytes.size())
        {
            if(sections.size() >= 4096) { return false; }
            auto const start{pos}; auto const id{bytes[pos++]}; ::std::uint32_t size{};
            if(id > 13 || !u32(bytes, pos, size) || size > bytes.size() - pos) { return false; }
            auto const end{pos + size}; sections.push_back({id, start, end - start});
            if(id == 10)
            {
                if(code) { return false; } code = true; ::std::uint32_t count{};
                if(!u32(bytes.first(end), pos, count) || count > 1000000) { return false; }
                for(::std::uint32_t i{}; i != count; ++i)
                {
                    ::std::uint32_t length{};
                    if(!u32(bytes.first(end), pos, length) || length < 2 || length > end - pos) { return false; }
                    bodies.push_back({pos, length}); pos += length;
                }
                if(pos != end) { return false; }
            }
            pos = end;
        }
        return code;
    }
    inline status function_scope(::std::span<unsigned char const> base, ::std::span<unsigned char const> target,
        ::std::span<::std::uint64_t const> allowed_local_indices, ::std::vector<function_body>& new_bodies)
    {
        if(allowed_local_indices.empty() || allowed_local_indices.size() > maximum_functions) { return status::invalid_input; }
        for(::std::size_t i{1}; i < allowed_local_indices.size(); ++i)
        { if(allowed_local_indices[i] <= allowed_local_indices[i-1]) { return status::invalid_input; } }
        ::std::vector<section> a, b; ::std::vector<function_body> old_bodies;
        if(!split(base, a, old_bodies) || !split(target, b, new_bodies) || a.size() != b.size() || old_bodies.size() != new_bodies.size()) { return status::validation_failed; }
        if(allowed_local_indices.back() >= new_bodies.size()) { return status::invalid_input; }
        for(::std::size_t i{}; i < a.size(); ++i)
        {
            if(a[i].id != b[i].id) { return status::scope_mismatch; }
            if(a[i].id != 10 && (a[i].size != b[i].size || ::std::memcmp(base.data()+a[i].offset, target.data()+b[i].offset, a[i].size) != 0)) { return status::scope_mismatch; }
        }
        bool changed{};
        for(::std::size_t i{}; i < new_bodies.size(); ++i)
        {
            bool const differs{old_bodies[i].size != new_bodies[i].size || ::std::memcmp(base.data()+old_bodies[i].offset, target.data()+new_bodies[i].offset, old_bodies[i].size) != 0};
            if(differs && !::std::binary_search(allowed_local_indices.begin(), allowed_local_indices.end(), i)) { return status::scope_mismatch; }
            changed = changed || differs;
        }
        return changed ? status::ok : status::invalid_input;
    }
}
