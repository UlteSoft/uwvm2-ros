/** @file
 * @brief Actual allocation-free event dispatcher over finite test-owned imports.
 * Native model only: no Gateway, ROS permission or middleware delivery is claimed.
 * Compile with -UNDEBUG. This source has not been executed by its author.
 */
#include <roswasm/events.h>
#include <assert.h>
#include <string.h>
static unsigned char frame[64];static unsigned polls,callbacks,closes;static int idle;
static roswasmc_event_subscription subscription;
static void put(unsigned char* p,uint64_t n){for(unsigned i=0;i<8;++i)p[i]=(unsigned char)(n>>(8*i));}
static void record(uint64_t sequence){memset(frame,0,64);memcpy(frame,"UWRE",4);frame[4]=1;frame[6]=1;frame[8]=2;put(frame+16,2);put(frame+32,sequence);put(frame+40,17);put(frame+48,19);}
static uint32_t watch(uint64_t o,uint64_t s,uint64_t c){return o==1&&s==2&&!c?0:13;}
static uint32_t poll(uint64_t o,uint32_t m,uint64_t out,uint64_t cap,uint64_t slot){assert(o==1&&!m);polls++;if(idle)return 11;put((unsigned char*)(uintptr_t)slot,64);if(cap<64)return 90;memcpy((void*)(uintptr_t)out,frame,64);return 0;}
static uint32_t close_owner(uint64_t o){assert(o==1);closes++;return 0;}
static uint32_t second(void* user,roswasmc_event_header const* h,unsigned char const* body,size_t length){(void)user;(void)body;assert(!length&&h->sequence==2);callbacks++;return 0;}
static uint32_t first(void* user,roswasmc_event_header const* h,unsigned char const* body,size_t length){
    (void)user;(void)body;assert(!length&&h->sequence==1);callbacks++;
    unsigned before=polls;assert(roswasmc_event_subscription_spin(&subscription)==17&&polls==before);
    assert(roswasmc_event_subscription_close(&subscription)==17&&closes==0);
    subscription.callback=second;return 0;
}
int main(void){
    roswasmc_backend backend={0};backend.version=2;backend.close=close_owner;roswasmc_context context={&backend};
    roswasmc_capability observer={&context,1},source={&context,2};roswasmc_event_backend api={2,watch,poll,0,0};
    unsigned char scratch[64];assert(roswasmc_event_subscription_init(&subscription,&api,&observer,&source,0,scratch,64,first,0)==0);
    roswasmc_event_subscription* entries[]={&subscription};size_t delivered=99;record(1);
    assert(roswasmc_event_spin_some(entries,1,1,&delivered)==0&&delivered==1&&callbacks==1&&polls==1);
    record(2);assert(roswasmc_event_spin_some(entries,1,1,&delivered)==0&&delivered==1&&callbacks==2);
    // Replay, wrong source/session and malformed body length never dispatch.
    assert(roswasmc_event_subscription_spin(&subscription)==22&&callbacks==2);
    record(3);put(frame+16,3);assert(roswasmc_event_subscription_spin(&subscription)==22&&callbacks==2);
    record(3);put(frame+48,20);assert(roswasmc_event_subscription_spin(&subscription)==22&&callbacks==2);
    record(3);put(frame+56,1);assert(roswasmc_event_subscription_spin(&subscription)==22&&callbacks==2);
    idle=1;assert(roswasmc_event_spin_some(entries,1,1,&delivered)==0&&delivered==0);
    assert(roswasmc_event_subscription_close(&subscription)==0&&closes==1&&source.handle==2);
    assert(roswasmc_event_subscription_spin(&subscription)==9);
    return 0;
}
