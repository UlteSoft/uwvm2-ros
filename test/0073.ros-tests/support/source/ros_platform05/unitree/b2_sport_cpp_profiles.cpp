/** C++98-compatible complete named B2 facade contract over the C ABI.
 * Compile in each actual requested compiler mode; the formatter stays C++20.
 * A passing value program is not ROS transport or the full shared C++ SDK.
 */
#include <platform/unitree/cpp/roswasm_unitree/b2_sport.hpp>
#include <stdio.h>
#include <stdlib.h>
static void require(bool value) {
    if (!value)
        abort();
}
int main() {
    typedef roswasm_unitree::B2SportClient Builder;
    roswasmc_bounded_2_unitree_api_msg_Request value = {0};
    int64_t identity = (static_cast<int64_t>(1) << 53) + 1;
    value.header.identity.id = identity;
    value.header.lease.id = -identity;
    value.header.policy.priority = -101;
    value.header.policy.noreply = 1;
    value.binary.size = 4;
    value.binary.data[1] = 255;
    value.binary.data[2] = 1;
    value.binary.data[3] = 128;
    uwvm_unitree_b2_path_point points[30] = {0};
    for (unsigned i = 0; i < 30; ++i)
        points[i].timeFromStart = static_cast<float>(i);
    unsigned calls = 0;
#define CALL_none(name) Builder::name(value)
#define CALL_xyz(name) Builder::name(value, 1.25f, -0.0f, 0.5f)
#define CALL_position(name) CALL_xyz(name)
#define CALL_integer(name) Builder::name(value, static_cast<int32_t>(-2147483647 - 1))
#define CALL_boolean(name) Builder::name(value, true)
#define CALL_height(name) Builder::name(value, -0.125f)
#define CALL_trajectory(name) Builder::name(value, points, 30)
#define UWVM_UNITREE_B2(name, operation, kind)                                         \
    require(CALL_##kind(name) == 0);                                                   \
    require(value.header.identity.api_id == operation);                                \
    require(value.header.identity.id == identity &&                                    \
            value.header.lease.id == -identity);                                       \
    require(value.header.policy.priority == -101 &&                                    \
            value.header.policy.noreply == 1 && value.binary.size == 4 &&              \
            value.binary.data[1] == 255 && value.binary.data[3] == 128);               \
    ++calls;
#include <platform/unitree/c/roswasm_unitree/b2_sport_operations.inc>
#undef UWVM_UNITREE_B2
    require(calls == 21);
    require(Builder::TrajectoryFollow(value, points, 29) == 22);
    require(Builder::TrajectoryFollow(value, points, 31) == 22);
    printf("B2_CPP_PROFILE_OK standard=%d operations=21\n", ROSWASM_CPP_STANDARD);
    return 0;
}
