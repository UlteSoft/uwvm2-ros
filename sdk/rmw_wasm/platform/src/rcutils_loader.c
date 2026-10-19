/* SPDX-License-Identifier: Apache-2.0
 * Copyright 2026 UWVM contributors.
 * ORIGINAL rcutils loader signatures with an explicit unsupported platform.
 * Static RMW and single actual typesupport forwarding never need dlopen. This
 * is a refusal implementation, not a static-symbol or fabricated-handle loader.
 */
#include <rcutils/shared_library.h>
#include <rcutils/error_handling.h>
#include <stdbool.h>
static rcutils_ret_t unsupported(void){RCUTILS_SET_ERROR_MSG("dynamic libraries are unsupported in the static WASI profile");return RCUTILS_RET_ERROR;}
rcutils_shared_library_t rcutils_get_zero_initialized_shared_library(void){
  rcutils_shared_library_t value={0};value.allocator=rcutils_get_zero_initialized_allocator();return value;
}
rcutils_ret_t rcutils_load_shared_library(rcutils_shared_library_t * lib,char const * name,rcutils_allocator_t allocator){
  if(!lib||!name||!rcutils_allocator_is_valid(&allocator)||lib->lib_pointer||lib->library_path){
    RCUTILS_SET_ERROR_MSG("invalid shared library arguments");return RCUTILS_RET_INVALID_ARGUMENT;}
  return unsupported();
}
void * rcutils_get_symbol(rcutils_shared_library_t const * lib,char const * symbol){(void)lib;(void)symbol;unsupported();return 0;}
bool rcutils_has_symbol(rcutils_shared_library_t const * lib,char const * symbol){(void)lib;(void)symbol;unsupported();return false;}
rcutils_ret_t rcutils_unload_shared_library(rcutils_shared_library_t * lib){
  if(!lib){RCUTILS_SET_ERROR_MSG("null shared library");return RCUTILS_RET_INVALID_ARGUMENT;}return unsupported();
}
bool rcutils_is_shared_library_loaded(rcutils_shared_library_t * lib){(void)lib;return false;}
rcutils_ret_t rcutils_get_platform_library_name(char const * base,char * output,unsigned int capacity,bool debug){
  (void)base;(void)output;(void)capacity;(void)debug;return unsupported();
}
