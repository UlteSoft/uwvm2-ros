/** @file
 * @brief Closed canonical records for bounded recording and private replay.
 * @details These descriptors do not register imports or grant authority. Buffer
 * references carry explicit capabilities, exact sizes and SHA256; providers must
 * resolve the actual immutable allocation and independently admit read/export.
 * Timestamps/sequence here are recording metadata claims, not a host clock or
 * native sensor provenance proof. A private checkpoint is volatile acceptance.
 * Replay yields bytes only; it never publishes to ROS or advances a clock.
 */
#pragma once
#include <array>
#define UWVM_ROS_RECORDING_FUNCTIONS(X) \
 X(recording_open,recording_open,"recording.open") \
 X(recording_append,recording_append,"recording.append") \
 X(recording_checkpoint,recording_checkpoint,"recording.checkpoint") \
 X(recording_stop,recording_stop,"recording.stop") \
 X(replay_open,replay_open,"replay.open") \
 X(replay_read,replay_read,"replay.read") \
 X(replay_seek,replay_seek,"replay.seek")
#ifndef UWVM_SCENE_RECORDING_CONTRACT_MACROS_ONLY
#include "recording_store.h"
namespace uwvm2::imported::ros::recording_contract
{
    namespace wire=scene::wire;
    namespace recording=scene::recording;
    enum class operation:std::uint8_t
    {recording_open,recording_append,recording_checkpoint,recording_stop,replay_open,replay_read,replay_seek};
    struct descriptor{operation id;std::string_view name;};
    inline constexpr std::array operations{
#define UWVM_RECORDING_DESCRIPTOR(symbol,action,name) descriptor{operation::action,name},
        UWVM_ROS_RECORDING_FUNCTIONS(UWVM_RECORDING_DESCRIPTOR)
#undef UWVM_RECORDING_DESCRIPTOR
    };
    /** @brief One primary claim and two independently checked claims per source fit one atomic group. */
    inline constexpr std::size_t maximum_items=63,maximum_objects=64,receipt_bytes=104,maximum_request=24+96*maximum_items;
    struct buffer_item
    {
        std::uint64_t capability{},buffer{},channel{},sequence{},log_time{},publish_time{},bytes{};
        scene::buffers::digest sha256{};
    };
    struct request
    {
        scene::buffers::profile profile;
        std::uint64_t resource{},key{},source_capability{},count{},position{};bool drain{};
        std::array<buffer_item,maximum_items> items{};
        std::array<recording::object_info,maximum_objects> objects{};
    };
    /** @brief Read an exact digest into owned bytes; malformed extents leave output unchanged. */
    inline bool digest(wire::reader& in,scene::buffers::digest& output) noexcept
    {
        wire::bytes bytes;if(!in.blob(bytes)||bytes.size()!=output.size())return in.reject();
        std::copy(bytes.begin(),bytes.end(),output.begin());return true;
    }
    inline bool profile(wire::reader& in,scene::buffers::profile& output) noexcept
    {
        scene::buffers::profile value;std::string_view name;
        if(!in.text(name)||name.empty()||!value.identity.assign(name)||!digest(in,value.sha256))return in.reject();
        output=value;return true;
    }
    /** @brief Decode one complete request; no native identifier or guest pointer is dereferenced. */
    inline bool decode(operation action,wire::bytes bytes,request& output) noexcept
    {
        if(bytes.size()>maximum_request)return false;
        wire::reader in{bytes};request value;
        switch(action)
        {
        case operation::recording_open:
            if(!profile(in,value.profile))return false;
            break;
        case operation::recording_append:
            if(!in.u64(value.resource)||!value.resource||!in.u64(value.key)||!value.key||!in.u64(value.count)||!value.count||value.count>maximum_items)return false;
            for(std::size_t i{};i<value.count;++i)
            {
                auto& item=value.items[i];
                if(!in.u64(item.capability)||!item.capability||!in.u64(item.buffer)||!item.buffer||!in.u64(item.channel)||!item.channel||item.channel>UINT16_MAX||
                   !in.u64(item.sequence)||item.sequence>UINT32_MAX||!in.u64(item.log_time)||!in.u64(item.publish_time)||!in.u64(item.bytes)||
                   item.bytes>scene::buffers::maximum_transfer||!digest(in,item.sha256))return false;
            }
            break;
        case operation::recording_checkpoint:case operation::recording_stop:
            if(!in.u64(value.resource)||!value.resource||!in.u64(value.key)||!value.key||(action==operation::recording_stop&&!in.boolean(value.drain)))return false;
            break;
        case operation::replay_open:
            if(!in.u64(value.source_capability)||!value.source_capability||!profile(in,value.profile)||!in.u64(value.count)||!value.count||value.count>maximum_objects)return false;
            for(std::size_t i{};i<value.count;++i)
            {
                auto& info=value.objects[i];
                if(!in.u64(info.id)||!info.id||!in.u64(info.bytes)||info.bytes>64u*1024u*1024u||!in.u64(info.messages)||info.messages>65536||!digest(in,info.sha256))return false;
                for(std::size_t j{};j<i;++j)if(value.objects[j].id==info.id)return false;
            }
            break;
        case operation::replay_read:
            if(!in.u64(value.resource)||!value.resource||!in.u64(value.count)||!value.count||value.count>recording::maximum_batch)return false;
            break;
        case operation::replay_seek:
            if(!in.u64(value.resource)||!value.resource||!in.u64(value.position))return false;
            break;
        default:return false;
        }
        if(!in.done())return false;
        output=value;return true;
    }
    /** @brief Fixed acceptance receipt; tags 1..4 mean append/checkpoint/drain/abort.
     * @details An all-zero object denotes no newly checkpointed segment. Its
     * 32-byte digest remains length-prefixed. The receipt never means durable
     * storage, remote delivery or permission to read another object.
     */
    inline bool encode(wire::writer& out,recording::receipt const& value) noexcept
    {
        auto tag=static_cast<unsigned>(value.operation);if(tag>3||!value.key)return false;
        return out.u64(value.key)&&out.u64(value.accepted_messages)&&out.u64(value.accepted_bytes)&&out.u64(tag+1)&&
            out.u64(value.object.id)&&out.u64(value.object.bytes)&&out.u64(value.object.messages)&&out.blob(value.object.sha256)&&out.boolean(value.stopped);
    }
    /** @brief Complete replay records in selected-object order; no partial payload is successful. */
    inline bool encode(wire::writer& out,std::uint64_t ordinal,std::span<recording::mcap::message const> records) noexcept
    {
        if(records.size()>recording::maximum_batch||!out.u64(ordinal)||!out.u64(records.size()))return false;
        for(auto const& record:records)
            if(!record.channel||!out.u64(record.channel)||!out.u64(record.sequence)||!out.u64(record.log_time)||!out.u64(record.publish_time)||!out.blob(record.data))return false;
        return true;
    }
}
#endif
