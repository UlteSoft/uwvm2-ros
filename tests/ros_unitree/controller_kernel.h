/**
 * @file controller_kernel.h
 * @brief Forward the single SDK-owned simulator controller implementation.
 *
 * Native and Wasm fixtures compile the exact same authoritative header. Keep
 * the mathematical implementation inside the installed SDK, so the public
 * direct-controller example never depends on a repository tests directory.
 * This forwarding file provides no alternative constants or control law.
 */
#ifndef UWVM_UNITREE_TEST_CONTROLLER_FORWARD_H
#define UWVM_UNITREE_TEST_CONTROLLER_FORWARD_H
#include "../../sdk/robots/unitree/include/roswasm_unitree/simulation_control.h"
#endif
