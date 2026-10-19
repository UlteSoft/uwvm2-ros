/* SPDX-License-Identifier: Apache-2.0 */
#ifndef UWVM_SPOT_PROTOBUF_SCHEMA_H
#define UWVM_SPOT_PROTOBUF_SCHEMA_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define UWVM_SPOT_PROTO_WIRE_MAX 16384u
#define UWVM_SPOT_PROTO_DEPTH_MAX 32u
#define UWVM_SPOT_PROTO_BANK_MAX 1048576u
#define UWVM_SPOT_PROTO_UNKNOWN_MESSAGE UINT32_MAX
typedef struct uwvm_spot_proto_field {
    uint32_t tag;
    uint32_t child; /* original message index, or UNKNOWN_MESSAGE */
    uint8_t type;  /* original FieldDescriptorProto.Type */
    uint8_t repeated;
} uwvm_spot_proto_field;
typedef struct uwvm_spot_proto_schema {
    const char *full_name;
    const uwvm_spot_proto_field *fields;
    size_t field_count;
} uwvm_spot_proto_schema;
extern const uwvm_spot_proto_schema uwvm_spot_proto_schemas[];
extern const size_t uwvm_spot_proto_schema_count;


uint32_t uwvm_spot_proto_preflight(const uwvm_spot_proto_schema *,
                                 const void *wire, size_t size);
#ifdef __cplusplus
}
#endif
#endif
