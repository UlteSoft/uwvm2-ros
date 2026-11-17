// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <uwvm2/utils/container/unique.h>
#include <uwvm2/utils/container/standard.h>
#include <atomic>
#include <cstdint>
#include <memory>
#include <span>
#include <string_view>
#include "checkpoint_environment.h"

namespace uwvm2::imported::ros::checkpoint
{
    // Native, process-local identities supplied by the closed runtime cohort.
    // These are NEVER decoded from an archive or exposed as a Wasm import.
    struct member
    {
        void const* previous{};
        void const* replacement{};
        ::std::uint64_t previous_epoch{}, replacement_epoch{};
        ::std::string_view module{};
        bool authority_retired{};
        ::std::uint64_t module_id{UINT64_MAX}; // source-bound actual dense ID, never archive authority
    };
    enum class status : unsigned char
    { ready, malformed, stale, unsupported, allocation_failed };

    // Preparation must not publish registrations, reopen resources, reset
    // quotas or emit an RPC. Publication is called only after physical worker
    // retirement with host admission still closed. A discarded candidate has
    // no effect on the original environment.
    class handoff
    {
    public:
        virtual ~handoff() = default;
        [[nodiscard]] virtual bool current() noexcept = 0;
        [[nodiscard]] virtual bool publish() noexcept = 0;
    };
    using owner = ::uwvm2::utils::container::std_unique_ptr<handoff>;
    using prepare_function = status(*)(::std::span<member const>, owner&) noexcept;
    inline ::std::atomic<prepare_function> prepare_hook{};
    struct source_member
    { void const* instance{}; ::std::uint64_t epoch{}, module{}; ::std::string_view name{}; bool retired{}; };
    using environment_owner = ::std::shared_ptr<data::environment const>;
    using capture_function = status(*)(::std::span<source_member const>,
        ::std::array<::std::byte,16u> const&, unsigned, ::uwvm2::utils::container::std_vector<environment_owner>&) noexcept;
    inline ::std::atomic<capture_function> capture_hook{};
    using prepare_recorded_function = status(*)(::std::span<member const>,
        ::std::span<environment_owner const>, owner&) noexcept;
    inline ::std::atomic<prepare_recorded_function> prepare_recorded_hook{};
}
