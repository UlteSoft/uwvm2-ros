/** @file
 * @brief Closed log emission and cached diagnostic page carriers.
 * @details Log records use logs::encode/decode and return six LE u64 words:
 * scope, session, sequence, record, accepted_bytes, lost_before. Diagnostic
 * requests are scope, source capability, optional cursor, limit; results are
 * revision, optional next cursor, count and {code,value,available,saturated}.
 * All flags are canonical u64 booleans. Scope/cursor numbers never grant rights.
 */
#pragma once
#include <array>
#define UWVM_ROS_LOG_DIAGNOSTIC_FUNCTIONS(X) \
 X(log_emit,emit,"log.emit") \
 X(diagnostic_read,read,"diagnostic.read")
#ifndef UWVM_SCENE_LOG_DIAGNOSTIC_CONTRACT_MACROS_ONLY
#include "diagnostic_snapshot.h"
namespace uwvm2::imported::ros::log_diagnostic_contract
{
    enum class operation:std::uint8_t{emit,read};
    struct descriptor{operation id;std::string_view name;};
    inline constexpr std::array operations{descriptor{operation::emit,"log.emit"},descriptor{operation::read,"diagnostic.read"}};
    inline constexpr std::size_t receipt_bytes=48,maximum_page_bytes=2080,maximum_read_request=40;
    struct read_request{std::uint64_t scope{},source_capability{},limit{};std::optional<std::uint64_t> cursor;};
    inline bool decode(scene::wire::bytes input,read_request& output) noexcept
    {
        scene::wire::reader reader{input};read_request next;bool present{};std::uint64_t cursor{};
        if(!reader.u64(next.scope)||!next.scope||!reader.u64(next.source_capability)||!next.source_capability||!reader.boolean(present)||
           (present&&(!reader.u64(cursor)||!cursor))||!reader.u64(next.limit)||!next.limit||next.limit>64||!reader.done())return false;
        if(present)next.cursor=cursor;
        output=next;return true;
    }
    inline bool encode(scene::wire::writer& writer,scene::diagnostics::page const& value) noexcept
    {
        if(!value.revision||value.items.size()>64||!writer.u64(value.revision)||!writer.boolean(value.cursor.has_value())||
           (value.cursor&&!writer.u64(*value.cursor))||!writer.u64(value.items.size()))return false;
        for(auto const& row:value.items)
            if(!writer.u64(row.code)||!writer.u64(row.value)||!writer.boolean(row.available)||!writer.boolean(row.saturated))return false;
        return true;
    }
}
#endif
