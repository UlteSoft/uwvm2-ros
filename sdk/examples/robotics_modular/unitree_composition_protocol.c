/** @file Real Runtime preload protocol regression, not a ROS physics result.
 *
 * Compile this root with no CRT/host imports and load the same separately
 * compiled callbacks, motion and perception modules as the Direct3 route.
 * Export composition_protocol and invoke its observed local function index.
 * A wrong real cross-module return traps. Exit success only verifies these
 * bounded scalar lifetimes; it does not qualify DDS, MuJoCo or performance.
 * Each provider must have all WASI/ROS contexts explicitly disabled.
 */
#include "unitree_modular_control.h"
static void check(int value){if(!value)__builtin_trap();}
UWVM_UM_EXTERN UWVM_UM_EXPORT("composition_protocol") void composition_protocol(void) {
    uint32_t words[36]={0},gen,next,checksum;
    uwvm_unitree_fixture_motor motors[20];
    check(!uwvm_unitree_perception_begin(0));check(uwvm_unitree_perception_status()==22);
    gen=uwvm_unitree_perception_begin(1);check(gen&&uwvm_unitree_perception_status()==0);
    check(!uwvm_unitree_perception_begin(2));check(uwvm_unitree_perception_status()==17);
    check(uwvm_unitree_perception_word(gen+1,0,0)==9);
    check(uwvm_unitree_perception_word(gen,1,0)==22);
    check(uwvm_unitree_perception_word(gen,0,UINT32_C(0x7f800000))==22);
    check(uwvm_unitree_perception_end(gen,0)==22);
    check(uwvm_unitree_perception_abort(gen+1)==9);
    check(uwvm_unitree_perception_abort(gen)==0);
    check(uwvm_unitree_perception_abort(gen)==9);
    next=uwvm_unitree_perception_begin(2);check(next>gen);checksum=uwvm_um_checksum_word(UINT32_C(2166136261),2);
    for(unsigned i=0;i<36;++i){check(uwvm_unitree_perception_word(next,i,0)==0);checksum=uwvm_um_checksum_word(checksum,0);}
    check(uwvm_unitree_perception_word(next,36,0)==22);
    check(uwvm_unitree_perception_end(next,checksum^1u)==22);
    check(uwvm_unitree_perception_end(next,checksum)==0);
    check(uwvm_unitree_perception_word(next,0,0)==9);
    check(uwvm_unitree_modular_observe(3,words)==0);
    check(!uwvm_unitree_motion_begin(0,1000000));check(uwvm_unitree_motion_status()==22);
    gen=uwvm_unitree_motion_begin(1,1000000);check(gen&&uwvm_unitree_motion_status()==0);
    check(!uwvm_unitree_motion_begin(2,1000000));check(uwvm_unitree_motion_status()==17);
    check(!uwvm_unitree_motion_word(gen+1,0,0));check(uwvm_unitree_motion_status()==9);
    check(uwvm_unitree_motion_end(gen)==0);check(uwvm_unitree_motion_end(gen)==9);
    check(uwvm_unitree_modular_compute(2,1000000,motors)==0);
    for(unsigned i=0;i<12;++i)check(motors[i].mode==1);
}
