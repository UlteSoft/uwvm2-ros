#include "../../sdk/ros1/abi.h"
__attribute__((import_module("wasi_snapshot_preview1"), import_name("proc_exit")))
void guest_exit(uint32_t);
__attribute__((import_module("wasi_snapshot_preview1"), import_name("clock_time_get")))
uint32_t guest_clock(uint32_t,uint64_t,uint32_t);
static void check(uint32_t error) { if(error) guest_exit(error); }
void _start(void) {
    uwvm_ros_handle subscriber; unsigned char value[4096],start[8],current[8]; uint64_t length=0;
    check(uwvm_ros1_open("sub",3,&subscriber)); check(uwvm_ros1_topic_subscribe(subscriber));
    check(guest_clock(1,1,(uint32_t)(uintptr_t)start));
    for(;;) {
        uint32_t error=uwvm_ros1_poll(subscriber,value,sizeof value,&length);
        if(!error) break;
        if(error!=UWVM_ROS_AGAIN) guest_exit(error);
        check(guest_clock(1,1,(uint32_t)(uintptr_t)current));
        if(uwvm_ros_load_u64(current)-uwvm_ros_load_u64(start)>10000000000ULL) guest_exit(UWVM_ROS_TIMEOUT);
    }
    if(length!=9 || value[0]!=5 || value[1] || value[2] || value[3] || value[4]!='h' || value[5]!='e' || value[6]!='l' || value[7]!='l' || value[8]!='o') guest_exit(201);
    check(uwvm_ros1_capability_close(subscriber));
}
