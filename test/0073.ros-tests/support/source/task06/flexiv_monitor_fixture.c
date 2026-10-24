/* SPDX-License-Identifier: Apache-2.0
 * Native data-fixture producer for complete RobotStates application checks.
 * No ROS import, peer, publisher, successful transport stub or robot is used.
 */
#include "bounded_records.h"
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#ifndef TASK06_MONITOR_FORCE_MEMBER
#define TASK06_MONITOR_FORCE_MEMBER ext_wrench_in_tcp
#endif
static roswasmc_bounded_2_flexiv_msgs_msg_RobotStates value;
static unsigned char wire[ROSWASMC_BOUNDED_2_FLEXIV_MSGS_MSG_ROBOTSTATES_MAX_WIRE_BYTES+1];
int main(int argc,char** argv) {
    static double const q[7]={0.125,-0.0,1.0/3.0,-1.25,100.0,-100.0,2.345};
    size_t i,length=0,theta_offset;unsigned char prefix[4];uint32_t error;
    char const* mode=argc==2?argv[1]:"valid";
    if(roswasmc_bounded_2_flexiv_msgs_msg_RobotStates_default(&value))return 1;
    value.q.size=value.theta.size=7;
    for(i=0;i<7;++i){value.q.data[i]=q[i];value.theta.data[i]=1000.0+(double)i;}
    value.tcp_pose.pose.position.x=1.25;value.tcp_pose.pose.position.y=-2.5;value.tcp_pose.pose.position.z=0.125;
    value.tcp_pose.pose.orientation.w=0.5;value.tcp_pose.pose.orientation.x=0.1;
    value.tcp_pose.pose.orientation.y=-0.2;value.tcp_pose.pose.orientation.z=0.3;
    value.TASK06_MONITOR_FORCE_MEMBER.wrench.force.x=3.0;
    value.TASK06_MONITOR_FORCE_MEMBER.wrench.force.y=4.0;
    value.TASK06_MONITOR_FORCE_MEMBER.wrench.force.z=12.0;
    if(!strcmp(mode,"square-overflow"))value.TASK06_MONITOR_FORCE_MEMBER.wrench.force.x=DBL_MAX;
    else if(!strcmp(mode,"empty-q"))value.q.size=0;
    else if(strcmp(mode,"valid")&&strcmp(mode,"nan-unobserved")&&strcmp(mode,"trailing"))return 22;
    error=roswasmc_bounded_2_flexiv_msgs_msg_RobotStates_encode(&value,wire,sizeof wire,&length);
    if(error)return (int)error;
    if(!strcmp(mode,"nan-unobserved")) {
        theta_offset=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value.header)+8+4+value.q.size*8+4;
        if(theta_offset>length||length-theta_offset<8)return 22;
        uwvm_bounded_put(wire+theta_offset,UINT64_C(0x7ff8000000000001),8);
    }
    if(!strcmp(mode,"trailing"))wire[length++]=0xa5;
    uwvm_bounded_put(prefix,(uint64_t)length,4);
    if(fwrite(prefix,1,4,stdout)!=4||fwrite(wire,1,length,stdout)!=length||fflush(stdout))return 5;
    return 0;
}
