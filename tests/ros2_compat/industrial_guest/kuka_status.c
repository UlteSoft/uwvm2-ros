/** Actual KUKA status reactor: independent canonical fields and zero authority.
 *
 * Compile this translation unit as a WASI command using the exact generated
 * KUKA C header. The complete nine-byte fixtures below are independent of the
 * generated encoder. All 64 Boolean combinations exercise every exported field.
 * Short, trailing and noncanonical Boolean inputs must preserve both the last
 * complete value and the sample counter. These are local pure-value checks;
 * they do not represent ROS delivery, timestamps or physical robot state.
 *
 * Including the real reactor gives access to its private sample commit helper
 * without adding a public Guest import/export or a policy administration path.
 */
#ifndef UWVM_INDUSTRIAL_C_SOURCE
#error "Select the exact actual KUKA status example source"
#endif
#include UWVM_INDUSTRIAL_C_SOURCE

static int matches(unsigned char const* expected, uint32_t count)
{
    return sample_ready() == (count != 0) && sample_count() == count &&
        control_mode() == expected[0] && cycle_time() == expected[1] &&
        drives_powered() == expected[2] && emergency_stop() == expected[3] &&
        guard_stop() == expected[4] && in_motion() == expected[5] &&
        motion_possible() == expected[6] && operation_mode() == expected[7] &&
        robot_stopped() == expected[8];
}

int main(void)
{
    unsigned char payload[10] = {0};
    static unsigned char const boolean_fields[] = {2, 3, 4, 5, 6, 8};
    unsigned i, j;
    if(sample_ready() || sample_count()) return 1;
    for(i = 0; i != 64; ++i)
    {
        payload[0] = (unsigned char)(i * 3);
        payload[1] = (unsigned char)(i * 5);
        payload[7] = (unsigned char)(i * 7);
        for(j = 0; j != sizeof boolean_fields; ++j)
            payload[boolean_fields[j]] = (unsigned char)((i >> j) & 1);
        if(accept_sample(payload, 9) || !matches(payload, i + 1)) return 2;
        for(j = 0; j != 9; ++j)
            if(!accept_sample(payload, j) || !matches(payload, i + 1)) return 3;
        payload[9] = 0;
        if(!accept_sample(payload, 10) || !matches(payload, i + 1)) return 4;
        for(j = 0; j != sizeof boolean_fields; ++j)
        {
            unsigned char previous = payload[boolean_fields[j]];
            uint32_t error;
            payload[boolean_fields[j]] = 2;
            error = accept_sample(payload, 9);
            payload[boolean_fields[j]] = previous;
            if(!error || !matches(payload, i + 1)) return 5;
        }
    }
    samples = UINT32_MAX;
    if(accept_sample(payload, 9) || !matches(payload, UINT32_MAX)) return 6;
    value = (roswasmc_bounded_2_kuka_driver_interfaces_msg_KssStatus){0};
    samples = 0;
    if(initialize_case(2) != 22 || initialize_case(0) != 0) return 7;
    if(denied_verified() != 1 || primary_status() || cleanup_status()) return 8;
    if(sample_ready() || sample_count() || poll_once() != 9) return 9;
    if(initialize_case(0) != 9 || close_case() != 9) return 10;
    return 0;
}
