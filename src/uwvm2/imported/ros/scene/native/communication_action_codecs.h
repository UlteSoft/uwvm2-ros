/** @file
 * @brief Complete canonical LookupTransform action bodies, without native layout.
 * @details Strings are length-prefixed UTF-8 with no embedded NUL; native ROS
 * Time/Duration seconds are signed 32-bit and nanoseconds are below one billion.
 * Every transform scalar must be finite. This codec preserves frame names and
 * error details; it does not attest frame provenance, calibration or a physical
 * transform. Such admission belongs to the separately configured source policy.
 */
#pragma once
#include "external_process_only.h"
#include "../wire.h"
#include "../communication_codecs.h"
#include <tf2_msgs/action/lookup_transform.hpp>
#include <cmath>
#include <array>
#include <limits>

namespace uwvm2::imported::ros::scene::communication::ros2
{
    namespace action_wire = ::uwvm2::imported::ros::scene::wire;

    inline bool action_text(action_wire::reader& input, std::string_view& text) noexcept
    { return input.text(text) && text.find('\0') == std::string_view::npos; }

    inline bool action_time(action_wire::reader& input, std::int32_t& seconds, std::uint32_t& nanos) noexcept
    {
        std::int64_t s{};
        std::uint64_t n{};
        if(!input.i64(s) || s < INT32_MIN || s > INT32_MAX || !input.u64(n) || n >= 1000000000) { return false; }
        seconds = static_cast<std::int32_t>(s);
        nanos = static_cast<std::uint32_t>(n);
        return true;
    }

    struct lookup_goal_codec : codecs::lookup_goal
    {
        using native_type = tf2_msgs::action::LookupTransform::Goal;
        inline static constexpr std::string_view identity{"tf2_msgs/action/LookupTransform_Goal"};

        static bool parse(action_wire::bytes body, native_type* value)
        {
            action_wire::reader input{body};
            std::string_view target, source, fixed;
            std::int32_t source_seconds{}, timeout_seconds{}, target_seconds{};
            std::uint32_t source_nanos{}, timeout_nanos{}, target_nanos{};
            bool advanced{};
            if(!action_text(input, target) || !action_text(input, source) || !action_time(input, source_seconds, source_nanos) ||
               !action_time(input, timeout_seconds, timeout_nanos) || !action_time(input, target_seconds, target_nanos) || !action_text(input, fixed) ||
               !input.boolean(advanced) || !input.done())
            {
                return false;
            }
            if(value)
            {
                value->target_frame.assign(target);
                value->source_frame.assign(source);
                value->fixed_frame.assign(fixed);
                value->source_time.sec = source_seconds;
                value->source_time.nanosec = source_nanos;
                value->timeout.sec = timeout_seconds;
                value->timeout.nanosec = timeout_nanos;
                value->target_time.sec = target_seconds;
                value->target_time.nanosec = target_nanos;
                value->advanced = advanced;
            }
            return true;
        }


        static bool decode(action_wire::bytes body, native_type& value) { return parse(body, &value); }

        static bool encode(native_type const& value, std::span<std::uint8_t> body, std::size_t& size) noexcept
        {
            action_wire::writer output{body};
            if(!output.text(value.target_frame) || !output.text(value.source_frame) || !output.i64(value.source_time.sec) ||
               !output.u64(value.source_time.nanosec) || !output.i64(value.timeout.sec) || !output.u64(value.timeout.nanosec) ||
               !output.i64(value.target_time.sec) || !output.u64(value.target_time.nanosec) || !output.text(value.fixed_frame) ||
               !output.boolean(value.advanced) || !validate(output.written()))
            {
                return false;
            }
            size = output.size();
            return true;
        }
    };

    struct lookup_feedback_codec : codecs::lookup_feedback
    {
        using native_type = tf2_msgs::action::LookupTransform::Feedback;
        inline static constexpr std::string_view identity{"tf2_msgs/action/LookupTransform_Feedback"};


        static bool decode(action_wire::bytes body, native_type&) noexcept { return body.empty(); }

        static bool encode(native_type const&, std::span<std::uint8_t>, std::size_t& size) noexcept
        {
            size = 0;
            return true;
        }
    };

    struct lookup_result_codec : codecs::lookup_result
    {
        using native_type = tf2_msgs::action::LookupTransform::Result;
        inline static constexpr std::string_view identity{"tf2_msgs/action/LookupTransform_Result"};

        static bool parse(action_wire::bytes body, native_type* value)
        {
            action_wire::reader input{body};
            std::int32_t seconds{};
            std::uint32_t nanos{};
            std::string_view frame, child, message;
            std::array<double, 7> scalars{};
            std::uint64_t error{};
            if(!action_time(input, seconds, nanos) || !action_text(input, frame) || !action_text(input, child)) { return false; }
            for(auto& scalar: scalars)
            {
                if(!input.f64(scalar) || !std::isfinite(scalar)) { return false; }
            }
            if(!input.u64(error) || error > UINT8_MAX || !action_text(input, message) || !input.done()) { return false; }
            if(value)
            {
                value->transform.header.stamp.sec = seconds;
                value->transform.header.stamp.nanosec = nanos;
                value->transform.header.frame_id.assign(frame);
                value->transform.child_frame_id.assign(child);
                auto& transform = value->transform.transform;
                transform.translation.x = scalars[0];
                transform.translation.y = scalars[1];
                transform.translation.z = scalars[2];
                transform.rotation.x = scalars[3];
                transform.rotation.y = scalars[4];
                transform.rotation.z = scalars[5];
                transform.rotation.w = scalars[6];
                value->error.error = static_cast<std::uint8_t>(error);
                value->error.error_string.assign(message);
            }
            return true;
        }


        static bool decode(action_wire::bytes body, native_type& value) { return parse(body, &value); }

        static bool encode(native_type const& value, std::span<std::uint8_t> body, std::size_t& size) noexcept
        {
            action_wire::writer output{body};
            if(!output.i64(value.transform.header.stamp.sec) || !output.u64(value.transform.header.stamp.nanosec) ||
               !output.text(value.transform.header.frame_id) || !output.text(value.transform.child_frame_id))
            {
                return false;
            }
            auto const& t = value.transform.transform;
            for(auto scalar: {t.translation.x, t.translation.y, t.translation.z, t.rotation.x, t.rotation.y, t.rotation.z, t.rotation.w})
            {
                if(!std::isfinite(scalar) || !output.f64(scalar)) { return false; }
            }
            if(!output.u64(value.error.error) || !output.text(value.error.error_string) || !validate(output.written())) { return false; }
            size = output.size();
            return true;
        }
    };
}  // namespace uwvm2::imported::ros::scene::communication::ros2
