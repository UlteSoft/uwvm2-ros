/* SPDX-License-Identifier: Apache-2.0
 * Finite real RMW readiness and wait. A time grant is used as actual elapsed
 * time; a driver credit/poll count is never converted to nanoseconds. Owned
 * prefetched canonical bytes survive until take/destroy, not upper callbacks. */
#include "c_internal.h"
#include <limits.h>
#if UWVM_RMW_ENABLE_THREADED_PROFILE
/* Every pointer below is either a Context fixed lifetime slot or holds an
 * actual wait pin. A caller's array remains its own accessible storage during
 * this real synchronous rmw_wait call, as required by upstream RMW. */
typedef struct {
 rmw_wait_set_t public_handle;uwvm_rmw_context*context;rmw_context_t*context_handle;uint64_t context_instance;
 rcutils_allocator_t allocator;size_t maximum,pins;int live,waiting;
 void**destinations[UWVM_RMW_C_MAX_CONDITIONS];
 uwvm_rmw_resource*resources[UWVM_RMW_C_MAX_CONDITIONS];uwvm_rmw_c_guard*guards[UWVM_RMW_C_MAX_CONDITIONS];
 void*addresses[UWVM_RMW_C_MAX_CONDITIONS];unsigned char ready[UWVM_RMW_C_MAX_CONDITIONS];
} local_wait_set;
static local_wait_set*sets[UWVM_RMW_C_WAIT_LIMIT];
static local_wait_set*find(rmw_wait_set_t const*h){for(size_t i=0;i<UWVM_RMW_C_WAIT_LIMIT;++i)if(sets[i]&&sets[i]->live&&&sets[i]->public_handle==h)return sets[i];return NULL;}
void uwvm_rmw_c_wait_drain(uwvm_rmw_context*c){
 for(size_t i=0;i<UWVM_RMW_C_WAIT_LIMIT;++i){local_wait_set*w=sets[i];
  if(w&&w->context==c&&!w->live&&!w->pins){sets[i]=NULL;rcutils_allocator_t a=w->allocator;
   a.deallocate(w,a.state);if(uwvm_rmw_context_child_release(c))__builtin_trap();}}
}
rmw_wait_set_t*rmw_create_wait_set(rmw_context_t*h,size_t maximum){
 UWVM_RMW_ENTRY(NULL);uwvm_rmw_context*c=uwvm_rmw_context_find(h,0);uwvm_rmw_scope scope;
 if(!c||maximum>UWVM_RMW_C_MAX_CONDITIONS){uwvm_rmw_error(c,c?90:22,"actual wait Context/capacity refused");return NULL;}
 if(uwvm_rmw_scope_enter(c,&scope,0)!=RMW_RET_OK)return NULL;
 size_t slot=0;while(slot<UWVM_RMW_C_WAIT_LIMIT&&sets[slot])++slot;
 if(slot==UWVM_RMW_C_WAIT_LIMIT){uwvm_rmw_error(c,90,"finite wait-set lifetime capacity exhausted");uwvm_rmw_scope_leave(&scope);return NULL;}
 uwvm_rmw_c_mark_started();local_wait_set*w=c->allocator.zero_allocate(1,sizeof*w,c->allocator.state);
 if(!w){uwvm_rmw_error(c,12,"wait-set storage allocation failed before effects");uwvm_rmw_scope_leave(&scope);return NULL;}
 uint32_t raw=uwvm_rmw_context_child_retain(c);
 if(raw){c->allocator.deallocate(w,c->allocator.state);uwvm_rmw_error(c,raw,"real wait Context lifetime refused");uwvm_rmw_scope_leave(&scope);return NULL;}
 w->context=c;w->context_handle=h;w->context_instance=c->local_instance;w->allocator=c->allocator;
 w->maximum=maximum?maximum:UWVM_RMW_C_MAX_CONDITIONS;w->live=1;
 w->public_handle.implementation_identifier=uwvm_rmw_identifier;w->public_handle.data=w;sets[slot]=w;
 uwvm_rmw_scope_leave(&scope);return &w->public_handle;
}
rmw_ret_t rmw_destroy_wait_set(rmw_wait_set_t*h){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);local_wait_set*w=find(h);if(!w)return h?RMW_RET_INCORRECT_RMW_IMPLEMENTATION:RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_context*c=w->context;uwvm_rmw_scope scope;rmw_ret_t rc=uwvm_rmw_scope_enter(c,&scope,1);if(rc!=RMW_RET_OK)return rc;
 /* Tombstone immediately. A real active wait keeps this object and Context
  * alive until its resume path observes retirement and releases its pin. */
 w->live=0;(void)uwvm_rmw_context_wake(c);uwvm_rmw_scope_leave(&scope);return RMW_RET_OK;
}
static uint32_t attach(local_wait_set*w,size_t*count,void**entries,size_t n,int kind){
 if(n&&!entries)return 22;if(n>w->maximum-*count)return 90;
 for(size_t i=0;i<n;++i){
  if(!entries[i])return 22;size_t at=*count;
  for(size_t j=0;j<at;++j)if(w->addresses[j]==entries[i])return 22;
  w->destinations[at]=entries+i;w->addresses[at]=entries[i];w->resources[at]=NULL;w->guards[at]=NULL;
  if(kind==0){uwvm_rmw_c_guard*g=uwvm_rmw_c_guard_find(entries[i]);
   if(!g||g->context!=w->context||g->context_instance!=w->context_instance)return 22;
   uint32_t raw=uwvm_rmw_c_guard_pin(g);if(raw)return raw;w->guards[at]=g;
  }else{uwvm_rmw_resource*r=uwvm_rmw_resource_find(entries[i],(enum uwvm_rmw_endpoint_kind)kind,0);
   if(!r||r->context!=w->context)return 22;uint32_t raw=uwvm_rmw_resource_pin(r);if(raw)return raw;w->resources[at]=r;}
  ++*count;
 }return 0;
}
static uint32_t observe(local_wait_set*w,size_t count,int*any,int*has_native){
 *any=0;*has_native=0;if(!w->live)return 9;
 for(size_t i=0;i<count;++i){int ready=0;uint32_t raw=0;
  if(w->guards[i]){if(!w->guards[i]->live)return 9;ready=uwvm_rmw_c_guard_ready(w->guards[i],0)!=0;}
  else {uwvm_rmw_resource*r=w->resources[i];*has_native=1;
   if(!r->live)return 9;
   /* No new native observation after logical Context close. Actual local
    * guard notification remains observable for original GraphListener join. */
   if(!uwvm_rmw_shutdown_requested(w->context)){
    if(r->kind==UWVM_RMW_SUBSCRIPTION)raw=uwvm_rmw_c_subscription_ready(r,&ready);
    else if(r->kind==UWVM_RMW_CLIENT)raw=uwvm_rmw_c_client_ready(r,&ready);
    else if(r->kind==UWVM_RMW_SERVICE)raw=uwvm_rmw_c_service_ready(r,&ready);else raw=22;
   }
  }
  if(raw)return raw;w->ready[i]=(unsigned char)(ready!=0);*any|=ready!=0;
 }
 if(!*any&&(*has_native||!count)&&uwvm_rmw_shutdown_requested(w->context))return 125;
 return 0;
}
rmw_ret_t rmw_wait(rmw_subscriptions_t*subscriptions,rmw_guard_conditions_t*guards,
 rmw_services_t*services,rmw_clients_t*clients,rmw_events_t*events,rmw_wait_set_t*h,rmw_time_t const*timeout){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);local_wait_set*w=find(h);if(!w)return h?RMW_RET_INCORRECT_RMW_IMPLEMENTATION:RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_context*c=uwvm_rmw_context_find(w->context_handle,1);
 if(!c||c!=w->context||c->local_instance!=w->context_instance)return RMW_RET_INCORRECT_RMW_IMPLEMENTATION;
 if(w->waiting)return uwvm_rmw_error(c,17,"same actual wait set has an active borrower");
 if(events&&events->event_count)return uwvm_rmw_error(c,95,"actual native status events require their independent provider");
 uint64_t duration=0;int infinite=timeout==NULL;
 if(timeout){uint64_t billion=UINT64_C(1000000000),maximum=(uint64_t)INT64_MAX;
  if(timeout->sec>maximum/billion||timeout->nsec>maximum-timeout->sec*billion)return uwvm_rmw_error(c,95,"actual finite signed steady range exceeded");
  duration=timeout->sec*billion+timeout->nsec;}
 uwvm_rmw_c_profile const*p=uwvm_rmw_c_configuration();
 if(duration&&(!p||p->wait.version!=1||duration>p->wait.maximum_wait_ns))return uwvm_rmw_error(c,95,"no explicitly bounded positive wait profile");
 uwvm_rmw_scope scope;rmw_ret_t result=uwvm_rmw_scope_enter(c,&scope,1);if(result!=RMW_RET_OK)return result;
 uwvm_rmw_context*pinned=uwvm_rmw_context_pin(w->context_handle);
 if(pinned!=c){if(pinned)uwvm_rmw_context_unpin(pinned);uwvm_rmw_scope_leave(&scope);return RMW_RET_INCORRECT_RMW_IMPLEMENTATION;}
 ++w->pins;w->waiting=1;++c->active_waits;
 uint32_t raw=0;size_t count=0,iterations=0;int retained=0,any=0,has_native=0;
 int64_t first=0,previous=0,now=0;uint64_t deadline=0;
 if(subscriptions)raw=attach(w,&count,subscriptions->subscribers,subscriptions->subscriber_count,UWVM_RMW_SUBSCRIPTION);
 if(!raw&&guards)raw=attach(w,&count,guards->guard_conditions,guards->guard_condition_count,0);
 if(!raw&&services)raw=attach(w,&count,services->services,services->service_count,UWVM_RMW_SERVICE);
 if(!raw&&clients)raw=attach(w,&count,clients->clients,clients->client_count,UWVM_RMW_CLIENT);
 if(raw){result=uwvm_rmw_error(c,raw,"whole wait list admission failed before observation");goto done;}
 if(duration){
  raw=uwvm_rmw_platform_retain();if(raw){result=uwvm_rmw_error(c,raw,"actual steady platform lifetime absent");goto done;}retained=1;
  raw=uwvm_rmw_platform_now(UWVM_RMW_STEADY_CLOCK,&first);
  if(raw||first<0||duration>(uint64_t)INT64_MAX-(uint64_t)first){result=uwvm_rmw_error(c,raw?raw:90,"actual wait deadline refused");goto done;}
  previous=first;deadline=(uint64_t)first+duration;
 }
 for(;;){
  uint32_t epoch=__atomic_load_n(&c->wake_epoch,__ATOMIC_ACQUIRE);
  raw=observe(w,count,&any,&has_native);
  if(raw){result=uwvm_rmw_error(c,raw,"actual wait object/Context retired or readiness failed");goto done;}
  if(any){result=RMW_RET_OK;break;}
  if(!infinite&&!duration){result=RMW_RET_TIMEOUT;break;}
  int64_t slice=has_native?INT64_C(1000000):-1;
  if(duration){
   raw=uwvm_rmw_platform_now(UWVM_RMW_STEADY_CLOCK,&now);
   if(raw||now<previous){result=uwvm_rmw_error(c,raw?raw:22,"real steady Clock error/regression");goto done;}previous=now;
   if((uint64_t)now>=deadline){result=RMW_RET_TIMEOUT;break;}
   if(iterations++>=p->wait.maximum_iterations){result=uwvm_rmw_error(c,110,"driver budget exhausted before actual deadline");goto done;}
   uint64_t remaining=deadline-(uint64_t)now,cap=p->wait.maximum_slice_ns;
   if(remaining<cap)cap=remaining;if(has_native&&cap>UINT64_C(1000000))cap=UINT64_C(1000000);
   slice=(int64_t)cap;
  }
  /* Fixed pins remain across the physical wait. scope_leave cannot release
   * their local storage. No provider/Host pointer or native callback is borrowed. */
  uwvm_rmw_scope_leave(&scope);uwvm_rmw_dispatch_suspend(&uwvm_dispatch_entry);
  raw=uwvm_rmw_dispatch_wait(&c->wake_epoch,epoch,slice);
  uwvm_rmw_dispatch_resume(&uwvm_dispatch_entry);
  result=uwvm_rmw_scope_enter(c,&scope,1);if(result!=RMW_RET_OK)__builtin_trap();
  if(raw){result=uwvm_rmw_error(c,raw,"physical wait/notification domain failed");goto done;}
 }
 done:
 if(retained){uint32_t code=uwvm_rmw_platform_release();if(code&&(result==RMW_RET_OK||result==RMW_RET_TIMEOUT))result=uwvm_rmw_error(c,code,"actual platform reference release failed");}
 /* Final fallible platform release precedes every user array write. A true
  * successful observation/actual measured timeout commits once; errors leave
  * caller arrays and retained binary guard notification unchanged. */
 if(result==RMW_RET_OK||result==RMW_RET_TIMEOUT)
  for(size_t i=0;i<count;++i){if(!w->ready[i])*w->destinations[i]=NULL;else if(w->guards[i])(void)uwvm_rmw_c_guard_ready(w->guards[i],1);}
 for(size_t i=0;i<count;++i){if(w->guards[i])uwvm_rmw_c_guard_unpin(w->guards[i]);else if(w->resources[i])uwvm_rmw_resource_unpin(w->resources[i]);}
 w->waiting=0;--w->pins;--c->active_waits;uwvm_rmw_context_unpin(c);
 uwvm_rmw_scope_leave(&scope);return result;
}
#else
typedef struct {
 rmw_wait_set_t public_handle;uwvm_rmw_context*context;rmw_context_t*context_handle;uint64_t context_instance;rcutils_allocator_t allocator;
 size_t maximum;int waiting;
 void**destinations[UWVM_RMW_C_MAX_CONDITIONS];
 uwvm_rmw_resource*resources[UWVM_RMW_C_MAX_CONDITIONS];
 uwvm_rmw_c_guard*guards[UWVM_RMW_C_MAX_CONDITIONS];
 void*addresses[UWVM_RMW_C_MAX_CONDITIONS];unsigned char ready[UWVM_RMW_C_MAX_CONDITIONS];
} local_wait_set;
static local_wait_set*sets[UWVM_RMW_C_WAIT_LIMIT];
static local_wait_set*find(rmw_wait_set_t const*p){
 for(size_t i=0;i<UWVM_RMW_C_WAIT_LIMIT;++i)if(sets[i] && &sets[i]->public_handle==p)return sets[i];return NULL;
}
rmw_wait_set_t*rmw_create_wait_set(rmw_context_t*handle,size_t maximum){
 UWVM_RMW_ENTRY(NULL);
 uwvm_rmw_context*c=uwvm_rmw_context_find(handle,0);uwvm_rmw_scope scope;
 if(!c){uwvm_rmw_error(NULL,22,"invalid real RMW wait context");return NULL;}
 if(maximum>UWVM_RMW_C_MAX_CONDITIONS){uwvm_rmw_error(c,90,"wait storage request exceeds all finite admitted local conditions");return NULL;}
 if(uwvm_rmw_scope_enter(c,&scope,0)!=RMW_RET_OK)return NULL;
 size_t slot=0;while(slot<UWVM_RMW_C_WAIT_LIMIT&&sets[slot])++slot;
 if(slot==UWVM_RMW_C_WAIT_LIMIT){uwvm_rmw_error(c,90,"finite wait-set lifetime storage exhausted");uwvm_rmw_scope_leave(&scope);return NULL;}
 uwvm_rmw_c_mark_started();
 local_wait_set*w=c->allocator.zero_allocate(1,sizeof*w,c->allocator.state);
 if(!w){uwvm_rmw_error(c,12,"wait-set allocation failed before readiness effects");uwvm_rmw_scope_leave(&scope);return NULL;}
 uint32_t retained=uwvm_rmw_context_child_retain(c);
 if(retained){c->allocator.deallocate(w,c->allocator.state);uwvm_rmw_scope_leave(&scope);uwvm_rmw_error(c,retained,"wait-set context lifetime reservation failed before exposure");return NULL;}
 w->allocator=c->allocator;w->context=c;w->context_handle=handle;w->context_instance=c->local_instance;w->maximum=maximum?maximum:UWVM_RMW_C_MAX_CONDITIONS;
 w->public_handle.implementation_identifier=uwvm_rmw_identifier;w->public_handle.data=w;sets[slot]=w;
 /* max_conditions0 accepts any number available in this actual bounded
  * implementation (at most256 guards plus256 endpoint resources per context).
  * No fixed application-supplied capacity replaces the zero/default meaning. */
 uwvm_rmw_scope_leave(&scope);return &w->public_handle;
}
rmw_ret_t rmw_destroy_wait_set(rmw_wait_set_t*handle){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
 local_wait_set*w=find(handle);if(!w)return handle?RMW_RET_INCORRECT_RMW_IMPLEMENTATION:RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_context*c=w->context;uwvm_rmw_scope scope;
 /* As in the genuine RMW contract, context lifetime must outlive this set. */
 uwvm_rmw_context*actual=uwvm_rmw_context_find(w->context_handle,1);
 if(!actual||actual!=c||actual->local_instance!=w->context_instance)return RMW_RET_INCORRECT_RMW_IMPLEMENTATION;
 rmw_ret_t code=uwvm_rmw_scope_enter(c,&scope,1);if(code!=RMW_RET_OK)return code;
 if(w->waiting){uwvm_rmw_scope_leave(&scope);return uwvm_rmw_error(c,17,"same wait set is currently borrowed");}
 rcutils_allocator_t allocator=w->allocator;
 for(size_t i=0;i<UWVM_RMW_C_WAIT_LIMIT;++i)if(sets[i]==w){sets[i]=NULL;break;}
 allocator.deallocate(w,allocator.state);uint32_t raw=uwvm_rmw_context_child_release(c);
 uwvm_rmw_scope_leave(&scope);return raw?uwvm_rmw_error(c,raw,"retired wait-set context lifetime release failed"):RMW_RET_OK;
}
static uint32_t attach(local_wait_set*w,size_t*count,void**entries,size_t n,int kind){
 if(n&&!entries)return 22;
 if(n>w->maximum-*count)return 90;
 for(size_t i=0;i<n;++i){
  if(!entries[i])return 22; /* Original rmw_wait explicitly rejects NULL input entries. */
  size_t at=*count;
  for(size_t j=0;j<at;++j)if(w->addresses[j]==entries[i])return 22;
  w->destinations[at]=entries+i;w->addresses[at]=entries[i];w->resources[at]=NULL;w->guards[at]=NULL;
  if(kind==0){
   uwvm_rmw_c_guard*g=uwvm_rmw_c_guard_find(entries[i]);if(!g||!g->live||g->context!=w->context||g->context_instance!=w->context_instance)return 22;w->guards[at]=g;
  }else{
   uwvm_rmw_resource*r=uwvm_rmw_resource_find(entries[i],(enum uwvm_rmw_endpoint_kind)kind,0);
   if(!r||r->context!=w->context)return 22;w->resources[at]=r;
  }
  ++*count;
 }return 0;
}
static uint32_t observe(local_wait_set*w,size_t count,int*any){
 *any=0;
 for(size_t i=0;i<count;++i){int ready=0;uint32_t raw=0;
  if(w->guards[i])ready=uwvm_rmw_c_guard_ready(w->guards[i],0)!=0;
  else if(w->resources[i]->kind==UWVM_RMW_SUBSCRIPTION)raw=uwvm_rmw_c_subscription_ready(w->resources[i],&ready);
  else if(w->resources[i]->kind==UWVM_RMW_CLIENT)raw=uwvm_rmw_c_client_ready(w->resources[i],&ready);
  else if(w->resources[i]->kind==UWVM_RMW_SERVICE)raw=uwvm_rmw_c_service_ready(w->resources[i],&ready);
  else raw=22;
  if(raw)return raw;w->ready[i]=(unsigned char)(ready!=0);*any|=ready!=0;
 }return 0;
}
rmw_ret_t rmw_wait(rmw_subscriptions_t*subscriptions,rmw_guard_conditions_t*guards,
 rmw_services_t*services,rmw_clients_t*clients,rmw_events_t*events,rmw_wait_set_t*handle,rmw_time_t const*timeout){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
 local_wait_set*w=find(handle);if(!w)return handle?RMW_RET_INCORRECT_RMW_IMPLEMENTATION:RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_context*c=uwvm_rmw_context_find(w->context_handle,0);
 if(!c||c!=w->context||c->local_instance!=w->context_instance)return RMW_RET_INCORRECT_RMW_IMPLEMENTATION;
 uwvm_rmw_scope scope;rmw_ret_t result;uint32_t raw=0;size_t count=0,iterations=0;int any=0,retained=0;
 if(!timeout)return uwvm_rmw_error(c,95,"indefinite native wait is not implemented by the finite explicit provider");
 /* Original rmw_time_t permits non-normalized nsec. Compare the algebraic
  * total before either multiply or add: representations such as {1,7} and
  * {0,1000000007} are the same duration, without unsigned overflow. A valid
  * duration outside this finite signed-monotonic implementation is unsupported
  * (95), not an invalid RMW argument and not silently saturated/shortened. */
 uint64_t const billion=UINT64_C(1000000000),maximum=(uint64_t)INT64_MAX;
 if(timeout->sec>maximum/billion)return uwvm_rmw_error(c,95,"valid RMW wait exceeds the finite signed monotonic range");
 uint64_t seconds_ns=timeout->sec*billion;
 if(timeout->nsec>maximum-seconds_ns)return uwvm_rmw_error(c,95,"valid RMW wait exceeds the finite signed monotonic range");
 uint64_t duration=seconds_ns+timeout->nsec;
 if(events&&events->event_count)return uwvm_rmw_error(c,95,"native QoS/status events need their actual independent provider");
 uwvm_rmw_c_profile const*p=uwvm_rmw_c_configuration();
 if(duration&&(!p||p->wait.version!=1||duration>p->wait.maximum_wait_ns))return uwvm_rmw_error(c,95,"no admitted real finite wait provider for this duration");
 if(w->waiting)return uwvm_rmw_error(c,17,"same actual RMW wait set is already borrowed");
 result=uwvm_rmw_scope_enter(c,&scope,0);if(result!=RMW_RET_OK)return result;w->waiting=1;
 /* Validate the COMPLETE list before any queue can be consumed into owned
  * storage. All type-erased entries are admitted addresses, not trusted casts. */
 if(subscriptions)raw=attach(w,&count,subscriptions->subscribers,subscriptions->subscriber_count,UWVM_RMW_SUBSCRIPTION);
 if(!raw&&guards)raw=attach(w,&count,guards->guard_conditions,guards->guard_condition_count,0);
 if(!raw&&services)raw=attach(w,&count,services->services,services->service_count,UWVM_RMW_SERVICE);
 if(!raw&&clients)raw=attach(w,&count,clients->clients,clients->client_count,UWVM_RMW_CLIENT);
 if(raw){result=uwvm_rmw_error(c,raw,"complete wait entity/context/capacity admission failed");goto done;}
 int64_t first=0,previous=0,now=0;uint64_t deadline=0;
 if(duration){
  raw=uwvm_rmw_platform_retain();if(raw){result=uwvm_rmw_error(c,raw,"actual A platform lifetime is not prepared");goto done;}retained=1;
  raw=uwvm_rmw_platform_now(UWVM_RMW_STEADY_CLOCK,&first);
  if(raw||first<0||duration>(uint64_t)INT64_MAX-(uint64_t)first){result=uwvm_rmw_error(c,raw?raw:90,"actual wait start/deadline refused");goto done;}
  deadline=(uint64_t)first+duration;previous=first;
 }
 for(;;){
  raw=observe(w,count,&any);if(raw){result=uwvm_rmw_error(c,raw,"real readiness/owned queue observation failed without replay");goto done;}
  if(any){result=RMW_RET_OK;break;}
  if(!duration){result=RMW_RET_TIMEOUT;break;}
  raw=uwvm_rmw_platform_now(UWVM_RMW_STEADY_CLOCK,&now);
  if(raw||now<previous){result=uwvm_rmw_error(c,raw?raw:22,"actual steady wait clock failed/rolled back");goto done;}previous=now;
  if((uint64_t)now>=deadline){result=RMW_RET_TIMEOUT;break;}
  if(iterations++>=p->wait.maximum_iterations){result=uwvm_rmw_error(c,110,"finite driver budget ended before measured deadline; not a fabricated timeout");goto done;}
  uint64_t remaining=deadline-(uint64_t)now,slice=remaining<p->wait.maximum_slice_ns?remaining:p->wait.maximum_slice_ns;
  raw=p->wait.wait_slice(p->wait.state,slice);
  if(raw){result=uwvm_rmw_error(c,raw,"actual finite Host wait driver failed; no fallback or replay");goto done;}
 }
 /* Only a complete successful observation/actual timeout updates the user's
  * array. A retained trigger is consumed exactly for a returned ready guard. */
 for(size_t i=0;i<count;++i){if(!w->ready[i])*w->destinations[i]=NULL;else if(w->guards[i])(void)uwvm_rmw_c_guard_ready(w->guards[i],1);}
 done:
 if(retained){uint32_t close=uwvm_rmw_platform_release();if(close&&(result==RMW_RET_OK||result==RMW_RET_TIMEOUT))result=uwvm_rmw_error(c,close,"platform wait borrower release failed");}
 w->waiting=0;uwvm_rmw_scope_leave(&scope);return result;
}

#endif /* explicit threaded profile */
