/* SPDX-License-Identifier: Apache-2.0 */
#ifndef UWVM_CUSTOM_SCALAR_C_CONTRACT_H
#define UWVM_CUSTOM_SCALAR_C_CONTRACT_H
#include <stddef.h>
#include <stdint.h>
#include <rosidl_runtime_c/message_type_support_struct.h>
#include <rosidl_runtime_c/service_type_support_struct.h>
#ifdef __cplusplus
extern "C" {
#endif
/** Real C-generated default/object and bounded-C wire witness, not a provider.
 * Original C objects never cross into a C++ Native codec. These static original
 * support handles remain borrowed; all three byte images and this result commit
 * only after complete local C object/default/field/codec checks have succeeded.
 * They confer no Node, alias, Native endpoint, client or Guest authority.
 */
struct uwvm_custom_modbus_c_observation {
 size_t default_request_bytes,request_bytes,response_bytes;
 rosidl_message_type_support_t const* request;
 rosidl_message_type_support_t const* response;
 rosidl_message_type_support_t const* event;
 rosidl_service_type_support_t const* service;
};
uint32_t uwvm_custom_modbus_c(unsigned char*,size_t,unsigned char*,size_t,unsigned char*,size_t,
 struct uwvm_custom_modbus_c_observation*);
#ifdef __cplusplus
}
#endif
#endif
