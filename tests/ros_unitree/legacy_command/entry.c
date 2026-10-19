/** Same-module command entry for genuine C/C++/Rust ROS1 legacy applications.
 * This TU supplies only finite basic stdio framing; all ROS/typed operations
 * call the actual linked language application in THIS main Wasm module.
 * No WAT positive wrapper, preload, native ROS handle, timer or retry exists.
 * Setup16B ULG1/family4/mode0or1/reserved0 is data, never authentication.
 */
#include <stdint.h>
#include <stddef.h>
#if !defined(__wasm__) || UINTPTR_MAX != UINT32_MAX
#error "Actual wasm32 compiler/pointer ABI required"
#endif
struct vector {uint32_t address,length;};
_Static_assert(sizeof(struct vector)==8,"Actual WASIp1 wasm32 iovec");
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_read")))
extern uint32_t read_fd(uint32_t,struct vector const*,uint32_t,uint32_t*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_write")))
extern uint32_t write_fd(uint32_t,struct vector const*,uint32_t,uint32_t*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("proc_exit"),noreturn))
extern void exit_process(uint32_t);
extern uint32_t initialize_case(uint32_t,uint32_t,uint32_t),poll_once(void),submit_once(void),close_case(void);
extern uint32_t case_status(void),received_count(void),denied_count(void),observed_length(void),observed_byte(uint32_t);
static unsigned char incoming[16];static char outgoing[4096];static uint32_t used,opened;
__attribute__((noreturn)) static void fail(uint32_t code){if(opened)(void)close_case();exit_process(code?code:22);}
static void check(int ok){if(!ok)fail(22);}
static void input(uint32_t n){
    uint32_t at=0,attempts=0;check(n>0&&n<=16);
    while(at<n&&attempts++<32){struct vector v={(uint32_t)(uintptr_t)(incoming+at),n-at};uint32_t got=0,status=read_fd(0,&v,1,&got);
        if(status==27)continue;if(status)fail(status);check(got>0&&got<=n-at);at+=got;}
    if(at!=n)fail(110);
}
static void append(char const* s){for(uint32_t i=0;s[i];++i){check(used<sizeof outgoing);outgoing[used++]=s[i];}}
static void number(uint32_t n){char b[10];unsigned size=0;do{b[size++]=(char)('0'+n%10);n/=10;}while(n);while(size){check(used<sizeof outgoing);outgoing[used++]=b[--size];}}
static void flush(void){
    uint32_t at=0,attempts=0;while(at<used&&attempts++<4096){struct vector v={(uint32_t)(uintptr_t)(outgoing+at),used-at};uint32_t got=0,status=write_fd(1,&v,1,&got);
        if(status==27)continue;if(status)fail(status);check(got>0&&got<=used-at);at+=got;}
    if(at!=used)fail(110);used=0;
}
static void line(char const* s){append(s);flush();}
static uint32_t le32(unsigned at){return(uint32_t)incoming[at]|(uint32_t)incoming[at+1]<<8|(uint32_t)incoming[at+2]<<16|(uint32_t)incoming[at+3]<<24;}
static void control(unsigned char expected){input(1);check(incoming[0]==expected);}
__attribute__((export_name("_start"),noreturn)) void _start(void){
    line("READY_LEGACY_MAIN\n");input(16);check(incoming[0]=='U'&&incoming[1]=='L'&&incoming[2]=='G'&&incoming[3]=='1'&&le32(4)==4&&le32(8)<=1&&le32(12)==0);
    uint32_t mode=le32(8);opened=1;uint32_t code=initialize_case(mode,4,0);if(code)fail(code);
    check(case_status()==0&&received_count()==0&&denied_count()==(mode?0u:2u));
    append("LEGACY_INITIALIZED ");number(mode);line("\n");
    if(!mode){check(poll_once()==9&&submit_once()==9&&close_case()==9);line("LEGACY_DENIED 2\n");control('F');}
    else{
        for(unsigned polls=0;;++polls){if(polls>=4096)fail(110);control('R');code=poll_once();if(code==11){line("LEGACY_AGAIN\n");continue;}if(code)fail(code);break;}
        check(case_status()==0&&received_count()==1&&denied_count()==0);uint32_t n=observed_length();check(n>0&&n<=1536);
        append("LEGACY_GUEST_STATE ");number(n);append(" ");char const digits[]="0123456789abcdef";
        for(uint32_t i=0;i<n;++i){uint32_t b=observed_byte(i);check(b<=255&&case_status()==0&&used+2<=sizeof outgoing);outgoing[used++]=digits[b>>4];outgoing[used++]=digits[b&15];}line("\n");
        control('S');code=submit_once();if(code)fail(code);check(submit_once()==9);line("LEGACY_SUBMITTED\n");
        control('C');code=close_case();if(code)fail(code);check(close_case()==9&&poll_once()==9&&submit_once()==9);line("LEGACY_CLOSED\n");control('F');
    }
    opened=0;line("PASS_LEGACY_CASE\n");exit_process(0);
}
