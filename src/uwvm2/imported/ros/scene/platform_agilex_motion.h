/** @file
 * @brief Closed AgileX four-axis motion conversion for the common device manager.
 * @details The reviewed wire layout is ugv_sdk f2704eacdc90357078cd93ec60aae08bb4baab35,
 * protocol_v2 motion command 0x111 and motion feedback 0x221: four signed
 * big-endian 16-bit milliscale quantities, exactly eight bytes. The upstream
 * protocol version is a hardware protocol identity, not an internal API name.
 * No general CAN identifier, socket, register access, firmware operation or
 * mode switch is accepted. This codec performs no I/O and grants no authority.
 *
 * The immutable deployment schema must name all four quantities in order.
 * Unused axes are explicitly constrained to zero. Bounds include zero so a
 * fixed zero-velocity request remains representable. That request is NOT a
 * certified brake or stopped witness: a separate watchdog and trusted state
 * observer are required. Quantization truncates toward zero in integer space,
 * independently of the host FP rounding mode; both original and quantized
 * values are checked against the deployment bounds. Invalid output is never
 * published. Source acquisition or passing codec tests does not qualify a
 * physical robot, native transport or Wasm platform-module registration.
 */
#pragma once
#if 0
// Retained historical port only. Vendor driver implementations must not enter
// the VM. ROSv2 uses original vendor binaries in isolated managed processes;
// portable control algorithms belong in Wasm. This port is not a build option.
#include "device_command.h"

namespace uwvm2::imported::ros::scene::platform_extensions::agilex
{
    inline constexpr std::array<std::string_view,4> motion_names{
        "linear.velocity","angular.velocity","lateral.velocity","steering.angle"};
    inline constexpr std::array motion_units{devices::unit::metre_per_second,devices::unit::radian_per_second,
        devices::unit::metre_per_second,devices::unit::radian};
    /** @brief Portable CAN data representation; identifier and length are closed by each operation. */
    struct motion_frame {std::uint32_t identifier{};std::array<std::uint8_t,8> payload{};};
    struct motion_sample {std::array<double,4> values{};};

    namespace details
    {
        /** @brief Integer conversion after finite and physical-range validation.
         * @pre The represented value is finite and in [-32.768, 32.767].
         * @details The immutable codec's deployment bounds are a subset of
         * that range. Validating against them proves this precondition without
         * another floating-point range check for every axis. A 53-bit
         * significand times 1000 fits uint64 and the resulting magnitude fits
         * signed int16. No input pointer, mutable schema or caller assertion is
         * used to establish the precondition.
         */
        inline std::int16_t truncate_milliscale(std::uint64_t bits)noexcept
        {
            auto exponent=static_cast<unsigned>((bits>>52)&2047u);
            auto mantissa=(bits&UINT64_C(0x000fffffffffffff))|(exponent?UINT64_C(0x0010000000000000):0);
            auto shift=1075u-(exponent?exponent:1u);
            auto magnitude=shift>=64u?UINT64_C(0):(mantissa*UINT64_C(1000))>>shift;
            auto value=static_cast<std::int32_t>(magnitude);
            return static_cast<std::int16_t>((bits>>63)?-value:value);
        }
    }

    /** @brief Convert finite binary64 to integral milliscale without FP multiplication or undefined casts.
     * @details A 53-bit significand times 1000 fits in uint64. Shifts are checked
     * before use. Subnormals truncate to zero; NaN, infinity and values beyond
     * the signed 16-bit physical range are refused. Negative zero becomes zero.
     * This is exact truncation of the represented SI value, not a host-rounded
     * multiply that can move a value across a protocol cell boundary.
     */
    inline bool milliscale(double value,std::int16_t& output)noexcept
    {
        auto bits=std::bit_cast<std::uint64_t>(value);auto exponent=static_cast<unsigned>((bits>>52)&2047u);
        if(exponent==2047u||value < -32.768||value > 32.767)return false;
        output=details::truncate_milliscale(bits);return true;
    }

    /** @brief Single-assignment trusted SI limits; no default active configuration. */
    class motion_codec
    {
        std::array<double,4> minimum_{},maximum_{};bool configured_{};
    public:
        devices::error configure(devices::scalar_schema const& schema)noexcept
        {
            if(configured_)return devices::error::denied;
            if(!devices::valid(schema)||schema.count!=4)return devices::error::invalid;
            for(std::size_t i{};i<4;++i)
            {
                auto const& field=schema.fields[i];
                if(field.name.view()!=motion_names[i]||field.quantity!=motion_units[i]||field.minimum>0||field.maximum<0||
                    field.minimum < -32.768||field.maximum > 32.767)return devices::error::invalid;
            }
            for(std::size_t i{};i<4;++i){minimum_[i]=schema.fields[i].minimum;maximum_[i]=schema.fields[i].maximum;}
            configured_=true;return devices::error::success;
        }
        /** @brief Validate and encode exactly one complete motion vector, with no saturation or dropped axes. */
        devices::error encode(devices::scalar_command const& command,motion_frame& output)const noexcept
        {
            if(!configured_)return devices::error::denied;
            if(command.count!=4)return devices::error::invalid;
            motion_frame frame{0x111,{}};
            for(std::size_t i{};i<4;++i)
            {
                auto number=command.values[i];if(!devices::finite(number))return devices::error::invalid;
                if(number<minimum_[i]||number>maximum_[i])return devices::error::denied;
                auto quantized=details::truncate_milliscale(std::bit_cast<std::uint64_t>(number));
                // Bounds contain zero; exact truncation toward zero cannot
                // increase magnitude or leave this inclusive interval.
                auto word=static_cast<std::uint16_t>(quantized);
                frame.payload[2*i]=static_cast<std::uint8_t>(word>>8);frame.payload[2*i+1]=static_cast<std::uint8_t>(word);
            }
            output=frame;return devices::error::success;
        }
        /** @brief Decode only a complete motion feedback frame; CAN flags and other identifiers are refused.
         * @details Feedback can legitimately exceed commanded bounds, so it is
         * reported rather than clamped. It has no command authority, freshness,
         * peer-authentication or stopped-state implication. Trusted sensor
         * ingress must supply capture sequence, monotonic time and provenance.
         */
        devices::error feedback(std::uint32_t identifier,wire::bytes payload,motion_sample& output)const noexcept
        {
            if(!configured_)return devices::error::denied;
            if(identifier!=0x221||payload.size()!=8)return devices::error::invalid;
            motion_sample sample;
            for(std::size_t i{};i<4;++i)
            {
                auto word=static_cast<std::uint16_t>((static_cast<unsigned>(payload[2*i])<<8)|payload[2*i+1]);
                sample.values[i]=static_cast<double>(std::bit_cast<std::int16_t>(word))/1000.0;
            }
            output=sample;return devices::error::success;
        }
        /** @brief Produce the fixed zero request only for a configured adapter; this does not attest a stop. */
        devices::error zero_motion(motion_frame& output)const noexcept
        {if(!configured_)return devices::error::denied;output={0x111,{}};return devices::error::success;}
    };
}
#endif // Historical vendor driver port: deliberately excluded from every build.
