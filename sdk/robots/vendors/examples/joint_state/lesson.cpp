/** C++20 complete generated typed subscription, no native ROS libraries.
 * The complete C inline representation is an intentional generator API, not a
 * projection. Codec<Message> and its bound come from the actual generated .hpp.
 */
#include <cstdio>
#include "vendor_binding.h"
#include "../../../../custom-types/generated-official-standard/bounded_records.hpp"
#include "../telemetry.hpp"
#if UWVM_JOINT_STATE_ROS_VERSION == 1
#include <roswasm/ros1.h>
using Message=uwvm_custom::Ros1Sensor_msgsJointState;
#define JOINT_BACKEND roswasmc_ros1_backend
#else
#include <roswasm/ros2.h>
using Message=uwvm_custom::Ros2Sensor_msgsMsgJointState;
#define JOINT_BACKEND roswasmc_ros2_backend
#endif
#ifndef UWVM_JOINT_STATE_MODE
#define UWVM_JOINT_STATE_MODE 0
#endif
static_assert(UWVM_JOINT_STATE_MODE==0||UWVM_JOINT_STATE_MODE==1);
static uint32_t observe(Message const& value){return joint_state_full_value(&value)?0u:22u;}
int main() {
    uwvm_vendor::Telemetry<Message> owner{*JOINT_BACKEND(),roswasmcpp::Codec<Message>::max_wire_bytes,observe};
    uint32_t first=owner.initialize(UWVM_JOINT_STATE_MODE,"joint-state");
    if(UWVM_JOINT_STATE_MODE==0) {
        if(!first&&(!owner.denied_verified()||owner.received()!=0u))first=22;
    } else if(!first) {
        for(unsigned pass{};pass!=64u&&owner.received()==0u;++pass) {
            if(std::getchar()!='R'){first=22;break;}
            first=owner.poll_once();if(first==11u){first=0;continue;}if(first)break;
        }
        if(!first&&owner.received()!=1u)first=110;
        auto cleanup{owner.close()};if(!first)first=cleanup;
    }
    if(!first&&owner.cleanup_error())first=owner.cleanup_error();
    if(!first&&(std::fputs(UWVM_JOINT_STATE_MODE?"PASS_JOINT_STATE_FULL_VALUE\n":"PASS_JOINT_STATE_DEFAULT_DENIED\n",stdout)<0||std::fflush(stdout)))first=107;
    return static_cast<int>(first);
}
