/* SPDX-License-Identifier: Apache-2.0
 * C-callable SOURCE-RECOMPILE interface. Implementation uses real generated C
 * Log/GetTypeDescription objects and real C++ parameter objects, never casts
 * between their object languages or replaces native TypeSupport. */
#ifndef UWVM_RMW_DEFAULT_CONTROL_H
#define UWVM_RMW_DEFAULT_CONTROL_H
#include <uwvm_rmw/codec.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct uwvm_default_control_identity {
 char const* native_type;
 uint8_t rihs01[32]; /* Mandatory independent actual export, not a default. */
} uwvm_default_control_identity;
typedef struct uwvm_default_control_registry uwvm_default_control_registry;
/* Cold, exclusive bootstrap only. Exactly the generated sorted complete name
 * list is required, including all service/Event/Info/Time/nested references.
 * One admitted attempt per physical Guest Source, even if warming fails. No
 * reset/retry, entity creation, capability open or authority is implied.
 * Feature-zero ->95 before allocation/getters. Outputs remain unchanged on
 * error. Real original SDK getter allocations require an external owner bound. */
uint32_t uwvm_default_control_create(uwvm_default_control_identity const*,size_t,
 uwvm_default_control_registry**);
/* Sticky publication before output commit. Caller retains this nonmoving
 * registry through every options/context/profile/codec/callback borrower and
 * actual physical Source retirement. No profile uninstall exists. Outputs must
 * be distinct, writable caller slots; bootstrap/destruction is not concurrent. */
uint32_t uwvm_default_control_export(uwvm_default_control_registry*,
 uwvm_rmw_codec const**,size_t*);
uint32_t uwvm_default_control_observe(uwvm_default_control_registry const*,bool*,size_t*);
/* Only never-published local state can be freed. Published ->17. Read-only
 * observation cannot unpublish or prove native/Group retirement. */
uint32_t uwvm_default_control_destroy_unexported(uwvm_default_control_registry**);
#ifdef __cplusplus
}
#endif
#endif
