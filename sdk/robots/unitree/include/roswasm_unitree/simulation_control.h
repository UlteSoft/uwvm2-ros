/** SDK-shipped simulator-only authoritative math shared with the independent
 * native/Wasm fixtures. This is not a physical robot controller or safety API.
 * Kept as one source: test headers forward here; no copied second trajectory.
 */
/**
 * @file controller_kernel.h
 * @brief Portable Go2 stand/lower trajectory shared by native and Wasm tests.
 *
 * Adapted mathematically from Unitree's BSD-3-Clause stand_go2 ROS2 example:
 * https://github.com/unitreerobotics/unitree_mujoco/blob/1eb6642e3f3fdfb7fb13a9794fd6a2dd93ea0e7d/example/ros2/src/stand_go2.cpp
 * The upstream license and exact commit belong in the selected acquisition
 * lock. This fixture is a simulator controller, not a physical robot driver.
 *
 * Motor order is FR, FL, RR, RL, with hip/thigh/calf within each leg. The first
 * twelve of twenty Go motors are active. The other motors retain the official
 * stop sentinel and zero gains. Outputs are q [rad], dq [rad/s], kp, kd, tau
 * [N m]. A step is a logical integer; this kernel performs no wall-clock read,
 * DDS call, allocation, capability lookup, or hidden I/O. No fast-math is valid
 * for the comparison. Native and Wasm use this very same header and constants.
 */
#ifndef UWVM_UNITREE_CONTROLLER_KERNEL_H
#define UWVM_UNITREE_CONTROLLER_KERNEL_H
#include <math.h>
#include <stdint.h>

typedef struct uwvm_unitree_fixture_motor {
    uint32_t mode;
    float q, dq, kp, kd, tau;
} uwvm_unitree_fixture_motor;

static int uwvm_unitree_go2_fixture_step(uint32_t step, double dt,
                                        uwvm_unitree_fixture_motor out[20]) {
    static double const up[12] = {0.00571868, 0.608813, -1.21763,
        -0.00571868, 0.608813, -1.21763, 0.00571868, 0.608813, -1.21763,
        -0.00571868, 0.608813, -1.21763};
    static double const down[12] = {0.0473455, 1.22187, -2.44375,
        -0.0473455, 1.22187, -2.44375, 0.0473455, 1.22187, -2.44375,
        -0.0473455, 1.22187, -2.44375};
    if (!out || step == 0 || step > 10000 || !isfinite(dt) || dt < .0001 || dt > .02)
        return 22;
    double time = (double)step * dt;
    double phase = tanh((time < 3.0 ? time : time - 3.0) / 1.2);
    for (unsigned i = 0; i != 20; ++i) {
        out[i].mode = 1;
        out[i].q = 2.146E9f;
        out[i].dq = 16000.0f;
        out[i].kp = out[i].kd = out[i].tau = 0;
        if (i < 12) {
            out[i].q = (float)(phase * (time < 3.0 ? up[i] : down[i])
                              + (1.0 - phase) * (time < 3.0 ? down[i] : up[i]));
            out[i].dq = 0;
            out[i].kp = time < 3.0 ? (float)(phase * 50.0 + (1.0 - phase) * 20.0) : 50.0f;
            out[i].kd = 3.5f;
        }
    }
    return 0;
}
#endif
