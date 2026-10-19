/* Copyright (c) 2026 MacroModel contributors. SPDX-License-Identifier: Apache-2.0 */
#ifndef UWVM_SPOT_PROTOBUF_DOCUMENT_H
#define UWVM_SPOT_PROTOBUF_DOCUMENT_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "pb.h"
#ifdef __cplusplus
extern "C" {
#endif

/* The generated metadata follows the ORIGINAL descriptor, before the C-only
 * length-preserving STRING->BYTES representation mapping. Neither it nor this
 * decoder grants a ROS alias or imports a native Boston Dynamics SDK. */
#include "schema.h"
typedef struct uwvm_spot_proto_type {
    const uwvm_spot_proto_schema *schema;
    const pb_msgdesc_t *nanopb;
    size_t object_size;
} uwvm_spot_proto_type;
extern const uwvm_spot_proto_type uwvm_spot_proto_types[];
extern const size_t uwvm_spot_proto_type_count;

/* Caller allocates two disjoint max_align_t-aligned banks. A complete typed
 * object, its pointer/repeated/map/oneof contents and an exact wire snapshot
 * live in one bank. Do not copy/move a live document. The returned object is
 * borrowed const storage, invalidated only by successful decode/clear.
 * A document is single-thread owned. No callback occurs during decode. */
typedef struct uwvm_spot_proto_document {
    unsigned char *banks[2];
    size_t capacity;
    size_t used;
    const void *value;
    const uint8_t *wire;
    size_t wire_size;
    const uwvm_spot_proto_type *type;
    uint32_t active_bank;
    uint32_t initialized;
    uint32_t busy;
} uwvm_spot_proto_document;

uint32_t uwvm_spot_proto_init(uwvm_spot_proto_document *, void *bank0,
                             void *bank1, size_t bytes_per_bank);
uint32_t uwvm_spot_proto_decode(uwvm_spot_proto_document *,
                               const uwvm_spot_proto_type *,
                               const void *wire, size_t size);
uint32_t uwvm_spot_proto_clear(uwvm_spot_proto_document *);
/* Standalone original-descriptor wire/depth/UTF8 validation. Unknown fields
 * are skipped without inventing a message type and retained in exact wire.
 * Deprecated protobuf group wire types are rejected22 (all157 inputs proto3).
 * A depth/node/work bound refusal is90; malformed original wire is22. */
/* Only official nanopb compiled with the supplied forced config calls these.
 * They never allocate the ambient heap, and fail when no lexical decode exists. */
void *uwvm_spot_pb_realloc(void *, size_t);
void uwvm_spot_pb_free(void *);
#ifdef __cplusplus
}
#endif
#endif
