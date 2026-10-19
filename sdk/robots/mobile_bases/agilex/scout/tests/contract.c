/* SPDX-License-Identifier: Apache-2.0
 * Whole-value/boundary contracts, not an upstream suite or robot simulation.
 * Every original record receives valid, every-prefix-truncated and trailing
 * input cases. Populated original fields are independently checked below.
 */
#ifdef NDEBUG
#error "Contract assertions must remain enabled"
#endif
#include <assert.h>
#include <string.h>
#include "scout.h"
static unsigned char wire[1025],again[1025],sentinel[1025];
static void inventory(void) {
    unsigned records=0;
#define SCOUT_RECORD(Prefix) { \
    Prefix input={0},output={0},before;size_t length=0,other=0; \
    assert(UWVM_SCOUT_JOIN(Prefix,_encode)(&input,wire,1024,&length)==0); \
    memset(&output,0xa5,sizeof output);memcpy(&before,&output,sizeof output); \
    for(size_t short_size=0;short_size<length;++short_size) { \
        assert(UWVM_SCOUT_JOIN(Prefix,_decode)(wire,short_size,&output)!=0); \
        assert(!memcmp(&before,&output,sizeof output)); \
    } \
    wire[length]=0;assert(UWVM_SCOUT_JOIN(Prefix,_decode)(wire,length+1,&output)!=0); \
    assert(!memcmp(&before,&output,sizeof output)); \
    assert(UWVM_SCOUT_JOIN(Prefix,_decode)(wire,length,&output)==0); \
    assert(UWVM_SCOUT_JOIN(Prefix,_encode)(&output,again,1024,&other)==0&&other==length&&!memcmp(wire,again,length)); \
    memset(again,0x5a,sizeof again);memcpy(sentinel,again,sizeof again);other=777; \
    assert(length&&UWVM_SCOUT_JOIN(Prefix,_encode)(&input,again,length-1,&other)!=0); \
    assert(other==777&&!memcmp(again,sentinel,sizeof again));++records; \
}
#if UWVM_SCOUT_ROS_VERSION == 1
#include "records_ros1.inc"
    assert(records==17);
#else
#include "records_ros2.inc"
    assert(records==15);
#endif
#undef SCOUT_RECORD
}
static void full_status(void) {
    uwvm_scout_status input={0},output={0},before;size_t length=0;
#if UWVM_SCOUT_ROS_VERSION == 1
    input.header.seq=123;input.header.stamp.sec=987;input.header.stamp.nsec=123456789;
    input.lateral_velocity=-0.125;input.base_state=7;input.fault_code=0x1234;
    for(unsigned i=0;i<4;++i) {
        input.motor_states[i].current=1.25+i;input.motor_states[i].rpm=-120.5-i;
        input.motor_states[i].temperature=31.5+i;input.motor_states[i].motor_pose=-0.5-i;
        input.driver_states[i].driver_voltage=23.5+i;input.driver_states[i].driver_temperature=40.25+i;
        input.driver_states[i].driver_state=(uint8_t)(0x80+i);
    }
#else
    input.header.stamp.sec=-987;input.header.stamp.nanosec=123456789;
    input.vehicle_state=7;input.error_code=0x1234;
    for(unsigned i=0;i<4;++i) {
        input.actuator_states[i].rpm=(int16_t)(-120-i);input.actuator_states[i].current=1.25+i;
        input.actuator_states[i].driver_voltage=23.5f+i;input.actuator_states[i].driver_temperature=40.25f+i;
        input.actuator_states[i].motor_temperature=(int8_t)(-20+(int)i);
    }
#endif
    input.header.frame_id.size=5;memcpy(input.header.frame_id.data,"base0",5);
    input.linear_velocity=0.25;input.angular_velocity=-0.5;input.control_mode=3;input.battery_voltage=25.75;
    input.light_control_enabled=1;input.front_light_state.mode=3;input.front_light_state.custom_value=201;
    input.rear_light_state.mode=2;input.rear_light_state.custom_value=101;
    assert(UWVM_SCOUT_JOIN(UWVM_SCOUT_RECORD(ScoutStatus),_encode)(&input,wire,1024,&length)==0);
    assert(UWVM_SCOUT_JOIN(UWVM_SCOUT_RECORD(ScoutStatus),_decode)(wire,length,&output)==0);
    assert(output.header.frame_id.size==5&&!memcmp(output.header.frame_id.data,"base0",5));
    assert(output.linear_velocity==0.25&&output.angular_velocity==-0.5&&output.control_mode==3&&output.battery_voltage==25.75);
    assert(output.light_control_enabled==1&&output.front_light_state.mode==3&&output.front_light_state.custom_value==201);
    assert(output.rear_light_state.mode==2&&output.rear_light_state.custom_value==101);
#if UWVM_SCOUT_ROS_VERSION == 1
    assert(output.header.seq==123&&output.header.stamp.sec==987&&output.header.stamp.nsec==123456789);
    assert(output.lateral_velocity==-0.125&&output.base_state==7&&output.fault_code==0x1234);
    for(unsigned i=0;i<4;++i) {
        assert(output.motor_states[i].current==1.25+i&&output.motor_states[i].rpm==-120.5-i);
        assert(output.motor_states[i].temperature==31.5+i&&output.motor_states[i].motor_pose==-0.5-i);
        assert(output.driver_states[i].driver_voltage==23.5+i&&output.driver_states[i].driver_temperature==40.25+i);
        assert(output.driver_states[i].driver_state==0x80+i);
    }
#else
    assert(output.header.stamp.sec==-987&&output.header.stamp.nanosec==123456789);
    assert(output.vehicle_state==7&&output.error_code==0x1234);
    for(unsigned i=0;i<4;++i) {
        assert(output.actuator_states[i].rpm==-120-(int)i&&output.actuator_states[i].current==1.25+i);
        assert(output.actuator_states[i].driver_voltage==23.5f+i&&output.actuator_states[i].driver_temperature==40.25f+i);
        assert(output.actuator_states[i].motor_temperature==-20+(int)i);
    }
#endif
    /* The final five original fields are one Bool plus two two-byte lights. */
    memcpy(&before,&output,sizeof output);wire[length-5]=2;
    assert(UWVM_SCOUT_JOIN(UWVM_SCOUT_RECORD(ScoutStatus),_decode)(wire,length,&output)!=0);
    assert(!memcmp(&before,&output,sizeof output));
    input.header.frame_id.size=129;
    assert(UWVM_SCOUT_JOIN(UWVM_SCOUT_RECORD(ScoutStatus),_encode)(&input,wire,1024,&length)!=0);
    input.header.frame_id.size=128;memset(input.header.frame_id.data,'x',128);
    assert(UWVM_SCOUT_JOIN(UWVM_SCOUT_RECORD(ScoutStatus),_encode)(&input,wire,1024,&length)==0);
    input.header.frame_id.data[127]=0;
    assert(UWVM_SCOUT_JOIN(UWVM_SCOUT_RECORD(ScoutStatus),_encode)(&input,wire,1024,&length)!=0);
}
static void covariance(void) {
    uwvm_scout_odometry input={0},output={0};size_t length=0;
    input.header.frame_id.size=4;memcpy(input.header.frame_id.data,"odom",4);
    input.child_frame_id.size=4;memcpy(input.child_frame_id.data,"base",4);
    input.pose.pose.position.x=1.25;input.pose.pose.position.y=-2.5;input.pose.pose.position.z=3.75;
    input.pose.pose.orientation.x=0.125;input.pose.pose.orientation.y=0.25;
    input.pose.pose.orientation.z=0.5;input.pose.pose.orientation.w=1;
    input.twist.twist.linear.x=-0.25;input.twist.twist.linear.y=0.5;input.twist.twist.linear.z=0.75;
    input.twist.twist.angular.x=1.25;input.twist.twist.angular.y=-1.5;input.twist.twist.angular.z=2;
    for(unsigned i=0;i<36;++i){input.pose.covariance[i]=0.25+i;input.twist.covariance[i]=-0.5-i;}
    assert(UWVM_SCOUT_JOIN(UWVM_SCOUT_ODOMETRY,_encode)(&input,wire,1024,&length)==0);
    assert(UWVM_SCOUT_JOIN(UWVM_SCOUT_ODOMETRY,_decode)(wire,length,&output)==0);
    assert(output.header.frame_id.size==4&&!memcmp(output.header.frame_id.data,"odom",4));
    assert(output.child_frame_id.size==4&&!memcmp(output.child_frame_id.data,"base",4));
    assert(output.pose.pose.position.x==1.25&&output.pose.pose.position.y==-2.5&&output.pose.pose.position.z==3.75);
    assert(output.pose.pose.orientation.x==0.125&&output.pose.pose.orientation.y==0.25&&output.pose.pose.orientation.z==0.5&&output.pose.pose.orientation.w==1);
    assert(output.twist.twist.linear.x==-0.25&&output.twist.twist.linear.y==0.5&&output.twist.twist.linear.z==0.75);
    assert(output.twist.twist.angular.x==1.25&&output.twist.twist.angular.y==-1.5&&output.twist.twist.angular.z==2);
    for(unsigned i=0;i<36;++i)assert(output.pose.covariance[i]==0.25+i&&output.twist.covariance[i]==-0.5-i);
#if UWVM_SCOUT_ROS_VERSION == 2
    assert(roswasmc_bounded_2_nav_msgs_msg_Odometry_default(&output)==0&&output.pose.pose.orientation.w==1);
#endif
}
static void commands(void) {
    uwvm_scout_motion_limits limits={0};roswasmc_twist value={{7,8,9},{10,11,12}},before=value;size_t length=0;
    assert(uwvm_scout_make_motion(&limits,0,0,0,&value)==22&&!memcmp(&value,&before,sizeof value));
    limits.model=UWVM_SCOUT;limits.forward=0.5;limits.yaw=1;
    assert(uwvm_scout_make_motion(&limits,0.25,0,-0.5,&value)==0&&value.linear[0]==0.25&&value.angular[2]==-0.5);
    assert(value.linear[1]==0&&value.linear[2]==0&&value.angular[0]==0&&value.angular[1]==0);
    before=value;
    assert(uwvm_scout_make_motion(&limits,0,0.1,0,&value)==22&&!memcmp(&value,&before,sizeof value));
    assert(uwvm_scout_make_motion(&limits,0.75,0,0,&value)==22&&!memcmp(&value,&before,sizeof value));
    assert(uwvm_scout_make_motion(&limits,NAN,0,0,&value)==22&&!memcmp(&value,&before,sizeof value));
    limits.model=UWVM_SCOUT_MINI_OMNI;limits.lateral=0.25;
    assert(uwvm_scout_make_motion(&limits,-0.5,0.25,1,&value)==0&&value.linear[1]==0.25);
    limits.forward=INFINITY;before=value;
    assert(uwvm_scout_make_motion(&limits,0,0,0,&value)==22&&!memcmp(&value,&before,sizeof value));
    uwvm_scout_light light={0},prior={0};
    assert(uwvm_scout_make_light(1,3,201,2,101,&light)==0);
    assert(UWVM_SCOUT_JOIN(UWVM_SCOUT_RECORD(ScoutLightCmd),_encode)(&light,wire,1024,&length)==0&&length==5);
    const unsigned char exact[]={1,3,201,2,101};assert(!memcmp(wire,exact,5));prior=light;
    assert(uwvm_scout_make_light(1,4,0,0,0,&light)==22&&!memcmp(&light,&prior,sizeof light));
    assert(uwvm_scout_make_light(2,0,0,0,0,&light)==22&&!memcmp(&light,&prior,sizeof light));
}
static void backend_boundary(void) {
    /* Pure refusal fixture: no function pointer or native provider is supplied.
     * Reaching a transport would not constitute a passing backend check. */
    roswasmc_backend backend={0};backend.version=3-UWVM_SCOUT_ROS_VERSION;
    roswasmc_context context={0};context.backend=&backend;
    roswasmc_capability cap={0};cap.context=&context;
    uwvm_scout_storage storage={0};uwvm_scout_status value={0},before=value;
    uwvm_scout_light light={0};
    assert(!uwvm_scout_matches_backend(&cap));
    assert(uwvm_scout_poll_status(&cap,&value,&storage)==22&&!memcmp(&value,&before,sizeof value));
    assert(uwvm_scout_publish_light(&cap,&light,&storage)==22);
    assert(uwvm_scout_stop(&cap)==22&&uwvm_scout_stop(NULL)==22);
}
int main(void) {inventory();full_status();covariance();commands();backend_boundary();return 0;}
