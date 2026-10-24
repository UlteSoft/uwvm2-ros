#include <assert.h>
#include <string.h>
#include "application.h"
int main(void){
#if UWVM_PAL_ROS_VERSION==1
    uwvm_pal_current_limits value={0};
    value.actuator_names.size=1;value.actuator_names.data[0].size=4;
    memcpy(value.actuator_names.data[0].data,"head",4);
    value.current_limits.size=1;value.current_limits.data[0]=0.5;
    assert(!uwvm_pal_validate_current_limits(&value));
    value.current_limits.data[0]=1.001;
    assert(uwvm_pal_validate_current_limits(&value)==22);
    value.current_limits.data[0]=0;assert(!uwvm_pal_validate_current_limits(&value));
    value.current_limits.size=2;assert(uwvm_pal_validate_current_limits(&value)==22);
    value.actuator_names.size=2;value.actuator_names.data[1]=value.actuator_names.data[0];
    assert(uwvm_pal_validate_current_limits(&value)==22);
#else
    /* The ROS2 selected family lacks ActuatorCurrentLimit. It is intentionally
     * absent from this header; successful header compilation is the sole check. */
    assert(UWVM_PAL_ROS_VERSION==2);
#endif
    return 0;
}
