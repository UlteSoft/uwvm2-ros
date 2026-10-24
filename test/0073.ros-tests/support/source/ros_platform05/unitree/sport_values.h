/** @file Nonzero complete-frame and transactional-refusal data contract.
 * This test performs no ROS call. An independent original-source Python oracle
 * checks every emitted field and parses the actual JSON semantically. It must
 * be compiled both as C11 and as C++20 with the real named facade enabled.
 */
#include <platform/unitree/c/roswasm_unitree/sport.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
#ifdef UWVM_UNITREE_SPORT_CPP_CONTRACT
#include <platform/unitree/cpp/roswasm_unitree/sport.hpp>
#endif
typedef roswasmc_bounded_2_unitree_api_msg_Request sport_request;
static sport_request initial(void) {
    sport_request r = {0};
    r.header.identity.id = INT64_C(0x123456789ab);
    r.header.identity.api_id = 319;
    r.header.lease.id = INT64_C(0x23456789abc);
    r.header.policy.priority = -13;
    r.header.policy.noreply = 1;
    {
        char const text[] = "{\"original\":19}";
        r.parameter.size = sizeof text - 1;
        memcpy(r.parameter.data, text, sizeof text - 1);
    }
    r.binary.size = 3;
    r.binary.data[0] = 17;
    r.binary.data[1] = 128;
    r.binary.data[2] = 255;
    return r;
}
static uint32_t emit(sport_request const *r) {
    unsigned char bytes[ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUEST_MAX_WIRE_BYTES],
        extent[4];
    size_t used = 0;
    if (roswasmc_bounded_2_unitree_api_msg_Request_encode(
            r, bytes, sizeof bytes, &used))
        return 1;
    uwvm_bounded_put(extent, used, 4);
    return fwrite(extent, 1, 4, stdout) != 4 || fwrite(bytes, 1, used, stdout) != used;
}
static int refusals(void) {
    sport_request r = initial(), before;
    uwvm_unitree_sport_arguments a = {0};
#define REFUSE(op)                                                                     \
    do {                                                                               \
        memcpy(&before, &r, sizeof r);                                                 \
        if (uwvm_unitree_sport_prepare(&r, op, a) != 22 ||                             \
            memcmp(&before, &r, sizeof r))                                             \
            return 1;                                                                  \
    } while (0)
    REFUSE(0);
    REFUSE(UINT32_MAX);
    a.x = NAN;
    REFUSE(1008);
    a.x = INFINITY;
    REFUSE(1007);
    a.x = -INFINITY;
    REFUSE(1007);
    a.x = 1;
    REFUSE(1001);
    a.x = 0;
    a.level = 1;
    REFUSE(1008);
    a.level = 0;
    a.flag = 2;
    REFUSE(1027);
    a.flag = 1;
    REFUSE(1015);
    a.flag = 0;
    r.header.policy.noreply = 2;
    REFUSE(1001);
    r.header.policy.noreply = 1;
    r.parameter.data[0] = 0;
    REFUSE(1001);
    r = initial();
    r.parameter.size = 4097;
    REFUSE(1001);
    r = initial();
    r.binary.size = 2049;
    REFUSE(1001);
    if (uwvm_unitree_sport_prepare(NULL, 1001, a) != 22)
        return 1;
    if (uwvm_unitree_sport_name(0) || uwvm_unitree_sport_kind(0) != -1)
        return 1;
#undef REFUSE
    return 0;
}
int main(void) {
    if (refusals())
        return 1;
    for (unsigned variant = 0; variant < 2; ++variant) {
#define UWVM_UNITREE_SPORT(name, id, kind)                                             \
    do {                                                                               \
        sport_request r = initial();                                                   \
        uwvm_unitree_sport_arguments a = {0};                                          \
        int k = uwvm_unitree_sport_kind(id);                                           \
        if (k == UWVM_UNITREE_SPORT_xyz) {                                             \
            a.x = variant ? FLT_MAX : 1.25f;                                           \
            a.y = variant ? FLT_TRUE_MIN : -2.5f;                                      \
            a.z = variant ? -0.0f : 0.0625f;                                           \
        }                                                                              \
        if (k == UWVM_UNITREE_SPORT_integer)                                           \
            a.level = variant ? INT32_MIN : -7;                                        \
        if (k == UWVM_UNITREE_SPORT_boolean)                                           \
            a.flag = variant ? 0 : 1;                                                  \
        if (uwvm_unitree_sport_prepare(&r, id, a) ||                                   \
            strcmp(uwvm_unitree_sport_name(id), #name))                                \
            return 2;                                                                  \
        if (emit(&r))                                                                  \
            return 3;                                                                  \
    } while (0);
#include <platform/unitree/c/roswasm_unitree/sport_operations.inc>
#undef UWVM_UNITREE_SPORT
    }
#ifdef UWVM_UNITREE_SPORT_CPP_CONTRACT
    // Compile and execute every named C++ facade, including every signature.
#define UWVM_UNITREE_SPORT_none(name) roswasm_unitree::SportClient::name(r)
#define UWVM_UNITREE_SPORT_query(name) UWVM_UNITREE_SPORT_none(name)
#define UWVM_UNITREE_SPORT_xyz(name)                                                   \
    roswasm_unitree::SportClient::name(r, 1.25f, -2.5f, 0.0625f)
#define UWVM_UNITREE_SPORT_integer(name) roswasm_unitree::SportClient::name(r, -7)
#define UWVM_UNITREE_SPORT_boolean(name) roswasm_unitree::SportClient::name(r, true)
#define UWVM_UNITREE_SPORT(name, id, kind)                                             \
    do {                                                                               \
        sport_request r = initial();                                                   \
        if (UWVM_UNITREE_SPORT_##kind(name) || r.header.identity.api_id != id)         \
            return 4;                                                                  \
    } while (0);
#include <platform/unitree/c/roswasm_unitree/sport_operations.inc>
#undef UWVM_UNITREE_SPORT
#undef UWVM_UNITREE_SPORT_none
#undef UWVM_UNITREE_SPORT_query
#undef UWVM_UNITREE_SPORT_xyz
#undef UWVM_UNITREE_SPORT_integer
#undef UWVM_UNITREE_SPORT_boolean
#endif
    return fflush(stdout) != 0;
}
