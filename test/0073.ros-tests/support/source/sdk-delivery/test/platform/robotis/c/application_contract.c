#include <assert.h>
#include "application.h"
int main(void){
    uwvm_turtlebot3_motion_profile profile={UWVM_TURTLEBOT3_BURGER,UWVM_TURTLEBOT3_TWIST,0.1,0.2};
    roswasmc_twist value={{0.1,0,0},{0,0,-0.2}};
    assert(!uwvm_turtlebot3_validate_motion(&profile,&value));
    value.linear[1]=0.01;assert(uwvm_turtlebot3_validate_motion(&profile,&value)==22);
    value.linear[1]=0;value.angular[2]=NAN;
    assert(uwvm_turtlebot3_validate_motion(&profile,&value)==22);
    value.angular[2]=0;profile.velocity_type=UWVM_TURTLEBOT3_TWIST_STAMPED;
#if UWVM_TURTLEBOT3_ROS_VERSION==1
    assert(uwvm_turtlebot3_validate_motion(&profile,&value)==95);
#else
    assert(!uwvm_turtlebot3_validate_motion(&profile,&value));
#endif
    profile.maximum_linear_mps=0;
    assert(uwvm_turtlebot3_validate_motion(&profile,&value)==22);
    return 0;
}
