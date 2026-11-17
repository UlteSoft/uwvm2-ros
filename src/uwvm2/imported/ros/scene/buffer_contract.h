/** @file
 * @brief Semantic buffer operations using the canonical proposed-record carrier.
 * @details Variable fields use the shared bounded scene wire codec. These
 * declarations do not change any existing WASI/ROS contract. A buffer reference
 * contains the separately admitted capability and the owner-checked resource.
 */
#pragma once
#include <array>
#include <cstdint>
#include <string_view>

#define UWVM_ROS_BUFFER_FUNCTIONS(X) \
 X(buffer_create,create,"buffer.create") \
 X(buffer_write,write,"buffer.write") \
 X(buffer_seal,seal,"buffer.seal") \
 X(buffer_read,read,"buffer.read") \
 X(buffer_describe,describe,"buffer.describe")

#ifndef UWVM_SCENE_BUFFER_CONTRACT_MACROS_ONLY
#include "buffers.h"
#include "wire.h"

namespace uwvm2::imported::ros::buffer_contract
{
    enum class operation : std::uint8_t {create,write,seal,read,describe};
    struct descriptor {operation id;std::string_view name;};
    inline constexpr std::array operations{
#define UWVM_BUFFER_DESCRIPTOR(symbol,action,name) descriptor{operation::action,name},
        UWVM_ROS_BUFFER_FUNCTIONS(UWVM_BUFFER_DESCRIPTOR)
#undef UWVM_BUFFER_DESCRIPTOR
    };
    inline constexpr std::uint64_t maximum_layout_wire=512;
    /** @brief Bounded detached decode fields; payload borrows the owned carrier copy. */
    struct request
    {
        scene::buffers::layout format;
        std::uint64_t buffer{},offset{},bytes{};
        scene::buffers::digest sha256{};
        std::span<std::uint8_t const> payload;
    };
    /** @brief Decode catalogue buffer_layout without allocation or partial output. */
    inline bool decode_layout(scene::wire::reader& input,scene::buffers::layout& output) noexcept
    {
        namespace buffers=scene::buffers;
        buffers::layout value;std::string_view name;std::span<std::uint8_t const> digest;std::uint64_t count{};
        if(!input.text(name)||!value.schema.identity.assign(name)||name.empty()||!input.blob(digest)||digest.size()!=32)return input.reject();
        std::copy(digest.begin(),digest.end(),value.schema.sha256.begin());
        if(!input.text(name))return false;
        auto element=std::find(buffers::element_names.begin(),buffers::element_names.end(),name);
        if(element==buffers::element_names.end())return input.reject();
        value.element=static_cast<buffers::element_type>(element-buffers::element_names.begin());
        if(!input.u64(count)||count>buffers::maximum_rank)return input.reject();
        value.rank=static_cast<std::size_t>(count);
        for(std::size_t i{};i!=value.rank;++i)if(!input.u64(value.shape[i]))return false;
        if(!input.u64(count)||count!=value.rank)return input.reject();
        for(std::size_t i{};i!=value.rank;++i)if(!input.u64(value.strides[i]))return false;
        if(!input.text(name))return false;
        if(name=="little")value.order=buffers::byte_order::little;
        else if(name=="big")value.order=buffers::byte_order::big;
        else return input.reject();
        bool present{};if(!input.boolean(present))return false;
        if(present)
        {
            if(!input.text(name))return false;
            auto color=std::find(buffers::color_names.begin()+1,buffers::color_names.end(),name);
            if(color==buffers::color_names.end())return input.reject();
            value.color=static_cast<buffers::color_space>(color-buffers::color_names.begin());
        }
        if(!input.u64(value.bytes))return false;
        output=value;return true;
    }
    /** @brief Encode/measure exactly the canonical ordered layout fields. */
    inline bool encode_layout(scene::wire::writer& output,scene::buffers::layout const& value) noexcept
    {
        namespace buffers=scene::buffers;
        auto element=static_cast<unsigned>(value.element),color=static_cast<unsigned>(value.color),order=static_cast<unsigned>(value.order);
        if(value.schema.identity.size>buffers::maximum_identity||value.rank>buffers::maximum_rank||element>=buffers::element_names.size()||
           color>=buffers::color_names.size()||order>1)return false;
        if(!output.text(value.schema.identity.view())||!output.blob(value.schema.sha256)||!output.text(buffers::element_names[element])||!output.u64(value.rank))return false;
        for(std::size_t i{};i!=value.rank;++i)if(!output.u64(value.shape[i]))return false;
        if(!output.u64(value.rank))return false;
        for(std::size_t i{};i!=value.rank;++i)if(!output.u64(value.strides[i]))return false;
        if(!output.text(order?"big":"little")||!output.boolean(color!=0)||(color&&!output.text(buffers::color_names[color])))return false;
        return output.u64(value.bytes);
    }
    /** @brief Reject malformed, trailing and excessive request bytes before any effect. */
    inline bool decode(operation action,std::span<std::uint8_t const> bytes,request& output) noexcept
    {
        scene::wire::reader input{bytes};request value;
        switch(action)
        {
        case operation::create:
            if(bytes.size()>maximum_layout_wire||!decode_layout(input,value.format))return false;
            break;
        case operation::write:
            if(!input.u64(value.buffer)||!input.u64(value.offset)||!input.blob(value.payload)||value.payload.size()>scene::buffers::maximum_transfer)return false;
            break;
        case operation::seal:
            if(!input.u64(value.buffer)||!input.blob(value.payload)||value.payload.size()!=32)return false;
            std::copy(value.payload.begin(),value.payload.end(),value.sha256.begin());break;
        case operation::read:
            if(!input.u64(value.buffer)||!input.u64(value.offset)||!input.u64(value.bytes)||value.bytes>scene::buffers::maximum_transfer)return false;
            break;
        case operation::describe:if(!input.u64(value.buffer))return false;break;
        default:return false;
        }
        if(!input.done())return false;
        output=value;return true;
    }
    /** @brief Encode only catalogue describe fields; digest/cursor stay host metadata. */
    inline bool encode_description(scene::wire::writer& output,scene::buffers::information const& value) noexcept
    {return encode_layout(output,value.value)&&output.boolean(value.sealed)&&output.text(value.provenance);}
    /** @brief Exact per-action input ceiling after independent policy lookup. */
    inline std::uint64_t request_bound(operation action,std::uint64_t transfer) noexcept
    {
        switch(action)
        {
        case operation::create:return maximum_layout_wire;
        case operation::write:return 24+std::min(transfer,scene::buffers::maximum_transfer);
        case operation::seal:return 48;
        case operation::read:return 24;
        case operation::describe:return 8;
        default:return 0;
        }
    }
}
#endif
