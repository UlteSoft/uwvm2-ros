/* Actual C value program; link the genuine C++20 formatter implementation.
 * Emit each complete canonical Request for an independent original-source
 * method/JSON oracle. This program alone establishes no native transport. */
#include <platform/unitree/c/roswasm_unitree/b2_sport.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
typedef roswasmc_bounded_2_unitree_api_msg_Request Request;
static void require(int value) {
    if (!value)
        abort();
}
static void emit(char const *name, Request const *request) {
    unsigned char bytes[ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUEST_MAX_WIRE_BYTES];
    size_t used = 0, i;
    static char const hex[] = "0123456789abcdef";
    require(roswasmc_bounded_2_unitree_api_msg_Request_encode(
                request, bytes, sizeof bytes, &used) == 0);
    fputs(name, stdout);
    fputc('\t', stdout);
    for (i = 0; i < used; ++i) {
        fputc(hex[bytes[i] >> 4], stdout);
        fputc(hex[bytes[i] & 15], stdout);
    }
    fputc('\n', stdout);
}
int main(void) {
    Request original = {0}, value, before;
    uwvm_unitree_b2_path_point points[30];
    size_t i;
    original.header.identity.id = INT64_C(9007199254740993);
    original.header.identity.api_id = -77;
    original.header.lease.id = -INT64_C(9007199254740993);
    original.header.policy.priority = -101;
    original.header.policy.noreply = 1;
    original.parameter.size = 4;
    memcpy(original.parameter.data, "keep", 4);
    original.binary.size = 4;
    original.binary.data[0] = 0;
    original.binary.data[1] = 255;
    original.binary.data[2] = 1;
    original.binary.data[3] = 128;
    for (i = 0; i < 30; ++i) {
        points[i].timeFromStart = (float)i;
        points[i].x = 0;
        points[i].y = -0.0f;
        points[i].yaw = 0;
        points[i].vx = 0;
        points[i].vy = 0;
        points[i].vyaw = 0;
    }
#define ARGS_none ((uwvm_unitree_sport_arguments){0})
#define ARGS_xyz ((uwvm_unitree_sport_arguments){1.25f, -0.0f, 0.5f, 0, 0})
#define ARGS_position ARGS_xyz
#define ARGS_integer ((uwvm_unitree_sport_arguments){0, 0, 0, INT32_MIN, 0})
#define ARGS_boolean ((uwvm_unitree_sport_arguments){0, 0, 0, 0, 1})
#define ARGS_height ((uwvm_unitree_sport_arguments){-0.125f, 0, 0, 0, 0})
#define ARGS_trajectory ARGS_none
#define POINTS_none NULL, 0
#define POINTS_xyz NULL, 0
#define POINTS_position NULL, 0
#define POINTS_integer NULL, 0
#define POINTS_boolean NULL, 0
#define POINTS_height NULL, 0
#define POINTS_trajectory points, 30
#define UWVM_UNITREE_B2(name, operation, kind)                                         \
    value = original;                                                                  \
    require(uwvm_unitree_b2_sport_prepare(                                             \
                &value, operation, ARGS_##kind, POINTS_##kind) == 0);                  \
    require(memcmp(&value.header.identity.id,                                          \
                   &original.header.identity.id,                                       \
                   sizeof value.header.identity.id) == 0);                             \
    require(value.header.lease.id == original.header.lease.id &&                       \
            value.header.policy.priority == -101 && value.header.policy.noreply == 1); \
    require(value.binary.size == 4 &&                                                  \
            memcmp(value.binary.data, original.binary.data, 4) == 0);                  \
    emit(#name, &value);
#include <platform/unitree/c/roswasm_unitree/b2_sport_operations.inc>
#undef UWVM_UNITREE_B2
    before = original;
    value = original;
    require(uwvm_unitree_b2_sport_prepare(&value, 1018, ARGS_none, points, 29) == 22);
    require(memcmp(&value, &before, sizeof value) == 0);
    require(uwvm_unitree_b2_sport_prepare(&value, 1018, ARGS_none, points, 31) == 22);
    require(memcmp(&value, &before, sizeof value) == 0);
    points[29].vyaw = NAN;
    require(uwvm_unitree_b2_sport_prepare(&value, 1018, ARGS_none, points, 30) == 22);
    require(memcmp(&value, &before, sizeof value) == 0);
    points[29].vyaw = 0;
    for (i = 0; i < 30; ++i)
        points[i] = (uwvm_unitree_b2_path_point){
            FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX};
    require(uwvm_unitree_b2_sport_prepare(&value, 1018, ARGS_none, points, 30) == 90);
    require(memcmp(&value, &before, sizeof value) == 0);
    require(uwvm_unitree_b2_sport_prepare(&value, 2055, ARGS_none, NULL, 0) == 22);
    require(memcmp(&value, &before, sizeof value) == 0);
    require(uwvm_unitree_b2_sport_prepare(&value, 1036, ARGS_xyz, NULL, 0) == 0);
    require(value.header.identity.api_id == 1036);
    require(uwvm_unitree_b2_sport_kind(2055) == -1 &&
            uwvm_unitree_b2_sport_name(2055) == NULL);
    puts("B2_SPORT_LOCAL_OK operations=21 negative_cases=5");
    return 0;
}
