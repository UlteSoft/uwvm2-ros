/** @file
 * @brief AgileX motion translation behind the existing common driver fencing protocol.
 * @details A trusted native adapter serializes this object with the physical
 * effect and its independent watchdog. It uses the common provider generation,
 * installed controller lease, authority epoch, ordered sequence and deadline.
 * A platform name, successful conversion, or receipt is never a substitute for
 * those checks. Default construction denies everything. Configuration never
 * reopens a retired/expired lease; stopped-state rearm requires new trusted
 * observer evidence. No ambient bus access or vendor library is introduced.
 *
 * Translation returns an inert, bounded frame. It does not acknowledge
 * execution. The adapter must recheck time at its bounded effect boundary,
 * report partial/unknown delivery honestly, and fence on any uncertain effect.
 * A zero-motion request is not a physical emergency-stop implementation.
 */
#pragma once
#if 0
// Retained historical port only. Native vendor execution is exclusively an
// isolated-process responsibility. Do not add an enable macro for this port.
#include "device_protocol.h"
#include "platform_agilex_motion.h"

namespace uwvm2::imported::ros::scene::platform_extensions::agilex
{
    class motion_receiver
    {
        devices::protocol::receiver common_;motion_codec codec_;bool configured_{};
    public:
        devices::error configure(devices::handle provider,devices::scalar_schema const& schema,buffers::profile const& stop)noexcept
        {
            if(configured_)return devices::error::denied;
            motion_codec codec;auto status=codec.configure(schema);if(status!=devices::error::success)return status;
            devices::protocol::receiver common;status=common.configure(provider,schema,stop);if(status!=devices::error::success)return status;
            codec_=codec;common_=common;configured_=true;return devices::error::success;
        }
        devices::error rearm_after_stopped(devices::handle provider,devices::handle evidence)noexcept
        {return common_.rearm_after_stopped(provider,evidence);}
        devices::error controller(wire::bytes frame,std::uint64_t now,devices::protocol::header& output)noexcept
        {return common_.controller(frame,now,output);}
        /** @brief Consume the common sequence only after complete semantic conversion succeeds.
         * @details The common transaction refuses malformed vendor semantics. A
         * successfully returned frame consumes the sequence even if the native
         * effect later fails; never replay it on the bus. Reconciliation belongs
         * to the common recovery ledger. Output is unchanged on refusal.
         */
        devices::error command(wire::bytes frame,std::uint64_t now,devices::protocol::header& output,motion_frame& native)noexcept
        {
            devices::protocol::header header;devices::scalar_command command;motion_frame result;
            auto status=common_.command_translate(frame,now,header,command,[&](devices::scalar_command const& value)noexcept{return codec_.encode(value,result);});
            if(status!=devices::error::success)return status;
            output=header;native=result;return devices::error::success;
        }
        /** @brief Latch the shared stop before returning the fixed zero-motion request. */
        devices::error stop(wire::bytes frame,devices::protocol::header& output,motion_frame& native)noexcept
        {
            devices::protocol::header header;auto status=common_.stop(frame,header);if(status!=devices::error::success)return status;
            motion_frame result;status=codec_.zero_motion(result);if(status!=devices::error::success)return status;
            output=header;native=result;return devices::error::success;
        }
        /** @brief Fence an expired command or common lease and return one fixed zero request for the independent watchdog. */
        bool expire(std::uint64_t now,motion_frame& native)noexcept
        {if(!common_.expire(now))return false;return codec_.zero_motion(native)==devices::error::success;}
        bool latched()const noexcept{return common_.latched();}
    };
}
#endif // Historical vendor driver port: deliberately excluded from every build.
