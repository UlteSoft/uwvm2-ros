/* SPDX-License-Identifier: Apache-2.0
 * Actual RMW init/options/context implementation. Common zero constructors and
 * allocator/security/discovery utilities remain the ORIGINAL rmw library's
 * symbols; this file does not duplicate or fake their declarations. */
#include "uwvm_rmw/internal.h"
#include <string.h>
#include <rmw/error_handling.h>
#include <rcutils/strdup.h>
extern uwvm_rmw_profile const * uwvm_rmw_selected_profile;
extern size_t uwvm_rmw_live_options,uwvm_rmw_live_contexts;
char const uwvm_rmw_identifier[]="uwvm_capability_rmw_v1";
struct rmw_init_options_impl_s {uwvm_rmw_profile const * profile;rcutils_allocator_t allocator;
#if UWVM_RMW_ENABLE_THREADED_PROFILE
 rmw_init_options_t const*public_address;
#endif
};
static rmw_init_options_impl_t * option_registry[64];
static uwvm_rmw_context * context_registry[UWVM_RMW_MAX_CONTEXTS];
static uint64_t next_process_instance=1;
static rmw_init_options_impl_t * options_find(rmw_init_options_t const * options) {
  if(!options)return NULL;
  for(size_t i=0;i<64;++i)if(
#if UWVM_RMW_ENABLE_THREADED_PROFILE
    option_registry[i]&&option_registry[i]->public_address==options&&
#endif
    options->impl&&option_registry[i]==options->impl&&
    options->implementation_identifier==uwvm_rmw_identifier)return options->impl;
  return NULL;
}
static int context_known(uwvm_rmw_context const * c) {
  for(size_t i=0;i<UWVM_RMW_MAX_CONTEXTS;++i)if(c&&context_registry[i]==c)return 1;
  return 0;
}
static int valid_scope(uwvm_rmw_scope const * s) {
  return s&&s->active&&context_known(s->context)&&s->context->leased;
}
static int resource_known(uwvm_rmw_context const * c,uwvm_rmw_resource const * r) {
  for(size_t i=0;i<c->resource_count;++i)if(c->resources+i==r)return 1;
  return 0;
}
static int node_known(uwvm_rmw_context const * c,uwvm_rmw_node const * n) {
  for(size_t i=0;i<c->node_count;++i)if(c->nodes+i==n)return 1;
  return 0;
}
char const * rmw_get_implementation_identifier(void){return uwvm_rmw_identifier;}
char const * rmw_get_serialization_format(void){return "uwvm_ros_canonical_v1";}
rmw_ret_t uwvm_rmw_error(uwvm_rmw_context * c,uint32_t raw,char const * message) {
  if(c&&!c->diagnostics.first_raw_primary)c->diagnostics.first_raw_primary=raw;
  RMW_SET_ERROR_MSG(message);
  if(raw==12)return RMW_RET_BAD_ALLOC;
  if(raw==22)return RMW_RET_INVALID_ARGUMENT;
  if(raw==95)return RMW_RET_UNSUPPORTED;
  return RMW_RET_ERROR;
}
static rmw_ret_t options_init_private(rmw_init_options_t * options,rcutils_allocator_t allocator) {
  if(!options||!rcutils_allocator_is_valid(&allocator)||options->impl||options->implementation_identifier||
    options->enclave||options->security_options.security_root_path||options->discovery_options.static_peers) {
    RMW_SET_ERROR_MSG("RMW options must be zero initialized with a valid allocator");return RMW_RET_INVALID_ARGUMENT;
  }
  if(!uwvm_rmw_selected_profile) {
    RMW_SET_ERROR_MSG("No explicit capability RMW selection (raw13)");return RMW_RET_ERROR;
  }
  size_t slot=0;while(slot<64&&option_registry[slot])++slot;
  if(slot==64){RMW_SET_ERROR_MSG("RMW option capacity exhausted");return RMW_RET_BAD_ALLOC;}
  if(rmw_initialize_error_handling_thread_local_storage(allocator)!=RCUTILS_RET_OK)return RMW_RET_BAD_ALLOC;
  rmw_init_options_impl_t * impl=allocator.allocate(sizeof(*impl),allocator.state);
  if(!impl){RMW_SET_ERROR_MSG("allocating RMW options failed");return RMW_RET_BAD_ALLOC;}
  impl->profile=uwvm_rmw_selected_profile;impl->allocator=allocator;
#if UWVM_RMW_ENABLE_THREADED_PROFILE
  impl->public_address=options;
#endif
  rmw_init_options_t next=rmw_get_zero_initialized_init_options();
  next.impl=impl;next.implementation_identifier=uwvm_rmw_identifier;next.allocator=allocator;
  /* No DDS discovery is performed by this implementation. OFF is an explicit
   * capability middleware option, not a NodeOptions rewrite or a graph claim.
   * A must separately constrain original rcl environment reads. */
  next.domain_id=impl->profile->domain_id;
  next.discovery_options.automatic_discovery_range=RMW_AUTOMATIC_DISCOVERY_RANGE_OFF;
  next.discovery_options.allocator=allocator;
  option_registry[slot]=impl;++uwvm_rmw_live_options;*options=next;return RMW_RET_OK;
}
static rmw_ret_t options_fini_private(rmw_init_options_t * options) {
  rmw_init_options_impl_t * impl=options_find(options);
  if(!impl){RMW_SET_ERROR_MSG("foreign/uninitialized RMW options");return RMW_RET_INVALID_ARGUMENT;}
  rcutils_allocator_t a=impl->allocator;
  rmw_ret_t first=rmw_security_options_fini(&options->security_options,&a);
  rmw_ret_t code=rmw_discovery_options_fini(&options->discovery_options);
  if(first==RMW_RET_OK)first=code;
  a.deallocate(options->enclave,a.state);
  for(size_t i=0;i<64;++i)if(option_registry[i]==impl){option_registry[i]=NULL;break;}
  a.deallocate(impl,a.state);--uwvm_rmw_live_options;
  *options=rmw_get_zero_initialized_init_options();return first;
}
static rmw_ret_t options_copy_private(rmw_init_options_t const * src,rmw_init_options_t * dst) {
  rmw_init_options_impl_t * old=options_find(src);
  if(!old||!dst||src==dst||dst->impl||dst->implementation_identifier||dst->enclave||
    dst->security_options.security_root_path||dst->discovery_options.static_peers) {
    RMW_SET_ERROR_MSG("RMW options copy requires an owned source and zero destination");return RMW_RET_INVALID_ARGUMENT;
  }
  rmw_init_options_t next=rmw_get_zero_initialized_init_options();
  rmw_ret_t result=options_init_private(&next,old->allocator);if(result!=RMW_RET_OK)return result;
  next.instance_id=src->instance_id;next.domain_id=src->domain_id;next.localhost_only=src->localhost_only;
  result=rmw_security_options_copy(&src->security_options,&old->allocator,&next.security_options);
  if(result==RMW_RET_OK) {
    result=rmw_discovery_options_copy(&src->discovery_options,&old->allocator,&next.discovery_options);
  }
  if(result==RMW_RET_OK&&src->enclave) {
    next.enclave=rcutils_strdup(src->enclave,old->allocator);
    if(!next.enclave)result=RMW_RET_BAD_ALLOC;
  }
  if(result!=RMW_RET_OK) {
    /* All fields are owned/initialized with the retained valid allocator. Every
     * local cleanup is attempted, then the original copy status is returned. */
    (void)options_fini_private(&next);
    RMW_SET_ERROR_MSG("copying complete RMW options failed");return result;
  }
  *dst=next;
#if UWVM_RMW_ENABLE_THREADED_PROFILE
  next.impl->public_address=dst;
#endif
  return RMW_RET_OK;
}
rmw_ret_t rmw_init_options_init(rmw_init_options_t*o,rcutils_allocator_t a){UWVM_RMW_ENTRY(RMW_RET_ERROR);return options_init_private(o,a);}
rmw_ret_t rmw_init_options_fini(rmw_init_options_t*o){UWVM_RMW_ENTRY(RMW_RET_ERROR);return options_fini_private(o);}
rmw_ret_t rmw_init_options_copy(rmw_init_options_t const*s,rmw_init_options_t*d){UWVM_RMW_ENTRY(RMW_RET_ERROR);return options_copy_private(s,d);}
#if UWVM_RMW_ENABLE_THREADED_PROFILE
/* Fast paths use real registry pins, never an unadmitted caller data pointer. */
uwvm_rmw_context*uwvm_rmw_context_pin(rmw_context_t const*h){
 uwvm_rmw_context*out=NULL;uwvm_rmw_registry_lock();
 for(size_t i=0;i<UWVM_RMW_MAX_CONTEXTS;++i){uwvm_rmw_context*c=context_registry[i];if(c&&c->public_handle==h){
  if(c->asynchronous_pins!=SIZE_MAX){++c->asynchronous_pins;out=c;}break;}}
 uwvm_rmw_registry_unlock();return out;
}
void uwvm_rmw_context_unpin(uwvm_rmw_context*c){
 uwvm_rmw_registry_lock();if(!context_known(c)||!c->asynchronous_pins)__builtin_trap();
 --c->asynchronous_pins;uwvm_rmw_registry_unlock();
}
uint32_t uwvm_rmw_context_wake(uwvm_rmw_context*c){return uwvm_rmw_dispatch_notify(&c->wake_epoch);}
uint32_t uwvm_rmw_resource_pin(uwvm_rmw_resource*r){uwvm_rmw_registry_lock();if(r->wait_pins){uwvm_rmw_registry_unlock();return 17;}++r->wait_pins;uwvm_rmw_registry_unlock();return 0;}
void uwvm_rmw_resource_unpin(uwvm_rmw_resource*r){uwvm_rmw_registry_lock();if(!r->wait_pins)__builtin_trap();--r->wait_pins;uwvm_rmw_registry_unlock();}
static void release_local(uwvm_rmw_context*c,uwvm_rmw_resource*r){
 if(!r->release)return;
 uwvm_rmw_release_function release=r->release;void*object=r->object;r->release=NULL;r->object=NULL;
 uint32_t code=release(object);r->local_release_code=code;
 c->diagnostics.local_release_codes[(size_t)(r-c->resources)]=code;
 if(code)++c->diagnostics.local_retirement_failures;
}
void uwvm_rmw_deferred_drain(uwvm_rmw_context*c){
 /* business gate excludes every other deferred-free consumer. No registry lock
  * spans callbacks. Pin counters are read under registry before local release. */
 for(size_t i=0;i<c->resource_count;++i){uwvm_rmw_resource*r=c->resources+i;
  uwvm_rmw_registry_lock();int ready=!r->live&&!r->wait_pins&&r->release;uwvm_rmw_registry_unlock();
  if(ready)release_local(c,r);
 }
 uwvm_rmw_c_guard_drain(c);uwvm_rmw_c_wait_drain(c);
}
#endif
uwvm_rmw_context * uwvm_rmw_context_find(rmw_context_t const * handle,int allow_shutdown) {
#if UWVM_RMW_ENABLE_THREADED_PROFILE
  if(!uwvm_rmw_dispatch_owned())return NULL;
#endif
  for(size_t i=0;i<UWVM_RMW_MAX_CONTEXTS;++i) {
    uwvm_rmw_context * c=context_registry[i];
    if(c&&c->public_handle==handle&&(allow_shutdown||!uwvm_rmw_shutdown_requested(c)))return c;
  }
  return NULL;
}
uwvm_rmw_node * uwvm_rmw_node_find(rmw_node_t const * handle,int allow_shutdown) {
#if UWVM_RMW_ENABLE_THREADED_PROFILE
  if(!uwvm_rmw_dispatch_owned())return NULL;
#endif
  for(size_t i=0;i<UWVM_RMW_MAX_CONTEXTS;++i) {
    uwvm_rmw_context * c=context_registry[i];if(!c||(!allow_shutdown&&uwvm_rmw_shutdown_requested(c)))continue;
    for(size_t j=0;j<c->node_count;++j)if(c->nodes[j].live&& &c->nodes[j].public_handle==handle)return c->nodes+j;
  }
  return NULL;
}
uwvm_rmw_resource * uwvm_rmw_resource_find(void const * handle,enum uwvm_rmw_endpoint_kind kind,int allow_shutdown) {
#if UWVM_RMW_ENABLE_THREADED_PROFILE
  if(!uwvm_rmw_dispatch_owned())return NULL;
#endif
  for(size_t i=0;i<UWVM_RMW_MAX_CONTEXTS;++i) {
    uwvm_rmw_context * c=context_registry[i];if(!c||(!allow_shutdown&&uwvm_rmw_shutdown_requested(c)))continue;
    for(size_t j=0;j<c->resource_count;++j) {
      uwvm_rmw_resource * r=c->resources+j;if(r->live&&r->object==handle&&r->kind==kind)return r;
    }
  }
  return NULL;
}
rmw_ret_t uwvm_rmw_scope_enter(uwvm_rmw_context * c,uwvm_rmw_scope * scope,int allow_shutdown) {
#if UWVM_RMW_ENABLE_THREADED_PROFILE
  if(!uwvm_rmw_dispatch_owned())return RMW_RET_ERROR;
#endif
  if(!scope||!context_known(c))return RMW_RET_INVALID_ARGUMENT;
  scope->active=0;scope->context=NULL;
  if(c->leased)return uwvm_rmw_error(c,17,"RMW context is leased by dispatch/setup/cleanup (raw17)");
  if(uwvm_rmw_shutdown_requested(c)&&!allow_shutdown)return uwvm_rmw_error(c,125,"RMW context is shut down (raw125)");
  c->leased=1;scope->context=c;scope->active=1;return RMW_RET_OK;
}
void uwvm_rmw_scope_leave(uwvm_rmw_scope * scope) {
  if(scope&&scope->active&&context_known(scope->context)) {
#if UWVM_RMW_ENABLE_THREADED_PROFILE
    uwvm_rmw_deferred_drain(scope->context);
#endif
    scope->context->leased=0;scope->active=0;scope->context=NULL;
  }
}
uwvm_rmw_resource * uwvm_rmw_resource_reserve(uwvm_rmw_scope * s,uwvm_rmw_node * node,
  uwvm_rmw_endpoint_selection const * selection,void * object,uwvm_rmw_release_function release) {
  if(!valid_scope(s)||!node_known(s->context,node)||node->context!=s->context||!node->live||!selection||!object||!release||
    uwvm_rmw_shutdown_requested(s->context)||s->context->resource_count==UWVM_RMW_RESOURCE_LIMIT)return NULL;
  uwvm_rmw_context * c=s->context;
  /* Caller obtained this exact row via select; reject foreign/unregistered row
   * addresses before dereferencing it as an authoritative resource binding. */
  size_t index=0;while(index<c->profile->endpoint_count&&c->profile->endpoints+index!=selection)++index;
  if(index==c->profile->endpoint_count||selection->node_index!=node->selection_index)return NULL;
  uwvm_rmw_resource * r=c->resources+c->resource_count++;
  *r=(uwvm_rmw_resource){0};r->context=c;r->node=node;r->selection=selection;
  r->kind=selection->kind;r->object=object;r->release=release;r->live=1;++node->resource_count;return r;
}
uint32_t uwvm_rmw_owner_open(uwvm_rmw_scope * s,uwvm_rmw_resource * r,size_t alias_index,roswasmc_capability ** out) {
  if(!valid_scope(s)||!resource_known(s->context,r)||!r->live||r->context!=s->context||!out||uwvm_rmw_shutdown_requested(s->context)||
    alias_index!=r->owner_count||alias_index>=r->selection->alias_count||r->owner_count==2)return 22;
  uwvm_rmw_context * c=s->context;size_t index=c->session.count;
  if(index==UWVM_RMW_MAX_OWNERS)return 90;
  char const * alias=r->selection->aliases[alias_index];size_t length=strlen(alias);
  /* All bookkeeping is fixed storage. The successful owner is retained by the
   * Session first and associated with the group immediately, with no allocation
   * or callback boundary between these ordinary C assignments. Terminal traps
   * or unknown native admission still require Host instance draining. */
  roswasmc_capability * owner=NULL;uint32_t code=roswasmc_session_open(&c->session,alias,length,&owner);
  if(code)return code;r->owner_indices[r->owner_count++]=index;
  c->diagnostics.owner_acquisitions=c->session.count;*out=owner;return 0;
}
uint32_t uwvm_rmw_owner_open_availability(uwvm_rmw_scope * s,uwvm_rmw_resource * r,roswasmc_capability ** out) {
  if(!valid_scope(s)||!resource_known(s->context,r)||!r->live||r->context!=s->context||!out||
    uwvm_rmw_shutdown_requested(s->context)||r->kind!=UWVM_RMW_CLIENT||r->selection->kind!=UWVM_RMW_CLIENT||
    r->selection->alias_count!=2||!r->selection->availability_alias||r->owner_count!=2)return 22;
  uwvm_rmw_context * c=s->context;
  if(c->session.owners!=c->owners||c->session.capacity!=UWVM_RMW_MAX_OWNERS||c->session.count>c->session.capacity||
    r->owner_indices[0]>=c->session.count||r->owner_indices[1]>=c->session.count||
    r->owner_indices[0]==r->owner_indices[1])return 22;
  roswasmc_capability const * source=c->owners+r->owner_indices[0];
  roswasmc_capability const * auxiliary=c->owners+r->owner_indices[1];
  if(!roswasmc_is_open(source)||!roswasmc_is_open(auxiliary)||
    source->context!=&c->session.context||auxiliary->context!=&c->session.context||
    source->handle==auxiliary->handle)return 22;
  size_t index=c->session.count;if(index>=UWVM_RMW_MAX_OWNERS)return 90;
  char const * alias=r->selection->availability_alias;
  roswasmc_capability * owner=NULL;uint32_t code=roswasmc_session_open(&c->session,alias,strlen(alias),&owner);
  if(code)return code;
  /* No allocation, callback or fallible operation between successful Session
   * retention and the third resource association. Native activation is later. */
  r->owner_indices[r->owner_count++]=index;
  c->diagnostics.owner_acquisitions=c->session.count;
  if(!roswasmc_is_open(owner)||owner->context!=&c->session.context||owner->handle==source->handle||owner->handle==auxiliary->handle)return 22;
  *out=owner;return 0;
}
uint32_t uwvm_rmw_context_child_retain(uwvm_rmw_context * c) {
  if(!context_known(c)||!c->leased||uwvm_rmw_shutdown_requested(c))return 22;
  if(c->provider_children==SIZE_MAX)return 90;
  ++c->provider_children;return 0;
}
uint32_t uwvm_rmw_context_child_release(uwvm_rmw_context * c) {
  if(!context_known(c)||!c->leased||!c->provider_children)return 22;
  --c->provider_children;return 0;
}
static uint32_t native_retire(uwvm_rmw_context * c,uwvm_rmw_resource * r) {
  if(
#if !UWVM_RMW_ENABLE_THREADED_PROFILE
    !r->live||
#endif
    !r->native_retire||r->native_retire_attempted)return 0;
  size_t index=(size_t)(r-c->resources);
  r->native_retire_attempted=1;c->diagnostics.native_retire_attempted[index]=1;
  ++c->diagnostics.native_retire_attempts;
  uint32_t code=r->native_retire(r->object);
  r->native_retire_code=code;c->diagnostics.native_retire_codes[index]=code;
  if(code)++c->diagnostics.native_retire_failures;
  return code;
}
uint32_t uwvm_rmw_resource_retire(uwvm_rmw_scope * s,uwvm_rmw_resource * r) {
  if(!valid_scope(s)||!resource_known(s->context,r)||r->context!=s->context)return 22;
  if(!r->live)return 0;uwvm_rmw_context * c=s->context;
#if UWVM_RMW_ENABLE_THREADED_PROFILE
  uwvm_rmw_registry_lock();r->live=0;uwvm_rmw_registry_unlock();
  (void)uwvm_rmw_context_wake(c);
  uint32_t first=native_retire(c,r);
#else
  uint32_t first=native_retire(c,r);r->live=0;
#endif
  for(size_t n=r->owner_count;n;--n) {
    size_t index=r->owner_indices[n-1];roswasmc_capability * owner=c->owners+index;
    if(!owner->handle)continue;
    uint32_t code=roswasmc_close(owner);c->diagnostics.owner_close_codes[index]=code;
    c->diagnostics.owner_close_attempted[index]=1;++c->diagnostics.owner_close_attempts;
    if(code){++c->diagnostics.owner_close_failures;if(!first)first=code;}
  }
#if UWVM_RMW_ENABLE_THREADED_PROFILE
  if(r->node->resource_count)--r->node->resource_count;
  uwvm_rmw_registry_lock();int can_release=!r->wait_pins;uwvm_rmw_registry_unlock();
  if(can_release){release_local(c,r);if(r->local_release_code&&!first)first=r->local_release_code;}
#else
  uint32_t local=r->release(r->object);r->local_release_code=local;
  c->diagnostics.local_release_codes[(size_t)(r-c->resources)]=local;
  if(local){++c->diagnostics.local_retirement_failures;if(!first)first=local;}
  r->object=NULL;r->release=NULL;if(r->node->resource_count)--r->node->resource_count;
#endif
  return first;
}
rmw_ret_t rmw_init(rmw_init_options_t const * options,rmw_context_t * context) {
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
  rmw_init_options_impl_t * impl=options_find(options);
  if(!impl||!context||context->impl||context->implementation_identifier||!options->instance_id||!options->enclave) {
    RMW_SET_ERROR_MSG("invalid/nonzero RMW initialization context/options");return RMW_RET_INVALID_ARGUMENT;
  }
  if(options->domain_id!=impl->profile->domain_id||
    options->discovery_options.automatic_discovery_range!=RMW_AUTOMATIC_DISCOVERY_RANGE_OFF||
    options->discovery_options.static_peers_count||options->security_options.security_root_path||
    options->security_options.enforce_security!=RMW_SECURITY_ENFORCEMENT_PERMISSIVE) {
    RMW_SET_ERROR_MSG("unimplemented discovery/security/domain option; no implicit native access");return RMW_RET_UNSUPPORTED;
  }
  size_t index=0;while(index<UWVM_RMW_MAX_CONTEXTS&&context_registry[index])++index;
  if(index==UWVM_RMW_MAX_CONTEXTS||!next_process_instance) {
    RMW_SET_ERROR_MSG("bounded RMW context/local-instance capacity exhausted");return RMW_RET_BAD_ALLOC;
  }
  rcutils_allocator_t a=impl->allocator;
  uwvm_rmw_context * c=a.zero_allocate(1,sizeof(*c),a.state);
  if(!c){RMW_SET_ERROR_MSG("allocating complete fixed RMW ownership arena failed");return RMW_RET_BAD_ALLOC;}
  rmw_context_t next=rmw_get_zero_initialized_context();
  rmw_ret_t code=options_copy_private(options,&next.options);
  if(code!=RMW_RET_OK){a.deallocate(c,a.state);return code;}
  c->allocator=a;c->profile=impl->profile;c->public_handle=context;c->next_local_gid=1;
  uint32_t raw=roswasmc_session_init(&c->session,c->profile->backend,c->owners,UWVM_RMW_MAX_OWNERS);
  if(!raw)raw=roswasmc_session_check_abi(&c->session);
  if(raw) {
    (void)options_fini_private(&next.options);a.deallocate(c,a.state);
    return uwvm_rmw_error(NULL,raw,"actual ROS2 ABI refused RMW context initialization");
  }
  c->local_instance=next_process_instance++;
  next.instance_id=options->instance_id;next.implementation_identifier=uwvm_rmw_identifier;
  next.actual_domain_id=c->profile->domain_id;next.impl=c;
#if UWVM_RMW_ENABLE_THREADED_PROFILE
  /* All allocator/provider work is complete before short publication. */
  next.options.impl->public_address=&context->options;
  uwvm_rmw_registry_lock();*context=next;context_registry[index]=c;uwvm_rmw_registry_unlock();
#else
  context_registry[index]=c;
#endif
  ++uwvm_rmw_live_contexts;
#if !UWVM_RMW_ENABLE_THREADED_PROFILE
  *context=next;
#endif
  return RMW_RET_OK;
}
#if UWVM_RMW_ENABLE_THREADED_PROFILE
rmw_ret_t rmw_shutdown(rmw_context_t*context){
 /* A callback reentering on its own dispatch thread must not mutate phase one
  * behind the owner's back. Another native thread can always publish/wake. */
 if(uwvm_rmw_dispatch_active()){uwvm_rmw_dispatch_guard refused={0};(void)uwvm_rmw_dispatch_enter(&refused);return RMW_RET_ERROR;}
 uwvm_rmw_context*c=uwvm_rmw_context_pin(context);
 if(!c)return RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_registry_lock();__atomic_store_n(&c->shutdown,1,__ATOMIC_RELEASE);
 uwvm_rmw_c_shutdown_guards(c);uwvm_rmw_registry_unlock();
 (void)uwvm_rmw_context_wake(c); /* before any wait for business/Host IO */
 uwvm_rmw_dispatch_guard entry={0};uint32_t admitted=uwvm_rmw_dispatch_enter(&entry);
 if(admitted){uwvm_rmw_context_unpin(c);return RMW_RET_ERROR;}
 rmw_ret_t result=RMW_RET_OK;uwvm_rmw_scope scope={0};uint32_t first=0;
 if(!c->shutdown_cleanup_complete){
  result=uwvm_rmw_scope_enter(c,&scope,1);
  if(result==RMW_RET_OK){
   for(size_t n=c->resource_count;n;--n){uint32_t e=native_retire(c,c->resources+n-1);if(e&&!first)first=e;}
   for(size_t n=c->session.count;n;--n){size_t i=n-1;if(!c->owners[i].handle)continue;
    uint32_t e=roswasmc_close(c->owners+i);c->diagnostics.owner_close_codes[i]=e;c->diagnostics.owner_close_attempted[i]=1;
    ++c->diagnostics.owner_close_attempts;if(e){++c->diagnostics.owner_close_failures;if(!first)first=e;}}
   c->shutdown_cleanup_complete=1;uwvm_rmw_scope_leave(&scope);
   if(first){RMW_SET_ERROR_MSG("RMW shutdown retained independent once-only cleanup failures");result=RMW_RET_ERROR;}
  }
 }
 uwvm_rmw_context_unpin(c);uwvm_rmw_dispatch_leave(&entry);return result;
}
#else
rmw_ret_t rmw_shutdown(rmw_context_t * context) {
  uwvm_rmw_context * c=uwvm_rmw_context_find(context,1);
  if(!c){RMW_SET_ERROR_MSG("foreign/uninitialized RMW context");return RMW_RET_INVALID_ARGUMENT;}
  if(c->leased)return uwvm_rmw_error(c,17,"RMW shutdown during a borrowed scope (raw17)");
  if(uwvm_rmw_shutdown_requested(c))return RMW_RET_OK;
  uwvm_rmw_scope scope;rmw_ret_t code=uwvm_rmw_scope_enter(c,&scope,0);if(code!=RMW_RET_OK)return code;
  c->shutdown=1;uint32_t first=0;
  /* Shutdown invalidates native operations and attempts every retained native
   * owner once, but MUST keep public RMW entity objects allocated. Real rclcpp
   * commonly destroys its original publishers/nodes after rcl_shutdown(). Their
   * later rmw_destroy_* calls still retire the local objects without a second
   * raw close. Context_fini requires those actual objects/nodes to be destroyed.
   * No upper-layer Future completion or native cancellation is fabricated. */
  /* Native incarnation retirement and raw capability retirement are separate,
   * preallocated once-only obligations. Failed native retire never skips raw
   * source close, other endpoints, or later local-object destruction. */
  for(size_t n=c->resource_count;n;--n) {
    uint32_t e=native_retire(c,c->resources+n-1);if(e&&!first)first=e;
  }
  for(size_t n=c->session.count;n;--n) {
    size_t i=n-1;if(!c->owners[i].handle)continue;uint32_t e=roswasmc_close(c->owners+i);
    c->diagnostics.owner_close_codes[i]=e;c->diagnostics.owner_close_attempted[i]=1;
    ++c->diagnostics.owner_close_attempts;if(e){++c->diagnostics.owner_close_failures;if(!first)first=e;}
  }
  uwvm_rmw_scope_leave(&scope);
  if(first){RMW_SET_ERROR_MSG("RMW shutdown retained cleanup failures; inspect every diagnostic slot");return RMW_RET_ERROR;}
  return RMW_RET_OK;
}
#endif
rmw_ret_t rmw_context_fini(rmw_context_t * context) {
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
  uwvm_rmw_context * c=uwvm_rmw_context_find(context,1);
#if UWVM_RMW_ENABLE_THREADED_PROFILE
  if(c&&!c->leased){uwvm_rmw_scope cleanup;rmw_ret_t admitted=uwvm_rmw_scope_enter(c,&cleanup,1);
   if(admitted==RMW_RET_OK)uwvm_rmw_scope_leave(&cleanup);}
#endif
  if(!c||!uwvm_rmw_shutdown_requested(c)||c->live_nodes||c->leased||c->provider_children) {
    RMW_SET_ERROR_MSG("RMW context_fini requires shutdown and all nodes/guard/wait objects retired");return RMW_RET_INVALID_ARGUMENT;
  }
#if UWVM_RMW_ENABLE_THREADED_PROFILE
  uwvm_rmw_registry_lock();int borrowed=c->asynchronous_pins||c->active_waits||!c->shutdown_cleanup_complete;
  for(size_t i=0;i<c->resource_count;++i)borrowed|=c->resources[i].wait_pins||c->resources[i].live||c->resources[i].release!=NULL;
  if(borrowed){uwvm_rmw_registry_unlock();return uwvm_rmw_error(c,17,"Context still has real borrowers/deferred local owners");}
  for(size_t i=0;i<UWVM_RMW_MAX_CONTEXTS;++i)if(context_registry[i]==c){context_registry[i]=NULL;break;}
  uwvm_rmw_registry_unlock();
#endif
  rcutils_allocator_t a=c->allocator;rmw_ret_t code=options_fini_private(&context->options);
#if !UWVM_RMW_ENABLE_THREADED_PROFILE
  for(size_t i=0;i<UWVM_RMW_MAX_CONTEXTS;++i)if(context_registry[i]==c){context_registry[i]=NULL;break;}
#endif
  a.deallocate(c,a.state);--uwvm_rmw_live_contexts;*context=rmw_get_zero_initialized_context();return code;
}
rmw_ret_t uwvm_rmw_get_diagnostics(rmw_context_t const * handle,uwvm_rmw_diagnostics * output) {
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
  uwvm_rmw_context * c=uwvm_rmw_context_find(handle,1);
  if(!c||!output)return RMW_RET_INVALID_ARGUMENT;*output=c->diagnostics;return RMW_RET_OK;
}
