/** @file Root-only C SDK access after all imported library calls unwind.
 * Default mode proves local composition plus real default refusal; it requires
 * no live Gateway. Positive mode is selected at compile time and requires the
 * complete explicit host policy. Publication acknowledgement is not native
 * receipt. An independent participant and supervisor must check receipt.
 */
#include "graph.h"
#include "main_io.h"
#include "denial.h"
#include <roswasm/node.h>
#ifndef ROBOT_MODULAR_PUBLISH
#define ROBOT_MODULAR_PUBLISH 0
#endif
static uint32_t robot_main(void) {
    int32_t decision = ROBOT_INVALID;
    uint32_t error = robot_graph(&decision); if(error) return error;
    if(decision != 60) return UWVM_ROS_INVALID;
    if(ROBOT_ROS(abi_version)() != 1) return UWVM_ROS_UNSUPPORTED;
#if ROBOT_MODULAR_PUBLISH
    roswasmc_backend backend = *ROBOT_BACKEND(); roswasmc_node node;
    roswasmc_capability publisher; unsigned char scratch[64]; uint32_t closed;
    roswasmc_string label = {"modular-main",12}, alias = {"modular-out",11};
    roswasmc_string warmup = {"robotics-modular:warmup",sizeof("robotics-modular:warmup")-1};
    roswasmc_string result = {"robotics-modular:command=60",sizeof("robotics-modular:command=60")-1};
    roswasmc_node_init(&node,&backend,label);
    error = roswasmc_node_create_publisher(&node,&publisher,alias);
    if(error) return error;
    error = roswasmc_publish_string(&publisher,warmup,scratch,sizeof scratch);
    if(!error) error = ROBOT_MARKER("MODULAR_ENDPOINT_CREATED\n");
    if(!error) error = robot_gate(); /* P only after independent native match. */
    if(!error) error = roswasmc_publish_string(&publisher,result,scratch,sizeof scratch);
    closed = roswasmc_close(&publisher); if(!error) error = closed;
    if(!error) error = ROBOT_MARKER("MODULAR_PUBLISHED\n");
    return error;
#else
    error = robot_denied_open();
    if(!error) error = ROBOT_MARKER("PASS_MODULAR_DEFAULT_DENIED\n");
    return error;
#endif
}
ROBOT_EXTERN ROBOT_EXPORT("_start") __attribute__((noreturn)) void _start(void) {robot_exit(robot_main());}
