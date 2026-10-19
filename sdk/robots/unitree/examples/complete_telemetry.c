/** @file C11: raw imports, then one complete typed telemetry value.
 * Supply the actual generated complete family and define its family selector
 * as documented in complete_family.h. The default executable must observe
 * permission refusal13 with no live handle. A positive application requires
 * UWVM_UNITREE_COMPLETE_RECEIVE=1 and independent operator-granted subscribe
 * authority for the exact alias, full native/canonical identity and quota.
 * This never publishes a command. One explicit poll returns11 when idle; there
 * is no hidden clock, queue retry, transport discovery or default permission.
 */
#include <roswasm_unitree/complete_family.h>
#ifndef UWVM_UNITREE_COMPLETE_RECEIVE
#define UWVM_UNITREE_COMPLETE_RECEIVE 0
#endif

/* Full owned copies; these never contain a pointer to native middleware. */
static unsigned char canonical[UWVM_UNITREE_COMPLETE_CAPACITY];
static uwvm_unitree_complete_value latest;

int main(void) {
    roswasmc_context context={UWVM_UNITREE_COMPLETE_BACKEND()};
    roswasmc_capability subscription;
    uint32_t code,cleanup;
    if(UWVM_UNITREE_COMPLETE_VERSION()!=1)return 95;
    roswasmc_capability_init(&subscription,&context);
    code=roswasmc_open(&subscription,UWVM_UNITREE_COMPLETE_ALIAS,
                       sizeof UWVM_UNITREE_COMPLETE_ALIAS-1);
    if(!UWVM_UNITREE_COMPLETE_RECEIVE) {
        if(code==13&&!roswasmc_is_open(&subscription))return 0;
        if(roswasmc_is_open(&subscription))(void)roswasmc_close(&subscription);
        return 22;
    }
    if(code)return (int)code;
    code=roswasmc_subscribe(&subscription);
    if(!code) {
        roswasmc_result result=roswasmc_poll(&subscription,canonical,sizeof canonical);
        code=result.error;
        if(!code) {
            if(!result.has_length||result.length>sizeof canonical)code=22;
            else code=UWVM_UNITREE_COMPLETE_DECODE(canonical,(size_t)result.length,&latest);
        }
    }
    cleanup=roswasmc_close(&subscription);
    if(!code)code=cleanup;
    /* A successful decode is local validation. Independent native observation
     * is still needed to establish who sent the payload and what it means. */
    return (int)code;
}
