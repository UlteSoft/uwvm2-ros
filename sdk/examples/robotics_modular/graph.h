/** @file Main-owned graph lifecycle. Called only after preload instantiation.
 * A function import, not a copied function index or native callback pointer,
 * selects each library. Distinct module memories remain unshared.
 */
#ifndef ROSWASM_ROBOTICS_GRAPH_H
#define ROSWASM_ROBOTICS_GRAPH_H
#include "scalar_abi.h"
ROBOT_EXTERN ROBOT_IMPORT("callbacks","callback_begin") uint32_t robot_begin(uint32_t);
ROBOT_EXTERN ROBOT_IMPORT("callbacks","callback_end") uint32_t robot_end(uint32_t,uint32_t);
ROBOT_EXTERN ROBOT_IMPORT("callbacks","callback_count") uint32_t robot_count(uint32_t,uint32_t);
ROBOT_EXTERN ROBOT_IMPORT("callbacks","callback_denials") uint32_t robot_denial_count(void);
ROBOT_EXTERN ROBOT_IMPORT("motion","motion_step") int32_t robot_step(uint32_t,uint32_t,int32_t,int32_t);
ROBOT_EXTERN ROBOT_IMPORT("vision","vision_initialize") uint32_t robot_vision_initialize(void);
ROBOT_EXTERN ROBOT_IMPORT("vision","vision_clearance_mm") int32_t robot_clearance(int32_t,int32_t,int32_t,int32_t);
#ifndef ROBOT_EXPECT_CHAIN_DENIALS
#define ROBOT_EXPECT_CHAIN_DENIALS 0
#endif
/** Return the first error; every acquired registration is retired once. */
static uint32_t robot_graph(int32_t* decision) {
    uint32_t error = 0, closed, first, second;
    int32_t clearance, approved = ROBOT_INVALID;
    if(!decision) return 22;
    if(robot_vision_initialize()) return 22;
    clearance = robot_clearance(900,700,800,600);
    if(clearance != 600 || robot_clearance(-1,700,800,600) != ROBOT_INVALID) return 22;
    first = robot_begin(ROBOT_OWNER); if(!first) return 22;
    if(robot_begin(ROBOT_OWNER) != 0 || robot_begin(ROBOT_OWNER + 1) != 0) error = 22;
    if(!error) {approved = robot_step(ROBOT_OWNER,first,60,clearance); if(approved != 60) error = 22;}
    if(!error && robot_step(ROBOT_OWNER,first,60,300) != 0) error = 22;
    if(!error && robot_step(ROBOT_OWNER + 1,first,60,600) != ROBOT_STALE) error = 22;
    if(!error && robot_step(ROBOT_OWNER,first,101,600) != ROBOT_INVALID) error = 22;
    if(!error && robot_count(ROBOT_OWNER,first) != 2) error = 22;
    closed = robot_end(ROBOT_OWNER,first); if(!error) error = closed;
    if(error) return error;
    if(robot_step(ROBOT_OWNER,first,60,600) != ROBOT_STALE || robot_end(ROBOT_OWNER,first) != 9) return 22;
    second = robot_begin(ROBOT_OWNER); if(!second) return 22;
    if(second <= first || robot_step(ROBOT_OWNER,first,60,600) != ROBOT_STALE) error = 22;
    if(!error && robot_step(ROBOT_OWNER,second,-20,800) != -20) error = 22;
    if(!error && robot_end(ROBOT_OWNER,first) != 9) error = 22;
    if(!error && robot_count(ROBOT_OWNER,second) != 1) error = 22;
    closed = robot_end(ROBOT_OWNER,second); if(!error) error = closed;
    if(!error && robot_denial_count() != (ROBOT_EXPECT_CHAIN_DENIALS ? 3u : 0u)) error = 22;
    /* This pointer is in root main's own memory. It never crosses an import.
     * Commit the real imported computation only after checked graph cleanup. */
    if(!error) *decision = approved;
    return error;
}
#endif
