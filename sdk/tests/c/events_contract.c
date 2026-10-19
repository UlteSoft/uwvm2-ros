/** @file
 * @brief Native C11 fake-dispatch/codec contract, not a ROS delivery witness.
 * This fixture exercises actual SDK functions with test-owned simulated handles;
 * no Runtime import, permission, ROS actor or external effect is fabricated.
 */
#include <roswasm/events.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
static unsigned calls;static int pending=1;static unsigned char record[72];
static void store64(unsigned char* p,uint64_t v){for(unsigned i=0;i<8;++i)p[i]=(unsigned char)(v>>(8*i));}
static void make_record(unsigned kind){
    memset(record,0,sizeof record);record[0]='U';record[1]='W';record[2]='R';record[3]='E';record[4]=1;record[6]=(unsigned char)kind;record[8]=2;
    store64(record+16,UINT64_C(0x9000000000000001));store64(record+32,1);store64(record+40,UINT64_C(0x9000000000000002));store64(record+48,UINT64_C(0x9000000000000003));
    if(kind>=2&&kind<=5)store64(record+24,17);if(kind==2||kind==4||kind==5){store64(record+56,8);store64(record+64,17);}
    if(kind==6)record[12]=110;if(kind==7)record[9]=1;
}
static uint32_t fake_watch(uint64_t observer,uint64_t source,uint64_t correlation){++calls;return observer==5&&source==9&&!correlation?0:13;}
static uint32_t fake_poll(uint64_t h,uint32_t m,uint64_t out,uint64_t capacity,uint64_t slot){
    ++calls;if(h!=5||m)return 13;if(!pending)return 11;store64((unsigned char*)(uintptr_t)slot,64);
    if(capacity<64)return 90;memcpy((void*)(uintptr_t)out,record,64);pending=0;return 0;
}
static uint32_t fake_ready(uint64_t h,uint32_t m,uint64_t out){++calls;if(h!=5||m)return 13;unsigned char* p=(unsigned char*)(uintptr_t)out;memset(p,0,32);store64(p,pending?1:0);store64(p+8,pending?64:0);return 0;}
static uint32_t fake_submit(uint64_t s,uint64_t o,uint32_t m,uint64_t input,uint64_t length,uint64_t slot){++calls;(void)input;(void)length;if(s!=9||o!=5||m)return 13;store64((unsigned char*)(uintptr_t)slot,UINT64_C(0x9000000000000021));return 0;}
int main(void){
    for(unsigned kind=1;kind<=7;++kind){make_record(kind);roswasmc_event_header h;uint64_t size=(kind==2||kind==4||kind==5)?72:64;assert(roswasmc_event_decode(record,size,&h)==0&&h.kind==kind&&h.source==UINT64_C(0x9000000000000001)&&h.instance==UINT64_C(0x9000000000000002));
        roswasmc_event_header sentinel=h;record[10]=1;assert(roswasmc_event_decode(record,size,&h)==22&&memcmp(&h,&sentinel,sizeof h)==0);record[10]=0;
        if(kind==2||kind==4||kind==5){record[64]=18;assert(roswasmc_event_decode(record,size,&h)==22);}}
    make_record(6);record[9]=2;roswasmc_event_header h;assert(roswasmc_event_decode(record,64,&h)==0&&h.flags==2);record[9]=4;assert(roswasmc_event_decode(record,64,&h)==22);
    roswasmc_backend base={0};base.version=2;roswasmc_context context={&base};roswasmc_capability observer={&context,5},source={&context,9};
    roswasmc_event_backend api={2,fake_watch,fake_poll,fake_ready,fake_submit};assert(roswasmc_event_watch(&api,&observer,&source,0)==0&&calls==1);
    roswasmc_event_backend wrong=api;wrong.version=1;assert(roswasmc_event_watch(&wrong,&observer,&source,0)==22&&calls==1);
    roswasmc_capability closed={&context,0};assert(roswasmc_event_watch(&api,&closed,&source,0)==9&&calls==1);
    make_record(1);unsigned char out[64];memset(out,0xa5,sizeof out);roswasmc_result result=roswasmc_event_poll(&api,&observer,out,63);assert(result.error==90&&result.has_length&&result.length==64&&pending==1&&out[0]==0xa5);
    result=roswasmc_event_poll(&api,&observer,out,64);assert(result.error==0&&result.length==64&&pending==0&&roswasmc_event_decode(out,64,&h)==0);
    result=roswasmc_event_poll(&api,&observer,out,64);assert(result.error==11&&!result.has_length);
    roswasmc_event_readiness ready;assert(roswasmc_event_ready(&api,&observer,&ready)==0&&ready.ready_events==0&&ready.next_event_bytes==0);
    uint64_t request=0;assert(roswasmc_service_submit(&api,&source,&observer,0,0,&request)==0&&request==UINT64_C(0x9000000000000021));
    unsigned before=calls;assert(roswasmc_service_submit(&api,&source,&observer,0,1,&request)==22&&calls==before);
    static char const marker[]="PASS_C_UNIFIED_EVENTS\n";assert(write(1,marker,sizeof marker-1)==(ssize_t)(sizeof marker-1));
}
