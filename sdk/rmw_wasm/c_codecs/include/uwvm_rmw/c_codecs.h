/* SPDX-License-Identifier: Apache-2.0
 * Seven immutable canonical codecs over ORIGINAL generated C objects. No C++
 * object, substituted struct, fabricated native hash or TypeSupport is used.
 * Build/probe first; warm actual original dispatch/metadata in the owned loader
 * phase BEFORE SIGSTOP/maps. No capability is opened by this registry.
 */
#ifndef UWVM_RMW_C_CODECS_H
#define UWVM_RMW_C_CODECS_H
#include <uwvm_rmw/codec.h>
#include <rcutils/allocator.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
enum uwvm_rmw_c_identity {
 UWVM_RMW_C_ID_STRING,UWVM_RMW_C_ID_TIME,UWVM_RMW_C_ID_EVENT_INFO,
 UWVM_RMW_C_ID_EMPTY_REQUEST,UWVM_RMW_C_ID_EMPTY_RESPONSE,UWVM_RMW_C_ID_EMPTY_EVENT,
 UWVM_RMW_C_ID_ADD_REQUEST,UWVM_RMW_C_ID_ADD_RESPONSE,UWVM_RMW_C_ID_ADD_EVENT,
 UWVM_RMW_C_ID_EMPTY_SERVICE,UWVM_RMW_C_ID_ADD_SERVICE,UWVM_RMW_C_ID_COUNT
};
typedef struct uwvm_rmw_c_native_identities {uint8_t hashes[UWVM_RMW_C_ID_COUNT][32];} uwvm_rmw_c_native_identities;
enum uwvm_rmw_c_codec_index {
 UWVM_RMW_C_CODEC_STRING,UWVM_RMW_C_CODEC_EMPTY_REQUEST,UWVM_RMW_C_CODEC_EMPTY_RESPONSE,
 UWVM_RMW_C_CODEC_EMPTY_SERVICE,UWVM_RMW_C_CODEC_ADD_REQUEST,UWVM_RMW_C_CODEC_ADD_RESPONSE,
 UWVM_RMW_C_CODEC_ADD_SERVICE,UWVM_RMW_C_CODEC_COUNT
};
typedef struct uwvm_rmw_c_codecs_registry uwvm_rmw_c_codecs_registry;
/* All11 identities are actual independently exported RIHS01 payload bytes.
 * String bound4..4194304 is canonical capacity, NOT a native String bound.
 * The allocator is copied into nonmoving opaque state before native warming.
 * Error leaves *output unchanged; missing complete compiled API is95 before
 * allocation/TypeSupport access. There is no legacy downgrade or fake fallback. */
uint32_t uwvm_rmw_c_codecs_create(uwvm_rmw_c_native_identities const*,size_t,
 rcutils_allocator_t const*,uwvm_rmw_c_codecs_registry**output);
/* Export freezes lifetime BEFORE returning any borrowed pointer. A C profile
 * has no uninstall: after export the registry lives until process exit even if
 * profile installation/options/entity creation later fail. No diagnostic or
 * error can revoke this fact; all callbacks validate the original self address. */
uint32_t uwvm_rmw_c_codecs_export(uwvm_rmw_c_codecs_registry*,uwvm_rmw_codec const**output,size_t*count);
uint32_t uwvm_rmw_c_codecs_get(uwvm_rmw_c_codecs_registry*,enum uwvm_rmw_c_codec_index,uwvm_rmw_codec const**output);
/* Read-only observations confer no authority and do not unexport anything. */
uint32_t uwvm_rmw_c_codecs_observe(uwvm_rmw_c_codecs_registry const*,bool*exported,size_t*string_bound);
/* Only an actual never-exported bootstrap registry may be destroyed. After
 * export returns17 and preserves state/output. Success deallocates once and
 * writes NULL; all native message objects have their own actual __fini. */
uint32_t uwvm_rmw_c_codecs_destroy_unexported(uwvm_rmw_c_codecs_registry**);
#ifdef __cplusplus
}
#endif
#endif
