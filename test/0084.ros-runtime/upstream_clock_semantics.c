/** @file
 * @brief Execute actual upstream Jazzy RCL/RMW/rcutils time implementations.
 * @details The same fixture is linked natively and as Wasm. Dynamic physical
 * timestamps are compared by invariants; duration/normalization, ROS override,
 * jump callback order and argument errors produce an identical transcript.
 * This qualifies only the selected source files, not packages or ROS graph I/O.
 */
#include <rcl/time.h>
#include <rcl/error_handling.h>
#include <rcutils/time.h>
#include <rmw/time.h>
#include <stdint.h>
#include <string.h>
#ifdef __wasm__
struct report_iovec{const char* data;uint32_t size;};
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_write")))
uint32_t report_write(uint32_t,const struct report_iovec*,uint32_t,uint32_t*);
#else
#include <unistd.h>
#endif
static unsigned checks;
static char report[8192];static unsigned used;
static void check(int condition)
{
    ++checks;
    if(!condition)
    {
        char failure[]="failed-check:0000\n";
        for(unsigned index=0,value=checks;index!=4;++index,value/=10)failure[16-index]=(char)('0'+value%10);
#ifdef __wasm__
        struct report_iovec buffer={failure,sizeof(failure)-1};uint32_t written=0;
        (void)report_write(1,&buffer,1,&written);
#else
        (void)write(1,failure,sizeof(failure)-1);
#endif
        __builtin_trap();
    }
}
static void text(const char* value){while(*value){if(used==sizeof(report))__builtin_trap();report[used++]=*value++;}}
static void number(int64_t value)
{
    char digits[20];unsigned count=0;uint64_t magnitude=(uint64_t)value;
    if(value<0){text("-");magnitude=0-magnitude;}
    do{digits[count++]=(char)('0'+magnitude%10);magnitude/=10;}while(magnitude);
    while(count){char value[2]={digits[--count],0};text(value);}
}
static void emit(void)
{
#ifdef __wasm__
    struct report_iovec buffer={report,used};uint32_t written=0;
    check(report_write(1,&buffer,1,&written)==0&&written==used);
#else
    check(write(1,report,used)==(ssize_t)used);
#endif
}
static unsigned before_count,after_count;static int callback_order;
static void jumped(const rcl_time_jump_t* event,bool before,void* context)
{
    check(context==&checks);
    if(before){check(!callback_order);callback_order=1;++before_count;}
    else{check(callback_order);callback_order=0;++after_count;}
    text(before?"before:":"after:");number(event->clock_change);text(":");number(event->delta.nanoseconds);text("\n");
}
static void run(void)
{
    rcl_allocator_t allocator=rcl_get_default_allocator();rcl_clock_t steady={0},system={0},ros={0};
    check(rcl_clock_init(RCL_STEADY_TIME,&steady,&allocator)==RCL_RET_OK);
    check(rcl_clock_init(RCL_SYSTEM_TIME,&system,&allocator)==RCL_RET_OK);
    int64_t first=-1,next=-1;
#ifdef EXPECT_CLOCK_DENIAL
    // Jazzy time.c directly forwards rcutils' error code (2), which differs
    // from RCL_RET_ERROR (1). Preserve this upstream behavior on refusal.
    check(rcl_clock_get_now(&steady,&first)==RCUTILS_RET_ERROR);
    check(first==-1);
    check(rcutils_error_is_set());rcutils_reset_error();
    check(rcl_clock_get_now(&system,&next)==RCUTILS_RET_ERROR);
    check(next==-1);
    check(rcutils_error_is_set());rcutils_reset_error();
    text("upstream-physical-clock-denial-preserved\n");
#else
    check(rcl_clock_get_now(&steady,&first)==RCL_RET_OK&&first>0);
    for(unsigned index=0;index!=128;++index)
    {check(rcl_clock_get_now(&steady,&next)==RCL_RET_OK&&next>=first);first=next;}
    check(rcl_clock_get_now(&system,&first)==RCL_RET_OK&&first>1700000000000000000LL);
    check(rcutils_steady_time_now(NULL)==RCUTILS_RET_INVALID_ARGUMENT);rcutils_reset_error();
    check(rcl_clock_get_now(NULL,&first)==RCL_RET_INVALID_ARGUMENT);rcutils_reset_error();
    check(rcl_clock_get_now(&steady,NULL)==RCL_RET_INVALID_ARGUMENT);rcutils_reset_error();
    text("physical-clock-invariants:130\n");
    check(rcl_clock_init(RCL_ROS_TIME,&ros,&allocator)==RCL_RET_OK);
    check(rcl_clock_get_now(&ros,&first)==RCL_RET_OK&&first>1700000000000000000LL);
    rcl_jump_threshold_t threshold={true,{10},{-5}};
    check(rcl_clock_add_jump_callback(&ros,threshold,jumped,&checks)==RCL_RET_OK);
    check(rcl_clock_add_jump_callback(&ros,threshold,jumped,&checks)==RCL_RET_ERROR);rcutils_reset_error();
    check(rcl_enable_ros_time_override(&ros)==RCL_RET_OK);
    check(!rcl_clock_time_started(&ros));
    bool active=false;check(rcl_is_enabled_ros_time_override(&ros,&active)==RCL_RET_OK&&active);
    const int64_t values[]={100,103,120,111,110,0,2000000001};
    for(unsigned index=0;index!=sizeof(values)/sizeof(*values);++index)
    {
        check(rcl_set_ros_time_override(&ros,values[index])==RCL_RET_OK);
        check(rcl_clock_get_now(&ros,&first)==RCL_RET_OK&&first==values[index]);
        text("ros:");number(first);text("\n");
    }
    check(rcl_clock_time_started(&ros));
    check(rcl_disable_ros_time_override(&ros)==RCL_RET_OK);
    check(rcl_clock_get_now(&ros,&first)==RCL_RET_OK&&first>1700000000000000000LL);
    check(before_count==7&&after_count==7&&!callback_order);
    check(rcl_clock_remove_jump_callback(&ros,jumped,&checks)==RCL_RET_OK);
    check(rcl_clock_remove_jump_callback(&ros,jumped,&checks)==RCL_RET_ERROR);rcutils_reset_error();
    check(rcl_clock_fini(&ros)==RCL_RET_OK);
    rcl_time_point_t start={.nanoseconds=42,.clock_type=RCL_ROS_TIME};
    rcl_time_point_t finish={.nanoseconds=100,.clock_type=RCL_ROS_TIME};rcl_duration_t difference;
    check(rcl_difference_times(&start,&finish,&difference)==RCL_RET_OK&&difference.nanoseconds==58);
    check(rcl_difference_times(&finish,&start,&difference)==RCL_RET_OK&&difference.nanoseconds==-58);
    finish.clock_type=RCL_SYSTEM_TIME;
    check(rcl_difference_times(&start,&finish,&difference)==RCL_RET_ERROR);rcutils_reset_error();
    const rmw_time_t times[]={{0,0},{0,1000000001},{2,999999999},{UINT64_MAX,UINT64_MAX},
        {9223372036ULL,854775807},{9223372036ULL,854775808}};
    const int64_t expected[]={0,1000000001,2999999999,INT64_MAX,INT64_MAX,INT64_MAX};
    for(unsigned index=0;index!=sizeof(times)/sizeof(*times);++index)
    {
        int64_t value=rmw_time_total_nsec(times[index]);check(value==expected[index]);
        rmw_time_t normalized=rmw_time_normalize(times[index]);check(normalized.nsec<1000000000);
        check(rmw_time_equal(times[index],normalized));
        text("rmw:");number(value);text(":");number(normalized.sec);text(":");number(normalized.nsec);text("\n");
    }
    check(rmw_time_total_nsec(rmw_time_from_nsec(-1))==INT64_MAX);
#endif
    check(rcl_clock_fini(&steady)==RCL_RET_OK);check(rcl_clock_fini(&system)==RCL_RET_OK);
    text("checks:");number(checks);text("\n");emit();
}
#ifdef __wasm__
void _start(void){run();}
#else
int main(void){run();}
#endif
