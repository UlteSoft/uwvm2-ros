/* SPDX-License-Identifier: Apache-2.0
 * Real Guest-local binary guard state, not a native graph subscription. Guard
 * scopes end at this RMW call boundary. No application callback runs here and
 * no artificial lease remains after rmw_wait/take returns to real upstream. */
#include "c_internal.h"
#if UWVM_RMW_ENABLE_THREADED_PROFILE
static uwvm_rmw_c_guard*guards[UWVM_RMW_C_GUARD_LIMIT];
uwvm_rmw_c_guard*uwvm_rmw_c_guard_find(void const*address){
 /* Private business-owned lookup. Fast trigger has its own locked admission. */
 if(!address)return NULL;
 for(size_t i=0;i<UWVM_RMW_C_GUARD_LIMIT;++i)if(guards[i]&&guards[i]->live&&&guards[i]->public_handle==address)return guards[i];
 return NULL;
}
uint32_t uwvm_rmw_c_guard_pin(uwvm_rmw_c_guard*g){uwvm_rmw_registry_lock();if(g->pins==SIZE_MAX){uwvm_rmw_registry_unlock();return 90;}if(g->wait_borrowed){uwvm_rmw_registry_unlock();return 17;}g->wait_borrowed=1;++g->pins;uwvm_rmw_registry_unlock();return 0;}
void uwvm_rmw_c_guard_unpin(uwvm_rmw_c_guard*g){uwvm_rmw_registry_lock();if(!g->pins||!g->wait_borrowed)__builtin_trap();g->wait_borrowed=0;--g->pins;uwvm_rmw_registry_unlock();}
void uwvm_rmw_c_guard_drain(uwvm_rmw_context*c){
 for(size_t i=0;i<UWVM_RMW_C_GUARD_LIMIT;++i){
  uwvm_rmw_registry_lock();uwvm_rmw_c_guard*g=guards[i];
  int retire=g&&g->context==c&&!g->live&&!g->pins;
  if(retire)guards[i]=NULL;uwvm_rmw_registry_unlock();
  if(retire){rcutils_allocator_t a=g->allocator;
   /* Context child remains until after the actual final allocator callback. */
   a.deallocate(g,a.state);if(uwvm_rmw_context_child_release(c))__builtin_trap();}
 }
}
static rmw_guard_condition_t*create_private(rmw_context_t*handle){
 uwvm_rmw_context*c=uwvm_rmw_context_find(handle,0);size_t slot=0,count=0;
 if(!c||!c->leased){uwvm_rmw_error(c,17,"private graph guard needs the admitted existing scope");return NULL;}
 for(size_t i=0;i<UWVM_RMW_C_GUARD_LIMIT;++i)if(guards[i]&&guards[i]->context==c)++count;
 while(slot<UWVM_RMW_C_GUARD_LIMIT&&guards[slot])++slot;
 if(count>=UWVM_RMW_MAX_OWNERS||slot==UWVM_RMW_C_GUARD_LIMIT){uwvm_rmw_error(c,90,"finite actual guard ownership exhausted");return NULL;}
 uwvm_rmw_c_mark_started();uwvm_rmw_c_guard*g=c->allocator.zero_allocate(1,sizeof*g,c->allocator.state);
 if(!g){uwvm_rmw_error(c,12,"local guard allocation failed");return NULL;}
 uint32_t raw=uwvm_rmw_context_child_retain(c);
 if(raw){c->allocator.deallocate(g,c->allocator.state);uwvm_rmw_error(c,raw,"guard context retain failed before exposure");return NULL;}
 g->context=c;g->context_instance=c->local_instance;g->allocator=c->allocator;g->live=1;atomic_init(&g->triggered,0);
 g->public_handle.implementation_identifier=uwvm_rmw_identifier;g->public_handle.data=g;g->public_handle.context=handle;
 uwvm_rmw_registry_lock();guards[slot]=g;uwvm_rmw_registry_unlock();return &g->public_handle;
}
static rmw_ret_t destroy_private(rmw_guard_condition_t*handle){
 uwvm_rmw_c_guard*g=uwvm_rmw_c_guard_find(handle);if(!g)return RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_context*c=g->context;
 if(!c->leased||c->local_instance!=g->context_instance)return uwvm_rmw_error(c,17,"guard retirement needs admitted scope");
 uwvm_rmw_registry_lock();g->live=0;uwvm_rmw_registry_unlock();
 (void)uwvm_rmw_context_wake(c);uwvm_rmw_c_guard_drain(c);return RMW_RET_OK;
}
static rmw_guard_condition_t*provider_create(rmw_context_t*c,void*state){(void)state;return create_private(c);}
static rmw_ret_t provider_destroy(rmw_guard_condition_t*g,void*state){(void)state;return destroy_private(g);}
static rmw_ret_t provider_notify(rmw_guard_condition_t const*handle,void*state){
 (void)state;uwvm_rmw_c_guard*g=NULL;uwvm_rmw_context*c=NULL;
 /* No business mutex: this must wake GraphListener even if another thread is
  * in bounded Host IO. Address admission and both lifetime pins are atomic with
  * respect to retirement. No caller data/public ::data field is dereferenced. */
 uwvm_rmw_registry_lock();
 for(size_t i=0;i<UWVM_RMW_C_GUARD_LIMIT;++i)if(guards[i]&&guards[i]->live&&&guards[i]->public_handle==handle){
  g=guards[i];c=g->context;
  if(g->pins==SIZE_MAX||c->asynchronous_pins==SIZE_MAX){g=NULL;c=NULL;break;}
  ++g->pins;++c->asynchronous_pins;break;
 }
 uwvm_rmw_registry_unlock();if(!g)return RMW_RET_INVALID_ARGUMENT;
 atomic_store_explicit(&g->triggered,1,memory_order_release);
 uint32_t raw=uwvm_rmw_context_wake(c);
 uwvm_rmw_registry_lock();--g->pins;--c->asynchronous_pins;uwvm_rmw_registry_unlock();
 return raw?RMW_RET_ERROR:RMW_RET_OK;
}
void uwvm_rmw_c_shutdown_guards(uwvm_rmw_context*c){
 /* Called under registry lock. Actual local guard notification, no native
  * graph query or implicit endpoint authority. Never calls application code. */
 for(size_t i=0;i<UWVM_RMW_C_GUARD_LIMIT;++i)if(guards[i]&&guards[i]->live&&guards[i]->context==c)
  atomic_store_explicit(&guards[i]->triggered,1,memory_order_release);
}
uwvm_rmw_graph_guard_provider uwvm_rmw_c_graph_provider(void){
 return (uwvm_rmw_graph_guard_provider){UWVM_RMW_CONTRACT_VERSION,NULL,provider_create,provider_destroy,provider_notify};
}
int uwvm_rmw_c_graph_provider_matches(uwvm_rmw_graph_guard_provider const*p){
 return p&&p->version==UWVM_RMW_CONTRACT_VERSION&&p->state==NULL&&p->create==provider_create&&p->destroy==provider_destroy&&p->notify==provider_notify;
}
rmw_guard_condition_t*rmw_create_guard_condition(rmw_context_t*handle){
 UWVM_RMW_ENTRY(NULL);uwvm_rmw_context*c=uwvm_rmw_context_find(handle,0);uwvm_rmw_scope scope;
 if(!c){uwvm_rmw_error(NULL,22,"invalid actual guard Context");return NULL;}
 if(uwvm_rmw_scope_enter(c,&scope,0)!=RMW_RET_OK)return NULL;
 rmw_guard_condition_t*g=create_private(handle);uwvm_rmw_scope_leave(&scope);return g;
}
rmw_ret_t rmw_destroy_guard_condition(rmw_guard_condition_t*handle){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);uwvm_rmw_c_guard*g=uwvm_rmw_c_guard_find(handle);
 if(!g)return handle?RMW_RET_INCORRECT_RMW_IMPLEMENTATION:RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_scope scope;rmw_ret_t code=uwvm_rmw_scope_enter(g->context,&scope,1);if(code!=RMW_RET_OK)return code;
 code=destroy_private(handle);uwvm_rmw_scope_leave(&scope);return code;
}
rmw_ret_t rmw_trigger_guard_condition(rmw_guard_condition_t const*handle){return provider_notify(handle,NULL);}
uint32_t uwvm_rmw_c_guard_ready(uwvm_rmw_c_guard*g,int consume){
 if(!g||!g->live)return 0;
 return consume?atomic_exchange_explicit(&g->triggered,0,memory_order_acq_rel):atomic_load_explicit(&g->triggered,memory_order_acquire);
}
#else
static uwvm_rmw_c_guard*guards[UWVM_RMW_C_GUARD_LIMIT];
uwvm_rmw_c_guard*uwvm_rmw_c_guard_find(void const*address){
 if(!address)return NULL;
 for(size_t i=0;i<UWVM_RMW_C_GUARD_LIMIT;++i)
  if(guards[i]&&(void const*)&guards[i]->public_handle==address)return guards[i];
 return NULL;
}
static rmw_guard_condition_t*create_private(rmw_context_t*handle){
 uwvm_rmw_context*c=uwvm_rmw_context_find(handle,0);size_t slot=0;
 if(!c||!c->leased){uwvm_rmw_error(c,17,"private local guard creation requires the existing admitted scope");return NULL;}
 size_t context_count=0;for(size_t i=0;i<UWVM_RMW_C_GUARD_LIMIT;++i)if(guards[i]&&guards[i]->context==c)++context_count;
 if(context_count>=UWVM_RMW_MAX_OWNERS){uwvm_rmw_error(c,90,"per-context actual guard capacity exhausted");return NULL;}
 while(slot<UWVM_RMW_C_GUARD_LIMIT&&guards[slot])++slot;
 if(slot==UWVM_RMW_C_GUARD_LIMIT){uwvm_rmw_error(c,90,"finite local guard storage exhausted before allocation");return NULL;}
 /* Allocator callbacks cannot recursively create/destroy public owners because
  * the original B scope is already held. No native resource is opened. */
 uwvm_rmw_c_mark_started();
 uwvm_rmw_c_guard*g=c->allocator.zero_allocate(1,sizeof*g,c->allocator.state);
 if(!g){uwvm_rmw_error(c,12,"local guard allocation failed");return NULL;}
 uint32_t retained=uwvm_rmw_context_child_retain(c);
 if(retained){c->allocator.deallocate(g,c->allocator.state);uwvm_rmw_error(c,retained,"guard context lifetime reservation failed before exposure");return NULL;}
 g->context=c;g->context_instance=c->local_instance;g->allocator=c->allocator;g->live=1;atomic_init(&g->triggered,0u);
 g->public_handle.implementation_identifier=uwvm_rmw_identifier;
 g->public_handle.data=g;g->public_handle.context=handle;guards[slot]=g;
 return &g->public_handle;
}
static rmw_ret_t destroy_private(rmw_guard_condition_t*handle){
 uwvm_rmw_c_guard*g=uwvm_rmw_c_guard_find(handle);
 if(!g||!g->live)return RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_context*c=uwvm_rmw_context_find(g->public_handle.context,1);
 if(!c||c!=g->context||c->local_instance!=g->context_instance||!c->leased)return uwvm_rmw_error(c,17,"private guard destruction requires its admitted existing scope");
 rcutils_allocator_t allocator=g->allocator;
 for(size_t i=0;i<UWVM_RMW_C_GUARD_LIMIT;++i)if(guards[i]==g){guards[i]=NULL;break;}
 g->live=0;atomic_store_explicit(&g->triggered,0u,memory_order_release);
 /* Retire admission before invoking an application allocator. All attempted
  * normal cleanup is once only; arbitrary traps need external instance drain. */
 allocator.deallocate(g,allocator.state);
 uint32_t raw=uwvm_rmw_context_child_release(c);
 return raw?uwvm_rmw_error(c,raw,"retired guard context lifetime release failed"):RMW_RET_OK;
}
static rmw_guard_condition_t*provider_create(rmw_context_t*c,void*state){(void)state;return create_private(c);}
static rmw_ret_t provider_destroy(rmw_guard_condition_t*g,void*state){(void)state;return destroy_private(g);}
static rmw_ret_t provider_notify(rmw_guard_condition_t const*handle,void*state){
 (void)state;uwvm_rmw_c_guard*g=uwvm_rmw_c_guard_find(handle);
 if(!g||!g->live)return RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_context*c=uwvm_rmw_context_find(g->public_handle.context,1);
 if(!c||c!=g->context||c->local_instance!=g->context_instance)return RMW_RET_INCORRECT_RMW_IMPLEMENTATION;
 atomic_store_explicit(&g->triggered,1u,memory_order_release);return RMW_RET_OK;
}
uwvm_rmw_graph_guard_provider uwvm_rmw_c_graph_provider(void){
 uwvm_rmw_graph_guard_provider result={UWVM_RMW_CONTRACT_VERSION,NULL,provider_create,provider_destroy,provider_notify};return result;
}
rmw_guard_condition_t*rmw_create_guard_condition(rmw_context_t*handle){
 UWVM_RMW_ENTRY(NULL);
 uwvm_rmw_context*c=uwvm_rmw_context_find(handle,0);uwvm_rmw_scope scope;
 if(!c){uwvm_rmw_error(NULL,22,"invalid genuine RMW guard context");return NULL;}
 if(uwvm_rmw_scope_enter(c,&scope,0)!=RMW_RET_OK)return NULL;
 rmw_guard_condition_t*g=create_private(handle);uwvm_rmw_scope_leave(&scope);return g;
}
rmw_ret_t rmw_destroy_guard_condition(rmw_guard_condition_t*handle){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
 uwvm_rmw_c_guard*g=uwvm_rmw_c_guard_find(handle);
 if(!g)return handle?RMW_RET_INCORRECT_RMW_IMPLEMENTATION:RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_context*c=uwvm_rmw_context_find(g->public_handle.context,1);uwvm_rmw_scope scope;
 if(!c||c!=g->context||c->local_instance!=g->context_instance)return RMW_RET_INCORRECT_RMW_IMPLEMENTATION;
 rmw_ret_t code=uwvm_rmw_scope_enter(c,&scope,1);if(code!=RMW_RET_OK)return code;
 code=destroy_private(handle);uwvm_rmw_scope_leave(&scope);return code;
}
rmw_ret_t rmw_trigger_guard_condition(rmw_guard_condition_t const*handle){
 /* Triggering an existing retained guard is allocation-free notification, not
  * an owner mutation. It is permitted during an actual wait scope. Destroy
  * remains excluded until that scope ends; original upper lifetimes still apply.
  * This single-threaded WASI cohort does not claim concurrent registry teardown. */
 return provider_notify(handle,NULL);
}
uint32_t uwvm_rmw_c_guard_ready(uwvm_rmw_c_guard*g,int consume){
 if(!g||!g->live)return 9;
 return consume?atomic_exchange_explicit(&g->triggered,0u,memory_order_acq_rel):
  atomic_load_explicit(&g->triggered,memory_order_acquire);
}

#endif /* explicit threaded profile */
