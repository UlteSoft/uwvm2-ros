/* SPDX-License-Identifier: Apache-2.0
 * This small C ABI owns ACTUAL generated C++ objects and CPP10 descriptors. It
 * is not a replacement native type or an undefined fixture function. The full
 * companion is built with this C bootstrap. RMW C takes typed-support+void*, so
 * supplying matching original C++ objects is legitimate; mixing C objects is not.
 */
#ifndef UWVM_RMW_SERVICE_EXAMPLE_NATIVE_OBJECTS_H
#define UWVM_RMW_SERVICE_EXAMPLE_NATIVE_OBJECTS_H
#include <uwvm_rmw/codec.h>
#ifdef __cplusplus
extern "C" {
#endif
uint32_t example_native_prepare(char const*const identities[11],int add,
 uwvm_rmw_codec const**codecs,size_t*count,size_t*service_index);
void*example_native_request(void);
void*example_native_response(void);
uint32_t example_native_set_request(void);
uint32_t example_native_check_request(void);
uint32_t example_native_set_response(void);
uint32_t example_native_check_response(void);
#ifdef __cplusplus
}
#endif
#endif
