/* SPDX-License-Identifier: Apache-2.0
 * Copyright 2026 UWVM contributors.
 * This adapter calls the original application entry after explicit preparation.
 * The application remains responsible for its real rcl init/options/node and
 * shutdown/context/timer finalization. No NodeOptions defaults are changed.
 */
#include "uwvm_rmw_platform.h"
#include <uwvm_rmw/bridge.h>
typedef int (*original_application)(int,char **);
int uwvm_run_original_application(uwvm_rmw_platform_profile const * platform,
    uwvm_rmw_profile const * endpoints,original_application application,
    int argc,char ** argv,uwvm_rmw_platform_diagnostics * diagnostics){
  uint32_t code,close_code;rmw_ret_t selected;int result;
  if(!application||!diagnostics)return 22;
  code=uwvm_rmw_platform_prepare(platform);if(code){
    (void)uwvm_rmw_platform_get_diagnostics(diagnostics);return (int)code;}
  selected=uwvm_rmw_install_profile(endpoints);
  if(selected!=RMW_RET_OK){
    (void)uwvm_rmw_platform_close();(void)uwvm_rmw_platform_get_diagnostics(diagnostics);return 22;}
  code=uwvm_rmw_platform_retain();if(code){
    (void)uwvm_rmw_install_profile(0);(void)uwvm_rmw_platform_close();
    (void)uwvm_rmw_platform_get_diagnostics(diagnostics);return (int)code;}
  result=application(argc,argv);
  /* Original app must have completed its real rcl finalization before returning.
   * B refuses profile uninstall while live options/contexts remain; if refused,
   * keep the platform lifetime reference and owners alive for explicit recovery.
   * Never tear clocks out from underneath a leaked/live original context. */
  selected=uwvm_rmw_install_profile(0);
  if(selected!=RMW_RET_OK){(void)uwvm_rmw_platform_get_diagnostics(diagnostics);return result?result:17;}
  code=uwvm_rmw_platform_release();close_code=uwvm_rmw_platform_close();
  (void)uwvm_rmw_platform_get_diagnostics(diagnostics);
  return result?result:(int)(code?code:close_code);
}
