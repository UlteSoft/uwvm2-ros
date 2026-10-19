/** @file
 * @brief Layer2: explicit capability ownership plus allocation-free typed encoding.
 * The host must grant `pub` as String topic.publish with at least25 canonical
 * bytes (u32 length plus21 UTF-8 bytes); publication permissions remain explicit.
 * See 01_raw_imports.c first: the wrapper preserves that import and wire contract.
 */
#include "select_backend.h"
#include <roswasm/codec.h>
/** @brief Publish through one owned alias and close it on every post-open path. */
int main(void) {
    if(ROSWASM_IMPORT(abi_version)()!=1) return UWVM_ROS_UNSUPPORTED;
    roswasmc_context context={ROSWASM_BACKEND()};roswasmc_capability pub;
    roswasmc_string message={"hello from C resource",21};unsigned char scratch[64];
    uint32_t error,close_error;roswasmc_capability_init(&pub,&context);
    error=roswasmc_open(&pub,"pub",3);if(error) return (int)error;
    error=roswasmc_publish_string(&pub,message,scratch,sizeof scratch);
    close_error=roswasmc_close(&pub);return (int)(error?error:close_error);
}
