/** @file Same-main stdio controller for genuine typed Unitree Guest examples.
 *
 * This test TU links INTO the actual C/C++/Rust/embedded-JavaScript module.
 * Every initialize/poll/getter/close below is the real example implementation
 * in that SAME Wasm module. It never implements ROS, codecs, native handles or
 * a permission bridge. The Runtime's cross-module ROS denial stays unchanged.
 *
 * Compile explicit UWVM_TYPED_FAMILY=1(GO),2(HG),3(API) and
 * UWVM_TYPED_JAVASCRIPT=0|1. Only fd_read/fd_write/proc_exit are introduced.
 * Standard pipes are owner-supplied; no optional WASI right is needed. The
 * driver must independently validate the actual import table, one bounded
 * memory, no implicit start and the selected _start export before execution.
 *
 * READY is emitted before any language/SDK/ROS initialization. The trusted
 * parent then sends16 bytes: UTM1, family:u32le, mode:u32le(0|1), reserved0.
 * This selects behavior, never grants authority. Mode0 requires denial13 from
 * the actual example. Mode1 requires exact external main-principal aliases.
 * R polls once (at most4096), S submits API once, C closes, F finishes. The
 * independent native UTN1 witness and process owner remain outside this TU.
 */
#include <stdint.h>
#include <stddef.h>
#if !defined(__wasm__) || UINTPTR_MAX != UINT32_MAX
#error "The genuine wasm32 compiler and pointer ABI are required"
#endif
#if !defined(UWVM_TYPED_FAMILY) || UWVM_TYPED_FAMILY < 1 || UWVM_TYPED_FAMILY > 3
#error "Select exactly one real GO/HG/API family"
#endif
#if !defined(UWVM_TYPED_JAVASCRIPT) || (UWVM_TYPED_JAVASCRIPT != 0 && UWVM_TYPED_JAVASCRIPT != 1)
#error "Select whether this is the actual embedded JavaScript module"
#endif
struct vector { uint32_t address,length; };
_Static_assert(sizeof(struct vector)==8,"actual WASIp1 wasm32 iovec");
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_read")))
extern uint32_t fd_read(uint32_t,struct vector const*,uint32_t,uint32_t*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_write")))
extern uint32_t fd_write(uint32_t,struct vector const*,uint32_t,uint32_t*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("proc_exit"),noreturn))
extern void exit_process(uint32_t);
extern uint32_t initialize_case(uint32_t,uint32_t,uint32_t);
extern uint32_t poll_once(void),case_status(void),received_count(void),denied_verified(void),close_case(void);
#if UWVM_TYPED_FAMILY == 3
extern uint32_t submit_once(void),response_code(void);
#else
extern uint32_t observed_tick(void),observed_crc(void),motor_word(uint32_t,uint32_t);
#endif
#if UWVM_TYPED_JAVASCRIPT
extern uint32_t roswasm_js_init(void),roswasm_js_status(void),roswasm_js_close(void);
#endif
static uint32_t opened,engine;
static char output[4096];
static uint32_t used;
/** Failure preserves the original status and attempts ordinary SDK cleanup.
 * No cleanup success can convert a failed operation into process success. */
__attribute__((noreturn)) static void fail(uint32_t code) {
    if(opened)(void)close_case();
#if UWVM_TYPED_JAVASCRIPT
    if(engine)(void)roswasm_js_close();
#endif
    exit_process(code?code:22);
}
static void require(int condition) {if(!condition)fail(22);}
/** Transfer bounded bytes. Short progress and bounded WASIp1 INTR retries are
 * accepted; EOF, EAGAIN and zero/oversized progress are terminal. No ROS retry. */
static void input(void* pointer,uint32_t length) {
    uint32_t at=0,attempts=0;require(length>0&&length<=16);
    while(at<length&&attempts++<32) {
        struct vector part={(uint32_t)(uintptr_t)((uint8_t*)pointer+at),length-at};
        uint32_t done=0,code=fd_read(0,&part,1,&done);
        if(code==27)continue;if(code)fail(code);
        require(done>0&&done<=length-at);at+=done;
    }
    if(at!=length)fail(110);
}
static void append(char const* value) {
    for(uint32_t n=0;value[n];++n){require(used<sizeof output);output[used++]=value[n];}
}
static void number(uint32_t value) {
    char digits[10];uint32_t n=0;
    do {digits[n++]=(char)('0'+value%10);value/=10;}while(value);
    while(n){require(used<sizeof output);output[used++]=digits[--n];}
}
static void flush(void) {
    uint32_t at=0,attempts=0;
    while(at<used&&attempts++<4096) {
        struct vector part={(uint32_t)(uintptr_t)(output+at),used-at};
        uint32_t done=0,code=fd_write(1,&part,1,&done);
        if(code==27)continue;if(code)fail(code);
        require(done>0&&done<=used-at);at+=done;
    }
    if(at!=used)fail(110);used=0;
}
static void line(char const* value){append(value);flush();}
static uint32_t le32(uint8_t const* p){return (uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;}
static uint8_t control(void){uint8_t byte;input(&byte,1);return byte;}
/** Actual process entry. All scratch bytes are ordinary compiler-owned static
 * or stack objects in this module; no guessed free linear-memory address. */
__attribute__((export_name("_start"),noreturn)) void _start(void) {
    uint8_t header[16];uint32_t mode,polls=0;
    line("READY_TYPED_MAIN\n");input(header,sizeof header);
    require(header[0]=='U'&&header[1]=='T'&&header[2]=='M'&&header[3]=='1');
    require(le32(header+4)==UWVM_TYPED_FAMILY&&le32(header+8)<=1&&le32(header+12)==0);mode=le32(header+8);
#if UWVM_TYPED_JAVASCRIPT
    engine=1;uint32_t code=roswasm_js_init();if(code)fail(code);
    if((code=roswasm_js_status())!=0)fail(code);
#endif
    uint32_t result=initialize_case(mode,UWVM_TYPED_FAMILY==3?1:UWVM_TYPED_FAMILY,UWVM_TYPED_FAMILY==3?1:0);opened=1;
    if(result)fail(result);require(case_status()==0&&received_count()==0&&denied_verified()==!mode);
    append("TYPED_INITIALIZED ");number(mode);append("\n");flush();
    if(!mode) {
        require(poll_once()==9&&close_case()==9);
#if UWVM_TYPED_FAMILY == 3
        require(submit_once()==9);
#endif
        line("TYPED_DENIED_VERIFIED\n");require(control()=='F');
    } else {
#if UWVM_TYPED_FAMILY == 3
        require(control()=='S');result=submit_once();if(result)fail(result);
        require(submit_once()==9);line("TYPED_SUBMITTED\n");
#endif
        for(;;) {
            require(control()=='R');if(polls++>=4096)fail(110);result=poll_once();
            if(result==11){line("TYPED_AGAIN\n");continue;}
            if(result)fail(result);require(case_status()==0&&received_count()==1&&denied_verified()==0);break;
        }
#if UWVM_TYPED_FAMILY == 3
        append("TYPED_RPC ");number(response_code());append(" 1 0\n");flush();
#else
        uint32_t slots=UWVM_TYPED_FAMILY==1?20:35;
        append("TYPED_STATE ");number(observed_tick());append(" ");number(observed_crc());append(" ");number(slots);
        for(uint32_t slot=0;slot<slots;++slot)for(uint32_t field=0;field<3;++field){append(" ");number(motor_word(slot,field));}
        append("\n");flush();require(case_status()==0);
#endif
        require(control()=='C');result=close_case();if(result)fail(result);
        require(close_case()==9&&poll_once()==9);
#if UWVM_TYPED_FAMILY == 3
        require(submit_once()==9);
#endif
        line("TYPED_CLOSED\n");require(control()=='F');
    }
    opened=0;
#if UWVM_TYPED_JAVASCRIPT
    result=roswasm_js_close();engine=0;if(result)fail(result);
#endif
    line("PASS_TYPED_CASE\n");exit_process(0);
}
