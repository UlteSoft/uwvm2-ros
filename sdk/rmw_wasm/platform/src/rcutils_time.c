/* SPDX-License-Identifier: Apache-2.0
 * Copyright 2026 UWVM contributors.
 * Implements the ORIGINAL rcutils/time.h symbols; no shadow ROS headers.
 * rcutils/src/time.c remains the original formatting implementation.
 */
#include <rcutils/time.h>
#include <rcutils/error_handling.h>
#include "uwvm_rmw_platform.h"
static rcutils_ret_t read_clock(enum uwvm_rmw_platform_clock kind,rcutils_time_point_value_t * output){
  int64_t next;uint32_t code;
  if(!output){RCUTILS_SET_ERROR_MSG("null time output");return RCUTILS_RET_INVALID_ARGUMENT;}
  code=uwvm_rmw_platform_now(kind,&next);
  if(code){RCUTILS_SET_ERROR_MSG("explicit UWVM platform clock request failed; inspect platform diagnostics");return RCUTILS_RET_ERROR;}
  *output=next;return RCUTILS_RET_OK;
}
rcutils_ret_t rcutils_system_time_now(rcutils_time_point_value_t * output){return read_clock(UWVM_RMW_SYSTEM_CLOCK,output);}
rcutils_ret_t rcutils_steady_time_now(rcutils_time_point_value_t * output){return read_clock(UWVM_RMW_STEADY_CLOCK,output);}
rcutils_ret_t rcutils_raw_steady_time_now(rcutils_time_point_value_t * output){
  if(!output){RCUTILS_SET_ERROR_MSG("null time output");return RCUTILS_RET_INVALID_ARGUMENT;}
  /* Existing time.read-steady proves monotonic only, not a slew-free source.
   * Do not relabel it RAW or emit a success timestamp from an ambient clock. */
  RCUTILS_SET_ERROR_MSG("raw slew-free clock is unsupported by this explicit platform profile");
  return RCUTILS_RET_ERROR;
}
