#ifndef ROSWASM_CSHARP_OWNED_BRIDGE_H
#define ROSWASM_CSHARP_OWNED_BRIDGE_H
#include <stdint.h>
#include "roswasm/rmw_service_v1.h"
#ifdef __cplusplus
extern "C" {
#endif
/* These same-Wasm exports use scalar addresses only. They do not grant rights.
 * The linked object's basename is roswasm_owned_bridge for Mono P/Invoke scan. */
uint32_t roswasm_owned_abi_version(void);
uint32_t roswasm_owned_client_activate(uint64_t,uint64_t,uint32_t,uint64_t);
uint32_t roswasm_owned_service_activate(uint64_t,uint64_t,uint32_t,uint64_t);
uint32_t roswasm_owned_endpoint_inspect(uint64_t,uint64_t,uint32_t,uint64_t);
uint32_t roswasm_owned_client_submit(uint64_t,uint64_t,uint32_t,uint64_t,uint64_t,uint64_t);
uint32_t roswasm_owned_client_abandon(uint64_t,uint64_t,uint64_t);
uint32_t roswasm_owned_service_respond(uint64_t,uint64_t,uint64_t,uint64_t,uint32_t,uint64_t,uint64_t);
uint32_t roswasm_owned_endpoint_peek(uint64_t,uint64_t,uint32_t,uint32_t,uint64_t,uint64_t);
uint32_t roswasm_owned_endpoint_commit(uint64_t,uint64_t,uint64_t,uint64_t);
uint32_t roswasm_owned_endpoint_cancel(uint64_t,uint64_t,uint64_t,uint64_t);
uint32_t roswasm_owned_client_available(uint64_t,uint64_t,uint64_t,uint64_t,uint32_t,uint64_t);
uint32_t roswasm_owned_endpoint_retire(uint64_t,uint64_t);
#ifdef __cplusplus
}
#endif
#endif
