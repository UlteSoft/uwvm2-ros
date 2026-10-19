/* SPDX-License-Identifier: Apache-2.0 */
#ifndef UWVM_RMW_CODEC_COMPOSER_H
#define UWVM_RMW_CODEC_COMPOSER_H
#include <uwvm_rmw/codec.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct uwvm_rmw_codec_composer uwvm_rmw_codec_composer;
typedef struct uwvm_rmw_codec_table_view {uwvm_rmw_codec const*data;size_t count;} uwvm_rmw_codec_table_view;
/* Build one bounded contiguous table required by original B selection. Borrow
 * each original immutable table/owner; copy descriptors without impersonating
 * original address-bound validation_state. Rewire service children to copies.
 * Every clone validates its own exact fields, verifies the original snapshot,
 * then calls original.validate(ORIGINAL_ADDRESS,supplied_support). Duplicate
 * names/pointers or missing children refuse. No owner lifetime is extended by
 * a naked C pointer: all original registries must physically outlive composer
 * publication and every consumer. Exclusive cold bootstrap only. */
uint32_t uwvm_rmw_codec_composer_create(uwvm_rmw_codec_table_view const*,size_t,uwvm_rmw_codec_composer**);
uint32_t uwvm_rmw_codec_composer_export(uwvm_rmw_codec_composer*,uwvm_rmw_codec const**,size_t*);
uint32_t uwvm_rmw_codec_composer_destroy_unexported(uwvm_rmw_codec_composer**);
#ifdef __cplusplus
}
#endif
#endif
