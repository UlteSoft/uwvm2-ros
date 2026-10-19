/** @file Real ROS1 Wasm port of the locked rospy subscriber-first echo case.
 *
 * This is one API adaptation, not the original Python unittest executable.
 * The owner must separately execute/discover the original locked upstream test.
 * It first proves the native listener exists WITHOUT a chatter publisher, then
 * releases this actual Worker. Ten owner-paced P tokens publish the same legal
 * upstream fixture value "hi [42]", 100 ms apart. R polls deliver a real native
 * echo through the actual ROS1 SDK import. No socket, clock, sleep, random,
 * filesystem or native pointer is imported by this Wasm module.
 *
 * Both authority modes use the SAME artifact: after the initial gate, '+' opens
 * exactly pub/sub while '-' requires both real alias-open errors13 and untouched
 * output slots. Default rights remain empty. F closes successful owners once.
 * Root must compile against actual current SDK headers with full -MD evidence,
 * validate the complete Wasm, and retain the original compiler/linker receipts.
 */
#include <roswasm/ros1.h>
#include <roswasm/node.h>

typedef struct port_iovec {void* data;size_t size;} port_iovec;
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_read")))
uint32_t port_read(uint32_t,port_iovec const*,uint32_t,uint32_t*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_write")))
uint32_t port_write(uint32_t,port_iovec const*,uint32_t,uint32_t*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("proc_exit"),noreturn))
void port_exit(uint32_t);

static uint32_t emit(char const* text,size_t length) {
    port_iovec vector={(void*)text,length};uint32_t count=UINT32_MAX;
    uint32_t code=port_write(1,&vector,1,&count);return code?code:count==length?0:22;
}
#define EMIT(text) emit(text,sizeof(text)-1)
static uint32_t token(char* output) {
    port_iovec vector={output,1};uint32_t count=UINT32_MAX;
    uint32_t code=port_read(0,&vector,1,&count);return code?code:count==1?0:22;
}
static uint32_t denied(char const* alias,size_t length) {
    unsigned char slot[8];size_t i;uint32_t code;
    for(i=0;i<8;++i) slot[i]=0xa5;
    code=uwvm_ros1_capability_open(0,uwvm_ros_address(alias),length,uwvm_ros_address(slot));
    if(code!=13) return 22;
    for(i=0;i<8;++i) if(slot[i]!=0xa5) return 22;
    return 0;
}
static uint32_t execute(void) {
    char control=0;uint32_t error;unsigned sent=0,polls=0,callbacks=0;
    roswasmc_node node;roswasmc_capability pub,sub;
    roswasmc_string label={"official-pubsub-order",21},pub_alias={"pub",3},sub_alias={"sub",3};
    roswasmc_string message={"hi [42]",7};unsigned char scratch[64],received[64];
    roswasmc_node_init(&node,roswasmc_ros1_backend(),label);
    roswasmc_capability_init(&pub,&node.context);roswasmc_capability_init(&sub,&node.context);
    error=EMIT("PORT_WORKER_GATE\n");if(!error) error=token(&control);
    if(error||control!='G') return error?error:22;
    if((error=token(&control))) return error;
    if(control=='-') {
        error=denied("pub",3);if(!error) error=denied("sub",3);
        if(!error) error=EMIT("PORT_WORKER_DENIED_13_13\n");
        if(!error) error=token(&control);return error?error:control=='F'?0:22;
    }
    if(control!='+') return 22;
    error=roswasmc_node_create_publisher(&node,&pub,pub_alias);
    if(!error) error=roswasmc_node_create_subscription(&node,&sub,sub_alias);
    if(!error) error=EMIT("PORT_WORKER_READY\n");
    while(!error) {
        error=token(&control);if(error) break;
        if(control=='P') {
            if(sent>=10) {error=22;break;}
            error=roswasmc_publish_string(&pub,message,scratch,sizeof scratch);++sent;
            if(!error) error=EMIT("PORT_WORKER_SENT\n");
        } else if(control=='R') {
            roswasmc_result result;roswasmc_string text;size_t i;
            if(sent!=10||++polls>2000||callbacks) {error=22;break;}
            result=roswasmc_poll(&sub,received,sizeof received);
            if(result.error==UWVM_ROS_AGAIN) {error=EMIT("PORT_WORKER_AGAIN\n");continue;}
            error=result.error;
            if(!error&&(!result.has_length||result.length>sizeof received)) error=22;
            if(!error) error=roswasmc_decode_string(received,(size_t)result.length,&text);
            if(!error&&text.size!=message.size) error=22;
            for(i=0;!error&&i<text.size;++i) if(text.data[i]!=message.data[i]) error=22;
            if(!error) {++callbacks;error=EMIT("{\"message\":\"hi [42]\"}\n");}
        } else if(control=='F') {
            if(sent!=10||callbacks!=1) error=22;
            break;
        } else {error=22;break;}
    }
    {roswasmc_cleanup_report cleanup=roswasmc_server_close(&pub,&sub,error,0);
     if(!error) error=cleanup.cleanup_error;}
    return error;
}
/** @brief Explicit standalone entry; no WASI CRT argument/environment queries. */
__attribute__((export_name("_start"),noreturn)) void _start(void) {port_exit(execute());}
