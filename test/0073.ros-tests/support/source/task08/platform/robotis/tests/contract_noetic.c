/* Whole original record contracts; authored tests, not the upstream suite.
 * Every truncation must refuse without committing a partial destination.
 */
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "state.h"
#define INIT_0(P,v) ((void)0)
#define INIT_1(P,v) assert(P##_default(&(v))==0)
#define CHECK(P,D) do {P a={0},out,before;unsigned char wire[32769];size_t n=0,i;INIT_##D(P,a); \
assert(P##_encode(&a,wire,32768,&n)==0);assert(P##_decode(wire,n,&out)==0); \
memset(&out,0x55,sizeof out);memcpy(&before,&out,sizeof out); \
for(i=0;i<n;++i){assert(P##_decode(wire,i,&out)!=0);assert(!memcmp(&out,&before,sizeof out));} \
wire[n]=0xff;assert(P##_decode(wire,n+1,&out)!=0);assert(!memcmp(&out,&before,sizeof out)); \
if(n){memset(wire,0x66,n);size_t used=99;assert(P##_encode(&a,wire,n-1,&used)!=0);for(i=0;i<n;++i)assert(wire[i]==0x66);} \
}while(0)
int main(void){
CHECK(roswasmc_bounded_1_geometry_msgs_Point,0);
CHECK(roswasmc_bounded_1_geometry_msgs_Quaternion,0);
CHECK(roswasmc_bounded_1_geometry_msgs_Pose,0);
CHECK(roswasmc_bounded_1_geometry_msgs_PoseWithCovariance,0);
CHECK(roswasmc_bounded_1_geometry_msgs_Vector3,0);
CHECK(roswasmc_bounded_1_geometry_msgs_Transform,0);
CHECK(roswasmc_bounded_1_time,0);
CHECK(roswasmc_bounded_1_std_msgs_Header,0);
CHECK(roswasmc_bounded_1_geometry_msgs_TransformStamped,0);
CHECK(roswasmc_bounded_1_geometry_msgs_Twist,0);
CHECK(roswasmc_bounded_1_geometry_msgs_TwistStamped,0);
CHECK(roswasmc_bounded_1_geometry_msgs_TwistWithCovariance,0);
CHECK(roswasmc_bounded_1_nav_msgs_Odometry,0);
CHECK(roswasmc_bounded_1_sensor_msgs_BatteryState,0);
CHECK(roswasmc_bounded_1_sensor_msgs_Imu,0);
CHECK(roswasmc_bounded_1_sensor_msgs_JointState,0);
CHECK(roswasmc_bounded_1_sensor_msgs_LaserScan,0);
CHECK(roswasmc_bounded_1_sensor_msgs_MagneticField,0);
CHECK(roswasmc_bounded_1_std_srvs_SetBoolRequest,0);
CHECK(roswasmc_bounded_1_std_srvs_SetBoolResponse,0);
CHECK(roswasmc_bounded_1_std_srvs_TriggerRequest,0);
CHECK(roswasmc_bounded_1_std_srvs_TriggerResponse,0);
CHECK(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal,0);
CHECK(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result,0);
CHECK(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback,0);
CHECK(roswasmc_bounded_1_turtlebot3_msgs_SensorState,0);
CHECK(roswasmc_bounded_1_turtlebot3_msgs_Sound,0);
CHECK(roswasmc_bounded_1_turtlebot3_msgs_VersionInfo,0);
uwvm_turtlebot3_sound value={0},saved;value.value=4;saved=value;
assert(uwvm_turtlebot3_make_sound(6,&value)==22&&value.value==saved.value);
assert(uwvm_turtlebot3_make_sound(UINT32_MAX,&value)==22&&value.value==saved.value);
assert(uwvm_turtlebot3_make_sound(1,0)==22);
for(uint32_t i=0;i<6;++i){assert(uwvm_turtlebot3_make_sound(i,&value)==0&&value.value==i);}
return 0;}
