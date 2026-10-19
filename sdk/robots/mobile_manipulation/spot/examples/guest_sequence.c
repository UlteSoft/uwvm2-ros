/** Actual C11 WASI entry with no default target, credential or grant.
 * argv: STATE ACQUIRE RETAIN STAND VELOCITY FEEDBACK STOP RETURN.
 * Explicit FD0 input: token(u64LE),nonce(u64LE),x/y/yaw(f64LE),duration(u64LE),
 * then literal 'S'. This is operator input, not a Root signal/control channel.
 * Static values/buffers exist before any capability open; no default motion.
 */
#include <wasi/api.h>
#include <roswasm/ros2.h>
#include "ros_sequence.h"
static mm_spot_sequence_report report;
static mm_spot_buffers storage;
static int exact(unsigned char* p,size_t n) {while(n){__wasi_iovec_t v={p,n};__wasi_size_t used=0;
    if(__wasi_fd_read(0,&v,1,&used)||!used||used>n)return 0;p+=used;n-=used;}return 1;}
int main(int argc,char** argv) {
    unsigned char wire[49];uint64_t t,n,d,bits;double x,y,yaw;unsigned i;uint32_t e;
    roswasmc_context context={0};
    if(argc!=9||!exact(wire,sizeof wire)||wire[48]!='S')return 22;
    t=uwvm_ros_load_u64(wire);n=uwvm_ros_load_u64(wire+8);d=uwvm_ros_load_u64(wire+40);
    bits=uwvm_ros_load_u64(wire+16);memcpy(&x,&bits,8);bits=uwvm_ros_load_u64(wire+24);memcpy(&y,&bits,8);
    bits=uwvm_ros_load_u64(wire+32);memcpy(&yaw,&bits,8);
    context.backend=roswasmc_ros2_backend();
    e=mm_spot_sequence(&context,(char const* const*)(argv+1),t,n,x,y,yaw,d,&report,&storage);
    for(i=0;i<8;++i){if(!e&&report.attempted[i])e=report.transport[i]?report.transport[i]:report.provider[i];
        if(!e&&report.unknown[i])e=115;if(!e&&report.cleanup[i])e=report.cleanup[i];}
    return e?(int)(e<126?e:125):0;
}
