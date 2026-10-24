/** Pure sampled-q/configuration model, not native ROS, MuJoCo or robot evidence.
 * Root may compile/run this exact C11 file independently. No source author run
 * is implied. Check explicit non-contiguous actuator maps, family differences,
 * unchanged inputs/output on errors, and finite/configuration boundaries.
 */
#include <platform/unitree/c/roswasm_unitree/simulation_hold.h>
#define CHECK(x)                                                                       \
    do {                                                                               \
        ++checks;                                                                      \
        if (!(x))                                                                      \
            return __LINE__;                                                           \
    } while (0)
static unsigned checks;
unsigned uwvm_unitree_completed_checks(void) {
    return checks;
}
int main(void) {
    uint8_t raw[59] = {0}, saved_raw[59];
    uint32_t word;
    unsigned i;
    uwvm_unitree_hold_config config, saved_config, unchanged;
    uwvm_unitree_hold_motor result[35], saved_result[35];
    float q[35], saved_q[35];
    memcpy(raw, "UHL1", 4);
    uwvm_unitree_put32(raw + 4, 3);
    uwvm_unitree_put32(raw + 8, 2000000);
    raw[12] = 20;
    raw[13] = 2;
    for (i = 24; i < 59; ++i)
        raw[i] = 255;
    raw[24] = 7;
    raw[25] = 1;
    {
        float kp = 25.0f, kd = 0.5f;
        memcpy(&word, &kp, 4);
        uwvm_unitree_put32(raw + 16, word);
        memcpy(&word, &kd, 4);
        uwvm_unitree_put32(raw + 20, word);
    }
    memcpy(saved_raw, raw, sizeof raw);
    memset(&config, 0xa5, sizeof config);
    CHECK(uwvm_unitree_hold_decode(raw, sizeof raw, 20, &config) == 0);
    CHECK(config.steps == 3 && config.dt_ns == 2000000 && config.carrier == 20 &&
          config.active_count == 2 && config.active_slots[0] == 7 &&
          config.active_slots[1] == 1);
    CHECK(config.kp == 25.0f && config.kd == 0.5f &&
          memcmp(raw, saved_raw, sizeof raw) == 0);
    memcpy(&saved_config, &config, sizeof config);
    for (i = 0; i < 35; ++i)
        q[i] = (float)i * 0.25f - 2.0f;
    memcpy(saved_q, q, sizeof q);
    memset(result, 0xa5, sizeof result);
    memcpy(saved_result, result, sizeof result);
    CHECK(uwvm_unitree_hold_step(&config, q, 20, result, 19) == 90 &&
          memcmp(result, saved_result, sizeof result) == 0);
    CHECK(uwvm_unitree_hold_step(&config, q, 20, result, 35) == 0);
    CHECK(result[7].mode == 1 && result[7].q == q[7] && result[7].dq == 0 &&
          result[7].kp == 25 && result[7].kd == 0.5f && result[7].tau == 0);
    CHECK(result[1].q == q[1] && result[1].kp == 25 && result[1].kd == 0.5f);
    CHECK(result[0].mode == 1 && result[0].q == 2.146E9f && result[0].dq == 16000 &&
          result[0].kp == 0 && result[0].kd == 0 && result[0].tau == 0);
    CHECK(memcmp(&config, &saved_config, sizeof config) == 0 &&
          memcmp(q, saved_q, sizeof q) == 0);
    memcpy(saved_result, result, sizeof result);
    word = UINT32_C(0x7fc00001);
    memcpy(&q[19], &word, 4);
    CHECK(uwvm_unitree_hold_step(&config, q, 20, result, 35) == 22 &&
          memcmp(result, saved_result, sizeof result) == 0);
    q[19] = saved_q[19];
    CHECK(uwvm_unitree_hold_step(&config, q, 19, result, 35) == 22 &&
          memcmp(result, saved_result, sizeof result) == 0);
    CHECK(uwvm_unitree_hold_step(&config, q, 20, (uwvm_unitree_hold_motor *)q, 35) ==
              22 &&
          memcmp(q, saved_q, sizeof q) == 0);
    config.active_slots[1] = 7;
    CHECK(uwvm_unitree_hold_step(&config, q, 20, result, 35) == 22 &&
          memcmp(result, saved_result, sizeof result) == 0);
    config = saved_config;
    memcpy(&unchanged, &config, sizeof config);
    raw[25] = 7;
    CHECK(uwvm_unitree_hold_decode(raw, sizeof raw, 20, &config) == 22 &&
          memcmp(&config, &unchanged, sizeof config) == 0);
    raw[25] = 1;
    raw[26] = 0;
    CHECK(uwvm_unitree_hold_decode(raw, sizeof raw, 20, &config) == 22 &&
          memcmp(&config, &unchanged, sizeof config) == 0);
    raw[26] = 255;
    CHECK(uwvm_unitree_hold_decode(raw, 58, 20, &config) == 22 &&
          memcmp(&config, &unchanged, sizeof config) == 0);
    CHECK(uwvm_unitree_hold_decode(raw, 59, 35, &config) == 22 &&
          memcmp(&config, &unchanged, sizeof config) == 0);
    raw[14] = 1;
    CHECK(uwvm_unitree_hold_decode(raw, 59, 20, &config) == 22 &&
          memcmp(&config, &unchanged, sizeof config) == 0);
    raw[14] = 0;
    uwvm_unitree_put32(raw + 16, 0);
    CHECK(uwvm_unitree_hold_decode(raw, 59, 20, &config) == 22 &&
          memcmp(&config, &unchanged, sizeof config) == 0);
    uwvm_unitree_put32(raw + 16, UINT32_C(0x7f800000));
    CHECK(uwvm_unitree_hold_decode(raw, 59, 20, &config) == 22 &&
          memcmp(&config, &unchanged, sizeof config) == 0);
    memcpy(raw, saved_raw, sizeof raw);
    raw[12] = 35;
    raw[14] = 1;
    raw[15] = 5;
    raw[24] = 34;
    raw[25] = 0;
    CHECK(uwvm_unitree_hold_decode(raw, 59, 35, &config) == 0 && config.mode_pr == 1 &&
          config.mode_machine == 5);
    CHECK(uwvm_unitree_hold_step(&config, q, 35, result, 35) == 0);
    CHECK(result[34].mode == 1 && result[34].q == q[34] && result[34].kp == 25 &&
          result[34].kd == 0.5f);
    CHECK(result[1].mode == 0 && result[1].q == 0 && result[1].dq == 0 &&
          result[1].kp == 0 && result[1].kd == 0 && result[1].tau == 0);
    config.kd = 101;
    memcpy(saved_result, result, sizeof result);
    CHECK(uwvm_unitree_hold_step(&config, q, 35, result, 35) == 22 &&
          memcmp(result, saved_result, sizeof result) == 0);
    return 0;
}
