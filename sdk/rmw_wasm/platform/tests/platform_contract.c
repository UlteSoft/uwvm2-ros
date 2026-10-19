/* SPDX-License-Identifier: Apache-2.0
 * Copyright 2026 UWVM contributors.
 * Source-only authored host-backend contracts. These are NOT ROS/RMW/native
 * execution evidence. Compile against the actual SDK and platform source.
 */
#include "uwvm_rmw_platform.h"
#include <roswasm/codec.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

static struct {
  unsigned opens,closes,writes,requests,deny_at,close_fail_at,version;
  uint64_t close_order[42],last_log_handle;
  unsigned char last_log[4100];size_t last_log_size;
  uint32_t request_error;unsigned response_length;
  uint64_t now;
  int reenter,duplicate;
} model;
static uint32_t version(void){return model.version;}
static uint64_t owner(unsigned n){return UINT64_C(0xf123456780000000)+n;}
static void put(unsigned char * bytes,uint64_t value){unsigned i;for(i=0;i<8;++i)bytes[i]=(unsigned char)(value>>(8*i));}
static uint32_t open_alias(uint32_t memory,uint64_t address,uint64_t size,uint64_t out){
  (void)memory;assert(address&&size&&size<=255);++model.opens;
  if(model.opens==model.deny_at)return 13;
  put((unsigned char*)(uintptr_t)out,owner(model.duplicate?1:model.opens));return 0;
}
static uint32_t close_owner(uint64_t handle){
  model.close_order[model.closes++]=handle;return model.closes==model.close_fail_at?110:0;
}
static uint32_t clock_request(uint64_t h,uint32_t m,uint64_t in,uint64_t n,uint64_t out,uint64_t cap,uint64_t length){
  (void)m;(void)in;assert(h==owner(1)||h==owner(2));assert(n==0&&cap==8);++model.requests;
  if(model.reenter==1)assert(uwvm_rmw_platform_close()==17);
  if(model.reenter==2)assert(uwvm_rmw_platform_log(20,"demo","recursive callback")==17);
  if(model.request_error)return model.request_error;
  put((unsigned char*)(uintptr_t)out,model.now);put((unsigned char*)(uintptr_t)length,model.response_length);return 0;
}
static uint32_t log_write(uint64_t handle,uint32_t memory,uint64_t wire,uint64_t size){
  (void)memory;assert(size<=sizeof(model.last_log));++model.writes;model.last_log_handle=handle;
  memcpy(model.last_log,(void const*)(uintptr_t)wire,(size_t)size);model.last_log_size=(size_t)size;return 0;
}
static roswasmc_backend backend;
static uwvm_rmw_platform_logger logger={"demo",{"log.debug","log.info","log.warn","log.error","log.fatal"}};
static uwvm_rmw_platform_profile profile;
static void fresh(void){
  uwvm_rmw_platform_diagnostics before;
  assert(uwvm_rmw_platform_get_diagnostics(&before)==0);
  if(before.external_logger_active)assert(uwvm_rmw_platform_logger_stop()==0);
  assert(before.lifetime_references==0);(void)uwvm_rmw_platform_close();
  memset(&model,0,sizeof(model));memset(&backend,0,sizeof(backend));
  model.version=1;model.response_length=8;model.now=UINT64_C(9007199254740993);
  backend.version=2;backend.abi_version=version;backend.open=open_alias;backend.close=close_owner;backend.log=log_write;
  backend.request[ROSWASMC_TIME_READ_SYSTEM]=clock_request;backend.request[ROSWASMC_TIME_READ_STEADY]=clock_request;
  profile=(uwvm_rmw_platform_profile){1,&backend,"clock.system","clock.steady",&logger,1};
}
static void denied_without_prepare(void){int64_t n=77;fresh();assert(uwvm_rmw_platform_now(UWVM_RMW_SYSTEM_CLOCK,&n)==13&&n==77&&model.opens==0);}
static void bad_profile_before_effects(void){fresh();profile.version=2;assert(uwvm_rmw_platform_prepare(&profile)==22&&model.opens==0);}
static void unsupported_operation_before_effects(void){fresh();backend.request[ROSWASMC_TIME_READ_STEADY]=0;assert(uwvm_rmw_platform_prepare(&profile)==95&&model.opens==0);}
static void duplicate_alias_before_effects(void){fresh();profile.steady_clock_alias=profile.system_clock_alias;assert(uwvm_rmw_platform_prepare(&profile)==22&&model.opens==0);}
static void reverse_rollback(void){fresh();model.deny_at=3;assert(uwvm_rmw_platform_prepare(&profile)==13&&model.closes==2);assert(model.close_order[0]==owner(2)&&model.close_order[1]==owner(1));}
static void every_close_and_primary_retained(void){uwvm_rmw_platform_diagnostics d;fresh();model.deny_at=6;model.close_fail_at=1;
  assert(uwvm_rmw_platform_prepare(&profile)==13&&model.closes==5);assert(uwvm_rmw_platform_get_diagnostics(&d)==0);
  assert(d.first_raw_error==13&&d.first_close_error==110&&d.close_failures==1&&d.close_attempts==5);
  assert(d.owner_close_attempted[4]&&d.owner_close_codes[4]==110&&d.owner_close_attempted[0]);}
static void genuine_full_u64_owners(void){uwvm_rmw_platform_diagnostics d;fresh();assert(uwvm_rmw_platform_prepare(&profile)==0&&model.opens==7);
  assert(uwvm_rmw_platform_close()==0&&model.closes==7&&model.close_order[0]==owner(7)&&model.close_order[6]==owner(1));
  assert(uwvm_rmw_platform_get_diagnostics(&d)==0&&d.acquired==7&&!d.prepared);}
static void exact_clock_and_errors(void){int64_t n=0;fresh();assert(!uwvm_rmw_platform_prepare(&profile));
  assert(!uwvm_rmw_platform_now(UWVM_RMW_SYSTEM_CLOCK,&n)&&n==INT64_C(9007199254740993));
  model.now=42;assert(!uwvm_rmw_platform_now(UWVM_RMW_STEADY_CLOCK,&n)&&n==42);
  model.request_error=125;assert(uwvm_rmw_platform_now(UWVM_RMW_STEADY_CLOCK,&n)==125&&n==42);}
static void exact_extent_and_signed_range(void){int64_t n=91;fresh();assert(!uwvm_rmw_platform_prepare(&profile));
  model.response_length=7;assert(uwvm_rmw_platform_now(UWVM_RMW_SYSTEM_CLOCK,&n)==22&&n==91);
  model.response_length=8;model.now=UINT64_MAX;assert(uwvm_rmw_platform_now(UWVM_RMW_SYSTEM_CLOCK,&n)==22&&n==91);}
static void reentrant_close_is_busy(void){int64_t n=0;uwvm_rmw_platform_diagnostics d;fresh();assert(!uwvm_rmw_platform_prepare(&profile));model.reenter=1;
  assert(!uwvm_rmw_platform_now(UWVM_RMW_SYSTEM_CLOCK,&n)&&model.closes==0);
  model.reenter=2;assert(!uwvm_rmw_platform_now(UWVM_RMW_SYSTEM_CLOCK,&n));
  assert(!uwvm_rmw_platform_get_diagnostics(&d)&&d.concurrent_log_refused&&model.writes==0);}
static void lifetime_prevents_retirement(void){fresh();assert(!uwvm_rmw_platform_prepare(&profile));assert(!uwvm_rmw_platform_retain());
  assert(uwvm_rmw_platform_close()==17&&model.closes==0);assert(!uwvm_rmw_platform_release());assert(!uwvm_rmw_platform_close()&&model.closes==7);}
static void unknown_logger_does_not_infer_rights(void){uwvm_rmw_platform_diagnostics d;fresh();assert(!uwvm_rmw_platform_prepare(&profile));
  assert(!uwvm_rmw_platform_logger_start(0,0));assert(uwvm_rmw_platform_log(20,"demo.child","not selected")==13&&model.writes==0);
  assert(!uwvm_rmw_platform_get_diagnostics(&d)&&d.log_failures==1&&d.first_raw_error==13);}
static void five_exact_severity_owners_and_wire(void){int s;fresh();assert(!uwvm_rmw_platform_prepare(&profile));assert(!uwvm_rmw_platform_logger_start(0,0));
  assert(!uwvm_rmw_platform_logger_level(0,10));
  for(s=10;s<=50;s+=10){assert(!uwvm_rmw_platform_log(s,"demo","hello"));assert(model.last_log_handle==owner(2+(unsigned)(s/10)));
    assert(model.last_log_size==9&&model.last_log[0]==5&&!memcmp(model.last_log+4,"hello",5));}
  assert(model.writes==5);}
static void strict_utf8(void){fresh();assert(!uwvm_rmw_platform_prepare(&profile));assert(!uwvm_rmw_platform_logger_start(0,0));
  assert(uwvm_rmw_platform_log(20,"demo","\xc0\x80")==22&&model.writes==0);}
static void exact_log_bound(void){char msg[4098];fresh();assert(!uwvm_rmw_platform_prepare(&profile));assert(!uwvm_rmw_platform_logger_start(0,0));
  memset(msg,'x',4096);msg[4096]=0;assert(!uwvm_rmw_platform_log(20,"demo",msg)&&model.last_log_size==4100);
  msg[4096]='x';msg[4097]=0;assert(uwvm_rmw_platform_log(20,"demo",msg)==90&&model.writes==1);}
static void no_file_config_permission(void){fresh();assert(!uwvm_rmw_platform_prepare(&profile));
  assert(uwvm_rmw_platform_logger_start("file",0)==95&&uwvm_rmw_platform_logger_start(0,"config")==95);}
static void levels_do_not_gain_aliases(void){fresh();assert(!uwvm_rmw_platform_prepare(&profile));assert(!uwvm_rmw_platform_logger_start(0,0));
  assert(uwvm_rmw_platform_logger_level("foreign",10)==13&&uwvm_rmw_platform_logger_level("demo",21)==22);
  assert(!uwvm_rmw_platform_logger_level("demo",40));assert(!uwvm_rmw_platform_log(20,"demo","filtered")&&model.writes==0);
  assert(!uwvm_rmw_platform_log(40,"demo","error")&&model.last_log_handle==owner(6));}
static void wrong_backend_and_version(void){fresh();backend.version=1;assert(uwvm_rmw_platform_prepare(&profile)==22&&model.opens==0);
  backend.version=2;model.version=2;assert(uwvm_rmw_platform_prepare(&profile)==95&&model.opens==0);}
static void no_implicit_close_replay(void){fresh();assert(!uwvm_rmw_platform_prepare(&profile));model.close_fail_at=2;
  assert(uwvm_rmw_platform_close()==110&&model.closes==7);assert(uwvm_rmw_platform_close()==110&&model.closes==7);}
static void duplicate_backend_handle_refused(void){fresh();model.duplicate=1;
  assert(uwvm_rmw_platform_prepare(&profile)==22&&model.opens==2&&model.closes==1&&model.close_order[0]==owner(1));}
int main(void){
  denied_without_prepare();bad_profile_before_effects();unsupported_operation_before_effects();duplicate_alias_before_effects();
  reverse_rollback();every_close_and_primary_retained();genuine_full_u64_owners();exact_clock_and_errors();
  exact_extent_and_signed_range();reentrant_close_is_busy();lifetime_prevents_retirement();unknown_logger_does_not_infer_rights();
  five_exact_severity_owners_and_wire();strict_utf8();exact_log_bound();no_file_config_permission();levels_do_not_gain_aliases();
  wrong_backend_and_version();no_implicit_close_replay();duplicate_backend_handle_refused();fresh();return 0;
}
