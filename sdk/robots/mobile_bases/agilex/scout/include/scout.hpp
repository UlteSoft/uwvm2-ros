/* SPDX-License-Identifier: Apache-2.0
 * C++20 names and checked operations over the same complete C representation.
 * No constructor opens ROS, allocates native objects, loads a driver or grants
 * authority. The matching generated header supplies real Codec specializations.
 */
#pragma once
#include "bounded_records.hpp"
#include "scout.h"
namespace roswasm_agilex::scout {
using Status=uwvm_scout_status;
using LightCommand=uwvm_scout_light;
using RemoteState=uwvm_scout_remote;
using Odometry=uwvm_scout_odometry;
using MotionLimits=uwvm_scout_motion_limits;
class Client {
    uwvm_scout_storage storage_{};
public:
    /** Serialize access to this caller-owned fixed buffer. Idle/transport/
     * decoder errors remain explicit, and refused values preserve output. */
    std::uint32_t status(roswasmc_capability const& source,Status& output) noexcept {
        return uwvm_scout_poll_status(&source,&output,&storage_);
    }
    std::uint32_t odometry(roswasmc_capability const& source,Odometry& output) noexcept {
        return uwvm_scout_poll_odometry(&source,&output,&storage_);
    }
    std::uint32_t remote(roswasmc_capability const& source,RemoteState& output) noexcept {
        return uwvm_scout_poll_remote(&source,&output,&storage_);
    }
    std::uint32_t lights(roswasmc_capability const& source,LightCommand const& value) noexcept {
        return uwvm_scout_publish_light(&source,&value,&storage_);
    }
    std::uint32_t motion(roswasmc_capability const& source,MotionLimits const& limits,
                         double forward,double lateral,double yaw) noexcept {
        if(!uwvm_scout_matches_backend(&source))return 22;
        roswasmc_twist value{};auto code=uwvm_scout_make_motion(&limits,forward,lateral,yaw,&value);
        return code?code:roswasmc_publish_twist(&source,&value);
    }
    std::uint32_t stop(roswasmc_capability const& source) noexcept {return uwvm_scout_stop(&source);}
};
}
