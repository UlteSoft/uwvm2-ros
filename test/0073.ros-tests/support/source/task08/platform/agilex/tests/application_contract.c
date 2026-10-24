/* Authored pure application contracts; no robot, middleware or mock backend. */
#include <assert.h>
#include <string.h>
#include "application.h"
int main(void){
    uwvm_scout_status state={0};
    uwvm_scout_observation_requirement required={2,1,0,100,20};
    uwvm_scout_motion_limits limits={UWVM_SCOUT,0.2,0,0.3};
    roswasmc_twist out={{0,0,0},{0,0,0}},saved;
    state.control_mode=2;state.battery_voltage=24;
#if UWVM_SCOUT_ROS_VERSION==1
    state.base_state=1;
#else
    state.vehicle_state=1;
#endif
    assert(!uwvm_scout_validate_observation(&state,&required,1000,1100));
    assert(uwvm_scout_validate_observation(&state,&required,1000,1101)==22);
    assert(uwvm_scout_validate_observation(&state,&required,1000,999)==22);
    state.control_mode=3;
    assert(uwvm_scout_validate_observation(&state,&required,1000,1001)==125);
    state.control_mode=2;
#if UWVM_SCOUT_ROS_VERSION==1
    state.fault_code=4;
#else
    state.error_code=4;
#endif
    assert(uwvm_scout_validate_observation(&state,&required,1000,1001)==125);
    required.allowed_fault_bits=4;
    assert(!uwvm_scout_validate_observation(&state,&required,1000,1001));
    assert(!uwvm_scout_make_motion(&limits,0.2,0,-0.3,&out));
    saved=out;
    assert(uwvm_scout_make_motion(&limits,0.1,0.01,0,&out)==22);
    assert(!memcmp(&saved,&out,sizeof out));
    assert(uwvm_scout_make_motion(&limits,NAN,0,0,&out)==22);
    limits.model=UWVM_SCOUT_MINI_OMNI;limits.lateral=0.1;
    assert(!uwvm_scout_make_motion(&limits,0.1,0.1,0,&out));
    return 0;
}
