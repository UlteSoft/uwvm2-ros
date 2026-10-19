#pragma once
#include <uwvm2/imported/ros/ipc/custom_type.h>
#include <cstddef>
#include <string_view>

namespace uwvm2::ros_gateway::ros2_compat
{
    /** Data-only pairing between an explicitly selected native macro and the
     * immutable common Action registry. A match grants no authority and proves
     * neither native typesupport nor delivery. The configure probe separately
     * requires the complete registry validator and real typed lifecycle link.
     */
    struct action_catalog_row
    { std::string_view type,native_identity,codec_identity; };

    inline constexpr bool action_catalog_matches(
        ::uwvm2::imported::ros::ipc::custom_action_registry registry,
        action_catalog_row const* rows,std::size_t size) noexcept
    {
        if(!size||size>128||!rows||registry.size>128||(registry.size&&!registry.data)) {return false;}
        std::size_t selected{};
        for(std::size_t i{};i!=registry.size;++i) {if(registry.data[i].ros_version==2) {++selected;}}
        if(selected!=size) {return false;}
        for(std::size_t i{};i!=size;++i)
        {
            for(std::size_t j{};j!=i;++j) {if(rows[j].type==rows[i].type) {return false;}}
            std::size_t matches{};
            for(std::size_t j{};j!=registry.size;++j)
            {
                auto const& item=registry.data[j];
                if(item.ros_version==2&&item.type==rows[i].type)
                {
                    if(item.native_identity!=rows[i].native_identity||item.codec_identity!=rows[i].codec_identity) {return false;}
                    ++matches;
                }
            }
            if(matches!=1) {return false;}
        }
        return true;
    }

    template<class Codec> inline constexpr bool action_component_matches(
        ::uwvm2::imported::ros::ipc::custom_topic_descriptor const* item) noexcept
    {
        return item&&item->ros_version==2&&item->type==Codec::type&&
            item->native_identity==Codec::native_identity&&item->codec_identity==Codec::codec_identity&&
            item->wire_bytes==Codec::wire_bytes;
    }

    /** All three role descriptors must name exactly their selected codec.
     * C++ Native::Goal/Result/Feedback equality is checked by action_probe.h;
     * both checks are instantiated for every actual native macro row.
     */
    template<class GoalCodec,class ResultCodec,class FeedbackCodec>
    inline constexpr bool action_components_match(
        ::uwvm2::imported::ros::ipc::custom_action_descriptor const* item) noexcept
    {
        return item&&action_component_matches<GoalCodec>(item->goal)&&
            action_component_matches<ResultCodec>(item->result)&&action_component_matches<FeedbackCodec>(item->feedback);
    }
}
