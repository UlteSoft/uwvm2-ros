// Private aggregation fragment after prepare_update_internal.
#pragma once
extern "C++" ::uwvm2::utils::update::status runtime_update_prepare_host_api(
    ::uwvm2::utils::update::permit const& p, ::std::span<unsigned char const> target,
    ::std::span<::std::uint64_t const> scopes, ::std::chrono::steady_clock::time_point deadline) noexcept
{ return prepare_update_internal(p, target, scopes, deadline); }
extern "C++" ::uwvm2::utils::update::status runtime_update_prepare_transition_host_api(
    ::uwvm2::utils::update::permit const& p, ::std::span<unsigned char const> target,
    ::std::span<unsigned char const> declaration, ::std::chrono::steady_clock::time_point deadline) noexcept
{
    namespace u = ::uwvm2::utils::update;
#if defined(UWVM_CPP_EXCEPTIONS)
    try
    {
        u::transition_manifest decoded{};
        if(!u::transition_manifest::decode(declaration, decoded)) { return u::status::invalid_input; }
        return prepare_update_internal(p, target, decoded.scopes, deadline, ::std::addressof(decoded), declaration);
    }
    catch(...) { return u::status::exhausted; }
#else
    (void)p; (void)target; (void)declaration; (void)deadline; return u::status::unsupported;
#endif
}
extern "C++" ::uwvm2::utils::update::status runtime_update_observe_transition_host_api(::uwvm2::utils::update::transition_facts& out) noexcept
{
    namespace u = ::uwvm2::utils::update;
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
    if(!production_update_detail::available()) { return u::status::invalid_state; }
    auto const& s{*production_update_detail::current}; auto const& cfg{s.protocol.current_configuration()};
    out = {s.pending_contract, cfg.policy_generation, cfg.revocation_generation, s.pending_policy_generation, s.pending_revocation_generation};
    return u::status::ok;
#else
    (void)out; return u::status::unsupported;
#endif
}
extern "C++" ::uwvm2::utils::update::status runtime_update_read_policy_host_api(::std::vector<unsigned char>& out) noexcept
{
    namespace u = ::uwvm2::utils::update;
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
    if(!production_update_detail::available()) { return u::status::invalid_state; }
#if !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1) && defined(UWVM_IMPORT_WASI_WASIP1) && defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
    try
    {
        out.clear(); auto const* binding{production_update_detail::active_policy_binding()};
        if(!binding) { return u::status::ok; }
        auto const policy{u::current_wasi_policy(binding->env)};
        return u::encode_wasi_policy(policy, out) ? u::status::ok : u::status::unsupported;
    }
    catch(...) { return u::status::exhausted; }
#else
    out.clear(); return u::status::ok;
#endif
#else
    (void)out; return u::status::unsupported;
#endif
}
