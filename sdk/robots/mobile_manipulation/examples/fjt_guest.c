/** Actual WASI Preview1 C11 entry, no fake host/SDK producer.
 * Arguments: gen3-6dof|gen3-7dof|stretch-lift TELEMETRY SEND FEEDBACK RESULT CANCEL
 * stdin: exactly sizeof(double)*joint_count little-endian IEEE754 bytes,
 * then caller credits T,S,F/R/C (one actual fd_read per credit). Values are
 * operator-selected and canonical finite validation precedes every command.
 * FD0 is explicitly granted input, never a management/signal/clock channel.
 */
#include <wasi/api.h>
#include <roswasm/ros2.h>
#include "fjt_lesson.h"
static mm_fjt_goal goal;
static mm_fjt_report report;
static int read_exact(unsigned char* p,size_t length) {
    while(length) {__wasi_iovec_t item={p,length};__wasi_size_t n=0;
        __wasi_errno_t e=__wasi_fd_read(0,&item,1,&n);if(e||!n||n>length)return 0;p+=n;length-=n;}
    return 1;
}
static int credit(void* unused) {unsigned char value;(void)unused;return read_exact(&value,1)?value:-1;}
int main(int argc,char** argv) {
    double positions[7];unsigned char raw[8];size_t i,count;uint64_t bits;uint32_t e;
    roswasmc_context context={0};
    if(argc!=7)return 22;
    count=!strcmp(argv[1],"gen3-6dof")?6:!strcmp(argv[1],"gen3-7dof")?7:!strcmp(argv[1],"stretch-lift")?1:0;
    if(!count)return 22;
    for(i=0;i<count;++i){if(!read_exact(raw,8))return 22;bits=uwvm_ros_load_u64(raw);memcpy(&positions[i],&bits,8);}
    e=count==1?mm_make_stretch_lift_goal(&goal,positions[0]):mm_make_gen3_goal(&goal,positions,count);
    if(e)return (int)e;
    context.backend=roswasmc_ros2_backend(); /* dispatch only; opens/grants nothing */
    e=mm_fjt_lesson(&context,(char const* const*)(argv+2),&goal,credit,0,&report);
    for(i=0;i<5;++i)if(!e&&report.cleanup[i])e=report.cleanup[i];
    return e?(int)(e<126?e:125):0;
}
