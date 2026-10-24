/**
 * @file llvm_jit_cache_refusal_diagnostics.cc
 * @brief Distinguish explicit disablement from actual identity/admission refusal.
 * @details Exercise production load, synchronous store and asynchronous store.
 * None of the refused operations may create cache files or return object bytes.
 */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/runtime/llvm_jit_cache/environment.h>
#include <uwvm2/runtime/llvm_jit_cache/store.h>
#include <filesystem>
#if defined(__linux__)
# include <sys/prctl.h>
#endif

int main(int argc, char** argv)
{
#if !defined(__linux__)
    return 77;
#else
    if(argc != 2 || std::filesystem::exists(argv[1])) { return 2; }
    if(::prctl(PR_SET_DUMPABLE, 0ul, 0ul, 0ul, 0ul) != 0 || ::prctl(PR_GET_DUMPABLE, 0ul, 0ul, 0ul, 0ul) != 0) { return 3; }
    namespace cache = uwvm2::runtime::llvm_jit_cache;
    cache::cache_context context{};
    context.cache_dir = cache::details::u8string_from_cstr(argv[1]);
    cache::cache_policy policy{};
    constexpr std::byte object[]{std::byte{0x14}, std::byte{0x82}};
    auto refused{[&](cache::cache_status expected)
                 {
                     auto loaded{cache::load_object(context, policy)};
                     return loaded.status == expected && loaded.object.empty() && !loaded.signature_verified &&
                            cache::store_object(context, object, sizeof(object), policy) == expected &&
                            cache::store_object_async(context, object, sizeof(object), policy) == expected && !std::filesystem::exists(argv[1]);
                 }};
    if(!refused(cache::cache_status::executable_identity_unavailable)) { return 4; }
    if(!cache::bind_self_executable_identity(context)) { return 5; }
    context.self_executable_sha256.index_unchecked(0) ^= std::byte{1};
    if(!refused(cache::cache_status::executable_identity_mismatch)) { return 6; }
    policy.enable = false;
    if(!refused(cache::cache_status::disabled)) { return 7; }

    struct refusal_case
    {
        cache::cache_disable_reason reason;
        cache::cache_status status;
    };

    constexpr refusal_case causes[]{
        {cache::cache_disable_reason::process_not_sealed,              cache::cache_status::process_not_sealed             },
        {cache::cache_disable_reason::executable_identity_unavailable, cache::cache_status::executable_identity_unavailable},
        {cache::cache_disable_reason::signature_unavailable,           cache::cache_status::unsupported_signature          },
        {cache::cache_disable_reason::generation_bound_plan,           cache::cache_status::generation_bound_plan          },
        {cache::cache_disable_reason::debugger_active,                 cache::cache_status::debugger_active                },
        {cache::cache_disable_reason::experimental_gc_exceptions,      cache::cache_status::experimental_gc_exceptions     },
        {cache::cache_disable_reason::native_eh_private_binding,       cache::cache_status::native_eh_private_binding      }
    };
    for(auto const& cause: causes)
    {
        policy.disabled_reason = cause.reason;
        if(!refused(cause.status)) { return 10; }
    }
    policy.disabled_reason = cache::cache_disable_reason::requested;
    policy.enable = true;
    if(!cache::bind_self_executable_identity(context)) { return 8; }
    if(!refused(cache::cache_status::origin_admission_unavailable)) { return 9; }
    return 0;
#endif
}
