/* SPDX-License-Identifier: Apache-2.0
 * Copyright 2026 UWVM contributors.
 * Real rcl_logging_interface backend. Original rcutils console and rcl rosout
 * code remain enabled/defaulted upstream; this adapter neither suppresses them
 * nor grants their publisher. A missing selection fails initialization.
 */
#include <rcl_logging_interface/rcl_logging_interface.h>
#include <rcutils/allocator.h>
#include "uwvm_rmw_platform.h"
rcl_logging_ret_t rcl_logging_external_initialize(char const * prefix,char const * config,rcutils_allocator_t allocator){
  if(!rcutils_allocator_is_valid(&allocator))return RCL_LOGGING_RET_INVALID_ARGUMENT;
  if(config&&*config)return RCL_LOGGING_RET_CONFIG_FILE_INVALID;
  if(prefix&&*prefix)return RCL_LOGGING_RET_ERROR;
  return uwvm_rmw_platform_logger_start(prefix,config)?RCL_LOGGING_RET_ERROR:RCL_LOGGING_RET_OK;
}
rcl_logging_ret_t rcl_logging_external_shutdown(void){return uwvm_rmw_platform_logger_stop()?RCL_LOGGING_RET_ERROR:RCL_LOGGING_RET_OK;}
void rcl_logging_external_log(int severity,char const * name,char const * message){
  /* Upstream deliberately has a void callback. Keep raw failure in the real
   * platform diagnostics instead of inventing a success result or retrying. */
  (void)uwvm_rmw_platform_log(severity,name,message);
}
rcl_logging_ret_t rcl_logging_external_set_logger_level(char const * name,int level){
  return uwvm_rmw_platform_logger_level(name,level)?RCL_LOGGING_RET_ERROR:RCL_LOGGING_RET_OK;
}
