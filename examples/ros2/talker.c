#include "../../sdk/ros2/abi.h"
__attribute__((import_module("wasi_snapshot_preview1"), import_name("proc_exit")))
void guest_exit(uint32_t);
__attribute__((import_module("wasi_snapshot_preview1"), import_name("clock_time_get")))
uint32_t guest_clock(uint32_t,uint64_t,uint32_t);
static void check(uint32_t error) { if(error) guest_exit(error); }
static void delay(void) {
    unsigned char start[8],current[8];
    check(guest_clock(1,1,(uint32_t)(uintptr_t)start));
    do { check(guest_clock(1,1,(uint32_t)(uintptr_t)current)); }
    while(uwvm_ros_load_u64(current)-uwvm_ros_load_u64(start)<1000000000ULL);
}
void _start(void) {
    uwvm_ros_handle publisher;
    unsigned char value[]={5,0,0,0,'h','e','l','l','o'};
    check(uwvm_ros2_open("pub",3,&publisher));
    check(uwvm_ros2_publish(publisher,value,sizeof value)); delay();
    check(uwvm_ros2_publish(publisher,value,sizeof value)); delay();
    check(uwvm_ros2_capability_close(publisher));
}
