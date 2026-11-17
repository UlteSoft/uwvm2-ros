// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <uwvm2/utils/container/standard.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace uwvm2::imported::ros::checkpoint::data
{
    // Detached stable-v1 DATA only. No native pointer, socket descriptor,
    // authenticated session credential, or permission-granting owner occurs
    // here. External peers/effects are deliberately outside this local census.
    struct field { ::uwvm2::utils::container::std_string name{}, value{}; friend bool operator==(field const& a,field const& b) noexcept{return a.name==b.name && a.value==b.value;} };
    struct member { ::std::uint64_t module{}; ::uwvm2::utils::container::std_string name{}; bool registered{}, retired{}; friend bool operator==(member const& a,member const& b) noexcept{return a.module==b.module && a.name==b.name && a.registered==b.registered && a.retired==b.retired;} };
    struct capability
    {
        ::uwvm2::utils::container::std_string alias{}, operation{}, resource{}, type{}, type_identity{};
        ::uwvm2::utils::container::std_vector<field> limits{};
        bool revoked{};
        // open_count, burst, bandwidth, tokens, creation_tokens, byte_tokens,
        // byte_fraction, log_bandwidth, log_tokens, log_fraction; limited flags.
        ::std::array<::std::uint64_t,13u> accounting{};
        ::std::array<::uwvm2::utils::container::std_string,3u> fractions{}; // rate, call and creation fraction (text, no native float ABI)
        // retained bytes, dropped, attempted, accepted, overwritten, rate_limited,
        // oversized, unconfigured, allocation_failed, sequence_exhausted,
        // drain_failed, capacity_exhausted, cancelled, clock_unavailable, saturated.
        ::std::array<::std::uint64_t,15u> audit{};
        ::uwvm2::utils::container::std_vector<::uwvm2::utils::container::std_string> audit_records{};
    };
    struct handle
    { ::std::uint32_t slot{}, generation{}; ::uwvm2::utils::container::std_string alias{}; bool active{}, retired{}; friend bool operator==(handle const& a,handle const& b) noexcept{return a.slot==b.slot && a.generation==b.generation && a.alias==b.alias && a.active==b.active && a.retired==b.retired;} };
    struct event
    {
        // 0 queued, 1 pending, 2 error, 3 closed, 4 reserved response.
        ::std::uint8_t location{};
        bool present{}; // An outstanding reservation need not have a response.
        ::std::uint64_t request{}, response_credit{}, owned_bytes{};
        ::std::uint16_t kind{};
        ::std::uint8_t flags{};
        ::std::uint32_t error{};
        ::std::uint64_t correlation{};
        ::uwvm2::utils::container::std_vector<::std::byte> payload{};
    };
    struct queue
    {
        // depth, bytes, record_bytes, pending_events, pending_bytes, overflow,
        // mask, sequence, queued_bytes, reserved_bytes, pinned_source, pinned_request.
        ::std::array<::std::uint64_t,12u> state{};
        bool configured{}, active{};
        // Exact cumulative statistics; current occupancy is in state/events.
        ::std::array<::std::uint64_t,17u> totals{};
        ::uwvm2::utils::container::std_vector<event> events{};
    };
    struct observation
    {
        ::std::uint64_t handle{}, source_handle{}, correlation{}, native_sequence{}, delivered{};
        ::uwvm2::utils::container::std_string observer_alias{}, source_alias{};
        bool retired{}, current_session{};
        ::uwvm2::utils::container::std_vector<::std::uint64_t> requests{};
        queue local{};
    };
    struct availability
    {
        ::std::uint64_t handle{}, source_handle{};
        ::uwvm2::utils::container::std_string availability_alias{}, source_alias{};
        bool retired{}, source_retired{}, availability_revoked{}, source_revoked{}, current_session{};
        friend bool operator==(availability const& a,availability const& b) noexcept
        {return a.handle==b.handle && a.source_handle==b.source_handle && a.availability_alias==b.availability_alias &&
            a.source_alias==b.source_alias && a.retired==b.retired && a.source_retired==b.source_retired &&
            a.availability_revoked==b.availability_revoked && a.source_revoked==b.source_revoked && a.current_session==b.current_session;}
    };
    struct group
    {
        ::std::uint8_t scope{};
        ::uwvm2::utils::container::std_string name{};
        bool enabled{}, registered{}, alive{}, session_present{}, connected{}, availability_sealed{};
        ::std::uint32_t timeout{};
        ::std::uint64_t next_service_request{}, session_label{}; // local diagnostic ordinal, never IPC credential
        ::uwvm2::utils::container::std_vector<member> members{};
        ::uwvm2::utils::container::std_vector<field> connection{}, interfaces{};
        ::uwvm2::utils::container::std_vector<capability> capabilities{};
        ::uwvm2::utils::container::std_vector<handle> handles{};
        ::uwvm2::utils::container::std_vector<observation> observations{};
        ::uwvm2::utils::container::std_vector<availability> availability_history{};
    };
    struct environment
    {
        ::std::array<::std::byte,16u> recording{};
        ::std::uint8_t backend{};
        ::uwvm2::utils::container::std_vector<group> groups{};
        static constexpr bool grants_restore_authority() noexcept { return false; }
    };
}
