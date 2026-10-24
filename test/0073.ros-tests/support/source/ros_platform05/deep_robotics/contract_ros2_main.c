/* Authored complete records and transactional malformed/capacity cases.
 * These are SDK contracts, not original upstream or hardware tests. */
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "state.h"
#define INIT_0(P, v) ((void)0)
#define INIT_1(P, v) assert(P##_default(&(v)) == 0)
#define CHECK(P, D)                                                                    \
    do {                                                                               \
        P a = {0}, out, before;                                                        \
        unsigned char wire[32769];                                                     \
        size_t n = 0, i;                                                               \
        INIT_##D(P, a);                                                                \
        assert(P##_encode(&a, wire, 32768, &n) == 0);                                  \
        assert(P##_decode(wire, n, &out) == 0);                                        \
        memset(&out, 0x55, sizeof out);                                                \
        memcpy(&before, &out, sizeof out);                                             \
        for (i = 0; i < n; ++i) {                                                      \
            assert(P##_decode(wire, i, &out) != 0);                                    \
            assert(!memcmp(&out, &before, sizeof out));                                \
        }                                                                              \
        wire[n] = 0xff;                                                                \
        assert(P##_decode(wire, n + 1, &out) != 0);                                    \
        assert(!memcmp(&out, &before, sizeof out));                                    \
        if (n) {                                                                       \
            size_t used = 99;                                                          \
            memset(wire, 0x66, n);                                                     \
            assert(P##_encode(&a, wire, n - 1, &used) != 0);                           \
            for (i = 0; i < n; ++i)                                                    \
                assert(wire[i] == 0x66);                                               \
        }                                                                              \
    } while (0)
#if defined(__GNUC__)
#define UWVM_DEEP_ROBOTICS_TEST_NOINLINE __attribute__((noinline))
#else
#define UWVM_DEEP_ROBOTICS_TEST_NOINLINE
#endif
/* One finite frame per record also works in unoptimized native CMake builds. */
#define DECL(P, D)                                                                     \
    static UWVM_DEEP_ROBOTICS_TEST_NOINLINE void check_##P(void) {                     \
        CHECK(P, D);                                                                   \
    }
DECL(roswasmc_bounded_2_builtin_interfaces_msg_Time, 0);
DECL(roswasmc_bounded_2_drdds_msg_MetaType, 0);
DECL(roswasmc_bounded_2_drdds_msg_BatteryDataValue, 0);
DECL(roswasmc_bounded_2_drdds_msg_BatteryData, 0);
DECL(roswasmc_bounded_2_drdds_msg_GamepadData, 0);
DECL(roswasmc_bounded_2_drdds_msg_ImuDataValue, 0);
DECL(roswasmc_bounded_2_drdds_msg_ImuData, 0);
DECL(roswasmc_bounded_2_drdds_msg_JointCmdValue, 0);
DECL(roswasmc_bounded_2_drdds_msg_JointData, 0);
DECL(roswasmc_bounded_2_drdds_msg_JointDataCmd, 0);
DECL(roswasmc_bounded_2_drdds_msg_JointValue, 0);
DECL(roswasmc_bounded_2_drdds_msg_JointsDataValue, 0);
DECL(roswasmc_bounded_2_drdds_msg_JointsData, 0);
DECL(roswasmc_bounded_2_drdds_msg_JointsDataCmdValue, 0);
DECL(roswasmc_bounded_2_drdds_msg_JointsDataCmd, 0);
DECL(roswasmc_bounded_2_std_msgs_msg_Header, 0);
DECL(roswasmc_bounded_2_geometry_msgs_msg_Vector3, 0);
DECL(roswasmc_bounded_2_geometry_msgs_msg_Twist, 0);
DECL(roswasmc_bounded_2_sensor_msgs_msg_JointState, 0);
DECL(roswasmc_bounded_2_drdds_msg_RobotStatus, 0);
DECL(roswasmc_bounded_2_drdds_msg_StdMsgFloat32, 0);
DECL(roswasmc_bounded_2_drdds_msg_StdMsgInt32, 0);
DECL(roswasmc_bounded_2_drdds_msg_StdStatus, 0);
DECL(roswasmc_bounded_2_drdds_msg_SteerValue, 0);
DECL(roswasmc_bounded_2_drdds_msg_Steer, 0);
DECL(roswasmc_bounded_2_drdds_srv_StdSrvInt32_Request, 0);
DECL(roswasmc_bounded_2_drdds_srv_StdSrvInt32_Response, 0);
int main(void) {
    check_roswasmc_bounded_2_builtin_interfaces_msg_Time();
    check_roswasmc_bounded_2_drdds_msg_MetaType();
    check_roswasmc_bounded_2_drdds_msg_BatteryDataValue();
    check_roswasmc_bounded_2_drdds_msg_BatteryData();
    check_roswasmc_bounded_2_drdds_msg_GamepadData();
    check_roswasmc_bounded_2_drdds_msg_ImuDataValue();
    check_roswasmc_bounded_2_drdds_msg_ImuData();
    check_roswasmc_bounded_2_drdds_msg_JointCmdValue();
    check_roswasmc_bounded_2_drdds_msg_JointData();
    check_roswasmc_bounded_2_drdds_msg_JointDataCmd();
    check_roswasmc_bounded_2_drdds_msg_JointValue();
    check_roswasmc_bounded_2_drdds_msg_JointsDataValue();
    check_roswasmc_bounded_2_drdds_msg_JointsData();
    check_roswasmc_bounded_2_drdds_msg_JointsDataCmdValue();
    check_roswasmc_bounded_2_drdds_msg_JointsDataCmd();
    check_roswasmc_bounded_2_std_msgs_msg_Header();
    check_roswasmc_bounded_2_geometry_msgs_msg_Vector3();
    check_roswasmc_bounded_2_geometry_msgs_msg_Twist();
    check_roswasmc_bounded_2_sensor_msgs_msg_JointState();
    check_roswasmc_bounded_2_drdds_msg_RobotStatus();
    check_roswasmc_bounded_2_drdds_msg_StdMsgFloat32();
    check_roswasmc_bounded_2_drdds_msg_StdMsgInt32();
    check_roswasmc_bounded_2_drdds_msg_StdStatus();
    check_roswasmc_bounded_2_drdds_msg_SteerValue();
    check_roswasmc_bounded_2_drdds_msg_Steer();
    check_roswasmc_bounded_2_drdds_srv_StdSrvInt32_Request();
    check_roswasmc_bounded_2_drdds_srv_StdSrvInt32_Response();
    return 0;
}
