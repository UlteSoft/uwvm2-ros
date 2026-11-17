/** @file
 * @brief Closed bounded scalar actuation schema with explicit SI quantities and frame.
 * @details This portable schema is one registered device protocol, not a generic
 * opcode/socket escape. Each immutable field names its physical quantity and
 * absolute permitted range. Every command supplies every field in that exact
 * order, repeating names and units so no implicit conversion or zero fill occurs.
 * More complex trajectories require a separately registered complete envelope
 * validator and qualified native interpolation/limit implementation; this schema
 * does not claim to enforce collision, coupled kinematics or trajectory derivatives.
 * Fresh source provenance, controller fencing, leases and execution deadlines are
 * checked by the device provider in addition to this content decoder.
 */
#pragma once
#include "buffers.h"
namespace uwvm2::imported::ros::scene::devices
{
    using error=resources::error;using handle=resources::handle;using bytes=wire::bytes;
    inline constexpr std::size_t maximum_fields=32;
    inline constexpr std::string_view scalar_type="robot.scalar.command",scalar_encoding="le64";
    enum class unit:std::uint8_t
    {metre,radian,metre_per_second,radian_per_second,metre_per_second_squared,radian_per_second_squared,
     metre_per_second_cubed,radian_per_second_cubed,newton,newton_metre,volt,ampere,pascal,kelvin,ratio};
    inline constexpr std::array unit_names{std::string_view{"m"},std::string_view{"rad"},std::string_view{"m/s"},std::string_view{"rad/s"},
        std::string_view{"m/s2"},std::string_view{"rad/s2"},std::string_view{"m/s3"},std::string_view{"rad/s3"},std::string_view{"N"},
        std::string_view{"N.m"},std::string_view{"V"},std::string_view{"A"},std::string_view{"Pa"},std::string_view{"K"},std::string_view{"1"}};
    inline bool finite(double value)noexcept
    {return (std::bit_cast<std::uint64_t>(value)&UINT64_C(0x7ff0000000000000))!=UINT64_C(0x7ff0000000000000);}
    inline bool valid_text(auto const& value)noexcept
    {return value.size&&value.size<=value.bytes.size()&&value.view().find('\0')==std::string_view::npos&&wire::utf8({reinterpret_cast<std::uint8_t const*>(value.bytes.data()),value.size});}
    /** @brief A declared physical command quantity; finite min/max are mandatory. */
    struct field
    {buffers::text<96> name;unit quantity{};double minimum{},maximum{};};
    struct scalar_schema
    {
        buffers::profile profile;
        buffers::text<96> frame;
        std::array<field,maximum_fields> fields{};
        std::size_t count{};
    };
    /** @brief Fully decoded fixed storage; numbers never refer back to guest memory. */
    struct scalar_command
    {std::array<double,maximum_fields> values{};std::size_t count{};};
    inline bool valid(scalar_schema const& policy)noexcept
    {
        if(!valid_text(policy.profile.identity)||!valid_text(policy.frame)||!policy.count||policy.count>maximum_fields)return false;
        for(std::size_t i{};i<policy.count;++i)
        {
            auto const& field=policy.fields[i];
            if(!valid_text(field.name)||static_cast<std::size_t>(field.quantity)>=unit_names.size()||!finite(field.minimum)||!finite(field.maximum)||field.minimum>field.maximum)return false;
            for(std::size_t n{};n<i;++n)if(field.name==policy.fields[n].name)return false;
        }
        return true;
    }
    /** @brief Exact wire is frame text, field count, then name/unit/f64 per field.
     * @details This method allocates nothing and leaves output unchanged on any
     * failure. NaN, infinity, wrong SI quantities, reordering, duplicate/omitted
     * fields, trailing bytes and out-of-range values are all rejected. Ranges
     * compare represented values directly; no derived floating-point tolerance
     * or host rounding-mode dependent unit conversion enlarges a bound.
     */
    inline error decode(bytes body,scalar_schema const& policy,scalar_command& output)noexcept
    {
        if(!valid(policy))return error::denied;
        // 96-byte frame and 32 bounded names/units/f64 values fit this hard bound.
        if(body.size()>8192)return error::too_large;
        wire::reader input{body};std::string_view frame;std::uint64_t count{};
        if(!input.text(frame)||frame!=policy.frame.view()||!input.u64(count)||count!=policy.count)return error::invalid;
        scalar_command value;value.count=static_cast<std::size_t>(count);
        for(std::size_t i{};i<value.count;++i)
        {
            std::string_view name,unit;auto const& field=policy.fields[i];
            if(!input.text(name)||name!=field.name.view()||!input.text(unit)||unit!=unit_names[static_cast<std::size_t>(field.quantity)]||!input.f64(value.values[i]))return error::invalid;
            auto number=value.values[i];if(!finite(number))return error::invalid;
            if(number<field.minimum||number>field.maximum)return error::denied;
        }
        if(!input.done()){return error::invalid;}
        output=value;return error::success;
    }
    /** @brief Encode the same closed frame and unit mapping for a qualified receiver. */
    inline bool encode(wire::writer& output,scalar_schema const& policy,scalar_command const& value)noexcept
    {
        if(!valid(policy)||value.count!=policy.count)return false;
        for(std::size_t i{};i<value.count;++i)
            if(!finite(value.values[i])||value.values[i]<policy.fields[i].minimum||value.values[i]>policy.fields[i].maximum)return false;
        if(!output.text(policy.frame.view())||!output.u64(value.count))return false;
        for(std::size_t i{};i<value.count;++i)
        {
            auto const& field=policy.fields[i];if(!output.text(field.name.view())||!output.text(unit_names[static_cast<std::size_t>(field.quantity)])||!output.f64(value.values[i]))return false;
        }
        return true;
    }
    /** @brief Freeze a valid scalar schema once for repeated trusted receiver decoding.
     * @details Ownership makes the cached validity independent of subsequent
     * caller mutation. Wire strings must match the already validated UTF-8
     * strings byte for byte, so rescanning UTF-8 and duplicate field names on
     * every command would be redundant. Every numeric value, exact extent,
     * unit, order and bound is still checked on each call. This optimization
     * caches no authority, source witness, time or controller admission.
     */
    class scalar_decoder
    {
        scalar_schema schema_;bool configured_{};
        static bool exact(wire::reader& input,std::string_view expected)noexcept
        {
            wire::bytes bytes;if(!input.blob(bytes)||bytes.size()!=expected.size())return false;
            return bytes.empty()||std::memcmp(bytes.data(),expected.data(),bytes.size())==0;
        }
    public:
        error configure(scalar_schema const& schema)noexcept
        {
            if(configured_)return error::denied;
            if(!valid(schema))return error::invalid;
            schema_=schema;configured_=true;return error::success;
        }
        buffers::profile const& profile()const noexcept{return schema_.profile;}
        error decode(bytes body,scalar_command& output)const noexcept
        {
            if(!configured_)return error::denied;
            if(body.size()>8192)return error::too_large;
            wire::reader input{body};std::uint64_t count{};
            if(!exact(input,schema_.frame.view())||!input.u64(count)||count!=schema_.count)return error::invalid;
            scalar_command value;value.count=schema_.count;
            for(std::size_t i{};i<value.count;++i)
            {
                auto const& field=schema_.fields[i];
                if(!exact(input,field.name.view())||!exact(input,unit_names[static_cast<std::size_t>(field.quantity)])||!input.f64(value.values[i]))return error::invalid;
                auto number=value.values[i];if(!finite(number))return error::invalid;
                if(number<field.minimum||number>field.maximum)return error::denied;
            }
            if(!input.done())return error::invalid;
            output=value;return error::success;
        }
    };
}
