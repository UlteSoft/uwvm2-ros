/* SPDX-License-Identifier: Apache-2.0
 * Copyright 2026 UWVM contributors.
 * Explicit process/Guest-profile platform, independent of native RMW rights.
 * There is no environment-derived selection and no implicit clock/log open.
 */
#ifndef UWVM_RMW_PLATFORM_H
#define UWVM_RMW_PLATFORM_H
#include <stddef.h>
#include <stdint.h>
#include <roswasm/roswasmc.h>
#ifdef __cplusplus
extern "C" {
#endif
#define UWVM_RMW_PLATFORM_VERSION 1u
#define UWVM_RMW_PLATFORM_MAX_LOGGERS 8u
#define UWVM_RMW_PLATFORM_MAX_OWNERS (2u+5u*UWVM_RMW_PLATFORM_MAX_LOGGERS)
#define UWVM_RMW_PLATFORM_MAX_LOG_BYTES 4096u
enum uwvm_rmw_platform_clock { UWVM_RMW_SYSTEM_CLOCK=0, UWVM_RMW_STEADY_CLOCK=1 };
/* Exactly one alias per real native severity, in DEBUG/INFO/WARN/ERROR/FATAL
 * order. An alias's actual operation/type/severity is checked by the Host;
 * assigning a name to this profile does not grant it. Names are exact matches,
 * not a wildcard or a hierarchy-derived permission. Empty name is the root. */
typedef struct uwvm_rmw_platform_logger {
  char const * name;
  char const * aliases[5];
} uwvm_rmw_platform_logger;
typedef struct uwvm_rmw_platform_profile {
  uint32_t version;
  roswasmc_backend const * backend;
  char const * system_clock_alias;
  char const * steady_clock_alias;
  uwvm_rmw_platform_logger const * loggers;
  size_t logger_count;
} uwvm_rmw_platform_profile;
typedef struct uwvm_rmw_platform_diagnostics {
  uint32_t first_raw_error,last_raw_error,first_close_error;
  uint32_t owner_close_codes[UWVM_RMW_PLATFORM_MAX_OWNERS];
  uint8_t owner_close_attempted[UWVM_RMW_PLATFORM_MAX_OWNERS];
  size_t acquired,close_attempts,close_failures;
  uint64_t clock_reads,log_writes,log_failures;
  size_t lifetime_references;
  int prepared,external_logger_active,concurrent_log_refused;
} uwvm_rmw_platform_diagnostics;
/* Call before rcl/rcutils logging or clocks initialize. Profile strings are
 * copied and all validation finishes before the first open. The backend table
 * must remain immutable/alive until close. On error every acquired owner gets
 * one reverse close attempt; diagnostics retain each result. No automatic retry.
 * A new prepare after close starts a new diagnostics epoch; save the old one.
 */
uint32_t uwvm_rmw_platform_prepare(uwvm_rmw_platform_profile const *);
/* Bootstrap must retain once per live upstream application/context and release
 * only AFTER its original rcl shutdown/context/clock finalization completes.
 * Close refuses while a lifetime reference or external logger remains active.
 * These are local lifecycle leases, not native permissions. Do not copy leases
 * into unrelated code; balance successful retains, never infer native cleanup.
 */
uint32_t uwvm_rmw_platform_retain(void);
uint32_t uwvm_rmw_platform_release(void);
uint32_t uwvm_rmw_platform_close(void);
uint32_t uwvm_rmw_platform_get_diagnostics(uwvm_rmw_platform_diagnostics *);
/* Actual Host clock request, exactly 8 LE bytes, nonnegative signed int64.
 * Output is unchanged on every failure. Raw/slew-free clock is NOT synthesized.
 * This API does not sleep. No poll count or executor credit represents time.
 */
uint32_t uwvm_rmw_platform_now(enum uwvm_rmw_platform_clock,int64_t *);
/* These functions implement the actual external logger adapter, not a console
 * logger replacement. Local levels filter only; they never increase Host rights.
 * A void upstream log callback's failure remains visible through diagnostics.
 */
uint32_t uwvm_rmw_platform_logger_start(char const * file_prefix,char const * config_file);
uint32_t uwvm_rmw_platform_logger_stop(void);
uint32_t uwvm_rmw_platform_logger_level(char const * name,int level);
uint32_t uwvm_rmw_platform_log(int severity,char const * name,char const * message);
#ifdef __cplusplus
}
#endif
#endif
