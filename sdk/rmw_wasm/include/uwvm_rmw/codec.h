/* SPDX-License-Identifier: Apache-2.0
 * Extensible canonical-wire conversion registry over REAL generated messages.
 * A descriptor is source-selected, not a type-support/native-identity forgery.
 * Its validate callback must inspect the complete original native graph (with
 * original capacities/defaults) and match the selected real native identity.
 * Canonical bounds are separate. Do not memcpy a native object or CDR payload.
 */
#ifndef UWVM_RMW_CODEC_H
#define UWVM_RMW_CODEC_H
#include <stddef.h>
#include <stdint.h>
#include <rosidl_runtime_c/message_type_support_struct.h>
#include <rosidl_runtime_c/service_type_support_struct.h>
#ifdef __cplusplus
extern "C" {
#endif
enum uwvm_rmw_codec_kind { UWVM_RMW_MESSAGE_CODEC=1, UWVM_RMW_SERVICE_CODEC=2 };
typedef struct uwvm_rmw_codec {
  uint32_t version;
  enum uwvm_rmw_codec_kind kind;
  char const * native_type; /* Complete original spelling, never an alias. */
  void const * generated_support; /* Actual statically linked accessor result. */
  uint8_t native_rihs01[32]; /* Actual exported selected identity, never zero. */
  size_t maximum_wire_bytes;
  uint32_t (*validate)(struct uwvm_rmw_codec const *, void const * supplied_support);
  uint32_t (*encode)(void const * native_message, void * canonical, size_t capacity, size_t * length);
  uint32_t (*decode)(void const * canonical, size_t length, void * initialized_native_message);
  /* MESSAGE: both NULL. SERVICE: exact registered MESSAGE components with
   * original _Request/_Response native names; root encode/decode are NULL.
   * Client encodes request/decodes response; Service does the reverse. The
   * root validate callback must still prove the COMPLETE original service
   * introspection/hash/event graph. Payload-only matching is not sufficient. */
  struct uwvm_rmw_codec const * request;
  struct uwvm_rmw_codec const * response;
  /* Immutable validator-owned state; NULL in existing C source aggregates.
   * This descriptor is a SOURCE-RECOMPILE seam, not a stable binary ABI.
   * version1 selects the source callback contract, never an old layout size.
   * Every B/C/adapter/application consumer must be rebuilt against this exact
   * current header with a qualified complete MD closure. Old descriptor object
   * files/plugins cannot be mixed by version1 or cast to this enlarged struct.
   * The real native generated_support above remains an actual TypeSupport;
   * this field must not overload it or manufacture any native authority. */
  void const * validation_state;
} uwvm_rmw_codec;
/* The first complete witness uses installed/generated std_msgs C structs and
 * real C introspection. It neither supplies substitute structs nor generates a
 * hash. The caller must provide the actual selected native RIHS01 bytes. Future
 * bounded-IR-generated adapters register the same complete vtable contract. */
uint32_t uwvm_rmw_string_codec_init(uwvm_rmw_codec * output,
  rosidl_message_type_support_t const * generated_support,
  uint8_t const actual_native_rihs01[32], size_t canonical_bound);
#ifdef __cplusplus
}
#endif
#endif
