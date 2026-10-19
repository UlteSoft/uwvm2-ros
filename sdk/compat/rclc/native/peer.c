/* Genuine installed rclc/rcl + generated std_msgs String peer source.
 * Build WITHOUT the opt-in adapter include path and WITHOUT its compat.c.
 * No native ROS implementation is stubbed. All native work remains UNEXECUTED.
 * Future owned runner must qualify SDK/full-MD/ELF/maps/current policy and bound
 * the whole process; SIGSTOP/I/M/P/F is a controller checkpoint, not authority. */
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/string.h>
#include <rosidl_runtime_c/string_functions.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static unsigned received,bad;
static void observe(void const*p){
    std_msgs__msg__String const*m=p;
    if(!m||m->data.size!=12||memcmp(m->data.data,"Hello World!",12))++bad;
    else ++received;
}
static int gate(int expected){
    int ch=getchar();
    if(ch!=expected)return 0;
    return getchar()=='\n';
}
int main(int argc,char const*argv[]){
    rcl_allocator_t a=rcl_get_default_allocator();
    rclc_support_t support;
    rcl_node_t node=rcl_get_zero_initialized_node();
    rcl_publisher_t pub=rcl_get_zero_initialized_publisher();
    rcl_subscription_t sub=rcl_get_zero_initialized_subscription();
    rclc_executor_t ex=rclc_executor_get_zero_initialized_executor();
    std_msgs__msg__String message={
        0
    }
    ;
    rcl_ret_t primary=0,cleanup=0,rc;
    int cleanup_codes[6]={
        -1,-1,-1,-1,-1,-1
    }
    ;
    int have_support=0,have_node=0,have_pub=0,have_sub=0,have_executor=0,have_message=0;
    unsigned tick;
    if(argc!=2)return 2;
    if(strcmp(argv[1],"publisher")&&strcmp(argv[1],"subscriber"))return 2;
    if((primary=rclc_support_init(&support,0,NULL,&a)))goto done;
    have_support=1;
    if((primary=rclc_node_init_default(&node,"native_rclc_peer","executor_examples",&support)))goto done;
    have_node=1;
    if(!std_msgs__msg__String__init(&message)){
        primary=RCL_RET_BAD_ALLOC;
        goto done;
    }
    have_message=1;
    if(!strcmp(argv[1],"publisher")){
        primary=rclc_publisher_init_default(&pub,&node,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String),"topic_0");
        if(primary)goto done;
        have_pub=1;
        if(!rosidl_runtime_c__String__assign(&message.data,"Hello World!")){
            primary=RCL_RET_BAD_ALLOC;
            goto done;
        }
    }
    else{
        primary=rclc_subscription_init_default(&sub,&node,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String),"topic_0");
        if(primary)goto done;
        have_sub=1;
        primary=rclc_executor_init(&ex,&support.context,1,&a);
        if(primary)goto done;
        have_executor=1;
        primary=rclc_executor_add_subscription(&ex,&sub,&message,observe,ON_NEW_DATA);
        if(primary)goto done;
        primary=rclc_executor_prepare(&ex);
        if(primary)goto done;
    }
    /* Installed middleware and full typesupport have been used before the first
  * stopped-image proof. I must be supplied only after that proof succeeds. */
    if(raise(SIGSTOP)||!gate('I')||!gate('M')){
        primary=RCL_RET_ERROR;
        goto done;
    }
    if(have_pub){
        if((primary=rcl_publish(&pub,&message,NULL)))goto done;
    }
    else for(tick=0;
    tick<200&&!received&&!bad;
    ++tick){
        rc=rclc_executor_spin_some(&ex,10000000);
        if(rc&&rc!=RCL_RET_TIMEOUT){
            primary=rc;
            goto done;
        }
    }
    if(!gate('P')){
        primary=RCL_RET_ERROR;
        goto done;
    }
    if(have_sub&&(!received||bad)){
        primary=RCL_RET_ERROR;
        goto done;
    }
    printf("OBSERVED_NATIVE_RCLC_STRING role=%s received=%u bad=%u\n",argv[1],received,bad);
    fflush(stdout);
    if(!gate('F'))primary=RCL_RET_ERROR;
    done:
    /* Every entered native owner receives an independent fini attempt. Keep the
  * primary result separate; no successful native delivery is inferred from rc0
  * until the future owner has validated this process's complete raw evidence. */
    if(have_executor){
        rc=rclc_executor_fini(&ex);
        cleanup_codes[0]=rc;
        if(rc&&!cleanup)cleanup=rc;
    }
    if(have_sub){
        rc=rcl_subscription_fini(&sub,&node);
        cleanup_codes[1]=rc;
        if(rc&&!cleanup)cleanup=rc;
    }
    if(have_pub){
        rc=rcl_publisher_fini(&pub,&node);
        cleanup_codes[2]=rc;
        if(rc&&!cleanup)cleanup=rc;
    }
    if(have_message){
        std_msgs__msg__String__fini(&message);
        cleanup_codes[3]=0;
    }
    if(have_node){
        rc=rcl_node_fini(&node);
        cleanup_codes[4]=rc;
        if(rc&&!cleanup)cleanup=rc;
    }
    if(have_support){
        rc=rclc_support_fini(&support);
        cleanup_codes[5]=rc;
        if(rc&&!cleanup)cleanup=rc;
    }
    fprintf(stderr,"NATIVE_RCLC_TERMINAL primary=%d cleanup=%d attempts=[%d,%d,%d,%d,%d,%d]\n",(int)primary,(int)cleanup,cleanup_codes[0],cleanup_codes[1],cleanup_codes[2],cleanup_codes[3],cleanup_codes[4],cleanup_codes[5]);
    return primary||cleanup?1:0;
}
