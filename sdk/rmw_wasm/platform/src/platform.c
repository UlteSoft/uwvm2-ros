/* SPDX-License-Identifier: Apache-2.0
 * Copyright 2026 UWVM contributors.
 * Allocation-free platform ownership over the REAL C SDK. C11 atomics protect
 * state against concurrent/reentrant entry; a busy operation returns17. No lock
 * is held waiting for another Guest thread, and no callback is invoked here.
 */
#include "uwvm_rmw_platform.h"
#include <roswasm/codec.h>
#include <stdatomic.h>
#include <string.h>
#include <limits.h>
#ifndef UWVM_RMW_ENABLE_THREADED_PROFILE
#define UWVM_RMW_ENABLE_THREADED_PROFILE 0
#endif
#if UWVM_RMW_ENABLE_THREADED_PROFILE
#include <uwvm_rmw/dispatch.h>
static _Thread_local uwvm_rmw_dispatch_guard platform_dispatch;
#endif

typedef struct platform_logger {
  char name[256];
  int threshold;
} platform_logger;
static atomic_flag busy=ATOMIC_FLAG_INIT;
static atomic_bool busy_log_refused;
static struct {
  roswasmc_context context;
  roswasmc_capability owners[UWVM_RMW_PLATFORM_MAX_OWNERS];
  platform_logger loggers[UWVM_RMW_PLATFORM_MAX_LOGGERS];
  size_t logger_count,count,references;
  int ready,logging,root_threshold;
  uwvm_rmw_platform_diagnostics diagnostics;
} state;
#if UWVM_RMW_ENABLE_THREADED_PROFILE
static int enter(void){
 /* Same-thread B->A entry borrows only the actual mutex ownership. Public RMW
  * callbacks still reject recursion; nested platform callbacks reject before
  * overwriting platform_dispatch or changing the outer operation's busy flag. */
 if(uwvm_rmw_dispatch_platform_enter(&platform_dispatch))return 0;
 if(atomic_flag_test_and_set_explicit(&busy,memory_order_acquire)){
  uwvm_rmw_dispatch_leave(&platform_dispatch);return 0;}
 return 1;
}
static void leave(void){atomic_flag_clear_explicit(&busy,memory_order_release);uwvm_rmw_dispatch_leave(&platform_dispatch);}
#else
static int enter(void){return !atomic_flag_test_and_set_explicit(&busy,memory_order_acquire);}
static void leave(void){atomic_flag_clear_explicit(&busy,memory_order_release);}
#endif
static uint32_t record(uint32_t code){if(code){if(!state.diagnostics.first_raw_error)state.diagnostics.first_raw_error=code;state.diagnostics.last_raw_error=code;}return code;}
/* Bounded C-string inspection: the caller, like the original rcutils API, must
 * provide an accessible string object. No unbounded strlen/truncation occurs. */
static int text_length(char const * text,size_t maximum,size_t * n){
  size_t i;if(!text)return 0;for(i=0;i<=maximum;++i)if(!text[i]){*n=i;return 1;}return 0;
}
static int copy_name(char output[256],char const * input,int empty){
  size_t n;roswasmc_string value;
  if(!text_length(input,255,&n)||(!empty&&!n))return 0;
  value.data=input;value.size=n;if(!roswasmc_utf8(value,0))return 0;
  memcpy(output,input,n+1);return 1;
}
static void retire(void){
  size_t n=state.count;while(n){size_t i=--n;uint32_t code=roswasmc_close(&state.owners[i]);
    state.diagnostics.owner_close_attempted[i]=1;state.diagnostics.owner_close_codes[i]=code;
    ++state.diagnostics.close_attempts;if(code){++state.diagnostics.close_failures;if(!state.diagnostics.first_close_error)state.diagnostics.first_close_error=code;record(code);}}
  state.count=0;state.ready=0;state.logging=0;state.references=0;
}
uint32_t uwvm_rmw_platform_prepare(uwvm_rmw_platform_profile const * profile){
  /* All alias/name copies are stack-local and validated BEFORE any side effect.
   * No pointer into mutable caller strings is used after acquisition begins. */
  char aliases[UWVM_RMW_PLATFORM_MAX_OWNERS][256];
  platform_logger loggers[UWVM_RMW_PLATFORM_MAX_LOGGERS];
  size_t count=0,i,j,k,logger_count;uint32_t code=0;roswasmc_backend const * backend;
  if(!enter())return UWVM_ROS_CONFLICT;
  if(state.ready||state.count||state.references||state.logging){leave();return UWVM_ROS_CONFLICT;}
  if(!profile||profile->version!=1||!profile->backend||profile->backend->version!=2||
     profile->logger_count>UWVM_RMW_PLATFORM_MAX_LOGGERS||(!profile->loggers&&profile->logger_count)){
    leave();return UWVM_ROS_INVALID;}
  backend=profile->backend;logger_count=profile->logger_count;
  if(!backend->abi_version||!backend->open||!backend->close||!backend->request[ROSWASMC_TIME_READ_SYSTEM]||
     !backend->request[ROSWASMC_TIME_READ_STEADY]||(profile->logger_count&&!backend->log)){
    leave();return UWVM_ROS_UNSUPPORTED;}
  if(!copy_name(aliases[count++],profile->system_clock_alias,0)||
     !copy_name(aliases[count++],profile->steady_clock_alias,0)){leave();return UWVM_ROS_INVALID;}
  for(i=0;i<logger_count;++i){
    if(!copy_name(loggers[i].name,profile->loggers[i].name,1)){leave();return UWVM_ROS_INVALID;}
    loggers[i].threshold=0;
    for(j=0;j<i;++j)if(!strcmp(loggers[i].name,loggers[j].name)){leave();return UWVM_ROS_INVALID;}
    for(j=0;j<5;++j)if(!copy_name(aliases[count++],profile->loggers[i].aliases[j],0)){leave();return UWVM_ROS_INVALID;}
  }
  for(i=0;i<count;++i)for(j=0;j<i;++j)if(!strcmp(aliases[i],aliases[j])){leave();return UWVM_ROS_INVALID;}
  /* Version check may call the explicit backend, but opens nothing. */
  if(backend->abi_version()!=1){leave();return UWVM_ROS_UNSUPPORTED;}
  memset(&state,0,sizeof(state));state.context.backend=backend;state.root_threshold=20;
  state.logger_count=logger_count;atomic_store_explicit(&busy_log_refused,0,memory_order_relaxed);
  for(i=0;i<state.logger_count;++i)state.loggers[i]=loggers[i];
  for(i=0;i<count;++i){
    roswasmc_capability_init(&state.owners[i],&state.context);
    code=roswasmc_open(&state.owners[i],aliases[i],strlen(aliases[i]));
    if(code){record(code);retire();leave();return code;}
    /* Reserve/retain each already initialized owner before another import. */
    ++state.count;++state.diagnostics.acquired;
    for(k=0;k<i;++k)if(state.owners[k].handle==state.owners[i].handle){
      /* A malformed backend cannot make two unique owners of one handle. Retire
       * the duplicate local slot without a second close; the first owns it. */
      state.owners[i].handle=0;--state.count;record(UWVM_ROS_INVALID);retire();leave();return UWVM_ROS_INVALID;
    }
  }
  state.ready=1;leave();return 0;
}
uint32_t uwvm_rmw_platform_retain(void){uint32_t code=0;if(!enter())return UWVM_ROS_CONFLICT;
  if(!state.ready)code=UWVM_ROS_DENIED;else if(state.references==SIZE_MAX)code=UWVM_ROS_TOO_LARGE;else ++state.references;
  leave();return code;
}
uint32_t uwvm_rmw_platform_release(void){uint32_t code=0;if(!enter())return UWVM_ROS_CONFLICT;
  if(!state.references)code=UWVM_ROS_BAD_HANDLE;else --state.references;leave();return code;
}
uint32_t uwvm_rmw_platform_close(void){uint32_t code=0;if(!enter())return UWVM_ROS_CONFLICT;
  if(state.references||state.logging){leave();return UWVM_ROS_CONFLICT;}
  if(state.count)retire();
  if(state.diagnostics.close_failures)code=state.diagnostics.first_close_error;
  leave();return code;
}
uint32_t uwvm_rmw_platform_get_diagnostics(uwvm_rmw_platform_diagnostics * out){
  if(!out)return UWVM_ROS_INVALID;if(!enter())return UWVM_ROS_CONFLICT;
  *out=state.diagnostics;out->lifetime_references=state.references;out->prepared=state.ready;
  out->concurrent_log_refused=atomic_load_explicit(&busy_log_refused,memory_order_relaxed);
  out->external_logger_active=state.logging;leave();return 0;
}
uint32_t uwvm_rmw_platform_now(enum uwvm_rmw_platform_clock kind,int64_t * output){
  unsigned char bytes[8];roswasmc_result result;uint64_t value;uint32_t code=0;
  if(!output||(kind!=UWVM_RMW_SYSTEM_CLOCK&&kind!=UWVM_RMW_STEADY_CLOCK))return UWVM_ROS_INVALID;
  if(!enter())return UWVM_ROS_CONFLICT;
  if(!state.ready){leave();return UWVM_ROS_DENIED;}
  result=roswasmc_request(&state.owners[(size_t)kind],kind==UWVM_RMW_SYSTEM_CLOCK?
    ROSWASMC_TIME_READ_SYSTEM:ROSWASMC_TIME_READ_STEADY,0,0,bytes,sizeof(bytes));
  ++state.diagnostics.clock_reads;
  if(result.error)code=result.error;
  else if(!result.has_length||result.length!=8||(value=uwvm_ros_load_u64(bytes))>INT64_MAX)code=UWVM_ROS_INVALID;
  else *output=(int64_t)value;
  record(code);leave();return code;
}
uint32_t uwvm_rmw_platform_logger_start(char const * prefix,char const * config){
  uint32_t code=0;if(!enter())return UWVM_ROS_CONFLICT;
  /* This backend has no log-file permission or config parser. Refuse explicit
   * file requests instead of claiming to honor or silently discarding them. */
  if((prefix&&*prefix)||(config&&*config))code=UWVM_ROS_UNSUPPORTED;
  else if(!state.ready||!state.logger_count)code=UWVM_ROS_DENIED;
  else if(state.logging)code=UWVM_ROS_CONFLICT;
  else state.logging=1;
  record(code);leave();return code;
}
uint32_t uwvm_rmw_platform_logger_stop(void){uint32_t code=0;if(!enter())return UWVM_ROS_CONFLICT;
  if(!state.logging)code=UWVM_ROS_BAD_HANDLE;else state.logging=0;record(code);leave();return code;
}
static size_t logger_index(char const * name){size_t i;char bounded[256];
  if(!copy_name(bounded,name?name:"",1))return SIZE_MAX;
  for(i=0;i<state.logger_count;++i)if(!strcmp(state.loggers[i].name,bounded))return i;return SIZE_MAX;
}
uint32_t uwvm_rmw_platform_logger_level(char const * name,int level){
  size_t i;uint32_t code=0;if(!enter())return UWVM_ROS_CONFLICT;
  if(level!=-1&&level!=0&&level!=10&&level!=20&&level!=30&&level!=40&&level!=50)code=UWVM_ROS_INVALID;
  else if(!state.ready||!state.logging)code=UWVM_ROS_DENIED;
  else if(!name||!*name)state.root_threshold=(level<=0?20:level);
  else if((i=logger_index(name))==SIZE_MAX)code=UWVM_ROS_DENIED;
  else state.loggers[i].threshold=level<=0?0:level;
  record(code);leave();return code;
}
uint32_t uwvm_rmw_platform_log(int severity,char const * name,char const * message){
  size_t i,n;uint32_t code=0;unsigned char wire[4+UWVM_RMW_PLATFORM_MAX_LOG_BYTES];
  roswasmc_string text;roswasmc_writer writer=roswasmc_writer_init(wire,sizeof(wire));
  if(!enter()){atomic_store_explicit(&busy_log_refused,1,memory_order_relaxed);return UWVM_ROS_CONFLICT;}
  if(!state.ready||!state.logging)code=UWVM_ROS_DENIED;
  else if(severity!=10&&severity!=20&&severity!=30&&severity!=40&&severity!=50)code=UWVM_ROS_INVALID;
  else if((i=logger_index(name))==SIZE_MAX)code=UWVM_ROS_DENIED;
  else if(!text_length(message,UWVM_RMW_PLATFORM_MAX_LOG_BYTES,&n))code=UWVM_ROS_TOO_LARGE;
  else {
    text.data=message;text.size=n;
    if(!roswasmc_utf8(text,0)||!roswasmc_write_string(&writer,text))code=UWVM_ROS_INVALID;
    else if(severity>=(state.loggers[i].threshold?state.loggers[i].threshold:state.root_threshold)){
      code=roswasmc_log(&state.owners[2+i*5+(size_t)(severity/10-1)],(char const*)wire,writer.size);
      ++state.diagnostics.log_writes;
    }
  }
  if(code)++state.diagnostics.log_failures;
  record(code);leave();return code;
}
