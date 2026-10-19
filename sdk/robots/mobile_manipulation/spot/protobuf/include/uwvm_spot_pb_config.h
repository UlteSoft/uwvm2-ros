/* SPDX-License-Identifier: Apache-2.0
 * Force-include for ALL nanopb runtime/generated/wrapper translation units.
 * Keep these options uniform: they change actual generated C object layouts. */
#ifndef UWVM_SPOT_NANOPB_CONFIG_H
#define UWVM_SPOT_NANOPB_CONFIG_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#define PB_ENABLE_MALLOC 1
#define PB_FIELD_32BIT 1
#define PB_BUFFER_ONLY 1
#define PB_SYSTEM_HEADER "uwvm_spot_pb_system.h"
#define pb_realloc uwvm_spot_pb_realloc
#define pb_free uwvm_spot_pb_free
#ifdef __cplusplus
extern "C" {
#endif
void *uwvm_spot_pb_realloc(void *, size_t);
void uwvm_spot_pb_free(void *);
#ifdef __cplusplus
}
#endif
#endif
