/** @file
 * @brief Canonical transform records with bounded frame names and publication batches.
 * @details Decoding grants no frame or clock authority. The broker independently
 * admits the selected clock capability and every configured source/frame rule.
 * An absent time means latest; an exact zero or negative timestamp remains an
 * explicit instant. Deadlines are positive absolute host-steady nanoseconds,
 * independent of the transform's clock domain and epoch.
 */
#pragma once
#include <array>
#define UWVM_ROS_TRANSFORM_FUNCTIONS(X) \
 X(transform_open,open,"transform.open") \
 X(transform_publish,publish,"transform.publish") \
 X(transform_lookup,lookup,"transform.lookup") \
 X(transform_available,available,"transform.available") \
 X(transform_velocity,velocity,"transform.velocity") \
 X(transform_submit,submit,"transform.submit") \
 X(transform_take,take,"transform.take") \
 X(transform_cancel,cancel,"transform.cancel") \
 X(transform_clear,clear,"transform.clear")
#ifndef UWVM_SCENE_TRANSFORM_CONTRACT_MACROS_ONLY
#include "buffers.h"
#include "transform_math.h"
namespace uwvm2::imported::ros::transform_contract
{
    namespace wire=scene::wire;
    namespace math=scene::transforms::math;
    inline constexpr std::size_t maximum_frames=128,maximum_batch=128,maximum_frame_bytes=255;
    using name=scene::buffers::text<maximum_frame_bytes>;
    enum class operation:std::uint8_t{open,publish,lookup,available,velocity,submit,take,cancel,clear};
    struct descriptor{operation id;std::string_view name;};
    inline constexpr std::array operations{
#define UWVM_TRANSFORM_DESCRIPTOR(symbol,action,name) descriptor{operation::action,name},
        UWVM_ROS_TRANSFORM_FUNCTIONS(UWVM_TRANSFORM_DESCRIPTOR)
#undef UWVM_TRANSFORM_DESCRIPTOR
    };
    struct stamp
    {
        std::uint64_t clock{},epoch{};std::int64_t nanoseconds{};
        friend bool operator==(stamp const&,stamp const&)=default;
    };
    struct transform{name parent,child;stamp time;math::rigid pose;bool is_static{};};
    struct query
    {name target,source,fixed;stamp target_time,source_time;bool target_stamped{},source_stamped{},has_fixed{};};
    struct velocity_query
    {name tracking,observation,reference,point_frame;math::vector point;stamp time;std::uint64_t averaging_ns{};};
    struct request
    {
        scene::buffers::profile profile;
        std::uint64_t clock_capability{},clock{},key_session{},key_sequence{},buffer{},request_id{},deadline{},expected_generation{};
        std::array<transform,maximum_batch> transforms{};std::size_t count{};
        query lookup;velocity_query velocity;
    };
    inline bool read_name(wire::reader& input,name& output) noexcept
    {
        std::string_view value;
        return input.text(value)&&!value.empty()&&value.find('\0')==value.npos&&output.assign(value);
    }
    inline bool read_stamp(wire::reader& input,stamp& output) noexcept
    {return input.u64(output.clock)&&output.clock&&input.u64(output.epoch)&&input.i64(output.nanoseconds);}
    inline bool write_stamp(wire::writer& out,stamp value) noexcept
    {return value.clock&&out.u64(value.clock)&&out.u64(value.epoch)&&out.i64(value.nanoseconds);}
    inline bool read_vector(wire::reader& input,math::vector& value) noexcept
    {return input.f64(value.x)&&input.f64(value.y)&&input.f64(value.z)&&math::finite(value);}
    inline bool write_vector(wire::writer& out,math::vector value) noexcept
    {return math::finite(value)&&out.f64(value.x)&&out.f64(value.y)&&out.f64(value.z);}
    inline bool read_transform(wire::reader& input,transform& value) noexcept
    {
        auto& q=value.pose.rotation;
        return read_name(input,value.parent)&&read_name(input,value.child)&&value.parent.view()!=value.child.view()&&
            read_stamp(input,value.time)&&read_vector(input,value.pose.translation)&&input.f64(q.x)&&input.f64(q.y)&&
            input.f64(q.z)&&input.f64(q.w)&&math::finite(q)&&input.boolean(value.is_static);
    }
    inline bool write_transform(wire::writer& out,transform const& value) noexcept
    {
        auto q=value.pose.rotation;
        return out.text(value.parent.view())&&out.text(value.child.view())&&write_stamp(out,value.time)&&
            write_vector(out,value.pose.translation)&&math::finite(q)&&out.f64(q.x)&&out.f64(q.y)&&out.f64(q.z)&&out.f64(q.w)&&out.boolean(value.is_static);
    }
    inline bool read_query(wire::reader& input,query& value) noexcept
    {
        return read_name(input,value.target)&&input.boolean(value.target_stamped)&&(!value.target_stamped||read_stamp(input,value.target_time))&&
            read_name(input,value.source)&&input.boolean(value.source_stamped)&&(!value.source_stamped||read_stamp(input,value.source_time))&&
            input.boolean(value.has_fixed)&&(!value.has_fixed||read_name(input,value.fixed));
    }
    /** @brief Decode exactly one operation with no heap allocation or borrowed guest span. */
    inline bool decode(operation action,wire::bytes bytes,request& output) noexcept
    {
        wire::reader input{bytes};request value;
        auto key=[&]{return input.u64(value.key_session)&&value.key_session&&input.u64(value.key_sequence)&&value.key_sequence;};
        switch(action)
        {
        case operation::open:
        {
            std::string_view identity;wire::bytes digest;
            if(!input.text(identity)||identity.empty()||identity.find('\0')!=identity.npos||!value.profile.identity.assign(identity)||
               !input.blob(digest)||digest.size()!=value.profile.sha256.size()||!input.u64(value.clock_capability)||!value.clock_capability||
               !input.u64(value.clock)||!value.clock)return false;
            std::copy(digest.begin(),digest.end(),value.profile.sha256.begin());break;
        }
        case operation::publish:
        {
            std::uint64_t count{};
            if(!key()||!input.u64(value.buffer)||!value.buffer||!input.u64(value.expected_generation)||!input.u64(count)||!count||count>maximum_batch)return false;
            value.count=static_cast<std::size_t>(count);
            for(std::size_t i{};i<value.count;++i)if(!read_transform(input,value.transforms[i]))return false;
            break;
        }
        case operation::lookup:case operation::available:
            if(!input.u64(value.buffer)||!value.buffer||!read_query(input,value.lookup))return false;
            break;
        case operation::velocity:
        {
            auto& v=value.velocity;
            if(!input.u64(value.buffer)||!value.buffer||!read_name(input,v.tracking)||!read_name(input,v.observation)||!read_name(input,v.reference)||
                !read_vector(input,v.point)||!read_name(input,v.point_frame)||!read_stamp(input,v.time)||!input.u64(v.averaging_ns)||!v.averaging_ns)return false;
            break;
        }
        case operation::submit:
            if(!key()||!input.u64(value.buffer)||!value.buffer||!read_query(input,value.lookup)||!input.u64(value.deadline)||!value.deadline||value.deadline>INT64_MAX)return false;
            break;
        case operation::take:case operation::cancel:
            if(!input.u64(value.buffer)||!value.buffer||!input.u64(value.request_id)||!value.request_id)return false;
            break;
        case operation::clear:
            if(!key()||!input.u64(value.buffer)||!value.buffer||!input.u64(value.expected_generation)||!value.expected_generation)return false;
            break;
        default:return false;
        }
        if(!input.done())return false;
        output=value;return true;
    }
    /** @brief Bound carrier allocation before decoding a publication batch. */
    inline std::size_t request_bound(operation action,std::size_t batch) noexcept
    {
        switch(action)
        {
        case operation::open:return 160;
        case operation::publish:return 40+622*std::min(batch,maximum_batch);
        case operation::lookup:case operation::available:return 870;
        case operation::velocity:return 1116;
        case operation::submit:return 894;
        case operation::take:case operation::cancel:return 16;
        case operation::clear:return 32;
        default:return 0;
        }
    }
}
#endif
